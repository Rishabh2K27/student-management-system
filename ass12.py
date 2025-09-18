from pymongo import MongoClient

class StudentManagementSystem:
    def __init__(self):
        self.client = MongoClient("mongodb://localhost:27017/")
        self.db = self.client["school"]
        self.students = self.db["students"]

    def add_student(self):
        name = input("Enter Name: ")
        rollNo = input("Enter Roll No: ")
        branch = input("Enter Branch: ")
        course = input("Enter Course: ")
        email = input("Enter Email: ")

        student = {
            "name": name,
            "rollNo": rollNo,
            "branch": branch,
            "course": course,
            "email": email
        }

        self.students.insert_one(student)
        print("Student added successfully!")

    def display_students(self):
        for student in self.students.find():
            print(student)

    def search_student(self):
        rollNo = input("Enter Roll No to search: ")
        student = self.students.find_one({"rollNo": rollNo})
        if student:
            print(student)
        else:
            print("Student not found.")

    def edit_student(self):
        rollNo = input("Enter Roll No to edit: ")
        student = self.students.find_one({"rollNo": rollNo})
        if not student:
            print("Student not found.")
            return

        name = input("Enter new Name: ")
        branch = input("Enter new Branch: ")
        course = input("Enter new Course: ")
        email = input("Enter new Email: ")

        update = {
            "$set": {
                "name": name,
                "branch": branch,
                "course": course,
                "email": email
            }
        }

        self.students.update_one({"rollNo": rollNo}, update)
        print("Student record updated.")

    def delete_student(self):
        rollNo = input("Enter Roll No to delete: ")
        result = self.students.delete_one({"rollNo": rollNo})
        if result.deleted_count > 0:
            print("Student record deleted.")
        else:
            print("Student not found.")

    def main_menu(self):
        while True:
            print("\n1. Add Student")
            print("2. Display Students")
            print("3. Search Student")
            print("4. Edit Student")
            print("5. Delete Student")
            print("6. Exit")
            choice = input("Enter choice: ")

            if choice == "1":
                self.add_student()
            elif choice == "2":
                self.display_students()
            elif choice == "3":
                self.search_student()
            elif choice == "4":
                self.edit_student()
            elif choice == "5":
                self.delete_student()
            elif choice == "6":
                print("Exiting...")
                break
            else:
                print("Invalid choice, try again.")

if __name__ == "__main__":
    sms = StudentManagementSystem()
    sms.main_menu()

