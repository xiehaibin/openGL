#pragma once
#include <gl/gl.h>

/**
@name OpenGL上下文
@author hayden
*/
class GLContext
{
protected:
	HWND          m_hWnd;
	HDC           m_hDc;
	HGLRC         m_hRc;

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

		return true;
	}

	/**
	*   交换缓冲区
	*/
	void swapBuffer()
	{
		SwapBuffers(m_hDc);
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

