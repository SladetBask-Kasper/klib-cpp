/*

    This dict (dictionary) class is written by "Kasper - Sladetbask" on github.
    Anyone editing this code can feel free to add there name.
    Please don't remove my name from this file, Thanks!

*/
#ifndef DICT_H
#define DICT_H
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "exceptions.h"

#define DEL_KEY       "::{D_V"
#define DEFUALT_VALUE "FORNOWEMPTYVALUE"


using namespace std;

/*

    This class is a class that was made to bring python dictionary (Dict) to C++
    The class is writen on windows but should work on any platform!

    [HOW IT WORKS]
    It takes two vectors (vector: basicly an array) and assings a value and a name two each vector,
    then when it trys to get the vectors value, because we assign a value and refrence name at the same time,
    the refrence name and value have the same index!

    [FOR PEOPLE WHO WILL READ THE SOURCE CODE]
    refrence = the vector with names
    value    = Is in most cases the content of the dict.

*/
class dict {
protected:
    std::vector<string> mainVector;
    std::vector<string> refrenceVector;

    std::string removeSpaces(std::string input)
    {
        input.erase(std::remove(input.begin(), input.end(), ' '),input.end());
        return input;
    }

public:
    void append(string, string);
    dict (string, string);
    virtual ~dict () {};


    int scanIndex(string);
    std::string get(string);
    std::string removeValue(string);
    std::string changeValue(string, string);
    bool isEmpty();
    std::string move(string, string);
    std::string scanAndFixBrokenBlocks();

};

/*
    constructor...
*/
dict::dict(string allocName, string content) {

    append(allocName, content);
}

/*
    Appends to the vectors
    adding the refrence along with its value
*/
void dict::append(string allocName, string content) {

    if (allocName == STD_CALL_ERROR || content == STD_CALL_ERROR || allocName == DEL_KEY || content == DEL_KEY) {
        cout << "ERROR: INVALID VALUES. PLEASE CHECK VALUES OF \"DEL_KEY\" AND \"STD_CALL_ERROR\"" << endl;
        return;
    }
    if (!refrenceVector.empty()) {
        for (int i = 0; i < refrenceVector.size(); i++) {
            if (refrenceVector[i] == allocName) {
                cout << "TWO NAMES CANNOT BE THE SAME" << endl;
                return;
            }
        }
    }

    // mainVector Holds Content
    mainVector.push_back(content);
    refrenceVector.push_back(allocName);

}

/*
    scans for the value, getting the index value!
*/
int dict::scanIndex(string valueToScanFor) {
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
std::string dict::get(string allocName) {

    if (allocName == DEL_KEY) {
        cout << "ERROR: ACCESSING DELETED VARIABLES IS NOT ALLOWED" << endl;
        return STD_CALL_ERROR;
    }

    int callIndex = scanIndex(allocName);
    if (callIndex == -1) {
        return STD_CALL_ERROR;
    }
    return mainVector[callIndex];
}

/*

    Removes a value from the thingy

*/

std::string dict::removeValue(string allocName) {
    int callIndex = scanIndex(allocName);
    if (callIndex == -1) {
        return STD_CALL_ERROR;
    }

    // <OLD>
    //mainVector[callIndex]     = DEL_KEY;
    //refrenceVector[callIndex] = DEL_KEY;
    // </OLD>
    mainVector.erase(mainVector.begin() + callIndex);
    refrenceVector.erase(refrenceVector.begin() + callIndex);

    return "s";
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
string dict::changeValue(string fromNAME, string toCONTENT) {
    int id  = scanIndex(fromNAME);

    if (id == -1) {
        return VALUE_DOES_NOT_EXIST;
    }

    mainVector[id] = toCONTENT;
    return SUCCESS;
}

/*

    Moves the value from a refrence to another

*/
string dict::move(string fromNAME, string toNAME) {

    int id  = scanIndex(fromNAME);
    int id2 = scanIndex(toNAME);

    string flags = "";

    if (id == -1) {
        return VALUE_DOES_NOT_EXIST;
    }
    if (id2 == -1) {
        flags = flags + "MAKENEW ";
    }

    if (flags.find("MAKENEW") != string::npos) {
        // if we gonna make new
        append(toNAME, mainVector[id]);// id = fromNAME id
        return SUCCESS;
    }
    else {
        // or not
        refrenceVector[id] = mainVector[id2];
        return SUCCESS;
    }

}
std::string dict::scanAndFixBrokenBlocks() {

    bool brokenBlockIndicator1;
    bool brokenBlockIndicator2;
    bool brokenBlockIndicator3;
    bool brokenBlockIndicator_INMAIN1;
    bool brokenBlockIndicator_INMAIN2;
    bool brokenBlockIndicator_INMAIN3;
    string blockInfo = "";
    string noSpacesIndicator;
    string noSpacesIndicator_INMAIN;

    for (int i = 0; i < refrenceVector.size(); i++) {

        noSpacesIndicator = removeSpaces(refrenceVector[i]);
        noSpacesIndicator_INMAIN = removeSpaces(mainVector[i]);
        //cout << "NOSPACE : \"" << noSpacesIndicator << "\"" << endl;
        //cout << "NOSPACE : \"" << noSpacesIndicator_INMAIN << "\"" << endl;

        brokenBlockIndicator1 = noSpacesIndicator == "";
        brokenBlockIndicator2 = refrenceVector[i] == DEL_KEY;
        brokenBlockIndicator3 = refrenceVector[i] == DEFUALT_VALUE;
        brokenBlockIndicator_INMAIN1 = noSpacesIndicator_INMAIN == "";
        brokenBlockIndicator_INMAIN2 = mainVector[i] == DEL_KEY;
        brokenBlockIndicator_INMAIN3 = mainVector[i] == DEFUALT_VALUE;

        if (brokenBlockIndicator1 || brokenBlockIndicator2 || brokenBlockIndicator3 || brokenBlockIndicator_INMAIN1 || brokenBlockIndicator_INMAIN2 || brokenBlockIndicator_INMAIN3) {

            blockInfo = blockInfo + "DETECTED:" + refrenceVector[i] + "! ... ";

            //refrenceVector.erase(refrenceVector.begin() + i);
            //mainVector.erase(mainVector.begin() + i);


            blockInfo = blockInfo + removeValue(mainVector[i]) + " ... ";
            blockInfo = blockInfo + removeValue(refrenceVector[i]) + "\n";

        }
    }

    return blockInfo;
}

#endif // DICT_H
