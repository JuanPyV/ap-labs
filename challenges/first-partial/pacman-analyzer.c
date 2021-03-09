#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define REPORT_FILE argv[4]
#define INPUT_FILE argv[2]

struct pkg {
    char name[100];
    char installDate[100];
    char lastUpdate[100];
    char removeDate[100];
    int updates;
};

struct pkg packages[1500];

char* getDate(const char *line) {
    int size = 0;
    // Move the size until find the first "]"
    while (line[size] != ']') size++;
    // create date variable with the gotten size
    char *date = calloc(1,size+++5);
    //writing in variable what date is in the line
    for (int i = 0; i < size; i++) {
        date[i] = line[i];
    }
    return date;
}

char* getName(const char *line) {
    int c = 0, nameStart = 0, nameSize = 0;
    // Move until date and [ALMP] finish
    for (int i = 0; i < 2; i++) {
        while (line[nameStart] != ']') {
            nameStart++;
        }
        nameStart += 2;
    }
    // Move until pkg transaction finish
    while (line[nameStart] != ' ') nameStart++;
    nameStart++;
    // know the size of pkg name until we found the next space
    for (int i = nameStart + 1; line[i] != ' '; i++) {
        nameSize++;
    }
    // Assign the size to variable name
    char *name = calloc(1, nameSize+5);
    // Assign variable name the name is in the line
    for (int i = nameStart; line[i] != ' '; i++, c++) {
        name[c] = line[i];
    }
    return name;
}

int pkgTransaction(const char *line) {
    int transChar = 0; // where "]" is, so lately we add 2 because theres a space between the letter and the "]"
    for (int i = 0; i < 2; i++) { // i need to be 2, because im searching for the closing ] of the ALMP/PACMAN stuff
        while (line[transChar] != '\0') {
            transChar++;
            if (line[transChar] == ']') {
                break;
            }
        }
    }
    //Not requires to add extra comparisons but may crash using a log using other transactions
    if (line[transChar + 2] == 'i') return 1; //install
    else if (line[transChar + 2] == 'u') return 2; //upgrade
        //Theres a running and reinstalled  transaction, so i need to add the 3rd condition "m"
    else if (line[transChar + 2] == 'r' && line[transChar + 4] == 'm') return 3; //removed
    // If theres is no transaction im interest just return 0
    return 0;
}

void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);

    // Implement your solution here.
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char tmp[15], pkgTmp[25];
    int lineCount = 0, pkgNumber = 0, installCount = 0, updateCount = 0, removeCount = 0, pacman = 0, almp = 0, almp_scri=0;

    fp = fopen(logFile, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }

    int reportFile = open(report, O_WRONLY|O_CREAT|O_TRUNC, 0644);
    if (reportFile < 0) {
        perror("An error happened creating or writing the file");
        return;
    }

    while ((read = getline(&line, &len, fp)) != -1) {
        //printf("%s\n", line);
        lineCount++;

        char *p = strstr(line, "[PACMAN]");
        char *a = strstr(line, "[ALPM]");
        char *as = strstr(line, "[ALPM-SCRIPTLET]");
        if (p != NULL) pacman++;
        if (a != NULL) almp++;
        if (as != NULL) almp_scri++;

        if (pkgTransaction(line) == 1) {
            installCount++;
            strcpy(packages[pkgNumber].installDate, getDate(line));
            strcpy(packages[pkgNumber].name, getName(line));
            strcpy(packages[pkgNumber].lastUpdate, "-");
            strcpy(packages[pkgNumber].removeDate, "-");
            pkgNumber++;
        } else if (pkgTransaction(line) == 2) {
            updateCount++;
            for (int i = 0; i < 1500; i++) {
                if (strcmp(packages[i].name, getName(line)) == 0) {
                    packages[i].updates++;
                    strcpy(packages[i].lastUpdate, getDate(line));
                    break;
                }
            }
        } else if (pkgTransaction(line) == 3) {
            removeCount++;
            for (int i = 1; i < 1500; i++) {
                if (strcmp(packages[i].name, getName(line)) == 0) {
                    strcpy(packages[i].removeDate, getDate(line));
                    break;
                }
            }
        }
    }

    fclose(fp);

    write(reportFile, "Pacman Packages Report\n", strlen("Pacman Packages Report\n"));
    write(reportFile,"-----------------------\n",strlen("-----------------------\n"));
    write(reportFile, "- Installed packages : ", strlen("- Installed packages : "));
    sprintf(tmp, "%d\n", installCount);
    write(reportFile, tmp, strlen(tmp));
    write(reportFile, "- Removed packages   : ", strlen("- Removed packages   : "));
    sprintf(tmp, "%d\n", removeCount);
    write(reportFile, tmp, strlen(tmp));
    write(reportFile, "- Upgraded packages  : ", strlen("- Upgraded packages  : "));
    sprintf(tmp, "%d\n", updateCount);
    write(reportFile, tmp, strlen(tmp));
    write(reportFile, "- Current installed  : ", strlen("- Current installed  : "));
    sprintf(tmp, "%d\n", installCount - removeCount);
    write(reportFile, tmp, strlen(tmp));

    write(reportFile,"-------------\n",strlen("-------------\n"));
    write(reportFile,"General Stats\n",strlen("General Stats\n"));
    write(reportFile,"-------------\n",strlen("-------------\n"));
    write(reportFile, "- Oldest package               : ", strlen("- Oldest package               : "));
    write(reportFile, packages[0].name, strlen(packages[0].name));
    write(reportFile, "\n- Newest package               : ", strlen("\n- Newest package               : "));
    write(reportFile, packages[pkgNumber-1].name, strlen(packages[pkgNumber-1].name));
    write(reportFile, "\n- Package with no upgrades     : ", strlen("\n- Package with no upgrades     : "));

    char* noUpdatesPkg[pkgNumber];
    int j = 0;
    for (int i = 0; i < pkgNumber; i++) {
        noUpdatesPkg[i] = "";
    }
    for (int i = 0; i < pkgNumber; i++) {
        noUpdatesPkg[j] = "";
        if(packages[i].updates == 0){
            noUpdatesPkg[j] = packages[i].name;
            j++;
        }
    }
    for (int i = 0; i < pkgNumber; i++) {
        if(strcmp(noUpdatesPkg[i],"") != 0){
            sprintf(pkgTmp, "%s, ", noUpdatesPkg[i]);
            write(reportFile, pkgTmp, strlen(pkgTmp));
        }
    }

    write(reportFile, "\n- [ALPM-SCRIPTLET] type count  : ", strlen("\n- [ALPM-SCRIPTLET] type count  : "));
    sprintf(tmp, "%d\n", almp_scri);
    write(reportFile, tmp, strlen(tmp));

    write(reportFile, "- [ALPM] count                 : ", strlen("- [ALPM] count                 : "));
    sprintf(tmp, "%d\n", almp);
    write(reportFile, tmp, strlen(tmp));

    write(reportFile, "- [PACMAN] count               : ", strlen("- [PACMAN] count               : "));
    sprintf(tmp, "%d\n", pacman);
    write(reportFile, tmp, strlen(tmp));

    write(reportFile,"-----------------\n",strlen("-----------------\n"));
    write(reportFile, "List of packages\n", strlen("List of packages\n"));
    write(reportFile,"-----------------\n",strlen("-----------------\n"));
    for (int i = 0; i < 1500; i++) {
        if (strcmp(packages[i].name, "") != 0){
            write(reportFile, "- Package Name          : ", strlen("- Package Name          : "));
            write(reportFile,packages[i].name, strlen(packages[i].name));
            write(reportFile, "\n    - Install date      : ", strlen("\n    - Install date      : "));
            write(reportFile,packages[i].installDate, strlen(packages[i].installDate));
            write(reportFile, "\n    - Last update date  : ", strlen("\n    - Last update date  : "));
            write(reportFile,packages[i].lastUpdate, strlen(packages[i].lastUpdate));
            write(reportFile, "\n    - How many updates  : ", strlen("\n    - How many updates  : "));
            sprintf(tmp, "%d", packages[i].updates);
            write(reportFile,tmp, strlen(tmp));
            write(reportFile, "\n    - Removal date      : ", strlen("\n    - Removal date      : "));
            write(reportFile,packages[i].removeDate, strlen(packages[i].removeDate));
            write(reportFile, "\n", strlen("\n"));
        }
    }
    close(reportFile);
    printf("Report is generated at: [%s]\n", report);
}

int main(int argc, char **argv) {
    if (argc != 5 || strcmp(argv[1], "-input") != 0 || strcmp(argv[3], "-report") != 0) {
        printf("Invalid Arguments. \n");
        return 0;
    }
    analizeLog(INPUT_FILE, REPORT_FILE);
    return 0;
}