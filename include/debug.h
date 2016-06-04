#ifndef NAIVE_OS_INCLUDE_DEBUG_H_
#define NAIVE_OS_INCLUDE_DEBUG_H_

#include <console.h>
#include <vargs.h>

#define assert(x, info)     do { \
                                if (!(x)) { \
                                    panic(info); \
                                } \
                            } while(0)

// 编译期间静态检测
#define static_assert(x)    switch (x) { case 0: case (x): ; }

// 初始化 Debug 信息
void init_debug();

// 打印当前的函数调用栈信息
void panic(const char *msg);

// 打印当前的段存器值
void print_cur_status();



void printk(const char *format, ...);

void printk_color(real_color_t back, real_color_t fore, const char *format, ...);

#endif	// !NAIVE_OS_INCLUDE_DEBUG_H_