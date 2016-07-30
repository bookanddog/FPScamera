//=========================================================================
/**
*  @file      CameraRove.h
*
*  ��Ŀ������ ���������
*  �ļ�����:  ����ʵ���� 

*
*  ��������б����������¼����麯��
*																								
*	virtual bool Init();														
*		ִ�����еĳ�ʼ������������ɹ���������true							
*																			
*	virtual void Uninit();													
*		ִ�����е�ж�ع���										
*																			
*	virtual void Update(DWORD milliseconds);										
*		ִ�����еĸ��²���������Ĳ���Ϊ���β���������ʱ�䣬�Ժ���Ϊ��λ
*																			
*	virtual void Draw();															
*		ִ�����еĻ��Ʋ���
*/
//=========================================================================

#ifndef __CAMERA_ROVE_H__
#define __CAMERA_ROVE_H__


#include "stdafx.h"
#include "ImageLoad.h"
#include "GLFrame.h"												/**< ���������Ŀ���� */
#include "Font.h"
#include "Camera.h"


/** ��GL_Application������һ������ */
class CameraRove : GLApp						
{
public:
	bool	Init();							/**< ִ�����еĳ�ʼ������������ɹ���������true */
	void	Uninit();						/**< ִ�����е�ж�ع��� */
	void	Update(DWORD milliseconds);		/**< ִ�����еĸ��²���������Ĳ���Ϊ���β���������ʱ�䣬�Ժ���Ϊ��λ */
	void	Draw();							/**< ִ�����еĻ��Ʋ��� */
	
	bool    LoadTexture();                  /**< �������� */
    void    UpdateCamera();                 /**< ��������� */
	void    CaculateFrameRate();            /**< ����֡�� */
	void    PrintText();                    /**< ���������Ϣ */
	
	void    SetLight();                     /**< ���ù�Դ */
	void    DrawGrid();                     /**< ����������� */
	void    DrawSphere();                   /**< �������� */
	void    DrawBox();                      /**< ����ľ�� */
private:
	friend class GLApp;				/**< ����Ϊ����һ����Ԫ�࣬�����������������ʵ����������GL_Application * GL_Application::Create(const char * class_name) */
	CameraRove(const char * class_name);	/**< ���캯�� */

	/** �û��Զ���ĳ������ */ 
	BMPLoad    m_Texture;                     /**< λͼ������ */
	GLFont     m_Font;                        /**< ������ */
	Camera     m_Camera;                      /**< ������� */     
  	float      m_Fps;                         /**< ֡�� */
				
};


#endif	// __CAMERA_ROVE_H__