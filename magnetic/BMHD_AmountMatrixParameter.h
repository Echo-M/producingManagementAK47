/***********************************************************************\ 
*                                                                      *
*            Copyright (C) 2010 - 2015 by Wuhan University             *
*                      All Rights Reserved                             * 
*   Property of Wuhan University. Restricted rights to use, duplicate  * 
*   or disclose this code are granted through contract.                *
*                                                                      *
*   THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF *
*   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED   *
*   TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A  *
*   PARTICULAR PURPOSE.                                                *
*                                                                      *
------------------------------------------------------------------------
$Id$                                                   
$Revision$		
$Author$          Liuyf                                         
$Date$			  2015-1-13-1500
------------------------------------------------------------------------
=====================================================================*/
#ifndef BMHD_AmountMatrixParameter_H_
#define BMHD_AmountMatrixParameter_H_

#define		WAVELENGTH					1024*12
#define		BMHDSPACE				64

#define		SIGNAL_NORMAL			0
#define		SIGNAL_ERROR			-1
#define		RETURN_OK				0
#define		RETURN_ERROR			-1

#define		POSITION_CODE			0
#define		POSITION_BM				1
#define		POSITION_HD				2
#define		POSITION_LSM			3
#define		POSITION_RSM			4
#define		POSITION_LM				5
#define		POSITION_RM				6

#define		BM_UNKNOW				0
#define		BM_NONE					-1
#define		BM_CODE					-2
#define		BM_INVERSE				-3

#define		SM_OK					0
#define		SM_NONE					-1
#define		SM_SMALL				-2

#define		MIDM_OK					0
#define		MIDM_NONE				-1

#define		SELF_CHECK_BUFF_SIZE	7
#define		RMB_CHECK_BUFF_SIZE		7

#define		AMOUNTCOMFIRM			185
#define		BMOFFEST_H				1400
#define		BMOFFEST_L				600
#define		DEFAULT_LEVELV			1500

#define		SM_CONFIRM_LSM			13
#define		SM_CONFIRM_RSM			24

/************************************************************************/
/*							Դ���ݽṹ��								*/
/************************************************************************/
typedef struct tagTdn_DataBuf
{
	int *BM_W;
	int *BM_T;
	int *BM_V;
	int *LSM_W;
	int *LSM_T;
	int *LSM_V;
	int *RSM_W;
	int *RSM_T;
	int *RSM_V;
	int *LM_W;
	int *LM_T;
	int *LM_V;
	int *RM_W;
	int *RM_T;
	int *RM_V;
	int *EIR0_W;
	int *EIR0_T;
	int *EIR0_V;
	int *EIR1_W;
	int *EIR1_T;
	int *EIR1_V;
	int *EIR2_W;
	int *EIR2_T;
	int *EIR2_V;
	int *EIR3_W;
	int *EIR3_T;
	int *EIR3_V;
	int *EIR4_W;
	int *EIR4_T;
	int *EIR4_V;
	int *EIR5_W;
	int *EIR5_T;
	int *EIR5_V;
	int *EIR6_W;
	int *EIR6_T;
	int *EIR6_V;
	int *EIR7_W;
	int *EIR7_T;
	int *EIR7_V;
	int *IR1_W;
	int *IR1_T;
	int *IR1_V;
	int *IR2_W;
	int *IR2_T;
	int *IR2_V;
	int *IR3_W;
	int *IR3_T;
	int *IR3_V;
	int *IR4_W;
	int *IR4_T;
	int *IR4_V;
	int *IR5_W;
	int *IR5_T;
	int *IR5_V;
	int *IR6_W;
	int *IR6_T;
	int *IR6_V;
	int *UV_W;
	int *UV_T;
	int *UV_V;
	int *UVL_W;
	int *UVL_T;
	int *UVL_V;
	int *UVR_W;
	int *UVR_T;
	int *UVR_V;
	int *HD_W;
	int *HD_T;
	int *HD_V;
	int *IR_NORM_V;

	int BM_Count;
	int LSM_Count;
	int RSM_Count;
	int LM_Count;
	int RM_Count;
	int EIR0_Count;
	int EIR1_Count;
	int EIR2_Count;
	int EIR3_Count;
	int EIR4_Count;
	int EIR5_Count;
	int EIR6_Count;
	int EIR7_Count;
	int IR1_Count;
	int IR2_Count;
	int IR3_Count;
	int IR4_Count;
	int IR5_Count;
	int IR6_Count;
	int UV_Count;
	int UVL_Count;
	int UVR_Count;
	int HD_Count;
	int IR_NORM_Count;

	//Tape_BackgroundInfo EIR_Back[EIR_NUM];
	int Tape_BackgroundValid;

	tagTdn_DataBuf()
	{
		memset(this, 0, sizeof(tagTdn_DataBuf));

		BM_W = new int[5 * WAVELENGTH * sizeof(UINT32)];
		BM_T = new int[5 * WAVELENGTH * sizeof(UINT32)];
		BM_V = new int[5 * WAVELENGTH * sizeof(UINT32)];
		LSM_W = new int[5 * WAVELENGTH * sizeof(UINT32)];
		LSM_T = new int[5 * WAVELENGTH * sizeof(UINT32)];
		LSM_V = new int[5 * WAVELENGTH * sizeof(UINT32)];
		RSM_W = new int[5 * WAVELENGTH * sizeof(UINT32)];
		RSM_T = new int[5 * WAVELENGTH * sizeof(UINT32)];
		RSM_V = new int[5 * WAVELENGTH * sizeof(UINT32)];
		LM_W = new int[5 * WAVELENGTH * sizeof(UINT32)];
		LM_T = new int[5 * WAVELENGTH * sizeof(UINT32)];
		LM_V = new int[5 * WAVELENGTH * sizeof(UINT32)];
		RM_W = new int[5 * WAVELENGTH * sizeof(UINT32)];
		RM_T = new int[5 * WAVELENGTH * sizeof(UINT32)];
		RM_V = new int[5 * WAVELENGTH * sizeof(UINT32)];
		EIR0_W = new int[5 * WAVELENGTH * sizeof(UINT32)];
		EIR0_T = new int[5 * WAVELENGTH * sizeof(UINT32)];
		EIR0_V = new int[5 * WAVELENGTH * sizeof(UINT32)];
		EIR1_W = new int[5 * WAVELENGTH * sizeof(UINT32)];
		EIR1_T = new int[5 * WAVELENGTH * sizeof(UINT32)];
		EIR1_V = new int[5 * WAVELENGTH * sizeof(UINT32)];
		EIR2_W = new int[5 * WAVELENGTH * sizeof(UINT32)];
		EIR2_T = new int[5 * WAVELENGTH * sizeof(UINT32)];
		EIR2_V = new int[5 * WAVELENGTH * sizeof(UINT32)];
		EIR3_W = new int[5 * WAVELENGTH * sizeof(UINT32)];
		EIR3_T = new int[5 * WAVELENGTH * sizeof(UINT32)];
		EIR3_V = new int[5 * WAVELENGTH * sizeof(UINT32)];
		EIR4_W = new int[5 * WAVELENGTH * sizeof(UINT32)];
		EIR4_T = new int[5 * WAVELENGTH * sizeof(UINT32)];
		EIR4_V = new int[5 * WAVELENGTH * sizeof(UINT32)];
		EIR5_W = new int[5 * WAVELENGTH * sizeof(UINT32)];
		EIR5_T = new int[5 * WAVELENGTH * sizeof(UINT32)];
		EIR5_V = new int[5 * WAVELENGTH * sizeof(UINT32)];
		EIR6_W = new int[5 * WAVELENGTH * sizeof(UINT32)];
		EIR6_T = new int[5 * WAVELENGTH * sizeof(UINT32)];
		EIR6_V = new int[5 * WAVELENGTH * sizeof(UINT32)];
		EIR7_W = new int[5 * WAVELENGTH * sizeof(UINT32)];
		EIR7_T = new int[5 * WAVELENGTH * sizeof(UINT32)];
		EIR7_V = new int[5 * WAVELENGTH * sizeof(UINT32)];
		IR1_W = new int[5 * WAVELENGTH * sizeof(UINT32)];
		IR1_T = new int[5 * WAVELENGTH * sizeof(UINT32)];
		IR1_V = new int[5 * WAVELENGTH * sizeof(UINT32)];
		IR2_W = new int[5 * WAVELENGTH * sizeof(UINT32)];
		IR2_T = new int[5 * WAVELENGTH * sizeof(UINT32)];
		IR2_V = new int[5 * WAVELENGTH * sizeof(UINT32)];
		IR3_W = new int[5 * WAVELENGTH * sizeof(UINT32)];
		IR3_T = new int[5 * WAVELENGTH * sizeof(UINT32)];
		IR3_V = new int[5 * WAVELENGTH * sizeof(UINT32)];
		IR4_W = new int[5 * WAVELENGTH * sizeof(UINT32)];
		IR4_T = new int[5 * WAVELENGTH * sizeof(UINT32)];
		IR4_V = new int[5 * WAVELENGTH * sizeof(UINT32)];
		IR5_W = new int[5 * WAVELENGTH * sizeof(UINT32)];
		IR5_T = new int[5 * WAVELENGTH * sizeof(UINT32)];
		IR5_V = new int[5 * WAVELENGTH * sizeof(UINT32)];
		IR6_W = new int[5 * WAVELENGTH * sizeof(UINT32)];
		IR6_T = new int[5 * WAVELENGTH * sizeof(UINT32)];
		IR6_V = new int[5 * WAVELENGTH * sizeof(UINT32)];
		UV_W = new int[5 * WAVELENGTH * sizeof(UINT32)];
		UV_T = new int[5 * WAVELENGTH * sizeof(UINT32)];
		UV_V = new int[5 * WAVELENGTH * sizeof(UINT32)];
		UVL_W = new int[5 * WAVELENGTH * sizeof(UINT32)];
		UVL_T = new int[5 * WAVELENGTH * sizeof(UINT32)];
		UVL_V = new int[5 * WAVELENGTH * sizeof(UINT32)];
		UVR_W = new int[5 * WAVELENGTH * sizeof(UINT32)];
		UVR_T = new int[5 * WAVELENGTH * sizeof(UINT32)];
		UVR_V = new int[5 * WAVELENGTH * sizeof(UINT32)];
		HD_W = new int[5 * WAVELENGTH * sizeof(UINT32)];
		HD_T = new int[5 * WAVELENGTH * sizeof(UINT32)];
		HD_V = new int[5 * WAVELENGTH * sizeof(UINT32)];
		IR_NORM_V = new int[5 * WAVELENGTH * sizeof(UINT32)];
		Tape_BackgroundValid = 1;
	}
	~tagTdn_DataBuf()
	{
		delete [] BM_W;
		delete [] BM_T;
		delete [] BM_V;
		delete [] LSM_W;
		delete [] LSM_T;
		delete [] LSM_V;
		delete [] RSM_W;
		delete [] RSM_T;
		delete [] RSM_V;
		delete [] LM_W;
		delete [] LM_T;
		delete [] LM_V;
		delete [] RM_W;
		delete [] RM_T;
		delete [] RM_V;
		delete [] EIR0_W;
		delete [] EIR0_T;
		delete [] EIR0_V;
		delete [] EIR1_W;
		delete [] EIR1_T;
		delete [] EIR1_V;
		delete [] EIR2_W;
		delete [] EIR2_T;
		delete [] EIR2_V;
		delete [] EIR3_W;
		delete [] EIR3_T;
		delete [] EIR3_V;
		delete [] EIR4_W;
		delete [] EIR4_T;
		delete [] EIR4_V;
		delete [] EIR5_W;
		delete [] EIR5_T;
		delete [] EIR5_V;
		delete [] EIR6_W;
		delete [] EIR6_T;
		delete [] EIR6_V;
		delete [] EIR7_W;
		delete [] EIR7_T;
		delete [] EIR7_V;
		delete [] IR1_W;
		delete [] IR1_T;
		delete [] IR1_V;
		delete [] IR2_W;
		delete [] IR2_T;
		delete [] IR2_V;
		delete [] IR3_W;
		delete [] IR3_T;
		delete [] IR3_V;
		delete [] IR4_W;
		delete [] IR4_T;
		delete [] IR4_V;
		delete [] IR5_W;
		delete [] IR5_T;
		delete [] IR5_V;
		delete [] IR6_W;
		delete [] IR6_T;
		delete [] IR6_V;
		delete [] UV_W;
		delete [] UV_T;
		delete [] UV_V;
		delete [] UVL_W;
		delete [] UVL_T;
		delete [] UVL_V;
		delete [] UVR_W;
		delete [] UVR_T;
		delete [] UVR_V;
		delete [] HD_W;
		delete [] HD_T;
		delete [] HD_V;
		delete [] IR_NORM_V;
	}
}Tdn_DataBuf;

/************************************************************************/
/*							�㷨����ṹ��								*/
/************************************************************************/
typedef struct tagM_BMHD_Info
{
	int validstart;//HD��ʼ��
	int validend;//HD������
	int validstart_BM;//BM��ʼ��
	int validend_BM;//BM������
	int valid_dianJugeShu;//������
	int average;//����ֵ
	int result;//��Ž��
}M_BMHD_Info;

typedef struct tagSM_RES
{
	int SMthre;//�ߴŸ���
	int flag;//�ߴ�����
}SM_RES;

typedef struct tagM_RES
{
	int sumMthre;//�дź���ֵ
	int MYes;//�д�����
}M_RES;

typedef struct tagM_LRSM_Info
{
	SM_RES LSMRes;//��ߴŽṹ��
	SM_RES RSMRes;//�ұߴŽṹ��
	M_RES LM_RES;//���дŽṹ��
	M_RES RM_RES;//���дŽṹ��
}M_LRSM_Info;

typedef struct tagResult_RMB
{
	int Moneystartw;//��ʼ����
	int Moneyendw;//��������
	int Lasterror;//ɲ�����
	int ERRtype;//��������
	int Amount;//���
	int Direction;//����
	M_BMHD_Info BMHD_Info;//��Žṹ��
	M_LRSM_Info LRSM_Info;//���дŽṹ��

}Result_RMB;

/************************************************************************/
/*								�Լ����								*/
/************************************************************************/
typedef struct tagselfCheck_Info
{
	int _LOW_DV;//��������
	int _MID_DV;//��������
	int _HIGH_DV;//��������
	int _LOW_AV;//��ֵ����
	int _HIGH_AV;//��ֵ����

}selfCheck_Info;

/************************************************************************/
/*								���̲���								*/
/************************************************************************/
typedef struct tag_CODE_Parameter
{
	int _initialSum;//���̲����������ʼֵ
	int _samplePointMin;//ÿ�����̲���������
	int _samplePointMax;//ÿ�����̲���������

}CODE_Parameter;

/************************************************************************/
/*								��Ų���								*/
/************************************************************************/
typedef struct tag_BMHD_Parameter
{
	int _StartEndOffset;//�����ʼ����ƫ��
	int _BMnum_ceil;//��Ÿ�������
	int _BMnum_floor;//��Ÿ�������
	int _BMdeltamaskThre;//��Ų�ֵģ��ƥ�����
	int _BMmaskThre;//���ģ��ƥ�����
	int _HDdeltaPeak;//HD��ֵ��
	int _HDdeltaPosMin;//HD�����Сֵ
	int _HDdeltaPosMax;//HD������ֵ

}BMHD_Parameter;

/************************************************************************/
/*						   	   �ߴ��дŲ���								*/
/************************************************************************/
typedef struct tag_LRSM_Parameter
{
	int _SMOFFEST_H;//�ߴŵ�ѹ����ƫ��
	int _SMOFFEST_L;//�ߴŵ�ѹ����ƫ��
	int _SMnum_strong;//�ߴŸ�������
	int _SMnum_weak;//�ߴŸ�������

	int _MIDMOFFEST_H;//�дŵ�ѹ����ƫ��
	int _MIDMOFFEST_L;//�дŵ�ѹ����ƫ��
	int _deltaMidMthre;//�дŷ����ֵ
	int _MidMthre;//�дŷ�Ⱥ���ֵ
	int _MidMnum_weak;//�дŸ�������

}LRSM_Parameter;


typedef struct tag_BMHD_LRSM__Parameter//���ṹ��
{
	selfCheck_Info _selfCheckInfo;
	CODE_Parameter _CODEParameter;
	BMHD_Parameter _BMHDParameter;
	LRSM_Parameter _LRSMParameter;

	tag_BMHD_LRSM__Parameter::tag_BMHD_LRSM__Parameter()
	{
		//�Լ����
		_selfCheckInfo._LOW_DV			= 100;
		_selfCheckInfo._MID_DV			= 40000;
		_selfCheckInfo._HIGH_DV			= 80000;
		_selfCheckInfo._LOW_AV			= 1600;
		_selfCheckInfo._HIGH_AV			= 2500;

		//���̲���
		_CODEParameter._initialSum		= 2;
		_CODEParameter._samplePointMin	= 5;
		_CODEParameter._samplePointMax	= 14;

		//��Ų���
		_BMHDParameter._StartEndOffset	= 0;//10;
		_BMHDParameter._BMnum_ceil		= 18;
		_BMHDParameter._BMnum_floor		= 11;
		_BMHDParameter._BMdeltamaskThre	= 7;
		_BMHDParameter._BMmaskThre		= 3;
		_BMHDParameter._HDdeltaPeak		= 200;
		_BMHDParameter._HDdeltaPosMin	= 35;
		_BMHDParameter._HDdeltaPosMax	= 300;

		//�ߴŲ���
		_LRSMParameter._SMOFFEST_H		= 1600;
		_LRSMParameter._SMOFFEST_L		= 1300;
		_LRSMParameter._SMnum_strong	= 3;
		_LRSMParameter._SMnum_weak		= 1;
		//�дŲ���
		_LRSMParameter._MIDMOFFEST_H	= 1500;
		_LRSMParameter._MIDMOFFEST_L	= 1300;
		_LRSMParameter._deltaMidMthre	= 1100;
		_LRSMParameter._MidMthre		  = 19000;
		_LRSMParameter._MidMnum_weak	= 2;
	}
}BMHD_LRSM_Parameter;

extern BMHD_LRSM_Parameter BMHD_LRSMParameter;
extern Tdn_DataBuf TdnDataBuf;
extern void InitialParameter(const BMHD_LRSM_Parameter * const _pParam);

#endif