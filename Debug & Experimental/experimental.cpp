#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <conio.h>
#include <windows.h>
#include <iomanip>
#include <limits>
#include <cstdlib>
#include <cstring>
#include <locale.h>

#define MAX_ID_LENGTH 15
#define MAX_NAME_LENGTH 50
#define MAX_CLASSNAME_LENGTH 50
#define MAX_ADDRESS_LENGTH 50
#define MAX_ATTENDING_CLASSES 10

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

typedef struct {
    char id[MAX_ID_LENGTH];
    char name[MAX_NAME_LENGTH];
    int mobile_number;
    char address[MAX_ADDRESS_LENGTH];
    float tuitionpaid;
    char attending_classes[MAX_ATTENDING_CLASSES][MAX_CLASSNAME_LENGTH]; // Increased to 100 classes
    int num_attending_classes;
} student;

typedef struct {
    char class_id[MAX_ID_LENGTH];
    char class_name[MAX_CLASSNAME_LENGTH];
    int tuition;
} subject;

// not up to date with structure (28/10) refer to experimental insert
void Insert_Students_List() { 
    int n, r; // n for new list counter, r for old counter
    int pos = 0; // start of new list counter
    long lsize; // file size check
    char id[MAX_ID_LENGTH];
    char name[MAX_NAME_LENGTH];
    student st;
    student temp[100]; // Temp data to save old list
    FILE *write1, *read1, *file2;
    read1 = fopen("students_input.txt", "r");
    file2 = fopen("students.txt", "a");

    printf("\nType the number of student you want to insert: ");
    scanf("%d", &n);

    // Obtain file size
    fseek(read1, 0, SEEK_END);
    lsize = ftell(read1);
    rewind(read1);

    if (lsize == 0) { //If file is empty
        fclose(read1);
    }
    else {
        fread(&r, sizeof(int),1, read1); //Save previous data to temp
        for (int a = 0; a < r; a++) {
            fread(&temp[a], sizeof(student), 1, read1);
            pos++; // move counter from the start to end of old list
            n++; // counter of old and new insert combined
        }
        fclose(read1);
    }

    // because "w" overwrites ALL data in file, so you have to call it AFTER saving all original data to temp
    // (I wasted 2 hours on this part).
    write1 = fopen("students_input.txt", "w");

    if (write1 == NULL) {
        printf("Khong mo duoc file!");
        exit(0);
    }

    //Despite the repetition, It is also important to rewrite previous data
    //For 'fread' is based on int n (the counter) and "a"(append) does not continue the counter.
    //Do try to find other clean methods that resolve around "a"(append) 
    fwrite(&n, sizeof(int), 1, write1);
    for (int i = 0; i < pos; i++) {
        fwrite(&temp[i], sizeof(student), 1, write1);
    }

    for (pos; pos < n; pos++) { // Continue off from the end of old list
        printf("\n\nEnter Student %d's Information\n", pos+1);

        printf("Enter Student's ID: ");
        fflush(stdin);
        fgets(st.id, sizeof(id), stdin);
        

        printf("Enter Student's Name: ");
        fflush(stdin);
        fgets(st.name, sizeof(name), stdin);
        

        st.id[strcspn(st.id, "\n")] = 0;
        st.name[strcspn(st.name, "\n")] = 0;
        fwrite(&st, sizeof(student), 1, write1);
        fprintf(file2, "%d.ID: %s\n| Name: %s\n\n", pos+1, st.id, st.name); // Readable text file
    }
    fclose(write1);
    fclose(file2);

    printf("Press any key to return.");
    getch();
    system("cls");
}


//add addr and mobile number
void experimental_insert_student() { 
    FILE *file;
    student new_student, existing_student;
    int student_count = 0;
    bool id_exists = false;

    // Open the file in append and read mode
    file = fopen("ex_student.txt", "a+");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    // Get new student information
    printf("\nEnter Student Information\n");
    printf("Enter Student's ID: ");
    fflush(stdin);
    fgets(new_student.id, sizeof(new_student.id), stdin);
    new_student.id[strcspn(new_student.id, "\n")] = 0;

    // Check if ID already exists
    rewind(file);
    while (fread(&existing_student, sizeof(student), 1, file) == 1) {
        student_count++;
        if (strcmp(existing_student.id, new_student.id) == 0) {
            id_exists = true;
            break;
        }
    }

    if (id_exists) {
        printf("Error: Student with ID %s already exists (>_<)!\n", new_student.id);
    } else {
        printf("Enter Student's Name: ");
        fflush(stdin);
        fgets(new_student.name, sizeof(new_student.name), stdin);
        new_student.name[strcspn(new_student.name, "\n")] = 0;

        printf("Enter Student's Mobile Number: ");
        scanf("%d", &new_student.mobile_number);

        printf("Enter Student's Address: ");
        fflush(stdin);
        fgets(new_student.address, sizeof(new_student.address), stdin);
        new_student.address[strcspn(new_student.address, "\n")] = 0;

        printf("Enter Tuition Paid: ");
        scanf("%f", &new_student.tuitionpaid);

        // Initialize attending classes
        new_student.num_attending_classes = 0;
        for (int i = 0; i < MAX_ATTENDING_CLASSES; i++) {
            new_student.attending_classes[i][0] = '\0';
        }

        // Write the new student to the file
        fseek(file, 0, SEEK_END);
        fwrite(&new_student, sizeof(student), 1, file);

        printf("Student added successfully. Number: %d\n", student_count + 1);
    }

    fclose(file);
    printf("                         (\\(\\ \n");
    printf("Press any key to return. ( -.-)");
    getch();
    system("cls");
}


//add address and mobile number
void experimental_print_student_list() {
    FILE *file;
    student st;
    int student_count = 0;

    // Open the file in binary read mode
    file = fopen("ex_student.txt", "rb");
    if (file == NULL) {
        printf("Error opening file or file doesn't exist!\n");
        printf("Press any key to return.");
        getch();
        system("cls");
        return;
    }

    system("cls");  // Clear the screen

    printf("\t\t\tList of Students\n\n");
    printf("%-5s %-15s %-30s %-15s %-30s %-15s %-30s\n", 
           "No.", "ID", "Full Name", "Mobile", "Address", "Tuition Paid", "Attending Classes");
    printf("--------------------------------------------------------------------------------------------------------\n");

    // Read and print each student
    while (fread(&st, sizeof(student), 1, file) == 1) {
        student_count++;
        printf("%-5d %-15s %-30s %-15d %-30s %-15.2f ", 
            student_count, st.id, st.name, st.mobile_number, st.address, st.tuitionpaid);
        
        // Print attending classes
        if (st.num_attending_classes > 0) {
            printf("%s", st.attending_classes[0]);
            for (int i = 1; i < st.num_attending_classes; i++) {
                printf(", %s", st.attending_classes[i]);
            }
        } else {
            printf("None");
        }
        printf("\n");
    }

    if (student_count == 0) {
        printf("No students found in the file.\n");
    } else {
        printf("\nTotal number of students: %d\n", student_count);
    }

    fclose(file);

    printf("\nPress any key to return.");
    getch();
    system("cls");
}

// Experimental delete student by ID based on experimental insert (27/10)
void experimental_delete_student_by_id() {
    FILE *file, *temp_file;
    student st;
    char delete_id[15];
    int found = 0;

    // Open the original file for reading
    file = fopen("ex_student.txt", "r");
    if (file == NULL) {
        printf("Error opening file or file doesn't exist!\n");
        return;
    }

    // Open a temporary file for writing
    temp_file = fopen("temp_ex_student.txt", "w");
    if (temp_file == NULL) {
        printf("Error creating temporary file!\n");
        fclose(file);
        return;
    }

    // Get the ID of the student to delete
    printf("Enter Student's ID to delete: ");
    fflush(stdin);
    fgets(delete_id, sizeof(delete_id), stdin);
    delete_id[strcspn(delete_id, "\n")] = 0;

    // Read each student from the original file
    while (fread(&st, sizeof(student), 1, file) == 1) {
        if (strcmp(st.id, delete_id) == 0) {
            found = 1;
            printf("Student with ID %s has been deleted.\n", delete_id);
        } else {
            // Write to temp file if it's not the student to be deleted
            fwrite(&st, sizeof(student), 1, temp_file);
        }
    }

    // Close both files
    fclose(file);
    fclose(temp_file);

    // Delete the original file and rename the temp file
    if (found) {
        remove("ex_student.txt");
        rename("temp_ex_student.txt", "ex_student.txt");
    } else {
        remove("temp_ex_student.txt");
        printf("Student with ID %s not found.\n", delete_id);
    }

    printf("                         (\\(\\ \n");
    printf("Press any key to return. ( -.-)");
    getch();
    system("cls");
}

// Experimental delete student by name based on experimental insert (27/10)
void experimental_delete_student_by_name() {
    FILE *file, *temp_file;
    student st;
    char delete_name[MAX_NAME_LENGTH];
    int found = 0;

    // Open the original file for reading
    file = fopen("ex_student.txt", "r");
    if (file == NULL) {
        printf("Error opening file or file doesn't exist!\n");
        return;
    }

    // Open a temporary file for writing
    temp_file = fopen("temp_ex_student.txt", "w");
    if (temp_file == NULL) {
        printf("Error creating temporary file!\n");
        fclose(file);
        return;
    }

    // Get the name of the student to delete
    printf("Enter Student's Full Name to delete: ");
    fflush(stdin);
    fgets(delete_name, sizeof(delete_name), stdin);
    delete_name[strcspn(delete_name, "\n")] = 0;

    // Read each student from the original file
    while (fread(&st, sizeof(student), 1, file) == 1) {
        if (strcmp(st.name, delete_name) == 0) {
            found = 1;
            printf("Student with name '%s' has been deleted.\n", delete_name);
        } else {
            // Write to temp file if it's not the student to be deleted
            fwrite(&st, sizeof(student), 1, temp_file);
        }
    }

    // Close both files
    fclose(file);
    fclose(temp_file);

    // Delete the original file and rename the temp file
    if (found) {
        remove("ex_student.txt");
        rename("temp_ex_student.txt", "ex_student.txt");
    } else {
        remove("temp_ex_student.txt");
        printf("Student with name '%s' not found.\n", delete_name);
    }

    printf("                         (\\(\\ \n");
    printf("Press any key to return. ( -.-)");
    getch();
    system("cls");
}

void Insert_Subjects_list() { //The same as Students List, just different data
    int n, r;
    int pos = 0;
    long lsize;
    char class_name[MAX_CLASSNAME_LENGTH];
    int tuition;
    setlocale(LC_NUMERIC, ""); //Number division by 1,000 (depend on system to work or not)
    subject sj;
    subject temp[100];
    FILE *write1, *read1, *file2;
    read1 = fopen("subjects_input.txt", "r");
    file2 = fopen("subjects.txt", "a");

    printf("Type the number of Subject you want to add: ");
    scanf("%d", &n);

    // Obtain file size
    fseek(read1, 0, SEEK_END);
    lsize = ftell(read1);
    rewind(read1);

    if (lsize == 0) { //If file is empty
        fclose(read1);
    }
    else {
        fread(&r, sizeof(int), 1, read1); //Save previous data to temp
        for (int a = 0; a < r; a++) {
            fread(&temp[a], sizeof(subject), 1, read1);
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
        fwrite(&temp[i], sizeof(subject), 1, write1);
    }

    for(pos; pos < n; pos++) {
        printf("\n\nInsert Subject %d's Information\n", pos+1);

        printf("Enter Subject: ");
        fflush(stdin);
        fgets(sj.class_name, sizeof(class_name), stdin);

        printf("Enter Subject's Tuition Cost: ");
        scanf("%d", &sj.tuition);

        sj.class_name[strcspn(sj.class_name, "\n")] = 0;
        fwrite(&sj, sizeof(subject), 1, write1);
        fprintf(file2, "%d.Subject: %s\n| Tuition: %'d\n\n", pos+1, sj.class_name, sj.tuition);
    }
    fclose(write1);
    fclose(file2);

    printf("Press any key to return.");
    _getch();
    system("cls");
}

void Register_Subject() { // Not finished
    int i, n;
    char search[15];
    int s = 0;
    FILE *file1;
    FILE *file2;
    student st[100];
    subject sj[100];
    file1 = fopen("students_input.txt", "r");
    file2 = fopen("subjects_input.txt", "r");
    fread(&n, sizeof(int), 1, file1);
    for (i = 0; i < n; i++) {
        fread(&st[i], sizeof(student), 1, file1);
    }
    printf("Enter Student's ID: ");
    fflush(stdin);
    fgets(search,sizeof(search), stdin);
    for (i = 0; i < n; i++) { // Search for the student's ID position
        if (strcmp(st[i].id, search) == 0) {
            s = i; // s is the current student's ID position
        }
    }
    
}

//Experimental Subjects list insert (28/10)
void experimental_insert_subjects_list() {
    FILE *file;
    subject new_subject, existing_subject;
    int subject_count = 0;
    bool subject_exists = false;

    // Open the file in append and read mode
    file = fopen("ex_subject.txt", "a+");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    // Get new class information
    printf("\nEnter Class Information\n");
    printf("Enter Class Name: ");
    fflush(stdin);
    fgets(new_subject.class_name, sizeof(new_subject.class_name), stdin);
    new_subject.class_name[strcspn(new_subject.class_name, "\n")] = 0;

    // Add class ID input
    printf("Enter Class ID: ");
    fflush(stdin);
    fgets(new_subject.class_id, sizeof(new_subject.class_id), stdin);
    new_subject.class_id[strcspn(new_subject.class_id, "\n")] = 0;

    // Check if subject ID already exists
    rewind(file);
    while (fread(&existing_subject, sizeof(subject), 1, file) == 1) {
        subject_count++;
        if (strcmp(existing_subject.class_id, new_subject.class_id) == 0) {
            subject_exists = true;
            break;
        }
    }

    if (subject_exists) {
        printf("Error: ClassID '%s' already exists (>_<)!\n", new_subject.class_id);
    } else {
        printf("Enter Class's Tuition Cost: ");
        scanf("%d", &new_subject.tuition);

        // Write the new subject to the file
        fseek(file, 0, SEEK_END);
        fwrite(&new_subject, sizeof(subject), 1, file);

        printf("Class added successfully. Number: %d\n", subject_count + 1);
    }

    fclose(file);
    printf("                         (\\(\\ \n");
    printf("Press any key to return. ( -.-)");
    getch();
    system("cls");
}

void Print_Students_List() {
    FILE *file;
    int n;
    student st[100];
    long lsize;
    system("cls");
    file = fopen("students_input.txt", "r");

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
            fread(&st[i], sizeof(student), 1, file);
        }
        fclose(file);
        printf("\t\t\tList of Students\n");
        gotoxy(2,3);  printf("STT"); 
        gotoxy(10,3); printf("ID");
        gotoxy(25,3); printf("Full Name");
        for(int i = 0; i < n; i++) {
            gotoxy(2, i+4);  printf("%d", i+1);
            gotoxy(10, i+4); printf("%s", st[i].id);
            gotoxy(25, i+4); printf("%s", st[i].name);
        }
    }
    
    printf("\n\nPress any key to return.");
    _getch();
    system("cls");
}

void Print_Subjects_List() {
    FILE *file;
    int n;
    subject sj[100];
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
            fread(&sj[i], sizeof(subject), 1, file);
        }
        fclose(file);
        printf("\t\t\tList of Classes\n");
        gotoxy(2,3);  printf("STT");
        gotoxy(10,3); printf("Subject");
        gotoxy(40,3); printf("Tuition");
        for (int i = 0; i < n; i++) {
            gotoxy(2, i+4);  printf("%d", i+1);
            gotoxy(10, i+4); printf("%s", sj[i].class_name);
            gotoxy(40, i+4); printf("%'d", sj[i].tuition);
        }
    }

    printf("\n\nPress any key to return.");
    _getch();
    system("cls");
}

//Experimental print subjects list (28/10)
void experimental_print_subjects_list() {
    FILE *file;
    subject sj;
    int subject_count = 0;

    // Open the file in binary read mode
    file = fopen("ex_subject.txt", "rb");
    if (file == NULL) {
        printf("Error opening file or file doesn't exist!\n");
        printf("Press any key to return.");
        getch();
        system("cls");
        return;
    }

    system("cls");  // Clear the screen

    printf("\t\t\tList of Subjects\n\n");
    printf("%-5s %-15s %-30s %-15s\n", 
           "No.", "Subject ID", "Subject Name", "Tuition Cost");
    printf("----------------------------------------------------------\n");

    // Read and print each subject
    while (fread(&sj, sizeof(subject), 1, file) == 1) {
        subject_count++;
        printf("%-5d %-15s %-30s %-15d\n", 
            subject_count, sj.class_id, sj.class_name, sj.tuition);
    }

    if (subject_count == 0) {
        printf("No subjects found in the file.\n");
    } else {
        printf("\nTotal number of subjects: %d\n", subject_count);
    }

    fclose(file);

    printf("\nPress any key to return.");
    getch();
    system("cls");
}

void Delete_Student_By_ID() {
    FILE *file, *temp;
    int n, found = 0;
    char search_id[15];
    student st;

    file = fopen("students_input.txt", "r");
    temp = fopen("temp.txt", "w");

    if (file == NULL || temp == NULL) {
        printf("Error opening files!");
        return;
    }

    printf("Enter Student's ID to delete: ");
    fflush(stdin);
    fgets(search_id, sizeof(search_id), stdin);
    search_id[strcspn(search_id, "\n")] = 0;

    fread(&n, sizeof(int), 1, file);
    int new_n = n - 1;
    fwrite(&new_n, sizeof(int), 1, temp);

    for (int i = 0; i < n; i++) {
        fread(&st, sizeof(student), 1, file);
        if (strcmp(st.id, search_id) != 0) {
            fwrite(&st, sizeof(student), 1, temp);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(temp);

    if (found) {
        remove("students_input.txt");
        rename("temp.txt", "students_input.txt");
        printf("Student with ID %s has been deleted.\n", search_id);
    } else {
        remove("temp.txt");
        printf("Student with ID %s not found.\n", search_id);
    }

    printf("Press any key to return.");
    getch();
    system("cls");
}

void Delete_Student_By_Name() {
    FILE *file, *temp;
    int n, found = 0;
    char search_name[MAX_NAME_LENGTH];
    student st;

    file = fopen("students_input.txt", "r");
    temp = fopen("temp.txt", "w");

    if (file == NULL || temp == NULL) {
        printf("Error opening files!");
        return;
    }

    printf("Enter Student's Full Name to delete: ");
    fflush(stdin);
    fgets(search_name, sizeof(search_name), stdin);
    search_name[strcspn(search_name, "\n")] = 0;

    fread(&n, sizeof(int), 1, file);
    int new_n = n - 1;
    fwrite(&new_n, sizeof(int), 1, temp);

    for (int i = 0; i < n; i++) {
        fread(&st, sizeof(student), 1, file);
        if (strcmp(st.name, search_name) != 0) {
            fwrite(&st, sizeof(student), 1, temp);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(temp);

    if (found) {
        remove("students_input.txt");
        rename("temp.txt", "students_input.txt");
        printf("Student with name %s has been deleted.\n", search_name);
    } else {
        remove("temp.txt");
        printf("Student with name %s not found.\n", search_name);
    }

    printf("Press any key to return.");
    getch();
    system("cls");
}

// Register a student for a class
void experimental_register_student_for_class() {
    FILE *student_file, *subject_file;
    student st;
    subject sj;
    char student_id[MAX_ID_LENGTH];
    char class_id[MAX_ID_LENGTH];
    bool student_found = false;
    bool subject_found = false;
    long student_pos;

    // Open files
    student_file = fopen("ex_student.txt", "r+b");
    subject_file = fopen("ex_subject.txt", "rb");
    
    if (student_file == NULL || subject_file == NULL) {
        printf("Error opening files!\n");
        return;
    }

    // Get student ID
    printf("Enter Student ID to register: ");
    fflush(stdin);
    fgets(student_id, sizeof(student_id), stdin);
    student_id[strcspn(student_id, "\n")] = 0;

    // Find student
    while (fread(&st, sizeof(student), 1, student_file) == 1) {
        if (strcmp(st.id, student_id) == 0) {
            student_found = true;
            student_pos = ftell(student_file) - sizeof(student);
            break;
        }
    }

    if (!student_found) {
        printf("Student not found!\n");
        fclose(student_file);
        fclose(subject_file);
        return;
    }

    // Check if student can register for more classes
    if (st.num_attending_classes >= MAX_ATTENDING_CLASSES) {
        printf("Student has reached maximum number of classes (>_<)!\n");
        fclose(student_file);
        fclose(subject_file);
        return;
    }

    // Get class ID
    printf("Enter Subject ID to register for: ");
    fflush(stdin);
    fgets(class_id, sizeof(class_id), stdin);
    class_id[strcspn(class_id, "\n")] = 0;

    // Find subject
    while (fread(&sj, sizeof(subject), 1, subject_file) == 1) {
        if (strcmp(sj.class_id, class_id) == 0) {
            subject_found = true;
            break;
        }
    }

    if (!subject_found) {
        printf("Subject not found!\n");
        fclose(student_file);
        fclose(subject_file);
        return;
    }

    // Check if student is already registered for this class
    for (int i = 0; i < st.num_attending_classes; i++) {
        if (strcmp(st.attending_classes[i], sj.class_name) == 0) {
            printf("Student is already registered for this class (>_<)!\n");
            fclose(student_file);
            fclose(subject_file);
            return;
        }
    }

    // Register student for class
    strcpy(st.attending_classes[st.num_attending_classes], sj.class_name);
    st.num_attending_classes++;

    // Update student record
    fseek(student_file, student_pos, SEEK_SET);
    fwrite(&st, sizeof(student), 1, student_file);

    printf("Successfully registered student for class!\n");
    
    fclose(student_file);
    fclose(subject_file);
    
    printf("                         (\\(\\ \n");
    printf("Press any key to return. ( -.-)");
    getch();
    system("cls");
}

// Remove a student from a class
void experimental_remove_student_from_class() {
    FILE *file;
    student st;
    char student_id[MAX_ID_LENGTH];
    char class_name[MAX_CLASSNAME_LENGTH];
    bool student_found = false;
    bool class_found = false;
    long student_pos;

    file = fopen("ex_student.txt", "r+b");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Student ID: ");
    fflush(stdin);
    fgets(student_id, sizeof(student_id), stdin);
    student_id[strcspn(student_id, "\n")] = 0;

    // Find student
    while (fread(&st, sizeof(student), 1, file) == 1) {
        if (strcmp(st.id, student_id) == 0) {
            student_found = true;
            student_pos = ftell(file) - sizeof(student);
            break;
        }
    }

    if (!student_found) {
        printf("Student not found!\n");
        fclose(file);
        return;
    }

    if (st.num_attending_classes == 0) {
        printf("Student is not enrolled in any classes!\n");
        fclose(file);
        return;
    }

    // Display current classes
    printf("\nCurrent classes:\n");
    for (int i = 0; i < st.num_attending_classes; i++) {
        printf("%d. %s\n", i + 1, st.attending_classes[i]);
    }

    printf("\nEnter class name to remove: ");
    fflush(stdin);
    fgets(class_name, sizeof(class_name), stdin);
    class_name[strcspn(class_name, "\n")] = 0;

    // Find and remove class
    for (int i = 0; i < st.num_attending_classes; i++) {
        if (strcmp(st.attending_classes[i], class_name) == 0) {
            class_found = true;
            // Shift remaining classes
            for (int j = i; j < st.num_attending_classes - 1; j++) {
                strcpy(st.attending_classes[j], st.attending_classes[j + 1]);
            }
            st.num_attending_classes--;
            break;
        }
    }

    if (!class_found) {
        printf("Class not found in student's schedule!\n");
        fclose(file);
        return;
    }

    // Update student record
    fseek(file, student_pos, SEEK_SET);
    fwrite(&st, sizeof(student), 1, file);
    
    printf("Successfully removed class from student's schedule!\n");
    
    fclose(file);
    
    printf("                         (\\(\\ \n");
    printf("Press any key to return. ( -.-)");
    getch();
    system("cls");
}

// View class enrollment
void experimental_view_class_enrollment() {
    FILE *student_file, *subject_file;
    student st;
    subject sj;
    char class_id[MAX_ID_LENGTH];
    int enrolled_count = 0;
    bool subject_found = false;

    subject_file = fopen("ex_subject.txt", "rb");
    student_file = fopen("ex_student.txt", "rb");
    
    if (subject_file == NULL || student_file == NULL) {
        printf("Error opening files!\n");
        return;
    }

    printf("Enter Subject ID to view enrollment: ");
    fflush(stdin);
    fgets(class_id, sizeof(class_id), stdin);
    class_id[strcspn(class_id, "\n")] = 0;

    // Find subject
    while (fread(&sj, sizeof(subject), 1, subject_file) == 1) {
        if (strcmp(sj.class_id, class_id) == 0) {
            subject_found = true;
            break;
        }
    }

    if (!subject_found) {
        printf("Subject not found!\n");
        fclose(subject_file);
        fclose(student_file);
        return;
    }

    printf("\nEnrolled students in %s:\n", sj.class_name);
    printf("%-5s %-15s %-30s\n", 
           "No.", "ID", "Name");
    printf("------------------------------------------------\n");

    // Find all students enrolled in the class
    while (fread(&st, sizeof(student), 1, student_file) == 1) {
        for (int i = 0; i < st.num_attending_classes; i++) {
            if (strcmp(st.attending_classes[i], sj.class_name) == 0) {
                enrolled_count++;
                printf("%-5d %-15s %-30s\n", 
                    enrolled_count, st.id, st.name);
                break;
            }
        }
    }

    printf("\nTotal enrolled students: %d\n", enrolled_count);

    fclose(subject_file);
    fclose(student_file);
    
    printf("\nPress any key to return.");
    getch();
    system("cls");
}

// Calculate total tuition for a student
void experimental_calculate_student_tuition() {
    FILE *student_file, *subject_file;
    student st;
    subject sj;
    char student_id[MAX_ID_LENGTH];
    float total_tuition = 0;
    bool student_found = false;

    student_file = fopen("ex_student.txt", "rb");
    subject_file = fopen("ex_subject.txt", "rb");
    
    if (student_file == NULL || subject_file == NULL) {
        printf("Error opening files!\n");
        return;
    }

    printf("Enter Student ID to calculate tuition: ");
    fflush(stdin);
    fgets(student_id, sizeof(student_id), stdin);
    student_id[strcspn(student_id, "\n")] = 0;

    // Find student
    while (fread(&st, sizeof(student), 1, student_file) == 1) {
        if (strcmp(st.id, student_id) == 0) {
            student_found = true;
            break;
        }
    }

    if (!student_found) {
        printf("Student not found!\n");
        fclose(student_file);
        fclose(subject_file);
        return;
    }

    printf("\nTuition breakdown for %s:\n", st.name);
    printf("%-30s %-15s\n", "Subject", "Tuition");
    printf("----------------------------------------\n");

    // Calculate tuition for each class
    for (int i = 0; i < st.num_attending_classes; i++) {
        rewind(subject_file);
        while (fread(&sj, sizeof(subject), 1, subject_file) == 1) {
            if (strcmp(sj.class_name, st.attending_classes[i]) == 0) {
                printf("%-30s %-15d\n", sj.class_name, sj.tuition);
                total_tuition += sj.tuition;
                break;
            }
        }
    }

    printf("----------------------------------------\n");
    printf("%-30s %-15.2f\n", "Total Tuition:", total_tuition);
    printf("%-30s %-15.2f\n", "Amount Paid:", st.tuitionpaid);
    printf("%-30s %-15.2f\n", "Balance Due:", total_tuition - st.tuitionpaid);

    fclose(student_file);
    fclose(subject_file);
    
    printf("\nPress any key to return.");
    getch();
    system("cls");
}

main() {
    int c;
    do {
        printf("\n\t\tStudent Data Management System\n\n"
               "\t1: Insert a new List of Students\n"
               "\t2: Insert a new List of Subjects\n"
               "\t3: Print out List of Students\n"
               "\t4: Print out List of Subjects\n"
               "\t5: Register Student for Class\n"
               "\t6: Remove Student from Class\n"
               "\t7: View Class Enrollment\n"
               "\t8: Calculate Student Tuition\n"
               "\t9: Delete Student by ID\n"
               "\t10: Delete Student by Name\n"
               "\t11: Exit System"
               "\nEnter your choice (1 to 11)\n");

        while (printf("\nChoice: ") && scanf("%d", &c) != 1) {
            while (getchar() != '\n');
            printf("Please enter again!");
            _getch();
            eraseLines(3);
        }

        switch(c) {
            case 1:
                //Insert_Students_List();
                experimental_insert_student();
                break;
            case 2:
                //Insert_Subjects_List();
                experimental_insert_subjects_list();
                break;
            case 3:
                //Print_Students_List();
                experimental_print_student_list();
                break;
            case 4:
                //Print_Subjects_List();
                experimental_print_subjects_list();
                break;
            case 5:
                experimental_register_student_for_class();
                break;
            case 6:
                experimental_remove_student_from_class();
                break;
            case 7:
                experimental_view_class_enrollment();
                break;
            case 8:
                experimental_calculate_student_tuition();
                break;
            case 9:
                experimental_delete_student_by_id();
                break;
            case 10:
                experimental_delete_student_by_name();
                break;
            case 11:
                printf("Exiting System... (-_-)zzz");
                exit(0);
            default:
                printf("From 1 to 11 only!");
                getch();
                system("cls");
                break;
        }
    } while (c != 11);
}
