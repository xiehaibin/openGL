#include "Lesson03_CompressTex.h"
#include <string>

/*
opengl压缩纹理
opengl2.0以后出来的，早期在D3D里出现的dds
目的：提升绘制的效率，性能.(传输效率高，硬件支持)
1.ETC1         android
2.PVRTC        ios
3.ATITC        高通cpu
4.S3TC         PC支持
*/
void Lesson03_CompressTex::onDisable()
{

}
void Lesson03_CompressTex::initGL()
{
	// 判断是否支持这个纹理压缩格式
	std::string extensions = (const char*)glGetString(GL_EXTENSIONS);
	bool has = extensions.find("GL_EXT_texture_compression_s3tc") != std::string::npos;
	// 也可以通过函数指针判断。认为用glew扩展库
	if (GL_EXT_texture_compression_s3tc)
	{
		int a = 0;
	}

	// 创建压缩纹理
	//glCompressedTexImage2DARB(); 
}

void Lesson03_CompressTex::render()
{

}
