#pragma once
#include "glew/glew.h"  // openGL��չ�� ������gl.h֮ǰ����
#include "glew/wglew.h" // w��windoes���
//#include <gl/GL.h>
#include "FreeImage/FreeImage.h"

/*
GLEW����OpenGL����չ�⣬����֧��OpenGL1.1���ϵİ汾[0]��������ɫ�����ԡ�һ��Ǽ����Կ�����֧�ֵ�OpenGL2.0���߸��ߣ�����û��GLEW����Ȼ�޷����ø߰汾OpenGL�ĺ�����
GLEW���Զ�ʶ�����ƽ̨��֧�ֵ�ȫ��OpenGL�߼���չ������Ҳ����˵��ֻҪ����һ��glew.hͷ�ļ��������ʹ��gl, glu, glext, wgl, glx��ȫ������[0]��
*/
#pragma comment(lib, "../Debug/lib/glew32.lib")
#pragma comment(lib, "FreeImage/FreeImage.lib")

/**
@name OpenGL������
@author hayden
*/
class GLContext
{
public:
	HWND          m_hWnd;  //! ���ھ��
	HDC           m_hDc;   //! �����豸������
	HGLRC         m_hRc;   //! OpenGL������

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

	// openGL��չ��
	int InitOpenGLEx()
	{
		glewInit();
		wglSwapIntervalEXT(1);
		/*
		PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");
		wglSwapIntervalEXT(1);//�򿪴�ֱͬ��������֡��
		wglSwapIntervalEXT(0);//�رմ�ֱͬ������ַ����Կ�����Ⱦ����
		*/

		/*
		ȱ�㣺�������Ѿ�������openGL������
		����pixelFormat ������Ϸ  SetPixelFormat(m_hDc, pixelFormat, &pfd)
		*/
		
		// ȫ�������
		if (wglChoosePixelFormatARB == 0)
		{
			// ��֧��ȫ�������
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
			WGL_SAMPLE_BUFFERS_ARB,GL_TRUE, // ֧�ֶ����������ݣ�
			WGL_SAMPLES_ARB,4,              // ����Ƶ�� ��4������2 8 16��
			0,0
		};

		//pixelFormat ���صĸ�ʽ
		valid = wglChoosePixelFormatARB(m_hDc, iAttributes, fAttributes, 1, &pixelFormat, &numFormats);
		if (valid && numFormats >= 1)
		{
			return  pixelFormat;
		}

		iAttributes[19] = 2; //4��������֧�� 2����������
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

		pixelFormat = 32;
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

		glewInit();

		//int formt = InitOpenGLEx(); //pixelFormat
		//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

		return true;
	}

	/**
	*   ����������
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

