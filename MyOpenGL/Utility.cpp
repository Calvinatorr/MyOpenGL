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

void Log::PrintToScreen(const std::string & Message, const float& Duration, const ImVec4& Colour)
{
	// We iterate through each line in the message and insert it into the screen log in reverse order
	// so that the message as a whole floats to the top, but keeps the message top to bottom

	if (Duration > 0)
	{
		std::stringstream ss(Message);
		std::string line;
		std::vector<std::string> reverse;
		while (std::getline(ss, line))
		{
			reverse.push_back(line);
		}
		for (auto t = reverse.rbegin(); t != reverse.rend(); ++t)
		{
			Log::ScreenLogMessage newMessage(*t, Duration, Colour);
			screenLog.push_back(newMessage);
		}
	}
}

void Log::PrintToConsole(const std::string & Message)
{
	std::cout << Log::FormatMessage(Message) << std::endl; // Use standard C++ iostream to print to console
}

void Log::PrintToLog(const std::string & Message)
{
	Log::log += Log::FormatMessage(Message) + "\n";
}

void Log::Print(const std::string & Message, const float& PrintToScreenDuration, const bool& bPrintToConsole, const bool& bPrintToLog)
{
	Log::PrintToScreen(Message, PrintToScreenDuration);
	Log::PrintInfo(Message, bPrintToConsole, bPrintToLog);
}

void Log::PrintInfo(const std::string & Message, const bool & bPrintToConsole, const bool & bPrintToLog)
{
	if (bPrintToConsole)
		Log::PrintToConsole(Message);
	if (bPrintToLog)
		Log::PrintToLog(Message);
}

void Log::PrintError(std::string Message, const float& PrintToScreenDuration, const bool& bPrintToConsole, const bool& bPrintToLog)
{
	Message = "Error!: " + Message;
	Log::Print(Message, -1, bPrintToConsole, bPrintToLog); // Don't print to screen so we can override colour by calling method directly
	Log::PrintToScreen(Message, PrintToScreenDuration, ImVec4(191.0f / 255.0f, 48.0f / 255.0f, 38.0f / 255.0f, 1.0f));
}

void Log::InitializeLog()
{
	if (Dump())
	{
		Log::PrintInfo("Initialized log file '" + Log::logFilename + "'");
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

std::string Log::GetLog()
{
	return log;
}

void Log::DrawScreenLog()
{
	glm::vec2 size = Window::GetCurrentObject()->GetSize();

	ImGui::SetNextWindowPos(ImVec2(10.0f, 20.0f));
	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	bool show = true;
	ImGui::Begin("Screen Log", &show, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMouseInputs);

	// Reverse iterate along screen log and print each message. Remove if passed duration.
	{
		auto t = screenLog.rbegin();
		while (t != screenLog.rend())
		{
			auto message = *t;
			message.Draw();

			// Remove if we passed the duration for this message
			if (message.PassedDuration())
				t = decltype(t)(screenLog.erase(std::next(t).base()));
			else
				++t;
		}
	}

	ImGui::End();
	ImGui::PopStyleVar();
}
