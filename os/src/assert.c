#include <timeros/assert.h>
#include <timeros/types.h>

// 强制阻塞
static void spin(char *name)
{
    printf("spinning in %s ...\n", name);
    while (true)
        ;
}


//提示报错信息
void assertion_failure(char *exp, char *file, char *base, int line)
{
    printf(
        "\n--> assert(%s) failed!!!\n"
        "--> file: %s \n"
        "--> base: %s \n"
        "--> line: %d \n",
        exp, file, base, line);

    spin("assertion_failure()");
}
