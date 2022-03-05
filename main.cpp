#include <iostream>
#include <iomanip>
#include <chrono>
#include "datetime.h"
#include <random>
#include <climits>

using namespace std;

void performance_test(long long size) {
    auto t1 = chrono::high_resolution_clock::now();
    for (long long i = 0; i < size; i++)
        datetime dtp(i);

    auto t2 = chrono::high_resolution_clock::now();
    chrono::duration<double, std::milli> ms_double = t2 - t1;
    std::cout << ms_double.count() * 1000000 / size << " ns/op\n";
}

void test2(long long size) {
    for (long long i = 0; i < size; i++) {
        datetime dtp(i);
        datetime dtn(-i);
        if (dtp.to_timestamp() != i) {
            cout << "Error 1: " << i << ", instead dt.timestamp()= " << datetime(i).to_timestamp() << endl;
            return;
        }
        if (dtn.to_timestamp() != -i) {
            cout << "Error 2: " << -i << ", instead dt.timestamp()= " << datetime(-i).to_timestamp() << endl;
            return;
        }

        if (i % 10000000 == 0)
            cout << datetime(i) << "          " << setw(3) << (long) (i / (double) size * 100) << " %          "
                 << datetime(size) << endl;
    }
    cout << datetime(size) << "          " << setw(3) << (long) (size / (double) size * 100) << " %          "
         << datetime(size) << endl;
}

void test3(long long size) {
    std::default_random_engine generator;
    std::uniform_int_distribution<long> distribution(LONG_MIN, LONG_MAX);

    for (long long i = 0; i < size; i++) {
        datetime d1(rand() % 60, rand() % 60, rand() % 24, 1 + rand() % 31, 1 + rand() % 12, rand() - RAND_MAX / 2,
                    true);
        period pt(distribution(generator));
        datetime d2 = d1 + pt;
        d2 = d2 - pt;

        if (d1 != d2) {
            cout << "Error: l1= " << setw(12) << d1 << ", l2= " << setw(12) << d2 << ", tt= " << pt.to_seconds()
                 << endl;
            return;
        }

        if (i % 10000000 == 0) {
            cout << "OK: l1= " << setw(12) << d1 << ", l2= " << setw(12) << d2 << ", tt= " << pt.to_seconds() << endl;
        }
    }
}

int main() {

    srand(time(nullptr));

    performance_test(10000);
    long long epoch = 15000000000;
    test3(epoch);

    cout << "\nFaster!" << endl;

    return 0;
}


