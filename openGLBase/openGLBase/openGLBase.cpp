#include <windows.h>
#include <math.h>
#include <corecrt_math_defines.h>

#include "GLContext.h"


#include "Lesson01.h"
#include "Lesson02.h"

#include "Lesson03.h"
#include "Lesson03_minMap.h"
#include "Lesson03_TexUpdate.h"
#include "Lesson03_TexAnim.h"
#include "Lesson03_CompressTex.h"
#include "Lesson03_EnvironmentTex.h"
#include "Lesson03_FrameAnim.h"
#include "Lesson03_Sprite.h"
#include "Lesson03_Blend.h"

#include "Lesson04_1_DisplayList.h"
#include "Lesson04_2_VBO.h"
#include "Lesson04_3_IBO.h"
#include "Lesson04_4_PBuffer.h"
#include "Lesson04_5_FBO.h"
#include "Lesson04_6_PBO.h"
#include "Lesson04_6_PBO_1.h"
#include "Lesson04_6_PBO_2.h"
#include "Lesson04_6_PBO_UNPACK.h"
#include "Lesson04_6_12.h"

// 全局变量:
HINSTANCE   hInst;
GLContext   glContext;

DWORD dwNow = 0;
DWORD dwPre = 0;
DWORD dwFps = 60;

extern unsigned int unWidth  = 640;
extern unsigned int unHeight = 480;


Lesson01 lesson01;
Lesson02 lesson02;

Lesson03                lesson03;
Lesson03_minMap         lesson03MinMap;
Lesson03_TexUpdate      lesson03_TexUpdate;
Lesson03_TexAnim        lesson03_TexAnim;
Lesson03_CompressTex    lesson03_CompressTex;
Lesson03_EnvironmentTex lesson03_EnvironmentTex;
Lesson03_FrameAnim      lesson03_FrameAnim;
Lesson03_Sprite         lesson03_Sprite;
Lesson03_Blend          lesson03_Blend;

Lesson04_1_DisplayList  lesson04_1_DisplayList;
Lesson04_2_VBO          lesson04_2_VBO;
Lesson04_3_IBO          lesson04_3_IBO;
Lesson04_4_PBuffer      lesson04_4_PBuffer;
Lesson04_5_FBO          lesson04_5_FBO;
Lesson04_6_PBO          lesson04_6_PBO;
Lesson04_6_PBO_1        lesson04_6_PBO_1;
Lesson04_6_PBO_2        lesson04_6_PBO_2;
Lesson04_6_PBO_UNPACK   lesson04_6_PBO_UNPACK;
Lesson04_6_12           lesson04_6_12;


// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

void                MainRender();


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
	unWidth = rect.right - rect.left;
	unHeight = rect.bottom - rect.top;

	//lesson03.initGL();
	//lesson03MinMap.initGL();
	//lesson03_TexUpdate.initGL();
	//lesson03_TexAnim.initGL();
	//lesson03_CompressTex.initGL();
	//lesson03_EnvironmentTex.initGL();
	//lesson03_FrameAnim.initGL();
	//lesson03_Sprite.initGL();
	//lesson03_Blend.initGL();

	//lesson04_1_DisplayList.initGL();
	//lesson04_2_VBO.initGL();
	//lesson04_3_IBO.initGL();
	//lesson04_4_PBuffer.initGL(glContext, hWnd, GetDC(hWnd), glContext.m_hRc, unWidth, unHeight);
	//lesson04_5_FBO.initGL();
	//lesson04_6_PBO.initGL();
	//lesson04_6_PBO_1.initGL();
	//lesson04_6_PBO_2.initGL();
	//lesson04_6_PBO_UNPACK.initGL();
	lesson04_6_12.initGL();


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
			if ((dwNow - dwPre) >= dwInterval)
			{
				dwPre = dwNow;
				MainRender();
			}
			else
			{
				Sleep(dwInterval - (dwNow - dwPre));
				MainRender();
			}
		}
	}

	//lesson03.onDisable();
	//lesson03MinMap.onDisable();
	//lesson03_TexUpdate.onDisable();
	//lesson03_TexAnim.onDisable();
	//lesson03_CompressTex.onDisable();
	//lesson03_EnvironmentTex.onDisable();
	//lesson03_FrameAnim.onDisable();
	//lesson03_Sprite.onDisable();
	//lesson03_Blend.onDisable();

	//lesson04_1_DisplayList.onDisable();
	//lesson04_2_VBO.onDisable();
	//lesson04_3_IBO.onDisable();
	//lesson04_4_PBuffer.onDisable();
	//lesson04_5_FBO.onDisable();
	//lesson04_6_PBO.onDisable();
	//lesson04_6_PBO_1.onDisable();
	//lesson04_6_PBO_2.onDisable();
	//lesson04_6_PBO_UNPACK.onDisable();
	lesson04_6_12.onDisable();

	glContext.destroy();

    return (int) msg.wParam;
}
/*
0,0   1,0
0,1   1,1
*/

/*
纹理坐标
0,1  1,1
0,0  1,0
*/
void MainRender()
{
	glClearColor(0,0,0,1);
	glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //清除 模板颜缓冲 色缓冲 和 深度缓冲区
	// 定义视口
	glViewport(0, 0, unWidth, unHeight);

	//lesson01.Render();
	//lesson02.Render();

	//lesson03.render();
	//lesson03MinMap.render();
	//lesson03_TexUpdate.render();
	//lesson03_TexAnim.render();
	//lesson03_CompressTex.render();
	//lesson03_EnvironmentTex.render();
	//lesson03_FrameAnim.render();
	//lesson03_Sprite.render();
	//lesson03_Blend.render();

	//lesson04_1_DisplayList.render();
	//lesson04_2_VBO.render();
	//lesson04_3_IBO.render();
	//lesson04_4_PBuffer.render();
	//lesson04_5_FBO.render();
	//lesson04_6_PBO.render();
	//lesson04_6_PBO_1.render();
	//lesson04_6_PBO_2.render();
	//lesson04_6_PBO_UNPACK.render();
	lesson04_6_12.render();

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
