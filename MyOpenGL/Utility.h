#pragma once


#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include <algorithm>

#include <chrono>  // chrono::system_clock
#include <time.h>
#include <ctime>   // localtime
#include <sstream> // stringstream
#include <iomanip> // put_time

// IMGUI - https://github.com/ocornut/imgui/
#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"


#include "Window.h"


// ===================================== TYPE-DEFS ============================================

typedef unsigned char		byte;
typedef unsigned char		uint8;
typedef uint8				uint;
typedef unsigned short int	uint16;
typedef unsigned int		uint32;
typedef unsigned long int	uint64;
typedef unsigned long		ulong;
using string = std::string;

#ifndef True
#define True true
#endif
#ifndef False
#define False false
#endif



// ===================================== UTILITY MISC ============================================


#ifndef ENGINE_PATH
#define ENGINE_PATH std::string("../Engine/")
#endif

#ifndef SAVED_PATH
#define SAVED_PATH std::string("../Saved/")
#endif

#ifndef CONTENT_PATH
#define CONTENT_PATH std::string("../Content/")
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
		std::string	log;
		std::string logFilename;


		class ScreenLogMessage
		{
		private:
			double timeAtCapture = 0.0f;
			std::string messageString;
			float duration;
			ImVec4 colour;

		public:
			ScreenLogMessage()
			{

			}

			/* Creates message object and initialzies values */
			ScreenLogMessage(const std::string& messageString, const float& duration = 5.0f, const ImVec4& colour = ImVec4(1.0f, 1.0f, 1.0f, 1.0f))
				: messageString(messageString), duration(duration), colour(colour)
			{
				timeAtCapture = glfwGetTime();
			}

			void Draw()
			{
				ImGui::PushStyleColor(ImGuiCol_Text, colour);
				ImGui::LabelText("", messageString.c_str());
				ImGui::PopStyleColor();
			}

			bool PassedDuration()
			{
				return glfwGetTime() > timeAtCapture + duration;
			}
		};


		// Screen Log
		std::vector<ScreenLogMessage> screenLog;
	}

	   
	/* Formats message for printing with date & time */
	std::string FormatMessage(const std::string& Message);

	void PrintToScreen	(const std::string& Message,	const float& Duration = 5.0f, const ImVec4& Colour = ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	void PrintToConsole	(const std::string& Message);
	void PrintToLog		(const std::string& Message);
	void Print			(const std::string& Message,	const float& PrintToScreenDuration = 5.0f, const bool& bPrintToConsole = true, const bool& bPrintToLog = true);
	void PrintInfo(const std::string& Message,			const bool&  bPrintToConsole = true,		   const bool& bPrintToLog = true);
	void PrintError		(std::string Message,			const float& PrintToScreenDuration = -1.0f, const bool& bPrintToConsole = true, const bool& bPrintToLog = true);

	/* Called at start of program */
	void InitializeLog();

	/* Called at end of program */
	bool Dump(bool bClearLog = true);

	/* Returns log dump from string */
	std::string GetLog();

	/* Draws the log to the screen. Requires ImGui */
	void DrawScreenLog();
}