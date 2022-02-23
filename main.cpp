#include <iostream>
#include <chrono>
#include "classes.h"

using namespace std;

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

        if (i % 1000000 == 0)
            cout << (long) (i / (double) size * 100) << " %" << endl;
    }
    cout << 100 << " %" << endl;

    auto t2 = chrono::high_resolution_clock::now();
    chrono::duration<double, std::milli> ms_double = t2 - t1;
    //std::cout << ms_double.count() * 1000000 / size << " ns/op\n";
    //std::cout << (long) ms_double.count() << " ms\n";
}

std::ostream &operator<<(std::ostream &os, datetime const &d) {
    return os << d.day << "." << d.month << "." << d.year << ", " << d.hrs << ":" << d.min << ":" << d.sec
              << std::endl;
}


int main() {

    //cout << datetime(94670856) << endl;


    long long epoch = 15000000000;

    test(epoch); //16700000
    cout << datetime(epoch) << endl;//30000000000
    cout << "\nFaster!" << endl;

    return 0;
}


