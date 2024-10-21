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
        printf("\r");   // Move cursor to start of line
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
    int id_no;
    char name[30];
    float math, physics, chemistry, average;
    struct student *next;
}node;

node *create_list();
void display(node *);
void count(node *);
void search(node *, int);
node *addatbeginning(node *);
node *addatend(node *);
node *addafter(node *,int);
node *addbefore(node *,int);
node *addatpos(node *,int);
node *del(node *,int);
node *reverse(node *);

int main() {
    int choice, choice2, data, item, pos;
    char cf = 'y';
    node *start;
    start = NULL;
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
                printf("Are you sure? Creating a new List will rewrite the existing one (If there is one). \nTo continue, press y. ");
                cf=getch();
                printf("\n");
                if (cf == 'y') {
                    start = create_list();
                }
                else {
                    printf("Thanks for confirmation. Press any key to return.");
                }
                _getch();
                system("cls");
                break;
            case 2:
                display(start);
                _getch();
                system("cls");
                break;
            case 3:
                count(start);
                _getch();
                system("cls");
                break;
            case 4:
                if (start == NULL) {
                printf("No list to search from. Press any key to return.");
                }
                else {
                    printf("Enter ID to be searched: ");
                    scanf("%d", &data);
                    search(start, data);
                }
                    _getch();
                    system("cls");
                break;
            case 5:
                system("cls");
                do {
                    printf("\n\t\tAdding new Student's Information. \n\n");
                    printf("\t1: To the start of the list.\n");
                    printf("\t2: To the end of the list.\n");
                    printf("\t3: Before a specific ID: \n");
                    printf("\t4: After a specific ID: \n");
                    printf("\t5: To a speicific position. \n");
                    printf("\t6: Return to Menu.\n\n");

                    printf("Enter your choice: ");
                    scanf("%d", &choice2);

                    switch(choice2) {
                        case 1:
                            start = addatbeginning(start);
                            _getch();
                            system("cls");
                            break;
                        case 2:
                            start = addatend(start);
                            _getch();
                            system("cls");
                            break;
                        case 3:
                            printf("Enter ID to insert before it: ");
                            scanf("%d", &item);
                            start = addbefore(start, item);
                            _getch();
                            system("cls");
                            break;
                        case 4:
                            printf("Enter ID to insert after it: ");
                            scanf("%d", &item);
                            start = addafter(start, item);
                            _getch();
                            system("cls");
                            break;
                        case 5:
                            printf("Enter position number to insert into: ");
                            scanf("%d", &pos);
                            start = addatpos(start, pos);
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
                if (start == NULL) {
                    printf("No list to delete from. Press any key to return.");
                }
                else {
                    printf("Enter the ID to be deleted: ");
                    scanf("%d", &data);
                    start = del(start, data);
                }
                _getch();
                system("cls");
                break;
            case 7:
                if (start == NULL) {
                    printf("No list to reverse. Press any key to return.");
                }
                else {
                    start = reverse(start);
                }
                _getch();
                system("cls");
                break;
            case 8:
                exit(0);
            default:
                printf("Input only between 1 to 8!Press any key to return.");
                _getch();
        }
    } while (choice != 8); 
}

node *create_list() {
    int id, flag = 1;
    float math, physics, chemistry, average;
    char Name[30];
    char ans='y';
    node *temp, *newer, *start;
    node *getnode();
    temp = NULL;
    do {
        printf("Enter the ID: ");
        scanf("%d", &id);
        printf("Enter student's name: ");
        fflush(stdin);
        gets(Name);
        printf("Enter Math's score: ");
        scanf("%f", &math);
        printf("Enter Physics's score: ");
        scanf("%f", &physics);
        printf("Enter Chemistry's score: ");
        scanf("%f", &chemistry);
        newer = getnode();
        if (newer == NULL) {
            printf("\nMemory is not allocated");
        }
        newer -> id_no = id;
        strcpy(newer -> name, Name);
        newer -> math = math;
        newer -> physics = physics;
        newer -> chemistry = chemistry;
        newer -> average = (math + physics + chemistry) / 3.0f;
        if (flag == 1) {
            start = newer;
            temp = start;
            flag = 0;
        }
        else {
            temp -> next = newer;
            temp = newer;
        }
        printf("To enter another ID, press y.\nPress any key other than y to return. ");
        ans=getch();
        printf("\n\n");
    } while (ans == 'y');
    printf("\nThe Single Linked List is created. Press any key to return.");
    return start;
}

node *getnode() {
    node *temp;
    temp = (node*)malloc(sizeof(node));
    temp -> next = NULL;
    return temp;
}

void display(node *start) {
    node *p;
    p = start;
    if (p == NULL) {
        printf("List is empty. Press any key to return.");
        return;
    }
    else {
        system("cls");
        gotoxy(2, 1);  printf("ID");
        gotoxy(15, 1); printf("Full Name");
        gotoxy(55, 1); printf("Math");
        gotoxy(70, 1); printf("Physics");
        gotoxy(85, 1); printf("Chemistry");
        gotoxy(100, 1); printf("Average");
        
        int i = 2;
        while (p != NULL) {
            gotoxy(2, i);  printf("%d", p->id_no);
            gotoxy(15, i); printf("%s", p->name);
            gotoxy(55, i); printf("%0.2f", p->math);
            gotoxy(70, i); printf("%0.2f", p->physics);
            gotoxy(85, i); printf("%0.2f", p->chemistry);
            gotoxy(100, i); printf("%0.2f", p->average);
            p = p -> next;
            i++;
        }
        printf("\nPress any key to return.");
    }
}

void count(node *start) {
    node *p;
    int i = 0;
    p = start;
    while (p != NULL) {
        p = p -> next;
        i++;
    }
    printf("there's %d ID.", i);
}

void search(node *start, int data) {
    node *p;
    p = start;
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

node *addatbeginning(node *start) {
    node *temp, *newer;
    newer = getnode();
    printf("Enter ID: ");
    scanf("%d", &newer -> id_no);
    printf("Enter the name: ");
    fflush(stdin);
    gets(newer -> name);
    printf("Enter Math's Score: ");
    scanf("%f", &newer -> math);
    printf("Enter Physics's Score: ");
    scanf("%f", &newer -> physics);
    printf("Enter Chemistry's Score: ");
    scanf("%f", &newer -> chemistry);
    newer -> average = (newer -> math + newer -> physics + newer -> chemistry) / 3.0f;
    if (start == NULL) {
        start = newer;
    }
    else {
        temp = start;
        newer -> next = temp;
        start = newer;
    }
    printf("Added successfully. Press any key to return.");
    return start;
}

node *addatend(node *start) {
    node *p, *temp;
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
    if (start == NULL) {
        start = p;
        printf("Added Successfully. Press any key to return.");
        return start;
    }
    else {
        temp = start;
        while (temp -> next != NULL) {
            temp = temp -> next;
        }
        temp -> next = p;
        p -> next = NULL;
        printf("Added Successfully. Press any key to return.");
        return start;
    }
}

node *addbefore(node *start, int item) {
    node *temp, *p;
    if (start == NULL) {
        printf ("List is empty\n");
        _getch();
        return start;
    }
    
    // If data to be inserted before first node*

    if (start -> id_no == item) {
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
        temp -> next = start;
        start = temp;
        printf("Added Successfully. Press any key to return.");
        return start;
    }
    
    p = start;
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
            return start;
        }
        p = p -> next;
    }
    printf("%d not present in the list\n", item);
    return start;
}

node *addafter(node *start, int item) {
    node *temp, *p;
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

    if (start == NULL) {
        start = p;
    }
    else {
        temp = start;
        do {
            if (temp -> id_no == item) {
                p -> next = temp -> next;
                temp -> next = p;
                printf("Added Successfully. Press any key to return.");
                return start;
            }
            else temp = temp -> next;
        } while (temp != NULL);
    }
}

node *addatpos (node *start, int pos) {
    node *temp, *p;
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
        temp -> next = start;
        start = temp;
        return start;
    }
    p = start;
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
    return start;
}

node *del (node *start, int data) {
    node *temp, *p;

    // Delete the first node
    if (start -> id_no == data) {
        temp = start;
        start = start -> next;
        free(temp);
        printf("ID %d has been deleted. Press any key to return.", data);
        return start;
    }

    //Delete after first node
    p = start;
    while (p -> next != NULL) {
        if (p -> next -> id_no == data) {
            temp = p -> next;
            p -> next = temp -> next;
            free(temp);
            printf("ID %d has been deleted. Press any key to return.", data);
            return start;
        }
        p = p -> next;
    }
    printf("ID %d not found. Press any key to return.", data);
    return start;
}

node *reverse (node *start) {
    node *prev, *ptr, *n;
    prev = NULL;
    ptr = start;
    while (ptr != NULL) {
        n = ptr -> next;
        ptr -> next = prev;
        prev = ptr;
        ptr = n;
    }
    start = prev;
    printf("List's order has been reversed. Press any key to return.");
    return start;
}