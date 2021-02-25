#include <stdio.h>

int main() {
    char c, word[10];
    int cont = 0;
    while ((c = getchar()) != EOF) {
        if (c == '\n') {
            cont--;
            while (cont >= 0) {
                printf("%c", word[cont--]);
            }
            cont = 0;
            printf("\n");
        } else {
            word[cont++] = c;
        }
    }
    return 0;
}