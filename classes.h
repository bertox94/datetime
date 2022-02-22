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

    [[nodiscard]] long long to_seconds() const {
        return days * 86400 + hrs * 3600 + min * 60 + sec;
    }

    long long extract_time() {
        period pd = *this;
        pd.days = 0;
        return pd.to_seconds();
    }

    long long strip_time() {
        period pd = *this;
        pd.hrs = 0;
        pd.min = 0;
        pd.sec = 0;
        return pd.to_seconds();
    }


};

class datetime {
    datetime after(datetime start, long long seconds) {

        datetime dt = start;
        long long estimation = seconds / (((double) 146097 / 400) * 86400);
        dt.year += estimation;

        const long long target = period(start.to_timestamp() + seconds).strip_time();
        period time(seconds - target);

        long long curr = dt.to_timestamp();

        //while curr>target go back 1 year
        while (curr > target) {
            curr -= dt.days_of_this_year() * 86400;
            dt.year--;
        }


        //come close with years
        if (curr <= target) {
            while (curr <= target) {
                curr += dt.days_of_this_year() * 86400;
                dt.year++;
            }
            dt.year--;
            curr -= dt.days_of_this_year() * 86400;
        }

        //come close with months
        if (curr <= target) {
            while (curr <= target) {
                curr += dt.days_of_this_month() * 86400;
                dt.month++;
                if (dt.month == 13) {
                    dt.year++;
                    dt.month = 1;
                }
            }
            dt.month--;
            if (dt.month == 0) {
                dt.year--;
                dt.month = 12;
            }
            curr -= dt.days_of_this_month() * 86400;
        }

        //come close with days
        if (curr <= target) {
            while (curr <= target) {
                curr += 86400;
                dt.day++;
                if (dt.day > dt.days_of_this_month()) {
                    dt.month++;
                    if (dt.month == 13) {
                        dt.year++;
                        dt.month = 1;
                    }
                    dt.day = 1;
                }
            }
            curr -= 86400;
            dt.day--;
            if (dt.day == 0) {
                dt.month--;
                if (dt.month == 0) {
                    dt.year--;
                    dt.month = 12;
                }
                dt.day = dt.days_of_this_month();
            }
        }

        //Since they are in canonical form we can do it easily
        dt.sec += time.sec;
        if (dt.sec >= 60) {
            dt.min++;
            if (dt.min == 60) {
                dt.hrs++;
                if (dt.hrs == 24) {
                    dt.day++;
                    if (dt.day > dt.days_of_this_month()) {
                        dt.month++;
                        if (dt.month == 13) {
                            dt.year++;
                            dt.month = 1;
                        }
                        dt.day = 1;
                    }
                    dt.hrs -= 24;
                }
                dt.min -= 60;
            }
            dt.sec -= 60;
        }

        dt.min += time.min;
        if (dt.min >= 60) {
            dt.hrs++;
            if (dt.hrs == 24) {
                dt.day++;
                if (dt.day > dt.days_of_this_month()) {
                    dt.month++;
                    if (dt.month == 13) {
                        dt.year++;
                        dt.month = 1;
                    }
                    dt.day = 1;
                }
                dt.hrs -= 24;
            }
            dt.min -= 60;
        }

        dt.hrs += time.hrs;
        if (dt.hrs == 24) {
            dt.day++;
            if (dt.day > dt.days_of_this_month()) {
                dt.month++;
                if (dt.month == 13) {
                    dt.year++;
                    dt.month = 1;
                }
                dt.day = 1;
            }
            dt.hrs -= 24;
        }


        return dt;
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
        r1 = dt.to_timestamp();

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

/**
 * Constructor based on
 * @param timestamp: seconds from epoch time.
 */
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

    bool operator==(datetime &&dt) const {
        return *this == dt;
    }

    bool operator!=(datetime &dt) const {
        return !(*this == dt);
    }

    bool operator!=(datetime &&dt) const {
        return *this != dt;
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

    long long seconds_from(datetime d2) const {
        return d2.seconds_to(*this);
    }

    long long seconds_to(datetime d2) const {
        datetime d1 = *this;

        int flag = (d2 > d1 ? 1 : -1);
        datetime dt1 = (d2 > d1 ? d1 : d2);
        datetime dt2 = (d2 > d1 ? d2 : d1);
        long long dd = 0;

        if (dt1.year == dt2.year) {
            if (dt1.month == dt2.month) {
                dd += dt2.day - dt1.day;
            } else {
                dd += dt1.days_of_this_month() - dt1.day;
                for (long long i = dt1.month + 1; i < dt2.month; i++)
                    dd += dt1.days_of_months[i - 1] + (dt1.is_leap() && i == 2 ? 1 : 0);
                dd += dt2.day;
            }
        } else {
            if (dt1.is_leap())
                dd++;

            dd += (dt2.year - dt1.year) * 365 +
                  (((dt2.year - 1) / 4 - (dt1.year) / 4) -
                   ((dt2.year - 1) / 100 - (dt1.year) / 100) +
                   ((dt2.year - 1) / 400 - (dt1.year) / 400));

            for (long long i = 1; i < dt1.month; i++)
                dd -= dt1.days_of_months[i - 1] + (dt1.is_leap() && i == 2 ? 1 : 0);
            for (long long i = 1; i < dt2.month; i++)
                dd += dt2.days_of_months[i - 1] + (dt2.is_leap() && i == 2 ? 1 : 0);

            dd -= dt1.day;
            dd += dt2.day;
        }

        long long ss = dt2.sec - dt1.sec;
        long long mm = dt2.min - dt1.min;
        long long hh = dt2.hrs - dt1.hrs;

        return (ss + mm * 60 + hh * 3600 + dd * 86400) * flag;
    }

    int days_of_this_year() const {
        return is_leap() + 365;
    }

    int days_of_this_month() const {
        return (is_leap() && month == 2 ? 1 : 0) + days_of_months[month - 1];
    }


    datetime go_on_one_sec() {
        datetime dt = *this;

        return dt;
    }

    datetime go_back_one_sec() {
        datetime dt = *this;

        return dt;
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
    long long to_timestamp() const {
        return seconds_from(datetime(1, 1, 1970));
    }

    bool is_leap() const {
        return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
    }


};


#endif //UNTITLED9_CLASSES_H
