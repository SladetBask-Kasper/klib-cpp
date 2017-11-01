#include <iostream>
#include <string>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>

#define VERSION "0.0.1"
#define tab "\t"

using namespace std;

void print(string txt)
{
    cout << txt << endl;
}
void prints(string txt)
{
    cout << txt;
}

class vars
{
public:
    /// IF contains (If String Contains)
    static bool iContains(string str, string contains) {

        if (str.find(contains) != string::npos) {
            return true;
        }
        else
            return false;

    }

    /// Remove Spaces Function
    std::string removeSpaces(std::string input)
    {
        input.erase(std::remove(input.begin(), input.end(), ' '),input.end());
        return input;
    }
};

class log
{
public:
    // type = 1(nothing) 2([]) 3(\"\") 4(())
    static void push(const string text, int type = 1)
    {

        if (type <= 0 || type >= 5) {
            cout << " ["<< getCurrentTime() << "]-> " << "ERROR: INVALID SYNTAX IN PUSH." << endl
                << "\t    Make Sure That Your \"type\" variable is 1 - 4 (nothing more, nothing less)";
        }

        if (type == 1) {
            cout << " ["<< getCurrentTime() << "]-> " << text << endl;
            return;
        }
        if (type == 2) {
            cout << " ["<< getCurrentTime() << "]-> [" << text << "]" << endl;
            return;
        }
        if (type == 3) {
            cout << " ["<< getCurrentTime() << "]-> \"" << text << "\"" << endl;
            return;
        }
        if (type == 4) {
            cout << " ["<< getCurrentTime() << "]-> (" << text << ")" << endl;
            return;
        }

    }

    static const string getCurrentTime() {

        time_t     now = time(0);
        struct tm  tstruct;
        char       buf[80];
        tstruct = *localtime(&now);

        // <<  Ad  >>
        // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime (can be a virus website or not for all I know)
        // for more information about date/time format
        // << /Ad >>
        // <KLIB DEV: > This was left here from an answer on stack overflow.
        // Some old post from 8 years ago (like 2012 or something)


        //strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct); // Answer original. Gets Year, Day, Everything...
        strftime(buf, sizeof(buf), "%X", &tstruct);
        return buf;
    }
};
