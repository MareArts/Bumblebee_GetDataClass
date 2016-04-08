// SensorStereo2.h: interface for the CSensorStereo2 class.
//
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Made by J.H.KIM, 2009 / feelmare@daum.net, feelmare@gmail.com        //
// blog : http://feelmare.blogspot.com                                  //
// My Lab : VISLAB(http://me.pusan.ac.kr)                               //
//////////////////////////////////////////////////////////////////////////



#if !defined(AFX_SENSORSTEREO2_H__05599910_7FDD_483D_9F3D_9CBB6F443969__INCLUDED_)
#define AFX_SENSORSTEREO2_H__05599910_7FDD_483D_9F3D_9CBB6F443969__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//=============================================================================
// OpenCV Includes
//=============================================================================
#include <cv.h>
#include <highgui.h>
//=============================================================================
// PGR Includes
//=============================================================================
#include "triclops.h"
#include "pgrflycapture.h"
#include "pgrflycapturestereo.h"
#include "pnmutils.h"
//=============================================================================
// STL Includes
//=============================================================================
#include <vector>

//=============================================================================
// Other Includes
//=============================================================================
#include "structures_Thresold.h"
using namespace std;



class CSensorStereo2  
{
public:
	bool GetPt(xyz &pt, int x, int y);
	bool GetPt3(int x, int y);
	bool GetData();

	//functions
	void Initial(int W, int H);
	bool Close();
	bool Open();
	CSensorStereo2();
	virtual ~CSensorStereo2();

	//variances
	xyz ** pDepth;

	int i,j, k;
	unsigned short* row;
	unsigned short disparity;
	float x, y, z;
	
	TriclopsContext   triclops;
	TriclopsImage     disparityImage;
	TriclopsImage     refImage;
	TriclopsInput     triclopsInput;
	
	FlyCaptureContext	   flycapture;
	FlyCaptureImage	   flycaptureImage;
	FlyCaptureInfoEx	   pInfo;
	FlyCapturePixelFormat   pixelFormat;
	TriclopsImage16     depthImage16;
	
	TriclopsInput       colorData;
	TriclopsColorImage  colorImage;
	
	int imageCols;
	int imageRows;
	int imageRowInc;
	int iSideBySideImages;
	unsigned long timeStampSeconds;
	unsigned long timeStampMicroSeconds;
	
	int Width;
	int Height;
	
	
//	IplImage * ImgDepth;
	IplImage * ImgReference;
	
	
	int pixelinc;
	
	TriclopsError     te;
	FlyCaptureError   fe;
	
	int iMaxCols;
	int iMaxRows;
	
	char* szCalFile;
	
	unsigned char* rowIntMono;	
	unsigned char* rowIntColor;
	
	FlyCaptureImage tempImage;
	FlyCaptureImage tempColorImage;
	
	
	// Pointers to positions in the mono buffer that correspond to the beginning
	// of the red, green and blue sections
	unsigned char* redMono;
	unsigned char* greenMono;
	unsigned char* blueMono;
	
	unsigned char* redColor;
	unsigned char* greenColor;
	unsigned char* blueColor;

	vector< xyz > Pt1;
	vector< xyz > Pt2;

};

#endif // !defined(AFX_SENSORSTEREO2_H__05599910_7FDD_483D_9F3D_9CBB6F443969__INCLUDED_)
