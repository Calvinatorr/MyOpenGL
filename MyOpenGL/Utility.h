#pragma once


#include <iostream>
#include <fstream>
#include <string>

#include <algorithm>

#include <chrono>  // chrono::system_clock
#include <time.h>
#include <ctime>   // localtime
#include <sstream> // stringstream
#include <iomanip> // put_time


// ===================================== TYPE-DEFS ============================================

typedef unsigned char		byte;
typedef unsigned char		uint8;
typedef uint8				uint;
typedef unsigned short int	uint16;
typedef unsigned int		uint32;
typedef unsigned long int	uint64;
typedef unsigned long		ulong;
using string = std::string;



// ===================================== UTILITY MISC ============================================


#ifndef ENGINE_PATH
#define ENGINE_PATH std::string("../Engine/")
#endif

#ifndef SAVED_PATH
#define SAVED_PATH std::string("../Saved/")
#endif


namespace Utility
{
	namespace // Anonmyous namespace
	{
		const char* DATE_TIME_FORMAT = "%d-%m-%Y-%X";
		std::string startDateTime;
	}

	/* Returns date/time as string. Not thread safe! Please update to C++20 std::chrono::date */
	const std::string GetCurrentDateTime(const char* Format = DATE_TIME_FORMAT);

	/* Called at start of program */
	void InitializeStartDateTime();

	/* Return date & time as string of when program starts */
	const std::string GetStartDateTime();
}



// ===================================== MACROS ============================================





// ===================================== LOGGING ============================================


#ifndef LOG_PATH
#define LOG_PATH std::string(SAVED_PATH + "Logs/")
#endif

namespace Log
{
	namespace
	{
		std::string log;
		std::string logFilename;
	}

	   
	/* Formats message for printing with date & time */
	std::string FormatMessage(const std::string& Message);

	void PrintToScreen(const std::string& Message);
	void PrintToConsole(const std::string& Message);
	void PrintToLog(const std::string& Message);
	void Print(const std::string& Message, bool bPrintToScreen = true, bool bPrintToConsole = true, bool bPrintToLog = true);
	void PrintError(std::string Message, bool bPrintToScreen = true, bool bPrintToConsole = true, bool bPrintToLog = true);

	/* Called at start of program */
	void InitializeLog();

	/* Called at end of program */
	bool Dump(bool bClearLog = true);

	/* Returns log dump from string */
	std::string GetLog();
}