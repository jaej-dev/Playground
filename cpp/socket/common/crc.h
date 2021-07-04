#ifndef _CRC_H_
#define _CRC_H_

#include <stddef.h>
#include <stdint.h>
#include <chrono>

uint32_t crc32(const void *buf, size_t size);
uint32_t crc32(uint32_t crc, const void *buf, size_t size);
void printBuffer(void * buffer, size_t size, bool printHead = true);
std::chrono::milliseconds toChronoMilli(int ms);
std::chrono::milliseconds toChronoMilli(float ms);

#endif // _CRC_H_
