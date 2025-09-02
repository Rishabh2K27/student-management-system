#include<bits/stdc++.h>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
using namespace std;

// Text color functions
void Red ()          { cout<<"\033[1;31m"; }
void Yellow()        { cout<<"\033[1;33m"; }
void Blue()          { cout<<"\033[0;34m"; }
void Green()         { cout<<"\033[0;32m"; }
void Purple()        { cout<<"\033[0;35m"; }
void White()         { cout<<"\033[0;37m"; }
void Cyan()          { cout<<"\033[0;36m"; }
void ResetTextColor(){ cout<<"\033[0m"; }

struct studentData {
    string name, rollNo, branch, course, emailId;
    void printStudentData() {
        Purple();
        cout << "\nStudent Name    : " << name << endl;
        cout << "Student Roll No : " << rollNo << endl;
        cout << "Student Branch  : " << branch << endl;
        cout << "Student Course  : " << course << endl;
        cout << "Student Email   : " << emailId << endl;
        cout << "----------------------------------" << endl;
        ResetTextColor();
    }
};

class studentManagementSystem {
private:
    sql::Driver *driver;
    sql::Connection *con;
public:
    studentManagementSystem() {
        driver = get_driver_instance();
        con = driver->connect("tcp://192.168.189.24:3306", "admin1", "ait123");
        con->setSchema("school");
    }

    ~studentManagementSystem() {
        delete con;
    }

    void mainMenu();
    void addStudentData();
    void displayStudentRecords();
    void searchStudentUsingRollNo();
    void editStudentRecord();
    void deleteStudentRecord();
};


void studentManagementSystem::mainMenu() {
    int choice;
    while (true) {
        Yellow();
        cout << "-------------------------------------" << endl;
        cout << "1. Add Student" << endl;
        cout << "2. Display All Students" << endl;
        cout << "3. Search Student by Roll No" << endl;
        cout << "4. Edit Student Record" << endl;
        cout << "5. Delete Student Record" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        ResetTextColor();
        cin.ignore();
        system("clear");
        switch (choice) {
            case 1: addStudentData(); break;
            case 2: displayStudentRecords(); break;
            case 3: searchStudentUsingRollNo(); break;
            case 4: editStudentRecord(); break;
            case 5: deleteStudentRecord(); break;
            case 6: exit(0);
            default: cout << "Invalid choice. Try again.\n";
        }
    }
}

void studentManagementSystem::addStudentData() {
    studentData s;
    cout << "Enter Name    : "; getline(cin, s.name);
    cout << "Enter Roll No : "; getline(cin, s.rollNo);
    cout << "Enter Branch  : "; getline(cin, s.branch);
    cout << "Enter Course  : "; getline(cin, s.course);
    cout << "Enter Email   : "; getline(cin, s.emailId);

    sql::PreparedStatement *pstmt = con->prepareStatement(
        "INSERT INTO students(name, rollNo, branch, course, emailId) VALUES (?, ?, ?, ?, ?)");
    pstmt->setString(1, s.name);
    pstmt->setString(2, s.rollNo);
    pstmt->setString(3, s.branch);
    pstmt->setString(4, s.course);
    pstmt->setString(5, s.emailId);
    pstmt->execute();
    delete pstmt;

    Green();
    cout << "Student added successfully!\n";
    ResetTextColor();
}

void studentManagementSystem::displayStudentRecords() {
    sql::Statement *stmt = con->createStatement();
    sql::ResultSet *res = stmt->executeQuery("SELECT * FROM students");

    int count = 0;
    while (res->next()) {
        studentData s;
        s.name = res->getString("name");
        s.rollNo = res->getString("rollNo");
        s.branch = res->getString("branch");
        s.course = res->getString("course");
        s.emailId = res->getString("emailId");
        s.printStudentData();
        count++;
    }

    if (count == 0) {
        Red();
        cout << "No student records found.\n";
        ResetTextColor();
    }

    delete res;
    delete stmt;
}

void studentManagementSystem::searchStudentUsingRollNo() {
    string rollNo;
    cout << "Enter Roll No to Search: ";
    getline(cin, rollNo);

    sql::PreparedStatement *pstmt = con->prepareStatement(
        "SELECT * FROM students WHERE rollNo = ?");
    pstmt->setString(1, rollNo);
    sql::ResultSet *res = pstmt->executeQuery();

    if (res->next()) {
        studentData s;
        s.name = res->getString("name");
        s.rollNo = res->getString("rollNo");
        s.branch = res->getString("branch");
        s.course = res->getString("course");
        s.emailId = res->getString("emailId");
        s.printStudentData();
    } else {
        Red();
        cout << "Student with Roll No " << rollNo << " not found.\n";
        ResetTextColor();
    }

    delete res;
    delete pstmt;
}

void studentManagementSystem::editStudentRecord() {
    string rollNo;
    cout << "Enter Roll No to Edit: ";
    getline(cin, rollNo);

    sql::PreparedStatement *check = con->prepareStatement(
        "SELECT * FROM students WHERE rollNo = ?");
    check->setString(1, rollNo);
    sql::ResultSet *res = check->executeQuery();

    if (res->next()) {
        studentData s;
        cout << "Enter New Name    : "; getline(cin, s.name);
        cout << "Enter New Branch  : "; getline(cin, s.branch);
        cout << "Enter New Course  : "; getline(cin, s.course);
        cout << "Enter New Email   : "; getline(cin, s.emailId);

        sql::PreparedStatement *update = con->prepareStatement(
            "UPDATE students SET name=?, branch=?, course=?, emailId=? WHERE rollNo=?");
        update->setString(1, s.name);
        update->setString(2, s.branch);
        update->setString(3, s.course);
        update->setString(4, s.emailId);
        update->setString(5, rollNo);
        update->execute();

        Green();
        cout << "Record updated successfully!\n";
        ResetTextColor();

        delete update;
    } else {
        Red();
        cout << "Student not found.\n";
        ResetTextColor();
    }

    delete res;
    delete check;
}

void studentManagementSystem::deleteStudentRecord() {
    string rollNo;
    cout << "Enter Roll No to Delete: ";
    getline(cin, rollNo);

    sql::PreparedStatement *del = con->prepareStatement(
        "DELETE FROM students WHERE rollNo = ?");
    del->setString(1, rollNo);
    int rows = del->executeUpdate();

    if (rows > 0) {
        Green();
        cout << "Record deleted successfully.\n";
    } else {
        Red();
        cout << "No student found with Roll No: " << rollNo << endl;
    }

    ResetTextColor();
    delete del;
}

int main() {
    studentManagementSystem s1;
    s1.mainMenu();
    return 0;
}

