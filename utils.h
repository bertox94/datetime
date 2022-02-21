//
// Created by Halib on 21.02.2022.
//

#ifndef UNTITLED9_UTILS_H
#define UNTITLED9_UTILS_H

bool is_leap_year(int year) {
    return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}

long long num_anni_bis(long a1, long a2) {
    long r1 = a2 / 4 - a1 / 4;
    long r2 = a2 / 100 - a1 / 100;
    long r3 = a2 / 400 - a1 / 400;

    return r1 - r2 + r3;
}

long long duration_to_timestamp(long days, long hrs, long min, long sec) {
    return days * 86400 + hrs * 3600 + min * 60 + sec;
}

long long date_to_timestamp(long year) {
    long long var = (year - 1970) * 365 + num_anni_bis(1970, year - 1);
    return var * 86400;
}


#endif //UNTITLED9_UTILS_H
