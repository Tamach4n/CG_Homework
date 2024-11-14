#pragma once
#include <gl/glew.h> // 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include "glm/glm.hpp"
#include "glm/ext.hpp"

#include <string>
#include <vector>
#include <random>
#include "Vertex.h"	
#include "shader.h"
#include "Shape.h"

class Scene
{
public:
	// 게임 로직에서, update, draw..., 키 입력
	Scene(int winWidth, int winHeight);

	bool initialize();

	void initDragBuffer();

	void update(float elapsedTime);

	void draw();

	void keyboard(unsigned char key);		// press
	void keyboardUp(unsigned char key);		// release
	void specialKeyboard(int key);			// press
	void specialKeyboardUp(int key);		// release
	void mouse(int button, int state, int x, int y);
	void mouseMove(int x, int y);

	void splitShape(std::shared_ptr<Shape> shape, glm::vec2 l1, glm::vec2 l2);

	void setWindowSize(int winWidth, int winHeight);

private:
	int width;
	int height;

	bool isDrag;
	glm::vec2 startPos;
	glm::vec2 endPos;
	
	class Shader* spriteShader;
	class Shader* dragShader;
	class Vertex* dragVertex;

	std::vector<Shape*> shapeList;

	bool loadShaders();

	std::random_device rd;
};