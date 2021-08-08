#pragma once
#include "glew/glew.h"  
#include "glew/wglew.h" 

//#pragma comment(lib, "../Debug/lib/glew32.lib")

class PBuffer
{
public :
	bool init(HWND hWnd, HDC hdc, HGLRC hrc, int width, int height)
	{
		m_hWnd = hWnd;
		m_width = width;
		m_height = height;

		int pbuffAttr[] = {
			WGL_SUPPORT_OPENGL_ARB, TRUE,       // P-buffer will be used with OpenGL
			WGL_DRAW_TO_PBUFFER_ARB, TRUE,      // Enable render to p-buffer
			WGL_RED_BITS_ARB, 8,                // At least 8 bits for RED channel
			WGL_GREEN_BITS_ARB, 8,              // At least 8 bits for GREEN channel
			WGL_BLUE_BITS_ARB, 8,               // At least 8 bits for BLUE channel
			WGL_ALPHA_BITS_ARB, 8,              // At least 8 bits for ALPHA channel
			WGL_DEPTH_BITS_ARB, 16,             // At least 16 bits for depth buffer
			WGL_DOUBLE_BUFFER_ARB, FALSE,       // We don't require double buffering
			0                                   // Zero terminates the list
		};
		unsigned count         = 0;
		int      pixelFormat   = 0;
		wglChoosePixelFormatARB(hdc, (const int*)pbuffAttr, NULL, 1, &pixelFormat, &count);
		if (count == 0)
		{
			// 没有匹配的
			return false;
		}

		m_hPBuffer = wglCreatePbufferARB(hdc, pixelFormat, width, height, NULL);
		if (m_hPBuffer == 0)
		{
			return false;
		}
		// 获取pbuffer的DC
		m_hDC      = wglGetPbufferDCARB(m_hPBuffer);
		// 在给定的DC上建立openGL上下文
		m_hRC      = wglCreateContext(m_hDC);

		int w = 0;
		int h = 0;
		wglQueryPbufferARB(m_hPBuffer, WGL_PBUFFER_WIDTH_ARB, &w);
		wglQueryPbufferARB(m_hPBuffer, WGL_PBUFFER_HEIGHT_ARB, &h);
		if (h != height || w != width)
		{
			return false;
		}
		
		// 1.dc与rc关联在一起， 当我们用rc绘制的时候数据就到pBuffer来了
		// 2.wglMakeCurrent告诉主线程谁是用的对象
		if (!wglMakeCurrent(m_hDC, m_hRC))
		{
			return false;
		}
		// wglShareLists使pBuffer可以使用主对象的资源
		if (!wglShareLists(hrc, m_hRC))
		{
			return false;
		}

		return true;
	}

	void makeCurrent()
	{
		wglMakeCurrent(m_hDC, m_hRC);
	}

	void destroy()
	{
		if (m_hRC)
		{
			wglMakeCurrent(m_hDC, m_hRC);
			wglDeleteContext(m_hRC);
			wglReleasePbufferDCARB(m_hPBuffer, m_hDC);
			wglDestroyPbufferARB(m_hPBuffer);
			m_hRC = NULL;
		}

		if (m_hDC)
		{
			ReleaseDC(m_hWnd, m_hDC);
			m_hDC = NULL;
		}
	}

public :
	HPBUFFERARB m_hPBuffer;    //存取数据缓冲区
	HDC         m_hDC;
	HWND        m_hWnd;
	HGLRC       m_hRC;         //主应用的rc
	int         m_width;
	int         m_height;
};
