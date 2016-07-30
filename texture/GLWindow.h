//==================================
/**
*@file  : glWindows.h
*描述： 窗口类
*
*
*
*/
//=================================

#ifndef __GLWIDOW_H__
#define __GLWINDOW_H__

#include "stdafx.h"

//窗口类
class GLWindow
{
	public:
		GLWindow();
		//~glWindows();

		//创建窗口（窗口标题，类名字？，是否全屏，程序当前实例句柄，传入程序类参数）
		bool Create(LPCSTR window_title, LPCSTR class_name, bool fullscreen,HINSTANCE hinstance, LPVOID lpParam );

		//删除窗口
		void Destroy();

		//改变窗口显示设置
		bool ChangeScreenSetting();

		//根据窗口大小调整opengl
		void ReshapeGL();

		//交换缓冲区           ::前面不跟类/命名空间就表示”普通空间“
		void SwapBuffers() {::SwapBuffers(m_hDC);}

		//设置窗口左上角位置,此处重载，针对各种类型的x，y.    往后去应该是要加一个函数setPosXY（x,y）？？
		void SetPosX(int x);
		void SetPosX(unsigned short x) { SetPosX((signed short)x); }
		void SetPosY(int y);
		void SetPosY(unsigned short y) { SetPosY((signed short)y); }

		//返回窗口大小
		int GetWidth();
		int GetHeight();

		//设置窗口大小,  往后去是跟设置窗口合并四个参数一步到位？？？？
		void SetWidth(int width);
		void SetHeight(int height);

		//返回窗口左上角位置
		int GetPosX();
		int GetPosY();

		//设置窗口的颜色位深
		void SetHiColor()   { m_bitPerPixel = 16; }
		void SetTrueColor() { m_bitPerPixel = 32; }

		//重载运算符 ，让可以让GL_Window m_Window声明后的m_Window作为窗口句柄使用 
		//重载的是类型转化运算符好像？？
		operator HWND() { return m_hWnd; }

	private:
		HWND     m_hWnd;                 //窗口句柄
		HDC      m_hDC;                  //设备描述表
		HGLRC    m_hRC;                  //opengl渲染描述表

		int      m_windowPosX,           //窗口左上角位置
				 m_windowPosY,
                 m_windowWidth,          //窗口宽高
				 m_windowHeight,
				 m_screenWidth,           //全屏宽高
				 m_screenHeight,
				 m_bitPerPixel;           //颜色位深
		bool     m_isFullScreen;          //是否全屏

};

#endif