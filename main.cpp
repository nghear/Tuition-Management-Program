#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <conio.h>
#include <windows.h>
#include <iomanip>
#include <limits>
#include <cstdlib>
#include <cstring>
#include <locale.h>

void eraseLines(int count) {
    if (count > 0) {
        printf("\x1b[2K"); // Clear current line
        for (int i = 1; i < count; i++) {
            printf("\x1b[1A\x1b[2K");
        }
        printf("\r"); // Move cursor to start of line
    }
}

void gotoxy(int x, int y) {
    static HANDLE h = nullptr;  
    if(!h)
        h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD c = { static_cast<SHORT>(x), static_cast<SHORT>(y) };  
    SetConsoleCursorPosition(h,c);
}


#define MAX_ID_LENGTH 15
#define MAX_NAME_LENGTH 200
#define MAX_CLASSNAME_LENGTH 200

typedef struct {
    char student_id[MAX_ID_LENGTH];
    char student_name[MAX_NAME_LENGTH];
    float tuition_paid;
    char class_attend[MAX_CLASSNAME_LENGTH];
} students;

typedef struct {
    char class_id[MAX_ID_LENGTH];
    char class_name[MAX_CLASSNAME_LENGTH];
    int tuition;
} classes;

void experimental_insert_student() {
    FILE *file;
    students new_student, existing_student;
    int student_count = 0;
    char no_class[MAX_CLASSNAME_LENGTH] = "Not Registered";
    bool id_found = false;

    //Open file in append and read mode
    file = fopen("ex_student.txt", "a+");
    if (file == NULL) {
        printf("Error Opening file (>_<)!\n");
        return;
    }

    // Get new student Information
    printf("\nEnter Student Information\n");
    printf("Enter Student's ID: ");
    fflush(stdin);
    fgets(new_student.student_id, sizeof(new_student.student_id), stdin);

    //Check if ID already exists
    rewind(file);
    while ( fread(&existing_student, sizeof(students), 1, file) == 1) {
        student_count++;
        if ( strcmp(existing_student.student_id, new_student.student_id) == 0) {
            id_found = true;
            break;
        }
    }

    if (id_found) {
        printf("Error: Student ID %s already exists (>_<)!\n", new_student.student_id);
    }
    else {
        printf("Enter Student's Name: ");
        fflush(stdin);
        fgets(new_student.student_name, sizeof(new_student.student_name), stdin);

        printf("Enter Tuition Paid: ");
        scanf("%f", &new_student.tuition_paid);

        strcpy(new_student.class_attend, no_class);

        //Write new student to file
        fseek(file, 0, SEEK_END);
        new_student.student_id[ strcspn(new_student.student_id, "\n") ] = 0;
        new_student.student_name[ strcspn(new_student.student_name, "\n") ] = 0;
        fwrite(&new_student, sizeof(students), 1, file);

        printf("                           |\\  |\\ \n");
        printf("Student added sucessfully ( =^.^=)! Number: %d", student_count + 1);
    }

    fclose(file);
    printf("\n");
    printf("                        (\\(\\  \n");
    printf("Press any key to return ( -.-) \n");
    getch();
    system("cls");
}

void experimental_print_students_list() {
    FILE *file;
    students st;
    int student_count = 0;

    // Open the file in binary read mode
    file = fopen("ex_student.txt", "r");
    if (file == NULL) {
        printf("Error opening file or file doesn't exist (>_<)!\n");
       
        printf("                        (\\(\\  \n");
        printf("Press any key to return ( -.-) \n");
        getch();
        system("cls");
        return;
    }

    system("cls");  // Clear the screen

    printf("\t\t\tList of Students\n\n");
    printf("%-5s %-15s %-30s %-15s %-30s\n", 
           "No.", "ID", "Full Name", "Tuition Paid", "Attending Classes");
    printf("--------------------------------------------------------------------------------\n");

    // Read and print each student
    while (fread(&st, sizeof(students), 1, file) == 1) {
        student_count++;
        printf("%-5d %-15s %-30s %-15.2f %-30s", 
            student_count, st.student_id, st.student_name, st.tuition_paid, st.class_attend);
        printf("\n");
    }

    if (student_count == 0) {
        printf("No students found in the file (=.=)\n");
    } else {
        printf("\nTotal number of students: %d\n", student_count);
    }

    fclose(file);

    printf("                        (\\(\\  \n");
    printf("Press any key to return ( -.-) \n");
    getch();
    system("cls");
}

void experimental_insert_class() {
    FILE *file;
    classes new_class, existing_class;
    int class_count = 0;
    bool class_found = false;

    // Open the file in append and read mode
    file = fopen("ex_class.txt","a+");
    if (file == NULL) {
        printf("Error Opening File (>_<)!\n");
        return;
    }

    // Get new class information
    printf("\nEnter Class Information\n");
    printf("Enter Class ID: ");
    fflush(stdin);
    fgets(new_class.class_id, sizeof(new_class.class_id), stdin);

    // Check if Class ID already exist
    rewind(file);
    while ( fread(&existing_class, sizeof(classes), 1, file) == 1) {
        class_count++;
        if (strcmp(existing_class.class_id, new_class.class_id) == 0) {
            class_found = true;
            break;
        }
    }

    if (class_found) {
        printf("Error: Class ID %s already exists (>_<)!\n", new_class.class_id);
    }
    else {
        printf("Enter Class's Name: ");
        fflush(stdin);
        fgets(new_class.class_name, sizeof(new_class.class_name), stdin);

        printf("Enter Class's Tuition Cost: ");
        scanf("%d", &new_class.tuition);

        //Write new class to file
        fseek(file, 0, SEEK_END);
        new_class.class_id[ strcspn(new_class.class_id, "\n") ] = 0;
        new_class.class_name[ strcspn(new_class.class_name, "\n") ] = 0;
        fwrite(&new_class, sizeof(classes), 1, file);

        printf("\nClass added sucessfully (/*^.^)/* Number: %d\n", class_count + 1);
    }

    fclose(file);

    printf("                        (\\(\\  \n");
    printf("Press any key to return ( -.-) \n");
    getch();
    system("cls");
}

void experimental_print_classes_list() {
    FILE *file;
    classes cl;
    int class_count = 0;

    //Open the file in binary read mode
    file = fopen("ex_class.txt", "r");
    if (file == NULL) {
        printf("Error opening file or file doesn't exist (>_<)!\n");
       
        printf("                        (\\(\\  \n");
        printf("Press any key to return ( -.-) \n");
        getch();
        system("cls");
        return;
    }

    system("cls");  // Clear the screen

    printf("\t\t\tList of Classes\n\n");
    printf("%-5s %-15s %-30s %-15s\n", 
           "No.", "Subject ID", "Subject Name", "Tuition Cost");
    printf("----------------------------------------------------------\n");

    // Read and print each subject
    while (fread(&cl, sizeof(classes), 1, file) == 1) {
        class_count++;
        printf("%-5d %-15s %-30s %-15d\n", 
            class_count, cl.class_id, cl.class_name, cl.tuition);
    }

    if (class_count == 0) {
        printf("No subjects found in the file (=.=)\n");
    } else {
        printf("\nTotal number of subjects: %d\n", class_count);
    }

    fclose(file);

    printf("                        (\\(\\  \n");
    printf("Press any key to return ( -.-) \n");
    getch();
    system("cls");
}

void experimental_class_register() { // This code currently may sometime bug out, zero clue why (29/10, N).
    FILE *student_file, *class_file;
    students st;
    classes cl;
    char student_id_s[MAX_ID_LENGTH];
    char class_id_s[MAX_ID_LENGTH];
    char ans[5];
    bool student_found = false;
    bool class_check = false;
    bool class_found = false;
    long student_pos;

    // Open files
    student_file = fopen("ex_student.txt", "r+");
    class_file = fopen("ex_class.txt", "r");
    
    if (student_file == NULL || class_file == NULL) {
        printf("Error opening file(s) (>_<)!\n");
        return;
    }

    // Get student ID
    printf("Enter Student ID to register: ");
    fflush(stdin);
    fgets(student_id_s, sizeof(student_id_s), stdin);
    student_id_s[ strcspn(student_id_s, "\n") ] = 0;

    //Find student
    while (fread(&st, sizeof(students), 1, student_file) == 1) {
        if (strcmp(st.student_id, student_id_s) == 0) {
            student_found = true;
            student_pos = ftell(student_file) - sizeof(students);
            break;
        }
    }

    if (!student_found) {
        printf("Student %s not found (>_<)!\n", student_id_s);
        fclose(student_file);
        fclose(class_file);
        return;
    }

    // Check if student has already assigned for a class
    while (fread(&cl, sizeof(classes), 1, class_file) == 1) {
        if (strcmp(st.class_attend, cl.class_name) == 0) {
            class_check = true;
            break;
        }
    }

    if (class_check) {
        printf("Student has already registered class %s\n", st.class_attend);
        printf("Would you like to re-register to a different class?\n"
                "Type 'yes' to continue or 'no' to cancel: ");
        do {
            fflush(stdin);
            fgets(ans, sizeof(ans), stdin);
            ans[ strcspn(ans, "\n") ] = 0;
            if (strcmp(ans, "no") == 0 || strcmp(ans, "No") == 0) {
                printf("The process has been cancelled.\n"
                        "Press any key to return.\n");
                getch();
                return;
            }

            if (strcmp(ans, "yes") == 0 || strcmp(ans, "Yes") == 0) {
                break;
            }
            printf("Invalid Input (>_<)! Please type either yes or no: ");
        } while (true);

    }

    // Get Class ID
    printf("\nEnter Class's ID to register for: ");
    fflush(stdin);
    fgets(class_id_s, sizeof(class_id_s), stdin);
    class_id_s[ strcspn(class_id_s, "\n") ] = 0;

    // Find Class
    rewind(class_file);
    while (fread(&cl, sizeof(classes), 1, class_file) == 1) {
        if (strcmp(cl.class_id, class_id_s) == 0) {
            class_found = true;
            break;
        }
    }

    if (!class_found) {
        printf("There is no Class with ID %s (>_<)!\n", class_id_s);
        fclose(student_file);
        fclose(class_file);
        return;
    }

    if (strcmp(st.class_attend, cl.class_name) == 0) {
        printf("Student %s is already registered for class %s (As stated from previous check already (>_<)!)", st.student_name, cl.class_name);
        fclose(student_file);
        fclose(class_file);
        return;
    }

    // Register student for class
    strcpy(st.class_attend, cl.class_name);

    // Update student Record
    fseek(student_file, student_pos, SEEK_SET);
    fwrite(&st, sizeof(students), 1, student_file);

    printf("\nSuccessfully Registered Student %s for class %s (^-^)!\n", st.student_name, st.class_attend);

    fclose(student_file);
    fclose(class_file);

    printf("                        (\\(\\  \n");
    printf("Press any key to return ( -.-) \n");
    getch();
    system("cls");
}

void experimental_class_unregister() {
    FILE *file;
    students st;
    char student_id_s[MAX_ID_LENGTH];
    char class_name_s[MAX_CLASSNAME_LENGTH];
    char no_class[MAX_CLASSNAME_LENGTH] = "Not Registered";
    char ans[5];
    bool student_found = false;
    long student_pos;

    file = fopen("ex_student.txt", "r+");
    if (file == NULL) {
        printf("Error opening file (>_<)!\n");
        return;
    }

    printf("Enter Student ID: ");
    fflush(stdin);
    fgets(student_id_s, MAX_CLASSNAME_LENGTH, stdin);
    student_id_s[ strcspn(student_id_s, "\n") ] = 0;

    // Find student
    while (fread(&st, sizeof(students), 1, file) == 1) {
        if (strcmp(st.student_id, student_id_s) == 0) {
            student_found = true;
            student_pos = ftell(file) - sizeof(students);
            break;
        }
    }

    if (!student_found) {
        printf("Student ID %s has not been found (>_<)!\n", student_id_s);
        fclose(file);
        return;
    }

    if (strcmp(st.class_attend, no_class) == 0) {
        printf("Student %s has not registered to any classes (>_<)!\n", st.student_name);
        fclose(file);
        return;
    }

    printf("Current Class: %s", st.class_attend);
    printf("Type 'yes' to continue or 'no' to cancel the unregister: ");
    do {
        fflush(stdin);
        fgets(ans, sizeof(ans), stdin);
        ans[ strcspn(ans, "\n") ] = 0;
    if (strcmp(ans, "no") == 0 || strcmp(ans, "No") == 0) {
        printf("The process has been cancelled.\n"
                "Press any key to return.");
        getch();
        return;
    }
    if (strcmp(ans, "yes") == 0 || strcmp(ans, "Yes") == 0) {
        break;
        }
        printf("Invalid Input (>_<)! Please type either yes or no: ");
    } while (true);


    // Unregister the class
    strcpy(class_name_s, st.class_attend); // Copy for confirmation line.
    strcpy(st.class_attend, no_class);
    
    // Update Student Record
    fseek(file, student_pos, SEEK_SET);
    fwrite(&st, sizeof(students), 1, file);

    printf("Successfully removed Student %s from class %s (=^.^=)", st.student_name, class_name_s);

    fclose(file);

    printf("                        (\\(\\  \n");
    printf("Press any key to return ( -.-) \n");
    getch();
    system("cls");
}

void experimental_view_class() {
    FILE *student_file, *class_file;
    students st;
    classes  cl;
    char class_id_s[MAX_ID_LENGTH];
    int enrolled_count = 0;
    bool class_found = false;

    student_file = fopen("ex_student.txt", "r");
    class_file = fopen("ex_class.txt", "r");

    if (class_file == NULL || student_file == NULL) {
        printf("Error opening file(s) (>_<)!\n");
        getch();
        return;
    }

    printf("Enter Class ID to view enrollment: ");
    fflush(stdin);
    fgets(class_id_s, MAX_ID_LENGTH, stdin);
    class_id_s[ strcspn(class_id_s, "\n") ] = 0;

    // Find class
    while (fread(&cl, sizeof(classes), 1, class_file) == 1) {
        if (strcmp(cl.class_id, class_id_s) == 0) {
            class_found = true;
            break;
        }
    }

    if (!class_found) {
        printf("Class ID %s has not been found (>_<)!\n", class_id_s);
        fclose(student_file);
        fclose(class_file);
        getch();
        return;
    }

    while (fread(&st, sizeof(students), 1, student_file) == 1) {
        if (strcmp(st.class_attend, cl.class_name) == 0) {
            enrolled_count++;
            printf("%-5d %-15s %-30s\n", enrolled_count, st.student_id, st.student_name);
        }
    }

    printf("\nTotal number of students in class %s: %d students.\n", cl.class_name, enrolled_count);

    fclose(student_file);
    fclose(class_file);

    printf("                        (\\(\\  \n");
    printf("Press any key to return ( -.-) \n");
    getch();
    system("cls");
}

void experimental_calculate_tuition() {
    FILE *student_file, *class_file;
    students st;
    classes  cl;
    char student_id_s[MAX_ID_LENGTH];
    float total_tuition = 0;
    bool student_found = false;

    student_file = fopen("ex_student.txt", "r");

    if (student_file == NULL || class_file == NULL) {
        printf("Error opening file(s) (>_<)!\n");
        getch();
        return;
    }
    printf("Enter Student ID: ");
    fflush(stdin);
    fgets(student_id_s, MAX_ID_LENGTH, stdin);
    student_id_s[ strcspn(student_id_s, "\n") ] = 0;

    // Find Student
    while (fread(&st, sizeof(students), 1, student_file) == 1) {
        if (strcmp(st.student_id, student_id_s) == 0) {
            student_found = true;
            break;
        }
    }

    if (!student_found) {
        printf("Student ID %s has not been found (>~<)!\n", student_id_s);
        fclose(student_file);
        getch();
        return;
    }
    
    class_file = fopen("ex_class.txt", "r");

    printf("\nStudent %s's Tuition Fee:\n", st.student_name);
    printf("%-30s %-15s\n", "Subject", "Tuition Fee");
    printf("----------------------------------------\n");

    // Tuition Fee's Calculation
    while(fread(&cl, sizeof(classes), 1, class_file) == 1) {
        if (strcmp(st.class_attend, cl.class_name) == 0) {
            printf("%-30s %-15d\n", cl.class_name, cl.tuition);
            total_tuition = cl.tuition;
            break;
        }
    }

    printf("----------------------------------------\n");
    printf("%-30s %-15.2f\n", "Total Tuition: ", total_tuition);
    printf("%-30s %-15.2f\n", "Amount Paid: ", st.tuition_paid);
    if (st.tuition_paid < total_tuition) {
        printf("%-30s %-15.2f\n", "Amount Required Left: ", total_tuition - st.tuition_paid);
    }

    fclose(student_file);
    fclose(class_file);

    printf("                        (\\(\\  \n");
    printf("Press any key to return ( -.-) \n");
    getch();
    system("cls");
}



void sub_student() {
    system("cls");
    int choice_student;
    do {
        printf("\n\t\t ===== Study Center Management System =====\n"
               "\t\t\t === Student Management ===\n\n"
               "\t1: Create a new Student\n"
               "\t2: List out all current student(s)\n"
               "\t3: Check Student's Tuition Fee\n"
               "\t4: Return to Main Menu"
               "\nTo select, please enter the according number from 1 to 4\n");
                
        while (printf("\nChoice: ") && scanf("%d", &choice_student) != 1) {
            while (getchar() != '\n');
            printf("Invalid data type! Please enter numeric data only!\n"
                   "Press any key to return.");
            _getch();
            eraseLines(4);
        }

        switch(choice_student) {
            case 1:
                experimental_insert_student();
                break;
            case 2:
                experimental_print_students_list();
                break;
            case 3:
                experimental_calculate_tuition();
                break;
            case 4:
                break;
            default:
                printf("From 1 to 4 only (>_<)!");
                getch();
                system("cls");
                break;
        }
    } while (choice_student != 4);


    printf("Returning to Main Menu.\n");
    
    printf("                        (\\(\\  \n");
    printf("Press any key to return ( -.-) \n");
    getch();
    system("cls");
    return;
}

void sub_class() {
    int choice_class;
    system("cls");
    do {
        printf("\n\t\t ===== Study Center Management System =====\n"
               "\t\t\t === Class Management ===\n\n"
               "\t1: Create a new Class\n"
               "\t2: List out all existing class(es)\n"
               "\t3: Register a new Student into Class\n"
               "\t4: Remove an old student from Class\n"
               "\t5: View Class's List of Student(s)\n"
               "\t6: Return to Main Menu"
               "\nTo select, please enter the according number from 1 to 6\n");
                
        while (printf("\nChoice: ") && scanf("%d", &choice_class) != 1) {
            while (getchar() != '\n');
            printf("Invalid data type! Please enter number only!\n"
                   "Press any key to return.");
            _getch();
            eraseLines(4);
        }

        switch(choice_class) {
            case 1:
                experimental_insert_class();
                break;
            case 2:
                experimental_print_classes_list();
                break;
            case 3:
                experimental_class_register();
                break;
            case 4:
                experimental_class_unregister();
                break;
            case 5:
                experimental_view_class();
                break;
            case 6:
                printf("Returning to Main Menu.\n");
                
                printf("                        (\\(\\  \n");
                printf("Press any key to return ( -.-) \n");
                getch();
                system("cls");
                break;
            default:
                printf("From 1 to 6 only (>_<)!");
                getch();
                system("cls");
                break;
        }
    } while (choice_class != 6);
    return;
}

main() {
    int choice_main;
    do {
        printf("\n\t\t ===== Student Data Management System =====\n\n"
               "\t1: Student Management\n"
               "\t2: Class Management\n"
               "\t3: Exit System"
               "\nEnter your choice (1 to 3)\n");

        while (printf("\nChoice: ") && scanf("%d", &choice_main) != 1) {
            while (getchar() != '\n');
            printf("Please enter again (>_<)!");
            _getch();
            eraseLines(3);
        }

        switch(choice_main) {
            case 1:
                sub_student();
                break;
            case 2:
                sub_class();
                break;
            case 3:
                printf("Exiting System... (-_-)zzz");
                exit(0);
            default:
                printf("From 1 to 3 only!");
                getch();
                system("cls");
                break;
        }
    } while (choice_main != 3);
}


/* Old Function (no longer in use, 29/10)
void Insert_Students_List() {
    int n, r; // n for new list counter, r for old counter
    int pos = 0; // start of new list counter
    long lsize; // file size check
    char student_id[15];
    char student_name[30];
    students st;
    students temp[100]; // Temp data to save old list
    FILE *write1, *read1, *file2;
    read1 = fopen("students_input.txt", "r");
    file2 = fopen("students.txt", "a");

    printf("\nType the number of students you want to insert: ");
    scanf("%d", &n);

    //Obtain file size
    fseek(read1, 0, SEEK_END);
    lsize = ftell(read1);
    rewind(read1);

    if (lsize == 0) { // If file is empty
        fclose(read1);
    }
    else {
        fread(&r, sizeof(int),1, read1); // Save previous data to temp
        for (int a = 0; a < r; a++) {
            fread(&temp[a], sizeof(students), 1, read1);
            pos++; // move counter from the start to end of old list
            n++; // counter of old and new insert combined
        }
        fclose(read1);
    }

    //Because "w" overwrites ALL data in file, so you have to call it AFTER saving all original data to temp
    //(I wasted 2 hours on this part).
    write1 = fopen("students_input.txt", "w");

    if (write1 == NULL) {
        printf("Khong mo duoc file!");
        exit(0);
    }

    //Despite the repetition, It is also important to rewrite previous data
    //For 'fread' is based on int n (the counter) and "a"(append) does not continue the counter.
    //Do try to find other clean methods that resolve around "a"(append), otherwise this stays the same.
    fwrite(&n, sizeof(int), 1, write1);
    for (int i = 0; i < pos; i++) {
        fwrite(&temp[i], sizeof(students), 1, write1);
    }

    for (pos; pos < n; pos++) { // Continue off from the end of old list
        printf("\n\nEnter Student %d's Information\n", pos+1);

        printf("Enter Student's ID: ");
        fflush(stdin);
        fgets(st.student_id, sizeof(student_id), stdin);
        

        printf("Enter Student's Name: ");
        fflush(stdin);
        fgets(st.student_name, sizeof(student_name), stdin);
        


        fwrite(&st, sizeof(students), 1, write1); // Write first THEN delete null at end of char/string or else strcmp doesn't work.
        st.student_id[strcspn(st.student_id, "\n")] = 0;
        st.student_name[strcspn(st.student_name, "\n")] = 0;
        fprintf(file2, "%d.ID: %s\n| Name: %s\n\n", pos+1, st.student_id, st.student_name); // Readable text file
    }
    fclose(write1);
    fclose(file2);

    printf("Press any key to return.");
    getch();
    system("cls");
}

void Insert_Subjects_list() { // The same as Students List, just different data
    int n, r;
    int pos = 0;
    long lsize;
    char class_id[15];
    char class_name[30];
    int tuition;
    setlocale(LC_NUMERIC, ""); // Number division by 1,000 (depend on system to work or not)
    classes cl;
    classes temp[100];
    FILE *write1, *read1, *file2;
    read1 = fopen("subjects_input.txt", "r");
    file2 = fopen("subjects.txt", "a");

    printf("Type the number of Subject you want to add: ");
    scanf("%d", &n);

    //Obtain file size
    fseek(read1, 0, SEEK_END);
    lsize = ftell(read1);
    rewind(read1);

    if (lsize == 0) { // If file is empty
        fclose(read1);
    }
    else {
        fread(&r, sizeof(int), 1, read1); // Save previous data to temp
        for (int a = 0; a < r; a++) {
            fread(&temp[a], sizeof(classes), 1, read1);
            pos++;
            n++;
        }
        fclose(read1);
    }

    write1 = fopen("subjects_input.txt", "w");

    if (write1 == NULL) {
        printf("Can't open file!");
        exit(0);
    }

    fwrite(&n, sizeof(int), 1, write1);
    for (int i = 0; i < pos; i++) {
        fwrite(&temp[i], sizeof(classes), 1, write1);
    }

    for(pos; pos < n; pos++) {
        printf("\n\nInsert Subject %d's Information\n", pos+1);

        printf("Enter class ID: ");
        fflush(stdin);
        fgets(cl.class_id, sizeof(class_id), stdin);

        printf("Enter Class Name: ");
        fflush(stdin);
        fgets(cl.class_name, sizeof(class_name), stdin);

        printf("Enter Class's Tuition Cost: ");
        scanf("%d", &cl.tuition);

        fwrite(&cl, sizeof(classes), 1, write1); // Write first, delete null after.
        cl.class_id[strcspn(cl.class_id, "\n")] = 0;
        cl.class_name[strcspn(cl.class_name, "\n")] = 0;
        fprintf(file2, "%d.ID:%s\n|Subject: %s\n| Tuition: %'d\n\n", pos+1, cl.class_id, cl.class_name, cl.tuition);
    }
    fclose(write1);
    fclose(file2);

    printf("Press any key to return.");
    _getch();
    system("cls");
}

void Register_Subject() { // Not finished
    int i, n, m, k;
    int s1 = -1; int s2 = -1;
    char search_st[16];
    char search_class[16];
    FILE *file1, *file2, *file3;
    students st[100]; classes cl[100]; check ck;
    file1 = fopen("students_input.txt", "r");
    file2 = fopen("subjects_input.txt", "r");
    check *t = new check();
    check *head = nullptr;

    fread(&n, sizeof(int), 1, file1);
    for (i = 0; i < n; i++) {
        fread(&st[i], sizeof(students), 1, file1);
    }
    fclose(file1);

    fread(&m, sizeof(int), 1 , file2);
    for (i = 0; i < m; i++) {
        fread(&cl[i], sizeof(classes), 1, file2);
    }
    fclose(file2);

    printf("Enter Student's ID: ");
    fflush(stdin);
    fgets(search_st,sizeof(search_st), stdin);
    for (i = 0; i < n; i++) { // Search for the student's ID position
        if (strcmp(st[i].student_id, search_st) == 0) {
            s1 = i; // s is the current student's ID position
            printf("\nSelected ID: %s", st[s1].student_id);
            printf("Student's Name: %s", st[s1].student_name);
        }
    }
    if (s1 < 0) {
        printf("Invalid ID. Press any key to return");
    }
    else {
        printf("\nEnter the Class's ID registering to: ");
        fflush(stdin);
        fgets(search_class, sizeof(search_class), stdin);
        for (i = 0; i < m; i++) { // Search for Class's ID position
            if (strcmp(cl[i].class_id, search_class) == 0) {
                s2 = i; // s is the current Class's ID position
                printf("\nSelected Class: %s", cl[s2].class_id);

            }
        }
        if (s2 < 0) {
            printf("Invalid ID. Press any key to return");
        }
        else {
            // Idea 2: Create a new struct specifically defining
            // student's ID and Class's ID only
            strcpy(ck.id_st_link, st[s1].student_id);
            strcpy(ck.id_class_link, cl[s2].class_id);
            file3 = fopen("link.txt", "w");
            fwrite(&ck, sizeof(check), 1, file3);
            fclose(file3);
        }
    }
    file3 = fopen("link.txt", "r");
    fread(&ck, sizeof(check), 1, file3);
    fclose(file3);
    printf("\n%s", ck.id_st_link);
    printf("\n%s", ck.id_class_link);
    _getch();
}

void Print_Students_List() {
    FILE *file;
    int n;
    students st[100];
    long lsize;
    system("cls");
    file = fopen("ex_student.txt", "rb");

    //Obtain file size
        fread(&n, sizeof(int), 1, file);
        for (int i = 0; i < n; i++) {
            fread(&st[i], sizeof(students), 1, file);
        }
        fclose(file);
        printf("\t\t\tList of Students\n");
        gotoxy(2,3);  printf("STT"); 
        gotoxy(10,3); printf("ID");
        gotoxy(25,3); printf("Full Name");
        for(int i = 0; i < n; i++) {
            gotoxy(2, i+4);  printf("%d", i+1);
            gotoxy(10, i+4); printf("%s", st[i].student_id);
            gotoxy(25, i+4); printf("%s", st[i].student_name);
            gotoxy(50, i+4); printf("%f", st[i].tuition_paid);
            gotoxy(75, i+4); printf("%s", st[i].class_attend);
        }
    
    printf("\n\nPress any key to return.");
    _getch();
    system("cls");
}

void Print_Subjects_List() {
    FILE *file;
    int n;
    classes cl[100];
    long lsize;
    setlocale(LC_NUMERIC, ""); //Number Division by 1,000 (depend on system to work or not)
    system("cls");
    file = fopen("subjects_input.txt", "r");

    //Obtain file size
    fseek(file, 0, SEEK_END);
    lsize = ftell(file);
    rewind(file);

    if (lsize == 0) {
        printf("File is empty.");
    }
    else {
        fread(&n, sizeof(int), 1, file);
        for (int i = 0; i < n; i++) {
            fread(&cl[i], sizeof(classes), 1, file);
        }
        fclose(file);
        printf("\t\t\tList of Classes\n");
        gotoxy(2,3);  printf("STT");
        gotoxy(10,3); printf("Subject");
        gotoxy(40,3); printf("Tuition");
        for (int i = 0; i < n; i++) {
            gotoxy(2, i+4);  printf("%d", i+1);
            gotoxy(10, i+4); printf("%s", cl[i].class_name);
            gotoxy(40, i+4); printf("%'d", cl[i].tuition);
        }
    }

    printf("\n\nPress any key to return.");
    _getch();
    system("cls");
}
*/