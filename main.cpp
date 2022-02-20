#include <iostream>
#include <ctime>
#include <utility>
#include <vector>
#include <fstream>
#include <iomanip>
#include <list>
#include <cmath>
#include <sstream>
#include <chrono>

using namespace std;

bool is_leap_year(int year) {

    if (year % 4 == 0)
        if (year % 100 == 0)
            if (year % 400 == 0)
                return true;
            else
                return false;
        else
            return true;
    else
        return false;

}

class cl {
public:

    unsigned int sec = NULL;
    unsigned int min = NULL;
    unsigned int hrs = NULL;
    unsigned int day = NULL;
    unsigned int month = NULL;
    int year = NULL;

};

std::ostream &operator<<(std::ostream &os, cl const &d) {
    return os << d.day << "." << d.month << "." << d.year << ", " << d.hrs << ":" << d.min << ":" << d.sec << endl;
}


cl elapsed_since_epoch(long timestamp) {

    int days = timestamp / (86400);
    int hrs = (timestamp - days * 86400) / 3600;
    int min = (timestamp - days * 86400 - hrs * 3600) / 60;
    int sec = timestamp - days * 86400 - hrs * 3600 - min * 60;

    cout << "Elapsed: Days: " << days << ", Hrs: " << hrs << ", Min: " << min << ", Sec: " << sec << endl;

    cl c;
    c.day = days;
    c.hrs = hrs;
    c.min = min;
    c.sec = sec;

    return c;
}

long elapsed_since_epoch_inverse(int days, int hrs, int min, int sec) {
    return days * 86400 + hrs * 3600 + min * 60 + sec;
}

cl timestamp_to_date(long timestamp) {

    //1a ottimizzazione. Le ore minuti e secondi aggiunti alla fine, for solo suli giorni

    int a[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int b[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    int year = 1970;
    int month = 1;
    int day = 1;
    int hrs = 0;
    int min = 0;
    int sec = 0;

    int i = 1;
    for (; i <= timestamp; i++) {
        if (i % 60 == 0) {
            min++;
            i = 0;
            timestamp -= 60;
            if (min % 60 == 0) {
                hrs++;
                min = 0;
                if (hrs % 24 == 0) {
                    day++;
                    hrs = 0;
                    if (is_leap_year(year)) {
                        if (day > b[month - 1]) {
                            month++;
                            if (month == 13) {
                                month = 1;
                                year++;
                            }
                            day = 1;
                        }
                    } else {
                        if (day > a[month - 1]) {
                            month++;
                            if (month == 13) {
                                month = 1;
                                year++;
                            }
                            day = 1;
                        }
                    }
                }

            }
        }
    }

    cl c;
    c.year = year;
    c.month = month;
    c.day = day;
    c.hrs = hrs;
    c.min = min;
    c.sec = i - 1;

    return c;

}


int main() {

    int year = 1970;
    unsigned int month = 1;
    unsigned int day = 1;

    //elapsed_since_epoch(31536001);
    cout << timestamp_to_date(1645394445);

    return 0;
}


