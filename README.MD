# SIMPLE BENCHMARK

This benchmarking tool is written completely in C, there are 2 additional scripts, one for building and testing while the other one is used for calculating AVGs and STDeviations

## Introduction

There are 3 kinds of test that it can perform:

- Disk R/W speed test
- RAM R/W speed test
- CPU test

The disk R/W test default size is 4.1MB and ram R/W allocates 65536B of memory
More information by calling :

- ```./build.sh help``` or
- ```./bin/main help```

### Build and run

To only compile just call the script

- ```./build.sh```

To run the tests call :

- ```./build.sh test``` or
- ```./bin/main [disk/ram/cpu]``` runs the test 1 time

The tests are ran 10 times and AVG is calculated.
