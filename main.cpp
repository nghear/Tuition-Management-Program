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
#define MAX_NAME_LENGTH 200
#define MAX_CLASSNAME_LENGTH 200

typedef struct {
    char student_id[MAX_ID_LENGTH];
    char student_name[MAX_NAME_LENGTH];
    float tuition_paid;
    char class_attend[MAX_CLASSNAME_LENGTH];
} students;

typedef struct { // old classes renamed, same function (30/10)
    char course_id[MAX_ID_LENGTH];
    char course_name[MAX_CLASSNAME_LENGTH];
    int tuition;
    int total_students; // new data type 
    int total_class; // new data type
} courses;

typedef struct {
    char course_id[MAX_ID_LENGTH];
    char student_id[MAX_ID_LENGTH];
    char student_name[MAX_NAME_LENGTH];
    char class_name[MAX_CLASSNAME_LENGTH];
    int student_no;
} classes;

void experimental_insert_student() {
    FILE *file;
    students new_student, existing_student;
    int student_count = 0;
    char no_class[MAX_CLASSNAME_LENGTH] = "Not Registered";
    bool id_found = false;

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
    fgets(new_student.student_id, sizeof(new_student.student_id), stdin);
    new_student.student_id[ strcspn(new_student.student_id, "\n") ] = 0;

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
        printf("Error: Student ID [ %s ] already exists (>_<)!\n", new_student.student_id);
    }
    else {
        printf("Enter Student's Name: ");
        fflush(stdin);
        fgets(new_student.student_name, sizeof(new_student.student_name), stdin);

        printf("Enter the amount that the Student has paid: ");
        scanf("%f", &new_student.tuition_paid);

        strcpy(new_student.class_attend, no_class);

        //Write new student to file
        fseek(file, 0, SEEK_END);
        new_student.student_id[ strcspn(new_student.student_id, "\n") ] = 0;
        new_student.student_name[ strcspn(new_student.student_name, "\n") ] = 0;
        fwrite(&new_student, sizeof(students), 1, file);
        printf("\nStudent added sucessfully ( =^.^=). Number: %d\n", student_count + 1);
    }

    fclose(file);

    printf("                        (\\(\\ \n");
    printf("Press any key to return ( -.-) \n");
    getch();
    system("cls");
}

void experimental_print_students_list() {
    FILE *file;
    students st;
    int student_count = 0;

    // Open the file in binary read mode
    file = fopen("ex_student.txt", "r+b");
    if (file == NULL) {
        printf("Error opening file or file doesn't exist (>_<)!\n");

        printf("                        (\\(\\ \n");
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
        printf("Cannot Detect any Student (>~<)\n");
    } else {
        printf("\nTotal number of Students: %d\n", student_count);
    }

    fclose(file);

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
    fgets(new_course.course_id, sizeof(new_course.course_id), stdin);
    new_course.course_id[ strcspn(new_course.course_id,"\n") ] = 0;

    // Check if Course ID already exist
    rewind(file);
    while ( fread(&existing_course, sizeof(courses), 1, file) == 1) {
        course_count++;
        if (strcmp(existing_course.course_id, new_course.course_id) == 0) {
            course_found = true;
            break;
        }
    }

    if (course_found) {
        printf("Error: Course ID [ %s ] already exists (>~<)!\n", new_course.course_id);
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
        new_course.course_id[ strcspn(new_course.course_id, "\n") ] = 0;
        new_course.course_name[ strcspn(new_course.course_name, "\n") ] = 0;
        fwrite(&new_course, sizeof(courses), 1, file);

        printf("\nCourse added sucessfully ( =^.^=). Number: %d\n", course_count + 1);
    }

    fclose(file);

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
            course_count, cs.course_id, cs.course_name, cs.tuition);
    }

    if (course_count == 0) {
        printf("Cannot detect any Course (>~<).\n");
    } else {
        printf("\nTotal number of Courses: %d\n", course_count);
    }

    fclose(file);

    printf("                        (\\(\\ \n");
    printf("Press any key to return ( -.-) \n");
    getch();
    system("cls");
}

void experimental_class_register() { // Still has that weird extra 1 key bug (30/10, N)
    FILE *student_file, *course_file, *class_file;
    students st;
    courses cs;
    classes cl;
    char student_id_s[MAX_ID_LENGTH];
    char course_id_s[MAX_ID_LENGTH];
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
        printf("Student [ %s ] not found (>~<)!\n", student_id_s);
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
    fgets(course_id_s, sizeof(course_id_s), stdin);
    course_id_s[ strcspn(course_id_s, "\n") ] = 0;

    // Find Course
    rewind(course_file);
    while (fread(&cs, sizeof(courses), 1, course_file) == 1) {
        if (strcmp(cs.course_id, course_id_s) == 0) {
            course_found = true;
            course_pos = ftell(course_file) - sizeof(courses);
            break;
        }
    }

    if (!course_found) {
        printf("There is no course with the ID [ %s ] (>~<)!\n", course_id_s);
    }

    else if (strcmp(st.class_attend, cs.course_name) == 0) {
        printf("Student [ %s ] is already registered for course [ %s ] (As stated from previous check already (>~<)!)",
                st.student_name, cs.course_name);
    }

    else {
        // Register Process (30/10)
        strcpy(st.class_attend, cs.course_name);

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

            // Get Course ID, Student ID, Student Name
        strcpy(cl.course_id, cs.course_id);
        strcpy(cl.student_id, st.student_id);
        strcpy(cl.student_name, st.student_name);

            // Get new Class Name
        strcpy(cl.class_name, cs.course_id); // Copy Class Name into Course ID (ex: BAS)
        appendChar(cl.class_name, "_0"); // Append into Class Name, C++ will give warning as direct value is unsafe (ex: BAS_0)
        appendChar(cl.class_name, count); // Append class number into Class Name (ex: if cs.total_class = 1 then BAS_01)
        cl.class_name[ strcspn(cl.class_name, "\n") ] = 0;

        // Update Class Record
        fwrite(&cl, sizeof(classes), 1, class_file);

        // Update Student Record
        fseek(student_file, student_pos, SEEK_SET);
        fwrite(&st, sizeof(students), 1, student_file);
    
        // Update Course Record
        fseek(course_file, course_pos, SEEK_SET);
        fwrite(&cs, sizeof(courses), 1, course_file); // You're here

        printf("\nSuccessfully Registered Student %s for class %s ( =^.^=)!\n", st.student_name, st.class_attend);
    }

    fclose(student_file);
    fclose(course_file);
    fclose(class_file);

    printf("                        (\\(\\ \n");
    printf("Press any key to return ( -.-) \n");
    getch();
    system("cls");
}

void experimental_class_unregister() { // Is not up-to-date, currently only delete from ex_student.txt (30/10, N)
    FILE *file;
    students st;
    char student_id_s[MAX_ID_LENGTH];
    char course_name_s[MAX_CLASSNAME_LENGTH];
    char no_class[MAX_CLASSNAME_LENGTH] = "Not Registered";
    char ans[5];
    bool student_found = false;
    long student_pos;

    file = fopen("ex_student.txt", "r+b");
    if (file == NULL) {
        printf("Error opening file (>_<)!\n");
        system("cls");
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

        // Unregister the class
        strcpy(course_name_s, st.class_attend); // Copy for confirmation line.
        strcpy(st.class_attend, no_class);
    
        // Update Student Record
        fseek(file, student_pos, SEEK_SET);
        fwrite(&st, sizeof(students), 1, file);

        printf("\nSuccessfully removed Student %s from class %s ( =^.^=)!\n", st.student_name, course_name_s);
    }

    fclose(file);
    printf("                        (\\(\\ \n");
    printf("Press any key to return ( -.-) \n");
    getch();
    system("cls");
}

void experimental_view_class() { // Implemented Student, Course, Class (30/10, N)
    FILE *class_file, *course_file;
    classes cl;
    courses  cs;
    char course_id_s[MAX_ID_LENGTH];
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
    fgets(course_id_s, MAX_ID_LENGTH, stdin);
    course_id_s[ strcspn(course_id_s, "\n") ] = 0;

    // Find class
    while (fread(&cs, sizeof(courses), 1, course_file) == 1) {
        if (strcmp(cs.course_id, course_id_s) == 0) {
            course_found = true;
            break;
        }
    }

    if (!course_found) {
        printf("Class ID [ %s ] has not been found (>~<)!\n", course_id_s);
    }

    else {
        system("cls");
        printf("%-5s %-15s %-30s %-30s\n",
               "No.", "Student-ID", "Full-Name", "Class");
        printf("----------------------------------------------------------\n");
        while (fread(&cl, sizeof(classes), 1, class_file) == 1) {
            if (strcmp(cl.course_id, cs.course_id) == 0) {
                enrolled_count++;
                cl.student_no = enrolled_count;
                printf("%-5d %-15s %-30s %-30s\n", cl.student_no, cl.student_id, cl.student_name, cl.class_name);
                if (enrolled_count % 10 == 0) {
                    printf("----------------------------------------------------------\n");
                }
            }
        }

        for (int i = 0; i < cs.total_class; i++) {
            if (i < cs.total_class - 1) {
                printf("\nTotal Student(s) in Class %s_0%d: %d Student(s)", cs.course_id, i + 1, 10);
            }
            else {
                printf("\nTotal Student(s) in class %s_0%d: %d Student(s)", cs.course_id, i + 1, enrolled_count - 10 * i);
            }
        }

        printf("\nTotal Student(s) of enroll for Course %s: %d Student(s).\n", cs.course_name, cs.total_students);
    }

    fclose(class_file);
    fclose(course_file);
    printf("                        (\\(\\ \n");
    printf("Press any key to return ( -.-) \n");
    getch();
    system("cls");
}

void experimental_calculate_tuition() {
    FILE *student_file, *course_file;
    students st;
    courses  cs;
    char student_id_s[MAX_ID_LENGTH];
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
        printf("Student ID [ %s ] has not been found (>~<)!\n", student_id_s);
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
    printf("                        (\\(\\ \n");
    printf("Press any key to return ( -.-) \n");
    getch();
    system("cls");
}

//Experimental delete student by ID (delete from ex_student.txt, and unregister from ex_class.txt) (30/10, D)
void experimental_delete_student_ID() {
    FILE *student_file, *temp_student_file, *class_file, *temp_class_file, *course_file;
    students st;
    classes cl;
    courses cs;
    char student_id_s[MAX_ID_LENGTH];
    char ans[5];
    bool student_found = false;
    bool needs_update = false;
    bool has_class_files = true;
    int id_count = 0;
    char student_ids_to_delete[100][MAX_ID_LENGTH];

    // Open all required files
    student_file = fopen("ex_student.txt", "rb");
    temp_student_file = fopen("temp_student.txt", "wb");
    class_file = fopen("ex_class.txt", "rb");
    temp_class_file = fopen("temp_class.txt", "wb");
    course_file = fopen("ex_course.txt", "r+b");

    // Check if student files are opened
    if (student_file == NULL || temp_student_file == NULL) {
        if (student_file) fclose(student_file);
        if (temp_student_file) fclose(temp_student_file);
        printf("Error opening student files (>_<)!\n");
        return;
    }

    // Check optional files (class and course files)
    if (class_file == NULL || temp_class_file == NULL || course_file == NULL) {
        has_class_files = false;
        if (class_file) fclose(class_file);
        if (temp_class_file) fclose(temp_class_file);
        if (course_file) fclose(course_file);
    }

    // Get student ID to delete
    printf("Enter Student ID to delete: ");
    fflush(stdin);
    fgets(student_id_s, sizeof(student_id_s), stdin);
    student_id_s[strcspn(student_id_s, "\n")] = 0;

    // Check if student exists and get confirmation
    while (fread(&st, sizeof(students), 1, student_file) == 1) {
        if (strcmp(st.student_id, student_id_s) == 0) {
            student_found = true;
            printf("\nStudent found:\nID: %s\nName: %s\nClass: %s\n", 
                   st.student_id, st.student_name, st.class_attend);
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
                if (strcmp(ans, "yes") == 0 || strcmp(ans, "Yes") == 0 || 
                    strcmp(ans, "YES") == 0 || strcmp(ans, "yEs") == 0) {
                    break;
                }
                printf("Invalid Input! Please type either yes or no: ");
            } while (true);
            break;
        }
    }

    if (!student_found) {
        printf("Student ID [ %s ] not found (>_<)!\n", student_id_s);
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
        if (strcmp(st.student_id, student_id_s) != 0) {
            fwrite(&st, sizeof(students), 1, temp_student_file);
        }
    }

    // Update class records and course total students only if we have access to class files
    if (has_class_files) {
        while (fread(&cl, sizeof(classes), 1, class_file) == 1) {
            bool should_delete = false;
            for (int i = 0; i < id_count; i++) {
                if (strcmp(cl.student_id, student_ids_to_delete[i]) == 0) {
                    should_delete = true;
                    // Update course total students
                    rewind(course_file);
                    while (fread(&cs, sizeof(courses), 1, course_file) == 1) {
                        if (strcmp(cs.course_id, cl.course_id) == 0) {
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

    printf("                        (\\(\\ \n");
    printf("Press any key to return ( -.-) \n");
    getch();
    system("cls");
}

//Experimental delete student by name (delete from ex_student.txt, decrease count in course and unregister from ex_class.txt) (30/10, D)
void experimental_delete_student_name() { //same as ID 
    FILE *student_file, *temp_student_file, *class_file, *temp_class_file, *course_file;
    students st;
    classes cl;
    courses cs;
    char student_name_s[MAX_NAME_LENGTH];
    char student_ids_to_delete[100][MAX_ID_LENGTH];
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
        if (student_file) fclose(student_file);
        if (temp_student_file) fclose(temp_student_file);
        printf("Error opening student files (>_<)!\n");
        return;
    }

    bool has_class_files = true;
    if (class_file == NULL || temp_class_file == NULL || course_file == NULL) {
        has_class_files = false;
        if (class_file) fclose(class_file);
        if (temp_class_file) fclose(temp_class_file);
        if (course_file) fclose(course_file);
    }

    printf("Enter Student Name to delete: ");
    fflush(stdin);
    fgets(student_name_s, sizeof(student_name_s), stdin);
    student_name_s[strcspn(student_name_s, "\n")] = 0;

    while (fread(&st, sizeof(students), 1, student_file) == 1) {
        if (strcmp(st.student_name, student_name_s) == 0) {
            student_found = true;
            strcpy(student_ids_to_delete[id_count], st.student_id);
            id_count++;
            printf("\nFound student:\nID: %s\nName: %s\nClass: %s\n", 
                   st.student_id, st.student_name, st.class_attend);
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
            if (strcmp(st.student_id, student_ids_to_delete[i]) == 0) {
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
                if (strcmp(cl.student_id, student_ids_to_delete[i]) == 0) {
                    should_delete = true;
                    // Update course total students
                    rewind(course_file);
                    while (fread(&cs, sizeof(courses), 1, course_file) == 1) {
                        if (strcmp(cs.course_id, cl.course_id) == 0) {
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

    printf("                        (\\(\\ \n");
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
               "\t4: Delete a Student by ID\n"
               "\t5: Delete a Student by Name\n"
               "\t6: Return to Main Menu"
               "\nTo select, please enter the according number from 1 to 6\n");
                
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
        printf("\n\t\t ===== Study Center Management System =====\n"
               "\t\t\t === Class Management ===\n\n"
               "\t1: Create a new Course\n"
               "\t2: List out all existing Course(s)\n"
               "\t3: Register a new Student into Class\n"
               "\t4: Remove an old student from Class\n"
               "\t5: View Class's List of Student(s)\n"
               "\t6: Return to Main Menu"
               "\nTo select, please enter the according number from 1 to 6\n");
                
        while (printf("\nChoice: ") && scanf("%d", &choice_class) != 1) {
            while (getchar() != '\n');
            printf("Invalid data type! Please enter number only!\n"
                   "Press any key to return ( ='.'=) \n");
            _getch();
            eraseLines(4);
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
            printf("Please enter again (>~<)!\n");
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
                printf("Exiting System... (-_-)zzz\n");
                exit(0);
            default:
                printf("From 1 to 3 only (>_<)!\n");
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
        printf("Khong mo duoc file (>_<)!");
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
    char course_id[15];
    char course_name[30];
    int tuition;
    setlocale(LC_NUMERIC, ""); // Number division by 1,000 (depend on system to work or not)
    courses cs;
    courses temp[100];
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
            fread(&temp[a], sizeof(courses), 1, read1);
            pos++;
            n++;
        }
        fclose(read1);
    }

    write1 = fopen("subjects_input.txt", "w");

    if (write1 == NULL) {
        printf("Can't open file (>_<)!");
        exit(0);
    }

    fwrite(&n, sizeof(int), 1, write1);
    for (int i = 0; i < pos; i++) {
        fwrite(&temp[i], sizeof(courses), 1, write1);
    }

    for(pos; pos < n; pos++) {
        printf("\n\nInsert Subject %d's Information\n", pos+1);

        printf("Enter class ID: ");
        fflush(stdin);
        fgets(cs.course_id, sizeof(course_id), stdin);

        printf("Enter Class Name: ");
        fflush(stdin);
        fgets(cs.course_name, sizeof(course_name), stdin);

        printf("Enter Class's Tuition Cost: ");
        scanf("%d", &cs.tuition);

        fwrite(&cs, sizeof(courses), 1, write1); // Write first, delete null after.
        cs.course_id[strcspn(cs.course_id, "\n")] = 0;
        cs.course_name[strcspn(cs.course_name, "\n")] = 0;
        fprintf(file2, "%d.ID:%s\n|Subject: %s\n| Tuition: %'d\n\n", pos+1, cs.course_id, cs.course_name, cs.tuition);
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
    students st[100]; courses cs[100]; check ck;
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
        fread(&cs[i], sizeof(courses), 1, file2);
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
            if (strcmp(cs[i].course_id, search_class) == 0) {
                s2 = i; // s is the current Class's ID position
                printf("\nSelected Class: %s", cs[s2].course_id);

            }
        }
        if (s2 < 0) {
            printf("Invalid ID. Press any key to return");
        }
        else {
            // Idea 2: Create a new struct specifically defining
            // student's ID and Class's ID only
            strcpy(ck.id_st_link, st[s1].student_id);
            strcpy(ck.id_class_link, cs[s2].course_id);
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
    courses cs[100];
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
            fread(&cs[i], sizeof(courses), 1, file);
        }
        fclose(file);
        printf("\t\t\tList of Classes\n");
        gotoxy(2,3);  printf("STT");
        gotoxy(10,3); printf("Subject");
        gotoxy(40,3); printf("Tuition");
        for (int i = 0; i < n; i++) {
            gotoxy(2, i+4);  printf("%d", i+1);
            gotoxy(10, i+4); printf("%s", cs[i].course_name);
            gotoxy(40, i+4); printf("%'d", cs[i].tuition);
        }
    }

    printf("\n\nPress any key to return.");
    _getch();
    system("cls");
}
*/
