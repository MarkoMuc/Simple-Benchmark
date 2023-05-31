#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * You can change CHUNK_SIZE here.
 * MZM, May 2023
 */

// Types
typedef struct {
    double sec; // Time in seconds
    double BperS; // Bytes per second
} MemRes;

typedef struct {
    uint8_t** memoryMap; // Pointer to the allocated memory
    int dimMapX; // X dimension
    int dimMapY; // Y dimension
} MemMap;

// Function Prototypes
void compute(int n);
double cpu_speed(int n);
uint8_t getRandom();
MemRes READ_TEST(FILE* read, uint8_t* buffer, int buffSize, long readwrites);
MemRes WRITE_TEST(FILE* write, uint8_t* buffer, int buffSize, long readwrites);
MemRes RAM_READ_TEST(MemMap* map, int* indexX, int* indexY, long maxRuns);
MemRes RAM_WRITE_TEST(MemMap* map, int* indexX, int* indexY, long maxRuns);

// Macros
#define CHUNK_SIZE 256
// KibiBytes constant
#define MIBKCONST 9.53674316*1e-4
// MebiBytes constant
#define MIBMCONST 9.53674316*1e-7
