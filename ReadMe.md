# Development-of-Canon-camera
<font color=#D2691E> 佳能相机的二次开发</font>
C++版本
<br>
## 目录
+ [文件夹说明](#文件夹说明)
+ [CanonCamera](#CanonCamera)
   + [CanonCamera代码解析](#CanonCamera代码解析) 
      + [相机控制代码](#相机控制代码)
      + [相机参数回调显示](#相机参数回调显示)
+ [如何使用](#如何使用) 
   + [回调函数](#回调函数)
   + [相机连接](#相机连接)
   + [相机设置](#相机设置)
## 文件夹说明
- Camra: 涉及到相机操作控制
- Class:相机动作，线程，线程控制
- Command:相机命令
- Event:事件通知，观察者
- EDSDK:相机原始SDK
## CanonCamera
CanonCamera类是用户使用的佳能相机控制接口代码
<br>

### CanonCamera代码解析
#### 相机控制代码
```C++
//设置订阅者和被订阅者
void Canon_setupListener(ActionListener* listener);
void Canon_setupObserver(Observable* ob);
//连接
bool Canon_ListCamerasAndConnect(void);
bool Canon_DisConnect(void);
//EVF
bool Canon_StartEVF(void);
bool Canon_EndEVF(void);
//缩放
bool Canon_ZoomFit(void);
bool Canon_ZoomZoom(void);	
//拍照
bool Canon_TakeAnImage(void);	
//对焦
bool Canon_Focus_Near(int value);
bool Canon_Focus_Far(int value);
bool Canon_Focus_Direction(int value);
//快门
bool Canon_SetTv(int sel);
//光圈
bool Canon_SetAv(int sel);
//感光度
bool Canon_SetIso(int sel);
//AE
bool Canon_SetAEMode(int sel);
//测光模式
bool Canon_SetAMeteringMode(int sel);
//曝光补偿
bool Canon_SetExposureComp(int sel);
//输出图像格式
bool Canon_SetImageQuality(int sel);
//输出图像格式
bool Canon_SetEvfAFMode(int sel);
```
#### 相机参数回调显示
```C++
//设置回调函数
bool Canon_SetDownloadEvfCallback(IMAGEDATACALLBACK imgdata,LPARAM lParam);             //预览的数据
void Canon_SetDownLoadCallback(DOWNLOADPROGRESSCALLBACK downloadstate,LPARAM lParam);   //下载图像的进度 
bool Canon_SetTvCallback(TVCALLBACK tvstate,LPARAM lParam);								//TV值						
bool Canon_SetAvCallback(AVCALLBACK avstate,LPARAM lParam);								//AV值
bool Canon_SetIsoCallback(ISOCALLBACK isostate,LPARAM lParam);							//ISO值
bool Canon_SetAEModeCallback(AECALLBACK aestate,LPARAM lParam);							//AE模式
bool Canon_SetMeteringModeCallback(METERINGMODECALLBACK meteringmodestate,LPARAM lParam);//测光模式
bool Canon_SetExposureCompCallback(EXPOSURECOMPCALLBACK exposurecompstate,LPARAM lParam);//曝光补偿
bool Canon_SetImageQualityCallback(IMGQUALITYCALLBACK imgqualitystate,LPARAM lParam);	//图像质量
bool Canon_SetEvfAFModeCallback(EVFAFMODECALLBACK evfafmodestate,LPARAM lParam);        //EVF AF模式
```
<br>

## 如何使用
```C++
CanonCamera canoncam;//定义CanonCamera对象
```
### 回调函数
```C++
//设置回调函数的目的是，当相机参数或状态改变时，相机将通知PC端，同步参数或状态
//这里以曝光时间，预览和下载为例
void xxxDlg::ShowTvState(EdsPropertyDesc TvDesc,EdsUInt32 Tv,LPARAM lParam)
{
	xxxDlg *pDlg =(xxxDlg *)lParam;
	
	pDlg->m_Tv.ResetContent();
	for(int i = 0; i < TvDesc.numElements; i++)
	{
		std::map<EdsUInt32, const char *>::iterator itr = canoncam._TvDesc.find((EdsUInt32)TvDesc.propDesc[i]);

		if (itr != canoncam._TvDesc.end())
		{
			int index = pDlg->m_Tv.InsertString(-1, itr->second);
			pDlg->m_Tv.SetItemData(index, itr->first);
		}
	}
	std::map<EdsUInt32, const char*>::iterator itr = canoncam._TvDesc.find(Tv);
	if (itr != canoncam._TvDesc.end())
	{		
		pDlg->m_Tv.SetWindowText(itr->second);
	}
}

void xxxDlg::ShowEvf(CComPtr<IStream> stream, LPARAM lParam)
{
	xxxDlg *pDlg =(xxxDlg *)lParam;
	if (!pDlg->m_Image.IsNull())pDlg->image.Destroy();

	pDlg->m_Image.Load(stream);
	if (pDlg->m_Image.IsNull())return;
	int cx, cy;  
	cx  = pDlg->m_Image.GetWidth();  
	cy  = pDlg->m_Image.GetHeight();  
	CWnd *pWnd = NULL;  
	CRect   winrect,imgrect;//显示图像的区域 
	pWnd= pDlg->GetDlgItem(IDC_STATIC_IMG);//获取控件句柄
	pWnd->GetClientRect(&winrect);
	//
	imgrect.left=0;
	imgrect.top=0;
	imgrect.right=cx-imgrect.left;
	imgrect.bottom=cy-imgrect.top;
	//显示相机预览画面
	CDC *pDc = NULL;  
	pDc = pWnd->GetDC();
	int ModeOld=SetStretchBltMode(pDc->m_hDC,STRETCH_HALFTONE);
	pDlg->m_Image.StretchBlt(pDc->m_hDC,winrect,imgrect,SRCCOPY);
	SetStretchBltMode(pDc->m_hDC,ModeOld); 
	pWnd->ReleaseDC(pDc);
}

void xxxDlg::ShowDownloadState(EdsUInt32 percent,int state,LPARAM lParam)
{	
	//percent是拍摄进度，可用作进度条参数
	xxxDlg *pDlg =(xxxDlg *)lParam;
	if (state==SATRTDOWN)
	{
		//开始拍照
	} 
	else if (state==ENDDOWN)
	{
		//拍摄激结束
	}
	else if (state==DOWNING)
	{
		//拍摄中
	}
	else
	{
		//拍摄失败
	}
}
```
### 相机连接
```C++
if (!canoncam.Canon_GetCameraConnectedState())
{
    if(canoncam.Canon_ListCamerasAndConnect())
    {
        canoncam.Canon_SetTvCallback(ShowTvState,LPARAM(this));
        canoncam.Canon_SetAvCallback(ShowAvState,LPARAM(this));
        canoncam.Canon_SetIsoCallback(ShowIsoState,LPARAM(this));
        canoncam.Canon_SetDownloadEvfCallback(ShowEvf,(LPARAM)this);
        canoncam.Canon_SetDownLoadCallback(ShowDownloadState,(LPARAM)this);
    }
    else
    {
    	//连接失败
        canoncam.Canon_DisConnect();
    }
}
else
{
	//断开连接
	canoncam.Canon_DisConnect();
}
```
### 相机设置
```C++
//相机设置这个部分非常简单
//拍摄
if(canoncam.Canon_GetCameraConnectedState())
{
	canoncam.Canon_TakeAnImage();
}
//设置曝光时间TV
DWORD_PTR data = m_Tv.GetItemData(m_Tv.GetCurSel());
canoncam.Canon_SetTv(data);
```
