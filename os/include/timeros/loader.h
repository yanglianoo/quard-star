#ifndef TOS_LOADER_H__
#define TOS_LOADER_H__

#include <timeros/types.h>
#include <timeros/assert.h>
#include <timeros/stdio.h>


// 假设这个结构用于存储应用程序元数据
typedef struct {
    uint64_t start;
    uint64_t size;
} AppMetadata;

typedef struct 
{
    u64 entry;  //程序入口地址
    


} proc_t;

// 获取加载的app数量
size_t get_num_app();
AppMetadata  get_app_data(size_t app_id);
void load_app(size_t app_id);

#endif