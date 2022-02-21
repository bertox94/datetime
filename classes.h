//
// Created by Halib on 21.02.2022.
//

#ifndef UNTITLED9_CLASSES_H
#define UNTITLED9_CLASSES_H

using namespace std;


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


class datetime {
public:
    int a[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    long long sec;
    long long min;
    long long hrs;
    long long day;
    long long month;
    long long year;

    datetime() = default;

    datetime(long long int timestamp) {
        year = 1970 + timestamp / (((double) 146097 / 400) * 24 * 60 * 60);
        long long r1 = ((year - 1970) * 365 + (((year - 1) / 4 - 1970 / 4) - ((year - 1) / 100 - 1970 / 100) +
                                               ((year - 1) / 400 - 1970 / 400))) * 86400;

        long long ddays = timestamp / (86400);
        hrs = (timestamp - ddays * 86400) / 3600;
        min = (timestamp - ddays * 86400 - hrs * 3600) / 60;
        sec = timestamp - ddays * 86400 - hrs * 3600 - min * 60;

        day = 0;
        timestamp -= hrs * 3600 + min * 60 + sec;

        int i = 0;
        while (r1 > timestamp) {
            ddays = 365;
            if ((i + year % 400 == 0) || (i + year % 4 == 0 && i + year % 100 != 0))
                ddays += 1;
            r1 -= ddays * 86400;
            i++;
            year--;
        }
        //calcola il numero di anni bisestili, e aggiunge un numero di giorni pari al numeri di anni bisestili
        r1 = ((year - 1970) * 365 + (((year - 1) / 4 - 1970 / 4) - ((year - 1) / 100 - 1970 / 100) +
                                     ((year - 1) / 400 - 1970 / 400))) * 86400;

        i = 0;
        bool fg = false;
        while (r1 <= timestamp) {
            ddays = 365;
            if ((i + year % 400 == 0) || (i + year % 4 == 0 && i + year % 100 != 0))
                ddays += 1;
            r1 += ddays * 86400;
            i++;
            fg = true;
        }
        if (fg) {
            i--;
            year += i;
            ddays = 365;
            if ((i + year % 400 == 0) || (i + year % 4 == 0 && i + year % 100 != 0))
                ddays += 1;
            r1 -= ddays * 86400;
        }

        bool leap = (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);

        i = 1;
        fg = false;
        while (r1 <= timestamp) {
            r1 += (leap && i == 2 ? 1 + a[i - 1] : a[i - 1]) * 86400;
            i++;
            fg = true;
        }
        if (fg) {
            i--;
            r1 -= (leap && i == 2 ? 1 + a[i - 1] : a[i - 1]) * 86400;
        }
        month = i;

        i = 1;
        fg = false;
        while (r1 <= timestamp) {
            r1 += 86400;
            i++;
            fg = true;
        }
        if (fg) {
            i--;
        }
        day = i;
    }

    bool operator==(date &&dt) const {
        return sec == dt.sec &&
               min == dt.min &&
               hrs == dt.hrs &&
               day == dt.day &&
               month == dt.month &&
               year == dt.year;
    }
};


#endif //UNTITLED9_CLASSES_H
