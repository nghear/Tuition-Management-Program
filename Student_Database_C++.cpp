#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <conio.h>
#include <windows.h>
#include <iomanip>
#include <limits>
#include <cstdlib>

using namespace std;

void eraseLines(int count) {
    if (count > 0) {
        cout << "\x1b[2K"; // Clear current line
        for (int i = 1; i < count; i++) {
            cout << "\x1b[1A" << "\x1b[2K";
        }
        cout << "\r"; // Move cursor to start of line
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
    string name;
    int math, physics, chemistry;
    float average;
    Node* next;

    Node() : id(0), math(0), physics(0), chemistry(0), average(0), next(nullptr) {}
};

Node* head = nullptr;

bool check(int x) {
    for (Node* t = head; t != nullptr; t = t->next) {
        if (t->id == x) return true;
    }
    return false;
}

void Insert_Record(int id, string name, int math, int physics, int chemistry) {
    if (check(id)) {
        cout << "A student with this ID already exists.\n";
        cout << "Press any key to return to the Menu.";
        _getch();
        system("cls");
        return;
    }

    Node* t = new Node();
    t->id = id;
    t->name = name;
    t->math = math;
    t->physics = physics;
    t->chemistry = chemistry;
    t->average = (math + physics + chemistry) / 3.0f;

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

    cout << "Data added successfully, press any key to return.\n";
    _getch();
    system("cls");
}

void Search_Record(int id) {
    if (!head) {
        cout << "No data available\n";
    } else {
        for (Node* p = head; p != nullptr; p = p->next) {
            if (p->id == id) {
                gotoxy(2, 1);  cout << "ID";
                gotoxy(12, 1); cout << "Full Name";
                gotoxy(45, 1); cout << "Math";
                gotoxy(55, 1); cout << "Physics";
                gotoxy(65, 1); cout << "Chemistry";
                gotoxy(75, 1); cout << "Average";
                gotoxy(2, 2);  cout << p->id;
                gotoxy(12, 2); cout << p->name;
                gotoxy(45, 2); cout << p->math;
                gotoxy(55, 2); cout << p->physics;
                gotoxy(65, 2); cout << p->chemistry;
                gotoxy(75, 2); cout << fixed << setprecision(2) << p->average << endl;
                _getch();
                system("cls");
                return;
            }
        }
        cout << "No data found\n";
    }
    cout << "\nPress any key to return to the menu.";
    _getch();
    system("cls");
}

void Delete_Record() {
    system("cls");
    int choice;
    int id;
    string name;
    Node* current = head;
    Node* prev = nullptr;

    //added loop for sub-menu Delete_Record (11/10/2024, N).

    while (choice != 3) {
    cout << "\n\t\tDelete student record by:\n";
    cout << "\t1. ID\n";
    cout << "\t2. Name\n";
    cout << "\t3. Return to Menu\n";
    cout << "\nEnter your choice (1 to 3):\n";

    // Confirmation + Delete of wrong datatype input (11/10/2024, N).

    while (cout << "\nChoice: " && !(cin >> choice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid Input! Press any key to try again";
        _getch();
        eraseLines(3);
    }

    //Replace (if else) into switch (11/10/2024, N).
    
    switch(choice) {
        case 1:
            while(cout << "Enter the ID of the student to delete: " && !(cin >> id)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid Input! Press any key to try again";
                _getch();
                eraseLines(2);
            }

            while (current != nullptr && current -> id != id) {
                prev = current;
                current = current -> next;
            }
            if (current == nullptr) {
                cout << "Student with ID " << id << " is not found.\n";
            }
            else {
                if (prev == nullptr) {
                    head = current -> next;
                }
                else {
                    prev -> next = current -> next;
                }
                delete current;
                cout << "Student ID " << id << " deleted successfully.\n";
            }
            cout << "Press any key to return.";
            _getch();
            system("cls");
            break;
        case 2:
            cout << "Enter the name of the student to delete: ";
            cin.ignore();
            getline(cin, name);

            while (current != nullptr && current -> name != name) {
                prev = current;
                current = current -> next;
            }

            if (current == nullptr) {
                cout << "Student name " << name << " is not found.\n";
            }
            else {
                if (prev == nullptr) {
                    head = current -> next;
                }
                else {
                    prev -> next = current -> next;
                }
                delete current;
                cout << "Student record name " << name << " deleted successfully.\n";
            }
            cout << "Press any key to return.";
            _getch();
            system("cls");
            break;
        case 3:
            break;
        default:
            cout << "Function calls only exist on 1 to 3! Press any key to try again.";
            _getch();
            system("cls");
            break;
    }
    }

    cout << "Press any key to return to the menu.";
    _getch();
    system("cls");
}

void Show_Record() {
    system("cls");
    if (head == nullptr) {
        cout << "No data available\n";
    } else {
        gotoxy(2, 1);  cout << "ID";
        gotoxy(12, 1); cout << "Full Name";
        gotoxy(45, 1); cout << "Math";
        gotoxy(55, 1); cout << "Physics";
        gotoxy(65, 1); cout << "Chemistry";
        gotoxy(75, 1); cout << "Average";

        int i = 2;
        for (Node* p = head; p != nullptr; p = p->next, ++i) {
            gotoxy(2, i);  cout << p->id;
            gotoxy(12, i); cout << p->name;
            gotoxy(45, i); cout << p->math;
            gotoxy(55, i); cout << p->physics;
            gotoxy(65, i); cout << p->chemistry;
            gotoxy(75, i); cout << fixed << setprecision(2) << p->average << endl;
        }
    }
}

int main() {

    //removed char yn cause no use (11/10/2024, N).

    string name; 
    int id, math, physics, chemistry;
    int Choice;
    
    while (true) {
        cout << "\n\t\tStudent Data Management System\n\n"
             << "\t1: Create New Data\n"
             << "\t2: Delete Student Record\n"  // Updated menu option
             << "\t3: Search Student Data\n"
             << "\t4: Display Full List\n"
             << "\t5: Exit System\n"
             << "\nEnter your choice (1 to 5)\n";

        while (cout << "\nChoice: " && !(cin >> Choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter again!";
            _getch();
            eraseLines(3);
        }

        switch(Choice) {
            case 1:
                system("cls");
                while (cout << "Enter ID: " && !(cin >> id)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input! Press any key to try again.";
                    _getch();
                    eraseLines(2);
                }
                cout << "Enter Full Name: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, name);

                // added confirmation line + delete previous wrong input of score for math, physics and chems (11/10/2024, N).

                do {
                    while(cout << "Enter Math Score: " && !(cin >> math)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid input! Press any key to try again.";
                        _getch();
                        eraseLines(2);
                    }
                    if (math < 0 || math > 10) {
                        cout << "Score only registers from 0 to 10! Press any key to try again";
                        _getch();
                        eraseLines(2);
                    }
                } while (math < 0 || math > 10);

                do {
                    while(cout << "Enter Physics Score: " && !(cin >> physics)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid input! Press any key to try again.";
                        _getch();
                        eraseLines(2);
                    }
                    if (physics < 0 || physics > 10) {
                        cout << "Score only registers from 0 to 10! Press any key to try again";
                        _getch();
                        eraseLines(2);
                    }
                } while (physics < 0 || physics > 10);

                do {
                    while(cout << "Enter Chemistry Score: " && !(cin >> chemistry)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid input! Press any key to try again.";
                        _getch();
                        eraseLines(2);
                    }
                    if (chemistry < 0 || chemistry > 10) {
                        cout << "Score only registers from 0 to 10! Press any key to try again";
                        _getch();
                        eraseLines(2);
                    }
                } while (chemistry < 0 || chemistry > 10);
                Insert_Record(id, name, math, physics, chemistry);
                break;
            case 2:
                Delete_Record();  // Call the new Delete_Record function
                break;
            case 3:
                system("cls");
                while (cout << "Enter ID to search: " && !(cin >> id)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input! Press any key to try again.";
                    _getch();
                    eraseLines(2);
                }
                Search_Record(id);
                break;
            case 4:
                Show_Record();
                _getch();
                system("cls");
                break;
            case 5:
                exit(0);
            default:
                cout << "The system function is from 1 to 5! Press any key to try again.";
                _getch();
                system("cls");
                break;
        }
    }
    return 0;
}