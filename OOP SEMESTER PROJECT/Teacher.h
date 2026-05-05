#pragma once
#include "AcademicEntity.h"
#include <string>
#include <iostream>
using namespace std;

const int MAX_FEEDBACK = 200;
const int MAX_TEACHER_COURSES = 20;

struct FeedbackEntry {
    string studentID;
    int    rating;   // this ranges from 1-5
    string comment;
};

class Teacher : public AcademicEntity {
private:
    FeedbackEntry feedback[MAX_FEEDBACK];
    int           feedbackCount;

    string assignedCourseIDs[MAX_TEACHER_COURSES];
    int    courseCount;

public:
    Teacher() : feedbackCount(0), courseCount(0) {}

    Teacher(const string& id, const string& n, const string& e)
        : AcademicEntity(id, n, e), feedbackCount(0), courseCount(0) {
    }

    //FEEDBACK FUNCTIONS

    void addFeedback(const string& sid, int rating, const string& comment) {
        // EDGE CASE 1: Check if student ID is empty
        if (sid.empty()) {
            cout << "Error: Cannot add feedback with empty student ID\n";
            return;
        }

        //EDGE CASE 2: Validate rating range (1-5)
        if (rating < 1) rating = 1;
        if (rating > 5) rating = 5;

        // EDGE CASE 3: Check if feedback array is full
        if (feedbackCount >= MAX_FEEDBACK) {
            cout << "Error: Feedback storage full! Cannot add feedback for " << sid << "\n";
            return;
        }

        //Check for duplicate student ID (update existing)
        for (int i = 0; i < feedbackCount; i++) {
            if (feedback[i].studentID == sid) {
                feedback[i].rating = rating;
                feedback[i].comment = comment;
                cout << "Updated existing feedback for student: " << sid << endl;
                return;
            }
        }

        // Adding new feedback
        feedback[feedbackCount].studentID = sid;
        feedback[feedbackCount].rating = rating;
        feedback[feedbackCount].comment = comment;
        feedbackCount++;

        cout << "Added feedback for student: " << sid << endl;
    }

    float getAverageFeedback() const {
        // EDGE CASE: No feedback received
        if (feedbackCount == 0) return 0.0f;

        float total = 0;
        for (int i = 0; i < feedbackCount; i++) {
            total += feedback[i].rating;
        }
        return total / feedbackCount;
    }

    int getFeedbackCount() const {
        return feedbackCount;
    }

    FeedbackEntry getFeedback(int i) const {
        // EDGE CASE: checking if  index is valid
        if (i < 0 || i >= feedbackCount) {
            cout << "Error: Invalid feedback index " << i << endl;
            FeedbackEntry empty;
            empty.studentID = "";
            empty.rating = 0;
            empty.comment = "";
            return empty;
        }
        return feedback[i];
    }

    void displayFeedback() const {
        cout << "\n  Feedback for " << name << " (Avg: " << getAverageFeedback() << "/5):\n";

        // EDGE CASE: No feedback
        if (feedbackCount == 0) {
            cout << " No feedback received yet.\n";
            return;
        }

        for (int i = 0; i < feedbackCount; i++) {
            cout << "    [" << feedback[i].studentID << "] "
                << feedback[i].rating << "/5. "
                << feedback[i].comment << "\n";
        }
    }

    //COURSE ASSIGNMENT FUNCTIONS

    void assignCourse(const string& cid) {
        // EDGE CASE 1: Empty course ID
        if (cid.empty()) {
            cout << "Error: Cannot assign empty course ID to teacher\n";
            return;
        }

        // EDGE CASE 2: Check if course already assigned
        if (teacherOf(cid)) {
            cout << "Warning: Course " << cid << " is already assigned to this teacher\n";
            return;
        }

        // EDGE CASE 3: Check if course array is full
        if (courseCount >= MAX_TEACHER_COURSES) {
            cout << "Error: Teacher cannot take more than " << MAX_TEACHER_COURSES << " courses\n";
            return;
        }

        assignedCourseIDs[courseCount++] = cid;
        cout << "Assigned course " << cid << " to teacher " << name << endl;
    }

    void removeCourse(const string& cid) {
        // EDGE CASE: empty course ID
        if (cid.empty()) {
            cout << "Error: Cannot remove empty course ID\n";
            return;
        }

        // Find and remove the course
        for (int i = 0; i < courseCount; i++) {
            if (assignedCourseIDs[i] == cid) {
                // Shift all courses after this one left
                for (int j = i; j < courseCount - 1; j++) {
                    assignedCourseIDs[j] = assignedCourseIDs[j + 1];
                }
                courseCount--;
                cout << "Removed course " << cid << " from teacher " << name << endl;
                return;
            }
        }

        // EDGE CASE: Course not found
        cout << "Warning: Course " << cid << " not found in teacher's assigned courses\n";
    }

    int getCourseCount() const {
        return courseCount;
    }

    string getAssignedCourse(int i) const {
        // EDGE CASE: Invalid index
        if (i < 0 || i >= courseCount) {
            cout << "Error: Invalid course index " << i << endl;
            return "";
        }
        return assignedCourseIDs[i];
    }

    bool teacherOf(const string& cid) const {
        // EDGE CASE: Empty course ID
        if (cid.empty()) return false;

        for (int i = 0; i < courseCount; i++) {
            if (assignedCourseIDs[i] == cid) return true;
        }
        return false;
    }

    void displayProfile() const override {
        cout << "[Teacher] ID: " << ID
            << " | Name: " << name
            << " | Email: " << email
            << " | Avg Rating: " << getAverageFeedback() << "/5"
            << " | Courses: " << courseCount << "\n";

        // EDGE CASE: Show warning if average rating is low
        if (getAverageFeedback() < 2.5 && feedbackCount > 0) {
            cout << "  Warning: Low teacher rating!\n";
        }
    }
};
