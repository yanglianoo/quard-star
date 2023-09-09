#include <timeros/loader.h>

extern u64 _num_app[];

// 获取加载的app数量
size_t get_num_app()
{
    return _num_app[0];
}

AppMetadata  get_app_data(size_t app_id)
{

    AppMetadata metadata;

    size_t num_app = get_num_app();

    metadata.start = _num_app[app_id];  // 获取app起始地址
    metadata.size = _num_app[app_id+1] - _num_app[app_id];    // 获取app结束地址

    printk("app start:%x , app end: %x\n",metadata.start,metadata.size);
    
    assert(app_id <= num_app);

    return metadata;
}