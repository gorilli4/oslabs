#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// Глобальные переменные
int *array;
int num_elements;
int max_threads;
int current_threads = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int left;
    int right;
} ThreadData;

// Функция для параллельной быстрой сортировки
void *quicksort(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int left = data->left;
    int right = data->right;

    if (left >= right)
        return NULL;

    int pivot = array[(left + right) / 2];
    int i = left, j = right;

    while (i <= j) {
        while (array[i] < pivot)
            i++;
        while (array[j] > pivot)
            j--;
        if (i <= j) {
            int temp = array[i];
            array[i] = array[j];
            array[j] = temp;
            i++;
            j--;
        }
    }

    pthread_t thread1, thread2;
    ThreadData leftData = {left, j};
    ThreadData rightData = {i, right};

    pthread_mutex_lock(&mutex);
    int create_left_thread = 0, create_right_thread = 0;
    if (current_threads < max_threads) {
        current_threads++;
        create_left_thread = 1;
    }
    if (current_threads < max_threads) {
        current_threads++;
        create_right_thread = 1;
    }
    pthread_mutex_unlock(&mutex);

    if (create_left_thread)
        pthread_create(&thread1, NULL, quicksort, &leftData);
    else
        quicksort(&leftData);

    if (create_right_thread)
        pthread_create(&thread2, NULL, quicksort, &rightData);
    else
        quicksort(&rightData);

    if (create_left_thread)
        pthread_join(thread1, NULL);
    if (create_right_thread)
        pthread_join(thread2, NULL);

    pthread_mutex_lock(&mutex);
    current_threads -= (create_left_thread + create_right_thread);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

// Функция для инициализации массива
void initialize_array() {
    srand(time(NULL));
    for (int i = 0; i < num_elements; i++) {
        array[i] = rand() % 100;
    }
}

// Функция для вывода массива
void print_array() {
    for (int i = 0; i < num_elements; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <number_of_elements> <max_threads>\n", argv[0]);
        return 1;
    }

    num_elements = atoi(argv[1]);
    max_threads = atoi(argv[2]);

    if (num_elements <= 0 || max_threads <= 0) {
        printf("Both number_of_elements and max_threads must be positive integers.\n");
        return 1;
    }

    array = (int *)malloc(num_elements * sizeof(int));
    if (array == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    initialize_array();

    printf("Original array:\n");
    print_array();

    ThreadData data = {0, num_elements - 1};
    quicksort(&data);

    printf("Sorted array:\n");
    print_array();

    free(array);
    return 0;
}