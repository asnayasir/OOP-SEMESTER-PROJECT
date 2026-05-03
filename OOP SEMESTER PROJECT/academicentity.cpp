#include <iostream>
#include <string>
using namespace std;

//academic entity
class AcademicEntity {
protected:
    string ID;
    string name;
    string email;
public:
    AcademicEntity() {       //default constructor
        ID = "";
        name = "Unknown";
        email = "unknown@example.com";
    }

    AcademicEntity(const string& id, const string& n, const string& e) {   //setters in
        setID(id);
        setName(n);
        setEmail(e);
    }

    virtual ~AcademicEntity() {}

    string getID()    const { return ID; }
    string getName()  const { return name; }
    string getEmail() const { return email; }

    void setID(const string& id) {
        if (id.empty()) {
            ID = "default0";
            cout << "Warning: Empty cannot be empty. Assigned default0\n";
        }
        else {
            ID = id;
        }
    }

    void setName(const string& n) {
        if (isValidName(n)) {
            name = n;
        }
        else {
            name = "Unknown";
            cout << "Warning: Invalid name '" << n << "'. Name cannot have numbers. Set to 'Unknown'" << endl;
        }
    }

    void setEmail(const string& e) {
        if (isValidEmail(e)) {
            email = e;
        }
        else {
            email = "default@email.com";
            cout << "Warning: Invalid email '" << e << ". Set to 'default@email.com'"<<endl;
        }
    }

    virtual void displayProfile() const = 0;

protected:
    bool isValidName(const string& n) const {
        if (n.empty())
          return false;
        for (int i = 0; i < (int)n.length(); i++) {
            char c = n[i];
            if (!((c >= 'a' && c <= 'z') ||
                (c >= 'A' && c <= 'Z') ||
                c == ' ' || c == '.' || c == '-')) {
                return false;
            }
        }
        return true;
    }

   
};