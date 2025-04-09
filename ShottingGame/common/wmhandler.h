#pragma once
#include <GLFW/glfw3.h>

// �ƹ����s���U�� callback function(�^�I�禡)
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

// �ƹ��b���w�����W���ʮɪ� callback function(�^�I�禡)
void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);

// �������j�p���ܪ� callback function
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

// ����L������U�ɪ� callback function (�^�I�禡)
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
