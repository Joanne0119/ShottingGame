// Draw a  Circle with a radius of 0.8 unit
//#define GLM_ENABLE_EXPERIMENTAL 1

#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtx/string_cast.hpp>

#include "common/initshader.h"
#include "common/arcball.h"
#include "common/wmhandler.h"
#include "common/CQuad.h"

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 800
#define QUAD_NUM 4

Arcball g_arcball;
bool g_bRotating = false;
bool g_bMoving = false;

GLuint g_shaderProg;
glm::mat4 g_viewMx = glm::mat4(1.0f);
glm::mat4 g_projMx = glm::mat4(1.0f);
GLfloat g_viewScale = 4.0f;

// 紅、藍、綠四方形的彼此關係
CQuad* g_RQuad;	// 紅色方塊再正中間
CQuad* g_GQuad;	// 綠色方塊為紅色方塊的 child
CQuad* g_BQuad;	// 藍色方塊為綠色方塊的 child
glm::mat4 g_mxGDist[QUAD_NUM];

glm::vec3  g_GDist[QUAD_NUM] = { glm::vec3(1.5f, 0.0f, 0.0f), glm::vec3(-1.5f, 0.0f, 0.0f),
           glm::vec3(0.0f, 1.5f, 0.0f), glm::vec3(0.0f, -1.5f, 0.0f) };
glm::vec3  g_BDist[QUAD_NUM] = { glm::vec3(1.5f, 0.0f, 0.0f), glm::vec3(-1.5f, 0.0f, 0.0f),
           glm::vec3(0.0f, 1.5f, 0.0f), glm::vec3(0.0f, -1.5f, 0.0f) };

//----------------------------------------------------------------------------
void loadScene(void)
{
    int i;
    // 建立紅、藍、綠四方形的初始狀態
    g_RQuad = new CQuad;
    g_shaderProg = g_RQuad->setShader("vshader21.glsl","fshader21.glsl");
    g_RQuad->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
    g_RQuad->setScale(glm::vec3(0.3f, 0.3f, 0.0f));
	g_RQuad->setPos(glm::vec3(0.0f, 0.0f, 0.0f));

    g_GQuad = new CQuad[4];
    for ( i = 0; i < 4; i++)
    {
        g_GQuad[i].setShaderID(g_shaderProg);
        g_GQuad[i].setColor(glm::vec3(0.0f, 1.0f, 0.0f));
        g_GQuad[i].setScale(glm::vec3(0.3f, 0.3f, 0.0f));
        g_GQuad[i].setPos(g_GDist[i]);
    }
 
    g_BQuad = new CQuad[4];
    for (i = 0; i < 4; i++)
    {
        g_BQuad[i].setShaderID(g_shaderProg);
        g_BQuad[i].setColor(glm::vec3(0.0f, 0.0f, 1.0f));
        g_BQuad[i].setScale(glm::vec3(0.3f, 0.3f, 0.0f));
        g_BQuad[i].setPos(g_BDist[i]);
        // 設定藍色相對於綠色的位置
        g_mxGDist[i] = glm::translate(glm::mat4(1.0f), g_GDist[i]);
        g_BQuad[i].setTransformMatrix(g_mxGDist[i]);
    }

    GLint viewLoc = glGetUniformLocation(g_shaderProg, "mxView"); 	// 取得 MVP 變數的位置
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(g_viewMx));

    g_projMx = glm::ortho(-4.0f, 4.0f, -4.0f, 4.0f, -2.0f, 2.0f);
    GLint projLoc = glGetUniformLocation(g_shaderProg, "mxProj"); 	// 取得 MVP 變數的位置
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(g_projMx));
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 設定清除 back buffer 背景的顏色
}
//----------------------------------------------------------------------------

void render( void )
{
    glClear( GL_COLOR_BUFFER_BIT );			// clear the window
    g_RQuad->draw();
    for (int i = 0; i < QUAD_NUM; i++) g_GQuad[i].draw();
    for (int i = 0; i < QUAD_NUM; i++) g_BQuad[i].draw();
  
}
//----------------------------------------------------------------------------

float g_angle = 0.0f;
void update(float dt)
{

}

void releaseAll()
{
    if(g_RQuad != nullptr) delete g_RQuad;
    if(g_GQuad != nullptr) delete [] g_GQuad;
    if(g_BQuad != nullptr) delete [] g_BQuad;
}

int main() {
    // ------- 檢查與建立視窗  ---------------  
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // 設定 OpenGL 版本與 Core Profile
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.3
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //只啟用 OpenGL 3.3 Core Profile（不包含舊版 OpenGL 功能）
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // 禁止視窗大小改變

    // 建立 OpenGL 視窗與該視窗執行時所需的的狀態、資源和環境(context 上下文)
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Parent-Child Relationship", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // 設定將這個視窗的資源(OpenGL 的圖形上下文）與當前執行緒綁定，讓該執行緒能夠操作該視窗的資源
    glfwMakeContextCurrent(window);

    // 設定視窗大小, 這樣 OpenGL 才能知道如何將視窗的內容繪製到正確的位置
    // 基本上寬與高設定成視窗的寬與高即可
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    // ---------------------------------------

    // 設定相關事件的 callback 函式，以便事件發生時，能呼叫對應的函式
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);// 視窗大小被改變時
    glfwSetKeyCallback(window, keyCallback);                        // 有鍵盤的按鍵被按下時
    glfwSetMouseButtonCallback(window, mouseButtonCallback);        // 有滑鼠的按鍵被按下時
    glfwSetCursorPosCallback(window, cursorPosCallback);            // 滑鼠在指定的視窗上面移動時

    // 呼叫 loadScene() 建立與載入 GPU 進行描繪的幾何資料 
    loadScene();

    float lastTime = (float)glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        float currentTime = (float)glfwGetTime();
        float deltaTime = currentTime - lastTime; // 計算前一個 frame 到目前為止經過的時間
        lastTime = currentTime;
        update(deltaTime);      // 呼叫 update 函式，並將 deltaTime 傳入，讓所有動態物件根據時間更新相關內容
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    releaseAll(); // 程式結束前釋放所有的資源
    glfwTerminate();
    return 0;
}
