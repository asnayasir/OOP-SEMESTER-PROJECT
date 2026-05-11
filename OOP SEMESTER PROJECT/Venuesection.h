#pragma once
#include <string>
#include <iostream>
using namespace std;


//  Venue
class Venue {
private:
    string roomID;
    int    capacity;
    bool   hasComputers;

public:
    Venue() : capacity(0), hasComputers(false) {
        roomID = "";
    }

    Venue(const string& rid, int cap, bool computers) {
        // EDGE CASE:empty room ID
        if (rid.empty()) {
            roomID = "";
        }
        else {
            roomID = rid;
        }

        // EDGE CASE:negative capacity
        if (cap < 0) {
            capacity = 0;
        }
        else {
            capacity = cap;
        }

        hasComputers = computers;
    }

    string getRoomID()      const { return roomID; }
    int    getCapacity()    const { return capacity; }
    bool   getHasComputers() const { return hasComputers; }

    void setRoomID(const string& r) {
        roomID = r;
    }

    void setCapacity(int c) {
        // EDGE CASE:negative capacity
        if (c < 0) {
            capacity = 0;
        }
        else {
            capacity = c;
        }
    }

    void setHasComputers(bool h) {
        hasComputers = h;
    }

    void display() const {
        cout << "[Venue] " << roomID
            << " | Capacity: " << capacity
            << " | Computers: " << (hasComputers ? "Yes" : "No") << "\n";
    }
};


//section

class Section {
private:
    string sectionID;
    string courseID;
    string teacherID;
    string venueID;
    string timeSlot;

public:
    Section() {
        sectionID = "";
        courseID = "";
        teacherID = "";
        venueID = "";
        timeSlot = "";
    }

    Section(const string& sid, const string& cid,
        const string& tid, const string& vid, const string& ts) {
        // EDGE CASE:empty section ID
        if (sid.empty()) {
            sectionID = "";
        }
        else {
            sectionID = sid;
        }

        // EDGE CASE:empty course ID
        if (cid.empty()) {
            courseID = "";
        }
        else {
            courseID = cid;
        }

        // EDGE CASE: empty teacher ID
        if (tid.empty()) {
            teacherID = "";
        }
        else {
            teacherID = tid;
        }

        // EDGE CASE: empty venue ID
        if (vid.empty()) {
            venueID = "";
        }
        else {
            venueID = vid;
        }

        // EDGE CASE: empty time slot
        if (ts.empty()) {
            timeSlot = "";
        }
        else {
            timeSlot = ts;
        }
    }

    string getSectionID() const { return sectionID; }
    string getCourseID()  const { return courseID; }
    string getTeacherID() const { return teacherID; }
    string getVenueID()   const { return venueID; }
    string getTimeSlot()  const { return timeSlot; }

    void setSectionID(const string& s) { sectionID = s; }
    void setCourseID(const string& s) { courseID = s; }
    void setTeacherID(const string& s) { teacherID = s; }
    void setVenueID(const string& s) { venueID = s; }
    void setTimeSlot(const string& s) { timeSlot = s; }

    void display() const {
        cout << "[Section] " << sectionID
            << " | Course: " << courseID
            << " | Teacher: " << teacherID
            << " | Venue: " << venueID
            << " | Time: " << timeSlot << "\n";
    }
};