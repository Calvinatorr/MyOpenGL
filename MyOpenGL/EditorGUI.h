#pragma once


#include "GUI.h"

#include "Game.h"
#include "Window.h"
#include "Shader.h"
#include "EditorCamera.h"

#include <string>



class EditorGUI : public Widget
{
private:
	bool bShowStats = false;
	bool bShowConsoleLog = true;
	
	float wireframeWidth = 1.0f;


public:

	void Layout() override
	{
		// Window properties
		Window* window = Window::GetCurrentObject();
		if (window == nullptr)
			return;
		auto size = window->GetSize();


		// Menu bar
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::Separator();

				// Close editor
				if (ImGui::MenuItem("Exit"))
				{
					glfwSetWindowShouldClose(Window::GetCurrent(), true);
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{
				ImGui::EndMenu();
			}


			if (ImGui::BeginMenu("Windows"))
			{
				if (ImGui::MenuItem("Output Log", ""))
					bShowConsoleLog = !bShowConsoleLog;

				ImGui::EndMenu();
			}


			if (ImGui::BeginMenu("Editor"))
			{
				// Toggle stats (FPS)
				if (ImGui::MenuItem("Toggle Stats"))
					bShowStats = !bShowStats;

				if (ImGui::MenuItem("Recompile Shaders", ""))
				{
					Log::PrintToLog("Recompiling all shaders..");
					Shader::RecompileAll();
				}

				ImGui::Separator();


				// Camera properties
				EditorCamera* defaultCamera = EditorCamera::GetDefaultCamera();
				if (defaultCamera != nullptr)
				{
					ImGui::SliderFloat("Camera FOV", &defaultCamera->fieldOfView, 1.0f, 120.0f);
				}
				else
				{
					ImGui::LabelText("", "No editor camera found");
				}

				ImGui::Separator();


				// View mode selection
				const char* items[] = { "Lit", "Wireframe", "Points" };
				static const char* currentItem = items[0];

				if (ImGui::BeginCombo("View Mode##combo", currentItem))
				{
					for (int n = 0; n < IM_ARRAYSIZE(items); n++)
					{
						bool isSelected = (currentItem == items[n]);
						if (ImGui::Selectable(items[n], isSelected))
						{
							currentItem = items[n];

							// Change viewmode
							if (items[n] == items[0])
							{
								glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
							}
							else if (items[n] == items[1])
							{
								glLineWidth(wireframeWidth);
								glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
							}
							else
							{
								glLineWidth(4.0f);
								glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
							}
						}
						if (isSelected)
							ImGui::SetItemDefaultFocus();

					}
					ImGui::EndCombo();
				}

				// Wireframe width slider
				if (currentItem == items[1])
				{
					if (ImGui::SliderFloat("", &wireframeWidth, 1.0f, 5.0f))
						glLineWidth(wireframeWidth);
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
						ImGui::TextUnformatted("Wireframe width");
						ImGui::PopTextWrapPos();
						ImGui::EndTooltip();
					}
				}


				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}


		// Stats
		if (bShowStats)
		{
			ImGui::SetNextWindowPos(ImVec2(size.x - 175.0f, 20.0f));
			ImGui::SetNextWindowBgAlpha(0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(.956f, .46f, .203f, 1.0f));
			ImGui::Begin("Stats", &bShowStats, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			
			ImGui::LabelText(std::to_string(Game::GetFPS()).c_str(), "FPS");

			ImGui::End();
			ImGui::PopStyleColor();
			ImGui::PopStyleVar();
		}



		// Console log
		if (bShowConsoleLog)
		{
			//ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			//ImGui::Begin("Stats", &bShowStats, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			ImGui::Begin("Console Log", &bShowConsoleLog);

			//ImGui::LabelText(std::to_string(Game::GetFPS()).c_str(), "FPS");
			std::istringstream iss(Log::GetLog());
			std::string line;
			while( getline(iss, line))
			{
				ImGui::LabelText("", line.c_str());
			}

			ImGui::End();
			//ImGui::PopStyleVar();
		}
	};
};

