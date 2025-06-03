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
        case BankTransfer: return "Bank Transfers";
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

void EmployeeDataInsert(PersonalData pd, EmploymentContract ec, Insurance in, SalaryManaging sm){
    struct EmployeeDataNode* newNode = (struct EmployeeDataNode*)malloc(sizeof(struct EmployeeDataNode));
    if (newNode == NULL) printf("DATA INSERTED UNSUCCESSFULLY!\n");
    else{
        newNode->PD = pd;
        newNode->EC = ec;
        newNode->I = in;
        newNode->SM = sm;
        newNode->SM.FinalSalary = FinalSalaryCalculation(sm.BaseSalary, sm.WorkTimeThisMonth, sm.Allowance, sm.Bonus, sm.Tax);
        newNode->next = EDNode;
        EDNode = newNode;
        printf("DATA INSERTED SUCCESSFULLY!\n");
    }
}

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

void saveEmployeeToFile(const PersonalData* pd, const char* filename) {
    FILE* file = fopen(filename, "a");
    if (!file) {
        printf("Cannot open file to write!\n");
        return;
    }

    fprintf(file, "%s|%s|%s|%s|%s|%s|%s|%s\n",
        pd->EmployeeID,
        pd->Name,
        pd->Birth,
        sexToString(pd->Gender),
        pd->NationalID,
        pd->TaxNumber,
        pd->PhoneNumber,
        pd->Email
    );

    fclose(file);
}

void generateEmployeeID(char* newID, const char* filename) {
    int lastID = getLastEmployeeID_FromFile(filename);
    sprintf(newID, "NV%04d", lastID + 1);
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
    saveEmployeeToFile(pd, "dulieu_nhanvien.txt");
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

    printf("\nEmployee data input complete.\n");
}

void WorkTimeInsert(WorkTime wt){
    struct WorkTimeNode* newNode = (struct WorkTimeNode*)malloc(sizeof(struct WorkTimeNode));
    if (newNode == NULL) printf("DATA INSERTED UNSUCESSFULLY!\n");
    else{
        newNode->WT = wt;
        newNode->next = WTNode;
        WTNode = newNode;
        printf("DATA INSERTED SUCESSFULLY!\n");
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
    if (newNode == NULL) printf("DATA INSERTED UNSUCESSFULLY!\n");
    else{
        newNode->DOff = doff;
        newNode->next = DONode;
        DONode = newNode;
        printf("DATA INSERTED SUCESSFULLY!\n");
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
        printf("DATA INSERTED UNSUCESSFULLY!\n");
        return;
    }
    newNode->E = e;
    newNode->next = ENode;
    ENode = newNode;
    printf("DATA INSERTED SUCESSFULLY!\n");
    
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
    if (newNode == NULL) printf("DATA INSERTED UNSUCESSFULLY!\n");
    else{
        newNode->LO = lo;
        newNode->next = LONode;
        LONode = newNode;
        printf("DATA INSERTED SUCESSFULLY!\n");
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

void EmployeeDataTraverse(){
    printf("\nDISPLAYING EMPLOYEE DATA\n");
    int cnt = 1;
    struct EmployeeDataNode* index = EDNode;
    while(index != NULL){
        printf("\nEMPLOYEE %03d:\n", cnt);
        printf("\nName: %s", index->PD.Name);
        printf("Employee ID: %s\n", index->PD.EmployeeID);
        printf("\n\nBirth: %s", index->PD.Birth);
        printf("\nGender: %s", sexToString(index->PD.Gender));
        printf("\nNationalID: %s", index->PD.NationalID);
        printf("\nTax Number: %s", index->PD.TaxNumber);
        printf("\nPhone: %s", index->PD.PhoneNumber);
        printf("\nEmail: %s", index->PD.Email);
        printf("\n\nContract: %s", contractTypeToString(index->EC.Type));
        printf("\nLabor Date: %s", index->EC.LaborDate);
        printf("\nPosition: %s", index->EC.Pos);
        printf("\nDepartment: %s", index->EC.Dept);
        printf("\nDealt Salary: %d", index->EC.DealtSalary);
        printf("\n\nSocial Insurance:");
        printf("\nCode: %s", index->I.BHXH.Code);
        printf("\nPayment Ratio: %.2f", index->I.BHXH.PayLevel);
        printf("\nDate: %s", index->I.BHXH.InsuranceDate);
        printf("\n\nHealth Insurance:");
        printf("\nCode: %s", index->I.BHYT.Code);
        printf("\nPayment Ratio: %.2f", index->I.BHYT.PayLevel);
        printf("\nDate: %s", index->I.BHYT.InsuranceDate);
        printf("\n\nUnemployment Insurance:");
        printf("\nCode: %s", index->I.BHTN.Code);
        printf("\nPayment Ratio: %.2f", index->I.BHTN.PayLevel);
        printf("\nDate: %s\n", index->I.BHTN.InsuranceDate);
        printf("Salary: %lld\n", index->SM.FinalSalary);
        printf("Payment Method: %s\n", paymentToString(index->SM.Method));
        printf("\n");
        index = index->next;
        cnt++;
    }
}

void WorkTimeTraverse(){
    printf("\nDISPLAYING TRACKING DATA\n");
    struct WorkTimeNode* index = WTNode;
    int cnt = 1;
    while(index != NULL){
        printf("\nWORK TIME RECORD %03d:\n", cnt);
        printf("Employee ID: %s\n", index->WT.EmployeeID);
        printf("\nCheckin: %s", index->WT.CheckIn);
        printf("\nCheckout: %s", index->WT.CheckOut);
        printf("\nOvertime: %d", index->WT.Overtime);
        printf("\n");
        index = index->next;
        cnt++;
    }
}

void DayOffTraverse(){
    printf("\nDISPLAYING DAY OFF DATA\n");
    struct DayOffNode* index = DONode;
    int cnt = 1;
    while(index != NULL){
        printf("\nName: %s", index->DOff.EmployeeID);
        printf("\nLetter: %s", letterToString(index->DOff.AbsenceLetter));
        printf("\nDate: %s", index->DOff.AbsenceDate);
        printf("\nLetter Status: %s", astatusToString(index->DOff.letterstatus));
        printf("\n");
        index = index->next;
        cnt++;
    }
}

void EducationTraverse(){
    printf("\nDISPLAYING EDUCATION DATA\n");
    struct EducationNode* index = ENode;
    int cnt = 1;
    while(index != NULL){
        printf("\nEDUCATION RECORD %03d:\n", cnt);
        printf("Employee ID: %s\n", index->E.EmployeeID);
        printf("Course Name: %s\n", index->E.CourseName);
        printf("Course Time: %d hours\n", index->E.CourseTime);
        printf("Course Fee: %lld\n", index->E.CourseFee);
        printf("Course Status: %s\n", cstatusToString(index->E.coursestatus));
        printf("\n");
        index = index->next;
        cnt++;
    }
}

void LayOffTraverse(){
    printf("\nDISPLAYING LAYOFF DATA\n");
    struct LayOffNode* index = LONode;
    int cnt = 1;
    while(index != NULL){
        printf("\nLAYOFF RECORD %03d:\n", cnt);
        printf("Employee ID: %s\n", index->LO.EmployeeID);
        printf("Date: %s\n", index->LO.Date);
        printf("Reason: %s\n", layoffreasonToString(index->LO.Reason));
        printf("\n");
        index = index->next;
        cnt++;
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

struct EmployeeDataNode* FindData(const char* employeeID) {
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

struct WorkTimeNode* FindData(const char* employeeID) {
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

struct DayOffNode* FindData(const char* employeeID) {
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

struct EducationNode* FindData(const char* employeeID) {
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

struct LayOffNode* FindData(const char* employeeID) {
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

int main(){
    int choice1, choice2, choice3;
    char EmployeeID_tmp[100];
    PersonalData newPD;
    EmploymentContract newEC;
    Insurance newI;
    SalaryManaging newSM;
    WorkTime newWT;
    DayOff newDO;
    Education newE;
    LayOff newLO;

    do {
        printf("\n--- HUMAN RESOURCE MANAGING SYSTEM MENU ---\n");
        printf("1. Add New Data\n");
        printf("2. Display Data\n");
        printf("3. Find and Update/Change Data\n");
        printf("4. Delete an Employee\n");
        printf("5. Generate Report\n");
        printf("6. Exit\n");
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
                        EmployeeDataTraverse();
                        break;
                    case 2:
                        WorkTimeTraverse();
                        break;
                    case 3:
                        DayOffTraverse();
                        break;
                    case 4:
                        EducationTraverse();
                        break;
                    case 5:
                        LayOffTraverse();
                        break;
                    case 6:
                        break;
                    default:
                        printf("Invalid choice. Please try again.\n");
                }
            case 3:
                printf("\n--- HUMAN RESOURCE MANAGING SYSTEM MENU ---\n");
                printf("\n--- FIND AND UPDATE/CHANGE DATA ---\n");
                printf("1. Employee Data\n");
                printf("2. Work Time Tracking Data\n");
                printf("3. Dayoff Data\n");
                printf("4. Education Data\n");
                printf("5. Layoff Data\n");
                printf("6. Back to previous options\n");
                printf("Enter What You Want To Find: \n");
                scanf("%d", &choice3);
                clear_input_buffer();
                getStringInput(EmployeeID_tmp, sizeof(EmployeeID_tmp), "Enter Employee ID: ");
                switch (choice3) {
                    case 1:
                        struct EmployeeDataNode* foundEmployee = FindData(EmployeeID_tmp);
                        // Change
                    case 2: 
                        struct WorkTimeNode* foundWorkTime = FindData(EmployeeID_tmp);
                        // Change
                    case 3:
                        struct DayOffNode* foundDayOff = FindData(EmployeeID_tmp);
                        // Change
                    case 4:
                        struct EducationNode* foundEducation = FindData(EmployeeID_tmp);
                        // Change
                    case 5:
                        struct LayOffNode* foundLO = FindData(EmployeeID_tmp);
                        // Change
                    case 6: 
                        break;
                    default:
                        printf("Invalid choice. Please try again.\n");
                }
                
            case 5:
                ReportPrint();
                break;
            case 6: 
                printf("Exiting program. Goodbye!\n");
                freeAllLists();
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice1 != 4);

    return 0;
}
