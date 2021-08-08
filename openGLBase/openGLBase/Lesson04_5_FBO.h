#pragma once
#include "GLContext.h"
#include "FrameBufferObject.h"

/*
FBO(FrameBuffer0bject 帧缓存对象) opengl扩展（openGL2.0以上） 离屏渲染
从屏幕读取数据
1.不需要创建额外的openGL上下文
2.灵活（可以创建那些缓冲区，不创建那些缓冲区），占用内存少
3.不保存状态，缓冲区可以动态指定 pbuffer创建后不能改
*/
class Lesson04_5_FBO
{
public:
	void initGL();
	void onDisable();
	void render();

private:
	unsigned int  createTextureFromImage(const char* fileName);
	unsigned int  createTexture(int w, int h, const void* data);
	void          createVbo();
	void          createIbo();
	void          drawCude(bool isRotate);
	bool          saveImage(int w, int h, char* data);

public:
	GLuint  m_dynamicTexId;
	GLuint  m_unTextureId;
	GLuint  m_unVbo;
	GLuint  m_unIbo;

	FrameBufferObject m_fbo;
};

