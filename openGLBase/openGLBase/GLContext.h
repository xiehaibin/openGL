#pragma once
#include "glew/glew.h"  // openGL扩展库 必须在gl.h之前引入
#include "glew/wglew.h" // w和windoes相关
//#include <gl/GL.h>
#include "FreeImage/FreeImage.h"

/*
GLEW库是OpenGL的扩展库，用于支持OpenGL1.1以上的版本[0]。比如着色器特性。一般非集成显卡都能支持到OpenGL2.0或者更高，但是没有GLEW库依然无法调用高版本OpenGL的函数。
GLEW能自动识别你的平台所支持的全部OpenGL高级扩展涵数。也就是说，只要包含一个glew.h头文件，你就能使用gl, glu, glext, wgl, glx的全部函数[0]。
*/
#pragma comment(lib, "../Debug/lib/glew32.lib")
#pragma comment(lib, "FreeImage/FreeImage.lib")

/**
@name OpenGL上下文
@author hayden
*/
class GLContext
{
public:
	HWND          m_hWnd;  //! 窗口句柄
	HDC           m_hDc;   //! 绘制设备上下文
	HGLRC         m_hRc;   //! OpenGL上下文

	int           m_format;

public:
	GLContext()
	{
		m_format     = 0;

		m_hWnd       = NULL;
		m_hDc        = NULL;
		m_hRc        = NULL;
	}
	~GLContext()
	{
		destroy();
	}

	// openGL扩展库
	int InitOpenGLEx()
	{
		glewInit();
		wglSwapIntervalEXT(1);
		/*
		PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");
		wglSwapIntervalEXT(1);//打开垂直同步，限制帧率
		wglSwapIntervalEXT(0);//关闭垂直同步，充分发挥显卡的渲染能力
		*/

		/*
		缺点：必须是已经创建起openGL上下文
		保存pixelFormat 重启游戏  SetPixelFormat(m_hDc, pixelFormat, &pfd)
		*/
		
		// 全屏反锯齿
		if (wglChoosePixelFormatARB == 0)
		{
			// 不支持全屏反锯齿
			return 0;
		}

		int		pixelFormat;
		int		valid;
		UINT	numFormats;
		float	fAttributes[] = { 0,0 };

		int iAttributes[] =
		{
			WGL_DRAW_TO_WINDOW_ARB,GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB,GL_TRUE,
			WGL_ACCELERATION_ARB,WGL_FULL_ACCELERATION_ARB,
			WGL_COLOR_BITS_ARB,24,
			WGL_ALPHA_BITS_ARB,8,
			WGL_DEPTH_BITS_ARB,16,
			WGL_STENCIL_BITS_ARB,0,
			WGL_DOUBLE_BUFFER_ARB,GL_TRUE,
			WGL_SAMPLE_BUFFERS_ARB,GL_TRUE, // 支持多采样（反锯齿）
			WGL_SAMPLES_ARB,4,              // 采样频率 （4倍采样2 8 16）
			0,0
		};

		//pixelFormat 返回的格式
		valid = wglChoosePixelFormatARB(m_hDc, iAttributes, fAttributes, 1, &pixelFormat, &numFormats);
		if (valid && numFormats >= 1)
		{
			return  pixelFormat;
		}

		iAttributes[19] = 2; //4倍采样不支持 2倍采样试试
		valid = wglChoosePixelFormatARB(m_hDc, iAttributes, fAttributes, 1, &pixelFormat, &numFormats);
		if (valid && numFormats >= 1)
		{
			return pixelFormat;
		}
	
		return  0;
	}

	bool init(HWND hWnd, HDC hDC)
	{
		m_hWnd    = hWnd;
		m_hDc     = hDC;
		unsigned int pixelFormat;
		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA,
			32,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			24,
			8,
			0,
			PFD_MAIN_PLANE,  // 主窗口 另一是直接画到屏幕（截不到图）
			0,
			0,
			0,
			0
		};

		if (0 == m_format)
		{
			// 在当前hdc上匹配当前像素格式
			pixelFormat = ChoosePixelFormat(m_hDc, &pfd);
		}
		else
		{
			pixelFormat = m_format;
		}

		pixelFormat = 32;
		// 在win内部建立各种缓冲区（颜色缓冲区， 深度 模板）
		if (!SetPixelFormat(m_hDc, pixelFormat, &pfd))
		{
			return false;
		}

		// 在给定的DC上建立openGL上下文
		m_hRc = wglCreateContext(m_hDc);
		/*
		使一个指定的OpenGL渲染上下文调用线程的当前呈现上下文 。所有后续的OpenGL调用线程所确定的设备 HDC上绘制 。
		您还可以使用wglMakeCurrent改变，所以旧的上下文将不再是当前调用线程的当前呈现的上下文
		open也支持多线程绘制，但要切换上下文
		*/
		if (!wglMakeCurrent(m_hDc, m_hRc))
		{
			return false;
		}

		glewInit();

		//int formt = InitOpenGLEx(); //pixelFormat
		//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

		return true;
	}

	/**
	*   交换缓冲区
	*/
	void swapBuffer()
	{
		SwapBuffers(m_hDc);
	}

	void makeCurrent()
	{
		wglMakeCurrent(m_hDc, m_hRc);
	}

	/**
	销毁openGL
	*/
	void destroy()
	{
		if (!m_hRc)
		{
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(m_hRc);
			m_hRc = NULL;
		}

		if (!m_hDc)
		{
			ReleaseDC(m_hWnd, m_hDc);
			m_hDc = NULL;
		}
	}
};

