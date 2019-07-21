//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 20/06/2019.
//
// Inspired by the following tutorials:
// - https://learnopengl.com/Getting-started/Hello-Window
// - https://learnopengl.com/Getting-started/Hello-Triangle

#include <iostream>

#include <cstdio>


#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
// initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language

#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices


// Setting the window size as a global parameter
const GLint WIDTH = 800, HEIGHT = 600;

// Can be used this way rather than returning from the createVertexBufferObject
// Declaring global variables VBO and VAO and shader 
// GLuint VBO, VAO, shader;

const char* getVertexShaderSource()
{
	// TODO - Insert Vertex Shaders here ...
	// For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files

	return

		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;"
		"layout (location = 1) in vec3 aColor;"
		""
		"uniform mat4 worldMatrix;"
		"uniform mat4 viewMatrix = mat4(1.0);" // default value for view matrix identity
		"uniform mat4 projectionMatrix = mat4(1.0);"

		"out vec3 vertexColor;"
		"void main()"
		"{"
		" vertexColor = aColor;"
		" mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;"
		" gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z,1.0);"
		"}";

	// return "need to return vertex shader source code here...";
}


const char* getFragmentShaderSource()
{
	// TODO - Insert Fragment Shaders here ...

	return
		"#version 330 core\n"
		"in vec3 vertexColor;"
		"out vec4 FragColor;"
		"void main()"
		"{"
		" FragColor = vec4(vertexColor.r, vertexColor.g,"
		" vertexColor.b, 1.0f);"
		"}";

	// return "need to return fragment shader source code here...";
}


int compileAndLinkShaders()
{
	// TODO
	// compile and link shader program
	// return shader program id
	// ------------------------------------

	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexShaderSource = getVertexShaderSource();
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentShaderSource = getFragmentShaderSource();
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	// link shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;
}

int createVertexBufferObject()
{
	// TODO
	// Upload geometry to GPU and return the Vertex Buffer Object ID


	// A vertex is a point on a polygon, it contains positions and other data (eg: colors)
	glm::vec3 vertexArray[] = {
	glm::vec3(0.0f, 0.5f, 0.0f), // top center position
	glm::vec3(1.0f, 0.0f, 0.0f), // top center color (red)
	glm::vec3(0.5f, -0.5f, 0.0f), // bottom right
	glm::vec3(0.0f, 1.0f, 0.0f), // bottom right color (green)
	glm::vec3(-0.5f, -0.5f, 0.0f), // bottom left
	glm::vec3(0.0f, 0.0f, 1.0f), // bottom left color (blue)
	};

	// Create a vertex array object (VAO)
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	// Binding allows all openGL function that requires VAO to use this one
	glBindVertexArray(vertexArrayObject);

	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject) (VBO)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	// Binding allows all openGL function that requires VBO to use this one
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

	// Connecting the bufferDataObject with the vertexArray (VBO -> Vertex[]
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

	// glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 9, vertexArray, GL_STATIC_DRAW); ???

	// Specify where the data is in the VAO � this allows OpenGL to bind data to vertex shader attributes
	glVertexAttribPointer(0, // attribute 0 matches aPos in Vertex Shader
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		2 * sizeof(glm::vec3), // stride - each vertex contain 2 vec3 (position, color)
		(void*)0 // array buffer offset
	);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		2 * sizeof(glm::vec3),
		(void*)sizeof(glm::vec3) // color is offseted a vec3 (comes after position)
	);
	glEnableVertexAttribArray(1);

	return vertexBufferObject;
}


int main(int argc, char* argv[])
{
	// Initialize GLFW and OpenGL version
	if (glfwInit() != GLFW_TRUE)
	{
		printf("GLFW failed to initialize");
		glfwTerminate();
		return -1;
	}

#if defined(PLATFORM_OSX)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
	// On windows, we set OpenGL version to 2.1, to support more hardware

	// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core Profile = No Backwards Compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);


#endif

	// Create Window and rendering context using GLFW, resolution is 800x600
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Comp371 - Lab 01", NULL, NULL);
	if (mainWindow == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Get Buffer Size information - The dimension of the area of the middle of the window - 
	// holds the data passed to the window by openGL - that would be the size of the openGL viewports that is drawn to 
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set the context for GLEW to use - OpenGL knows which window to draw to through the context
	glfwMakeContextCurrent(mainWindow);


	// Allow modern extension features - enabling GLEW
	glewExperimental = true; // Needed for core profile

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to create GLEW" << std::endl;
		glfwTerminate();
		return -1;
	}

	// optional - setting up viewport of the window that has to be drawn to - Use the bufferwidth and bufferheight
	glViewport(0, 0, bufferWidth, bufferHeight);


	// Black background
	// glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearColor(static_cast<float>(99.0 / 255.0), static_cast<float>(21.0 / 255.0), static_cast<float>(35.0 / 255.0), 1.0f);


	// Compile and link shaders here ...
	int shaderProgram = compileAndLinkShaders();

	// Define and upload geometry to the GPU here ...
	int vbo = createVertexBufferObject();

	// Variables to be used later in tutorial
	float angle = 0;
	float rotationSpeed = 180.0f;  // 180 degrees per second
	float lastFrameTime = glfwGetTime();

	//glEnable(GL_CULL_FACE);

	// Entering Main Loop
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Each frame, reset color of each pixel to glClearColor
		glClear(GL_COLOR_BUFFER_BIT);


		// TODO - draw rainbow triangle
		glUseProgram(shaderProgram);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		float dt = glfwGetTime() - lastFrameTime;
		lastFrameTime += dt;

		angle = (angle + rotationSpeed * dt);

		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

		GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");

		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &rotationMatrix[0][0]);



		glDrawArrays(GL_TRIANGLES, 0, 3);



		// End frame
		glfwSwapBuffers(mainWindow);

		// Detect inputs
		glfwPollEvents();

		if (glfwGetKey(mainWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(mainWindow, true);

		if (glfwGetKey(mainWindow, GLFW_KEY_1) == GLFW_PRESS)
		{
			glm::mat4 viewMatrix = glm::mat4(1.0f);

			GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
			glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
		}

		//(0,0,1)

		if (glfwGetKey(mainWindow, GLFW_KEY_2) == GLFW_PRESS)
		{
			glm::mat4 viewMatrix = glm::lookAt(
				glm::vec3(-0.0f, 0.5f, 1.0f),	//eye
				glm::vec3(-0.0f, 0.0f, -1.0f),	//center
				glm::vec3(0.0f, 1.0f, 0.0f));	//up
			/*
			glm::mat4 viewMatrix = glm::lookAt(glm::vec3(-0.5f, 0.0f, 0.0f),
											   glm::vec3(-0.5f, 0.0f, -1.0f),
											   glm::vec3(0.0f, 1.0f, 0.0f));
				*/

			GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
			glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
		}

		if (glfwGetKey(mainWindow, GLFW_KEY_5) == GLFW_PRESS)
		{
			glm::mat4 viewMatrix = glm::lookAt(
				glm::vec3(-0.0f, -0.5f, 0.0f),	//eye
				glm::vec3(-0.0f, 0.0f, -1.0f),	//center
				glm::vec3(0.0f, 1.0f, 0.0f));	//up
			/*
			glm::mat4 viewMatrix = glm::lookAt(glm::vec3(-0.5f, 0.0f, 0.0f),
											   glm::vec3(-0.5f, 0.0f, -1.0f),
											   glm::vec3(0.0f, 1.0f, 0.0f));
				*/

			GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
			glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
		}

		// Binding a perspective and an orthographic camera to keys 3 and 4
		if (glfwGetKey(mainWindow, GLFW_KEY_3) == GLFW_PRESS)
		{

			glm::mat4 viewMatrix = glm::lookAt(
				glm::vec3(-0.0f, 0.5f, 1.0f),	//eye
				glm::vec3(-0.0f, 0.0f, -1.0f),	//center
				glm::vec3(0.0f, 1.0f, 0.0f));	//up
			/*
			glm::mat4 viewMatrix = glm::lookAt(glm::vec3(-0.5f, 0.0f, 0.0f),
											   glm::vec3(-0.5f, 0.0f, -1.0f),
											   glm::vec3(0.0f, 1.0f, 0.0f));
				*/

			GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
			glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

			glm::mat4 projectionMatrix = glm::perspective(70.0f, (800.0f / 600.0f), 0.01f, 100.0f);

			GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
			glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
		}

		if (glfwGetKey(mainWindow, GLFW_KEY_4) == GLFW_PRESS)
		{
			//ortho vector is (left, right, bottom, top , near , far
			glm::mat4 projectionMatrix = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, -100.0f, 100.0f);

			GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
			glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
		}


	}

	// Shutdown GLFW
	glfwTerminate();

	return 0;
}

float rgb2OpenGlColor(int rgbColor)
{
	return (rgbColor / 255);
}