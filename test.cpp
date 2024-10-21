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
    char id[15];
    char name[30];
} student;

typedef struct {
    char class_name[30];
    int tuition;
} subject;

void Insert_Students_List() {
    int n, r; // n for new list counter, r for old counter
    int pos = 0; // start of new list counter
    long lsize; // file size check
    char id[15];
    char name[30];
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

void Insert_Subjects_list() { //The same as Students List, just different data
    int n, r;
    int pos = 0;
    long lsize;
    char class_name[30];
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

main() {
    int c;
    do {
        printf("\n\t\tStudent Data Management System\n\n"
               "\t1: Insert a new List of Students\n"
               "\t2: Insert a new List of Subjects\n"
               "\t3: Print out List of Students\n"
               "\t4: Print out List of Subjects\n"
               "\t5: Check Student\n"
               "\t6: Exit System"
               "\nEnter your choice (1 to 3)\n");

        while (printf("\nChoice: ") && scanf("%d", &c) != 1) {
            while (getchar() != '\n');
            printf("Please enter again!");
            _getch();
            eraseLines(3);
        }

        switch(c) {
            case 1:
                Insert_Students_List();
                break;
            case 2:
                Insert_Subjects_list();
                break;
            case 3:
                Print_Students_List();
                break;
            case 4:
                Print_Subjects_List();
                break;
            case 5:
                Register_Subject();
                break;
            case 6:
                exit(0);
            default:
                printf("From 1 to 5 only!");
                getch();
                system("cls");
                break;
        }
    } while (c != 6);
}