#include <glad/glad.h>  // Hardware-specific loader
#include <GLFW/glfw3.h> // Window & input handler
#include "stb_image.h"  // Image loading library


// Our classes
#include "Window.h"
#include "Shader.h"
#include "Texture2D.h"
#include "Mesh.h"
#include "EditorCamera.h"
#include "StaticMeshObject.h"
#include "Level.h"



// Python wrapper
//#include <pybind11/pybind11.h>


const GLuint SRC_WIDTH = 1280;
const GLuint SRC_HEIGHT = 720;
Window window;
EditorCamera camera;
Shader shaderProgram, unlitShader, cubemapShader;

double elapsedTime = 0.0f, deltaTime = 0.0f;



// ===================================== EVENTS ============================================

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.UpdateFOV(deltaTime, glm::vec2(xoffset, yoffset));
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	camera.UpdateMouse(deltaTime, glm::vec2(xpos, ypos));
}

void EditorInput(const float& DeltaTime)
{
	// Input
	if (glfwGetKey(Window::GetCurrent(), GLFW_KEY_ESCAPE) == GLFW_PRESS) // Close window on escape press
		glfwSetWindowShouldClose(Window::GetCurrent(), true);

	else if (glfwGetKey(Window::GetCurrent(), GLFW_KEY_1) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		std::cout << "MODE: LIT" << std::endl;
	}
	else if (glfwGetKey(Window::GetCurrent(), GLFW_KEY_2) == GLFW_PRESS)
	{
		glLineWidth(5.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		std::cout << "MODE: WIREFRAME" << std::endl;
	}
	else if (glfwGetKey(Window::GetCurrent(), GLFW_KEY_3) == GLFW_PRESS)
	{
		glPointSize(5.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		std::cout << "MODE: POINTS" << std::endl;
	}
	else if (glfwGetKey(Window::GetCurrent(), GLFW_KEY_R) == GLFW_PRESS) // HOT RECOMPILE
	{
		std::cout << "SHADERS HOT RECOMPILE" << std::endl;
		Shader::RecompileAll();
	}
}

// ===================================== MAIN ============================================

int main(int argc, char* argv[])
{
	// GLFW initialize
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// GLFW create window
	window = Window(SRC_WIDTH, SRC_HEIGHT, "MyOpenGL");
	glfwSetScrollCallback(window.window, ScrollCallback);
	glfwSetCursorPosCallback(window.window, MouseCallback);
	glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Capture cursor input but always reset cursor to screen centre


	// ===================================== LOAD IN OPENGL CONTEXT ============================================

	// Initialize GLAD: load all function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // Load address of OpenGL function pointers to handle OS-specifics
	{
		std::cout << "Failed to initialize GLAD" << std::endl; // Error
		return -1; // Return error code
	}

	glEnable(GL_DEPTH_TEST);



	StaticMeshObject testObject;
	testObject.Construct();
	testObject.transform.rotation = glm::quat(glm::radians(glm::vec3(0.0f, 45.0f, 0.0f)));

	StaticMeshObject testObject2;
	testObject2.Construct();
	testObject2.transform.rotation = glm::quat(glm::radians(glm::vec3(0.0f, 90.0f, 0.0f)));

	Level level;
	//level.LoadFromDisk("../Content/TestScene.json");
	//level.AddSceneObject(&testObject);
	//level.AddSceneObject(&testObject2);
	level.LoadFromDisk("../LoadFrom.json");
	level.SaveToDisk("../SaveTo.json");
	std::system("pause");


	// ===================================== CAMERA ============================================

	camera.fieldOfView = 65.0f;
	camera.transform.position = glm::vec3(0.0f, 0.0f, 3.0f);
	camera.cursorPosition = glm::vec2((GLfloat)window.GetSize().x / 2.0f, (GLfloat)window.GetSize().y / 2.0f);
	//camera.transform.rotation = glm::quat(glm::vec3(0.0f, glm::radians(180.0f), 0.0));


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
	unlitMaterial.name = "Unlit_MI";

	Material cubemapMaterial(&cubemapShader);
	cubemapMaterial.name = "Cubemap_MI";
	cubemapMaterial.SetTextureParameter("EquirectangularMap", &environmentMap);

	Material checkerMaterial(&shaderProgram);
	checkerMaterial.name = "Checker_MI";
	checkerMaterial.SetTextureParameter("tex", &tex);
	checkerMaterial.SetTextureParameter("tex2", &environmentMap);
	checkerMaterial.SetVectorParameter("inMaterial.Albedo", glm::vec3(1.0f));
	checkerMaterial.SetFloatParameter("inMaterial.Metalness", 0.0f);
	checkerMaterial.SetFloatParameter("inMaterial.Roughness", 0.25f);
	checkerMaterial.SetFloatParameter("inMaterial.AmbientOcclusion", 1.0f);

	Material sphereMaterial(&shaderProgram);
	sphereMaterial.name = "Sphere_MI";
	sphereMaterial.SetTextureParameter("tex", &tex);
	sphereMaterial.SetTextureParameter("tex2", &tex2);
	sphereMaterial.SetVectorParameter("inMaterial.Albedo", glm::vec3(1.0f, 0.1f, 0.7f));
	sphereMaterial.SetFloatParameter("inMaterial.Metalness", 1.0f);
	sphereMaterial.SetFloatParameter("inMaterial.Roughness", 0.25f);
	sphereMaterial.SetFloatParameter("inMaterial.AmbientOcclusion", 1.0f);


	// ===================================== VAO & VBO ============================================
	
	Mesh boxMesh;
	boxMesh.LoadMeshObj("../Content/Box_SM.obj");
	//box.transform.rotation = glm::quat(glm::radians(glm::vec3(0.0f, 45.0f, 0.0f)));
	//box.transform.position = glm::vec3(-5.0f, 0.0f, 0.0f);
	boxMesh.material = &unlitMaterial;

	Mesh sphereMesh;
	//sphereMesh.LoadMeshObj("../Content/Sphere_SM.obj");
	sphereMesh.LoadMeshObj("../Content/MaterialTest_SM.obj");
	sphereMesh.material = &sphereMaterial;
	//sphere.transform.position = glm::vec3(-2.0f, -.3f, 1.0f);


	Primitive planeMesh;

	planeMesh.AddVertex(Primitive::Vertex({  0.5f,  0.0f, -0.5f }, { 1.0f, 1.0f })); // Front - Top right
	planeMesh.AddVertex(Primitive::Vertex({  0.5f, -0.0f,  0.5f }, { 1.0f, 0.0f })); // Front - Bottom right
	planeMesh.AddVertex(Primitive::Vertex({ -0.5f, -0.0f,  0.5f }, { 0.0f, 0.0f })); // Front - Bottom left
	planeMesh.AddVertex(Primitive::Vertex({ -0.5f,  0.0f, -0.5f }, { 0.0f, 1.0f })); // Front - Top left
	planeMesh.indices = {
		0, 1, 3,
		1, 2, 3
	};
	planeMesh.Construct();
	//plane.transform.scale = glm::vec3(3.0f);
	//plane.transform.position = glm::vec3(0.0f, -.35f, 0.0f);
	planeMesh.material = &unlitMaterial;


	Primitive primMesh;
	primMesh.material = &cubemapMaterial;

	// Geometry
	// Front face
	primMesh.AddVertex(Primitive::Vertex({  0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f })); // Front - Top right
	primMesh.AddVertex(Primitive::Vertex({  0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f })); // Front - Bottom right
	primMesh.AddVertex(Primitive::Vertex({ -0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f })); // Front - Bottom left
	primMesh.AddVertex(Primitive::Vertex({ -0.5f,  0.5f,  0.5f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f })); // Front - Top left

	// Back face
	primMesh.AddVertex(Primitive::Vertex({  0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, -1.0f })); // Back  - Top right
	primMesh.AddVertex(Primitive::Vertex({  0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f })); // Back  - Bottom right
	primMesh.AddVertex(Primitive::Vertex({ -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f })); // Back  - Bottom left
	primMesh.AddVertex(Primitive::Vertex({ -0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, -1.0f })); // Back -  Top left

	// Top face
	primMesh.AddVertex(Primitive::Vertex({  0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f })); // Front - Top right
	primMesh.AddVertex(Primitive::Vertex({  0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f })); // Back  - Top right
	primMesh.AddVertex(Primitive::Vertex({ -0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f })); // Back  - Top left
	primMesh.AddVertex(Primitive::Vertex({ -0.5f,  0.5f,  0.5f }, { 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f })); // Front - Top left

	// Bottom face
	primMesh.AddVertex(Primitive::Vertex({ 0.5f,   -0.5f,  0.5f }, { 1.0f, 1.0f }, { 0.0f, -1.0f, 0.0f })); // Front - Top right
	primMesh.AddVertex(Primitive::Vertex({ 0.5f,   -0.5f, -0.5f }, { 1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f })); // Back  - Top right
	primMesh.AddVertex(Primitive::Vertex({ -0.5f,  -0.5f, -0.5f }, { 0.0f, 0.0f }, { 0.0f, -1.0f, 0.0f })); // Back  - Top left
	primMesh.AddVertex(Primitive::Vertex({ -0.5f,  -0.5f,  0.5f }, { 0.0f, 1.0f }, { 0.0f, -1.0f, 0.0f })); // Front - Top left

	// Left face
	primMesh.AddVertex(Primitive::Vertex({ -0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f }, { -1.0f, 0.0f, 0.0f })); // Front - Top left
	primMesh.AddVertex(Primitive::Vertex({ -0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f }, { -1.0f, 0.0f, 0.0f })); // Front - Bottom left
	primMesh.AddVertex(Primitive::Vertex({ -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f })); // Back  - Bottom left
	primMesh.AddVertex(Primitive::Vertex({ -0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f }, { -1.0f, 0.0f, 0.0f })); // Back -  Top left

	// Right face
	primMesh.AddVertex(Primitive::Vertex({ 0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f })); // Front - Top left
	primMesh.AddVertex(Primitive::Vertex({ 0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f })); // Front - Bottom left
	primMesh.AddVertex(Primitive::Vertex({ 0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f })); // Back  - Bottom left
	primMesh.AddVertex(Primitive::Vertex({ 0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f })); // Back -  Top left

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


	// ===================================== SCENE OBJECTS ============================================


	StaticMeshObject boxMeshObject;
	boxMeshObject.staticMeshComponent = StaticMeshComponent(&boxMesh);
	boxMeshObject.Construct();
	boxMeshObject.transform.rotation = glm::quat(glm::radians(glm::vec3(0.0f, 45.0f, 0.0f)));

	StaticMeshObject sphereMeshObject;
	sphereMeshObject.staticMeshComponent = StaticMeshComponent(&sphereMesh);
	sphereMeshObject.Construct();
	sphereMeshObject.transform.position = glm::vec3(-2.0f, -.3f, 1.0f);



	// ===================================== MAIN THREAD ============================================

	window.Bind();
	camera.Bind();

	while( !glfwWindowShouldClose(Window::GetCurrent()) ) // While window is open
	{
		// Delta & elapsed time
		double time = glfwGetTime();
		deltaTime = time - elapsedTime;
		elapsedTime = time;


		// Camera controls
		camera.Update(deltaTime);
		EditorInput(deltaTime);


		// Clear screen with colour
		glClearColor(0.2f, 0.3f, 0.3f, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear screen & depth buffers with bitwise operation on flags


		//Draw meshes
		boxMeshObject.Draw();
		sphereMeshObject.Draw();

		/*
		box.Draw();
		sphere.Draw();
		plane.Draw();
		prim.Draw();

		for (int i = 0; i < positions.size(); i++)
		{
			glm::mat4 transform = glm::mat4(1.0f); // Identity
			transform = glm::translate(transform, positions[i]);
			prim.Draw(prim.transform.GetMatrix() * transform);
		}*/


		// Check events to call & swap buffers
		window.SwapBuffers();

		// Safely unbind last shader
		Shader::Unbind();

		glfwPollEvents(); // Check if any events (i.e. inputs) have been triggered
	}



	// ===================================== CLEAN-UP ============================================

	Shader::Cleanup();
	Primitive::Cleanup();
	glfwTerminate(); // Clean up GLFW context
	return 0; // Return success code
}