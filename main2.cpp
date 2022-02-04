#include <iostream>
#include <ctime>
#include <vector>
#include <fstream>

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

    bool operator==(date &d2) {
        return get_month() == d2.get_month() && get_day() == d2.get_day() && get_year() == d2.get_year();
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

    string self() {
        string self = ctime(&d);
        int len = self.length();
        return self.substr(0, len - 1);
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
};

class row {
public:
    row(const string &name, const date &initialDate, int f1, const string &f2, double amount) : name(name),
                                                                                                initial_date(
                                                                                                        initialDate),
                                                                                                f1(f1),
                                                                                                f2(f2),
                                                                                                amount(amount) {}

public:

    string name;
    date initial_date;
    int f1;
    string f2;
    double amount;
};


bool is_today(row &row, date today) {
    date start_date = row.initial_date;
    string f2 = row.f2;
    int f1 = row.f1;

    if (f2 == "weeks") {
        if (start_date.days_between(today) % f1 != 0)
            return false;
        return true;
    }


    if (f2 == "months") {
        if (start_date.get_day() != today.get_day())
            return false;
        if (start_date.months_between(today) % f1 != 0)
            return false;
        return true;
    }

    if (f2 == "years") {
        if (start_date.get_day() != today.get_day())
            return false;
        if (start_date.get_month() != today.get_month())
            return false;
        if (start_date.years_between(today) % f1 != 0)
            return false;
        return true;
    }


    return false;
}


int main() {

    vector<row> vec = {
            row("Rent", date(25, 10, 2021), 5, "months", -1070),
            row("Rundfunkbeitrag", date(15, 11, 2021), 3, "months", -55.08),
            row("Netflix", date(2, 2, 2022), 1, "months", -17.99),
            row("JetBrains", date(12, 4, 2022), 1, "years", -206.22),
            row("Youtube Premium", date(26, 1, 2022), 1, "months", -11.99),
            row("Vodafone", date(17, 1, 2022), 4, "weeks", -9.99),
            row("ImmobilienScout24", date(26, 01, 2022), 1, "months", -9.98),
            row("Office 365", date(3, 2, 2022), 1, "months", -7),
            row("Amazon Prime", date(30, 11, 2021), 1, "years", -69),
            row("Kas Cloud", date(7, 12, 2021), 1, "years", -34.97),
            row("Kas VPN", date(19, 12, 2021), 1, "years", -29.95),
            row("Night Eye", date(17, 12, 2021), 1, "years", -9.55),
            row("Cerberus", date(7, 8, 2021), 1, "years", -5),
            row("Refill", date(25, 12, 2021), 1, "months", 1180)
    };

    ofstream myfile;
    myfile.open("schedule.txt");


    date today(25, 12, 2021);
    double var = 0;
    int k = 0;
    while (today.get_year() <= 2022) {
        for (auto &el:vec) {
            if (is_today(el, today)) {
                var += el.amount;
                myfile << today.self() << " " << el.name << " " << el.amount;
                if (var <= 0)
                    myfile << "=============================>";
                myfile << var << endl;
            }
        }
        k++;
        if (k == 372)
            cout << k;
        today.after_days(1);
    }

    cout << "Done" << endl;

    myfile.close();

    return 0;
}
