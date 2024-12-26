#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 256

void remove_vowels(char *str) {
    char *read_ptr = str, *write_ptr = str;
    while (*read_ptr) {
        if (!strchr("aeiouAEIOU", *read_ptr)) {
            *write_ptr++ = *read_ptr;
        }
        read_ptr++;
    }
    *write_ptr = '\0';
}

int main(int argc, char *argv[]) {
    char buffer[BUFFER_SIZE];

    const char *child_id = (argc > 1) ? argv[1] : "Unknown Child";

    while (fgets(buffer, BUFFER_SIZE, stdin)) {
        remove_vowels(buffer);
        printf("%s processed: %s", child_id, buffer);
    }

    return 0;
}
