#include <iostream>
#include <ctime>
#include <vector>
#include <fstream>
#include <iomanip>
#include <list>

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
    }

    date(const date &dt) {
        this->d = dt.d;
    }

    //v =t is same as v func(t), func as =
    date &operator=(date &dt) {
        this->d = dt.d;
        return *this;
    }

    bool operator==(date &d2) {
        return get_month() == d2.get_month() && get_day() == d2.get_day() && get_year() == d2.get_year();
    }

    bool operator!=(date &d2) {
        return !(*this == d2);
    }

    bool operator<(date &d2) {
        if (get_year() < d2.get_year())
            return true;
        else if (get_year() == d2.get_year()) {
            if (get_month() < d2.get_month())
                return true;
            else if (get_month() == d2.get_month()) {
                if (get_day() < d2.get_day())
                    return true;
                else
                    return false;
            } else
                return false;
        } else
            return false;
    }

    bool operator>(date &d2) {
        if (get_year() > d2.get_year())
            return true;
        else if (get_year() == d2.get_year()) {
            if (get_month() > d2.get_month())
                return true;
            else if (get_month() == d2.get_month()) {
                if (get_day() > d2.get_day())
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

    time_t d;

    int get_day() {
        struct tm *tm = localtime(&d);
        return tm->tm_mday;
    }

    int get_month() {
        struct tm *tm = localtime(&d);
        return tm->tm_mon;
    }

    int get_year() {
        struct tm *tm = localtime(&d);
        return 1900 + tm->tm_year;
    }

    int get_wday() {
        struct tm *tm = localtime(&d);
        return tm->tm_wday;
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
    }

    void after_months(int n) {
        struct tm *tm = localtime(&d);
        tm->tm_mon += n;
        d = mktime(tm);
    }

    void after_years(int n) {
        struct tm *tm = localtime(&d);
        tm->tm_year += n;
        d = mktime(tm);
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
        return !(get_wday() == 6 || get_wday() == 0);
    }

    date next_working_day() {
        date dt = *this;
        if (get_wday() == 6)
            dt.after_days(2);
        else if (get_wday() == 0)
            dt.after_days(1);
        return dt;
    }
};

class transaction {
public:
    transaction(const string &name, const date &initialDate, int f1, const string &f2, double amount) : name(name),
                                                                                                        initial_date(
                                                                                                                initialDate),
                                                                                                        ends(false),
                                                                                                        f1(f1),
                                                                                                        f2(f2),
                                                                                                        amount(amount) {}

    transaction(const string &name, const date &initialDate, bool ends, const date &finalDate, int f1, const string &f2,
                double amount)
            : name(name),
              initial_date(
                      initialDate),
              ends(ends),
              final_date(
                      finalDate),
              f1(f1),
              f2(f2),
              amount(amount) {}

public:

    string name;
    date initial_date;
    bool ends;
    date final_date;
    int f1;
    string f2;
    double amount;
};

std::vector<transaction> waiting_queue;

bool execute(date today, const transaction &row) {
    if (today.is_working_day())
        return true;
    else {
        date dt = today.next_working_day();
        waiting_queue.emplace_back(row.name, dt, true, dt, row.f1, row.f2, row.amount);
        return false;
    }
}

bool is_today(transaction &row, date today) {
    date start_date = row.initial_date;
    string f2 = row.f2;
    int f1 = row.f1;
    string d1 = today.self();
    string d2 = start_date.self();

    //if the first occurrence is in the future
    if (today < start_date)
        return false;

    //if the end date is in the past
    if (row.ends) {
        if (today > row.final_date)
            return false;
    }

    if (f2 == "weeks") {
        if (start_date.days_between(today) % (f1 * 7) != 0)
            return false;
        return execute(today, row);
    }


    if (f2 == "months") {
        if (start_date.get_day() != today.get_day())
            return false;
        if (start_date.months_between(today) % f1 != 0)
            return false;
        return execute(today, row);
    }

    if (f2 == "years") {
        if (start_date.get_day() != today.get_day())
            return false;
        if (start_date.get_month() != today.get_month())
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
    vec.emplace_back("Rent", date(25, 1, 2022), 1, "months", -1070);
    vec.emplace_back("Rundfunkbeitrag", date(15, 11, 2021), 3, "months", -55.08);
    vec.emplace_back("Netflix", date(2, 2, 2022), 1, "months", -17.99);
    vec.emplace_back("JetBrains", date(12, 4, 2022), 1, "years", -206.22);
    vec.emplace_back("Youtube Premium", date(26, 1, 2022), 1, "months", -11.99);
    vec.emplace_back("Vodafone", date(17, 1, 2022), 4, "weeks", -9.99);
    vec.emplace_back("ImmobilienScout24", date(26, 01, 2022), 1, "months", -9.98);
    vec.emplace_back("Office365", date(3, 2, 2022), 1, "months", -7);
    vec.emplace_back("Amazon Prime", date(30, 11, 2021), 1, "years", -69);
    vec.emplace_back("Kas Cloud", date(7, 12, 2021), 1, "years", -34.97);
    vec.emplace_back("Kas VPN", date(19, 12, 2021), 1, "years", -29.95);
    vec.emplace_back("Night Eye", date(17, 12, 2021), 1, "years", -9.55);
    vec.emplace_back("Cerberus", date(7, 8, 2021), 1, "years", -5);
    vec.emplace_back("Mamma", date(18, 6, 2022), true, date(18, 6, 2022), 20, "years", -400);
    vec.emplace_back("Lina", date(18, 6, 2022), true, date(18, 6, 2022), 20, "years", -500);
    vec.emplace_back("Kautz", date(18, 6, 2022), true, date(18, 6, 2022), 20, "years", -1000);
    vec.emplace_back("Refill", date(10, 3, 2022), 1, "months", 1239); //lo fai il 5 dunque assumi che arriva il 10


    ofstream myfile;
    myfile.open("schedule.asm");


    date today(4, 2, 2022);
    double var = 1301.06;
    while (today.get_year() <= 2022) {
        for (auto &el:waiting_queue) {
            process(myfile, today, var, el);
        }
        for (auto &el:vec) {
            process(myfile, today, var, el);
        }
        today.after_days(1);
    }

    cout << "Done" << endl;

    myfile.close();

    return 0;
}


