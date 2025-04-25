#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#include "CStar.h"
#include "initshader.h"

Star::Star()
{
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
    _speed = 0.3f;
    _points = new GLfloat[STAR_VTX_COUNT * STAR_VTX_ATTR_COUNT] {
        // ¶Ï∏m            // √C¶‚         // ™k¶V∂q       // ∂KπœÆyº–
        -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // •™§U
        -0.1f, -0.1f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // •™§U
         0.0f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // •k§U
         0.1f, -0.1f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // •k§U
         0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // •k§W
         0.1f,  0.1f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // •k§W
         0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // •™§W
        -0.1f,  0.1f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f  // •™§W
    };
    _idx = new GLuint[STAR_INDEX_COUNT]{ 0, 1, 7, 1 ,2 ,3, 3, 4, 5, 5, 6, 7, 7, 3, 5, 7, 1, 3 };

    setupVertexAttributes();
}

Star::~Star()
{
    glDeleteBuffers(1, &_vbo);  //•˝ƒ¿©Ò VBO ªP EBO
    glDeleteBuffers(1, &_ebo);
    glDeleteVertexArrays(1, &_vao); //¶Aƒ¿©Ò VAO
    glDeleteProgram(_shaderProg);  //ƒ¿©Ò shader program
    if (_points != NULL) delete[] _points;
    if (_idx != NULL) delete[] _idx;
}

void Star::setupVertexAttributes()
{
    // ≥]©w VAO°BVBO ªP EBO
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(_vao);

    // ≥]©w VBO
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, STAR_VTX_COUNT * STAR_VTX_ATTR_COUNT * sizeof(_points), _points, GL_STATIC_DRAW);

    // ≥]©w EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, STAR_INDEX_COUNT * sizeof(GLuint), _idx, GL_STATIC_DRAW);

    // ¶Ï∏mƒ›©
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STAR_VTX_ATTR_COUNT * sizeof(float), BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    // √C¶‚ƒ›©
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, STAR_VTX_ATTR_COUNT * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //™k¶V∂qƒ›©
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, STAR_VTX_ATTR_COUNT * sizeof(float), BUFFER_OFFSET(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //∂KπœÆyº–ƒ›©
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, STAR_VTX_ATTR_COUNT * sizeof(float), BUFFER_OFFSET(9 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glBindVertexArray(0); // ∏—∞£πÔ VAO ™∫∏j©w
}

GLuint Star::setShader(const char* vshader, const  char* fshader)
{
    _shaderProg = createShader(vshader, fshader);
    glUseProgram(_shaderProg);
    _modelMxLoc = glGetUniformLocation(_shaderProg, "mxModel");     // ®˙±o MVP ≈‹º∆™∫¶Ï∏m
    glUniformMatrix4fv(_modelMxLoc, 1, GL_FALSE, glm::value_ptr(_mxTRS));
    return _shaderProg;
}

void Star::setShaderID(GLuint shaderID)
{
    _shaderProg = shaderID;
    glUseProgram(_shaderProg);
    _modelMxLoc = glGetUniformLocation(_shaderProg, "mxModel");     // ®˙±o MVP ≈‹º∆™∫¶Ï∏m
    glUniformMatrix4fv(_modelMxLoc, 1, GL_FALSE, glm::value_ptr(_mxTRS));
}

void Star::setColor(glm::vec3 vColor)
{
    _color = vColor;
    for (int i = 0; i < STAR_VTX_COUNT; i++) {
        _points[i * STAR_VTX_ATTR_COUNT + COLOR_OFFSET ]  = _color.x;
        _points[i * STAR_VTX_ATTR_COUNT + COLOR_OFFSET+1] = _color.y;
        _points[i * STAR_VTX_ATTR_COUNT + COLOR_OFFSET+2] = _color.z;
    }
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, STAR_VTX_COUNT * STAR_VTX_ATTR_COUNT * sizeof(_points), _points, GL_STATIC_DRAW);
}

void Star::draw()
{
    updateMatrix();
    glUseProgram(_shaderProg);
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, STAR_INDEX_COUNT, GL_UNSIGNED_INT, 0);
}

void Star::update(float dt){
    _pos.y -= _speed * dt;  // 星星向下移動，模擬星空流動的效果
    if (_pos.y < -50.0f) {
        _pos.y = 5.0f;  // 當星星離開螢幕時，重新出現在上方
        _pos.x = rand() % 20 - 10;  // 隨機X位置
    }
    
    setPos(_pos);  // 更新星星位置
}

void Star::setScale(glm::vec3 vScale)
{
    _scale = vScale;
    _bScale = true;
    _mxScale = glm::scale(glm::mat4(1.0f), _scale);
}

void Star::setPos(glm::vec3 vPt)
{
    _pos = vPt;
    _bPos = true;
    _mxPos = glm::translate(glm::mat4(1.0f), _pos);
}

void Star::setRotX(float angle)
{
    _rotX = glm::radians(angle);
    _rotAxis = _rotAxis | 1;
    _mxRotX = glm::rotate(glm::mat4(1.0f), _rotX, glm::vec3(1.0f, 0.0f, 0.0f));
    _mxRotation = _mxRotX;
    _bRotation = true;
}

void Star::setRotY(float angle)
{
    _rotY = glm::radians(angle);
    _rotAxis = _rotAxis | 2;
    _mxRotY = glm::rotate(glm::mat4(1.0f), _rotY, glm::vec3(0.0f, 1.0f, 0.0f));
    if( _rotAxis & 1 )_mxRotation = _mxRotY * _mxRotX;  // ¶≥ X ∂b™∫±€¬‡∂q
    else _mxRotation = _mxRotY ;
    _bRotation = true;
}

void Star::setRotZ(float angle)
{
    _rotZ = glm::radians(angle);
    _mxRotZ = glm::rotate(glm::mat4(1.0f), _rotZ, glm::vec3(0.0f, 0.0f, 1.0f));
    if ( _rotAxis == 1 ) _mxRotation = _mxRotZ * _mxRotX; // •u¶≥ X ∂b™∫±€¬‡∂q
    else if ( _rotAxis == 2 ) _mxRotation = _mxRotZ * _mxRotY; // •u¶≥ Y ∂b™∫±€¬‡∂q
    else if ( _rotAxis == 3 ) _mxRotation = _mxRotZ * _mxRotY * _mxRotX; // ¶≥ X°BY ∂b™∫±€¬‡∂q
    else _mxRotation = _mxRotZ;
    _bRotation = true;
}

void Star::updateMatrix()
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

void Star::setTransformMatrix(glm::mat4 mxMatrix)
{
    _bOnTransform = _bTransform = true;
    _mxTransform = mxMatrix;
}

glm::mat4 Star::getModelMatrix() { return _mxFinal; }

GLuint Star::getShaderProgram() { return _shaderProg; }

void Star::reset()
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
