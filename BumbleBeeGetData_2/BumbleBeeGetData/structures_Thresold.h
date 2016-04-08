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


#define MAHAL_DIST 4  //실제값 //Update 할때 거리 
#define MAHAL_MERGE_DIST 0.5 //0.5 //Merging 할때 거리
#define Update_TH 5 // Update수가 이것 미만 일때 삭제 함
#define Nomality_TH 5 // 정규분포 정도가 이것 이하일때 삭제함
#define UPenalty_TH 10 // 한 프레임에서 up date수가 이것 이하일때 amount의 카운트를 줄인다.
#define UpdateMax_TH 2000 //한 분포의 amount의 최대값

//#define PLANE_TH 0.01

#define PLANE_TH 0.05 //평면 파라미터의 오차 범위
#define PI 3.1416
#define dist_G 20 //데이터 샘플링시 최대거리
#define angle_TH 8 //데이터 샘플링시 허용 각도   
#define ScanIncreaseTH 5 //학습에 데이터 샘플링에서 샘플링 간격
#define ScanIterationTH 2 //2실제값 //학습에 데이터 샘플링 전체 반복 횟수

#endif // !defined(AFX_STRUCTURES1_H__C9E558C8_1CEF_49C0_B3CC_B34F3B72FCBD__INCLUDED_)

