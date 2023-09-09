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

// 获取加载的app数量
size_t get_num_app();
AppMetadata  get_app_data(size_t app_id);
#endif