#include <timeros/types.h>
#include <timeros/syscall.h>
#include <timeros/string.h>
int main()
{

    while (1)
    {
        char data = getchar();
        printf("%c",data);
    }
    return 0;
}
