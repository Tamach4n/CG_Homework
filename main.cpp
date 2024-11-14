#include <iostream>
#include <chrono>
#include "Scene.h"

// 각종 콜백 함수 미리 선언
void DisplayFunc(void);
void ReshapeFunc(int width, int height);
void KeyboardFunc(unsigned char, int, int);
void KeyboardUpFunc(unsigned char, int, int);
void SpecialFunc(int, int, int);
void SpecialUpFunc(int, int, int);
void MouseFunc(int, int, int, int);
void MotionFunc(int, int);
void TimerFunc();

// 전역 변수
constexpr int winWidth = 800, winHeight = 800;

Scene g_scene{ winWidth, winHeight };

int main(int argc, char** argv)
{
	//--- 윈도우 생성하기 (freeglut)
	glutInit(&argc, argv);							// glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);	// 디스플레이 모드 설정
	glutInitWindowPosition(100, 100);				// 윈도우의 위치 지정
	glutInitWindowSize(winWidth, winHeight);			// 윈도우의 크기 지정
	glutCreateWindow("Polygon Slice");				// 윈도우 생성 (윈도우 이름)

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {					// glew 초기화
		std::cerr << "Unable to initialize GLEW\n";
		exit(EXIT_FAILURE);
	}

	else {
		std::cout << "GLEW Initialized\n";
	}

	glClearColor(0.5f, 0.5f, 0.9f, 1.0f);

	// 초기 설정들
	std::cout << std::boolalpha;

	if (g_scene.initialize()) {
		glEnable(GL_CULL_FACE);             // 컬링 활성화
		//glCullFace(GL_BACK);                // 뒷면을 컬링
		//glFrontFace(GL_CCW);                // 앞면을 반시계 방향으로 정의
		
		// 콜백 함수들
		glutKeyboardFunc(KeyboardFunc);		// 일반 키보드 (abcde..)
		glutKeyboardUpFunc(KeyboardUpFunc);	// 일반 키보드가 떼졌을 때
		glutSpecialFunc(SpecialFunc);		// 특수 키보드 (F1 ~ F12, HOME, CTRL, ALT 등등..)
		glutSpecialUpFunc(SpecialUpFunc);	// 특수 키보드가 떼졌을 때

		glutMouseFunc(MouseFunc);			// 마우스 입력 (좌클릭, 우클릭, 휠클릭... 
		glutMotionFunc(MotionFunc);			// 화면 누르고 드래그

		glutDisplayFunc(DisplayFunc);		// 출력 함수의 지정
		glutReshapeFunc(ReshapeFunc);		// 화면 크기가 변경되었을 때....
		glutIdleFunc(TimerFunc);	// 16ms --> 초당 60번

		glutMainLoop(); // 이벤트 처리 시작
	}
}

// 전역변수를 최소화!

// 클래스 사용!, 분할 컴파일!
// .cpp당 하나의 obj가 나오도록 컴파일 - 라이브러리와 obj들을 함께 링크하여 실행파일...

void DisplayFunc(void)
{
	glClear(GL_COLOR_BUFFER_BIT);	// 후면버퍼 지우기

	// 씬을 그린다
	g_scene.draw();

	// 후면버퍼를 전면 버퍼와 바꿔준다!
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
	// 지난 시각
	static std::chrono::high_resolution_clock::time_point last_time = std::chrono::high_resolution_clock::now();
	static std::chrono::high_resolution_clock::duration accm_time;	// 누적 시간

	++fps;
	auto now_time = std::chrono::high_resolution_clock::now();
	auto dur_time = now_time - last_time;

	accm_time += dur_time;
	last_time = now_time;

	if (accm_time >= std::chrono::seconds(1)) {
		std::string str = "Example | (FPS : " + std::to_string(fps) + ")";
		glutSetWindowTitle(str.c_str());		// 창에 FPS 표시

		fps = 0;
		accm_time = std::chrono::seconds(0);
	}

	g_scene.update(dur_time.count() / 1E09);

	glutPostRedisplay();
}