#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

/* month_day function's prototype*/
void month_day(int year, int yearday, int *pmonth, int *pday) {
    char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    int daysPerMonths[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (yearday <= 366 && yearday >= 1) {
        if (year % 4 == 0 && year % 100 != 0 || year % 400 == 0) {
            // If its a leap year, it replace the 28 days of february to be 29 days
            daysPerMonths[1] = 29;
        }
        for (int i = 0; i < 12; i++) {
            if (yearday <= daysPerMonths[i]) {
                *pmonth = i;
                *pday = yearday;
                break;
            } else {
                yearday -= daysPerMonths[i];
            }
        }
        printf("%s %d, %d\n", months[*pmonth], *pday, year);
    } else {
        printf("Not valid date, day number must be between 1 and 366\n");
    }
}

int main(int argc, char **argv) {

    int year, yearday, pmonth, pday;
    char *pyear, *pyearday;
    errno = 0;

    if (argc != 3) {
        printf("Invalid Arguments, expected: ./month_day <year> <yearday>\n");
        return 0;
    }

    long convYear = strtol(argv[1], &pyear, 10);
    long convYearDay = strtol(argv[2], &pyearday, 10);

    if (errno != 0 || *pyear != '\0' || convYear > INT_MAX || convYear < INT_MIN) {
        printf("'%s' is not a valid year number, it must be a number.\n", argv[1]);
    }
    if (errno != 0 || *pyearday != '\0' || convYearDay > INT_MAX || convYearDay < INT_MIN) {
        printf("'%s' is not a valid day number, it must be a number between 1 and 366.\n", argv[2]);
    } else {
        year = convYear;
        yearday = convYearDay;
        month_day(year, yearday, &pmonth, &pday);
    }

    return 0;
}
