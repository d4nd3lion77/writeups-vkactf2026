#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/string.h>

#define DEVICE_NAME "scoreboard"

#define IOC_MAGIC 0x77

#define CREATE_TEAM        _IOW(IOC_MAGIC, 0, struct request)
#define DELETE_TEAM        _IOW(IOC_MAGIC, 1, struct request)
#define EDIT_TEAM          _IOW(IOC_MAGIC, 2, struct request)
#define SHOW_TEAM          _IOW(IOC_MAGIC, 3, struct request)

#define CREATE_ATHLETE     _IOW(IOC_MAGIC, 4, struct athlete_request)
#define DELETE_ATHLETE     _IOW(IOC_MAGIC, 5, struct athlete_request)
#define EDIT_ATHLETE       _IOW(IOC_MAGIC, 6, struct athlete_request)
#define SHOW_ATHLETE       _IOW(IOC_MAGIC, 7, struct athlete_request)

#define ADD_TO_TEAM        _IOW(IOC_MAGIC, 8, struct athlete_request)
#define REMOVE_FROM_TEAM   _IOW(IOC_MAGIC, 9, struct athlete_request)
#define SHOW_TEAM_ROSTER   _IOW(IOC_MAGIC, 10, struct request)
#define EDIT_TEAM_MEMBER   _IOW(IOC_MAGIC, 11, struct athlete_request)

#define CREATE_EVENT       _IOW(IOC_MAGIC, 12, struct event_request)
#define DELETE_EVENT       _IOW(IOC_MAGIC, 13, struct event_request)
#define EDIT_EVENT         _IOW(IOC_MAGIC, 14, struct event_request)
#define SHOW_EVENT         _IOW(IOC_MAGIC, 15, struct event_request)
#define TRIGGER_EVENT      _IOW(IOC_MAGIC, 16, struct event_request)

#define MAX_TEAMS      8
#define MAX_ATHLETES   16
#define MAX_EVENTS     16
#define TEAM_MEMBERS   4

struct athlete;

struct athlete {
    char name[0x20];
    char sport[0x20];
    uint64_t age;
    char country[0x8];
};

struct event {
    char title[0x20];
    char location[0x20];
    uint64_t secret;
    void (*callback)(uint64_t);
};

struct team {
    char name[0x40];
    uint64_t medals;
    struct athlete *members[TEAM_MEMBERS];
};

struct request {
    int idx;
    char name[0x40];
    uint64_t score;
};

struct athlete_request {
    int idx;
    int team_idx;
    int member_idx;

    char name[0x20];
    char sport[0x20];
    uint64_t age;
    char country[0x8];
};

struct event_request {
    int idx;

    char title[0x20];
    char location[0x20];
    uint64_t secret;
    uint64_t callback;
};

static struct team *teams[MAX_TEAMS];
static struct athlete *athletes[MAX_ATHLETES];
static struct event *events[MAX_EVENTS];

static void safe_event_callback(uint64_t arg)
{
    pr_info("[EVENT] safe callback called with arg = 0x%llx\n", arg);
}

static void debug_event_callback(uint64_t arg)
{
    pr_info("[EVENT] debug callback called with arg = 0x%llx\n", arg);
}

static void show_team(struct team *t)
{
    int i;

    pr_info("[TEAM] %s | medals = %llu\n", t->name, t->medals);

    for (i = 0; i < TEAM_MEMBERS; i++) {
        if (t->members[i])
            pr_info("  member[%d] = %px\n", i, t->members[i]);
        else
            pr_info("  member[%d] = NULL\n", i);
    }
}

static void show_athlete(struct athlete *a)
{
    pr_info("[ATHLETE] name = %.32s | sport = %.32s | age = %llu | country = %.8s\n",
            a->name,
            a->sport,
            a->age,
            a->country);
}

static void show_event(struct event *e)
{
    pr_info("[EVENT] title = %.32s | location = %.32s | secret = 0x%llx | callback = %px\n",
            e->title,
            e->location,
            e->secret,
            e->callback);
}

static int valid_team_idx(int idx)
{
    return idx >= 0 && idx < MAX_TEAMS;
}

static int valid_athlete_idx(int idx)
{
    return idx >= 0 && idx < MAX_ATHLETES;
}

static int valid_event_idx(int idx)
{
    return idx >= 0 && idx < MAX_EVENTS;
}

static int valid_member_idx(int idx)
{
    return idx >= 0 && idx < TEAM_MEMBERS;
}

static long scoreboard_ioctl(struct file *file,
                             unsigned int cmd,
                             unsigned long arg)
{
    struct request req;
    struct athlete_request areq;
    struct event_request ereq;
    int i;

    switch (cmd) {
    case CREATE_TEAM:
        if (copy_from_user(&req, (void __user *)arg, sizeof(req)))
            return -EFAULT;

        if (!valid_team_idx(req.idx))
            return -EINVAL;

        if (teams[req.idx])
            return -EINVAL;

        teams[req.idx] = kzalloc(sizeof(struct team), GFP_KERNEL);
        if (!teams[req.idx])
            return -ENOMEM;

        memcpy(teams[req.idx]->name, req.name, sizeof(req.name));
        teams[req.idx]->medals = req.score;

        pr_info("[+] created team %d (%px)\n", req.idx, teams[req.idx]);
        break;

    case DELETE_TEAM:
        if (copy_from_user(&req, (void __user *)arg, sizeof(req)))
            return -EFAULT;

        if (!valid_team_idx(req.idx))
            return -EINVAL;

        if (!teams[req.idx])
            return -EINVAL;

        pr_info("[-] freeing team %d (%px)\n", req.idx, teams[req.idx]);

        kfree(teams[req.idx]);
        teams[req.idx] = NULL;
        break;

    case EDIT_TEAM:
        if (copy_from_user(&req, (void __user *)arg, sizeof(req)))
            return -EFAULT;

        if (!valid_team_idx(req.idx))
            return -EINVAL;

        if (!teams[req.idx])
            return -EINVAL;

        memcpy(teams[req.idx]->name, req.name, sizeof(req.name));
        teams[req.idx]->medals = req.score;

        pr_info("[+] edited team %d\n", req.idx);
        break;

    case SHOW_TEAM:
        if (copy_from_user(&req, (void __user *)arg, sizeof(req)))
            return -EFAULT;

        if (!valid_team_idx(req.idx))
            return -EINVAL;

        if (!teams[req.idx])
            return -EINVAL;

        show_team(teams[req.idx]);
        break;

    case CREATE_ATHLETE:
        if (copy_from_user(&areq, (void __user *)arg, sizeof(areq)))
            return -EFAULT;

        if (!valid_athlete_idx(areq.idx))
            return -EINVAL;

        if (athletes[areq.idx])
            return -EINVAL;

        athletes[areq.idx] = kzalloc(sizeof(struct athlete), GFP_KERNEL);
        if (!athletes[areq.idx])
            return -ENOMEM;

        memcpy(athletes[areq.idx]->name,
               areq.name,
               sizeof(athletes[areq.idx]->name));

        memcpy(athletes[areq.idx]->sport,
               areq.sport,
               sizeof(athletes[areq.idx]->sport));

        memcpy(athletes[areq.idx]->country,
               areq.country,
               sizeof(athletes[areq.idx]->country));

        athletes[areq.idx]->age = areq.age;

        pr_info("[+] created athlete %d (%px)\n",
                areq.idx,
                athletes[areq.idx]);
        break;

    case DELETE_ATHLETE:
        if (copy_from_user(&areq, (void __user *)arg, sizeof(areq)))
            return -EFAULT;

        if (!valid_athlete_idx(areq.idx))
            return -EINVAL;

        if (!athletes[areq.idx])
            return -EINVAL;

        pr_info("[-] freeing athlete %d (%px)\n",
                areq.idx,
                athletes[areq.idx]);

        kfree(athletes[areq.idx]);
        athletes[areq.idx] = NULL;
        break;

    case EDIT_ATHLETE:
        if (copy_from_user(&areq, (void __user *)arg, sizeof(areq)))
            return -EFAULT;

        if (!valid_athlete_idx(areq.idx))
            return -EINVAL;

        if (!athletes[areq.idx])
            return -EINVAL;

        memcpy(athletes[areq.idx]->name,
               areq.name,
               sizeof(athletes[areq.idx]->name));

        memcpy(athletes[areq.idx]->sport,
               areq.sport,
               sizeof(athletes[areq.idx]->sport));

        memcpy(athletes[areq.idx]->country,
               areq.country,
               sizeof(athletes[areq.idx]->country));

        athletes[areq.idx]->age = areq.age;

        pr_info("[+] edited athlete %d\n", areq.idx);
        break;

    case SHOW_ATHLETE:
        if (copy_from_user(&areq, (void __user *)arg, sizeof(areq)))
            return -EFAULT;

        if (!valid_athlete_idx(areq.idx))
            return -EINVAL;

        if (!athletes[areq.idx])
            return -EINVAL;

        show_athlete(athletes[areq.idx]);
        break;

    case ADD_TO_TEAM:
        if (copy_from_user(&areq, (void __user *)arg, sizeof(areq)))
            return -EFAULT;

        if (!valid_athlete_idx(areq.idx))
            return -EINVAL;

        if (!valid_team_idx(areq.team_idx))
            return -EINVAL;

        if (!valid_member_idx(areq.member_idx))
            return -EINVAL;

        if (!athletes[areq.idx])
            return -EINVAL;

        if (!teams[areq.team_idx])
            return -EINVAL;

        teams[areq.team_idx]->members[areq.member_idx] = athletes[areq.idx];

        pr_info("[+] added athlete %d (%px) to team %d slot %d\n",
                areq.idx,
                athletes[areq.idx],
                areq.team_idx,
                areq.member_idx);
        break;

    case REMOVE_FROM_TEAM:
        if (copy_from_user(&areq, (void __user *)arg, sizeof(areq)))
            return -EFAULT;

        if (!valid_team_idx(areq.team_idx))
            return -EINVAL;

        if (!valid_member_idx(areq.member_idx))
            return -EINVAL;

        if (!teams[areq.team_idx])
            return -EINVAL;

        teams[areq.team_idx]->members[areq.member_idx] = NULL;

        pr_info("[-] removed member slot %d from team %d\n",
                areq.member_idx,
                areq.team_idx);
        break;

    case SHOW_TEAM_ROSTER:
        if (copy_from_user(&req, (void __user *)arg, sizeof(req)))
            return -EFAULT;

        if (!valid_team_idx(req.idx))
            return -EINVAL;

        if (!teams[req.idx])
            return -EINVAL;

        pr_info("[+] roster for team %d (%px)\n", req.idx, teams[req.idx]);

        for (i = 0; i < TEAM_MEMBERS; i++) {
            struct athlete *a = teams[req.idx]->members[i];

            if (!a)
                continue;

            pr_info("[ROSTER] slot %d ptr = %px\n", i, a);
            show_athlete(a);
        }
        break;

    case EDIT_TEAM_MEMBER:
        if (copy_from_user(&areq, (void __user *)arg, sizeof(areq)))
            return -EFAULT;

        if (!valid_team_idx(areq.team_idx))
            return -EINVAL;

        if (!valid_member_idx(areq.member_idx))
            return -EINVAL;

        if (!teams[areq.team_idx])
            return -EINVAL;

        if (!teams[areq.team_idx]->members[areq.member_idx])
            return -EINVAL;

        memcpy(teams[areq.team_idx]->members[areq.member_idx]->name,
               areq.name,
               sizeof(areq.name));

        memcpy(teams[areq.team_idx]->members[areq.member_idx]->sport,
               areq.sport,
               sizeof(areq.sport));

        memcpy(teams[areq.team_idx]->members[areq.member_idx]->country,
               areq.country,
               sizeof(areq.country));

        teams[areq.team_idx]->members[areq.member_idx]->age = areq.age;

        pr_info("[+] edited team member team=%d slot=%d\n",
                areq.team_idx,
                areq.member_idx);
        break;

    case CREATE_EVENT:
        if (copy_from_user(&ereq, (void __user *)arg, sizeof(ereq)))
            return -EFAULT;

        if (!valid_event_idx(ereq.idx))
            return -EINVAL;

        if (events[ereq.idx])
            return -EINVAL;

        events[ereq.idx] = kzalloc(sizeof(struct event), GFP_KERNEL);
        if (!events[ereq.idx])
            return -ENOMEM;

        memcpy(events[ereq.idx]->title,
               ereq.title,
               sizeof(events[ereq.idx]->title));

        memcpy(events[ereq.idx]->location,
               ereq.location,
               sizeof(events[ereq.idx]->location));

        events[ereq.idx]->secret = ereq.secret;

        if (ereq.callback == 1)
            events[ereq.idx]->callback = safe_event_callback;
        else if (ereq.callback == 2)
            events[ereq.idx]->callback = debug_event_callback;
        else
            events[ereq.idx]->callback = safe_event_callback;

        pr_info("[+] created event %d (%px)\n",
                ereq.idx,
                events[ereq.idx]);
        break;

    case DELETE_EVENT:
        if (copy_from_user(&ereq, (void __user *)arg, sizeof(ereq)))
            return -EFAULT;

        if (!valid_event_idx(ereq.idx))
            return -EINVAL;

        if (!events[ereq.idx])
            return -EINVAL;

        pr_info("[-] freeing event %d (%px)\n",
                ereq.idx,
                events[ereq.idx]);

        kfree(events[ereq.idx]);
        events[ereq.idx] = NULL;
        break;

    case EDIT_EVENT:
        if (copy_from_user(&ereq, (void __user *)arg, sizeof(ereq)))
            return -EFAULT;

        if (!valid_event_idx(ereq.idx))
            return -EINVAL;

        if (!events[ereq.idx])
            return -EINVAL;

        memcpy(events[ereq.idx]->title,
               ereq.title,
               sizeof(events[ereq.idx]->title));

        memcpy(events[ereq.idx]->location,
               ereq.location,
               sizeof(events[ereq.idx]->location));

        events[ereq.idx]->secret = ereq.secret;

        if (ereq.callback == 1)
            events[ereq.idx]->callback = safe_event_callback;
        else if (ereq.callback == 2)
            events[ereq.idx]->callback = debug_event_callback;
        else
            events[ereq.idx]->callback = safe_event_callback;

        pr_info("[+] edited event %d\n", ereq.idx);
        break;

    case SHOW_EVENT:
        if (copy_from_user(&ereq, (void __user *)arg, sizeof(ereq)))
            return -EFAULT;

        if (!valid_event_idx(ereq.idx))
            return -EINVAL;

        if (!events[ereq.idx])
            return -EINVAL;

        show_event(events[ereq.idx]);
        break;

    case TRIGGER_EVENT:
        if (copy_from_user(&ereq, (void __user *)arg, sizeof(ereq)))
            return -EFAULT;

        if (!valid_event_idx(ereq.idx))
            return -EINVAL;

        if (!events[ereq.idx])
            return -EINVAL;

        pr_info("[+] triggering event %d callback=%px arg=0x%llx\n",
                ereq.idx,
                events[ereq.idx]->callback,
                events[ereq.idx]->secret);

        events[ereq.idx]->callback(events[ereq.idx]->secret);
        break;

    default:
        return -EINVAL;
    }

    return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = scoreboard_ioctl,
};

static int major;

static int __init scoreboard_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0)
        return major;

    pr_info("[+] Olympic Scoreboard loaded\n");
    pr_info("[+] major = %d\n", major);
    pr_info("[+] athlete size = 0x%zx\n", sizeof(struct athlete));
    pr_info("[+] event size   = 0x%zx\n", sizeof(struct event));
    pr_info("[+] team size    = 0x%zx\n", sizeof(struct team));

    return 0;
}

static void __exit scoreboard_exit(void)
{
    int i;

    for (i = 0; i < MAX_TEAMS; i++) {
        kfree(teams[i]);
        teams[i] = NULL;
    }

    for (i = 0; i < MAX_ATHLETES; i++) {
        kfree(athletes[i]);
        athletes[i] = NULL;
    }

    for (i = 0; i < MAX_EVENTS; i++) {
        kfree(events[i]);
        events[i] = NULL;
    }

    unregister_chrdev(major, DEVICE_NAME);

    pr_info("[-] Olympic Scoreboard unloaded\n");
}

module_init(scoreboard_init);
module_exit(scoreboard_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ctf");
MODULE_DESCRIPTION("Olympic Scoreboard kernel challenge");