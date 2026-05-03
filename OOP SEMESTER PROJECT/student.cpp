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
    int             transcriptCount;

public:
    Student() : enrolledCount(0), transcriptCount(0) {}
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
        return enrolledCourseIDs[i];
    }

    bool isEnrolledIn(const string& cid) const {
        for (int i = 0; i < enrolledCount; i++)
            if (enrolledCourseIDs[i] == cid)
                return true;
        return false;
    }

    void addCourse(const string& cid) {
        if (enrolledCount < MAX_COURSES_PER_STUDENT && !isEnrolledIn(cid))
            enrolledCourseIDs[enrolledCount++] = cid;
    }
    void dropCourse(const string& cid) {
        for (int i = 0; i < enrolledCount; i++) {
            if (enrolledCourseIDs[i] == cid) {
                for (int j = i; j < enrolledCount - 1; j++)
                    enrolledCourseIDs[j] = enrolledCourseIDs[j + 1];
                enrolledCount--;
                return;
            }
        }
    }
};