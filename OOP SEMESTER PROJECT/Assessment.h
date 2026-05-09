#pragma once
#include <string>
#include <iostream>
using namespace std;

//  Assessment  (Abstract)
class Assessment {
protected:
    string type;       // can be exam, quiz,and assignments
    float  rawScore;
    float  maxScore;
    float  weightage;  //  0.40 for 40%

public:
    Assessment() : rawScore(0), maxScore(100), weightage(0) {}

    Assessment(const string& t, float raw, float mx, float w)
        : type(t) {
        setRaw(raw);
        setMax(mx);
        setWeightage(w);
    }

    virtual ~Assessment() {}

    string getType() const { return type; }
    float getRaw() const { return rawScore; }
    float getMax() const { return maxScore; }
    float getWeightage() const { return weightage; }

    void setRaw(float r) {
        // EDGE CASE:raw score cannot be negative
        if (r < 0) {
            rawScore = 0;
            cout << "Warning: Raw score cannot be negative. Set to 0\n";
        }
        // EDGE CASE:raw score cannot exceed max score
        else if (r > maxScore) {
            rawScore = maxScore;
            cout << "Warning: Raw score exceeds max score. Set to " << maxScore << "\n";
        }
        else {
            rawScore = r;
        }
    }

    void setMax(float m) {
        // EDGE CASE:max score cannot be negative
        if (m < 0) {
            maxScore = 0;
            cout << "Warning: Max score cannot be negative. Set to 0\n";
        }
        // EDGE CASE:max score cannot be zero
        else if (m == 0) {
            maxScore = 0;
            cout << "Warning: Max score is 0. This will cause division by zero!\n";
        }
        else {
            maxScore = m;
        }

        // EDGE CASE:if rawScore exceeds new maxScore, adjust it
        if (rawScore > maxScore && maxScore > 0) {
            rawScore = maxScore;
            cout << "Warning: Raw score adjusted to new max score " << maxScore << "\n";
        }
    }

    void setWeightage(float w) {
        // EDGE CASE:weightage cannot be negative
        if (w < 0) {
            weightage = 0;
            cout << "Warning: Weightage cannot be negative. Set to 0\n";
        }
        // EDGE CASE:weightage cannot exceed 1(100%)
        else if (w > 1.0f) {
            weightage = 1.0f;
            cout << "Warning: Weightage exceeds 100%. Set to 1.0\n";
        }
        else {
            weightage = w;
        }
    }

    // Contribution to final grade (0-100 scale)
    virtual float contribute() const {
        // EDGE CASE: Division by zero protection
        if (maxScore <= 0) {
            cout << "Warning: Cannot calculate contribution - maxScore is " << maxScore << endl;
            return 0;
        }
        return (rawScore / maxScore) * weightage * 100.0f;
    }

    virtual void display() const {
        cout << "  [" << type << "] Score: " << rawScore
            << "/" << maxScore
            << "  | Weightage: " << (weightage * 100) << "%"
            << "  | Contribution: " << contribute() << " points\n";
    }
};


//  Derived Assessment Types
class Exam : public Assessment {
public:
    Exam() { type = "Exam"; }

    Exam(float raw, float mx, float w) : Assessment("Exam", raw, mx, w) {}

    // EDGE CASE:exam-specific validation
    void setExamSpecific() {
        if (weightage > 0.5f) {
            cout << "Note: Exam weightage is " << (weightage * 100) << "% (high impact)\n";
        }
    }
};

class Quiz : public Assessment {
public:
    Quiz() { type = "Quiz"; }

    Quiz(float raw, float mx, float w) : Assessment("Quiz", raw, mx, w) {}

    // EDGE CASE:quiz-specific validation
    void validateQuiz() {
        if (maxScore > 50 && weightage < 0.1f) {
            cout << "Warning: Quiz has high max score but low weightage\n";
        }
    }
};

class Assignment : public Assessment {
public:
    Assignment() { type = "Assignment"; }

    Assignment(float raw, float mx, float w) : Assessment("Assignment", raw, mx, w) {}

    // EDGE CASE:assignment-specific validation
    void validateAssignment() {
        if (weightage > 0.4f) {
            cout << "Note: Assignment weightage is " << (weightage * 100) << "%\n";
        }
    }
};