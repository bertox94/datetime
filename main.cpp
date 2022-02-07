#include <iostream>
#include <ctime>
#include <utility>
#include <vector>
#include <fstream>
#include <iomanip>
#include <list>
#include <cmath>
#include <sstream>

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
    //the trivial operator = copies all the instance fields
    date &operator=(const date &dt) = default;

    bool operator==(date &d2) const {
        return get_year() == d2.get_year() && get_month() == d2.get_month() && get_day() == d2.get_day();
    }

    bool operator!=(date &d2) const {
        return !(*this == d2);
    }

    bool operator<(date &d2) const {
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

    bool operator>(date &d2) const {
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

    bool operator<=(date &d2) const {
        return (*this == d2 || *this < d2);
    }

    bool operator>=(date &d2) const {
        return (*this == d2 || *this > d2);
    }

    time_t d = -1;

    bool is_null() const {
        return d == -1;
    }

    int get_day() const {
        struct tm *tm = localtime(&d);
        return tm->tm_mday;
    }

    int get_month() const {
        struct tm *tm = localtime(&d);
        return tm->tm_mon;
    }

    int get_year() const {
        struct tm *tm = localtime(&d);
        return 1900 + tm->tm_year;
    }

    int get_wday() const {
        struct tm *tm = localtime(&d);
        return tm->tm_wday;
    }

    string self() const {
        if (d == -1)
            return "date undefined";
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

    bool is_working_day() const {
        return !(get_wday() == 6 || get_wday() == 0);
    }

    void first_working_day() {
        if (get_wday() == 6)
            after_days(2);
        else if (get_wday() == 0)
            after_days(1);
    }
};

std::ostream &operator<<(std::ostream &os, date const &d) {
    return os << d.self();
}

class order {
public:
    //single order
    order(string name, bool is_wire_transfer,
          const date &initialDate,
          double amount) :
            name(std::move(name)), is_wire_transfer(is_wire_transfer),
            once(true),
            initial_date(initialDate),
            amount(amount) {
    }

    //repeating order with final date
    order(string name, bool is_wire_transfer,
          const date &initialDate, const date &finalDate,
          int f1, string f2,
          double amount)
            : name(std::move(name)), is_wire_transfer(is_wire_transfer),
              once(false),
              initial_date(initialDate), ends(true), final_date(finalDate),
              f1(f1), f2(std::move(f2)),
              amount(amount) {}

    //repeating order with no final date
    order(string name, bool is_wire_transfer,
          const date &initialDate,
          int f1, string f2,
          double amount)
            : name(std::move(name)), is_wire_transfer(is_wire_transfer),
              once(false),
              initial_date(initialDate), ends(false),
              f1(f1), f2(std::move(f2)),
              amount(amount) {}

    string name;
    bool is_wire_transfer;
    bool ends;
    bool once;
    date initial_date;
    date final_date;
    date execution_date;
    int f1;
    string f2;
    double amount;
};

class transaction {
public:
    transaction(string name, double amount) :
            name(std::move(name)), amount(amount) {}

    string name;
    double amount;
};

double account_balance;
date today;
vector<order> orders;
std::list<transaction> transactions_of_the_day;
std::vector<double> stat;
vector<string> orders_to_print;
std::list<string> transactions_to_print;


//TODO: do checks functions to check all possibilites of wrong definition of input

//we assume that the holidays are known in advance, here the execution date is always "today"
void insert_in_order(string &name, double amount) {
    auto it = transactions_of_the_day.begin();

    if (it == transactions_of_the_day.end()) {
        transactions_of_the_day.emplace(it, name, amount);
        return;
    }

    while (it != transactions_of_the_day.end() && it->amount < amount) {
        ++it;
    }
    transactions_of_the_day.emplace(it, name, amount);
}

void insert_stat(double bal) {
    stat.push_back(bal);
    if (stat.size() > 365)
        stat.erase(stat.begin());
}

double find_m() {
    double v1 = 0, v2 = 0, v3 = 0, v4 = 0, v5 = 0;
    int i = 0;

    for (auto el:stat) {
        v1 += (i * el);
        v2 += i;
        v3 += el;
        v4 += pow(i, 2);
        v5 += i;
        i++;
    }

    v1 *= stat.size();
    v2 *= v3;
    v4 *= stat.size();
    v5 = pow(v5, 2);

    return (v1 - v2) / (v4 - v5);

}

double find_q() {
    double v1 = 0, v2 = 0;
    int i = 0;
    for (auto el:stat) {
        v1 += el;
        v2 += i;
        i++;
    }
    v2 *= find_m();

    return ((double) 1 / stat.size()) * (v1 - v2);

}

void f1() {

    auto it = orders.begin();

    while (it != orders.end()) {
        if (it->execution_date.is_null()) {
            //TODO:check if it ends or if it is the past, or the end date is in the past...

            date dt = it->initial_date;
            if (it->f2 == "days")
                while (dt < today)
                    dt.after_days(it->f1);
            if (it->f2 == "weeks")
                while (dt < today)
                    dt.after_days(it->f1 * 7);
            if (it->f2 == "months")
                while (dt < today)
                    dt.after_months(it->f1);
            if (it->f2 == "years")
                while (dt < today)
                    dt.after_years(it->f1);

            if (it->is_wire_transfer)
                dt.first_working_day();
            it->execution_date = dt;

            std::stringstream stream;
            stream << right << setw(14) << "Scheduled " << setw(20) << it->name << "       for: " << setw(16)
                   << it->execution_date << setw(13) << it->amount << endl;
            orders_to_print.push_back(stream.str());

            order current = *it;
            it = orders.erase(it);
            orders.push_back(current);
        } else
            ++it;
    }
}

void reschedule(std::vector<order>::iterator &it) {
    if (it->once) {
        std::stringstream stream;
        stream << right << setw(14) << "Stopped " << setw(20) << it->name << endl;
        orders_to_print.push_back(stream.str());
        return;
    }

    date dt = it->initial_date;
    if (it->f2 == "days")
        while (dt <= today)
            dt.after_days(it->f1);
    if (it->f2 == "weeks")
        while (dt <= today)
            dt.after_days(it->f1 * 7);
    if (it->f2 == "months")
        while (dt <= today)
            dt.after_months(it->f1);
    if (it->f2 == "years")
        while (dt <= today)
            dt.after_years(it->f1);
    if (it->is_wire_transfer)
        dt.first_working_day();

    if (it->ends && it->final_date < dt) {
        std::stringstream stream;
        stream << right << setw(14) << "Stopped " << setw(20) << it->name << endl;
        orders_to_print.push_back(stream.str());
        return;
    }

    it->execution_date = dt;
    std::stringstream stream;
    stream << right << setw(14) << "Rescheduled " << setw(20) << it->name << "       for: " << setw(16)
           << it->execution_date << endl;
    orders_to_print.push_back(stream.str());

    order current = *it;
    orders.push_back(current);
}

void f2() {
    auto it = orders.begin();

    while (it != orders.end()) {
        if (it->execution_date == today) {
            insert_in_order(it->name, it->amount);
            reschedule(it);
            it = orders.erase(it);
        } else
            ++it;
    }
}

void f3(ostream &myfile) {
    for (auto &el:transactions_of_the_day) {
        account_balance += el.amount;
        myfile << right << setw(16) << today.self() << setw(20) << el.name << setw(10) << el.amount
               << setw(10);
        if (account_balance <= 0) {
            myfile << "=============================> ";
        }
        myfile << account_balance << endl;
        std::stringstream stream;
        stream << right << setw(14) << "Executed " << setw(20) << el.name << setw(41) << el.amount
               << endl;
        transactions_to_print.push_back(stream.str());
    }
    transactions_of_the_day.clear();
}

void print() {
    if (!orders_to_print.empty() || !transactions_to_print.empty())
        cout << "On " << today << endl;

    for (auto &el: transactions_to_print)
        cout << el;
    for (auto &el: orders_to_print)
        cout << el;

    if (!orders_to_print.empty() || !transactions_to_print.empty())
        cout << "--------------------------------------------------" << endl;
    orders_to_print.clear();
    transactions_to_print.clear();
}

std::string trim(const std::string &str,
                 const std::string &whitespace = " \t") {
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

void parse() {
    ifstream file("file.txt");

    vector<string> lines;
    string line;

    while (getline(file, line)) {
        if (line[0] == '#')
            continue;
        std::stringstream ss(line);
        vector<string> row;
        string data;
        while (getline(ss, data, ';')) {
            data = trim(data);
            row.push_back(data);
        }

        if (row.size() == 4) {
            vector<int> nums;
            ss = stringstream(row[2]);
            while (getline(ss, data, '.')) {
                nums.push_back(stoi(data));
            }
            date dt = date(nums[0], nums[1], nums[2]);
            orders.emplace_back(row[0], row[1] == "true", dt, stod(row[3]));
        }

        if (row.size() == 6) {
            vector<int> nums;
            ss = stringstream(row[2]);
            while (getline(ss, data, '.')) {
                nums.push_back(stoi(data));
            }
            date dt = date(nums[0], nums[1], nums[2]);
            orders.emplace_back(row[0], row[1] == "true", dt, stoi(row[3]), row[4], stod(row[5]));
        }

        if (row.size() == 7) {
            vector<int> nums;
            ss = stringstream(row[2]);
            while (getline(ss, data, '.')) {
                nums.push_back(stoi(data));
            }
            vector<int> nums2;
            ss = stringstream(row[3]);
            while (getline(ss, data, '.')) {
                nums2.push_back(stoi(data));
            }
            date dt = date(nums[0], nums[1], nums[2]);
            date dt2 = date(nums2[0], nums2[1], nums2[2]);
            orders.emplace_back(row[0], row[1] == "true", dt, dt2, stoi(row[4]), row[5], stod(row[6]));
        }


        row.clear();
    }


    file.close();
}


int main() {

    parse();

    ofstream myfile;
    myfile.open("schedule.asm");

    today = date(5, 2, 2022);
    account_balance = 1301.06;

    date end(31, 12, 2022);
    while (today <= end) {
        f1();
        f2();
        f3(myfile);
        print();
        insert_stat(account_balance);
        today.after_days(1);
    }

    cout << endl << "Done: " << "m: " << find_m() << ", q: " << find_q() << endl;

    myfile.close();

    return 0;
}


