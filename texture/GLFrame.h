//================================================
/**
*@file:prepare.h
*��;��������ͳ�������
*

*��������ļ̳�����������º�����ʵ��
*																			
*  GLApplication * GLApplication::Create(const char * class_name)		
*		������������һ��ʵ��		
*																			
*  bool Init();														
*		ִ�����еĳ�ʼ������������ɹ���������true							
*																			
*  void Uninit();													
*		ִ�����е�ж�ع���										
*																			
*  void Update(DWORD milliseconds);										
*		ִ�����еĸ��²���������Ĳ���Ϊ���β���������ʱ�䣬�Ժ���Ϊ��λ
*																			
*  void Draw();															
*		ִ�����еĻ��Ʋ���
*/
//==============================================

#ifndef __GLFRAME_H__
#define __GLFRAME_H__

#include "GLWindow.h"

//���������
class Keys
{
public:
	Keys() { Clear(); }

	//������а�����Ϣ
	void Clear() { ZeroMemory(&m_keyDown, sizeof(m_keyDown)); }

	//�ж�ĳ�����Ƿ���
	bool IsPressed(unsigned int key)    { return (key < MAX_KEYS) ? (m_keyDown[key] == true) : false; }

	//����Ī��������
	void SetPressed(unsigned int key)   { if (key < MAX_KEYS) m_keyDown[key] = true; }

	//����ĳ�������ͷ�
	void SetReleased(unsigned int key)  { if (key < MAX_KEYS) m_keyDown[key] = false; }

private:
	static const unsigned int MAX_KEYS = 256;
	bool m_keyDown[MAX_KEYS];                    //����256������״̬
};

//���������࣬�̳���������opengl����
class GLApp
{
public:
	//����һ��ȫ�ֵ�Create����������������뱻�̳���ʵ��
	static GLApp *Create(LPCSTR class_name);  //�����Լ������ࡣ�����������ࣿ����������������

    //����������
	virtual ~GLApp() {};          //����һ�� "��"��ʲô��˼��������������������������������

protected:
	//���溯�����뱻�̳���ʵ�֣���ɻ�����opengl��Ⱦ����
	virtual bool Init() = 0;               //opengl��ʼ��
	virtual void Uninit() = 0;				//ж��
	virtual void Update(DWORD milliseconds) = 0;             //ִ��opengl����ĸ���
	virtual void Draw() = 0;				//����opengl����

	//ͨ�ú���
	void ToggleFullscreen();             //�л�ȫ������ģʽ
	void TerminateApp();				//������ѫ
	void ResizeDraw(bool enable) { m_resizeDraw = enable; }				//���ڴ�С�ı��ʱ�����
	
	//�������Ա
	Keys m_keys;

	GLApp(LPCSTR class_name);

private:
	//�������ѭ��
	//(��ǰʵ�������32λ��������λNULL0�����г���������У�������ʾ�������ʽ����С��/���/����)typedef CHAR *LPSTR,*PSTR
	friend int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
	int Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

	//��Ϣ����ص�����
	friend LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT Message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	static const UINT WM_TOGGLEFULLSCREEN = (WM_USER + 1);//�Զ�����Ϣ�ṹ�����л�����ģʽ��ʱ����

	GLWindow    m_window;                   //window��
	const char* m_className;				//������
	bool        m_isProgramLooping;			//����ѭ����ǣ����Ϊfalse���˳�ѭ��
	bool        m_createFullScreen,         //��Ϊtrue������ȫ��ģʽ
				m_isVisible,				//�����Ƿ�ɼ�
				m_resizeDraw;				//�Ƿ��ڴ��ڸı��С�µ����˻��ƺ���
	DWORD       m_lastTickCount;			//��һ�μ�������ֵ
};


#endif
