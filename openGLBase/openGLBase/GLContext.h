#pragma once
#include <gl/gl.h>

/**
@name OpenGL������
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
			PFD_MAIN_PLANE,  // ������ ��һ��ֱ�ӻ�����Ļ���ز���ͼ��
			0,
			0,
			0,
			0
		};

		if (0 == m_format)
		{
			// �ڵ�ǰhdc��ƥ�䵱ǰ���ظ�ʽ
			pixelFormat = ChoosePixelFormat(m_hDc, &pfd);
		}
		else
		{
			pixelFormat = m_format;
		}

		// ��win�ڲ��������ֻ���������ɫ�������� ��� ģ�壩
		if (!SetPixelFormat(m_hDc, pixelFormat, &pfd))
		{
			return false;
		}

		// �ڸ�����DC�Ͻ���openGL������
		m_hRc = wglCreateContext(m_hDc);
		/*
		ʹһ��ָ����OpenGL��Ⱦ�����ĵ����̵߳ĵ�ǰ���������� �����к�����OpenGL�����߳���ȷ�����豸 HDC�ϻ��� ��
		��������ʹ��wglMakeCurrent�ı䣬���Ծɵ������Ľ������ǵ�ǰ�����̵߳ĵ�ǰ���ֵ�������
		openҲ֧�ֶ��̻߳��ƣ���Ҫ�л�������
		*/
		if (!wglMakeCurrent(m_hDc, m_hRc))
		{
			return false;
		}

		return true;
	}

	/**
	*   ����������
	*/
	void swapBuffer()
	{
		SwapBuffers(m_hDc);
	}

	/**
	����openGL
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

