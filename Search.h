//
//  Search.hpp
//  Project 1
//
//  Created by Christian Hackelman on 10/3/19.
//  Copyright © 2019 Christian Hackelman. All rights reserved.
//

// Signature must be retained. Implementation must be added.

#ifndef SEARCH_H
#define SEARCH_H

#include "Exceptions.h"
#include "ResizableArray.h"
#include "Comparator.h"

// returns FIRST array location matching the given item (based on the comparator)
// if not found, returns -(location to insert + 1)
template <typename T>
long long binarySearch(const T& item, const ResizableArray<T>& array, const Comparator<T>& comparator) {
    // implementation goes here
    long long mid = 0;
    long long low = 0;
    long long ret = 0;
    long long high = (int)array.getSize() - 1;
    
    while (high >= low) {
        mid = (high + low)/2;
        if (comparator.compare(array.get(mid), item) == -1) {
            low = mid + 1;
        }
        else if (comparator.compare(array.get(mid), item) == 1) {
            high = mid - 1;
        }
        else if (comparator.compare(array.get(mid), item) == 0){
            while (comparator.compare(array.get(mid), item) == 0) {
                --mid;
            }
            ret = mid + 1;
        }
        else {
            ret = (mid + 1) * -1;
        }
    }
    return ret;
}
template <typename T>
long long linearSearch(const T& item, int index, const ResizableArray<T>& array, const Comparator<T>& comparator) {
    for (unsigned int i = index; i < array.getSize(); i++) {
        if (comparator.compare(array[i], item) == 0) {
        return i;
        }
    }
    return -1;
}
#endif
