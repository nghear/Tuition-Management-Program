#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <conio.h>
#include <windows.h>
#include <iomanip>
#include <limits>
#include <cstdlib>
#include <cstring> // Add this for string functions

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

class Node {
public:
    int id;
    char name[100]; // Changed from string to char array
    int math, physics, chemistry;
    float average;
    Node* next;

    Node() : id(0), math(0), physics(0), chemistry(0), average(0), next(nullptr) {
        name[0] = '\0';
    }
};

/*typedef struct student {
    int id;
    char name[100];
    int math, physics, chemistry;
    float average;
    struct student *next;
} student; */

Node* head = nullptr;

bool check(int x) {
    // Check in memory
    for (Node* t = head; t != nullptr; t = t->next) {
        if (t->id == x) return true;
    }
    
    // Check in file
    FILE* file = fopen("students.txt", "r");
    if (file != NULL) {
        char line[256];
        while (fgets(line, sizeof(line), file)) {
            int id;
            if (sscanf(line, "%d,", &id) == 1 && id == x) {
                fclose(file);
                return true;
            }
        }
        fclose(file);
    }
    return false;
}

void Insert_Record(int id, const char* name, int math, int physics, int chemistry) {
    char Name[100];
    Node* t = new Node();
    system("cls");

    // Input ID + Check ID for match
    while (printf("Enter ID: ") && scanf("%d", &id) != 1) {
        while (getchar() != '\n');
        printf("Invalid Input! Press any key to try again.");
        _getch();
        eraseLines(2);
    }
    if (check(id)) {
        printf("A student with this ID already exists.\n");
        printf("Press any key to return to the Menu.");
        _getch();
        system("cls");
        return;
    }
    t->id = id;

    //Input Name
    printf("Enter Full Name: ");
    fflush(stdin);
    fgets(Name, sizeof(Name), stdin);
    Name[strcspn(Name, "\n")] = 0;

    fflush(stdin);
    strncpy(t->name, Name, sizeof(t->name) - 1);
    t->name[sizeof(t->name) - 1] = '\0';

    //Input Math
    do {
        while(printf("Enter Math Score: ") && scanf("%d", &math) != 1) {
            while (getchar() != '\n');
            printf("Invalid Input! Press any key to try again.");
            _getch();
            eraseLines(2);
        }
        if (math < 0 || math > 10) {
            printf("Score only registers from 0 to 10! Press any key to try again.");
            _getch();
            eraseLines(2);
        }
    } while (math < 0 || math > 10);
    t->math = math;

    //Input Physics
    do {
        while(printf("Enter Physics Score: ") && scanf("%d", &physics) != 1) {
            while (getchar() != '\n');
            printf("Invalid Input! Press any key to try again.");
            _getch();
            eraseLines(2);
        }
        if (physics < 0 || physics > 10) {
            printf("Score only registers from 0 to 10! Press any key to try again.");
            _getch();
            eraseLines(2);
        }
    } while (physics < 0 || physics > 10);
    t->physics = physics;

    //Input Chemistry
    do {
        while(printf("Enter Chemistry Score: ") && scanf("%d", &chemistry) != 1) {
            while (getchar() != '\n');
            printf("Invalid Input! Press any key to try again.");
            _getch();
            eraseLines(2);
        }
        if (chemistry < 0 || chemistry > 10) {
            printf("Score only registers from 0 to 10! Press any key to try again.");
            _getch();
            eraseLines(2);
        }
    } while (chemistry < 0 || chemistry > 10);
    t->chemistry = chemistry;

    // Calculate Average
    t->average = (math + physics + chemistry) / 3.0f;

    // Insert into linked list
    if (head == nullptr || head->id >= t->id) {
        t->next = head;
        head = t;
    } else {
        Node* c = head;
        while (c->next != nullptr && c->next->id < t->id) {
            c = c->next;
        }
        t->next = c->next;
        c->next = t;
    }

    // Write to file
    /*student st;
    FILE* file_w = fopen("students_write.txt","wb");
    if (file_w == NULL) {
        printf("Khong mo duoc file!\n");
        exit(0);
    }
    t -> id = st.id;
    fgets(st.name, sizeof(name), stdin);
    t -> math = st.math;
    t -> physics = st.physics;
    t -> chemistry = st.chemistry;
    t -> average = // you're here 

    fwrite(&st, sizeof(student), 1, file_w);
    fclose(file_w); */

    FILE* file = fopen("students.txt", "a");
    if (file != NULL) {
        fprintf(file, "%d,%s,%d,%d,%d,%.2f\n", t->id, t->name, t->math, t->physics, t->chemistry, t->average);
        fclose(file);
        printf("Data added successfully to memory and file.\n");
    } else {
        printf("Data added to memory, but failed to write to file.\n");
    }

    printf("Press any key to return.\n");
    _getch();
    system("cls");
}

void Search_Record(int id) {
    if (!head) {
        printf("No data available\n");
    } else {
        for (Node* p = head; p != nullptr; p = p->next) {
            if (p->id == id) {
                gotoxy(2, 1);  printf("ID");
                gotoxy(12, 1); printf("Full Name");
                gotoxy(45, 1); printf("Math");
                gotoxy(55, 1); printf("Physics");
                gotoxy(65, 1); printf("Chemistry");
                gotoxy(75, 1); printf("Average");
                gotoxy(2, 2);  printf("%d", p->id);
                gotoxy(12, 2); printf("%s", p->name);
                gotoxy(45, 2); printf("%d", p->math);
                gotoxy(55, 2); printf("%d", p->physics);
                gotoxy(65, 2); printf("%d", p->chemistry);
                gotoxy(75, 2); printf("%.2f\n", p->average);
                _getch();
                system("cls");
                return;
            }
        }
        printf("No data found\n");
    }
    printf("\nPress any key to return to the menu.");
    _getch();
    system("cls");
}

void Delete_Record() { // Schrodinger's Function (sometime works, sometime breaks)
    system("cls");
    int choice;
    int id_s;
    char name[100];
    Node* current = head;
    Node* prev;

    if (head == nullptr) {
        printf("No data to delete. Press any key to return.");
        _getch();
        system("cls");
        return;
    }

    //added loop for sub-menu Delete_Record (11/10/2024, N).

    while (choice != 3) {
    printf("\n\t\tDelete student record by:\n");
    printf("\t1. ID\n");
    printf("\t2. Name\n");
    printf("\t3. Return to Menu\n");
    printf("\nEnter your choice (1 to 3):\n");

    // Confirmation + Delete of wrong datatype input (11/10/2024, N).

    while (printf("\nChoice: ") && scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        printf("Invalid Input! Press any key to try again");
        _getch();
        eraseLines(3);
    }

    //Replace (if else) into switch (11/10/2024, N).
    
    switch(choice) {
        case 1:
            while(printf("Enter the ID of the student to delete: ") && scanf("%d", &id_s) != 1) {
                while (getchar() != '\n');
                printf("Invalid Input! Press any key to try again");
                _getch();
                eraseLines(2);
            }

            //Delete the First Node
            if (head -> id == id_s) {
                prev = head;
                head = head -> next;
                free(prev);
                printf("ID %d has been deleted. Press any key to return.", id_s);
                _getch();
                system("cls");
                return;
            }

            //Delete after First Node
            while (current -> next != nullptr) {
                if (current -> next -> id == id_s) {
                    prev = current -> next;
                    current -> next = prev -> next;
                    free(prev);
                    printf("ID %d has been deleted. Press any key to return", id_s);
                    _getch();
                    system("cls");
                    return;
                }
                current = current -> next;
            }
            printf("ID %d not found.", id_s);
            printf("\nPress any key to return.");
            _getch();
            system("cls");
            break;



            // Old Delete by ID function (does not work anymore)

            /* while (current != nullptr && current -> id != id) {
                prev = current;
                current = current -> next;
            }
            if (current == nullptr) {
                printf("Student with ID %d is not found.\n", id);
            }
            else {
                if (prev == nullptr) {
                    head = current -> next;
                }
                else {
                    prev -> next = current -> next;
                }
                delete current;
                printf("Student ID %d deleted successfully.\n", id);
            } */
        case 2:
            printf("Enter the name of the student to delete: ");
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = 0; // Remove newline

            while (current != nullptr && strcmp(current->name, name) != 0) {
                prev = current;
                current = current->next;
            }

            if (current == nullptr) {
                printf("Student name %s is not found.\n", name);
            }
            else {
                if (prev == nullptr) {
                    head = current -> next;
                }
                else {
                    prev -> next = current -> next;
                }
                delete current;
                printf("Student record name %s deleted successfully.\n", name);
            }
            printf("Press any key to return.");
            _getch();
            system("cls");
            break;
        case 3:
            break;
        default:
            printf("Function calls only exist on 1 to 3! Press any key to try again.");
            _getch();
            system("cls");
            break;
    }
    }

    printf("Press any key to return to the menu.");
    _getch();
    system("cls");
}

void Show_Record() {
    system("cls");
    if (!head) {
        printf("No data available\n");
    } else {
        gotoxy(2, 1);  printf("ID");
        gotoxy(12, 1); printf("Full Name");
        gotoxy(45, 1); printf("Math");
        gotoxy(55, 1); printf("Physics");
        gotoxy(65, 1); printf("Chemistry");
        gotoxy(75, 1); printf("Average");

        int row = 2;
        for (Node* p = head; p != nullptr; p = p->next, ++row) {
            gotoxy(2, row);  printf("%d", p->id);
            gotoxy(12, row); printf("%s", p->name);
            gotoxy(45, row); printf("%d", p->math);
            gotoxy(55, row); printf("%d", p->physics);
            gotoxy(65, row); printf("%d", p->chemistry);
            gotoxy(75, row); printf("%.2f", p->average);
        }
    }
    printf("\nPress any key to return to the menu.");
    _getch();
    system("cls");
}

void Show_File_Records() {
    system("cls");
    FILE* file = fopen("students.txt", "r");
    if (file == NULL) {
        printf("No data available in file\n");
    } else {
        int row = 1;
        char line[256];
        bool header_printed = false;

        while (fgets(line, sizeof(line), file)) {
            int id, math, physics, chemistry;
            char name[100];
            float average;
            if (sscanf(line, "%d,%[^,],%d,%d,%d,%f", &id, name, &math, &physics, &chemistry, &average) == 6) {
                if (!header_printed) {
                    gotoxy(2, row);  printf("ID");
                    gotoxy(12, row); printf("Full Name");
                    gotoxy(45, row); printf("Math");
                    gotoxy(55, row); printf("Physics");
                    gotoxy(65, row); printf("Chemistry");
                    gotoxy(75, row); printf("Average");
                    row++;
                    header_printed = true;
                }
                gotoxy(2, row);  printf("%d", id);
                gotoxy(12, row); printf("%s", name);
                gotoxy(45, row); printf("%d", math);
                gotoxy(55, row); printf("%d", physics);
                gotoxy(65, row); printf("%d", chemistry);
                gotoxy(75, row); printf("%.2f", average);
                row++;
            }
        }
        fclose(file);

        if (!header_printed) {
            printf("No data available in file\n");
        }
    }
    printf("\nPress any key to return to the menu.");
    _getch();
    system("cls");
}

int main() {

    //removed char yn cause no use (11/10/2024, N).

    char name[100]; 
    int id, math, physics, chemistry;
    int Choice;
    
    while (true) {
        printf("\n\t\tStudent Data Management System\n\n"
               "\t1: Create New Data\n"
               "\t2: Delete Student Record\n"  // Updated menu option
               "\t3: Search Student Data\n"
               "\t4: Display Full List (Memory)\n"
               "\t5: Display Full List (File)\n"
               "\t6: Exit System\n"
               "\nEnter your choice (1 to 6)\n");

        while (printf("\nChoice: ") && scanf("%d", &Choice) != 1) {
            while (getchar() != '\n');
            printf("Please enter again!");
            _getch();
            eraseLines(3);
        }

        switch(Choice) {
            case 1:
                Insert_Record(id, name, math, physics, chemistry);
                break;
            case 2:
                Delete_Record();  // Call the new Delete_Record function
                break;
            case 3:
                system("cls");
                while (printf("Enter ID to search: ") && scanf("%d", &id) != 1) {
                    while (getchar() != '\n');
                    printf("Invalid input! Press any key to try again.");
                    _getch();
                    eraseLines(2);
                }
                Search_Record(id);
                break;
            case 4:
                Show_Record();
                break;
            case 5:
                Show_File_Records();
                break;
            case 6:
                printf("\nThank you for using the system!");
                _getch();
                exit(0);
            default:
                printf("The system function is from 1 to 6! Press any key to try again.");
                _getch();
                system("cls");
                break;
        }
    }
    return 0;
}