// OpenGLEnvironment.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

//int main()
//{
//
//    std::cout << "Hello World!\n";
//}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
#define GLEW_STATIC
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include <glm/>


void processInput(GLFWwindow* window);


//模型顶点数据
float vertices[] = {
	0.5f, 0.5f, 0.0f,   /* Postion 右上角  Color*/  1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.0f,  /* 右下角 */			    0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.0f, /* 左下角 */			    0.0f, 0.0f, 1.0f,
	-0.5f, 0.5f, 0.0f,  /* 左上角*/					1.0f, 0.0f, 1.0f,
};


unsigned int indices[] = { // 注意索引从0开始! 
	0, 1, 3, // 第一个三角形
	1, 2, 3  // 第二个三角形
};

const char* vertexShaderSource =
"#version 330 core                                \n		"
"layout(location = 0) in vec3 aPos;              \n			"
"layout(location = 1) in vec3 aColor;              \n			"
"out vec4 vertexColor;                                      \n"
"void main(){\n												"
"		gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n	"
"		vertexColor = vec4(aColor.x,aColor.y,aColor.z,1.0);}\n					";

const char* fragmentShaderSource =
"#version 330 core                             \n	"
"in vec4 vertexColor; \n"
"out vec4 FragColor;                           \n	"
"void main(){\n										"
"		FragColor = vertexColor;}\n";


void DisplayGPUInfo()
{
	const GLubyte* info = glGetString(GL_VENDOR);
	printf("Graphics Company:%s\n", info);

	info = glGetString(GL_RENDERER);
	printf("Graphics card model:%s\n", info);

	info = glGetString(GL_VERSION);
	printf("GL_VERSION:%s\n", info);

	info = glGetString(GL_SHADING_LANGUAGE_VERSION);
	printf("GL_SHADING_LANGUAGE_VERSION:%s\n", info);

	//info = glGetString(GL_EXTENSIONS);
	//printf("GL_EXTENSIONS:%s\n", info);
}

int main(int argc, char* argv[]) 
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(800, 600, "Test window", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();//销毁窗口，释放资源
		//return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);//指定线程
	DisplayGPUInfo();
	
	//设置opengl版本3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//告诉glfw主要版本3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//告诉glfw次要版本3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//告诉glfw使用core-profile
	
	glewExperimental = true;

	if (glewInit() != GLEW_OK)//初始glew工具
	{
		std::cout << "glew init failed." << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	//设置视口视角
	glViewport(0, 0, 800, 600);
	/*
	//框线模式 Wireframe Mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//剔除多边形背面
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);//GL_BACK:剔除背面 ，GL_FRONT:剔除正面
	*/
	//VAO 绑定Vertex Arrays
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//VBO
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//EBO
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//编译
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	//link 程序
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	while (!glfwWindowShouldClose(window))
	{
		//1.input
		//--------
		processInput(window);

		//2.render
		//--------
		//设置要填充的颜色（r 0-1.0，g 0-1.0，b 0-1.0，不透明度0-1.0）
		glClearColor(0.2f, 0.3f, 0.3f, 0.1f);
		//清空colorbuff的缓冲区
		glClear(GL_COLOR_BUFFER_BIT);
		//--------

		//Draw Triangle
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		glUseProgram(shaderProgram);
		//glDrawArrays(GL_TRIANGLES, 0, 6);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//3.swap buffers and Poll IO events
		//-------
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwTerminate();
	//std::cin.get();
	return 0;
}
//Esc按键响应程序
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}


}
