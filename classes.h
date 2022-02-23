#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-nodiscard"
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
        long long ss = days * 86400;
        hrs = (seconds - ss) / 3600;
        ss += hrs * 3600;
        min = (seconds - ss) / 60;
        ss += min * 60;
        sec = seconds - ss;
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

    long long to_seconds() const {
        return days * 86400 + hrs * 3600 + min * 60 + sec;
    }

    long long extract_time() const {
        period pd = *this;
        pd.days = 0;
        return pd.to_seconds();
    }

    long long strip_time() const {
        period pd = *this;
        pd.hrs = 0;
        pd.min = 0;
        pd.sec = 0;
        return pd.to_seconds();
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

    datetime(long long int day, long long int month, long long int year) :
            day(day), month(month), year(year) {}

    datetime(long long sec, long long min, long long hrs, long long day, long long month, long long year) :
            sec(sec), min(min), hrs(hrs), day(day), month(month), year(year) {}

    /**
 * Constructor based on
 * @param timestamp: seconds from epoch time.
 */
    explicit datetime(long long timestamp) {
        datetime dt = after(datetime(0, 0, 0, 1, 1, 1970), timestamp);
        *this = dt;
    }

    bool operator==(datetime &dt) const {
        return sec == dt.sec && min == dt.min && hrs == dt.hrs && day == dt.day && month == dt.month && year == dt.year;
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
        return !(year < dt.year && month < dt.month && day < dt.day);
    }

    bool operator>(datetime &dt) const {
        return !(year > dt.year && month > dt.month && day > dt.day);
    }

    bool operator<=(datetime &d2) const {
        return (*this == d2 || *this < d2);
    }

    bool operator>=(datetime &d2) const {
        return (*this == d2 || *this > d2);
    }

    datetime operator+(period &p) const {
        return after(p.to_seconds());
    }

    datetime operator+(period &&p) const {
        return operator+(p);
    }

    datetime operator+=(period &p) {
        *this = after(p.to_seconds());
        return *this;
    }

    datetime operator+=(period &&p) {
        return operator+=(p);
    }

    datetime operator-(period &p) const {
        return after(-p.to_seconds());
    }

    datetime operator-(period &&p) const {
        return operator-(p);
    }

    datetime operator-=(period &p) {
        *this = after(-p.to_seconds());
        return *this;
    }

    datetime operator-=(period &&p) {
        return operator-=(p);
    }

    /**
 * Computes the (signed) time from @this to @dt
 * @param dt: a date
 * @return a period (days, hrs, min, sec) in canonical form
 */
    period operator-(datetime &dt) const {
        return period(seconds_from(dt));
    }

    /**
 * Computes the (signed) time from @this to @dt
 * @param dt: a lvalue date
 * @return a period (days, hrs, min, sec) in canonical form
 */
    period operator-(datetime &&dt) const {
        return period(seconds_from(dt));
    }

    datetime after(datetime start, long long seconds) const {

        datetime dt = start;
        long long estimation = seconds / (((double) 146097 / 400) * 86400);
        dt.year += estimation;

        const long long target = period(start.to_timestamp() + seconds).strip_time();
        period time(seconds - target);

        long long curr = dt.to_timestamp();

        //while curr>target go back 1 year
        while (curr > target) {
            dt.year--;
            curr -= dt.days_of_this_year() * 86400;
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
        } else if (dt.sec < 0) {
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
                        dt.day = dt.days_of_this_month();
                    }
                    dt.hrs += 24;
                }
                dt.min += 60;
            }
            dt.sec += 60;
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
        } else if (dt.min < 0) {
            dt.hrs--;
            if (dt.hrs < 0) {
                dt.day--;
                if (dt.day == 0) {
                    dt.month--;
                    if (dt.month <= 0) {
                        dt.year--;
                        dt.month += 12;
                    }
                    dt.day = dt.days_of_this_month();
                }
                dt.hrs += 24;
            }
            dt.min += 60;
        }

        dt.hrs += time.hrs;
        if (dt.hrs >= 24) {
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
        } else if (dt.hrs < 0) {
            dt.day--;
            if (dt.day == 0) {
                dt.month--;
                if (dt.month <= 0) {
                    dt.year--;
                    dt.month += 12;
                }
                dt.day = dt.days_of_this_month();
            }
            dt.hrs += 24;
        }


        return dt;
    }

    datetime after(long long seconds) const {

        datetime dt = *this;
        long long estimation = seconds / (((double) 146097 / 400) * 86400);
        dt.year += estimation;

        const long long target = period(this->to_timestamp() + seconds).strip_time();
        period time(seconds - target);

        long long curr = dt.to_timestamp();

        //while curr>target go back 1 year
        while (curr > target) {
            dt.year--;
            curr -= dt.days_of_this_year() * 86400;
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
        } else if (dt.sec < 0) {
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
                        dt.day = dt.days_of_this_month();
                    }
                    dt.hrs += 24;
                }
                dt.min += 60;
            }
            dt.sec += 60;
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
        } else if (dt.min < 0) {
            dt.hrs--;
            if (dt.hrs < 0) {
                dt.day--;
                if (dt.day == 0) {
                    dt.month--;
                    if (dt.month <= 0) {
                        dt.year--;
                        dt.month += 12;
                    }
                    dt.day = dt.days_of_this_month();
                }
                dt.hrs += 24;
            }
            dt.min += 60;
        }

        dt.hrs += time.hrs;
        if (dt.hrs >= 24) {
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
        } else if (dt.hrs < 0) {
            dt.day--;
            if (dt.day == 0) {
                dt.month--;
                if (dt.month <= 0) {
                    dt.year--;
                    dt.month += 12;
                }
                dt.day = dt.days_of_this_month();
            }
            dt.hrs += 24;
        }


        return dt;
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

    long long to_timestamp() const {
        return seconds_from(datetime(1, 1, 1970));
    }

    [[deprecated]]
    bool is_leap() const {
        return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
    }

};

period operator-(period &p) {
    return period(-p.to_seconds());
}

period operator-(period &&p) {
    return -p;
}

std::ostream &operator<<(std::ostream &os, datetime const &d) {
    return os << std::setfill('0') << std::setw(2) << d.day << "." << std::setfill('0') << std::setw(2) << d.month
              << "." << std::setfill('0') << std::setw(4) << d.year << ", " << std::setfill('0') << std::setw(2)
              << d.hrs << ":" << std::setfill('0') << std::setw(2) << d.min << ":" << std::setfill('0') << std::setw(2)
              << d.sec;
}

std::ostream &operator<<(std::ostream &os, period const &d) {
    return os << "Days: " << std::setfill('0') << std::setw(2) << d.days << ", hrs: " << std::setfill('0')
              << std::setw(2) << d.hrs << ", min: " << std::setfill('0') << std::setw(2) << d.min << ", sec: "
              << std::setfill('0') << std::setw(2) << d.sec;
}


#endif //UNTITLED9_CLASSES_H

#pragma clang diagnostic pop