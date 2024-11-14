#include <iostream>
#include <chrono>
#include "Scene.h"

// ���� �ݹ� �Լ� �̸� ����
void DisplayFunc(void);
void ReshapeFunc(int width, int height);
void KeyboardFunc(unsigned char, int, int);
void KeyboardUpFunc(unsigned char, int, int);
void SpecialFunc(int, int, int);
void SpecialUpFunc(int, int, int);
void MouseFunc(int, int, int, int);
void MotionFunc(int, int);
void TimerFunc();

// ���� ����
constexpr int winWidth = 800, winHeight = 800;

Scene g_scene{ winWidth, winHeight };

int main(int argc, char** argv)
{
	//--- ������ �����ϱ� (freeglut)
	glutInit(&argc, argv);							// glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);	// ���÷��� ��� ����
	glutInitWindowPosition(100, 100);				// �������� ��ġ ����
	glutInitWindowSize(winWidth, winHeight);			// �������� ũ�� ����
	glutCreateWindow("Polygon Slice");				// ������ ���� (������ �̸�)

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {					// glew �ʱ�ȭ
		std::cerr << "Unable to initialize GLEW\n";
		exit(EXIT_FAILURE);
	}

	else {
		std::cout << "GLEW Initialized\n";
	}

	glClearColor(0.5f, 0.5f, 0.9f, 1.0f);

	// �ʱ� ������
	std::cout << std::boolalpha;

	if (g_scene.initialize()) {
		glEnable(GL_CULL_FACE);             // �ø� Ȱ��ȭ
		//glCullFace(GL_BACK);                // �޸��� �ø�
		//glFrontFace(GL_CCW);                // �ո��� �ݽð� �������� ����
		
		// �ݹ� �Լ���
		glutKeyboardFunc(KeyboardFunc);		// �Ϲ� Ű���� (abcde..)
		glutKeyboardUpFunc(KeyboardUpFunc);	// �Ϲ� Ű���尡 ������ ��
		glutSpecialFunc(SpecialFunc);		// Ư�� Ű���� (F1 ~ F12, HOME, CTRL, ALT ���..)
		glutSpecialUpFunc(SpecialUpFunc);	// Ư�� Ű���尡 ������ ��

		glutMouseFunc(MouseFunc);			// ���콺 �Է� (��Ŭ��, ��Ŭ��, ��Ŭ��... 
		glutMotionFunc(MotionFunc);			// ȭ�� ������ �巡��

		glutDisplayFunc(DisplayFunc);		// ��� �Լ��� ����
		glutReshapeFunc(ReshapeFunc);		// ȭ�� ũ�Ⱑ ����Ǿ��� ��....
		glutIdleFunc(TimerFunc);	// 16ms --> �ʴ� 60��

		glutMainLoop(); // �̺�Ʈ ó�� ����
	}
}

// ���������� �ּ�ȭ!

// Ŭ���� ���!, ���� ������!
// .cpp�� �ϳ��� obj�� �������� ������ - ���̺귯���� obj���� �Բ� ��ũ�Ͽ� ��������...

void DisplayFunc(void)
{
	glClear(GL_COLOR_BUFFER_BIT);	// �ĸ���� �����

	// ���� �׸���
	g_scene.draw();

	// �ĸ���۸� ���� ���ۿ� �ٲ��ش�!
	glutSwapBuffers();
}

void ReshapeFunc(int width, int height)
{
	g_scene.setWindowSize(width, height);

	glutPostRedisplay();

	glViewport(0, 0, width, height);
}

void KeyboardFunc(unsigned char key, int x, int y)
{
	if (27 == key or 'Q' == key)
		glutLeaveMainLoop();

	g_scene.keyboard(key);
}

void KeyboardUpFunc(unsigned char key, int, int)
{
	g_scene.keyboardUp(key);
}

void SpecialFunc(int key, int x, int y)
{
	g_scene.specialKeyboard(key);
}

void SpecialUpFunc(int key, int, int)
{
	g_scene.specialKeyboardUp(key);
}

void MouseFunc(int button, int state, int x, int y)
{
	g_scene.mouse(button, state, x, y);
}

void MotionFunc(int x, int y)
{
	g_scene.mouseMove(x, y);
}

void TimerFunc()
{
	static int fps = 0;
	// ���� �ð�
	static std::chrono::high_resolution_clock::time_point last_time = std::chrono::high_resolution_clock::now();
	static std::chrono::high_resolution_clock::duration accm_time;	// ���� �ð�

	++fps;
	auto now_time = std::chrono::high_resolution_clock::now();
	auto dur_time = now_time - last_time;

	accm_time += dur_time;
	last_time = now_time;

	if (accm_time >= std::chrono::seconds(1)) {
		std::string str = "Example | (FPS : " + std::to_string(fps) + ")";
		glutSetWindowTitle(str.c_str());		// â�� FPS ǥ��

		fps = 0;
		accm_time = std::chrono::seconds(0);
	}

	g_scene.update(dur_time.count() / 1E09);

	glutPostRedisplay();
}