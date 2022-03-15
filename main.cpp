#include <iostream>
#include <iomanip>
#include "datetime.h"
#include <random>
#include <climits>

using namespace std;

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

    datetime::performance_test();

    auto epoch = datetime();

    long long size = 15000000000;
    test3(size);

    cout << "\nFaster!" << endl;

    return 0;
}


