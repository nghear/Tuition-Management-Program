#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iomanip>
#include <limits>
#include <cstdlib>

void eraseLines(int count) {
    if (count > 0) {
        printf("\x1b[2K");  // Clear current line
        for (int i = 1; i < count; i++) {
            printf("\x1b[1A");
            printf("\x1b[2K");
        }
        printf("\r");   // Move cursor to head of line
    }
}

void gotoxy(int x, int y) { // Move cursor to coordinate (x,y)
    static HANDLE h = nullptr;  
    if(!h)
        h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD c = { static_cast<SHORT>(x), static_cast<SHORT>(y) };  
    SetConsoleCursorPosition(h,c);
}

typedef struct student {
    char id_no[30];
    char name[30];
    char subject[30];
    int tuition;
    struct student *next;
} node_st;

node_st *create_list();
void display_memory(node_st *);
void display_file(node_st *);
void count(node_st *);
void search(node_st *, int);
/*node_st *addatbeginning(node_st *);
node_st *addatend(node_st *);
node_st *addafter(node_st *,int);
node_st *addbefore(node_st *,int);
node_st *addatpos(node_st *,int);
node_st *del(node_st *,int);
node_st *reverse(node_st *);*/

node_st* head = nullptr;

bool check(const char* x) {
    //Check in memory
    for(node_st* t = head; t != nullptr; t = t -> next) {
        if (t -> id_no == x) return true;
    }

    //Check in file
    FILE* file = fopen("students.txt", "r");
    if (file != NULL) {
        char line[256];
        while (fgets(line, sizeof(line), file)) {
            char id_no[30];
            if (sscanf(line, "%s", &id_no) == 1 && strcmp(x, id_no) == 0) {
                fclose(file);
                return true;
            }
        }
        fclose(file);
    }
    return false;
}

int main() {
    int choice, choice2, data, item, pos;
    char cf = 'y';
    node_st *head;
    head = NULL;
    do {
        printf("\n\t\tStudent's Data Management System\n\n");
        printf("\t1: Create List\n");
        printf("\t2: Display\n");
        printf("\t3: Count\n");
        printf("\t4: Search\n");
        printf("\t5: Add to List\n");
        printf("\t6: Delete\n");
        printf("\t7: Reverse\n");
        printf("\t8: Quit\n\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                head = create_list();
                _getch();
                system("cls");
                break;
            case 2:
                display_memory(head);
                break;
            case 3:
                count(head);
                _getch();
                system("cls");
                break;
            case 4:
                display_file(head);
                break;
            case 5:
                exit(0);
            /*case 4:
                if (head == NULL) {
                printf("No list to search from. Press any key to return.");
                }
                else {
                    printf("Enter ID to be searched: ");
                    scanf("%d", &data);
                    search(head, data);
                }
                    _getch();
                    system("cls");
                break;
            case 5:
                system("cls");
                do {
                    printf("\n\t\tAdding new Student's Information. \n\n");
                    printf("\t1: To the head of the list.\n");
                    printf("\t2: To the end of the list.\n");
                    printf("\t3: Before a specific ID: \n");
                    printf("\t4: After a specific ID: \n");
                    printf("\t5: To a speicific position. \n");
                    printf("\t6: Return to Menu.\n\n");

                    printf("Enter your choice: ");
                    scanf("%d", &choice2);

                    switch(choice2) {
                        case 1:
                            head = addatbeginning(head);
                            _getch();
                            system("cls");
                            break;
                        case 2:
                            head = addatend(head);
                            _getch();
                            system("cls");
                            break;
                        case 3:
                            printf("Enter ID to insert before it: ");
                            scanf("%d", &item);
                            head = addbefore(head, item);
                            _getch();
                            system("cls");
                            break;
                        case 4:
                            printf("Enter ID to insert after it: ");
                            scanf("%d", &item);
                            head = addafter(head, item);
                            _getch();
                            system("cls");
                            break;
                        case 5:
                            printf("Enter position number to insert into: ");
                            scanf("%d", &pos);
                            head = addatpos(head, pos);
                            _getch();
                            system("cls");
                            break;
                        case 6:
                            break;
                        default: 
                            printf("Input only between 1 to 6! Press any key to return.");
                            getch();
                            break;
                    }
                } while (choice2 != 6);
    printf("Press any key to return to Menu.");
    _getch();
    system("cls");
                break;
            case 6:
                if (head == NULL) {
                    printf("No list to delete from. Press any key to return.");
                }
                else {
                    printf("Enter the ID to be deleted: ");
                    scanf("%d", &data);
                    head = del(head, data);
                }
                _getch();
                system("cls");
                break;
            case 7:
                if (head == NULL) {
                    printf("No list to reverse. Press any key to return.");
                }
                else {
                    head = reverse(head);
                }
                _getch();
                system("cls");
                break;
            case 8:
                exit(0);
            default:
                printf("Input only between 1 to 8!Press any key to return.");
                _getch(); */
        }
    } while (choice != 5); 
}

node_st *create_list() {
    node_st st;
    char id[30];
    char Name[30];
    node_st *temp, *p, *head;
    node_st *getnode();
    temp = NULL;
    
    p = getnode();
    if (p == NULL) {
        printf("\nMemory is not allocated");
    }

    //Input ID + Check ID for match
    printf("Enter new ID: ");
    fflush(stdin);
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = 0;
    strcpy(p -> id_no, id);
    if (check(id)) {
        printf("A student with this ID already exists.\n");
        printf("Press any key to return to the Menu.");
        _getch();
        system("cls");
        return head;
    }

    //Input name
    printf("Enter Student's Full Name: ");
    fflush(stdin);
    fgets(Name, sizeof(Name), stdin);
    Name[strcspn(Name,"\n")] = 0;
    strcpy(p -> name, Name);

    //Insert into Head
    if (head == NULL) {
        head = p;
    }
    else {
        temp = head;
        p -> next = temp;
        head = p;
    }

    FILE *file = fopen("students.txt", "a");
    if (file != NULL) {
        fprintf(file, "%s,%s\n", p -> id_no, p -> name);
        fclose(file);
        printf("Data added successfully to memory and file.\n");
    }
    else {
        printf("Data added to memory, however write to file has failed.\n");
    }

    printf("Press any key to return.");
    _getch();
    system("cls");
    return head;
}

node_st *getnode() {
    node_st *temp;
    temp = (node_st*)malloc(sizeof(node_st));
    temp -> next = NULL;
    return temp;
}

void display_memory(node_st *head) {
    node_st *p;
    p = head;
    if (!head) {
        printf("List is empty. Press any key to return.");
        _getch();
        system("cls");
    }
    else {
        system("cls");
        gotoxy(2, 1);  printf("ID");
        gotoxy(32, 1); printf("Full Name");
        
        int i = 2;
        while (p != NULL) {
            gotoxy(2, i);  printf("%s", p->id_no);
            gotoxy(32, i); printf("%s", p->name);
            p = p -> next;
            i++;
        }
    }
    printf("\nPress any key to return.");
    _getch();
    system("cls");
}

void display_file(node_st *head) {
    system("cls");
    node_st st;
    FILE* file = fopen("students.txt", "r");
    if (file == NULL) {
        printf("No data available in file.\n");
    }
    else {
        int row = 1;
        char line[256];
        bool header_printed = false;

        while (fgets(line, sizeof(line), file)) {
            char id_no[30];
            char name[30];
            if (sscanf(line, "%s,%s", &id_no, &name) == 2) {
                if (!header_printed) {
                    gotoxy(2, row);  printf("ID");
                    gotoxy(32, row); printf("Full Name");
                    row++;
                    header_printed = true;
                }
                gotoxy(2, row);  printf("%s", id_no);
                gotoxy(32, row); printf("%s", name);
                row++;
            }
        }
        fclose(file);

        if (!header_printed) {
            printf("No data available in file\n");
        }
    }
    printf("\nPress any key to return to Menu.");
    _getch();
    system("cls");
}

void count(node_st *head) {
    node_st *p;
    int count = 0;
    int i, n;
    node_st st[256];
    FILE *file = fopen("students.txt","r");
    if (file == NULL) {
        printf("No data in file.\n");
    }
    else {
        char line[256];
        fread(&n, sizeof(int), 1, file);
        for (int i = 0; i < n; i++) {
            fread(&st[i], sizeof(node_st), 1, file);
        }
        fclose(file);
        for (i = 0; i < n; i++) {
            count++;
        }

    }
    /*p = head;
    while (p != NULL) {
        p = p -> next;
        i++;
    } */
    printf("there's %d ID.", count);
}

/*void search(node_st *head, int data) {
    node_st *p;
    p = head;
    system("cls");
        while (p != NULL) {
            if (p -> id_no == data) {
                gotoxy(2, 1);  printf("ID");
                gotoxy(15, 1); printf("Full Name");
                gotoxy(55, 1); printf("Math");
                gotoxy(70, 1); printf("Physics");
                gotoxy(85, 1); printf("Chemistry");
                gotoxy(100, 1); printf("Average");
                gotoxy(2, 2);  printf("%d", p -> id_no);
                gotoxy(15, 2); printf("%s", p -> name);
                gotoxy(55, 2); printf("%0.2f", p -> math);
                gotoxy(70, 2); printf("%0.2f", p -> physics);
                gotoxy(85, 2); printf("%0.2f", p -> chemistry);
                gotoxy(100, 2); printf("%0.2f", p-> average);
                return;
            }
            p = p -> next;
        }
        printf("No data found.\n");
    printf("\nPress any key to return to the menu.");
}

node_st *addatbeginning(node_st *head) {
    node_st *temp, *p;
    p = getnode();
    printf("Enter ID: ");
    scanf("%d", &p -> id_no);
    printf("Enter the name: ");
    fflush(stdin);
    gets(p -> name);
    printf("Enter Math's Score: ");
    scanf("%f", &p -> math);
    printf("Enter Physics's Score: ");
    scanf("%f", &p -> physics);
    printf("Enter Chemistry's Score: ");
    scanf("%f", &p -> chemistry);
    p -> average = (p -> math + p -> physics + p -> chemistry) / 3.0f;
    if (head == NULL) {
        head = p;
    }
    else {
        temp = head;
        p -> next = temp;
        head = p;
    }
    printf("Added successfully. Press any key to return.");
    return head;
}

node_st *addatend(node_st *head) {
    node_st *p, *temp;
    p = getnode();
    printf("Enter ID: ");
    scanf("%d", &p -> id_no);
    printf("Enter the name: ");
    fflush(stdin);
    gets(p -> name);
    printf("Enter Math's Score: ");
    scanf("%f", &p -> math);
    printf("Enter Physics's Score: ");
    scanf("%f", &p -> physics);
    printf("Enter Chemistry's Score: ");
    scanf("%f", &p -> chemistry);
    p -> average = (p -> math + p -> physics + p -> chemistry) / 3.0f;
    if (head == NULL) {
        head = p;
        printf("Added Successfully. Press any key to return.");
        return head;
    }
    else {
        temp = head;
        while (temp -> next != NULL) {
            temp = temp -> next;
        }
        temp -> next = p;
        p -> next = NULL;
        printf("Added Successfully. Press any key to return.");
        return head;
    }
}

node_st *addbefore(node_st *head, int item) {
    node_st *temp, *p;
    if (head == NULL) {
        printf ("List is empty\n");
        _getch();
        return head;
    }
    
    // If data to be inserted before first node*

    if (head -> id_no == item) {
        temp = getnode();
        printf("Enter ID: ");
        scanf("%d", &temp -> id_no);
        printf("Enter the name: ");
        fflush(stdin);
        gets(temp -> name);
        printf("Enter Math's Score: ");
        scanf("%f", &temp -> math);
        printf("Enter Physics's Score: ");
        scanf("%f", &temp -> physics);
        printf("Enter Chemistry's Score: ");
        scanf("%f", &temp -> chemistry);
        temp -> average = (temp -> math + temp -> physics + temp -> chemistry) / 3.0f;
        temp -> next = head;
        head = temp;
        printf("Added Successfully. Press any key to return.");
        return head;
    }
    
    p = head;
    while (p -> next != NULL) {
        if (p -> next -> id_no == item) {
            temp = getnode();
            printf("Enter ID: ");
            scanf("%d", &temp -> id_no);
            printf("Enter the name: ");
            fflush(stdin);
            gets(temp -> name);
            printf("Enter Math's Score: ");
            scanf("%f", &temp -> math);
            printf("Enter Physics's Score: ");
            scanf("%f", &temp -> physics);
            printf("Enter Chemistry's Score: ");
            scanf("%f", &temp -> chemistry);
            temp -> average = (temp -> math + temp -> physics + temp -> chemistry) / 3.0f;
            temp -> next = p -> next;
            p -> next = temp;
            printf("Added Successfully. Press any key to return.");
            return head;
        }
        p = p -> next;
    }
    printf("%d not present in the list\n", item);
    return head;
}

node_st *addafter(node_st *head, int item) {
    node_st *temp, *p;
    p = getnode();
    printf("Enter ID: ");
    scanf("%d", &p -> id_no);
    printf("Enter the name: ");
    fflush(stdin);
    gets(p -> name);
    printf("Enter Math's score: ");
    scanf("%f", &p -> math);
    printf("Enter Physics's score: ");
    scanf("%f", &p -> physics);
    printf("Enter Chemistry's score: ");
    scanf("%f", &p -> chemistry);
    p -> average = (p -> math + p -> physics + p -> chemistry) / 3.0f;

    if (head == NULL) {
        head = p;
    }
    else {
        temp = head;
        do {
            if (temp -> id_no == item) {
                p -> next = temp -> next;
                temp -> next = p;
                printf("Added Successfully. Press any key to return.");
                return head;
            }
            else temp = temp -> next;
        } while (temp != NULL);
    }
}

node_st *addatpos (node_st *head, int pos) {
    node_st *temp, *p;
    int i;
    temp = getnode();
    printf("Enter ID: ");
    scanf("%d", &temp -> id_no);
    printf("Enter the name: ");
    fflush(stdin);
    gets(temp -> name);
    printf("Enter Math's Score: ");
    scanf("%f", &temp -> math);
    printf("Enter Physics's Score: ");
    scanf("%f", &temp -> physics);
    printf("Enter Chemistry's Score: ");
    scanf("%f", &temp -> chemistry);
    temp -> average = (temp -> math + temp -> physics + temp -> chemistry) / 3.0f;

    if (pos == 1) {
        temp -> next = head;
        head = temp;
        return head;
    }
    p = head;
    for (i = 1; i < pos - 1 && p != NULL; i++) {
        p = p -> next;
    }
    if (p == NULL) {
        printf("There are less than %d ID", pos);
    }
    else {
        temp -> next = p -> next;
        p -> next = temp;
    }
    printf("Added sucessfully. Press any key to return.");
    return head;
}

node_st *del (node_st *head, int data) {
    node_st *temp, *p;

    // Delete the first node
    if (head -> id_no == data) {
        temp = head;
        head = head -> next;
        free(temp);
        printf("ID %d has been deleted. Press any key to return.", data);
        return head;
    }

    //Delete after first node
    p = head;
    while (p -> next != NULL) {
        if (p -> next -> id_no == data) {
            temp = p -> next;
            p -> next = temp -> next;
            free(temp);
            printf("ID %d has been deleted. Press any key to return.", data);
            return head;
        }
        p = p -> next;
    }
    printf("ID %d not found. Press any key to return.", data);
    return head;
}

node_st *reverse (node_st *head) {
    node_st *prev, *ptr, *n;
    prev = NULL;
    ptr = head;
    while (ptr != NULL) {
        n = ptr -> next;
        ptr -> next = prev;
        prev = ptr;
        ptr = n;
    }
    head = prev;
    printf("List's order has been reversed. Press any key to return.");
    return head;
} */