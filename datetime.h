#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "google-explicit-constructor"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "modernize-use-nodiscard"
//
// Created by Halib on 21.02.2022.
//

#ifndef DATETIME_CLASSES_H
#define DATETIME_CLASSES_H

#include <iomanip>
#include <string>

using namespace std;

class days {
private:
    long long param;
public:
    explicit days(long long _param) : param(_param) {}

    long long operator*(long long factor) const { return param * factor; }
};

class hrs {
private:
    long long param;
public:
    explicit hrs(long long _param) : param(_param) {}

    long long operator*(long long factor) const { return param * factor; }
};

class mins {
private:
    long long param;
public:
    explicit mins(long long _param) : param(_param) {}

    long long operator*(long long factor) const { return param * factor; }
};

class secs {
private:
    long long param;
public:
    explicit secs(long long _param) : param(_param) {}

    long long operator+(long long addendum) const { return param + addendum; }

    long long operator*(long long factor) const { return param * factor; }
};


class period {
private:
    long long days{};
    int hrs{};
    int min{};
    int sec{};

public:

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"

    /**
     * Creates a period in canonical form from @param _seconds.
     * A period is in canonical form iff (_days <= 0 && _hrs <= 0 && _min <= 0 && _sec <= 0) ||
     *                              (_days >= 0 && _hrs >= 0 && _min >= 0 && _sec >= 0)
     * NB: this is a non-explicit constructor.
     */
    period(long long seconds) {
        days = seconds / 86400;
        long long ss = days * 86400;
        hrs = (seconds - ss) / 3600;
        ss += hrs * 3600;
        min = (seconds - ss) / 60;
        ss += min * 60;
        sec = seconds - ss;
    }

#pragma clang diagnostic pop

    /**
     * Creates a period in canonical form from @param _sec, @param _min, @param _hrs, @param _days in whatever form.
     * NB: this is a non explicit constructor.
     */

    period(::days _days = ::days(0), ::hrs _hrs = ::hrs(0), ::mins _min = ::mins(0),
           ::secs _sec = ::secs(0)) { *this = _sec + _min * 60 + _hrs * 3600 + _days * 86400; }

    /**
     * @return = @this == @pd
     */
    bool operator==(period &pd) const { return !(*this > pd || *this < pd); }

    bool operator==(period &&pd) const { return this->operator==(pd); }

    /**
     * @return = @this != @pd
     */
    bool operator!=(period &dt) const { return !(*this == dt); }

    bool operator!=(period &&dt) const { return this->operator!=(dt); }

    /**
    * @return = @this < @pd
    */
    bool operator<(period &pd) const {
        return days < pd.days || days == pd.days && (
                hrs < pd.hrs || hrs == pd.hrs && (
                        min < pd.min || min == pd.min && sec < pd.sec
                )
        );
    }

    bool operator<(period &&pd) const { return this->operator<(pd); }

    /**
    * @return = @this > @pd
    */
    bool operator>(period &pd) const {
        return days > pd.days || days == pd.days && (
                hrs > pd.hrs || hrs == pd.hrs && (
                        min > pd.min || min == pd.min && sec > pd.sec
                )
        );
    }

    bool operator>(period &&pd) const { return this->operator>(pd); }

    /**
    * @return = @this <= @pd
    */
    bool operator<=(period &pd) const { return !(*this > pd); }

    bool operator<=(period &&pd) const { return this->operator<=(pd); }

    /**
    * @return = @this >= @pd
    */
    bool operator>=(period &pd) const { return !(*this < pd); }

    bool operator>=(period &&pd) const { return this->operator>=(pd); }

    /**
    * @return = @this + @pd
    */
    period operator+(period &pd) const { return this->to_seconds() + pd.to_seconds(); }

    period operator+(period &&pd) const { return this->operator+(pd); }

    /**
    * @this = @return = @this < @pd
    */
    period operator+=(period &pd) {
        *this = this->to_seconds() + pd.to_seconds();
        return *this;
    }

    period operator+=(period &&pd) { return this->operator+=(pd); }

    /**
    * @return = @this - @pd
    */
    period operator-(period &pd) const { return this->to_seconds() - pd.to_seconds(); }

    period operator-(period &&pd) const { return this->operator-(pd); }

    /**
    * @this = @return = @this < @pd
    */
    period operator-=(period &pd) {
        *this = this->to_seconds() - pd.to_seconds();
        return *this;
    }

    period operator-=(period &&pd) { return this->operator-=(pd); }

    /**
    * @return = @this * @pd
    */
    period operator*(period &pd) const { return this->to_seconds() * pd.to_seconds(); }

    period operator*(period &&pd) const { return this->operator*(pd); }

    /**
    * @this = @return = @this * @pd
    */
    period operator*=(period &pd) {
        *this = this->operator*(pd);
        return *this;
    }

    period operator*=(period &&pd) { return this->operator*=(pd); }

    /**
    * @return = @this / @pd
    */
    period operator/(period &pd) const { return this->to_seconds() / pd.to_seconds(); }

    period operator/(period &&pd) const { return this->operator/(pd); }

    /**
    * @this = @return = @this / @pd
    */
    period operator/=(period &pd) {
        *this = this->operator/(pd);
        return *this;
    }

    period operator/=(period &&pd) { return this->operator/=(pd); }

    /**
     * @return = @this % @pd
     */
    period operator%(period &pd) const { return this->to_seconds() % pd.to_seconds(); }

    period operator%(period &&pd) const { return this->operator%(pd); }

    /**
    * @this = @return = @this % @pd
    */
    period operator%=(period &pd) {
        *this = this->operator%(pd);
        return *this;
    }

    period operator%=(period &&pd) { return this->operator%=(pd); }

    /**
     * Getter functions.
     */
    int get_sec() const { return sec; }

    int get_min() const { return min; }

    int get_hrs() const { return hrs; }

    long long int get_days() const { return days; }

    /**
     * Convert @this to seconds.
     */
    long long to_seconds() const { return days * 86400 + hrs * 3600 + min * 60 + sec; }

    period strip_days() const {
        auto el = *this;
        el.days = 0;
        return el;
    }

};

/**
 * www, dd.MMM.yy, hh:mm:ss
 * ~~~, !!.@@@.##, $$:%%:&&
 */
class datetime_formatter {
public:
    string format = "~~~, !!.@@@.##, $$:%%:&&";
    bool month_str = true;
    bool h24 = true;
    bool keep_original_length = true;
};


int days_of_months[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

class datetime {
private:
    //do not use unsigned to avoid bad surprises on narrowing etc. casting!
    int day = 0;
    int month = 0;
    long long year = 1970;
    int hrs = 0;
    int min = 0;
    int sec = 0;

public:
    datetime_formatter format;

    /**
     * Construct a datetime which is the epoch time
     */
    datetime() = default;

    /**
     * Construct a new date which is @param seconds after epoch time.
     */
    explicit datetime(long long timestamp) { *this = after(timestamp); }

    /**
     * Constructor of datetime. Enforce the month to be valid (1 <= _month <= 12)
     * and then fixes (@fix_date) the date accordingly.
     */
    datetime(int _day, int _month, long long _year, bool autofix) :
            day(_day - 1), month(_month - 1), year(_year) {
        if (month < 0 || month > 11)
            throw runtime_error("");
        if (day >= days_of_this_month())
            this->day = days_of_this_month() - 1;
    }

    /**
     * Constructor of datetime. Enforce the following constraints: 1 <= _month <= 12, 0 <= ...
     * and then fixes (@fix_date) the date accordingly.
     */
    datetime(int _day, int _month, long long _year, int _hrs, int _min, int _sec, bool autofix) :
            datetime(_day, _month, _year, autofix) {
        sec = _sec;
        min = _min;
        hrs = _hrs;

        if (sec < 0 || sec > 59)
            throw runtime_error("");
        if (min < 0 || min > 59)
            throw runtime_error("");
        if (hrs < 0 || hrs > 23)
            throw runtime_error("");
    }

    /**
     * Constructor of datetime. Enforce the month to be valid (1 <= _month <= 12) and the day to be valid...
     */
    datetime(int _day, int _month, long long _year) :
            day(_day - 1), month(_month - 1), year(_year) {
        if (month < 0 || month > 11)
            throw runtime_error("");
        if (day < 0 || day >= days_of_this_month())
            throw runtime_error("");
    }

    /**
     * Constructor of datetime. Enforce the following constraints: 1 <= _month <= 12, 0 <= ...
     */
    datetime(int _day, int _month, long long _year, int _hrs, int _min, int _sec) :
            datetime(_day, _month, _year) {
        sec = _sec;
        min = _min;
        hrs = _hrs;

        if (sec < 0 || sec > 59)
            throw runtime_error("");
        if (min < 0 || min > 59)
            throw runtime_error("");
        if (hrs < 0 || hrs > 23)
            throw runtime_error("");
    }

private:
    /**
     * Auxiliary function for @f.
     */
    static long long fK(long long x, long long y) {
        long long y1 = y - 4 * (((x >= 0) * 3 + x) / 4);
        long long y2 = y - 100 * (((x >= 0) * 99 + x) / 100);
        long long y3 = y - 400 * (((x >= 0) * 399 + x) / 400);

        return (y1 > 0) + (y1 - (y1 > 0)) / 4
               - ((y2 > 0) + (y2 - (y2 > 0)) / 100)
               + (y3 > 0) + (y3 - (y3 > 0)) / 400;
    }

    /**
     * @return the number of days from 01.01.@param x 00:00:00 to 01.01.@param y 00:00:00
     */
    static long long f(long long x, long long y) { return (y - x) * 365 + fK(x, y); }

    static int days_of_month(unsigned int _month, long long _year) {
        return ( //if leap _year add 1 day to the normal number of days of February.
                       _month == 1 && ((_year % 400 == 0) || (_year % 4 == 0 && _year % 100 != 0))
               ) + days_of_months[_month];
    }

    /**
     * @return = @param start + @param seconds
     */
    datetime after(long long seconds) const {

        long long sts = this->to_timestamp() + seconds;
        long long _year = 1970 + (sts / (365.2425 * 86400)); // NOLINT(cppcoreguidelines-narrowing-conversions)
        auto secs = sts - datetime(1, 1, _year).to_timestamp();

        long long _day = secs / 86400;
        long long time_sec = secs - _day * 86400;
        if (time_sec < 0) {
            time_sec += 86400;
            _day--;
        }
        period time(time_sec);

        int _month = 0;

        int _dom = days_of_month(_month, _year);
        while (_day >= _dom) {
            _day -= _dom;
            _month++;
            if (_month == 12) {
                _year++;
                _month = 0;
            }
            _dom = days_of_month(_month, _year);
        }
        while (_day < 0) {
            _month--;
            if (_month == -1) {
                _year--;
                _month = 11;
            }
            _day += days_of_month(_month, _year);
        }

        return {static_cast<int>(_day + 1), _month + 1, _year, time.get_hrs(), time.get_min(), time.get_sec()};
    }

    /**
     * @return = @param end - @param start
     */
    long long seconds_to(const datetime &end) const {

        int flag = (end > *this ? 1 : -1);
        datetime dt1 = (end > *this ? *this : end);
        datetime dt2 = (end > *this ? end : *this);
        long long dd = f(dt1.year, dt2.year) + dt2.day - dt1.day;

        for (int _month = 0; _month < dt1.month; _month++)
            dd -= days_of_month(_month, dt1.year);
        for (int _month = 0; _month < dt2.month; _month++)
            dd += days_of_month(_month, dt2.year);

        return (dt2.sec - dt1.sec + (dt2.min - dt1.min) * 60 + (dt2.hrs - dt1.hrs) * 3600 + dd * 86400) * flag;
    }

public:

    /**
     * @return = @this == @dt
     */
    bool operator==(datetime &dt) const { return !(*this < dt || *this > dt); }

    bool operator==(datetime &&dt) const { return *this == dt; }

    /**
     * @return = @this != @dt
     */
    bool operator!=(datetime &dt) const { return !(*this == dt); }

    bool operator!=(datetime &&dt) const { return *this != dt; }

    /**
     * @return = @this < @dt
     */
    bool operator<(datetime &dt) const {
        return year < dt.year || year == dt.year && (
                month < dt.month || month == dt.month && (
                        day < dt.day || day == dt.day && (
                                hrs < dt.hrs || hrs == dt.hrs && (
                                        min < dt.min || min == dt.min && sec < dt.sec
                                )
                        )
                )
        );
    }

    bool operator<(datetime &&dt) const { return this->operator<(dt); }

    /**
     * @return = @this > @dt
     */
    bool operator>(const datetime &dt) const {
        return year > dt.year || year == dt.year && (
                month > dt.month || month == dt.month && (
                        day > dt.day || day == dt.day && (
                                hrs > dt.hrs || hrs == dt.hrs && (
                                        min > dt.min || min == dt.min && sec > dt.sec
                                )
                        )
                )
        );
    }

    bool operator>(datetime &&dt) const { return this->operator>(dt); }

    /**
     * @return = @this <= @dt
     */
    bool operator<=(datetime &d2) const { return !(*this > d2); }

    bool operator<=(datetime &&d2) const { return this->operator<=(d2); }

    /**
     * @return = @this >= @dt
     */
    bool operator>=(datetime &d2) const { return !(*this < d2); }

    bool operator>=(datetime &&d2) const { return this->operator>=(d2); }

    /**
     * @return = @this + @p
     */
    datetime operator+(period &p) const { return after(p.to_seconds()); }

    datetime operator+(period &&p) const { return operator+(p); }

    /**
     * @this = @return = @this + @p
     */
    datetime operator+=(period &p) {
        *this = operator+(p);
        return *this;
    }

    datetime operator+=(period &&p) { return operator+=(p); }

    /**
     * @return = @this - @p
     */
    datetime operator-(period &p) const { return after(-p.to_seconds()); }

    datetime operator-(period &&p) const { return operator-(p); }

    /**
     * @this = @return = @this - @p
     */
    datetime operator-=(period &p) {
        *this = this->operator-(p);
        return *this;
    }

    datetime operator-=(period &&p) { return this->operator-=(p); }

    /**
     * @return = @this - @dt
     */
    period operator-(datetime &dt) const { return seconds_from(dt); }

    period operator-(datetime &&dt) const { return operator-(dt); }

    /**
     * Getter functions.
     */
    long long int get_sec() const { return sec; }

    long long int get_min() const { return min; }

    long long int get_hrs() const { return hrs; }

    long long int get_day() const { return day + 1; }

    long long int get_month() const { return month + 1; }

    long long int get_year() const { return year; }

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
        dt.month += n - ((n / 12) * 12); // NOLINT(cppcoreguidelines-narrowing-conversions)

        if (dt.month > 11) {
            dt.year++;
            dt.month -= 12;
        }

        if (dt.month < 0) {
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

    /**
     * The date is fixed in the following way: given a month, if the day is greater than the days of the specified month,
     * the day is set to the last day of that month. Leap years are taken into account (February has 29 days).
     * @return: the fixed date.
     */
    datetime fix_date() const {
        datetime dt = *this;
        if (dt.day >= dt.days_of_this_month())
            dt.day = dt.days_of_this_month() - 1;
        return dt;
    }

    /**
     * @return the week_day of @this.
     */
    int get_week_day() const {
        long long tt = this->to_timestamp();
        long long res;
        if (tt >= 0) {
            res = (4 + (tt / 86400)) % 7;
        } else {
            res = (4 + 7 + (((tt - 86399) / 86400) % 7)) % 7;
        }
        return res; // NOLINT(cppcoreguidelines-narrowing-conversions)
    }

    /**
     * @return the first working day (today is included in the computation).
     */
    datetime first_working_day() const {
        int wd = this->get_week_day();

        if (wd == 6)
            return *this + days(2);
        else if (wd == 0)
            return *this + days(1);
        else
            return *this;
    }

    /**
     * @return the number of days of @this year.
     */
    int days_of_this_year() const { return 365 + ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0)); }

    /**
     * @return the number of days of @this month.
     */
    int days_of_this_month() const {
        return ( //if leap year add 1 day to the normal number of days of February.
                       month == 1 && ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0))
               ) + days_of_months[month];
    }

    /**
     * @return @this whose day is the last of the month.
     */
    datetime end_of_month() const {
        datetime dt = *this;
        dt.day = dt.days_of_this_month() - 1;
        return dt;
    }

    /**
     * @return the seconds from @epoch to @this.
     */
    long long to_timestamp() const { return seconds_from(datetime()); }

    /**
     * @return the seconds from @d2 to @this.
     */
    long long seconds_from(const datetime &d2) const { return d2.seconds_to(*this); }

};

/**
 * @return = -@p
 */
period operator-(period &p) { return -p.to_seconds(); }

period operator-(period &&p) { return -p; }

void replace(string &input, const string &from, const string &to) {
    unsigned int pos = 0;
    while (true) {
        size_t startPosition = input.find(from, pos);
        if (startPosition == string::npos)
            return;
        input.replace(startPosition, from.length(), to);
        pos += to.length();
    }
}

string to_week_day(int wk) {
    switch (wk) {
        case 0:
            return "Sunday";
        case 1:
            return "Monday";
        case 2:
            return "Tuesday";
        case 3:
            return "Wednesday";
        case 4:
            return "Thursday";
        case 5:
            return "Friday";
        case 6:
            return "Saturday";
        default:
            throw runtime_error("");
    }
}

string to_month(int mm) {
    switch (mm) {
        case 1:
            return "January";
        case 2:
            return "February";
        case 3:
            return "March";
        case 4:
            return "April";
        case 5:
            return "May";
        case 6:
            return "June";
        case 7:
            return "July";
        case 8:
            return "August";
        case 9:
            return "September";
        case 10:
            return "October";
        case 11:
            return "November";
        case 12:
            return "December";
        default:
            throw runtime_error("");
    }
}

std::ostream &operator<<(std::ostream &os, datetime const &dd) {
    string output = dd.format.format;
    bool month_str = dd.format.month_str;
    bool h24 = dd.format.h24;
    bool keep_original_length = dd.format.keep_original_length;

    unsigned int num = output.find_last_of('~') - output.find('~') + 1;
    std::string W = to_week_day(dd.get_week_day());
    if (!keep_original_length) {
        W = W.substr(0, num);
        W += std::string(num - W.length(), ' ');
    }
    replace(output, std::string(num, '~'), W);

    num = output.find_last_of('!') - output.find('!') + 1;
    std::string D = to_string(dd.get_day());
    if (!keep_original_length) {
        D.insert(0, num - D.length(), '0');
    }
    replace(output, std::string(num, '!'), D);

    num = output.find_last_of('@') - output.find('@') + 1;
    std::string M;
    if (month_str) {
        M = to_month(dd.get_month()); // NOLINT(cppcoreguidelines-narrowing-conversions)
        if (!keep_original_length) {
            M = M.substr(0, num);
            M += std::string(num - M.length(), ' ');
        }
    } else {
        M = to_string(dd.get_month());
        if (!keep_original_length)
            M.insert(0, num - M.length(), '0');
    }
    replace(output, std::string(num, '@'), M);

    num = output.find_last_of('#') - output.find('#') + 1;
    std::string Y = to_string(abs(dd.get_year()));
    if (!keep_original_length) {
        if (dd.get_year() >= 0) {
            if (Y.length() > num) {
                Y = Y.substr(Y.length() - num, num);
            } else {
                Y.insert(0, num - Y.length(), '0');
            }
        } else {
            if (Y.length() > num) {
                Y = Y.substr(Y.length() - num + 1, num);
            } else if (Y.length() == num) {
                Y = Y.substr(1, num);
            } else {
                Y.insert(0, num - Y.length() - 1, '0');
            }
            Y = "-" + Y;
        }
    }
    replace(output, std::string(num, '#'), Y);

    num = output.find_last_of('$') - output.find('$') + 1;
    std::string h;
    if (!h24) {
        if (dd.get_hrs() > 12) {
            h = to_string(dd.get_hrs() - 12);
            output += " PM";
        } else {
            h = to_string(dd.get_hrs());
            output += " AM";
        }
    } else {
        h = to_string(dd.get_hrs());
    }
    if (!keep_original_length)
        h.insert(0, num - h.length(), '0');
    replace(output, std::string(num, '$'), h);

    num = output.find_last_of('%') - output.find('%') + 1;
    std::string m = to_string(dd.get_min());
    if (!keep_original_length)
        m.insert(0, num - m.length(), '0');
    replace(output, std::string(num, '%'), m);

    num = output.find_last_of('&') - output.find('&') + 1;
    std::string s = to_string(dd.get_sec());
    if (!keep_original_length)
        s.insert(0, num - s.length(), '0');
    replace(output, std::string(num, '&'), s);

    return os << output;
}

std::ostream &operator<<(std::ostream &os, period const &d) {
    return os << "Days: " << std::setfill('0') << std::setw(2) << d.get_days() << ", hrs: " << std::setfill('0')
              << std::setw(2) << d.get_hrs() << ", mins: " << std::setfill('0') << std::setw(2) << d.get_min()
              << ", secs: "
              << std::setfill('0') << std::setw(2) << d.get_sec();
}

#endif //DATETIME_CLASSES_H

#pragma clang diagnostic pop
