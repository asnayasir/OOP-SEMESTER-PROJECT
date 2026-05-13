#pragma once
#include <string>
#include <iostream>
using namespace std;

class Assessment {
protected:
    string type;
    float  rawScore;
    float  maxScore;
    float  weightage;

public:
    Assessment() : rawScore(0), maxScore(100), weightage(0) {}

    Assessment(const string& t, float raw, float mx, float w)
        : type(t), rawScore(0), maxScore(mx), weightage(0) {

        // Validate raw score
        if (raw < 0) {
            rawScore = 0;
            cout << "Warning: Raw score cannot be negative. Set to 0\n";
        }
        else if (raw > maxScore) {
            rawScore = maxScore;
            cout << "Warning: Raw score exceeds max score. Set to " << maxScore << "\n";
        }
        else {
            rawScore = raw;
        }

        // Validate weightage
        if (w < 0) {
            weightage = 0;
            cout << "Warning: Weightage cannot be negative. Set to 0\n";
        }
        else if (w > 1.0f) {
            weightage = 1.0f;
            cout << "Warning: Weightage exceeds 100%. Set to 1.0\n";
        }
        else {
            weightage = w;
        }
    }

    virtual ~Assessment() {}

    string getType() const { return type; }
    float getRaw() const { return rawScore; }
    float getMax() const { return maxScore; }
    float getWeightage() const { return weightage; }

    void setRaw(float r) {
        if (r < 0) {
            rawScore = 0;
            cout << "Warning: Raw score cannot be negative. Set to 0\n";
        }
        else if (r > maxScore) {
            rawScore = maxScore;
            cout << "Warning: Raw score exceeds max score. Set to " << maxScore << "\n";
        }
        else {
            rawScore = r;
        }
    }

    void setMax(float m) {
        if (m < 0) {
            maxScore = 0;
            cout << "Warning: Max score cannot be negative. Set to 0\n";
        }
        else if (m == 0) {
            maxScore = 0;
            cout << "Warning: Max score is 0. This will cause division by zero!\n";
        }
        else {
            maxScore = m;
        }
        if (rawScore > maxScore && maxScore > 0) {
            rawScore = maxScore;
        }
    }

    void setWeightage(float w) {
        if (w < 0) {
            weightage = 0;
            cout << "Warning: Weightage cannot be negative. Set to 0\n";
        }
        else if (w > 1.0f) {
            weightage = 1.0f;
            cout << "Warning: Weightage exceeds 100%. Set to 1.0\n";
        }
        else {
            weightage = w;
        }
    }

    virtual float contribute() const {
        if (maxScore <= 0) {
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

class Exam : public Assessment {
public:
    Exam() { type = "Exam"; }
    Exam(float raw, float mx, float w) : Assessment("Exam", raw, mx, w) {}
};

class Quiz : public Assessment {
public:
    Quiz() { type = "Quiz"; }
    Quiz(float raw, float mx, float w) : Assessment("Quiz", raw, mx, w) {}
};

class Assignment : public Assessment {
public:
    Assignment() { type = "Assignment"; }
    Assignment(float raw, float mx, float w) : Assessment("Assignment", raw, mx, w) {}
};