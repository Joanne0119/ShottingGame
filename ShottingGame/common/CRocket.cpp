//
//  CRocket.cpp
//  ShottingGame
//
//  Created by 劉丞恩 on 2025/3/26.
//
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include "CRocket.h"
#include "initshader.h"

CRocket::CRocket() {
    _vao = 0; _vbo = 0; _ebo = 0;
    _shaderProg = 0;
    _scale = glm::vec3(1.0f, 1.0f, 1.0f);
    _color = glm::vec3(1.0f, 1.0f, 1.0f);
    _pos = glm::vec3(0.0f, 0.0f, 0.0f);
    _rotX = 0.0f; _rotY = 0.0f; _rotZ = 0.0f;
    _rotAxis = 0;
    _bRotation = _bScale = _bPos = _bTransform = _bOnTransform = false;
    _mxScale = glm::mat4(1.0f);
    _mxPos = glm::mat4(1.0f);
    _mxRotX = glm::mat4(1.0f);
    _mxRotY = glm::mat4(1.0f);
    _mxRotZ = glm::mat4(1.0f);
    _mxRotation = glm::mat4(1.0f);
    _mxTRS = glm::mat4(1.0f);
    _mxTransform = glm::mat4(1.0f);
    _mxFinal = glm::mat4(1.0f);
    _points = new GLfloat[ROCKET_VTX_COUNT * ROCKET_VTX_ATTR_COUNT] {
        // 位置            // 顏色            // 法向量       // 貼圖座標
        0.0f,  0.6f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 鼻錐頂點
        -0.3f,  0.3f, 0.0f,  0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // 左上機身
        0.3f,  0.3f, 0.0f,  0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 右上機身
        -0.4f, -0.3f, 0.0f,  0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // 左下機身
        0.4f, -0.3f, 0.0f,  0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // 右下機身
        -0.2f, -0.35f, 0.0f,  0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // 左尾翼
        0.2f, -0.35f, 0.0f,  0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // 右尾翼
        -0.3f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // 左火焰
        0.3f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // 右火焰
        0.0f, -0.7f, 0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f // 火焰
    };
    _idx = new GLuint[ROCKET_INDEX_COUNT]{
        0, 1, 2,  // 鼻錐三角形
        1, 3, 4,  // 機身左下部分
        2, 4, 1,  // 機身右下部分
        3, 5, 6,  // 左尾翼
        4, 6, 3,   // 右尾翼
        5, 7, 8,   // 左火焰
        6, 8, 5,    // 右火焰
        7, 9, 8
    };

    setupVertexAttributes();
}

CRocket::~CRocket() {
    glDeleteBuffers(1, &_vbo);  //•˝ƒ¿©Ò VBO ªP EBO
    glDeleteBuffers(1, &_ebo);
    glDeleteVertexArrays(1, &_vao); //¶Aƒ¿©Ò VAO
    glDeleteProgram(_shaderProg);  //ƒ¿©Ò shader program
    if (_points != NULL) delete[] _points;
    if (_idx != NULL) delete[] _idx;
}

void CRocket::setupVertexAttributes()
{
    // ≥]©w VAO°BVBO ªP EBO
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(_vao);

    // ≥]©w VBO
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, ROCKET_VTX_COUNT * ROCKET_VTX_ATTR_COUNT * sizeof(_points), _points, GL_STATIC_DRAW);

    // ≥]©w EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ROCKET_INDEX_COUNT * sizeof(GLuint), _idx, GL_STATIC_DRAW);

    // ¶Ï∏mƒ›©
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, ROCKET_VTX_ATTR_COUNT * sizeof(float), BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    // √C¶‚ƒ›©
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, ROCKET_VTX_ATTR_COUNT * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //™k¶V∂qƒ›©
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, ROCKET_VTX_ATTR_COUNT * sizeof(float), BUFFER_OFFSET(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //∂KπœÆyº–ƒ›©
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, ROCKET_VTX_ATTR_COUNT * sizeof(float), BUFFER_OFFSET(9 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glBindVertexArray(0); // ∏—∞£πÔ VAO ™∫∏j©w
}

GLuint CRocket::setShader(const char* vshader, const  char* fshader)
{
    _shaderProg = createShader(vshader, fshader);
    glUseProgram(_shaderProg);
    _modelMxLoc = glGetUniformLocation(_shaderProg, "mxModel");     // ®˙±o MVP ≈‹º∆™∫¶Ï∏m
    glUniformMatrix4fv(_modelMxLoc, 1, GL_FALSE, glm::value_ptr(_mxTRS));
    return _shaderProg;
}

void CRocket::setShaderID(GLuint shaderID)
{
    _shaderProg = shaderID;
    glUseProgram(_shaderProg);
    _modelMxLoc = glGetUniformLocation(_shaderProg, "mxModel");     // ®˙±o MVP ≈‹º∆™∫¶Ï∏m
    glUniformMatrix4fv(_modelMxLoc, 1, GL_FALSE, glm::value_ptr(_mxTRS));
}

void CRocket::setColor(glm::vec3 vColor)
{
    _color = vColor;
    for (int i = 0; i < ROCKET_VTX_COUNT; i++) {
        _points[i * ROCKET_VTX_ATTR_COUNT + COLOR_OFFSET ]  = _color.x;
        _points[i * ROCKET_VTX_ATTR_COUNT + COLOR_OFFSET+1] = _color.y;
        _points[i * ROCKET_VTX_ATTR_COUNT + COLOR_OFFSET+2] = _color.z;
    }
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, ROCKET_VTX_COUNT * ROCKET_VTX_ATTR_COUNT * sizeof(_points), _points, GL_STATIC_DRAW);
}

void CRocket::draw()
{
    updateMatrix();
    glUseProgram(_shaderProg);
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, ROCKET_INDEX_COUNT, GL_UNSIGNED_INT, 0);
}

void CRocket::setScale(glm::vec3 vScale)
{
    _scale = vScale;
    _bScale = true;
    _mxScale = glm::scale(glm::mat4(1.0f), _scale);
}

void CRocket::setPos(glm::vec3 vPt)
{
    _pos = vPt;
    _bPos = true;
    _mxPos = glm::translate(glm::mat4(1.0f), _pos);
}

void CRocket::setRotX(float angle)
{
    _rotX = glm::radians(angle);
    _rotAxis = _rotAxis | 1;
    _mxRotX = glm::rotate(glm::mat4(1.0f), _rotX, glm::vec3(1.0f, 0.0f, 0.0f));
    _mxRotation = _mxRotX;
    _bRotation = true;
}

void CRocket::setRotY(float angle)
{
    _rotY = glm::radians(angle);
    _rotAxis = _rotAxis | 2;
    _mxRotY = glm::rotate(glm::mat4(1.0f), _rotY, glm::vec3(0.0f, 1.0f, 0.0f));
    if (_rotAxis & 1)_mxRotation = _mxRotY * _mxRotX;
    else _mxRotation = _mxRotY;
    _bRotation = true;
}

void CRocket::setRotZ(float angle)
{
    _rotZ = glm::radians(angle);
    _mxRotZ = glm::rotate(glm::mat4(1.0f), _rotZ, glm::vec3(0.0f, 0.0f, 1.0f));
    if (_rotAxis == 1) _mxRotation = _mxRotZ * _mxRotX;
    else if (_rotAxis == 2) _mxRotation = _mxRotZ * _mxRotY;
    else if (_rotAxis == 3) _mxRotation = _mxRotZ * _mxRotY * _mxRotX;
    else _mxRotation = _mxRotZ;
    _bRotation = true;
}

void CRocket::updateMatrix()
{
    if (_bScale || _bPos || _bRotation)
    {
        if (_bOnTransform == false) {
            _mxFinal = _mxTRS = _mxPos * _mxRotation * _mxScale;
        }
        else {
            _mxTRS = _mxPos * _mxRotation * _mxScale;
            _mxFinal = _mxTransform * _mxTRS;
        }
        _bScale = _bPos = _bRotation = false;
    }
    if (_bTransform) {
        _mxFinal = _mxTransform * _mxTRS;
        _bTransform = false;
    }
    // ¶p¶h≠”º“´¨®œ•Œ¨€¶P™∫ shader program,¶]®C§@≠”º“´¨™∫ mxTRS ≥£§£¶P°A©“•H®C≠”frame≥£≠nßÛ∑s
    glUniformMatrix4fv(_modelMxLoc, 1, GL_FALSE, glm::value_ptr(_mxFinal));
}

void CRocket::setTransformMatrix(glm::mat4 mxMatrix)
{
    _bOnTransform = _bTransform = true;
    _mxTransform = mxMatrix;
}
glm::mat4 CRocket::getModelMatrix() { return _mxFinal; }

GLuint CRocket::getShaderProgram() { return _shaderProg; }

void CRocket::reset()
{
    _scale = glm::vec3(1.0f, 1.0f, 1.0f);
    _color = glm::vec3(1.0f, 1.0f, 1.0f);
    _pos = glm::vec3(0.0f, 0.0f, 0.0f);
    _rotX = 0.0f; _rotY = 0.0f; _rotZ = 0.0f;
    _rotAxis = 0;
    _bRotation = _bScale = _bPos = _bTransform = _bOnTransform = false;
    _mxScale = glm::mat4(1.0f);
    _mxPos = glm::mat4(1.0f);
    _mxTRS = glm::mat4(1.0f);
    _mxRotX = glm::mat4(1.0f);
    _mxRotY = glm::mat4(1.0f);
    _mxRotZ = glm::mat4(1.0f);
    _mxRotation = glm::mat4(1.0f);
    _mxTransform = glm::mat4(1.0f);
    _mxFinal = glm::mat4(1.0f);
}

