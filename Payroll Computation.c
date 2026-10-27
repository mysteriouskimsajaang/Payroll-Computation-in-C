
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//IMPORTANT RATES
#define SSS_RATE 0.045
#define PHILHEALTH_RATE 0.05
#define HDMF_RATE 0.02
#define HDMF_MAX 200.0
/***********************************************
* Function : clearScreen()
* Description : Clears the terminal screen
* Preconditions : None
* Parameters : None
* Return Value : None
***********************************************/
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
/***********************************************
* Function : pauseProgram()
* Description : Pauses the program until the user presses
Enter
* Preconditions : None
* Parameters : None
* Return Value : None
***********************************************/
void pauseProgram() {
    printf("\nPress Enter to continue...");
    while (getchar() != '\n' && !feof(stdin));
}
/***********************************************
* Function : displayMonths()
* Description : Displays the list of months in a year
* Preconditions : None
* Parameters : None
* Return Value : None
***********************************************/
void displayMonths() {
    printf("Months:\n");
    printf("[1] January\n[2] February\n[3] March\n[4] April\n");
    printf("[5] May\n[6] June\n[7] July\n[8] August\n");
    printf("[9] September\n[10] October\n[11] November\n[12] December\n");
}
/***********************************************
* Function : getWageDetails()
* Description : Prompts the user to enter wage details
* Preconditions : None
* Parameters : cWageType - pointer to store wage type
(daily/hourly)
* fWageRate - pointer to store wage rate
* cName - pointer to store the users full name
* totalAbsences - total number of absences
* Return Value : 1 if the user wants to compute another
salary,
* 0 otherwise
***********************************************/
void getWageDetails(char *cWageType, float *fWageRate, char *cName) {
    do {
        printf("Enter your full name: ");
        fgets(cName, 50, stdin);
        cName[strcspn(cName, "\n")] = '\0';
    } while (cName[0] == '\0');

    do {
        printf("Enter wage type [D]aily or [H]ourly: ");
        scanf(" %c", cWageType);
        while (getchar() != '\n');
    } while (*cWageType != 'D' && *cWageType != 'd' &&
             *cWageType != 'H' && *cWageType != 'h');

    do {
        printf("Enter wage rate: ");
        if (scanf("%f", fWageRate) != 1 || *fWageRate < 0) {
            *fWageRate = -1;
        }
        while (getchar() != '\n');
    } while (*fWageRate < 0);
}

void getAttendance(int *nTotalDaysPresent, int *nTotalHoursWorked,
                   char cWageType, int *month, int *year,
                   int *totalAbsences) {
    displayMonths();
    do {
        printf("Enter the month number: ");
        if (scanf("%d", month) != 1 || *month < 1 || *month > 12) {
            *month = 0;
        }
        while (getchar() != '\n');
    } while (*month == 0);

    do {
        printf("Enter the year: ");
        if (scanf("%d", year) != 1 || *year < 1) {
            *year = 0;
        }
        while (getchar() != '\n');
    } while (*year == 0);

    if (cWageType == 'D' || cWageType == 'd') {
        do {
            printf("Enter total days present: ");
            if (scanf("%d", nTotalDaysPresent) != 1 ||
                *nTotalDaysPresent < 0) {
                *nTotalDaysPresent = -1;
            }
            while (getchar() != '\n');
        } while (*nTotalDaysPresent < 0);

        do {
            printf("Enter total absences: ");
            if (scanf("%d", totalAbsences) != 1 || *totalAbsences < 0) {
                *totalAbsences = -1;
            }
            while (getchar() != '\n');
        } while (*totalAbsences < 0);
        *nTotalHoursWorked = 0;
    } else {
        do {
            printf("Enter total hours worked: ");
            if (scanf("%d", nTotalHoursWorked) != 1 ||
                *nTotalHoursWorked < 0) {
                *nTotalHoursWorked = -1;
            }
            while (getchar() != '\n');
        } while (*nTotalHoursWorked < 0);
        *nTotalDaysPresent = 0;
        *totalAbsences = 0;
    }
}

int computeAndDisplaySalary(char cWageType, float fWageRate, int
    nTotalDaysPresent, int nTotalHoursWorked, char *cName, int
    totalAbsences) {
    char computeAgainChoice;
    do {
        clearScreen();
        printf("%s\n", cName);
        if (cWageType == 'D' || cWageType == 'd') {
            printf("Total Number of Days Present: %d\n",
                nTotalDaysPresent);
            printf("Total Number of Days Absent: %d\n",
                totalAbsences);
        } else if (cWageType == 'H' || cWageType == 'h') {
            printf("Total Number of Hours Worked: %d\n",
                nTotalHoursWorked);
        }
        float loanAmount = 0.0;
        char loanChoice;
        int validInput = 0;
        do {
            printf("\nDo you have a current loan [Y]es or [N]o: ");
            if (scanf(" %c", &loanChoice) == 1) {
                while (getchar() != '\n');
                if (loanChoice == 'Y' || loanChoice == 'y' ||
                    loanChoice == 'N' || loanChoice == 'n') {
                    validInput = 1;
                } else {
                    printf("Invalid input! Please enter 'Y' for "
                           "Yes or 'N' for No.\n");
                }
            } else {
                printf("Invalid input! Please try again.\n");
                while (getchar() != '\n');
            }
        } while (!validInput);
        if (loanChoice == 'Y' || loanChoice == 'y') {
            validInput = 0;
            do {
                printf("\nEnter the loan amount: ");
                if (scanf("%f", &loanAmount) == 1 && loanAmount >=
                    0) {
                    while (getchar() != '\n');
                    validInput = 1;
                } else {
                    printf("Invalid input! Please enter a positive "
                           "number.\n");
                    while (getchar() != '\n');
                }
            } while (!validInput);
        }
        float grossSalary;
        if (cWageType == 'D' || cWageType == 'd') {
            grossSalary = fWageRate * nTotalDaysPresent;
        } else {
            grossSalary = fWageRate * nTotalHoursWorked;
        }
        float adjustedGross = grossSalary - loanAmount;
        float HDMF;
        if (adjustedGross * HDMF_RATE > HDMF_MAX) {
            HDMF = HDMF_MAX;
        } else {
            HDMF = adjustedGross * HDMF_RATE;
        }
        float SSS = adjustedGross * SSS_RATE;
        float PHILHEALTH = adjustedGross * PHILHEALTH_RATE;
        float tax = 0.0;
        if (adjustedGross > 21500) {
            tax = (adjustedGross - 21500) * 0.15;
        }
        float totalDeductions = SSS + PHILHEALTH + HDMF + tax;
        float netSalary = adjustedGross - totalDeductions;
        // Output salary breakdown
        printf("\nBasic Pay: %.2f\n", grossSalary);
        printf("Loan: %.2f\n", loanAmount);
        printf("Gross Pay: %.2f\n", grossSalary);
        printf("\nSSS Contribution: %.2f\n", SSS);
        printf("PhilHealth Contribution: %.2f\n", PHILHEALTH);
        printf("PAG-IBIG Contribution: %.2f\n", HDMF);
        printf("Tax (15%% excess above 21,500): %.2f\n", tax);
        printf("Total Deductions: %.2f\n", totalDeductions);
        printf("Net Pay: %.2f\n", netSalary);
        validInput = 0;
        do {
            printf("\nDo you want to compute another salary? [Y]es "
                   "or [N]o: ");
            if (scanf(" %c", &computeAgainChoice) == 1) {
                while (getchar() != '\n');
                if (computeAgainChoice == 'Y' ||
                    computeAgainChoice == 'y' || computeAgainChoice == 'N' ||
                    computeAgainChoice == 'n') {
                    validInput = 1;
                } else {
                    printf("Invalid input! Please enter 'Y' for "
                           "Yes or 'N' for No.\n");
                }
            } else {
                printf("Invalid input! Please try again.\n");
                while (getchar() != '\n');
            }
        } while (!validInput);
        if (computeAgainChoice == 'Y' || computeAgainChoice ==
            'y') {
            return 1;
        }
    } while (computeAgainChoice != 'N' && computeAgainChoice !=
        'n');
    printf("\nThank you for using the salary computation system.\n");
    pauseProgram();
    return 0;
}
/***********************************************
* Function : main()
* Description : Main function to control the flow of the
program
* Preconditions : None
* Parameters : None
* Return Value : 0 upon successful program termination
***********************************************/
int main() {
    char cName[50];
    char cWageType = '\0';
    float fWageRate = 0.0;
    int nTotalDaysPresent = 0, nTotalHoursWorked = 0;
    int month = 0, year = 0;
    char cOptions;
    int totalAbsences;
    int hasDetails = 0;
    int hasAttendance = 0;
    int continueProgram = 1;
    /***********************************************
    * Loop : do-while in main()
    * Description : Executes the main menu options repeatedly
    until the user chooses to exit
    * Preconditions : None
    * Parameters : None
    * Return Value : Terminates the program when the user chooses
    to exit
    ***********************************************/
    do { // main menu
        clearScreen();
        printf("Welcome to Payslip Computation!\n");
        printf("[D / d] Add my Details\n");
        printf("[A / a] Add my Attendance\n");
        printf("[E / e] Compute my Salary\n");
        printf("[X / x] Exit\n");
        printf("\nAnswer: ");
        scanf(" %c", &cOptions);
        while (getchar() != '\n');
        switch (cOptions) {
        case 'D': case 'd': // ADD MY DETAILS
            getWageDetails(&cWageType, &fWageRate, cName);
            hasDetails = 1;
            break;
        case 'A': case 'a': // ADD MY ATTENDANCE
            if (!hasDetails) {
                printf("\nError: Please complete option 'D' "
                       "(Add my Details) first.\n");
                pauseProgram();
            } else {
                getAttendance(&nTotalDaysPresent,
                    &nTotalHoursWorked, cWageType, &month, &year, &totalAbsences);
                hasAttendance = 1;
            }
            break;
        case 'E': case 'e': //COMPUTE SALARY
            if (!hasDetails) {
                printf("\nError: Please complete option 'D' "
                       "(Add my Details) first.\n");
                pauseProgram();
            } else if (!hasAttendance) {
                printf("\nError: Please complete option 'A' "
                       "(Add my Attendance) first.\n");
                pauseProgram();
            } else {
                continueProgram =
                computeAndDisplaySalary(cWageType, fWageRate, nTotalDaysPresent,
                    nTotalHoursWorked, cName, totalAbsences);
            }
            break;
        case 'X': case 'x':
            printf("\nThank you for using the program!\n");
            continueProgram = 0;
            break;
        default:
            printf("\nInvalid Option. Please try again.\n");
            pauseProgram();
            break;
        }
    } while (continueProgram);
    return 0;
}
