#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>  

#define TARGET_PATH "../user/bin/"

int compare_strings(const void* a, const void* b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

void insert_app_data() {

    FILE* f = fopen("src/link_app.S", "w");
    if (f == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }
    
    char* apps[100]; // Assuming a maximum of 100 apps
    int app_count = 0;

    // Read the directory and collect app names
    DIR* dir = opendir("./user/bin");
    if (dir == NULL) {
        perror("Failed to open directory");
        exit(EXIT_FAILURE);
    }
    

    struct dirent* dir_entry;
    while ((dir_entry = readdir(dir)) != NULL) {


        char* name_with_ext = dir_entry->d_name;
        
        // 排除掉 . 和 .. 条目
        if (name_with_ext[0] == '.' && (name_with_ext[1] == '\0' || (name_with_ext[1] == '.' && name_with_ext[2] == '\0'))) {
            continue; // Skip this entry
        }
        
        int len = strlen(name_with_ext);

        // Remove file extension by replacing the dot with a null terminator  
        for (int i = 0; i < len; i++) {
            if (name_with_ext[i] == '.') {
                name_with_ext[i] = '\0';
                break;
            }
        }
        
        // strdup 函数用于创建一个字符串的副本，并返回指向新字符串的指针。
        apps[app_count] = strdup(name_with_ext);
        app_count++;
        printf("File name: %s, app_count: %d\n", name_with_ext, app_count);
    }
    
    closedir(dir);
    
    // 对 app name 排序
    qsort(apps, app_count, sizeof(char*), compare_strings);

    fprintf(f, "\n.align 3\n.section .data\n.global _num_app\n_num_app:\n.quad %d", app_count);
    
    for (int i = 0; i < app_count; i++) {
        fprintf(f, "\n.quad app_%d_start", i);
    }
    fprintf(f, "\n.quad app_%d_end", app_count - 1);

    for (int i = 0; i < app_count; i++) {
        printf("app_%d: %s\n", i, apps[i]);
        fprintf(f, "\n.section .data\n.global app_%d_start\n.global app_%d_end\n.align 3\napp_%d_start:\n.incbin \"%s%s\"\napp_%d_end:", i, i, i, TARGET_PATH, apps[i], i);
        free(apps[i]);
    }
    
    fclose(f);
}


int main() {
    insert_app_data();
    return 0;
}
