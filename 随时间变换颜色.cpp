#include <glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
using namespace std;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
//顶点着色器glsl
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//片段着色器glsl
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourcolor;\n"
"void main()\n"
"{\n"
"FragColor = ourcolor;\n"
"}\0 ";
int main()
{   //初始化步骤
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//所选客户端 API 的任何有效主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//所选客户端 API 的任何有效次要版本号
	glfwWindowHint(GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//创建窗口对象
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//初始化glad  用于管理函数指针
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}
	//视口 长800 宽600
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//注册回调函数，让窗口调用
	//顶点着色器
	unsigned int vertexshader;
	vertexshader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexshader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexshader);
	int success;
	char infolog[512];
	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexshader, 512, NULL, infolog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infolog << endl;
	}
	//片段着色器
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infolog << endl;
	}
	//着色器合并
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexshader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infolog);
		cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infolog << endl;
	}
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentShader);
	//顶点坐标
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};
	//顶点缓存
	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//启动引擎
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);//调用输入函数
		//清空上次渲染
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		//uniform赋值
		float timevalue = glfwGetTime();
		float greenvalue = sin(timevalue) / 2.0f+0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourcolor");
		glUniform4f(vertexColorLocation, 1.0f, greenvalue, 1.0f, 1.0f);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);//交换颜色缓冲
		glfwPollEvents();//触发事件
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	glfwTerminate();
	return 0;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}//回调函数调整窗口
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}//输入函数