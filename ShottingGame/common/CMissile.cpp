#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "CMissile.h"
#include "initshader.h"

CMissile::CMissile()
{
    _vao = 0; _vbo = 0; _ebo = 0;
    _shaderProg = 0;
    _scale = glm::vec3(1.0f, 1.0f, 1.0f);
    _color = glm::vec3(1.0f, 1.0f, 1.0f);
    _pos = glm::vec3(0.0f, 0.0f, 0.0f);
    _rotX = 0.0f; _rotY = 0.0f; _rotZ = 0.0f;
    _rotAxis = 0;
    _speed = 0.1f;
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
    _points = new GLfloat[MISSILE_VTX_COUNT * MISSILE_VTX_ATTR_COUNT] { // don't touch
        // ¶Ï∏m            // √C¶‚         // ™k¶V∂q       // ∂KπœÆyº–
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // •™§U
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // •k§U
         0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // •k§W
         0.0f,  0.75f,0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f  // •™§W
    };
    _idx = new GLuint[MISSILE_INDEX_COUNT]{ 0, 1, 2, 2, 4, 0, 2, 3, 4 };

    setupVertexAttributes();
}
CMissile::CMissile(glm::vec3 pos, GLfloat speed) : CMissile() {
    _pos = pos;
    _speed = speed;
}
CMissile::~CMissile()
{
    glDeleteBuffers(1, &_vbo);  //•˝ƒ¿©Ò VBO ªP EBO
    glDeleteBuffers(1, &_ebo);
    glDeleteVertexArrays(1, &_vao); //¶Aƒ¿©Ò VAO
    glDeleteProgram(_shaderProg);  //ƒ¿©Ò shader program
    if (_points != NULL) delete[] _points;
    if (_idx != NULL) delete[] _idx;
}

bool CMissile::isOutOfBounds()
{
    return (_pos.y > 8.0f) || _pos.y < -8.0f;
}

void CMissile::setupVertexAttributes()
{
    // ≥]©w VAO°BVBO ªP EBO
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(_vao);

    // ≥]©w VBO
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, MISSILE_VTX_COUNT * MISSILE_VTX_ATTR_COUNT * sizeof(_points), _points, GL_STATIC_DRAW);

    // ≥]©w EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, MISSILE_INDEX_COUNT * sizeof(GLuint), _idx, GL_STATIC_DRAW);

    // ¶Ï∏mƒ›©
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, MISSILE_VTX_ATTR_COUNT * sizeof(float), BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    // √C¶‚ƒ›©
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, MISSILE_VTX_ATTR_COUNT * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //™k¶V∂qƒ›©
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, MISSILE_VTX_ATTR_COUNT * sizeof(float), BUFFER_OFFSET(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //∂KπœÆyº–ƒ›©
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, MISSILE_VTX_ATTR_COUNT * sizeof(float), BUFFER_OFFSET(9 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glBindVertexArray(0); // ∏—∞£πÔ VAO ™∫∏j©w
}

GLuint CMissile::setShader(const char* vshader, const  char* fshader)
{
    _shaderProg = createShader(vshader, fshader);
    glUseProgram(_shaderProg);
    _modelMxLoc = glGetUniformLocation(_shaderProg, "mxModel");     // ®˙±o MVP ≈‹º∆™∫¶Ï∏m
    glUniformMatrix4fv(_modelMxLoc, 1, GL_FALSE, glm::value_ptr(_mxTRS));
    return _shaderProg;
}

void CMissile::setShaderID(GLuint shaderID)
{
    _shaderProg = shaderID;
    glUseProgram(_shaderProg);
    _modelMxLoc = glGetUniformLocation(_shaderProg, "mxModel");     // ®˙±o MVP ≈‹º∆™∫¶Ï∏m
    glUniformMatrix4fv(_modelMxLoc, 1, GL_FALSE, glm::value_ptr(_mxTRS));
}

void CMissile::setColor(glm::vec3 vColor)
{
    _color = vColor;
    for (int i = 0; i < MISSILE_VTX_COUNT; i++) {
        _points[i * MISSILE_VTX_ATTR_COUNT + COLOR_OFFSET ]  = _color.x;
        _points[i * MISSILE_VTX_ATTR_COUNT + COLOR_OFFSET+1] = _color.y;
        _points[i * MISSILE_VTX_ATTR_COUNT + COLOR_OFFSET+2] = _color.z;
    }
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, MISSILE_VTX_COUNT * MISSILE_VTX_ATTR_COUNT * sizeof(_points), _points, GL_STATIC_DRAW);
}

void CMissile::draw()
{
    updateMatrix();
    glUseProgram(_shaderProg);
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, MISSILE_INDEX_COUNT, GL_UNSIGNED_INT, 0);
}

void CMissile::update(float dt)
{
    _pos += glm::vec3(0.0f, _speed * dt, 0.0f); // 假設 speed 是每 frame 的速度
    setPos(_pos);
}

void CMissile::updateEnemy(float dt)
{
    _pos -= glm::vec3(0.0f, _speed * dt, 0.0f); // 假設 speed 是每 frame 的速度
    setPos(_pos);
}

void CMissile::setScale(glm::vec3 vScale)
{
    _scale = vScale;
    _bScale = true;
    _mxScale = glm::scale(glm::mat4(1.0f), _scale);
}

void CMissile::setPos(glm::vec3 vPt)
{
    _pos = vPt;
    _bPos = true;
    _mxPos = glm::translate(glm::mat4(1.0f), _pos);
}

void CMissile::setRotX(float angle)
{
    _rotX = glm::radians(angle);
    _rotAxis = _rotAxis | 1;
    _mxRotX = glm::rotate(glm::mat4(1.0f), _rotX, glm::vec3(1.0f, 0.0f, 0.0f));
    _mxRotation = _mxRotX;
    _bRotation = true;
}

void CMissile::setRotY(float angle)
{
    _rotY = glm::radians(angle);
    _rotAxis = _rotAxis | 2;
    _mxRotY = glm::rotate(glm::mat4(1.0f), _rotY, glm::vec3(0.0f, 1.0f, 0.0f));
    if (_rotAxis & 1)_mxRotation = _mxRotY * _mxRotX;
    else _mxRotation = _mxRotY;
    _bRotation = true;
}

void CMissile::setRotZ(float angle)
{
    _rotZ = glm::radians(angle);
    _mxRotZ = glm::rotate(glm::mat4(1.0f), _rotZ, glm::vec3(0.0f, 0.0f, 1.0f));
    if (_rotAxis == 1) _mxRotation = _mxRotZ * _mxRotX;
    else if (_rotAxis == 2) _mxRotation = _mxRotZ * _mxRotY;
    else if (_rotAxis == 3) _mxRotation = _mxRotZ * _mxRotY * _mxRotX;
    else _mxRotation = _mxRotZ;
    _bRotation = true;
}

void CMissile::updateMatrix()
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

void CMissile::setTransformMatrix(glm::mat4 mxMatrix)
{
    _bOnTransform = _bTransform = true;
    _mxTransform = mxMatrix;
}
glm::mat4 CMissile::getModelMatrix() { return _mxFinal; }

GLuint CMissile::getShaderProgram() { return _shaderProg; }

glm::vec3 CMissile::getPos() const {
    std::cout << "missile (" << _pos.x << ',' << _pos.y << ',' << _pos.z << ')' << std::endl;
    return _pos;
}

void CMissile::reset()
{
    _scale = glm::vec3(1.0f, 1.0f, 1.0f);
    _color = glm::vec3(1.0f, 1.0f, 1.0f);
    _pos = glm::vec3(0.0f, 0.0f, 0.0f);
    _rotX = 0.0f; _rotY = 0.0f; _rotZ = 0.0f;
    _rotAxis = 0;
    _speed = 0.1f;
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
