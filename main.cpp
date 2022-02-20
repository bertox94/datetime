#include <iostream>
#include <ctime>

using namespace std;

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

long num_anni_bis(int a1, int a2) {
    int r1 = a2 / 4 - a1 / 4;
    int r2 = a2 / 100 - a1 / 100;
    int r3 = a2 / 400 - a1 / 400;

    return r1 - r2 + r3;
}

cl timestamp_to_date(long long timestamp) {

    unsigned long long days = elapsed_since_epoch(timestamp).day;
    unsigned long long days_est1 = 0;
    unsigned long long days_est2 = 0;
    int anno_fin_est = 1970 + timestamp / (365 * 24 * 60 * 60);

    int num_bis = num_anni_bis(1970, anno_fin_est);
    days_est1 = (anno_fin_est - 1970) * 365;
    days_est2 = (anno_fin_est - 1970) * 365 + num_bis;


    return cl();
}


int main() {

    int year = 1970;
    unsigned int month = 1;
    unsigned int day = 1;

    int r = num_anni_bis(1970, 2442);


    cl c;
    for (int i = 1; i < 1000; i++)
        c = timestamp_to_date(253375165696);
    cout << timestamp_to_date2(253375165696);


    return 0;
}


