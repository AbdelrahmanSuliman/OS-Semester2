#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>


size_t file_sizes[100];
size_t file_count = 0;

void traverse_directory(const char *dir_path) {
    struct dirent *entry;
    struct stat file_stat;
    char path[1024];

    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);

        if (stat(path, &file_stat) == 0) {
            if (S_ISREG(file_stat.st_mode)) {
                if (file_count < 100) {
                    file_sizes[file_count++] = file_stat.st_size;
                }
            } else if (S_ISDIR(file_stat.st_mode)) {
                traverse_directory(path);
            }
        } else {
            perror("stat");
        }
    }

    closedir(dir);
}

int compare_sizes(const void *a, const void *b) {
    return (*(size_t *)a - *(size_t *)b);
}

void print_histogram(size_t bin_width) {
    if (file_count == 0) {
        printf("No files found.\n");
        return;
    }

    qsort(file_sizes, file_count, sizeof(size_t), compare_sizes);

    size_t min_size = file_sizes[0];
    size_t max_size = file_sizes[file_count - 1];
    size_t bin_start = min_size;
    size_t bin_end = bin_start + bin_width - 1;

    printf("\nHistogram (Bin Width = %zu bytes):\n", bin_width);

    while (bin_start <= max_size) {
        int count = 0;
        for (size_t i = 0; i < file_count; i++) {
            if (file_sizes[i] >= bin_start && file_sizes[i] <= bin_end) {
                count++;
            }
        }

        printf("[%6zu - %6zu]: %d\n", bin_start, bin_end, count);
        bin_start = bin_end + 1;
        bin_end = bin_start + bin_width - 1;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <directory> <bin_width>\n", argv[0]);
        return 1;
    }

    size_t bin_width = atoi(argv[2]);
    if (bin_width == 0) {
        printf("Invalid bin width.\n");
        return 1;
    }

    traverse_directory(argv[1]);
    print_histogram(bin_width);

    return 0;
}

