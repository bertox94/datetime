//
// Created by Halib on 21.02.2022.
//

#ifndef UNTITLED9_CLASSES_H
#define UNTITLED9_CLASSES_H

using namespace std;

class period {
public:

    long long sec{};
    long long min{};
    long long hrs{};
    long long days{};
    long long months{};
    long long years{};

    period() = default;

    explicit period(long long timestamp) {
        days = timestamp / (86400);
        hrs = (timestamp - days * 86400) / 3600;
        min = (timestamp - days * 86400 - hrs * 3600) / 60;
        sec = timestamp - days * 86400 - hrs * 3600 - min * 60;
    }

    long long to_seconds() {
        return days * 86400 + hrs * 3600 + min * 60 + sec;
    }


};

class datetime {
public:
    int days_of_months[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    long long sec{};
    long long min{};
    long long hrs{};
    long long day{};
    long long month{};
    long long year{};

    datetime() = default;

    explicit datetime(long long timestamp) {
        year = 1970 + timestamp / (((double) 146097 / 400) * 24 * 60 * 60);
        long long r1 = ((year - 1970) * 365 + (((year - 1) / 4 - 1970 / 4) - ((year - 1) / 100 - 1970 / 100) +
                                               ((year - 1) / 400 - 1970 / 400))) * 86400;
        month = 1;
        day = 1;
        hrs = (timestamp - timestamp / (86400) * 86400) / 3600;
        min = (timestamp - timestamp / (86400) * 86400 - hrs * 3600) / 60;
        sec = timestamp - timestamp / (86400) * 86400 - hrs * 3600 - min * 60;
        timestamp -= hrs * 3600 + min * 60 + sec;

        long long ddays;
        while (r1 > timestamp) {
            ddays = 365;
            if (((year - 1) % 400 == 0) || ((year - 1) % 4 == 0 && (year - 1) % 100 != 0))
                ddays += 1;
            r1 -= ddays * 86400;
            year--;
        }

        if (r1 <= timestamp) {
            while (r1 <= timestamp) {
                ddays = 365;
                if ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0))
                    ddays += 1;
                r1 += ddays * 86400;
                year++;
            }
            year--;
            ddays = 365;
            if ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0))
                ddays += 1;
            r1 -= ddays * 86400;
        }

        bool leap = (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);

        if (r1 <= timestamp) {
            while (r1 <= timestamp) {
                r1 += (leap && month == 2 ? 1 + days_of_months[month - 1] : days_of_months[month - 1]) * 86400;
                month++;
            }
            month--;
            r1 -= (leap && month == 2 ? 1 + days_of_months[month - 1] : days_of_months[month - 1]) * 86400;
        }

        if (r1 <= timestamp) {
            while (r1 <= timestamp) {
                r1 += 86400;
                day++;
            }
            day--;
        }
    }

    datetime operator+(period &p) {
        datetime dt = *this;
        dt.year += p.years;

        dt.month += p.months;
        dt.year += dt.month / 12;
        dt.month = (dt.month % 12 == 0 ? 12 : dt.month - (dt.month / 12) * 12);


        dt.day += p.days;

        long long estimated_year = year = 1970 + dt.day / (((double) 146097 / 400));
        long long r1 = (year - 1970) * 365 + (((year - 1) / 4 - 1970 / 4) - ((year - 1) / 100 - 1970 / 100) +
                                               ((year - 1) / 400 - 1970 / 400));


    }

    long long to_timestamp() {
        long long res = (year - 1970) * 365 + (((year - 1) / 4 - 1970 / 4) - ((year - 1) / 100 - 1970 / 100) +
                                               ((year - 1) / 400 - 1970 / 400));
        for (int i = 1; i < month; i++) {
            res += days_of_months[i - 1];
        }
        if (month > 2 && is_leap())
            res += 1;
        return (res + day - 1) * 86400 + hrs * 3600 + min * 60 + sec;
    }

    bool is_leap() {
        return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
    }


};


#endif //UNTITLED9_CLASSES_H
