#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <random>
#include <optional>
#include <vector>
#include "gl/glew.h"
#include <gl/glut.h>
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "Vertex.h"
#include "Shader.h"

class Shape
{
public:
	Shape();
	Shape(float r, float degree);
	Shape(const std::vector<glm::vec2>& vertices);

	
	Shape(const Shape& other);
	Shape& operator=(const Shape& other);
	~Shape();

	virtual void clearBuffer();

	virtual void init();
	virtual void initBuffer();
	void initTriangleBuffer();
	void initRectangleBuffer();

	virtual void setActive(Shader* shader);
	virtual void setPos(glm::vec2 v);
	virtual void setRotation(glm::vec2 v);
	virtual void setDirection(glm::vec2 v);
	virtual void setMove(glm::vec2 v);

	virtual bool getBroken() const;
	virtual void setBroken(bool st);

	//virtual std::optional<std::pair<glm::vec2, glm::vec2>> isCollide(glm::vec2 l1, glm::vec2 l2);

	virtual void Update();

	virtual void Draw(Shader* shaderProgram);

protected:
	Vertex* shapeVertex;

	std::vector<glm::vec2> vertices;

	//virtual std::optional<glm::vec2> isCollideLineByLine(glm::vec2 p1Start, glm::vec2 p1End, glm::vec2 p2Start, glm::vec2 p2End) const;
	virtual float cross2D(glm::vec2 p1, glm::vec2 p2) const;

	float size;
	glm::vec2 pos;
	glm::vec3 color;

	bool isBroken;

	std::random_device rd;
};

