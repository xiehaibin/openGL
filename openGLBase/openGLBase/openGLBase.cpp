#include <windows.h>
#include <gl/GL.h>
#include <math.h>
#include <corecrt_math_defines.h>

#include "GLContext.h"

// 全局变量:
HINSTANCE   hInst;

GLContext   glContext;

DWORD dwNow = 0;
DWORD dwPre = 0;
DWORD dwFps = 60;

unsigned int unWidth  = 640;
unsigned int unHeight = 480;

struct Vertex
{
	float x;
	float y;
	float z;

public :
	Vertex()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}
};

struct float3
{
	float x;
	float y;
	float z;
};

struct VertexC
{
	float x;
	float y;
	float z;

	float r;
	float g;
	float b;
};


// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

void                MainRender();

void Render()
{
	// 指定以下操作针对投影矩阵
	glMatrixMode(GL_PROJECTION);
	// 将投影矩阵清空成单位矩阵
	glLoadIdentity();
	// 正交矩阵
	glOrtho(0, unWidth, unHeight, 0, -100, 100);

	//==================================================
	// 指定绘制模式“画线”
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(100, 100, 0);
	glEnd(); // 结束画线
	//===================================================


	//===================================================
	//三角形
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_TRIANGLES);
		glVertex3f(unWidth*0.5, 0, 0);
		glVertex3f(unWidth*0.5 - 50, 100, 0);
		glVertex3f(unWidth*0.5 + 50, 100, 0);
	glEnd();

	// 三角形带
	float3 circle2[4] = {
		{0.0f,   100.0f, 0.0f},
		{100.0f, 100.0f, 0.0f},
		{0.0f,   200.0f, 0.0f},
		{100.0f, 200.0f, 0.0f}
	};
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(float3), circle2);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);  // 减少顶点传递量

	// 三角形颜色
	VertexC v[4] = {
		{0.0f,   200.0f, 0.0f,  1.0f, 0.0f, 0.0f},
		{100.0f, 200.0f, 0.0f,  0.0f, 1.0f, 0.0f},
		{0.0f,   300.0f, 0.0f,  0.0f, 0.0f, 1.0f},
		{100.0f, 300.0f, 0.0f,  0.0f, 1.0f, 1.0f},
	};
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(VertexC), v);
	glColorPointer(3, GL_FLOAT, sizeof(VertexC), &v[0].r);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);  
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	//===================================================


	//===================================================
	// 圆
	glColor3f(1.0, 0, 0);
	float nCenterPointX = unWidth * 0.5f;
	float nCenterPointY = unHeight * 0.5f;
	float radius = 50.0f;
	const int step = 10;
	//glBegin glEnd 比较原始旧 效率低  1.0版本 高版本已抛弃
	glBegin(GL_TRIANGLES);
		for (int i = 0; i < 360; i += step)
		{
			float   rad = (double)i*(M_PI/180.0f); // pi的精度要够
			float x = radius * cos(rad) + nCenterPointX;
			float y = radius * sin(rad) + nCenterPointY;

			rad = ((double)i + step)*(M_PI/180.0f);
			float x1 = radius * cos(rad) + nCenterPointX;
			float y1 = radius * sin(rad) + nCenterPointY;

			glVertex3f(nCenterPointX, nCenterPointY, 0);
			glVertex3f(x, y, 0);
			glVertex3f(x1, y1, 0);
		}
	glEnd();

	//优化1
	glColor3f(0, 0, 1.0);
	Vertex circle[36*3] = {};
	nCenterPointX = unWidth * 0.5f + 100;
	nCenterPointY = unHeight * 0.5f;
	int nIndex = 0;
	for (int i = 0; i < 360; i += step)
	{
		circle[nIndex].x = nCenterPointX;
		circle[nIndex].y = nCenterPointY;
		circle[nIndex].z = 0;

		float   rad = (double)i*(M_PI/180.0f);
		circle[nIndex+1].x = radius * cos(rad) + nCenterPointX;
		circle[nIndex+1].y = radius * sin(rad) + nCenterPointY;
		circle[nIndex+1].z = 0;

		rad = ((double)i + step)*(M_PI/180.0f);
		circle[nIndex+2].x = radius * cos(rad) + nCenterPointX;
		circle[nIndex+2].y = radius * sin(rad) + nCenterPointY;
		circle[nIndex+2].z = 0;

		nIndex += 3;
	}
	// 启用顶点数组模式
	glEnableClientState(GL_VERTEX_ARRAY);
	//0为偏移  3*GL_FLOAT/sizeof(Vertex)
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex)/*0*/, circle); // 顶点一批传递给显卡或openGL,减小压栈
	glDrawArrays(GL_TRIANGLES, 0, 36*3);     // 绘制

	//优化2
	glColor3f(0, 1.0, 1.0);
	Vertex circle1[38] = {};
	nCenterPointX = unWidth * 0.5f - 100;
	nCenterPointY = unHeight * 0.5f;
	nIndex = 0;
	circle1[nIndex].x = nCenterPointX;
	circle1[nIndex].y = nCenterPointY;
	circle1[nIndex].z = 0;
	for (int i = 0; i <= 360; i += step)
	{
		nIndex += 1;
		float   rad = (double)i*(M_PI/180.0f);
		circle1[nIndex].x = radius * cos(rad) + nCenterPointX;
		circle1[nIndex].y = radius * sin(rad) + nCenterPointY;
		circle1[nIndex].z = 0;
	}
	// 启用顶点数组模式
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), circle1); 
	//三角形扇
	glDrawArrays(GL_TRIANGLE_FAN, 0, 38);  // 减少顶点传递量 2/3
	//===================================================

	//===================================================
	//===================================================

	//===================================================
	//===================================================
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    MyRegisterClass(hInstance);

	HWND hWnd = CreateWindowW(L"OpenGL基础", L"OpenGL基础", WS_OVERLAPPEDWINDOW, 0, 0, unWidth, unHeight, nullptr, nullptr, hInstance, nullptr);
	if (!hWnd)
	{
		return FALSE;
	}
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


	if (!glContext.init(hWnd, GetDC(hWnd)))
	{
		return FALSE;
	}

	// 绘制区域
	RECT rect;
	GetClientRect(hWnd, &rect);
	unWidth  = rect.right - rect.left;
	unHeight = rect.bottom - rect.top;

    MSG msg;
	memset(&msg, 0, sizeof(msg));
	DWORD dwInterval = 1000/dwFps;
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			dwNow = GetTickCount();
			if (dwNow - dwPre >= dwInterval)
			{
				MainRender();
			}
			else
			{
				Sleep(dwInterval - (dwNow - dwPre));
				MainRender();
			}
		}
	}

	glContext.destroy();

    return (int) msg.wParam;
}
/*
0,0   1,0
0,1   1,1
*/
void MainRender()
{
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //清除颜色缓冲和深度缓冲区
	// 定义视口
	glViewport(0, 0, unWidth, unHeight);

	Render();

	glContext.swapBuffer();
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW);
    wcex.lpszMenuName   = 0;
    wcex.lpszClassName  = L"OpenGL基础";
    wcex.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);

    return RegisterClassExW(&wcex);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
