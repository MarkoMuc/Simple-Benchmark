#!/bin/python
import numpy as np

# Just calculates the AVG times of all tests and the Standard deviation
# MZM. May 2023


# DISK
disk = open("./outs/disk.time", "r")
sumW = 0.0
sumR = 0.0
lwrite = []
lread = []
count = 0
for line in disk.readlines() :
    val = float(line)
    if count % 2 == 0:
        lwrite.append(val)
        sumW += val
    else :
        lread.append(val)
        sumR += val
    count += 1
count /=2
print("Write DISK AVG: ", sumW / count, "MB/s", "\n\t STDDEV ", np.std(lwrite))
print("Read DISK AVG: ", sumR / count, "MB/s",  "\n\t STDDEV ", np.std(lread))
disk.close()

# RAM
ram = open("./outs/ram.time", "r")
sumW = 0.0
sumR = 0.0
lwrite = []
lread = []
count = 0
for line in ram.readlines() :
    val = float(line)
    if count % 2 == 0:
        lwrite.append(val)
        sumW += val
    else :
        lread.append(val)
        sumR += val
    count += 1
count /=2
print("Write RAM AVG: ", sumW / count, "MB/s", "\n\t STDDEV ", np.std(lwrite))
print("Read RAM AVG: ", sumR / count, "MB/s", "\n\t STDDEV ", np.std(lread))
ram.close()

# CPU
cpu = open("./outs/cpu.time", "r")
sum = 0.0
count = 0
ltime = []
for line in cpu.readlines() :
    count += 1
    ltime.append(float(line))
    sum += float(line)
print("CPU AVG: ", sum / count, "Hz", "\n\t STDDEV ", np.std(ltime))
