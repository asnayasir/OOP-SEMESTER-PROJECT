#pragma once
#include "Assessment.h"
#include "Weightageconfig.h"
#include <string>
#include <iostream>
using namespace std;

const int MAX_ASSESSMENTS = 50;
const int MAX_ENROLLED = 100;

// abstract course

class Course {
protected:
    string courseID;
    string title;
    string teacherID;
    string courseType;

    Assessment* assessments[MAX_ASSESSMENTS];
    int assessmentCount;

    string enrolledIDs[MAX_ENROLLED];
    int enrolledCount;
    int capacity;

public:
    Course() : assessmentCount(0), enrolledCount(0), capacity(50) {}

    Course(const string& cid, const string& t, const string& tid, int cap = 50)
        : assessmentCount(0), enrolledCount(0) {

        if (cid.empty()) {
            courseID = "UNKNOWN";
            cout << "Warning: Empty course ID. Set to UNKNOWN\n";
        }
        else {
            courseID = cid;
        }

        if (t.empty()) {
            title = "No Title";
            cout << "Warning: Empty course title for " << courseID << endl;
        }
        else {
            title = t;
        }

        if (tid.empty()) {
            teacherID = "NO_TEACHER";
            cout << "Warning: Empty teacher ID for " << courseID << endl;
        }
        else {
            teacherID = tid;
        }

        if (cap <= 0) {
            capacity = 50;
            cout << "Warning: Invalid capacity " << cap << ". Set to 50\n";
        }
        else if (cap > MAX_ENROLLED) {
            capacity = MAX_ENROLLED;
            cout << "Warning: Capacity " << cap << " exceeds maximum allowed (" << MAX_ENROLLED << "). Set to " << MAX_ENROLLED << "\n";
        }
        else {
            capacity = cap;
        }
    }

    virtual ~Course() {
        for (int i = 0; i < assessmentCount; i++) {
            delete assessments[i];
        }
    }

    string getCourseID()   const { return courseID; }
    string getTitle()      const { return title; }
    string getTeacherID()  const { return teacherID; }
    string getCourseType() const { return courseType; }
    int    getCapacity()   const { return capacity; }
    int    getEnrolled()   const { return enrolledCount; }

    void setCourseID(const string& id) {
        courseID = id.empty() ? "UNKNOWN" : id;
    }

    void setTitle(const string& t) {
        title = t.empty() ? "No Title" : t;
    }

    void setTeacherID(const string& t) {
        teacherID = t.empty() ? "NO_TEACHER" : t;
    }

    void setCapacity(int c) {
        if (c <= 0) {
            capacity = 50;
            cout << "Warning: Invalid capacity. Set to 50\n";
        }
        else if (c > MAX_ENROLLED) {
            capacity = MAX_ENROLLED;
            cout << "Warning: Capacity reduced to " << MAX_ENROLLED << "\n";
        }
        else {
            capacity = c;
        }
        if (enrolledCount > capacity) {
            cout << "Warning: Enrolled students (" << enrolledCount
                << ") exceed new capacity (" << capacity << ")\n";
        }
    }

    bool isEnrolled(const string& sid) const {
        if (sid.empty()) return false;
        for (int i = 0; i < enrolledCount; i++) {
            if (enrolledIDs[i] == sid) return true;
        }
        return false;
    }

    bool enroll(const string& sid) {
        if (sid.empty()) {
            cout << "Error: Cannot enroll with empty student ID\n";
            return false;
        }
        if (enrolledCount >= capacity) {
            cout << "Error: Course " << courseID << " is full (Capacity: " << capacity << ")\n";
            return false;
        }
        if (enrolledCount >= MAX_ENROLLED) {
            cout << "Error: Internal array limit reached. Cannot enroll more students.\n";
            return false;
        }
        if (isEnrolled(sid)) {
            cout << "Warning: Student " << sid << " is already enrolled in " << courseID << endl;
            return false;
        }
        enrolledIDs[enrolledCount++] = sid;
        cout << "Student " << sid << " enrolled in " << courseID << endl;
        return true;
    }

    bool drop(const string& sid) {
        if (sid.empty()) {
            cout << "Error: Cannot drop with empty student ID\n";
            return false;
        }
        for (int i = 0; i < enrolledCount; i++) {
            if (enrolledIDs[i] == sid) {
                for (int j = i; j < enrolledCount - 1; j++) {
                    enrolledIDs[j] = enrolledIDs[j + 1];
                }
                enrolledCount--;
                cout << "Student " << sid << " dropped from " << courseID << endl;
                return true;
            }
        }
        cout << "Warning: Student " << sid << " not found in " << courseID << endl;
        return false;
    }

    string getEnrolledID(int i) const {
        if (i < 0 || i >= enrolledCount) {
            cout << "Error: Invalid enrolled index " << i << endl;
            return "";
        }
        return enrolledIDs[i];
    }

    void addAssessment(Assessment* a) {
        if (a == NULL) {
            cout << "Error: Cannot add NULL assessment\n";
            return;
        }
        if (assessmentCount >= MAX_ASSESSMENTS) {
            cout << "Error: Cannot add more assessments. Maximum " << MAX_ASSESSMENTS << endl;
            return;
        }
        assessments[assessmentCount++] = a;
        cout << "Added " << a->getType() << " assessment to " << courseID << endl;
    }

    int getAssessmentCount() const { return assessmentCount; }

    Assessment* getAssessment(int i) const {
        if (i < 0 || i >= assessmentCount) {
            cout << "Error: Invalid assessment index " << i << endl;
            return NULL;
        }
        return assessments[i];
    }

    virtual float calculateFinalGrade(const string& studentID) const = 0;
    virtual int   getExamDuration()    const = 0;
    virtual void  displayInfo()        const = 0;

    static string gradeToLetter(float pct) {
        if (pct < 0) pct = 0;
        if (pct > 100) pct = 100;
        if (pct >= 90) return "A+";
        if (pct >= 85) return "A";
        if (pct >= 80) return "A-";
        if (pct >= 75) return "B+";
        if (pct >= 70) return "B";
        if (pct >= 65) return "B-";
        if (pct >= 60) return "C+";
        if (pct >= 55) return "C";
        if (pct >= 50) return "D";
        return "F";
    }
};

class CoreCourse : public Course {
public:
    CoreCourse() {
        courseType = "Core";
    }
    CoreCourse(const string& cid, const string& t, const string& tid, int cap = 50)
        : Course(cid, t, tid, cap) {
        courseType = "Core";
    }

    float calculateFinalGrade(const string&) const override {
        if (assessmentCount == 0) {
            cout << "Warning: No assessments for course " << courseID << endl;
            return 0;
        }
        float total = 0;
        for (int i = 0; i < assessmentCount; i++) {
            if (assessments[i] != NULL) {
                total += assessments[i]->contribute();
            }
        }
        return total;
    }

    int getExamDuration() const override {
        return 3;
    }

    void displayInfo() const override {
        cout << "[Core Course] " << courseID << " - " << title
            << " | Teacher: " << teacherID
            << " | Enrolled: " << enrolledCount << "/" << capacity
            << " | Exam Duration: 3 hours\n";
    }
};

class ElectiveCourse : public Course {
public:
    ElectiveCourse() {
        courseType = "Elective";
    }
    ElectiveCourse(const string& cid, const string& t, const string& tid, int cap = 50)
        : Course(cid, t, tid, cap) {
        courseType = "Elective";
    }

    float calculateFinalGrade(const string&) const override {
        if (assessmentCount == 0) {
            cout << "Warning: No assessments for course " << courseID << endl;
            return 0;
        }
        float total = 0;
        for (int i = 0; i < assessmentCount; i++) {
            if (assessments[i] != NULL) {
                total += assessments[i]->contribute();
            }
        }
        return total;
    }

    int getExamDuration() const override {
        return 2;
    }

    void displayInfo() const override {
        cout << "[Elective Course] " << courseID << " - " << title
            << " | Teacher: " << teacherID
            << " | Enrolled: " << enrolledCount << "/" << capacity
            << " | Exam Duration: 2 hours\n";
    }
};

class LabCourse : public Course {
public:
    LabCourse() {
        courseType = "Lab";
    }
    LabCourse(const string& cid, const string& t, const string& tid, int cap = 30)
        : Course(cid, t, tid, cap) {
        courseType = "Lab";
    }

    float calculateFinalGrade(const string&) const override {
        if (assessmentCount == 0) {
            cout << "Warning: No assessments for course " << courseID << endl;
            return 0;
        }
        float total = 0;
        for (int i = 0; i < assessmentCount; i++) {
            if (assessments[i] != NULL) {
                total += assessments[i]->contribute();
            }
        }
        return total;
    }

    int getExamDuration() const override {
        return 0;
    }

    void displayInfo() const override {
        cout << "[Lab Course] " << courseID << " - " << title
            << " | Teacher: " << teacherID
            << " | Enrolled: " << enrolledCount << "/" << capacity
            << " | No Final Exam (Continuous Assessment)\n";
    }
};