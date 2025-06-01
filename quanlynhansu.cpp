/* HUMAN RESOURCE MANAGING SYSTEM */
#include <stdio.h>
#include <string.h>  
#include <stdlib.h>

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
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

typedef struct {
    long long BaseSalary;
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
    char Name[100];
    char CheckIn[10], CheckOut[10]; // Both store as xx:yy
    int Overtime;
} WorkTime;

typedef struct WorkTimeNode {
    WorkTime WT;
    struct WorkTimeNode* next;
} WorkTimeNode;

typedef struct {
    char Day[30]; // Store as dd/mm/yy format
    WorkTimeNode* WTNode;
} Tracking;

typedef struct TrackingNode {
    Tracking T;
    struct TrackingNode* next;
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
    char Name[100];
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
    char Name[100];
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
    char Name[100];
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
struct TrackingNode* TRNode = NULL;
struct DayOffNode* DONode = NULL;
struct EducationNode* ENode = NULL;
struct LayOffNode* LONode = NULL;

void read_line_from_file(char* buffer, int size, FILE* file) {
    if (fgets(buffer, size, file) != NULL) {
        buffer[strcspn(buffer, "\n\r")] = '\0';
    } else {
        buffer[0] = '\0';
    }
}

int InsertEmployeeData(PersonalData pd, EmploymentContract ec, Insurance ins, SalaryManaging sm) {
    struct EmployeeDataNode* newNode = (struct EmployeeDataNode*)malloc(sizeof(struct EmployeeDataNode));
    if (newNode == NULL) return -1;
    newNode->PD = pd;
    newNode->EC = ec;
    newNode->I = ins;
    newNode->SM = sm;
    newNode->next = EDNode;
    EDNode = newNode;
    return 0;
}

int inputEmployeeDataFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    PersonalData pd;
    EmploymentContract ec;
    Insurance ins;
    SalaryManaging sm;
    char buffer[256];

    while (1) {
        //Personal Data
        read_line_from_file(pd.Name, sizeof(pd.Name), file); 
        if (feof(file)) break; 
        read_line_from_file(pd.Birth, sizeof(pd.Birth), file); 
        fscanf(file, "%d", (int*)&pd.Gender); 
        read_line_from_file(buffer, sizeof(buffer), file); 
        read_line_from_file(pd.NationalID, sizeof(pd.NationalID), file); 
        read_line_from_file(pd.TaxNumber, sizeof(pd.TaxNumber), file); 
        read_line_from_file(pd.PhoneNumber, sizeof(pd.PhoneNumber), file); 
        read_line_from_file(pd.Email, sizeof(pd.Email), file); 

        //Labor Contract Data
        fscanf(file, "%d", (int*)&ec.Type); 
        read_line_from_file(buffer, sizeof(buffer), file); 
        read_line_from_file(ec.LaborDate, sizeof(ec.LaborDate), file); 
        read_line_from_file(ec.Pos, sizeof(ec.Pos), file); 
        read_line_from_file(ec.Dept, sizeof(ec.Dept), file); 
        fscanf(file, "%llu", &ec.DealtSalary); 
        read_line_from_file(buffer, sizeof(buffer), file); 

        //Insurance Data
        read_line_from_file(ins.BHXH.Code, sizeof(ins.BHXH.Code), file); 
        fscanf(file, "%lf", &ins.BHXH.PayLevel); 
        read_line_from_file(buffer, sizeof(buffer), file); 
        read_line_from_file(ins.BHXH.InsuranceDate, sizeof(ins.BHXH.InsuranceDate), file); 
        read_line_from_file(ins.BHYT.Code, sizeof(ins.BHYT.Code), file); 
        fscanf(file, "%lf", &ins.BHYT.PayLevel); 
        read_line_from_file(buffer, sizeof(buffer), file); 
        read_line_from_file(ins.BHYT.InsuranceDate, sizeof(ins.BHYT.InsuranceDate), file); 
        read_line_from_file(ins.BHTN.Code, sizeof(ins.BHTN.Code), file); 
        fscanf(file, "%lf", &ins.BHTN.PayLevel);
        read_line_from_file(buffer, sizeof(buffer), file);
        read_line_from_file(ins.BHTN.InsuranceDate, sizeof(ins.BHTN.InsuranceDate), file);

        //Salary Data
        fscanf(file, "%d", &sm.Allowance[0]);
        read_line_from_file(buffer, sizeof(buffer), file);
        fscanf(file, "%d", &sm.Allowance[1]);
        read_line_from_file(buffer, sizeof(buffer), file);
        fscanf(file, "%d", &sm.Allowance[2]);
        read_line_from_file(buffer, sizeof(buffer), file);
        fscanf(file, "%d", &sm.Bonus[0]);
        read_line_from_file(buffer, sizeof(buffer), file);
        fscanf(file, "%d", &sm.Bonus[1]);
        read_line_from_file(buffer, sizeof(buffer), file);
        fscanf(file, "%d", &sm.Bonus[2]);
        read_line_from_file(buffer, sizeof(buffer), file);
        fscanf(file, "%f", &sm.Tax);
        read_line_from_file(buffer, sizeof(buffer), file);
        fscanf(file, "%d", &sm.Method);
        read_line_from_file(buffer, sizeof(buffer), file);

        if (InsertEmployeeData(pd, ec, ins, sm) != 0) {
            printf("Error inserting data from file.\n");
            fclose(file);
            return -1;
        }
    }

    fclose(file);
    return 0;
}


//Tim kiem nhan vien theo ten
struct EmployeeDataNode* findEmployeeByName(const char* name) {
    struct EmployeeDataNode* current = EDNode;
    while (current != NULL) {
        if (strcmp(current->PD.Name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL; // Not found
}

//Sua thong tin nhan vien
int updateEmployeeData(const char* name, PersonalData pd, EmploymentContract ec, Insurance ins, SalaryManaging sm) {
    struct EmployeeDataNode* employee = findEmployeeByName(name);
    if (employee == NULL) {
        printf("Employee not found.\n");
        return -1;
    }

    employee->PD = pd;
    employee->EC = ec;
    employee->I = ins;
    employee->SM = sm;

    return 0; // Success
}

// Xoa thong tin nhan vien
int deleteEmployeeData(const char* name) {
    struct EmployeeDataNode* current = EDNode;
    struct EmployeeDataNode* previous = NULL;

    while (current != NULL) {
        if (strcmp(current->PD.Name, name) == 0) {
            if (previous == NULL) {
                EDNode = current->next; // Deleting the head node
            } else {
                previous->next = current->next; // Bypass the current node
            }
            free(current); // Free the memory
            return 0; // Success
        }
        previous = current;
        current = current->next;
    }
    printf("Employee not found.\n");
    return -1; // Not found
}





int InsertDayOffData(DayOff doff) {
    struct DayOffNode* newNode = (struct DayOffNode*)malloc(sizeof(struct DayOffNode));
    if (newNode == NULL) return -1;
    newNode->DOff = doff;
    newNode->next = DONode;
    DONode = newNode;
    return 0;
}

int inputDayOffFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    DayOff doff;
    char buffer[256];

    while (1) {
        //Work Time
        fscanf(file, "%d", &doff.AbsenceLetter);
        read_line_from_file(buffer, sizeof(buffer), file);
        read_line_from_file(doff.AbsenceDate, sizeof(doff.AbsenceDate), file);
        fscanf(file, "%d", &doff.letterstatus);
        read_line_from_file(buffer, sizeof(buffer), file);

        if (InsertDayOffData(doff) != 0) {
            printf("Error inserting data from file.\n");
            fclose(file);
            return -1;
        }
    }

    fclose(file);
    return 0;
}

int InsertEducationData(Education e){
    struct EducationNode* newNode = (struct EducationNode*)malloc(sizeof(struct EducationNode));
    if (newNode == NULL) return -1;
    newNode->E = e;
    newNode->next = ENode;
    ENode = newNode;
    return 0;
}

int inputEducationFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    Education e;
    char buffer[256];

    while (1) {
        //Education
        read_line_from_file(e.CourseName, sizeof(e.CourseName), file);
        fscanf(file, "%d", &e.CourseTime);
        read_line_from_file(buffer, sizeof(buffer), file);
        fscanf(file, "%llu", &e.CourseFee);
        read_line_from_file(buffer, sizeof(buffer), file);
        fscanf(file, "%llu", &e.coursestatus);
        read_line_from_file(buffer, sizeof(buffer), file);

        if (InsertEducationData(e) != 0) {
            printf("Error inserting data from file.\n");
            fclose(file);
            return -1;
        }
    }

    fclose(file);
    return 0;
}

int InsertTrackingData(Tracking t) {
    struct TrackingNode *newNode = (struct TrackingNode*)malloc(sizeof(struct TrackingNode));
    if (newNode == NULL) return -1;
    newNode->T = t;
    newNode->next = TRNode;
    TRNode = newNode;
    return 0;
}

int inputTrackingFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror ("Error opening file");
        return -1;
    }

    char buffet[256];
    Tracking t;

    while (!feof(file)) {

        read_line_from_file(t.Day, sizeof(t.Day), file);
        if (strlen(t.Day) == 0) break;

        t.WTNode = NULL;
        int employeeCount = 0;
        fscanf(file, "%d", &employeeCount);
        fgets(buffet, sizeof(buffet), file);

        for (int i = 0; i < employeeCount; i++) {
            WorkTimeNode* newWTNode = (WorkTimeNode*)malloc(sizeof(WorkTimeNode));
            if (newWTNode == NULL) continue;

            read_line_from_file(newWTNode->WT.Name, sizeof(newWTNode->WT.Name), file);
            read_line_from_file(newWTNode->WT.CheckIn, sizeof(newWTNode->WT.CheckIn), file);
            read_line_from_file(newWTNode->WT.CheckOut, sizeof(newWTNode->WT.CheckOut), file);
            fscanf(file, "%d", &newWTNode->WT.Overtime);
            fgets(buffet, sizeof(buffet), file);

            newWTNode->next = t.WTNode;
            t.WTNode = newWTNode;   
        }
        if(InsertTrackingData(t) != 0) {
                printf("Error inserting tracking data from file.\n");
                fclose(file);
                return -1;
        }
    }
    fclose(file);
    return 0;
}

void PersonalDataTraverse(){
    struct EmployeeDataNode* index = EDNode;
    while(index != NULL){
        printf("\nPERSONAL DATA:\n");
        printf("Name: %s", index->PD.Name);
        printf("\nBirth: %s", index->PD.Birth);
        printf("\nGender: %s", sexToString(index->PD.Gender));
        printf("\nNationalID: %s", index->PD.NationalID);
        printf("\nTax Number: %s", index->PD.TaxNumber);
        printf("\nPhone: %s", index->PD.PhoneNumber);
        printf("\nEmail: %s", index->PD.Email);

        printf("\n\nLABOR CONTRACT DATA:\n");
        printf("\nContract: %s", contractTypeToString(index->EC.Type));
        printf("\nLabor Date: %s", index->EC.LaborDate);
        printf("\nPosition: %s", index->EC.Pos);
        printf("\nDepartment: %s", index->EC.Dept);
        printf("\nDealt Salary: %d", index->EC.DealtSalary);
        
        printf("\n\nINSURANCE DATA:");

        printf("\n\nSocial Insurance:");
        printf("\nCode: %s", index->I.BHXH.Code);
        printf("\nPayment Ratio: %f", index->I.BHXH.PayLevel);
        printf("\nDate: %s", index->I.BHXH.InsuranceDate);

        printf("\n\nHealth Insurance:");
        printf("\nCode: %s", index->I.BHYT.Code);
        printf("\nPayment Ratio: %f", index->I.BHYT.PayLevel);
        printf("\nDate: %s", index->I.BHYT.InsuranceDate);

        printf("\n\nUnemployment Insurance:");
        printf("\nCode: %s", index->I.BHTN.Code);
        printf("\nPayment Ratio: %f", index->I.BHTN.PayLevel);
        printf("\nDate: %s\n", index->I.BHTN.InsuranceDate);
        
        index = index->next;
    }
}

int main() {

    
}