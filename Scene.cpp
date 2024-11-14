#include <iostream>
#include <fstream>
#include <chrono>
#include "Scene.h"

Scene::Scene(int winWidth, int winHeight)
	: width{ winWidth }, height{ winHeight }
{
}

bool Scene::initialize()
{
	if (!loadShaders()) {
		std::cerr << "Failed to Load Shaders." << '\n';
		return false;
	}

	isDrag = false;
	startPos = endPos = glm::vec2(0.f);

	shapeList.push_back(new Shape());
	shapeList.back()->initTriangleBuffer();

	glLineWidth(1.5f);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	return true;
}

void Scene::initDragBuffer()
{
	float VAO[] = {
		startPos.x, startPos.y,
		endPos.x, endPos.y
	};

	dragVertex = new Vertex(VAO, 2);
}

void Scene::update(float elapsedTime)
{
	for (auto& shape : shapeList) {
		shape->Update();
	}
}

void Scene::draw()
{
	for (auto& shape : shapeList) {
		shape->Draw(spriteShader);
	}

	if (isDrag) {
		dragShader->setActive();
		//dragVertex->setActive();
		dragShader->setUniform4("startEndPos", startPos.x, startPos.y, endPos.x, endPos.y);

		glDrawArrays(GL_LINES, 0, 2);
	}
}

void Scene::keyboard(unsigned char key)
{
	switch (key) {
	case 'l':
	case 'L':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;

	case 'f':
	case 'F':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;

	case 'p':
	case 'P':
		break;

	case '+':
		break;

	case '-':
		break;
	}
}

void Scene::keyboardUp(unsigned char key)
{
}

void Scene::specialKeyboard(int key)
{
	switch (key) {
	case GLUT_KEY_UP:
		std::cout << "Up" << '\n';
		break;

	case GLUT_KEY_DOWN:
		std::cout << "Down" << '\n';
		break;

	case GLUT_KEY_LEFT:
		std::cout << "Left" << '\n';
		break;

	case GLUT_KEY_RIGHT:
		std::cout << "Right" << '\n';
		break;
	}
}

void Scene::specialKeyboardUp(int key)
{
}

void Scene::mouse(int button, int state, int x, int y)
{
	// È­¸é ¾÷µ¥ÀÌÆ®°¡ µÈ´Ù....

	constexpr int WHEEL_UP = 3;
	constexpr int WHEEL_DOWN = 4;	


	switch (state) {
	case GLUT_DOWN:
		switch (button) {
		case GLUT_LEFT_BUTTON:
		{
			float mx = (static_cast<float>(x) / width * 2.f - 1.f);
			float my = -(static_cast<float>(y) / height * 2.f - 1.f);

			startPos = endPos = glm::vec2(mx, my);
			isDrag = true;

			std::cout << "ÁÂÅ¬¸¯ : " << x << ", " << y << std::endl;
			std::cout << "OpenGL x ÁÂÇ¥´Â " << mx << std::endl;
			std::cout << "OpenGL y ÁÂÇ¥´Â " << my << std::endl;
			break;
		}

		case GLUT_MIDDLE_BUTTON:
			std::cout << "ÈÙÅ¬¸¯ : " << x << ", " << y << std::endl;
			break;

		case GLUT_RIGHT_BUTTON:
			std::cout << "¿ìÅ¬¸¯ : " << x << ", " << y << std::endl;
			break;

		case WHEEL_UP:
			std::cout << "ÈÙ  ¾÷ : " << x << ", " << y << std::endl;
			break;

		case WHEEL_DOWN:
			std::cout << "ÈÙ´Ù¿î : " << x << ", " << y << std::endl;
			break;
		}

		break;

	case GLUT_UP:
		switch (button) {
		case GLUT_LEFT_BUTTON:
			isDrag = false;


			break;

		case GLUT_RIGHT_BUTTON:
			break;

		case GLUT_MIDDLE_BUTTON:
			break;
		}

		break;
	}
}

void Scene::mouseMove(int x, int y)
{
	if (isDrag) {
		float mx = (static_cast<float>(x) / width * 2.f - 1.f);
		float my = (static_cast<float>(y) / height * 2.f - 1.f);
		endPos = glm::vec2(mx, my);
		std::cout << endPos.x << " " << endPos.y << '\n';
		//initDragBuffer();
	}
}
void Scene::splitShape(std::shared_ptr<Shape> shape, glm::vec2 l1, glm::vec2 l2)
{
}

void Scene::setWindowSize(int winWidth, int winHeight)
{
	width = winWidth;
	height = winHeight;
}
bool Scene::loadShaders()
{
	spriteShader = new Shader();

	if (!spriteShader->Load("shapeVertex.vert", "shapeFragment.frag"))
		return false;

	spriteShader->setActive();

	dragShader = new Shader();

	if (!dragShader->Load("lineVertex.vert", "lineFrag.frag"))
		return false;

	dragShader->setActive();

	return true;
}