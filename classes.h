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
private:
    datetime afterI(datetime start, long long seconds) {

        datetime dt;
        dt.day = start.day;
        dt.month = start.month;
        dt.year = start.year;

        while (seconds > 0) {
            ++dt;
            seconds--;
        }

        return dt;
    }

    datetime afterII(datetime start, long long seconds) {

        long long estimated_final_year = start.year + seconds / (((double) 146097 / 400) * 24 * 60 * 60);


        return datetime();
    }
/*
    datetime before2(datetime start, long long seconds) {
        //assume start is always 1.1.1970
        datetime dt;
        dt.day = start.day;
        dt.month = start.month;
        dt.year = start.year + seconds / (((double) 146097 / 400) * 24 * 60 * 60);
        long long r1 = seconds_to(dt, start);
        long long orig = seconds;

        seconds = period(seconds).strip_time();
        long long ddays;
        while (r1 < seconds) {
            ddays = 365;
            if (dt.is_leap())
                ddays += 1;
            r1 += ddays * 86400;
            dt.year++;
        }
        r1 = dt.seconds_from_epoch();//assume always from epoch here

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
        r1 = dt.seconds_from_epoch();

        if (r1 > seconds) {
            while (r1 > seconds) {
                r1 -= (dt.is_leap() && dt.month == 2 ? 1 + days_of_months[dt.month - 1] : days_of_months[dt.month -
                                                                                                         1]) * 86400;
                dt.month--;
                if (dt.month <= 0) {
                    dt.year--;
                    dt.month += 12;
                }
            }
            dt.month++;
            if (dt.month > 12) {
                dt.year++;
                dt.month -= 12;
            }
            r1 += (dt.is_leap() && dt.month == 2 ? 1 + days_of_months[dt.month - 1] : days_of_months[dt.month - 1]) *
                  86400;
        }
        r1 = dt.seconds_from_epoch();

        if (r1 >= seconds) {
            while (r1 >= seconds) {
                r1 -= 86400;
                dt.day--;
                if (dt.day == 0) {
                    dt.month--;
                    if (dt.month <= 0) {
                        dt.year--;
                        dt.month += 12;
                    }
                    dt.day = (dt.is_leap() && dt.month == 2 ? 1 : 0) + days_of_months[dt.month - 1];
                }
            }
            dt.day++;
            if (dt.day > days_of_months[dt.month - 1] + (dt.is_leap() && dt.month == 2 ? 1 : 0)) {
                dt.month++;
                if (dt.month > 12) {
                    dt.year++;
                    dt.month -= 12;
                }
                dt.day = 1;
            }
        }

        int hhrs = start.hrs + (orig - orig / (86400) * 86400) / 3600;
        dt.hrs = hhrs;
        if (dt.hrs < 0) {
            dt.day--;
            if (dt.day == 0) {
                dt.month--;
                if (dt.month <= 0) {
                    dt.year--;
                    dt.month += 12;
                }
                dt.day = (dt.is_leap() && dt.month == 2 ? 1 : 0) + days_of_months[dt.month - 1];
            }
            dt.hrs += 24;
        }

        int mmin = (orig - orig / (86400) * 86400 - hhrs * 3600) / 60;
        dt.min = mmin;
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
                    dt.day = (dt.is_leap() && dt.month == 2 ? 1 : 0) + days_of_months[dt.month - 1];
                }
                dt.hrs += 24;
            }
            dt.min += 60;
        }

        int ssec = orig - orig / (86400) * 86400 - hhrs * 3600 - mmin * 60;
        dt.sec = ssec;
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
                        dt.day = (dt.is_leap() && dt.month == 2 ? 1 : 0) + dt.days_of_months[dt.month - 1];
                    }
                    dt.hrs += 24;
                }
                dt.min += 60;
            }
            dt.sec += 60;
        }

        return dt;
    }
*/

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
        datetime dt = afterI(datetime(0, 0, 0, 1, 1, 1970), timestamp);
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

    bool operator==(datetime &&dt) const {
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

    bool operator!=(datetime &&dt) const {
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


    long long days_between_beginning_of_years(datetime end, datetime start) {

        if (start.year == end.year)
            return 0;

        long long var = 0;

        if (start.is_leap())
            var++;
        var += (end.year - start.year) * 365 +
               (((end.year - 1) / 4 - (start.year) / 4) -
                ((end.year - 1) / 100 - (start.year) / 100) +
                ((end.year - 1) / 400 - (start.year) / 400));

        return var;
    }


    long long days_between_end_of_years(datetime end, datetime start) {

        if (start.year == end.year)
            return 0;

        long long var = 0;

        if (end.is_leap())
            var++;

        var += (end.year - start.year) * 365 +
               (((end.year - 1) / 4 - (start.year) / 4) -
                ((end.year - 1) / 100 - (start.year) / 100) +
                ((end.year - 1) / 400 - (start.year) / 400));

        return var;
    }

    long long seconds_to(datetime dt) const {


        if (year == dt.year) {
            if (month == dt.month) {
                if (day == dt.day) {
                    if (hrs == dt.hrs) {
                        if (min == dt.min) {
                            return dt.sec - sec;
                        } else {
                            long long ssec = dt.sec - sec;
                            long long mmin = dt.min - min;
                            return ssec + mmin * 60;
                        }
                    } else {

                    }
                } else {

                }
            } else {

            }
        } else {

        }


        long long time = 86400 - sec - min * 60 - hrs * 3600;

        long long days = days_of_this_month() - day;

        for (long long i = month + 1; i <= 12; i++) {
            days += days_of_months[i - 1];
        }
        if (is_leap() && month > 2)
            days++;

        if (year != dt.year) {
            if (is_leap())
                days++;

            days += (dt.year - year) * 365 +
                    (((dt.year - 1) / 4 - (year) / 4) -
                     ((dt.year - 1) / 100 - (year) / 100) +
                     ((dt.year - 1) / 400 - (year) / 400));
        }
        return time + days * 86400;

    }

    long long seconds_to_end_of_year(datetime start, datetime end) {
        return days_between_end_of_years(end, start) * 86400;
    }

    int days_of_this_month() const {
        return (is_leap() && month == 2 ? 1 : 0) + days_of_months[month - 1];
    }

    datetime operator++() {
        sec++;
        if (sec == 60) {
            min++;
            if (min == 60) {
                hrs++;
                if (hrs == 24) {
                    day++;
                    if (day > days_of_this_month()) {
                        month++;
                        if (month == 13) {
                            year++;
                        }
                        month = 1;
                    }
                    day = 1;
                }
                hrs = 0;
            }
            sec = 0;
        }
        return *this;
    }

    datetime operator--() {
        sec++;
        if (sec == 60) {
            min++;
            if (min == 60) {
                hrs++;
                if (hrs == 24) {
                    day++;
                    if (day > days_of_this_month()) {
                        month++;
                        if (month == 13) {
                            year++;
                        }
                        month = 1;
                    }
                    day = 1;
                }
                hrs = 0;
            }
            sec = 0;
        }
    }

/*


    datetime operator+(period &p) {
        if (p.to_seconds() < 0)
            return before2(*this, p.to_seconds());
        return afterI(*this, p.to_seconds());
    }

    datetime operator+(period &&p) {
        return operator+(p);
    }

    datetime operator+=(period &p) {
        datetime dt;
        if (p.to_seconds() < 0)
            dt = before2(*this, p.to_seconds());
        else
            dt = afterI(*this, p.to_seconds());
        *this = dt;
        return *this;
    }

    datetime operator+=(period &&p) {
        return operator+=(p);
    }

    datetime operator-(period &p) {
        if (p.to_seconds() < 0)
            return afterI(*this, -p.to_seconds());
        return before2(*this, -p.to_seconds());
    }

    datetime operator-(period &&p) {
        return operator-(p);
    }

    period operator-(datetime &dt) {
        return period();
    }

    period operator-(datetime &&dt) {
        return period();
    }

    datetime operator-=(period &p) {
        datetime dt;
        if (p.to_seconds() < 0)
            dt = afterI(*this, -p.to_seconds());
        else
            dt = before2(*this, -p.to_seconds());
        *this = dt;
        return *this;
    }

    datetime operator-=(period &&p) {
        return operator-=(p);
    }
*/
    long long seconds_from_epoch() {
        long long res = days_between_beginning_of_years(*this, datetime(1, 1, 1970));
        for (int i = 1; i < month; i++) {
            res += days_of_months[i - 1];
        }
        if (month > 2 && is_leap())
            res += 1;
        return (res + day - 1) * 86400 + hrs * 3600 + min * 60 + sec;
    }

    bool is_leap() const {
        return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
    }


};


#endif //UNTITLED9_CLASSES_H
