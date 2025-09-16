#include<iostream>
#include<conio.h>
#include<string>
#include<iomanip>
#include<fstream>

using namespace std;

class Employee {
protected:
    int emp_id;
    string emp_name;
    double base_salary;
public:
    Employee(int a = 0, string n = "", float b = 0.0) {
        emp_id = a;
        emp_name = n;
        base_salary = b;
    }

    virtual void show_info() {
        cout << "Employee ID: " << emp_id << endl;
        cout << "Employee Name: " << emp_name << endl;
        cout << "Base Salary: " << base_salary << endl;
    }
    string get_name() { return emp_name; }
    int get_ID() { return emp_id; }
};

class ITSkills {
protected:
    string IT_sector;
public:
    ITSkills(string n = "") { IT_sector = n; }
    virtual void show_info() {
        cout << "IT Sector: " << IT_sector << endl;
    }
    friend string choose_sect();
};

class Team;

class ManagementSkills {
protected:
    int No_of_emp;
public:
    ManagementSkills(int a = 0) { No_of_emp = a; }
    virtual void show_info() {
        cout << "This Person is in charge of " << No_of_emp << " employees " << endl;
    }
    friend class Team;
};

class Team {
public:
    double calc_team_bonus(ManagementSkills& M) {
        return M.No_of_emp * 500.0;
    }
};

template<class X = int>
X in_role(string role) {
    X temp;
    try {
        cout << "Enter no of years of experience as " << role << ": " << endl;
        cin >> temp;
        if (temp > 40) throw(temp);
        return temp;
    }
    catch (...) {
        cout << "Experience cannot exceed 40!!" << endl;
        do {
            cout << "Enter no of years of experience as " << role << ": " << endl;
            cin >> temp;
        } while (temp > 40);
        return temp;
    }
}

template<class T1 = double, class T2 = int, class T3>
T1 role_sal(T2 exp, T3* obj) {
    if (obj->com_num() == 4)
        return exp * 1000;
    else if (obj->com_num() == 5)
        return exp * 1500;
}

class ITSpecialist : virtual public Employee, public ITSkills {
protected:
    double base_salary_ITsk;
    double ITspecial_salary;
    int IT_exp;
public:
    ITSpecialist(int id = 0, string name = "", float b_sal = 0.0, string dept = "")
        : Employee(id, name, b_sal), ITSkills(dept) {
        IT_exp = in_role("IT Specialist");
    }

    void calc_IT_special() {
        ITSpecialist* s = this;
        base_salary_ITsk = role_sal(IT_exp, s);
        ITspecial_salary = base_salary_ITsk + base_salary;
    }

    void show_info() {
        Employee* E, E1(emp_id, emp_name, base_salary);
        E = &E1;
        ITSkills* IT, IT1(IT_sector);
        IT = &IT1;
        E->show_info();
        IT->show_info();
        cout << "Base IT salary: " << base_salary_ITsk << endl;
        cout << emp_name << " has " << IT_exp << " yrs of experience as an IT Specialist " << endl;
    }

    int com_num() { return 4; }
};

class Manager : virtual public Employee, public ManagementSkills {
protected:
    int manager_exp;
    double Manager_salary, Team_sal;
    float base_salary_msk;
public:
    Manager(int id = 0, string name = "", float b_sal = 0.0, int emp = 0)
        : Employee(id, name, b_sal), ManagementSkills(emp) {
        manager_exp = in_role("Manager");
    }

    void calc_Manager_sal() {
        Team t;
        Team_sal = t.calc_team_bonus(*this);
        Manager* s = this;
        base_salary_msk = role_sal(manager_exp, s);
        Manager_salary = base_salary_msk + Team_sal;
    }

    void show_info() {
        ManagementSkills* M, M1(No_of_emp);
        M = &M1;
        M->show_info();
        cout << "Bonus Management salary: " << base_salary_msk << endl;
        cout << emp_name << " has " << manager_exp << "yrs of experience as a Manager " << endl;
    }

    int com_num() { return 5; }
};

class ITManager : public ITSpecialist, public Manager {
protected:
    double Total_sal;
public:
    ITManager(int id = 0, string name = "", float b_sal = 0.0, string dept = "", int no_of_emp = 0)
        : Employee(id, name, b_sal), ITSpecialist(id, name, b_sal, dept),
        Manager(id, name, b_sal, no_of_emp) {}

    void calc_total_sal() {
        calc_IT_special();
        calc_Manager_sal();
        Total_sal = Manager_salary + ITspecial_salary;
    }

    void operator=(ITManager& B) {
        Total_sal = B.Total_sal;
        emp_name = B.emp_name;
    }

    bool operator>(ITManager& A) { return Total_sal > A.Total_sal; }
    bool operator==(string& name) { return emp_name == name; }
    bool operator==(double tot) { return Total_sal == tot; }

    void show_info();

    friend void set_info(ITManager* a, int n);
    friend void get_all();
    friend float compare(ITManager* a[], int n, string name);
    friend void compare(ITManager a, int n);
    friend int compare(ITManager* a[], int n, int id);
};

double temp;

void ITManager::show_info() {
    cout << setfill('*') << setw(40) << "" << endl;
    ITSpecialist::show_info();
    Manager::show_info();
    cout << "Final Salary of Manager is: " << Total_sal << endl;
}

inline void set_info(ITManager* a, int n) {
    fstream myfile;
    myfile.open("Manager-info.txt", (n == 0 ? ios::out : ios::app));
    myfile << setfill('*') << setw(20) << "";
    myfile << "Manager " << n + 1;
    myfile << setfill('*') << setw(20) << "" << endl;
    myfile << "Employee ID: " << a->emp_id << endl;
    myfile << "Employee Name: " << a->emp_name << endl;
    myfile << "Base Salary: " << a->base_salary << endl;
    myfile << "IT Sector: " << a->IT_sector << endl;
    myfile << "This Person is in charge of " << a->No_of_emp << " employees " << endl;
    myfile << "Base IT salary: " << a->base_salary_ITsk << endl;
    myfile << a->emp_name << " has " << a->IT_exp << " yrs of experience as an IT Specialist " << endl;
    myfile << "Bonus Management salary: " << a->base_salary_msk << endl;
    myfile << a->emp_name << " has " << a->manager_exp << "yrs of experience as a Manager " << endl;
    myfile << "Final Salary of Manager is: " << a->Total_sal << endl;
    myfile << endl;
    myfile.close();
}

inline void get_all() {
    fstream myfile;
    myfile.open("Manager-info.txt", ios::in);
    string line;
    while (getline(myfile, line)) cout << line << endl;
    myfile.close();
}

inline float compare(ITManager* a[], int n, string name) {
    try {
        if (n == 0) cout << "No managers' Info in database" << endl;
        for (int i = 0; i < n; i++) {
            if (*(a[i]) == name) throw(2);
        }
        return 0.0;
    }
    catch (int) {
        cout << "Manager with given name already exists!!\n";
        return 2.0;
    }
}

inline void compare(ITManager a, int n) {
    temp = a.Total_sal;
    if (a.Total_sal > temp) temp = a.Total_sal;
}

inline int compare(ITManager* a[], int n, int id) {
    for (int i = 0; i < n; i++) {
        if (a[i]->emp_id == id) {
            a[i]->show_info();
            return 1;
        }
    }
    cout << "Manager with given ID is not present in the company" << endl;
    return 0;
}

inline string choose_sect() {
    int choice;
    cout << "1.Cyber-Security: " << endl;
    cout << setw(30) << setfill('-') << "2.Front-End: " << endl;
    cout << setw(29) << setfill('-') << "3.Back-End: " << endl;
    cout << setw(26) << setfill('-') << "4.AI/ML: " << endl;
    cout << setw(36) << setfill('-') << "5.Cloud-Computing: " << endl;
    cout << setw(40) << setfill('-') << "6.Database-Management: " << endl;
    cin >> choice;
    switch (choice) {
        case 1: return "Cyber-Security";
        case 2: return "Front-End";
        case 3: return "Back-End";
        case 4: return "AI/ML";
        case 5: return "Cloud-Computing";
        case 6: return "Database_Management";
        default: cout << "Wrong choice" << endl;
    }
}

int main() {
    ITManager* M[20];
    int n = 0, choice;
    while (1) {
        cout << setw(39) << setfill('=') << "=" << endl;
        cout << "Enter 1.Input Manager Details\n2.Compare salaries\n3.Display Details\n4.EXIT\n";
        cout << "-> ";
        cin >> choice;
        switch (choice) {
            case 1:
                try {
                    if (n >= 20) throw(n);
                    else {
                        cout << setfill('*') << setw(20) << "";
                        cout << "Manager " << n + 1;
                        cout << setfill('*') << setw(20) << "" << endl;
                        string name;
                        cout << "Enter Employee Name: ";
                        cin >> name;
                        if (n != 0) {
                            float temp = compare(M, n, name);
                            if (temp == 2.0) continue;
                        }
                        int id, no_of_emp;
                        string sector;
                        float base_sal;
                        cout << "Enter Employee ID: ";
                        cin >> id;
                        cout << "Enter Base Salary: ";
                        cin >> base_sal;
                        cout << "Enter IT Sector: ";
                        sector = choose_sect();
                        cout << "Enter Team Size: ";
                        cin >> no_of_emp;
                        if (no_of_emp > 20) throw('x');
                        M[n] = new ITManager(id, name, base_sal, sector, no_of_emp);
                        M[n]->calc_total_sal();
                        set_info(M[n], n);
                        n++;
                    }
                }
                catch (int x) { cout << "No. of Managers exceeded the limit\n"; }
                catch (char c) { cout << "Team cannot have more than 20 members\n"; }
                break;
            case 2:
                for (int i = 0; i < n; i++) compare(*M[i], n);
                for (int i = 0; i < n; i++) {
                    if (*(M[i]) == temp) {
                        cout << "Manager with highest salary is: " << M[i]->get_name() << endl;
                        break;
                    }
                }
                break;
            case 3:
                int choice2;
                cout << setw(19) << setfill('-') << "-" << endl;
                cout << "1.Display One Manager\n2.Display All Managers" << endl;
                cout << "-> ";
                cin >> choice2;
                int found;
                switch (choice2) {
                    case 1:
                        int ID;
                        cout << "Enter Manager ID : ";
                        cin >> ID;
                        found = compare(M, n, ID);
                        if (found == 0) cout << "Manager not found." << endl;
                        break;
                    case 2:
                        get_all();
                        break;
                    default:
                        cout << "Wrong choice " << endl;
                }
                break;
            case 4:
                for (int i = 0; i < n; i++) delete M[i];
                exit(0);
            default:
                cout << "Wrong choice" << endl;
                break;
        }
    }
    _getch();
    return 0;
}
