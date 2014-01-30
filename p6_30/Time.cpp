// Author: Justin Fung

#include <ctime>
#include <iostream>
#include <iomanip>
#include "Time.h"

using namespace std;

Time::Time()
{
  update();
} // Time()


void Time::print() const
{
  static const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

  cout << months[month] << " " << setw(2)
    << day << ' '
    << setfill('0') << setw(2)
    << hour << ':' << setw(2)
    << minute << ':' << setw(2) << second
    << setfill(' ');
} // print()


void Time::update()
{
  time_t rawtime;
  struct tm * timeinfo;
  time( &rawtime );
  timeinfo = localtime ( &rawtime );
  month = timeinfo->tm_mon;
  day = timeinfo->tm_mday;
  hour = timeinfo->tm_hour;
  minute = timeinfo->tm_min;
  second = timeinfo->tm_sec;
} // Time()


ostream& operator<< (ostream &os, const Time &time)
{
  os << time.month << ' ' << time.day << ' ' << time.hour << ' '
    << time.minute << ' ' << time.second << ' ';
  return os;
} // operator<<


istream& operator>> (istream &is, Time &time)
{
  is >> time.month >> time.day >> time.hour >> time.minute >> time.second;
  is.ignore(); // eat up last space char
  return is;
} // operator>>