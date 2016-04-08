
#include <stdio.h>
#include "SensorStereo2.h"
#include <time.h>

void main()
{


	int Width = 320;
	int Height = 240;	

	CSensorStereo2 CSS2;
	CSS2.Initial(Width,Height);	
	CSS2.Open();

	cvNamedWindow("Reference");

	char str[1000];
	while(1)
	{
		//get 1 frame data(Image, Depth information)
		CSS2.GetData();
		//Show Image
		cvShowImage("Reference",CSS2.ImgReference);	

		//Save Depth 
		sprintf(str,"./DepthData/%d_Depth.txt",time(0));
		printf("%s\n", str);
		FILE * fp;
		fp = fopen(str,"w");
		for(int i=0; i<Width; ++i)
		{
			for(int j=0; j<Height; ++j)
			{
				fprintf(fp,"%lf %lf %lf\n", CSS2.pDepth[i][j].x, CSS2.pDepth[i][j].y, CSS2.pDepth[i][j].z );
			}
		}
		fclose(fp);

		if(cvWaitKey(1) >= 0 )
			break;
	}

	cvDestroyWindow("Reference");
	CSS2.Close();

}