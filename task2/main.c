#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>

#define MAX_WORD_LEN 100
#define MAX_WORDS 1000
#define NUM_THREADS 5
#define FILENAME "test.txt" 

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordFrequency;

typedef struct {
    char* text;
    long start;
    long end;
    WordFrequency frequency[MAX_WORDS];
    int word_count;
} ThreadData;

WordFrequency final_freqs[MAX_WORDS];
int final_word_count = 0;
pthread_mutex_t lock; 

void* count_words(void* arg) {
    ThreadData* data = (ThreadData*) arg;
    char word[MAX_WORD_LEN];
    int pos = 0;

    for (long i = data->start; i < data->end; i++) {
        char c = data->text[i];

        if (isalpha(c)) {
            if (pos < MAX_WORD_LEN - 1) {
                word[pos++] = tolower(c);
            }
        } else if (pos > 0) {
            word[pos] = '\0';

            int found = 0;
            for (int j = 0; j < data->word_count; j++) {
                if (strcmp(data->frequency[j].word, word) == 0) {
                    data->frequency[j].count++;
                    found = 1;
                    break;
                }
            }

            if (!found && data->word_count < MAX_WORDS) {
                strcpy(data->frequency[data->word_count].word, word);
                data->frequency[data->word_count].count = 1;
                data->word_count++;
            }


            pos = 0;
        }
    }
    return NULL;
}

void merge_results(ThreadData* thread_data) {
    for (int t = 0; t < NUM_THREADS; t++) {
        for (int i = 0; i < thread_data[t].word_count; i++) {
            char* curr_word = thread_data[t].frequency[i].word;
            int curr_count = thread_data[t].frequency[i].count;

            int found = 0;
            pthread_mutex_lock(&lock); 
            for (int j = 0; j < final_word_count; j++) {
                if (strcmp(final_freqs[j].word, curr_word) == 0) {
                    final_freqs[j].count += curr_count;
                    found = 1;
                    break;
                }
            }

            if (!found && final_word_count < MAX_WORDS) {
                strcpy(final_freqs[final_word_count].word, curr_word);
                final_freqs[final_word_count].count = curr_count;
                final_word_count++;
            }
            pthread_mutex_unlock(&lock); 
        }
    }
}

int main() {
    FILE* file = fopen(FILENAME, "r");
    if (!file) {
        printf("Error opening file\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char* text = malloc(file_size + 1);
    if (!text) {
        printf("Memory allocation failed\n");
        fclose(file);
        return 1;
    }

    fread(text, file_size, 1, file);
    text[file_size] = '\0';
    fclose(file);

    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    pthread_mutex_init(&lock, NULL);

    long segment_size = file_size / NUM_THREADS;

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].text = text;
        thread_data[i].start = i * segment_size;

        while (thread_data[i].start > 0 && isalpha(text[thread_data[i].start - 1])) {
            thread_data[i].start--;
        }

        thread_data[i].end = (i == NUM_THREADS - 1) ? file_size : (i + 1) * segment_size;

        while (thread_data[i].end < file_size && isalpha(text[thread_data[i].end])) {
            thread_data[i].end++;
        }

        thread_data[i].word_count = 0;

        pthread_create(&threads[i], NULL, count_words, &thread_data[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    merge_results(thread_data);

    printf("\nWord Frequencies:\n");
    for (int i = 0; i < final_word_count; i++) {
        printf("%s: %d\n", final_freqs[i].word, final_freqs[i].count);
    }

    free(text);
    pthread_mutex_destroy(&lock);
    return 0;
}

