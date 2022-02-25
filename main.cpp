#include <iostream>
#include <iomanip>
#include <chrono>
#include "classes.h"
#include <random>
#include <climits>

using namespace std;

void test1(long long size) {
    auto t1 = chrono::high_resolution_clock::now();
    for (long long i = 0; i < size; i++) {//63072000
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

    auto t2 = chrono::high_resolution_clock::now();
    chrono::duration<double, std::milli> ms_double = t2 - t1;
    std::cout << ms_double.count() * 1000000 / size << " ns/op\n";
    //std::cout << (long) ms_double.count() << " ms\n";
}

void test2(long long size) {

    std::default_random_engine generator;
    std::uniform_int_distribution<long> distribution(LONG_MIN, LONG_MAX);

    auto t1 = chrono::high_resolution_clock::now();
    for (long long i = 0; i < size; i++) {
        datetime d1(rand() % 60, rand() % 60, rand() % 24, 1 + rand() % 31, 1 + rand() % 12, rand() - RAND_MAX / 2);
        long long tt = distribution(generator);
        datetime d2 = after(after(d1, tt), -tt);

        if (d1 != d2) {
            cout << "Error: l1= " << setw(12) << d1 << ", l2= " << setw(12) << d2 << ", tt= " << tt << endl;
            return;
        }

        if (i % 10000000 == 0) {
            cout << "OK: l1= " << setw(12) << d1 << ", l2= " << setw(12) << d2 << ", tt= " << tt << endl;
        }

    }

    auto t2 = chrono::high_resolution_clock::now();
    chrono::duration<double, std::milli> ms_double = t2 - t1;
    std::cout << ms_double.count() * 1000000 / size << " ns/op\n";
    //std::cout << (long) ms_double.count() << " ms\n";
}

int main() {

    srand(time(nullptr));

    // tt= 1775435781
    //datetime d1(29, 25, 2, 31, 11, -16076);
    datetime d1(29, 25, 2, 31, 11, -4);
    datetime d2 = after(d1, 1);
    datetime d3 = after(d2, -1);


    long long epoch = 15000000000;
    test2(epoch); //16700000


    cout << "\nFaster!" << endl;

    return 0;
}


