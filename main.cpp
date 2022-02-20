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

    unsigned long long sec = NULL;
    unsigned long long min = NULL;
    unsigned long long hrs = NULL;
    unsigned long long day = NULL;
    unsigned long long month = NULL;
    long long year = NULL;

};

std::ostream &operator<<(std::ostream &os, cl const &d) {
    return os << d.day << "." << d.month << "." << d.year << ", " << d.hrs << ":" << d.min << ":" << d.sec << endl;
}


cl elapsed_since_epoch(long long timestamp) {

    long long days = timestamp / (86400);
    long long hrs = (timestamp - days * 86400) / 3600;
    long long min = (timestamp - days * 86400 - hrs * 3600) / 60;
    long long sec = timestamp - days * 86400 - hrs * 3600 - min * 60;

    //cout << "Elapsed: Days: " << days << ", Hrs: " << hrs << ", Min: " << min << ", Sec: " << sec << endl;

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

cl timestamp_to_date(long long timestamp) {

    //1a ottimizzazione. Le ore minuti e secondi aggiunti alla fine, for solo suli giorni
    //2a ottimizzazione. Skippa di (1 anno a[]) ad ogni for e poi aggiusta
    //3a ottimizzazione. Calcola approssimativamente (double) il numero di anni e poi aggiusta
    int a[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int b[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    auto el = elapsed_since_epoch(timestamp);

    long days = el.day;
    int year = 1970;
    int day = 1;
    int month = 1;
    int hrs = el.hrs;
    int min = el.min;
    int sec = el.sec;

    for (; day <= days; day++) {
        if (is_leap_year(year)) {
            if (day > b[month - 1]) {
                day = 1;
                days -= b[month - 1];
                month++;
                if (month == 13) {
                    month = 1;
                    year++;
                }
            }
        } else {
            if (day > a[month - 1]) {
                day = 1;
                days -= a[month - 1];
                month++;
                if (month == 13) {
                    month = 1;
                    year++;
                }
            }
        }
    }
    if (is_leap_year(year)) {
        if (day > b[month - 1]) {
            day = 1;
            month++;
            if (month == 13) {
                month = 1;
                year++;
            }
        }
    } else {
        if (day > a[month - 1]) {
            day = 1;
            month++;
            if (month == 13) {
                month = 1;
                year++;
            }
        }
    }

    cl c;
    c.year = year;
    c.month = month;
    c.day = day;
    c.hrs = hrs;
    c.min = min;
    c.sec = sec;

    return c;

}


int main() {

    int year = 1970;
    unsigned int month = 1;
    unsigned int day = 1;

    //elapsed_since_epoch(31536001);
    cout << timestamp_to_date(253375165696);

    return 0;
}


