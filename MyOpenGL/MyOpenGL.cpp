#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include "ShaderProgram.h"

const unsigned int SRC_WIDTH = 800;
const unsigned int SRC_HEIGHT = 600;


// ===================================== EVENTS ============================================

void FramebufferSizeCallback(GLFWwindow* Window, int Width, int Height)
{
	// Make sure viewport matches new window dimensions when resized
	glViewport(0, 0, Width, Height);
}


// ===================================== MAIN ============================================

int main()
{
	// GLFW initialize
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// GLFW create window
	GLFWwindow* window = glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, "MyOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1; // Error
	}
	glfwMakeContextCurrent(window); // Set context to our new window
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);


	// ===================================== LOAD IN OPENGL CONTEXT ============================================

	// Initialize GLAD: load all function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // Load address of OpenGL function pointers to handle OS-specifics
	{
		std::cout << "Failed to initialize GLAD" << std::endl; // Error
		return -1; // Return error code
	}


	// ===================================== SHADERS ============================================

	std::string shadersDir = "E:/Documents/PostUniversity/OpenGL/MyOpenGL/Shaders/Main";
	ShaderProgram shaderProgram;
	shaderProgram.CompileShadersFromFolder(shadersDir);
	shaderProgram.LinkShaders();


	// ===================================== VAO & VBO ============================================
	
	// EBO
	GLfloat vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	GLuint indices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// Bind VAO, then bind and set buffers, then configure attributes
	glBindVertexArray(VAO);

	// Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);


	// Note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 	// uncomment this call to draw in wireframe polygons.



	// ===================================== MAIN THREAD ============================================

	while( !glfwWindowShouldClose(window) ) // While window is open
	{
		// Input
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // Close window on escape press
			glfwSetWindowShouldClose(window, true);

		else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			std::cout << "MODE: LIT" << std::endl;
		}
		else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		{
			glLineWidth(5.0f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			std::cout << "MODE: WIREFRAME" << std::endl;
		}
		else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		{
			glPointSize(5.0f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			std::cout << "MODE: POINTS" << std::endl;
		}

		else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		{
			std::cout << "SHADERS HOT RECOMPILE" << std::endl;
			shaderProgram.CompileShadersFromFolder(shadersDir);
			shaderProgram.LinkShaders();
		}



		// Clear screen with colour
		glClearColor(0.2f, 0.3f, 0.3f, 1.0);
		glClear(GL_COLOR_BUFFER_BIT); // Clear screen

		// ...

		shaderProgram.Bind();
		glBindVertexArray(VAO); // Bind VAO
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, sizeof(vertices) / sizeof(vertices[0]), GL_UNSIGNED_INT, 0);
		// glBindVertexArray(0); // Unbind


		// Check events to call & swap buffers
		glfwSwapBuffers(window); // Swap the colour buffer and show to screen
		glfwPollEvents(); // Check if any events (i.e. inputs) have been triggered
	}



	// ===================================== CLEAN-UP ============================================

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate(); // Clean up GLFW context
	return 0; // Return success code
}