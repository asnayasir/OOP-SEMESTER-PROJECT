#pragma once
#include "Student.h"
#include "Teacher.h"
#include "Course.h"
#include "VenueSection.h"
#include <fstream>
#include <iostream>
using namespace std;

class DatabaseManager {
public:
    // students
    static void saveStudents(Student** students, int count, const string& filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "error: cannot save students\n";
            return;
        }

        for (int i = 0; i < count; i++) {
            Student* s = students[i];
            file << s->getID() << "|"
                << s->getName() << "|"
                << s->getEmail() << "|"
                << s->getStudentType() << "\n";
        }

        file.close();
        cout << "saved " << count << " students\n";
    }

    static int loadStudents(const string& filename, Student** students, int maxCount) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "no student file found\n";
            return 0;
        }

        int count = 0;
        string line;

        while (getline(file, line) && count < maxCount) {
            if (line.length() == 0) continue;

            string id = "";
            string name = "";
            string email = "";
            string type = "";

            int part = 0;
            for (int i = 0; i < (int)line.length(); i++) {
                if (line[i] == '|') {
                    part++;
                }
                else {
                    if (part == 0) id += line[i];
                    else if (part == 1) name += line[i];
                    else if (part == 2) email += line[i];
                    else if (part == 3) type += line[i];
                }
            }

            Student* s = NULL;
            if (type == "Regular") {
                s = new RegularStudent(id, name, email);
            }
            else if (type == "Scholarship") {
                s = new ScholarshipStudent(id, name, email);
            }
            else if (type == "Exchange") {
                s = new ExchangeStudent(id, name, email);
            }

            if (s != NULL) {
                students[count++] = s;
            }
        }

        file.close();
        cout << "loaded " << count << " students\n";
        return count;
    }

    // teachers
    static void saveTeachers(Teacher** teachers, int count, const string& filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "error: cannot save teachers\n";
            return;
        }

        for (int i = 0; i < count; i++) {
            Teacher* t = teachers[i];
            file << t->getID() << "|"
                << t->getName() << "|"
                << t->getEmail() << "\n";
        }

        file.close();
        cout << "saved " << count << " teachers\n";
    }

    static int loadTeachers(const string& filename, Teacher** teachers, int maxCount) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "no teacher file found\n";
            return 0;
        }

        int count = 0;
        string line;

        while (getline(file, line) && count < maxCount) {
            if (line.length() == 0) continue;

            string id = "";
            string name = "";
            string email = "";

            int part = 0;
            for (int i = 0; i < (int)line.length(); i++) {
                if (line[i] == '|') {
                    part++;
                }
                else {
                    if (part == 0) id += line[i];
                    else if (part == 1) name += line[i];
                    else if (part == 2) email += line[i];
                }
            }

            teachers[count++] = new Teacher(id, name, email);
        }

        file.close();
        cout << "loaded " << count << " teachers\n";
        return count;
    }

    // courses
    static void saveCourses(Course** courses, int count, const string& filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "error: cannot save courses\n";
            return;
        }

        for (int i = 0; i < count; i++) {
            Course* c = courses[i];
            file << c->getCourseID() << "|"
                << c->getTitle() << "|"
                << c->getTeacherID() << "|"
                << c->getCourseType() << "|"
                << c->getCapacity() << "\n";
        }

        file.close();
        cout << "saved " << count << " courses\n";
    }

    static int loadCourses(const string& filename, Course** courses, int maxCount) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "no course file found\n";
            return 0;
        }

        int count = 0;
        string line;

        while (getline(file, line) && count < maxCount) {
            if (line.length() == 0) continue;

            string id = "";
            string title = "";
            string teacherId = "";
            string type = "";
            string capStr = "";

            int part = 0;
            for (int i = 0; i < (int)line.length(); i++) {
                if (line[i] == '|') {
                    part++;
                }
                else {
                    if (part == 0) id += line[i];
                    else if (part == 1) title += line[i];
                    else if (part == 2) teacherId += line[i];
                    else if (part == 3) type += line[i];
                    else if (part == 4) capStr += line[i];
                }
            }

            // convert capacity to number
            int cap = 0;
            for (int i = 0; i < (int)capStr.length(); i++) {
                cap = cap * 10 + (capStr[i] - '0');
            }
            if (cap <= 0) cap = 50;

            Course* c = NULL;
            if (type == "Core") {
                c = new CoreCourse(id, title, teacherId, cap);
            }
            else if (type == "Elective") {
                c = new ElectiveCourse(id, title, teacherId, cap);
            }
            else if (type == "Lab") {
                c = new LabCourse(id, title, teacherId, cap);
            }

            if (c != NULL) {
                courses[count++] = c;
            }
        }

        file.close();
        cout << "loaded " << count << " courses\n";
        return count;
    }
};