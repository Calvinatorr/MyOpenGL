#pragma once

#include "Shader.h"
#include "Asset.h"

#include "Editor.h"


#include <map>

class Object;


class Material : public Asset, public _BindableBase, public _EditorDetailsBase
{
public:
	template <class T> class Parameter
	{
	public:
		std::string name;
		T value;
#if WITH_EDITOR
		T min_Editor, max_Editor;
#endif

		//Parameter() { };
		Parameter(const std::string& Name)
			: name(Name) { };
		Parameter(const std::string& Name, const T& Value)
			: name(Name), value(Value) { };
		Parameter(const std::string& Name, const T& Value, const T& Min, const T& Max)
			: name(Name), value(Value)
		{
#if WITH_EDITOR
			min_Editor = Min;
			max_Editor = Max;
#endif
		};
		~Parameter() { };

		bool operator==(const Parameter<T>& rhs)
		{
			return name == rhs.name && value == rhs.value;
		};
	};

private:
	static Material* current;

protected:
	template<typename T>
	void SetParameter(std::vector<Parameter<T>>& ParameterList, const GLchar* Name, const T& Value);

	template<typename T>
	T GetParameter(std::vector<Parameter<T>>* ParameterList, const GLchar* Name);

public:

	// Properties
	Shader* shader; // The shader this is bound to
	std::vector<Parameter<GLfloat>>		floatParameters;
	std::vector<Parameter<GLint>>		intParameters;
	std::vector<Parameter<glm::vec3>>	vectorParameters;
	std::vector<Parameter<glm::vec4>>	vector4Parameters;
	std::vector<Parameter<Texture*>>	textureParameters;


	// Constructors & destructors
	Material();
	Material(Shader* Shader);
	~Material();


	// Setters
	void SetFloatParameter(const GLchar* Name, const GLfloat& Value);
	void SetIntParameter(const GLchar* Name, const GLint& Value);
	void SetVectorParameter(const GLchar* Name, const glm::vec3& Value);
	void SetVector4Parameter(const GLchar* Name, const glm::vec4& Value);
	void SetTextureParameter(const GLchar* Name, Texture* Value);

	
	// Getters
	GLfloat		GetFloatParameter	(const GLchar* Name) const;
	GLint		GetIntParameter		(const GLchar* Name) const;
	glm::vec3	GetVectorParameter	(const GLchar* Name) const;
	glm::vec4	GetVector4Parameter	(const GLchar* Name) const;

	
	// Methods
	virtual void Bind() override;
	virtual void Unbind() override;
	static Material* GetCurrent();

	
	// Interfaces
	bool Reimport() override;
	void DrawDetails() override;
	std::string GetAssetWindowTitle() override;
	void DrawWindow() override;
};



// Templates for compiler

template<typename T>
inline void Material::SetParameter(std::vector<Parameter<T>>& ParameterList, const GLchar * Name, const T & Value)
{
	Parameter<T> parameter = Parameter<T>(Name, Value);
	auto it = std::find(ParameterList.begin(), ParameterList.end(), parameter);

	if (it == ParameterList.end())			// If we can't find this parameter
	{
		ParameterList.push_back(parameter); // Then add it to our list
	}
	else									// Otherwise we set the value of the parameter which already exists
	{
		int index = std::distance(ParameterList.begin(), it);
		ParameterList[index].value = Value;
	}
}

template<typename T>
inline T Material::GetParameter(std::vector<Parameter<T>>* ParameterList, const GLchar * Name)
{
	T result;										// Initialize with default constructor
	for (int i = 0; i < ParameterList.size(); i++)	// For each parameter
	{
		if (ParameterList[i]->name == Name)			// If we find the name then break the loop and return the value
		{
			result = ParameterList[i]->value;
			break;
		}
	}

	return result;
}
