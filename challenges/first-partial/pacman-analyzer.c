#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define REPORT_FILE argv[4]
#define INPUT_FILE argv[2]

void analizeLog(char *logFile, char *report);

int main(int argc, char **argv) {

    if (argc != 5 || strcmp(argv[1], "-input") != 0 || strcmp(argv[3], "-report") != 0) {
        printf("Invalid Argument. \n");
        return 0;
    }

    analizeLog(INPUT_FILE, REPORT_FILE);

    return 0;
}

char *getDate(char *line) {
    int size = 0;
    while (line[size] != ']') size++;
    char *date = malloc(size++);
    for (int i = 0; i < size; i++) {
        date[i] = line[i];
        //printf("%s\n", date);
    }
    return date;
}

char *getName(char *line) {
    int size = 0, space = 0, sizeName = 0, indexName = 0;
    for (int i = 0; i < strlen(line); i++) {
        if(line[i] == ' '){
            space++;
        }
        if (space == 4){
            size = i;
            indexName = size;
            break;
        }
    }
    while(line[size+1] != ' '){
        size++;
        sizeName++;
    }
    char *name = calloc(1,sizeName);
    //printf("tamaño palabra asignado: %lu\n", strlen(name));
    int c = 0;
    for (int i = indexName+1; i <= size; i++) {
        name[c] = line[i];
        c++;
        //printf("%s\n", name);
    }
    //printf("indice de palabra: %d\n", indexName);
    //printf("tamaño palabra: %d\n", sizeName);
    //printf("size: %d\n", size);
    //printf("name: %s\n", name);

    return name;
}

int pkgTransaction(char *line){
    char *i = strstr(line, "installed");
    char *u = strstr(line, "upgraded");
    char *r = strstr(line, "removed");
    if (i != NULL) return 1;
    if (u != NULL) return 2;
    if (r != NULL) return 3;

    return 0;
}

void analizeLog(char *logFile, char *report) {
    //printf("Generating Report from: [%s] log file\n", logFile);

    // Implement your solution here.
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int lineCount = 0;

    fp = fopen(logFile, "r");
    if (fp == NULL) {
        perror("Error");
        return;
    }

    while ((read = getline(&line, &len, fp)) != -1) {
        //printf("Retrieved line of length %zu:\n", read);

        lineCount++;
        if (pkgTransaction(line) == 1 || pkgTransaction(line) == 2 || pkgTransaction(line) == 3) {
            printf("Date: %s\n",getDate(line));
            printf("Transaction: %d\n", pkgTransaction(line));
            printf("PKG Name: %s\n", getName(line));
            printf("Line: %d\n", lineCount);
        }
    }

    fclose(fp);

    //printf("Report is generated at: [%s]\n", report);
    return;
}
