#include <iostream>
#include <chrono>
#include "classes.h"

using namespace std;
using namespace chrono;

void test(long long size) {
    auto t1 = high_resolution_clock::now();
    for (long long i = 0; i <= size; i++) {
        //datetime c1 = timestamp_to_date(i);
    }
    auto t2 = high_resolution_clock::now();

    chrono::duration<double, std::milli> ms_double = t2 - t1;
    std::cout << ms_double.count() * 1000000 / size << " ns/op\n";
}

std::ostream &operator<<(std::ostream &os, datetime const &d) {
    return os << d.day << "." << d.month << "." << d.year << ", " << d.hrs << ":" << d.min << ":" << d.sec
              << std::endl;
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


int main() {

    for (int i = 94670856; i < 1000000000; i++) {
        if (!(datetime(i) == timestamp_to_date(i))) {
            cout << "Error: " << i << endl;
            return 0;
        }
    }

    //cout << datetime(253375202774) << endl;

    cout << "Faster!" << endl;

    return 0;
}


