#include "types.h"
#include "stat.h"
#include "user.h"

int main(void)
{
    int max_pid = getNumProc();

    printf(1, "Number of active processes: %d\n", max_pid);
    exit();
}