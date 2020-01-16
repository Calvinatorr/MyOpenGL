#include "Utility.h"


// ===================================== UTILITY MISC ============================================


const std::string Utility::GetCurrentDateTime(const char* Format)
{
	/*auto now = std::chrono::system_clock::now();
	std::time_t time = std::chrono::system_clock::to_time_t(now);
	auto ctime = std::ctime(&time); // Not thread safe
	std::stringstream ss;
	ss << ctime;
	return ss.str();*/

	auto n = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(n);
	std::tm buf;
	localtime_s(&buf, &in_time_t);
	std::stringstream ss;
	ss << std::put_time(&buf, Format);
	return ss.str();
}

void Utility::InitializeStartDateTime()
{
	std::string dateTime = Utility::GetCurrentDateTime("%d-%m-%Y_%X");
	std::replace(dateTime.begin(), dateTime.end(), ':', '-'); // Replace all instances of ':' with '-'
	Utility::startDateTime = dateTime;
}


const std::string Utility::GetStartDateTime()
{
	return Utility::startDateTime;
}



// ===================================== LOGGING ============================================


std::string Log::FormatMessage(const std::string & Message)
{
	return "(" + Utility::GetCurrentDateTime() + "): " + Message;
}

void Log::PrintToScreen(const std::string & Message)
{
	// IMPLEMENT SCREEN PRINTING HERE
}

void Log::PrintToConsole(const std::string & Message)
{
	std::cout << Log::FormatMessage(Message) << std::endl; // Use standard C++ iostream to print to console
}

void Log::PrintToLog(const std::string & Message)
{
	Log::log += Log::FormatMessage(Message) + "\n";
}

void Log::Print(const std::string & Message, bool bPrintToScreen, bool bPrintToConsole, bool bPrintToLog)
{
	if (bPrintToScreen)
		Log::PrintToScreen(Message);
	if (bPrintToConsole)
		Log::PrintToConsole(Message);
	if (bPrintToLog)
		Log::PrintToLog(Message);
}

void Log::PrintError(std::string Message, bool bPrintToScreen, bool bPrintToConsole, bool bPrintToLog)
{
	Message = "Error!: " + Message;
	Print(Message, bPrintToScreen, bPrintToConsole, bPrintToLog);
}

void Log::InitializeLog()
{
	if (Dump())
	{
		Log::Print("Initialized log file '" + Log::logFilename + "'");
	}
}

bool Log::Dump(bool bClearLog)
{
	Log::logFilename = LOG_PATH + Utility::GetStartDateTime() + ".log"; // Generate path for log file
	std::ofstream file(Log::logFilename);											// Try to generate log file

	// If we successfully generated the log file
	if (file.is_open())
	{
		file << Log::log;			// Dump the log so far to initialize the log file

		if (bClearLog)
			Log::log.clear();

		file.close();				// Now close it, we will only dump to the log when we need it and at an interval
		return true;				// Return success
	}

	return false;
}
