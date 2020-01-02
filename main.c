
#include <AT89X55.H>
#include "sysos.h"
#include "key.h"
#include "menu.h"
#include "seg7font.h"
#include "SegDynamicScan.h"

#define BIT0 (1<<0)
#define BIT1 (1<<1)
#define BIT2 (1<<2)
#define BIT3 (1<<3)
#define BIT4 (1<<4)
#define BIT5 (1<<5)
#define BIT6 (1<<6)
#define BIT7 (1<<7)

unsigned short config[10];
unsigned char aKey;

//�˵�����
code const CMenuItem menuList[]=
{
	{{0xd4,0,0x06,0},MENU_VALUTYPE_U16,-9999,9999,&config[0],CValueMenu_Proc},
	{{0xd4,0,0x5b,0},MENU_VALUTYPE_U16,0,100,&config[1],CValueMenu_Proc},
	{{0xd4,0,0x4f,0},MENU_VALUTYPE_U16,0,100,&config[2],CValueMenu_Proc},
	{{0xd4,0,0x66,0},MENU_VALUTYPE_U16,0,100,&config[3],CValueMenu_Proc},
	{{0xd4,0,0x6d,0},MENU_VALUTYPE_U16,0,100,&config[4],CValueMenu_Proc},
	{{0xd4,0,0x7d,0},MENU_VALUTYPE_U16,0,100,&config[5],CValueMenu_Proc},
	{{0xd4,0,0x07,0},MENU_VALUTYPE_U16,0,100,&config[6],CValueMenu_Proc},
	{{0xd4,0,0x7f,0},MENU_VALUTYPE_U16,0,100,&config[7],CValueMenu_Proc},
	{{0xd4,0,0x6f,0},MENU_VALUTYPE_U16,0,100,&config[8],CValueMenu_Proc},
	{0,0,0,0,0,0}
};

unsigned char keyTimer,cursorTimer;

//1ms��ʱ������
#define TIME_TIME0_SYSTEM 0xff00

void systemInit(void)
{
	//��ʼ����ʱ��0
	//��ʱ��0��Ϊ����������ж�ʱ��
	TMOD = BIT0|BIT4;		//��ʱ��0��1��Ϊģʽ1
	TR0 = 0;
	TH0 = (unsigned char)(TIME_TIME0_SYSTEM>>8);
	TL0 = (unsigned char)TIME_TIME0_SYSTEM;
	ET0 = 1;
	TR0 = 1;

	EA = 1;
}
void main(void)
{

	systemInit();
	CMenu_MenuInit(menuList);
	
	while (1)
	{
		OS_BLOCK_BEGIN(taskMask.m_timermS)
		{
			//ɨ����ʾ
			SegDynamicScan();

			if (++keyTimer > 10)
			{
				keyTimer = 0;
				OS_TASK_READY(taskMask.m_keyScan);
			}
			if (++cursorTimer>50)
			{
				cursorTimer=0;
				OS_TASK_READY(taskMask.m_cursor);
			}
		}
		OS_BLOCK_END();
		//����ɨ��
		OS_BLOCK_BEGIN(taskMask.m_keyScan)
		{
			P2 = 0xff;

			if (!P2_0) {aKey=WM_KEY_ADD;}
			else if (!P2_1){aKey = WM_KEY_SET;}
			else if (!P2_2){aKey = WM_KEY_SUB;}
			else if (!P2_3){aKey = WM_KEY_LEFT;}
			else if (!P2_4){aKey = WM_KEY_RIGHT;}
			else {aKey = WM_KEY_NULL;}

			CMenu_KeyEvent(aKey);
		}
		OS_BLOCK_END()
		//���
		OS_BLOCK_BEGIN(taskMask.m_cursor)
		{
			CMenu_CursorEvent();
		}
		OS_BLOCK_END()
		
	}
}

void interrupt_time0_handle(void) interrupt 1
{
	TR0 = 0;
	TH0 = (unsigned char)(TIME_TIME0_SYSTEM>>8);
	TL0 = (unsigned char)TIME_TIME0_SYSTEM;
	TR0 = 1;
	OS_TASK_READY(taskMask.m_timermS);
}
//�˵���ʼ
void CMenu_Start(void)
{
}
//�˵�����
void CMenu_Quit(void)
{
	CMenu_MenuInit(menuDC.theMenu.m_pMenuList);
}

