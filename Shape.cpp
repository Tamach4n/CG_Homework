#include "Shape.h"

Shape::Shape()
{
	std::cout << "Shape()" << '\n';
	init();
}

Shape::Shape(float r,float degree)
{
	std::cout << "Shape(float r, float degree)" << '\n';
	init();
}

Shape::Shape(const std::vector<glm::vec2>& vertices)
{
	init();
	initBuffer();
}

Shape::Shape(const Shape& other)
{
}

Shape& Shape::operator=(const Shape& other)
{
	if (this != &other) {
	}

	return *this;
}

Shape::~Shape()
{
	delete shapeVertex;
}

void Shape::clearBuffer()
{
	delete shapeVertex;
}

void Shape::init()
{
	std::uniform_real_distribution<float> urdSize(0.2f, 0.5f);

	size = urdSize(rd);

	std::uniform_real_distribution<float> urdPos(-1.f + size, 1.f - size);
	std::uniform_real_distribution<float> urdColor(0.f, 1.f);

	pos = { urdPos(rd), urdPos(rd) };
	color = { urdColor(rd), urdColor(rd), urdColor(rd) };

	clearBuffer();
}

void Shape::initBuffer()
{
	std::vector<float> vertexData;
	for (const auto& v : vertices) {
		vertexData.push_back(v.x);
		vertexData.push_back(v.y);
	}

	shapeVertex = new Vertex(vertexData);
}

void Shape::initTriangleBuffer()
{
	std::uniform_real_distribution<float> urd(0.f, 1.f);

	float VAO[] = {
		 0.5f,  0.5f,	color.x, color.y, color.z,
		-0.5f, -0.5f,	color.x, color.y, color.z,
		 0.5f, -0.5f, 	color.x, color.y, color.z
	};
	
	unsigned int VBO[] = {
		0, 1, 2
	};

	shapeVertex = new Vertex(VAO, 3, VBO, 3);
}

void Shape::initRectangleBuffer()
{

}

void Shape::setActive(Shader* shader)
{
	shapeVertex->setActive();
}

void Shape::setPos(glm::vec2 v)
{
}

void Shape::setRotation(glm::vec2 v)
{
}

void Shape::setDirection(glm::vec2 v)
{
}

void Shape::setMove(glm::vec2 v)
{
}

bool Shape::getBroken() const
{
	return isBroken;
}

void Shape::setBroken(bool st)
{
	isBroken = st;
}

void Shape::Update()
{
	std::cout << "Size: " << size << ", Pos:" << pos.x << ", " << pos.y << '/n';
}

void Shape::Draw(Shader* shaderProgram)
{
	shapeVertex->setActive();
	shaderProgram->setUniform("pos", pos.x, pos.y);
	glDrawArrays(GL_LINE_LOOP, 0, vertices.size());
}


float Shape::cross2D(glm::vec2 p1, glm::vec2 p2) const
{
	return (p1.x * p2.y) - (p1.y * p2.x);
}
