#pragma once
#include <fstream>
#include <string>
#include <iostream>
using namespace std;

struct WeightageConfig {
    float exam;
    float assignment;
    float quiz;
};

// Global variables for weightages
static WeightageConfig coreWeight;
static WeightageConfig electiveWeight;
static WeightageConfig labWeight;

// Function to load weightages from file
inline void loadWeightages(const string& filename) {
    ifstream file(filename);

    // If file doesn't exist, use default values
    if (!file.is_open()) {
        coreWeight.exam = 0.60f;
        coreWeight.assignment = 0.20f;
        coreWeight.quiz = 0.20f;

        electiveWeight.exam = 0.00f;
        electiveWeight.assignment = 0.60f;
        electiveWeight.quiz = 0.40f;

        labWeight.exam = 0.00f;
        labWeight.assignment = 0.70f;
        labWeight.quiz = 0.30f;

        cout << "Using default weightages\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        // Skip empty lines
        if (line.length() == 0) continue;

       
        char courseType = line[0];

        // Find the |
        int pos1 = -1, pos2 = -1, pos3 = -1;
        for (int i = 0; i < (int)line.length(); i++) {
            if (line[i] == '|') {
                if (pos1 == -1) pos1 = i;
                else if (pos2 == -1) pos2 = i;
                else if (pos3 == -1) pos3 = i;
            }
        }

        if (pos1 == -1 || pos2 == -1 || pos3 == -1) continue;

        //values as strings
        string examStr = "";
        string assignStr = "";
        string quizStr = "";

        for (int i = pos1 + 1; i < pos2; i++) examStr += line[i];
        for (int i = pos2 + 1; i < pos3; i++) assignStr += line[i];
        for (int i = pos3 + 1; i < (int)line.length(); i++) quizStr += line[i];

        //converting string to float manually
        float e = 0, a = 0, q = 0;

        // Convert exam string
        for (int i = 0; i < (int)examStr.length(); i++) {
            char c = examStr[i];
            if (c >= '0' && c <= '9') {
                e = e * 10 + (c - '0');
            }
        }
        e = e / 100; // Convert 60 to 0.60

        //convert assignment string
        for (int i = 0; i < (int)assignStr.length(); i++) {
            char c = assignStr[i];
            if (c >= '0' && c <= '9') {
                a = a * 10 + (c - '0');
            }
        }
        a = a / 100;

        // convert quiz string
        for (int i = 0; i < (int)quizStr.length(); i++) {
            char c = quizStr[i];
            if (c >= '0' && c <= '9') {
                q = q * 10 + (c - '0');
            }
        }
        q = q / 100;

        // Assign based on course type
        if (courseType == 'C') {  // Core
            coreWeight.exam = e;
            coreWeight.assignment = a;
            coreWeight.quiz = q;
        }
        else if (courseType == 'E') {  // Elective
            electiveWeight.exam = e;
            electiveWeight.assignment = a;
            electiveWeight.quiz = q;
        }
        else if (courseType == 'L') {  // Lab
            labWeight.exam = e;
            labWeight.assignment = a;
            labWeight.quiz = q;
        }
    }
    file.close();
}

//this is the function to get weightages for a course type
inline void getWeightage(const string& courseType, float& exam, float& assignment, float& quiz) {
    if (courseType == "Core") {
        exam = coreWeight.exam;
        assignment = coreWeight.assignment;
        quiz = coreWeight.quiz;
    }
    else if (courseType == "Elective") {
        exam = electiveWeight.exam;
        assignment = electiveWeight.assignment;
        quiz = electiveWeight.quiz;
    }
    else {
        exam = labWeight.exam;
        assignment = labWeight.assignment;
        quiz = labWeight.quiz;
    }
}
//function to save default weightages to file
inline void saveDefaultWeightages(const string& filename) {
    ofstream file(filename);

    if (!file.is_open()) {
        cout << "Error: Cannot create " << filename << endl;
        return;
    }

    file << "C|60|20|20\n";
    file << "E|0|60|40\n";
    file << "L|0|70|30\n";

    file.close();
    cout << "Default weightages saved to " << filename << endl;
}