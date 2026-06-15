#include <iostream>
#include "opcodes.h"

size_t encode_bytecode(WORD* in, size_t size, WORD* out)
{
    size_t ip = 0;
    size_t out_pos = 0;

    printf("=== ENCODE BYTECODE ===\n");

    while (ip < size)
    {
        WORD opcode = in[ip];

        WORD key = 0xB00B ^ ip;

        WORD enc = opcode ^ key;

        printf("[ip=%zu] opcode=%d key=0x%04X enc=0x%04X\n",
               ip, opcode, key, enc);

        out[out_pos++] = enc;

        ip++;

        switch (opcode)
        {

        // --- 1 operand ---
        case CONST_:
        case PUSH:
        case POP:
        case JMP:
        case CALL:
        case PRINT:
        case SCAN:

            out[out_pos++] = in[ip++];
            break;


        // --- conditional jumps ---
        case CMPJE:
        case CMPJNE:
        case CMPJG:
        case CMPJGE:
        case CMPJL:
        case CMPJLE:

            out[out_pos++] = in[ip++];
            break;


        // --- MOV ---
        case MOV:
        {
            WORD dest_type = in[ip++];
            WORD src_type  = in[ip++];

            out[out_pos++] = dest_type;
            out[out_pos++] = src_type;

            WORD src = in[ip++];
            out[out_pos++] = src;

            if (src_type == 2)
            {
                WORD imm = in[ip++];
                out[out_pos++] = imm;
            }

            WORD dest = in[ip++];
            out[out_pos++] = dest;

            break;
        }

        default:
            break;
        }
    }

    return out_pos;
}

int main(){

    const WORD INPUT_SEGMENT = 0x4000;
    const WORD OUTPUT_SEGMENT = 0x4500;
    const WORD STR_SEGMENT = 0x6000;
    const WORD VAR_SEGMENT = 0x5000;
    

    WORD code_dec[] = {
        // "enter key: "
        CONST_, STR_SEGMENT,        // 0-1
        PRINT, 1,                   // 2-3
        // user inputs key
        
        SCAN, 0,
        CONST_, VAR_SEGMENT+0,        // 4-5
        STOREIND,
        // Инициализация: i = 0
        CONST_, 0x0000,                    // 0-1
        POP, VAR_SEGMENT + 3,              // 2-3
        
        // Начало цикла
        PUSH, VAR_SEGMENT + 3,             // 4-5
        PUSH, VAR_SEGMENT + 4,             // 6-7
        CMPJGE, 159,                       // 8-9 (HALT)
        
        // Чтение байта
        CONST_, INPUT_SEGMENT,             // 10-11
        PUSH, VAR_SEGMENT + 3,             // 12-13
        ADD,                               // 14
        LOADIND,                           // 15
        
        // i % 4
        PUSH, VAR_SEGMENT + 3,             // 16-17
        CONST_, 0x0004,                    // 18-19
        MOD,                               // 20
        POP, VAR_SEGMENT + 5,              // 21-22
        
        // Проверка остатков
        PUSH, VAR_SEGMENT + 5,             // 23-24
        CONST_, 0x0000,                    // 25-26
        CMPJE, 56,                         // 27-28
        
        PUSH, VAR_SEGMENT + 5,             // 29-30
        CONST_, 0x0001,                    // 31-32
        CMPJE, 70,                         // 33-34
        
        PUSH, VAR_SEGMENT + 5,             // 35-36
        CONST_, 0x0002,                    // 37-38
        CMPJE, 96,                         // 39-40
        
        PUSH, VAR_SEGMENT + 5,             // 41-42
        CONST_, 0x0003,                    // 43-44
        CMPJE, 119,                        // 45-46
        

        // case0
        PUSH, VAR_SEGMENT + 0,             // 47-48
        CONST_, 0x00FF,                    // 49-50
        AND,                               // 51
        PUSH, VAR_SEGMENT + 3,             // 52-53
        ADD,                               // 54
        XOR,                               // 55
        CONST_, 0x00FF,                    // 56-57
        AND,                               // 58
        JMP, 144,                          // 59-60
        

        // case1
        PUSH, VAR_SEGMENT + 0,             // 61-62
        CONST_, 0x0008,                    // 63-64
        SHR,                               // 65
        CONST_, 0x00FF,                    // 66-67
        AND,                               // 68
        
        MUL,                               // 69
        
        PUSH, VAR_SEGMENT + 3,             // 70-71
        PUSH, VAR_SEGMENT + 0,             // 72-73
        CONST_, 0x0008,                    // 74-75
        SHR,                               // 76
        CONST_, 0x00FF,                    // 77-78
        AND,                               // 79
        ADD,                               // 80
        
        XOR,                               // 81
        
        CONST_, 0x00FF,                    // 82-83
        AND,                               // 84
        
        JMP, 144,                          // 85-86
        

        // case2
        DUP,                               // 87
        CONST_, 0x0002,                    // 88-89
        SHL,                               // 90
        SWAP,                              // 91
        CONST_, 0x0006,                    // 92-93
        SHR,                               // 94
        OR,                                // 95
        
        PUSH, VAR_SEGMENT + 0,             // 96-97
        CONST_, 0x0010,                    // 98-99
        SHR,                               // 100
        CONST_, 0x00FF,                    // 101-102
        AND,                               // 103
        
        XOR,                               // 104
        
        CONST_, 0x00FF,                    // 105-106
        AND,                               // 107
        
        JMP, 144,                          // 108-109
        

        // case3
        PUSH, VAR_SEGMENT + 0,             // 110-111
        CONST_, 0x0018,                    // 112-113
        SHR,                               // 114
        CONST_, 0x00FF,                    // 115-116
        AND,                               // 117
        
        ADD,                               // 118
        
        CONST_, 0x00FF,                    // 119-120
        AND,                               // 121
        
        CONST_, OUTPUT_SEGMENT,            // 122-123
        PUSH, VAR_SEGMENT + 3,             // 124-125
        CONST_, 0x0001,                    // 126-127
        SUB,                               // 128
        ADD,                               // 129
        LOADIND,                           // 130
        
        ADD,                               // 131
        
        CONST_, 0x00FF,                    // 132-133
        AND,                               // 134
        

        // save
        CONST_, OUTPUT_SEGMENT,            // 135-136
        PUSH, VAR_SEGMENT + 3,             // 137-138
        ADD,                               // 139
        STOREIND,                          // 140
        

        // i++
        PUSH, VAR_SEGMENT + 3,             // 141-142
        CONST_, 0x0001,                    // 143-144
        ADD,                               // 145
        POP, VAR_SEGMENT + 3,              // 146-147
        

        // loop
        JMP, 13,                            // 148-149
        
                // Проверка: output[0] == 'v'
        CONST_, OUTPUT_SEGMENT,          // 159-160
        LOADIND,                         // 161
        CONST_, 0x0076,                  // 162-163  'v'
        CMPJNE, 201,                     // 164-165  если не равно — HALT

        // Проверка: output[1] == 'k'
        CONST_, OUTPUT_SEGMENT,          // 166-167
        CONST_, 0x0001,                  // 168-169
        ADD,                             // 170
        LOADIND,                         // 171
        CONST_, 0x006B,                  // 172-173  'k'
        CMPJNE, 201,                     // 174-175

        // Проверка: output[2] == 'a'
        CONST_, OUTPUT_SEGMENT,          // 176-177
        CONST_, 0x0002,                  // 178-179
        ADD,                             // 180
        LOADIND,                         // 181
        CONST_, 0x0061,                  // 182-183  'a'
        CMPJNE, 201,                     // 184-185

        // Проверка: output[3] == '{'
        CONST_, OUTPUT_SEGMENT,          // 186-187
        CONST_, 0x0003,                  // 188-189
        ADD,                             // 190
        LOADIND,                         // 191
        CONST_, 0x007B,                  // 192-193  '{'
        CMPJNE, 201,                     // 194-195

        // Если начало строки совпало с "vka{" — печатаем
        CONST_, OUTPUT_SEGMENT,          // 196-197
        PRINT, 1,                        // 198-199
        HALT,                            // 200

        // Если не совпало — ничего не печатаем
        HALT                             // 201                               // 150
    };
    
    WORD code_enc[sizeof(code_dec)] = {};
    encode_bytecode(code_dec, sizeof(code_dec), code_enc);
    std::cout << "WORD code_dec[] = { ";
    for (int i=0; i < sizeof(code_enc); i++){
        std::cout << code_enc[i];
        if ((i+1)!=sizeof(code_enc)) std::cout << ", ";
        else std::cout << " };\n";
    }

    return 0;
}