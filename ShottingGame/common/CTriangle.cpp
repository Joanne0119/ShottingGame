#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "CTriangle.h"
#include "initshader.h"

CTriangle::CTriangle()
{
	_vao = 0; _vbo = 0; _ebo = 0; _shaderProg = 0;
	_scale = glm::vec3(1.0f, 1.0f, 1.0f);
	_color = glm::vec3(1.0f, 1.0f, 1.0f);
	_pos = glm::vec3(0.0f, 0.0f, 0.0f);
	_rotX = 0.0f; _rotY = 0.0f; _rotZ = 0.0f; _rotAxis = 0;
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

	_points = new GLfloat[TRI_VTX_COUNT * TRI_VTX_ATTR_COUNT]{
		// 位置            // 顏色         // 法向量       // 貼圖座標
		0.0f, 0.57735f,   0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f, // 上頂點
	   -0.5f, -0.288675f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下角
		0.5f, -0.288675f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f // 右下角
	};
	_idx = new GLuint[TRI_INDEX_COUNT]{ 0, 1, 2 };

	setupVertexAttributes();
}

CTriangle::~CTriangle()
{
	glDeleteBuffers(1, &_vbo);  //先釋放 VBO 與 EBO
	glDeleteBuffers(1, &_ebo);
	glDeleteVertexArrays(1, &_vao); //再釋放 VAO
	glDeleteProgram(_shaderProg);  //釋放 shader program
	if (_points != NULL) delete[] _points;
	if (_idx != NULL) delete[] _idx;
}

void CTriangle::setupVertexAttributes()
{
	// 設定 VAO、VBO 與 EBO
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(_vao);

	// 設定 VBO
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, TRI_VTX_COUNT * TRI_VTX_ATTR_COUNT * sizeof(_points), _points, GL_STATIC_DRAW);

	// 設定 EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, TRI_INDEX_COUNT * sizeof(GLuint), _idx, GL_STATIC_DRAW);

	// 位置屬性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, TRI_VTX_ATTR_COUNT * sizeof(float), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	// 顏色屬性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, TRI_VTX_ATTR_COUNT * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//法向量屬性
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, TRI_VTX_ATTR_COUNT * sizeof(float), BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//貼圖座標屬性
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, TRI_VTX_ATTR_COUNT * sizeof(float), BUFFER_OFFSET(9 * sizeof(float)));
	glEnableVertexAttribArray(3);
	glBindVertexArray(0); // 解除對 VAO 的綁定
}

GLuint CTriangle::setShader(const char* vshader, const  char* fshader)
{
	_shaderProg = createShader(vshader, fshader);
	glUseProgram(_shaderProg);
	_modelMxLoc = glGetUniformLocation(_shaderProg, "mxModel"); 	// 取得 MVP 變數的位置
	glUniformMatrix4fv(_modelMxLoc, 1, GL_FALSE, glm::value_ptr(_mxTRS));
	return _shaderProg;
}

void CTriangle::setShaderID(GLuint shaderID)
{
	_shaderProg = shaderID;
	glUseProgram(_shaderProg);
	_modelMxLoc = glGetUniformLocation(_shaderProg, "mxModel"); 	// 取得 MVP 變數的位置
	glUniformMatrix4fv(_modelMxLoc, 1, GL_FALSE, glm::value_ptr(_mxTRS));
}

void CTriangle::setColor(glm::vec3 vColor)
{
	_color = vColor;
	for (int i = 0; i < TRI_VTX_COUNT; i++) {
		_points[i * TRI_VTX_ATTR_COUNT + COLOR_OFFSET ]  = _color.x;
		_points[i * TRI_VTX_ATTR_COUNT + COLOR_OFFSET+1] = _color.y;
		_points[i * TRI_VTX_ATTR_COUNT + COLOR_OFFSET+2] = _color.z;
	}
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, TRI_VTX_COUNT * TRI_VTX_ATTR_COUNT * sizeof(_points), _points, GL_STATIC_DRAW);
}

void CTriangle::draw()
{
	updateMatrix();
	glUseProgram(_shaderProg);
	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, TRI_INDEX_COUNT, GL_UNSIGNED_INT, 0);
}

void CTriangle::setScale(glm::vec3 vScale)
{
	_scale = vScale;
	_bScale = true;
	_mxScale = glm::scale(glm::mat4(1.0f), _scale);
}

void CTriangle::setPos(glm::vec3 vPt)
{
	_pos = vPt;
	_bPos = true;
	_mxPos = glm::translate(glm::mat4(1.0f), _pos);
}
void CTriangle::setRotX(float angle)
{
	_rotX = glm::radians(angle);
	_rotAxis = _rotAxis | 1;
	_mxRotX = glm::rotate(glm::mat4(1.0f), _rotX, glm::vec3(1.0f, 0.0f, 0.0f));
	_mxRotation = _mxRotX;
	_bRotation = true;
}

void CTriangle::setRotY(float angle)
{
	_rotY = glm::radians(angle);
	_rotAxis = _rotAxis | 2;
	_mxRotY = glm::rotate(glm::mat4(1.0f), _rotY, glm::vec3(0.0f, 1.0f, 0.0f));
	if (_rotAxis & 1)_mxRotation = _mxRotY * _mxRotX;
	else _mxRotation = _mxRotY;
	_bRotation = true;
}

void CTriangle::setRotZ(float angle)
{
	_rotZ = glm::radians(angle);
	_mxRotZ = glm::rotate(glm::mat4(1.0f), _rotZ, glm::vec3(0.0f, 0.0f, 1.0f));
	if (_rotAxis == 1) _mxRotation = _mxRotZ * _mxRotX;
	else if (_rotAxis == 2) _mxRotation = _mxRotZ * _mxRotY;
	else if (_rotAxis == 3) _mxRotation = _mxRotZ * _mxRotY * _mxRotX;
	else _mxRotation = _mxRotZ;
	_bRotation = true;
}

void CTriangle::updateMatrix()
{
	if (_bScale || _bPos || _bRotation)
	{
		if (_bOnTransform == false) {
			_mxFinal = _mxTRS = _mxPos * _mxRotation * _mxScale;
		}
		else{
			_mxTRS = _mxPos * _mxRotation * _mxScale;
			_mxFinal = _mxTransform * _mxTRS;
		}
		_bScale = _bPos = _bRotation = false;
	}
	if ( _bTransform ) {
		_mxFinal = _mxTransform * _mxTRS;
		_bTransform = false;
	}
	// 如多個模型使用相同的 shader program,因每一個模型的 mxTRS 都不同，所以每個frame都要更新
	glUniformMatrix4fv(_modelMxLoc, 1, GL_FALSE, glm::value_ptr(_mxFinal));
}

void CTriangle::setTransformMatrix(glm::mat4 mxMatrix)
{
	_bOnTransform = _bTransform = true;
	_mxTransform = mxMatrix;
}
glm::mat4 CTriangle::getModelMatrix() { return _mxFinal; }

GLuint CTriangle::getShaderProgram() { return _shaderProg; }

glm::vec3 CTriangle::getPos() const {
    return _pos;
}

void CTriangle::reset()
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
