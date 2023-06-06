#include "memtest.h"

/*
 * MASS STORAGE DISK R/W TEST FUNCTIONS
 * RAM R/W TEST FUNCTIONS
 * MZM, May 2023
 */

/* Reads buffSize of Bytes at a time off the disk, for given amount of reads,
 * calculates time and transfer Parameters:
 *     - FILE* read -> file pointer to the file to read
 *     - uint8_t* buffer -> buffer, not really used for anything else than
 * checking for fail
 *     - int buffSize -> size of the buffer, reads this amount of bytes per loop
 *     - long maxRuns -> the number of reads that it should perform,
 *        |-note that it does buffSize * maxRuns of actual reads, since it reads
 * 4096 per every iteration of maxRuns
 */
MemRes READ_TEST(FILE *read, uint8_t *buffer, int buffSize, long maxRuns) {
  MemRes memRes;
  clock_t clkstart;
  long sizeoffile = 0;

  // Reads READWRITES times into the buffer

  clkstart = clock();

  for (int i = 0; i < maxRuns; i++) {
    for (int j = 0; j < buffSize; j++) {

      int readB = fgetc(read);
      if (readB != buffer[j]) {
        printf("Failed to write\n");
        exit(EXIT_FAILURE);
      } else {
        sizeoffile++;
      }
    }

    fflush(read);
  }

  // Time in seconds
  double seconds = ((double)clock() - clkstart) / CLOCKS_PER_SEC;

  // Read speed in B/s
  double readSpeed = (sizeoffile * sizeof(uint8_t)) / seconds;

  memRes.sec = seconds;
  memRes.BperS = readSpeed;

  return memRes;
}

/* Writes 4096 Bytes at onto the disk, for given amount of writes, calculates
 * time and transfer Parameters:
 *     - FILE* write -> file pointer to the file to write to
 *     - uint8_t* buffer -> buffer that holds values to write into the file
 *     - int buffSize -> size of the buffer, writes this amount of bytes per
 * loop
 *     - long maxRuns -> the number of writes that it should perform,
 *        |-note that it does buffSize * maxRuns of actual writes, since it
 * writes 4096 per every iteration of maxRuns
 */
MemRes WRITE_TEST(FILE *write, uint8_t *buffer, int buffSize, long maxRuns) {
  MemRes memRes;
  clock_t clkstart;
  long sizeoffile = 0;

  // Writes READWRITES times out of the buffer
  clkstart = clock();

  for (int i = 0; i < maxRuns; i++) {
    for (int j = 0; j < buffSize; j++) {

      int writeB = fputc(buffer[j], write);

      if (writeB != buffer[j]) {
        printf("Failed to write\n");
        exit(EXIT_FAILURE);
      } else {
        sizeoffile++;
      }
    }
    fflush(write);
  }

  // Time in seconds
  double seconds = ((double)clock() - clkstart) / CLOCKS_PER_SEC;

  // Write speed in B/s
  double writeSpeed = (sizeoffile * sizeof(uint8_t)) / seconds;

  memRes.sec = seconds;
  memRes.BperS = writeSpeed;
  return memRes;
}

/* Reads 1 Bytes at a time from RAM, for given amount of reads, calculates time
 * and transfer Parameters:
 *     - MemMap* map -> holds the allocated memory and its size in X and Y
 *     - int* indexX -> holds random x indexes generated beforehand
 *     - int* indexY -> holds random y indexes generated beforehand
 *     - long maxRuns -> the number of reads that it should perform
 */
MemRes RAM_READ_TEST(MemMap *map, int *indexX, int *indexY, long maxRuns) {
  MemRes memRes;
  clock_t clkstart;
  long sizeofread = 0;

  clkstart = clock();

  // Random reads
  long counter = 0L;
  while (counter < maxRuns) {
    uint8_t value = map->memoryMap[indexX[counter]][indexY[counter]];
    sizeofread += 8 * sizeof(uint8_t);
    counter++;
  }

  // Time in seconds
  double seconds = (double)(clock() - clkstart) / CLOCKS_PER_SEC;

  // Write speed in B/s
  double readSpeed = (sizeofread * sizeof(uint8_t)) / seconds;

  memRes.sec = seconds;
  memRes.BperS = readSpeed;
  return memRes;
}
/* Writes 1 Byte at a time from RAM, for given amount of reads, calculates time
 * and transfer Parameters:
 *     - MemMap* map -> holds the allocated memory and its size in X and Y
 *     - int* indexX -> holds random x indexes generated beforehand
 *     - int* indexY -> holds random y indexes generated beforehand
 *     - long maxRuns -> the number of writes that it should perform
 */
MemRes RAM_WRITE_TEST(MemMap *map, int *indexX, int *indexY, long maxRuns) {
  MemRes memRes;
  clock_t clkstart;
  long sizeofwrite = 0;

  clkstart = clock();

  // Generates random indexes
  long counter = 0L;
  while (counter < maxRuns) {
    map->memoryMap[indexX[counter]][indexY[counter]] = 77;

    sizeofwrite += 8 * sizeof(uint8_t);
    counter++;
  }

  // Time in seconds
  double seconds = ((double)clock() - clkstart) / CLOCKS_PER_SEC;

  // Write speed in B/s
  double writeSpeed = (sizeofwrite * sizeof(uint8_t)) / seconds;

  memRes.sec = seconds;
  memRes.BperS = writeSpeed;
  return memRes;
}
