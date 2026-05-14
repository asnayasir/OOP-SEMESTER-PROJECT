#pragma once
#include "Student.h"
#include "Teacher.h"
#include "Course.h"
#include "Venuesection.h"
#include <fstream>
#include <iostream>
#include <sstream>
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
            string id, name, email, type;
            int part = 0;
            for (char c : line) {
                if (c == '|') part++;
                else {
                    if (part == 0) id += c;
                    else if (part == 1) name += c;
                    else if (part == 2) email += c;
                    else if (part == 3) type += c;
                }
            }
            Student* s = NULL;
            if (type == "Regular") s = new RegularStudent(id, name, email);
            else if (type == "Scholarship") s = new ScholarshipStudent(id, name, email);
            else if (type == "Exchange") s = new ExchangeStudent(id, name, email);
            if (s) students[count++] = s;
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
            if (line.empty()) continue;
            string id, name, email;
            int part = 0;
            for (char c : line) {
                if (c == '|') part++;
                else {
                    if (part == 0) id += c;
                    else if (part == 1) name += c;
                    else if (part == 2) email += c;
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
            if (line.empty()) continue;
            string id, title, teacherId, type, capStr;
            int part = 0;
            for (char c : line) {
                if (c == '|') part++;
                else {
                    if (part == 0) id += c;
                    else if (part == 1) title += c;
                    else if (part == 2) teacherId += c;
                    else if (part == 3) type += c;
                    else if (part == 4) capStr += c;
                }
            }
            int cap = 0;
            for (char c : capStr) cap = cap * 10 + (c - '0');
            if (cap <= 0) cap = 50;
            Course* c = NULL;
            if (type == "Core") c = new CoreCourse(id, title, teacherId, cap);
            else if (type == "Elective") c = new ElectiveCourse(id, title, teacherId, cap);
            else if (type == "Lab") c = new LabCourse(id, title, teacherId, cap);
            if (c) courses[count++] = c;
        }
        file.close();
        cout << "loaded " << count << " courses\n";
        return count;
    }

    // Venues
    static void saveVenues(Venue* venues, int count, const string& filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "error: cannot save venues\n";
            return;
        }
        for (int i = 0; i < count; i++) {
            file << venues[i].getRoomID() << "|"
                << venues[i].getCapacity() << "|"
                << (venues[i].getHasComputers() ? 1 : 0) << "\n";
        }
        file.close();
        cout << "saved " << count << " venues\n";
    }

    static int loadVenues(const string& filename, Venue* venues, int maxCount) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "no venue file found\n";
            return 0;
        }
        int count = 0;
        string line;
        while (getline(file, line) && count < maxCount) {
            if (line.empty()) continue;
            string rid, capStr, compStr;
            int part = 0;
            for (char c : line) {
                if (c == '|') part++;
                else {
                    if (part == 0) rid += c;
                    else if (part == 1) capStr += c;
                    else if (part == 2) compStr += c;
                }
            }
            int cap = 0;
            for (char c : capStr) cap = cap * 10 + (c - '0');
            bool hasComp = (compStr == "1");
            venues[count++] = Venue(rid, cap, hasComp);
        }
        file.close();
        cout << "loaded " << count << " venues\n";
        return count;
    }

    // Sections
    static void saveSections(Section* sections, int count, const string& filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "error: cannot save sections\n";
            return;
        }
        for (int i = 0; i < count; i++) {
            file << sections[i].getSectionID() << "|"
                << sections[i].getCourseID() << "|"
                << sections[i].getTeacherID() << "|"
                << sections[i].getVenueID() << "|"
                << sections[i].getTimeSlot() << "\n";
        }
        file.close();
        cout << "saved " << count << " sections\n";
    }

    static int loadSections(const string& filename, Section* sections, int maxCount) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "no section file found\n";
            return 0;
        }
        int count = 0;
        string line;
        while (getline(file, line) && count < maxCount) {
            if (line.empty()) continue;
            string sid, cid, tid, vid, ts;
            int part = 0;
            for (char c : line) {
                if (c == '|') part++;
                else {
                    if (part == 0) sid += c;
                    else if (part == 1) cid += c;
                    else if (part == 2) tid += c;
                    else if (part == 3) vid += c;
                    else if (part == 4) ts += c;
                }
            }
            sections[count++] = Section(sid, cid, tid, vid, ts);
        }
        file.close();
        cout << "loaded " << count << " sections\n";
        return count;
    }

   
    static void saveAssessments(Course** courses, int courseCount, const string& filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "error: cannot save assessments\n";
            return;
        }
        for (int i = 0; i < courseCount; i++) {
            Course* c = courses[i];
            for (int j = 0; j < c->getAssessmentCount(); j++) {
                Assessment* a = c->getAssessment(j);
                if (a) {
                    file << c->getCourseID() << "|"
                        << a->getType() << "|"
                        << a->getRaw() << "|"
                        << a->getMax() << "|"
                        << a->getWeightage() << "\n";
                }
            }
        }
        file.close();
        cout << "saved assessments\n";
    }

    static void loadAssessments(const string& filename, Course** courses, int courseCount) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "no assessment file found\n";
            return;
        }
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            string cid, type, rawStr, maxStr, wtStr;
            int part = 0;
            for (char c : line) {
                if (c == '|') part++;
                else {
                    if (part == 0) cid += c;
                    else if (part == 1) type += c;
                    else if (part == 2) rawStr += c;
                    else if (part == 3) maxStr += c;
                    else if (part == 4) wtStr += c;
                }
            }
            // find course
            Course* target = NULL;
            for (int i = 0; i < courseCount; i++) {
                if (courses[i]->getCourseID() == cid) {
                    target = courses[i];
                    break;
                }
            }
            if (!target) continue;
            float raw = 0, max = 0, wt = 0;
            for (char c : rawStr) raw = raw * 10 + (c - '0');
            for (char c : maxStr) max = max * 10 + (c - '0');
            for (char c : wtStr) wt = wt * 10 + (c - '0');
            wt = wt / 100.0f; 
            Assessment* a = NULL;
            if (type == "Exam") a = new Exam(raw, max, wt);
            else if (type == "Quiz") a = new Quiz(raw, max, wt);
            else if (type == "Assignment") a = new Assignment(raw, max, wt);
            if (a) target->addAssessment(a);
        }
        file.close();
        cout << "loaded assessments\n";
    }
};