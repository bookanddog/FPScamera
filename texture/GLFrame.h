//================================================
/**
*@file:prepare.h
*用途：键盘类和程序框架类
*

*必须在你的继承类中完成以下函数的实现
*																			
*  GLApplication * GLApplication::Create(const char * class_name)		
*		创建你的子类的一个实例		
*																			
*  bool Init();														
*		执行所有的初始化工作，如果成功函数返回true							
*																			
*  void Uninit();													
*		执行所有的卸载工作										
*																			
*  void Update(DWORD milliseconds);										
*		执行所有的更新操作，传入的参数为两次操作经过的时间，以毫秒为单位
*																			
*  void Draw();															
*		执行所有的绘制操作
*/
//==============================================

#ifndef __GLFRAME_H__
#define __GLFRAME_H__

#include "GLWindow.h"

//定义键盘类
class Keys
{
public:
	Keys() { Clear(); }

	//清空所有按键信息
	void Clear() { ZeroMemory(&m_keyDown, sizeof(m_keyDown)); }

	//判断某个键是否按下
	bool IsPressed(unsigned int key)    { return (key < MAX_KEYS) ? (m_keyDown[key] == true) : false; }

	//设置莫个键按下
	void SetPressed(unsigned int key)   { if (key < MAX_KEYS) m_keyDown[key] = true; }

	//设置某个键被释放
	void SetReleased(unsigned int key)  { if (key < MAX_KEYS) m_keyDown[key] = false; }

private:
	static const unsigned int MAX_KEYS = 256;
	bool m_keyDown[MAX_KEYS];                    //保存256个键的状态
};

//基本程序类，继承它来创建opengl程序
class GLApp
{
public:
	//创建一个全局的Create函数，这个函数必须被继承类实现
	static GLApp *Create(LPCSTR class_name);  //创建自己的子类。，这里问题多多？？？？？？？？？

    //虚析构函数
	virtual ~GLApp() {};          //这里一个 "；"，什么意思？？？？？？？？？？？？？？？？

protected:
	//下面函数必须被继承类实现，完成基本的opengl渲染过程
	virtual bool Init() = 0;               //opengl初始化
	virtual void Uninit() = 0;				//卸载
	virtual void Update(DWORD milliseconds) = 0;             //执行opengl程序的更新
	virtual void Draw() = 0;				//绘制opengl场景

	//通用函数
	void ToggleFullscreen();             //切换全屏窗口模式
	void TerminateApp();				//结束程勋
	void ResizeDraw(bool enable) { m_resizeDraw = enable; }				//窗口大小改变的时候绘制
	
	//按键类成员
	Keys m_keys;

	GLApp(LPCSTR class_name);

private:
	//程序的主循环
	//(当前实例句柄，32位废弃不用位NULL0，运行程序的命令行，程序显示的最初方式：最小化/最大化/正常)typedef CHAR *LPSTR,*PSTR
	friend int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
	int Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

	//消息处理回调函数
	friend LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT Message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	static const UINT WM_TOGGLEFULLSCREEN = (WM_USER + 1);//自定义消息结构，在切换窗口模式的时候发送

	GLWindow    m_window;                   //window类
	const char* m_className;				//程序名
	bool        m_isProgramLooping;			//程序循环标记，如果为false，退出循环
	bool        m_createFullScreen,         //若为true，创建全屏模式
				m_isVisible,				//窗口是否可见
				m_resizeDraw;				//是否在窗口改变大小事调用了绘制函数
	DWORD       m_lastTickCount;			//上一次计数器的值
};


#endif
