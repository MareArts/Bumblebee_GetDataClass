#if !defined(AFX_STRUCTURES1_H__C9E558C8_1CEF_49C0_B3CC_B34F3B72FCBD__INCLUDED_)
#define AFX_STRUCTURES1_H__C9E558C8_1CEF_49C0_B3CC_B34F3B72FCBD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


struct xyz{
	double x;
	double y;
	double z;
};


#define MAHAL_DIST 4  //������ //Update �Ҷ� �Ÿ� 
#define MAHAL_MERGE_DIST 0.5 //0.5 //Merging �Ҷ� �Ÿ�
#define Update_TH 5 // Update���� �̰� �̸� �϶� ���� ��
#define Nomality_TH 5 // ���Ժ��� ������ �̰� �����϶� ������
#define UPenalty_TH 10 // �� �����ӿ��� up date���� �̰� �����϶� amount�� ī��Ʈ�� ���δ�.
#define UpdateMax_TH 2000 //�� ������ amount�� �ִ밪

//#define PLANE_TH 0.01

#define PLANE_TH 0.05 //��� �Ķ������ ���� ����
#define PI 3.1416
#define dist_G 20 //������ ���ø��� �ִ�Ÿ�
#define angle_TH 8 //������ ���ø��� ��� ����   
#define ScanIncreaseTH 5 //�н��� ������ ���ø����� ���ø� ����
#define ScanIterationTH 2 //2������ //�н��� ������ ���ø� ��ü �ݺ� Ƚ��

#endif // !defined(AFX_STRUCTURES1_H__C9E558C8_1CEF_49C0_B3CC_B34F3B72FCBD__INCLUDED_)

