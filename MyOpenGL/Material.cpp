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

void Material::DrawGUI()
{
#if WITH_EDITOR

	// Recompile button
	if (ImGui::Button("Recompile"))
	{
		if (shader != nullptr)
			shader->Recompile();
	}


	// Float parameters
	for (auto& parm : floatParameters)
	{
		ImGui::PushItemWidth(100);
		ImGui::DragFloat(parm.name.c_str(), &parm.value, .005f, parm.min_Editor, parm.max_Editor, "%.3f", .05f);
		ImGui::PopItemWidth();
	}
	
	// Int parameters
	for (auto& parm : intParameters)
	{
		ImGui::PushItemWidth(100);
		ImGui::DragInt(parm.name.c_str(), &parm.value, .005f, parm.min_Editor, parm.max_Editor, "%.3f");
		ImGui::PopItemWidth();
	}

	// Vector parameters
	for (auto& parm : vectorParameters)
	{
		ImGui::PushItemWidth(200);
		ImGui::ColorEdit3(parm.name.c_str(), glm::value_ptr(parm.value));
		ImGui::PopItemWidth();
	}

	// Vector4 parameters
	for (auto& parm : vector4Parameters)
	{
		ImGui::PushItemWidth(200);
		ImGui::ColorEdit4(parm.name.c_str(), glm::value_ptr(parm.value));
		ImGui::PopItemWidth();
	}
#endif
}
