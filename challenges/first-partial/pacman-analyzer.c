#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define REPORT_FILE argv[4]
#define INPUT_FILE argv[2]

struct pkg {
    char name[50];
    char date[50];
    char lastUpdate[50];
    int updates;
    char removeDate[50];
};

struct pkg packages[1000];

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
    }
    return date;
}

char *getName(char *line) {
    int size = 0, space = 0, sizeName = 0, indexName = 0;
    for (int i = 0; i < strlen(line); i++) {
        if (line[i] == ' ') {
            space++;
        }
        if (space == 4) {
            size = i;
            indexName = size;
            break;
        }
    }
    while (line[size + 1] != ' ') {
        size++;
        sizeName++;
    }
    char *name = calloc(1, sizeName);
    int c = 0;
    for (int i = indexName + 1; i <= size; i++) {
        name[c] = line[i];
        c++;
    }
    return name;
}

int pkgTransaction(char *line) {
    int size = 0, space = 0, sizeName = 0, indexName = 0;
    for (int i = 0; i < strlen(line); i++) {
        if (line[i] == ' ') {
            space++;
        }
        if (space == 3) {
            size = i;
            indexName = size;
            break;
        }
    }
    while (line[size + 1] != ' ') {
        size++;
        sizeName++;
    }
    char *name = calloc(1, sizeName);
    int c = 0;
    for (int i = indexName + 1; i <= size; i++) {
        name[c] = line[i];
        c++;
    }
    if(strcmp(name,"installed")==0){
        return 1;
    }else if(strcmp(name,"upgraded")==0){
        return 2;
    }else if(strcmp(name,"removed")==0){
        return 3;
    }else{
        return 0;
    }
}

void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);

    // Implement your solution here.
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int lineCount = 0, installCount = 0, updateCount = 0, removeCount = 0;

    fp = fopen(logFile, "r");
    if (fp == NULL) {
        perror("Error");
        return;
    }
    int pkgNumber = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        //printf("Retrieved line of length %zu:\n", read);
        lineCount++;
        if (pkgTransaction(line) == 1) {
            pkgNumber++;
            installCount++;
            strcpy(packages[pkgNumber].date, getDate(line));
            strcpy(packages[pkgNumber].name, getName(line));
            /*
            printf("--------------------------------\n");
            printf("Package: %d\n", pkgNumber);
            printf("Install Date: %s\n", packages[pkgNumber].date);
            printf("Transaction: installed\n");
            printf("PKG Name: %s\n", packages[pkgNumber].name);
            printf("Line: %d\n", lineCount);
            */
        } else if (pkgTransaction(line) == 2) {
            updateCount++;
            /*
            printf("--------------------------------\n");
            printf("UPDATE FOUND!!!!\n");
            printf("pkgName: %s\n", getName(line));
            printf("pkgName in array: %s\n",packages[0].name);
             */
            for (int i = 1; i < 1000; i++) {
                if(strcmp(packages[i].name, getName(line)) == 0){
                    //printf("pkgName in array: %s\n",packages[i].name);
                    packages[i].updates++;
                    strcpy(packages[i].lastUpdate, getDate(line));
                    /*
                    printf("--------------------------------\n");
                    printf("Install Date: %s\n", packages[i].date);
                    printf("PKG Name: %s\n", packages[i].name);
                    printf("Last update: %s\n", packages[i].lastUpdate);
                    printf("Updates: %d\n", packages[i].updates);
                    printf("Line: %d\n", lineCount);
                    */
                    break;
                }
            }
        } else if (pkgTransaction(line) == 3) {
            removeCount++;
            /*
            printf("--------------------------------\n");
            printf("REMOVED FOUND!!!!\n");
            printf("pkgName: %s\n", getName(line));
             */
            for (int i = 1; i < 1000; i++) {
                if(strcmp(packages[i].name, getName(line)) == 0){
                    //printf("pkgName in array: %s\n",packages[i].name);
                    strcpy(packages[i].removeDate, getDate(line));
                    /*
                    printf("--------------------------------\n");
                    printf("Install Date: %s\n", packages[i].date);
                    printf("PKG Name: %s\n", packages[i].name);
                    printf("Last update: %s\n", packages[i].lastUpdate);
                    printf("Updates: %d\n", packages[i].updates);
                    printf("Remove Date: %s\n", packages[i].removeDate);
                    printf("Line: %d\n", lineCount);
                     */
                    break;
                }
            }
        }
    }
    printf("Pacman Packages Report\n");
    printf("--------------------------------\n");
    printf("Total lines: %d\n", lineCount);
    printf("- Installed packages : %d\n", installCount);
    printf("- Removed packages   : %d\n", removeCount);
    printf("- Upgraded packages  : %d\n", updateCount);
    printf("- Current installed  : %d\n", installCount-removeCount);
    printf("List of packages\n");
    printf("--------------------------------\n");

    for (int i = 1; i < 1000; i++) {
        if (strcmp(packages[i].name, "") != 0) {
            printf("- Package Name          : %s\n", packages[i].name);
            printf("    - Install date      : %s\n", packages[i].date);
            printf("    - Last update date  : %s\n", packages[i].lastUpdate);
            printf("    - How many updates  : %d\n", packages[i].updates);
            printf("    - Removal date      : %s\n", packages[i].removeDate);
        } else {
            break;
        }
    }
    fclose(fp);

}
