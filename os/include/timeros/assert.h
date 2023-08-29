#ifndef TOS_ASSERT_H
#define TOS_ASSERT_H

void assertion_failure(char *exp, char *file, char *base, int line);


/*
__FILE__是一个预定义的宏，在C语言中表示当前源文件的文件名。
在预处理阶段，编译器会将所有的__FILE__宏替换为当前源文件的文件名字符串。
__BASE_FILE__是一个预定义的宏，在某些编译器中用于表示当前编译单元的顶层源文件的文件名，
即当前源文件所属的工程或者库的主文件名。
__LINE__是一个预定义的宏，在C语言中表示当前代码所在的行号。
在预处理阶段，编译器会将所有的__LINE__宏替换为当前代码所在的行号。
*/


#define assert(exp) \
    if (exp)        \
        ;           \
    else            \
        assertion_failure(#exp, __FILE__, __BASE_FILE__, __LINE__)

#endif