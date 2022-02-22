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

    datetime(long long sec, long long min, long long hrs, long long day, long long month, long long year) : sec(sec),
                                                                                                            min(min),
                                                                                                            hrs(hrs),
                                                                                                            day(day),
                                                                                                            month(month),
                                                                                                            year(year) {}

    explicit datetime(long long timestamp) {
        datetime dt = after(datetime(0, 0, 0, 1, 1, 1970), timestamp);
        *this = dt;
    }

    datetime after(datetime start, long seconds) {

        datetime dt;

        dt.year = start.year + seconds / (((double) 146097 / 400) * 24 * 60 * 60);
        long long r1 = ((dt.year - start.year) * 365 +
                        (((dt.year - 1) / 4 - start.year / 4) - ((dt.year - 1) / 100 - start.year / 100) +
                         ((dt.year - 1) / 400 - start.year / 400))) * 86400;
        dt.month = start.month;
        dt.day = start.day;
        dt.hrs = start.hrs +
                 (seconds - seconds / (86400) * 86400) / 3600; //those can overflow but it is in canonic form
        if (dt.hrs >= 24) {
            dt.hrs -= 24;
            dt.day++;
        }
        dt.min = (seconds - seconds / (86400) * 86400 - dt.hrs * 3600) / 60;
        if (dt.min >= 60) {
            dt.min -= 60;
            if (dt.hrs >= 24) {
                dt.hrs -= 24;
                dt.day++;
            }
        }
        dt.sec = seconds - seconds / (86400) * 86400 - dt.hrs * 3600 - dt.min * 60;
        if (dt.sec >= 60) {
            dt.sec -= 60;
            if (dt.min >= 60) {
                dt.min -= 60;
                if (dt.hrs >= 24) {
                    dt.hrs -= 24;
                    dt.day++;
                }
            }
        }
        seconds -= dt.hrs * 3600 + dt.min * 60 + dt.sec;

        long long ddays;
        while (r1 > seconds) {
            ddays = 365;
            if (((dt.year - 1) % 400 == 0) || ((dt.year - 1) % 4 == 0 && (dt.year - 1) % 100 != 0))
                ddays += 1;
            r1 -= ddays * 86400;
            dt.year--;
        }

        if (r1 <= seconds) {
            while (r1 <= seconds) {
                ddays = 365;
                if ((dt.year % 400 == 0) || (dt.year % 4 == 0 && dt.year % 100 != 0))
                    ddays += 1;
                r1 += ddays * 86400;
                dt.year++;
            }
            dt.year--;
            ddays = 365;
            if ((dt.year % 400 == 0) || (dt.year % 4 == 0 && dt.year % 100 != 0))
                ddays += 1;
            r1 -= ddays * 86400;
        }

        bool leap = (dt.year % 400 == 0) || (dt.year % 4 == 0 && dt.year % 100 != 0);

        if (r1 <= seconds) {
            while (r1 <= seconds) {
                r1 += (leap && dt.month == 2 ? 1 + days_of_months[dt.month - 1] : days_of_months[dt.month - 1]) * 86400;
                dt.month++;
            }
            dt.month--;
            r1 -= (leap && dt.month == 2 ? 1 + days_of_months[dt.month - 1] : days_of_months[dt.month - 1]) * 86400;
        }

        if (r1 <= seconds) {
            while (r1 <= seconds) {
                r1 += 86400;
                dt.day++;
            }
            dt.day--;
        }
        return dt;
    }

    datetime operator+(period &p) {
        datetime dt = *this;
        long long timestamp = p.to_seconds();


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
