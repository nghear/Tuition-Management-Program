#include <stdio.h> // fread, fwrite, etc...
#include <string.h> // strcpy, strcmp, etc...
#include <stdlib.h>
#include <math.h>
#include <vector> 
#include <algorithm> 
#include <conio.h> // getch()
#include <windows.h> // Sleep()
#include <limits> // max size of data types
#include <ctype.h> // tolower()

#define MAX_ID_LENGTH 15
#define MAX_NAME_LENGTH 31
#define MAX_CLASSNAME_LENGTH 21
#define MAX_NUMBER_LENGTH 12

typedef struct { // (1/12, N)
    char student_ID[MAX_ID_LENGTH];
    char student_name[MAX_NAME_LENGTH];
    char student_email[MAX_NAME_LENGTH];
    char student_phone[MAX_NUMBER_LENGTH];  // Changed from int array to char array
    char course_attend[MAX_NAME_LENGTH]; // To Cross-Check courses's data
    char class_attend[MAX_CLASSNAME_LENGTH]; // To Cross-Check classes's data
    float tuition_paid; // Amount need to pay (negative = haven't pay) 
} students;

typedef struct { // (25/11, N)
    char course_ID[MAX_ID_LENGTH];
    char course_name[MAX_NAME_LENGTH];
    int tuition; // Course's Fee
    int total_students; // count all student(s) studying course
    int total_class; // count total class exist
    int max_students; // maximum amount of students can be in a class
} courses;

typedef struct { // (1/12 N)
    char course_ID[MAX_ID_LENGTH];
    char class_ID[MAX_ID_LENGTH];
    char class_name[MAX_CLASSNAME_LENGTH];
    char class_teacher[MAX_NAME_LENGTH]; // New data type
    int total_students; // current number of student(s) in class 
} classes;

typedef struct {
    char employee_ID[MAX_ID_LENGTH];
    char employee_name[MAX_NAME_LENGTH];
    char employee_phone[MAX_NUMBER_LENGTH];
    char employee_email[MAX_NAME_LENGTH];
    char employee_password[MAX_NUMBER_LENGTH];
    int employee_role; // 1 = Admin, 2 = Accountant, 3 = Staff, 4 = Teacher
} employee;


void eraseLines(int count) {
    if (count > 0) {
        printf("\x1b[2K"); // Clear current line
        for (int i = 1; i < count; i++) {
            printf("\x1b[1A\x1b[2K");
        }
        printf("\r"); // Move cursor to start of line
    }
}

//add employee functions
void employee_add() {
    FILE *file;
    employee new_employee, existing_employee;
    int employee_count = 0;
    bool employee_found = false;
    int phone_length;
    char phone_str[MAX_NUMBER_LENGTH];

    //open file in append and read mode
    file = fopen("ex_employee.txt", "a+b");
    if (file == NULL) {
        printf("Error Opening file (>_<)!\n");
        printf("\n\n");
        printf("\t                        (\\(\\ \n");
        printf("\tPress any key to return ( -.-) \n");
        getch();
        return;
    }

    //get new employee information
    system("cls");
    printf("\n");
    printf("\t+========================================================+\n");
    printf("\t|              Study Center Management System            |\n");
    printf("\t|========================================================|\n");
    printf("\t|                Add Employee Information                |\n");
    printf("\t+========================================================+\n\n");

    do {
        printf("\tEnter Employee's ID: ");
        fflush(stdin);
        fgets(new_employee.employee_ID, sizeof(new_employee.employee_ID), stdin);
        new_employee.employee_ID[ strcspn(new_employee.employee_ID, "\n") ] = 0;
        if (strlen(new_employee.employee_ID) == 0) {
            printf("\tEmployee's ID cannot be empty (>_<)!\n");
        }
    } while (strlen(new_employee.employee_ID) == 0);

    //Check if ID already exists
    rewind(file);
    while ( fread(&existing_employee, sizeof(employee), 1, file) == 1) {
        employee_count++;
        if ( strcmp(existing_employee.employee_ID, new_employee.employee_ID) == 0) {
            employee_found = true;
            break;
        }
    }

    if (employee_found) {
        printf("\tError: Employee ID [ %s ] already exists (>_<)!\n", new_employee.employee_ID);
    } else {

        // Employee's Name validation
        do
        {
            printf("\tEnter Employee's Name: ");
            fflush(stdin);
            fgets(new_employee.employee_name, sizeof(new_employee.employee_name), stdin);
            new_employee.employee_name[strcspn(new_employee.employee_name, "\n")] = 0;
            if (strlen(new_employee.employee_name) == 0) {
                printf("\tEmployee's Name cannot be empty (>_<)!\n");
                continue;
            }
            break;
        } while (1);

        // Phone number validation
        do {
            printf("\tEnter Student's Phone Number: ");
            fflush(stdin);
            fgets(phone_str, sizeof(phone_str), stdin);
            fflush(stdin);
            phone_str[strcspn(phone_str, "\n")] = 0;
            
            phone_length = strlen(phone_str);
            if (phone_length > 10) {
                printf("\tYou typed over 10 digits [ %s ].", phone_str);
            }
            else if (phone_length < 10) {
                printf("\tYou typed only %d digits [ %s ].", phone_length, phone_str);
            }
            printf("\n");
            bool valid = true;
            
            // Check if exactly 10 digits
            if(phone_length != 10) {
                printf("\tPhone number must be exactly 10 digits!\n");
                getch();
                eraseLines(4);
                continue;
            }

            // Check if first 2 digits are valid
            if(strncmp(phone_str, "09", 2) != 0 && strncmp(phone_str, "08", 2) != 0) {
                printf("\tPhone number must start with '09' or '08'!\n");
                getch();
                eraseLines(4);
                continue;
            }
            
            // Verify all characters are digits
            for(int i = 0; i < phone_length; i++) {
                if(!isdigit(phone_str[i])) {
                    valid = false;
                    break;
                }
            }
            
            if(valid) {
                strcpy(new_employee.employee_phone, phone_str);
                printf("\tValid phone number format.\n");
                break;
            }
            printf("\tInvalid phone number format (>_<)! Please enter 10 digits (number) only.\n");
            getch();
            eraseLines(4);
        } while(1);

        //Email validation
        do {
            printf("\tEnter Employee's Email: ");
            fflush(stdin);
            fgets(new_employee.employee_email, sizeof(new_employee.employee_email), stdin);
            fflush(stdin);
            new_employee.employee_email[strcspn(new_employee.employee_email, "\n")] = 0;
            if (strlen(new_employee.employee_email) == 0) {
                printf("\tEmployee's Email cannot be empty (>_<)!\n");
            } else if (strchr(new_employee.employee_email, '@') == NULL) {
                printf("\tEmployee's Email must contain '@' (>_<)!\n");
            }
        } while (strlen(new_employee.employee_email) == 0 || strchr(new_employee.employee_email, '@') == NULL);

        //Password validation
        printf("\tEnter Employee's Password: ");
        fflush(stdin);
        fgets(new_employee.employee_password, sizeof(new_employee.employee_password), stdin);
        new_employee.employee_password[strcspn(new_employee.employee_password, "\n")] = 0;

        //Role validation
        do {
            printf("\tEnter Employee's Role: ");
            scanf("%d", &new_employee.employee_role);
            if (new_employee.employee_role < 1 || new_employee.employee_role > 4) {
                printf("\tInvalid role (>_<)! Enter 1 for Admin, 2 for Accountant, 3 for Staff, or 4 for Teacher.\n");
            }
        } while (new_employee.employee_role < 1 || new_employee.employee_role > 4);


        //Write new student to file
        fseek(file, 0, SEEK_END);
        new_employee.employee_name[ strcspn(new_employee.employee_name, "\n") ] = 0;
        fwrite(&new_employee, sizeof(employee), 1, file);
        printf("\n\tEmployee added successfully ( =^.^=). Number: %d\n", employee_count + 1);
    }

    fclose(file);

    printf("\n\n");
    printf("\t                        (\\(\\ \n");
    printf("\tPress any key to return ( -.-) !\n");
    getch();
}


//print all employee
void employee_print() {
    FILE *file;
    employee emp;
    int employee_count = 0;

    file = fopen("ex_employee.txt", "rb");

    if (file == NULL) {
        printf("Error opening file or file doesn't exist (>_<)!\n");
        printf("\n\n");
        printf("                        (\\(\\ \n");
        printf("Press any key to return ( -.-) \n");
        getch();
        return;
    }

    system("cls");
    printf("\n");
    printf("\t+========================================================================+\n");
    printf("\t|                    Study Center Management System                      |\n");
    printf("\t|========================================================================|\n");
    printf("\t|                          List of Employees                              |\n");
    printf("\t+========================================================================+\n\n");

    // Header row with consistent formatting
    printf("\t+------+----------------+--------------------------------+");
    printf("--------------------------------+---------------+\n");
    printf("\t| %-4s | %-14s | %-30s | %-30s | %-13s |\n",
           "No.", "ID", "Name", "Phone", "Email", "Role");
    printf("\t+------+----------------+--------------------------------+");
    printf("--------------------------------+---------------+\n");

    // Data rows with consistent alignment
    while (fread(&emp, sizeof(employee), 1, file) == 1) {
        employee_count++;
        printf("\t| %-4d | %-14s | %-30s | %-30s | %13.2f |\n",
            employee_count, emp.employee_ID, emp.employee_name, emp.employee_phone, emp.employee_email, emp.employee_role);
    }

    // Bottom border
    printf("\t+------+----------------+--------------------------------+");
    printf("--------------------------------+---------------+\n");

    if (employee_count == 0) {
        printf("\t|                      Cannot Detect any Employee (>~<)                        |\n");
        printf("\t+------------------------------------------------------------------------+\n");
    } else {
        printf("\n\tTotal number of Employee(s): %d\n\n", employee_count);
    }
}



int main() {
    employee_add();
    employee_print();
}
