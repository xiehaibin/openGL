#pragma once

/*
* 基础图元
* @date: 2021/7/3
* @author Hayden
*/
class Lesson01
{
public:
	Lesson01();
	void Render();

private:
	void DrawPoint();
	void DrawLine();
	void DrawTriangle();
	void DrawSpline();
	void DrawBesselCurve();
	void DrawImage();
	void OpenGLEx();
};
// 垂直同步：绘制的刷新率与屏幕的刷新率一直

