#pragma once
#include "glew/glew.h"  

class FrameBufferObject
{
public :
	unsigned  m_width;
	unsigned  m_heigth;
	unsigned  m_FBOID;
	unsigned  m_RBOID;

public :
	bool init(int w, int h)
	{
		m_width = w;
		m_heigth = h;

		glGenFramebuffers(1, &m_FBOID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBOID);

		glGenRenderbuffers(1, &m_RBOID);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RBOID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, m_width, m_heigth);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RBOID);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return true;
	}

	void begin(GLuint texId)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBOID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texId, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RBOID);
	}

	void end()
	{
		//×´Ì¬ÇÐ»»µ½Ö÷´°¿Ú
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void destroy()
	{
		glDeleteFramebuffers(1, &m_FBOID);
		glDeleteFramebuffers(1, &m_RBOID);
		m_FBOID = 0;
		m_RBOID = 0;
	}
};

