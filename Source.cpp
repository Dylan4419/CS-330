
//Dylan Coulter
// Module 3: Assignment Creating a 3D object
// 01/26/2024
//Resource: https://www.bing.com/videos/search?q=opengl+creating+multiple+3d+objects&docid=608040955936206028&mid=81098760493A26E86D4581098760493A26E86D45&view=detail&FORM=VIRE
//Resource:https://www.bing.com/videos/search?q=opengl+creating+multiple+3d+objects&&view=detail&mid=4BB5E8F0AFD65254B7514BB5E8F0AFD65254B751&&FORM=VRDGAR&ru=%2Fvideos%2Fsearch%3Fq%3Dopengl%2Bcreating%2Bmultiple%2B3d%2Bobjects%26FORM%3DHDRSC4
//Resource:https://www.bing.com/videos/search?q=opengl+creating+multiple+3d+objects&&view=detail&mid=37D51AF0122813AB718F37D51AF0122813AB718F&&FORM=VRDGAR&ru=%2Fvideos%2Fsearch%3Fq%3Dopengl%2Bcreating%2Bmultiple%2B3d%2Bobjects%26FORM%3DHDRSC4
#include <iostream>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;

/*Shader program Macro*/
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

// Unnamed namespace
namespace
{ //header for window title
	const char* const WINDOW_TITLE = "3-3 Assignment Building A 3D Pyramid";
	// Variables for window width and height
	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;
	// Struct to hold the GL data relative to a given mesh
	struct GLMesh {
		GLuint vao; //handle for vertex array object
		GLuint vbos[2]; //handles for the vertex buffer objects 
		GLuint nIndices; // Number of Indices of the mesh
	};
	// Main GLFW window
	GLFWwindow* gWindow = nullptr;
	// Triangle mesh data 
	GLMesh gMesh;
	// Shader Program
	GLuint gProgramId;
}
/* User-defined Function prototypes to:
 * initialize the program, set the window size,
 * redraw graphics on the window when resized,
 * and render graphics on the screen
 */
bool UInitialize(int, char* [], GLFWwindow** window);
void UResizeWindow(GLFWwindow* window, int width, int height);
void UProcessInput(GLFWwindow* window);
void UCreateMesh(GLMesh& mesh);
void UDestroyMesh(GLMesh& mesh);
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint&
	programId);
void UDestroyShaderProgram(GLuint programId);
void URender();
// Vertex Shader Program Source Code
const char* vertexShaderSource = GLSL(440,
	layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
out vec4 vertexColor; // variable to transfer color data to the fragment shader

//Global variables for the  transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
	// transforms vertices to clip coordinates
	gl_Position = projection * view * model * vec4(position.x, position.y, position.z, 1.0);
	// references incoming color data
	vertexColor = color;
}
);
// Fragment Shader Program Source Code
const char* fragmentShaderSource = GLSL(440,
	// Variable to hold incoming color data from vertex shader
	in vec4 vertexColor;
out vec4 fragmentColor;
void main()
{
	fragmentColor = vec4(vertexColor);
}
);
int main(int argc, char* argv[])
{
	if (!UInitialize(argc, argv, &gWindow))
		return EXIT_FAILURE;
	//The program will exit if the creation of the shader program fails
	if (!UCreateShaderProgram(vertexShaderSource, fragmentShaderSource, gProgramId))
		return EXIT_FAILURE;
	// Create the mesh
	UCreateMesh(gMesh);
	// render loop
	// -----------
	while (!glfwWindowShouldClose(gWindow))
	{
		// input
		// -----
		UProcessInput(gWindow);
		// Render this frame
		URender();

		glfwPollEvents();
	}
	// Release mesh data
	UDestroyMesh(gMesh);
	// Release shader program
	UDestroyShaderProgram(gProgramId);
	exit(EXIT_SUCCESS); // Terminates the program successfully
}
// Initialize GLFW, GLEW, and create a window
bool UInitialize(int argc, char* argv[], GLFWwindow** window)
{
	// GLFW: initialize and configure (specify desired OpenGL version)
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	// GLFW: window creation
	// ---------------------
	* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (*window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(*window);
	glfwSetFramebufferSizeCallback(*window, UResizeWindow);
	// GLEW: initialize
	// ----------------
	// Note: if using GLEW version 1.13 or earlier
	glewExperimental = GL_TRUE;
	GLenum GlewInitResult = glewInit();
	if (GLEW_OK != GlewInitResult)
	{
		std::cerr << glewGetErrorString(GlewInitResult) << std::endl;
		return false;
	}
	// Displays GPU OpenGL version
	cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << endl;
	return true;
}
// Process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void UProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
// glfw: whenever the window size changes (by OS or user resize) this callback function executes
void UResizeWindow(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
// Function called to render a frame
void URender() {
	// Enable z-depth
	glEnable(GL_DEPTH_TEST);
	// Clear the z buffers anf frame
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Object A is scaled by 2 
	glm::mat4 scaleA = glm::scale(glm::vec3(2.0f, 2.0f, 2.0f));
	//Rotates object A
	glm::mat4 rotationA = glm::rotate(40.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	//Object A being placed at the origin
	glm::mat4 translationA = glm::translate(glm::vec3(-1.0f, 0.0f, 0.0f));
	// Scales the solid pyramid object B by 2
	glm::mat4 scaleB = glm::scale(glm::vec3(2.0f, 2.0f, 2.0f));
	//Rotates the solid pyramid object B 
	glm::mat4 rotationB = glm::rotate(40.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	// Object A being placed at the origin
	glm::mat4 translationB = glm::translate(glm::vec3(2.0f, 0.0f, 0.0f));
	// Model matrices for line pyramid A
	glm::mat4 modelA = translationA * rotationA * scaleA;
	//Model matrices for solid  pyramid B 
	glm::mat4 modelB = translationB * rotationB * scaleB;
	// Transforms the camera to move back to the (z axis)
	glm::mat4 view = glm::translate(glm::vec3(0.0f, 0.0f, -6.0f));
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)WINDOW_WIDTH /
		(GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
	//Retrieves and passes transformation matrices to the Shader program
	GLuint modelLoc = glGetUniformLocation(gProgramId, "model");
	GLuint viewLoc = glGetUniformLocation(gProgramId, "view");
	GLuint projLoc = glGetUniformLocation(gProgramId, "projection");

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	// Activate the VBOs contained within the mesh's VAO
	glBindVertexArray(gMesh.vao);
	//Link the model matrix with uniform and draw the line pyramid
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelA));
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//Draws the triangle 
	glDrawElements(GL_TRIANGLES, gMesh.nIndices, GL_UNSIGNED_SHORT, NULL);

	//Link the model matrix with uniform and draw the solid pyramid
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelB));
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//Draws the triangle
	glDrawElements(GL_TRIANGLES, gMesh.nIndices, GL_UNSIGNED_SHORT, NULL);
	// Deactivate the Vertex Array Object
	glBindVertexArray(0);
	// glfw: swap buffers and pol IO events (keys pressed/ released, mouse moved etc.)
	glfwSwapBuffers(gWindow);    // Flips the back buffer with the front buffer every frame
}
//Implements the UCreateMesh function
void UCreateMesh(GLMesh& mesh) {
	//VERTEX POSITIONS AND COLOR DATA  
	GLfloat verts[] =
	{
		// Vertex positions			// Colors (r,g,b,a)
		0.5f, -0.5f, -1.0f,			1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.0f,			0.0f, 1.0f, 0.2f, 0.0f,
		-0.5f, -0.5f, -1.0f,		0.8f, 1.0f, 0.5f, 1.0f,
		-0.5f, -0.5f, 0.0f,			1.0f, 0.8f, 0.0f, 1.0f,
		0.0f, 0.5f, -0.5f,			1.0f, 0.0f, 0.5f, 1.0f
	};
	//Create one vertex array object
	glGenVertexArrays(1, &mesh.vao);
	glBindVertexArray(mesh.vao);
	//Generation of 2 buffers. One for index and the other for vertex
	glGenBuffers(2, mesh.vbos);
	//Activate the buffer
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]);
	//Sending coordinate data to the gpu
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	// Index data to share position
	GLushort index[] = {
	0,1,2,
	2,3,1,
	1,4,3,
	0,1,4,
	0,2,4,
	2,3,4 };

	mesh.nIndices = sizeof(index) / sizeof(index[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
	// Amount of coordinates per vertex
	const GLuint floatsPerVertex = 3;
	//Number of floats per color (r,g,b,a)
	const GLuint floatsPerColor = 4;
	// 6 strides between vertex coordinates (x, y, r, g, b, a) 
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerColor);
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, floatsPerColor, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) *
		floatsPerVertex));
	glEnableVertexAttribArray(1);
}
//memory take up by mesh will be released
void UDestroyMesh(GLMesh& mesh) {
	glDeleteVertexArrays(1, &mesh.vao);
	glDeleteBuffers(2, mesh.vbos);
}
//Implements the UCreateShaders function
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint&
	programId) {
	// Compilation and linkage error reporting
	int success = 0;
	char infoLog[512];
	// Create a shader program object
	programId = glCreateProgram();
	// Create the vertex and fragment shader objects
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	// Retrieve the shader source
	glShaderSource(vertexShaderId, 1, &vtxShaderSource, NULL);
	glShaderSource(fragmentShaderId, 1, &fragShaderSource, NULL);
	// Compile the vertex shader, and print compilation errors (if any) 
	glCompileShader(vertexShaderId);// compile the vertex shader
	//Check for shader compiler errors
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShaderId, sizeof(infoLog), NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
		return false;
	}
	// Compile fragment shader and check for errors
	glCompileShader(fragmentShaderId);
	//check for compile errors
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShaderId, sizeof(infoLog), NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
		return false;
	}
	//Attach compiled shaders to the shader program
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);

	glLinkProgram(programId);//links the shader program
	// check for linking errors
	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(programId, sizeof(infoLog), NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
		return false;
	}

	glUseProgram(programId);	// Use shader program
	return true;
}

void UDestroyShaderProgram(GLuint programId) {
	glDeleteProgram(programId);
}

