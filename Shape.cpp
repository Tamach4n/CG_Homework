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

std::shared_ptr<Shape> Shape::createSquare(float size, glm::vec2 position)
{
	std::random_device rd;
	std::uniform_real_distribution<float> urd(0.f, 1.f);

	std::vector<float> squareVertices = {
	   position.x - size / 2, position.y - size / 2, urd(rd), urd(rd), urd(rd),
	   position.x + size / 2, position.y - size / 2, urd(rd), urd(rd), urd(rd),
	   position.x + size / 2, position.y + size / 2, urd(rd), urd(rd), urd(rd),
	   position.x - size / 2, position.y + size / 2, urd(rd), urd(rd), urd(rd)
	};

	return std::make_shared<Shape>(squareVertices);
}

std::shared_ptr<Shape> Shape::createCircle(float radius, int segments, glm::vec2 position)
{
	std::vector<glm::vec2> circleVertices;

	for (int i = 0; i < segments; ++i) {
		float angle = i * 2.0f * M_PI / segments;
		circleVertices.push_back({ position.x + radius * cos(angle), position.y + radius * sin(angle) });
	}

	return std::make_shared<Shape>(circleVertices);
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

std::optional<std::pair<glm::vec2, glm::vec2>> Shape::isCollide(glm::vec2 l1, glm::vec2 l2)
{
	glm::vec2 leftTop = glm::vec2{ -size, size } + pos;
	glm::vec2 leftBottom = glm::vec2{ -size, -size } + pos;
	glm::vec2 rightTop = glm::vec2{ size, size } + pos;
	glm::vec2 rightBottom = glm::vec2{ size, -size } + pos;

	std::vector<glm::vec2> intersections;

	// 각 변과의 교차점을 찾아 벡터에 저장
	if (auto intersection = isCollideLineByLine(l1, l2, leftTop, rightTop); intersection)
		intersections.push_back(*intersection);

	if (auto intersection = isCollideLineByLine(l1, l2, rightTop, rightBottom); intersection)
		intersections.push_back(*intersection);

	if (auto intersection = isCollideLineByLine(l1, l2, rightBottom, leftBottom); intersection)
		intersections.push_back(*intersection);

	if (auto intersection = isCollideLineByLine(l1, l2, leftBottom, leftTop); intersection)
		intersections.push_back(*intersection);

	// 교차점이 두 개 이상인 경우만 반환
	if (intersections.size() >= 2) {
		return std::make_pair(intersections[0], intersections[1]);
	}

	return std::nullopt; // 교차점이 없거나 하나일 경우 null 반환
}

std::vector<glm::vec2> Shape::findLineIntersections(glm::vec2 l1, glm::vec2 l2) const {
	std::vector<glm::vec2> intersections;

	for (size_t i = 0; i < vertices.size(); ++i) {
		glm::vec2 edgeStart = vertices[i];
		glm::vec2 edgeEnd = vertices[(i + 1) % vertices.size()];

		if (auto intersection = isCollideLineByLine(l1, l2, edgeStart, edgeEnd); intersection) {
			intersections.push_back(*intersection);
		}
	}

	return intersections;
}

std::optional<std::pair<std::shared_ptr<Shape>, std::shared_ptr<Shape>>> Shape::split(glm::vec2 l1, glm::vec2 l2) {
	// Similar logic as splitRectangle, modified to return Shape instances
	auto intersections = findLineIntersections(l1, l2);
	if (intersections.size() != 2) return std::nullopt;

	std::vector<glm::vec2> poly1, poly2;
	bool addToPoly1 = true;
	for (const auto& vertex : vertices) {
		(addToPoly1 ? poly1 : poly2).push_back(vertex);
		if (vertex == intersections[0] || vertex == intersections[1]) {
			addToPoly1 = !addToPoly1;
		}
	}

	// Add intersections to each polygon
	poly1.push_back(intersections[0]);
	poly2.push_back(intersections[1]);

	return std::make_pair(std::make_shared<Shape>(poly1), std::make_shared<Shape>(poly2));
}

void Shape::Update()
{

}

void Shape::Draw(GLuint shaderProgram)
{
	shapeVertex->setActive();
	glDrawArrays(GL_LINE_LOOP, 0, vertices.size());
}

std::optional<glm::vec2> Shape::isCollideLineByLine(glm::vec2 p1Start, glm::vec2 p1End, glm::vec2 p2Start, glm::vec2 p2End) const
{
	glm::vec2 d1 = p1End - p1Start;
	glm::vec2 d2 = p2End - p2Start;
	float crossD1D2 = cross2D(d1, d2);

	if (crossD1D2 == 0.0f) {
		return std::nullopt;
	}

	float t = cross2D(p2Start - p1Start, d2) / crossD1D2;
	float u = cross2D(p2Start - p1Start, d1) / crossD1D2;

	// t와 u가 0과 1 사이에 있을 때 두 선분이 실제로 교차함
	if (t >= 0 && t <= 1 && u >= 0 && u <= 1) {
		glm::vec2 intersection = p1Start + t * d1;
		return intersection;
	}

	return std::nullopt;
}

float Shape::cross2D(glm::vec2 p1, glm::vec2 p2) const
{
	return (p1.x * p2.y) - (p1.y * p2.x);
}
