//功能需求
//   (8%) 操控與背景部分
//   ✓   (1%) 滑鼠可以控制戰鬥機的左右移動
//   ✓   (1%) 戰鬥機傭有防禦裝置，並以父子關係方式呈現
//   ✓   (1%) 可以發射飛彈
//   ✓   (3%) 能提供連續發射(LINKED LIST，自己撰寫，使用STL 2分)
//   ✓   (2%)  能產生有速度感的背景物件，或是其他裝飾性的物件
//   (11%) 敵人部分
//   ✓   (2%)  有至少三種以上不同外形的敵人(不同的顏色)，基本的四方型不算在內
//   ✓   (3%) 以物件導向的多型來控制所有的敵人
//   ✓   (1%)  敵人可以不斷的產生，而且具有不同的顏色
//   ✓   (1%)  敵人能隨機朝向玩家發射子彈攻擊
//   ✓   (2%)  戰鬥機發射的子彈可以打到敵人，而且敵人會消失
//      (2%)  有 BOSS 級的敵人，且至會根據被攻擊的多寡至少三種不同的狀態(外型改變或攻擊方式)可以切換
//      (4%) (玩家部分)
//   ✓   (2%)  能判斷玩家是否被打中 並做出合理的反應
//      (2%)  玩家的船艦至少有三種狀態(外型改變)，且有提供玩家的船艦可改變狀態的機制
//      (8%) 其他你覺得可以展示的技術，包含物理或是數學的運算
//      (2%)提供階層式動態控制，並以時間為基礎進行動態的展示(如: OpenGL_2 的 Example4 ，以自動產生的軌跡去控制相關的物件運動)
//      (2%)發射導向飛彈攻擊移動的 Boss
//          敵人被打到有其他的效果
//          戰鬥機被打到時有其他的效果
//          背景除了速度感物件外，有其他的效果


#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <cstdlib>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "common/CQuad.h"
#include "common/CTriangle.h"
#include "common/CPenta.h"
#include "common/CTrapezid.h"
#include "common/Cplayer.h"
#include "common/CMissile.h"
#include "common/CStar.h"
#include "common/EnemyA.h"
#include "common/EnemyB.h"
#include "common/EnemyC.h"
#include "common/CEnemy.h"
#include "common/CHeart.h"
#include "common/initshader.h"
#include "common/arcball.h"
#include "common/wmhandler.h"

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 800
const int MAX_ENEMIES = 12;

Arcball g_arcball;
bool g_bRotating = false;
bool g_bMoving = false;
bool gameStart = false;
int styleType;

GLuint g_shaderProg;
glm::mat4 g_viewMx = glm::mat4(1.0f);
glm::mat4 g_projMx = glm::mat4(1.0f);
GLfloat g_viewScale = 4.0f;

CTrapezid* g_trap_body = nullptr;
CQuad* g_quad_shild = nullptr;
CPlayer* player = nullptr;
Star* star = nullptr;
std::vector<CHeart*> hearts;
std::vector<CEnemy*> enemies;

static float spawnTimer = 0.0f;

int starNumber = 50;
glm::vec3 g_GDist[12];
void initCirclePoints() {
    glm::vec3 center(0.0f, 0.0f, 1.0f);  // 圓心
    float radius = 1.1f;                  // 半徑
    int pointCount = 12;

    for (int i = 0; i < pointCount; ++i) {
        float angle = glm::radians(360.0f / pointCount * i);  // 角度轉換為弧度
        float x = center.x + radius * cos(angle);
        float y = center.y + radius * sin(angle);
        float z = center.z;
        g_GDist[i] = glm::vec3(x, y, z);
    }
}

void spawnEnemy();

void loadScene(void){
    g_shaderProg = createShader("vshader21.glsl","fshader21.glsl");
    
    star = new Star[starNumber];
    for (int i = 0; i <starNumber; i++) {
        star[i].setShaderID(g_shaderProg);
        star[i].setColor(glm::vec3(0.5f, 0.5f, 0.0f));
        star[i].setScale(glm::vec3(0.2f));  // 調整星星的大小
        star[i].setPos(glm::vec3(rand() % 20 - 10, rand() % 20 - 10, 0.0f));  // 隨機生成星星位置
        star[i]._speed = 0.6f + static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    }
    
    player = new CPlayer(g_shaderProg);
    
    initCirclePoints();
    g_quad_shild = new CQuad[12];
    for (int i = 0; i < 12; i++)
    {
        glm::mat4 translateZ = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, 1.0f));
        g_quad_shild[i].setShaderID(g_shaderProg);
        g_quad_shild[i].setColor(glm::vec3(0.2f, 0.5f, 0.1f));
        g_quad_shild[i].setScale(glm::vec3(0.1f, 0.1f, 0.0f));
        g_quad_shild[i].setPos(glm::vec3(g_GDist[i]));
        g_quad_shild[i].setTransformMatrix(translateZ);
    }
    
    for (int i = 0; i < 5; i++) {
        CHeart* heart = new CHeart();
        heart->setShaderID(g_shaderProg); // 如果有 shader
        heart->setPos(glm::vec3(-3.5f + i * 0.3f, -3.5f, 0.0f)); // 靠左上排列
        heart->setScale(glm::vec3(0.3f, 0.3f, 1.0f)); // 視情況調整大小
        hearts.push_back(heart);
    }
    
    GLint viewLoc = glGetUniformLocation(g_shaderProg, "mxView");     // 取得 MVP 變數的位置
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(g_viewMx));

    g_projMx = glm::ortho(-4.0f, 4.0f, -4.0f, 4.0f, -2.0f, 2.0f);
    GLint projLoc = glGetUniformLocation(g_shaderProg, "mxProj");     // 取得 MVP 變數的位置
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(g_projMx));
    glClearColor(0.08f, 0.08f, 0.18f, 1.0f); // 設定清除 back buffer 背景的顏色
}
// MARK: -render

void render(void){
    glClear( GL_COLOR_BUFFER_BIT );            // clear the window
    for (int i = 0; i < starNumber; i++) star[i].draw();
    for (int i = 0; i < 12; i++) g_quad_shild[i].draw();
    player->draw();
    for (auto e : enemies) e->draw();
    
    int hp = player->getHp();
    for (int i = 0; i < hearts.size(); i++) {
        if (i < hp) {
            hearts[i]->setColor(glm::vec3(1.0f, 0.2f, 0.3f)); // 紅色
        } else {
            hearts[i]->setColor(glm::vec3(0.3f, 0.3f, 0.3f)); // 灰色
        }
        hearts[i]->draw();
    }

}

// MARK: - update
float g_angle = 0.0f;
float missileCooldown = 0.0f;
float fireRate = 0.3f; // 每隔 0.3 秒發一發
void update(float dt)
{
    if(gameStart){
        
        
        for (int i = 0; i < starNumber; i++) star[i].update(dt);
        player->update(dt);
    //    std::cout << player->getPos().x << "\n";
        
        for (auto enemyIt = enemies.begin(); enemyIt != enemies.end(); ) {
            CEnemy* enemy = *enemyIt;

            enemy->update(dt);  // 更新計時器或狀態
//            enemy->shoot();

            if (enemy->getState() == EnemyState::Dead) {
                delete enemy;
                enemyIt = enemies.erase(enemyIt);
            } else {
                ++enemyIt;
            }
        }
    
    
        spawnTimer += dt;
        if (spawnTimer > 1.8f) { // 每 1.5 秒產生一個
            spawnEnemy();
            spawnTimer = 0.0f;
        }
        // 拿到玩家的子彈清單（是 std::list<CMissile*>）
        auto& missiles = player->getMissiles();

        for (auto missileIt = missiles.begin(); missileIt != missiles.end(); ) {
            CMissile* missile = *missileIt;
            bool hit = false;

            for (auto enemyIt = enemies.begin(); enemyIt != enemies.end(); ) {
                CEnemy* enemy = *enemyIt;

                // 碰撞邏輯（使用 glm::distance 或你的判斷方式）
                float dist = glm::distance(missile->getPos(), enemy->getPos());
                if (dist < 0.3f) { // 根據實際模型大小調整
                    enemy->onHit(1); // 扣血
                    hit = true;

                    // 如果敵人死了，移除敵人
                    if (enemy->isDead()) {
                        enemy->setState(EnemyState::Exploding);
                        ++enemyIt;
                    } else {
                        ++enemyIt;
                    }

                    break; // 一顆子彈只打中一個敵人
                } else {
                    ++enemyIt;
                }
            }

            if (hit) {
                delete missile;
                missileIt = missiles.erase(missileIt);
            } else {
                ++missileIt;
            }
        }
        
        for (auto& enemy : enemies) {
            auto& bullets = enemy->getMissiles();

            for (auto bulletIt = bullets.begin(); bulletIt != bullets.end(); ) {
                CMissile* bullet = *bulletIt;
                bool shouldDelete = false;

                // 1. 先檢查護盾碰撞
                for (int i = 0; i < 12; i++) {
                    float distShield = glm::distance(bullet->getPos(), g_quad_shild[i].getPos());
                    if (distShield < 0.5f) {
                        shouldDelete = true;
                        break;
                    }
                }

                // 2. 如果沒有撞護盾，再檢查玩家碰撞
                if (!shouldDelete) {
                    float dist = glm::distance(bullet->getPos(), player->getPos());
                    if (dist < 0.5f) {
                        player->onHit(1);
                        shouldDelete = true;
                    }
                }

                // 3. 根據結果處理子彈
                if (shouldDelete) {
                    delete bullet;
                    bulletIt = bullets.erase(bulletIt);
                } else {
                    ++bulletIt;
                }
            }
        }


    }

}

// MARK: - release
void releaseAll()
{
    if(star != nullptr) delete [] star;
    if(g_quad_shild != nullptr) delete [] g_quad_shild;
    
    if(player != nullptr) delete player;
    
    for (auto e : enemies) {
        if (e != nullptr) delete e;
    }
    enemies.clear();
    
    for (auto e : hearts) {
        if (e != nullptr) delete e;
    }
    hearts.clear();
}


void spawnEnemy() {
    if (enemies.size() >= MAX_ENEMIES) return;
    int type = rand() % 3; // 假設三種敵人類型

    CEnemy* newEnemy = nullptr;
    switch (type) {
        case 0:
            newEnemy = new EnemyA();
            if (newEnemy) {
                newEnemy->setShaderID(g_shaderProg);
                newEnemy->setPos(glm::vec3(rand() % 8 - 4, 4.0f, 0.0f)); // 頂端隨機生成
                newEnemy->setColor(glm::vec3(0.8f, 0.8f, 0.1f));
                newEnemy->setScale(glm::vec3(0.6f));
                enemies.push_back(newEnemy);
            }
            
            break;
        case 1:
            newEnemy = new EnemyB();  //你需要先定義 EnemyB 類別
            if (newEnemy) {
                newEnemy->setShaderID(g_shaderProg);
                newEnemy->setPos(glm::vec3(rand() % 8 - 4, 4.0f, 0.0f)); // 頂端隨機生成
                newEnemy->setColor(glm::vec3(0.0f, 0.6f, 0.2f));
                newEnemy->setScale(glm::vec3(0.9f));
                enemies.push_back(newEnemy);
            }
            break;
        case 2:
//            newEnemy = new BossEnemy(); // 同理定義 BossEnemy
            newEnemy = new EnemyC();  //你需要先定義 EnemyB 類別
            if (newEnemy) {
                newEnemy->setShaderID(g_shaderProg);
                newEnemy->setPos(glm::vec3(rand() % 8 - 4, 4.0f, 0.0f)); // 頂端隨機生成
                newEnemy->setColor(glm::vec3(0.8f, 0.3f, 0.2f));
                newEnemy->setScale(glm::vec3(0.9f));
                enemies.push_back(newEnemy);
            }
            break;
    }

    
}

int main()
{
    // ------- 檢查與建立視窗  ---------------
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

//     設定 OpenGL 版本與 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //只啟用 OpenGL 3.3 Core Profile（不包含舊版 OpenGL 功能）
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // 禁止視窗大小改變
    
    // 建立 OpenGL 視窗與該視窗執行時所需的的狀態、資源和環境(context 上下文)
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Mouse and Keyboard Events", nullptr, nullptr);
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
