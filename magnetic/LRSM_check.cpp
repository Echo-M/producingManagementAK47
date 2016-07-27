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
$Date$			  2015-1-8-1100
------------------------------------------------------------------------
=====================================================================*/
#include <stdafx.h>
#include <math.h>
#include "LRSM_check.h"
#include "JW_type.h"


//�ߴŸ������
int SMCross(int SM_V[], int SM_W[], int startPosi, int endPosi, int averageV, int* SMthre)
{
	int i = 0;
	int point = 0;
	int highgeShu = 0;
	int lowgeShu = 0;

	int highV = averageV + BMHD_LRSMParameter._LRSMParameter._SMOFFEST_H;//4273;//3500;
	int lowV = averageV - BMHD_LRSMParameter._LRSMParameter._SMOFFEST_L;//488;//400;

	for (point = startPosi ; point <= endPosi-1; point++)
	{
		if (SM_V[point] < highV && SM_V[point+1] >= highV)
		{
			highgeShu++;
		}
		if (SM_V[point] >= highV && SM_V[point+1] < highV)
		{
			highgeShu++;
		}
		if (SM_V[point] > lowV && SM_V[point+1] <= lowV)
		{
			lowgeShu++;
		}
		if (SM_V[point] <= lowV && SM_V[point+1] > lowV)
		{
			lowgeShu++;
		}
	}

	*SMthre = highgeShu + lowgeShu;

	if (highgeShu > BMHD_LRSMParameter._LRSMParameter._SMnum_strong || lowgeShu > BMHD_LRSMParameter._LRSMParameter._SMnum_strong)
	{
		return SM_OK;
	}
	else if (highgeShu > BMHD_LRSMParameter._LRSMParameter._SMnum_weak || lowgeShu > BMHD_LRSMParameter._LRSMParameter._SMnum_weak)
	{
		return SM_SMALL;
	}
	else
	{
		return SM_NONE;
	}
}

//�ߴż��
int MagnetCheck(int SM_V[], int SM_W[], int SM_Length, Result_RMB* CheckResult, int* SMthre)
{	
	int i = 0;
	int deltaWheel = 0;
	int flag = -1;          
	int dir = -1;          
	int staPosi = 0;                 //����Ǯ���źſ�ʼ�ͽ�����λ��ֵ ��������
	int endPosi = SM_Length - 1;   
	int midPosi = 0;
	int averageV = 0;
	int startWheel = CheckResult->Moneystartw;
	int endWheel = CheckResult->Moneyendw;

	//������ʼ����λ��
	for(i = 0; i < SM_Length; i++)													
	{
		if(SM_W[i] >= startWheel)
		{
			staPosi = i;
			break;
		}
	}
	for(i = SM_Length - 1; i >= 0; i--)
	{
		if(SM_W[i] <= endWheel)
		{
			endPosi = i;
			break;
		}
	}

	if (endPosi == SM_Length -1 && endWheel - startWheel > 90 && endWheel - startWheel < 133)
	{
		deltaWheel = 133 - (endWheel - startWheel);//100Ԫ
	}

	for(i = 0; i < SM_Length; i++)
	{
		if(SM_W[i] >= (startWheel + endWheel)/2 + deltaWheel)
		{
			midPosi = i;
			break;
		}
	}
	if (endPosi - staPosi > 0)
	{
		for (i = staPosi; i < endPosi; i++)
		{
			averageV += SM_V[i];
		}
		averageV = averageV/(endPosi - staPosi);
	}
	else
	{
		averageV = DEFAULT_LEVELV;
	}

	return SMCross(SM_V, SM_W, staPosi, endPosi, averageV, SMthre);
}

//�дż��
int MidMCheck(int M_V[], int M_W[], int SM_Length, Result_RMB *CheckResult)
{
	int i = 0;
	float coefficient = 0;	//����Ӧ����ϵ��
	int areaLength = 0;		//����Ӧ���䳤��20����
	int point = 0;
	int delta = 0;
	int Mthre = 0;			//��Ⱥ���ֵ
	int deltaMthre = 0;		//�����ֵ
	int num = 0;
	int tempMax = 0;
	int MaxFlag = 0;
	int plusCount = 0;
	int highgeShu = 0;
	int lowgeShu = 0;
	int staPosi = 0;
	int endPosi = 0; 
	int averageV = 0;
	int sumMthre = 0;
	int Moneystartw = CheckResult->Moneystartw;
	int Moneyendw = CheckResult->Moneyendw;

	deltaMthre = BMHD_LRSMParameter._LRSMParameter._deltaMidMthre;
	Mthre = BMHD_LRSMParameter._LRSMParameter._MidMthre;

	for(i = 0; i < SM_Length; i++)				  //������ʼλ��
	{
		if (M_W[i] == Moneystartw)
		{
			staPosi = i;
			break;
		}
	}
	for (i = SM_Length-1; i >= 0; i--)
	{
		if(M_W[i] == Moneyendw)
		{
			endPosi = i;
			break;
		}
	}

	if (endPosi - staPosi > 0)
	{
		for (i = staPosi; i < endPosi; i++)
		{
			averageV += M_V[i];
		}
		averageV = averageV/(endPosi - staPosi);
	}
	else
	{
		averageV = DEFAULT_LEVELV;
	}

	for (point = staPosi; point < endPosi; point++)
	{
		if (M_V[point] >= averageV + BMHD_LRSMParameter._LRSMParameter._MIDMOFFEST_H)//4029
		{
			highgeShu++;
		}

		if (M_V[point] <= averageV - BMHD_LRSMParameter._LRSMParameter._MIDMOFFEST_H)//610
		{
			lowgeShu++;
		}
	}

	if (highgeShu < BMHD_LRSMParameter._LRSMParameter._MidMnum_weak || lowgeShu < BMHD_LRSMParameter._LRSMParameter._MidMnum_weak)
	{
		return MIDM_NONE;
	}

	////////////////////////////// �д����㷨 ///////////////////////////////////////////
	coefficient = (endPosi - staPosi)/((Moneyendw - Moneystartw)*8.0);

	areaLength = ceil(20*coefficient);

	for (point = (int)(staPosi*0.8+endPosi*0.2); point<=endPosi - areaLength - 15; point++ )
	{
		delta = abs(M_V[point + areaLength] - M_V[point]);

		if (delta > 700)//976) //������������ ���ֵ 800
		{
			if(abs(M_V[point + areaLength + 5] - M_V[point]) > delta)
			{
				delta = abs(M_V[point + areaLength+5] - M_V[point]);
			}

			if(abs(M_V[point + areaLength + 10] - M_V[point]) > delta)
			{
				delta = abs(M_V[point + areaLength+10] - M_V[point]);
			}

			if(abs(M_V[point + areaLength + 15] - M_V[point]) > delta)
			{
				delta = abs(M_V[point + areaLength + 15] - M_V[point]);
			}
		}
		if (delta > deltaMthre)
		{
			if(delta > tempMax)
			{
				tempMax = delta;
				MaxFlag = 1;
			}
		}
		num++;
		if (num == 20)
		{
			if (MaxFlag == 1)
			{
				sumMthre = sumMthre + tempMax;
				plusCount++;
			}
			num = 0;
			MaxFlag = 0;
			tempMax = 0;
		}
	}
	
	if (sumMthre > Mthre)
	{
		return MIDM_OK;
	}
	else
	{
		return MIDM_NONE;
	}
}

//�ߴ��дż��������
void LRSM_check(Tdn_DataBuf *pTdnDataBuf, Result_RMB *CheckResult, int _RMBCheck_Result[])
{
	int* LSM_V = pTdnDataBuf->LSM_V;
	int* LSM_W = pTdnDataBuf->LSM_W;
	int* RSM_V = pTdnDataBuf->RSM_V;
	int* RSM_W = pTdnDataBuf->RSM_W;
	int* LM_V = pTdnDataBuf->LM_V;
	int* LM_W = pTdnDataBuf->LM_W;
	int* RM_V = pTdnDataBuf->RM_V;
	int* RM_W = pTdnDataBuf->RM_W;
	int LSM_Length = pTdnDataBuf->LSM_Count;
	int RSM_Length = pTdnDataBuf->RSM_Count;
	int LM_Length = pTdnDataBuf->LM_Count;
	int RM_Length = pTdnDataBuf->RM_Count;

	CheckResult->Direction = 0;
	CheckResult->ERRtype = 0;

	if (CheckResult->Amount == 100)
	{
		////////��ʼ�б�////////
		//////���ұߴ��б�//////
		CheckResult->LRSM_Info.LSMRes.flag = MagnetCheck(LSM_V, LSM_W, LSM_Length, CheckResult, &CheckResult->LRSM_Info.LSMRes.SMthre);
		CheckResult->LRSM_Info.RSMRes.flag = MagnetCheck(RSM_V, RSM_W, RSM_Length, CheckResult, &CheckResult->LRSM_Info.RSMRes.SMthre);
		CheckResult->LRSM_Info.LM_RES.MYes = MidMCheck(LM_V, LM_W, LM_Length, CheckResult);
		CheckResult->LRSM_Info.RM_RES.MYes = MidMCheck(RM_V, RM_W, RM_Length, CheckResult);

		/////////////// �ۺ��ж� //////////////////
		if(CheckResult->LRSM_Info.LSMRes.flag == SM_OK && CheckResult->LRSM_Info.RSMRes.flag == SM_OK)//���ұߴŶ����ź�
		{
			if (CheckResult->LRSM_Info.LSMRes.SMthre >= CheckResult->LRSM_Info.RSMRes.SMthre)//��ߴŴ�
			{
				CheckResult->Direction = SM_CONFIRM_LSM;
			}
			else//�ұߴŴ�
			{
				CheckResult->Direction = SM_CONFIRM_RSM;
			}
		}
		else if(CheckResult->LRSM_Info.LSMRes.flag == SM_OK)//��ߴ�����
		{
			CheckResult->Direction = SM_CONFIRM_LSM;
		}
		else if(CheckResult->LRSM_Info.RSMRes.flag == SM_OK)//�ұߴ�����
		{
			CheckResult->Direction = SM_CONFIRM_RSM;
		}
		else if(CheckResult->LRSM_Info.LSMRes.flag == SM_NONE && CheckResult->LRSM_Info.RSMRes.flag == SM_NONE)//���ұߴŶ����ź�
		{
			CheckResult->ERRtype = JW_M_FEATURE_NOSM;
			if (CheckResult->LRSM_Info.RM_RES.MYes == MIDM_OK && CheckResult->LRSM_Info.LM_RES.MYes != MIDM_OK)//���д��� ���д���
			{
				CheckResult->Direction = SM_CONFIRM_LSM;
				_RMBCheck_Result[POSITION_LSM] = SIGNAL_ERROR;
			}
			else if (CheckResult->LRSM_Info.RM_RES.MYes != MIDM_OK && CheckResult->LRSM_Info.LM_RES.MYes == MIDM_OK)//���д��� ���д���
			{
				CheckResult->Direction = SM_CONFIRM_RSM;
				_RMBCheck_Result[POSITION_RSM] = SIGNAL_ERROR;
			}
			else//���д��� ���д���
			{
				CheckResult->ERRtype = JW_M_FEATURE_NOSM;
				_RMBCheck_Result[POSITION_LSM] = SIGNAL_ERROR;
				_RMBCheck_Result[POSITION_RSM] = SIGNAL_ERROR;
			}		
		}
		else if(CheckResult->LRSM_Info.LSMRes.flag != SM_NONE)//��ߴ�С
		{
			CheckResult->Direction = SM_CONFIRM_LSM;
			CheckResult->ERRtype = JW_M_FEATURE_LCROSS;
			_RMBCheck_Result[POSITION_LSM] = SIGNAL_ERROR;
		}
		else//�ұߴ�С
		{
			CheckResult->Direction = SM_CONFIRM_RSM;
			CheckResult->ERRtype = JW_M_FEATURE_RCROSS;
			_RMBCheck_Result[POSITION_RSM] = SIGNAL_ERROR;
		}

		////////�����д��б�////////
		if(CheckResult->Direction == SM_CONFIRM_LSM)
		{
			if (CheckResult->LRSM_Info.RM_RES.MYes != MIDM_OK)//���д���
			{
				CheckResult->ERRtype = JW_M_FEATURE_RMLESS;
				_RMBCheck_Result[POSITION_RM] = SIGNAL_ERROR;													
			}
		}
		if(CheckResult->Direction == SM_CONFIRM_RSM)
		{
			if (CheckResult->LRSM_Info.LM_RES.MYes != MIDM_OK)//���д���
			{
				CheckResult->ERRtype = JW_M_FEATURE_LMLESS;
				_RMBCheck_Result[POSITION_LM] = SIGNAL_ERROR;
			}
		}
		////////ʶ�����////
	}	
}