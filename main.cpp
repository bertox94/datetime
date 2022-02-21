#include <iostream>
#include <ctime>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace chrono;

class date {
public:
// since those can expresss also a duration, it make sense to use long long (e.g. when huge number of days)

    unsigned int sec = NULL;
    unsigned int min = NULL;
    unsigned int hrs = NULL;
    unsigned int day = NULL;
    unsigned int month = NULL;
    long long year = NULL;

    bool operator==(date &other) const {
        return sec == other.sec &&
               min == other.min &&
               hrs == other.hrs &&
               day == other.day &&
               month == other.month &&
               year == other.year;
    }

};
//It is not always true that (with start1!=start2) start_1 + duration == start2 + duration, but given two dates we may need the number of months, years, between them
//can be negative and not necessarily in canonic form (sec<=60), for example if I want to know how many seconds from an event.
class duration {
    long long sec = NULL;
    long long min = NULL;
    long long hrs = NULL;
    long long day = NULL;

    bool operator==(date &other) const {
        return sec == other.sec &&
               min == other.min &&
               hrs == other.hrs &&
               day == other.day;
    }
};

std::ostream &operator<<(std::ostream &os, date const &d) {
    return os << d.day << "." << d.month << "." << d.year << ", " << d.hrs << ":" << d.min << ":" << d.sec << endl;
}

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

long num_anni_bis(int a1, int a2) {
    int r1 = a2 / 4 - a1 / 4;
    int r2 = a2 / 100 - a1 / 100;
    int r3 = a2 / 400 - a1 / 400;

    return r1 - r2 + r3;
}

date elapsed_since_epoch(long long timestamp) {

    long long days = timestamp / (86400);
    long long hrs = (timestamp - days * 86400) / 3600;
    long long min = (timestamp - days * 86400 - hrs * 3600) / 60;
    long long sec = timestamp - days * 86400 - hrs * 3600 - min * 60;

    //cout << "Elapsed: Days: " << days << ", Hrs: " << hrs << ", Min: " << min << ", Sec: " << sec << endl;

    date c;
    c.day = days;
    c.hrs = hrs;
    c.min = min;
    c.sec = sec;

    return c;
}

long elapsed_since_epoch_inverse(int days, int hrs, int min, int sec) {
    return days * 86400 + hrs * 3600 + min * 60 + sec;
}

long long year_to_timestamp(int year) {
    long long var = (year - 1970) * 365 + num_anni_bis(1970, year - 1);
    return var * 86400;
}

date extract_time(long long timestamp) {
    date c;
    auto cc = elapsed_since_epoch(timestamp);
    c.hrs = cc.hrs;
    c.min = cc.min;
    c.sec = cc.sec;
    return c;
}

long long strip_time(long long timestamp) {
    date c = elapsed_since_epoch(timestamp);
    c.year = 0;
    c.month = 0;
    c.day = 0;
    return timestamp - elapsed_since_epoch_inverse(0, c.hrs, c.min, c.sec);
}

date timestamp_to_date(long long timestamp) {

    int a[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int anno_fin_est = 1970 + timestamp / (((double) 146097 / 400) * 24 * 60 * 60);
    long long r1 = year_to_timestamp(anno_fin_est);
    date res = extract_time(timestamp);
    timestamp = strip_time(timestamp);


    int i = 0;
    while (elapsed_since_epoch(r1).day > elapsed_since_epoch(timestamp).day) {
        r1 -= (is_leap_year(i + anno_fin_est) ? 366 : 365) * 86400;
        i++;
        anno_fin_est--;
    }
    res.year = anno_fin_est;
    r1 = year_to_timestamp(anno_fin_est);

    //jump years, months, days, aggiungi data ora

    long long r2 = r1;
    i = 0;
    bool fg = false;
    while (r2 <= timestamp) {
        int tmp = (is_leap_year(i + anno_fin_est) ? 366 : 365);
        r2 += tmp * 86400;
        i++;
        fg = true;
    }
    if (fg) {
        i--;
        res.year += i;
        int tmp = (is_leap_year(i + anno_fin_est) ? 366 : 365);
        r2 -= tmp * 86400;
    }


    i = 1;
    fg = false;
    while (r2 <= timestamp) {
        int tmp = (is_leap_year(anno_fin_est) && i == 2 ? 1 + a[i - 1] : a[i - 1]);
        r2 += tmp * 86400;
        i++;
        fg = true;
    }
    if (fg) {
        i--;
        int tmp = (is_leap_year(anno_fin_est) && i == 2 ? 1 + a[i - 1] : a[i - 1]);
        r2 -= tmp * 86400;
    }
    res.month = i;

    i = 1;
    fg = false;
    while (r2 <= timestamp) {
        r2 += 86400;
        i++;
        fg = true;
    }
    if (fg) {
        i--;
        r2 -= 86400;
    }
    res.day = i;

    return res;
}

void test(long long size) {
    auto t1 = high_resolution_clock::now();
    for (long long i = 0; i <= size; i++) {
        date c1 = timestamp_to_date(i);
    }
    auto t2 = high_resolution_clock::now();

    chrono::duration<double, std::milli> ms_double = t2 - t1;
    std::cout << ms_double.count() * 1000000 / size << " ns/op\n";
}


int main() {

    int year = 1970;
    unsigned int month = 1;
    unsigned int day = 1;

    int r = num_anni_bis(1970, 2442);


    //date c;
    //for (int i = 1; i < 1000; i++)
    long long timestamp = 86400;
    //cout << timestamp_to_date2(timestamp);
    //cout << timestamp_to_date(timestamp);




    long long size = 999999;
    //test(size);
    //f2(size);

    cout << timestamp_to_date(253375202774) << endl;

    cout << "Faster!" << endl;

    return 0;
}


