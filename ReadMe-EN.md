# Development-of-Canon-camera
<font color=#D2691E> 瞎几把翻译的，请看中文</font>
C++
<br>

English| [简体中文](/ReadMe.md ) 

## Catalogue
+ [Folderdescription](#Folderdescription)
+ [CanonCamera](#CanonCamera)
   + [CodeAnalysis](#CodeAnalysis) 
      + [CameraControl](#CameraControl)
      + [CameraCallback](#CameraCallback)
   + [Calling process analysis](#CallingAnalysis)
      + [TakeImage](#TakeImage)
      + [GetProperty](#GetProperty)
+ [HowToUse](#HowToUse) 
   + [Callback](#Callback)
   + [Connection](#Connection)
   + [Settings](#Settings)
## Folderdescription
- Camra:  About the camera control
- Class: About the camera action thread
- Command:About the specific actions of the camera
- Event:About the camera state change event
- EDSDK:Canon Camera SDK
## CanonCamera
CanonCamera class is the Canon camera control interface code used by users
<br>

### CodeAnalysis
#### CameraControl
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
#### CameraCallback
```C++
//设置回调函数
bool Canon_SetDownloadEvfCallback(IMAGEDATACALLBACK imgdata,LPARAM lParam);             //预览的数据
void Canon_SetDownLoadCallback(DOWNLOADPROGRESSCALLBACK downloadstate,LPARAM lParam);   //下载图像的进度 
bool Canon_SetTvCallback(TVCALLBACK tvstate,LPARAM lParam);				//TV值					
bool Canon_SetAvCallback(AVCALLBACK avstate,LPARAM lParam);				//AV值
bool Canon_SetIsoCallback(ISOCALLBACK isostate,LPARAM lParam);				//ISO值
bool Canon_SetAEModeCallback(AECALLBACK aestate,LPARAM lParam);				//AE模式
bool Canon_SetMeteringModeCallback(METERINGMODECALLBACK meteringmodestate,LPARAM lParam);//测光模式
bool Canon_SetExposureCompCallback(EXPOSURECOMPCALLBACK exposurecompstate,LPARAM lParam);//曝光补偿
bool Canon_SetImageQualityCallback(IMGQUALITYCALLBACK imgqualitystate,LPARAM lParam);	//图像质量
bool Canon_SetEvfAFModeCallback(EVFAFMODECALLBACK evfafmodestate,LPARAM lParam);        //EVF AF模式
```
### CallingAnalysis
#### TakeImage
```C++
//1.定义对象
ActionSource	_TakePicture;
//2.添加订阅者，命令字符
_TakePicture.addActionListener(listener);
_TakePicture.setActionCommand("TakePicture");
//3.执行命令
_TakePicture.fireEvent();
	actionPerformed(event);//对比命令字符
		StoreAsync(new TakePictureCommand(_model));//创建对应任务
			_processor.enqueue( command );//添加到任务队列
				command->execute();//在线程中执行命令TakePictureCommand::execute()
//4.下载图像
//设置SDK对象事件回调
err = EdsSetPropertyEventHandler( camera, kEdsObjectEvent_All, CameraEventListener::handleObjectEvent , (EdsVoid *)_controller);
//下载
case kEdsObjectEvent_DirItemRequestTransfer:
				fireEvent(controller, "download", inRef);
```
#### GetProperty
```C++
//1.设置SDK属性事件回调
err = EdsSetPropertyEventHandler( camera, kEdsPropertyEvent_All, CameraEventListener::handlePropertyEvent , (EdsVoid *)_controller);
//2.将属性添加到观察者
ob->addObserver(static_cast<Observer*>(&_Tv));
//3.当相机属性改变时，会触发SDK属性事件回调，通知PC属性改变
switch(inEvent)
{
    case kEdsPropertyEvent_PropertyChanged:
        fireEvent(controller, "get_Property", &inPropertyID);
        break;

    case kEdsPropertyEvent_PropertyDescChanged:
        fireEvent(controller, "get_PropertyDesc", &inPropertyID);
        break;
}
```
## HowToUse
```C++
CanonCamera canoncam;//定义CanonCamera对象
```
### Callback
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
### Connection
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
### Settings
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
