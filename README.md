# Advanced Learning Management System (LMS)

A console-based **Learning Management System (LMS)** built in C++, offering role-based access for Admins, Teachers, Students, Parents, and the Principal. It allows seamless management of students, attendance, grades, fee processing, staff records, and academic reporting.

---

## 💡 Features

### 👩‍💼 Admin
- Manage students, teachers, and staff
- Process payments (student fees, salaries)
- Generate fee challans
- Handle leave requests
- Promote students yearly

### 👩‍🏫 Teacher
- Mark attendance
- Enter and view grades
- Generate term reports
- Apply for leave
- View timetable

### 🎓 Student
- View timetable
- View assignments
- Check grades
- Review attendance

### 👪 Parent
- Monitor child’s academic progress
- View fee status and attendance
- Submit concerns/requests

### 🧑‍💼 Principal
- Handle leave and parent requests
- Manage staff salaries
- View overall reports

---

## 🔐 How to Login

When the program starts, you’ll be prompted to:

1. Select a login role (Admin, Teacher, etc.)
2. Enter a username
3. Enter a password

> The system verifies the username and password by checking `.txt` records. Make sure your credentials match an entry in the corresponding file.

### ✅ Example Logins:

| Role       | Example Username | Example Password           |
|------------|------------------|----------------------------|
| Admin      | `admin1`         | `admin123`                 |
| Teacher    | `Ayesha`         | `pass123`                  |
| Student    | `Ali`            | `03001234567` *(see note)* |
| Parent     | `Ali_parent`     | `xyz123`                   |
| Principal  | `principal1`     | `pass456`                  |

**ℹ️ Note for Students:**  
> The **student password is the parent contact number** (as stored in the `students.txt` file under the "Parent Contact" field).

---

## 🛠️ How to Compile and Run

Make sure you have a C++ compiler installed (e.g., `g++`).

### Compile:
```bash
g++ main.cpp -o lms
```

### Run:

```bash
./lms
```

## 🗂️ File Structure

| File Name       | Purpose                                 |
|-----------------|------------------------------------------|
| `main.cpp`      | Program entry point and role menus       |
| `models.h`      | Role and data structure definitions      |
| `utils.h`       | Utility functions (parsing, date, etc.)  |
| `*.txt` files   | Persistent data for each module          |

---

## 📝 Data Files Used

The program automatically initializes and uses the following text files:

- `students.txt`  
- `teachers.txt`  
- `staff.txt`  
- `attendance.txt`  
- `grades.txt`  
- `timetable.txt`  
- `fee_challans.txt`  
- `salary_payments.txt`  
- `assignments_due.txt`  
- `termReports.txt`  
- `parent_requests.txt`  
- `leave_requests.txt`  

Each file uses a simple `|`-delimited format and includes a section header (e.g., `[STUDENT RECORD]`) for easy parsing.

## 📬 Contact

For questions, suggestions, or contributions, feel free to reach out:

**Email:** 

[abeerashraf158@gmail.com](mailto:abeerashraf158@gmail.com)
