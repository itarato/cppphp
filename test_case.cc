#pragma once

#include <iostream>
#include <string>

using namespace std;

class TestCase {
private:
    unsigned int count_success;
    unsigned int count_failure;
    
public:
    TestCase() : count_success(0), count_failure(0) {};
    
    virtual void run() = 0;
    virtual string getName() { return ""; };
    
    virtual void report() {
        if (count_failure > 0) {
            cout << "TEST FAILED" << endl;
        }
        cout << getName() << " -- SUCCESS: " << count_success << " FAILURE: " << count_failure << endl;
    }
    
    void assert(bool check, string msg) {
        if (!check) {
            cout << "ERROR: " << msg << endl;
            count_failure++;
        } else {
            count_success++;
        }
    }
    
    template <class T1, class T2>
    void assert(bool check, string msg, T1 expected, T2 got) {
        if (!check) {
            cout << "ERROR: " << msg << " Expected: <" << expected << "> Got: <" << got << ">" << endl;
            count_failure++;
        } else {
            count_success++;
        }
    }
};
