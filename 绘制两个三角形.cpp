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
"   gl_Position = vec4(aPos.x,-aPos.y, aPos.z, 1.0);\n"
"}\0";
//片段着色器glsl
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0 ";
const char* fragmentShaderSource1 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(1.0f, 1.0f, 0.2f, 1.0f);\n"
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
	unsigned int fragmentShader1;
	fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);
	glCompileShader(fragmentShader1);
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
	unsigned int shaderProgram1;
	shaderProgram1 = glCreateProgram();
	glAttachShader(shaderProgram1, vertexshader);
	glAttachShader(shaderProgram1, fragmentShader1);
	glLinkProgram(shaderProgram1);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infolog);
		cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infolog << endl;
	}
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentShader);
	glDeleteShader(fragmentShader1);
	//顶点坐标
	float vertices[] = {
	 -0.9f, -0.5f, 0.0f,   
		-0.0f, -0.5f, 0.0f, 
		-0.45f, 0.5f, 0.0f,      
	};
	float vertices1[] = {
		0.0f, -0.5f, 0.0f,  
		0.9f, -0.5f, 0.0f,  
		0.45f, 0.5f, 0.0f    
	};
	//顶点缓存
	unsigned int VBO[2], VAO[2];
	glGenBuffers(2, VBO);
	glGenVertexArrays(2, VAO);
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	
	//启动引擎
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);//调用输入函数
		//清空上次渲染
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glUseProgram(shaderProgram1);
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);//交换颜色缓冲
		glfwPollEvents();//触发事件
	}
	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);
	glDeleteProgram(shaderProgram);
	glDeleteProgram(shaderProgram1);
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