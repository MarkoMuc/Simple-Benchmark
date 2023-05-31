#include "../lib/memtest.c"
#include "../lib/cputest.c"
#include <string.h>

/*
 * The main function just checks for args, prints help if non are given
 *  - "disk" -> performs disk transfer speed test, default is 4.1MB file
 *  - "ram" -> performs ram transfer speed test, default is 0.8MB of data,
 *              default size of allocated memory is 256x256 * 8B
 *  - "cpu" -> tests cpu freq., default is 1000 iterations of fib., DONT EVEN USE IT ITS BAD!!!!!!!!
 *
 * Buffer size can be changed
 * MZM, May 2023
 */
void storage_test(const char path[], long maxRuns);
void dyn_mem_test(int sizeX, int sizeY, long maxRuns);
void cpu_test(long long loop);

// Set this to change the size of the buffer
long bufferSize = 4096L;
// For multiplying the size of the memory map, at start its 256x256
int multiplier = 1;
int main(int argc, char** argv) {

    if(argc > 1 && strcmp(argv[1], "disk") == 0) {
        long maxRuns = 1000;
        if(argc > 2) {
            maxRuns = atol(argv[2]);
        }
       const char* path = "./outs/temp.out";
       storage_test(path, maxRuns);
    } else if (argc > 1 && strcmp(argv[1], "ram") == 0) {
        int x = 256 * multiplier;
        int y = 256 * multiplier;
        long maxRuns = 100000;

        if (argc > 2) {
            x = atoi(argv[2]);
            y = atoi(argv[3]);
            maxRuns = atol(argv[4]);
        }

        dyn_mem_test(x, y, maxRuns);
    } else if (argc > 1 && strcmp(argv[1], "cpu") == 0) {
        long long loop = 1000;
        if (argc > 2) {
            loop = atol(argv[2]);
        }

        cpu_test(loop);
    } else {
        printf("HELP!\n");
        printf("Every iteration of R/W tests reads/writes 4096Bytes, meaning total amount of data is bufferSize * maxRuns\n");
        printf("\t- DISK R/W speed test : disk [maxRuns] | default is 1000 = 4.1MB\n");
        printf("\t- RAM R/W speed test: ram [size x] [size y] [maxRuns] | default is 1000000 = 8.0MB\n");
        printf("\t- CPU speed test : cpu [complexity] | Dont even use honestly\n");
    }

    return 0;
}
/*
 * Runs the disk R/W test on maxRuns * bufferSize of data
 * Parameters :
 *  - const char path[] -> path to the temp file
 *  - long maxRuns -> number of runs to perform each test
 */
void storage_test(const char path[], long maxRuns) {
    // Open file
    FILE* f = fopen(path, "wb+");
    if( f == NULL) {
        printf("Failed opening file\n");
        exit(EXIT_FAILURE);
    }

    // Prepare Buffer
    bufferSize = bufferSize * sizeof(uint8_t);
    uint8_t* buff = (uint8_t*) malloc(bufferSize);

    // BufferSize is the number of elements in the buffer, it is not the size of the buffer in bytes
    bufferSize /= sizeof(uint8_t);

    for(int i = 0; i < bufferSize; i++ ){
        buff[i] = 77;
    }

    // Starts Write test

    double sizeoffile = bufferSize * maxRuns;
    printf("Write test of size [ %.1f MB | %.1f MiB ]\n", sizeoffile / 1.0e6, sizeoffile * MIBMCONST);
    MemRes rez = WRITE_TEST(f, buff, bufferSize, maxRuns);
    printf("\tTransfer : [ %.4f MB/s | %.4f MiB/s ]\n",  rez.BperS * 1.0e-6, rez.BperS * MIBMCONST);

    // Reset reading index to start of the file
    fseek(f, 0, SEEK_SET);

    // Start Read test

    printf("Read test of size [ %.1f MB | %.1f MiB ]\n", sizeoffile / 1.0e6, sizeoffile * MIBMCONST );
    rez = READ_TEST(f, buff, bufferSize, maxRuns);
    printf("\tTransfer : [ %.4f MB/s | %.4f MiB/s ]\n",  rez.BperS * 1.0e-6, rez.BperS * MIBMCONST);


    // Close files and free memory
    fclose(f);
    remove(path);
    free(buff);
}
/*
 * Runs the ram R/W test on maxRuns * bufferSize of data
 * Parameters :
 *  - int sizeX -> used for the size of the memory map
 *  - int sizeY -> used for the size of the memory map
 *  - long maxRuns -> number of runs to perform each test
 */
void dyn_mem_test(int sizeX, int sizeY, long maxRuns) {

    // Prepares the memory map
    MemMap memoryMap;
    int* randomX;
    int* randomY;

    memoryMap.dimMapX = sizeX;
    memoryMap.dimMapY = sizeY;
    memoryMap.memoryMap = (uint8_t**) malloc(sizeof(uint8_t*) * sizeX);

    for(int i = 0; i < sizeX; i++) {
        memoryMap.memoryMap[i] = (uint8_t*) malloc(sizeof(uint8_t) * sizeY);
    }

    // Generates random indexes
    randomX = (int*) malloc(maxRuns * sizeof(int));
    randomY = (int*) malloc(maxRuns * sizeof(int));

    for(int i = 0; i < maxRuns; i++) {
        randomX[i] = (long) rand() % sizeX;
        randomY[i] = (long) rand() % sizeY;
    }

    double size = maxRuns * sizeof(uint8_t) * 8;
    printf("RAM Write test of size [ %.1f MB | %.1f MiB ]\n", size / 1.0e6, size * MIBMCONST);
    MemRes rez = RAM_WRITE_TEST(&memoryMap, randomX, randomY, maxRuns);
    printf("\tTransfer : [ %.4f MB/s | %.4f MiB/s ]\n",  rez.BperS * 1.0e-6, rez.BperS * MIBMCONST);

    // Start Read test

    printf("RAM Read test of size [ %.1f MB | %.1f MiB ]\n", size / 1.0e6, size * MIBMCONST );
    rez = RAM_READ_TEST(&memoryMap, randomX, randomY, maxRuns);
    printf("\tTransfer : [ %.4f MB/s | %.4f MiB/s ]\n",  rez.BperS * 1.0e-6, rez.BperS * MIBMCONST);

    // Frees memory

    for(int i = 0; i < sizeX; i++) {
        free(memoryMap.memoryMap[i]);
    }
    free(memoryMap.memoryMap);
}
/*
 * Tests CPU freq.
 * Parameters :
 *  - long long loop -> number of iterations of fib. to perform
 */
void cpu_test(long long loop) {
    double time = CPU_TEST(loop);
    printf("\tFreq of CPU : %f Hz\n", 1/time);
}
