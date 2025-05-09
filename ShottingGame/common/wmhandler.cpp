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
extern CQuad* g_RQuad;
extern CQuad* g_GQuad;
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
extern int styleType;
#endif

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
        GLfloat dx = g_viewScale * (xpos - width / 2.0f) / (width / 2.0f);
        GLfloat dy = -g_viewScale * (ypos - height / 2.0f) / (height / 2.0f);
        float xAngle = 3.1415916f*dx/4.0f;
        float yAngle = 3.1415916f*dy/2.0f;
        mxGRot = glm::rotate(glm::mat4(1.0f), xAngle, glm::vec3(0.0f, 0.0f, 1.0f));
        mxBRot = glm::rotate(glm::mat4(1.0f), yAngle, glm::vec3(0.0f, 0.0f, 1.0f));

        for (i = 0; i < 4; i++) g_GQuad[i].setTransformMatrix(mxGRot);
        for (i = 0; i < 4; i++) g_BQuad[i].setTransformMatrix(mxGRot*g_mxGDist[i]*mxBRot);
    }
    
#elif EXAMPLE == 5
    if (g_bMoving)
    {
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
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

    
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
        default:
            if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                
                bool isShiftPressed = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ||
                                      (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);
                
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

#elif EXAMPLE == 5
                    switch (letter) {
                        case 's':
                        case 'S':
                            styleType = (styleType + 1) % 3;
                            switch (styleType){
                                case 0 :
                                    player->setStyle(PlayerState::Style1);
                                    break;
                                case 1 :
                                    player->setStyle(PlayerState::Style2);
                                    break;
                                case 2 :
                                    player->setStyle(PlayerState::Style3);
                                    break;
                                    
                            }
                    }
                
                    

#endif
                }   
            }
        break;
    }
}
