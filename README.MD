# SIMPLE BENCHMARK

This benchmarking tool is written in C using the standard libraries. There are 2 additional scripts, a BASH shell script for building and testing, and a Python script that is used for calculating AVGs and Standard Deviations of the results.

## Introduction

There are 3 kinds of test that the tool can perform:

- Disk R/W speed test
- RAM R/W speed test
- CPU test

The disk R/W test default size is 4.1MB and ram R/W allocates 65.536kB of memory.

Additional information can be found by calling :

- ```./build.sh help``` or
- ```./bin/main help```

### Build and run

To compile just call the build script :

- ```./build.sh```

The compiled executable is saved in ./bin/ directory.

To run the simple benchmark tool with default tests call :

- ```./build.sh test```

To run all three tests 10 times and calculate the AVG and STDev.

If you don't want to perform the default tests, just edit the ./build.sh file.

To run the tool only one time, for a specific test :

- ```./bin/main [disk/ram/cpu]```

