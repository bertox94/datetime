#include <iostream>
#include <chrono>
#include "classes.h"

using namespace std;
using namespace chrono;

void test(long long size) {
    auto t1 = high_resolution_clock::now();
    for (long long i = 1; i < size; i++) {
        datetime dt(i);
        if (dt.to_timestamp() != i) {
            cout << "Error 1: " << i << endl;
            return;
        }
    }
    auto t2 = high_resolution_clock::now();

    chrono::duration<double, std::milli> ms_double = t2 - t1;
    std::cout << ms_double.count() * 1000000 / size << " ns/op\n";
}

std::ostream &operator<<(std::ostream &os, datetime const &d) {
    return os << d.day << "." << d.month << "." << d.year << ", " << d.hrs << ":" << d.min << ":" << d.sec
              << std::endl;
}

int main() {

    //cout << datetime() << endl;30000000000

    test(16744099741);
    cout << "\nFaster!" << endl;

    return 0;
}


