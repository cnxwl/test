#ifndef __HIPACKER_H
#define __HIPACKER_H

#include <string>
#include <time.h>
#include <sys/time.h>
#include <fstream>
#include "include/DLPrinter.h"//print 
#include "include/modbus.h"

using namespace std;
string logger_name;
ofstream logger_file;
device_info *device;//label printer
/*
Function to get timestamp with year-month-day hour:minute:second:milisecond format
Parameter:
time_t nowtime: current time
*/
string get_current_date_time(time_t nowtime)
{
    struct timeval tv;

    struct tm *nowtm;
    char tmbuf[64], buf[64];

    gettimeofday(&tv, NULL);
    nowtime = tv.tv_sec;
    nowtm = localtime(&nowtime);

    //strftime info: https://en.cppreference.com/w/c/chrono/strftime
    strftime(tmbuf, sizeof tmbuf, "%Y-%m-%d-%H:%M:%S", nowtm);
    snprintf(buf, sizeof buf, "%s:%06ld", tmbuf, tv.tv_usec);

    return buf;
}

// Function to generate logging message line by line with current time and level
string logging_generation(string level, string msg)
{
    return get_current_date_time(time(0)) + "-" + level + ": " + msg + "\n";
}


//initialization dascom
int init_open_label_printer()
{
   const char *cpath;
   int number;
   device = (device_info *)malloc(sizeof (device_info));
   setdpi(304);
   int r;
   r = enumDevice(device);
   if(r != 0)
   {
     logger_file << logging_generation("INFO", "Label printer device not found!");
     return -1;
   }
   else
   {
     cout << device -> name << endl;
   }
   r = openDevice(device);
   if(r == 0)
   {
     logger_file << logging_generation("INFO", "open Label printer device Ok!");
     return 0;
   }
    return -1;
}

#endif
