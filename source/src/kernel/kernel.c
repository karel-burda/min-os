#include "kernel.h"

#include <stdbool.h>

#include "common.h"
#include "kernel_test.h"
#include "keyboard.h"
#include "shell.h"
#include "strings.h"
#include "terminal.h"
#include "video.h"

bool kernel_init()
{
    bool ret = true;

    // init components
    ret &= terminal_init();

    // run self-test
    ret &= kernel_self_test();

    return ret;
}

int kernel_main()
{
    // run self-test
    if (!kernel_init())
    {
        terminal_writestring(C_STR_KERNEL_TEST_FAILED);

        return -1;
    }

    // keep on...
    terminal_run();

    return 0;
}
