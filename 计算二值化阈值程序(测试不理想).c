//实测不好用
//*---------------计算二值化阈值---------------*//
void Cal_Threshold(void)
{
    unsigned char ii,jj;
	  unsigned char MaxB,MaxW;	//B-Black;W-White
	  unsigned char CompareTimes;
	  int  SumB,SumW,SumDelta,SumDeltaOld;
	  unsigned char Tmp;
   
    
    uCapRow     = 0;	//采集行清零
    uImaCapFlg = 0;		//图像采集标志
    uImaGetFlg = 0;		//图像取出标志
    VSYNC_Enable();		//开启场中断
    
    while( !uImaCapFlg ) 
    {
        Ima_GetDataReady();  
        //---取得直方图---//           
        for(ii=0; ii<80; ii++)  
            for(jj=0; jj<160; jj++) 
                { FIFORCK = 0;Tmp=PORTA;FIFORCK = 1;Pixel[Tmp]++; }  
        //---找直方图黑色最高点---//  
    		for(ii=1; ii<100; ii++)
    		{
      			if(Pixel[ii] > Pixel[ii-1])	{MaxB = ii;CompareTimes = 0;}
      			else	CompareTimes++; 
      			if(CompareTimes > 10)	break; 
    		}
    		//---找直方白色最高点---//  
    		for(ii=254; ii>150; ii--)
    		{
      			if(Pixel[ii] > Pixel[ii+1]) {MaxW = ii;CompareTimes = 0;}
      			else	CompareTimes++; 
      			if(CompareTimes > 10)	break; 
    		}
    		//找与黑白最高点之间同时斜率最大处,面积差值最少即为二值化点

    		for(ii=MaxB,SumB=0,SumDelta=10000; ii<MaxW; ii++)
    		{
      			//计算前后块面积
      			SumB += Pixel[ii];
      			for(jj=ii+1,SumW=0; jj<=MaxW; jj++) SumW += Pixel[jj];
      			SumDeltaOld = SumDelta;
      			SumDelta = SumW - SumB;    			
      			if(SumDelta < 0) SumDelta *= -1;
      			if(SumDelta < SumDeltaOld) Threshold = ii;
    		}

    }
    VSYNC_Disable(); //PIEJ_PIEJ7 = 0U;
}
//*---------------计算二值化阈值---------------*//