#include <iostream>
#include <chrono>
#include "classes.h"

using namespace std;

void test(long long size) {
    auto t1 = chrono::high_resolution_clock::now();
    for (long long i = 0; i < size; i++) {//63072000
        if (datetime(i).to_timestamp() != i) {
            cout << "Error 1: " << i << ", instead dt.timestamp()= " << datetime(i).to_timestamp() << endl;
            return;
        }
    }
    auto t2 = chrono::high_resolution_clock::now();

    chrono::duration<double, std::milli> ms_double = t2 - t1;
    std::cout << ms_double.count() * 1000000 / size << " ns/op\n";
}

std::ostream &operator<<(std::ostream &os, datetime const &d) {
    return os << d.day << "." << d.month << "." << d.year << ", " << d.hrs << ":" << d.min << ":" << d.sec
              << std::endl;
}


int main() {

    cout << datetime(16744079044) << endl;//30000000000

    long start_year = 1970;
    long seconds = 63072000;

    test(1670000000); //16700000
    cout << "\nFaster!" << endl;

    return 0;
}


