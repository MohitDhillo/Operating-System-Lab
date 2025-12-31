#include "types.h"
#include "stat.h"
#include "user.h"

int main(void)
{
    int max_pid = getMaxPid();

    if (max_pid < 0)
    {
        printf(1, "No processes are active in the process table.\n");
    }
    else
    {
        printf(1, "Maximum PID: %d\n", max_pid);
    }
    exit();
}