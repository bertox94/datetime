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
    for (long long i = 0; i < size; i++) {//63072000
        long long l1 = distribution(generator) * distribution(generator);
        long long l2 = distribution(generator) * distribution(generator);

        datetime d1(l1);
        datetime d2 = after(d1, l2);
        datetime d3 = after(d2, -l2);

        if (d1 != d3) {
            cout << "Error: l1= " << l1 << ", l2= " << l2 << endl;
        }

        if (i % 1000000 == 0) {
            cout << "OK: l1= " << l1 << ", l2= " << l2 << endl;
        }

    }

    auto t2 = chrono::high_resolution_clock::now();
    chrono::duration<double, std::milli> ms_double = t2 - t1;
    std::cout << ms_double.count() * 1000000 / size << " ns/op\n";
    //std::cout << (long) ms_double.count() << " ms\n";
}

void test3(long long size) {

    std::default_random_engine generator;
    std::uniform_int_distribution<long> distribution(LONG_MIN, LONG_MAX);

    auto t1 = chrono::high_resolution_clock::now();
    for (long long i = 0; i < size; i++) {//63072000
        datetime d1(rand() % 60, rand() % 60, rand() % 60, 1 + rand() % 31, 1 + rand() % 12, rand() - RAND_MAX / 2);
        datetime d2(rand() % 60, rand() % 60, rand() % 60, 1 + rand() % 31, 1 + rand() % 12, rand() - RAND_MAX / 2);

        if (d1 != d2) {
            cout << "Error: l1= " << d1 << ", l2= " << d2 << endl;
        }

        if (i % 1000000 == 0) {
            cout << "OK: l1= " << d1 << ", l2= " << d2 << endl;
        }

    }

    auto t2 = chrono::high_resolution_clock::now();
    chrono::duration<double, std::milli> ms_double = t2 - t1;
    std::cout << ms_double.count() * 1000000 / size << " ns/op\n";
    //std::cout << (long) ms_double.count() << " ms\n";
}

int main() {

    //Error: l1= 1742863086, l2= 1438850937

    srand(time(nullptr));

    long l1 = 638828928;
    long l2 = 1297654017;

    datetime d1(l1);
    datetime d2 = after(d1, l2);
    datetime d3 = after(d2, -l2);


    long long epoch = 15000000000;
    test2(epoch); //16700000
    //test3(epoch); //16700000


    cout << "\nFaster!" << endl;

    return 0;
}


