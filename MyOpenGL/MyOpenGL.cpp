// ===================================== GLAD & GLFW ============================================

#include <glad/glad.h>  // Initialize with gladLoadGL()
#include <GLFW/glfw3.h> // Window & input handler


// ===================================== ImGui & GUI/Widgets ============================================

// IMGUI - https://github.com/ocornut/imgui/
#include "imgui.cpp"
#include "imgui_demo.cpp"
#include "imgui_draw.cpp"
#include "imgui_widgets.cpp"
#include "examples/imgui_impl_glfw.cpp"
#include "examples/imgui_impl_opengl3.cpp"
// Includes glad and GLFW
#include "Editor.h"


// ===================================== OUR INCLUDES ============================================

// Python environment
#include "PythonEnvironment.h"

// Our classes
#include "Game.h"
#include "EditorGUI.h"
#include "Window.h"
#include "Shader.h"
#include "Texture2D.h"
#include "EditorCamera.h"
#include "StaticMeshObject.h"
#include "Level.h"


// Python wrapper
//#include <pybind11/pybind11.h>


const GLuint SRC_WIDTH = 1280;
const GLuint SRC_HEIGHT = 720;
Window window;
//EditorCamera camera;
Shader shaderProgram, unlitShader, cubemapShader;


// ===================================== EVENTS ============================================

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	/*EditorCamera* defaultCamera = EditorCamera::GetDefaultCamera();
	if (defaultCamera != nullptr)
		defaultCamera->UpdateFOV(Game::GetDeltaTime(), glm::vec2(xoffset, yoffset));*/
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	EditorCamera* defaultCamera = EditorCamera::GetDefaultCamera();
	if (defaultCamera != nullptr)
		defaultCamera->UpdateMouse(Game::GetDeltaTime(), glm::vec2(xpos, ypos));
}

void EditorInput(const float& DeltaTime)
{
	// Input
	/*if (glfwGetKey(Window::GetCurrent(), GLFW_KEY_ESCAPE) == GLFW_PRESS) // Close window on escape press
		glfwSetWindowShouldClose(Window::GetCurrent(), true);

	else if (glfwGetKey(Window::GetCurrent(), GLFW_KEY_1) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		std::cout << "MODE: LIT" << std::endl;
	}
	else if (glfwGetKey(Window::GetCurrent(), GLFW_KEY_2) == GLFW_PRESS)
	{
		glLineWidth(2.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		std::cout << "MODE: WIREFRAME" << std::endl;
	}
	else if (glfwGetKey(Window::GetCurrent(), GLFW_KEY_3) == GLFW_PRESS)
	{
		glPointSize(2.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		std::cout << "MODE: POINTS" << std::endl;
	}
	else if (glfwGetKey(Window::GetCurrent(), GLFW_KEY_R) == GLFW_PRESS) // HOT RECOMPILE
	{
		std::cout << "SHADERS HOT RECOMPILE" << std::endl;
		Shader::RecompileAll();
	}*/
}



// ===================================== MAIN ============================================
int main(int argc, char* argv[])
{
	// Initialize utilities
	Utility::InitializeStartDateTime();
	Log::InitializeLog();
	Python::Initialize();


	// GLFW initialize
	glfwSetErrorCallback(GLFW_ErrorCallback);
	if (!glfwInit())
		return 1;
	const std::string GLSL_VERSION = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// GLFW create window
	window.Create(SRC_WIDTH, SRC_HEIGHT, "Iridium Engine", true);
	if (window.window == nullptr)
		return 1;
	glfwSetScrollCallback(window.window, ScrollCallback);
	glfwSetCursorPosCallback(window.window, MouseCallback);
	//glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Capture cursor input but always reset cursor to screen centre



	// ===================================== LOAD IN OPENGL CONTEXT ============================================

	// Initialize GLAD: load all function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // Load address of OpenGL function pointers to handle OS-specifics
	{
		Log::PrintError("Failed to initialize GLAD");
		return -1; // Return error code
	}
	{
		const GLubyte* version = glGetString(GL_VERSION);
		std::stringstream s;
		s << "Using OpenGL version `" << version << "`";
		Log::PrintInfo(s.str());
	}

	glEnable(GL_DEPTH_TEST);


	// Setup Dear ImGUI context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();
	//ImGui::StyleColorsLight();
	ImGuiStyle* style = &ImGui::GetStyle();
	io.Fonts->AddFontFromFileTTF((ENGINE_PATH + std::string("Editor/Fonts/Ruda-Bold.ttf")).c_str(), 15.0f);
	ImGui::GetStyle().FrameRounding = 4.0f;
	ImGui::GetStyle().GrabRounding = 4.0f;

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.12f, 0.14f, 0.65f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.39f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.09f, 0.21f, 0.31f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.29f, 0.55f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);



	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window.window, true);
	ImGui_ImplOpenGL3_Init(GLSL_VERSION.c_str());

	
	// ===================================== EDITOR ============================================

	Shader::Initialize();

#if WITH_EDITOR
	EditorGrid editorGrid = EditorGrid();
	EditorGUI editorGUI;
#endif


	// ===================================== TEST LEVEL ============================================


	StaticMeshObject testObject;
	testObject.Construct();
	testObject.transform.rotation = glm::quat(glm::radians(glm::vec3(0.0f, 45.0f, 0.0f)));

	StaticMeshObject testObject2;
	testObject2.Construct();
	testObject2.transform.rotation = glm::quat(glm::radians(glm::vec3(0.0f, 90.0f, 0.0f)));

	Level level;
	//level.AddSceneObject(&testObject);
	//level.AddSceneObject(&testObject2);
	level.LoadFromDisk("../LoadFrom.json");
	//level.SaveToDisk("../SaveTo.json");
	//std::system("pause");


	// ===================================== CAMERA ============================================


	EditorCamera* defaultCamera = EditorCamera::GetDefaultCamera();
	if (defaultCamera != nullptr)
	{
		defaultCamera->fieldOfView = 65.0f;
		defaultCamera->cursorPosition = glm::vec2((GLfloat)window.GetSize().x / 2.0f, (GLfloat)window.GetSize().y / 2.0f);
		defaultCamera->Reset();
	}

	// ===================================== FRAME BUFFER ============================================

	/*
	GLuint FBO;
	glGenFramebuffers(1, &FBO); // Generate framebuffer object
	glBindFramebuffer(GL_FRAMEBUFFER, FBO); // Bind read/write framebuffer
	*/


	// ===================================== TEXTURES ============================================



	Texture tex = Texture("../Content/1024x1024 Texel Density Texture 1.png");
	Texture tex2;
	tex2.SetFormat(Texture::Format::RGBA);
	tex2.LoadResource("../Content/houdini-763d999dfe.png");

	Texture environmentMap;
	environmentMap.SetFormatHDR();
	environmentMap.SetWrapMode(Texture::WrapMode::ClampToEdge);
	environmentMap.LoadResource("../Content/small_hangar_01_1k.hdr");



	// ===================================== SHADERS & MATERIALS ============================================


	shaderProgram.Compile(SHADER_PATH + "Main");
	unlitShader.Compile(SHADER_PATH + "Unlit");
	cubemapShader.Compile(SHADER_PATH + "Cubemap");


	Material unlitMaterial(&unlitShader);
	unlitMaterial.SetDisplayName("Unlit_MI");
	AssetManager::Register(&unlitMaterial);

	Material cubemapMaterial(&cubemapShader);
	cubemapMaterial.SetDisplayName("Cubemap_MI");
	cubemapMaterial.SetTextureParameter("EquirectangularMap", &environmentMap);
	AssetManager::Register(&cubemapMaterial);

	Material checkerMaterial(&shaderProgram);
	checkerMaterial.SetDisplayName("Checker_MI");
	checkerMaterial.SetTextureParameter("tex", &tex);
	checkerMaterial.SetTextureParameter("tex2", &environmentMap);
	checkerMaterial.SetVectorParameter("inMaterial.Albedo", glm::vec3(1.0f));
	checkerMaterial.SetFloatParameter("inMaterial.Metalness", 0.0f);
	checkerMaterial.SetFloatParameter("inMaterial.Roughness", 0.25f);
	checkerMaterial.SetFloatParameter("inMaterial.AmbientOcclusion", 1.0f);
	AssetManager::Register(&checkerMaterial);

	Material sphereMaterial(&shaderProgram);
	sphereMaterial.SetDisplayName("Sphere_MI");
	sphereMaterial.SetTextureParameter("tex", &tex);
	sphereMaterial.SetTextureParameter("tex2", &tex2);
	sphereMaterial.SetVectorParameter("inMaterial.Albedo", glm::vec3(1.0f, 0.1f, 0.7f));
	sphereMaterial.SetFloatParameter("inMaterial.Metalness", 1.0f);
	sphereMaterial.SetFloatParameter("inMaterial.Roughness", 0.25f);
	sphereMaterial.SetFloatParameter("inMaterial.Anisotropic", 0.0f);
	sphereMaterial.SetFloatParameter("inMaterial.AnisotropicDirection", 0.0f);
	sphereMaterial.SetFloatParameter("inMaterial.AmbientOcclusion", 1.0f);
	AssetManager::Register(&sphereMaterial);


	// ===================================== VAO & VBO ============================================
	


	StaticMesh boxMesh;
	//boxMesh.LoadMeshObj("../Content/Box_SM.obj");
	//box.transform.rotation = glm::quat(glm::radians(glm::vec3(0.0f, 45.0f, 0.0f)));
	//box.transform.position = glm::vec3(-5.0f, 0.0f, 0.0f);
	//boxMesh.LoadMeshFromDisk(CONTENT_PATH + "Box_SM.obj");
	//boxMesh.LoadMeshFromDisk(CONTENT_PATH + "Sphere_SM.obj");
	boxMesh.LoadMeshFromDisk(CONTENT_PATH + "BD1_SM.FBX");
	//boxMesh.SetMaterial(0, &unlitMaterial);
	for (uint i = 0; i < boxMesh.GetMeshSections().size(); i++)
	{
		boxMesh.SetMaterial(i, &sphereMaterial);
	}

	StaticMesh sphereMesh;
	//sphereMesh.LoadMeshObj("../Content/Sphere_SM.obj");
	//sphereMesh.LoadMeshObj("../Content/MaterialTest_SM.obj");
		//sphereMesh.material = &sphereMaterial;
	//sphere.transform.position = glm::vec3(-2.0f, -.3f, 1.0f);

	/*
	MeshSection planeMesh;

	planeMesh.AddVertex(MeshSection::Vertex({  0.5f,  0.0f, -0.5f }, { 1.0f, 1.0f })); // Front - Top right
	planeMesh.AddVertex(MeshSection::Vertex({  0.5f, -0.0f,  0.5f }, { 1.0f, 0.0f })); // Front - Bottom right
	planeMesh.AddVertex(MeshSection::Vertex({ -0.5f, -0.0f,  0.5f }, { 0.0f, 0.0f })); // Front - Bottom left
	planeMesh.AddVertex(MeshSection::Vertex({ -0.5f,  0.0f, -0.5f }, { 0.0f, 1.0f })); // Front - Top left
	planeMesh.indices = {
		0, 1, 3,
		1, 2, 3
	};
	planeMesh.Construct();
	//plane.transform.scale = glm::vec3(3.0f);
	//plane.transform.position = glm::vec3(0.0f, -.35f, 0.0f);
	planeMesh.material = &unlitMaterial;


	MeshSection primMesh;
	primMesh.material = &cubemapMaterial;

	// Geometry
	// Front face
	primMesh.AddVertex(MeshSection::Vertex({  0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f })); // Front - Top right
	primMesh.AddVertex(MeshSection::Vertex({  0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f })); // Front - Bottom right
	primMesh.AddVertex(MeshSection::Vertex({ -0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f })); // Front - Bottom left
	primMesh.AddVertex(MeshSection::Vertex({ -0.5f,  0.5f,  0.5f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f })); // Front - Top left

	// Back face
	primMesh.AddVertex(MeshSection::Vertex({  0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, -1.0f })); // Back  - Top right
	primMesh.AddVertex(MeshSection::Vertex({  0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f })); // Back  - Bottom right
	primMesh.AddVertex(MeshSection::Vertex({ -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f })); // Back  - Bottom left
	primMesh.AddVertex(MeshSection::Vertex({ -0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, -1.0f })); // Back -  Top left

	// Top face
	primMesh.AddVertex(MeshSection::Vertex({  0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f })); // Front - Top right
	primMesh.AddVertex(MeshSection::Vertex({  0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f })); // Back  - Top right
	primMesh.AddVertex(MeshSection::Vertex({ -0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f })); // Back  - Top left
	primMesh.AddVertex(MeshSection::Vertex({ -0.5f,  0.5f,  0.5f }, { 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f })); // Front - Top left

	// Bottom face
	primMesh.AddVertex(MeshSection::Vertex({ 0.5f,   -0.5f,  0.5f }, { 1.0f, 1.0f }, { 0.0f, -1.0f, 0.0f })); // Front - Top right
	primMesh.AddVertex(MeshSection::Vertex({ 0.5f,   -0.5f, -0.5f }, { 1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f })); // Back  - Top right
	primMesh.AddVertex(MeshSection::Vertex({ -0.5f,  -0.5f, -0.5f }, { 0.0f, 0.0f }, { 0.0f, -1.0f, 0.0f })); // Back  - Top left
	primMesh.AddVertex(MeshSection::Vertex({ -0.5f,  -0.5f,  0.5f }, { 0.0f, 1.0f }, { 0.0f, -1.0f, 0.0f })); // Front - Top left

	// Left face
	primMesh.AddVertex(MeshSection::Vertex({ -0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f }, { -1.0f, 0.0f, 0.0f })); // Front - Top left
	primMesh.AddVertex(MeshSection::Vertex({ -0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f }, { -1.0f, 0.0f, 0.0f })); // Front - Bottom left
	primMesh.AddVertex(MeshSection::Vertex({ -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f })); // Back  - Bottom left
	primMesh.AddVertex(MeshSection::Vertex({ -0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f }, { -1.0f, 0.0f, 0.0f })); // Back -  Top left

	// Right face
	primMesh.AddVertex(MeshSection::Vertex({ 0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f })); // Front - Top left
	primMesh.AddVertex(MeshSection::Vertex({ 0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f })); // Front - Bottom left
	primMesh.AddVertex(MeshSection::Vertex({ 0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f })); // Back  - Bottom left
	primMesh.AddVertex(MeshSection::Vertex({ 0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f })); // Back -  Top left

	primMesh.SetColour(glm::vec3(0.0f, 1.0f, 0.0f));
	primMesh.indices = {
		0, 1, 3, // first triangle
		1, 2, 3,  // second triangle

		4, 5, 7,
		5, 6, 7,

		8, 9, 11,
		9, 10, 11,

		12, 13, 15,
		13, 14, 15,

		16, 17, 19,
		17, 18, 19,

		20, 21, 23,
		21, 22, 23
	};

	primMesh.Construct();
	std::vector<glm::vec3> positions = { glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(2.0f, -1.0f, -.5f), glm::vec3(-2.0f, 1.5f, -2.0f) };
	*/

	// ===================================== LEVELS ============================================


	StaticMeshObject boxMeshObject;
	boxMeshObject.staticMeshComponent = StaticMeshComponent(&boxMesh);
	boxMeshObject.Construct();
	boxMeshObject.transform.rotation = glm::quat(glm::radians(glm::vec3(0.0f, 45.0f, 0.0f)));
	boxMeshObject.SetDisplayName("Box mesh object");

	StaticMeshObject sphereMeshObject;
	sphereMeshObject.staticMeshComponent = StaticMeshComponent(&sphereMesh);
	sphereMeshObject.Construct();
	sphereMeshObject.transform.position = glm::vec3(-2.0f, -.3f, 1.0f);


	level.AddSceneObject(&boxMeshObject);
	level.AddSceneObject(&sphereMeshObject);
	level.Load();

	SceneOutliner::Select(&boxMeshObject);


	// ===================================== MAIN THREAD ============================================

	Game::Initialize();

	window.Bind();
	defaultCamera->Bind();


	while( !glfwWindowShouldClose(Window::GetCurrent()) ) // While window is open
	{
		// Update game specifics - i.e. deltaTime
		Game::Update();
		auto deltaTime = Game::GetDeltaTime();
		auto elapsedTime = Game::GetElapsedTime();


		glfwPollEvents(); // Check if any events (i.e. inputs) have been triggered


		// Camera controls
		defaultCamera->Update(deltaTime);
		EditorInput(deltaTime);
		SceneOutliner::Update(); // Update (tick) all objects in scene outliner


		// Start Dear ImGUI frame & draw Log & Dear ImGui widgets
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		Editor::DrawWidgets();
		Log::DrawScreenLog();
		ImGui::Render();


		// Clear screen with colour
		glClearColor(editorGUI.clearColour[0], editorGUI.clearColour[1], editorGUI.clearColour[2], editorGUI.clearColour[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear screen & depth buffers with bitwise operation on flags


		// Draw meshes
		SceneOutliner::Draw();
		Editor::Draw();


		// Draw ImGui data to screen - after we've rendered our scene
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Check events to call & swap buffers
		window.SwapBuffers();

		// Safely unbind last shader
		Shader::UnbindCurrent();

		// Reset asset manager's states
		AssetManager::ResetState();
	}



	// ===================================== CLEAN-UP ============================================

	Shader::Cleanup();
	SceneOutliner::Cleanup();
	EditorCamera::CleanupDefaultCamera();
	Editor::Cleanup();
	Python::Cleanup();
	Log::PrintInfo("Closing program");
	Log::Dump(); // Dump rest of the log

	// Clean-up ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Clean-up window
	if ( Window::GetCurrentObject()->window != nullptr )
		glfwDestroyWindow(Window::GetCurrentObject()->window);
	glfwTerminate(); // Clean up GLFW context
	return 0; // Return success code
}