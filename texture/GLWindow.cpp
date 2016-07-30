//========================================================
/**
*  @file      glWindows.cpp
*
*  项目描述： 纹理映射
*  文件描述:  程序窗口类 
*/     
//========================================================

#include "GLWindow.h"	


//构造函数
GLWindow::GLWindow()
{
	m_windowPosX = 0;
	m_windowPosY = 0;
	m_windowWidth = 800;
	m_windowHeight = 600;
	m_screenWidth = 1366;
	m_screenHeight =768;
	m_bitPerPixel = 32;
	m_isFullScreen =false;

	m_hWnd = 0;
	m_hDC = 0;
	m_hRC = 0;
}

//返回窗口的大小
int GLWindow::GetWidth()
{
	if(m_isFullScreen == true)
		return m_screenWidth;
	else
		return m_windowWidth;
}

int GLWindow::GetHeight()
{
	if (m_isFullScreen == true)
	{
		return m_screenHeight;
	}
	else
	{
		return m_windowHeight;
	}
}

/** 设置窗口的大小 */
void GLWindow::SetWidth(int width)
{
	if (m_isFullScreen == true)
	{
		m_screenWidth = width;
	}
	else
	{
		m_windowWidth = width;
	}
}
void GLWindow::SetHeight(int height)
{
	if (m_isFullScreen == true)
	{
		m_screenHeight = height;
	}
	else
	{
		m_windowHeight = height;
	}
}

/** 返回窗口左上角的位置 */
int GLWindow::GetPosX()
{
	if (m_isFullScreen == false)
	{
		return m_windowPosX;
	}
	return 0;
}
int GLWindow::GetPosY()
{
	if (m_isFullScreen == false)
	{
		return m_windowPosY;
	}
	return 0;
}

/** 设置窗口左上角的位置 */
void GLWindow::SetPosX(int x)
{
	if (m_isFullScreen == false)
	{
		m_windowPosX = x;
	}
}
void GLWindow::SetPosY(int y)
{
	if (m_isFullScreen == false)
	{
		m_windowPosY = y;
	}
}

//当窗口大小改变时，通知opengl改变大小
void GLWindow::ReshapeGL()
{
	GLsizei width = GetWidth();
	GLsizei height = GetHeight();
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//缺省值时视点是在原点的
	gluPerspective(45.0, (GLfloat)width/(GLfloat)height, 1.0, 4000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/** 改变窗口的显示设置 */
bool GLWindow::ChangeScreenSetting()
{
	DEVMODE dmScreenSettings;
	ZeroMemory(&dmScreenSettings, sizeof(DEVMODE));
	dmScreenSettings.dmSize       = sizeof(DEVMODE);
	dmScreenSettings.dmPelsWidth  = GetWidth();
	dmScreenSettings.dmPelsHeight = GetHeight();
	dmScreenSettings.dmBitsPerPel = m_bitPerPixel;           //设置位深
	//dmScreenSettings.dmDisplayFrequency = 75;                           /**< 设置屏幕刷新率 */
	dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;//| DM_DISPLAYFREQUENCY;频率

	//改变显示模式//把缺省显示设备的设置改变为由lpDevMode设定的图形模式，要改变一个特定显示设备的设置，请使用ChangeDisplaySettingsEx函数。
	//0：表明当前屏幕的图形模式要动态地改变
	if(ChangeDisplaySettings(&dmScreenSettings, 0) != DISP_CHANGE_SUCCESSFUL) 
	{ 
		return false;
	}
	return true;
}

//创建窗口
bool GLWindow::Create(LPCSTR window_title, LPCSTR class_name, bool fullscreen, HINSTANCE hinstance, LPVOID lpParam)
{
	m_isFullScreen = fullscreen;
	int nX = 0,
		nY = 0;
	PIXELFORMATDESCRIPTOR pfd = 
	{                                           //设置相熟描述结构
		sizeof(PIXELFORMATDESCRIPTOR),         //像素结构的大小
		1,                                     //版本号
		PFD_DRAW_TO_WINDOW |                  //相熟缓冲区属性：缓冲区的输出显示在一个窗口中
		PFD_SUPPORT_OPENGL |                  //                 缓冲区支持opengl绘图
		PFD_STEREO         |                  //                颜色缓存区是立体缓存
		PFD_DOUBLEBUFFER,                     //                 颜色缓存区是双缓存
		PFD_TYPE_RGBA,                       //使用rgba颜色格式
		m_bitPerPixel,                       //颜色缓冲区中颜色值所占的位深
		0, 0, 0, 0, 0, 0,                   //使用默认的颜色设置
		0,                                   //无alpha缓存
		0,                                  //颜色缓冲区中alpha成分的移位计数
		0,									/**< 无累计缓存区 */
		0, 0, 0, 0,							/**< 累计缓存区无移位 *///积累缓冲区的rgba位数
		32,                                //深度缓冲区位数
		0,					     			/**< 无蒙版缓存 *///模板缓冲区位数
		0,						    		/**< 无辅助缓存区 *///辅助缓冲区个数
		PFD_MAIN_PLANE,                    // 必须为PFD_MAIN_PLANE，设置为主绘图层 */、已废弃不用
		0,                                  //表示OpenGL实现所支持的上层或下层平面的数量 */下层平面的透明颜色
		0, 0, 0					           /**< 过时，已不再使用 *///
	};

	///**< 定义我们窗口类型，使用常规设定，去掉最大化按钮，并不能改变窗体大小 */
	DWORD windowStyle = WS_OVERLAPPEDWINDOW; //& ~WS_SIZEBOX & ~WS_MAXIMIZEBOX;  //WS_OVERLAPPEDWINDOW &
	DWORD windowExtendStyle = WS_EX_APPWINDOW;

	if(m_isFullScreen == true)
	{
		if(ChangeScreenSetting() == false)
		{
			MessageBox(HWND_DESKTOP, TEXT("模式装换失败。\n在窗口模式下运行。"), TEXT("错误"), MB_OK | MB_ICONEXCLAMATION);
			m_isFullScreen = false;
		}
		else
		{
			ShowCursor(true);
			windowStyle = WS_POPUP;                    //设置窗口模式为顶层窗口
			windowExtendStyle |= WS_EX_TOPMOST; 
		}
	}

	//调整窗口区的大小，使其客户区的大小为我们设置的大小
	RECT windowRect = {GetPosX(), GetPosY(), GetPosX() + GetWidth(), GetPosY() + GetHeight()};
	if(m_isFullScreen = false)          //窗口模式
	{
		windowExtendStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;// | WS_EX_CLIENTEDGE;        //窗口具有3d外观
		int wid = GetSystemMetrics(SM_CXSCREEN);                    //获取当前屏幕宽高
		int hei = GetSystemMetrics(SM_CYSCREEN);
		nX = (wid - GetWidth()) / 2;                    /**< 计算窗口居中用 */
		nY = (hei - GetHeight()) / 2;			//屏幕中心点

		/// 调整我们窗口的大小，使其客户区的大小为我们设置的大小。计算出的窗口矩形随后可以传送给CreateWindowEx函数，用于创建一个客户区所需大小的窗口。
		AdjustWindowRectEx(&windowRect, windowStyle, 0, windowExtendStyle);
		/// 判断窗口的左上角是否隐藏在桌面外
		if(windowRect.left < 0)
		{
			windowRect.right -= windowRect.left;						
			windowRect.left = 0;	
		}
		if (windowRect.top < 0)											/**< 如果窗口Y坐标为负，移动坐标到0处，并调整窗口的位置 */
		{
			windowRect.bottom -= windowRect.top;						
			windowRect.top = 0;											
		}
	}

	//创建窗口
	m_hWnd = CreateWindowExA(windowExtendStyle,                        //窗口的扩展风格
		                    class_name,                              //窗口的类名
							window_title,                             //窗口标题
							windowStyle,                             //窗口风格
							nX,nY,                                  /**< 窗口的左上角位置 *///这貌似是窗口中心位置啊
							                                       //应该是windowRect.left,windowRect.top
							windowRect.right - windowRect.left,			/**< 窗口的宽度 */
							windowRect.bottom - windowRect.top,			/**< 窗口的高度 */
							HWND_DESKTOP,								/**< 窗口的父窗口为桌面 */
							0,											/**< 无菜单 */
							hinstance,									/**< 传入窗口的实例句柄 */
							lpParam);									/**< 传入程序类参数 */

	while (m_hWnd != 0)
	{
		m_hDC = GetDC(m_hWnd);//窗口设备描述符
		if (m_hDC == 0)
		{
			break;
		}

		GLuint pixelFormat = ChoosePixelFormat(m_hDC, &pfd);         /**< 查找一个兼容的像素格式 */
		if (pixelFormat == 0)
		{
			break;
		}
		if (SetPixelFormat(m_hDC, pixelFormat, &pfd) == false)        /**< 设置像素格式 */
		{
			break;
		}

		m_hRC = wglCreateContext(m_hDC);                             /**< 创建OpenGL的渲染描述表 */
		if (m_hRC == 0)													/**< 如果为空 */
		{																/**< 失败 */
			break;														
		}
		if (wglMakeCurrent(m_hDC, m_hRC) == false)                     /**< 设置当前的OpenGL的渲染对象为当前的窗口 */
		{																/**< 失败 */
			break;														
		}
		
		//该函数设置指定窗口的显示状态
		ShowWindow(m_hWnd, SW_NORMAL);
		ReshapeGL();
		return true;													/**< 成功返回 */
	}
	Destroy();															/**< 释放资源 */
	return false;														/**< 返回失败 */
}

/** 删除OpenGL窗口 */
void GLWindow::Destroy()	
	{
	if (m_hWnd != 0)										/**< 窗口句柄是否存在 */
	{
		if (m_hDC != 0)										/**< 窗口设备描述表是否存在 */
		{
			wglMakeCurrent(m_hDC, 0);						/**< 设置当前的OpenGL的渲染对象为空NULL */
			if (m_hRC != 0)									/**< OpenGL的渲染描述表是否存在 */
			{
				wglDeleteContext(m_hRC);					/**< 释放OpenGL的渲染描述表 */
				m_hRC = 0;									/**< 设置OpenGL的渲染描述表为0 */
			}
			ReleaseDC(m_hWnd, m_hDC);						/**< 释放窗口的设备描述表 */
			m_hDC = 0;										/**< 设置窗口的设备描述表为0 */
		}
		DestroyWindow(m_hWnd);								/**< 删除窗口 */
		m_hWnd = 0;											/**< 设置窗口句柄为0 */
	}

	if (m_isFullScreen)										/**< 如果为全屏模式，在程序结束后，变换到窗口模式，并显示鼠标 */
	{
		ChangeDisplaySettings(NULL, 0);						/**< 变换到窗口模式 */
		ShowCursor(true);									/**< 显示鼠标 */
	}
}
