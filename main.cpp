#include <iostream>
#include <ctime>
#include <utility>
#include <vector>
#include <fstream>
#include <iomanip>
#include <list>
#include <cmath>
#include <sstream>
#include <chrono>

using namespace std;
using namespace chrono;


class date {
public:
    year year;
    month month;
    day day;
    bool last_of_month;


    date(::month m_month, ::year y_year) : last_of_month(true), day(0), month(m_month), year(y_year) {}

    date(::day d_day, ::month m_month, ::year y_year) : last_of_month(false), day(d_day), month(m_month),
                                                        year(y_year) {}

    bool operator==(shared_ptr<date> &d2) const { return false; }

    bool operator!=(shared_ptr<date> &d2) const { return false; }

    bool operator<(shared_ptr<date> &d2) const { return false; }

    bool operator>(shared_ptr<date> &d2) const { return false; }

    bool operator<=(shared_ptr<date> &d2) const { return false; }

    bool operator>=(shared_ptr<date> &d2) const { return false; }

    [[nodiscard]] ::day get_day() const {
        if (last_of_month)
            return (year / month / std::chrono::last).day();
        else
            return day;
    }

    [[nodiscard]] ::month get_month() const {
        return month;
    }

    [[nodiscard]] ::year get_year() const {
        return year;
    }

};

std::ostream &operator<<(std::ostream &os, date const &d) {
    //return os << d.what();
    return os << "";
}

class order {
public:
    string name;
    bool is_wire_transfer;
    bool ends;
    bool once;
    shared_ptr<my> initial_date;
    bool last_day;
    shared_ptr<my> final_date;
    shared_ptr<my> execution_date;
    int f1;
    string f2;
    double amount;

    //single order
    order(string name, bool is_wire_transfer,
          const shared_ptr<my> &_initial_date,
          double amount) :
            name(std::move(name)), is_wire_transfer(is_wire_transfer),
            once(true),
            last_day(false),
            amount(amount) {
        if (initial_date->what() == "date")
            initial_date = make_shared<date>(_initial_date->get_day(), _initial_date->get_month(),
                                             _initial_date->get_year());
        else
            initial_date = make_shared<end_of_month>(_initial_date->get_day(), _initial_date->get_month(),
                                                     _initial_date->get_year());
    }

    //repeating order with final my
    order(string name, bool is_wire_transfer,
          const my &initialDate, bool last_day, const my &finalDate,
          int f1, string f2,
          double amount)
            : name(std::move(name)), is_wire_transfer(is_wire_transfer),
              once(false),
              initial_date(initialDate), last_day(last_day), ends(true), final_date(finalDate),
              f1(f1), f2(std::move(f2)),
              amount(amount) {}

    //repeating order with no final date
    order(string name, bool is_wire_transfer,
          const my &initialDate, bool last_day,
          int f1, string f2,
          double amount)
            : name(std::move(name)), is_wire_transfer(is_wire_transfer),
              once(false),
              initial_date(initialDate), last_day(last_day), ends(false),
              f1(f1), f2(std::move(f2)),
              amount(amount) {}
};

class transaction {
public:
    transaction(string name, double amount) :
            name(std::move(name)), amount(amount) {}

    string name;
    double amount;
};

double account_balance;
shared_ptr<my> today;
list<order> orders;
list<transaction> transactions_of_the_day;
list<double> stats;
list<string> orders_to_print_s;
list<string> orders_to_print;
list<string> transactions_to_print;


//TODO: do checks functions to check all possibilites of wrong definition of input

//we assume that the holidays are known in advance, here the execution my is always "today"
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
    stats.push_back(bal);
    if (stats.size() > 365)
        stats.erase(stats.begin());
}

double find_m() {
    double v1 = 0, v2 = 0, v3 = 0, v4 = 0, v5 = 0;
    int i = 0;

    for (auto el:stats) {
        v1 += (i * el);
        v2 += i;
        v3 += el;
        v4 += pow(i, 2);
        v5 += i;
        i++;
    }

    v1 *= stats.size();
    v2 *= v3;
    v4 *= stats.size();
    v5 = pow(v5, 2);

    return (v1 - v2) / (v4 - v5);

}

double find_q() {
    double v1 = 0, v2 = 0;
    int i = 0;
    for (auto el:stats) {
        v1 += el;
        v2 += i;
        i++;
    }
    v2 *= find_m();

    return ((double) 1 / stats.size()) * (v1 - v2);

}

void f1() {}

void reschedule(std::list<order>::iterator &it) {}

void f2() {}

void f3(ostream &myfile) {}

void print() {}

std::string trim(const std::string &str,
                 const std::string &whitespace = " \t") {
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

void parse(string filename) {
    ifstream file(filename);

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
        } else if (row.size() == 6) {
            vector<int> nums;
            ss = stringstream(row[2]);
            while (getline(ss, data, '.')) {
                nums.push_back(stoi(data));
            }
            date dt = date(nums[0], nums[1], nums[2]);
            orders.emplace_back(row[0], row[1] == "true", dt, row[3] == "true", stoi(row[4]), row[5], stod(row[6]));
        } else if (row.size() == 7) {
            vector<int> nums;
            ss = stringstream(row[2]);
            while (getline(ss, data, '.')) {
                nums.push_back(stoi(data));
            }
            vector<int> nums2;
            ss = stringstream(row[4]);
            while (getline(ss, data, '.')) {
                nums2.push_back(stoi(data));
            }
            date dt = date(nums[0], nums[1], nums[2]);
            date dt2 = date(nums2[0], nums2[1], nums2[2]);
            orders.emplace_back(row[0], row[1] == "true", dt, row[3] == "true", dt2, stoi(row[5]), row[6],
                                stod(row[7]));
        } else
            throw runtime_error("");


        row.clear();
    }


    file.close();
}

std::chrono::day
get_max_day(std::chrono::month m, std::chrono::year y) {
    return (y / m / std::chrono::last).day();
}


int main() {

    //allow also for single orders for the last of month?

    parse("file4.txt");

    ofstream myfile;
    myfile.open("schedule.asm");

    today = make_shared<date>(15, 2, 2022);
    account_balance = 150 + 1060;

    shared_ptr<my> end = make_shared<date>(31, 12, 2022);
    while (today <= end) {
        f1();
        f2();
        f3(myfile);
        print();
        insert_stat(account_balance);
        //today.after_days(1);
    }

    cout << endl << "Done: " << "m: " << find_m() << ", q: " << find_q() << endl;

    myfile.close();

    return 0;
}


