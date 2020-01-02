/***********************************************************************************************
�ļ���	SYSOS.h
����	2013.1.13
����	��ʫ��	�¶���
�޶�	2013.1.13
�ļ�˵��	Э��ʽ������OS
�޶�˵��	��ʼ�汾
2013.06.07	��ʫ��	���������˳���
2013.08.06	��ʫ��	���ӿ��������ͳ����������ͳ��CPUʹ����
***********************************************************************************************/
#ifndef _SYS_OS_ZX
#define _SYS_OS_ZX

/*ͷ�ļ�--------------------------------------------------------------------------------------*/
//#include ""

/*�û�����------------------------------------------------------------------------------------*/
//�����־����
typedef struct
{
	unsigned char m_timermS:1;
	unsigned char m_keyScan:1;
	unsigned char m_cursor:1;
}OS_CTaskMask;

//��������
#define OS_01_STATITASK		0			/*ʹ��ͳ��CPUʹ����*/
/*�����--------------------------------------------------------------------------------------*/

//����鿪ʼ
#define OS_BLOCK_BEGIN(X) 		\
	if((volatile)(X))			\
	{							\
		(X)=0;					\
		
//��������
#define OS_BLOCK_END() 			\
	continue;					\
	}							\

//��������
#define OS_TASK_END()	continue

//�����־��Ч
#define OS_TASK_READY(X) ((X)=1)
//��������
#define OS_TASK_BLOCK(X) ((X)=0)

/*�ⲿ����------------------------------------------------------------------------------------*/

extern OS_CTaskMask taskMask;

#if OS_01_STATITASK==1
extern unsigned long OS_IdleCtr;		/*���������������*/
extern unsigned long OS_IdleCtrMax;		/*�����������ȫ���������ֵ*/
extern unsigned char OS_CPUPerc;		/*CPUʹ�ðٷֱ�*/

//��������
void OS_IdleTask(void);
//ͳ������ ÿ�����һ��
void OS_StatisTask(void);
//��ȡCPUʹ����
#define OS_GetCPUPerc() ((const)(OS_CPUPerc))
#endif

#endif
