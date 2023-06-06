#!/bin/bash

# Main script for compiling and testing
# MZM, May 2023

# verbose -> more echos
# test -> run test
# clean -> cleans bin and outs, second arg can only be test or nothing, first can be everything
# nocomp -> don't compile

verbose="${1:-nverbose}"
test="${2:-ntest}"
if [ $verbose == "test" ]; then test="test"; fi
if [ ! -d "./bin" ]; then mkdir "bin"; fi
if [ ! -d "./outs" ]; then mkdir "outs"; fi

LONGPATH="$(realpath .)"
SRC="$LONGPATH/src"
BIN="$LONGPATH/bin"
LIB="$LONGPATH/lib"

#Compile main
if [ $verbose != "nocomp" ]; then
    if [ $verbose == "verbose" ]; then echo "Compiling src"; fi
    for f in $(ls "$SRC"| grep "\.[ch]")
    do
        if [ $verbose == "verbose" ]; then echo "Compiling $f"; fi
        gcc "$SRC/$f" -o "$BIN/${f%.c}" -lm
        if [ $? -ne 0 ]; then exit -1; fi
    done
fi
#Testing

if [ $test == "test" ]; then
    rm -f "$LONGPATH/outs/disk.out"
    rm -f "$LONGPATH/outs/ram.out"
    rm -f "$LONGPATH/outs/cpu.out"

    for a in $(seq 1 10)
    do
        # Call main
        # 4Mb
        $BIN/main disk >> "$LONGPATH/outs/disk.out"
        # 8Mb
        $BIN/main ram >> "$LONGPATH/outs/ram.out"

        $BIN/main cpu 10000 >> "$LONGPATH/outs/cpu.out"
    done
    # Parses out the MB/s or Hz, removing everything else
    cat "$LONGPATH/outs/disk.out" | grep "Transfer" | grep -o "[0-9]*.[0-9]* MB/s" | sed 's/ MB\/s//' > "$LONGPATH/outs/disk.time"
    cat "$LONGPATH/outs/ram.out" | grep "Transfer" | grep -o "[0-9]*.[0-9]* MB/s" | sed 's/ MB\/s//' > "$LONGPATH/outs/ram.time"
    cat "$LONGPATH/outs/cpu.out" | grep -o "[0-9]*.[0-9]* Hz" | sed 's/ Hz//' > "$LONGPATH/outs/cpu.time"
    # Calc. averages
    ./scripts/average.py
    
fi

#HELP
if [ $verbose == "help" ]; then
    echo "# verbose -> more echos
    # test -> run test
    # clean -> cleans bin and outs, second arg can only be test or nothing, first can be everything
    # nocomp -> don't compile
    Use example : build.sh verbose -> compile with verbose information
                  build.sh verbose test -> compile with verbose info. and test
                  build.sh test -> compile and test
                  build.sh nocomp test -> only test, doesn't compile"
fi

#Clean
if [ $verbose == "clean" ]; then
    echo "Cleaning"
    rm -r "$LONGPATH/outs/"
    rm -r "$BIN"
fi

exit 0
