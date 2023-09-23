#include <timeros/types.h>
#include <timeros/syscall.h>
#include <timeros/string.h>
int main()
{

    const char *message = "task write is running!\n";
    while (1)
    {
       printf(message);
    }
    return 0;
}
