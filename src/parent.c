// parent.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 256

int main() {
    int pipe1[2], pipe2[2];
    pid_t pid1, pid2;

    // Создание двух pipe
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Создание первого дочернего процесса
    if ((pid1 = fork()) == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        // Дочерний процесс 1
        close(pipe1[1]); // Закрываем конец записи pipe1
        dup2(pipe1[0], STDIN_FILENO); // Перенаправляем stdin на pipe1
        close(pipe1[0]);

        execl("./child", "child", "Child 1", NULL); // Запуск дочернего процесса 1
        perror("execl");
        exit(EXIT_FAILURE);
    }

    // Создание второго дочернего процесса
    if ((pid2 = fork()) == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0) {
        // Дочерний процесс 2
        close(pipe2[1]); // Закрываем конец записи pipe2
        dup2(pipe2[0], STDIN_FILENO); // Перенаправляем stdin на pipe2
        close(pipe2[0]);

        execl("./child", "child", "Child 2", NULL); // Запуск дочернего процесса 2
        perror("execl");
        exit(EXIT_FAILURE);
    }

    // Родительский процесс
    close(pipe1[0]); // Закрываем конец чтения pipe1
    close(pipe2[0]); // Закрываем конец чтения pipe2

    char input[BUFFER_SIZE];
    while (1) {
        printf("Enter a string (or 'exit' to quit): ");
        if (!fgets(input, BUFFER_SIZE, stdin)) {
            break;
        }

        if (strncmp(input, "exit", 4) == 0) {
            break;
        }

        // Отправка строки в соответствующий pipe
        if (strlen(input) > 11) { // 10 символов + \n
            write(pipe2[1], input, strlen(input));
        } else {
            write(pipe1[1], input, strlen(input));
        }
    }

    close(pipe1[1]); // Закрываем конец записи pipe1
    close(pipe2[1]); // Закрываем конец записи pipe2

    // Ожидание завершения дочерних процессов
    wait(NULL);
    wait(NULL);

    return 0;
}