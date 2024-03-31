#ifndef __SBI_CONST_H__
#define __SBI_CONST_H__

/*提供一种在汇编和C代码之间共享常量定义的便捷方法*/
#ifdef __ASSEMBLER__
#define _AC(X,Y)	X
#define _AT(T,X)	X
#else
#define __AC(X,Y)	(X##Y)
#define _AC(X,Y)	__AC(X,Y)
#define _AT(T,X)	((T)(X))
#endif

//添加 UL后缀，定义无符号长整型和无符号长长整型常量
#define _UL(x)		(_AC(x, UL))
#define _ULL(x)		(_AC(x, ULL))

//用于生成位掩码，方便进行位操作，如设置、清除或检查某位。
#define _BITUL(x)	(_UL(1) << (x))
#define _BITULL(x)	(_ULL(1) << (x))

#define UL(x)		(_UL(x))
#define ULL(x)		(_ULL(x))

//将宏参数转换成字符串
#define __STR(s)	#s
#define STRINGIFY(s)	__STR(s)

#endif