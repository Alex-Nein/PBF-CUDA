#include "Input.h"
#include <GLFW\glfw3.h>

const float Input::SCREEN_ROTATE_RATE = 0.005f;
const float Input::SCREEN_PAN_RATE = 0.002f;
const float Input::SCREEN_SCROLL_RATE = 0.1f;

Input::Input() {
	reset();
}

Input* Input::m_instance = nullptr;

Input& Input::getInstance() {
	if (!m_instance) {
		m_instance = new Input();
	}
	return *m_instance;
}

glm::vec2 Input::updateMousePos(glm::vec2 new_mouse)
{
	if (!last_mouse_valid) {
		last_mouse_valid = true;
		last_mouse = mouse = new_mouse;
	}
	else {
		last_mouse = mouse;
		mouse = new_mouse;
	}

	return mouse - last_mouse;
}

glm::vec2 Input::getMouseDiff() {
	return mouse - last_mouse;
}

void Input::reset() {
	last_mouse_valid = false;
	lastFrame = false;
	running = false;
	right_mouse = left_mouse = UP;
	hlIndex = 0;
	frameCount = 0;
	startMovingFrame = 0;
	moving = false;
}