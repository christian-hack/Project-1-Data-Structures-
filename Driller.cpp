//
//  main.cpp
//  Project 1
//
//  Created by Christian Hackelman on 10/3/19.
//  Copyright © 2019 Christian Hackelman. All rights reserved.
//

#include <iostream>
#include "DrillingRecord.h"
#include "ResizableArray.h"
#include "DrillingRecordComparator.h"
#include "Sorter.h"
#include "Search.h"
#include <vector>
#include <sstream>
#include <iomanip>
#include <fstream>

using namespace std;

bool readFile(ResizableArray<DrillingRecord>& array, int& linesRead, int& validRecords, int& recsInMem, string input, bool& isFirst, string& date, vector<string>& validTimeStamps) {
    vector<string> tempTimestamps;
    vector<string> tempFloaters;
    string time;
    string line;
    string nxtColValue;
    string tempDate;
    string floatValue;
    int duplicateTimeIndex = -1;
    int duplicateTimeCtr = 0;
    int lineCtr = 0;
    int floatValueCtr = 0;
    int validCount = 0;
    bool isReadable;
    bool isFirstDuplicate = false;
    ifstream ifs(input);
    if (ifs.is_open()) {                                    // check if file was opened; otherwise break from loop
        getline(ifs, line);
        //ResizableArray<DrillingRecord> resizedArray = ResizableArray<DrillingRecord>();
        while (getline(ifs, line)) {
            DrillingRecord drillRec;
            ++lineCtr;
            stringstream ss(line);
            if (lineCtr == 1 && isFirst == true) {
                getline(ss, date, ',');
                getline(ss, nxtColValue, ',');
                tempTimestamps.push_back(nxtColValue);
                for (int i = 0; i < 16; ++i) {
                    getline(ss, floatValue, ',');
                    tempFloaters.push_back(floatValue);
                }                                           // push all float values to temporary vector
                for (int j = 0; j < 16; ++j) {
                    if (stod(tempFloaters[j]) < 0) {
                        ++floatValueCtr;
                        cout << "Invalid floating-point data at line " << lineCtr << "." << endl;
                    }
                }                                           // check temporary vector for invalid float values
                if (floatValueCtr == 0) {
                    drillRec.addString(date);
                    drillRec.addString(nxtColValue);
                    for (int k = 0; k < 16; ++k) {
                        drillRec.addNum(stod(tempFloaters[k]));
                    }
                    validTimeStamps.push_back(nxtColValue);
                    array.add(drillRec);
                    ++validCount;
                    tempFloaters.clear();
                    isFirst = false;
                }                                           // if all float values, date, and time are valid add drilling record to array
            }
            else {
                getline(ss, tempDate, ',');
                if (date.compare(tempDate) == 0) {
                    getline(ss, nxtColValue, ',');
                    for (unsigned int s = 0; s < validTimeStamps.size(); ++s) {
                        if (nxtColValue == validTimeStamps[s]) {
                            cout << "Duplicate timestamp " << nxtColValue << " at line " << lineCtr << "." << endl;
                            duplicateTimeIndex = s;
                            ++duplicateTimeCtr;
                        }
                    }
                    tempTimestamps.push_back(nxtColValue);
                    for (int i = 0; i < 16; ++i) {
                        getline(ss, floatValue, ',');
                        tempFloaters.push_back(floatValue);
                    }
                    for (int j = 0; j < 16; ++j) {
                        if (stod(tempFloaters[j]) < 0) {
                            ++floatValueCtr;
                        }
                    }
                    if (floatValueCtr == 0) {
                        drillRec.addString(date);
                        drillRec.addString(nxtColValue);
                        for (int k = 0; k < 16; ++k) {
                            drillRec.addNum(stod(tempFloaters[k]));
                        }
                        if (duplicateTimeCtr < 2) {
                            if (duplicateTimeIndex != -1) {
                                array.replaceAt(drillRec, duplicateTimeIndex);
                                isFirstDuplicate = false;
                                --duplicateTimeCtr;
                            }
                            if (duplicateTimeCtr == 0) {
                                array.add(drillRec);
                                ++validCount;
                                validTimeStamps.push_back(nxtColValue);
                            }
                        }
                        tempFloaters.clear();
                    }
                }
                else {
                    cout << "Date mismatch; file closed." << endl;
                    break;
                }
            }                                                   // repeat process for subsequently entered files
        }
    }
    else {
        isReadable = false;
        return isReadable;
    }
    //end of file open conditional
    linesRead = lineCtr;                                            // assign linesRead variable for total lines read
    validRecords = validCount;                                      // assign validRecords variable for total valid records
    recsInMem = validCount;                                         // assign recsInMem variable for total records stored in memory
    return true;                                                    // return true if file was opened and read in successfully
}

void outputDataManipulation(int validCount, ResizableArray<DrillingRecord>* array, int totalLines, int recordsInMemory) {
    string input;
    cout << "Enter output file name: ";
    getline(cin, input);
    if (input.size() != 0) {                                                // if user enters a file name, execute
        ofstream ofs(input);
        if (ofs.is_open()) {                                                // if file is valid, execute
            for (unsigned int i = 0; i <= array->getSize() -1; ++i) {
                ofs << array->get(i) << endl;
            }
            ofs << "Data lines read: " << totalLines << "; Valid drilling records read: " << validCount << "; Drilling records in memory: " << recordsInMemory << endl;
    }
        else {
            cout << "File is not available." << endl;
            outputDataManipulation(validCount, array, totalLines, recordsInMemory);
        }
    }
    else {                                                                  // if user hits "ENTER", output data using standard cout
        //cout << array.getSize() << endl;
        for (unsigned int i = 0; i <= array->getSize()-1; ++i) {
            cout << array->get(i) << endl;
        }
        cout << "Data lines read: " << totalLines << "; Valid drilling records read: " << validCount << "; Drilling records in memory: " << recordsInMemory << endl;
    }
}

void sortingDataManipulation(ResizableArray<DrillingRecord>* array, int& sortedColumn) {
    string input;
    cout << "Enter sort field (0-17): " << endl;
    getline(cin, input);
    int column = stoi(input);
    if (column < 0 || column > 17) {                              // if user enters invalid number exit function
        return;
    }
    else {                                                                  // if user enter valid number, execute
        if (column < 18 && column > 0) {
            DrillingRecordComparator drc = DrillingRecordComparator(column);
            Sorter<DrillingRecord>::sort(*array, drc);
            sortedColumn = column;
        }
    }
}

void findDataManipulation(ResizableArray<DrillingRecord>* array, int sortedCol) {
    //DrillingRecordComparator timecompare = DrillingRecordComparator(1);
    //Sorter<DrillingRecord> sort;
    string input;
    int recordsFound = 0;
    int index = -1;
    cout << "Enter search field (0-17): " << endl;
    getline(cin, input);
    if (input.size() == 0) {
        return;
    }
    int field = stoi(input);
    double fieldspec;
    if (field < 0 || field > 17) {
        return;
    }
    if (field > 1) {
        input.clear();
        cout << "Enter positive field value: " << endl;
        getline(cin, input);
        if (input.size() == 0) {
            return;
        }
        try {
            fieldspec = stod(input);
        } catch (const exception e) {
            fieldspec = -1;
        }
        if (fieldspec > 0) {
            
            DrillingRecord drNum = DrillingRecord();
            drNum.setNum(fieldspec, field);
            DrillingRecordComparator drc = DrillingRecordComparator(field);
            if (field == sortedCol) {
                index = (int)binarySearch(drNum, *array, drc);
                if (index > 0) {
                    while (array->get(index).getNum(field) == drNum.getNum(field)) {
                        cout << array -> get(index) << endl;
                        ++index;
                        ++recordsFound;
                    }
                    cout << "Drilling records found: " << recordsFound << "." << endl;
                }
            }
            else {
                index = (int)linearSearch(drNum, field, *array, drc);
                if (index > 0) {
                    while (array->get(index).getNum(field) == drNum.getNum(field)) {
                        cout << array-> get(index) << endl;
                        ++index;
                        ++recordsFound;
                    }
                    cout << "Drilling records found: " << recordsFound << "." << endl;
                }
            }
        }
        if (fieldspec < 0) {
            cout << "Drilling records found: 0." << endl;
            return;
        }
    }
    if (field < 2) {
        input.clear();
        cout << "Enter exact text on which to sort: " << endl;
        getline(cin, input);
        if (input.size() == 0) {
            return;
        }
        DrillingRecord drString = DrillingRecord();
        drString.setString(input, field);
        DrillingRecordComparator drc2 = DrillingRecordComparator(field);
        if (field == sortedCol) {
            index = (int)binarySearch(drString, *array, drc2);
            if (index > 0) {
                while (array->get(index).getString(field) == drString.getString(field)) {
                    cout << array -> get(index) << endl;
                    ++index;
                }
            }
        }
        else {
            index = (int)linearSearch(drString, field, *array, drc2);
            if (index > 0) {
                while (array->get(index).getString(field) == drString.getString(field)) {
                    cout << array -> get(index) << endl;
                    ++index;
                }
            }
        }
    }
}

int main() {
    string input = " ";
    string date;
    vector<string> times;
    int totalLines = 0;
    int validRecs = 0;
    int drillRecsInMem = 0;
    bool firstFile = true;
    bool readable = true;
    int sortedCol = 1;
    //vector<string> times;
    ResizableArray<DrillingRecord>* resizedArray = new ResizableArray<DrillingRecord>();
    //DrillingRecordComparator drc = DrillingRecordComparator(1);
    cout << "Enter data file name: ";
    getline(cin, input);
    while (input.size() != 0) {
        readable = readFile(*resizedArray, totalLines, validRecs, drillRecsInMem, input, firstFile, date, times);
        if (readable == false) {
            cout << "File is not available." << endl;
        }
        cout << "Enter data file name: ";
        getline(cin, input);
    }
    input.clear();
    while (input != "q") {
        cout << "Enter (o)utput, (s)ort, (f)ind, or (q)uit: " << endl;
        getline(cin, input);
        if (input == "o") {
            outputDataManipulation(validRecs, resizedArray, totalLines, drillRecsInMem);
        }
        if (input == "s") {
            sortingDataManipulation(resizedArray, sortedCol);
        }
        if (input == "f") {
            findDataManipulation(resizedArray, sortedCol);
        }
        if (input == "q") {
            cout << "Thanks for using Driller." << endl;
        }
    }
    fclose(stdin);
    return 0;
}
