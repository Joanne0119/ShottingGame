#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "CHeart.h"
#include "initshader.h"

CHeart::CHeart()
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

    _points = new GLfloat[HEART_VTX_COUNT * HEART_VTX_ATTR_COUNT] {
        -0.2f,   0.2f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f,
        0.0f,  -0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.794f, 0.096f,
        0.2f, 0.2f,   0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.975f, 0.654f,
       0.4f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.206f, 0.096f,
       0.0f, -0.3f,   0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.025f, 0.654f,
        -0.4f, 0.0f,   0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.025f, 0.654f
    };
    _idx = new GLuint[HEART_INDEX_COUNT]{ 0, 1, 5, 1 ,2 ,3, 5, 3, 4 };

    setupVertexAttributes();
}

CHeart::~CHeart()
{
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ebo);
    glDeleteVertexArrays(1, &_vao);
    glDeleteProgram(_shaderProg);
    if (_points != NULL) delete[] _points;
    if (_idx != NULL) delete[] _idx;
}

void CHeart::setupVertexAttributes()
{
    
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(_vao);

    
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, HEART_VTX_COUNT * HEART_VTX_ATTR_COUNT * sizeof(_points), _points, GL_STATIC_DRAW);

    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, HEART_INDEX_COUNT * sizeof(GLuint), _idx, GL_STATIC_DRAW);

    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, HEART_VTX_ATTR_COUNT * sizeof(float), BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

   
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, HEART_VTX_ATTR_COUNT * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, HEART_VTX_ATTR_COUNT * sizeof(float), BUFFER_OFFSET(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, HEART_VTX_ATTR_COUNT * sizeof(float), BUFFER_OFFSET(9 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glBindVertexArray(0);
}

GLuint CHeart::setShader(const char* vshader, const  char* fshader)
{
    _shaderProg = createShader(vshader, fshader);
    glUseProgram(_shaderProg);
    _modelMxLoc = glGetUniformLocation(_shaderProg, "mxModel");
    glUniformMatrix4fv(_modelMxLoc, 1, GL_FALSE, glm::value_ptr(_mxTRS));
    return _shaderProg;
}

void CHeart::setShaderID(GLuint shaderID)
{
    _shaderProg = shaderID;
    glUseProgram(_shaderProg);
    _modelMxLoc = glGetUniformLocation(_shaderProg, "mxModel");
    glUniformMatrix4fv(_modelMxLoc, 1, GL_FALSE, glm::value_ptr(_mxTRS));
}

void CHeart::setColor(glm::vec3 vColor)
{
    _color = vColor;
    for (int i = 0; i < HEART_VTX_COUNT; i++) {
        _points[i * HEART_VTX_ATTR_COUNT + COLOR_OFFSET ]  = _color.x;
        _points[i * HEART_VTX_ATTR_COUNT + COLOR_OFFSET+1] = _color.y;
        _points[i * HEART_VTX_ATTR_COUNT + COLOR_OFFSET+2] = _color.z;
    }
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, HEART_VTX_COUNT * HEART_VTX_ATTR_COUNT * sizeof(_points), _points, GL_STATIC_DRAW);
}

void CHeart::draw()
{
    updateMatrix();
    glUseProgram(_shaderProg);
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, HEART_INDEX_COUNT, GL_UNSIGNED_INT, 0);
}

void CHeart::setScale(glm::vec3 vScale)
{
    _scale = vScale;
    _bScale = true;
    _mxScale = glm::scale(glm::mat4(1.0f), _scale);
}

void CHeart::setPos(glm::vec3 vPt)
{
    _pos = vPt;
    _bPos = true;
    _mxPos = glm::translate(glm::mat4(1.0f), _pos);
}

void CHeart::setRotX(float angle)
{
    _rotX = glm::radians(angle);
    _rotAxis = _rotAxis | 1;
    _mxRotX = glm::rotate(glm::mat4(1.0f), _rotX, glm::vec3(1.0f, 0.0f, 0.0f));
    _mxRotation = _mxRotX;
    _bRotation = true;
}

void CHeart::setRotY(float angle)
{
    _rotY = glm::radians(angle);
    _rotAxis = _rotAxis | 2;
    _mxRotY = glm::rotate(glm::mat4(1.0f), _rotY, glm::vec3(0.0f, 1.0f, 0.0f));
    if( _rotAxis & 1 )_mxRotation = _mxRotY * _mxRotX; 
    else _mxRotation = _mxRotY ;
    _bRotation = true;
}

void CHeart::setRotZ(float angle)
{
    _rotZ = glm::radians(angle);
    _mxRotZ = glm::rotate(glm::mat4(1.0f), _rotZ, glm::vec3(0.0f, 0.0f, 1.0f));
    if ( _rotAxis == 1 ) _mxRotation = _mxRotZ * _mxRotX;
    else if ( _rotAxis == 2 ) _mxRotation = _mxRotZ * _mxRotY;
    else if ( _rotAxis == 3 ) _mxRotation = _mxRotZ * _mxRotY * _mxRotX;
    else _mxRotation = _mxRotZ;
    _bRotation = true;
}

void CHeart::updateMatrix()
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
    
    glUniformMatrix4fv(_modelMxLoc, 1, GL_FALSE, glm::value_ptr(_mxFinal));
}

void CHeart::setTransformMatrix(glm::mat4 mxMatrix)
{
    _bOnTransform = _bTransform = true;
    _mxTransform = mxMatrix;
}

glm::mat4 CHeart::getModelMatrix() { return _mxFinal; }

GLuint CHeart::getShaderProgram() { return _shaderProg; }

void CHeart::reset()
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
