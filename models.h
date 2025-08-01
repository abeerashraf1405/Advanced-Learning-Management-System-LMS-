#pragma once
#ifndef MODELS_H
#define MODELS_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <ctime>
#include <iomanip>
#include <memory>
#include <algorithm>
#include <limits>

#include "utils.h"


using namespace std;

// Record Structures
struct StaffRecord 
{
    string id;
    string name;
    string role;
    string contact;
    string salary;
    int leavesTaken;

    StaffRecord(const string& id, const string& name, const string& role, const string& contact, const string& salary, int leaves) : id(id), name(name), role(role), contact(contact), salary(salary), leavesTaken(leaves) {}

    string toString() const 
    {
        return "ID: " + id + " | Name: " + name + " | Role: " + role + " | Contact: " + contact + " | Salary: " + salary + " | Leaves Taken: " + to_string(leavesTaken);
    }
};

class StudentRecord 
{
public:
    string id;
    string name;
    string className;
    string rollNo;
    string parentContact;
    string feeStatus;

    StudentRecord(const string& id, const string& name, const string& className, const string& rollNo, const string& parentContact, const string& feeStatus) : id(id), name(name), className(className), rollNo(rollNo), parentContact(parentContact), feeStatus(feeStatus) {}

    string toString() const
    {
        return "ID: " + id + " | Name: " + name + " | Class: " + className + " | Roll No: " + rollNo + " | Parent Contact: " + parentContact + " | Fee Status: " + feeStatus;
    }
};

class TeacherRecord 
{
public:
    string id;
    string name;
    vector<string> subjects;
    string qualification;
    string contact;
	int leavesTaken = 0; // Default to 0 leaves taken
    vector<string> assignedClasses;
    int periodsPerWeek;

    TeacherRecord(const string& id, const string& name, const vector<string>& subjects, const string& qualification, const string& contact, const vector<string>& assignedClasses, int periodsPerWeek) : id(id), name(name), subjects(subjects), qualification(qualification), contact(contact), assignedClasses(assignedClasses), periodsPerWeek(periodsPerWeek) {}

    string toString() const 
    {
        string subjs = Utils::joinString(subjects, ", ");
        string classes = Utils::joinString(assignedClasses, ", ");
        return "ID: " + id + " | Name: " + name + " | Subjects: " + subjs + " | Qualification: " + qualification + " | Contact: " + contact + " | Classes: " + classes + " | Periods/Week: " + to_string(periodsPerWeek);
    }
};

// Base User Class
class User 
{
protected:
    string username;
    string password;

public:
    User(const string& uname, const string& pwd) : username(uname), password(pwd) {}
    virtual ~User() {}

    string getUsername() const 
    {
        return username; 
    }
    string getPassword() const 
    {
        return password; 
    }

    virtual void displayMenu() = 0;
};

// Admin Class
class Admin : public User 
{
private:
    vector<StaffRecord> staffMembers;

public:
    Admin(const string& uname, const string& pwd) : User(uname, pwd) {}

    void manageStudents() 
    {
        cout << "\n=== Manage Students ===" << endl;
        vector<StudentRecord> students = loadStudents();

        int choice;
        do {
            cout << "\n1. Add Student\n2. View All Students\n3. Update Student\n4. Remove Student\n0. Back\n";
            cout << "Enter choice: ";
            cin >> choice;
            Utils::clearInputBuffer();

            switch (choice) 
            {
            case 1: 
                addStudent(students);
                break;
            case 2:
                viewStudents(students); 
                break;
            case 3: 
                updateStudent(students); 
                break;
            case 4: 
                removeStudent(students); 
                break;
            case 0: break;
            default:
                cout << "Invalid choice!" << endl;
            }
        } while (choice != 0);

        saveStudents(students);
    }

    void manageTeachers() 
    {
        cout << "\n=== Manage Teachers ===" << endl;
        vector<TeacherRecord> teachers = loadTeachers();

        int choice;
        do {
            cout << "\n1. Add Teacher\n2. View All Teachers\n3. Update Teacher\n4. Remove Teacher\n0. Back\n";
            cout << "Enter choice: ";
            cin >> choice;
            Utils::clearInputBuffer();

            switch (choice) {
            case 1:
                addTeacher(teachers);
                break;
            case 2: 
                viewTeachers(teachers); 
                break;
            case 3: 
                updateTeacher(teachers);
                break;
            case 4: 
                removeTeacher(teachers); 
                break;
            case 0: break;
            default: cout << "Invalid choice!" << endl;
            }
        } while (choice != 0);

        saveTeachers(teachers);
    }

    void manageStaff() 
    {
        cout << "\n=== Manage Staff ===" << endl;
        staffMembers = loadStaff();

        int choice;
        do {
            cout << "\n1. Add Staff\n2. View All Staff\n3. Update Staff\n4. Remove Staff\n0. Back\n";
            cout << "Enter choice: ";
            cin >> choice;
            Utils::clearInputBuffer();

            switch (choice) {
            case 1: addStaff(staffMembers); break;
            case 2: viewStaff(staffMembers); break;
            case 3: updateStaff(staffMembers); break;
            case 4: removeStaff(staffMembers); break;
            case 0: break;
            default: cout << "Invalid choice!" << endl;
            }
        } while (choice != 0);

        saveStaff(staffMembers);
    }

    void processPayments() {
        cout << "\n=== Process Payments ===" << endl;
        vector<TeacherRecord> teachers = loadTeachers();
        staffMembers = loadStaff();

        int choice;
        do {
            cout << "\n1. Process Teacher Salaries\n" << "2. Process Staff Salaries\n" << "3. Process Fee Payments\n" << "0. Back\n" << "Enter choice: ";
            cin >> choice;
            Utils::clearInputBuffer();

            switch (choice) 
            {
            case 1: processTeacherPayments(teachers); break;
            case 2: processStaffPayments(staffMembers); break;
            case 3: processFeePayments(); break;
            case 0: break;
            default: cout << "Invalid choice!" << endl;
            }
        } while (choice != 0);
    }

    void handleLeaveRequests() 
    {
        cout << "\n=== Handle Leave Requests ===" << endl;
        ifstream file("leave_requests.txt");
        string line;
        vector<string> pendingRequests;

        // Read all pending requests
        while (getline(file, line)) 
        {
            if (line.empty()) continue;
            vector<string> parts = Utils::parseRecord(line);
            if (parts.size() >= 6 && parts[5] == "Pending") 
            {
                pendingRequests.push_back(line);
            }
        }

        if (pendingRequests.empty()) 
        {
            cout << "No pending leave requests." << endl;
            return;
        }

        // Display pending requests
        cout << "\nPending Leave Requests:\n";
        for (size_t i = 0; i < pendingRequests.size(); ++i)
        {
            vector<string> parts = Utils::parseRecord(pendingRequests[i]);
            cout << i + 1 << ". " << parts[1] << " (" << parts[0] << "): " << parts[2] << " to " << parts[3] << " - " << parts[4] << endl;
        }

        // Process requests
        cout << "\nEnter request number to process (0 to cancel): ";
        int requestNum;
        cin >> requestNum;
        Utils::clearInputBuffer();

        if (requestNum > 0 && requestNum <= static_cast<int>(pendingRequests.size())) 
        {
            cout << "1. Approve\n2. Reject\nEnter choice: ";
            int action;
            cin >> action;
            Utils::clearInputBuffer();

            string status = (action == 1) ? "Approved" : "Rejected";
            vector<string> parts = Utils::parseRecord(pendingRequests[requestNum - 1]);
            parts[5] = status;

            // Update the request in the file
            ofstream outFile("leave_requests.txt", ios::app);
            outFile << Utils::joinString(parts, "|") << "\n";
            cout << "Request " << status << " successfully!" << endl;
        }
    }

    void generateFeeChallans() 
    {
        cout << "\n=== Generate Fee Challans ===" << endl;
        vector<StudentRecord> students = loadStudents();
        string monthYear;

        cout << "Enter month and year (MM-YYYY): ";
        getline(cin, monthYear);

        ofstream challanFile("fee_challans.txt", ios::app);
        challanFile << "\n[FEE CHALLANS " << monthYear << "]\n";

        for (const auto& student : students)
        {
            cout << "\nStudent: " << student.name << " (" << student.id << ")" << "\nClass: " << student.className << "\nGenerate challan? (y/n): ";
            char choice;
            cin >> choice;
            Utils::clearInputBuffer();

            if (tolower(choice) == 'y') 
            {
                string amount;
                cout << "Enter fee amount: ";
                getline(cin, amount);

                challanFile << student.id << "|" << student.name << "|" << student.className << "|" << monthYear << "|" << amount << "|Unpaid\n";
                cout << "Challan generated for " << student.name << endl;
            }
        }

        cout << "\nFee challan generation complete." << endl;
    }

    void yearlyClassAllocation() 
    {
        cout << "\n=== Yearly Class Allocation ===" << endl;
        vector<StudentRecord> students = loadStudents();

        for (auto& student : students) 
        {
            string currentClass = student.className;
            if (!currentClass.empty() && isdigit(currentClass[0]))
            {
                // Promote to next class (e.g., 10-A becomes 11-A)
                int classNum = stoi(currentClass.substr(0, currentClass.find('-')));
                if (classNum < 12)
                {  // Don't promote beyond 12th grade
                    classNum++;
                    string newClass = to_string(classNum) + currentClass.substr(currentClass.find('-'));
                    cout << "Promoting " << student.name << " from " << currentClass << " to " << newClass << endl;
                    student.className = newClass;
                }
            }
        }

        saveStudents(students);
        cout << "Yearly class promotion completed successfully!" << endl;
    }

    void displayMenu() override
    {
        int choice;
        do {
            cout << "\n=== ADMIN MENU ===" << endl;
            cout << "1. Manage Students\n2. Manage Teachers\n3. Manage Staff\n" << "4. Process Payments\n5. Handle Leave Requests\n" << "6. Generate Fee Challans\n7. Yearly Class Allocation\n" << "0. Logout\nEnter choice: ";
            cin >> choice;
            Utils::clearInputBuffer();

            switch (choice) {
            case 1: manageStudents(); break;
            case 2: manageTeachers(); break;
            case 3: manageStaff(); break;
            case 4: processPayments(); break;
            case 5: handleLeaveRequests(); break;
            case 6: generateFeeChallans(); break;
            case 7: yearlyClassAllocation(); break;
            case 0: cout << "Logging out...\n"; break;
            default: cout << "Invalid choice!" << endl;
            }
        } while (choice != 0);
    }

private:
    // Student Management
    vector<StudentRecord> loadStudents() 
    {
        vector<StudentRecord> students;
        ifstream file("students.txt");
        string line;

        while (getline(file, line)) 
        {
            if (line.empty() || line[0] == '[') continue;

            vector<string> parts = Utils::parseRecord(line);
            if (parts.size() >= 6) {
                students.emplace_back(parts[0], parts[1], parts[2], parts[3], parts[4], parts[5]);
            }
        }

        return students;
    }

    void saveStudents(const vector<StudentRecord>& students)
    {
        ofstream file("students.txt");
        file << "[STUDENT RECORD]\n";

        for (const auto& student : students) 
        {
            file << student.id << "|" << student.name << "|" << student.className << "|"
                << student.rollNo << "|" << student.parentContact << "|" << student.feeStatus << "\n";
        }
    }

    void addStudent(vector<StudentRecord>& students) 
    {
        string id, name, className, rollNo, parentContact, feeStatus;

        cout << "Enter student ID: ";
        getline(cin, id);
        cout << "Enter student name: ";
        getline(cin, name);
        cout << "Enter class: ";
        getline(cin, className);
        cout << "Enter roll number: ";
        getline(cin, rollNo);
        cout << "Enter parent contact: ";
        getline(cin, parentContact);
        cout << "Enter fee status: ";
        getline(cin, feeStatus);

        students.emplace_back(id, name, className, rollNo, parentContact, feeStatus);
        cout << "Student added successfully!" << endl;
    }

    void viewStudents(const vector<StudentRecord>& students)
    {
        if (students.empty())
        {
            cout << "No students found!" << endl;
            return;
        }

        for (const auto& student : students) 
        {
            cout << student.toString() << endl;
        }
    }

    void updateStudent(vector<StudentRecord>& students) 
    {
        string id;
        cout << "Enter student ID to update: ";
        getline(cin, id);

        auto it = find_if(students.begin(), students.end(), [&id](const StudentRecord& s)
            {
            return s.id == id;
            });

        if (it == students.end())
        {
            cout << "Student not found!" << endl;
            return;
        }

        cout << "Current student details: " << it->toString() << endl;

        string name, className, rollNo, parentContact, feeStatus;
        cout << "Enter new name (leave empty to keep current): ";
        getline(cin, name);
        if (!name.empty()) 
            it->name = name;

        cout << "Enter new class (leave empty to keep current): ";
        getline(cin, className);
        if (!className.empty()) 
            it->className = className;

        cout << "Enter new roll number (leave empty to keep current): ";
        getline(cin, rollNo);
        if (!rollNo.empty()) 
            it->rollNo = rollNo;

        cout << "Enter new parent contact (leave empty to keep current): ";
        getline(cin, parentContact);
        if (!parentContact.empty()) 
            it->parentContact = parentContact;

        cout << "Enter new fee status (leave empty to keep current): ";
        getline(cin, feeStatus);
        if (!feeStatus.empty()) 
            it->feeStatus = feeStatus;

        cout << "Student updated successfully!" << endl;
    }

    void removeStudent(vector<StudentRecord>& students)
    {
        string id;
        cout << "Enter student ID to remove: ";
        getline(cin, id);

        auto it = find_if(students.begin(), students.end(), [&id](const StudentRecord& s) 
            {
            return s.id == id;
            });

        if (it == students.end())
        {
            cout << "Student not found!" << endl;
            return;
        }

        students.erase(it);
        cout << "Student removed successfully!" << endl;
    }

    // Teacher Management
    vector<TeacherRecord> loadTeachers() 
    {
        vector<TeacherRecord> teachers;
        ifstream file("teachers.txt");
        string line;

        while (getline(file, line))
        {
            if (line.empty() || line[0] == '[') 
                continue;

            vector<string> parts = Utils::parseRecord(line);
            if (parts.size() >= 7)
            {
                vector<string> subjects = Utils::splitString(parts[2], ',');
                vector<string> classes = Utils::splitString(parts[5], ',');
                int periods = stoi(parts[6]);

                teachers.emplace_back(parts[0], parts[1], subjects, parts[3], parts[4], classes, periods);
            }
        }

        return teachers;
    }

    void saveTeachers(const vector<TeacherRecord>& teachers) 
    {
        ofstream file("teachers.txt");
        file << "[TEACHER RECORD]\n";

        for (const auto& teacher : teachers)
        {
            file << teacher.id << "|" << teacher.name << "|" << Utils::joinString(teacher.subjects, ",") << "|" << teacher.qualification << "|" << teacher.contact << "|" << Utils::joinString(teacher.assignedClasses, ",") << "|" << teacher.periodsPerWeek << "\n";
        }
    }

    void addTeacher(vector<TeacherRecord>& teachers)
    {
        string id, name, qualification, contact;
        vector<string> subjects, assignedClasses;
        int periodsPerWeek;

        cout << "Enter teacher ID: ";
        getline(cin, id);
        cout << "Enter teacher name: ";
        getline(cin, name);

        cout << "Enter subjects (comma separated): ";
        string subjInput;
        getline(cin, subjInput);
        subjects = Utils::splitString(subjInput, ',');

        cout << "Enter qualification: ";
        getline(cin, qualification);
        cout << "Enter contact: ";
        getline(cin, contact);

        cout << "Enter assigned classes (comma separated): ";
        string classInput;
        getline(cin, classInput);
        assignedClasses = Utils::splitString(classInput, ',');

        cout << "Enter periods per week: ";
        cin >> periodsPerWeek;
        Utils::clearInputBuffer();

        teachers.emplace_back(id, name, subjects, qualification, contact, assignedClasses, periodsPerWeek);
        cout << "Teacher added successfully!" << endl;
    }

    void viewTeachers(const vector<TeacherRecord>& teachers) 
    {
        if (teachers.empty())
        {
            cout << "No teachers found!" << endl;
            return;
        }

        for (const auto& teacher : teachers) 
        {
            cout << teacher.toString() << endl;
        }
    }

    void updateTeacher(vector<TeacherRecord>& teachers) 
    {
        string id;
        cout << "Enter teacher ID to update: ";
        getline(cin, id);

        auto it = find_if(teachers.begin(), teachers.end(), [&id](const TeacherRecord& t) 
            {
            return t.id == id;
            });

        if (it == teachers.end())
        {
            cout << "Teacher not found!" << endl;
            return;
        }

        cout << "Current teacher details: " << it->toString() << endl;

        string name, qualification, contact;
        vector<string> subjects, assignedClasses;
        int periodsPerWeek;

        cout << "Enter new name (leave empty to keep current): ";
        getline(cin, name);
        if (!name.empty()) 
            it->name = name;

        cout << "Enter new subjects (comma separated, leave empty to keep current): ";
        string subjInput;
        getline(cin, subjInput);
        if (!subjInput.empty()) 
            it->subjects = Utils::splitString(subjInput, ',');

        cout << "Enter new qualification (leave empty to keep current): ";
        getline(cin, qualification);
        if (!qualification.empty())
            it->qualification = qualification;

        cout << "Enter new contact (leave empty to keep current): ";
        getline(cin, contact);
        if (!contact.empty())
            it->contact = contact;

        cout << "Enter new assigned classes (comma separated, leave empty to keep current): ";
        string classInput;
        getline(cin, classInput);
        if (!classInput.empty())
            it->assignedClasses = Utils::splitString(classInput, ',');

        cout << "Enter new periods per week (enter 0 to keep current): ";
        cin >> periodsPerWeek;
        Utils::clearInputBuffer();
        if (periodsPerWeek > 0) 
            it->periodsPerWeek = periodsPerWeek;

        cout << "Teacher updated successfully!" << endl;
    }

    void removeTeacher(vector<TeacherRecord>& teachers) 
    {
        string id;
        cout << "Enter teacher ID to remove: ";
        getline(cin, id);

        auto it = find_if(teachers.begin(), teachers.end(), [&id](const TeacherRecord& t) 
            {
            return t.id == id;
            });

        if (it == teachers.end()) 
        {
            cout << "Teacher not found!" << endl;
            return;
        }

        teachers.erase(it);
        cout << "Teacher removed successfully!" << endl;
    }

    // Staff Management
    vector<StaffRecord> loadStaff() 
    {
        vector<StaffRecord> staff;
        ifstream file("staff.txt");
        string line;

        while (getline(file, line))
        {
            if (line.empty() || line[0] == '[') 
                continue;

            vector<string> parts = Utils::parseRecord(line);
            if (parts.size() >= 6) 
            {
                staff.emplace_back(parts[0], parts[1], parts[2], parts[3], parts[4], stoi(parts[5]));
            }
        }

        return staff;
    }

    void saveStaff(const vector<StaffRecord>& staff) 
    {
        ofstream file("staff.txt");
        file << "[STAFF RECORD]\n";

        for (const auto& member : staff)
        {
            file << member.id << "|" << member.name << "|" << member.role << "|"
                << member.contact << "|" << member.salary << "|" << member.leavesTaken << "\n";
        }
    }

    void addStaff(vector<StaffRecord>& staff) 
    {
        string id, name, role, contact, salary;
        int leaves = 0;

        cout << "Enter staff ID: ";
        getline(cin, id);
        cout << "Enter staff name: ";
        getline(cin, name);
        cout << "Enter role: ";
        getline(cin, role);
        cout << "Enter contact: ";
        getline(cin, contact);
        cout << "Enter salary: ";
        getline(cin, salary);

        staff.emplace_back(id, name, role, contact, salary, leaves);
        cout << "Staff member added successfully!" << endl;
    }

    void viewStaff(const vector<StaffRecord>& staff)
    {
        if (staff.empty())
        {
            cout << "No staff members found!" << endl;
            return;
        }

        for (const auto& member : staff) 
        {
            cout << member.toString() << endl;
        }
    }

    void updateStaff(vector<StaffRecord>& staff) 
    {
        string id;
        cout << "Enter staff ID to update: ";
        getline(cin, id);

        auto it = find_if(staff.begin(), staff.end(), [&id](const StaffRecord& s)
            {
            return s.id == id;
            });

        if (it == staff.end()) 
        {
            cout << "Staff member not found!" << endl;
            return;
        }

        cout << "Current staff details: " << it->toString() << endl;

        string name, role, contact, salary;
        cout << "Enter new name (leave empty to keep current): ";
        getline(cin, name);
        if (!name.empty())
            it->name = name;

        cout << "Enter new role (leave empty to keep current): ";
        getline(cin, role);
        if (!role.empty())
            it->role = role;

        cout << "Enter new contact (leave empty to keep current): ";
        getline(cin, contact);
        if (!contact.empty())
            it->contact = contact;

        cout << "Enter new salary (leave empty to keep current): ";
        getline(cin, salary);
        if (!salary.empty())
            it->salary = salary;

        cout << "Staff member updated successfully!" << endl;
    }

    void removeStaff(vector<StaffRecord>& staff)
    {
        string id;
        cout << "Enter staff ID to remove: ";
        getline(cin, id);

        auto it = find_if(staff.begin(), staff.end(), [&id](const StaffRecord& s)
            {
            return s.id == id;
            });

        if (it == staff.end()) 
        {
            cout << "Staff member not found!" << endl;
            return;
        }

        staff.erase(it);
        cout << "Staff member removed successfully!" << endl;
    }

    // Payment Processing
    void processTeacherPayments(vector<TeacherRecord>& teachers)
    {
        cout << "\n=== Teacher Salary Payments ===" << endl;
        ofstream paymentFile("salary_payments.txt", ios::app);
        string currentDate = Utils::getCurrentDate();

        paymentFile << "\n[TEACHER SALARIES " << currentDate << "]\n";

        for (auto& teacher : teachers) 
        {
            // Check if salary already processed this month
            if (isSalaryProcessed(teacher.id, currentDate.substr(0, 7))) 
            {
                cout << "Salary already processed for " << teacher.name << endl;
                continue;
            }

            // Calculate salary (basic salary - deductions for excess leaves)
            float salary = calculateTeacherSalary(teacher);

            cout << "Processing payment for " << teacher.name << " (" << teacher.id << "): " << fixed << setprecision(2) << salary << endl;

            paymentFile << teacher.id << "|" << teacher.name << "|" << salary << "|" << currentDate << "\n";
        }

        cout << "Teacher salary processing complete." << endl;
    }

    void processStaffPayments(vector<StaffRecord>& staff) 
    {
        cout << "\n=== Staff Salary Payments ===" << endl;
        ofstream paymentFile("salary_payments.txt", ios::app);
        string currentDate = Utils::getCurrentDate();

        paymentFile << "\n[STAFF SALARIES " << currentDate << "]\n";

        for (auto& member : staff) 
        {
            // Check if salary already processed this month
            if (isSalaryProcessed(member.id, currentDate.substr(0, 7))) 
            {
                cout << "Salary already processed for " << member.name << endl;
                continue;
            }

            // Calculate salary (basic salary - deductions for excess leaves)
            float salary = calculateStaffSalary(member);

            cout << "Processing payment for " << member.name << " (" << member.id << "): " << fixed << setprecision(2) << salary << endl;

            paymentFile << member.id << "|" << member.name << "|" << salary << "|" << currentDate << "\n";
        }

        cout << "Staff salary processing complete." << endl;
    }

    void processFeePayments()
    {
        cout << "\n=== Student Fee Payments ===" << endl;
        vector<StudentRecord> students = loadStudents();
        string currentDate = Utils::getCurrentDate();

        for (auto& student : students)
        {
            // Skip if fee already paid
            if (student.feeStatus.find("Paid till") != string::npos) {
                continue;
            }

            cout << "\nStudent: " << student.name << " (" << student.id << ")" << "\nClass: " << student.className << "\nFee Status: " << student.feeStatus << "\nEnter amount received: ";

            string amount;
            getline(cin, amount);

            // Update fee status
            student.feeStatus = "Paid till " + currentDate.substr(0, 7);
            cout << "Fee payment recorded for " << student.name << endl;
        }

        // Save updated student records
        saveStudents(students);
        cout << "Fee payment processing complete." << endl;
    }

    bool isSalaryProcessed(const string& id, const string& monthYear)
    {
        ifstream paymentFile("salary_payments.txt");
        string line;

        while (getline(paymentFile, line)) {
            if (line.find(id) != string::npos && line.find(monthYear) != string::npos) 
            {
                return true;
            }
        }
        return false;
    }

    float calculateTeacherSalary(const TeacherRecord& teacher) 
    {
        // Base salary calculation (example: 2000 per period)
        float baseSalary = teacher.periodsPerWeek * 2000 * 4; // Monthly salary

        // Deduct for excess leaves (example: 500 per excess leave)
        int allowedLeaves = 2; // 2 leaves per month allowed
        int excessLeaves = max(0, (teacher.leavesTaken - allowedLeaves));
        class TeacherRecord 
        {  
        public:  
            string id;  
            string name;  
            vector<string> subjects;  
            string qualification;  
            string contact;  
            vector<string> assignedClasses;  
            int periodsPerWeek;  
            int leavesTaken;   

            TeacherRecord(const string& id, const string& name, const vector<string>& subjects,  
                const string& qualification, const string& contact, const vector<string>& assignedClasses, int periodsPerWeek, int leavesTaken = 0)  
                : id(id), name(name), subjects(subjects), qualification(qualification), contact(contact), assignedClasses(assignedClasses), periodsPerWeek(periodsPerWeek), leavesTaken(leavesTaken) {}  

            string toString() const 
            {  
                string subjs = Utils::joinString(subjects, ", ");  
                string classes = Utils::joinString(assignedClasses, ", ");  
                return "ID: " + id + " | Name: " + name + " | Subjects: " + subjs + " | Qualification: " + qualification + " | Contact: " + contact +   " | Classes: " + classes + " | Periods/Week: " + to_string(periodsPerWeek) +   " | Leaves Taken: " + to_string(leavesTaken); // Include leavesTaken in the string representation  
            }  
        };
        float deductions = excessLeaves * 500;

        return max(0.0f, baseSalary - deductions);
    }

    float calculateStaffSalary(const StaffRecord& staff) 
    {
        // Convert string salary to float
        float salary = stof(staff.salary);

        // Deduct for excess leaves (example: 1% per excess leave)
        int allowedLeaves = 2; // 2 leaves per month allowed
        int excessLeaves = max(0, (staff.leavesTaken - allowedLeaves));
        float deductions = salary * (excessLeaves * 0.01f);

        return max(0.0f, salary - deductions);
    }
};

// Teacher Class
class Teacher : public User
{
private:
    string teacherId;
    vector<string> subjects;
    vector<string> assignedClasses;

public:
    Teacher(const string& uname, const string& pwd) : User(uname, pwd)
    {
        // Load teacher data from file
        ifstream file("teachers.txt");
        string line;

        while (getline(file, line)) {
            if (line.empty() || line[0] == '[')
                continue;

            vector<string> parts = Utils::parseRecord(line);
            if (parts.size() >= 2 && parts[1] == username) 
            {
                teacherId = parts[0];
                subjects = Utils::splitString(parts[2], ',');
                assignedClasses = Utils::splitString(parts[5], ',');
                break;
            }
        }
        if (assignedClasses.empty()) 
        {
            assignedClasses.push_back("10-A"); // Default class
            cout << "Warning: No classes assigned to teacher. Using default class 10-A\n";
        }
    }

    void markAttendance()
    {
        cout << "\n=== Mark Attendance ===" << endl;
        string date = Utils::getCurrentDate();
        cout << "Marking attendance for date: " << date << endl;

        // Let teacher select which class to mark attendance for
        cout << "Your assigned classes: ";
        for (const auto& cls : assignedClasses) 
        {
            cout << cls << " ";
        }
        cout << endl;

        string selectedClass;
        cout << "Enter class to mark attendance for: ";
        getline(cin, selectedClass);

        // Verify the teacher is assigned to this class
        if (find(assignedClasses.begin(), assignedClasses.end(), selectedClass) == assignedClasses.end()) 
        {
            cout << "You are not assigned to this class!\n";
            return;
        }

        // Load students for the selected class
        vector<StudentRecord> students;
        ifstream file("students.txt");
        string line;

        while (getline(file, line)) 
        {
            if (line.empty() || line[0] == '[') 
                continue;

            vector<string> parts = Utils::parseRecord(line);
            if (parts.size() >= 3 && parts[2] == selectedClass) 
            {
                students.emplace_back(parts[0], parts[1], parts[2], parts[3], parts[4], parts[5]);
            }
        }

        if (students.empty())
        {
            cout << "No students found in class " << selectedClass << "!\n";
            return;
        }

        // Mark attendance for each student
        ofstream attFile("attendance.txt", ios::app);
        attFile << "\n[ATTENDANCE " << date << " CLASS " << selectedClass << "]\n";

        for (const auto& student : students)
        {
            cout << "Student: " << student.name << " (" << student.id << ") - Present? (y/n): ";
            char present;
            cin >> present;
            Utils::clearInputBuffer();

            attFile << student.id << ": " << (tolower(present) == 'y' ? "Present" : "Absent") << "\n";
        }

        cout << "Attendance marked successfully for class " << selectedClass << "!\n";
    }

    void enterGrades()
    {
        cout << "\n=== Enter Grades ===" << endl;

        // Show assigned classes
        cout << "Your assigned classes: ";
        for (const auto& cls : assignedClasses)
        {
            cout << cls << " ";
        }
        cout << endl;

        // Select class
        string selectedClass;
        cout << "Enter class to enter grades for: ";
        getline(cin, selectedClass);

        // Verify assignment
        if (find(assignedClasses.begin(), assignedClasses.end(), selectedClass) == assignedClasses.end()) 
        {
            cout << "You are not assigned to this class!\n";
            return;
        }

        // Load students
        vector<StudentRecord> students;
        ifstream file("students.txt");
        string line;

        while (getline(file, line))
        {
            if (line.empty() || line[0] == '[') 
                continue;

            vector<string> parts = Utils::parseRecord(line);
            if (parts.size() >= 3 && parts[2] == selectedClass) 
            {
                students.emplace_back(parts[0], parts[1], parts[2], parts[3], parts[4], parts[5]);
            }
        }

        if (students.empty())
        {
            cout << "No students found in class " << selectedClass << "!\n";
            return;
        }

        // Select assignment type
        cout << "Enter assignment type (quiz/midterm/final): ";
        string assignmentType;
        getline(cin, assignmentType);

        // Enter grades
        ofstream gradeFile("grades.txt", ios::app);
        gradeFile << "\n[GRADES " << assignmentType << " CLASS " << selectedClass << " TEACHER " << teacherId << "]\n";

        for (const auto& student : students)
        {
            cout << "Enter grade for " << student.name << " (" << student.id << "): ";
            string gradeStr;
            getline(cin, gradeStr);

            // Validate grade
            try {
                int grade = stoi(gradeStr);
                if (grade < 0 || grade > 100) 
                {
                    cout << "Invalid grade! Must be 0-100. Skipping...\n";
                    continue;
                }
                gradeFile << student.id << "|" << grade << "\n";
            }
            catch (...) 
            {
                cout << "Invalid grade format! Skipping...\n";
            }
        }

        cout << "Grades entered successfully!\n";
    }

    void generateReports() 
    {
        cout << "\n=== Generate Reports ===" << endl;
        // Show assigned classes
        cout << "Your assigned classes: ";
        for (const auto& cls : assignedClasses) 
        {
            cout << cls << " ";
        }
        cout << endl;

        // Select class
        string selectedClass;
        cout << "Enter class to generate reports for: ";
        getline(cin, selectedClass);

        // Verify assignment
        if (find(assignedClasses.begin(), assignedClasses.end(), selectedClass) == assignedClasses.end()) 
        {
            cout << "You are not assigned to this class!\n";
            return;
        }

        // Load students
        vector<StudentRecord> students;
        ifstream studentFile("students.txt");
        string line;

        while (getline(studentFile, line))
        {
            if (line.empty() || line[0] == '[')
                continue;

            vector<string> parts = Utils::parseRecord(line);
            if (parts.size() >= 3 && parts[2] == selectedClass)
            {
                students.emplace_back(parts[0], parts[1], parts[2], parts[3], parts[4], parts[5]);
            }
        }

        if (students.empty()) 
        {
            cout << "No students found in class " << selectedClass << "!\n";
            return;
        }

        // Get current term
        string currentTerm;
        cout << "Enter term (e.g., Fall-2023): ";
        getline(cin, currentTerm);

        // Generate report for each student
        ofstream reportFile("termReports.txt", ios::app);
        reportFile << "\n[TERM REPORTS " << currentTerm << " CLASS " << selectedClass << "]\n";

        for (const auto& student : students)
        {
            // Get grades
            map<string, int> grades = loadStudentGrades(student.id);
            // Get attendance
            pair<int, int> attendance = loadStudentAttendance(student.id);

            // Calculate weighted grade
            float weightedGrade = 0.0f;
            if (grades.count("quiz")) weightedGrade += grades["quiz"] * 0.3f;
            if (grades.count("midterm")) weightedGrade += grades["midterm"] * 0.4f;
            if (grades.count("final")) weightedGrade += grades["final"] * 0.3f;

            // Generate report
            reportFile << "STUDENT: " << student.name << " (" << student.id << ")\n";
            reportFile << "CLASS: " << selectedClass << "\n";
            reportFile << "TERM: " << currentTerm << "\n";

            // Grades section
            reportFile << "GRADES:\n";
            for (const auto& grade : grades) 
            {
                reportFile << "  " << grade.first << ": " << grade.second << "/100\n";
            }
            reportFile << "WEIGHTED GRADE: " << weightedGrade << "/100\n";

            // Attendance section
            float attendancePercent = (attendance.first * 100.0f) / max(1, attendance.second);
            reportFile << "ATTENDANCE: " << attendance.first << "/" << attendance.second
                << " (" << attendancePercent << "%)\n";

            // Comments
            reportFile << "COMMENTS: \n";
            reportFile << "----------------------------------------\n";

            // Print to console
            cout << "\nGenerated report for " << student.name << " (" << student.id << ")\n";
        }

        cout << "\nReports generated successfully for " << selectedClass << "!\n";
        cout << "Saved to termReports.txt\n";
    }

    void applyForLeave()
    {
        cout << "\n=== Apply for Leave ===" << endl;
        string startDate, endDate, reason;

        cout << "Enter start date (YYYY-MM-DD): ";
        getline(cin, startDate);
        if (!Utils::validateDate(startDate)) 
        {
            cout << "Invalid date format!" << endl;
            return;
        }

        cout << "Enter end date (YYYY-MM-DD): ";
        getline(cin, endDate);
        if (!Utils::validateDate(endDate))
        {
            cout << "Invalid date format!" << endl;
            return;
        }

        cout << "Enter reason: ";
        getline(cin, reason);

        ofstream file("leave_requests.txt", ios::app);
        file << teacherId << "|" << username << "|" << startDate << "|" << endDate << "|" << reason << "|Pending\n";

        cout << "Leave application submitted successfully!" << endl;
    }

    void viewTimetable()
    {
        cout << "\n=== View Timetable ===" << endl;
        ifstream file("timetable.txt");
        if (!file.is_open())
        {
            cout << "Timetable not found or unable to open!" << endl;
            return;
        }

        cout << "\nYour Weekly Schedule:\n";
        cout << "--------------------------------------------------\n";
        cout << left << setw(10) << "Day" << setw(15) << "Period"
            << setw(15) << "Class" << setw(15) << "Room" << endl;
        cout << "--------------------------------------------------\n";

        string line;
        bool found = false;

        while (getline(file, line)) {
            if (line.empty() || line[0] == '[') 
                continue;

            vector<string> parts = Utils::parseRecord(line);
            if (parts.size() >= 5 && parts[2] == teacherId)
            {
                cout << left << setw(10) << parts[0] << setw(15) << parts[1] << setw(15) << parts[3] << setw(15) << parts[4] << endl;
                found = true;
            }
        }

        if (!found) 
        {
            cout << "No timetable entries found for you." << endl;
        }
        cout << "--------------------------------------------------\n";
    }

    void displayMenu() override 
    {
        int choice;
        do {
            cout << "\n=== TEACHER MENU ===" << endl;
            cout << "1. Mark Attendance\n2. Enter Grades\n3. Generate Reports\n"
                << "4. Apply for Leave\n5. View Timetable\n0. Logout\nEnter choice: ";
            cin >> choice;
            Utils::clearInputBuffer();

            switch (choice)
            {
            case 1: markAttendance(); break;
            case 2: enterGrades(); break;
            case 3: generateReports(); break;
            case 4: applyForLeave(); break;
            case 5: viewTimetable(); break;
            case 0: cout << "Logging out...\n"; break;
            default: cout << "Invalid choice!" << endl;
            }
        } while (choice != 0);
    }

private:
    map<string, int> loadStudentGrades(const string& studentId) 
    {
        map<string, int> grades;
        ifstream gradeFile("grades.txt");
        string line;
        string currentSection;

        while (getline(gradeFile, line)) 
        {
            if (line.empty()) continue;

            if (line[0] == '[') 
            {
                currentSection = line;
                continue;
            }

            if (currentSection.find(studentId) != string::npos) 
            {
                vector<string> parts = Utils::parseRecord(line);
                if (parts.size() >= 2)
                {
                    string type = currentSection.substr(currentSection.find("GRADES") + 6);
                    type = type.substr(0, type.find(" "));
                    grades[type] = stoi(parts[1]);
                }
            }
        }

        return grades;
    }

    pair<int, int> loadStudentAttendance(const string& studentId) 
    {
        int present = 0, total = 0;
        ifstream attFile("attendance.txt");
        string line;
        string currentDate;

        while (getline(attFile, line))
        {
            if (line.empty()) 
                continue;

            if (line.find("[ATTENDANCE") != string::npos) 
            {
                currentDate = line;
                total++;
                continue;
            }

            if (line.find(studentId) != string::npos && line.find("Present") != string::npos) 
            {
                present++;
            }
        }

        return make_pair(present, total);
    }

    vector<StudentRecord> loadStudentsForClass()
    {
        vector<StudentRecord> students;
        ifstream file("students.txt");
        string line;

        while (getline(file, line)) {
            if (line.empty() || line[0] == '[') 
                continue;

            vector<string> parts = Utils::parseRecord(line);
            if (parts.size() >= 3)
            {
                if (find(assignedClasses.begin(), assignedClasses.end(), parts[2]) != assignedClasses.end()) 
                {
                    students.emplace_back(parts[0], parts[1], parts[2], parts[3], parts[4], parts[5]);
                }
            }
        }

        return students;
    }
};

// Student Class
class Student : public User
{
private:
    string studentId;
    string className;
    string parentContact;

    string getTeacherName(const string& teacherId) 
    {
        ifstream file("teachers.txt");
        if (!file.is_open())
        {
            cerr << "Error opening teachers file!" << endl;
            return "Unknown";
        }

        string line;
        while (getline(file, line))
        {
            if (line.empty() || line[0] == '[') 
                continue;

            vector<string> parts = Utils::parseRecord(line);
            if (parts.size() >= 2 && parts[0] == teacherId)
            {
                return parts[1];
            }
        }
        return "Unknown Teacher";
    }

public:
    Student(const string& uname, const string& pwd) : User(uname, pwd)
    {
        // Load student data from file
        ifstream file("students.txt");
        string line;

        while (getline(file, line)) {
            if (line.empty() || line[0] == '[') 
                continue;

            vector<string> parts = Utils::parseRecord(line);
            if (parts.size() >= 2 && parts[1] == username) 
            {
                studentId = parts[0];
                className = parts[2];
                parentContact = parts[4];
                break;
            }
        }
    }

    void viewTimetable() 
    {
        cout << "\n=== My Timetable ===" << endl;
        cout << "Student Class: " << className << endl;

        ifstream file("timetable.txt");
        if (!file.is_open()) 
        {
            cout << "ERROR: Could not open timetable file!" << endl;
            return;
        }

        cout << "\nWeekly Schedule for " << className << ":\n";
        cout << "-------------------------------------------------------------\n";
        cout << left << setw(10) << "Day" << setw(12) << "Period"
            << setw(25) << "Teacher" << setw(15) << "Room" << endl;
        cout << "-------------------------------------------------------------\n";

        string line;
        bool found = false;

        while (getline(file, line)) 
        {
            if (line.empty() || line[0] == '[' || line.find("--") != string::npos) 
            {
                continue;
            }

            vector<string> parts = Utils::parseRecord(line);

            if (parts.size() >= 5 && parts[3] == className)
            {
                string teacherName = getTeacherName(parts[2]);
                cout << left << setw(10) << parts[0] << setw(12) << parts[1] << setw(25) << teacherName << setw(15) << parts[4] << endl;
                found = true;
            }
        }

        if (!found) 
        {
            cout << "No classes found for your class (" << className << ")" << endl;
        }
        cout << "-------------------------------------------------------------\n";
    }

    void viewAssignments() 
    {
    cout << "\n=== View Assignments ===" << endl;
    ifstream file("assignments_due.txt");

    if (!file.is_open()) 
    {
        cout << "No assignments found for your class (" << className << ")" << endl;
        return;
    }

    bool found = false;
    string line;

    cout << "--------------------------------------------------\n";
    cout << left << setw(15) << "Subject" << setw(30) << "Assignment" 
         << setw(12) << "Due Date" << endl;
    cout << "--------------------------------------------------\n";

    while (getline(file, line)) 
    {
        if (line.empty() || line[0] == '[') 
            continue;

        vector<string> parts = Utils::parseRecord(line);
        
        if (parts.size() >= 4 && Utils::toUpper(parts[1]) == Utils::toUpper(className)) 
        {
            cout << setw(15) << left << parts[0] << setw(30) << left << parts[2] << setw(12) << left << parts[3] << endl;
            found = true;
        }
    }

    if (!found)
    {
        cout << "No assignments found for your class (" << className << ")" << endl;
    }
    cout << "--------------------------------------------------\n";
}

    void viewGrades()
    {
        cout << "\n=== View Grades ===" << endl;
        ifstream file("grades.txt");

        if (!file.is_open())
        {
            cout << "No grades recorded yet. Possible reasons:\n" << "1. Teachers haven't entered grades\n" << "2. grades.txt file doesn't exist\n";
            return;
        }

        map<string, map<string, int>> grades;
        string currentSection;
        string line;

        while (getline(file, line))
        {
            if (line.empty()) 
                continue;

            if (line[0] == '[')
            {
                currentSection = line;
                continue;
            }

            vector<string> parts;
            size_t start = 0;
            size_t end = line.find('|');
            while (end != string::npos)
            {
                parts.push_back(line.substr(start, end - start));
                start = end + 1;
                end = line.find('|', start);
            }
            parts.push_back(line.substr(start));

            if (parts.size() >= 2 && parts[0] == studentId) 
            {
                // Extract subject from section header
                size_t subjectStart = currentSection.find("CLASS") + 6;
                size_t subjectEnd = currentSection.find(" ", subjectStart);
                string subject = currentSection.substr(subjectStart, subjectEnd - subjectStart);

                // Extract grade type
                size_t typeStart = currentSection.find("GRADES") + 7;
                size_t typeEnd = currentSection.find(" ", typeStart);
                string type = currentSection.substr(typeStart, typeEnd - typeStart);

                try {
                    grades[subject][type] = stoi(parts[1]);
                }
                catch (...) {
                    cout << "Warning: Invalid grade format for " << subject << endl;
                }
            }
        }

        if (grades.empty())
        {
            cout << "No grades found for you yet." << endl;
            return;
        }

        // Display grades
        cout << "\nGrade Report:\n";
        cout << "--------------------------------------------------\n";
        cout << left << setw(15) << "Subject" << setw(10) << "Quiz" << setw(10) << "Midterm" << setw(10) << "Final" << endl;
        cout << "--------------------------------------------------\n";

        for (const auto& subject : grades) {
            cout << setw(15) << left << subject.first;
            cout << setw(10) << (subject.second.count("quiz") ? to_string(subject.second.at("quiz")) : "-");
            cout << setw(10) << (subject.second.count("midterm") ? to_string(subject.second.at("midterm")) : "-");
            cout << setw(10) << (subject.second.count("final") ? to_string(subject.second.at("final")) : "-");
            cout << endl;
        }
        cout << "--------------------------------------------------\n";
    }

    void viewAttendance() 
    {
        cout << "\n=== View Attendance ===" << endl;
        ifstream file("attendance.txt");

        if (!file.is_open())
        {
            cout << "No attendance records found. Possible reasons:\n" << "1. Attendance not taken yet\n" << "2. attendance.txt file doesn't exist\n";
            return;
        }

        string line;
        string currentDate;
        int presentDays = 0, totalDays = 0;
        bool found = false;

        while (getline(file, line)) {
            if (line.empty())
                continue;

            if (line.find("[ATTENDANCE") != string::npos) {
                currentDate = line.substr(11, 10);
                totalDays++;
                continue;
            }

            if (line.find(studentId) != string::npos) {
                string status = line.substr(line.find(":") + 2);
                cout << "Date: " << currentDate << " - " << status << endl;
                found = true;
                if (status == "Present") presentDays++;
            }
        }

        if (!found) {
            cout << "No attendance records found for you." << endl;
        }
        else {
            float percentage = (float)presentDays / totalDays * 100;
            cout << "\nSummary: " << presentDays << "/" << totalDays << " days present (" << fixed << setprecision(1) << percentage << "%)" << endl;
        }
    }

    void displayMenu() override {
        int choice;
        do {
            cout << "\n=== STUDENT MENU ===" << endl;
            cout << "1. View Timetable\n2. View Assignments\n3. View Grades\n" << "4. View Attendance\n0. Logout\nEnter choice: ";
            cin >> choice;
            Utils::clearInputBuffer();

            switch (choice) {
            case 1: viewTimetable(); break;
            case 2: viewAssignments(); break;
            case 3: viewGrades(); break;
            case 4: viewAttendance(); break;
            case 0: cout << "Logging out...\n"; break;
            default: cout << "Invalid choice!" << endl;
            }
        } while (choice != 0);
    }
};

// Parent Class
class Parent : public User {
private:
    string parentId;
    vector<string> childrenIds;

    StudentRecord getChildRecord(const string& childId) {
        ifstream file("students.txt");
        string line;

        while (getline(file, line)) {
            if (line.empty() || line[0] == '[')
                continue;

            vector<string> parts = Utils::parseRecord(line);
            if (parts.size() >= 6 && parts[0] == childId) {
                return StudentRecord(parts[0], parts[1], parts[2], parts[3], parts[4], parts[5]);
            }
        }
        return StudentRecord("", "", "", "", "", "");
    }

    map<string, map<string, int>> getChildGrades(const string& childId) {
        map<string, map<string, int>> gradesBySubject;
        ifstream gradeFile("grades.txt");
        string line;
        string currentSection;

        while (getline(gradeFile, line)) {
            if (line.empty()) 
                continue;

            if (line[0] == '[') {
                currentSection = line;
                continue;
            }

            vector<string> parts = Utils::parseRecord(line);
            if (parts.size() >= 2 && parts[0] == childId) {
                size_t subjectPos = currentSection.find("CLASS") + 6;
                size_t subjectEnd = currentSection.find(" ", subjectPos);
                string subject = currentSection.substr(subjectPos, subjectEnd - subjectPos);

                size_t typePos = currentSection.find("GRADES") + 7;
                size_t typeEnd = currentSection.find(" ", typePos);
                string type = currentSection.substr(typePos, typeEnd - typePos);

                gradesBySubject[subject][type] = stoi(parts[1]);
            }
        }
        return gradesBySubject;
    }

    pair<int, int> getChildAttendance(const string& childId) {
        int present = 0, total = 0;
        ifstream attFile("attendance.txt");
        string line;

        while (getline(attFile, line)) {
            if (line.empty()) 
                continue;

            if (line.find("[ATTENDANCE") != string::npos) {
                total++;
                continue;
            }

            if (line.find(childId) != string::npos && line.find("Present") != string::npos) {
                present++;
            }
        }
        return make_pair(present, total);
    }

    float calculateOverallAverage(const map<string, map<string, int>>& gradesBySubject) {
        float total = 0.0f;
        int count = 0;

        for (const auto& subject : gradesBySubject) {
            float weightedAvg = 0.0f;
            if (subject.second.count("quiz"))
                weightedAvg += subject.second.at("quiz") * 0.3f;
            if (subject.second.count("midterm")) 
                weightedAvg += subject.second.at("midterm") * 0.4f;
            if (subject.second.count("final"))
                weightedAvg += subject.second.at("final") * 0.3f;

            total += weightedAvg;
            count++;
        }

        return count > 0 ? total / count : 0.0f;
    }

public:
    Parent(const string& uname, const string& pwd) : User(uname, pwd)
    {
        // Load parent data from file
        ifstream file("students.txt");
        string line;

        while (getline(file, line)) {
            if (line.empty() || line[0] == '[')
                continue;

            vector<string> parts = Utils::parseRecord(line);
            if (parts.size() >= 5 && parts[4] == username) {
                childrenIds.push_back(parts[0]);
            }
        }
    }

    void viewChildProgress() {
        cout << "\n=== Child Progress Report ===" << endl;

        if (childrenIds.empty()) {
            cout << "No children registered under your account." << endl;
            return;
        }

        for (const string& childId : childrenIds) {
            StudentRecord child = getChildRecord(childId);
            if (child.id.empty()) {
                cout << "Could not find records for child ID: " << childId << endl;
                continue;
            }

            cout << "\nChild: " << child.name << " (" << childId << ")";
            cout << "\nClass: " << child.className;
            cout << "\n--------------------------------------------------\n";

            map<string, map<string, int>> gradesBySubject = getChildGrades(childId);
            pair<int, int> attendance = getChildAttendance(childId);
            float attendancePercent = (attendance.first * 100.0f) / max(1, attendance.second);

            if (!gradesBySubject.empty()) {
                cout << "Academic Performance:\n";
                cout << left << setw(15) << "Subject" << setw(10) << "Quiz" << setw(10) << "Midterm" << setw(10) << "Final" << setw(15) << "Average" << endl;
                cout << "--------------------------------------------------\n";

                for (const auto& subject : gradesBySubject) {
                    float weightedAvg = 0.0f;
                    if (subject.second.count("quiz")) 
                        weightedAvg += subject.second.at("quiz") * 0.3f;
                    if (subject.second.count("midterm")) 
                        weightedAvg += subject.second.at("midterm") * 0.4f;
                    if (subject.second.count("final")) 
                        weightedAvg += subject.second.at("final") * 0.3f;

                    cout << left << setw(15) << subject.first << setw(10) << (subject.second.count("quiz") ? to_string(subject.second.at("quiz")) : "-") << setw(10) << (subject.second.count("midterm") ? to_string(subject.second.at("midterm")) : "-") << setw(10) << (subject.second.count("final") ? to_string(subject.second.at("final")) : "-") << setw(15) << fixed << setprecision(1) << weightedAvg << endl;
                }
            }
            else {
                cout << "No grade records available.\n";
            }

            cout << "\nAttendance: " << attendance.first << "/" << attendance.second  << " (" << attendancePercent << "%)\n";

            cout << "\nOverall Progress: ";
            if (attendancePercent < 75) {
                cout << "Needs improvement (Low attendance)";
            }
            else if (!gradesBySubject.empty()) {
                float overallAvg = calculateOverallAverage(gradesBySubject);
                if (overallAvg >= 85) 
                    cout << "Excellent";
                else if (overallAvg >= 70)
                    cout << "Good";
                else 
                    cout << "Needs improvement";
            }
            cout << "\n--------------------------------------------------\n";
        }
    }

    void viewFeeStatus() {
        cout << "\n=== View Fee Status ===" << endl;
        ifstream file("fees_ledger.txt");
        string line;
        bool found = false;

        for (const auto& childId : childrenIds) {
            cout << "\nFee status for child ID " << childId << ":\n";
            while (getline(file, line)) {
                if (line.empty() || line[0] == '[')
                    continue;

                vector<string> parts = Utils::parseRecord(line);
                if (parts.size() >= 2 && parts[0] == childId) {
                    cout << "Month: " << parts[1] << " | Amount: " << parts[2] << " | Status: " << parts[3] << endl;
                    found = true;
                }
            }
            file.seekg(0);
        }

        if (!found) {
            cout << "No fee records found for your children!" << endl;
        }
    }

    void submitRequest() {
        cout << "\n=== Submit Request ===" << endl;
        string childId, requestType, note;

        cout << "Select child ID: ";
        for (const auto& id : childrenIds) {
            cout << id << " ";
        }
        cout << "\nEnter child ID: ";
        getline(cin, childId);

        if (find(childrenIds.begin(), childrenIds.end(), childId) == childrenIds.end()) {
            cout << "Invalid child ID!" << endl;
            return;
        }

        cout << "Enter request type (e.g., section change, seat change): ";
        getline(cin, requestType);
        cout << "Enter additional note: ";
        getline(cin, note);

        ofstream file("parent_requests.txt", ios::app);
        file << childId << "|" << username << "|" << requestType << "|" << note << "|Pending\n";

        cout << "Request submitted successfully!" << endl;
    }

    void viewChildAttendance() {
        cout << "\n=== View Child Attendance ===" << endl;
        ifstream file("attendance.txt");
        string line;
        string date;
        bool found = false;

        for (const auto& childId : childrenIds) {
            cout << "\nAttendance records for child ID " << childId << ":\n";
            while (getline(file, line)) {
                if (line.find("[ATTENDANCE") != string::npos) {
                    date = line.substr(11, 10);
                    continue;
                }

                if (line.find(childId) != string::npos) {
                    cout << "Date: " << date << " | Status: " << line.substr(line.find(":") + 2) << endl;
                    found = true;
                }
            }
            file.seekg(0);
        }

        if (!found) {
            cout << "No attendance records found for your children!" << endl;
        }
    }

    void displayMenu() override {
        int choice;
        do {
            cout << "\n=== PARENT MENU ===" << endl;
            cout << "1. View Child Progress\n2. View Fee Status\n" << "3. Submit Request\n4. View Child Attendance\n0. Logout\nEnter choice: ";
            cin >> choice;
            Utils::clearInputBuffer();

            switch (choice) {
            case 1: viewChildProgress(); break;
            case 2: viewFeeStatus(); break;
            case 3: submitRequest(); break;
            case 4: viewChildAttendance(); break;
            case 0: cout << "Logging out...\n"; break;
            default: cout << "Invalid choice!" << endl;
            }
        } while (choice != 0);
    }
};

// Principal Class
class Principal : public User {
public:
    Principal(const string& uname, const string& pwd) : User(uname, pwd) {}

    void handleRequests() {
        cout << "\n=== Handle Requests ===" << endl;
        ifstream file("parent_requests.txt");
        string line;
        vector<string> pendingRequests;

        while (getline(file, line)) {
            if (line.empty())
                continue;
            vector<string> parts = Utils::parseRecord(line);
            if (parts.size() >= 5 && parts[4] == "Pending") {
                pendingRequests.push_back(line);
            }
        }

        if (pendingRequests.empty()) {
            cout << "No pending requests." << endl;
            return;
        }

        cout << "\nPending Requests:\n";
        for (size_t i = 0; i < pendingRequests.size(); ++i) {
            vector<string> parts = Utils::parseRecord(pendingRequests[i]);
            cout << i + 1 << ". " << parts[2] << " for " << parts[0] << ": " << parts[3] << endl;
        }

        cout << "\nEnter request number to process (0 to cancel): ";
        int requestNum;
        cin >> requestNum;
        Utils::clearInputBuffer();

        if (requestNum > 0 && requestNum <= static_cast<int>(pendingRequests.size())) {
            cout << "1. Approve\n2. Reject\nEnter choice: ";
            int action;
            cin >> action;
            Utils::clearInputBuffer();

            string status = (action == 1) ? "Approved" : "Rejected";
            vector<string> parts = Utils::parseRecord(pendingRequests[requestNum - 1]);
            parts[4] = status;

            ofstream outFile("parent_requests.txt", ios::app);
            outFile << Utils::joinString(parts, "|") << "\n";
            cout << "Request " << status << " successfully!" << endl;
        }
    }

    void manageSalaries() {
        cout << "\n=== Manage Salaries ===" << endl;
        vector<TeacherRecord> teachers;
        vector<StaffRecord> staff;
        ifstream teacherFile("teachers.txt");
        ifstream staffFile("staff.txt");
        string line;

        // Load teachers
        while (getline(teacherFile, line)) {
            if (line.empty() || line[0] == '[') 
                continue;
            vector<string> parts = Utils::parseRecord(line);
            if (parts.size() >= 7) {
                vector<string> subjects = Utils::splitString(parts[2], ',');
                vector<string> classes = Utils::splitString(parts[5], ',');
                teachers.emplace_back(parts[0], parts[1], subjects, parts[3], parts[4], classes, stoi(parts[6]));
            }
        }

        // Load staff
        while (getline(staffFile, line)) {
            if (line.empty() || line[0] == '[') 
                continue;
            vector<string> parts = Utils::parseRecord(line);
            if (parts.size() >= 6) {
                staff.emplace_back(parts[0], parts[1], parts[2], parts[3], parts[4], stoi(parts[5]));
            }
        }

        int choice;
        do {
            cout << "\n1. View Teacher Salaries\n2. View Staff Salaries\n" << "3. Update Teacher Salary\n4. Update Staff Salary\n0. Back\nEnter choice: ";
            cin >> choice;
            Utils::clearInputBuffer();

            switch (choice) {
            case 1: {
                cout << "\nTeacher Salaries:\n";
                for (const auto& teacher : teachers) {
                    float salary = teacher.periodsPerWeek * 2000 * 4;
                    cout << teacher.name << " (" << teacher.id << "): " << salary << endl;
                }
                break;
            }
            case 2: {
                cout << "\nStaff Salaries:\n";
                for (const auto& member : staff) {
                    cout << member.name << " (" << member.id << "): " << member.salary << endl;
                }
                break;
            }
            case 3: {
                string id;
                cout << "Enter teacher ID: ";
                getline(cin, id);
                auto it = find_if(teachers.begin(), teachers.end(), [&id](const TeacherRecord& t) {
                    return t.id == id;
                    });
                if (it == teachers.end()) {
                    cout << "Teacher not found!" << endl;
                    break;
                }
                cout << "Current periods per week: " << it->periodsPerWeek << endl;
                cout << "Enter new value: ";
                int periods;
                cin >> periods;
                Utils::clearInputBuffer();
                it->periodsPerWeek = periods;
                cout << "Teacher salary updated!" << endl;
                break;
            }
            case 4: {
                string id;
                cout << "Enter staff ID: ";
                getline(cin, id);
                auto it = find_if(staff.begin(), staff.end(), [&id](const StaffRecord& s) {
                    return s.id == id;
                    });
                if (it == staff.end()) {
                    cout << "Staff member not found!" << endl;
                    break;
                }
                cout << "Current salary: " << it->salary << endl;
                cout << "Enter new salary: ";
                string salary;
                getline(cin, salary);
                it->salary = salary;
                cout << "Staff salary updated!" << endl;
                break;
            }
            case 0: break;
            default: cout << "Invalid choice!" << endl;
            }
        } while (choice != 0);

        // Save changes
        ofstream teacherOut("teachers.txt");
        teacherOut << "[TEACHER RECORD]\n";
        for (const auto& teacher : teachers) 
        {
            teacherOut << teacher.id << "|" << teacher.name << "|" << Utils::joinString(teacher.subjects, ",") << "|" << teacher.qualification << "|" << teacher.contact << "|" << Utils::joinString(teacher.assignedClasses, ",") << "|" << teacher.periodsPerWeek << "\n";
        }

        ofstream staffOut("staff.txt");
        staffOut << "[STAFF RECORD]\n";
        for (const auto& member : staff) 
        {
            staffOut << member.id << "|" << member.name << "|" << member.role << "|" << member.contact << "|" << member.salary << "|" << member.leavesTaken << "\n";
        }
    }

    void viewSchoolReports()
    {
        cout << "\n=== View School Reports ===" << endl;
        ifstream file("termReports.txt");
        string line;
        bool found = false;

        cout << "Available Term Reports:\n";
        while (getline(file, line))
        {
            if (line.find("[TERM REPORTS") != string::npos) 
            {
                cout << line.substr(1, line.length() - 2) << endl;
                found = true;
            }
        }

        if (!found) 
        {
            cout << "No reports available!" << endl;
            return;
        }

        cout << "\nEnter report name to view (or 'all' for all reports): ";
        string reportName;
        getline(cin, reportName);

        file.clear();
        file.seekg(0);
        bool specificReport = (reportName != "all");
        bool reportFound = false;

        while (getline(file, line)) 
        {
            if (line.empty()) 
                continue;

            if (line[0] == '[')
            {
                if (specificReport && line.find(reportName) != string::npos) 
                {
                    reportFound = true;
                }
                continue;
            }

            if (!specificReport || reportFound) 
            {
                cout << line << endl;
            }

            if (reportFound && line.find("----------------------------------------") != string::npos) 
            {
                reportFound = false;
                if (specificReport) break;
            }
        }
    }

    void approveLeaveRequests()
    {
        cout << "\n=== Approve Leave Requests ===" << endl;
        ifstream file("leave_requests.txt");
        string line;
        vector<string> pendingRequests;

        while (getline(file, line)) 
        {
            if (line.empty()) continue;
            vector<string> parts = Utils::parseRecord(line);
            if (parts.size() >= 6 && parts[5] == "Pending") 
            {
                pendingRequests.push_back(line);
            }
        }

        if (pendingRequests.empty()) 
        {
            cout << "No pending leave requests." << endl;
            return;
        }

        cout << "\nPending Leave Requests:\n";
        for (size_t i = 0; i < pendingRequests.size(); ++i)
        {
            vector<string> parts = Utils::parseRecord(pendingRequests[i]);
            cout << i + 1 << ". " << parts[1] << " (" << parts[0] << "): "
                << parts[2] << " to " << parts[3] << " - " << parts[4] << endl;
        }

        cout << "\nEnter request number to process (0 to cancel): ";
        int requestNum;
        cin >> requestNum;
        Utils::clearInputBuffer();

        if (requestNum > 0 && requestNum <= static_cast<int>(pendingRequests.size())) 
        {
            cout << "1. Approve\n2. Reject\nEnter choice: ";
            int action;
            cin >> action;
            Utils::clearInputBuffer();

            string status = (action == 1) ? "Approved" : "Rejected";
            vector<string> parts = Utils::parseRecord(pendingRequests[requestNum - 1]);
            parts[5] = status;

            ofstream outFile("leave_requests.txt", ios::app);
            outFile << Utils::joinString(parts, "|") << "\n";
            cout << "Request " << status << " successfully!" << endl;
        }
    }

    void displayMenu() override
    {
        int choice;
        do {
            cout << "\n=== PRINCIPAL MENU ===" << endl;
            cout << "1. Handle Requests\n2. Manage Salaries\n3. View School Reports\n" << "4. Approve Leave Requests\n0. Logout\nEnter choice: ";
            cin >> choice;
            Utils::clearInputBuffer();

            switch (choice)
            {
            case 1: 
                handleRequests(); 
                break;
            case 2: 
                manageSalaries(); 
                break;
            case 3: 
                viewSchoolReports(); 
                break;
            case 4: 
                approveLeaveRequests();
                break;
            case 0: 
                cout << "Logging out...\n";
                break;
            default: 
                cout << "Invalid choice!" << endl;
            }
        } while (choice != 0);
    }
};

#endif // MODELS_H