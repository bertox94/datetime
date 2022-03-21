#include <iostream>
#include <iomanip>
#include "datetime.h"
#include <random>
#include <climits>

using namespace std;


#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc50-cpp"

static void performance_test() {
    int size = 1000000;
    _datetime av_compiler_opt;
    long long res = 0;
    _datetime dtp;
    //auto t1 = chrono::high_resolution_clock::now();
    for (long long i = 0; i < size; i++) {
        av_compiler_opt = _datetime(rand() - RAND_MAX / 2);
        //res = dtp.seconds_to(av_compiler_opt);
        if (rand() % 345)
            av_compiler_opt = _datetime(rand() - RAND_MAX / 2);
    }
    //auto t2 = chrono::high_resolution_clock::now();

    //auto t3 = chrono::high_resolution_clock::now();
    for (long long i = 0; i < size; i++) {
        av_compiler_opt = _datetime(rand() - RAND_MAX / 2);
        if (rand() % 345)
            av_compiler_opt = _datetime(rand() - RAND_MAX / 2);
    }
    //auto t4 = chrono::high_resolution_clock::now();

    //chrono::duration<double, std::milli> ms_double = t2 - t1 - (t4 - t3);
    //std::cout << "seconds_to(_datetime): " << ms_double.count() << " ns/op .."
    //          << av_compiler_opt.getYear() + res << endl;

    int avoid_compiler_optimization = 0;
    //t1 = chrono::high_resolution_clock::now();
    for (long long i = 0; i < size; i++) {
        avoid_compiler_optimization = rand() - RAND_MAX / 2;
        //dtp.after(avoid_compiler_optimization);
        if (rand() % 345)
            avoid_compiler_optimization = rand();
    }
    //t2 = chrono::high_resolution_clock::now();

    //t3 = chrono::high_resolution_clock::now();
    for (long long i = 0; i < size; i++) {
        avoid_compiler_optimization = rand() - RAND_MAX / 2;
        if (rand() % 345)
            avoid_compiler_optimization = rand();
    }
    //t4 = chrono::high_resolution_clock::now();

    //ms_double = t2 - t1 - (t4 - t3);
    //std::cout << "after(long long): " << ms_double.count() << " ns/op .."
    //          << avoid_compiler_optimization << endl;
}

#pragma clang diagnostic pop

void test2(long long size) {
    for (long long i = 0; i < size; i++) {
        _datetime dtp(i);
        _datetime dtn(-i);
        if (dtp.to_timestamp() != i) {
            cout << "Error 1: " << i << ", instead dt.timestamp()= " << _datetime(i).to_timestamp() << endl;
            return;
        }
        if (dtn.to_timestamp() != -i) {
            cout << "Error 2: " << -i << ", instead dt.timestamp()= " << _datetime(-i).to_timestamp() << endl;
            return;
        }

        if (i % 1000000 == 0)
            cout << _datetime(i) << "          " << setw(3) << (long) (i / (double) size * 100) << " %          "
                 << _datetime(size) << endl;
    }
    cout << _datetime(size) << "          " << setw(3) << (long) (size / (double) size * 100) << " %          "
         << _datetime(size) << endl;
}

void test3(long long size) {
    std::default_random_engine generator;
    std::uniform_int_distribution<long> distribution(LONG_MIN, LONG_MAX);

    for (long long i = 0; i < size; i++) {
        _datetime d1(rand(), rand(), rand() - RAND_MAX / 2, rand(), rand(), rand());
        d1 = d1.fix();
        long pt = distribution(generator);
        _datetime d2 = d1 + ss(pt) - ss(pt);

        if (d1 != d2) {
            cout << "Error: l1= " << setw(12) << d1 << ", l2= " << setw(12) << d2 << ", tt= " << pt
                 << endl;
            return;
        }

        if (i % 1000000 == 0) {
            cout << "OK: l1= " << setw(12) << d1 << ", l2= " << setw(12) << d2 << ", tt= " << pt << endl;
        }
    }
}

int main() {

    srand(time(nullptr));

    //performance_test();

    auto epoch = _datetime();

    cout << _datetime(-1, 1, 2022).fix() << endl;
    cout << _datetime(-1, 1, 2022).fix(false) << endl;

    long long size = 15000000000;
    test3(size);

    cout << "\nFaster!" << endl;

    return 0;
}


