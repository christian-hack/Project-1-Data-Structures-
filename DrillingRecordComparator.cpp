//
//  DrillingRecordComparator.cpp
//  Project 1
//
//  Created by Christian Hackelman on 10/3/19.
//  Copyright © 2019 Christian Hackelman. All rights reserved.
//

#include "DrillingRecordComparator.h"
#include <iostream>
using namespace std;

DrillingRecordComparator::DrillingRecordComparator(unsigned int column) {
    //DrillingRecordComparator::column = column;
    this->column = column;
}
int DrillingRecordComparator::compare(const DrillingRecord &item1, const DrillingRecord &item2)const {
    if (column == 0) {
        string time1 = item1.getString(0);
        string time2 = item2.getString(0);
        if (time1 < time2) {
            return -1;
        }
        else if (time1 > time2) {
            return 1;
        }
        else {
            return 0;
        }
    }
    if (column == 1) {
        string time1 = item1.getString(1);
        string time2 = item2.getString(1);
        if (time1 < time2) {
            return -1;
        }
        else if (time1 > time2) {
            return 1;
        }
        else {
            return 0;
        }
    }
    else if (column >= 2) {
        if (item1.getNum(column-2) < item2.getNum(column-2)) {
            return -1;
        }
        else if (item1.getNum(column-2) > item2.getNum(column-2)) {
            return 1;
        }
        else {
            return 0;
        }
    }
    else {
        return 0;
    }
}
