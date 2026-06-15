#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "generated_secrets.h"

#define NVRAM_SIZE 100
#define STATE_SIZE 64
#define CALIB_MAX 1024

struct board_info {
    char model[64];
    char serial[64];
    char region[16];
    uint32_t hwrev;
};

struct nvram_image {
    uint8_t magic[4];
    uint16_t version;
    uint16_t profile;
    uint32_t board_crc;
    uint32_t boot_counter;
    uint8_t seed[16];
    uint8_t state[STATE_SIZE];
    uint32_t checksum;
};

static uint32_t crc32_update(uint32_t crc, const uint8_t *buf, size_t len) {
    crc = ~crc;
    for (size_t i = 0; i < len; i++) {
        crc ^= buf[i];
        for (int j = 0; j < 8; j++) {
            uint32_t mask = -(crc & 1u);
            crc = (crc >> 1) ^ (0xedb88320u & mask);
        }
    }
    return ~crc;
}

static uint32_t crc32_buf(const uint8_t *buf, size_t len) {
    return crc32_update(0, buf, len);
}

static uint32_t rol32(uint32_t x, unsigned n) {
    n &= 31u;
    return (x << n) | (x >> ((32u - n) & 31u));
}

static uint32_t xorshift32(uint32_t x) {
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return x;
}

static uint16_t rd16(const uint8_t *p) {
    return ((uint16_t)p[0] << 8) | p[1];
}

static uint32_t rd32(const uint8_t *p) {
    return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) |
           ((uint32_t)p[2] << 8) | p[3];
}

static void wr32(uint8_t *p, uint32_t v) {
    p[0] = (uint8_t)(v >> 24);
    p[1] = (uint8_t)(v >> 16);
    p[2] = (uint8_t)(v >> 8);
    p[3] = (uint8_t)v;
}

static void trim(char *s) {
    size_t n = strlen(s);
    while (n && (s[n - 1] == '\n' || s[n - 1] == '\r' || s[n - 1] == ' ' || s[n - 1] == '\t')) {
        s[--n] = 0;
    }
}

static void copy_value(char *dst, size_t dst_sz, const char *src) {
    size_t i = 0;
    if (!dst_sz) {
        return;
    }
    while (i + 1 < dst_sz && src[i]) {
        dst[i] = src[i];
        i++;
    }
    dst[i] = 0;
}

static int make_path(char *out, size_t out_sz, const char *root, const char *rel) {
    int n = snprintf(out, out_sz, "%s/%s", root, rel);
    return n > 0 && (size_t)n < out_sz;
}

static int load_file(const char *path, uint8_t *buf, size_t cap, size_t *out_len) {
    FILE *fp = fopen(path, "rb");
    if (!fp) {
        return -1;
    }
    size_t n = fread(buf, 1, cap, fp);
    int extra = fgetc(fp);
    fclose(fp);
    if (extra != EOF) {
        return -2;
    }
    *out_len = n;
    return 0;
}

static int read_board(const char *path, struct board_info *bi) {
    FILE *fp = fopen(path, "rb");
    char line[160];
    memset(bi, 0, sizeof(*bi));
    if (!fp) {
        return -1;
    }
    while (fgets(line, sizeof(line), fp)) {
        trim(line);
        if (!strncmp(line, "model=", 6)) {
            copy_value(bi->model, sizeof(bi->model), line + 6);
        } else if (!strncmp(line, "serial=", 7)) {
            copy_value(bi->serial, sizeof(bi->serial), line + 7);
        } else if (!strncmp(line, "region=", 7)) {
            copy_value(bi->region, sizeof(bi->region), line + 7);
        } else if (!strncmp(line, "hwrev=", 6)) {
            bi->hwrev = (uint32_t)strtoul(line + 6, NULL, 10);
        }
    }
    fclose(fp);
    return bi->model[0] && bi->serial[0] && bi->region[0] && bi->hwrev ? 0 : -2;
}

static uint32_t board_crc(const struct board_info *bi) {
    uint32_t c = 0;
    c = crc32_update(c, (const uint8_t *)bi->model, strlen(bi->model));
    c = crc32_update(c, (const uint8_t *)"|", 1);
    c = crc32_update(c, (const uint8_t *)bi->serial, strlen(bi->serial));
    c = crc32_update(c, (const uint8_t *)"|", 1);
    c = crc32_update(c, (const uint8_t *)bi->region, strlen(bi->region));
    c = crc32_update(c, (const uint8_t *)"|", 1);
    uint8_t h[4];
    wr32(h, bi->hwrev);
    c = crc32_update(c, h, sizeof(h));
    return c;
}

static uint32_t seed_word(const uint8_t *seed, size_t idx) {
    return rd32(seed + idx * 4);
}

static uint32_t stream_seed(const struct board_info *bi, const uint8_t *calib,
                            size_t calib_len, const uint8_t *seed, char slot) {
    uint32_t k = crc32_buf((const uint8_t *)bi->serial, strlen(bi->serial));
    k ^= crc32_buf(calib, calib_len);
    k ^= seed_word(seed, 0);
    k ^= rol32(seed_word(seed, 2), 11);
    k = rol32(k, bi->hwrev);
    k ^= (slot == 'B') ? 0x42424242u : 0x41414141u;
    return k ? k : 0x4d495053u;
}

static void crypt_state(uint8_t *state, size_t len, uint32_t k) {
    for (size_t i = 0; i < len; i++) {
        k = xorshift32(k + 0x9e3779b9u + (uint32_t)i);
        state[i] ^= (uint8_t)(k >> ((i & 3u) * 8u));
    }
}

static void mac128(uint8_t out[16], const uint8_t *key, size_t key_len,
                   const uint8_t *data, size_t data_len, uint32_t extra) {
    uint32_t s[4] = {0x243f6a88u ^ extra, 0x85a308d3u, 0x13198a2eu, 0x03707344u};
    for (size_t i = 0; i < key_len; i++) {
        s[i & 3u] = rol32(s[i & 3u] ^ key[i] ^ (uint32_t)i, 5) + s[(i + 1u) & 3u];
    }
    for (size_t i = 0; i < data_len; i++) {
        s[i & 3u] ^= (uint32_t)data[i] + 0x9e3779b9u + (uint32_t)(i << 8);
        s[(i + 1u) & 3u] = rol32(s[(i + 1u) & 3u] + s[i & 3u], 9);
    }
    for (int r = 0; r < 24; r++) {
        s[r & 3] = rol32(s[r & 3] + s[(r + 1) & 3] + 0x7f4a7c15u, 7);
        s[(r + 2) & 3] ^= s[r & 3];
    }
    for (int i = 0; i < 4; i++) {
        wr32(out + i * 4, s[i]);
    }
}

static void flag_stream(uint8_t *out, size_t len, const uint8_t token_key[32],
                        const struct board_info *bi, uint32_t calib_crc) {
    uint8_t mac[16];
    uint8_t data[160];
    int n = snprintf((char *)data, sizeof(data), "coldboot-flag|%s|%08x", bi->serial, calib_crc);
    if (n < 0 || (size_t)n >= sizeof(data)) {
        memset(out, 0, len);
        return;
    }
    uint32_t ctr = 0;
    size_t pos = 0;
    while (pos < len) {
        wr32(data + n, ctr++);
        mac128(mac, token_key, 32, data, (size_t)n + 4, 0x636f6c64u);
        for (size_t i = 0; i < sizeof(mac) && pos < len; i++) {
            out[pos++] = mac[i];
        }
    }
}

static void derive_root(char *root, size_t root_sz, const char *argv0) {
    const char *env = getenv("COLDROOT");
    if (env && env[0]) {
        snprintf(root, root_sz, "%s", env);
        return;
    }
    snprintf(root, root_sz, ".");
    const char *sbin = strstr(argv0, "/sbin/");
    if (!sbin) {
        sbin = strstr(argv0, "\\sbin\\");
    }
    if (sbin) {
        size_t n = (size_t)(sbin - argv0);
        if (n && n < root_sz) {
            memcpy(root, argv0, n);
            root[n] = 0;
        }
    }
}

int main(int argc, char **argv) {
    (void)argc;
    char root[512], path[768];
    struct board_info bi;
    uint8_t calib[CALIB_MAX], nvbuf[NVRAM_SIZE];
    uint8_t plain[STATE_SIZE], expect_tag[16], ks[FLAG_LEN];
    size_t calib_len = 0, nv_len = 0;
    char slot = 0;

    derive_root(root, sizeof(root), argv[0]);
    printf("watchdogd: restoring board profile\n");

    if (!make_path(path, sizeof(path), root, "etc/board.conf") || read_board(path, &bi) != 0) {
        puts("watchdogd: board profile unavailable");
        return 1;
    }
    if (!make_path(path, sizeof(path), root, "etc/calib.dat") ||
        load_file(path, calib, sizeof(calib), &calib_len) != 0 || calib_len < FLAG_LEN + 64) {
        puts("watchdogd: calibration area rejected");
        return 1;
    }
    if (!make_path(path, sizeof(path), root, "var/state/boot.slot")) {
        return 1;
    }
    FILE *slot_fp = fopen(path, "rb");
    if (!slot_fp) {
        puts("watchdogd: boot slot missing");
        return 1;
    }
    slot = (char)fgetc(slot_fp);
    fclose(slot_fp);
    if (slot != 'A' && slot != 'B') {
        puts("watchdogd: boot slot out of range");
        return 1;
    }

    if (!make_path(path, sizeof(path), root, "etc/nvram.bin") ||
        load_file(path, nvbuf, sizeof(nvbuf), &nv_len) != 0 || nv_len != NVRAM_SIZE) {
        puts("watchdogd: nvram read failed");
        return 1;
    }

    struct nvram_image ni;
    memcpy(ni.magic, nvbuf, 4);
    ni.version = rd16(nvbuf + 4);
    ni.profile = rd16(nvbuf + 6);
    ni.board_crc = rd32(nvbuf + 8);
    ni.boot_counter = rd32(nvbuf + 12);
    memcpy(ni.seed, nvbuf + 16, 16);
    memcpy(ni.state, nvbuf + 32, STATE_SIZE);
    ni.checksum = rd32(nvbuf + 32 + STATE_SIZE);

    if (memcmp(ni.magic, "NVR2", 4) || ni.version != 2 || ni.profile != 7) {
        puts("watchdogd: nvram header rejected");
        return 1;
    }
    if (ni.board_crc != board_crc(&bi)) {
        puts("watchdogd: board binding mismatch");
        return 1;
    }
    if ((crc32_buf(nvbuf, NVRAM_SIZE - 4) ^ 0x4d495053u) != ni.checksum) {
        puts("watchdogd: nvram checksum mismatch");
        return 1;
    }

    memcpy(plain, ni.state, sizeof(plain));
    crypt_state(plain, sizeof(plain), stream_seed(&bi, calib, calib_len, ni.seed, slot));

    uint32_t calib_crc = crc32_buf(calib, calib_len);
    uint8_t tag_data[96];
    int tag_n = snprintf((char *)tag_data, sizeof(tag_data), "%s|%08x|%08x",
                         bi.serial, calib_crc, ni.board_crc);
    if (tag_n < 0 || (size_t)tag_n >= sizeof(tag_data)) {
        return 1;
    }
    mac128(expect_tag, plain + 8, 32, tag_data, (size_t)tag_n, 0x626f6f74u ^ seed_word(ni.seed, 1));
    if (memcmp(expect_tag, plain + 40, 16)) {
        puts("watchdogd: protected state tag mismatch");
        return 1;
    }

    if (memcmp(plain, "DIAG", 4) || rd32(plain + 4) != 0x1337u) {
        puts("watchdogd: diagnostic interface locked");
        return 0;
    }

    flag_stream(ks, FLAG_LEN, plain + 8, &bi, calib_crc);
    printf("watchdogd: diagnostic token: ");
    for (size_t i = 0; i < FLAG_LEN; i++) {
        uint8_t ch = ENC_FLAG[i] ^ MASK_A[i] ^ calib[64 + i] ^ ks[i];
        putchar((int)ch);
    }
    putchar('\n');
    return 0;
}
