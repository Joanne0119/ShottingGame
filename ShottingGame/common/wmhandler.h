#pragma once
#include <GLFW/glfw3.h>

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
