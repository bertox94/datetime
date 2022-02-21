//
// Created by Halib on 21.02.2022.
//

#ifndef UNTITLED9_MYTIME_H
#define UNTITLED9_MYTIME_H


#include "utils.h"

using namespace std;

class mytime {
public:
    int a[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    long long sec;
    long long min;
    long long hrs;
    long long days;
    long long months;
    long long years;

    mytime() = default;

    mytime(long long int timestamp) {
        int anno_fin_est = 1970 + timestamp / (((double) 146097 / 400) * 24 * 60 * 60);
        long long r1 = date_to_timestamp(anno_fin_est);
        mytime res = extract_time(timestamp);
        sec = res.sec;
        min = res.min;
        hrs = res.hrs;
        timestamp = strip_time(timestamp);


        int i = 0;
        while (elapsed_since_epoch(r1).days > elapsed_since_epoch(timestamp).days) {
            r1 -= (is_leap_year(i + anno_fin_est) ? 366 : 365) * 86400;
            i++;
            anno_fin_est--;
        }
        years = anno_fin_est;
        r1 = date_to_timestamp(anno_fin_est);

        //jump years, months, days, aggiungi data ora

        long long r2 = r1;
        i = 0;
        bool fg = false;
        while (r2 <= timestamp) {
            int tmp = (is_leap_year(i + anno_fin_est) ? 366 : 365);
            r2 += tmp * 86400;
            i++;
            fg = true;
        }
        if (fg) {
            i--;
            years += i;
            int tmp = (is_leap_year(i + anno_fin_est) ? 366 : 365);
            r2 -= tmp * 86400;
        }


        i = 1;
        fg = false;
        while (r2 <= timestamp) {
            int tmp = (is_leap_year(anno_fin_est) && i == 2 ? 1 + a[i - 1] : a[i - 1]);
            r2 += tmp * 86400;
            i++;
            fg = true;
        }
        if (fg) {
            i--;
            int tmp = (is_leap_year(anno_fin_est) && i == 2 ? 1 + a[i - 1] : a[i - 1]);
            r2 -= tmp * 86400;
        }
        months = i;

        i = 1;
        fg = false;
        while (r2 <= timestamp) {
            r2 += 86400;
            i++;
            fg = true;
        }
        if (fg) {
            i--;
            r2 -= 86400;
        }
        days = i;


    }

    mytime elapsed_since_epoch(long long timestamp) {

        long long days = timestamp / (86400);
        long long hrs = (timestamp - days * 86400) / 3600;
        long long min = (timestamp - days * 86400 - hrs * 3600) / 60;
        long long sec = timestamp - days * 86400 - hrs * 3600 - min * 60;

        //cout << "Elapsed: Days: " << days << ", Hrs: " << hrs << ", Min: " << min << ", Sec: " << sec << endl;

        mytime c;
        c.days = days;
        c.hrs = hrs;
        c.min = min;
        c.sec = sec;

        return c;
    }

    mytime extract_time(long long timestamp) {
        mytime c;
        auto cc = elapsed_since_epoch(timestamp);
        c.hrs = cc.hrs;
        c.min = cc.min;
        c.sec = cc.sec;
        return c;
    }

    long long strip_time(long long timestamp) {
        mytime c = elapsed_since_epoch(timestamp);
        c.years = 0;
        c.months = 0;
        c.days = 0;
        return timestamp - duration_to_timestamp(0, c.hrs, c.min, c.sec);
    }

};


#endif //UNTITLED9_MYTIME_H
