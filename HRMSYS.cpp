/* HUMAN RESOURCE MANAGING SYSTEM */
#include <stdio.h>
#include <string.h>  
#include <stdlib.h>

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void getStringInput(char *buffer, int size, const char *prompt) {
    printf("%s", prompt);
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0; // Remove trailing newline
}

typedef enum {
    Male = 0,
    Female = 1
} Sex;

const char* sexToString(Sex gender) {
    switch (gender) {
        case Male: return "Male";
        case Female: return "Female";
        default: return "Unknown";
    }
}

typedef struct {
    char Name[100];
    char EmployeeID[100];
    char Birth[20]; // Store as dd/mm/yy
    Sex Gender;
    char NationalID[20];
    char TaxNumber[20];
    char PhoneNumber[15];
    char Email[50];
} PersonalData;

typedef enum {
    Probation = 0,      // Thử việc
    DefiniteTerm = 1,   // Xác định thời gian
    IndefiniteTerm = 2  // Không xác định thời gian
} ContractType;

const char* contractTypeToString(ContractType type) {
    switch (type) {
        case Probation: return "Probation";
        case DefiniteTerm: return "Definite Term";
        case IndefiniteTerm: return "Indefinite Term";
        default: return "Unknown";
    }
}

typedef struct {
    ContractType Type;
    char LaborDate[30]; // Store as dd/mm/yy - dd/mm/yy
    char Pos[20]; // Position
    char Dept[20]; // Department
    unsigned long long DealtSalary;
} EmploymentContract;

typedef struct {
    char Code[20];
    double PayLevel; // ?% Salary
    char InsuranceDate[30]; // Store as dd/mm/yy - dd/mm/yy
} InsuranceInfo;

typedef struct {
    InsuranceInfo BHXH, BHYT, BHTN;
} Insurance;

typedef enum {
    Cash = 0,
    BankTransfer = 1
} Payment; 

const char* paymentToString(Payment method){
    switch (method) {
        case Cash: return "Cash";
        case BankTransfer: return "Bank Transfer";
        default: return "Unknown";
    }
}

int FinalSalaryCalculation(long long baseSalary, int workTimeThisMonth, long long allowance[3], long long bonus[3], double taxRate) {
    long long totalAllowance = allowance[0] + allowance[1] + allowance[2];
    long long totalBonus = bonus[0] + bonus[1] + bonus[2];
    long long grossSalary = baseSalary + (workTimeThisMonth * (baseSalary / 160)) + totalAllowance + totalBonus; // Assuming 160 hours in a month
    return (int)(grossSalary * (1 - taxRate));
}

typedef struct {
    long long BaseSalary;
    int WorkTimeThisMonth;
    long long Allowance[3]; // Lunch, transportation, residence
    long long Bonus[3]; // KPIs, holidays, projects 
    double Tax;
    long long FinalSalary; // Calculated
    Payment Method;
} SalaryManaging;

struct EmployeeDataNode {
    PersonalData PD;
    EmploymentContract EC;
    Insurance I;
    SalaryManaging SM;
    struct EmployeeDataNode* next;
};

typedef struct {
    char EmployeeID[100];
    char CheckIn[10], CheckOut[10]; // Both store as xx:yy
    int Overtime;
} WorkTime;

struct WorkTimeNode {
    WorkTime WT;
    struct WorkTimeNode* next;
};

typedef enum {
    No = 0,
    Yes = 1
} Letter;

const char* letterToString(Letter absenceletter){
    switch (absenceletter) {
        case No: return "No";
        case Yes: return "Yes";
        default: return "Unknown";
    }
}

typedef enum {
    Pending = 0,
    Approved = 1,
    Rejected = 2
} LetterStatus;

const char* astatusToString(LetterStatus letterstatus){
    switch (letterstatus) {
        case Pending: return "Pending";
        case Approved: return "Approved";
        case Rejected: return "Rejected";
        default: return "Unknown";
    }
}

typedef struct {
    char EmployeeID[100];
    Letter AbsenceLetter;
    char AbsenceDate[30];
    LetterStatus letterstatus;
} DayOff;

struct DayOffNode {
    DayOff DOff;
    struct DayOffNode* next;
};

typedef enum {
    Failed = 0,
    Ongoing = 1,
    Passed = 2
} CourseStatus;

const char* cstatusToString(CourseStatus coursestatus){
    switch (coursestatus) {
        case Failed: return "Failed";
        case Ongoing: return "Ongoing";
        case Passed: return "Passed";
        default: return "Unknown";
    }
}

typedef struct {
    char EmployeeID[100];
    char CourseName[100];
    int CourseTime; // Hours
    long long CourseFee;
    CourseStatus coursestatus;
} Education;

struct EducationNode {
    Education E;
    struct EducationNode* next;
};

typedef enum {
    Volunteered = 0,
    Fired = 1,
    ContractEnded = 2
} LayOffReason;

const char* layoffreasonToString(LayOffReason reason){
    switch (reason) {
        case Volunteered: return "Volunteered";
        case Fired: return "Fired";
        case ContractEnded: return "Contract Ended";
        default: return "Unknown";
    }
}

typedef struct {
    char EmployeeID[100];
    char Date[30];
    LayOffReason Reason;
} LayOff;

struct LayOffNode{
    LayOff LO;
    struct LayOffNode* next;
};

typedef struct {
    double LayOffRatio;
    int SalaryFee;
    double InsuranceFee;
    int EducationFee;
} HRReport;

//Initializing the linked lists
struct EmployeeDataNode* EDNode = NULL;
struct WorkTimeNode* WTNode = NULL;
struct DayOffNode* DONode = NULL;
struct EducationNode* ENode = NULL;
struct LayOffNode* LONode = NULL;


int getLastEmployeeID_FromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return 0;

    char line[512];
    char lastID[10] = "NV0000";

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "NV", 2) == 0) {
            sscanf(line, "%[^|]", lastID);
        }
    }

    fclose(file);
    int numericID = 0;
    sscanf(lastID, "NV%d", &numericID);
    return numericID;
}

void saveEmployeeToFile(const struct EmployeeDataNode* node, const char* filename) {
    FILE* file = fopen(filename, "a");
    if (!file) {
        printf("Cannot open file %s to write!\n", filename);
        return;
    }
    fprintf(file, "%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%llu|%s|%.2f|%s|%s|%.2f|%s|%s|%.2f|%s|%lld|%d|%lld|%lld|%lld|%lld|%lld|%lld|%.2f|%lld|%s\n",
        node->PD.EmployeeID, node->PD.Name, node->PD.Birth, sexToString(node->PD.Gender),
        node->PD.NationalID, node->PD.TaxNumber, node->PD.PhoneNumber, node->PD.Email,
        contractTypeToString(node->EC.Type), node->EC.LaborDate, node->EC.Pos, node->EC.Dept, node->EC.DealtSalary,
        node->I.BHXH.Code, node->I.BHXH.PayLevel, node->I.BHXH.InsuranceDate,
        node->I.BHYT.Code, node->I.BHYT.PayLevel, node->I.BHYT.InsuranceDate,
        node->I.BHTN.Code, node->I.BHTN.PayLevel, node->I.BHTN.InsuranceDate,
        node->SM.BaseSalary, node->SM.WorkTimeThisMonth,
        node->SM.Allowance[0], node->SM.Allowance[1], node->SM.Allowance[2],
        node->SM.Bonus[0], node->SM.Bonus[1], node->SM.Bonus[2],
        node->SM.Tax, node->SM.FinalSalary, paymentToString(node->SM.Method));
    fclose(file);
}

void updateEmployeeFile(const struct EmployeeDataNode* node, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Cannot open file %s to read!\n", filename);
        return;
    }
    FILE* tempFile = fopen("temp_nhanvien.txt", "w");
    if (!tempFile) {
        printf("Cannot open temp file to write!\n");
        fclose(file);
        return;
    }
    char line[1024], employeeID[100];
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^|]", employeeID);
        if (strcmp(employeeID, node->PD.EmployeeID) == 0) {
            fprintf(tempFile, "%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%llu|%s|%.2f|%s|%s|%.2f|%s|%s|%.2f|%s|%lld|%d|%lld|%lld|%lld|%lld|%lld|%lld|%.2f|%lld|%s\n",
                node->PD.EmployeeID, node->PD.Name, node->PD.Birth, sexToString(node->PD.Gender),
                node->PD.NationalID, node->PD.TaxNumber, node->PD.PhoneNumber, node->PD.Email,
                contractTypeToString(node->EC.Type), node->EC.LaborDate, node->EC.Pos, node->EC.Dept, node->EC.DealtSalary,
                node->I.BHXH.Code, node->I.BHXH.PayLevel, node->I.BHXH.InsuranceDate,
                node->I.BHYT.Code, node->I.BHYT.PayLevel, node->I.BHYT.InsuranceDate,
                node->I.BHTN.Code, node->I.BHTN.PayLevel, node->I.BHTN.InsuranceDate,
                node->SM.BaseSalary, node->SM.WorkTimeThisMonth,
                node->SM.Allowance[0], node->SM.Allowance[1], node->SM.Allowance[2],
                node->SM.Bonus[0], node->SM.Bonus[1], node->SM.Bonus[2],
                node->SM.Tax, node->SM.FinalSalary, paymentToString(node->SM.Method));
            found = 1;
        } else {
            fprintf(tempFile, "%s", line);
        }
    }
    fclose(file);
    fclose(tempFile);
    if (found) {
        remove(filename);
        rename("temp_nhanvien.txt", filename);
        printf("Employee file updated successfully.\n");
    } else {
        remove("temp_nhanvien.txt");
        printf("Employee ID not found in file.\n");
    }
}

// Read data from file
void loadEmployeesFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("No existing employee file found. Starting fresh.\n");
        return;
    }
    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        struct EmployeeDataNode* newNode = (struct EmployeeDataNode*)malloc(sizeof(struct EmployeeDataNode));
        if (newNode == NULL) {
            printf("Memory allocation failed. Skipping record.\n");
            continue;
        }
        char genderStr[10], contractStr[20], bhxhCode[20], bhytCode[20], bhtnCode[20], paymentStr[20];
        sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%llu|%[^|]|%lf|%[^|]|%[^|]|%lf|%[^|]|%[^|]|%lf|%[^|]|%lld|%d|%lld|%lld|%lld|%lld|%lld|%lld|%lf|%lld|%[^|\n]",
               newNode->PD.EmployeeID, newNode->PD.Name, newNode->PD.Birth, genderStr,
               newNode->PD.NationalID, newNode->PD.TaxNumber, newNode->PD.PhoneNumber, newNode->PD.Email,
               contractStr, newNode->EC.LaborDate, newNode->EC.Pos, newNode->EC.Dept, &newNode->EC.DealtSalary,
               bhxhCode, &newNode->I.BHXH.PayLevel, newNode->I.BHXH.InsuranceDate,
               bhytCode, &newNode->I.BHYT.PayLevel, newNode->I.BHYT.InsuranceDate,
               bhtnCode, &newNode->I.BHTN.PayLevel, newNode->I.BHTN.InsuranceDate,
               &newNode->SM.BaseSalary, &newNode->SM.WorkTimeThisMonth,
               &newNode->SM.Allowance[0], &newNode->SM.Allowance[1], &newNode->SM.Allowance[2],
               &newNode->SM.Bonus[0], &newNode->SM.Bonus[1], &newNode->SM.Bonus[2],
               &newNode->SM.Tax, &newNode->SM.FinalSalary, paymentStr);
        newNode->PD.Gender = strcmp(genderStr, "Male") == 0 ? Male : Female;
        newNode->EC.Type = strcmp(contractStr, "Probation") == 0 ? Probation :
                           strcmp(contractStr, "Definite Term") == 0 ? DefiniteTerm : IndefiniteTerm;
        strcpy(newNode->I.BHXH.Code, bhxhCode);
        strcpy(newNode->I.BHYT.Code, bhytCode);
        strcpy(newNode->I.BHTN.Code, bhtnCode);
        newNode->SM.Method = strcmp(paymentStr, "Cash") == 0 ? Cash : BankTransfer;
        newNode->next = EDNode;
        EDNode = newNode;
    }
    fclose(file);
    printf("Employee data loaded successfully from %s\n", filename);
}

// WorkTime
void saveWorkTimeToFile(const WorkTime* wt, const char* filename) {
    FILE* file = fopen(filename, "a");
    if (!file) {
        printf("Cannot open file to write!\n");
        return;
    }
    fprintf(file, "%s|%s|%s|%d\n",
        wt->EmployeeID, wt->CheckIn, wt->CheckOut, wt->Overtime);
    fclose(file);
}

void loadWorkTimeFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("No existing work time file found. Starting fresh.\n");
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        struct WorkTimeNode* newNode = (struct WorkTimeNode*)malloc(sizeof(struct WorkTimeNode));
        if (newNode == NULL) {
            printf("Memory allocation failed. Skipping record.\n");
            continue;
        }
        sscanf(line, "%[^|]|%[^|]|%[^|]|%d",
            newNode->WT.EmployeeID, newNode->WT.CheckIn, newNode->WT.CheckOut, &newNode->WT.Overtime);
        newNode->next = WTNode;
        WTNode = newNode;
    }
    fclose(file);
    printf("Work time data loaded successfully from %s\n", filename);
}

void updateWorkTimeFile(const WorkTime* wt, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Cannot open file to read!\n");
        return;
    }
    FILE* tempFile = fopen("temp_worktime.txt", "w");
    if (!tempFile) {
        printf("Cannot open temp file to write!\n");
        fclose(file);
        return;
    }
    char line[256], employeeID[100];
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^|]", employeeID);
        if (strcmp(employeeID, wt->EmployeeID) == 0) {
            fprintf(tempFile, "%s|%s|%s|%d\n",
                wt->EmployeeID, wt->CheckIn, wt->CheckOut, wt->Overtime);
            found = 1;
        } else {
            fprintf(tempFile, "%s", line);
        }
    }
    fclose(file);
    fclose(tempFile);
    if (found) {
        remove(filename);
        rename("temp_worktime.txt", filename);
        printf("Work time file updated successfully.\n");
    } else {
        remove("temp_worktime.txt");
        printf("Employee ID not found in work time file.\n");
    }
}

// DayOff
void saveDayOffToFile(const DayOff* doff, const char* filename) {
    FILE* file = fopen(filename, "a");
    if (!file) {
        printf("Cannot open file to write!\n");
        return;
    }
    fprintf(file, "%s|%s|%s|%s\n",
        doff->EmployeeID, letterToString(doff->AbsenceLetter),
        doff->AbsenceDate, astatusToString(doff->letterstatus));
    fclose(file);
}

void loadDayOffFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("No existing day off file found. Starting fresh.\n");
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        struct DayOffNode* newNode = (struct DayOffNode*)malloc(sizeof(struct DayOffNode));
        if (newNode == NULL) {
            printf("Memory allocation failed. Skipping record.\n");
            continue;
        }
        char letterStr[10], statusStr[20];
        sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]",
            newNode->DOff.EmployeeID, letterStr, newNode->DOff.AbsenceDate, statusStr);
        newNode->DOff.AbsenceLetter = strcmp(letterStr, "Yes") == 0 ? Yes : No;
        if (strcmp(statusStr, "Pending") == 0) newNode->DOff.letterstatus = Pending;
        else if (strcmp(statusStr, "Approved") == 0) newNode->DOff.letterstatus = Approved;
        else newNode->DOff.letterstatus = Rejected;
        newNode->next = DONode;
        DONode = newNode;
    }
    fclose(file);
    printf("Day off data loaded successfully from %s\n", filename);
}

void updateDayOffFile(const DayOff* doff, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Cannot open file to read!\n");
        return;
    }
    FILE* tempFile = fopen("temp_dayoff.txt", "w");
    if (!tempFile) {
        printf("Cannot open temp file to write!\n");
        fclose(file);
        return;
    }
    char line[256], employeeID[100];
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^|]", employeeID);
        if (strcmp(employeeID, doff->EmployeeID) == 0) {
            fprintf(tempFile, "%s|%s|%s|%s\n",
                doff->EmployeeID, letterToString(doff->AbsenceLetter),
                doff->AbsenceDate, astatusToString(doff->letterstatus));
            found = 1;
        } else {
            fprintf(tempFile, "%s", line);
        }
    }
    fclose(file);
    fclose(tempFile);
    if (found) {
        remove(filename);
        rename("temp_dayoff.txt", filename);
        printf("Day off file updated successfully.\n");
    } else {
        remove("temp_dayoff.txt");
        printf("Employee ID not found in day off file.\n");
    }
}

// Education
void saveEducationToFile(const Education* e, const char* filename) {
    FILE* file = fopen(filename, "a");
    if (!file) {
        printf("Cannot open file to write!\n");
        return;
    }
    fprintf(file, "%s|%s|%d|%lld|%s\n",
        e->EmployeeID, e->CourseName, e->CourseTime, e->CourseFee,
        cstatusToString(e->coursestatus));
    fclose(file);
}

void loadEducationFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("No existing education file found. Starting fresh.\n");
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        struct EducationNode* newNode = (struct EducationNode*)malloc(sizeof(struct EducationNode));
        if (newNode == NULL) {
            printf("Memory allocation failed. Skipping record.\n");
            continue;
        }
        char statusStr[20];
        sscanf(line, "%[^|]|%[^|]|%d|%lld|%[^|]",
            newNode->E.EmployeeID, newNode->E.CourseName, &newNode->E.CourseTime,
            &newNode->E.CourseFee, statusStr);
        if (strcmp(statusStr, "Failed") == 0) newNode->E.coursestatus = Failed;
        else if (strcmp(statusStr, "Ongoing") == 0) newNode->E.coursestatus = Ongoing;
        else newNode->E.coursestatus = Passed;
        newNode->next = ENode;
        ENode = newNode;
    }
    fclose(file);
    printf("Education data loaded successfully from %s\n", filename);
}

void updateEducationFile(const Education* e, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Cannot open file to read!\n");
        return;
    }
    FILE* tempFile = fopen("temp_education.txt", "w");
    if (!tempFile) {
        printf("Cannot open temp file to write!\n");
        fclose(file);
        return;
    }
    char line[256], employeeID[100];
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^|]", employeeID);
        if (strcmp(employeeID, e->EmployeeID) == 0) {
            fprintf(tempFile, "%s|%s|%d|%lld|%s\n",
                e->EmployeeID, e->CourseName, e->CourseTime, e->CourseFee,
                cstatusToString(e->coursestatus));
            found = 1;
        } else {
            fprintf(tempFile, "%s", line);
        }
    }
    fclose(file);
    fclose(tempFile);
    if (found) {
        remove(filename);
        rename("temp_education.txt", filename);
        printf("Education file updated successfully.\n");
    } else {
        remove("temp_education.txt");
        printf("Employee ID not found in education file.\n");
    }
}

// LayOff
void saveLayOffToFile(const LayOff* lo, const char* filename) {
    FILE* file = fopen(filename, "a");
    if (!file) {
        printf("Cannot open file to write!\n");
        return;
    }
    fprintf(file, "%s|%s|%s\n",
        lo->EmployeeID, lo->Date, layoffreasonToString(lo->Reason));
    fclose(file);
}

void loadLayOffFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("No existing layoff file found. Starting fresh.\n");
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        struct LayOffNode* newNode = (struct LayOffNode*)malloc(sizeof(struct LayOffNode));
        if (newNode == NULL) {
            printf("Memory allocation failed. Skipping record.\n");
            continue;
        }
        char reasonStr[20];
        sscanf(line, "%[^|]|%[^|]|%[^|]",
            newNode->LO.EmployeeID, newNode->LO.Date, reasonStr);
        if (strcmp(reasonStr, "Volunteered") == 0) newNode->LO.Reason = Volunteered;
        else if (strcmp(reasonStr, "Fired") == 0) newNode->LO.Reason = Fired;
        else newNode->LO.Reason = ContractEnded;
        newNode->next = LONode;
        LONode = newNode;
    }
    fclose(file);
    printf("Layoff data loaded successfully from %s\n", filename);
}

void updateLayOffFile(const LayOff* lo, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Cannot open file to read!\n");
        return;
    }
    FILE* tempFile = fopen("temp_layoff.txt", "w");
    if (!tempFile) {
        printf("Cannot open temp file to write!\n");
        fclose(file);
        return;
    }
    char line[256], employeeID[100];
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^|]", employeeID);
        if (strcmp(employeeID, lo->EmployeeID) == 0) {
            fprintf(tempFile, "%s|%s|%s\n",
                lo->EmployeeID, lo->Date, layoffreasonToString(lo->Reason));
            found = 1;
        } else {
            fprintf(tempFile, "%s", line);
        }
    }
    fclose(file);
    fclose(tempFile);
    if (found) {
        remove(filename);
        rename("temp_layoff.txt", filename);
        printf("Layoff file updated successfully.\n");
    } else {
        remove("temp_layoff.txt");
        printf("Employee ID not found in layoff file.\n");
    }
}

void generateEmployeeID(char* newID, const char* filename) {
    int lastID = getLastEmployeeID_FromFile(filename);
    sprintf(newID, "NV%04d", lastID + 1);
}

void EmployeeDataInsert(PersonalData pd, EmploymentContract ec, Insurance in, SalaryManaging sm) {
    struct EmployeeDataNode* newNode = (struct EmployeeDataNode*)malloc(sizeof(struct EmployeeDataNode));
    if (newNode == NULL) {
        printf("DATA INSERTED UNSUCCESSFULLY!\n");
        return;
    }
    newNode->PD = pd;
    newNode->EC = ec;
    newNode->I = in;
    newNode->SM = sm;
    newNode->SM.FinalSalary = FinalSalaryCalculation(sm.BaseSalary, sm.WorkTimeThisMonth, sm.Allowance, sm.Bonus, sm.Tax);
    newNode->next = EDNode;
    EDNode = newNode;
    saveEmployeeToFile(newNode, "dulieu_nhanvien.txt");
    printf("DATA INSERTED SUCCESSFULLY!\n");
}

void EmployeeDataInput(PersonalData *pd, EmploymentContract *ec, Insurance *in, SalaryManaging *sm) {
    printf("\n--- ENTER PERSONAL DATA ---\n");

    generateEmployeeID(pd->EmployeeID, "dulieu_nhanvien.txt");
    printf("Generated Employee ID: %s\n", pd->EmployeeID);
    getStringInput(pd->Name, sizeof(pd->Name), "Enter Name: ");
    clear_input_buffer(); 
    getStringInput(pd->Birth, sizeof(pd->Birth), "Enter Birth Date (dd/mm/yy): ");

    int genderChoice;
    printf("Enter Gender (0 for Male, 1 for Female): ");
    scanf("%d", &genderChoice);
    clear_input_buffer();
    pd->Gender = (genderChoice == 0) ? Male : Female;

    getStringInput(pd->NationalID, sizeof(pd->NationalID), "Enter National ID: ");
    getStringInput(pd->TaxNumber, sizeof(pd->TaxNumber), "Enter Tax Number: ");
    getStringInput(pd->PhoneNumber, sizeof(pd->PhoneNumber), "Enter Phone Number: ");
    getStringInput(pd->Email, sizeof(pd->Email), "Enter Email: ");
    printf("Personal data input complete.\n");

    printf("\n--- ENTER EMPLOYMENT CONTRACT DATA ---\n");
    int contractTypeChoice;
    printf("Enter Contract Type (0: Probation, 1: Definite Term, 2: Indefinite Term): ");
    scanf("%d", &contractTypeChoice);
    clear_input_buffer();
    ec->Type = (ContractType)contractTypeChoice;

    getStringInput(ec->LaborDate, sizeof(ec->LaborDate), "Enter Labor Date (dd/mm/yy - dd/mm/yy): ");
    getStringInput(ec->Pos, sizeof(ec->Pos), "Enter Position: ");
    getStringInput(ec->Dept, sizeof(ec->Dept), "Enter Department: ");
    printf("Enter Dealt Salary: ");
    scanf("%llu", &ec->DealtSalary);
    clear_input_buffer();

    printf("\n--- ENTER INSURANCE DATA ---\n");
    printf("\nSocial Insurance:\n");
    getStringInput(in->BHXH.Code, sizeof(in->BHXH.Code), "Enter Social Insurance Code: ");
    printf("Enter Social Insurance Payment Ratio (e.g., input 0.08 for 8%%): ");
    scanf("%lf", &in->BHXH.PayLevel);
    clear_input_buffer();
    getStringInput(in->BHXH.InsuranceDate, sizeof(in->BHXH.InsuranceDate), "Enter Social Insurance Date (dd/mm/yy - dd/mm/yy): ");

    printf("\nHealth Insurance:\n");
    getStringInput(in->BHYT.Code, sizeof(in->BHYT.Code), "Enter Health Insurance Code: ");
    printf("Enter Health Insurance Payment Ratio (e.g., input 0.015 for 1.5%%): ");
    scanf("%lf", &in->BHYT.PayLevel);
    clear_input_buffer();
    getStringInput(in->BHYT.InsuranceDate, sizeof(in->BHYT.InsuranceDate), "Enter Health Insurance Date (dd/mm/yy - dd/mm/yy): ");

    printf("\nUnemployment Insurance:\n");
    getStringInput(in->BHTN.Code, sizeof(in->BHTN.Code), "Enter Unemployment Insurance Code: ");
    printf("Enter Unemployment Insurance Payment Ratio (e.g., input 0.01 for 1%%): ");
    scanf("%lf", &in->BHTN.PayLevel);
    clear_input_buffer();
    getStringInput(in->BHTN.InsuranceDate, sizeof(in->BHTN.InsuranceDate), "Enter Unemployment Insurance Date (dd/mm/yy - dd/mm/yy): ");

    printf("\n--- ENTER SALARY MANAGING DATA ---\n");
    sm->BaseSalary = ec->DealtSalary;

    printf("Enter Work Time This Month (in hours): ");
    scanf("%d", &sm->WorkTimeThisMonth);
    clear_input_buffer();

    printf("Enter Lunch Allowance: ");
    scanf("%lld", &sm->Allowance[0]);
    clear_input_buffer();
    printf("Enter Transportation Allowance: ");
    scanf("%lld", &sm->Allowance[1]);
    clear_input_buffer();
    printf("Enter Residence Allowance: ");
    scanf("%lld", &sm->Allowance[2]);
    clear_input_buffer();

    printf("Enter KPIs Bonus: ");
    scanf("%lld", &sm->Bonus[0]);
    clear_input_buffer();
    printf("Enter Holidays Bonus: ");
    scanf("%lld", &sm->Bonus[1]);
    clear_input_buffer();
    printf("Enter Projects Bonus: ");
    scanf("%lld", &sm->Bonus[2]);
    clear_input_buffer();

    printf("Enter Tax Rate (e.g., input 0.1 for 10%%): ");
    scanf("%lf", &sm->Tax);
    clear_input_buffer();

    int paymentMethodChoice;
    printf("Enter Payment Method (0 for Cash, 1 for Bank Transfer): ");
    scanf("%d", &paymentMethodChoice);
    clear_input_buffer();
    sm->Method = (Payment)paymentMethodChoice;

    sm->FinalSalary = FinalSalaryCalculation(sm->BaseSalary, sm->WorkTimeThisMonth, sm->Allowance, sm->Bonus, sm->Tax);
    printf("\nEmployee data input complete.\n");
}

void WorkTimeInsert(WorkTime wt){
    struct WorkTimeNode* newNode = (struct WorkTimeNode*)malloc(sizeof(struct WorkTimeNode));
    if (newNode == NULL) printf("DATA INSERTED UNSUCCESSFULLY!\n");
    else{
        newNode->WT = wt;
        newNode->next = WTNode;
        WTNode = newNode;
        saveWorkTimeToFile(&wt, "worktime.txt");
        printf("DATA INSERTED SUCCESSFULLY!\n");
    }
}

void WorkTimeInput(WorkTime *wt){
    printf("\n--- ENTER WORKING TIME ---\n");
    getStringInput(wt->EmployeeID, sizeof(wt->EmployeeID), "Enter Employee ID: ");
    getStringInput(wt->CheckIn, sizeof(wt->CheckIn), "Enter Checkin Time (xx:yy): ");
    getStringInput(wt->CheckOut, sizeof(wt->CheckOut), "Enter Checkout Time (xx:yy): ");

    int ovt;
    printf("Enter Overtime Hours: ");
    scanf("%d", &ovt);
    clear_input_buffer();
    wt->Overtime = ovt;
}

void DayOffInsert(DayOff doff){
    struct DayOffNode* newNode = (struct DayOffNode*)malloc(sizeof(struct DayOffNode));
    if (newNode == NULL) printf("DATA INSERTED UNSUCCESSFULLY!\n");
    else{
        newNode->DOff = doff;
        newNode->next = DONode;
        DONode = newNode;
        saveDayOffToFile(&doff, "dayoff.txt");
        printf("DATA INSERTED SUCCESSFULLY!\n");
    }
}

void DayOffInput(DayOff *doff){
    printf("\n--- ENTER DAY OFF DATA ---\n");
    getStringInput(doff->EmployeeID, sizeof(doff->EmployeeID), "Enter Employee ID: ");

    int absenceletteryesno;
    printf("Yes/No Absence Letter (0: No, 1: Yes): ");
    scanf("%d", &absenceletteryesno);
    clear_input_buffer();
    doff->AbsenceLetter = (Letter)absenceletteryesno;

    getStringInput(doff->AbsenceDate, sizeof(doff->AbsenceDate), "Enter Absence (dd/mm/yy - dd/mm/yy): ");

    int absenceletterstatus;
    printf("Enter Letter Status (0: Pending, 1: Approved, 2: Rejected): ");
    scanf("%d", &absenceletterstatus);
    clear_input_buffer();
    doff->letterstatus = (LetterStatus)absenceletterstatus;
}

void EducationInsert(Education e){
    struct EducationNode* newNode = (struct EducationNode*)malloc(sizeof(struct EducationNode));
    if (newNode == NULL) {
        printf("DATA INSERTED UNSUCCESSFULLY!\n");
        return;
    }
    newNode->E = e;
    newNode->next = ENode;
    ENode = newNode;
    saveEducationToFile(&e, "education.txt");
    printf("DATA INSERTED SUCCESSFULLY!\n");
    
}

void EducationInput(Education *e){
    printf("\n--- ENTER EDUCATION DATA ---\n");
    getStringInput(e->EmployeeID, sizeof(e->EmployeeID), "Enter Employee ID: ");
    getStringInput(e->CourseName, sizeof(e->CourseName), "Enter Course Name: ");

    printf("Enter Course Time (hrs): ");
    scanf("%d", &e->CourseTime);
    clear_input_buffer();

    printf("Enter Course Fee: ");
    scanf("%lld", &e->CourseFee);
    clear_input_buffer();

    int coursestatus;
    printf("Enter Course Status (0: Failed, 1: Ongoing, 2: Passed):");
    scanf("%d", &coursestatus);
    clear_input_buffer();
    e->coursestatus = (CourseStatus)coursestatus;
}

void LayOffInsert(LayOff lo){
    struct LayOffNode* newNode = (struct LayOffNode*)malloc(sizeof(struct LayOffNode));
    if (newNode == NULL) printf("DATA INSERTED UNSUCCESSFULLY!\n");
    else{
        newNode->LO = lo;
        newNode->next = LONode;
        LONode = newNode;
        saveLayOffToFile(&lo, "layoff.txt");
        printf("DATA INSERTED SUCCESSFULLY!\n");
    }
}

void LayOffInput(LayOff *lo){
    printf("\n--- ENTER LAY OFF DATA ---\n");
    getStringInput(lo->EmployeeID, sizeof(lo->EmployeeID), "Enter Employee ID: ");
    getStringInput(lo->Date, sizeof(lo->Date), "Enter Layoff Date: ");

    int rson;
    printf("Enter Layoff Reason (0: Volunteered, 1: Fired, 2: Contract Ended):");
    scanf("%d", &rson);
    clear_input_buffer();
    lo->Reason = (LayOffReason)rson;
}

void EmployeeDataTraverse(const char* employeeID){
    printf("\nDISPLAYING EMPLOYEE DATA\n");
    struct EmployeeDataNode* index = EDNode;
    int found = 0;
    while (index != NULL) {
        if (employeeID == NULL || strcmp(index->PD.EmployeeID, employeeID) == 0) {
            found = 1;
            printf("\nEMPLOYEE:\n");
            printf("Name: %s\n", index->PD.Name);
            printf("Employee ID: %s\n", index->PD.EmployeeID);
            printf("Birth: %s\n", index->PD.Birth);
            printf("Gender: %s\n", sexToString(index->PD.Gender));
            printf("National ID: %s\n", index->PD.NationalID);
            printf("Tax Number: %s\n", index->PD.TaxNumber);
            printf("Phone: %s\n", index->PD.PhoneNumber);
            printf("Email: %s\n", index->PD.Email);
            printf("Contract: %s\n", contractTypeToString(index->EC.Type));
            printf("Labor Date: %s\n", index->EC.LaborDate);
            printf("Position: %s\n", index->EC.Pos);
            printf("Department: %s\n", index->EC.Dept);
            printf("Dealt Salary: %llu\n", index->EC.DealtSalary);
            printf("Social Insurance: Code: %s, Ratio: %.2f%%, Date: %s\n",
                index->I.BHXH.Code, index->I.BHXH.PayLevel * 100, index->I.BHXH.InsuranceDate);
            printf("Health Insurance: Code: %s, Ratio: %.2f%%, Date: %s\n",
                index->I.BHYT.Code, index->I.BHYT.PayLevel * 100, index->I.BHYT.InsuranceDate);
            printf("Unemployment Insurance: Code: %s, Ratio: %.2f%%, Date: %s\n",
                index->I.BHTN.Code, index->I.BHTN.PayLevel * 100, index->I.BHTN.InsuranceDate);
            printf("Salary: %lld\n", index->SM.FinalSalary);
            printf("Payment Method: %s\n", paymentToString(index->SM.Method));
        }
        index = index->next;
    }
    if (!found && employeeID != NULL) printf("No employee found with ID: %s\n", employeeID);
}

void WorkTimeTraverse(const char* employeeID){
    printf("\nDISPLAYING TRACKING DATA\n");
    struct WorkTimeNode* index = WTNode;
    int found = 0, cnt = 1;
    while (index != NULL) {
    if (employeeID == NULL || strcmp(index->WT.EmployeeID, employeeID) == 0) {
            found = 1;
            printf("\nWORK TIME RECORD %03d:\n", cnt++);
            printf("Employee ID: %s\n", index->WT.EmployeeID);
            printf("Check-in: %s\n", index->WT.CheckIn);
            printf("Check-out: %s\n", index->WT.CheckOut);
            printf("Overtime: %d hours\n", index->WT.Overtime);
        }
        index = index->next;
    }
    if (!found && employeeID != NULL) printf("No tracking data found for ID: %s\n", employeeID);
}

void DayOffTraverse(const char* employeeID){
    printf("\nDISPLAYING DAY OFF DATA\n");
    struct DayOffNode* index = DONode;
    int found = 0, cnt = 1;
    while (index != NULL) {
        if (employeeID == NULL || strcmp(index->DOff.EmployeeID, employeeID) == 0) {
            found = 1;
            printf("\nDAY OFF RECORD %03d:\n", cnt++);
            printf("Employee ID: %s\n", index->DOff.EmployeeID);
            printf("Absence Letter: %s\n", letterToString(index->DOff.AbsenceLetter));
            printf("Absence Date: %s\n", index->DOff.AbsenceDate);
            printf("Letter Status: %s\n", astatusToString(index->DOff.letterstatus));
        }
        index = index->next;
    }
    if (!found && employeeID != NULL) printf("No day off data found for ID: %s\n", employeeID);
}

void EducationTraverse(const char* employeeID){
    printf("\nDISPLAYING EDUCATION DATA\n");
    struct EducationNode* index = ENode;
    int found = 0, cnt = 1;
    while (index != NULL) {
        if (employeeID == NULL || strcmp(index->E.EmployeeID, employeeID) == 0) {
            found = 1;
            printf("\nEDUCATION RECORD %03d:\n", cnt++);
            printf("Employee ID: %s\n", index->E.EmployeeID);
            printf("Course Name: %s\n", index->E.CourseName);
            printf("Course Time: %d hours\n", index->E.CourseTime);
            printf("Course Fee: %lld\n", index->E.CourseFee);
            printf("Course Status: %s\n", cstatusToString(index->E.coursestatus));
        }
        index = index->next;
    }
    if (!found && employeeID != NULL) printf("No education data found for ID: %s\n", employeeID);
}

void LayOffTraverse(const char* employeeID){
    printf("\nDISPLAYING LAYOFF DATA\n");
    struct LayOffNode* index = LONode;
    int found = 0, cnt = 1;
    while (index != NULL) {
        if (employeeID == NULL || strcmp(index->LO.EmployeeID, employeeID) == 0) {
            found = 1;
            printf("\nLAYOFF RECORD %03d:\n", cnt++);
            printf("Employee ID: %s\n", index->LO.EmployeeID);
            printf("Date: %s\n", index->LO.Date);
            printf("Reason: %s\n", layoffreasonToString(index->LO.Reason));
        }
        index = index->next;
    }
    if (!found && employeeID != NULL) printf("No layoff data found for ID: %s\n", employeeID);
}

//Update functions
void updatePersonalData() {
    char employeeID[100];
    printf("\n--- UPDATE PERSONAL DATA ---\n");
    getStringInput(employeeID, sizeof(employeeID), "Enter Employee ID to update: ");
    
    struct EmployeeDataNode* current = EDNode;
    while (current != NULL && strcmp(current->PD.EmployeeID, employeeID) != 0) {
        current = current->next;
    }

    if (current == NULL) {
        printf("No employee record found for Employee ID: %s\n", employeeID);
        return;
    }

    printf("\nFound Employee Record:\n");
    EmployeeDataTraverse(employeeID);

    int choice;
    do {
        printf("\nSelect field to update:\n");
        printf("1. Name\n");
        printf("2. Birth Date\n");
        printf("3. Gender\n");
        printf("4. National ID\n");
        printf("5. Tax Number\n");
        printf("6. Phone Number\n");
        printf("7. Email\n");
        printf("8. Finish updating\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clear_input_buffer();

        switch (choice) {
            case 1:
                getStringInput(current->PD.Name, sizeof(current->PD.Name), "Enter new Name: ");
                printf("Name updated successfully.\n");
                break;
            case 2:
                getStringInput(current->PD.Birth, sizeof(current->PD.Birth), "Enter new Birth Date (dd/mm/yy): ");
                printf("Birth Date updated successfully.\n");
                break;
            case 3:
                printf("Enter new Gender (0: Male, 1: Female): ");
                scanf("%d", &choice);
                clear_input_buffer();
                current->PD.Gender = (choice == 0) ? Male : Female;
                printf("Gender updated successfully.\n");
                break;
            case 4:
                getStringInput(current->PD.NationalID, sizeof(current->PD.NationalID), "Enter new National ID: ");
                printf("National ID updated successfully.\n");
                break;
            case 5:
                getStringInput(current->PD.TaxNumber, sizeof(current->PD.TaxNumber), "Enter new Tax Number: ");
                printf("Tax Number updated successfully.\n");
                break;
            case 6:
                getStringInput(current->PD.PhoneNumber, sizeof(current->PD.PhoneNumber), "Enter new Phone Number: ");
                printf("Phone Number updated successfully.\n");
                break;
            case 7:
                getStringInput(current->PD.Email, sizeof(current->PD.Email), "Enter new Email: ");
                printf("Email updated successfully.\n");
                break;
            case 8:
                printf("Finished updating personal data.\n");
                updateEmployeeFile(current, "dulieu_nhanvien.txt");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 8);
}

void updateDayOffData() {
    char id[100];
    printf("\n--- UPDATE DAY OFF DATA ---\n");
    getStringInput(id, sizeof(id), "Enter Employee ID to update: ");
    
    struct DayOffNode* current = DONode;
    while (current != NULL && strcmp(current->DOff.EmployeeID, id) != 0) {
        current = current->next;
    }

    if (current == NULL) {
        printf("No day off record found for Employee ID: %s\n", id);
        return;
    }

    printf("\nFound Day Off Record:\n");
    DayOffTraverse(id);

    int choice;
    do {
        printf("\nSelect field to update:\n");
        printf("1. Absence Letter\n");
        printf("2. Absence Date\n");
        printf("3. Letter Status\n");
        printf("4. Finish updating\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clear_input_buffer();

        switch (choice) {
            case 1:
                printf("Yes/No Absence Letter (0: No, 1: Yes): ");
                scanf("%d", &choice);
                clear_input_buffer();
                current->DOff.AbsenceLetter = (Letter)choice;
                printf("Absence Letter updated successfully.\n");
                break;
            case 2:
                getStringInput(current->DOff.AbsenceDate, sizeof(current->DOff.AbsenceDate), "Enter new Absence Date (dd/mm/yy - dd/mm/yy): ");
                printf("Absence Date updated successfully.\n");
                break;
            case 3:
                printf("Enter Letter Status (0: Pending, 1: Approved, 2: Rejected): ");
                scanf("%d", &choice);
                clear_input_buffer();
                current->DOff.letterstatus = (LetterStatus)choice;
                printf("Letter Status updated successfully.\n");
                break;
            case 4:
                printf("Finished updating day off data.\n");
                updateDayOffFile(&current->DOff, "dayoff.txt");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 4);
}

void updateCourseData() {
    char id[100];
    printf("\n--- UPDATE EDUCATION DATA ---\n");
    getStringInput(id, sizeof(id), "Enter Employee ID to update: ");
    
    struct EducationNode* current = ENode;
    while (current != NULL && strcmp(current->E.EmployeeID, id) != 0) {
        current = current->next;
    }

    if (current == NULL) {
        printf("No education record found for Employee ID: %s\n", id);
        return;
    }

    printf("\nFound Education Record:\n");
    EducationTraverse(id);

    int choice;
    do {
        printf("\nSelect field to update:\n");
        printf("1. Course Name\n");
        printf("2. Course Time\n");
        printf("3. Course Fee\n");
        printf("4. Course Status\n");
        printf("5. Finish updating\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clear_input_buffer();

        switch (choice) {
            case 1:
                getStringInput(current->E.CourseName, sizeof(current->E.CourseName), "Enter new Course Name: ");
                printf("Course Name updated successfully.\n");
                break;
            case 2:
                printf("Enter new Course Time (hrs): ");
                scanf("%d", &current->E.CourseTime);
                clear_input_buffer();
                printf("Course Time updated successfully.\n");
                break;
            case 3:
                printf("Enter new Course Fee: ");
                scanf("%lld", &current->E.CourseFee);
                clear_input_buffer();
                printf("Course Fee updated successfully.\n");
                break;
            case 4:
                printf("Enter Course Status (0: Failed, 1: Ongoing, 2: Passed): ");
                scanf("%d", &choice);
                clear_input_buffer();
                current->E.coursestatus = (CourseStatus)choice;
                printf("Course Status updated successfully.\n");
                break;
            case 5:
                printf("Finished updating education data.\n");
                updateEducationFile(&current->E, "education.txt");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);
}

void updateTrackingData() {
    char id[100];
    printf("\n--- UPDATE TRACKING DATA ---\n");
    getStringInput(id, sizeof(id), "Enter Employee ID to update: ");
    
    struct WorkTimeNode* current = WTNode;
    while (current != NULL && strcmp(current->WT.EmployeeID, id) != 0) {
        current = current->next;
    }

    if (current == NULL) {
        printf("No tracking record found for Employee ID: %s\n", id);
        return;
    }

    printf("\nFound Tracking Record:\n");
    WorkTimeTraverse(id);

    int choice;
    do {
        printf("\nSelect field to update:\n");
        printf("1. Check-in Time\n");
        printf("2. Check-out Time\n");
        printf("3. Overtime Hours\n");
        printf("4. Finish updating\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clear_input_buffer();

        switch (choice) {
            case 1:
                getStringInput(current->WT.CheckIn, sizeof(current->WT.CheckIn), "Enter new Check-in Time (xx:yy): ");
                printf("Check-in Time updated successfully.\n");
                break;
            case 2:
                getStringInput(current->WT.CheckOut, sizeof(current->WT.CheckOut), "Enter new Check-out Time (xx:yy): ");
                printf("Check-out Time updated successfully.\n");
                break;
            case 3:
                printf("Enter new Overtime Hours: ");
                scanf("%d", &current->WT.Overtime);
                clear_input_buffer();
                printf("Overtime Hours updated successfully.\n");
                break;
            case 4:
                printf("Finished updating tracking data.\n");
                updateWorkTimeFile(&current->WT, "worktime.txt");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 4);
}


struct EmployeeDataNode* findEmployeeData(const char* employeeID) {
    printf("\n--- SEARCHING FOR EMPLOYEE DATA (ID: %s) ---\n", employeeID);
    
    struct EmployeeDataNode* currentED = EDNode;
    while (currentED != NULL) {
        if (strcmp(currentED->PD.EmployeeID, employeeID) == 0) {
            printf("\n--- Employee Data Found! ---\n");
            return currentED; 
        }
        currentED = currentED->next;
    }
    
    printf("No data found in Employee Data for Employee ID: %s\n", employeeID);
    return NULL; // Return NULL if no data is found
}

struct WorkTimeNode* findWorkTimeData(const char* employeeID) {
    printf("\n--- SEARCHING FOR EMPLOYEE DATA (ID: %s) ---\n", employeeID);
    
    struct WorkTimeNode* currentWT = WTNode;
    while (currentWT != NULL) {
        if (strcmp(currentWT->WT.EmployeeID, employeeID) == 0) {
            printf("\n--- Employee Data Found! ---\n");
            return currentWT; 
        }
        currentWT = currentWT->next;
    }
    
    printf("No data found in Work Time Tracking Data for Employee ID: %s\n", employeeID);
    return NULL; // Return NULL if no data is found
}

struct DayOffNode* findDayOffData(const char* employeeID) {
    printf("\n--- SEARCHING FOR EMPLOYEE DATA (ID: %s) ---\n", employeeID);
    
    struct DayOffNode* currentDOff = DONode;
    while (currentDOff != NULL) {
        if (strcmp(currentDOff->DOff.EmployeeID, employeeID) == 0) {
            printf("\n--- Employee Data Found! ---\n");
            return currentDOff; 
        }
        currentDOff = currentDOff->next;
    }
    
    printf("No data found in Day Off Data for Employee ID: %s\n", employeeID);
    return NULL; // Return NULL if no data is found
}

struct EducationNode* findEducationData(const char* employeeID) {
    printf("\n--- SEARCHING FOR EMPLOYEE DATA (ID: %s) ---\n", employeeID);
    
    struct EducationNode* currentE = ENode;
    while (currentE != NULL) {
        if (strcmp(currentE->E.EmployeeID, employeeID) == 0) {
            printf("\n--- Employee Data Found! ---\n");
            return currentE; 
        }
        currentE = currentE->next;
    }
    
    printf("No data found in Education Data for Employee ID: %s\n", employeeID);
    return NULL; // Return NULL if no data is found
}

struct LayOffNode* findLayOffData(const char* employeeID) {
    printf("\n--- SEARCHING FOR EMPLOYEE DATA (ID: %s) ---\n", employeeID);
    
    struct LayOffNode* currentLO = LONode;
    while (currentLO != NULL) {
        if (strcmp(currentLO->LO.EmployeeID, employeeID) == 0) {
            printf("\n--- Employee Data Found! ---\n");
            return currentLO; 
        }
        currentLO = currentLO->next;
    }
    
    printf("No data found in Lay Off Data for Employee ID: %s\n", employeeID);
    return NULL; // Return NULL if no data is found
}

void deleteEmployee(const char* employeeID) {
    struct EmployeeDataNode* current = EDNode;
    struct EmployeeDataNode* prev = NULL;
    while (current != NULL && strcmp(current->PD.EmployeeID, employeeID) != 0) {
        prev = current;
        current = current->next;
    }
    if (current == NULL) {
        printf("No employee found with ID: %s\n", employeeID);
        return;
    }
    if (prev == NULL) {
        EDNode = current->next;
    } else {
        prev->next = current->next;
    }
    free(current);
    printf("Employee %s deleted from memory.\n", employeeID);

    // Xóa khỏi dulieu_nhanvien.txt
    FILE* file = fopen("dulieu_nhanvien.txt", "r");
    if (!file) {
        printf("Cannot open employee file to read!\n");
        return;
    }
    FILE* tempFile = fopen("temp_nhanvien.txt", "w");
    if (!tempFile) {
        printf("Cannot open temp file to write!\n");
        fclose(file);
        return;
    }
    char line[1024], fileID[100];
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^|]", fileID);
        if (strcmp(fileID, employeeID) != 0) {
            fprintf(tempFile, "%s", line);
        } else {
            found = 1;
        }
    }
    fclose(tempFile);
    fclose(file);
    if (found) {
        remove("dulieu_nhanvien.txt");
        rename("temp_nhanvien.txt", "dulieu_nhanvien.txt");
        printf("Employee %s removed from employee file.\n");
    } else {
        remove("temp_nhanvien.txt");
        printf("Employee %s not found in employee file.\n", employeeID);
    }

    // Xóa khỏi worktime.txt
    file = fopen("worktime.txt", "r");
    if (file) {
        tempFile = fopen("temp_worktime.txt", "w");
        if (tempFile) {
            found = 0;
            while (fgets(line, sizeof(line), file)) {
                sscanf(line, "%[^|]", fileID);
                if (strcmp(fileID, employeeID) != 0) {
                    fprintf(tempFile, "%s", line);
                } else {
                    found = 1;
                }
            }
            fclose(tempFile);
            fclose(file);
            if (found) {
                remove("worktime.txt");
                rename("temp_worktime.txt", "worktime.txt");
                printf("Employee %s removed from work time file.\n");
            } else {
                remove("temp_worktime.txt");
            }
        } else {
            fclose(file);
        }
    }

    // Xóa khỏi dayoff.txt
    file = fopen("dayoff.txt", "r");
    if (file) {
        tempFile = fopen("temp_dayoff.txt", "w");
        if (tempFile) {
            found = 0;
            while (fgets(line, sizeof(line), file)) {
                sscanf(line, "%[^|]", fileID);
                if (strcmp(fileID, employeeID) != 0) {
                    fprintf(tempFile, "%s", line);
                } else {
                    found = 1;
                }
            }
            fclose(tempFile);
            fclose(file);
            if (found) {
                remove("dayoff.txt");
                rename("temp_dayoff.txt", "dayoff.txt");
                printf("Employee %s removed from day off file.\n");
            } else {
                remove("temp_dayoff.txt");
            }
        } else {
            fclose(file);
        }
    }

    // Xóa khỏi education.txt
    file = fopen("education.txt", "r");
    if (file) {
        tempFile = fopen("temp_education.txt", "w");
        if (tempFile) {
            found = 0;
            while (fgets(line, sizeof(line), file)) {
                sscanf(line, "%[^|]", fileID);
                if (strcmp(fileID, employeeID) != 0) {
                    fprintf(tempFile, "%s", line);
                } else {
                    found = 1;
                }
            }
            fclose(tempFile);
            fclose(file);
            if (found) {
                remove("education.txt");
                rename("temp_education.txt", "education.txt");
                printf("Employee %s removed from education file.\n");
            } else {
                remove("temp_education.txt");
            }
        } else {
            fclose(file);
        }
    }

    // Xóa khỏi layoff.txt
    file = fopen("layoff.txt", "r");
    if (file) {
        tempFile = fopen("temp_layoff.txt", "w");
        if (tempFile) {
            found = 0;
            while (fgets(line, sizeof(line), file)) {
                sscanf(line, "%[^|]", fileID);
                if (strcmp(fileID, employeeID) != 0) {
                    fprintf(tempFile, "%s", line);
                } else {
                    found = 1;
                }
            }
            fclose(tempFile);
            fclose(file);
            if (found) {
                remove("layoff.txt");
                rename("temp_layoff.txt", "layoff.txt");
                printf("Employee %s removed from layoff file.\n", employeeID);
            } else {
                remove("temp_layoff.txt");
            }
        } else {
            fclose(file);
        }
    }
}


void ReportPrint(){
    printf("\nGENERATING HR REPORT\n");
    HRReport report = {0};
    int employeeCount = 0, layoffCount = 0;
    long long totalSalary = 0;
    double totalInsuranceFee = 0;
    long long totalEducationFee = 0;

    // Calculate employee statistics
    struct EmployeeDataNode* emp = EDNode;
    while(emp != NULL) {
        employeeCount++;
        totalSalary += emp->SM.FinalSalary;
        totalInsuranceFee += (emp->I.BHXH.PayLevel + emp->I.BHYT.PayLevel + emp->I.BHTN.PayLevel) * emp->SM.BaseSalary;
        emp = emp->next;
    }

    // Calculate layoff statistics
    struct LayOffNode* lo = LONode;
    while(lo != NULL) {
        layoffCount++;
        lo = lo->next;
    }

    // Calculate education fees
    struct EducationNode* edu = ENode;
    while(edu != NULL) {
        totalEducationFee += edu->E.CourseFee;
        edu = edu->next;
    }

    report.LayOffRatio = employeeCount ? (double)layoffCount / employeeCount : 0;
    report.SalaryFee = totalSalary;
    report.InsuranceFee = totalInsuranceFee;
    report.EducationFee = totalEducationFee;

    printf("Total Employees: %d\n", employeeCount);
    printf("Layoff Ratio: %.2f%%\n", report.LayOffRatio * 100);
    printf("Total Salary Expenses: %lld\n", report.SalaryFee);
    printf("Total Insurance Fees: %.2f\n", report.InsuranceFee);
    printf("Total Education Fees: %lld\n", report.EducationFee);
}

void freeAllLists() {
    struct EmployeeDataNode* emp = EDNode;
    while(emp != NULL) {
        struct EmployeeDataNode* temp = emp;
        emp = emp->next;
        free(temp);
    }
    
    struct WorkTimeNode* wt = WTNode;
    while(wt != NULL) {
        struct WorkTimeNode* temp = wt;
        wt = wt->next;
        free(temp);
    }
    
    struct DayOffNode* doff = DONode;
    while(doff != NULL) {
        struct DayOffNode* temp = doff;
        doff = doff->next;
        free(temp);
    }
    
    struct EducationNode* edu = ENode;
    while(edu != NULL) {
        struct EducationNode* temp = edu;
        edu = edu->next;
        free(temp);
    }
    
    struct LayOffNode* lo = LONode;
    while(lo != NULL) {
        struct LayOffNode* temp = lo;
        lo = lo->next;
        free(temp);
    }
    
    EDNode = NULL;
    WTNode = NULL;
    DONode = NULL;
    ENode = NULL;
    LONode = NULL;
}



int main(){
    int choice1, choice2, choice3;
    char employeeID_tmp[100];
    PersonalData newPD;
    EmploymentContract newEC;
    Insurance newI;
    SalaryManaging newSM;
    WorkTime newWT;
    DayOff newDO;
    Education newE;
    LayOff newLO;
    
    loadEmployeesFromFile("dulieu_nhanvien.txt");
    loadWorkTimeFromFile("worktime.txt");
    loadDayOffFromFile("dayoff.txt");
    loadEducationFromFile("education.txt");
    loadLayOffFromFile("layoff.txt");
    printf("Welcome to the Human Resource Managing System!\n");



    do {
        printf("\n--- HUMAN RESOURCE MANAGING SYSTEM MENU ---\n");
        printf("1. Add New Data\n");
        printf("2. Display Data\n");
        printf("3. Find and Display Data\n");
        printf("4. Update Data\n");
        printf("5. Delete an Employee\n");
        printf("6. Generate Report\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice1);
        clear_input_buffer();

        switch (choice1) {
            case 1:
                printf("\n--- HUMAN RESOURCE MANAGING SYSTEM MENU ---\n");
                printf("\n--- ADD NEW DATA ---\n");
                printf("1. Add New Employee Data\n");
                printf("2. Add New Tracking Time\n");
                printf("3. Add New Day Off Data\n");        
                printf("4. Add New Education Data\n");
                printf("5. Add New Layoff Data\n");
                printf("6. Return to previous options\n");
                printf("Enter your choice: ");
                scanf("%d", &choice2);
                clear_input_buffer();
                switch (choice2){
                    case 1:
                        EmployeeDataInput(&newPD, &newEC, &newI, &newSM);
                        EmployeeDataInsert(newPD, newEC, newI, newSM);
                        break;
                    case 2:
                        WorkTimeInput(&newWT);
                        WorkTimeInsert(newWT);
                        break;
                    case 3:
                        DayOffInput(&newDO);
                        DayOffInsert(newDO);
                        break;
                    case 4:
                        EducationInput(&newE);
                        EducationInsert(newE);
                        break;
                    case 5:
                        LayOffInput(&newLO);
                        LayOffInsert(newLO);
                        break;
                    case 6:
                        break;
                    default:
                        printf("Invalid choice. Please try again.\n");
                }
                break;
            case 2:
                printf("\n--- HUMAN RESOURCE MANAGING SYSTEM MENU ---\n");
                printf("\n--- DISPLAY DATA ---\n");
                printf("1. Display Employee Data\n");
                printf("2. Display Tracking Time\n");
                printf("3. Display Day Off Data\n");        
                printf("4. Display Education Data\n");
                printf("5. Display Layoff Data\n");
                printf("6. Return to previous options\n");
                printf("Enter your choice: ");
                scanf("%d", &choice2);
                clear_input_buffer();
                switch (choice2) {
                    case 1:
                        EmployeeDataTraverse(NULL);
                        break;
                    case 2:
                        WorkTimeTraverse(NULL);
                        break;
                    case 3:
                        DayOffTraverse(NULL);
                        break;
                    case 4:
                        EducationTraverse(NULL);
                        break;
                    case 5:
                        LayOffTraverse(NULL);
                        break;
                    case 6:
                        break;
                    default:
                        printf("Invalid choice. Please try again.\n");
                }
                break;
            case 3:
                printf("\n--- HUMAN RESOURCE MANAGING SYSTEM MENU ---\n");
                printf("\n--- FIND AND DISPLAY DATA ---\n");
                printf("1. Employee Data\n");
                printf("2. Work Time Tracking Data\n");
                printf("3. Dayoff Data\n");
                printf("4. Education Data\n");
                printf("5. Layoff Data\n");
                printf("6. Back to previous options\n");
                printf("Enter What You Want To Find: \n");
                scanf("%d", &choice3);
                clear_input_buffer();
                if (choice3 >= 1 && choice3 <= 5) {
                    getStringInput(employeeID_tmp, sizeof(employeeID_tmp), "Enter Employee ID: ");
                    switch (choice3) {
                        case 1:
                            if (findEmployeeData(employeeID_tmp)) EmployeeDataTraverse(employeeID_tmp);
                            break;
                        case 2:
                            if (findWorkTimeData(employeeID_tmp)) WorkTimeTraverse(employeeID_tmp);
                            break;
                        case 3:
                            if (findDayOffData(employeeID_tmp)) DayOffTraverse(employeeID_tmp);
                            break;
                        case 4:
                            if (findEducationData(employeeID_tmp)) EducationTraverse(employeeID_tmp);
                            break;
                        case 5:
                            if (findLayOffData(employeeID_tmp)) LayOffTraverse(employeeID_tmp);
                            break;
                    }
                } else if (choice3 != 6) {
                    printf("Invalid choice.\n");
                }
                break;
                
            case 4:
                printf("\n--- UPDATE DATA ---\n");
                printf("1. Update Personal Data\n");
                printf("2. Update Tracking Time\n");
                printf("3. Update Day Off Data\n");
                printf("4. Update Education Data\n");
                printf("5. Return\n");
                printf("Enter your choice: ");
                scanf("%d", &choice2);
                clear_input_buffer();
                switch (choice2) {
                    case 1: updatePersonalData(); 
                        break;
                    case 2: updateTrackingData(); 
                        break;
                    case 3: updateDayOffData(); 
                        break;
                    case 4: updateCourseData(); 
                        break;
                    case 5: 
                        break;
                    default: 
                        printf("Invalid choice.\n");
                }
                break;
            case 5:
                printf("\n--- DELETE AN EMPLOYEE ---\n");
                getStringInput(employeeID_tmp, sizeof(employeeID_tmp), "Enter Employee ID to delete: ");
                deleteEmployee(employeeID_tmp);
                break;
            case 6:
                ReportPrint();
                break;
            case 7:
                printf("Exiting program. Goodbye!\n");
                freeAllLists();
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }

        printf("\nPress Enter to continue...\n");
        getchar();
     
    } while (choice1 != 7);

    return 0;
}