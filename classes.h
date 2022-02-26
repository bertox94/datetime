#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-nodiscard"
//
// Created by Halib on 21.02.2022.
//

#ifndef DATETIME_CLASSES_H
#define DATETIME_CLASSES_H

#include <ctime>
#include <iomanip>

using namespace std;

//try to keep in canonical form: only years negative, all in their respective bounds
class calendar_period {
public:
    long long months{};
    long long years{};

    calendar_period(long long int months, long long int years) : months(months), years(years) {}

    calendar_period operator+(calendar_period &cp) const {
        return calendar_period(months + cp.months, years + cp.months);
    }

    calendar_period operator+=(calendar_period &cp) {
        *this = calendar_period(months + cp.months, years + cp.months);
        return *this;
    }

};

class period {
public:

    long long sec{};
    long long min{};
    long long hrs{};
    long long days{};

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

    period(long long sec, long long min, long long hrs, long long days) :
            sec(sec), min(min), hrs(hrs), days(days) {}

    bool operator==(period &pd) const { return !(*this > pd || *this < pd); }

    bool operator!=(period &dt) const { return !(*this == dt); }

    bool operator<(period &pd) const {
        if (days < pd.days) {
            return true;
        } else if (days == pd.days) {
            if (hrs < pd.hrs) {
                return true;
            } else if (hrs == pd.hrs) {
                if (min < pd.min) {
                    return true;
                } else if (min == pd.min) {
                    if (sec < pd.sec) {
                        return true;
                    } else
                        return false;
                } else
                    return false;
            } else
                return false;
        } else
            return false;
    }

    bool operator>(period &pd) const {
        if (days > pd.days) {
            return true;
        } else if (days == pd.days) {
            if (hrs > pd.hrs) {
                return true;
            } else if (hrs == pd.hrs) {
                if (min > pd.min) {
                    return true;
                } else if (min == pd.min) {
                    if (sec > pd.sec) {
                        return true;
                    } else
                        return false;
                } else
                    return false;
            } else
                return false;
        } else
            return false;
    }

    bool operator<=(period &pd) const { return !(*this > pd); }

    bool operator>=(period &pd) const { return !(*this < pd); }

    period operator+(period &pd) const { return period(this->to_seconds() + pd.to_seconds()); }

    period operator+=(period &pd) {
        *this = period(this->to_seconds() + pd.to_seconds());
        return *this;
    }

    period operator-(period &pd) const { return period(this->to_seconds() - pd.to_seconds()); }

    period operator-=(period &pd) {
        *this = period(this->to_seconds() - pd.to_seconds());
        return *this;
    }

    period operator*(long long times) const { return period(this->to_seconds() * times); }

    period operator*=(long long times) {
        *this = period(this->to_seconds() * times);
        return *this;
    }

    period operator/(long long times) const { return period(this->to_seconds() / times); }

    period operator/=(long long times) {
        *this = period(this->to_seconds() / times);
        return *this;
    }

    long long to_seconds() const { return days * 86400 + hrs * 3600 + min * 60 + sec; }

    period extract_time() const { return {sec, min, hrs, 0}; }

    period strip_time() const { return {0, 0, 0, days}; }

};

class datetime;

datetime after(datetime start, long long seconds);

long long seconds_to(datetime start, datetime end);

class datetime {
public:
    int days_of_months[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    /**
     * By default date is epoch time.
     */
    long long sec = 0;
    long long min = 0;
    long long hrs = 0;
    long long day = 1;
    long long month = 1;
    long long year = 1970;

    datetime() = default;

    datetime(long long day, long long month, long long year, bool autofix) :
            day(day), month(month), year(year) {
        if (day > days_of_this_month())
            this->day = days_of_this_month();
    }

    datetime(long long sec, long long min, long long hrs, long long day, long long month, long long year, bool autofix)
            : sec(sec), min(min), hrs(hrs), day(day), month(month), year(year) {
        if (day > days_of_this_month())
            this->day = days_of_this_month();
    }

    datetime(long long day, long long month, long long year) :
            day(day), month(month), year(year) {}

    datetime(long long sec, long long min, long long hrs, long long day, long long month, long long year) :
            sec(sec), min(min), hrs(hrs), day(day), month(month), year(year) {}

/**
 * Constructor based on
 * @param timestamp: seconds from epoch time.
 */
    explicit datetime(long long timestamp) {
        datetime dt = ::after(datetime(), timestamp);
        *this = dt;
    }

    bool operator==(datetime &dt) const { return !(*this < dt || *this > dt); }

    bool operator==(datetime &&dt) const { return *this == dt; }

    bool operator!=(datetime &dt) const { return !(*this == dt); }

    bool operator!=(datetime &&dt) const { return *this != dt; }

    bool operator<(datetime &dt) const {
        if (year < dt.year) {
            return true;
        } else if (year == dt.year) {
            if (month < dt.month) {
                return true;
            } else if (month == dt.month) {
                if (day < dt.day) {
                    return true;
                } else if (day == dt.day) {
                    if (hrs < dt.hrs) {
                        return true;
                    } else if (hrs == dt.hrs) {
                        if (min < dt.min) {
                            return true;
                        } else if (min == dt.min) {
                            if (sec < dt.sec) {
                                return true;
                            } else
                                return false;
                        } else
                            return false;
                    } else
                        return false;
                } else
                    return false;
            } else
                return false;
        } else
            return false;
    }

    bool operator>(datetime &dt) const {
        if (year > dt.year) {
            return true;
        } else if (year == dt.year) {
            if (month > dt.month) {
                return true;
            } else if (month == dt.month) {
                if (day > dt.day) {
                    return true;
                } else if (day == dt.day) {
                    if (hrs > dt.hrs) {
                        return true;
                    } else if (hrs == dt.hrs) {
                        if (min > dt.min) {
                            return true;
                        } else if (min == dt.min) {
                            if (sec > dt.sec) {
                                return true;
                            } else
                                return false;
                        } else
                            return false;
                    } else
                        return false;
                } else
                    return false;
            } else
                return false;
        } else
            return false;
    }

    bool operator<=(datetime &d2) const { return !(*this > d2); }

    bool operator>=(datetime &d2) const { return !(*this < d2); }

    datetime operator+(period &p) const { return after(p.to_seconds()); }

    datetime operator+(period &&p) const { return operator+(p); }

    datetime operator+=(period &p) {
        *this = operator+(p);
        return *this;
    }

    datetime operator+=(period &&p) { return operator+=(p); }

    datetime operator-(period &p) const { return after(-p.to_seconds()); }

    datetime operator-(period &&p) const { return operator-(p); }

    datetime operator-=(period &p) {
        *this = this->operator-(p);
        return *this;
    }

    datetime operator-=(period &&p) { return this->operator-=(p); }

/**
 * Computes the (signed) time from @dt to @this
 * @param dt: an rvalue date
 * @return a period (days, hrs, min, sec) in canonical form
 */
    period operator-(datetime &dt) const { return period(seconds_from(dt)); }

/**
 * Computes the (signed) time from @dt to @this
 * @param dt: an lvalue date
 * @return a period (days, hrs, min, sec) in canonical form
 */
    period operator-(datetime &&dt) const { return operator-(dt); }

    /**
 * Computes the (signed) calendar period from @dt to @this
 * @param dt: an rvalue date
 * @return a period (days, hrs, min, sec) in canonical form
 */
    calendar_period operator/(datetime &dt) const {
        long long yyears = year - dt.year;
        long long mmonths = month - dt.month;
        if (mmonths <= 0) {
            yyears--;
            mmonths += 12;
        } else if (mmonths > 12) {
            yyears++;
            mmonths -= 12;
        }
        return {mmonths, yyears};
    }

/**
 * Computes the (signed) calendar period from @dt to @this
 * @param dt: an lvalue date
 * @return a period (days, hrs, min, sec) in canonical form
 */
    period operator/(datetime &&dt) const { return operator-(dt); }

/**
 * For example add to a date 13 months
 */
    datetime operator+(calendar_period &cp) const {
        datetime curr = *this;

        curr.year += (cp.months - 1) / 12;
        long long months = cp.months - cp.months / 12;

        curr.month += months;
        if (curr.month > 12) {
            curr.year++;
            curr.month -= 12;
        } else if (curr.month <= 0) {
            curr.year--;
            curr.month += 12;
        }

        curr.year += cp.years;
        return curr;
    }

    datetime operator+(calendar_period &&p) const { return this->operator+(p); }

    datetime operator+=(calendar_period &p) {
        *this = this->operator+(p);
        return *this;
    }

    datetime operator+=(calendar_period &&p) { return operator+=(p); }

    int get_week_day() const { return 4 + ((this->to_timestamp() / 86400) % 7); }

    datetime first_working_day() const {
        int wd = get_week_day();
        period pd;
        if (wd == 6)
            pd.days = 2;
        if (wd == 0)
            pd.days = 1;
        return *this + pd;
    }

    datetime today() const { return datetime(std::time(nullptr)); }

    datetime after(long long seconds) const { return ::after(*this, seconds); }

    long long seconds_from(datetime d2) const { return d2.seconds_to(*this); }

    long long seconds_to(datetime d2) const { return ::seconds_to(*this, d2); }

    int days_of_this_year() const { return 365 + ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0)); }

    int days_of_this_month() const {
        return (((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0)) && month == 2 ? 1 : 0) +
               days_of_months[month - 1];
    }

    long long to_timestamp() const { return seconds_from(datetime()); }

    /**
     * Be careful, month and year are treated as 0
     */
    period extract_period() const { return {sec, min, hrs, day}; }

    period extract_time() const { return {sec, min, hrs, 0}; }
};

period operator-(period &p) { return period(-p.to_seconds()); }

period operator-(period &&p) { return -p; }

calendar_period operator-(calendar_period &p) { return {-p.months, -p.years}; }

calendar_period operator-(calendar_period &&p) { return -p; }

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

long long fK(int PERIOD, long long x, long long y) {
    if (x >= 0)
        y -= PERIOD * ((PERIOD + x - 1) / PERIOD);
    else
        y -= PERIOD * (x / PERIOD);

    if (y <= 0)
        return y / PERIOD;
    else
        return 1 + (y - 1) / PERIOD;

}

/**
 * @return the number of days from 01.01.@param x 00:00:00 to 01.01.@param y 00:00:00
 */
long long f(long long x, long long y) { return (y - x) * 365 + fK(4, x, y) - fK(100, x, y) + fK(400, x, y); }

/**
 * @return = @param start + @param seconds
 */
datetime after(datetime start, long long seconds) {

    long long start_timestamp = start.to_timestamp();
    datetime dt(0, 0, 0, 1, 1, 1970 + ((start_timestamp + seconds) / (((double) 146097 / 400) * 86400)));
    period increment(start_timestamp + seconds - dt.to_timestamp());

    dt.sec += increment.sec;
    if (dt.sec < 0) {
        dt.year--;
        dt.month = 12;
        dt.day = 31;
        dt.hrs = 23;
        dt.min = 59;
        dt.sec += 60;
    }

    dt.min += increment.min;
    if (dt.min < 0) { // here min and sec can be anything
        dt.hrs--;
        if (dt.hrs == -1) { //here hrs can be either -1 or 22
            dt.day--;
            if (dt.day == 0) { //here day can be either 0 or 30
                dt.month--;
                if (dt.month == 0) { // here month can be either 0 or 11
                    dt.year--;
                    dt.month = 12;
                }
                dt.day = dt.days_of_this_month();
            }
            dt.hrs = 23;
        }
        dt.min += 60;
    }

    dt.hrs += increment.hrs;
    if (dt.hrs < 0) {
        dt.day--;
        if (dt.day == 0) {
            dt.month--;
            if (dt.month == 0) {
                dt.year--;
                dt.month = 12;
            }
            dt.day = dt.days_of_this_month();
        }
        dt.hrs += 24;
    }

    dt.day += increment.days;
    while (dt.day > dt.days_of_this_month()) {
        dt.day -= dt.days_of_this_month();
        dt.month++;
        if (dt.month == 13) {
            dt.year++;
            dt.month = 1;
        }
    }
    while (dt.day <= 0) {
        dt.month--;
        if (dt.month == 0) {
            dt.year--;
            dt.month = 12;
        }
        dt.day += dt.days_of_this_month();
    }

    return dt;
}

/**
 * @return = @param end - @param start
 */
long long seconds_to(datetime start, datetime end) {

    int flag = (end > start ? 1 : -1);
    datetime dt1 = (end > start ? start : end);
    datetime dt2 = (end > start ? end : start);
    long long dd = 0;

    if (dt1.year == dt2.year) {
        if (dt1.month == dt2.month) {
            dd += dt2.day - dt1.day;
        } else {
            dd += dt1.days_of_this_month() - dt1.day;
            //because of the check on dt2 it is not a problem if ddd.month==13
            for (datetime ddd(1, dt1.month + 1, dt1.year); ddd.month < dt2.month; ddd.month++)
                dd += ddd.days_of_this_month();
            dd += dt2.day;
        }
    } else {

        dd += f(dt1.year, dt2.year);

        datetime ddd(1, 1, dt1.year);
        for (ddd.month = 1; ddd.month < dt1.month; ddd.month++)
            dd -= ddd.days_of_this_month();
        ddd = datetime(1, 1, dt2.year);
        for (ddd.month = 1; ddd.month < dt2.month; ddd.month++)
            dd += ddd.days_of_this_month();

        dd -= dt1.day;
        dd += dt2.day;
    }

    long long ss = dt2.sec - dt1.sec;
    long long mm = dt2.min - dt1.min;
    long long hh = dt2.hrs - dt1.hrs;

    return (ss + mm * 60 + hh * 3600 + dd * 86400) * flag;
}

#endif //DATETIME_CLASSES_H

#pragma clang diagnostic pop
