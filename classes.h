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

    explicit period(long long seconds) {
        days = seconds / (86400);
        hrs = (seconds - days * 86400) / 3600;
        min = (seconds - days * 86400 - hrs * 3600) / 60;
        sec = seconds - days * 86400 - hrs * 3600 - min * 60;
    }

    period(long long int days, long long int months, long long int years) : days(days), months(months), years(years) {}

    period(long long int sec, long long int min, long long int hrs, long long int days, long long int months,
           long long int years) : sec(sec), min(min), hrs(hrs), days(days), months(months), years(years) {}

    bool operator==(period &dt) const {
        return to_seconds() == dt.to_seconds();
    }

    bool operator!=(period &dt) const {
        return !(*this == dt);
    }

    bool operator<(period &pd) const {
        return to_seconds() < pd.to_seconds();
    }

    bool operator>(period &pd) const {
        return to_seconds() < pd.to_seconds();
    }

    bool operator<=(period &pd) const {
        return (*this == pd || *this < pd);
    }

    bool operator>=(period &pd) const {
        return (*this == pd || *this > pd);
    }

    period operator+(period &pd) const {
        return period(this->to_seconds() + pd.to_seconds());
    }

    period operator+=(period &pd) {
        *this = period(this->to_seconds() + pd.to_seconds());
        return *this;
    }

    period operator-(period &pd) const {
        return period(this->to_seconds() - pd.to_seconds());
    }

    period operator-=(period &pd) {
        *this = period(this->to_seconds() - pd.to_seconds());
        return *this;
    }

    period operator*(long long times) const {
        return period(this->to_seconds() * times);
    }

    period operator*=(long long times) {
        *this = period(this->to_seconds() * times);
        return *this;
    }

    period operator/(long long times) const {
        return period(this->to_seconds() / times);
    }

    period operator/=(long long times) {
        *this = period(this->to_seconds() / times);
        return *this;
    }

    long long strip_time() {
        period pd = *this;
        pd.sec = 0;
        pd.min = 0;
        pd.hrs = 0;
        return pd.to_seconds();
    }

    [[nodiscard]] long long to_seconds() const {
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

    datetime(long long int day, long long int month, long long int year) : day(day), month(month), year(year) {}

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

    bool operator==(datetime &dt) const {
        return sec == dt.sec &&
               min == dt.min &&
               hrs == dt.hrs &&
               day == dt.day &&
               month == dt.month &&
               year == dt.year;
    }

    bool operator!=(datetime &dt) const {
        return !(*this == dt);
    }

    bool operator<(datetime &dt) const {
        if (year < dt.year)
            return true;
        else if (year == dt.year) {
            if (month < dt.month)
                return true;
            else if (month == dt.month) {
                if (day < dt.day)
                    return true;
                else
                    return false;
            } else
                return false;
        } else
            return false;
    }

    bool operator>(datetime &dt) const {
        if (year > dt.year)
            return true;
        else if (year == dt.year) {
            if (month > dt.month)
                return true;
            else if (month == dt.month) {
                if (day > dt.day)
                    return true;
                else
                    return false;
            } else
                return false;
        } else
            return false;
    }

    bool operator<=(datetime &d2) const {
        return (*this == d2 || *this < d2);
    }

    bool operator>=(datetime &d2) const {
        return (*this == d2 || *this > d2);
    }

    long long seconds_between_years(datetime a2, datetime a1) {
        return a1 == a2 ? 0 : ((a1.is_leap() ? 1 : 0) + ((a2.year - a1.year) * 365 +
                                                         (((a2.year - 1) / 4 - (a1.year) / 4) -
                                                          ((a2.year - 1) / 100 - (a1.year) / 100) +
                                                          ((a2.year - 1) / 400 - (a1.year) / 400)))) * 86400;
    }


    datetime after(datetime start, long long seconds) {

        datetime dt;
        dt.day = start.day;
        dt.month = start.month;
        dt.year = start.year + seconds / (((double) 146097 / 400) * 24 * 60 * 60);
        long long r1 = seconds_between_years(dt, start);
        long long orig = seconds;

        seconds = period(seconds).strip_time();
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


        dt.hrs = start.hrs +
                 (orig - orig / (86400) * 86400) / 3600; //those can overflow but it is in canonic form
        if (dt.hrs >= 24) {
            dt.day++;
            if (dt.day > (dt.is_leap() && dt.month == 2 ? 1 : 0) + days_of_months[dt.month]) {
                dt.month++;
                if (dt.month > 12) {
                    dt.year++;
                    dt.month -= 12;
                }
                dt.day = 1;
            }
            dt.hrs -= 24;
        }

        dt.min = start.min + (orig - orig / (86400) * 86400 - dt.hrs * 3600) / 60;
        if (dt.min >= 60) {
            dt.hrs++;
            if (dt.hrs >= 24) {
                dt.day++;
                if (dt.day > (dt.is_leap() && dt.month == 2 ? 1 : 0) + days_of_months[dt.month]) {
                    dt.month++;
                    if (dt.month > 12) {
                        dt.year++;
                        dt.month -= 12;
                    }
                    dt.day = 1;
                }
                dt.hrs -= 24;
            }
            dt.min -= 60;
        }

        dt.sec = start.sec + orig - orig / (86400) * 86400 - dt.hrs * 3600 - dt.min * 60;
        if (dt.sec >= 60) {
            dt.min++;
            if (dt.min >= 60) {
                dt.hrs++;
                if (dt.hrs >= 24) {
                    dt.day++;
                    if (dt.day > (dt.is_leap() && dt.month == 2 ? 1 : 0) + days_of_months[dt.month]) {
                        dt.month++;
                        if (dt.month > 12) {
                            dt.year++;
                            dt.month -= 12;
                        }
                        dt.day = 1;
                    }
                    dt.hrs -= 24;
                }
                dt.min -= 60;
            }
            dt.sec -= 60;
        }

        return dt;
    }

    datetime before(datetime start, long long seconds) {

        datetime dt;

        dt.year = start.year + seconds / (((double) 146097 / 400) * 24 * 60 * 60);

        dt.month = start.month;
        dt.day = start.day;
        dt.hrs = start.hrs +
                 (seconds - seconds / (86400) * 86400) / 3600; //those can overflow but it is in canonic form
        if (dt.hrs < 0) {
            dt.day--;
            if (dt.day == 0) {
                dt.month--;
                if (dt.month <= 0) {
                    dt.year--;
                    dt.month += 12;
                }
                dt.day = (dt.is_leap() && dt.month == 2 ? 1 : 0) + days_of_months[dt.month];
            }
            dt.hrs += 24;
        }

        dt.min = (seconds - seconds / (86400) * 86400 - dt.hrs * 3600) / 60;
        if (dt.min < 0) {
            dt.hrs--;
            if (dt.hrs < 0) {
                dt.day--;
                if (dt.day == 0) {
                    dt.month--;
                    if (dt.month <= 0) {
                        dt.year--;
                        dt.month += 12;
                    }
                    dt.day = (dt.is_leap() && dt.month == 2 ? 1 : 0) +
                             days_of_months[dt.month];  //TODO: controlla se e' l'anno prima dopo o lo stesso da guardare!
                }
                dt.hrs += 24;
            }
            dt.min += 60;
        }

        dt.sec = seconds - seconds / (86400) * 86400 - dt.hrs * 3600 - dt.min * 60;
        if (dt.sec < 0) {
            dt.min--;
            if (dt.min < 0) {
                dt.hrs--;
                if (dt.hrs < 0) {
                    dt.day--;
                    if (dt.day == 0) {
                        dt.month--;
                        if (dt.month <= 0) {
                            dt.year--;
                            dt.month += 12;
                        }
                        dt.day = (dt.is_leap() && dt.month == 2 ? 1 : 0) +
                                 days_of_months[dt.month];
                    }
                    dt.hrs += 24;
                }
                dt.min += 60;
            }
            dt.sec += 60;
        }
        seconds -= dt.hrs * 3600 + dt.min * 60 + dt.sec;

        long long r1 = seconds_between_years(dt, start);

        long long ddays;
        while (r1 <= seconds) {
            ddays = 365;
            if (((dt.year + 1) % 400 == 0) || ((dt.year + 1) % 4 == 0 && (dt.year + 1) % 100 != 0))
                ddays += 1;
            r1 += ddays * 86400;
            dt.year++;
        }


        if (r1 > seconds) {
            while (r1 > seconds) {
                ddays = 365;
                if ((dt.year % 400 == 0) || (dt.year % 4 == 0 && dt.year % 100 != 0))
                    ddays += 1;
                r1 -= ddays * 86400;
                dt.year--;
            }
            dt.year++;
            ddays = 365;
            if ((dt.year % 400 == 0) || (dt.year % 4 == 0 && dt.year % 100 != 0))
                ddays += 1;
            r1 += ddays * 86400;
        }

        bool leap = (dt.year % 400 == 0) || (dt.year % 4 == 0 && dt.year % 100 != 0);

        if (r1 > seconds) {
            while (r1 > seconds) {
                r1 -= (leap && dt.month == 2 ? 1 + days_of_months[dt.month - 1] : days_of_months[dt.month - 1]) * 86400;
                dt.month--;
            }
            dt.month++;
            r1 += (leap && dt.month == 2 ? 1 + days_of_months[dt.month - 1] : days_of_months[dt.month - 1]) * 86400;
        }

        if (r1 > seconds) {
            while (r1 > seconds) {
                r1 -= 86400;
                dt.day--;
            }
            dt.day++;
        }
        return dt;
    }

    datetime operator+(period &p) {
        return after(*this, p.to_seconds());
    }

    datetime operator+=(period &p) {
        datetime dt = after(*this, p.to_seconds());
        *this = dt;
        return *this;
    }

    datetime operator-(period &p) {
        return before(*this, -p.to_seconds());
    }

    datetime operator-=(period &p) {
        datetime dt = before(*this, -p.to_seconds());
        *this = dt;
        return *this;
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
