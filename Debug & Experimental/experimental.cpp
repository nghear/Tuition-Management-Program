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

void appendChar (char *str, char *ch) {
    // Find the length of both chars
    int len_str = strlen(str);
    int len_ch = strlen(ch);

    // Place the character at the end
    for (int i = 0; i < len_ch; i++) {
        str[len_str + i] = ch[i];
    }
}

#define MAX_ID_LENGTH 15
#define MAX_NAME_LENGTH 30
#define MAX_CLASSNAME_LENGTH 20

typedef struct {
    char student_ID[MAX_ID_LENGTH];
    char student_name[MAX_NAME_LENGTH];
    char student_email[MAX_NAME_LENGTH];    
    int student_number;
    float tuition_paid;
    char class_attend[MAX_CLASSNAME_LENGTH];
} students;

typedef struct { // old classes renamed, same function (30/10)
    char course_ID[MAX_ID_LENGTH];
    char course_name[MAX_NAME_LENGTH];
    int tuition;
    int total_students; // new data type
    int total_class; // new data type
} courses;

typedef struct {
    char class_ID[MAX_ID_LENGTH];
    char course_ID[MAX_ID_LENGTH];
    char student_ID[MAX_ID_LENGTH];
    char student_name[MAX_NAME_LENGTH];
    char class_name[MAX_CLASSNAME_LENGTH];
    int student_no;
} classes;

void experimental_insert_student() { // Added new Student's Information (21/11, N)
    FILE *file;
    students new_student, existing_student;
    int student_count = 0;
    char no_class[MAX_CLASSNAME_LENGTH] = "Not Registered";
    bool ID_found = false;

    //Open file in append and read mode
    file = fopen("ex_student.txt", "a+b");
    if (file == NULL) {
        printf("Error Opening file (>_<)!\n");
        system("cls");
        return;
    }

    // Get new student Information
    system("cls");
    printf("\n\tEntering new Student's Information\n");
    printf("Enter Student's ID: ");
    fflush(stdin);
    fgets(new_student.student_ID, sizeof(new_student.student_ID), stdin);
    new_student.student_ID[ strcspn(new_student.student_ID, "\n") ] = 0;

    //Check if ID already exists
    rewind(file);
    while ( fread(&existing_student, sizeof(students), 1, file) == 1) {
        student_count++;
        if ( strcmp(existing_student.student_ID, new_student.student_ID) == 0) {
            ID_found = true;
            break;
        }
    }

    if (ID_found) {
        printf("Error: Student ID [ %s ] already exists (>_<)!\n", new_student.student_ID);
    }
    else {
        printf("Enter Student's Name: ");
        fflush(stdin);
        fgets(new_student.student_name, sizeof(new_student.student_name), stdin);

        printf("Enter Student's Phone Number: ");
        scanf("%d", &new_student.student_number);

        printf("Enter Student's Email: ");
        fflush(stdin);
        fgets(new_student.student_email, sizeof(new_student.student_email), stdin);

        strcpy(new_student.class_attend, no_class);


        //Write new student to file
        fseek(file, 0, SEEK_END);
        new_student.student_ID[ strcspn(new_student.student_ID, "\n") ] = 0;
        new_student.student_name[ strcspn(new_student.student_name, "\n") ] = 0;
        fwrite(&new_student, sizeof(students), 1, file);
        printf("\nStudent added sucessfully ( =^.^=). Number: %d\n", student_count + 1);
    }

    fclose(file);

    printf("\n");
    printf("                        (\\(\\ \n");
    printf("Press any key to return ( -.-) \n");
    getch();
    system("cls");
}

void experimental_print_students_list() { // Added Student Query for Detailed Information (21/11, N)
    FILE *file;
    students st;
    int Student_Count = 0;
    int Choice_View;
    char ID_Search[MAX_ID_LENGTH];
    bool Search_Check = false;

    // Open the file in binary read mode
    file = fopen("ex_student.txt", "r+b");
    if (file == NULL) {
        printf("Error opening file or file doesn't exist (>_<)!\n");

        printf("\n");
        printf("                        (\\(\\ \n");
        printf("Press any key to return ( -.-) \n");
        getch();
        system("cls");
        return;
    }

    system("cls");  // Clear the screen

    printf("\t\t\tList of Students\n\n");
    printf("%-5s %-15s %-30s %-30s\n", 
           "No.", "ID", "Full Name", "Class");
    printf("--------------------------------------------------------------------------------\n");

    // Read and print each student
    while (fread(&st, sizeof(students), 1, file) == 1) {
        Student_Count++;
        printf("%-5d %-15s %-30s %-30s", 
            Student_Count, st.student_ID, st.student_name, st.class_attend);
        printf("\n");
    }

    if (Student_Count == 0) {
        printf("Cannot Detect any Student (>~<)\n");
    } else {
        printf("\nTotal number of Students: %d\n", Student_Count);

        printf("\n\n1. View Detailed Information Of A Student");
        printf("\n2. Return");

        printf("\n\nEnter your choice (1-2) : ");

        while (scanf("%d", &Choice_View) != 1) {
            while (getchar() != '\n');
            printf("\t  Invalid input (>_<)! Please enter a number (1-2): ");
            _getch();
            eraseLines(2);
        }
        switch(Choice_View) {
            case 1:
            do {
                printf("\nEnter Student's ID to look: ");
                fflush(stdin);
                fgets(ID_Search, sizeof(ID_Search), stdin);
                ID_Search[ strcspn(ID_Search, "\n") ] = 0;
                rewind(file);
                while (fread (&st, sizeof(students), 1, file) == 1) {
                    if (strcmp(ID_Search, st.student_ID) == 0) {
                        printf("\n\n%-30s %-30s", "ID", st.student_ID);
                        printf("\n%-30s %-30s", "Full Name", st.student_name);
                        printf("\n%-30s %-10d", "Phone Number", st.student_number);
                        printf("\n%-30s %-30s", "Email", st.student_email);
                        printf("\n%-30s %-30s", "Class", st.class_attend);
                        printf("\n%-30s %-10f", "Tuition Paid", st.tuition_paid);
                        Search_Check = true;
                    }
                }
                if (!Search_Check) {
                    printf("\nStudent ID [ %s ] was not found.", ID_Search);
                }
                printf("\n\nTo do another detailed search, press 'y'.");
                printf("\nPress any other key to return.");
            } while (getche() == 'y');
            case 2:
            break;
        }
    }

    fclose(file);

    printf("\n");
    printf("                        (\\(\\ \n");
    printf("Press any key to return ( -.-) \n");
    getch();
    system("cls");
}

void experimental_insert_course() {
    FILE *file;
    courses new_course, existing_course;
    int course_count = 0;
    bool course_found = false;

    // Open the file in append and read mode
    file = fopen("ex_course.txt","a+b");
    if (file == NULL) {
        printf("Error Opening File (>_<)!\n");
        return;
    }

    // Get new class information
    printf("\n\tEntering new Course's Information\n");
    printf("Enter Course ID: ");
    fflush(stdin);
    fgets(new_course.course_ID, sizeof(new_course.course_ID), stdin);
    new_course.course_ID[ strcspn(new_course.course_ID,"\n") ] = 0;

    // Check if Course ID already exist
    rewind(file);
    while ( fread(&existing_course, sizeof(courses), 1, file) == 1) {
        course_count++;
        if (strcmp(existing_course.course_ID, new_course.course_ID) == 0) {
            course_found = true;
            break;
        }
    }

    if (course_found) {
        printf("Error: Course ID [ %s ] already exists (>~<)!\n", new_course.course_ID);
    }
    else {
        printf("Enter Course's Name: ");
        fflush(stdin);
        fgets(new_course.course_name, sizeof(new_course.course_name), stdin);

        printf("Enter Course's Tuition Cost: ");
        scanf("%d", &new_course.tuition);

        new_course.total_students = 0;
        new_course.total_class = 0;

        //Write new class to file
        fseek(file, 0, SEEK_END);
        new_course.course_ID[ strcspn(new_course.course_ID, "\n") ] = 0;
        new_course.course_name[ strcspn(new_course.course_name, "\n") ] = 0;
        fwrite(&new_course, sizeof(courses), 1, file);

        printf("\nCourse added sucessfully ( =^.^=). Number: %d\n", course_count + 1);
    }

    fclose(file);

    printf("\n");
    printf("                        (\\(\\ \n");
    printf("Press any key to return ( -.-) \n");
    getch();
    system("cls");
}

void experimental_print_course_list() {
    FILE *file;
    courses cs;
    int course_count = 0;

    //Open the file in binary read mode
    file = fopen("ex_course.txt", "r+b");
    if (file == NULL) {
        printf("Error opening file or file doesn't exist (>_<)!\n");

        printf("\n");
        printf("                        (\\(\\ \n");
        printf("Press any key to return ( -.-) \n");
        getch();
        system("cls");
        return;
    }

    system("cls");  // Clear the screen

    printf("\t\t\tList of Course(s)\n\n");
    printf("%-5s %-15s %-30s %-15s\n", 
           "No.", "Course ID", "Course Name", "Tuition Cost");
    printf("----------------------------------------------------------\n");

    // Read and print each subject
    while (fread(&cs, sizeof(courses), 1, file) == 1) {
        course_count++;
        printf("%-5d %-15s %-30s %-15d\n", 
            course_count, cs.course_ID, cs.course_name, cs.tuition);
    }

    if (course_count == 0) {
        printf("Cannot detect any Course (>~<).\n");
    } else {
        printf("\nTotal number of Courses: %d\n", course_count);
    }

    fclose(file);

    printf("\n");
    printf("                        (\\(\\ \n");
    printf("Press any key to return ( -.-) \n");
    getch();
    system("cls");
}

void experimental_class_register() { // that one bug was fixed by using binary open (Forgor to mention this, 2/11, N)
    FILE *student_file, *course_file, *class_file;
    students st;
    courses cs;
    classes cl;
    char student_ID_s[MAX_ID_LENGTH];
    char course_ID_s[MAX_ID_LENGTH];
    char ans[5];
    bool student_found = false;
    bool class_check = false;
    bool course_found = false;
    long int student_pos;
    long int course_pos;

    // Open files
    student_file = fopen("ex_student.txt", "r+b");
    course_file = fopen("ex_course.txt", "r+b");
    class_file = fopen("ex_class.txt", "a+b");
    
    if (student_file == NULL || course_file == NULL || class_file == NULL) {
        printf("Error opening file(s) (>_<)!\n");
        system("cls");
        return;
    }

    // Get student ID
    printf("Enter Student ID to register: ");
    fflush(stdin);
    fgets(student_ID_s, sizeof(student_ID_s), stdin);
    student_ID_s[ strcspn(student_ID_s, "\n") ] = 0;

    //Find student
    while (fread(&st, sizeof(students), 1, student_file) == 1) {
        if (strcmp(st.student_ID, student_ID_s) == 0) {
            student_found = true;
            student_pos = ftell(student_file) - sizeof(students);
            break;
        }
    }

    if (!student_found) {
        printf("Student [ %s ] not found (>~<)!\n", student_ID_s);
        fclose(student_file);
        fclose(course_file);
        fclose(class_file);
        system("cls");
        return;
    }

    // Check if student has already assigned for a class
    while (fread(&cs, sizeof(courses), 1, course_file) == 1) {
        if (strcmp(st.class_attend, cs.course_name) == 0) {
            class_check = true;
            break;
        }
    }

    if (class_check) {
        printf("\n\tStudent has already been registered to course [ %s ] \n", st.class_attend);
        printf("\tWould you like to re-register to a different course?\n"
                "Type 'yes' to continue or 'no' to cancel: ");
        do {
            fflush(stdin);
            fgets(ans, sizeof(ans), stdin);
            ans[ strcspn(ans, "\n") ] = 0;
            if (strcmp(ans, "no") == 0 || strcmp(ans, "No") == 0 || strcmp(ans, "NO") == 0 || strcmp(ans, "nO") == 0) {
                printf("\nThe process has been cancelled.\n"
                        "Press any key to return.\n");
                getch();
                system("cls");
                return;
            }

            if (strcmp(ans, "yes") == 0 || strcmp(ans, "Yes") == 0 || strcmp(ans, "YES") == 0 || strcmp(ans, "yEs") == 0) {
                break;
            }
            printf("Invalid Input! Please type either yes or no: ");
        } while (true);

    }

    // Get Class ID
    printf("\nEnter Course's ID to register for: ");
    fflush(stdin);
    fgets(course_ID_s, sizeof(course_ID_s), stdin);
    course_ID_s[ strcspn(course_ID_s, "\n") ] = 0;

    // Find Course
    rewind(course_file);
    while (fread(&cs, sizeof(courses), 1, course_file) == 1) {
        if (strcmp(cs.course_ID, course_ID_s) == 0) {
            course_found = true;
            course_pos = ftell(course_file) - sizeof(courses);
            break;
        }
    }

    if (!course_found) {
        printf("There is no course with the ID [ %s ] (>~<)!\n", course_ID_s);
    }

    else if (strcmp(st.class_attend, cs.course_name) == 0) {
        printf("Student [ %s ] is already registered for course [ %s ] (As stated from previous check already (>~<)!)",
                st.student_name, cs.course_name);
    }

    else {
        // Register Process (30/10)
        strcpy(st.class_attend, cs.course_name);
        st.class_attend[ strcspn(st.class_attend, "\n") ] = 0;

            // Get new Total Student(s) & Total Class(es)
        cs.total_students = cs.total_students + 1;
        if (cs.total_students > 10) {
            cs.total_class = (cs.total_students / 10) + 1;
        }
        else {
            cs.total_class = 1;
        }

            // convert number of total class into char
        int length = snprintf(NULL, 0, "%d", cs.total_class); // Length required to convert
        char* count = (char*)malloc(length + 1); // Allocate size to new char for convert (+1 for null at END)
        snprintf(count, length + 1, "%d", cs.total_class);
        char holder[3] = "_0";
        holder[ strcspn(holder, "\n") ] = 0;

            // Get Course ID, Student ID, Student Name
        strcpy(cl.course_ID, cs.course_ID);
        strcpy(cl.student_ID, st.student_ID);
        strcpy(cl.student_name, st.student_name);

            // Get new Class Name
        strcpy(cl.class_name, cs.course_ID); // Copy Class Name into Course ID (ex: BAS)
        appendChar(cl.class_name, holder); // yippie no ISO C++ warning
        appendChar(cl.class_name, count); // Append class number into Class Name (ex: if cs.total_class = 1 then BAS_01)
        cl.class_name[ strcspn(cl.class_name, "\n") ] = 0;

        // Update Class Record
        fwrite(&cl, sizeof(classes), 1, class_file);

        // Update Student Record
        fseek(student_file, student_pos, SEEK_SET);
        fwrite(&st, sizeof(students), 1, student_file);
    
        // Update Course Record
        fseek(course_file, course_pos, SEEK_SET);
        fwrite(&cs, sizeof(courses), 1, course_file);

        printf("\nSuccessfully Registered Student %s for class %s ( =^.^=)!\n", st.student_name, st.class_attend);
    }

    fclose(student_file);
    fclose(course_file);
    fclose(class_file);

    printf("\n");
    printf("                        (\\(\\ \n");
    printf("Press any key to return ( -.-) \n");
    getch();
    system("cls");
}

void experimental_class_unregister() { // Is not up-to-date, currently only delete from ex_student.txt (30/10, N)
    FILE *student_file, *course_file, *class_file;
    students st;
    courses cs;
    classes cl[100];
    char student_ID_s[MAX_ID_LENGTH];
    char course_name_s[MAX_CLASSNAME_LENGTH];
    char no_class[MAX_CLASSNAME_LENGTH] = "Not Registered";
    char ans[5];
    bool student_found = false;
    int i, all_students;
    long int student_pos;
    long int course_pos;

    student_file = fopen("ex_student.txt", "r+b");
    course_file = fopen("ex_course.txt","r+b");
    class_file = fopen("ex_class.txt", "rb");
    
    if (student_file == NULL || course_file == NULL || class_file == NULL) {
        printf("Error opening file (>_<)!\n");
        system("cls");
        return;
    }

    // Get Student ID
    printf("Enter Student ID: ");
    fflush(stdin);
    fgets(student_ID_s, MAX_CLASSNAME_LENGTH, stdin);
    student_ID_s[ strcspn(student_ID_s, "\n") ] = 0;

    // Find Student Position
    while (fread(&st, sizeof(students), 1, student_file) == 1) {
        if (strcmp(st.student_ID, student_ID_s) == 0) {
            student_found = true;
            student_pos = ftell(student_file) - sizeof(students);
            break;
        }
    }

    if (!student_found) {
        printf("Student ID %s has not been found (>_<)!\n", student_ID_s);
    }

    else if (strcmp(st.class_attend, no_class) == 0) {
        printf("Student %s has not registered to any classes (>~<)!\n", st.student_name);
    }

    else {
        printf("\nCurrent Class: %s", st.class_attend);
        printf("\nType 'yes' to continue or 'no' to cancel the unregister: ");
        do {
            fflush(stdin);
            fgets(ans, sizeof(ans), stdin);
            ans[ strcspn(ans, "\n") ] = 0;
            if (strcmp(ans, "no") == 0 || strcmp(ans, "No") == 0 || strcmp(ans, "NO") == 0 || strcmp(ans, "nO") == 0) {
                printf("\nThe process has been cancelled.\n"
                       "Press any key to return.\n");
                getch();
                system("cls");
                return;
            }
            if (strcmp(ans, "yes") == 0 || strcmp(ans, "Yes") == 0 || strcmp(ans, "YES") == 0 || strcmp(ans, "yEs") == 0) {
            break;
            }
        printf("\nInvalid Input (>~<)! Please type either yes or no: ");
        } while (true);

        // Count all Students That are registered
        all_students = 0;
        while (fread(&cs, sizeof(courses), 1, course_file) == 1) {
            all_students = all_students + cs.total_students;
        }
        rewind(course_file);

        // Find Course Position
        while (fread(&cs, sizeof(courses), 1, course_file) == 1) {
            if (strcmp(cs.course_name, st.class_attend) == 0) {
                course_pos = ftell(course_file) - sizeof(courses);
                break;
            }
        }

        for (i = 0; i < all_students; i++) {
            fread(&cl[i], sizeof(classes), 1, class_file);
        }
        rewind(class_file);

        // Unregister the class
        strcpy(course_name_s, st.class_attend); // Copy for confirmation line.
        strcpy(st.class_attend, no_class);


        cs.total_students = cs.total_students - 1;
        if (cs.total_students % 10 == 0) {
            cs.total_class = cs.total_class - 1;
        }
        
        // Update Class Record
        fclose(class_file);
        class_file = fopen("ex_class.txt", "wb");
        for (i = 0; i < all_students; i++) {
            if (strcmp(st.student_ID, cl[i].student_ID) != 0) {
                fwrite(&cl[i], sizeof(classes), 1, class_file);
            }
        }

        // Update Student Record
        fseek(student_file, student_pos, SEEK_SET);
        fwrite(&st, sizeof(students), 1, student_file);

        // Update Course Record
        fseek(course_file, course_pos, SEEK_SET);
        fwrite(&cs, sizeof(courses), 1, course_file);

        printf("\nSuccessfully removed Student %s from class %s ( =^.^=)!\n", st.student_name, course_name_s);
    }

    fclose(student_file);
    fclose(course_file);
    fclose(class_file);
    printf("\n");
    printf("                        (\\(\\ \n");
    printf("Press any key to return ( -.-) \n");
    getch();
    system("cls");
}

void experimental_view_class() { // Implemented Student, Course, Class (30/10, N)
    FILE *class_file, *course_file;
    classes cl;
    courses  cs;
    char course_ID_s[MAX_ID_LENGTH];
    int enrolled_count = 0;
    bool course_found = false;

    class_file = fopen("ex_class.txt", "rb");
    course_file = fopen("ex_course.txt", "rb");

    if (course_file == NULL || class_file == NULL) {
        printf("Error opening file(s) (>_<)!\n");
        getch();
        system("cls");
        return;
    }

    printf("Enter Class ID to view enrollment: ");
    fflush(stdin);
    fgets(course_ID_s, MAX_ID_LENGTH, stdin);
    course_ID_s[ strcspn(course_ID_s, "\n") ] = 0;

    // Find class
    while (fread(&cs, sizeof(courses), 1, course_file) == 1) {
        if (strcmp(cs.course_ID, course_ID_s) == 0) {
            course_found = true;
            break;
        }
    }

    if (!course_found) {
        printf("Class ID [ %s ] has not been found (>~<)!\n", course_ID_s);
    }

    else {
        system("cls");
        printf("%-5s %-15s %-30s %-30s\n",
               "No.", "Student-ID", "Full-Name", "Class");
        printf("----------------------------------------------------------\n");
        while (fread(&cl, sizeof(classes), 1, class_file) == 1) {
            if (strcmp(cl.course_ID, cs.course_ID) == 0) {
                enrolled_count++;
                cl.student_no = enrolled_count;
                printf("%-5d %-15s %-30s %-30s\n", cl.student_no, cl.student_ID, cl.student_name, cl.class_name);
                if (enrolled_count % 10 == 0) {
                    printf("----------------------------------------------------------\n");
                }
            }
        }

        for (int i = 0; i < cs.total_class; i++) {
            if (i < cs.total_class - 1) {
                printf("\nTotal Student(s) in Class %s_0%d: %d Student(s)", cs.course_ID, i + 1, 10);
            }
            else {
                printf("\nTotal Student(s) in class %s_0%d: %d Student(s)", cs.course_ID, i + 1, enrolled_count - 10 * i);
            }
        }

        printf("\nTotal Student(s) of enroll for Course %s: %d Student(s).\n", cs.course_name, cs.total_students);
    }

    fclose(class_file);
    fclose(course_file);
    printf("\n");
    printf("                        (\\(\\ \n");
    printf("Press any key to return ( -.-) \n");
    getch();
    system("cls");
}

void experimental_calculate_tuition() {
    FILE *student_file, *course_file;
    students st;
    courses  cs;
    char student_ID_s[MAX_ID_LENGTH];
    char no_class[MAX_CLASSNAME_LENGTH] = "Not Registered";
    float total_tuition = 0;
    int total_semester, total_remain;
    bool student_found = false;

    student_file = fopen("ex_student.txt", "rb");
    course_file = fopen("ex_course.txt", "rb");

    if (student_file == NULL || course_file == NULL) {
        printf("Error opening file(s) (>_<)!\n");
        getch();
        system("cls");
        return;
    }
    printf("Enter Student ID: ");
    fflush(stdin);
    fgets(student_ID_s, MAX_ID_LENGTH, stdin);
    student_ID_s[ strcspn(student_ID_s, "\n") ] = 0;

    // Find Student
    while (fread(&st, sizeof(students), 1, student_file) == 1) {
        if (strcmp(st.student_ID, student_ID_s) == 0) {
            student_found = true;
            break;
        }
    }

    if (!student_found) {
        printf("Student ID [ %s ] has not been found (>~<)!\n", student_ID_s);
    }
    
    else if (strcmp(st.class_attend, no_class) == 0) {
        printf("Student [ %s ] has not register any course (>~<)!\n", st.student_name);
    }

    else {
        system("cls");
        printf("\nStudent %s's Tuition Fee:\n", st.student_name);
        printf("%-30s %-15s\n", "Subject", "Tuition Fee");
        printf("----------------------------------------\n");

        // Tuition Fee's Calculation
        while(fread(&cs, sizeof(courses), 1, course_file) == 1) {
            if (strcmp(st.class_attend, cs.course_name) == 0) {
                printf("%-30s %-15d\n", cs.course_name, cs.tuition);
                total_tuition = cs.tuition;
                total_semester = st.tuition_paid / total_tuition;
                total_remain = st.tuition_paid - (total_tuition * total_semester);
                break;
            }
        }

        printf("----------------------------------------\n");
        printf("%-30s %-15.2f\n", "Total Tuition: ", total_tuition);
        printf("%-30s %-15.2f\n", "Amount Paid: ", st.tuition_paid);

        printf("\n");
        if (st.tuition_paid < total_tuition) {
            printf("%-30s %-15.2f\n", "Amount Required Left: ", total_tuition - st.tuition_paid);
        }
        else if (total_semester > 1) {
            printf("Student %s has paid enough for %d semesters", st.student_name, total_semester);
            if (total_remain > 0) {
                printf("\nStudent %s has overpaid and has %d remain.", st.student_name, total_remain);
            }
        }
        else {
            printf("Student %s has paid the amount required for 1 semester.", st.student_name);
            if (total_remain > 0) {
                printf("\nStudent %s has overpaid and has %d remain.", st.student_name, total_remain);
            }
        }
    }

    fclose(student_file);
    fclose(course_file);
    printf("\n");
    printf("                        (\\(\\ \n");
    printf("Press any key to return ( -.-) \n");
    getch();
    system("cls");
}


//Experimental delete student by ID (delete from ex_student.txt, and unregister from ex_class.txt) (31/10, D)
void experimental_delete_student_ID() {
    FILE *student_file, *temp_student_file, *class_file, *temp_class_file, *course_file;
    students st;
    classes cl;
    courses cs;
    char student_ID_s[MAX_ID_LENGTH];
    char ans[5];
    bool student_found = false;
    bool has_class_files = true;
    int id_count = 0;
    char student_IDs_to_delete[100][MAX_ID_LENGTH];

    // Open all required files
    student_file = fopen("ex_student.txt", "rb");
    temp_student_file = fopen("temp_student.txt", "wb");
    class_file = fopen("ex_class.txt", "rb");
    temp_class_file = fopen("temp_class.txt", "wb");
    course_file = fopen("ex_course.txt", "r+b");

    // Check if student files are opened
    if (student_file == NULL || temp_student_file == NULL) {
        if (student_file) {
            fclose(student_file);
        }
        if (temp_student_file) {
            fclose(temp_student_file);
        }
        printf("Error opening student files (>_<)!\n");
        return;
    }

    // Check optional files (class and course files)
    if (class_file == NULL || temp_class_file == NULL || course_file == NULL) {
        has_class_files = false;
        if (class_file) {
            fclose(class_file);
        }
        if (temp_class_file) {
            fclose(temp_class_file);
        }
        if (course_file) {
            fclose(course_file);
        }
    }

    // Get student ID to delete
    printf("Enter Student ID to delete: ");
    fflush(stdin);
    fgets(student_ID_s, sizeof(student_ID_s), stdin);
    student_ID_s[strcspn(student_ID_s, "\n")] = 0;

    // Check if student exists and get confirmation
    while (fread(&st, sizeof(students), 1, student_file) == 1) {
        if (strcmp(st.student_ID, student_ID_s) == 0) {
            student_found = true;
            printf("\nStudent found:\nID: %s\nName: %s\nClass: %s\n", 
                   st.student_ID, st.student_name, st.class_attend);
            printf("\nAre you sure you want to delete this student?\n"
                   "Type 'yes' to continue or 'no' to cancel: ");
            do {
                fflush(stdin);
                fgets(ans, sizeof(ans), stdin);
                ans[strcspn(ans, "\n")] = 0;
                if (strcmp(ans, "no") == 0 || strcmp(ans, "No") == 0 || 
                    strcmp(ans, "NO") == 0 || strcmp(ans, "nO") == 0) {
                    printf("\nDeletion cancelled.\n");
                    fclose(student_file);
                    fclose(temp_student_file);
                    fclose(class_file);
                    fclose(temp_class_file);
                    fclose(course_file);
                    remove("temp_student.txt");
                    remove("temp_class.txt");
                    return;
                }
                if (strcmp(ans, "yes") == 0 || strcmp(ans, "Yes") == 0 || strcmp(ans, "YES") == 0 || strcmp(ans, "yEs") == 0) {
                    break;
                }
                printf("Invalid Input! Please type either yes or no: ");
            } while (true);
            break;
        }
    }

    if (!student_found) {
        printf("Student ID [ %s ] not found (>_<)!\n", student_ID_s);
        fclose(student_file);
        fclose(temp_student_file);
        fclose(class_file);
        fclose(temp_class_file);
        fclose(course_file);
        remove("temp_student.txt");
        remove("temp_class.txt");
        return;
    }

    // Copy all records except the one to be deleted
    rewind(student_file);
    while (fread(&st, sizeof(students), 1, student_file) == 1) {
        if (strcmp(st.student_ID, student_ID_s) != 0) {
            fwrite(&st, sizeof(students), 1, temp_student_file);
        }
    }

    // Update class records and course total students only if we have access to class files
    if (has_class_files) {
        while (fread(&cl, sizeof(classes), 1, class_file) == 1) {
            bool should_delete = false;
            for (int i = 0; i < id_count; i++) {
                if (strcmp(cl.student_ID, student_IDs_to_delete[i]) == 0) {
                    should_delete = true;
                    // Update course total students
                    rewind(course_file);
                    while (fread(&cs, sizeof(courses), 1, course_file) == 1) {
                        if (strcmp(cs.course_ID, cl.course_ID) == 0) {
                            cs.total_students--;
                            cs.total_class = (cs.total_students + 9) / 10;
                            if (fseek(course_file, -(long)sizeof(courses), SEEK_CUR) == 0) {
                                fwrite(&cs, sizeof(courses), 1, course_file);
                            }
                            break;
                        }
                    }
                    break;
                }
            }
            if (!should_delete) {
                fwrite(&cl, sizeof(classes), 1, temp_class_file);
            }
        }

        // Close class-related files
        fclose(class_file);
        fclose(temp_class_file);
        fclose(course_file);

        // Replace class file
        remove("ex_class.txt");
        rename("temp_class.txt", "ex_class.txt");
    }

    // Close all files
    fclose(student_file);
    fclose(temp_student_file);
    fclose(class_file);
    fclose(temp_class_file);
    fclose(course_file);

    // Replace original files with temp files
    remove("ex_student.txt");
    rename("temp_student.txt", "ex_student.txt");
    remove("ex_class.txt");
    rename("temp_class.txt", "ex_class.txt");

    printf("\nStudent successfully deleted ( =^.^=)!\n");

    printf("\n");
    printf("                        (\\(\\ \n");
    printf("Press any key to return ( -.-) \n");
    getch();
    system("cls");
}

//Experimental delete student by name (delete from ex_student.txt, decrease count in course and unregister from ex_class.txt) (31/10, D)
void experimental_delete_student_name() { //same as ID 
    FILE *student_file, *temp_student_file, *class_file, *temp_class_file, *course_file;
    students st;
    classes cl;
    courses cs;
    char student_name_s[MAX_NAME_LENGTH];
    char student_IDs_to_delete[100][MAX_ID_LENGTH];
    int id_count = 0;
    char ans[5];
    bool student_found = false;
    int deleted_count = 0;

    student_file = fopen("ex_student.txt", "rb");
    temp_student_file = fopen("temp_student.txt", "wb");
    class_file = fopen("ex_class.txt", "rb");
    temp_class_file = fopen("temp_class.txt", "wb");
    course_file = fopen("ex_course.txt", "r+b");

    if (student_file == NULL || temp_student_file == NULL) {
        if (student_file) {
            fclose(student_file);
        }
        if (temp_student_file) {
            fclose(temp_student_file);
        }
        printf("Error opening student files (>_<)!\n");
        return;
    }

    bool has_class_files = true;
    if (class_file == NULL || temp_class_file == NULL || course_file == NULL) {
        has_class_files = false;
        if (class_file) {
            fclose(class_file);
        }
        if (temp_class_file) {
            fclose(temp_class_file);
        }
        if (course_file) {
            fclose(course_file);
        }
    }

    printf("Enter Student Name to delete: ");
    fflush(stdin);
    fgets(student_name_s, sizeof(student_name_s), stdin);
    student_name_s[strcspn(student_name_s, "\n")] = 0;

    while (fread(&st, sizeof(students), 1, student_file) == 1) {
        if (strcmp(st.student_name, student_name_s) == 0) {
            student_found = true;
            strcpy(student_IDs_to_delete[id_count], st.student_ID);
            id_count++;
            printf("\nFound student:\nID: %s\nName: %s\nClass: %s\n", 
                   st.student_ID, st.student_name, st.class_attend);
        }
    }

    if (!student_found) {
        printf("No students found with name [ %s ] (>_<)!\n", student_name_s);
        fclose(student_file);
        fclose(temp_student_file);
        if (has_class_files) {
            fclose(class_file);
            fclose(temp_class_file);
            fclose(course_file);
        }
        remove("temp_student.txt");
        if (has_class_files) {
            remove("temp_class.txt");
        }
        return;
    }

    printf("\nFound %d student(s) with this name. Are you sure you want to delete?\n"
           "Type 'yes' to continue or 'no' to cancel: ", id_count);
    do {
        fflush(stdin);
        fgets(ans, sizeof(ans), stdin);
        ans[strcspn(ans, "\n")] = 0;
        if (strcmp(ans, "no") == 0 || strcmp(ans, "No") == 0 || 
            strcmp(ans, "NO") == 0 || strcmp(ans, "nO") == 0) {
            printf("\nDeletion cancelled.\n");
            fclose(student_file);
            fclose(temp_student_file);
            if (has_class_files) {
                fclose(class_file);
                fclose(temp_class_file);
                fclose(course_file);
            }
            remove("temp_student.txt");
            if (has_class_files) {
                remove("temp_class.txt");
            }
            return;
        }
        if (strcmp(ans, "yes") == 0 || strcmp(ans, "Yes") == 0 || 
            strcmp(ans, "YES") == 0 || strcmp(ans, "yEs") == 0) {
            break;
        }
        printf("Invalid Input! Please type either yes or no: ");
    } while (true);

    rewind(student_file);
    while (fread(&st, sizeof(students), 1, student_file) == 1) {
        bool should_delete = false;
        for (int i = 0; i < id_count; i++) {
            if (strcmp(st.student_ID, student_IDs_to_delete[i]) == 0) {
                should_delete = true;
                deleted_count++;
                break;
            }
        }
        if (!should_delete) {
            fwrite(&st, sizeof(students), 1, temp_student_file);
        }
    }

    if (has_class_files) {
        while (fread(&cl, sizeof(classes), 1, class_file) == 1) {
            bool should_delete = false;
            for (int i = 0; i < id_count; i++) {
                if (strcmp(cl.student_ID, student_IDs_to_delete[i]) == 0) {
                    should_delete = true;
                    // Update course total students
                    rewind(course_file);
                    while (fread(&cs, sizeof(courses), 1, course_file) == 1) {
                        if (strcmp(cs.course_ID, cl.course_ID) == 0) {
                            cs.total_students--;
                            cs.total_class = (cs.total_students + 9) / 10;
                            if (fseek(course_file, -(long)sizeof(courses), SEEK_CUR) == 0) {
                                fwrite(&cs, sizeof(courses), 1, course_file);
                            }
                            break;
                        }
                    }
                    break;
                }
            }
            if (!should_delete) {
                fwrite(&cl, sizeof(classes), 1, temp_class_file);
            }
        }
    }

    fclose(student_file);
    fclose(temp_student_file);

    if (has_class_files) {
        fclose(class_file);
        fclose(temp_class_file);
        fclose(course_file);
    }

    remove("ex_student.txt");
    rename("temp_student.txt", "ex_student.txt");
    if (has_class_files) {
        remove("ex_class.txt");
        rename("temp_class.txt", "ex_class.txt");
    }

    printf("\n%d student(s) successfully deleted ( =^.^=)!\n", deleted_count);

    printf("\n");
    printf("                        (\\(\\ \n");
    printf("Press any key to return ( -.-) \n");
    getch();
    system("cls");
}


//delete course by ID (delete from ex_course.txt, decrease count in class and unregister student from ex_student.txt) (31/10, D)
void experimental_delete_course_ID() {
    FILE *course_file, *temp_course_file, *student_file, *temp_student_file, *class_file, *temp_class_file;
    courses cs;
    students st;
    classes cl;
    char course_ID_s[MAX_ID_LENGTH];
    char ans[5];
    bool course_found = false;
    char no_class[MAX_CLASSNAME_LENGTH] = "Not Registered";
    int deleted_count = 0;

    course_file = fopen("ex_course.txt", "rb");
    temp_course_file = fopen("temp_course.txt", "wb");
    student_file = fopen("ex_student.txt", "rb");
    temp_student_file = fopen("temp_student.txt", "wb");
    class_file = fopen("ex_class.txt", "rb");
    temp_class_file = fopen("temp_class.txt", "wb");

    if (course_file == NULL || temp_course_file == NULL) {
        if (course_file) {
            fclose(course_file);
        }
        if (temp_course_file) {
            fclose(temp_course_file);
        }
        printf("Error opening course files (>_<)!\n");
        return;
    }

    bool has_related_files = true;
    if (student_file == NULL || temp_student_file == NULL || class_file == NULL || temp_class_file == NULL) {
        has_related_files = false;
        if (student_file) {
            fclose(student_file);
        }
        if (temp_student_file) {
            fclose(temp_student_file);
        }
        if (class_file) {
            fclose(class_file);
        }
        if (temp_class_file) {
            fclose(temp_class_file);
        }
    }

    printf("Enter Course ID to delete: ");
    fflush(stdin);
    fgets(course_ID_s, sizeof(course_ID_s), stdin);
    course_ID_s[strcspn(course_ID_s, "\n")] = 0;

    while (fread(&cs, sizeof(courses), 1, course_file) == 1) {
        if (strcmp(cs.course_ID, course_ID_s) == 0) {
            course_found = true;
            printf("\nFound course:\nID: %s\nName: %s\nTuition: %d\n"
                   "Total Students: %d\nTotal Classes: %d\n", 
                   cs.course_ID, cs.course_name, cs.tuition,
                   cs.total_students, cs.total_class);
            break;
        }
    }

    if (!course_found) {
        printf("Course ID [ %s ] not found (>_<)!\n", course_ID_s);
        fclose(course_file);
        fclose(temp_course_file);
        remove("temp_course.txt");
        if (has_related_files) {
            fclose(student_file);
            fclose(temp_student_file);
            fclose(class_file);
            fclose(temp_class_file);
            remove("temp_student.txt");
            remove("temp_class.txt");
        }
        return;
    }

    printf("\nAre you sure you want to delete this course? (=O-O=)\n"
           "Type 'yes' to continue or 'no' to cancel: ");
    do {
        fflush(stdin);
        fgets(ans, sizeof(ans), stdin);
        ans[strcspn(ans, "\n")] = 0;
        if (strcmp(ans, "no") == 0 || strcmp(ans, "No") == 0 || strcmp(ans, "NO") == 0 || strcmp(ans, "nO") == 0) {
            printf("\nDeletion cancelled.\n");
            fclose(course_file);
            fclose(temp_course_file);
            remove("temp_course.txt");
            if (has_related_files) {
                fclose(student_file);
                fclose(temp_student_file);
                fclose(class_file);
                fclose(temp_class_file);
                remove("temp_student.txt");
                remove("temp_class.txt");
            }
            return;
        }
        if (strcmp(ans, "yes") == 0 || strcmp(ans, "Yes") == 0 || strcmp(ans, "YES") == 0 || strcmp(ans, "yEs") == 0) {
            break;
        }
        printf("Invalid Input! Please type either yes or no: ");
    } while (true);

    rewind(course_file);
    while (fread(&cs, sizeof(courses), 1, course_file) == 1) {
        if (strcmp(cs.course_ID, course_ID_s) != 0) {
            fwrite(&cs, sizeof(courses), 1, temp_course_file);
        }
    }

    if (has_related_files) {
        while (fread(&st, sizeof(students), 1, student_file) == 1) {
            if (strcmp(st.class_attend, cs.course_name) == 0) {
                strcpy(st.class_attend, no_class);
                deleted_count++;
            }
            fwrite(&st, sizeof(students), 1, temp_student_file);
        }

        while (fread(&cl, sizeof(classes), 1, class_file) == 1) {
            if (strcmp(cl.course_ID, course_ID_s) != 0) {
                fwrite(&cl, sizeof(classes), 1, temp_class_file);
            }
        }
    }

    fclose(course_file);
    fclose(temp_course_file);
    if (has_related_files) {
        fclose(student_file);
        fclose(temp_student_file);
        fclose(class_file);
        fclose(temp_class_file);
    }

    remove("ex_course.txt");
    rename("temp_course.txt", "ex_course.txt");
    if (has_related_files) {
        remove("ex_student.txt");
        rename("temp_student.txt", "ex_student.txt");
        remove("ex_class.txt");
        rename("temp_class.txt", "ex_class.txt");
    }

    printf("\nCourse successfully deleted ( =^.^=)!\n");
    if (has_related_files && deleted_count > 0) {
        printf("%d student(s) have been unregistered from the course.\n", deleted_count);
    }

    printf("\n");
    printf("                        (\\(\\ \n");
    printf("Press any key to return ( -.-) \n");
    getch();
    system("cls");
}

void sub_student() {
    system("cls");
    int choice_student;
    do {
        printf("\n");
        printf("\t+========================================================+\n");
        printf("\t|          Study Center Management System                |\n");
        printf("\t|========================================================|\n");
        printf("\t|                  Student Management                    |\n");
        printf("\t+========================================================+\n\n");
        printf("\n\t\tSelect your choice:\n\n");
        printf("\t\t* 1: Create a new Student\n");
        printf("\t\t* 2: List out all current student(s)\n");
        printf("\t\t* 3: Check Student's Tuition Fee\n");
        printf("\t\t* 4: Delete a Student by ID\n");
        printf("\t\t* 5: Delete a Student by Name\n");
        printf("\t\t* 6: Return to Main Menu\n");
        printf("\t+------------------------------------------------------+\n");
        printf("\t  Enter your choice (1-6): ");

        while (scanf("%d", &choice_student) != 1) {
            while (getchar() != '\n');
            printf("\t  (!_!) Invalid input! Please enter a number (1-6): ");
            _getch();
            eraseLines(2);
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
                experimental_delete_student_ID();
                break;
            case 5:
                experimental_delete_student_name();
                break;
            case 6:
                printf("\nReturning to Main Menu.\n"
                       "Press any key to return ( ='.'=) \n");
                getch();
                system("cls");
                break;
            default:
                printf("From 1 to 6 only (>~<)!\n");
                getch();
                system("cls");
                break;
        }
    } while (choice_student != 6);
    return;
}

void sub_class() {
    int choice_class;
    system("cls");
    do {
        printf("\n");
        printf("\t+========================================================+\n");
        printf("\t|          Study Center Management System                |\n");
        printf("\t|========================================================|\n");
        printf("\t|                  Class Management                      |\n");
        printf("\t+========================================================+\n\n");
        printf("\n\t\tSelect your choice:\n\n");
        printf("\t\t* 1: Create a new Course\n"
               "\t\t* 2: List out all existing Course(s)\n"
               "\t\t* 3: Register a new Student into Class\n"
               "\t\t* 4: Remove an old student from Class\n"
               "\t\t* 5: View Class's List of Student(s)\n"
               "\t\t* 6: Delete a Course by ID\n"
               "\t\t* 7: Return to Main Menu\n");
        printf("\t+--------------------------------------------------------+\n");
        printf("\t  Enter your choice (1-7): ");

        while (scanf("%d", &choice_class) != 1) {
            while (getchar() != '\n');
            printf("\t  (!_!) Invalid input! Please enter a number (1-7): ");
            _getch();
            eraseLines(2);
        }

        switch(choice_class) {
            case 1:
                experimental_insert_course();
                break;
            case 2:
                experimental_print_course_list();
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
                experimental_delete_course_ID();
                break;
            case 7:
                printf("\nReturning to Main Menu.\n"
                       "Press any key to return ( ='.'=) \n");
                getch();
                system("cls");
                break;
            default:
                printf("From 1 to 7 only (>~<)!\n");
                getch();
                system("cls");
                break;
        }
    } while (choice_class != 7);
    return;
}

//menu for head admin
void sub_head_admin() {
    int choice_head_admin;
    system("cls");
    do {
        printf("\n");
        printf("\t+========================================================+\n");
        printf("\t|          Study Center Management System                |\n");
        printf("\t|========================================================|\n");
        printf("\t|                  Student Management                    |\n");
        printf("\t+========================================================+\n\n");
        printf("\n\t\tSelect your choice:\n\n");
        printf("\t\t* 1: Student related\n"
               "\t\t* 2: Class related\n"
               "\t\t* 3: Return to Main Menu\n");
        printf("\t+--------------------------------------------------------+\n");
        printf("\t  Enter your choice (1-3): ");

        while (scanf("%d", &choice_head_admin) != 1) {
            while (getchar() != '\n');
            printf("\t  (!_!) Invalid input! Please enter a number (1-3): ");
            _getch();
            eraseLines(2);
        }
    
        switch(choice_head_admin) {
            case 1:
                sub_student();
                break;
            case 2:
                sub_class();
                break;
            case 3:
                printf("\nReturning to Main Menu.\n"
                       "Press any key to return ( ='.'=) \n");
                getch();
                system("cls");
                break;  
            default:
                printf("From 1 to 3 only (>_<)!\n");
                getch();
                system("cls");
                break;
        }
    } while (choice_head_admin != 3);
    return;
}

main() {
    int choice_main;
    do {
        printf("\n\t+=======================================================+\n");
        printf("\t|          Extracurricular Center Management System     |\n");
        printf("\t|=======================================================|\n");
        printf("\t|                      Main Menu                        |\n");
        printf("\t+=======================================================+\n\n");
        printf("\t\tSelect your role:\n\n");
        printf("\t\t* 1: Head Admin\n");
        printf("\t\t* 2: Accountant\n");
        printf("\t\t* 3: Student Management Officer\n");
        printf("\t\t* 4: Exit System\n");
        printf("\t+-------------------------------------------------------+\n");
        printf("\t  Enter your choice (1-4): ");

        while (scanf("%d", &choice_main) != 1) {
            while (getchar() != '\n');
            printf("\t  Invalid input (>_<)! Please enter a number (1-4): ");
            _getch();
            eraseLines(2);
        }

        switch(choice_main) {
            case 1:
                sub_head_admin();
                break;
            case 2:
                //sub_accountant();
                break;
            case 3:
                //sub_student_management_officer();
                break;
            case 4: 
                printf("\nExiting System... (-_-)zzz\n");
                exit(0);
            default:
                printf("\t  Invalid input (>_<)! Please select options 1-4 only!\n");
                getch();
                system("cls");
                break;
        }
    } while (choice_main != 4);
}