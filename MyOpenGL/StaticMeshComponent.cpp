#include "StaticMeshComponent.h"



StaticMeshComponent::StaticMeshComponent()
{
}

StaticMeshComponent::StaticMeshComponent(StaticMesh * StaticMesh)
{
	this->staticMesh = StaticMesh;
}


StaticMeshComponent::~StaticMeshComponent()
{
}

void StaticMeshComponent::Draw()
{
	// Render our static mesh
	if (staticMesh != nullptr)
		staticMesh->Draw(GetWorldTransformMatrix());
}

void StaticMeshComponent::Cleanup()
{
	if (staticMesh != nullptr)
		staticMesh->Cleanup();
}

void StaticMeshComponent::DrawGUI()
{
	SceneComponent::DrawGUI();

	if (ImGui::TreeNodeEx("Static Mesh", panelFlags))
	{
		ImGui::LabelText("Static Mesh Asset", staticMesh->GetDisplayName().c_str());

		ImGui::NewLine();

		ImGui::LabelText("Source", staticMesh->source.c_str());
		if (ImGui::Button("Reimport from Disk"))
		{
			staticMesh->Reimport();
		}
		ImGui::SameLine();
		if (ImGui::Button("Import from Source"))
		{
/*#if OS_Windows
			char filename[MAX_PATH] = "../";

			OPENFILENAME ofn;
			ZeroMemory(&filename, sizeof(filename));
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
			ofn.lpstrFilter = "Text Files\0*.txt\0Any File\0*.*\0";
			ofn.lpstrFile = filename;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrTitle = "Select a File, yo!";
			ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

			if (GetOpenFileNameA(&ofn))
			{
				std::cout << "You chose the file \"" << filename << "\"\n";
			}
#endif*/
		}

		ImGui::NewLine();

		if (staticMesh != nullptr)
		{
			auto minBounds = staticMesh->GetMinBounds();
			auto maxBounds = staticMesh->GetMaxBounds();
			ImGui::Text(("Min Bounds (" + std::to_string(minBounds.x) + ", " + std::to_string(minBounds.y) + ", " + std::to_string(minBounds.z) + ")").c_str());
			ImGui::Text(("Max Bounds (" + std::to_string(maxBounds.x) + ", " + std::to_string(maxBounds.y) + ", " + std::to_string(maxBounds.z) + ")").c_str());
		}

		ImGui::NewLine();

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
		if (ImGui::TreeNodeEx(("Materials (" + std::to_string(staticMesh->GetMaterials().size()) + ")").c_str(), flags))
		{
			// We will change this when multiple materials are functional
			ImGui::Text("0");
			ImGui::SameLine();

			for (auto& m : staticMesh->GetMaterials())
			{
				std::string materialName = "NULL_MATERIAL";
				std::string shaderName = "NULL_SHADER";
				if (m != nullptr)
				{
					materialName = m->GetDisplayName();
					if (m->shader != nullptr)
						shaderName = m->shader->GetDisplayName();
				}

				if (ImGui::TreeNodeEx((materialName + " (" + shaderName + ")").c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf))
				{

					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx(("Mesh Sections (" + std::to_string(staticMesh->GetMeshSections().size()) + ")").c_str(), flags ^ ImGuiTreeNodeFlags_DefaultOpen))
		{
			uint i = 0;
			for (auto& m : staticMesh->GetMeshSections())
			{
				if (ImGui::TreeNodeEx(("Section [" + std::to_string(i) + "]").c_str(), flags ^ ImGuiTreeNodeFlags_DefaultOpen))
				{

					// IDs
					ImGui::Text(("VAO " + std::to_string(m.GetVAO())).c_str());
					ImGui::Text(("EBO " + std::to_string(m.GetEBO())).c_str());
					ImGui::Text(("VBO " + std::to_string(m.GetVBO())).c_str());

					// Vertices
					if (ImGui::TreeNodeEx(("Vertices (" + std::to_string(m.vertices.size()) + ")").c_str(), flags ^ ImGuiTreeNodeFlags_DefaultOpen))
					{
						uint j = 0;
						for (auto& v : m.vertices)
						{
							if (ImGui::TreeNodeEx(("[" + std::to_string(j) + "] (" + std::to_string(v.position.x) + ", " + std::to_string(v.position.y) + ", " + std::to_string(v.position.z) + ")").c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf))
							{

								ImGui::TreePop();
							}
							++j;
						}
						ImGui::TreePop();
					}

					// Indices
					if (ImGui::TreeNodeEx(("Indices (" + std::to_string(m.indices.size()) + ")").c_str(), flags ^ ImGuiTreeNodeFlags_DefaultOpen))
					{
						uint j = 0;
						for (auto& ind : m.indices)
						{
							if (ImGui::TreeNodeEx(("[" + std::to_string(j) + "] " + std::to_string(ind)).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf))
							{

								ImGui::TreePop();
							}
							++j;
						}
						ImGui::TreePop();
					}

					// Raw Vertices
					auto rawVertices = m.GetRawVertices();
					if (ImGui::TreeNodeEx(("Raw Vertices (" + std::to_string(rawVertices.size()) + ")").c_str(), flags ^ ImGuiTreeNodeFlags_DefaultOpen))
					{
						uint j = 0;
						for (auto& v : rawVertices)
						{
							if (ImGui::TreeNodeEx(("[" + std::to_string(j) + "] " + std::to_string(v)).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf))
							{

								ImGui::TreePop();
							}
							++j;
						}
						ImGui::TreePop();
					}

					ImGui::TreePop();
				}
				++i;
			}
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
}
