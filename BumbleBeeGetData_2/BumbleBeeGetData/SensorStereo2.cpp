// SensorStereo2.cpp: implementation of the CSensorStereo2 class.
//
//////////////////////////////////////////////////////////////////////

#include "SensorStereo2.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSensorStereo2::CSensorStereo2()
{
	printf("//////////////////////////////////////////////////////////////////////////\n");
	printf("// Made by J.H.KIM, 2009 / feelmare@daum.net, feelmare@gmail.com        //\n");
	printf("// blog : http://feelmare.blogspot.com                                  //\n");
	printf("// My Lab : VISLAB(http://me.pusan.ac.kr)                               //\n");
	printf("//////////////////////////////////////////////////////////////////////////)\n");	
}

CSensorStereo2::~CSensorStereo2()
{
	
}

bool CSensorStereo2::Open()
{
	// Open the camera
	if( flycaptureCreateContext( &flycapture ) != FLYCAPTURE_OK ) 
		return false;	
	
	// Initialize the Flycapture context
	if( flycaptureInitialize( flycapture, 0 ) != FLYCAPTURE_OK ) 
		return false;	
	
	// Save the camera's calibration file, and return the path 
	if( flycaptureGetCalibrationFileFromCamera( flycapture, &szCalFile ) != FLYCAPTURE_OK ) 
		return false;
	
	// Create a Triclops context from the cameras calibration file
	if( triclopsGetDefaultContextFromFile( &triclops, szCalFile ) != TriclopsErrorOk ) 
		return false;	
	
	// Get camera information
	if( flycaptureGetCameraInfo( flycapture, &pInfo ) != FLYCAPTURE_OK ) 
		return false;
	
	if (pInfo.CameraType == FLYCAPTURE_COLOR)
	{
		pixelFormat = FLYCAPTURE_RAW16;
	} 
	else 
	{
		pixelFormat = FLYCAPTURE_MONO16;
	}
	
	
	switch (pInfo.CameraModel)
	{
	case FLYCAPTURE_BUMBLEBEE2:
		{
			unsigned long ulValue;
			flycaptureGetCameraRegister( flycapture, 0x1F28, &ulValue );
			
			if ( ( ulValue & 0x2 ) == 0 )
			{
				// Hi-res BB2
				iMaxCols = 1024; 
				iMaxRows = 768;
			}
			else
			{
				// Low-res BB2
				iMaxCols = 640;
				iMaxRows = 480;
			}
		}
		break;
		
	case FLYCAPTURE_BUMBLEBEEXB3:
		iMaxCols = 1280;
		iMaxRows = 960;
		//iMaxCols = 640;
		//iMaxRows = 480;
		break;
		
	default:
		return false;		
	}
	
	
	// Start transferring images from the camera to the computer
	if( flycaptureStartCustomImage(flycapture, 3, 0, 0, iMaxCols, iMaxRows, 100, pixelFormat) != FLYCAPTURE_OK ) 
		return false;	
	
	// Set up some stereo parameters:
	// Set to 320x240 output images
	if( triclopsSetResolution( triclops, Height, Width ) != TriclopsErrorOk ) 
		return false;	
	
	// Set disparity range
	if( triclopsSetDisparity( triclops, 0, 100 ) != TriclopsErrorOk ) 
		return false;	
	
	// Lets turn off all validation except subpixel and surface
	// This works quite well
	if( triclopsSetTextureValidation( triclops, 0 ) != TriclopsErrorOk ) 
		return false;	
	
	if( triclopsSetUniquenessValidation( triclops, 0 ) != TriclopsErrorOk ) 
		return false;
	
	if( triclopsSetSubpixelInterpolation( triclops, 1 ) != TriclopsErrorOk ) 
		return false;
	
	
	return true;
}

bool CSensorStereo2::Close()
{
	if(flycapture != 0 )
	{
		// Close the camera
		if( flycaptureStop( flycapture ) != FLYCAPTURE_OK )
			return false;
		
		if( flycaptureDestroyContext( flycapture ) != FLYCAPTURE_OK )
			return false;
		flycapture=0;
	}
	
	if(rowIntMono != 0)
	{
		// Delete the image buffer
		delete rowIntMono;
		redMono = NULL;
		greenMono = NULL;
		blueMono = NULL;
		rowIntMono=0;
	}
	
	if(rowIntColor != 0){
		delete rowIntColor;
		redColor = NULL;
		greenColor = NULL;
		blueColor = NULL;
		rowIntColor = 0;
	}		
	
	if(triclops != 0 ){		
		// Destroy the Triclops context
		if( triclopsDestroyContext( triclops ) != FLYCAPTURE_OK )
			return false;
		triclops=0;
	}
	
//	if(ImgDepth != 0)
//	{
//		cvReleaseImage(&ImgDepth);
//		ImgDepth=0;
//	}
	
	if(ImgReference != 0)
	{
		cvReleaseImage(&ImgReference);
		ImgReference=0;
	}
	
	if(pDepth != 0)
	{
		for(int i=0; i<Width; ++i)
		{
			delete pDepth[i];
		}
		delete pDepth;
		pDepth=0;
	}
	
	
	return true;
}

void CSensorStereo2::Initial(int W, int H)
{		
	Width = W;
	Height = H;
	
//	ImgDepth = cvCreateImage(cvSize(Width,Height),IPL_DEPTH_8U,1 );
	ImgReference = cvCreateImage(cvSize(Width,Height),IPL_DEPTH_8U,3 );
	
	iMaxCols = 0;
	iMaxRows = 0;
	
	rowIntMono=0;
	rowIntColor=0;
	
	// Pointers to positions in the mono buffer that correspond to the beginning
	// of the red, green and blue sections
	redMono = NULL;
	greenMono = NULL;
	blueMono = NULL;
	
	redColor = NULL;
	greenColor = NULL;
	blueColor = NULL; 
	
	pDepth = new xyz*[Width];
	for(int i=0; i<Width; ++i)
	{
		pDepth[i] = new xyz[Height];		
	}
	
}

bool CSensorStereo2::GetData()
{
	// Grab an image from the camera
	if( flycaptureGrabImage2( flycapture, &flycaptureImage ) != FLYCAPTURE_OK)
		return false;
	
	// Extract information from the FlycaptureImage
	imageCols = flycaptureImage.iCols;
	imageRows = flycaptureImage.iRows;
	imageRowInc = flycaptureImage.iRowInc;
	iSideBySideImages = flycaptureImage.iNumImages;
	timeStampSeconds = flycaptureImage.timeStamp.ulSeconds;
	timeStampMicroSeconds = flycaptureImage.timeStamp.ulMicroSeconds;
	
	// Create buffers for holding the mono images
	if( rowIntMono == 0)
	{
		rowIntMono = new unsigned char[ imageCols * imageRows * iSideBySideImages ];
		// Create a temporary FlyCaptureImage for preparing the stereo image
		tempImage.pData = rowIntMono;		
	}
	
	if(rowIntColor == 0)
	{
		rowIntColor = new unsigned char[ imageCols * imageRows * iSideBySideImages * 4 ];
		// Create a temporary FlyCaptureImage for preparing the stereo image
		tempColorImage.pData = rowIntColor;
	}
	
	// Convert the pixel interleaved raw data to row interleaved format
	if(flycapturePrepareStereoImage( flycapture, flycaptureImage, &tempImage, &tempColorImage  ) != FLYCAPTURE_OK)
		return false;
	
	redColor = rowIntColor;
	greenColor = redColor + ( 4 * imageCols );
	blueColor = redColor + ( 4 * imageCols );
	
	redMono = rowIntMono;
	greenMono = redMono + imageCols;
	blueMono = redMono + imageCols;
	
	// Use the row interleaved images to build up a packed TriclopsInput.
	// A packed triclops input will contain a single image with 32 bpp.
	if ( triclopsBuildPackedTriclopsInput( imageCols, imageRows, imageRowInc * 4, timeStampSeconds, timeStampMicroSeconds, redColor, &colorData ) != TriclopsErrorOk)
		return false;
	
	// Use the row interleaved images to build up an RGB TriclopsInput.  
	// An RGB triclops input will contain the 3 raw images (1 from each camera).
	if( triclopsBuildRGBTriclopsInput(imageCols, imageRows, imageRowInc,  timeStampSeconds, timeStampMicroSeconds, redMono, greenMono, blueMono, &triclopsInput) != TriclopsErrorOk )
		return false;
	
	// Rectify the images/ surely need
	if( triclopsRectify( triclops, &triclopsInput ) != TriclopsErrorOk)
		return false;
	
	if ( triclopsGetImage16( triclops, TriImg16_DISPARITY, TriCam_REFERENCE, &depthImage16 ) != TriclopsErrorOk )
		return false;
	
	// Do stereo processing
	if( triclopsStereo( triclops ) != TriclopsErrorOk )
		return false;
	
		
	if( triclopsRectifyColorImage( triclops, TriCam_REFERENCE, &colorData, &colorImage ) != TriclopsErrorOk)
		return false;
	
	if ( pixelFormat == FLYCAPTURE_RAW16 )
	{
		if( triclopsRectifyColorImage( triclops, TriCam_REFERENCE, &colorData, &colorImage ) != TriclopsErrorOk)
			return false;
	}
	
	// Determine the number of pixels spacing per row
	pixelinc = depthImage16.rowinc/2;				
//	cvSetZero(ImgDepth);
	for ( i = 0, k = 0; i < depthImage16.nrows; i++ )
	{
		row     = depthImage16.data + i * pixelinc;
		for ( j = 0; j < depthImage16.ncols; j++, k++ )
		{
			ImgReference->imageData[ i*ImgReference->widthStep+j*3+0] = (unsigned char) colorImage.blue[k];
			ImgReference->imageData[ i*ImgReference->widthStep+j*3+1] = (unsigned char) colorImage.green[k];
			ImgReference->imageData[ i*ImgReference->widthStep+j*3+2] = (unsigned char) colorImage.red[k];

			disparity = row[j];	
			
			// filter invalid points
			if ( disparity < 0xFF00 )
			{
				// convert the 16 bit disparity value to doubleing point x,y,z
				triclopsRCD16ToXYZ( triclops, i, j, disparity, &x, &y, &z );
				
				if( z > 10){
					pDepth[j][i].x = 0;
					pDepth[j][i].y = 0;
					pDepth[j][i].z = 0;
				}else{
					pDepth[j][i].x = x;
					pDepth[j][i].y = y;
					pDepth[j][i].z = z;
				}
				
				//printf("[%d][%d] - %lf %lf %lf\n",i,j,x,y,z);
			}else{
				pDepth[j][i].x = 0;
				pDepth[j][i].y = 0;
				pDepth[j][i].z = 0;
			}			
		}
	}
	
	return true;
}

bool CSensorStereo2::GetPt3(int x, int y)
{
	Pt1.clear();
	Pt2.clear();
	
	xyz pt_xyz;	
	
	//1번째 pt
	if(GetPt(pt_xyz, x,y))
		Pt1.push_back(pt_xyz);
	else
		return false;
	if(GetPt(pt_xyz, x,y))
		Pt1.push_back(pt_xyz);
	else
		return false;
	if(GetPt(pt_xyz, x,y))
		Pt1.push_back(pt_xyz);
	else
		return false;		
	
	
	
	//2번째 pt
	if(GetPt(pt_xyz, x,y))
		Pt2.push_back(pt_xyz);
	else
		return false;
	if(GetPt(pt_xyz, x,y))
		Pt2.push_back(pt_xyz);
	else
		return false;
	if(GetPt(pt_xyz, x,y))
		Pt2.push_back(pt_xyz);
	else
		return false;
	
	return true;
}

bool CSensorStereo2::GetPt(xyz &pt, int x, int y)
{
	int rX2,rY2;
	
	//한점에서 더해질 x,y거리 
	int p_rX = rand() % (dist_G)+1;// + 100;
	int p_rY = rand() % (dist_G)+1;// + 100;	
	//x,y 방향으로 + or -로 더해짐 
	int signX = rand() %2;
	int signY = rand() %2;
	
	//부호에 따라 새로운 점 생김
	if(signX)
		rX2 = x + p_rX;
	else
		rX2 = x - p_rX;
	
	if(signY)
		rY2 = y + p_rY;
	else
		rY2 = y - p_rY;
	
	
	
	//영상 크기보다 작거나 크면 다시 			
	if( rX2<0 )
		rX2 = 0;
	if( rX2>=Width)
		rX2 = Width-1;
	if( rY2<0)
		rY2 = 0;
	if( rY2>=Height )
		rY2 = Height-1;
	
	if( ( pDepth[rX2][rY2].x == 0 && pDepth[rX2][rY2].y == 0 && pDepth[rX2][rY2].z == 0) )
		return false;
	else{
		//printf("%lf %lf %lf\n", pDepth[rX2][rY2].x, pDepth[rX2][rY2].y, pDepth[rX2][rY2].z);
		//printf("%d %d\n", rX2, rY2);
		
		pt.x = pDepth[rX2][rY2].x;
		pt.y = pDepth[rX2][rY2].y;
		pt.z = pDepth[rX2][rY2].z;
		
		return true;
	}
}


