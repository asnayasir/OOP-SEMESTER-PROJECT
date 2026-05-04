#pragma once
#include "AcademicEntity.h"
#include <string>
#include <iostream>
using namespace std;

const int MAX_COURSES_PER_STUDENT = 20;

struct TranscriptEntry {
    string courseID;
    string courseTitle;
    float  percentage;
    string grade;
    string passFail; // for exchange students
};

class Student : public AcademicEntity {
protected:
    string studentType; // "Regular" | "Scholarship" | "Exchange"
    string enrolledCourseIDs[MAX_COURSES_PER_STUDENT];
    int    enrolledCount;

    TranscriptEntry transcript[MAX_COURSES_PER_STUDENT];
    int transcriptCount;

public:
    Student() : enrolledCount(0), transcriptCount(0) {
    }

    Student(const string& id, const string& n, const string& e)
        : AcademicEntity(id, n, e), enrolledCount(0), transcriptCount(0) {
    }

    virtual ~Student() {}

    string getStudentType() const {
        return studentType;
    }

    int getEnrolledCount() const {
        return enrolledCount;
    }

    string getEnrolledCourse(int i) const {
        if (i >= 0 && i < enrolledCount) {
            return enrolledCourseIDs[i];
        }
        return "";
    }

    bool isEnrolledIn(const string& cid) const {
        for (int i = 0; i < enrolledCount; i++) {
            if (enrolledCourseIDs[i] == cid) {
                return true;
            }
        }
        return false;  
    }

    void addCourse(const string& cid) {
        if (enrolledCount < MAX_COURSES_PER_STUDENT && !isEnrolledIn(cid)) {
            enrolledCourseIDs[enrolledCount++] = cid;
        }
    }

    void dropCourse(const string& cid) {
        for (int i = 0; i < enrolledCount; i++) {
            if (enrolledCourseIDs[i] == cid) {
                for (int j = i; j < enrolledCount - 1; j++) {
                    enrolledCourseIDs[j] = enrolledCourseIDs[j + 1];
                }
                enrolledCount--;
                return;
            }
        }
    }

    void updateTranscript(const string& cid, const string& ctitle,
        float pct, const string& grade,
        const string& pf = "") {

        // EDGE CASE 1: Validating percentage range
        if (pct < 0) pct = 0;
        if (pct > 100) pct = 100;

        // EDGE CASE 2: Check if course ID is empty
        if (cid.empty()) {
            cout << "Error: Cannot update transcript with empty course ID\n";
            return;
        }

        // EDGE CASE 3: Check if course title is empty (giving warning only)
        if (ctitle.empty()) {
            cout << "Warning: Empty course title for " << cid << endl;
        }

        // Update existing course
        for (int i = 0; i < transcriptCount; i++) {
            if (transcript[i].courseID == cid) {
                transcript[i].percentage = pct;
                transcript[i].grade = grade;
                transcript[i].passFail = pf;
                cout << "Updated existing course: " << cid << endl;
                return;
            }
        }

        // EDGE CASE 4: Check if transcript is full
        if (transcriptCount >= MAX_COURSES_PER_STUDENT) {
            cout << "Error: Transcript full! Cannot add " << cid << endl;
            return;
        }

        // EDGE CASE 5: Validate grades (warning only)
        if (grade != "A" && grade != "A-" && grade != "B+" && grade != "B" &&
            grade != "B-" && grade != "C+" && grade != "C" && grade != "F" && !grade.empty()) {
            cout << "Warning: no such grade as '" << grade << "' for " << cid << endl;
        }

        // Add new course
        transcript[transcriptCount].courseID = cid;
        transcript[transcriptCount].courseTitle = ctitle;
        transcript[transcriptCount].percentage = pct;
        transcript[transcriptCount].grade = grade;
        transcript[transcriptCount].passFail = pf;
        transcriptCount++;

        cout << "Added new course: " << cid << endl;
    }

    int getTranscriptCount() const { return transcriptCount; }

    TranscriptEntry getTranscriptEntry(int i) const {
        if (i < 0 || i >= transcriptCount) {
            cout << "Error: Invalid transcript index " << i << endl;
            return TranscriptEntry();
        }
        return transcript[i];
    }

    // Pure virtual functions
    virtual float calculateGPA() const = 0;
    virtual void viewTranscript() const = 0;
    virtual void displayProfile() const = 0;
};

// Regular Student
class RegularStudent : public Student {
public:
    RegularStudent() {
        studentType = "Regular";
    }

    RegularStudent(const string& id, const string& n, const string& e)
        : Student(id, n, e) {
        studentType = "Regular";
    }

    float calculateGPA() const override {
        if (transcriptCount == 0) return 0.0f;

        float total = 0;
        int count = 0;

        for (int i = 0; i < transcriptCount; i++) {
            float pct = transcript[i].percentage;

            if (pct < 0) pct = 0;
            if (pct > 100) pct = 100;

            float pts = 0;
            if (pct >= 90) pts = 4.0f;
            else if (pct >= 85) pts = 3.7f;
            else if (pct >= 80) pts = 3.3f;
            else if (pct >= 75) pts = 3.0f;
            else if (pct >= 70) pts = 2.7f;
            else if (pct >= 65) pts = 2.3f;
            else if (pct >= 60) pts = 2.0f;
            else if (pct >= 55) pts = 1.7f;
            else if (pct >= 50) pts = 1.0f;

            total += pts;
            count++;
        }

        return total / count;
    }

    void viewTranscript() const override {
        cout << "\n========== TRANSCRIPT: " << name << " (" << ID << ") ==========\n";

        if (transcriptCount == 0) {
            cout << "  No grades recorded yet.\n";
        }
        else {
            for (int i = 0; i < transcriptCount; i++) {
                cout << "  " << transcript[i].courseID
                    << " - " << transcript[i].courseTitle
                    << "  | " << transcript[i].percentage << "%"
                    << "  | Grade: " << transcript[i].grade << "\n";
            }
            cout << "  GPA: " << calculateGPA() << "\n";
        }

        cout << "=====================================================\n";
    }

    void displayProfile() const override {
        cout << "[Regular Student] ID: " << ID
            << " | Name: " << name
            << " | Email: " << email
            << " | GPA: " << calculateGPA() << "\n";
    }
};

    //scholarship student
    //exchange student