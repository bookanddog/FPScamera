//======================================================================
/**
*  @file      GLFrame.cpp
*
*  项目描述： 纹理映射
*  文件描述:  键盘类和程序框架类 
*	
*  必须在继承类中完成以下函数的实现
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
//======================================================================

#include "stdafx.h"
#include "GLFrame.h"	

//主程序入口
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int ret = -1;
	GLApp * app1 = GLApp::Create("opengl");              //创建程序类
	if(app1 != 0)
	{
		ret = app1->Main(hInstance, hPrevInstance, lpCmdLine, nCmdShow); //指向程序主循环
		delete app1;        ///**< 删除程序类（在继承类中，使用GL_Example * example = new GL_Example(class_name);分配了一块内存）*/
	}
	else
	{
		MessageBoxA(HWND_DESKTOP, "创建程序出错", "Error", MB_ICONEXCLAMATION);
	}
	return ret;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg,WPARAM wParam, LPARAM lParam)
{
	LONG user_data = GetWindowLong(hWnd, GWL_USERDATA);/**< 返回用户自定义的32位的程序附加值 */
	if (user_data == 0)
	{
		//如果程序第一次运行
		//WM_CREATE当一个应用程序通过CreateWindowEx函数或者CreateWindow函数请求创建窗口时发送此消息，(此消息在函数返回之前发送)。
       //产生时间是窗口创建之后显示之前，同时WM_CREATE也必须是不进队列消息。
		if (uMsg == WM_CREATE)
		{
			/// 返回窗口结构的指针，它保存刚创建的程序实例的类
			CREATESTRUCT  *creation = reinterpret_cast<CREATESTRUCT *>(lParam);

			//获得程序实例的指针
			GLApp *app1 = reinterpret_cast<GLApp *>(creation->lpCreateParams);

			//保存程序实例的指针为用户自定义的程序的程序附加值
			SetWindowLong(hWnd, GWL_USERDATA, reinterpret_cast<LONG>(app1));
			app1->m_isVisible = true;         //设置程序可见
			return 0;
		}
	}
	else
	{
		//如果不是第一次窗口，返回程序实例指针
		GLApp *app1 = reinterpret_cast<GLApp *>(user_data);
		return app1->Message(hWnd, uMsg, wParam, lParam);  //调用程序实例自己的消息处理函数

	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);        ///**< 调用默认的窗口消息处理函数 */
}

GLApp::GLApp(LPCSTR class_name)
{
	m_className = class_name;											/**< 保存类名 */
	m_isProgramLooping = true;											/**< 设置程序循环为true */
	m_createFullScreen = true;											/**< 使用全屏模式 */
	m_isVisible = false;												/**< 不可见 */
	m_resizeDraw = false;												/**< 在窗口改变大小的时候，不可绘制 */
	m_lastTickCount = 0;
}

void GLApp::ToggleFullscreen()             //切换  <全屏 / 窗口> 模式
{
	PostMessage(m_window, WM_TOGGLEFULLSCREEN, 0, 0);   //发送自定的切换消息
}

void GLApp::TerminateApp()                                //结束程序
{
	PostMessage(m_window, WM_QUIT, 0, 0);                 //发送退出消息
	m_isProgramLooping = false;
}

//消息循环
LRESULT GLApp::Message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_SYSCOMMAND:                          /**< 截获系统命令 */
		switch(wParam)
		{
		case SC_SCREENSAVE:                        /**< 截获屏幕保护启动命令 */
		case SC_MONITORPOWER:                      /**< 截获显示其省电模式启动命令 */
			return 0;                          //不启用这两个，命令
		    break;
		}
		break;

	case WM_CLOSE:                              //关闭窗口
		TerminateApp();                       //调用函数
		return 0;
		break;

	case WM_EXITMENULOOP:
	case WM_EXITSIZEMOVE:
		m_lastTickCount = GetTickCount();       //更新计数器的值
		return 0;
		break;

	case WM_MOVE:
			m_window.SetPosX(LOWORD(lParam));							/**< 更新鼠标的坐标 */
			m_window.SetPosY(HIWORD(lParam));							
			return 0;
		break;

	case WM_PAINT:
			if (m_resizeDraw == true)									/**< 如果需要重绘 */
			{
				m_window.ReshapeGL();									/**< 重新设置窗口的大小 */
				Draw();													/**< 重新绘制 */
				m_window.SwapBuffers();									/**< 交换前后帧缓存 */
			}
		break;

	case WM_SIZING:													/**< 窗口正在改变大小 */
		{
			RECT * rect = (RECT *)lParam;
			m_window.SetWidth(rect->right - rect->left);				/**< 设置窗口宽度 */
			m_window.SetHeight(rect->bottom - rect->top);				/**< 设置窗口高度 */
			return TRUE;
		}
		break;

	case WM_SIZE:													/**< 窗口改变大小后 */
			switch (wParam)												/**< 处理不同的窗口状态 */
			{
				case SIZE_MINIMIZED:									/**< 是否最小化? */
					m_isVisible = false;								/**< 如果是，则设置不可见 */
					return 0;											
				break;

				case SIZE_MAXIMIZED:									/**< 窗口是否最大化? */
				case SIZE_RESTORED:										/**< 窗口被还原? */
					m_isVisible = true;									/**< 设置为可见 */
					m_window.SetWidth(LOWORD(lParam));					/**< 设置窗口宽度 */
					m_window.SetHeight(HIWORD(lParam));					/**< 设置窗口高度 */
					m_window.ReshapeGL();								/**< 改变窗口大小 */
					m_lastTickCount = GetTickCount();					/**< 更新计数器的值 */
					return 0;											
				break;
			}
		break;			

	case WM_KEYDOWN:												/**< 更新按键信息 */
			m_keys.SetPressed(wParam);									
			return 0;													
		break;

	case WM_KEYUP:													/**< 更新释放键信息 */
			m_keys.SetReleased(wParam);									
			return 0;													
		break;

	case WM_TOGGLEFULLSCREEN:										/**< 切换 全屏/窗口模式 *///为什么切换屏幕了还发送退出消息
			m_createFullScreen = !m_createFullScreen;                 //有一个循环，关闭窗口是程序没有停止，继续创建新窗口  get
			PostMessage(hWnd, WM_QUIT, 0, 0);                         //好像是没有这个函数就窗口就无法关闭？并且没有新消息不进行处理？*/
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);					/**< 调用默认的窗口消息处理函数 */
}

///** 程序的主循环 */
int GLApp::Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	/// 注册一个窗口
	WNDCLASSEXA windowClass;                                             /**< 窗口类 */
	ZeroMemory(&windowClass, sizeof(WNDCLASSEXA));                        /**< 清空结构为0 */
	windowClass.cbSize			= sizeof(WNDCLASSEXA);					/**< 窗口结构的大小 */
	windowClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	/**< 设置窗口类型为，移动时重画，并为窗口取得DC */
	windowClass.lpfnWndProc		= (WNDPROC)(WndProc);				    /**< WndProc处理消息 */
	windowClass.hInstance		= hInstance;							/**< 设置实例 */
	windowClass.hbrBackground	= (HBRUSH)(COLOR_APPWORKSPACE);			/**< 设置背景 */
	windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);			/**< 载入光标 */
	windowClass.lpszClassName	= m_className;							/**< 设置类名 */

	if (RegisterClassExA(&windowClass) == 0)								/**< 尝试注册窗口类 */
	{																	/**< NOTE: Failure, Should Never Happen */
		MessageBoxA(HWND_DESKTOP, "注册窗口失败!", "Error", MB_OK | MB_ICONEXCLAMATION);
		return -1;														/**< 退出并返回FALSE */
	}

	// 询问是否在全屏状态下运行?
	if (MessageBoxA(HWND_DESKTOP, "你想在全屏状态下运行么 ?", "设置运行模式", MB_YESNO | MB_ICONQUESTION) == IDNO)
	{
		m_createFullScreen = false;										/**< m_CreateFullScreen记录当前的显示模式为窗口 */
	}
	
	while (m_isProgramLooping)											/**< 循环直到WM_QUIT退出程序 */
	{																	
		/// 创建一个窗口
		if (m_window.Create("OpenGL 3D游戏编程——纹理映射", m_className, m_createFullScreen, hInstance, this) == true)
		{	
			
			/// 如果初始化失败，则退出
			if (Init() == false)									    /**< 调用自定义的初始化函数 */
			{															/**< 失败 */
				TerminateApp();							         		/**< 关闭窗口退出程序 */
			}
			else														/**< 成功开始消息循环 */
			{															 
				MSG msg;												/**< Window消息结构 */ 
				bool isMessagePumpActive = true;						/**< 当消息不为空时，处理消息循环 */
				m_lastTickCount = GetTickCount();						/**< 返回当前的计时器的值 *///从操作系统启动所经过的毫秒数，它的返回值是DWORD。
				m_keys.Clear();											/**< 清空所有的按键信息 */
				while (isMessagePumpActive == true)						/**< 当消息不为空时，处理消息循环 */
				{														/**< 成功创建窗口，检测窗口消息 */
					if (PeekMessage(&msg, m_window, 0, 0, PM_REMOVE) != 0)
					{													
						/// 检测是否为WM_QUIT消息
						if (msg.message != WM_QUIT)						
						{
							DispatchMessage(&msg);						/**< 如果不是,发送消息到消息回调函数中处理 */
						}
						else											
						{
							isMessagePumpActive = false;				/**< 如果是，则退出 */
						}
					}
					/// 如果没有消息
					else												
					{
						if (m_isVisible == false)						/**< 如果窗口不可见 */
						{
							WaitMessage();								/**< 暂停程序，等待消息 */
						}
						else											/**< 如果窗口可见 */
						{												/**< 执行消息循环 */
							DWORD tickCount = GetTickCount();			/**< 返回计时器的当前值 */
							Update(tickCount - m_lastTickCount);		/**< 调用用户自定义的更新函数 */
							m_lastTickCount = tickCount;				/**< 重新设置上一次，计数器的值 */
							Draw();										/**< 调用用户自定义的绘制函数 */
							m_window.SwapBuffers();						/**< 交换前后帧缓存 */
						}
					}
				}														/**< 如果isMessagePumpActive == true，则循环 */
			}															
																		/**< 程序结束 */
			Uninit();												/**< 用户自定义的卸载函数 */
			m_window.Destroy();											/**< 删除窗口 */
		}
		else															/**< 如果创建窗口失败 */
		{																
			MessageBox(HWND_DESKTOP, TEXT("创建OpenGL窗口错误"), TEXT("Error"), MB_OK | MB_ICONEXCLAMATION);
			m_isProgramLooping = false;									/**< 停止程序循环 */
		}
	}																	

	UnregisterClassA(m_className, hInstance);							/**< 取消注册的窗口 */
	return 0;
}