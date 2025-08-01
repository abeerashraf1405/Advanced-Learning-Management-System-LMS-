#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <limits>
#include <memory>

#include "models.h"
#include "utils.h"
using namespace std;

class LMS 
{
private:
    unique_ptr<User> currentUser;

    void initializeFiles() 
    {
        vector<pair<string, string>> requiredFiles = {
            {"students.txt", "[STUDENT RECORD]\n"},
            {"teachers.txt", "[TEACHER RECORD]\n"},
            {"timetable.txt", "[TIMETABLE]\n"},
            {"attendance.txt", "[ATTENDANCE]\n"},
            {"termReports.txt", "[TERM REPORTS]\n"},
            {"assignments_due.txt", "[ASSIGNMENTS]\n"},
            {"fees_ledger.txt", "[FEES LEDGER]\n"},
            {"staff.txt", "[STAFF RECORD]\n"},
            {"leave_requests.txt", "[LEAVE REQUESTS]\n"},
            {"parent_requests.txt", "[PARENT REQUESTS]\n"},
            {"fee_challans.txt", "[FEE CHALLANS]\n"}, {"salary_payments.txt", "[STAFF SALARIES]\n"},
            {"grades.txt", "[GRADES]\n"}
        };
        for (const auto& file : requiredFiles)
        {
            ifstream infile(file.first);
            if (!infile.good()) 
            {
                ofstream outfile(file.first);
                outfile << file.second;
                outfile.close();
            }
            infile.close();
        }
    }

    string selectRole() 
    {
        int choice;
        cout << "\n=== Select Login Role ===" << endl;
        cout << "1. Admin" << endl;
        cout << "2. Teacher" << endl;
        cout << "3. Student" << endl;
        cout << "4. Parent" << endl;
        cout << "5. Principal" << endl;
        cout << "Enter your role (1-5): ";

        while (!(cin >> choice) || choice < 1 || choice > 5) 
        {
            clearInputBuffer();
            cout << "Invalid input! Please enter a number between 1 and 5: ";
        }
        clearInputBuffer();

        switch (choice) 
        {
        case 1: 
            return "admin";
        case 2: 
            return "teacher";
        case 3:
            return "student";
        case 4: 
            return "parent";
        case 5: 
            return "principal";
        default:
            return "";
        }
    }

    bool login() 
    {
        string role = selectRole();
        if (role.empty()) 
            return false;

        cout << "\n=== " << role << " Login ===" << endl;
        string username, password;

        cout << "Username: ";
        getline(cin, username);
        cout << "Password: ";
        getline(cin, password);

        if (username.empty() || password.empty()) 
        {
            cout << "Username and password cannot be empty!" << endl;
            return false;
        }

        // Create user based on selected role
        if (role == "admin")
        {
            currentUser = make_unique<Admin>(username, password);
        }
        else if (role == "teacher")
        {
            currentUser = make_unique<Teacher>(username, password);
        }
        else if (role == "student")
        {
            currentUser = make_unique<Student>(username, password);
        }
        else if (role == "parent")
        {
            currentUser = make_unique<Parent>(username, password);
        }
        else if (role == "principal")
        {
            currentUser = make_unique<Principal>(username, password);
        }

        return true;
    }

    void clearInputBuffer() 
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    void showAdminMenu() 
    {
        Admin* admin = dynamic_cast<Admin*>(currentUser.get());
        if (!admin) 
            return;

        int choice;
        do {
            cout << "\n----------- ADMIN MENU -------------" << endl;
            cout << "1. Manage Students" << endl;
            cout << "2. Manage Teachers" << endl;
            cout << "3. Manage Staff" << endl;
            cout << "4. Process Payments" << endl;
            cout << "5. Handle Leave Requests" << endl;
            cout << "6. Generate Fee Challans" << endl;
            cout << "7. Yearly Class Allocation" << endl;
            cout << "0. Logout" << endl;
            cout << "Enter your choice: ";

            if (!(cin >> choice)) 
            {
                clearInputBuffer();
                cout << "Invalid input! Please enter a number." << endl;
                continue;
            }
            clearInputBuffer();

            switch (choice) 
            {
            case 1: 
                admin->manageStudents(); 
                break;
            case 2: 
                admin->manageTeachers();
                break;
            case 3: 
                admin->manageStaff(); 
                break;
            case 4: 
                admin->processPayments(); 
                break;
            case 5: 
                admin->handleLeaveRequests(); 
                break;
            case 6: 
                admin->generateFeeChallans(); 
                break;
            case 7: 
                admin->yearlyClassAllocation(); 
                break;
            case 0: 
                break;
            default: cout << "Invalid choice!" << endl;
            }
        } while (choice != 0);
    }

    void showTeacherMenu() 
    {
        Teacher* teacher = dynamic_cast<Teacher*>(currentUser.get());
        if (!teacher) 
        {
            cout << "Access denied - not a teacher!" << endl;
            return;
        }
        int choice;
        do {
            cout << "\n----------- TEACHER MENU -------------" << endl;
            cout << "1. Mark Attendance" << endl;
            cout << "2. Enter Grades" << endl;
            cout << "3. Generate Reports" << endl;
            cout << "4. Apply for Leave" << endl;
            cout << "5. View Timetable" << endl;
            cout << "0. Logout" << endl;
            cout << "Enter your choice: ";

            if (!(cin >> choice)) 
            {
                clearInputBuffer();
                cout << "Invalid input! Please enter a number." << endl;
                continue;
            }
            clearInputBuffer();

            switch (choice) 
            {
            case 1: 
                teacher->markAttendance(); 
                break;
            case 2: 
                teacher->enterGrades(); 
                break;
            case 3: 
                teacher->generateReports();
                break;
            case 4: 
                teacher->applyForLeave(); 
                break;
            case 5:
                teacher->viewTimetable();
                break;
            case 0:
                break;
            default: cout << "Invalid choice!" << endl;
            }
        } while (choice != 0);
    }

    void showStudentMenu() 
    {
        Student* student = dynamic_cast<Student*>(currentUser.get());
        if (!student)
            return;

        int choice;
        do {
            cout << "\n---------- STUDENT MENU ------------" << endl;
            cout << "1. View Timetable" << endl;
            cout << "2. View Assignments" << endl;
            cout << "3. View Grades" << endl;
            cout << "4. View Attendance" << endl;
            cout << "0. Logout" << endl;
            cout << "Enter your choice: ";

            if (!(cin >> choice)) 
            {
                clearInputBuffer();
                cout << "Invalid input! Please enter a number." << endl;
                continue;
            }
            clearInputBuffer();

            switch (choice)
            {
            case 1: 
                student->viewTimetable(); 
                break;
            case 2:
                student->viewAssignments(); 
                break;
            case 3:
                student->viewGrades(); 
                break;
            case 4: 
                student->viewAttendance();
                break;
            case 0:
                break;
            default: cout << "Invalid choice!" << endl;
            }
        } while (choice != 0);
    }

    void showParentMenu() 
    {
        Parent* parent = dynamic_cast<Parent*>(currentUser.get());
        if (!parent)
            return;

        int choice;
        do {
            cout << "\n---------- PARENT MENU ------------" << endl;
            cout << "1. View Child Progress" << endl;
            cout << "2. View Fee Status" << endl;
            cout << "3. Submit Request" << endl;
            cout << "4. View Child Attendance" << endl;
            cout << "0. Logout" << endl;
            cout << "Enter your choice: ";

            if (!(cin >> choice))
            {
                clearInputBuffer();
                cout << "Invalid input! Please enter a number." << endl;
                continue;
            }
            clearInputBuffer();

            switch (choice)
            {
            case 1: 
                parent->viewChildProgress();
                break;
            case 2: 
                parent->viewFeeStatus(); 
                break;
            case 3: 
                parent->submitRequest(); 
                break;
            case 4: 
                parent->viewChildAttendance(); 
                break;
            case 0: 
                break;
            default: cout << "Invalid choice!" << endl;
            }
        } while (choice != 0);
    }

    void showPrincipalMenu() 
    {
        Principal* principal = dynamic_cast<Principal*>(currentUser.get());
        if (!principal)
            return;

        int choice;
        do {
            cout << "\n----------- PRINCIPAL MENU ---------------" << endl;
            cout << "1. Handle Requests" << endl;
            cout << "2. Manage Staff Salaries" << endl;
            cout << "3. View School Reports" << endl;
            cout << "4. Approve Leave Requests" << endl;
            cout << "0. Logout" << endl;
            cout << "Enter your choice: ";

            if (!(cin >> choice))
            {
                clearInputBuffer();
                cout << "Invalid input! Please enter a number." << endl;
                continue;
            }
            clearInputBuffer();

            switch (choice) 
            {
            case 1:
                principal->handleRequests();
                break;
            case 2: 
                principal->manageSalaries(); 
                break;
            case 3: 
                principal->viewSchoolReports(); 
                break;
            case 4: 
                principal->approveLeaveRequests(); 
                break;
            case 0: 
                break;
            default: cout << "Invalid choice!" << endl;
            }
        } while (choice != 0);
    }

public:
    LMS()
    {
        initializeFiles();
    }

    void run()
    {
        while (true)
        {
            if (!currentUser)
            {
                if (!login())
                {
                    continue;
                }
            }

            if (dynamic_cast<Admin*>(currentUser.get()))
            {
                showAdminMenu();
            }
            else if (dynamic_cast<Teacher*>(currentUser.get())) 
            {
                showTeacherMenu();
            }
            else if (dynamic_cast<Student*>(currentUser.get()))
            {
                showStudentMenu();
            }
            else if (dynamic_cast<Parent*>(currentUser.get()))
            {
                showParentMenu();
            }
            else if (dynamic_cast<Principal*>(currentUser.get()))
            {
                showPrincipalMenu();
            }

            currentUser.reset();

            cout << "\nWould you like to exit? (y/n): ";
            char choice;
            cin >> choice;
            clearInputBuffer();
            if (tolower(choice) == 'y') 
            {
                break;
            }
        }
    }
};

int main() 
{
	cout << "\n\t--------------  ADVANCED LEARNING MANAGEMENT SYSTEM  --------------\n\n";
    LMS system;
    system.run();
    return 0;
}
