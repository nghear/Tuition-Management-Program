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

void eraseLines(int count) {
    if (count > 0) {
        printf("\x1b[2K"); // Clear current line
        for (int i = 1; i < count; i++) {
            printf("\x1b[1A\x1b[2K");
        }
        printf("\r"); // Move cursor to start of line
    }
}

bool get_course_id(char *course_ID) {
    do {
        system("cls");
        printf("\n");
        printf("\t+========================================================+\n");
        printf("\t|              Study Center Management System            |\n");
        printf("\t|========================================================|\n");
        printf("\t|               Class and Course Management              |\n");
        printf("\t+========================================================+\n\n");
        printf("\t Enter Course ID to delete: ");
        fflush(stdin);
        fgets(course_ID, MAX_ID_LENGTH, stdin);
        course_ID[strcspn(course_ID, "\n")] = 0;
        
        if (strlen(course_ID) == 0) {
            printf("\tError: Course ID cannot be empty (>_<)!\n");
            continue;
        }
        return true;
    } while (true);
    return false;
}

bool find_and_confirm_course(FILE *course_file, const char *course_ID, courses *cs) {
    char confirm[5];
    bool course_found = false;
    
    rewind(course_file);
    while (fread(cs, sizeof(courses), 1, course_file) == 1) {
        if (strcmp(cs->course_ID, course_ID) == 0) {
            course_found = true;
            
            printf("\n\t\tCourse found:\n");
            printf("\n\t\t+-------------------+-------------------+\n");
            printf("\t\t| ID:               | %s\n", cs->course_ID);
            printf("\t\t| Name:             | %s\n", cs->course_name);
            printf("\t\t| Tuition:          | %d\n", cs->tuition);
            printf("\t\t| Total Students:   | %d\n", cs->total_students);
            printf("\t\t| Total Classes:    | %d\n", cs->total_class);
            printf("\t\t+-------------------+-------------------+\n");
            
            printf("\n\t  Are you sure you want to delete this course? (yes/no): ");
            do {
                fflush(stdin);
                fgets(confirm, sizeof(confirm), stdin);
                confirm[strcspn(confirm, "\n")] = 0;
                
                if (strcmp(confirm, "no") == 0 || strcmp(confirm, "No") == 0) {
                    printf("\n\tDeletion cancelled.\n");
                    return false;
                }
                if (strcmp(confirm, "yes") == 0 || strcmp(confirm, "Yes") == 0) {
                    return true;
                }
                printf("\tInvalid input! Please type yes or no: ");
            } while (true);
        }
    }
    
    if (!course_found) {
        printf("\tCourse ID [ %s ] not found (>_<)!\n", course_ID);
    }
    return false;
}

bool process_course_deletion(FILE *files[], const char *course_search, int *deleted_count) {
    courses cs;
    classes cl;
    students st;
    char no_class[MAX_CLASSNAME_LENGTH] = "Not Registered";
    char no_course[MAX_CLASSNAME_LENGTH] = "Not Registered";

    // Copy non-deleted courses
    rewind(files[0]); // course_file
    while (fread(&cs, sizeof(courses), 1, files[0]) == 1) {
        if (strcmp(cs.course_ID, course_search) != 0) {
            fwrite(&cs, sizeof(courses), 1, files[1]); // temp_course_file
        }
    }
    
    // Update class records - remove classes from deleted course
    rewind(files[2]); // class_file
    while (fread(&cl, sizeof(classes), 1, files[2]) == 1) {
        if (strcmp(cl.course_ID, course_search) != 0) {
            fwrite(&cl, sizeof(classes), 1, files[3]); // temp_class_file
        }
    }
    
    // Update student records - unregister from deleted classes
    rewind(files[4]); // student_file
    while (fread(&st, sizeof(students), 1, files[4]) == 1) {
        
        // Check if student is in any class of the deleted course
        rewind(files[2]); // class_file
        while (fread(&cl, sizeof(classes), 1, files[2]) == 1) {
            if (strcmp(cl.course_ID, course_search) == 0 && 
                strcmp(st.class_attend, cl.class_name) == 0) {
                strcpy(st.class_attend, no_class);
                strcpy(st.course_attend, no_course);
                st.tuition_paid = 0;
                (*deleted_count)++;
                break;
            }
        }
        
        fwrite(&st, sizeof(students), 1, files[5]); // temp_student_file
    }
    
    return true;
}

// Add Functions
void student_add() { // Added new Student's Information (21/11, N)
    FILE *file;
    students new_student, existing_student;
    int student_count = 0;
    int phone_length;
    char no_class[MAX_CLASSNAME_LENGTH] = "Not Registered";
    bool ID_found = false;
    char phone_str[MAX_NUMBER_LENGTH];  // For phone input validation

    //Open file in append and read mode
    file = fopen("ex_student.txt", "a+b");
    if (file == NULL) {
        printf("Error Opening file (>_<)!\n");
        return;
    }

    // Get new student Information
    system("cls");
    printf("\n");
    printf("\t+========================================================+\n");
    printf("\t|              Study Center Management System            |\n");
    printf("\t|========================================================|\n");
    printf("\t|                Add Student Information                 |\n");
    printf("\t+========================================================+\n\n");

    do {
        printf("\tEnter Student's ID: ");
        fflush(stdin);
        fgets(new_student.student_ID, sizeof(new_student.student_ID), stdin);
        new_student.student_ID[ strcspn(new_student.student_ID, "\n") ] = 0;
        if (strlen(new_student.student_ID) == 0) {
            printf("\tStudent's ID cannot be empty (>_<)!\n");
        }
    } while (strlen(new_student.student_ID) == 0);

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
        printf("\tError: Student ID [ %s ] already exists (>_<)!\n", new_student.student_ID);
        system("cls");
    }
    else {
        // Student's Name validation
        do
        {
            printf("\tEnter Student's Name: ");
            fflush(stdin);
            fgets(new_student.student_name, sizeof(new_student.student_name), stdin);
            new_student.student_name[strcspn(new_student.student_name, "\n")] = 0;
            if (strlen(new_student.student_name) == 0) {
                printf("\tStudent's Name cannot be empty (>_<)!\n");
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
            
            // Verify all characters are digits
            for(int i = 0; i < phone_length; i++) {
                if(!isdigit(phone_str[i])) {
                    valid = false;
                    break;
                }
            }
            
            if(valid) {
                strcpy(new_student.student_phone, phone_str);
                printf("\tValid phone number format.\n");
                break;
            }
            printf("\tInvalid phone number format (>_<)! Please enter 10 digits (number) only.\n");
            getch();
            eraseLines(4);
        } while(1);

        do {
            printf("\tEnter Student's Email: ");
            fflush(stdin);
            fgets(new_student.student_email, sizeof(new_student.student_email), stdin);
            fflush(stdin);
            new_student.student_email[strcspn(new_student.student_email, "\n")] = 0;
            if (strlen(new_student.student_email) == 0) {
                printf("\tStudent's Email cannot be empty (>_<)!\n");
            } else if (strchr(new_student.student_email, '@') == NULL) {
                printf("\tStudent's Email must contain '@' (>_<)!\n");
            }
        } while (strlen(new_student.student_email) == 0 || strchr(new_student.student_email, '@') == NULL);

        strcpy(new_student.class_attend, no_class);

        strcpy(new_student.course_attend, no_class);

        new_student.tuition_paid = 0;


        //Write new student to file
        fseek(file, 0, SEEK_END);
        new_student.student_name[ strcspn(new_student.student_name, "\n") ] = 0;
        fwrite(&new_student, sizeof(students), 1, file);
        printf("\n\tStudent added successfully ( =^.^=). Number: %d\n", student_count + 1);
    }

    fclose(file);

    printf("\n\n");
    printf("\t                        (\\(\\ \n");
    printf("\tPress any key to return ( -.-) !\n");
    getch();
}

void course_add() { // Minor adjustment (23/11, N)
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
    system("cls");
    printf("\n");
    printf("\t+========================================================+\n");
    printf("\t|              Study Center Management System            |\n");
    printf("\t|========================================================|\n");
    printf("\t|                    Add New Course                      |\n");
    printf("\t+========================================================+\n\n");

    do {
        system("cls");
        printf("\n");
        printf("\t+========================================================+\n");
        printf("\t|              Study Center Management System            |\n");
        printf("\t|========================================================|\n");
        printf("\t|                    Add New Course                      |\n");
        printf("\t+========================================================+\n\n");

        printf("\tEnter Course ID: ");
        fflush(stdin);
        fgets(new_course.course_ID, sizeof(new_course.course_ID), stdin);
        new_course.course_ID[ strcspn(new_course.course_ID, "\n") ] = 0;
        if (strlen(new_course.course_ID) == 0) {
            printf("\tError: Course ID cannot be empty (>_<)!\n");
            getch();
        }
    } while (strlen(new_course.course_ID) == 0);

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
        printf("\tError: Course ID [ %s ] already exists (>~<)!\n", new_course.course_ID);
    }
    else {
        do {
            printf("\tEnter Course's Name: ");
            fflush(stdin);
            fgets(new_course.course_name, sizeof(new_course.course_name), stdin);
            new_course.course_name[ strcspn(new_course.course_name, "\n") ] = 0;
            if (strlen(new_course.course_name) == 0) {
                printf("\tError: Course Name cannot be empty (>_<)!\n");
            }
        } while (strlen(new_course.course_name) == 0);

        do {
            printf("\tEnter Course's Tuition Cost: ");
            while (scanf("%d", &new_course.tuition) != 1) {
                while (getchar() != '\n');
                printf("\t(!_!) Invalid input! Please enter only numeric value!");
                _getch();
                eraseLines(2);
            }
            if (new_course.tuition == 0) {
                printf("\tError: Tuition Cost cannot be empty (>_<)!");
                eraseLines(2);
            }
        } while (new_course.tuition == 0);

        do {
            printf("\tEnter maximum students allowed per class: ");
            scanf("%d", &new_course.max_students);
            if (new_course.max_students <= 0) {
                printf("\tError: Maximum students must be at least 1 or above (>_<)!\n");
                new_course.max_students = 0;
            }
        } while (new_course.max_students == 0);

        new_course.total_class = 0;

        new_course.total_students = 0;

        //Write new class to file
        fseek(file, 0, SEEK_END);
        new_course.course_name[ strcspn(new_course.course_name, "\n") ] = 0;
        fwrite(&new_course, sizeof(courses), 1, file);

        printf("\n\tCourse added successfully ( =^.^=). Number: %d\n", course_count + 1);
    }

    fclose(file);

    printf("\n\n");
    printf("                        (\\(\\ \n");
    printf("Press any key to return ( -.-) \n");
    getch();
}

void class_add() { // Minor fix (25/11, N)
    FILE *course_file, *class_file;
    courses cs;
    classes new_class, existing_class;
    int class_count = 0;
    char course_search[30];
    char new_class_name[MAX_CLASSNAME_LENGTH];
    bool new_name_valid = true;
    bool course_found = false;
    bool class_found = false;
    long int course_pos;

    // Open the file in Append and Read mode
    course_file = fopen("ex_course.txt", "r+b");
    class_file = fopen("ex_class.txt", "a+b");
    if (course_file == NULL || class_file == NULL) {
        printf("Error Opening File (>_<)!\n");
        getch();
        return;
    }

    // Get Course ID
    system("cls");
    printf("\n");
    printf("\t+========================================================+\n");
    printf("\t|              Study Center Management System            |\n");
    printf("\t|========================================================|\n");
    printf("\t|                    Add New Class                       |\n");
    printf("\t+========================================================+\n\n");

    do {
        system("cls");
        printf("\n");
        printf("\t+========================================================+\n");
        printf("\t|              Study Center Management System            |\n");
        printf("\t|========================================================|\n");
        printf("\t|                    Add New Class                       |\n");
        printf("\t+========================================================+\n\n");
        printf("\tEnter Course ID: ");
        fflush(stdin);
        fgets(course_search, sizeof(course_search), stdin);
        course_search[ strcspn(course_search,"\n") ] = 0;
        if (strlen(course_search) == 0) {
            printf("\tError: Course ID cannot be empty (>_<)!\n");
            getch();
        }
    } while (strlen(course_search) == 0);

    // Query for Course
    while ( fread(&cs, sizeof(courses), 1, course_file) == 1) {
        if (strcmp(cs.course_ID, course_search) == 0) {
            course_found = true;
            course_pos = ftell(course_file) - sizeof(courses);
            break;
        }
    }

    // If course don't exist, return.
    if (!course_found) {
        printf("\tError: Course ID [ %s ] was not found (>_<)!\n", course_search);

        fclose(course_file);
        fclose(class_file);

        printf("\n\n");
        printf("                        (\\(\\ \n");
        printf("Press any key to return ( -.-) \n");
        getch();
        return;
    }

    // New Class ID Input
    system("cls");
    printf("\n");
    printf("\t+========================================================+\n");
    printf("\t|              Study Center Management System            |\n");
    printf("\t|========================================================|\n");
    printf("\t|                    Add New Class                       |\n");
    printf("\t+========================================================+\n\n");
    printf("\tCreating a new class for Course [ %s ]\n\n", cs.course_name);

    do {
        printf("\tEnter Class ID: ");
        fflush(stdin);
        fgets(new_class.class_ID, sizeof(new_class.class_ID), stdin);
        new_class.class_ID[ strcspn(new_class.class_ID,"\n") ] = 0;
        if (strlen(new_class.class_ID) == 0) {
            printf("\tError: Class ID cannot be empty (>_<)!\n");
        }
    } while (strlen(new_class.class_ID) == 0);

    // Check If Class ID already exist
    rewind(class_file);
    while ( fread(&existing_class, sizeof(classes), 1, class_file) == 1) {
        class_count++;
        if (strcmp(existing_class.class_ID, new_class.class_ID) == 0) {
            class_found = true;
            break;
        }
    }
    
    rewind(course_file);
    while ( fread(&cs, sizeof(courses), 1, course_file) == 1) {
        if (strcmp(cs.course_ID, new_class.class_ID) == 0) {
            class_found = true;
            break;
        }
    }

    if (class_found) {
        printf("\tError: Class ID [ %s ] already exists (>~<)!\n", new_class.class_ID);
    }
    else {
        //Enter Class's Information
        do { // Loop for class name validation
            new_name_valid = true; // set true to break loop
            do {
                printf("\tEnter Class's Name: ");
                fflush(stdin);
                fgets(new_class_name, sizeof(new_class_name), stdin);
                new_class_name[ strcspn(new_class_name, "\n") ] = 0;
                if (strlen(new_class_name) == 0) {
                    printf("\tClass's Name cannot be empty (>_<)!\n");
                }
            } while (strlen(new_class_name) == 0);

            rewind(class_file);
            while ( fread(&existing_class, sizeof(classes), 1, class_file) == 1) {
                if (strcmp(existing_class.class_name, new_class_name) == 0) {
                    printf("\tClass's Name [ %s ] is already taken (>~<)!\n", new_class_name);
                    printf("\tPlease re-enter a different class name.\n");
                    getch();
                    eraseLines(4);
                    memset( new_class_name, 0, sizeof(new_class_name) ); // Empty new_class_name
                    new_name_valid = false; // set false to repeat loop
                    break;
                }
            }
        } while (!new_name_valid);

        do {
            printf("\tEnter Professor's Name: ");
            fflush(stdin);
            fgets(new_class.class_teacher, MAX_NAME_LENGTH, stdin);
            new_class.class_teacher[ strcspn(new_class.class_teacher, "\n") ] = 0;
            if (strlen(new_class.class_teacher) == 0) {
                printf("\tError: Professor's Name cannot be empty (>_<)!");
                eraseLines(2);
            }
        } while (strlen(new_class.class_teacher) == 0);

        new_class.total_students = 0;

        // Increase class total number in courses
        rewind(course_file);
        while (fread(&cs, sizeof(courses), 1, course_file) == 1) {
            if (strcmp(cs.course_ID, course_search) == 0) {
                break;
            }
        }
        cs.total_class = cs.total_class + 1;

        
        strcpy(new_class.class_name, new_class_name); // Copy the valid name
        new_class.class_name[ strcspn(new_class.class_name, "\n") ] = 0;

        strcpy(new_class.course_ID, cs.course_ID); // Copy course ID
        new_class.course_ID[ strcspn(new_class.course_ID, "\n") ] = 0;
        
        // Write new class to file
        fseek(class_file, 0, SEEK_END);
        fwrite(&new_class, sizeof(classes), 1, class_file);

        // Rewrite course file
        fseek(course_file, course_pos, SEEK_SET);
        fwrite(&cs, sizeof(courses), 1, course_file);

        printf("\n\tClass [ %s ] added successfully ( =^.^=). Number: %d\n", new_class.class_name, class_count +1);
    }

    fclose(course_file);
    fclose(class_file);

    printf("\n\n");
    printf("                        (\\(\\ \n");
    printf("Press any key to return ( -.-) \n");
    getch();
}

// Class Assign Functions
void class_register() { // Fixed and good to go! (25/11, N)
    FILE *student_file, *course_file, *class_file;
    students st;
    courses cs;
    classes cl;
    char student_search[MAX_ID_LENGTH];
    char class_search[MAX_ID_LENGTH];
    char ans[5];
    int ans_length;
    bool student_found = false;
    bool class_check = false;
    bool class_found = false;
    long int student_pos;
    long int course_pos;
    long int class_pos;

    // Open files
    student_file = fopen("ex_student.txt", "r+b");
    course_file = fopen("ex_course.txt", "r+b");
    class_file = fopen("ex_class.txt", "r+b");
    
    if (student_file == NULL || course_file == NULL || class_file == NULL) {
        printf("Error opening file(s) (>_<)!\n");
        return;
    }

    system("cls");

    // Get student ID
    do {
        system("cls");
        printf("\n");
        printf("\t+========================================================+\n");
        printf("\t|              Study Center Management System            |\n");
        printf("\t|========================================================|\n");
        printf("\t|                   Class Registration                   |\n");
        printf("\t+========================================================+\n\n");
        printf("\tEnter Student ID to register: ");
        fflush(stdin);
        fgets(student_search, sizeof(student_search), stdin);
        student_search[ strcspn(student_search, "\n") ] = 0;
        if (strlen(student_search) == 0) {
            printf("\tError: Student ID cannot be empty (>_<)!\n");
            getch();
        }
    } while (strlen(student_search) == 0);

    //Find student
    while (fread(&st, sizeof(students), 1, student_file) == 1) {
        if (strcmp(st.student_ID, student_search) == 0) {
            student_found = true;
            student_pos = ftell(student_file) - sizeof(students);
            break;
        }
    }

    if (!student_found) {
        printf("Student [ %s ] not found (>~<)!\n", student_search);
        printf("\n\n");
        printf("\t                        (\\(\\ \n");
        printf("\tPress any key to return ( -.-) \n");
        getch();


        fclose(student_file);
        fclose(course_file);
        fclose(class_file);

        return;
    }

    system("cls");
    printf("\n");
    printf("\t+========================================================+\n");
    printf("\t|              Study Center Management System            |\n");
    printf("\t|========================================================|\n");
    printf("\t|                   Class Registration                   |\n");
    printf("\t+========================================================+\n\n");
    printf("\tStudent [ %s ]", st.student_name);

    // Check if student has already assigned for a class
    while (fread(&cl, sizeof(classes), 1, class_file) == 1) {
        if (strcmp(st.class_attend, cl.class_name) == 0) {
            class_check = true;
            break;
        }
    }

    if (class_check) {
        printf("\n\tStudent has already been registered to class [ %s ] \n", st.class_attend);
        printf("\tWould you like to move student to a different class?\n"
               "\t(Disclaimer: the current class's fee will not be discharged).\n"
               "\tType 'yes' to continue or 'no' to cancel: ");
        do {
            fflush(stdin);
            fgets(ans, sizeof(ans), stdin);
            ans[ strcspn(ans, "\n") ] = 0;
            ans_length = strlen(ans);
            for (int i = 0; i < ans_length; i++) {
                ans[i] = tolower(ans[i]);
            }
            if (strcmp(ans,"no") == 0) {
                printf("The process has been cancelled.\n"
                       "Press any key to return.\n");
                getch();
                return;
            }
            if (strcmp(ans, "yes") == 0) {
                break;
            }
            printf("Invalid Input (>~<)! Please type either yes or no: ");
        } while (true);

    }

    // Get Class ID
    do {
        printf("\n\tEnter Class's ID: ");
        fflush(stdin);
        fgets(class_search, sizeof(class_search), stdin);
        class_search[ strcspn(class_search, "\n") ] = 0;
        if (strlen(class_search) == 0) {
            printf("\tError: Class ID cannot be empty (>_<)!\n");
        }
    } while (strlen(class_search) == 0);

    // Find Class
    rewind(class_file);
    while (fread(&cl, sizeof(classes), 1, class_file) == 1) {
        if (strcmp(cl.class_ID, class_search) == 0) {
            class_found = true;
            class_pos = ftell(class_file) - sizeof(classes);
            break;
        }
    }

    // Get Course Position
    if (class_found) {
        while (fread(&cs, sizeof(courses), 1, course_file) == 1) {
            if (strcmp(cs.course_ID, cl.course_ID) == 0) {
                course_pos = ftell(course_file) - sizeof(courses);
                break;
            }
        }
    }

    if (!class_found) {
        printf("\tThere is no class with the ID [ %s ] (>~<)!\n", class_search);
    }

    else if (strcmp(st.class_attend, cl.class_name) == 0) {
        printf("\tStudent [ %s ] is already registered for class [ %s ] (As stated from previous check already (>~<)!)\n",
                st.student_name, cl.class_name);
    }

    else if (cl.total_students == cs.max_students) {
        printf("\tClass [ %s ] is filled with maximum amount of students (>~<)!\n", cl.class_name);
    }

    else {
        // Register Process (25/11)
        strcpy(st.class_attend, cl.class_name);
        st.class_attend[ strcspn(st.class_attend, "\n") ] = 0;

        strcpy(st.course_attend, cs.course_name);
        st.course_attend[ strcspn(st.course_attend, "\n") ] = 0;

        // Add one to total_students
        cs.total_students = cs.total_students + 1; // In Course
        cl.total_students = cl.total_students + 1; // In Class

        // Calculate Student's Tuition Fee
        st.tuition_paid = st.tuition_paid - cs.tuition;

        // Update Class Record
        fseek(class_file, class_pos, SEEK_SET);
        fwrite(&cl, sizeof(classes), 1, class_file);

        // Update Student Record
        fseek(student_file, student_pos, SEEK_SET);
        fwrite(&st, sizeof(students), 1, student_file);
    
        // Update Course Record
        fseek(course_file, course_pos, SEEK_SET);
        fwrite(&cs, sizeof(courses), 1, course_file);

        printf("\n\tSuccessfully Registered Student %s for class %s ( =^.^=)!\n", st.student_name, st.class_attend);
    }

    fclose(student_file);
    fclose(course_file);
    fclose(class_file);

    printf("\n\n");
    printf("\t                        (\\(\\ \n");
    printf("\tPress any key to return ( -.-) \n");
    getch();
}

void class_unregister() { // Fixed and good to go! (27/11, N)
    FILE *student_file, *course_file, *class_file;
    students st;
    courses cs;
    classes cl;
    char student_search[MAX_ID_LENGTH];
    char class_name_remove[MAX_CLASSNAME_LENGTH];
    char no_class[MAX_CLASSNAME_LENGTH] = "Not Registered";
    char ans[5];
    int ans_length;
    bool student_found = false;
    long int student_pos;
    long int course_pos;
    long int class_pos;

    student_file = fopen("ex_student.txt", "r+b");
    course_file = fopen("ex_course.txt","r+b");
    class_file = fopen("ex_class.txt", "rb");
    
    if (student_file == NULL || course_file == NULL || class_file == NULL) {
        printf("Error opening file (>_<)!\n");
        return;
    }

    system("cls");
    printf("\n");
    printf("\t+========================================================+\n");
    printf("\t|              Study Center Management System            |\n");
    printf("\t|========================================================|\n");
    printf("\t|                   Class Unregistration                 |\n");
    printf("\t+========================================================+\n\n");
    printf("\t(Disclaimer: the class's fee will not be discharged).\n");

    // Get Student ID
    do {
        printf("\tEnter Student ID: ");
        fflush(stdin);
        fgets(student_search, MAX_CLASSNAME_LENGTH, stdin);
        student_search[ strcspn(student_search, "\n") ] = 0;
        if (strlen(student_search) == 0) {
            printf("\tError: Student ID cannot be empty (>_<)!\n");
        }
    } while (strlen(student_search) == 0);

    // Find Student Position
    while (fread(&st, sizeof(students), 1, student_file) == 1) {
        if (strcmp(st.student_ID, student_search) == 0) {
            student_found = true;
            student_pos = ftell(student_file) - sizeof(students);
            break;
        }
    }

    if (!student_found) {
        printf("\tStudent ID [ %s ] has not been found (>_<)!\n", student_search);
    }
    else if (strcmp(st.class_attend, no_class) == 0) {
        printf("\tStudent [ %s ] has not registered to any classes (>~<)!\n", st.student_name);
    }
    else {
        printf("\n\tCurrent Class: %s", st.class_attend);
        printf("\n\tType 'yes' to continue or 'no' to cancel the unregister: ");
        do {
            fflush(stdin);
            fgets(ans, sizeof(ans), stdin);
            ans[ strcspn(ans, "\n") ] = 0;

            // Remove uppercase lowercase variable
            ans_length = strlen(ans);
            for (int i = 0; i < ans_length; i++) {
                ans[i] = tolower(ans[i]);
            }

            if (strcmp(ans,"no") == 0) {
                printf("\tThe process has been cancelled.\n"
                       "\tPress any key to return.\n");
                getch();
                return;
            }
            if (strcmp(ans, "yes") == 0) {
                break;
            }
            printf("\n\tInvalid Input (>~<)! Please type either yes or no: ");
        } while (true);


        // Find Class Position
        while (fread(&cl, sizeof(classes), 1, class_file) == 1) {
            if (strcmp(cl.class_name, st.class_attend) == 0) {
                class_pos = ftell(class_file) - sizeof(classes);
                break;
            }
        }

        // Find Course Position
        while (fread(&cs, sizeof(courses), 1, course_file) == 1) {
            if (strcmp(cl.course_ID, cs.course_ID) == 0) {
                course_pos = ftell(course_file) - sizeof(courses);
            }
        }

        // Unregister the class
        strcpy(class_name_remove, st.class_attend); // Copy for confirmation line.
        strcpy(st.course_attend, no_class);
        strcpy(st.class_attend, no_class);
        st.tuition_paid = 0;

        // Reduce the amount of student(s) in Class & Course
        cs.total_students = cs.total_students - 1;
        cl.total_students = cl.total_students - 1;

        // 

        // Update Student Record
        fseek(student_file, student_pos, SEEK_SET);
        fwrite(&st, sizeof(students), 1, student_file);

        // Update Course Record
        fseek(course_file, course_pos, SEEK_SET);
        fwrite(&cs, sizeof(courses), 1, course_file);

        // Update Class Record
        fseek(class_file, class_pos, SEEK_SET);
        fwrite(&cl, sizeof(classes), 1, class_file);

        printf("\n\tSuccessfully removed student %s from class %s ( =^.^=)!\n", st.student_name, class_name_remove);
    }

    fclose(student_file);
    fclose(course_file);
    fclose(class_file);
    printf("\n\n");
    printf("\t                        (\\(\\ \n");
    printf("\tPress any key to return ( -.-) \n");
    getch();
}

void class_view() {
    FILE *class_file, *student_file, *course_file;
    classes cl;
    students st;
    courses cs;
    char class_search[MAX_ID_LENGTH];
    int enrolled_count = 0;
    bool class_found = false;

    class_file = fopen("ex_class.txt", "rb");
    student_file = fopen("ex_student.txt", "rb");
    course_file = fopen("ex_course.txt","rb");

    if (student_file == NULL || class_file == NULL || course_file == NULL) {
        printf("\tError opening file(s) (>_<)!\n");
        getch();
        return;
    }

    system("cls");
    printf("\n");
    printf("\t+========================================================+\n");
    printf("\t|              Study Center Management System            |\n");
    printf("\t|========================================================|\n");
    printf("\t|                   Class Information                    |\n");
    printf("\t+========================================================+\n\n");

    do {
        printf("\tEnter Class ID to view enrollment: ");
        fflush(stdin);
        fgets(class_search, sizeof(class_search), stdin);
        class_search[strcspn(class_search, "\n")] = 0;
        if (strlen(class_search) == 0) {
            printf("\tError: Class ID cannot be empty (>_<)!\n");
        }
    } while (strlen(class_search) == 0);

    // Find class
    while (fread(&cl, sizeof(classes), 1, class_file) == 1) {
        if (strcmp(cl.class_ID, class_search) == 0) {
            class_found = true;
            break;
        }
    }

    if (!class_found) {
        printf("\tClass ID [ %s ] has not been found (>~<)!\n", class_search);
    }
    else {
        system("cls");
        printf("\n");
        printf("\t+========================================================+\n");
        printf("\t|              Study Center Management System            |\n");
        printf("\t|========================================================|\n");
        printf("\t|                   Class Information                    |\n");
        printf("\t+========================================================+\n\n");
        printf("\tClass [ %s ]\n", cl.class_name);
        printf("\tBy Professor [ %s ]\n\n", cl.class_teacher);
        printf("\t%-5s |%-15s |%-30s |%-15s\n",
               "No.", "Student ID", "Full Name", "Tuition");
        printf("\t----------------------------------------------------------\n");
        
        while (fread(&st, sizeof(students), 1, student_file) == 1) {
            if (strcmp(cl.class_name, st.class_attend) == 0) {
                enrolled_count++;
                printf("\t%-5d |%-15s |%-30s |%-15.2f\n", 
                    enrolled_count, st.student_ID, st.student_name, st.tuition_paid);
            }
        }
        
        while (fread(&cs, sizeof(courses), 1, course_file) == 1) {
            if (strcmp(cs.course_ID, cl.course_ID) == 0) {
                break;
            }
        }
        printf("\n\tTotal Student(s) in Class %s: %d Student(s)", cl.class_name, cl.total_students);
        printf("\n\tMaximum allowed in Class: %d Students", cs.max_students);
    }

    fclose(class_file);
    fclose(student_file);
    printf("\n\n");
    printf("\t                        (\\(\\ \n");
    printf("\tPress any key to return ( -.-) \n");
    getch();
}


void student_tuition_view() {
    FILE *student_file, *course_file;
    students st;
    courses  cs;
    char student_search[MAX_ID_LENGTH];
    char no_class[MAX_CLASSNAME_LENGTH] = "Not Registered";
    float paid;
    int fee, overpaid;
    bool student_found = false;

    student_file = fopen("ex_student.txt", "rb");
    course_file = fopen("ex_course.txt", "rb");

    if (student_file == NULL || course_file == NULL) {
        printf("Error opening file(s) (>_<)!\n");
        getch();
        return;
    }

    system("cls");
    printf("\n");
    printf("\t+========================================================+\n");
    printf("\t|              Study Center Management System            |\n");
    printf("\t|========================================================|\n");
    printf("\t|                Student Tuition Status                  |\n");
    printf("\t+========================================================+\n\n");

    do {
        system("cls");
        printf("\n");
        printf("\t+========================================================+\n");
        printf("\t|              Study Center Management System            |\n");
        printf("\t|========================================================|\n");
        printf("\t|                Student Tuition Status                  |\n");
        printf("\t+========================================================+\n\n");
        printf("\tEnter Student ID: ");
        fflush(stdin);
        fgets(student_search, sizeof(student_search), stdin);
        student_search[ strcspn(student_search, "\n") ] = 0;
        if (strlen(student_search) == 0) {
            printf("\tError: Student ID cannot be empty (>_<)!\n");
            getch();
        }
    } while (strlen(student_search) == 0);

    // Find Student
    while (fread(&st, sizeof(students), 1, student_file) == 1) {
        if (strcmp(st.student_ID, student_search) == 0) {
            student_found = true;
            break;
        }
    }

    if (!student_found) {
        printf("\tStudent ID [ %s ] has not been found (>~<)!\n", student_search);
    }
    else if (strcmp(st.class_attend, no_class) == 0) {
        printf("\tStudent [ %s ] is not currently enrolled in any class (>~<)!\n", st.student_name);
    }
    else {
        system("cls");
        printf("\n");
        printf("\t+========================================================+\n");
        printf("\t|              Study Center Management System            |\n");
        printf("\t|========================================================|\n");
        printf("\t|                Student Tuition Status                  |\n");
        printf("\t+========================================================+\n\n");

        // Get Course's Fee
        while(fread(&cs, sizeof(courses), 1, course_file) == 1) {
            if (strcmp(st.course_attend, cs.course_name) == 0 ) {
                break;
            }
        }
        
        // Calculate Fee
        paid = st.tuition_paid;
        fee = cs.tuition;
        overpaid = paid / fee;

        // Print out Result
        printf("\t%-30s |%-30s |%-10s\n", "Student", "Class", "Fee");
        printf("\t%-30s |%-30s |%-15d\n", st.student_name, st.class_attend, cs.tuition);
        printf("\t+--------------------------------------------------------+\n");

        printf("\n");
        if (paid < 0) {
            printf("\t%-20s %-15.2f\n", "Unpaid Fee:", paid);
        }
        else if (paid > 0) {
            printf("\t%-20s %-15.2f\n", "Overpaid Amount:", paid);
            if (overpaid < 1) {
                printf("\tStudent [ %s ] has paid for the current semester.\n", st.student_name);
                printf("\tThe extra %.2f will be accounted for the next semester.\n", paid);
            }
            else {
                printf("\tStudent [ %s ] has paid for the current semester.\n", st.student_name);
                printf("\tAnd eligible for %d extra semester(s) after the current one.\n", overpaid);
            }
        }
        else {
            printf("\tStudent [ %s ] has paid the exact amount for the current semester.\n", st.student_name);
        }
    }

    fclose(student_file);
    fclose(course_file);
    printf("\n\n");
    printf("\t                        (\\(\\ \n");
    printf("\tPress any key to return ( -.-) \n");
    getch();
}

void student_tuition_update() {
    FILE *student_file;
    students st;
    char student_search[MAX_ID_LENGTH];
    float pay;
    long int student_pos;
    bool student_found = false;

    student_file = fopen("ex_student.txt", "r+b");

    if (student_file == NULL) {
        printf("Error opening file(s) (>_<)!\n");
        getch();
        return;
    }

    system("cls");
    printf("\n");
    printf("\t+========================================================+\n");
    printf("\t|              Study Center Management System            |\n");
    printf("\t|========================================================|\n");
    printf("\t|                Update Student Payment                  |\n");
    printf("\t+========================================================+\n\n");

    printf("\tEnter Student ID: ");
    while (fgets(student_search, sizeof(student_search), stdin) != NULL) {
        student_search[strcspn(student_search, "\n")] = 0;
        if (strlen(student_search) > 0) {
            break;
        } else {
            printf("\tError: Student ID cannot be empty (>_<)!\n");
            printf("\tEnter Student ID: ");
        }
    }

    // Find Student
    while (fread(&st, sizeof(students), 1, student_file) == 1) {
        if (strcmp(st.student_ID, student_search) == 0) {
            student_found = true;
            student_pos = ftell(student_file) - sizeof(students);
            break;
        }
    }

    if (!student_found) {
        printf("\n\tStudent ID [ %s ] has not been found (>~<)!\n", student_search);
    } else {
        system("cls");
        printf("\n");
        printf("\t+========================================================+\n");
        printf("\t|              Study Center Management System            |\n");
        printf("\t|========================================================|\n");
        printf("\t|                Update Student Payment                  |\n");
        printf("\t+========================================================+\n\n");

        printf("\tStudent [ %s ]\n", st.student_name);
        if (st.tuition_paid < 0) {
            printf("\tCurrent Fee required to pay: %.2f\n", st.tuition_paid);
        }
        else if (st.tuition_paid > 0) {
            printf("\tCurrent Overpaid Amount: %.2f\n", st.tuition_paid);
        }
        else {
            printf("\tNo Fee Required to Pay.\n");
        }
        printf("\t+--------------------------------------------------------+\n\n");

        // Get the new amount
        printf("\tType the amount of money student %s has paid: ", st.student_name);
        while (scanf("%f", &pay) != 1) {
            while (getchar() != '\n');
            printf("\t(!_!) Invalid input! Please enter numeric value only!");
            _getch();
            eraseLines(2);
            printf("\tType the amount of money student %s has paid: ", st.student_name);
        }
        
        // Update Student Fee
        st.tuition_paid = st.tuition_paid + pay;

        system("cls");
        printf("\n");
        printf("\t+========================================================+\n");
        printf("\t|              Study Center Management System            |\n");
        printf("\t|========================================================|\n");
        printf("\t|                Update Student Payment                  |\n");
        printf("\t+========================================================+\n\n");

        // Confirmation Line
        printf("\tConfirming student %s has paid %.2f\n", st.student_name, pay);
        printf("\t+--------------------------------------------------------+\n");
        if (st.tuition_paid > 0) {
            printf("\tNew Overpaid Amount: %.2f\n", st.tuition_paid);
        }
        else if (st.tuition_paid < 0) {
            printf("\tFee remaining after: %.2f\n", st.tuition_paid);
        }
        else {
            printf("\tThe remaining fee has been paid.\n");
        }

        // Update Student Record
        fseek(student_file, student_pos, SEEK_SET);
        fwrite(&st, sizeof(students), 1, student_file);
    }

    fclose(student_file);
    printf("\n\n");
    printf("\t                        (\\(\\ \n");
    printf("\tPress any key to return ( -.-) \n");
    getch();
}

void course_tuition_view() {
    FILE *student_file, *course_file;
    students st;
    courses cs;
    char course_search[MAX_ID_LENGTH];
    float paid, remain;
    int student_paid = 0;
    int student_remain = 0;
    bool course_found = false;

    student_file = fopen("ex_student.txt", "rb");
    course_file = fopen("ex_course.txt", "rb");

    if (student_file == NULL || course_file == NULL) {
        printf("Error opening file(s) (>_<)!\n");
        getch();
        return;
    }

    system("cls");
    printf("\n");
    printf("\t+========================================================+\n");
    printf("\t|              Study Center Management System            |\n");
    printf("\t|========================================================|\n");
    printf("\t|                Course Tuition Overview                 |\n");
    printf("\t+========================================================+\n\n");

    printf("\tEnter Course ID: ");
    do {
        fflush(stdin);
        fgets(course_search, MAX_ID_LENGTH, stdin);
        course_search[ strcspn(course_search, "\n") ] = 0;
        if (strlen(course_search) > 0) {
            break;
        } else {
            printf("\tError: Course ID cannot be empty (>_<)!\n");
            _getch();
            eraseLines(2);
            printf("\tEnter Course ID: ");
        }
    } while (true);

    // Find Course
    while (fread(&cs, sizeof(courses), 1, course_file) == 1) {
        if (strcmp(cs.course_ID, course_search) == 0) {
            course_found = true;
            break;
        }
    }

    if (!course_found) {
        printf("\tCourse ID [ %s ] has not been found (>~<)!\n", course_search);
    }
    else if (cs.total_students == 0) {
        printf("\tCourse has zero students, thus zero income (>_<)!\n");
    }
    else {
        system("cls");
        printf("\n");
        printf("\t+========================================================+\n");
        printf("\t|              Study Center Management System            |\n");
        printf("\t|========================================================|\n");
        printf("\t|                Course Tuition Overview                 |\n");
        printf("\t+========================================================+\n\n");

        // Print course info header
        printf("\t+--------------------------------------------------------+\n");
        printf("\t| %-20s | %-30s |\n", "Course Name", cs.course_name);
        printf("\t| %-20s | %-30d |\n", "Course Fee", cs.tuition);
        printf("\t| %-20s | %-30d |\n", "Total Students", cs.total_students);
        printf("\t+--------------------------------------------------------+\n\n");

        // Check Student's Payment Status
        while(fread(&st, sizeof(students), 1, student_file) == 1) {
            if (strcmp(st.course_attend, cs.course_name) == 0) {
                if (st.tuition_paid < 0) {
                    remain = remain + st.tuition_paid;
                    student_remain++;
                }
                else if (st.tuition_paid == 0) {
                    paid = paid + cs.tuition;
                    student_paid++;
                }
                else {
                    paid = paid + st.tuition_paid + cs.tuition;
                    student_paid++;
                }
            }
        }

        // Print payment summary
        printf("\t+--------------------------------------------------------+\n");
        if (paid > 0) {
            printf("\t| %-40s | %-15.2f |\n", "Tuition Fee Collected", paid);
            printf("\t| %-40s | %-15d |\n", "Students Paid", student_paid);
        }
        else {
            printf("\t| No students have paid their tuition fees yet.        |\n");
        }
        
        if (remain < 0) {
            printf("\t| %-40s | %-15.2f |\n", "Outstanding Tuition", -remain);
            printf("\t| %-40s | %-15d |\n", "Students with Outstanding Fees", student_remain);
        }
        else {
            printf("\t| All students have paid their tuition fees ( =^.^=)!  |\n");
        }
        printf("\t+--------------------------------------------------------+\n");
    }

    fclose(student_file);
    fclose(course_file);
    printf("\n\n");
    printf("\t                        (\\(\\ \n");
    printf("\tPress any key to return ( -.-) \n");
    getch();
}

void course_tuition_update() {
    FILE *student_file, *course_file;
    students st;
    courses cs;
    char course_search[MAX_ID_LENGTH];
    char current_student[MAX_ID_LENGTH];
    bool course_found = false;
    bool end_check = true;
    int new_fee;
    int old_fee;
    int old_student_pay;
    int new_student_pay;
    long int course_pos;
    long int student_pos = 0;

    // Open files
    student_file = fopen("ex_student.txt", "r+b");
    course_file = fopen("ex_course.txt", "r+b");

    if (student_file == NULL || course_file == NULL) {
        printf("\tError Opening File(s) (>_<)!\n");
        getch();
        return;
    }

    system("cls");
    printf("\n");
    printf("\t+========================================================+\n");
    printf("\t|              Study Center Management System            |\n");
    printf("\t|========================================================|\n");
    printf("\t|                Update Course Tuition Fee               |\n");
    printf("\t+========================================================+\n\n");

    printf("\tEnter Course ID: ");
    do {
        fflush(stdin);
        fgets(course_search, MAX_ID_LENGTH, stdin);
        course_search[ strcspn(course_search, "\n") ] = 0;
        if (strlen(course_search) > 0) {
            break;
        }
        else {
            printf("\tError: Course ID cannot be empty (>_<)!\n");
            eraseLines(2);
            printf("\tEnter Course ID: ");
        }
    } while (true);

    // Find Course
    while (fread(&cs, sizeof(courses), 1, course_file) == 1) {
        if (strcmp(cs.course_ID, course_search) == 0) {
            course_found = true;
            course_pos = ftell(course_file) - sizeof(courses);
            break;
        }
    }

    if (!course_found) {
        printf("\tCourse ID [ %s ] has not been found (>~<)!\n", course_search);
    }
    else {
        old_fee = cs.tuition;
        system("cls");
        printf("\n");
        printf("\t+========================================================+\n");
        printf("\t|              Study Center Management System            |\n");
        printf("\t|========================================================|\n");
        printf("\t|                Update Course Tuition Fee               |\n");
        printf("\t+========================================================+\n\n");

        printf("\tCourse [ %s ]\n", cs.course_name);
        printf("\tCurrent Fee: %d\n", cs.tuition);
        printf("\t+--------------------------------------------------------+\n\n");

        // Get the new amount
        printf("\tType the new tuition fee for course %s: ", cs.course_name);
        while (scanf("%d", &new_fee) != 1) {
            while (getchar() != '\n');
            printf("\t(!_!) Invalid input! Please enter numeric value only!");
            _getch();
            eraseLines(2);
            printf("\tType the new tuition fee for course %s: ", cs.course_name);
        }

        // Update Course Fee
        cs.tuition = new_fee;
        
        // Confirmation Line
        system("cls");
        printf("\n");
        printf("\t+========================================================+\n");
        printf("\t|              Study Center Management System            |\n");
        printf("\t|========================================================|\n");
        printf("\t|                Update Course Tuition Fee               |\n");
        printf("\t+========================================================+\n\n");

        printf("\tCourse %s's new fee is: %d\n", cs.course_name, cs.tuition);
        printf("\t+--------------------------------------------------------+\n\n");
        printf("\tList of Student's Payment Status studying course %s\n\n", cs.course_name);
        printf("\t%-30s |%-30s |%-15s\n", "Name", "Previous Payment", "Current Payment");
        printf("\t----------------------------------------------------------\n");
        
        do {
            fseek(student_file, student_pos, SEEK_SET);
            end_check = true;
            while (fread(&st, sizeof(students), 1, student_file) == 1) {
                if (strcmp(st.course_attend, cs.course_name) == 0 && strcmp(st.student_ID,current_student) != 0) {
                    strcpy(current_student, st.student_ID);
                    old_student_pay = st.tuition_paid;
                    new_student_pay = st.tuition_paid + old_fee - new_fee;
                    st.tuition_paid = new_student_pay;
                    student_pos = ftell(student_file) - sizeof(students);
                    end_check = false;
                    break;
                }
            }

            if (!end_check) {
                printf("\t%-30s |%-30d |%-15d\n", st.student_name, old_student_pay, new_student_pay);
                
                // Update Student Record
                fseek(student_file, student_pos, SEEK_SET);
                fwrite(&st, sizeof(students), 1, student_file);
            }
        } while (!end_check);
    }

    // Update Course Record
    fseek(course_file, course_pos, SEEK_SET);
    fwrite(&cs, sizeof(courses), 1, course_file);

    fclose(student_file);
    fclose(course_file);
    printf("\n\n");
    printf("                        (\\(\\ \n");
    printf("Press any key to return ( -.-) \n");
    getch();
}

//Delete student by ID (work now 29/11,D)
void student_delete() {
    FILE *student_file, *temp_student_file;
    FILE *class_file = NULL, *course_file = NULL;
    students st;
    classes cl;
    courses cs;
    char student_ID[MAX_ID_LENGTH];
    char class_name[MAX_CLASSNAME_LENGTH];
    bool student_found = false;
    char confirm[5];

    // Open student files
    student_file = fopen("ex_student.txt", "rb");
    temp_student_file = fopen("temp_student.txt", "wb");
    if (student_file == NULL || temp_student_file == NULL) {
        printf("Error opening student files (>_<)!\n");
        if (student_file) fclose(student_file);
        if (temp_student_file) fclose(temp_student_file);
        return;
    }

    // Try to open class and course files
    class_file = fopen("ex_class.txt", "r+b");
    course_file = fopen("ex_course.txt", "r+b");

    system("cls");

    // Get student ID with validation
    do {
        printf("\n");
        printf("\t+========================================================+\n");
        printf("\t|              Study Center Management System            |\n");
        printf("\t|========================================================|\n");
        printf("\t|               Delete Student Information               |\n");
        printf("\t+========================================================+\n\n");
        printf("\tEnter Student ID to delete: ");
        fflush(stdin);
        fgets(student_ID, sizeof(student_ID), stdin);
        student_ID[strcspn(student_ID, "\n")] = 0;
        if (strlen(student_ID) == 0) {
            printf("\tError: Student ID cannot be empty (>_<)!\n");
        }
    } while (strlen(student_ID) == 0);

    // Find and confirm student deletion
    while (fread(&st, sizeof(students), 1, student_file) == 1) {
        if (strcmp(st.student_ID, student_ID) == 0) {
            student_found = true;
            strcpy(class_name, st.class_attend);
            
            printf("\n\tStudent found:\n");
            printf("\t+--------------------------------------------------------+\n");
            printf("\t| %-20s | %s\n", "ID:", st.student_ID);
            printf("\t| %-20s | %s\n", "Full Name:", st.student_name);
            printf("\t| %-20s | %s\n", "Class:", st.class_attend);
            printf("\t+--------------------------------------------------------+\n");
            
            printf("\n\tAre you sure you want to delete this student? (yes/no): ");
            do {
                fflush(stdin);
                fgets(confirm, sizeof(confirm), stdin);
                confirm[strcspn(confirm, "\n")] = 0;
                
                if (strcmp(confirm, "no") == 0 || strcmp(confirm, "No") == 0) {
                    printf("\n\tDeletion cancelled.\n");
                    goto cleanup;
                }
                if (strcmp(confirm, "yes") == 0 || strcmp(confirm, "Yes") == 0) {
                    break;
                }
                printf("\tInvalid input! Please type yes or no: ");
            } while (true);
            break;
        }
    }

    if (!student_found) {
        printf("\n\tStudent ID [ %s ] not found (>_<)!\n", student_ID);
        goto cleanup;
    }

    // Rewrite student file excluding deleted student
    rewind(student_file);
    while (fread(&st, sizeof(students), 1, student_file) == 1) {
        if (strcmp(st.student_ID, student_ID) != 0) {
            fwrite(&st, sizeof(students), 1, temp_student_file);
        }
    }

    // Update class and course files if they exist
    if (class_file && course_file) {
        FILE *temp_class_file = fopen("temp_class.txt", "wb");
        FILE *temp_course_file = fopen("temp_course.txt", "wb");
        
        if (!temp_class_file || !temp_course_file) {
            printf("\tError creating temporary files (>_<)!\n");
            if (temp_class_file) fclose(temp_class_file);
            if (temp_course_file) fclose(temp_course_file);
            goto cleanup;
        }

        // Copy class records with updated student count
        rewind(class_file);
        while (fread(&cl, sizeof(classes), 1, class_file) == 1) {
            if (strcmp(cl.class_name, class_name) == 0) {
                cl.total_students--;
            }
            fwrite(&cl, sizeof(classes), 1, temp_class_file);
        }

        // Copy course records with updated student count
        rewind(course_file);
        while (fread(&cs, sizeof(courses), 1, course_file) == 1) {
            if (strcmp(cs.course_ID, cl.course_ID) == 0) {
                cs.total_students--;
            }
            fwrite(&cs, sizeof(courses), 1, temp_course_file);
        }

        // Close and replace class and course files
        fclose(class_file);
        fclose(course_file);
        fclose(temp_class_file);
        fclose(temp_course_file);

        remove("ex_class.txt");
        rename("temp_class.txt", "ex_class.txt");
        remove("ex_course.txt");
        rename("temp_course.txt", "ex_course.txt");
    }

    // Replace original student file with temp file
    fclose(student_file);
    fclose(temp_student_file);
    remove("ex_student.txt");
    rename("temp_student.txt", "ex_student.txt");

    printf("\n\tStudent successfully deleted ( =^.^=)!\n");
    goto end;

cleanup:
    fclose(student_file);
    fclose(temp_student_file);
    remove("temp_student.txt");

end:
    if (class_file) fclose(class_file);
    if (course_file) fclose(course_file);
    
    printf("\n\n");
    printf("\t                        (\\(\\ \n");
    printf("\tPress any key to return ( -.-) \n");
    getch();
}

//delete course by ID (unregister student from ex_student.txt, delete from ex_course.txt, delete class from ex_class.txt) (29/11, D)
void course_delete() {
    FILE *files[6] = {NULL}; // Array to track all file handles
    int deleted_count = 0;
    char course_search[MAX_ID_LENGTH];
    
    // Struct to hold all temp file names for cleanup
    struct {
        const char *course;
        const char *class_;
        const char *student;
    } filenames = {
        "ex_course.txt", "ex_class.txt", "ex_student.txt"
    };

    // Open all required files
    files[0] = fopen(filenames.course, "rb");     // course_file
    files[1] = fopen("temp_course.txt", "wb");    // temp_course_file
    files[2] = fopen(filenames.class_, "rb");     // class_file 
    files[3] = fopen("temp_class.txt", "wb");     // temp_class_file
    files[4] = fopen(filenames.student, "rb");    // student_file
    files[5] = fopen("temp_student.txt", "wb");   // temp_student_file

    system("cls");
    printf("\n");
    printf("\t+========================================================+\n");
    printf("\t|              Study Center Management System            |\n");
    printf("\t|========================================================|\n");
    printf("\t|                     Delete Course                      |\n");
    printf("\t+========================================================+\n\n");

    // Validate file opens
    for (int i = 0; i < 6; i++) {
        if (!files[i]) {
            printf("\tError opening files (>_<)!\n");
            goto cleanup;
        }
    }

    // Get and validate course ID
    if (!get_course_id(course_search)) goto cleanup;

    // Find course and get confirmation
    courses cs;
    if (!find_and_confirm_course(files[0], course_search, &cs)) {
        printf("\n\tCourse ID [ %s ] not found (>_<)!\n", course_search);
        goto cleanup;
    }

    // Process files
    if (!process_course_deletion(files, course_search, &deleted_count)) goto cleanup;

    // Commit changes
    for (int i = 0; i < 6; i++) fclose(files[i]);
    remove(filenames.course); rename("temp_course.txt", filenames.course);
    remove(filenames.class_); rename("temp_class.txt", filenames.class_);
    remove(filenames.student); rename("temp_student.txt", filenames.student);

    printf("\n\tCourse successfully deleted ( =^.^=)!\n");
    if (deleted_count > 0) {
        printf("\t%d student(s) have been unregistered.\n", deleted_count);
    }
    goto end;

cleanup:
    // Close all open files
    for (int i = 0; i < 6; i++) {
        if (files[i]) fclose(files[i]);
    }
    remove("temp_course.txt");
    remove("temp_class.txt"); 
    remove("temp_student.txt");

end:    
    printf("\n\n");
    printf("\t                        (\\(\\ \n");
    printf("\tPress any key to return ( -.-) \n");
    getch();
}

//sort student by name, tuition (30/11, D)
void student_sort_ascending() {
    FILE *file = fopen("ex_student.txt", "rb");
    if (!file) {
        printf("\tError opening file (>_<)!\n");
        return;
    }

    // Count total students
    fseek(file, 0, SEEK_END);
    int count = ftell(file) / sizeof(students);
    rewind(file);

    // Read all students into array
    students *st = (students*)malloc(count * sizeof(students));
    fread(st, sizeof(students), count, file);
    fclose(file);

    // Bubble sort by name
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(st[j].student_name, st[j + 1].student_name) > 0) {
                students temp = st[j];
                st[j] = st[j + 1];
                st[j + 1] = temp;
            }
        }
    }

    // Display sorted list
    system("cls");
    printf("\t+========================================================+\n");
    printf("\t|              Study Center Management System            |\n");
    printf("\t|========================================================|\n");
    printf("\t|                  Student Information                   |\n");
    printf("\t+========================================================+\n\n");
    printf("\n\tSorted Student List (A-Z)\n\n");
    printf("\t%-5s |%-15s |%-30s |%-30s\n", 
           "No.", "ID", "Full Name", "Class");
    printf("\t----------------------------------------------------------\n");
    
    for (int i = 0; i < count; i++) {
        printf("\t%-5d |%-15s |%-30s |%-30s\n", 
            i + 1, st[i].student_ID, st[i].student_name, st[i].class_attend);
    }

    free(st);
    printf("\n\n");
    printf("\t                        (\\(\\ \n");
    printf("\tPress any key to return ( -.-) \n");
    getch();
}

//Delete class by ID (unregister student from ex_student.txt, delete from ex_class.txt, update course total_class and total_students) (01/12, D)
void class_delete() {
    FILE *class_file, *temp_class_file;
    FILE *student_file, *temp_student_file;
    FILE *course_file;
    classes cl;
    students st;
    courses cs;
    char class_ID[MAX_ID_LENGTH];
    char class_name[MAX_CLASSNAME_LENGTH];
    char no_class[MAX_CLASSNAME_LENGTH] = "Not Registered";
    char no_course[MAX_CLASSNAME_LENGTH] = "Not Registered";
    char confirm[5];
    bool class_found = false;
    long int course_pos;

    // Open all required files
    class_file = fopen("ex_class.txt", "rb");
    temp_class_file = fopen("temp_class.txt", "wb");
    student_file = fopen("ex_student.txt", "r+b");
    temp_student_file = fopen("temp_student.txt", "wb");
    course_file = fopen("ex_course.txt", "r+b");

    if (!class_file || !temp_class_file || !student_file || !temp_student_file || !course_file) {
        printf("\tError opening files (>_<)!\n");
        goto cleanup;
    }

    system("cls");
    printf("\n");
    printf("\t+========================================================+\n");
    printf("\t|              Study Center Management System            |\n");
    printf("\t|========================================================|\n");
    printf("\t|                    Delete Class                        |\n");
    printf("\t+========================================================+\n\n");

    // Get class ID with validation
    do {
        printf("\tEnter Class ID to delete: ");
        fflush(stdin);
        fgets(class_ID, sizeof(class_ID), stdin);
        class_ID[strcspn(class_ID, "\n")] = 0;
        if (strlen(class_ID) == 0) {
            printf("\tError: Class ID cannot be empty (>_<)!\n");
        }
    } while (strlen(class_ID) == 0);

    // Find and confirm class deletion
    while (fread(&cl, sizeof(classes), 1, class_file) == 1) {
        if (strcmp(cl.class_ID, class_ID) == 0) {
            class_found = true;
            strcpy(class_name, cl.class_name);
            
            printf("\n\tClass found:\n");
            printf("\t+--------------------------------------------------------+\n");
            printf("\t| %-20s | %s\n", "ID:", cl.class_ID);
            printf("\t| %-20s | %s\n", "Name:", cl.class_name);
            printf("\t| %-20s | %s\n", "Course ID:", cl.course_ID);
            printf("\t| %-20s | %s\n", "Professor:", cl.class_teacher);
            printf("\t| %-20s | %d\n", "Total Students:", cl.total_students);
            printf("\t+--------------------------------------------------------+\n");
            
            printf("\n\tAre you sure you want to delete this class? (yes/no): ");
            do {
                fflush(stdin);
                fgets(confirm, sizeof(confirm), stdin);
                confirm[strcspn(confirm, "\n")] = 0;
                
                if (strcmp(confirm, "no") == 0 || strcmp(confirm, "No") == 0) {
                    printf("\n\tDeletion cancelled.\n");
                    goto cleanup;
                }
                if (strcmp(confirm, "yes") == 0 || strcmp(confirm, "Yes") == 0) {
                    break;
                }
                printf("\tInvalid input! Please type yes or no: ");
            } while (true);
            break;
        }
    }

    if (!class_found) {
        printf("\n\tClass ID [ %s ] not found (>_<)!\n", class_ID);
        goto cleanup;
    }

    // Update course total class count
    rewind(course_file);
    while (fread(&cs, sizeof(courses), 1, course_file) == 1) {
        if (strcmp(cs.course_ID, cl.course_ID) == 0) {
            cs.total_class--;
            cs.total_students -= cl.total_students;
            course_pos = ftell(course_file) - sizeof(courses);
            fseek(course_file, course_pos, SEEK_SET);
            fwrite(&cs, sizeof(courses), 1, course_file);
            break;
        }
    }

    // Copy classes excluding deleted class
    rewind(class_file);
    while (fread(&cl, sizeof(classes), 1, class_file) == 1) {
        if (strcmp(cl.class_ID, class_ID) != 0) {
            fwrite(&cl, sizeof(classes), 1, temp_class_file);
        }
    }

    // Update student records - unregister from deleted class
    rewind(student_file);
    while (fread(&st, sizeof(students), 1, student_file) == 1) {
        if (strcmp(st.class_attend, class_name) == 0) {
            strcpy(st.class_attend, no_class);
            strcpy(st.course_attend, no_course);
            st.tuition_paid = 0;
        }
        fwrite(&st, sizeof(students), 1, temp_student_file);
    }

    // Close and replace files
    fclose(class_file);
    fclose(temp_class_file);
    fclose(student_file);
    fclose(temp_student_file);
    fclose(course_file);

    remove("ex_class.txt");
    rename("temp_class.txt", "ex_class.txt");
    remove("ex_student.txt");
    rename("temp_student.txt", "ex_student.txt");

    printf("\n\tClass successfully deleted ( =^.^=)!\n");
    goto end;

cleanup:
    if (class_file) fclose(class_file);
    if (temp_class_file) fclose(temp_class_file);
    if (student_file) fclose(student_file);
    if (temp_student_file) fclose(temp_student_file);
    if (course_file) fclose(course_file);
    remove("temp_class.txt");
    remove("temp_student.txt");

end:
    printf("\n\n");
    printf("\t                        (\\(\\ \n");
    printf("\tPress any key to return ( -.-) \n");
    getch();
}

void student_sort_descending() {
    FILE *file = fopen("ex_student.txt", "rb");
    if (!file) {
        printf("\tError opening file (>_<)!\n");
        return;
    }

    fseek(file, 0, SEEK_END);
    int count = ftell(file) / sizeof(students);
    rewind(file);

    students *st = (students*)malloc(count * sizeof(students));
    fread(st, sizeof(students), count, file);
    fclose(file);

    // Bubble sort by name (Z-A)
    for (int i = 0; i < count - 1; i++) {
        for (int j =0; j < count - i - 1; j++) {
            if (strcmp(st[j].student_name, st[j + 1].student_name) < 0) { // Reversed comparison
                students temp = st[j];
                st[j] = st[j + 1];
                st[j + 1] = temp;
            }
        }
    }

    system("cls");
    printf("\t+========================================================+\n");
    printf("\t|              Study Center Management System            |\n");
    printf("\t|========================================================|\n");
    printf("\t|                  Student Information                   |\n");
    printf("\t+========================================================+\n\n");
    printf("\n\tSorted Student List (Z-A)\n\n");
    printf("\t%-5s |%-15s |%-30s |%-30s\n", 
           "No.", "ID", "Full Name", "Class");
    printf("\t----------------------------------------------------------\n");
    
    for (int i =0; i < count; i++) {
        printf("\t%-5d |%-15s |%-30s |%-30s\n", 
            i + 1, st[i].student_ID, st[i].student_name, st[i].class_attend);
    }

    free(st);
    printf("\n\n");
    printf("\t                        (\\(\\ \n");
    printf("\tPress any key to return ( -.-) \n");
    getch();
}

void student_sort_tuition_ascending() {
    FILE *file = fopen("ex_student.txt", "rb");
    if (!file) {
        printf("\tError opening file (>_<)!\n");
        return;
    }

    fseek(file, 0, SEEK_END);
    int count = ftell(file) / sizeof(students);
    rewind(file);

    students *st = (students*)malloc(count * sizeof(students));
    fread(st, sizeof(students), count, file);
    fclose(file);

    // Bubble sort by tuition (Low to High)
    for (int i = 0; i < count - 1; i++) {
        for (int j =0; j < count - i - 1; j++) {
            if (st[j].tuition_paid > st[j + 1].tuition_paid) {
                students temp = st[j];
                st[j] = st[j + 1];
                st[j + 1] = temp;
            }
        }
    }

    system("cls");
    printf("\t+========================================================+\n");
    printf("\t|              Study Center Management System            |\n");
    printf("\t|========================================================|\n");
    printf("\t|                  Student Information                   |\n");
    printf("\t+========================================================+\n\n");
    printf("\n\tSorted Student List (Tuition: Low to High)\n\n");
    printf("\t%-5s |%-15s |%-30s |%-15s\n", 
           "No.", "ID", "Full Name", "Tuition");
    printf("\t----------------------------------------------------------\n");
    
    for (int i =0; i < count; i++) {
        printf("\t%-5d |%-15s |%-30s |%-15.2f\n", 
            i + 1, st[i].student_ID, st[i].student_name, st[i].tuition_paid);
    }

    free(st);
    printf("\n\n");
    printf("\t                        (\\(\\ \n");
    printf("\tPress any key to return ( -.-) \n");
    getch();
}

void student_sort_tuition_descending() {
    FILE *file = fopen("ex_student.txt", "rb");
    if (!file) {
        printf("\tError opening file (>_<)!\n");
        return;
    }

    fseek(file, 0, SEEK_END);
    int count = ftell(file) / sizeof(students);
    rewind(file);

    students *st = (students*)malloc(count * sizeof(students));
    fread(st, sizeof(students), count, file);
    fclose(file);

    // Bubble sort by tuition (High to Low)
    for (int i =0; i < count - 1; i++) {
        for (int j =0; j < count - i - 1; j++) {
            if (st[j].tuition_paid < st[j + 1].tuition_paid) { // Reversed comparison
                students temp = st[j];
                st[j] = st[j + 1];
                st[j + 1] = temp;
            }
        }
    }

    system("cls");
    printf("\t+========================================================+\n");
    printf("\t|              Study Center Management System            |\n");
    printf("\t|========================================================|\n");
    printf("\t|                  Student Information                   |\n");
    printf("\t+========================================================+\n\n");
    printf("\n\tSorted Student List (Tuition: High to Low)\n\n");
    printf("\t%-5s |%-15s |%-30s |%-15s\n", 
           "No.", "ID", "Full Name", "Tuition");
    printf("\t----------------------------------------------------------\n");
    
    for (int i =0; i < count; i++) {
        printf("\t%-5d |%-15s |%-30s |%-15.2f\n", 
            i + 1, st[i].student_ID, st[i].student_name, st[i].tuition_paid);
    }

    free(st);
    printf("\n\n");
    printf("\t                        (\\(\\ \n");
    printf("\tPress any key to return ( -.-) \n");
    getch();
}


//update student personal info (30/11, D)
void student_update_info() {
    FILE *file;
    students st;
    char student_search[MAX_ID_LENGTH];
    bool student_found = false;
    long int student_pos;
    int update_choice;
    int phone_length;
    char phone_str[MAX_NUMBER_LENGTH];
    int student_count = 0;

    // Open file in read and write mode
    file = fopen("ex_student.txt", "r+b");
    if (file == NULL) {
        printf("Error opening file (>_<)!\n");
        return;
    }

    system("cls");
    printf("\n");
    printf("\t+========================================================+\n");
    printf("\t|              Study Center Management System            |\n");
    printf("\t|========================================================|\n");
    printf("\t|               Update Student Information               |\n");
    printf("\t+========================================================+\n\n");

    //print all student ID and names
    printf("\t%-5s |%-15s |%-30s |%-30s\n", 
           "No.", "ID", "Full Name", "Class");
    printf("\t+--------------------------------------------------------+\n");

    while (fread(&st, sizeof(students), 1, file) == 1) {
        student_count++;
        printf("\t%-5d |%-15s |%-30s |%-30s\n", 
            student_count, st.student_ID, st.student_name, st.class_attend);
    }
    printf("\t+--------------------------------------------------------+\n");

    // Get student ID
    rewind(file);
    do {
        printf("\n\tEnter Student ID to update: ");
        fflush(stdin);
        fgets(student_search, sizeof(student_search), stdin);
        student_search[strcspn(student_search, "\n")] = 0;
        if (strlen(student_search) == 0) {
            printf("\tError: Student ID cannot be empty (>_<)!\n");
        }
    } while (strlen(student_search) == 0);

    // Find student
    while (fread(&st, sizeof(students), 1, file) == 1) {
        if (strcmp(st.student_ID, student_search) == 0) {
            student_found = true;
            student_pos = ftell(file) - sizeof(students);
            break;
        }
    }

    if (!student_found) {
        printf("\n\tStudent ID [ %s ] not found (>_<)!\n", student_search);
    } else {
        system("cls");
        printf("\n");
        printf("\t+========================================================+\n");
        printf("\t|              Study Center Management System            |\n");
        printf("\t|========================================================|\n");
        printf("\t|               Update Student Information               |\n");
        printf("\t+========================================================+\n\n");

        // Display current information
        printf("\n\tCurrent Student Information:\n");
        printf("\t+--------------------------------------------------------+\n");
        printf("\t| %-20s | %s\n", "ID:", st.student_ID);
        printf("\t| %-20s | %s\n", "Full Name:", st.student_name);
        printf("\t| %-20s | %s\n", "Phone Number:", st.student_phone);
        printf("\t| %-20s | %s\n", "Email:", st.student_email);
        printf("\t+--------------------------------------------------------+\n\n");

        do {
            printf("\tWhat would you like to update?\n\n");
            printf("\t\t* 1. Name\n");
            printf("\t\t* 2. Phone Number\n");
            printf("\t\t* 3. Email\n");
            printf("\t\t* 4. Return\n");
            printf("\t+--------------------------------------------------------+\n\n");
            printf("\tEnter your choice (1-4): ");

            while (scanf("%d", &update_choice) != 1) {
                while (getchar() != '\n');
                printf("\t(!_!) Invalid input! Please enter a number (1-4): ");
                _getch();
                eraseLines(2);
            }

            switch(update_choice) {
                case 1: // Update Name
                    do {
                        printf("\n\tEnter new name: ");
                        fflush(stdin);
                        fgets(st.student_name, sizeof(st.student_name), stdin);
                        st.student_name[strcspn(st.student_name, "\n")] = 0;
                        if (strlen(st.student_name) == 0) {
                            printf("\tStudent's Name cannot be empty (>_<)!\n");
                        }
                    } while (strlen(st.student_name) == 0);
                    break;

                case 2: // Update Phone
                    do {
                        printf("\n\tEnter new phone number: ");
                        fflush(stdin);
                        fgets(phone_str, sizeof(phone_str), stdin);
                        phone_str[strcspn(phone_str, "\n")] = 0;
                        
                        phone_length = strlen(phone_str);
                        printf("\tYou typed in %d digits [ %s ].", phone_length, phone_str);
                        printf("\n");
                        bool valid = true;
                        
                        // Check if exactly 10 digits
                        if(phone_length != 10) {
                            printf("\tPhone number must be exactly 10 digits!\n");
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
                            strcpy(st.student_phone, phone_str);
                            break;
                        }
                        printf("\tInvalid phone number format (>_<)! Please enter 10 digits only.\n");
                    } while(1);
                    break;

                case 3: // Update Email
                    do {
                        printf("\n\tEnter new email: ");
                        fflush(stdin);
                        fgets(st.student_email, sizeof(st.student_email), stdin);
                        st.student_email[strcspn(st.student_email, "\n")] = 0;
                        if (strlen(st.student_email) == 0) {
                            printf("\tStudent's Email cannot be empty (>_<)!\n");
                        } else if (strchr(st.student_email, '@') == NULL) {
                            printf("\tInvalid email format (>_<)! Please enter a valid email address.\n");
                        }
                    } while (strlen(st.student_email) == 0 || strchr(st.student_email, '@') == NULL);
                    break;

                case 4: // Return
                    fclose(file);
                    return;

                default:
                    printf("\tInvalid choice! Please select 1-4 only (>_<)!\n");
                    getch();
                    continue;
            }

            // Update the record
            fseek(file, student_pos, SEEK_SET);
            fwrite(&st, sizeof(students), 1, file);
            printf("\n\tInformation updated successfully ( =^.^=)!\n");
            getch();

            // Display updated information
            system("cls");
            printf("\n");
            printf("\t+========================================================+\n");
            printf("\t|              Study Center Management System            |\n");
            printf("\t|========================================================|\n");
            printf("\t|               Update Student Information               |\n");
            printf("\t+========================================================+\n\n");
            printf("\n\tUpdated Student Information:\n");
            printf("\t+--------------------------------------------------------+\n");
            printf("\t| %-20s | %s\n", "ID:", st.student_ID);
            printf("\t| %-20s | %s\n", "Full Name:", st.student_name);
            printf("\t| %-20s | %s\n", "Phone Number:", st.student_phone);
            printf("\t| %-20s | %s\n", "Email:", st.student_email);
            printf("\t+--------------------------------------------------------+\n\n");

        } while (update_choice != 4);
    }

    fclose(file);
    printf("\n\n");
    printf("\t                        (\\(\\ \n");
    printf("\tPress any key to return ( -.-) \n");
    getch();
}

void course_update_info() {
    FILE *course_file, *class_file, *student_file;
    courses cs;
    classes cl;
    students st;
    char course_search[MAX_ID_LENGTH];
    char course_ex_name[MAX_NAME_LENGTH];
    bool course_found = false;
    bool over_max = false;
    bool ex_check = false;
    long int course_pos;
    long int student_pos;
    int update_choice;
    int highest_student_count = 0;
    int course_count = 0;
    bool student_check = false;
    // Open file in read and write mode
    course_file = fopen("ex_course.txt", "r+b");
    class_file = fopen("ex_class.txt","r+b");
    student_file = fopen("ex_student.txt","r+b");
    if (course_file == NULL || class_file == NULL) {
        printf("\tError opening files (>_<)!\n");
        getch();
        return;
    }
    if (student_file == NULL) {
        student_check = false;
        fclose(student_file);
    } else {
        student_check = true;
    }

    // Get course ID
    rewind(course_file);
    do {
        system("cls");
        printf("\n");
        printf("\t+========================================================+\n");
        printf("\t|              Study Center Management System            |\n");
        printf("\t|========================================================|\n");
        printf("\t|                 Update Course Information              |\n");
        printf("\t+========================================================+\n\n");

        // Print all course ID and names
        printf("\t%-5s |%-15s |%-30s |%-30s\n", 
            "No.", "ID", "Course Name", "Fee");
        printf("\t+--------------------------------------------------------+\n");

        while (fread(&cs, sizeof(courses), 1, course_file) == 1) {
            course_count++;
            printf("\t%-5d |%-15s |%-30s |%-10d\n", 
                course_count, cs.course_ID, cs.course_name, cs.tuition);
        }
        printf("\t+--------------------------------------------------------+\n\n");

        printf("\n\tEnter Course ID to update: ");
        fflush(stdin);
        fgets(course_search, sizeof(course_search), stdin);
        course_search[strcspn(course_search, "\n")] = 0;
        if (strlen(course_search) == 0) {
            printf("\tError: Course ID cannot be empty (>_<)!\n");
        }
    } while (strlen(course_search) == 0);

    // Find course
    rewind(course_file);
    while (fread(&cs, sizeof(courses), 1, course_file) == 1) {
        if (strcmp(cs.course_ID, course_search) == 0) {
            course_found = true;
            course_pos = ftell(course_file) - sizeof(courses);
            break;
        }
    }

    if (!course_found) {
        printf("\n\tCourse ID [ %s ] not found (>_<)!\n", course_search);
    } else {
        system("cls");
        system("cls");
        printf("\n");
        printf("\t+========================================================+\n");
        printf("\t|              Study Center Management System            |\n");
        printf("\t|========================================================|\n");
        printf("\t|                 Update Course Information              |\n");
        printf("\t+========================================================+\n\n");

        // Display current information
        printf("\n\tCurrent Course Information:\n");
        printf("\t+--------------------------------------------------------+\n");
        printf("\t| %-25s | %s\n", "ID:", cs.course_ID);
        printf("\t| %-25s | %s\n", "Course Name:", cs.course_name);
        printf("\t| %-25s | %d\n", "Fee:", cs.tuition);
        printf("\t| %-25s | %d\n", "Maximum In a Class:", cs.max_students);
        printf("\t+--------------------------------------------------------+\n\n");

        do {
            printf("\tWhat would you like to update?\n\n");
            printf("\t\t* 1. Name\n");
            printf("\t\t* 2. Fee\n");
            printf("\t\t* 3. Maximum allowed in class\n");
            printf("\t\t* 4. Return\n");
            printf("\t+--------------------------------------------------------+\n\n");
            printf("\tEnter your choice (1-4): ");

            while (scanf("%d", &update_choice) != 1) {
                while (getchar() != '\n');
                printf("\t(!_!) Invalid input! Please enter a number (1-4): ");
                _getch();
                eraseLines(2);
            }

            switch(update_choice) {
                case 1: // Update Name
                    strcpy(course_ex_name, cs.course_name);
                    do {
                        printf("\n\tEnter new name: ");
                        fflush(stdin);
                        fgets(cs.course_name, sizeof(cs.course_name), stdin);
                        cs.course_name[ strcspn(cs.course_name, "\n") ] =0;
                        if (strlen(cs.course_name) == 0) {
                            printf("\tCourse's Name cannot be empty (>_<)!");
                            eraseLines(2);
                        }
                    } while (strlen(cs.course_name) == 0);

                    if (cs.total_students > 0 && student_check) {
                        student_pos = 0;
                    
                        printf("\n\tUpdating All Student Studying the Course...\n");
                        do {
                            ex_check = false;
                            fseek(student_file, student_pos, SEEK_SET);
                            while(fread(&st, sizeof(students), 1, student_file) == 1) {
                                if (strcmp(st.course_attend, course_ex_name) == 0) {
                                    strcpy(st.course_attend, cs.course_name);
                                    student_pos = ftell(student_file) - sizeof(students);
                                    ex_check = true;
                                    break;
                                }
                            }

                            if (ex_check) {
                                fseek(student_file, student_pos, SEEK_SET);
                                fwrite(&st, sizeof(students), 1, student_file);
                            }
                        } while (ex_check);
                        Sleep(1000);
                        printf("\n\tFinished!\n");
                    }
                    break;

                case 2: // Update Maximum Amount in A Class
                    printf("\n\tCurrent Max Allowed in Class: %d", cs.max_students);
                    while (fread(&cl, sizeof(classes), 1, class_file) == 1) {
                        if (strcmp(cl.course_ID, cs.course_ID) == 0) {
                            if (cl.total_students == cs.max_students) {
                                highest_student_count = cs.max_students;
                                break;
                            }
                            else if (highest_student_count < cl.total_students) {
                                highest_student_count = cl.total_students;
                            }
                        }
                    }
                    printf("\n\tHighest number currently in Class: %d", highest_student_count);
                    printf("\n\t(The system will not update if you enter a value below %d)", highest_student_count);
                    do {
                        do {
                            printf("\n\n\tEnter new number of allowed student in a class: ");
                            while (scanf("%d", &cs.max_students) != 1) {
                                while (getchar() != '\n');
                                printf("\t (!_!) Invalid input! Please enter only numeric value!");
                                _getch();
                                eraseLines(2);
                            }
                            if (cs.max_students <= 0) {
                                printf("\tThere must be at least 1 student in class (>_<)!");
                                eraseLines(2);
                                cs.max_students = 0;
                            }
                        } while (cs.max_students == 0);

                        if (cs.max_students < highest_student_count) {
                            printf("\tCurrent Amount of Student in class exceed new maximum of students.");
                            eraseLines(2);
                            getch();
                        }
                        else {
                            over_max = true;
                            printf("\n\tUpdating...");
                            Sleep(1000);
                            printf("\n\tFinished!");
                        }
                    } while (!over_max);
                    break;

                case 4: // Return
                    if (student_check) {
                        fclose(student_file);
                    }
                    fclose(course_file);
                    fclose(class_file);
                    return;

                default:
                    printf("\tInvalid choice! Please select 1-4 only (>_<)!\n");
                    getch();
                    continue;
            }

            // Update the record
            fseek(course_file, course_pos, SEEK_SET);
            fwrite(&cs, sizeof(courses), 1, course_file);
            printf("\n\tInformation updated successfully ( =^.^=)!\n");
            getch();
            
            system("cls");
            printf("\n");
            printf("\t+========================================================+\n");
            printf("\t|              Study Center Management System            |\n");
            printf("\t|========================================================|\n");
            printf("\t|                 Update Course Information              |\n");
            printf("\t+========================================================+\n\n");
            // Display updated information
            printf("\n\tCurrent Course Information:\n");
            printf("\t+--------------------------------------------------------+\n");
            printf("\t| %-25s | %s\n", "ID:", cs.course_ID);
            printf("\t| %-25s | %s\n", "Course Name:", cs.course_name);
            printf("\t| %-25s | %d\n", "Fee:", cs.tuition);
            printf("\t| %-25s | %d\n", "Maximum In a Class:", cs.max_students);
            printf("\t+--------------------------------------------------------+\n\n");

        } while (update_choice != 4);
    }

    fclose(course_file);
    printf("\n\n");
    printf("                        (\\(\\ \n");
    printf("Press any key to return ( -.-) \n");
    getch();
}

void class_update_info() {
    FILE *class_file, *student_file;
    classes cl;
    students st;
    char class_search[MAX_ID_LENGTH];
    char class_ex_name[MAX_NAME_LENGTH];
    bool class_found = false;
    bool ex_check = false;
    long int class_pos;
    long int student_pos;
    int class_count = 0;
    int update_choice;
    bool student_check = false;

    // Open file
    class_file = fopen("ex_class.txt", "r+b");
    student_file = fopen("ex_student.txt", "r+b");
    
    if (class_file == NULL) {
        printf("Error opening file (>_<)!\n");
        getch();
        return;
    }
    
    if (student_file == NULL) {
        student_check = false;
        fclose(student_file);
    } else {
        student_check = true;
    }

    system("cls");
    printf("\n");
    printf("\t+========================================================+\n");
    printf("\t|              Study Center Management System            |\n");
    printf("\t|========================================================|\n");
    printf("\t|                 Update Class Information               |\n");
    printf("\t+========================================================+\n\n");

    // Print all course ID and names
    printf("\t%-5s |%-15s |%-30s |%-30s\n", 
           "No.", "ID", "Class Name", "Professor");
    printf("\t+--------------------------------------------------------+\n");

    while (fread(&cl, sizeof(classes), 1, class_file) == 1 ) {
        class_count++;
        printf("\t%-5d |%-15s |%-30s |%-30s\n",
            class_count, cl.class_ID, cl.class_name, cl.class_teacher);
    }
    printf("\t+--------------------------------------------------------+\n");

    // Get Class ID
    rewind(class_file);
    
    printf("\n\tEnter Class ID to Update: ");
    do {
        fflush(stdin);
        fgets(class_search, MAX_ID_LENGTH, stdin);
        class_search[ strcspn(class_search, "\n") ] =0;
        if (strlen(class_search) > 0) {
            break;
        }
        else {
            printf("\n\tError: Class ID cannot be empty (>_<)!");
            eraseLines(2);
            printf("\n\tEnter Class ID to Update: ");
        }
    } while (true);

    // Find Class
    while (fread(&cl, sizeof(classes), 1, class_file) == 1) {
        if (strcmp(cl.class_ID, class_search) == 0) {
            class_found = true;
            class_pos = ftell(class_file) - sizeof(classes);
            break;
        }
    }

    if (!class_found) {
        printf("\n\tClass ID [ %s ] not found (>_<)!\n", class_search);
    } else {
        system("cls");
        printf("\n");
        printf("\t+========================================================+\n");
        printf("\t|              Study Center Management System            |\n");
        printf("\t|========================================================|\n");
        printf("\t|                 Update Class Information               |\n");
        printf("\t+========================================================+\n\n");

        // Display current information
        printf("\n\tCurrent Class Information:\n");
        printf("\t+--------------------------------------------------------+\n");
        printf("\t| %-25s | %s\n", "ID:", cl.class_ID);
        printf("\t| %-25s | %s\n", "Class Name:", cl.class_name);
        printf("\t| %-25s | %s\n", "Proffesor:", cl.class_teacher);
        printf("\t+--------------------------------------------------------+\n\n");

        do {
            printf("\tWhat would you like to update?\n\n");
            printf("\t\t* 1. Name\n");
            printf("\t\t* 2. Professor\n");
            printf("\t\t* 3. Return\n");
            printf("\tEnter your choice (1-3): ");

            while (scanf("%d", &update_choice) != 1) {
                while (getchar() != '\n');
                printf("\t(!_!) Invalid input! Please enter a number (1-4): ");
                _getch();
                eraseLines(2);
            }

            switch(update_choice) {
                case 1: // Update Name
                    strcpy(class_ex_name, cl.class_name);
                    do {
                        printf("\n\tEnter new name: ");
                        fflush(stdin);
                        fgets(cl.class_name, MAX_NAME_LENGTH, stdin);
                        cl.class_name[ strcspn(cl.class_name, "\n") ] =0;
                        if (strlen(cl.class_name) == 0) {
                            printf("\tClass's Name cannot be empty (>_<)!");
                            eraseLines(2);
                        }
                    } while (strlen(cl.class_name) == 0);

                    if (cl.total_students > 0 && student_check) {
                        student_pos = 0;

                        printf("\n\tUpdating All Student currently in Class...\n");
                        do {
                            ex_check = false;
                            fseek(student_file, student_pos, SEEK_SET);
                            while(fread(&st, sizeof(students), 1, student_file) == 1) {
                                if (strcmp(st.class_attend, class_ex_name) == 0) {
                                    strcpy(st.class_attend, cl.class_name);
                                    student_pos = ftell(student_file) - sizeof(students);
                                    ex_check = true;
                                    break;
                                }
                            }

                            if (ex_check) {
                                fseek(student_file, student_pos, SEEK_SET);
                                fwrite(&st, sizeof(students), 1, student_file);
                            }
                        } while (ex_check);
                        Sleep(1000);
                        printf("\n\tFinished!\n");
                    }
                    break;

                case 2: // Update Professor
                    printf("\n\tCurrent Professor: %s", cl.class_teacher);
                    do {
                        printf("\n\tEnter new Professor's Name: ");
                        fflush(stdin);
                        fgets(cl.class_teacher, MAX_NAME_LENGTH, stdin);
                        cl.class_teacher[ strcspn(cl.class_teacher, "\n") ] =0;

                        if (strlen(cl.class_teacher) == 0) {
                            printf("\tProfessor's Name cannot be empty (>_<)!");
                            eraseLines(2);
                        }
                    } while (strlen(cl.class_name) == 0);
                    break;
                
                case 3: // Return
                    if (student_check) {
                        fclose(student_file);
                    }
                    return;

                default:
                    printf("\tInvalid choice! Please select 1-4 only (>_<)!\n");
                    getch();
                    continue;
            }


            // Update The Record
            fseek(class_file, class_pos, SEEK_SET);
            fwrite(&cl, sizeof(classes), 1, class_file);
            printf("\n\tInformation Updated Sucessfully ( =^.^=)!\n");
            getch();

           system("cls");
            printf("\n");
            printf("\t+========================================================+\n");
            printf("\t|              Study Center Management System            |\n");
            printf("\t|========================================================|\n");
            printf("\t|                 Update Class Information               |\n");
            printf("\t+========================================================+\n\n");

            // Display updated information
            printf("\n\tCurrent Class Information:\n");
            printf("\t+--------------------------------------------------------+\n");
            printf("\t| %-25s | %s\n", "ID:", cl.class_ID);
            printf("\t| %-25s | %s\n", "Class Name:", cl.class_name);
            printf("\t| %-25s | %s\n", "Proffesor:", cl.class_teacher);
            printf("\t+--------------------------------------------------------+\n\n");

        } while (update_choice != 3);
    }

    fclose(class_file);
    printf("\n\n");
    printf("                        (\\(\\ \n");
    printf("Press any key to return ( -.-) \n");
    getch();
}


// List Print Functions
void student_list() {
    FILE *file;
    students st;
    int student_count = 0;
    int choice_view;
    char student_search[MAX_ID_LENGTH];
    bool search_check = false;

    file = fopen("ex_student.txt", "rb");
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
    printf("\t|                          List of Students                              |\n");
    printf("\t+========================================================================+\n\n");

    // Header row with consistent formatting
    printf("\t+------+----------------+--------------------------------+");
    printf("--------------------------------+---------------+\n");
    printf("\t| %-4s | %-14s | %-30s | %-30s | %-13s |\n",
           "No.", "ID", "Name", "Class", "Tuition");
    printf("\t+------+----------------+--------------------------------+");
    printf("--------------------------------+---------------+\n");

    // Data rows with consistent alignment
    while (fread(&st, sizeof(students), 1, file) == 1) {
        student_count++;
        printf("\t| %-4d | %-14s | %-30s | %-30s | %13.2f |\n",
            student_count, st.student_ID, st.student_name, st.class_attend, st.tuition_paid);
    }

    // Bottom border
    printf("\t+------+----------------+--------------------------------+");
    printf("--------------------------------+---------------+\n");

    if (student_count == 0) {
        printf("\t|                      Cannot Detect any Student (>~<)                        |\n");
        printf("\t+------------------------------------------------------------------------+\n");
    } else {
        printf("\n\tTotal number of Student(s): %d\n\n", student_count);

        printf("\t\t* 1: View Detailed Information Of A Student\n");
        printf("\t\t* 2: Sort Student Information\n");
        printf("\t\t* 3: Print List Out\n");
        printf("\t\t* 4: Return\n");
        printf("\t+------------------------------------------------------+\n");
        printf("\t  Enter your choice (1-4): ");
    }

    while (scanf("%d", &choice_view) != 1) {
        while (getchar() != '\n');
        printf("\t  (!_!) Invalid input! Please enter a number (1-4): ");
        _getch();
        eraseLines(2);
    }
    
    switch(choice_view) {
        case 1:
            system("cls");
            printf("\n");
            printf("\t+========================================================+\n");
            printf("\t|              Study Center Management System            |\n");
            printf("\t|========================================================|\n");
            printf("\t|                  Student Information                   |\n");
            printf("\t+========================================================+\n\n");
            
            do {
                system("cls");
                printf("\n");
                printf("\t+========================================================+\n");
                printf("\t|              Study Center Management System            |\n");
                printf("\t|========================================================|\n");
                printf("\t|                  Student Information                   |\n");
                printf("\t+========================================================+\n\n");
                printf("\tEnter Student's ID to look: ");
                fflush(stdin);
                fgets(student_search, sizeof(student_search), stdin);
                student_search[strcspn(student_search, "\n")] =0;
                if (strlen(student_search) == 0) {
                    printf("\tError: Student ID cannot be empty (>_<)!\n");
                    system("pause");
                }
            } while (strlen(student_search) == 0);
            
            rewind(file);
            while (fread(&st, sizeof(students), 1, file) == 1) {
                if (strcmp(student_search, st.student_ID) == 0) {
                    printf("\n\t+--------------------------------------------------------+\n");
                    printf("\t| %-20s | %s \n", "ID:", st.student_ID);
                    printf("\t| %-20s | %s \n", "Full Name:", st.student_name);
                    printf("\t| %-20s | %s \n", "Phone Number:", st.student_phone);
                    printf("\t| %-20s | %s \n", "Email:", st.student_email);
                    printf("\t| %-20s | %s \n", "Course:", st.course_attend);
                    printf("\t| %-20s | %s \n", "Class:", st.class_attend);
                    printf("\t| %-20s | %.2f |\n", "Tuition Paid:", st.tuition_paid);
                    printf("\t+--------------------------------------------------------+\n");
                    search_check = true;
                }
            }
            if (!search_check) {
                printf("\n\tStudent ID [ %s ] was not found (>~<)!", student_search);
            }
            printf("\n\n");
            printf("                        (\\(\\ \n");
            printf("Press any key to return ( -.-) \n");
            getch();
            break;
        case 2:
            do {
                system("cls");
                printf("\n");
                printf("\t+========================================================+\n");
                printf("\t|              Study Center Management System            |\n");
                printf("\t|========================================================|\n");
                printf("\t|                  Sort Student Information              |\n");
                printf("\t+========================================================+\n\n");
                printf("\t\tSelect sorting criteria:\n\n");
                printf("\t\t* 1: Sort by Name (A-Z)\n");
                printf("\t\t* 2: Sort by Name (Z-A)\n");
                printf("\t\t* 3: Sort by Tuition (Ascending)\n");
                printf("\t\t* 4: Sort by Tuition (Descending)\n");
                printf("\t\t* 5: Return\n");
                printf("\t+------------------------------------------------------+\n");
                printf("\t  Enter your choice (1-5): ");

                int sort_choice;
                while (scanf("%d", &sort_choice) != 1) {
                    while (getchar() != '\n');
                    printf("\t  (!_!) Invalid input! Please enter a number (1-5): ");
                    _getch();
                    eraseLines(2);
                }

                switch(sort_choice) {
                    case 1:
                        student_sort_ascending();
                        break;
                    case 2:
                        student_sort_descending();
                        break;
                    case 3:
                        student_sort_tuition_ascending();
                        break;
                    case 4:
                        student_sort_tuition_descending();
                        break;
                    case 5:
                        return;
                    default:
                        printf("\t  Invalid choice! Please select 1-5 only (>_<)!\n");
                        getch();
                        break;
                }
            } while (1);
            break;
        case 3:
            FILE *print;
            print = fopen("student_list.txt","w+b");
            
            if (print == NULL) {
                printf("Error opening file(s) (>_<)!\n");
                break;
            }

            rewind(file);
            student_count = 0;

            // Print
            fprintf(print,"| %-4s | %-14s | %-30s | %-30s | %-13s |\n",
           "No.", "ID", "Name", "Class", "Tuition");
            fprintf(print,"-----------------------------------------------------------------------------+---------------+\n");
            while (fread(&st, sizeof(students), 1, file) == 1) {
                student_count++;
                fprintf(print,"| %-4d | %-14s | %-30s | %-30s | %13.2f |\n",
                student_count, st.student_ID, st.student_name, st.class_attend, st.tuition_paid);
            }
            fclose(print);
            break;
        case 4:
            printf("\n\n");
            printf("                        (\\(\\ \n");
            printf("Press any key to return ( -.-) \n");
            getch();
            break;
    }
    fclose(file);
}

void course_list() {
    FILE *file;
    courses cs;
    int course_count = 0;
    int choice_view;
    char course_search[MAX_ID_LENGTH];
    bool search_check = false;

    //Open the file in binary read mode
    file = fopen("ex_course.txt", "rb");
    if (file == NULL) {
        printf("\tError opening file or file doesn't exist (>_<)!\n");

        printf("\n\n");
        printf("                        (\\(\\ \n");
        printf("Press any key to return ( -.-) \n");
        getch();
        return;
    }

    system("cls");  // Clear the screen

    printf("\n");
    printf("\t+========================================================================+\n");
    printf("\t|                    Study Center Management System                      |\n");
    printf("\t|========================================================================|\n");
    printf("\t|                          List of Courses                               |\n");
    printf("\t+========================================================================+\n\n");

    printf("\t+------+----------------+--------------------------------+---------------+\n");
    printf("\t| %-4s | %-14s | %-30s | %-13s |\n", 
           "No.", "Course ID", "Course Name", "Tuition Cost");
    printf("\t+------+----------------+--------------------------------+---------------+\n");

    while (fread(&cs, sizeof(courses), 1, file) == 1) {
        course_count++;
        printf("\t| %-4d | %-14s | %-30s | %13d |\n", 
            course_count, cs.course_ID, cs.course_name, cs.tuition);
    }

    printf("\t+------+----------------+--------------------------------+---------------+\n");

    if (course_count == 0) {
        printf("\tCannot detect any Course (>~<).\n");
        getch();
    } else {
        printf("\nTotal number of Course(s): %d\n\n", course_count);

        printf("\t\t* 1. View Detailed Information Of A Course\n");
        printf("\t\t* 2. Print List Out\n");
        printf("\t\t* 3. Return\n");
        printf("\t+--------------------------------------------------------+\n\n");

        printf("\tEnter your choice (1-3) : ");

        while (scanf("%d", &choice_view) != 1) {
            while (getchar() != '\n');
            printf("\t  (!_!) Invalid input (>_<)! Please enter a number (1-3): ");
            _getch();
            eraseLines(2);
        }
        switch(choice_view) {
            case 1:
                system("cls");
                printf("\n\tEnter Course's ID to look: ");
                fflush(stdin);
                fgets(course_search, sizeof(course_search), stdin);
                course_search[ strcspn(course_search, "\n") ] =0;
                rewind(file);
                while (fread (&cs, sizeof(courses), 1, file) == 1) {
                    if (strcmp(course_search, cs.course_ID) == 0) {
                        printf("\n\n%-25s %s", "ID:", cs.course_ID);
                        printf("\n%-25s %s", "Course Name:", cs.course_name);
                        printf("\n%-25s %d", "Fee:", cs.tuition);
                        printf("\n%-25s %d", "Number Of Class(es):", cs.total_class);
                        printf("\n%-25s %d", "Number Of Student(s):", cs.total_students);
                        search_check = true;
                    }
                }
                if (!search_check) {
                    printf("\nStudent ID [ %s ] was not found.", course_search);
                }             
                printf("\n\n");
                printf("                        (\\(\\ \n");
                printf("Press any key to return ( -.-) \n");
                getch();
                break;
            case 2:
                FILE *print;
                print = fopen("course_list.txt","w+b");
            
                if (print == NULL) {
                    printf("Error opening file(s) (>_<)!\n");
                    break;
                }

                rewind(file);
                course_count = 0;

                // Print
                fprintf(print, "| %-4s | %-14s | %-30s | %-13s |\n", 
                    "No.", "Course ID", "Course Name", "Tuition Cost");
                fprintf(print, "\t+------+----------------+--------------------------------------------------+---------------+\n");

                while (fread(&cs, sizeof(courses), 1, file) == 1) {
                    course_count++;
                    fprintf(print, "\t| %-4d | %-14s | %-30s | %13d |\n", 
                        course_count, cs.course_ID, cs.course_name, cs.tuition);
                }
                fclose(print);
                break;

            case 3:
                printf("\n\n");
                printf("                        (\\(\\ \n");
                printf("Press any key to return ( -.-) \n");
                getch();
                break;
        }
    }
    fclose(file);
}

void class_list() {
    FILE *file;
    classes cl;
    int class_count = 0;
    int choice_view;

    // Open the file in binary read mode
    file = fopen("ex_class.txt", "rb");
    if (file == NULL) {
        printf("Error opening file or file doesn't exist (>_<)!\n");

        printf("\n\n");
        printf("                        (\\(\\ \n");
        printf("Press any key to return ( -.-) \n");
        getch();
        return;
    }

    system("cls");  // Clear the screen
    
    printf("\n");
    printf("\t+========================================================================+\n");
    printf("\t|                    Study Center Management System                      |\n");
    printf("\t|========================================================================|\n");
    printf("\t|                          List of Classes                               |\n");
    printf("\t+========================================================================+\n\n");

    printf("\t+------+----------------+--------------------------------+\n");
    printf("\t| %-4s | %-14s | %-30s |\n");
    printf("\t+------+----------------+--------------------------------+\n");

    while (fread(&cl, sizeof(classes), 1, file) == 1) {
        class_count++;
        printf("\t| %-4d | %-14s | %-30s |\n", 
            class_count, cl.class_ID, cl.class_name);
    }

    printf("\t+------+----------------+--------------------------------+\n");

    if (class_count == 0) {
        printf("Cannot detect any Class (>~<).\n");
    } else {
        printf("\nTotal number of Class(es): %d\n\n", class_count);

        printf("\n1. View Detailed Information Of A Class");
        printf("\n2. Print List Out");
        printf("\n3. Return");

        printf("\n\nEnter your choice (1-3) : ");

        while (scanf("%d", &choice_view) != 1) {
            while (getchar() != '\n');
            printf("\t  (!_!) Invalid input (>_<)! Please enter a number (1-3): ");
            _getch();
            eraseLines(2);
        }
        switch(choice_view) {
            case 1:
                class_view();
                break;
            case 2:
                FILE *print;
                print = fopen("class_list.txt","w+b");
            
                if (print == NULL) {
                    printf("Error opening file(s) (>_<)!\n");
                    break;
                }

                rewind(file);
                class_count = 0;

                // Print
                fprintf(print, "| %-4s | %-14s | %-30s |\n", 
                    "No.", "Class ID", "Class Name");
                fprintf(print, "\t+------+----------------+-----------------------------------------------+\n");

                while (fread(&cl, sizeof(classes), 1, file) == 1) {
                    class_count++;
                    fprintf(print, "| %-4d | %-14s | %-30s |\n", 
                    class_count, cl.class_ID, cl.class_name);
                }
                fclose(print);
                break;

            case 3:
                printf("\n\n");
                printf("                        (\\(\\ \n");
                printf("Press any key to return ( -.-) \n");
                getch();
                break;
        }
    }
    fclose(file);
}


// Menu System

// Staff Sub Menu
void sub_update_student() {
    int choice_update_student;
    
    do {
        system("cls");
        printf("\n");
        printf("\t+========================================================+\n");
        printf("\t|              Study Center Management System            |\n");
        printf("\t|========================================================|\n");
        printf("\t|                     Student Update                     |\n");
        printf("\t+========================================================+\n\n");
        printf("\n\t\tSelect your choice:\n\n");
        printf("\t\t* 1: Update a Student's Information\n");
        printf("\t\t* 2: Register a Student Into Class\n");
        printf("\t\t* 3: Remove a Student from Class\n");
        printf("\t\t* 4: Return\n");
        printf("\t+------------------------------------------------------+\n");
        printf("\t  Enter your choice (1-4): ");

        while (scanf("%d", &choice_update_student) != 1) {
            while (getchar() != '\n');
            printf("\t  (!_!) Invalid input! Please enter a number (1-4): ");
            _getch();
            eraseLines(2);
        }

        switch(choice_update_student) {
            case 1:
                student_update_info();
                break;
            case 2:
                class_register();
                break;
            case 3:
                class_unregister();
            case 4:
                break;
            default:
                printf("From 1 to 4 only (>~<)!\n");
                getch();
                break;
        }
    } while (choice_update_student != 4);
    return;
}


void sub_student() {
    int choice_student;

    do {
        system("cls");
        printf("\n");
        printf("\t+========================================================+\n");
        printf("\t|              Study Center Management System            |\n");
        printf("\t|========================================================|\n");
        printf("\t|                    Student Management                  |\n");
        printf("\t+========================================================+\n\n");
        printf("\n\t\tSelect your choice:\n\n");
        printf("\t\t* 1: Add\n");
        printf("\t\t* 2: Update\n");
        printf("\t\t* 3: Delete\n");
        printf("\t\t* 4: Return\n");
        printf("\t+------------------------------------------------------+\n");
        printf("\t  Enter your choice (1-4): ");

        while (scanf("%d", &choice_student) != 1) {
            while (getchar() != '\n');
            printf("\t  (!_!) Invalid input! Please enter a number (1-4): ");
            _getch();
            eraseLines(2);
        }

        switch(choice_student) {
            case 1:
                student_add();
                break;
            case 2:
                sub_update_student();
                break;
            case 3:
                student_delete();
                break;
            case 4:
                break;
            default:
                printf("From 1 to 4 only (>~<)!\n");
                getch();
                break;
        }
    } while (choice_student != 4);
    return;
}

void sub_update_class() {
    int choice_update_class;

    do {
        system("cls");
        printf("\n");
        printf("\t+========================================================+\n");
        printf("\t|              Study Center Management System            |\n");
        printf("\t|========================================================|\n");
        printf("\t|                      Class Update                      |\n");
        printf("\t+========================================================+\n\n");
        printf("\n\t\tSelect your choice:\n\n");
        printf("\t\t* 1: Update a Class's Information\n");
        printf("\t\t* 2: Register a Student Into Class\n");
        printf("\t\t* 3: Remove a Student from Class\n");
        printf("\t\t* 4: Return\n");
        printf("\t+------------------------------------------------------+\n");
        printf("\t  Enter your choice (1-4): ");

        while (scanf("%d", &choice_update_class) != 1) {
            while (getchar() != '\n');
            printf("\t  (!_!) Invalid input! Please enter a number (1-4): ");
            _getch();
            eraseLines(2);
        }

        switch(choice_update_class) {
            case 1:
                student_update_info();
                break;
            case 2:
                class_register();
                break;
            case 3:
                class_unregister();
            case 4:
                break;
            default:
                printf("From 1 to 4 only (>~<)!\n");
                getch();
                break;
        }
    } while (choice_update_class != 4);
    return;
}

void sub_class() {
    int choice_class;

    do {
        system("cls");
        printf("\n");
        printf("\t+========================================================+\n");
        printf("\t|              Study Center Management System            |\n");
        printf("\t|========================================================|\n");
        printf("\t|                     Class Management                   |\n");
        printf("\t+========================================================+\n\n");
        printf("\n\t\tSelect your choice:\n\n");
        printf("\t\t* 1: Register a Student to Class\n"
               "\t\t* 2: Remove a Student from Class\n"
               "\t\t* 3: Return\n");
        printf("\t+--------------------------------------------------------+\n");
        printf("\t  Enter your choice (1-3): ");

        while (scanf("%d", &choice_class) != 1) {
            while (getchar() != '\n');
            printf("\t  (!_!) Invalid input! Please enter a number (1-3): ");
            _getch();
            eraseLines(2);
        }

        switch(choice_class) {
            case 1:
                class_register();
                break;
            case 2:
                class_unregister();
                break;
            case 3:
                break;
            default:
                printf("From 1 to 3 only (>~<)!\n");
                getch();
                break;
        }
    } while (choice_class != 3);
    return;
}

void sub_course() {
    int choice_course;

    do {
        system("cls");
        printf("\n");
        printf("\t+========================================================+\n");
        printf("\t|              Study Center Management System            |\n");
        printf("\t|========================================================|\n");
        printf("\t|                    Course Management                   |\n");
        printf("\t+========================================================+\n\n");
        printf("\n\t\tSelect your choice:\n\n");
        printf("\t\t* 1: Add\n"
               "\t\t* 2: Update\n"
               "\t\t* 3: Delete\n"
               "\t\t* 4: Return\n");
        printf("\t+--------------------------------------------------------+\n");
        printf("\t  Enter your choice (1-4): ");

        while (scanf("%d", &choice_course) != 1) {
            while (getchar() != '\n');
            printf("\t  (!_!) Invalid input! Please enter a number (1-4): ");
            _getch();
            eraseLines(2);
        }

        switch(choice_course) {
            case 1:
                course_add();
                break;
            case 2:
                course_update_info();
                break;
            case 3:
                course_delete();
                break;
            case 4:
                break;
            default:
                printf("From 1 to 4 only (>~<)!\n");
                getch();
                break;
        }
    } while (choice_course != 4);
    return;
}

void sub_view() {
    int choice_view;

    do {
        system("cls");
        printf("\n");
        printf("\t+========================================================+\n");
        printf("\t|              Study Center Management System            |\n");
        printf("\t|========================================================|\n");
        printf("\t|                     Information View                   |\n");
        printf("\t+========================================================+\n\n");
        printf("\n\t\tSelect your choice:\n\n");
        printf("\t\t* 1: Student\n");
        printf("\t\t* 2: Course\n");
        printf("\t\t* 3: Class\n");
        printf("\t\t* 4: Return\n");
        printf("\t+------------------------------------------------------+\n");
        printf("\t  Enter your choice (1-4): ");

        while (scanf("%d", &choice_view) != 1) {
            while (getchar() != '\n');
            printf("\t  (!_!) Invalid input! Please enter a number (1-4): ");
            _getch();
            eraseLines(2);
        }

        switch(choice_view) {
            case 1:
                student_list();
                break;
            case 2:
                course_list();
                break;
            case 3:
                class_list();
                break;
            case 4:
                break;
            default:
                printf("From 1 to 4 only (>~<)!\n");
                getch();
                break;
        }
    } while (choice_view != 4);
    return;
}


// Head Admin Sub Menu

void sub_student_admin() {
    int choice_category, choice_action;

    do {
        system("cls");
        printf("\n");
        printf("\t+========================================================+\n");
        printf("\t|              Study Center Management System            |\n");
        printf("\t|========================================================|\n");
        printf("\t|                    Student Management                  |\n");
        printf("\t+========================================================+\n\n");
        printf("\n\t\tSelect a category:\n\n");
        printf("\t\t* 1: Student Records\n");
        printf("\t\t* 2: Class Registration\n");
        printf("\t\t* 3: Financial Management\n");
        printf("\t\t* 4: Return\n");
        printf("\t+------------------------------------------------------+\n");
        printf("\t  Enter your choice (1-4): ");

        while (scanf("%d", &choice_category) != 1) {
            while (getchar() != '\n');
            printf("\t  (!_!) Invalid input! Please enter a number (1-4): ");
            _getch();
            eraseLines(2);
        }

        switch(choice_category) {
            case 1: // Student Records
                system("cls");
                printf("\n");
                printf("\t+========================================================+\n");
                printf("\t|              Study Center Management System            |\n");
                printf("\t|========================================================|\n");
                printf("\t|                  Student Records                       |\n");
                printf("\t+========================================================+\n\n");
                printf("\n\t\tSelect your choice:\n\n");
                printf("\t\t* 1: Add New Student\n");
                printf("\t\t* 2: Update Student Information\n");
                printf("\t\t* 3: Delete Student\n");
                printf("\t\t* 4: View All Students\n");
                printf("\t\t* 5: Return\n");
                printf("\t+------------------------------------------------------+\n");
                printf("\t  Enter your choice (1-5): ");
                
                while (scanf("%d", &choice_action) != 1) {
                    while (getchar() != '\n');
                    printf("\t  (!_!) Invalid input! Please enter a number (1-5): ");
                    _getch();
                    eraseLines(2);
                }

                switch(choice_action) {
                    case 1: student_add(); break;
                    case 2: student_update_info(); break;
                    case 3: student_delete(); break;
                    case 4: student_list(); break;
                    case 5: break;
                    default: 
                        printf("\t  Invalid choice! Please select 1-5 only (>_<)!\n");
                        getch();
                }
                break;

            case 2: // Class Registration
                system("cls");
                printf("\n");
                printf("\t+========================================================+\n");
                printf("\t|              Study Center Management System            |\n");
                printf("\t|========================================================|\n");
                printf("\t|                Class Registration                      |\n");
                printf("\t+========================================================+\n\n");
                printf("\n\t\tSelect your choice:\n\n");
                printf("\t\t* 1: Register Student to Class\n");
                printf("\t\t* 2: Remove Student from Class\n");
                printf("\t\t* 3: Return\n");
                printf("\t+------------------------------------------------------+\n");
                printf("\t  Enter your choice (1-3): ");
                
                while (scanf("%d", &choice_action) != 1) {
                    while (getchar() != '\n');
                    printf("\t  (!_!) Invalid input! Please enter a number (1-3): ");
                    _getch();
                    eraseLines(2);
                }

                switch(choice_action) {
                    case 1: class_register(); break;
                    case 2: class_unregister(); break;
                    case 3: break;
                    default:
                        printf("\t  Invalid choice! Please select 1-3 only (>_<)!\n");
                        getch();
                }
                break;

            case 3: // Financial Management
                system("cls");
                printf("\n");
                printf("\t+========================================================+\n");
                printf("\t|              Study Center Management System            |\n");
                printf("\t|========================================================|\n");
                printf("\t|                Financial Management                    |\n");
                printf("\t+========================================================+\n\n");
                printf("\n\t\tSelect your choice:\n\n");
                printf("\t\t* 1: Update Student Tuition\n");
                printf("\t\t* 2: View Tuition Status\n");
                printf("\t\t* 3: Return\n");
                printf("\t+------------------------------------------------------+\n");
                printf("\t  Enter your choice (1-3): ");
                
                while (scanf("%d", &choice_action) != 1) {
                    while (getchar() != '\n');
                    printf("\t  (!_!) Invalid input! Please enter a number (1-3): ");
                    _getch();
                    eraseLines(2);
                }

                switch(choice_action) {
                    case 1: student_tuition_update(); break;
                    case 2: student_tuition_view(); break;
                    case 3: break;
                    default:
                        printf("\t  Invalid choice! Please select 1-3 only (>_<)!\n");
                        getch();
                }
                break;

            case 4: // Return
                return;

            default:
                printf("\t  Invalid choice! Please select 1-4 only (>_<)!\n");
                getch();
                break;
        }
    } while (true);
}

void sub_course_admin() {
    int choice_category, choice_action;

    do {
        system("cls");
        printf("\n");
        printf("\t+========================================================+\n");
        printf("\t|              Study Center Management System            |\n");
        printf("\t|========================================================|\n");
        printf("\t|                    Course Management                   |\n");
        printf("\t+========================================================+\n\n");
        printf("\n\t\tSelect a category:\n\n");
        printf("\t\t* 1: Course Records\n");
        printf("\t\t* 2: Financial Management\n");
        printf("\t\t* 3: Return\n");
        printf("\t+------------------------------------------------------+\n");
        printf("\t  Enter your choice (1-3): ");

        while (scanf("%d", &choice_category) != 1) {
            while (getchar() != '\n');
            printf("\t  (!_!) Invalid input! Please enter a number (1-3): ");
            _getch();
            eraseLines(2);
        }

        switch(choice_category) {
            case 1: // Course Records
                system("cls");
                printf("\n");
                printf("\t+========================================================+\n");
                printf("\t|              Study Center Management System            |\n");
                printf("\t|========================================================|\n");
                printf("\t|                    Course Records                      |\n");
                printf("\t+========================================================+\n\n");
                printf("\n\t\tSelect your choice:\n\n");
                printf("\t\t* 1: Add New Course\n");
                printf("\t\t* 2: Update Course Information\n");
                printf("\t\t* 3: Delete Course\n");
                printf("\t\t* 4: View All Courses\n");
                printf("\t\t* 5: Return\n");
                printf("\t+------------------------------------------------------+\n");
                printf("\t  Enter your choice (1-5): ");
                
                while (scanf("%d", &choice_action) != 1) {
                    while (getchar() != '\n');
                    printf("\t  (!_!) Invalid input! Please enter a number (1-5): ");
                    _getch();
                    eraseLines(2);
                }

                switch(choice_action) {
                    case 1: course_add(); break;
                    case 2: course_update_info(); break;
                    case 3: course_delete(); break;
                    case 4: course_list(); break;
                    case 5: break;
                    default: 
                        printf("\t  Invalid choice! Please select 1-5 only (>_<)!\n");
                        getch();
                }
                break;

            case 2: // Financial Management
                system("cls");
                printf("\n");
                printf("\t+========================================================+\n");
                printf("\t|              Study Center Management System            |\n");
                printf("\t|========================================================|\n");
                printf("\t|                Financial Management                    |\n");
                printf("\t+========================================================+\n\n");
                printf("\n\t\tSelect your choice:\n\n");
                printf("\t\t* 1: Update Course Fee\n");
                printf("\t\t* 2: View Course Tuition Status\n");
                printf("\t\t* 3: Return\n");
                printf("\t+------------------------------------------------------+\n");
                printf("\t  Enter your choice (1-3): ");
                
                while (scanf("%d", &choice_action) != 1) {
                    while (getchar() != '\n');
                    printf("\t  (!_!) Invalid input! Please enter a number (1-3): ");
                    _getch();
                    eraseLines(2);
                }

                switch(choice_action) {
                    case 1: course_tuition_update(); break;
                    case 2: course_tuition_view(); break;
                    case 3: break;
                    default:
                        printf("\t  Invalid choice! Please select 1-3 only (>_<)!\n");
                        getch();
                }
                break;

            case 3: // Return
                return;

            default:
                printf("\t  Invalid choice! Please select 1-3 only (>_<)!\n");
                getch();
                break;
        }
    } while (true);
}

void sub_class_admin() {
    int choice_category, choice_action;

    do {
        system("cls");
        printf("\n");
        printf("\t+========================================================+\n");
        printf("\t|              Study Center Management System            |\n");
        printf("\t|========================================================|\n");
        printf("\t|                    Class Management                    |\n");
        printf("\t+========================================================+\n\n");
        printf("\n\t\tSelect a category:\n\n");
        printf("\t\t* 1: Class Records\n");
        printf("\t\t* 2: Student Registration\n");
        printf("\t\t* 3: Return\n");
        printf("\t+------------------------------------------------------+\n");
        printf("\t  Enter your choice (1-3): ");

        while (scanf("%d", &choice_category) != 1) {
            while (getchar() != '\n');
            printf("\t  (!_!) Invalid input! Please enter a number (1-3): ");
            _getch();
            eraseLines(2);
        }

        switch(choice_category) {
            case 1: // Class Records
                system("cls");
                printf("\n");
                printf("\t+========================================================+\n");
                printf("\t|              Study Center Management System            |\n");
                printf("\t|========================================================|\n");
                printf("\t|                    Class Records                       |\n");
                printf("\t+========================================================+\n\n");
                printf("\n\t\tSelect your choice:\n\n");
                printf("\t\t* 1: Add New Class\n");
                printf("\t\t* 2: Update Class Information\n");
                printf("\t\t* 3: Delete Class\n");
                printf("\t\t* 4: View All Classes\n");
                printf("\t\t* 5: Return\n");
                printf("\t+------------------------------------------------------+\n");
                printf("\t  Enter your choice (1-5): ");
                
                while (scanf("%d", &choice_action) != 1) {
                    while (getchar() != '\n');
                    printf("\t  (!_!) Invalid input! Please enter a number (1-5): ");
                    _getch();
                    eraseLines(2);
                }

                switch(choice_action) {
                    case 1: class_add(); break;
                    case 2: class_update_info(); break;
                    case 3: class_delete(); break;
                    case 4: class_list(); break;
                    case 5: break;
                    default: 
                        printf("\t  Invalid choice! Please select 1-5 only (>_<)!\n");
                        getch();
                }
                break;

            case 2: // Student Registration
                system("cls");
                printf("\n");
                printf("\t+========================================================+\n");
                printf("\t|              Study Center Management System            |\n");
                printf("\t|========================================================|\n");
                printf("\t|                Student Registration                    |\n");
                printf("\t+========================================================+\n\n");
                printf("\n\t\tSelect your choice:\n\n");
                printf("\t\t* 1: Register Student to Class\n");
                printf("\t\t* 2: Remove Student from Class\n");
                printf("\t\t* 3: Return\n");
                printf("\t+------------------------------------------------------+\n");
                printf("\t  Enter your choice (1-3): ");
                
                while (scanf("%d", &choice_action) != 1) {
                    while (getchar() != '\n');
                    printf("\t  (!_!) Invalid input! Please enter a number (1-3): ");
                    _getch();
                    eraseLines(2);
                }

                switch(choice_action) {
                    case 1: class_register(); break;
                    case 2: class_unregister(); break;
                    case 3: break;
                    default:
                        printf("\t  Invalid choice! Please select 1-3 only (>_<)!\n");
                        getch();
                }
                break;

            case 3: // Return
                return;

            default:
                printf("\t  Invalid choice! Please select 1-3 only (>_<)!\n");
                getch();
                break;
        }
    } while (true);
}

void sub_head_admin() {
    int choice_head_admin;

    do {
        system("cls");
        printf("\n");
        printf("\t+========================================================+\n");
        printf("\t|              Study Center Management System            |\n");
        printf("\t|========================================================|\n");
        printf("\t|                       Head Admin                       |\n");
        printf("\t+========================================================+\n\n");
        printf("\n\t\tSelection of All Command Relating:\n\n");
        printf("\t\t* 1: Student\n"
               "\t\t* 2: Course\n"
               "\t\t* 3: Class\n"
               "\t\t* 4: Return to Main Menu\n");
        printf("\t+--------------------------------------------------------+\n");
        printf("\t  Enter your choice (1-4): ");

        while (scanf("%d", &choice_head_admin) != 1) {
            while (getchar() != '\n');
            printf("\t  (!_!) Invalid input! Please enter a number (1-4): ");
            _getch();
            eraseLines(2);
        }
    
        switch(choice_head_admin) {
            case 1:
                sub_student_admin();
                break;
            case 2:
                sub_course_admin();
                break;
            case 3:
                sub_class_admin();
                break;
            case 4:
                break;
            default:
                printf("From 1 to 4 only (>_<)!\n");
                getch();
                break;
        }
    } while (choice_head_admin != 4);

    printf("\nReturning to Main Menu.\n"
    "Press any key to return ( ='.'=) \n");
    getch();
    return;
}

//student managment staff menu
void sub_staff() {
    int choice_staff;

    do {
        system("cls");
        printf("\n");
        printf("\t+========================================================+\n");
        printf("\t|              Study Center Management System            |\n");
        printf("\t|========================================================|\n");
        printf("\t|                        Staff Menu                      |\n");
        printf("\t+========================================================+\n\n");
        printf("\n\t\tSelect your choice:\n\n");
        printf("\t\t* 1: Student Management\n"
               "\t\t* 2: Class Management\n"
               "\t\t* 3: Information List\n"
               "\t\t* 4: Return to Main Menu\n");
        printf("\t+--------------------------------------------------------+\n");
        printf("\t  Enter your choice (1-4): ");

        while (scanf("%d", &choice_staff) != 1) {
            while (getchar() != '\n');
            printf("\t  (!_!) Invalid input! Please enter a number (1-4): ");
            _getch();
            eraseLines(2);
        }
    
        switch(choice_staff) {
            case 1:
                sub_student();
                break;
            case 2:
                sub_class();
                break;
            case 3:
                sub_view();
                break;
            case 4:
                break;  
            default:
                printf("From 1 to 4 only (>_<)!\n");
                getch();
                break;
        }
    } while (choice_staff != 4);

    printf("\nReturning to Main Menu.\n"
    "Press any key to return ( ='.'=) \n");
    getch();
    return;
}

//accountant menu
void sub_accountant() {
    int choice_accountant;
    int sub_choice_accountant;
    do
    {
        system("cls");
        printf("\n");
        printf("\t+========================================================+\n");
        printf("\t|              Study Center Management System            |\n");
        printf("\t|========================================================|\n");
        printf("\t|                       Accountant                       |\n");
        printf("\t+========================================================+\n\n");
        printf("\n\t\tSelect your choice:\n\n");
        printf("\t\t* 1: Check Student Payment\n"
               "\t\t* 2: Check Class Payment\n"
               "\t\t* 3: Return to Main Menu\n");
        printf("\t+--------------------------------------------------------+\n");
        printf("\t  Enter your choice (1-3): ");

        while (scanf("%d", &choice_accountant) != 1) {
            while (getchar() != '\n');
            printf("\t  (!_!) Invalid input (>_<)! Please enter a number (1-3): ");
            _getch();
            eraseLines(2);
        }

         switch(choice_accountant) {
            case 1:
                system("cls");
                printf("\n");
                printf("\t+========================================================+\n");
                printf("\t|              Study Center Management System            |\n");
                printf("\t|========================================================|\n");
                printf("\t|                       Accountant                       |\n");
                printf("\t+========================================================+\n\n");
                printf("\n\t\tSelect your choice:\n\n");
                printf("\t\t* 1: Update a Student's Payment\n");
                printf("\t\t* 2: Calculate a Student's Payment Progress\n");
                printf("\t\t* 3: View all Students\n");
                printf("\t\t* 4: Return\n");
                printf("\t+--------------------------------------------------------+\n");
                printf("\t  Enter your choice (1-4): ");

                 while (scanf("%d", &sub_choice_accountant) != 1) {
                    while (getchar() != '\n');
                    printf("\t  (!_!) Invalid input (>_<)! Please enter a number (1-2): ");
                    _getch();
                    eraseLines(2);
                }

                switch(sub_choice_accountant) {
                    case 1:
                        student_tuition_update();
                        break;
                    case 2:
                        student_tuition_view();
                        break;
                    case 3:
                        student_list();
                        break;
                    case 4:
                        break;
                    default:
                        printf("\t  Invalid input (>_<)! Please select options 1-2 only!\n");
                        getch();
                        break;
                }
                break;

            case 2:
                system("cls");
                printf("\n");
                printf("\t+========================================================+\n");
                printf("\t|              Study Center Management System            |\n");
                printf("\t|========================================================|\n");
                printf("\t|                       Accountant                       |\n");
                printf("\t+========================================================+\n\n");
                printf("\n\t\tSelect your choice:\n\n");
                printf("\t\t* 1: Change a Course's Fee\n");
                printf("\t\t* 2: Calculate a Course's Payment Progress\n");
                printf("\t\t* 3: View all Courses\n");
                printf("\t\t* 4: Return\n");
                printf("\t+--------------------------------------------------------+\n");
                printf("\t  Enter your choice (1-4): ");

                 while (scanf("%d", &sub_choice_accountant) != 1) {
                    while (getchar() != '\n');
                    printf("\t  (!_!) Invalid input (>_<)! Please enter a number (1-4): ");
                    _getch();
                    eraseLines(2);
                }

                switch(sub_choice_accountant) {
                    case 1:
                        course_tuition_update();
                        break;
                    case 2:
                        course_tuition_view();
                        break;
                    case 3:
                        course_list();
                    case 4:
                        break;
                    default:
                        printf("\t  Invalid input (>_<)! Please select options 1-4 only!\n");
                        getch();
                        break;
                }
                break;

            case 3:
                break;
            default:
                printf("\t  Invalid input (>_<)! Please select options 1-3 only!\n");
                getch();
                break;
        }
    } while (choice_accountant != 3);
    
    printf("\nReturning to Main Menu.\n"
    "Press any key to return ( ='.'=) \n");
    getch();
    return;
}


//main menu
main() {
    int choice_main;

    do {
        system("cls");
        printf("\n");
        printf("\t+========================================================+\n");
        printf("\t|              Study Center Management System            |\n");
        printf("\t|========================================================|\n");
        printf("\t|                        Main Menu                       |\n");
        printf("\t+========================================================+\n\n");
        printf("\t\tSelect your role:\n\n");
        printf("\t\t* 1: Head Admin\n");
        printf("\t\t* 2: Accountant\n");
        printf("\t\t* 3: Management Staff\n");
        printf("\t\t* 4: Exit System\n");
        printf("\t+-------------------------------------------------------+\n");
        printf("\t  Enter your choice (1-4): ");

        while (scanf("%d", &choice_main) != 1) {
            while (getchar() != '\n');
            printf("\t  (!_!) Invalid input (>_<)! Please enter a number (1-4): ");
            _getch();
            eraseLines(2);
        }

        switch(choice_main) {
            case 1:
                sub_head_admin();
                break;
            case 2:
                sub_accountant();
                break;
            case 3:
                sub_staff();
                break;
            case 4:
                system("cls");
                printf("\nExiting System... (-_-)zzz\n");
                Sleep(1000); // Wait for 1 second
                exit(0);
            default:
                printf("\t  Invalid input (>_<)! Please select options 1-4 only!\n");
                getch();
                break;
        }
    } while (choice_main != 4);
}
