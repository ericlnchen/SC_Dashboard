#ifndef LOGGING_H
#define LOGGING_H
#include <TimeLib.h>
#include <SdFat.h>

time_t getTeensy3Time();
//String millisecond();
//void updateMillisecond();
String constructDateTime(uint8_t i);
void initializeSysClock();
void millisecondUpdate();
void initializeSD();
void logData();
//void FilePrintTime(FsFile dataFile, char fullTime);

#endif