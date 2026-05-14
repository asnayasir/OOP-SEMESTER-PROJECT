#include "AcademicEntity.h"
#include "Assessment.h"
#include "Weightageconfig.h"
#include "Course.h"
#include "Student.h"
#include "Teacher.h"
#include "Venuesection.h"
#include "Databasemanager.h"
#include "Scheduler.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

const int MAX_STUDENTS = 500;
const int MAX_TEACHERS = 100;
const int MAX_COURSES = 100;

Student* students[MAX_STUDENTS];
int      studentCount = 0;

Teacher* teachers[MAX_TEACHERS];
int      teacherCount = 0;

Course* courses[MAX_COURSES];
int      courseCount = 0;

Venue    venues[MAX_VENUES];
int      venueCount = 0;

Section  sections[MAX_SECTIONS];
int      sectionCount = 0;

Scheduler scheduler;

Student* findStudent(const string& id) {
    for (int i = 0; i < studentCount; i++) {
        if (students[i]->getID() == id) return students[i];
    }
    return NULL;
}

Teacher* findTeacher(const string& id) {
    for (int i = 0; i < teacherCount; i++) {
        if (teachers[i]->getID() == id) return teachers[i];
    }
    return NULL;
}

Course* findCourse(const string& id) {
    for (int i = 0; i < courseCount; i++) {
        if (courses[i]->getCourseID() == id) return courses[i];
    }
    return NULL;
}

bool isValidName(const string& s) {
    if (s.length() == 0) return false;
    for (int i = 0; i < (int)s.length(); i++) {
        char c = s[i];
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ')) {
            return false;
        }
    }
    return true;
}

bool isAllDigits(const string& s) {
    if (s.length() == 0) return false;
    for (int i = 0; i < (int)s.length(); i++) {
        if (s[i] < '0' || s[i] > '9') {
            return false;
        }
    }
    return true;
}

bool isValidEmail(const string& s) {
    if (s.length() == 0) return false;
    bool hasAt = false;
    bool hasDot = false;
    int atPos = -1;
    for (int i = 0; i < (int)s.length(); i++) {
        if (s[i] == '@') {
            hasAt = true;
            atPos = i;
        }
        if (s[i] == '.' && atPos != -1 && i > atPos + 1) {
            hasDot = true;
        }
    }
    return (hasAt && hasDot && atPos > 0);
}

int getSafeChoice(int min, int max) {
    string input;
    int choice;
    while (true) {
        cout << "choice: ";
        getline(cin, input);
        if (input.length() == 0) {
            cout << "invalid. enter " << min << "-" << max << "\n";
            continue;
        }
        bool isNumber = true;
        for (int i = 0; i < (int)input.length(); i++) {
            if (input[i] < '0' || input[i] > '9') {
                isNumber = false;
                break;
            }
        }
        if (!isNumber) {
            cout << "invalid. enter " << min << "-" << max << "\n";
            continue;
        }
        choice = 0;
        for (int i = 0; i < (int)input.length(); i++) {
            choice = choice * 10 + (input[i] - '0');
        }
        if (choice < min || choice > max) {
            cout << "invalid. enter " << min << "-" << max << "\n";
        }
        else {
            return choice;
        }
    }
}

string getValidID(const string& prompt) {
    string id;
    while (true) {
        cout << prompt;
        getline(cin, id);
        if (id.length() == 0) {
            cout << "id cannot be empty\n";
            continue;
        }
        bool valid = true;
        for (int i = 0; i < (int)id.length(); i++) {
            char c = id[i];
            if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))) {
                valid = false;
                break;
            }
        }
        if (!valid) {
            cout << "id can only contain letters and numbers\n";
            continue;
        }
        return id;
    }
}

string getValidName(const string& prompt) {
    string name;
    while (true) {
        cout << prompt;
        getline(cin, name);
        if (name.length() == 0) {
            cout << "name cannot be empty\n";
            continue;
        }
        bool valid = true;
        for (int i = 0; i < (int)name.length(); i++) {
            char c = name[i];
            if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ')) {
                valid = false;
                break;
            }
        }
        if (!valid) {
            cout << "name can only contain letters and spaces\n";
            continue;
        }
        return name;
    }
}

string getValidEmail(const string& prompt) {
    string email;
    while (true) {
        cout << prompt;
        getline(cin, email);
        if (email.length() == 0) {
            cout << "email cannot be empty\n";
            continue;
        }
        bool hasAt = false;
        bool hasDot = false;
        int atPos = -1;
        for (int i = 0; i < (int)email.length(); i++) {
            if (email[i] == '@') {
                hasAt = true;
                atPos = i;
            }
            if (email[i] == '.' && atPos != -1 && i > atPos + 1) {
                hasDot = true;
            }
        }
        if (!hasAt || !hasDot || atPos <= 0) {
            cout << "invalid email. must contain @ and . (example: name@domain.com)\n";
            continue;
        }
        return email;
    }
}

int getPositiveInt(const string& prompt) {
    string input;
    int num;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (input.length() == 0) {
            cout << "cannot be empty\n";
            continue;
        }
        bool isNumber = true;
        for (int i = 0; i < (int)input.length(); i++) {
            if (input[i] < '0' || input[i] > '9') {
                isNumber = false;
                break;
            }
        }
        if (!isNumber) {
            cout << "enter a positive number\n";
            continue;
        }
        num = 0;
        for (int i = 0; i < (int)input.length(); i++) {
            num = num * 10 + (input[i] - '0');
        }
        if (num <= 0) {
            cout << "enter a positive number greater than 0\n";
            continue;
        }
        return num;
    }
}

float getPositiveFloat(const string& prompt) {
    string input;
    float num;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (input.length() == 0) {
            cout << "cannot be empty\n";
            continue;
        }
        num = 0.0f;
        float decimal = 0.1f;
        bool afterDecimal = false;
        bool valid = true;
        for (int i = 0; i < (int)input.length(); i++) {
            if (input[i] >= '0' && input[i] <= '9') {
                if (!afterDecimal) {
                    num = num * 10.0f + (input[i] - '0');
                }
                else {
                    num = num + (input[i] - '0') * decimal;
                    decimal = decimal * 0.1f;
                }
            }
            else if (input[i] == '.') {
                if (afterDecimal) {
                    valid = false;
                    break;
                }
                afterDecimal = true;
            }
            else {
                valid = false;
                break;
            }
        }
        if (!valid || num < 0) {
            cout << "enter a valid positive number\n";
            continue;
        }
        return num;
    }
}

void printHeader(const string& title) {
    cout << "\n========================================\n";
    cout << "  " << title << "\n";
    cout << "========================================\n";
}

void printLine() {
    cout << "----------------------------------------\n";
}

void pause() {
    cout << "\npress enter to continue...";
    cin.get();
}
void saveAll() {
    DatabaseManager::saveStudents(students, studentCount, "students.txt");
    DatabaseManager::saveTeachers(teachers, teacherCount, "teachers.txt");
    DatabaseManager::saveCourses(courses, courseCount, "courses.txt");
    DatabaseManager::saveVenues(venues, venueCount, "venues.txt");
    DatabaseManager::saveSections(sections, sectionCount, "sections.txt");
    DatabaseManager::saveAssessments(courses, courseCount, "assessments.txt");
    cout << "all data saved\n";
}

void loadAll() {
    loadWeightages("weightages.txt");
    studentCount = DatabaseManager::loadStudents("students.txt", students, MAX_STUDENTS);
    teacherCount = DatabaseManager::loadTeachers("teachers.txt", teachers, MAX_TEACHERS);
    courseCount = DatabaseManager::loadCourses("courses.txt", courses, MAX_COURSES);
    venueCount = DatabaseManager::loadVenues("venues.txt", venues, MAX_VENUES);
    sectionCount = DatabaseManager::loadSections("sections.txt", sections, MAX_SECTIONS);
    scheduler.setVenues(venues, venueCount);
    DatabaseManager::loadAssessments("assessments.txt", courses, courseCount);
}

void addStudent() {
    printHeader("add student");
    if (studentCount >= MAX_STUDENTS) {
        cout << "max students reached\n";
        return;
    }
    string id = getValidID("enter student id: ");
    if (findStudent(id) != NULL) {
        cout << "id already exists\n";
        return;
    }
    string name = getValidName("enter name: ");
    string email = getValidEmail("enter email: ");
    cout << "student type:\n";
    cout << "1. regular\n";
    cout << "2. scholarship\n";
    cout << "3. exchange\n";
    int choice = getSafeChoice(1, 3);
    Student* s = NULL;
    if (choice == 1) {
        s = new RegularStudent(id, name, email);
    }
    else if (choice == 2) {
        float mg = getPositiveFloat("minimum gpa required: ");
        if (mg > 4) mg = 4;
        s = new ScholarshipStudent(id, name, email, mg);
    }
    else if (choice == 3) {
        string hu;
        cout << "home university: ";
        getline(cin, hu);
        if (hu.length() == 0) hu = "unknown";
        s = new ExchangeStudent(id, name, email, hu);
    }
    if (s != NULL) {
        students[studentCount++] = s;
        cout << "student added\n";
    }
}

void viewAllStudents() {
    printHeader("all students");
    if (studentCount == 0) {
        cout << "no students\n";
        return;
    }
    for (int i = 0; i < studentCount; i++) {
        students[i]->displayProfile();
    }
}

void viewStudentProfile() {
    string id = getValidID("enter student id: ");
    Student* s = findStudent(id);
    if (s == NULL) {
        cout << "student not found\n";
        return;
    }
    s->displayProfile();
    s->viewTranscript();
}

void deleteStudent() {
    string id = getValidID("enter student id to delete: ");
    for (int i = 0; i < studentCount; i++) {
        if (students[i]->getID() == id) {
            delete students[i];
            for (int j = i; j < studentCount - 1; j++) {
                students[j] = students[j + 1];
            }
            studentCount--;
            cout << "student deleted\n";
            return;
        }
    }
    cout << "student not found\n";
}

void studentMenu() {
    int choice;
    do {
        printHeader("student management");
        cout << "1. add student\n";
        cout << "2. view all students\n";
        cout << "3. view student profile\n";
        cout << "4. delete student\n";
        cout << "0. back\n";
        printLine();
        choice = getSafeChoice(0, 4);
        if (choice == 1) addStudent();
        else if (choice == 2) viewAllStudents();
        else if (choice == 3) viewStudentProfile();
        else if (choice == 4) deleteStudent();
        if (choice != 0) pause();
    } while (choice != 0);
}

void addTeacher() {
    printHeader("add teacher");
    if (teacherCount >= MAX_TEACHERS) {
        cout << "max teachers reached\n";
        return;
    }
    string id = getValidID("enter teacher id: ");
    if (findTeacher(id) != NULL) {
        cout << "id already exists\n";
        return;
    }
    string name = getValidName("enter name: ");
    string email = getValidEmail("enter email: ");
    teachers[teacherCount++] = new Teacher(id, name, email);
    cout << "teacher added\n";
}

void viewAllTeachers() {
    printHeader("all teachers");
    if (teacherCount == 0) {
        cout << "no teachers\n";
        return;
    }
    for (int i = 0; i < teacherCount; i++) {
        teachers[i]->displayProfile();
    }
}

void viewTeacherFeedback() {
    string id = getValidID("enter teacher id: ");
    Teacher* t = findTeacher(id);
    if (t == NULL) {
        cout << "teacher not found\n";
        return;
    }
    t->displayFeedback();
}

void deleteTeacher() {
    string id = getValidID("enter teacher id to delete: ");
    for (int i = 0; i < teacherCount; i++) {
        if (teachers[i]->getID() == id) {
            delete teachers[i];
            for (int j = i; j < teacherCount - 1; j++) {
                teachers[j] = teachers[j + 1];
            }
            teacherCount--;
            cout << "teacher deleted\n";
            return;
        }
    }
    cout << "teacher not found\n";
}

void leaveFeedback() {
    printHeader("leave feedback");
    string sid = getValidID("your student id: ");
    Student* s = findStudent(sid);
    if (s == NULL) {
        cout << "student not found\n";
        return;
    }
    string tid = getValidID("teacher id: ");
    Teacher* t = findTeacher(tid);
    if (t == NULL) {
        cout << "teacher not found\n";
        return;
    }
    int rating = getSafeChoice(1, 5);
    string comment;
    cout << "comment: ";
    getline(cin, comment);
    t->addFeedback(sid, rating, comment);
    cout << "feedback submitted\n";
}

void teacherMenu() {
    int choice;
    do {
        printHeader("teacher management");
        cout << "1. add teacher\n";
        cout << "2. view all teachers\n";
        cout << "3. view teacher feedback\n";
        cout << "4. delete teacher\n";
        cout << "5. leave feedback\n";
        cout << "0. back\n";
        printLine();
        choice = getSafeChoice(0, 5);
        if (choice == 1) addTeacher();
        else if (choice == 2) viewAllTeachers();
        else if (choice == 3) viewTeacherFeedback();
        else if (choice == 4) deleteTeacher();
        else if (choice == 5) leaveFeedback();
        if (choice != 0) pause();
    } while (choice != 0);
}

void addCourse() {
    printHeader("add course");
    if (courseCount >= MAX_COURSES) {
        cout << "max courses reached\n";
        return;
    }
    string cid = getValidID("enter course id: ");
    if (findCourse(cid) != NULL) {
        cout << "course id already exists\n";
        return;
    }
    string title;
    cout << "enter title: ";
    getline(cin, title);
    if (title.length() == 0) {
        cout << "title cannot be empty\n";
        return;
    }
    string tid;
    cout << "enter teacher id: ";
    getline(cin, tid);
    int cap = getPositiveInt("enter capacity: ");
    if (cap > MAX_ENROLLED) {
        cout << "Note: Maximum capacity is " << MAX_ENROLLED << ". Setting to " << MAX_ENROLLED << "\n";
        cap = MAX_ENROLLED;
    }
    cout << "course type:\n";
    cout << "1. core\n";
    cout << "2. elective\n";
    cout << "3. lab\n";
    int type = getSafeChoice(1, 3);
    Course* c = NULL;
    if (type == 1) {
        c = new CoreCourse(cid, title, tid, cap);
    }
    else if (type == 2) {
        c = new ElectiveCourse(cid, title, tid, cap);
    }
    else if (type == 3) {
        c = new LabCourse(cid, title, tid, cap);
    }
    if (c != NULL) {
        courses[courseCount++] = c;
        Teacher* t = findTeacher(tid);
        if (t != NULL) {
            t->assignCourse(cid);
        }
        cout << "course added\n";
    }
}

void viewAllCourses() {
    printHeader("all courses");
    if (courseCount == 0) {
        cout << "no courses\n";
        return;
    }
    for (int i = 0; i < courseCount; i++) {
        courses[i]->displayInfo();
    }
}

void deleteCourse() {
    string id = getValidID("enter course id to delete: ");
    for (int i = 0; i < courseCount; i++) {
        if (courses[i]->getCourseID() == id) {
            delete courses[i];
            for (int j = i; j < courseCount - 1; j++) {
                courses[j] = courses[j + 1];
            }
            courseCount--;
            cout << "course deleted\n";
            return;
        }
    }
    cout << "course not found\n";
}

void courseMenu() {
    int choice;
    do {
        printHeader("course management");
        cout << "1. add course\n";
        cout << "2. view all courses\n";
        cout << "3. delete course\n";
        cout << "0. back\n";
        printLine();
        choice = getSafeChoice(0, 3);
        if (choice == 1) addCourse();
        else if (choice == 2) viewAllCourses();
        else if (choice == 3) deleteCourse();
        if (choice != 0) pause();
    } while (choice != 0);
}

void registerStudent() {
    printHeader("register student");
    string sid = getValidID("student id: ");
    Student* s = findStudent(sid);
    if (s == NULL) {
        cout << "student not found\n";
        return;
    }
    string cid = getValidID("course id: ");
    Course* c = findCourse(cid);
    if (c == NULL) {
        cout << "course not found\n";
        return;
    }
    if (c->getEnrolled() >= c->getCapacity()) {
        cout << "course is full\n";
        return;
    }
    if (c->isEnrolled(sid)) {
        cout << "student already enrolled\n";
        return;
    }
    c->enroll(sid);
    s->addCourse(cid);
    cout << "student enrolled\n";
}

void dropCourse() {
    printHeader("drop course");
    string sid = getValidID("student id: ");
    Student* s = findStudent(sid);
    if (s == NULL) {
        cout << "student not found\n";
        return;
    }
    string cid = getValidID("course id: ");
    Course* c = findCourse(cid);
    if (c == NULL) {
        cout << "course not found\n";
        return;
    }
    c->drop(sid);
    s->dropCourse(cid);
    cout << "course dropped\n";
}

void viewRoster() {
    string cid = getValidID("course id: ");
    Course* c = findCourse(cid);
    if (c == NULL) {
        cout << "course not found\n";
        return;
    }
    c->displayInfo();
    cout << "enrolled students:\n";
    for (int i = 0; i < c->getEnrolled(); i++) {
        Student* s = findStudent(c->getEnrolledID(i));
        if (s != NULL) {
            cout << "  - " << s->getID() << " " << s->getName() << "\n";
        }
    }
}

void registrationMenu() {
    int choice;
    do {
        printHeader("registration");
        cout << "1. register student\n";
        cout << "2. drop course\n";
        cout << "3. view course roster\n";
        cout << "0. back\n";
        printLine();
        choice = getSafeChoice(0, 3);
        if (choice == 1) registerStudent();
        else if (choice == 2) dropCourse();
        else if (choice == 3) viewRoster();
        if (choice != 0) pause();
    } while (choice != 0);
}

void enterMarks() {
    printHeader("enter marks");
    string tid = getValidID("teacher id: ");
    Teacher* t = findTeacher(tid);
    if (t == NULL) {
        cout << "teacher not found\n";
        return;
    }
    string cid = getValidID("course id: ");
    Course* c = findCourse(cid);
    if (c == NULL) {
        cout << "course not found\n";
        return;
    }
    if (c->getTeacherID() != tid) {
        cout << "you are not the teacher for this course\n";
        return;
    }
    cout << "assessment type:\n";
    cout << "1. exam\n";
    cout << "2. quiz\n";
    cout << "3. assignment\n";
    int atype = getSafeChoice(1, 3);
    float raw = getPositiveFloat("raw score: ");
    float max = getPositiveFloat("max score: ");
    float examWt, assignWt, quizWt;
    getWeightage(c->getCourseType(), examWt, assignWt, quizWt);

    float wt = 0.0f;
    if (atype == 1) wt = examWt;
    else if (atype == 2) wt = quizWt;
    else wt = assignWt;
    Assessment* a = NULL;
    if (atype == 1) {
        a = new Exam(raw, max, wt);
    }
    else if (atype == 2) {
        a = new Quiz(raw, max, wt);
    }
    else {
        a = new Assignment(raw, max, wt);
    }
    if (a != NULL) {
        c->addAssessment(a);
    }
    float finalPct = c->calculateFinalGrade("");
    string grade = "";
    if (finalPct >= 90) grade = "A+";
    else if (finalPct >= 85) grade = "A";
    else if (finalPct >= 80) grade = "A-";
    else if (finalPct >= 75) grade = "B+";
    else if (finalPct >= 70) grade = "B";
    else if (finalPct >= 65) grade = "B-";
    else if (finalPct >= 60) grade = "C+";
    else if (finalPct >= 55) grade = "C";
    else if (finalPct >= 50) grade = "D";
    else grade = "F";
    for (int i = 0; i < studentCount; i++) {
        if (c->isEnrolled(students[i]->getID())) {
            if (students[i]->getStudentType() == "Exchange") {
                string pf = (finalPct >= 50) ? "Pass" : "Fail";
                students[i]->updateTranscript(cid, c->getTitle(), finalPct, pf, pf);
            }
            else {
                students[i]->updateTranscript(cid, c->getTitle(), finalPct, grade, "");
            }
        }
    }
    cout << "marks added. final grade: " << finalPct << "% (" << grade << ")\n";
}

void viewCourseAssessments() {
    string cid = getValidID("course id: ");
    Course* c = findCourse(cid);
    if (c == NULL) {
        cout << "course not found\n";
        return;
    }
    c->displayInfo();
    for (int i = 0; i < c->getAssessmentCount(); i++) {
        if (c->getAssessment(i) != NULL) {
            c->getAssessment(i)->display();
        }
    }
    cout << "final grade: " << c->calculateFinalGrade("") << "%\n";
}
void showWeightages() {
    printHeader("current grading weightages");
    float exam, assign, quiz;

    getWeightage("Core", exam, assign, quiz);
    cout << "Core Course:    Exam: " << exam * 100 << "%  |  Assignment: " << assign * 100 << "%  |  Quiz: " << quiz * 100 << "%\n";

    getWeightage("Elective", exam, assign, quiz);
    cout << "Elective Course: Exam: " << exam * 100 << "%  |  Assignment: " << assign * 100 << "%  |  Quiz: " << quiz * 100 << "%\n";

    getWeightage("Lab", exam, assign, quiz);
    cout << "Lab Course:     Exam: " << exam * 100 << "%  |  Assignment: " << assign * 100 << "%  |  Quiz: " << quiz * 100 << "%\n";

    cout << "\n(To change these values, edit 'weightages.txt' and restart the program.)\n";
}

void gradingMenu() {
    int choice;
    do {
        printHeader("grading");
        cout << "1. enter marks\n";
        cout << "2. view assessments\n";
        cout << "3. view current weightages\n";
        cout << "0. back\n";
        printLine();
        choice = getSafeChoice(0, 3);
        if (choice == 1) enterMarks();
        else if (choice == 2) viewCourseAssessments();
        else if (choice == 3) showWeightages();
        if (choice != 0) pause();
    } while (choice != 0);
}

void addVenue() {
    printHeader("add venue");
    if (venueCount >= MAX_VENUES) {
        cout << "max venues reached\n";
        return;
    }
    string rid;
    cout << "room id: ";
    getline(cin, rid);
    int cap = getPositiveInt("capacity: ");
    int hc = getSafeChoice(0, 1);
    venues[venueCount++] = Venue(rid, cap, hc == 1);
    scheduler.setVenues(venues, venueCount);
    cout << "venue added\n";
}

void viewAllVenues() {
    printHeader("all venues");
    if (venueCount == 0) {
        cout << "no venues\n";
        return;
    }
    for (int i = 0; i < venueCount; i++) {
        venues[i].display();
    }
}

void addSection() {
    printHeader("add section");
    if (sectionCount >= MAX_SECTIONS) {
        cout << "max sections reached\n";
        return;
    }
    string sid;
    cout << "section id: ";
    getline(cin, sid);
    string cid;
    cout << "course id: ";
    getline(cin, cid);
    string tid;
    cout << "teacher id: ";
    getline(cin, tid);
    string vid;
    cout << "venue id: ";
    getline(cin, vid);
    string ts;
    cout << "time slot: ";
    getline(cin, ts);
    sections[sectionCount++] = Section(sid, cid, tid, vid, ts);
    cout << "section added\n";
}

void viewAllSections() {
    printHeader("all sections");
    if (sectionCount == 0) {
        cout << "no sections\n";
        return;
    }
    for (int i = 0; i < sectionCount; i++) {
        sections[i].display();
    }
}

void venueMenu() {
    int choice;
    do {
        printHeader("venue and section");
        cout << "1. add venue\n";
        cout << "2. view all venues\n";
        cout << "3. add section\n";
        cout << "4. view all sections\n";
        cout << "0. back\n";
        printLine();
        choice = getSafeChoice(0, 4);
        if (choice == 1) addVenue();
        else if (choice == 2) viewAllVenues();
        else if (choice == 3) addSection();
        else if (choice == 4) viewAllSections();
        if (choice != 0) pause();
    } while (choice != 0);
}

void schedulerMenu() {
    int choice;
    do {
        printHeader("exam scheduler");
        cout << "1. auto-schedule all exams\n";
        cout << "2. view schedule\n";
        cout << "3. check conflicts\n";
        cout << "4. save schedule\n";
        cout << "0. back\n";
        printLine();
        choice = getSafeChoice(0, 4);
        if (choice == 1) {
            for (int i = 0; i < courseCount; i++) {
                if (courses[i] != NULL) {
                    scheduler.scheduleCourse(courses[i]);
                }
            }
        }
        else if (choice == 2) scheduler.showSchedule();
        else if (choice == 3) scheduler.findConflicts();
        else if (choice == 4) scheduler.saveSchedule("exam_schedule.txt");
        if (choice != 0) pause();
    } while (choice != 0);
}

void mainMenu() {
    int choice;
    do {
        printHeader("fast academic office");
        cout << "1. student management\n";
        cout << "2. teacher management\n";
        cout << "3. course management\n";
        cout << "4. registration\n";
        cout << "5. grading\n";
        cout << "6. venue and section\n";
        cout << "7. exam scheduler\n";
        cout << "8. save all data\n";
        cout << "0. exit\n";
        printLine();
        choice = getSafeChoice(0, 8);
        if (choice == 1) studentMenu();
        else if (choice == 2) teacherMenu();
        else if (choice == 3) courseMenu();
        else if (choice == 4) registrationMenu();
        else if (choice == 5) gradingMenu();
        else if (choice == 6) venueMenu();
        else if (choice == 7) schedulerMenu();
        else if (choice == 8) {
            saveAll();
            pause();
        }
        else if (choice == 0) {
            saveAll();
            cout << "goodbye\n";
        }
        if (choice != 0 && choice != 8) pause();
    } while (choice != 0);
}

int main() {
    cout << "\n========================================\n";
    cout << "  fast academic office system\n";
    cout << "========================================\n\n";
    ifstream test("weightages.txt");
    if (!test.is_open()) {
        saveDefaultWeightages("weightages.txt");
    }
    test.close();
    loadAll();
    mainMenu();
    for (int i = 0; i < studentCount; i++) {
        delete students[i];
    }
    for (int i = 0; i < teacherCount; i++) {
        delete teachers[i];
    }
    for (int i = 0; i < courseCount; i++) {
        delete courses[i];
    }
    return 0;
}