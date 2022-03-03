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

class period {
private:
    long long sec{};
    long long min{};
    long long hrs{};
    long long days{};

public:

    period() = default;

    explicit period(long long seconds) {
        days = seconds / 86400;
        long long ss = days * 86400;
        hrs = (seconds - ss) / 3600;
        ss += hrs * 3600;
        min = (seconds - ss) / 60;
        ss += min * 60;
        sec = seconds - ss;
    }

    period(long long _sec, long long _min, long long _hrs, long long _days) :
            sec(_sec), min(_min), hrs(_hrs), days(_days) {
        if (abs(_sec) > 59)
            throw runtime_error("");
        if (abs(_min) > 59)
            throw runtime_error("");
        if (abs(_hrs) > 23)
            throw runtime_error("");

        //beware, for example period(0,-1,0,-2) is allowed.
        if (!(
                (_days <= 0 && _hrs <= 0 && _min <= 0 && _sec <= 0) ||
                (_days >= 0 && _hrs >= 0 && _min >= 0 && _sec >= 0)
        ))
            throw runtime_error("");
    }

    long long int get_sec() const { return sec; }

    long long int get_min() const { return min; }

    long long int get_hrs() const { return hrs; }

    long long int get_days() const { return days; }

    bool operator==(period &pd) const { return !(*this > pd || *this < pd); }

    bool operator==(period &&pd) const { return this->operator==(pd); }

    bool operator!=(period &dt) const { return !(*this == dt); }

    bool operator!=(period &&dt) const { return this->operator!=(dt); }

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

    bool operator<(period &&pd) const { return this->operator<(pd); }

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

    bool operator>(period &&pd) const { return this->operator>(pd); }

    bool operator<=(period &pd) const { return !(*this > pd); }

    bool operator<=(period &&pd) const { return this->operator<=(pd); }

    bool operator>=(period &pd) const { return !(*this < pd); }

    bool operator>=(period &&pd) const { return this->operator>=(pd); }

    period operator+(period &pd) const { return period(this->to_seconds() + pd.to_seconds()); }

    period operator+(period &&pd) const { return this->operator+(pd); }

    period operator+=(period &pd) {
        *this = period(this->to_seconds() + pd.to_seconds());
        return *this;
    }

    period operator+=(period &&pd) {
        return this->operator+=(pd);
    }

    period operator-(period &pd) const { return period(this->to_seconds() - pd.to_seconds()); }

    period operator-(period &&pd) const { return this->operator-(pd); }

    period operator-=(period &pd) {
        *this = period(this->to_seconds() - pd.to_seconds());
        return *this;
    }

    period operator-=(period &&pd) { return this->operator-=(pd); }

    period operator*(period &pd) const { return period(this->to_seconds() * pd.to_seconds()); }

    period operator*(period &&pd) const { return this->operator*(pd); }

    period operator*=(period &pd) {
        *this = this->operator*(pd);
        return *this;
    }

    period operator*=(period &&pd) { return this->operator*=(pd); }

    period operator/(period &pd) const { return period(this->to_seconds() / pd.to_seconds()); }

    period operator/(period &&pd) const { return this->operator/(pd); }

    period operator/=(period &pd) {
        *this = this->operator/(pd);
        return *this;
    }

    period operator/=(period &&pd) { return this->operator/=(pd); }

    long long to_seconds() const { return days * 86400 + hrs * 3600 + min * 60 + sec; }

};


class datetime {
private:

    long long fK(int PERIOD, long long x, long long y) const {
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
    long long f(long long x, long long y) const { return (y - x) * 365 + fK(4, x, y) - fK(100, x, y) + fK(400, x, y); }

/**
 * @return = @param start + @param seconds
 */
    datetime after(long long seconds) const {

        datetime start = *this;
        long long start_timestamp = start.to_timestamp();
        datetime dt(0, 0, 0, 1, 1, 1970 + ((start_timestamp + seconds) / (((double) 146097 / 400) * 86400)));
        period from_dt(start_timestamp + seconds - dt.to_timestamp());

        dt.sec += from_dt.get_sec();
        if (dt.sec < 0) {
            dt.year--;
            dt.month = 12;
            dt.day = 31;
            dt.hrs = 23;
            dt.min = 59;
            dt.sec += 60;
        }

        dt.min += from_dt.get_min();
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

        dt.hrs += from_dt.get_hrs();
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

        dt.day += from_dt.get_days();
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
    long long seconds_to(datetime end) const {

        datetime start = *this;
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

public:

    datetime() = default;

    datetime(long long _day, long long _month, long long _year, bool autofix) :
            day(_day), month(_month), year(_year) {
        if (_month < 1 || _month > 12)
            throw runtime_error("");
        if (_day > days_of_this_month())
            this->day = days_of_this_month();
    }

    datetime(long long _sec, long long _min, long long _hrs, long long _day, long long _month, long long _year,
             bool autofix)
            : sec(_sec), min(_min), hrs(_hrs), day(_day), month(_month), year(_year) {
        if (_sec < 0 || _sec > 59)
            throw runtime_error("");
        if (_min < 0 || _min > 59)
            throw runtime_error("");
        if (_hrs < 0 || _hrs > 23)
            throw runtime_error("");
        if (_month < 1 || _month > 12)
            throw runtime_error("");
        if (_day > days_of_this_month())
            this->day = days_of_this_month();
    }

    datetime(long long _day, long long _month, long long _year) :
            day(_day), month(_month), year(_year) {
        if (_month < 1 || _month > 12)
            throw runtime_error("");
        if (_day > days_of_this_month())
            throw runtime_error("");
    }

    datetime(long long _sec, long long _min, long long _hrs, long long _day, long long _month, long long _year) :
            sec(_sec), min(_min), hrs(_hrs), day(_day), month(_month), year(_year) {
        if (_sec < 0 || _sec > 59)
            throw runtime_error("");
        if (_min < 0 || _min > 59)
            throw runtime_error("");
        if (_hrs < 0 || _hrs > 23)
            throw runtime_error("");
        if (_month < 1 || _month > 12)
            throw runtime_error("");
        if (_day > days_of_this_month())
            throw runtime_error("");
    }

    long long int get_sec() const { return sec; }

    long long int get_min() const { return min; }

    long long int get_hrs() const { return hrs; }

    long long int get_day() const { return day; }

    long long int get_month() const { return month; }

    long long int get_year() const { return year; }

    /**
    * Construct a new date which is @param seconds after epoch time.
    * To get the current date pass std::time(nullptr) as parameter.
    */
    explicit datetime(long long timestamp) { *this = after(timestamp); }

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

    bool operator<(datetime &&dt) const { return this->operator<(dt); }

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

    bool operator>(datetime &&dt) const { return this->operator>(dt); }

    bool operator<=(datetime &d2) const { return !(*this > d2); }

    bool operator<=(datetime &&d2) const { return this->operator<=(d2); }

    bool operator>=(datetime &d2) const { return !(*this < d2); }

    bool operator>=(datetime &&d2) const { return this->operator>=(d2); }

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
 * Computes the (signed) period from @dt to @this
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
 * @return is the number of months from @this and @param dt regardless of the days,
 * e.g. (5.1.2020).months_between(3.2.2020) =====> 1.
 */
    long long months_between(datetime &dt) const { return 12 * (dt.year - year) + dt.month - month; }

/**
 * @return =  @this after @param n years. The obtained date is adjusted to return the last of the when it overflows,
 * e.g. (31.1.2020).after_months(1) =====> 28.1.2020
 */
    datetime after_months(long long n) const {
        datetime dt = *this;
        dt.year += n / 12;
        dt.month += n - ((n / 12) * 12);

        if (dt.month > 12) {
            dt.year++;
            dt.month -= 12;
        }

        if (dt.month <= 0) {
            dt.year--;
            dt.month += 12;
        }

        return dt.fix_date();
    }

/**
 * @return is the number of years from @this and @param dt regardless of the days,
 * e.g. (5.1.2020).months_between(3.8.2021) =====> 1.
 */
    long long years_between(datetime &dt) const { return dt.year - year; }

/**
 * @return =  @this after @param n years.
 */
    datetime after_years(long long n) const {
        datetime dt = *this;
        dt.year += n;
        return dt;
    }

    datetime fix_date() const {
        datetime dt = *this;
        if (dt.day > dt.days_of_this_month())
            dt.day = dt.days_of_this_month();
        return dt;
    }

    int get_week_day() const { return 4 + ((this->to_timestamp() / 86400) % 7); }

    datetime first_working_day() const {
        datetime dt = *this;
        int wd = dt.get_week_day();

        period pd;
        if (wd == 6)
            pd = period(0, 0, 0, 2);
        else if (wd == 0)
            pd = period(0, 0, 0, 1);
        return dt + pd;
    }

    int days_of_this_year() const { return 365 + ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0)); }

    int days_of_this_month() const {
        return (((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0)) && month == 2 ? 1 : 0) +
               days_of_months[month - 1];
    }

    datetime end_of_month() const {
        datetime dt = *this;
        dt.day = dt.days_of_this_month();
        return dt;
    }

    long long to_timestamp() const { return seconds_from(datetime()); }

    long long seconds_from(datetime d2) const { return d2.seconds_to(*this); }

};

period operator-(period &p) { return period(-p.to_seconds()); }

period operator-(period &&p) { return -p; }

std::ostream &operator<<(std::ostream &os, datetime const &d) {
    return os << std::setfill('0') << std::setw(2) << d.get_day() << "." << std::setfill('0') << std::setw(2)
              << d.get_month() << "." << std::setfill('0') << std::setw(4) << d.get_year() << ", " << std::setfill('0')
              << std::setw(2) << d.get_hrs() << ":" << std::setfill('0') << std::setw(2) << d.get_min() << ":"
              << std::setfill('0') << std::setw(2) << d.get_sec();
}

std::ostream &operator<<(std::ostream &os, period const &d) {
    return os << "Days: " << std::setfill('0') << std::setw(2) << d.get_days() << ", hrs: " << std::setfill('0')
              << std::setw(2) << d.get_hrs() << ", min: " << std::setfill('0') << std::setw(2) << d.get_min()
              << ", sec: "
              << std::setfill('0') << std::setw(2) << d.get_sec();
}

#endif //DATETIME_CLASSES_H

#pragma clang diagnostic pop
