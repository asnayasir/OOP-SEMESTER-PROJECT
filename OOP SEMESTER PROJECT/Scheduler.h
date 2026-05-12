#pragma once
#include "VenueSection.h"
#include "Course.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const int MAX_SLOTS = 20;
const int MAX_VENUES = 50;
const int MAX_SECTIONS = 100;

// time slots
static const string TIME_SLOTS[MAX_SLOTS] = {
    "Mon 08:00-11:00", "Mon 11:00-14:00", "Mon 14:00-17:00",
    "Tue 08:00-11:00", "Tue 11:00-14:00", "Tue 14:00-17:00",
    "Wed 08:00-11:00", "Wed 11:00-14:00", "Wed 14:00-17:00",
    "Thu 08:00-11:00", "Thu 11:00-14:00", "Thu 14:00-17:00",
    "Fri 08:00-11:00", "Fri 11:00-14:00", "Fri 14:00-17:00",
    "Sat 08:00-11:00", "Sat 11:00-14:00", "Sat 14:00-17:00",
    "Sun 08:00-11:00", "Sun 11:00-14:00"
};

struct ExamSlot {
    string courseID;
    string venueID;
    string timeSlot;
};

class Scheduler {
private:
    bool occupied[MAX_SLOTS][MAX_VENUES];
    ExamSlot schedule[MAX_SECTIONS];
    int scheduleCount;

    Venue* venues;
    int venueCount;

    int findVenue(const string& rid) const {
        for (int i = 0; i < venueCount; i++) {
            if (venues[i].getRoomID() == rid) {
                return i;
            }
        }
        return -1;
    }

public:
    Scheduler() : scheduleCount(0), venues(NULL), venueCount(0) {
        for (int i = 0; i < MAX_SLOTS; i++) {
            for (int j = 0; j < MAX_VENUES; j++) {
                occupied[i][j] = false;
            }
        }
    }

    void setVenues(Venue* v, int count) {
        venues = v;
        venueCount = count;
    }

    // find free slot for a course
    bool findFreeSlot(int needCapacity, bool needComputers,
        int& outSlot, int& outVenue) const {
        // edge case: no venues set
        if (venues == NULL || venueCount == 0) {
            return false;
        }

        for (int s = 0; s < MAX_SLOTS; s++) {
            for (int v = 0; v < venueCount; v++) {
                if (occupied[s][v]) continue;
                if (venues[v].getCapacity() < needCapacity) continue;
                if (needComputers && !venues[v].getHasComputers()) continue;

                outSlot = s;
                outVenue = v;
                return true;
            }
        }
        return false;
    }

    // schedule a course exam
    int scheduleCourse(Course* course) {
        // edge case: null course
        if (course == NULL) {
            cout << "error: cannot schedule null course\n";
            return 0;
        }

        // edge case: no venues set
        if (venues == NULL || venueCount == 0) {
            cout << "error: no venues available for " << course->getCourseID() << "\n";
            return 0;
        }

        // edge case: lab courses have no exam
        if (course->getCourseType() == "Lab") {
            cout << course->getCourseID() << " is lab course - no exam\n";
            return 0;
        }

        // edge case: no students enrolled
        int enrolled = course->getEnrolled();
        if (enrolled <= 0) {
            cout << course->getCourseID() << " has no students\n";
            return 0;
        }

        int remaining = enrolled;
        int assigned = 0;
        int chosenSlot = -1;

        // find a time slot
        for (int s = 0; s < MAX_SLOTS && chosenSlot == -1; s++) {
            int tempRemaining = remaining;
            for (int v = 0; v < venueCount; v++) {
                if (occupied[s][v]) continue;
                tempRemaining -= venues[v].getCapacity();
                if (tempRemaining <= 0) {
                    chosenSlot = s;
                    break;
                }
            }
        }

        // edge case: no slot found
        if (chosenSlot == -1) {
            cout << "error: no time slot for " << course->getCourseID() << "\n";
            return 0;
        }

        // assign venues in that slot
        int rem = remaining;
        for (int v = 0; v < venueCount && rem > 0; v++) {
            if (occupied[chosenSlot][v]) continue;

            // edge case: schedule array full
            if (scheduleCount >= MAX_SECTIONS) {
                cout << "warning: schedule array full\n";
                break;
            }

            occupied[chosenSlot][v] = true;
            schedule[scheduleCount].courseID = course->getCourseID();
            schedule[scheduleCount].venueID = venues[v].getRoomID();
            schedule[scheduleCount].timeSlot = TIME_SLOTS[chosenSlot];
            scheduleCount++;
            assigned++;
            rem -= venues[v].getCapacity();
        }

        // edge case: not all students got seats
        if (rem > 0) {
            cout << "warning: " << rem << " students have no seat for "
                << course->getCourseID() << "\n";
        }
        else {
            cout << course->getCourseID() << " scheduled at "
                << TIME_SLOTS[chosenSlot] << " in " << assigned << " venues\n";
        }

        return assigned;
    }

    void showSchedule() const {
        cout << "\n===== exam schedule =====\n";

        if (scheduleCount == 0) {
            cout << "no exams scheduled\n";
            return;
        }

        for (int i = 0; i < scheduleCount; i++) {
            cout << "course: " << schedule[i].courseID
                << " | venue: " << schedule[i].venueID
                << " | time: " << schedule[i].timeSlot << "\n";
        }
        cout << "========================\n";
    }

    void saveSchedule(const string& filename) const {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "error: cannot save schedule\n";
            return;
        }

        for (int i = 0; i < scheduleCount; i++) {
            file << schedule[i].courseID << "|"
                << schedule[i].venueID << "|"
                << schedule[i].timeSlot << "\n";
        }

        file.close();
        cout << "schedule saved to " << filename << "\n";
    }

    void findConflicts() const {
        cout << "\n===== checking conflicts =====\n";

        if (scheduleCount == 0) {
            cout << "no schedule to check\n";
            return;
        }

        bool hasConflict = false;

        for (int i = 0; i < scheduleCount; i++) {
            for (int j = i + 1; j < scheduleCount; j++) {
                if (schedule[i].venueID == schedule[j].venueID &&
                    schedule[i].timeSlot == schedule[j].timeSlot) {
                    cout << "conflict: " << schedule[i].courseID
                        << " and " << schedule[j].courseID
                        << " at " << schedule[i].venueID
                        << " " << schedule[i].timeSlot << "\n";
                    hasConflict = true;
                }
            }
        }

        if (!hasConflict) {
            cout << "no conflicts found\n";
        }
    }

    // getters
    int getScheduleCount() const { return scheduleCount; }
    ExamSlot getExamSlot(int i) const { return schedule[i]; }
};