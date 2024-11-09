/**
 * Linux IOC shell utilities
 */
#include <sys/resource.h>
#include <iocsh.h>
#include <string.h>
#include <epicsStdlib.h>

const static struct
{
    const char* name;
    int resource;
    const char* units;
} LIMITS[] =
{
    {"AS", RLIMIT_AS, "bytes"},
    {"CORE", RLIMIT_CORE, "bytes"},
    {"CPU", RLIMIT_CPU, "seconds"},
    {"DATA", RLIMIT_DATA, "bytes"},
    {"FSIZE", RLIMIT_FSIZE, "bytes"},
    {"LOCKS", RLIMIT_LOCKS, "locks"},
    {"MEMLOCK", RLIMIT_MEMLOCK, "bytes"},
    {"MSGQUEUE", RLIMIT_MSGQUEUE, "bytes"},
    {"NICE", RLIMIT_NICE, ""},
    {"NOFILE", RLIMIT_NOFILE, "files"},
    {"NPROC", RLIMIT_NPROC, "processes"},
    {"RSS", RLIMIT_RSS, "bytes"},
    {"RTPRIO", RLIMIT_RTPRIO, ""},
    {"RTTIME", RLIMIT_RTTIME, "seconds"},
    {"SIGPENDING", RLIMIT_SIGPENDING, "signals"},
    {"STACK", RLIMIT_STACK, "bytes"},
};

#define TMPBUF_LEN 64

static const char* rlimitStr(rlim_t lim, char tmpbuf[TMPBUF_LEN])
{
    if (lim == RLIM_INFINITY)
        return "unlimited";
    snprintf(tmpbuf, TMPBUF_LEN, "%ld", lim);
    return tmpbuf;
}

static void getrlimitCallFunc(const iocshArgBuf* args)
{
    const char* res = args[0].sval;
    if (!res)
        printf("%-15s %-15s %-15s %-15s\n", "Resource", "Soft", "Hard", "Units");

    for (int i = 0; i < sizeof(LIMITS)/sizeof(LIMITS[0]); ++i) {
        if (res && strcasecmp(res, LIMITS[i].name))
            continue;

        struct rlimit rl;
        if (getrlimit(LIMITS[i].resource, &rl) < 0) {
            perror("Unable to get rlimit");
            continue;
        }

        char tmpbuf[TMPBUF_LEN];
        if (!res)
            printf("%-15s %-15s %-15s %-15s\n", LIMITS[i].name, rlimitStr(rl.rlim_cur, tmpbuf), rlimitStr(rl.rlim_max, tmpbuf), LIMITS[i].units);
        else
            printf("%s %s\n", rlimitStr(rl.rlim_cur, tmpbuf), rlimitStr(rl.rlim_max, tmpbuf));
    }
}

static rlim_t parseLimit(const char* str) {
    if (!strcmp(str, "unlimited"))
        return RLIM_INFINITY;
    epicsInt64 r;
    if (epicsParseInt64(str, &r, 10, NULL) != 0)
        return 0;
    return r;
}

static void setrlimitCallFunc(const iocshArgBuf* args)
{
    const char* res = args[0].sval;
    const char* soft = args[1].sval;
    const char* hard = args[2].sval;
    
    if (!res || !soft) {
        printf("Resource name and one limit required\n");
        iocshSetError(-1);
        return;
    }

    int resource = -1;
    for (int i = 0; i < sizeof(LIMITS)/sizeof(LIMITS[0]); ++i) {
        if (strcasecmp(res, LIMITS[i].name) == 0) {
            resource = LIMITS[i].resource;
            break;
        }
    }

    if (resource < 0)  {
        printf("No such resource '%s'\n", res);
        iocshSetError(-1);
        return;
    }

    /* Query current limits in case hard limit is not provided */
    struct rlimit rl;
    if (getrlimit(resource, &rl) < 0) {
        perror("Failed to query rlimit");
        iocshSetError(-1);
        return;
    }

    rl.rlim_cur = parseLimit(soft);
    if (hard)
        rl.rlim_max = parseLimit(hard);

    if (setrlimit(resource, &rl) < 0) {
        perror("Failed to set rlimit");
        iocshSetError(-1);
        return;
    }

    char tmpbuf[TMPBUF_LEN];
    printf("%s %s\n", rlimitStr(rl.rlim_cur, tmpbuf), rlimitStr(rl.rlim_max, tmpbuf));
}

void registerPlatformIocsh()
{
    /* getrlimit */
    {
        static const iocshArg arg0 = {"resource", iocshArgString};
        static const iocshArg* args[] = {&arg0};
        static const iocshFuncDef func = {"getrlimit", 1, args};
        iocshRegister(&func, getrlimitCallFunc);
    }

    /* setrlimit */
    {
        static const iocshArg arg0 = {"resource", iocshArgString};
        static const iocshArg arg1 = {"soft", iocshArgString};
        static const iocshArg arg2 = {"hard", iocshArgString};
        static const iocshArg* args[] = {&arg0, &arg1, &arg2};
        static const iocshFuncDef func = {"setrlimit", 3, args};
        iocshRegister(&func, setrlimitCallFunc);
    }
}
