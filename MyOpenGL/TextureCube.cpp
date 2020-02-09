#include "TextureCube.h"



Shader TextureCube::equirectangularToCubemapShader;

void TextureCube::InitializeEquirectangularToCubemapShader()
{
	static bool bInitializedEquirectangularToCubemapShader = false;
	if (!bInitializedEquirectangularToCubemapShader)
	{
		equirectangularToCubemapShader.Import(SHADER_PATH + "EquirectangularToCubemap");

		bInitializedEquirectangularToCubemapShader = true;
	}
}

TextureCube::TextureCube()
	: Texture()
{
	SetType(Type::TextureCube);
	SetWrapMode(WrapMode::ClampToEdge);
	SetFormat(Format::RGB);
	internalFormat = Format::HDR;
}

bool TextureCube::Import(const std::string & Filename)
{
	// Load the HDR environment map
	equirectangularMap.bGenerateMipMaps = false;
	equirectangularMap.SetFormat(Format::RGB);
	equirectangularMap.internalFormat = Format::HDR;
	equirectangularMap.SetType(Type::Texture2D);
	equirectangularMap.SetWrapMode(WrapMode::ClampToEdge);
	bool bSuccess = equirectangularMap.Import(Filename); // Import HDR texture
	// Early out
	if (!bSuccess)
		return false;
	else
	{
		source = Filename;
		if (displayName.length() <= 0 || displayName == DEFAULT_DISPLAY_NAME)
		{
			std::string name = Filename;
			name = name.substr(name.rfind("/") + 1, name.length() - 1);
			name = name.substr(0, name.find("."));
			SetDisplayName(name);
		}
	}


	// Generate cubemap
	uint envCubemap;
	glGenTextures(1, &envCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	// Generate each face texture
	for (uint i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	// Projection matrices for each side
	glm::mat4 captureViews[] =
	{
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};


	InitializeEquirectangularToCubemapShader(); // Initialize our conversion shader if not already initialized (we only do this once)
	equirectangularToCubemapShader.Bind();
	equirectangularToCubemapShader.SetProjectionMatrix(captureProjection);
	equirectangularToCubemapShader.SetInt("EquirectangularMap", 0);
	glActiveTexture(GL_TEXTURE0);
	equirectangularMap.Bind();


	// Generate framebuffer & renderbuffer
	uint captureFBO, captureRBO;
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);
	// Bind buffers
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);
	// Configure viewport & bind framebuffer
	glViewport(0, 0, 512, 512);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

	// Capture each face
	for (uint i = 0; i < 6; ++i)
	{
		equirectangularToCubemapShader.SetViewMatrix(captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Primitives::cube.Draw(glm::mat4(1.0f), {}, false, false);
	}

	// Unbind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Assign the temp env cubemap to our ID
	ID = envCubemap;

	return bSuccess;
}

Texture & const TextureCube::GetEquirectangularMap()
{
	return equirectangularMap;
}

GLint TextureCube::GetPreviewID()
{
	return equirectangularMap.GetID();
}
