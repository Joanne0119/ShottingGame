#include <iostream>
#include <list>

#include "wmhandler.h"
#include "arcball.h"
#include "CQuad.h"
#include "CTriangle.h"
#include "CPenta.h"
#include "CTrapezid.h"
#include "CMissile.h"
#include "Cplayer.h"
#include "CStar.h"

#define EXAMPLE 5

extern Arcball g_arcball;
#if EXAMPLE == 1
extern CQuad* g_quad;
#elif EXAMPLE == 2
extern GLfloat g_viewScale;
extern CPenta* g_penta;
extern bool g_bMoving;
extern bool g_bRotating;
#elif EXAMPLE == 3
extern CQuad* g_quad;
extern CTriangle* g_tri;
extern CPenta* g_penta;

extern GLfloat g_viewScale;
extern bool g_bRotating;
extern bool g_bMoving;
#elif EXAMPLE == 4
#define QUAD_NUM 4
extern CQuad* g_RQuad;	// 紅色方塊再正中間
extern CQuad* g_GQuad;	// 綠色方塊為紅色方塊的 child
extern CQuad* g_BQuad;
extern bool g_bRotating;
extern bool g_bMoving;
extern glm::vec3 g_GDist[QUAD_NUM];
extern glm::vec3 g_BDist[QUAD_NUM];
extern glm::mat4 g_mxGDist[QUAD_NUM];
extern GLfloat g_viewScale;

#elif EXAMPLE == 5
extern CQuad* g_quad_shild;
extern CPlayer* player;


extern GLfloat g_viewScale;
extern bool g_bRotating;
extern bool g_bMoving;
extern bool gameStart;
#endif

// 滑鼠按鈕按下後 callback function(回呼函式) ---------------
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    // Arcball* arcball = static_cast<Arcball*>(glfwGetWindowUserPointer(window));
    g_arcball.onMouseButton(button, action, xpos, ypos);
    //std::cout << "button = " << button << "action = " << action << "mods = " << mods << std::endl;
#if EXAMPLE == 2
    if ( g_bMoving )
    {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			g_bRotating = ! g_bRotating;
		}
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        g_penta->reset();
    }
#elif EXAMPLE == 3
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		g_bRotating = !g_bRotating;
	}
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        
        g_tri->reset(); g_tri->setPos(glm::vec3(-1.0f, -2.0f, 0.0f));
        g_quad->reset(); g_quad->setPos(glm::vec3(1.0f, -2.0f, 0.0f));
        g_penta[0].reset(); g_penta[0].setPos(glm::vec3(-1.0f, -4.0f, 0.0f));
        g_penta[1].reset(); g_penta[1].setPos(glm::vec3(1.0f, -4.0f, 0.0f));
        g_penta[2].reset(); g_penta[2].setPos(glm::vec3(0.0f, -3.0f, 0.0f));
    }
#elif EXAMPLE == 4
#elif EXAMPLE == 5
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        g_bRotating = !g_bRotating;
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        player->reset();
    }
#endif
}
// ---------------------------------------------------------------------------------------

// 滑鼠在指定視窗上移動時的 callback function (回呼函式)
void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    // Arcball* arcball = static_cast<Arcball*>(glfwGetWindowUserPointer(window));
    g_arcball.onCursorMove(xpos, ypos, width, height);
    //std::cout << "x = " << xpos << "y = " << ypos << std::endl;
#if EXAMPLE == 2
    if (g_bMoving) {
        float px =  g_viewScale * (xpos - width / 2) / (width / 2);
        float py = -g_viewScale * (ypos - height / 2) / (height / 2);
        g_penta->setPos(glm::vec3(px, py, 0));
    }
#elif EXAMPLE == 3
    if (g_bMoving)
    {
        // 以滑鼠距離(0,0)的距離作為每一個模型的位移
		GLfloat dx = g_viewScale * (xpos - width / 2.0f) / (width / 2.0f); 
//		GLfloat dy = -g_viewScale * (ypos - height / 2.0f) / (height / 2.0f);

		glm::mat4 mxMove = glm::translate(glm::mat4(1.0f), glm::vec3(dx, 0.0f, 0.0f));
        g_tri->setTransformMatrix(mxMove);
		g_quad->setTransformMatrix(mxMove);
		g_penta[0].setTransformMatrix(mxMove);
		g_penta[1].setTransformMatrix(mxMove);
        g_penta[2].setTransformMatrix(mxMove);
    }
#elif EXAMPLE == 4
    glm::mat4 mxGRot, mxBRot;
    int i;
    if (g_bMoving)
    {
        // 以滑鼠距離(0,0)的距離作為每一個模型的位移
        GLfloat dx = g_viewScale * (xpos - width / 2.0f) / (width / 2.0f);
        GLfloat dy = -g_viewScale * (ypos - height / 2.0f) / (height / 2.0f);
        float xAngle = 3.1415916f*dx/4.0f; // dx -4 到 +4 代表角度  -PI 到  PI 度
        float yAngle = 3.1415916f*dy/2.0f; // dy -4 到 +4 代表角度 -2PI 到 2PI 度
        mxGRot = glm::rotate(glm::mat4(1.0f), xAngle, glm::vec3(0.0f, 0.0f, 1.0f));
        mxBRot = glm::rotate(glm::mat4(1.0f), yAngle, glm::vec3(0.0f, 0.0f, 1.0f));

        for (i = 0; i < 4; i++) g_GQuad[i].setTransformMatrix(mxGRot);
        for (i = 0; i < 4; i++) g_BQuad[i].setTransformMatrix(mxGRot*g_mxGDist[i]*mxBRot);
    }
    
#elif EXAMPLE == 5
    if (g_bMoving)
    {
        // 以滑鼠距離(0,0)的距離作為每一個模型的位移
        GLfloat dx = g_viewScale * (xpos - width / 2.0f) / (width / 2.0f);
//        GLfloat dy = -g_viewScale * (ypos - height / 2.0f) / (height / 2.0f);

        glm::mat4 mxMove = glm::translate(glm::mat4(1.0f), glm::vec3(dx, 0.0f, 0.0f));
        
        player->setTransformMatrix(mxMove);
        
        
        float xAngle = 3.1415916f*dx/2.0f;
        glm::mat4 mxGRot = glm::rotate(glm::mat4(1.0f), xAngle, glm::vec3(0.0f, 0.0f, 1.0f));
        
        glm::mat4 translateZ = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, 1.0f));

        
        glm::mat4 mxTransform = mxMove * translateZ * mxGRot ;
        
        for (int i = 0; i < 12; i++){ g_quad_shild[i].setTransformMatrix(mxTransform);
        }
        
        
    }
    
    if(gameStart){
        player->shoot();
        if(player->getState() == PlayerState::Dead){
            
            gameStart = !gameStart;
            g_bMoving = !g_bMoving;
        }
    }
#endif
}
// ---------------------------------------------------------------------------------------
// 
// 當視窗大小改變的 callback function
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// key : GLFW_KEY_0、 GLFW_KEY_a、GLFW_KEY_ESCAPE、GLFW_KEY_SPACE
//       完整定義在這邊 https://www.glfw.org/docs/latest/group__keys.html
// action : 
//          GLFW_PRESS：按鍵被按下。
//          GLFW_RELEASE：按鍵被釋放。
//          GLFW_REPEAT：按鍵被重複觸發（長按時會觸發多次）
// mods : 
//          GLFW_MOD_SHIFT：Shift 鍵被按下。
//          GLFW_MOD_CONTROL：Ctrl 鍵被按下。
//          GLFW_MOD_ALT：Alt 鍵被按下。
//          GLFW_MOD_SUPER：Windows 鍵或 Command 鍵被按下。
//          GLFW_MOD_CAPS_LOCK：Caps Lock 鍵被啟用。
//          GLFW_MOD_NUM_LOCK：Num Lock 鍵被啟用。
// 

    
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    switch (key)
    {
        case GLFW_KEY_ESCAPE:
            if (action == GLFW_PRESS) { glfwSetWindowShouldClose(window, true); }
            break;

        case GLFW_KEY_SPACE:
#if EXAMPLE == 2
            if (action == GLFW_PRESS) { g_bMoving = !g_bMoving; }
#elif EXAMPLE == 3
            if (action == GLFW_PRESS) { g_bMoving = !g_bMoving; }
#elif EXAMPLE == 4
            if (action == GLFW_PRESS) { g_bMoving = !g_bMoving; }
#elif EXAMPLE == 5
            if (action == GLFW_PRESS) { 
                g_bMoving = !g_bMoving;
                gameStart = !gameStart;
            }
#endif
            break;
        default: // 針對英文字母大小寫進行處理
            if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                // 檢查 Shift 鍵(左右兩邊各一個)是否被按下
                bool isShiftPressed = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ||
                                      (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);
                // 判斷字母鍵的大小寫，預設使用鍵盤是沒有按下 CAPS 鍵
                if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z) {
                    char letter = (isShiftPressed) ? ('A' + (key - GLFW_KEY_A)) : ('a' + (key - GLFW_KEY_A));
                    //std::cout << "key = " << letter << std::endl;
#if EXAMPLE == 1    
                    switch (letter) {
                        case 'R':
                        case 'r':
                            g_arcball.reset();
                            break;
						case 'S':
						case 's':
                            g_quad->setScale(glm::vec3(0.25f, 0.25f, 1.0f));
                            break;
                        case 'L':
                        case 'l':
                            g_quad->setScale(glm::vec3(1.2f, 1.2f, 1.0f));
                            break;
                        case 'X':
                        case 'x':
                            g_quad->setRotX(45.0f);
                            break;
                        case 'Y':
                        case 'y':
                            g_quad->setRotY(45.0f);
                            break;
                        case 'Z':
                        case 'z':
                            g_quad->setRotZ(45.0f);
                            break;
                    }
#elif EXAMPLE == 2
                    switch (letter) {
                        case 'S':
                        case 's':
                            g_penta->setScale(glm::vec3(0.2f, 0.2f, 1.0f));
                            break;
                        case 'L':
                        case 'l':
                            g_penta->setScale(glm::vec3(1.2f, 1.2f, 1.0f));
                            break;
                    }
#elif EXAMPLE == 3
                    switch (letter) {
                    case 'S':
                    case 's':
                        g_tri->setScale(glm::vec3(0.25f, 0.25f, 1.0f));
                        break;
                    case 'L':
                    case 'l':
                        g_tri->setScale(glm::vec3(1.2f, 1.2f, 1.0f));
                        break;
                    }
#elif EXAMPLE == 4



#endif
                }   
            }
        break;
    }
}
