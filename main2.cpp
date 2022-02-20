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
    unsigned int day{};
    unsigned int month{};
    int year{};
    bool valid = false;

    date() = default;

    explicit date(year_month_day ymd) : day(ymd.day()), month(ymd.month()), year(ymd.year()), valid(true) {}

    date(::month m_month, ::year y_year) : day(0), month(m_month), year(y_year), valid(true) {}

    date(::day d_day, ::month m_month, ::year y_year) : day(d_day), month(m_month), year(y_year), valid(true) {}

    bool operator==(date &d2) const {
        return year == d2.year && month == d2.month && day == d2.day;
    }

    bool operator!=(date &d2) const {
        return !(*this == d2);
    }

    bool operator<(date &d2) const {
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

    bool operator>(date &d2) const {
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

    bool operator<=(date &d2) const {
        return (*this == d2 || *this < d2);
    }

    bool operator>=(date &d2) const {
        return (*this == d2 || *this > d2);
    }

    date last_day_date() {
        auto ymt = ::year(year) / ::month(month) / last;
        return date(ymt);
    }

    [[nodiscard]] date add(unsigned int days, unsigned int months, int years) const {
        year_month_day ymt = ::year(year) / ::month(month) / ::day(day);

        ymt = sys_days{ymt.year() / ymt.month() / ::day(ymt.day().operator unsigned int() + days)};
        ymt = sys_days{ymt.year() / ::month(ymt.month().operator unsigned int() + months) / ymt.day()};
        ymt = sys_days{::year(ymt.year().operator int() + years) / ymt.month() / ymt.day()};

        return date(ymt);
    }

    bool is_valid() {
        return valid;
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
    bool last_day_of_month;
    date initial_date;
    date final_date;
    date execution_date;
    int f1;
    string f2;
    double amount;

    //single order
    order(string name, bool is_wire_transfer, const date &_initial_date, bool _last_day_of_month, double amount) :
            name(std::move(name)),
            is_wire_transfer(is_wire_transfer),
            initial_date(_initial_date),
            last_day_of_month(_last_day_of_month),
            once(true),
            amount(amount) {}

    //repeating order with final my
    order(string name, bool is_wire_transfer, const date &initialDate, const date &finalDate, bool _last_day_of_month,
          int f1, string f2, double amount)
            : name(std::move(name)),
              is_wire_transfer(is_wire_transfer),
              once(false),
              initial_date(initialDate),
              final_date(finalDate),
              last_day_of_month(_last_day_of_month),
              ends(true),
              f1(f1),
              f2(std::move(f2)),
              amount(amount) {}

    //repeating order with no final date
    order(string name, bool is_wire_transfer, const date &initialDate, bool _last_day_of_month, int f1, string f2,
          double amount)
            : name(std::move(name)),
              is_wire_transfer(is_wire_transfer),
              once(false),
              initial_date(initialDate),
              last_day_of_month(_last_day_of_month),
              ends(false),
              f1(f1),
              f2(std::move(f2)),
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
date today;
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

void f1() {
    auto it = orders.begin();

    while (it != orders.end()) {
        if (!it->execution_date.is_valid()) {
            //TODO:check if it ends or if it is the past, or the end date is in the past...

            int i = 1;
            date dt = it->initial_date;
            date dt2 = dt;
            if (it->f2 == "days")
                while (dt2 < today) {
                    dt2 = dt;
                    dt2.after_days(it->f1 * i);
                    i++;
                }
            if (it->f2 == "weeks")
                while (dt2 < today) {
                    dt2 = dt;
                    dt2.after_days(it->f1 * 7 * i);
                    i++;
                }
            if (it->f2 == "months") {
                if (it->last_day) {
                    while (dt2 < today) {
                        dt2 = dt;
                        dt2.after_months(it->f1 * i);
                        dt2.last_day_of("month");
                        i++;
                    }
                } else {
                    while (dt2 < today) {
                        dt2 = dt;
                        dt2.after_months(it->f1 * i);
                        i++;
                    }
                }
            }
            if (it->f2 == "years")
                while (dt2 < today) {
                    dt2 = dt;
                    dt2.after_years(it->f1 * i);
                    i++;
                }

            dt = dt2;
            if (it->is_wire_transfer)
                dt.first_working_day();
            it->execution_date = dt;

            std::stringstream stream;
            stream << right << setw(14) << "Scheduled " << setw(20) << it->name << "       for: " << setw(16)
                   << it->execution_date << setw(13) << it->amount << endl;
            orders_to_print_s.push_back(stream.str());

            order current = *it;
            orders.push_back(current);
            it = orders.erase(it);
        } else
            ++it;
    }
}

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
            date dt;
            if (nums.size() == 2)
                dt = date(month(nums[0]), year(nums[1]));
            else
                dt = date(day(nums[0]), month(nums[1]), year(nums[2]));

            orders.emplace_back(row[0], row[1] == "true", dt, stod(row[3]));
        } else if (row.size() == 6) {
            vector<int> nums;
            ss = stringstream(row[2]);
            while (getline(ss, data, '.')) {
                nums.push_back(stoi(data));
            }
            date dt;
            if (nums.size() == 2)
                dt = date(month(nums[0]), year(nums[1]));
            else
                dt = date(day(nums[0]), month(nums[1]), year(nums[2]));
            orders.emplace_back(row[0], row[1] == "true", dt, stoi(row[3]), row[4], stod(row[5]));
        } else if (row.size() == 7) {
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
            date dt;
            date dt2;
            if (nums.size() == 2) {
                dt = date(month(nums[0]), year(nums[1]));
                dt2 = date(month(nums2[0]), year(nums2[1]));
            } else {
                dt = date(day(nums[0]), month(nums[1]), year(nums[2]));
                dt2 = date(day(nums2[0]), month(nums2[1]), year(nums2[2]));
            }
            orders.emplace_back(row[0], row[1] == "true", dt, dt2, stoi(row[4]), row[5],
                                stod(row[6]));
        } else
            throw runtime_error("");


        row.clear();
    }


    file.close();
}

int main() {

    //allow also for single orders for the last of month? Yes
    //ordini mensili giorno max 28 oppure ultimo del mese

    auto now = (year(1)/month(1)/day(1));
    auto today = floor<days>(now);
    year_month_day future = today + days{100};
    //std::cout << future << '\n';

    parse("file4.txt");

    ofstream myfile;
    myfile.open("schedule.asm");

    today = date(day(15), month(2), year(2022));
    account_balance = 150 + 1060;

    date end(day(31), month(12), year(2022));
    while (today <= end) {
        f1();
        f2();
        f3(myfile);
        print();
        insert_stat(account_balance);
        today = today.add(1, 0, 0);
    }

    cout << endl << "Done: " << "m: " << find_m() << ", q: " << find_q() << endl;

    myfile.close();

    return 0;
}


