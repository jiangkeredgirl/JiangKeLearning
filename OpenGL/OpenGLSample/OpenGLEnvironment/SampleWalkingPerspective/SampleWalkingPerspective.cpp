//#include "stdafx.h"
//#include<gl/glut.h>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>
#include<gl/glu.h>
#include<gl/gl.h>
//#include <gl\GLAUX.h>

#include "Camera.h"
#include "CBMPLoader.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define WindowWidth  800
#define WindowHeight 600

/** 定义光源的属性值 */
GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };    /**< 环境光参数 */
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };    /**< 漫射光参数 */
GLfloat LightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };   /**< 镜面光参数 */
GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };   /**< 光源位置 */

Camera m_Camera;
//CBMPLoader m_Texture;

/** 加载位图纹理(木箱贴图) */
GLuint texture_id = 0;
bool LoadTexture()
{
    //if (!m_Texture.LoadImage("resources/textures/"))                                         /**< 载入位图文件 */
    //{
    //    MessageBox(NULL, (LPCSTR)"loadTexture error", (LPCSTR)"error", MB_OK);    /**< 如果载入失败则弹出对话框 */
    //    return false;
    //}

    
    glGenTextures(1, &texture_id);                                                /**< 生成一个纹理对象名称 */
    glBindTexture(GL_TEXTURE_2D, texture_id);                                     /**< 创建纹理对象 */

    /** 控制滤波 */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    /** 创建纹理 */
    int width, height, numChannel;
    unsigned char* data = stbi_load(".\\textures\\box.jpg", &width, &height, &numChannel, 0);
#if 1
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width,
        height, GL_RGB, GL_UNSIGNED_BYTE,
        data);
#else
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "load image failed0." << std::endl;
    }
#endif
    stbi_image_free(data);

    return true;
}

/** 绘制网格地面 */
void DrawGrid()
{
    /** 获得场景中一些状态  */
    GLboolean  lp, tp;
    glGetBooleanv(GL_LIGHTING, &lp);
    glGetBooleanv(GL_TEXTURE_2D, &tp);

    /** 关闭纹理和光照 */
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    /** 绘制过程 */
    glPushAttrib(GL_CURRENT_BIT);   /**< 保存当前属性 */
    glPushMatrix();                 /**< 压入堆栈 */
    glTranslatef(0.0f, 0.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);    /**< 设置颜色 */

    /** 在X,Z平面上绘制网格 */
    for (float i = -50; i <= 50; i += 1)
    {
        /** 绘制线 */
        glBegin(GL_LINES);

        /** X轴方向 */
        glVertex3f(-50, 0, i);
        glVertex3f(50, 0, i);

        /** Z轴方向 */
        glVertex3f(i, 0, -50);
        glVertex3f(i, 0, 50);

        glEnd();
    }
    glPopMatrix();
    glPopAttrib();

    /** 恢复场景状态 */
    if (tp)
        glEnable(GL_TEXTURE_2D);
    if (lp)
        glEnable(GL_LIGHTING);
}

/** 绘制球体 */
void DrawSphere()
{
    /** 设置材质属性 */
    GLfloat mat_ambient[] = { 0.9f, 0.5f, 0.8f, 1.0f };
    GLfloat mat_diffuse[] = { 0.9f, 0.5f, 0.8f, 1.0f };
    GLfloat mat_shininess[] = { 100.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    /** 获得纹理启用状态 */
    GLboolean tp;
    glGetBooleanv(GL_TEXTURE_2D, &tp);
    glDisable(GL_TEXTURE_2D);                   /**< 关闭纹理 */

    /** 绘制过程 */
    glPushMatrix();
    glTranslatef(-5.0f, 2.0f, -10.0f);
    GLUquadricObj* sphere = gluNewQuadric();
    gluQuadricOrientation(sphere, GLU_OUTSIDE);
    gluQuadricNormals(sphere, GLU_SMOOTH);
    gluSphere(sphere, 2.0, 50, 50);
    gluDeleteQuadric(sphere);
    glPopMatrix();

    /** 恢复状态 */
    if (tp)
        glEnable(GL_TEXTURE_2D);
}

/** 绘制木箱 */
void DrawBox()
{
    /** 设置材质属性 */
    GLfloat mat_ambient[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

    glPushMatrix();
    glTranslatef(5.0f, 2.0f, -10.0f);
    glScalef(2.0f, 2.0f, 2.0f);
    /** 选择纹理 */
    glBindTexture(GL_TEXTURE_2D, texture_id);
    /** 开始绘制四边形 */
    glBegin(GL_QUADS);
    /// 前侧面
    glNormal3f(0.0f, 0.0f, 1.0f);                               /**< 指定法线指向观察者 */
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    /// 后侧面
    glNormal3f(0.0f, 0.0f, -1.0f);                              /**< 指定法线背向观察者 */
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    /// 顶面
    glNormal3f(0.0f, 1.0f, 0.0f);                               /**< 指定法线向上 */
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    /// 底面
    glNormal3f(0.0f, -1.0f, 0.0f);                              /**< 指定法线朝下 */
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    /// 右侧面
    glNormal3f(1.0f, 0.0f, 0.0f);                               /**< 指定法线朝右 */
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    /// 左侧面
    glNormal3f(-1.0f, 0.0f, 0.0f);                              /**< 指定法线朝左 */
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glEnd();
    glPopMatrix();
}

void ChangeSize(int width, int height)
{
    glViewport(0, 0, width, height);                                    /**< 重新设置视口 */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 1.0f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void init(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    /** 设置光源的属性值 */
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);     /**< 设置环境光 */
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);     /**< 设置漫射光 */
    glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpecular);   /**< 设置漫射光 */

    /** 启用光源和纹理 */
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);
    glEnable(GL_TEXTURE_2D);

    /** 载入纹理 */
    if (!LoadTexture())
        MessageBox(NULL, (LPCSTR)"载入纹理失败!", (LPCSTR)"错误", MB_OK);

    /** 设置摄像机 */
    m_Camera.setCamera(0.0f, 1.5f, 6.0f, 0.0f, 1.5f, 0.0f, 0.0f, 1.0f, 0.0f);
    ChangeSize(WindowWidth, WindowHeight);
}

void display(void)
{
    /** 用户自定义的绘制过程 */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    /** 放置摄像机 */
    m_Camera.setLook();
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);   /**< 设置光源位置 */

    /**< 绘制过程 */
    DrawGrid();
    DrawSphere();
    DrawBox();

    glFlush();
}

void motion(int x, int y)
{
    m_Camera.setViewByMouse();

    //glutPostRedisplay();
}

static int xpos = 0;
static int ypos = 0;
void keyboard(GLFWwindow* window, int key, int x, int y)
{
    switch (key)
    {
    case 27:
        exit(0);
        break;
    case 49:
        m_Camera.setSpeed(0.2f);
        break;
    case 50:
        m_Camera.setSpeed(0.6f);
        break;
    case GLFW_KEY_UP:
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS
            && glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
        {
            if (ypos > 0)
            {
                ypos = 0;
            }
            ypos -= 2;
            m_Camera.setViewByKey(0, ypos);
        }
        else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        {  
            m_Camera.moveCamera(m_Camera.getSpeed());
        }
        else
        {
            m_Camera.pitchCamera(m_Camera.getSpeed());
        }
        break;
    case GLFW_KEY_DOWN:
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS
            && glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
        {
            if (ypos < 0)
            {
                ypos = 0;
            }
            ypos += 2;
            m_Camera.setViewByKey(0, ypos);
        }
        else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        {
            
            m_Camera.moveCamera(-m_Camera.getSpeed());
        }
        else
        {
            m_Camera.pitchCamera(-m_Camera.getSpeed());
        }
        break;
    case GLFW_KEY_LEFT:
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS
            && glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
        {  
            if (xpos > 0)
            {
                xpos = 0;
            }
            xpos -= 2;
            m_Camera.setViewByKey(xpos, 0);
        }
        else
        {
            m_Camera.yawCamera(-m_Camera.getSpeed());
        }
        break;
    case GLFW_KEY_RIGHT:
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS
            && glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
        {
            if (xpos < 0)
            {
                xpos = 0;
            }
            xpos += 2;
            m_Camera.setViewByKey(xpos, 0);
        }
        else
        {
            m_Camera.yawCamera(m_Camera.getSpeed());
        }
        break;
    }

    //glutPostRedisplay();
    //printf("========%d", key);
}

#if 0
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition((GetSystemMetrics(SM_CXSCREEN) >> 1) - 400, (GetSystemMetrics(SM_CYSCREEN) >> 1) - 300);
    glutCreateWindow("摄像机漫游");
    init();
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(display);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
#endif

//窗口大小改变回调函数：
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // 处理窗口大小改变事件
    ChangeSize(width, height);
}
//键盘按键回调函数：
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // 处理键盘按键事件
    std::cout << key << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE); 
    keyboard(window, key, 0, 0);
}
//鼠标点击回调函数：
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    // 处理鼠标点击事件
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos = 0;
        double ypos = 0;
        glfwGetCursorPos(window, &xpos, &ypos);
#if 0
        centerpoint_x = WindowWidth / 2/*xpos*/;
        centerpoint_y = WindowHeight / 2/*ypos*/;
#else
        m_Camera.middleX = xpos;
        m_Camera.middleY = ypos;
#endif
        printf("mouse_button_callback _Camera.middleX:%d, m_Camera.middleY:%d\n", m_Camera.middleX, m_Camera.middleY);

    }
}
//鼠标移动回调函数：
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    // 处理鼠标移动事件    
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        m_Camera.setViewByMouse(xpos, ypos);
    }
}
//滚轮滚动回调函数：
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // 处理滚轮滚动事件
    //camera.speedZ = yoffset * 100;
}

int main()
{
    std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
#if 0
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
#endif
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WindowWidth, WindowHeight, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    //注册鼠标事件回调函数
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    //注册滑轮事件回调函数
    glfwSetScrollCallback(window, scroll_callback);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    init();
    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        
        //viewMat = camera.GetViewMatrix();	

        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        display();
        // Swap the screen buffers
        //glfwSwapBuffers(window);
        //camera.UpdateCameraPos();
        //glfwPollEvents();
        // 交换缓冲区
        glfwSwapBuffers(window);

        // 处理事件
        glfwPollEvents();
    }

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}
