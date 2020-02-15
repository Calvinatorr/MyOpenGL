#include "Material.h"

Material* Material::current = nullptr;

Material::Material()
{
}

Material::Material(Shader * Shader)
{
	this->shader = Shader;
}


Material::~Material()
{
}

void Material::SetFloatParameter(const GLchar* Name, const GLfloat & Value)
{
	SetParameter(floatParameters, Name, Value);
}

void Material::SetIntParameter(const GLchar* Name, const GLint & Value)
{
	SetParameter(intParameters, Name, Value);
}

void Material::SetVectorParameter(const GLchar* Name, const glm::vec3 & Value)
{
	SetParameter(vectorParameters, Name, Value);
}

void Material::SetVector4Parameter(const GLchar* Name, const glm::vec4 & Value)
{
	SetParameter(vector4Parameters, Name, Value);
}

void Material::SetTextureParameter(const GLchar* Name, Texture * Value)
{
	SetParameter(textureParameters, Name, Value);
}

GLfloat Material::GetFloatParameter(const GLchar * Name) const
{
	//return GetParameter<GLfloat>(&floatParameters, Name);
	return GLfloat();
}

GLint Material::GetIntParameter(const GLchar * Name) const
{
	return GLint();
}

glm::vec3 Material::GetVectorParameter(const GLchar * Name) const
{
	return glm::vec3();
}

glm::vec4 Material::GetVector4Parameter(const GLchar * Name) const
{
	return glm::vec4();
}

void Material::Bind()
{
	//if (shader != nullptr && current != this)
	if (shader != nullptr)
	{
		// Shader context switch
		if (Shader::GetCurrent() == nullptr || Shader::GetCurrent()->GetID() != shader->GetID()) // If this shader isn't already bound, then bind it, we expect no bound shaders at start of render thread
		{
			shader->Bind();
		}


		// Bind our parameters
		if (shader->IsValid()) // If the shader we bound was valid, then bind the attributes
		{
			if (current != nullptr)
				current->Unbind();

			for (unsigned int i = 0; i < floatParameters.size(); i++)
				shader->SetFloat(floatParameters[i].name.c_str(), floatParameters[i].value);


			for (unsigned int i = 0; i < intParameters.size(); i++)
				shader->SetInt(intParameters[i].name.c_str(), intParameters[i].value);


			for (unsigned int i = 0; i < vectorParameters.size(); i++)
				shader->SetVec3(vectorParameters[i].name.c_str(), vectorParameters[i].value);


			for (unsigned int i = 0; i < vector4Parameters.size(); i++)
				shader->SetVec4(vector4Parameters[i].name.c_str(), vector4Parameters[i].value);


			for (unsigned int i = 0; i < textureParameters.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + (GLint)i); // Set which texture we are modfying
				shader->SetInt(textureParameters[i].name.c_str(), i);
				textureParameters[i].value->Bind();
			}

			current = this;
		}
	}
}

void Material::Unbind()
{
	for (unsigned int i = 0; i < textureParameters.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + (GLint)i); // Set which texture we are modfying
		glDisable(textureParameters[i].value->GetType());
		//textureParameters[i].value->Unbind();
	}
}

Material * Material::GetCurrent()
{
	return current;
}

bool Material::Reimport()
{
	if (shader != nullptr)
	{
		return shader->Recompile();
	}
	else
	{
		return false;
	}
}


void Material::DrawDetails()
{
#if WITH_EDITOR
	std::string shaderName = shader != nullptr ? shader->GetDisplayName() : "NULL_SHADER";
	if (ImGui::TreeNodeEx((GetDisplayName() + " (" + shaderName + ")##").c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf | ImGuiSelectableFlags_AllowDoubleClick))
	{
		ImGui::TreePop();
	}

	if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered(ImGuiHoveredFlags_None))
	{
		OpenWindow();
	}
#endif
}



std::string Material::GetAssetWindowTitle()
{
	std::string shaderName = shader != nullptr ? shader->GetDisplayName() : "NULL_SHADER";
	return (GetDisplayName() + " (Material: " + shaderName + ")").c_str();
}


void Material::DrawWindowContents()
{
#if WITH_EDITOR
	const float ITEM_WIDTH = 200.0f;


	// Float parameters
	if (floatParameters.size() > 0 && Editor::DrawPanel("Float Parameters (" + std::to_string(floatParameters.size()) + ")"))
	{
		for (auto& parm : floatParameters)
		{
			ImGui::PushItemWidth(ITEM_WIDTH);
			ImGui::DragFloat(parm.name.c_str(), &parm.value, .005f, parm.min_Editor, parm.max_Editor, "%.3f", .05f);
			ImGui::PopItemWidth();
		}
		ImGui::TreePop();
		ImGui::NewLine();
		ImGui::Separator();
		ImGui::NewLine();
	}


	// Int parameters
	if (intParameters.size() > 0 && Editor::DrawPanel("Integer Parameters (" + std::to_string(intParameters.size()) + ")"))
	{
		for (auto& parm : intParameters)
		{
			ImGui::PushItemWidth(ITEM_WIDTH);
			ImGui::DragInt(parm.name.c_str(), &parm.value, .005f, parm.min_Editor, parm.max_Editor, "%.3f");
			ImGui::PopItemWidth();
		}
		ImGui::TreePop();
		ImGui::NewLine();
		ImGui::Separator();
		ImGui::NewLine();
	}


	// Vector parameters
	if (vectorParameters.size() > 0 && Editor::DrawPanel("Vector Parameters (" + std::to_string(vectorParameters.size()) + ")"))
	{
		for (auto& parm : vectorParameters)
		{
			ImGui::PushItemWidth(ITEM_WIDTH);
			ImGui::ColorEdit3(parm.name.c_str(), glm::value_ptr(parm.value));
			ImGui::PopItemWidth();
		}
		ImGui::TreePop();
		ImGui::NewLine();
		ImGui::Separator();
		ImGui::NewLine();
	}


	// Vector4 parameters
	if (vector4Parameters.size() > 0 && Editor::DrawPanel("Vector4 Parameters (" + std::to_string(vector4Parameters.size()) + ")"))
	{
		for (auto& parm : vector4Parameters)
		{
			ImGui::PushItemWidth(ITEM_WIDTH);
			ImGui::ColorEdit4(parm.name.c_str(), glm::value_ptr(parm.value));
			ImGui::PopItemWidth();
		}
		ImGui::TreePop();
		ImGui::NewLine();
		ImGui::Separator();
		ImGui::NewLine();
	}

	// Texture parameters
	if (textureParameters.size() > 0 && Editor::DrawPanel("Texture Parameters (" + std::to_string(textureParameters.size()) + ")"))
	{
		for (auto& parm : textureParameters)
		{
			ImGui::PushItemWidth(ITEM_WIDTH);
			std::string texName = parm.value == nullptr ? "NULL" : parm.value->GetClassNameA();
			if (ImGui::ImageButton(ImTextureID(parm.value->GetPreviewID()), ImVec2(96, 96), ImVec2(0, 1), ImVec2(1, 0)))
			{
				parm.value->OpenWindow();
			}
			ImGui::SameLine();
			ImGui::Text(parm.name.c_str());
			ImGui::PopItemWidth();

			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
			{
				parm.value->OpenWindow();
			}

			AssetManager::DrawAssetBrowserContextMenu(&parm.value);
		}
		ImGui::TreePop();
		ImGui::NewLine();
		ImGui::Separator();
		ImGui::NewLine();
	}
#endif
}
