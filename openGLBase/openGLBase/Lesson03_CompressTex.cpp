#include "Lesson03_CompressTex.h"
#include <string>

/*
openglѹ������
opengl2.0�Ժ�����ģ�������D3D����ֵ�dds
Ŀ�ģ��������Ƶ�Ч�ʣ�����.(����Ч�ʸߣ�Ӳ��֧��)
1.ETC1         android
2.PVRTC        ios
3.ATITC        ��ͨcpu
4.S3TC         PC֧��
*/
void Lesson03_CompressTex::onDisable()
{

}
void Lesson03_CompressTex::initGL()
{
	// �ж��Ƿ�֧���������ѹ����ʽ
	std::string extensions = (const char*)glGetString(GL_EXTENSIONS);
	bool has = extensions.find("GL_EXT_texture_compression_s3tc") != std::string::npos;
	// Ҳ����ͨ������ָ���жϡ���Ϊ��glew��չ��
	if (GL_EXT_texture_compression_s3tc)
	{
		int a = 0;
	}

	// ����ѹ������
	//glCompressedTexImage2DARB(); 
}

void Lesson03_CompressTex::render()
{

}
