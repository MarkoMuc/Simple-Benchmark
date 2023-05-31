#include <stdio.h>
#include <time.h>

/*
 * Honestly this CPU test is bad, don't even use it
 * MZM, May 2023
 */

void work(long long loop);
double CPU_TEST(long long loop);

void work(long long loop) {
    // Fibbonacci
    long long val = 0;
    long long val1 = 0;
    long long val2 = 0;

    for(int i = 0; i < loop; i++) {
        val = val1 + val2;
        val1 = val2;
        val2 = val;
    }
}

double CPU_TEST(long long loop) {
    double start, end;
    start = (double)clock()/CLOCKS_PER_SEC;
    work(loop);
    end = (double)clock()/CLOCKS_PER_SEC;

    return end - start;
}
