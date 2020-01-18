#pragma once


#include "GUI.h"

#include "Game.h"
#include "Window.h"
#include "Shader.h"
#include "EditorCamera.h"

#include "Level.h"

#include <string>


class SceneOutlinerGUI : public Widget
{
private:


public:

	void Layout() override
	{
		ImGui::Begin("Scene Outliner", &bIsActive, ImGuiWindowFlags_NoCollapse);

		// For each level
		for (auto level : SceneOutliner::GetLoadedLevels())
		{
			if (ImGui::TreeNodeEx((level->GetDisplayName() + " (level)").c_str(),
				ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanFullWidth))
			{

				// For each object in the level
				for (auto object : level->GetSceneObjects())
				{
					ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Leaf;
					if (SceneOutliner::IsSelected(object))
						flags |= ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_Selected;

					ImGui::Columns(3);
					if (ImGui::TreeNodeEx((object->GetDisplayName()).c_str(), flags))
					{

						ImGui::TreePop();
					}

					if (ImGui::IsItemClicked())
						SceneOutliner::ToggleSelection(object, !(glfwGetKey(Window::GetCurrent(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS));

					ImGui::NextColumn();
					ImGui::Text(object->GetDisplayName().c_str());
					ImGui::NextColumn();
					ImGui::Text(object->GetClassNameA().c_str());
					ImGui::NextColumn();
				}

				ImGui::TreePop();
			}
		}

		ImGui::End();
	}
};


class ContentBrowserGUI : public Widget
{
private:
	void DrawTreeNodesInDirectory(const std::string& path)
	{
		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			auto path = entry.path();
			bool bIsDirectory = std::filesystem::is_directory(path);

			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
			if (!bIsDirectory)
				flags |= ImGuiTreeNodeFlags_Leaf;
			else
				flags |= ImGuiTreeNodeFlags_Framed;

			if (ImGui::TreeNodeEx(path.filename().u8string().c_str(), flags))
			{
				if (bIsDirectory)
				{
					DrawTreeNodesInDirectory(path.u8string());
				}

				ImGui::TreePop();
			}
		}
	}


public:

	void Layout() override
	{
		ImGui::Begin("Content Browser", &bIsActive, ImGuiWindowFlags_NoCollapse);

		DrawTreeNodesInDirectory(CONTENT_PATH);

		ImGui::End();
	}
};



class EditorGUI : public Widget
{
private:

	// Properties
	bool bShowDemoWindow = false;

	ContentBrowserGUI contentBrowser = ContentBrowserGUI();
	SceneOutlinerGUI sceneOutliner = SceneOutlinerGUI();
	bool bShowStats = false;
	bool bShowConsoleLog = true;
	
	float wireframeWidth = 1.0f;



public:

	// Properties
	float clearColour[4] = { 0.2f, 0.3f, 0.3f, 1.0f };


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
				if (ImGui::MenuItem("Content Browser", ""))
					contentBrowser.ToggleActive();

				if (ImGui::MenuItem("Scene Outliner", ""))
					sceneOutliner.ToggleActive();

				if (ImGui::MenuItem("Console Log", ""))
					bShowConsoleLog = !bShowConsoleLog;

				ImGui::Separator();

				if (ImGui::MenuItem("ImGui Demo Window", ""))
					bShowDemoWindow = !bShowDemoWindow;

				ImGui::EndMenu();
			}


			if (ImGui::BeginMenu("Editor"))
			{
				// Toggle stats (FPS)
				if (ImGui::MenuItem("Toggle Stats"))
					bShowStats = !bShowStats;

				if (ImGui::MenuItem("Recompile Shaders", ""))
				{
					Log::PrintToScreen("Recompiling all shaders..", 10.0f, ImVec4(183.0f / 255.0f, 46.0f / 255.0f, 201.0f / 255.0f, 1.0f));
					Shader::RecompileAll();
				}

				if (ImGui::MenuItem("Clear Console Log", ""))
					Log::Dump(true);

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
				{
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
				}

				
				// Clear colour picker
				ImGui::ColorEdit4("Clear Colour##4", clearColour);



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
			ImGui::Begin("Stats", &bShowStats, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMouseInputs);
			
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
			ImGui::Begin("Console Log", &bShowConsoleLog, ImGuiWindowFlags_NoCollapse);

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



		// Demo window
		if (bShowDemoWindow)
		{
			ImGui::ShowDemoWindow(&bShowDemoWindow);
		}
	};
};

