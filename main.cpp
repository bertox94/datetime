#include <iostream>
#include <ctime>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;

class date {
public:
    date() = default;

    date(int day, int month, int year) {
        time_t rawtime;
        struct tm *tm = localtime(&rawtime);
        tm->tm_year = year - 1900;
        tm->tm_mon = month - 1;
        tm->tm_mday = day;
        d = mktime(tm);
        update();
    }

    date(const date &dt) {
        this->d = dt.d;
        update();
    }

    //v =t is same as v func(t), func as =
    date &operator=(const date &dt) {
        this->d = dt.d;
        update();
        return *this;
    }

    bool operator==(date &d2) {
        return month == d2.month && day == d2.day && year == d2.year;
    }

    bool operator!=(date &d2) {
        return !(*this == d2);
    }

    bool operator<(date &d2) {
        if (year < d2.year)
            return true;
        else if (year == d2.year) {
            if (month < d2.month)
                return true;
            else if (month == d2.month) {
                if (day < d2.day)
                    return true;
                else
                    return false;
            } else
                return false;
        } else
            return false;
    }

    bool operator>(date &d2) {
        if (year > d2.year)
            return true;
        else if (year == d2.year) {
            if (month > d2.month)
                return true;
            else if (month == d2.month) {
                if (day > d2.day)
                    return true;
                else
                    return false;
            } else
                return false;
        } else
            return false;
    }

    bool operator<=(date &d2) {
        return (*this == d2 || *this < d2);
    }


    bool operator>=(date &d2) {
        return (*this == d2 || *this > d2);
    }

    time_t d = 0;

    int day = 0;
    int month = 0;
    int year = 0;
    int wday = 0;

    void update() {
        struct tm *tm = localtime(&d);
        day = tm->tm_mday;
        month = tm->tm_mon;
        year = tm->tm_year + 1900;
        wday = tm->tm_wday;
    }

    string self() {
        const char *weekday[] = {"Sun", "Mon",
                                 "Tue", "Wed",
                                 "Thu", "Fri", "Sat"};
        const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

        struct tm *tm = localtime(&d);
        string str;
        str += weekday[tm->tm_wday];
        str += ", ";
        str += std::to_string(tm->tm_mday);
        str += ".";
        str += months[tm->tm_mon];
        str += ".";
        str += std::to_string(tm->tm_year + 1900);
        return str;
    }

    void after_days(int n) {
        struct tm *tm = localtime(&d);
        tm->tm_mday += n;
        d = mktime(tm);
        update();
    }

    void after_months(int n) {
        struct tm *tm = localtime(&d);
        tm->tm_mon += n;
        d = mktime(tm);
        update();
    }

    void after_years(int n) {
        struct tm *tm = localtime(&d);
        tm->tm_year += n;
        d = mktime(tm);
        update();
    }

    //no preconditions
    int days_between(date &d2) {
        int res = 0;
        date d3 = *this;
        date d4 = d2;
        if (d > d2.d) {
            d3 = d2;
            d4 = *this;
        }

        while (!(d3 == d4)) {
            string st3 = d3.self();
            string st4 = d4.self();
            res++;
            d3.after_days(1);
        }
        return res;
    }

    //assume preconditions: same day, ...
    int months_between(date &d2) {
        int res = 0;
        date d3 = *this;
        date d4 = d2;
        if (d > d2.d) {
            d3 = d2;
            d4 = *this;
        }
        while (!(d3 == d4)) {
            res++;
            d3.after_months(1);
        }
        return res;
    }

    //assume preconditions: same day, same month ...
    int years_between(date &d2) {
        int res = 0;
        date d3 = *this;
        date d4 = d2;
        if (d > d2.d) {
            d3 = d2;
            d4 = *this;
        }
        while (!(d3 == d4)) {
            res++;
            d3.after_years(1);
        }
        return res;
    }

    bool is_working_day() {
        return !(wday == 6 || wday == 0);
    }

    date next_working_day() {
        date dt = *this;
        if (wday == 6)
            dt.after_days(2);
        else if (wday == 0)
            dt.after_days(1);
        return dt;
    }
};

class dates {
public:
    date initial_date;
    date final_date;
};

class transaction {
public:
    transaction(const string &name, bool wire_transfer, const date &initialDate, int f1, const string &f2,
                double amount) :
            name(name),
            is_wire_transfer(wire_transfer),
            ends(false),
            f1(f1),
            f2(f2),
            amount(amount) {
        dates.initial_date = initialDate;
    }

    transaction(const string &name, bool wire_transfer, const date &initialDate, bool ends, const date &finalDate,
                int f1, const string &f2,
                double amount)
            : name(name),
              is_wire_transfer(wire_transfer),
              ends(ends),
              f1(f1),
              f2(f2),
              amount(amount) {

        dates.initial_date = initialDate;
        dates.final_date = finalDate;
    }

public:

    string name;
    bool is_wire_transfer;
    bool ends;
    dates dates;
    int f1;
    string f2;
    double amount;
};

std::vector<transaction> waiting_queue;

bool execute(date today, const transaction &row) {
    if (row.is_wire_transfer && !today.is_working_day()) {
        date dt = today.next_working_day();
        waiting_queue.emplace_back(row.name, row.is_wire_transfer, dt, true, dt, row.f1, row.f2, row.amount);
        return false;
    } else
        return true;
}

bool is_today(transaction &row, date today) {
    date start_date = row.dates.initial_date;
    string f2 = row.f2;
    int f1 = row.f1;

    //if the first occurrence is in the future
    if (today < start_date)
        return false;

    //if the end date is in the past
    if (row.ends) {
        if (today > row.dates.final_date)
            return false;
    }

    if (f2 == "weeks") {
        if (start_date.days_between(today) % (f1 * 7) != 0)
            return false;
        return execute(today, row);
    }


    if (f2 == "months") {
        if (start_date.day != today.day)
            return false;
        if (start_date.months_between(today) % f1 != 0)
            return false;
        return execute(today, row);
    }

    if (f2 == "years") {
        if (start_date.day != today.day)
            return false;
        if (start_date.month != today.month)
            return false;
        if (start_date.years_between(today) % f1 != 0)
            return false;
        return execute(today, row);
    }


    return false;
}

void process(ofstream &myfile, date &today, double &var, transaction &el) {
    if (is_today(el, today)) {
        var += el.amount;
        myfile << left << setw(20) << today.self() << setw(20) << el.name << setw(20) << el.amount
               << setw(20);
        if (var <= 0) {
            myfile << "=============================> ";
            //cout << "Failure detected at: " << today.self();
        }
        myfile << var << endl;
    }

}

int main() {

    //attenzione, non considera che il refill non considera le feste nazionali
    vector<transaction> vec;
    vec.emplace_back("Rent", true, date(25, 1, 2022), true, date(25, 4, 2022), 1, "months", -1060);
    vec.emplace_back("Rent", true, date(25, 6, 2022), 1, "months", -1070);
    vec.emplace_back("Rundfunkbeitrag", false, date(15, 11, 2021), 3, "months", -55.08);
    vec.emplace_back("Netflix", false, date(2, 2, 2022), 1, "months", -17.99);
    vec.emplace_back("JetBrains", false, date(12, 4, 2022), 1, "years", -206.22);
    vec.emplace_back("Youtube Premium", false, date(26, 1, 2022), 1, "months", -11.99);
    vec.emplace_back("Vodafone", false, date(17, 1, 2022), 4, "weeks", -9.99);
    vec.emplace_back("ImmoScout24", false, date(26, 1, 2022), 1, "months", -9.98);
    vec.emplace_back("Office365", false, date(3, 2, 2022), 1, "months", -7);
    vec.emplace_back("Amazon Prime", false, date(30, 11, 2021), 1, "years", -69);
    vec.emplace_back("Kaspersky Cloud", false, date(7, 12, 2021), 1, "years", -34.97);
    vec.emplace_back("Kaspersky VPN", false, date(19, 12, 2021), 1, "years", -29.95);
    vec.emplace_back("Night Eye", false, date(17, 12, 2021), 1, "years", -9.55);
    vec.emplace_back("Cerberus", false, date(7, 8, 2021), 1, "years", -5);
    //vec.emplace_back("Mamma",true, date(18, 6, 2022), true, date(18, 6, 2022), 20, "years", -400);
    //vec.emplace_back("Lina",true, date(18, 6, 2022), true, date(18, 6, 2022), 20, "years", -500);
    //vec.emplace_back("Kautz",true, date(18, 6, 2022), true, date(18, 6, 2022), 20, "years", -1000);
    vec.emplace_back("Refill", true, date(10, 3, 2022), 1, "months",
                     1180); //lo fai il 5 dunque assumi che arriva il 10


    ofstream myfile;
    myfile.open("schedule.asm");


    date today(4, 2, 2022);
    double var = 1301.06;

    while (today.year <= 2030) {
        for (auto &el:waiting_queue)
            process(myfile, today, var, el);
        for (auto &el:vec)
            process(myfile, today, var, el);
        today.after_days(1);
    }

    cout << "Done" << endl;

    myfile.close();

    return 0;
}


