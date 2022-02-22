#include <iostream>
#include "classes.h"

using namespace std;

void test(long long size) {
    //auto t1 = chrono::high_resolution_clock::now();
    for (long long i = 26438400; i < size; i++) {//63072000
        datetime dt(i);
        if (dt.seconds_from_epoch() != -i) {
            cout << "Error 1: " << i << ", instead dt.timestamp()= " << dt.seconds_from_epoch() << endl;
            return;
        }
    }
    //auto t2 = chrono::high_resolution_clock::now();

    //chrono::duration<double, std::milli> ms_double = t2 - t1;
    //std::cout << ms_double.count() * 1000000 / size << " ns/op\n";
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


