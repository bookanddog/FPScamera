//=========================================================================
/**
*  @file      SkyAndTerrain.h
*
*  ��Ŀ������ ������պ͵���
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

#ifndef __SKY_AND_TERRAIN_H__
#define __SKY_AND_TERRAIN_H__

#include "stdafx.h"
#include "CBMPLoader.h"
#include "GLFrame.h"												/**< ���������Ŀ���� */
#include "Font.h"
#include "Camera.h"
#include "SkyBox.h"
#include "Terrain.h"


/** ��GL_Application������һ������ */
class SkyTerrain : GLApplication								
{
public:
	bool	Init();							/**< ִ�����еĳ�ʼ������������ɹ���������true */
	void	Uninit();						/**< ִ�����е�ж�ع��� */
	void	Update(DWORD milliseconds);		/**< ִ�����еĸ��²���������Ĳ���Ϊ���β���������ʱ�䣬�Ժ���Ϊ��λ */
	void	Draw();							/**< ִ�����еĻ��Ʋ��� */
	
	void    UpdateCamera();                 /**< ��������� */
	void    CaculateFrameRate();            /**< ����֡�� */
	void    PrintText();                    /**< ���������Ϣ */
		
	
private:
	friend class GLApplication;				/**< ����Ϊ����һ����Ԫ�࣬�����������������ʵ����������GL_Application * GL_Application::Create(const char * class_name) */
	SkyTerrain(const char * class_name);	/**< ���캯�� */

	/** �û��Զ���ĳ������ */ 
	
	CTerrain   m_Terrain;                     /**< ������ */
	CSkyBox    m_SkyBox;                      /**< ����� */
	GLFont     m_Font;                        /**< ������ */
	Camera     m_Camera;                      /**< ������� */     
  	float      m_Fps;                         /**< ֡�� */
	bool       m_RenderMode;		          /**< ����ģʽ */
	bool       sp;                            /**< �ո���Ƿ��ͷ� */

};


#endif	// __SKY_AND_TERRAIN_H__
