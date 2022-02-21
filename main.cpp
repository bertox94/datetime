#include <iostream>
#include <ctime>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace chrono;

class cl {
public:

    unsigned int sec = NULL;
    unsigned int min = NULL;
    unsigned int hrs = NULL;
    unsigned int day = NULL;
    unsigned int month = NULL;
    int year = NULL;

    bool operator==(cl &other) const {
        return sec == other.sec &&
               min == other.min &&
               hrs == other.hrs &&
               day == other.day &&
               month == other.month;
    }

};

std::ostream &operator<<(std::ostream &os, cl const &d) {
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

long year_to_timestamp(int year) {
    long var = (year - 1970) * 365 + num_anni_bis(1970, year - 1);
    return var * 86400;
}

cl timestamp_to_date2(long long timestamp) {

    //1a ottimizzazione. Le ore minuti e secondi aggiunti alla fine, for solo suli giorni
    //2a ottimizzazione. Skippa di (1 anno a[]) ad ogni for e poi aggiusta
    //3a ottimizzazione. Calcola approssimativamente (double) il numero di anni e poi aggiusta
    int a[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    auto el = elapsed_since_epoch(timestamp);

    long days = el.day;
    int year = 1970;
    int day = 1;
    int month = 1;
    int hrs = el.hrs;
    int min = el.min;
    int sec = el.sec;

    for (; day <= days; day++) {
        int days_of_month = a[month - 1];
        if (is_leap_year(year) && month == 2)
            days_of_month++;
        if (day > days_of_month) {
            day = 1;
            days -= days_of_month;
            month++;
            if (month == 13) {
                month = 1;
                year++;
            }
        }
    }
    int days_of_month = a[month - 1];
    if (is_leap_year(year) && month == 2)
        days_of_month++;
    if (day > days_of_month) {
        day = 1;
        month++;
        if (month == 13) {
            month = 1;
            year++;
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

cl extract_time(long long timestamp) {
    cl c;
    auto cc = elapsed_since_epoch(timestamp);
    c.hrs = cc.hrs;
    c.min = cc.min;
    c.sec = cc.sec;
    return c;
}

long long strip_time(long long timestamp) {
    cl c = elapsed_since_epoch(timestamp);
    c.year = 0;
    c.month = 0;
    c.day = 0;
    return timestamp - elapsed_since_epoch_inverse(0, c.hrs, c.min, c.sec);
}

cl timestamp_to_date(long long timestamp) {

    int a[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int anno_fin_est = 1970 + timestamp / (((double) 146097 / 400) * 24 * 60 * 60);
    long long r1 = year_to_timestamp(anno_fin_est);
    cl res = extract_time(timestamp);
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

void f1(long long size) {
    auto t1 = high_resolution_clock::now();
    for (long long i = 0; i <= size; i++) {
        cl c1 = timestamp_to_date(i);
    }
    auto t2 = high_resolution_clock::now();

    duration<double, std::milli> ms_double = t2 - t1;
    std::cout << ms_double.count() * 1000000 / size << " ns/op\n";
}

void f2(long long size) {
    auto t1 = high_resolution_clock::now();
    for (long long i = 0; i <= size; i++) {
        cl c1 = timestamp_to_date2(i);
    }
    auto t2 = high_resolution_clock::now();

    duration<double, std::milli> ms_double = t2 - t1;
    std::cout << ms_double.count() * 1000000 / size << " ns/op\n";
}


int main() {

    int year = 1970;
    unsigned int month = 1;
    unsigned int day = 1;

    int r = num_anni_bis(1970, 2442);


    //cl c;
    //for (int i = 1; i < 1000; i++)
    long long timestamp = 86400;
    //cout << timestamp_to_date2(timestamp);
    //cout << timestamp_to_date(timestamp);


    long long size = 999999999;

    f1(size);
    //f2(size);

    cout << "Faster!" << endl;

    return 0;
}


