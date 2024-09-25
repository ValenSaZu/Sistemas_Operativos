#include <stdio.h>
#include <stdlib.h>

#define MAX_LENGTH 256

int main() {
    char buffer[MAX_LENGTH];
    
    while (fgets(buffer, MAX_LENGTH, stdin) != NULL) {
        printf("%s", buffer);
    }

    return 0;
}

