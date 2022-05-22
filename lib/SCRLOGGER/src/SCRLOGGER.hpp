#ifndef LOGGING_H
#define LOGGING_H
#include <TimeLib.h>
#include <SdFat.h>

time_t getTeensy3Time();
void initializeSysClock();
String constructDateTime(uint8_t i);
void initializeSD();
bool initializeLog();
void logData();
//void FilePrintTime(FsFile dataFile, char fullTime);

#endif