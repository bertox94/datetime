#include <iostream>
#include <iomanip>
#include <chrono>
#include "classes.h"

using namespace std;

std::ostream &operator<<(std::ostream &os, datetime const &d) {
    return os << std::setfill('0') << std::setw(2) << d.day << "." << std::setfill('0') << std::setw(2) << d.month
              << "." << std::setfill('0') << std::setw(4) << d.year << ", " << std::setfill('0') << std::setw(2)
              << d.hrs << ":" << std::setfill('0') << std::setw(2) << d.min << ":" << std::setfill('0') << std::setw(2)
              << d.sec;
}

void test(long long size) {
    auto t1 = chrono::high_resolution_clock::now();
    for (long long i = 0; i < size; i++) {//63072000
        //datetime dt(i);
        if (datetime(i).to_timestamp() != i) {
            cout << "Error 1: " << i << ", instead dt.timestamp()= " << datetime(i).to_timestamp() << endl;
            return;
        }
        if (datetime(-i).to_timestamp() != -i) {
            cout << "Error 2: " << -i << ", instead dt.timestamp()= " << datetime(-i).to_timestamp() << endl;
            return;
        }

        if (i % 10000000 == 0) //10000000
            cout << datetime(i) << "          " << (long) (i / (double) size * 100) << " %          " << datetime(size)
                 << endl;
    }
    cout << 100 << " %" << endl;

    auto t2 = chrono::high_resolution_clock::now();
    chrono::duration<double, std::milli> ms_double = t2 - t1;
    //std::cout << ms_double.count() * 1000000 / size << " ns/op\n";
    //std::cout << (long) ms_double.count() << " ms\n";
}


int main() {

    //cout << datetime(94670856) << endl;

    period pd(10);
    pd=pd.to_negative();

    long long epoch = 15000000000;

    test(epoch); //16700000
    cout << datetime(epoch) << endl;//30000000000
    cout << "\nFaster!" << endl;

    return 0;
}


