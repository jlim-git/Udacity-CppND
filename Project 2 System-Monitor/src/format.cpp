#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    string time;
    
    if (seconds > 0) {
        int hr = seconds / 3660;
        int mm = (seconds % 3600) / 60; 
        int ss = seconds % 60;
    
        time = padZero(hr) + ":" + padZero(mm) + ":" + padZero(ss);
    }

    else {
        time = padZero(0) + padZero(0) + padZero(0);
    }       

    return time; 
}


string Format::padZero (int time) {
    string formattedTime = std::to_string(time);

    if (time >= 0 && time < 10) {
        formattedTime = "0" + formattedTime;
    }

    return formattedTime;    
}