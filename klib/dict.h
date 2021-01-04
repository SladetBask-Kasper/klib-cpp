/*

    This dict (dictionary) class is written by "Kasper - Sladetbask" on github.
    (https://github.com/SladetBask-Kasper/)
    Anyone editing this code can feel free to add there name.
    Please don't remove my name from this file, Thanks!

*/
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#define DICT_GET_ERROR "Get Error (from dict)"

/*

    This class is a class that was made to bring python dictionary (Dict) to C++
    The class is writen on windows but should work on any platform!

    [HOW IT WORKS]
    It takes two vectors and assings a value and a name two each vector,
    then when it trys to get the vectors value, because we assign a value and refrence name at the same time,
    the refrence name and value have the same index!

    [FOR PEOPLE WHO WILL READ THE SOURCE CODE]
    refrence = the vector with names
    value    = Is in most cases the content of the dict. (Edit: What do I mean "most cases?")

*/
class dict {
protected:
    std::vector<std::string> mainVector;
    std::vector<std::string> refrenceVector;
public:
    bool append(std::string, std::string);
    dict(std::string, std::string);
    dict();
    virtual ~dict() {};

    int scanIndex(std::string);
    std::string get(std::string);
    std::vector<std::string> get(int pos);
    size_t length();
    size_t size();
    bool removeValue(std::string);
    bool changeValue(std::string, std::string);
    bool isValue(std::string allocName);
    bool isEmpty();
    bool move(std::string, std::string);

};

/*
    constructor...
*/
dict::dict(std::string allocName, std::string content) {
    append(allocName, content);
}
dict::dict() {}

/*
    Appends to the vectors
    adding the refrence along with its value
*/
bool dict::append(std::string allocName, std::string content) {

    if (!refrenceVector.empty()) {
        for (int i = 0; i < refrenceVector.size(); i++) {
            if (refrenceVector[i] == allocName) {
                return false;
            }
        }
    }

    // mainVector Holds Content
    mainVector.push_back(content);
    refrenceVector.push_back(allocName);

    return true;
}

/*
    scans for the value, getting the index value!
*/
int dict::scanIndex(std::string valueToScanFor) {
    for (int i = 0; i < refrenceVector.size(); i++) {
        if (refrenceVector[i] == valueToScanFor) {
            return i;
        }
    }
    return -1;
}

/*
    Gets the value of refrence
*/
std::string dict::get(std::string allocName) {

    int callIndex = scanIndex(allocName);
    if (callIndex == -1) {
        return DICT_GET_ERROR;
    }
    return mainVector[callIndex];
}
std::vector<std::string> dict::get(int pos) {
    std::vector<std::string> rv;
    rv.push_back(refrenceVector[pos]);
    rv.push_back(mainVector[pos]);
    return rv;
}

/*
    Gets length of vector
*/
size_t dict::length() {
    return refrenceVector.size();
}
size_t dict::size() {
    return this->length(); // couldn't decide what to call it so..
}

/*

    Removes a value from the thingy

*/

bool dict::removeValue(std::string allocName) {
    int callIndex = scanIndex(allocName);
    if (callIndex == -1) {
        return false;
    }

    mainVector.erase(mainVector.begin() + callIndex);
    refrenceVector.erase(refrenceVector.begin() + callIndex);

    return true;
}

/*

    Checks if value exists.

*/

bool dict::isValue(std::string allocName) {
    return (scanIndex(allocName) != -1);
}

/*

    if dict is empty

*/
bool dict::isEmpty() {
    return refrenceVector.empty();
}

/*

    Allows you to change the value of a refrence

*/
bool dict::changeValue(std::string fromNAME, std::string toCONTENT) {
    int id = scanIndex(fromNAME);

    if (id == -1) {
        return false;
    }

    mainVector[id] = toCONTENT;
    return true;
}

/*

    Moves the value from a refrence to another

*/
bool dict::move(std::string fromNAME, std::string toNAME) {

    int id = scanIndex(fromNAME);
    int id2 = scanIndex(toNAME);

    std::string flags = "";

    if (id == -1) {
        return false;
    }
    if (id2 == -1) {
        flags = flags + "MAKENEW ";
    }

    if (flags.find("MAKENEW") != std::string::npos) {
        // if we gonna make new
        append(toNAME, mainVector[id]);// id = fromNAME id
        return true;
    }
    else {
        // or not
        refrenceVector[id] = mainVector[id2];
        return true;
    }

}
