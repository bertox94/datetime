#include <iostream>
#include <iomanip>
#include <chrono>
#include "datetime.h"
#include <random>
#include <climits>

using namespace std;

void performance_test(long long size) {
    datetime dtp;
    datetime av_compiler_opt;

    auto t1 = chrono::high_resolution_clock::now();
    for (long long i = 0; i < size; i++) {
        av_compiler_opt = datetime(rand() - RAND_MAX / 2);
        //dtp.seconds_to(av_compiler_opt);
        if (rand() % 345)
            av_compiler_opt = datetime(rand() - RAND_MAX / 2);
    }
    auto t2 = chrono::high_resolution_clock::now();

    auto t3 = chrono::high_resolution_clock::now();
    for (long long i = 0; i < size; i++) {
        av_compiler_opt = datetime(rand() - RAND_MAX / 2);
        if (rand() % 345)
            av_compiler_opt = datetime(rand() - RAND_MAX / 2);
    }
    auto t4 = chrono::high_resolution_clock::now();

    chrono::duration<double, std::milli> ms_double = t2 - t1 - (t4 - t3);
    std::cout << "seconds_to(datetime): " << ms_double.count() * 1000000 / size << " ns/op .."
              << av_compiler_opt << endl;

    int avoid_compiler_optimization = 0;
    t1 = chrono::high_resolution_clock::now();
    for (long long i = 0; i < size; i++) {
        avoid_compiler_optimization = rand() - RAND_MAX / 2;
        //dtp.after(avoid_compiler_optimization);
        if (rand() % 345)
            avoid_compiler_optimization = rand();
    }
    t2 = chrono::high_resolution_clock::now();

    t3 = chrono::high_resolution_clock::now();
    for (long long i = 0; i < size; i++) {
        avoid_compiler_optimization = rand() - RAND_MAX / 2;
        if (rand() % 345)
            avoid_compiler_optimization = rand();
    }
    t4 = chrono::high_resolution_clock::now();

    ms_double = t2 - t1 - (t4 - t3);
    std::cout << "after(long long): " << ms_double.count() * 1000000 / size << " ns/op .."
              << avoid_compiler_optimization << endl;
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

        if (i % 1000000 == 0)
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
        datetime d1(1 + rand() % 31, 1 + rand() % 12, rand() - RAND_MAX / 2, rand() % 24, rand() % 60, rand() % 60,
                    true);
        period pt(distribution(generator));
        datetime d2 = d1 + pt - pt;

        if (d1 != d2) {
            cout << "Error: l1= " << setw(12) << d1 << ", l2= " << setw(12) << d2 << ", tt= " << pt.to_seconds()
                 << endl;
            return;
        }

        if (i % 1000000 == 0) {
            cout << "OK: l1= " << setw(12) << d1 << ", l2= " << setw(12) << d2 << ", tt= " << pt.to_seconds() << endl;
        }
    }
}

int main() {

    srand(time(nullptr));

    auto epoch = datetime();

    //performance_test(1000000);
    long long epochs = 15000000000;
    test3(epochs);

    cout << "\nFaster!" << endl;

    return 0;
}


