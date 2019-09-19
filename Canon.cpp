#include "StdAfx.h"
#include "Canon.h"
#include <math.h>
#include <iostream>
#include <fstream> 
using namespace std;

Canon::Canon(void)
{
}


Canon::~Canon(void)
{
}


CameraData Cameradata;

void InitStruct()
{
	Cameradata.isOpenSession=FALSE;
	Cameradata.isEvf=FALSE;
	Cameradata.isfinsh=false;
}


EdsCameraRef camera;
bool isSDKLoaded ;
//SAMPLE1//从初始化到最终确定
void applicationRun()
{
	EdsError err = EDS_ERR_OK;
	/*EdsCameraRef */camera = NULL;
	isSDKLoaded = false;
	bool	 locked = false;
	// 初始化 SDK
	err = EdsInitializeSDK();
	if(err == EDS_ERR_OK)
	{
		isSDKLoaded = true;
	}
	// 获取第一个相机
	if(err == EDS_ERR_OK)
	{
		err = getFirstCamera (&camera);
	}
	//// 设置对象事件处理器
	if(err == EDS_ERR_OK)
	{
		err = EdsSetObjectEventHandler(camera, kEdsObjectEvent_All,handleObjectEvent, NULL);
	}
	//// 设置属性事件处理器
	if(err == EDS_ERR_OK)
	{
		err = EdsSetPropertyEventHandler(camera, kEdsPropertyEvent_All,handlePropertyEvent, NULL);
	}
	// 设置状态事件处理器
	if(err == EDS_ERR_OK)
	{
		err = EdsSetCameraStateEventHandler(camera, kEdsStateEvent_All,handleStateEvent, NULL);
	}
	// 打开相机会话
	if(err == EDS_ERR_OK)
	{
		err = EdsOpenSession(camera);
	}

	

	//预先保存设置为PC
	if(err == EDS_ERR_OK)
	{
		EdsUInt32 saveTo = kEdsSaveTo_Host;
		err = EdsSetPropertyData(camera, kEdsPropID_SaveTo, 0, sizeof(saveTo) , &saveTo);
	}

	//UI锁定
	if(err == EDS_ERR_OK)
	{
		err = EdsSendStatusCommand(camera, kEdsCameraStatusCommand_UILock, 0);
	}

	if(err == EDS_ERR_OK)
	{
		locked = true;
	}

	//if(err == EDS_ERR_OK)
	//{
	//	EdsCapacity capacity = {0x7FFFFFFF, 0x1000, 1};
	//	err = EdsSetCapacity( camera, capacity);//在将存储目标设置到计算机后，使用此API通知摄像机主机的可用磁盘容量。
	//}

	
	getPropertyCommand(camera,kEdsPropID_ProductName);
	/////
	// do something
	EdsPropertyDesc AvDesc,TvDesc,IsoDesc,ExposureCompensation,MeteringMode,AEModeSelec;
	getPropertyDescCommand(camera,kEdsPropID_Av, &AvDesc);
	getPropertyDescCommand(camera,kEdsPropID_Tv, &TvDesc);
	getPropertyDescCommand(camera,kEdsPropID_ISOSpeed, &IsoDesc);
	getPropertyDescCommand(camera,kEdsPropID_ExposureCompensation,&ExposureCompensation);
	getPropertyDescCommand(camera,kEdsPropID_MeteringMode,&MeteringMode);
	getPropertyDescCommand(camera,kEdsPropID_AEModeSelect,&AEModeSelec);
	for (int i=0;i<=AvDesc.numElements;i++)
	{
		if(i==0) Cameradata.AvDesc[i]=AvDesc.numElements;
		else Cameradata.AvDesc[i]=AvDesc.propDesc[i];
	}

	for (int i=0;i<=TvDesc.numElements;i++)
	{
		if(i==0) Cameradata.TvDesc[i]=TvDesc.numElements;
		else Cameradata.TvDesc[i]=TvDesc.propDesc[i];
	}
	for (int i=0;i<=IsoDesc.numElements;i++)
	{
		if(i==0) Cameradata.IsoDesc[i]=IsoDesc.numElements;
		else Cameradata.IsoDesc[i]=IsoDesc.propDesc[i];
	}
	CAv();//光圈值和ID的银映射
	CTv();//快门值和ID的银映射
	CIso();//感光度值和ID的银映射
	//EdsUInt32 data;
	//err = EdsGetPropertyData(camera,kEdsPropID_Av,0,sizeof(data),&data );
	//Cameradata.curAv=data;
	//err = EdsGetPropertyData(camera,kEdsPropID_Tv,0,sizeof(data),&data );
	//Cameradata.curTv=data;
	//err = EdsGetPropertyData(camera,kEdsPropID_ISOSpeed,0,sizeof(data),&data );
	//Cameradata.curIso=data;



	//解锁UI
	//if(locked)
	//{
	//	EdsSendStatusCommand(camera, kEdsCameraStatusCommand_UIUnLock, 0);
	//}

	Cameradata.isOpenSession=TRUE;
	//takePicture(camera);
	////
	// 关闭与相机的会话
	//if(err == EDS_ERR_OK)
	//{
	//	err = EdsCloseSession(camera);
	//}
	//// 释放相机
	//if(camera != NULL)
	//{
	//	EdsRelease(camera);
	//	camera=FALSE;
	//}
	//// 中止 SDK
	//if(isSDKLoaded)
	//{
	//	EdsTerminateSDK();
	//}
}

void applicationEnd()
{
	EdsError err = EDS_ERR_OK;
	if(err == EDS_ERR_OK)
	{
		err = EdsCloseSession(camera);
	}
	// 释放相机
	if(camera != NULL)
	{
		EdsRelease(camera);
		camera=FALSE;
	}
	// 中止 SDK
	if(isSDKLoaded)
	{
		EdsTerminateSDK();
	}
	Cameradata.isOpenSession=FALSE;
}

static EdsError EDSCALLBACK  handleObjectEvent (
	EdsUInt32			inEvent,
	EdsBaseRef			inRef,
	EdsVoid *			inContext				
	)
{
	switch(inEvent)
	{
	case kEdsObjectEvent_DirItemRequestTransfer://下载图像到PC
		DownloadCommand(inRef);
		break;
	case kEdsObjectEvent_DirItemCreated://生成图像到相机
		//download_img(inRef, inContext);
			 break;
	case kEdsObjectEvent_DirItemContentChanged:
		break;
	default:
		//Object without the necessity is released
		if(inRef != NULL)
		{
			EdsRelease(inRef);
		}
		break;
	}

	return EDS_ERR_OK;		
}	

static EdsError EDSCALLBACK  handlePropertyEvent (
	EdsUInt32			inEvent,
	EdsUInt32			inPropertyID,
	EdsUInt32			inParam, 
	EdsVoid *			inContext				
	)
{
	switch(inEvent)
	{
	case kEdsPropertyEvent_PropertyChanged:
		getPropertyCommand(camera,inPropertyID);
		break;

	case kEdsPropertyEvent_PropertyDescChanged:
		//getPropertyDesc(inPropertyID);
		break;
	}

	return EDS_ERR_OK;		
}	

static EdsError EDSCALLBACK  handleStateEvent (
	EdsUInt32			inEvent,
	EdsUInt32			inParam, 
	EdsVoid *			inContext				
	)
{
	switch(inEvent)
	{
	case kEdsStateEvent_Shutdown:
		break;
	}

	return EDS_ERR_OK;		
}	




EdsError getPropertyDesc(EdsPropertyID propertyID)
{
	EdsError  err = EDS_ERR_OK;
	EdsPropertyDesc	 propertyDesc = {0};

	if(propertyID == kEdsPropID_Unknown)
	{
		//If unknown is returned for the property ID , the required property must be retrieved again
		if(err == EDS_ERR_OK) err = getPropertyDesc(kEdsPropID_AEModeSelect);
		if(err == EDS_ERR_OK) err = getPropertyDesc(kEdsPropID_Tv);
		if(err == EDS_ERR_OK) err = getPropertyDesc(kEdsPropID_Av);
		if(err == EDS_ERR_OK) err = getPropertyDesc(kEdsPropID_ISOSpeed);
		if(err == EDS_ERR_OK) err = getPropertyDesc(kEdsPropID_MeteringMode);
		if(err == EDS_ERR_OK) err = getPropertyDesc(kEdsPropID_ExposureCompensation);			
		if(err == EDS_ERR_OK) err = getPropertyDesc(kEdsPropID_ImageQuality);			

		return err;
	}		

	//Acquisition of value list that can be set
	if(err == EDS_ERR_OK)
	{
		err = EdsGetPropertyDesc( camera,
			propertyID,
			&propertyDesc);
	}


	return err;
}

/*******************************************************************************************************************/
EdsError getPropertyCommand(EdsCameraRef camera,EdsPropertyID propertyID)
{
	EdsError err = EDS_ERR_OK;
	EdsDataType	dataType = kEdsDataType_Unknown;
	EdsUInt32   dataSize = 0;


	if(propertyID == kEdsPropID_Unknown)
	{
		//If unknown is returned for the property ID , the required property must be retrieved again
		if(err == EDS_ERR_OK) err = getPropertyCommand(camera,kEdsPropID_AEModeSelect);
		if(err == EDS_ERR_OK) err = getPropertyCommand(camera,kEdsPropID_Tv);
		if(err == EDS_ERR_OK) err = getPropertyCommand(camera,kEdsPropID_Av);
		if(err == EDS_ERR_OK) err = getPropertyCommand(camera,kEdsPropID_ISOSpeed);
		if(err == EDS_ERR_OK) err = getPropertyCommand(camera,kEdsPropID_MeteringMode);
		if(err == EDS_ERR_OK) err = getPropertyCommand(camera,kEdsPropID_ExposureCompensation);
		if(err == EDS_ERR_OK) err = getPropertyCommand(camera,kEdsPropID_ImageQuality);

		return err;
	}

	//从相机对象或图像对象获取指定属性的字节大小和数据类型。
	if(err == EDS_ERR_OK)
	{
		err = EdsGetPropertySize(camera,propertyID,0,&dataType,&dataSize );
	}

	if(err == EDS_ERR_OK)
	{

		if(dataType == kEdsDataType_UInt32)
		{
			EdsUInt32 data;

			//获取属性类型
			err = EdsGetPropertyData(camera,propertyID,0,dataSize,&data );

			//Acquired property value is set
			if(err == EDS_ERR_OK)
			{
				SetPropertyCommand(camera,propertyID, data);
			}
		}

		if(dataType == kEdsDataType_String)
		{

			EdsChar str[EDS_MAX_NAME];
			//Acquisition of the property
			err = EdsGetPropertyData( camera,propertyID,0,dataSize,str );
		
			Sleep(500);
	
		}
		if(dataType == kEdsDataType_FocusInfo)
		{
			EdsFocusInfo focusInfo;
			//Acquisition of the property
			err = EdsGetPropertyData(camera,propertyID,0,dataSize,&focusInfo );

			Sleep(500);
		}
	}

	return err;
}

/*
propertyID可以有以下取值
kEdsPropID_AEModeSelect
kEdsPropID_Av
kEdsPropID_Tv
kEdsPropID_ISOSpeed
kEdsPropID_MeteringMode
kEdsPropID_ExposureCompensation
kEdsPropID_ImageQuality
kEdsPropID_Evf_ZoomPosition
kEdsPropID_Evf_Zoom
kEdsPropID_Evf_Histogram
kEdsPropID_Evf_CoordinateSystem
kEdsPropID_Evf_AFMode
*/
EdsError SetPropertyCommand(EdsCameraRef camera,EdsUInt32 propertyID, EdsUInt32 value)	
{
	EdsError err = EDS_ERR_OK;
	err = EdsSetPropertyData(camera, propertyID, 0 , sizeof(value),(EdsVoid *)&value);
	return err;
}

//获取属性数据列表
/*
propertyID可以有以下取值
kEdsPropID_AEModeSelect
kEdsPropID_ISOSpeed
kEdsPropID_MeteringMode
kEdsPropID_Av
kEdsPropID_Tv
kEdsPropID_ExposureCompensation
*/
EdsError getPropertyDescCommand(EdsCameraRef camera,EdsUInt32 propertyID,EdsPropertyDesc *Desc)
{
	EdsError err = EDS_ERR_OK;
	err = EdsGetPropertyDesc(camera, propertyID, Desc);
	return err;
}

/*
status可以有以下取值
kEdsCameraCommand_ShutterButton_Halfway//聚焦
kEdsCameraCommand_ShutterButton_Completely//拍摄
kEdsCameraCommand_ShutterButton_OFF//释放
*/
EdsError PressShutterButtonCommand(EdsCameraRef camera, EdsUInt32 status)	
{
	EdsError err = EDS_ERR_OK;
	err = EdsSendCommand(camera, kEdsCameraCommand_PressShutterButton, status);
	return err;
}

EdsError SaveSettingCommand()
{
	EdsSaveTo saveTo;
	EdsError err = EDS_ERR_OK;
	bool	 locked = false;
	err = EdsSendStatusCommand(camera, kEdsCameraStatusCommand_UILock, 0);

	if(err == EDS_ERR_OK)
	{
		locked = true;
	}		
	if(err == EDS_ERR_OK)
	{
		err = EdsSetPropertyData(camera, kEdsPropID_SaveTo, 0, sizeof(saveTo) , &saveTo);
	}
	if(locked)
	{
		EdsSendStatusCommand(camera, kEdsCameraStatusCommand_UIUnLock, 0);
	}
	return err;
}

EdsError TakePictureCommand(EdsCameraRef camera)
{
	EdsError err;
	bool	 locked = false;
	//PressShutterButtonCommand(camera,kEdsCameraCommand_ShutterButton_Completely);
	//PressShutterButtonCommand(camera,kEdsCameraCommand_ShutterButton_OFF);
	err = EdsSendStatusCommand(camera, kEdsCameraStatusCommand_UILock, 0);//设置锁定用户界面

	if(err == EDS_ERR_OK)
	{
		locked = true;
	}		
	err = EdsSendCommand(camera, kEdsCameraCommand_TakePicture, 0);//向远程摄像机发送诸如“拍摄”之类的命令
	if(locked)
	{
		err = EdsSendStatusCommand(camera, kEdsCameraStatusCommand_UIUnLock, 0);//解锁用户界面
	}

	return err;
}

EdsError StartEvfCommand(EdsCameraRef camera)//开始取景
{
	EdsError err = EDS_ERR_OK;
	// 获取实时视图图像的输出设备
	EdsUInt32 device;
	err = EdsGetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0 , sizeof(device), &device );
	// PC实时查看通过将PC设置为实时查看图像的输出设备开始。
	if(err == EDS_ERR_OK)
	{
		device |= kEdsEvfOutputDevice_PC;
		err = EdsSetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0 , sizeof(device), &device);
	}
	//如果属性设置成功，则相机发出属性更改事件通知。
	//一旦属性更改通知到达，就开始下载实时视图图像。
	return err;
}

EdsError DownloadEvfCommand(EdsCameraRef camera,CDC *pDC,CRect rect)
{
	EdsError err = EDS_ERR_OK;
	EdsStreamRef stream = NULL;
	EdsEvfImageRef evfImage= NULL;
	unsigned char* pBuff = NULL;  ;
	EdsUInt32 imageLen;
	// 创建内存流。
	err = EdsCreateMemoryStream( 0, &stream);
	// 创建 EvfImageRef.
	if(err == EDS_ERR_OK)
	{
		err = EdsCreateEvfImageRef(stream, &evfImage);
	}
	// 下载实时查看图像数据。
	if(err == EDS_ERR_OK)
	{
		err = EdsDownloadEvfImage(camera, evfImage);
	}

	EdsUInt32 zoom;
	EdsPoint point;
	EdsUInt32 histogram;
	EdsRect zoomRect;
	EdsSize sizeJpegLarge;
	// 获取图像的附带数据。
	//if(err == EDS_ERR_OK)
	//{
	//	// 获取缩放比例
	//	EdsGetPropertyData(evfImage,kEdsPropID_Evf_Zoom, 0 , sizeof(zoom), &zoom);
	//	// 获取焦点并缩放边框位置
	//	EdsGetPropertyData(evfImage,kEdsPropID_Evf_ZoomPosition, 0 , sizeof(point), &point);
	//	//获取直方图（RGBY）
	//	EdsGetPropertyData(evfImage, kEdsPropID_Evf_Histogram, 0, sizeof(histogram), &histogram);
	//	//获取焦点边框的矩形
	//	EdsGetPropertyData(evfImage, kEdsPropID_Evf_ZoomRect, 0, sizeof(zoomRect), &zoomRect);
	//	// 获取大小作为焦点边框矩形坐标的参考		
	//	EdsGetPropertyData(evfImage, kEdsPropID_Evf_CoordinateSystem, 0, sizeof(sizeJpegLarge), &sizeJpegLarge);
	//}
	////SetPropertyCommand(camera,kEdsPropID_Evf_Zoom,zoom);
	//SetPropertyCommand(camera,kEdsPropID_Evf_ZoomPosition,point);
	//SetPropertyCommand(camera,kEdsPropID_Evf_ZoomRect,zoomRect);
	//
	// 显示图像
	EdsGetPointer(stream,(EdsVoid**)&pBuff);
	EdsGetLength(stream,&imageLen);
	if (pBuff != NULL && imageLen!=0&&Cameradata.isOpenSession&&Cameradata.isEvf)
	{
		OnDrawImage(pDC,pBuff,int(imageLen),rect);
	}
	
	//
	// Release stream
	if(stream != NULL)
	{
		EdsRelease(stream);
		stream = NULL;
	}
	// Release evfImage
	if(evfImage != NULL)
	{
		EdsRelease(evfImage);
		evfImage = NULL;
	}
	return err;
}

EdsError EndEvfCommand(EdsCameraRef camera)
{
	EdsError err = EDS_ERR_OK;
	// Get the output device for the live view image
	EdsUInt32 device;
	err = EdsGetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0 , sizeof(device), &device );
	// 如果PC与实时取景图像输出设备断开，则PC实时取景结束。
	if(err == EDS_ERR_OK)
	{
		device &= ~kEdsEvfOutputDevice_PC;
		err = EdsSetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0 , sizeof(device), &device);
	}
	return err;
}

EdsError DownloadCommand(EdsDirectoryItemRef directoryItem)
{
	EdsError err = EDS_ERR_OK;
	EdsStreamRef stream = NULL;
	//获取目录项目信息
	EdsDirectoryItemInfo dirItemInfo;
	//dirItemInfo.size=19591915;
	//dirItemInfo.isFolder=0;
	//dirItemInfo.groupID=806466912;
	//dirItemInfo.option=2;
	//dirItemInfo.format=45315;
	//dirItemInfo.dateTime=0;


	//TCHAR sFilePath[100];
	//CString m_strBrFilePath;
	//GetModuleFileName(NULL,sFilePath,100);
	//m_strBrFilePath = sFilePath;
	//m_strBrFilePath = /*m_strBrFilePath.Left(m_strBrFilePath.ReverseFind('\\')) +*/ "IMG_9725.CR2";
	//DWORD dwAttr = GetFileAttributes(m_strBrFilePath);
	//if(dwAttr==0xFFFFFFFF)  //若文件夹不存在就创建
	//{
	//	CreateDirectory(m_strBrFilePath,NULL);
	//}
	//for (int i=0;i<	m_strBrFilePath.GetLength();i++)
	//{
	//	if (i<	m_strBrFilePath.GetLength())
	//	{
	//		dirItemInfo.szFileName[i]=m_strBrFilePath.GetAt(i);
	//	}
	//}
	err = EdsGetDirectoryItemInfo(directoryItem, & dirItemInfo);//获取有关远程相机中存储卡（音量）上的目录或文件对象的信息。

	// 为传输目标创建文件流
	if(err == EDS_ERR_OK)
	{
		err = EdsCreateFileStream( dirItemInfo.szFileName,kEdsFileCreateDisposition_CreateAlways,kEdsAccess_ReadWrite, &stream);//在主机上创建一个新文件（或打开一个现有文件）并创建一个文件流以访问该文件。如果在执行此API之前指定了新文件，则该文件实际上是在写入时间之后创建的通过EdsWrite等方式对开放流进行处理。
	}
	// Download image
	if(err == EDS_ERR_OK)
	{
		err = EdsDownload( directoryItem, dirItemInfo.size, stream);//将远程相机上的文件（在相机存储器或存储卡中）下载到主机，再将下载的文件直接发送到预先创建的文件流中。
	}
	// Issue notification that download is complete
	if(err == EDS_ERR_OK)
	{
		err = EdsDownloadComplete(directoryItem);//下载目录项目完成后必须调用。 执行此API使相机认识到文件传输已完成。
	}

	if(directoryItem != NULL)
	{
		err = EdsRelease( directoryItem);
		directoryItem = NULL;
	}
	// Release stream
	if( stream != NULL)
	{
		EdsRelease(stream);
		stream = NULL;
	}
	return err;
}

/*
driveLens可以有以下取值
kEdsEvfDriveLens_Near3
kEdsEvfDriveLens_Near2
kEdsEvfDriveLens_Near1
kEdsEvfDriveLens_Far3
kEdsEvfDriveLens_Far2
kEdsEvfDriveLens_Far1
*/
EdsError DriveLensCommand(EdsCameraRef camera, EdsUInt32 driveLens)
{
	EdsError err = EDS_ERR_OK;
	err = EdsSendCommand(camera, kEdsCameraCommand_DriveLensEvf, driveLens);
	return err;
}

/*
status有以下取值
kEdsCameraCommand_EvfAf_ON
kEdsCameraCommand_EvfAf_OFF
*/
EdsError DoEvfAFCommand(EdsCameraRef camera, EdsUInt32 status)
{
	EdsError err = EDS_ERR_OK;
	err = EdsSendCommand(camera, kEdsCameraCommand_DriveLensEvf, status);
	return err;
}

void OnDrawImage(CDC *pDC, unsigned char* pbyteImage, int size,CRect rect)
{
	CImage image;

	CComPtr<IStream> stream;
	stream = NULL;

	HGLOBAL hMem = ::GlobalAlloc(GHND, size); //从堆中分配一定数目的字节数 GHND为分配方式（可移动且初始为0）返回一个新分配的内存对象的句柄。
	LPVOID pBuff = ::GlobalLock(hMem);//锁定内存中指定的内存块，并返回一个地址值，令其指向内存块的起始处。除非用 GlobalUnlock 函数将内存块解锁，否则地址会一直保持有效。

	memcpy(pBuff, pbyteImage, size);

	::GlobalUnlock(hMem);//解锁内存块
	CreateStreamOnHGlobal(hMem, TRUE, &stream);//从指定内存创建流对象 TRUE：自动释放内存

	image.Load(stream);

	// Drawing
	SetStretchBltMode(pDC->GetSafeHdc() , COLORONCOLOR);
	image.StretchBlt(pDC->GetSafeHdc(),  rect.left,rect.top,rect.Width(),rect.Height(),0,0,image.GetWidth(), image.GetHeight(),SRCCOPY);

	image.Destroy();

	::GlobalFree(hMem);//释放指定的全局内存块
}

EdsError getFirstCamera(EdsCameraRef *camera)
{
	EdsError err = EDS_ERR_OK;
	EdsCameraListRef cameraList = NULL;
	EdsUInt32 count = 0;
	// 获取相机列表
	err = EdsGetCameraList(&cameraList);
	// 获取相机数量
	if(err == EDS_ERR_OK)
	{
		err = EdsGetChildCount(cameraList, &count);
		if(count == 0)
		{
			err = EDS_ERR_DEVICE_NOT_FOUND;
		}
	}
	// 获取第一台相机
	if(err == EDS_ERR_OK)
	{
		err = EdsGetChildAtIndex(cameraList , 0 , camera);
	}

	EdsDeviceInfo deviceInfo;
	if(err == EDS_ERR_OK)
	{	
		err = EdsGetDeviceInfo(*camera , &deviceInfo);	//获取设备信息，如设备名称。
		if(err == EDS_ERR_OK && camera == NULL)
		{
			err = EDS_ERR_DEVICE_NOT_FOUND;
		}
	}

	// 释放相机列表
	if(cameraList != NULL)
	{
		EdsRelease(cameraList);
		cameraList = NULL;
	}
	return err;
}
/*******************************************************************************************************************/




char * directory = "D:\\";
char * file_name = "IMG_9980.CR2";

void download_img(EdsBaseRef & object, EdsVoid * & context)
{
	EdsStreamRef stream = NULL;
	EdsDirectoryItemInfo dirItemInfo;
	EdsGetDirectoryItemInfo(object, &dirItemInfo);
	strcat(directory, file_name);
	EdsCreateFileStream(directory, kEdsFileCreateDisposition_CreateAlways, kEdsAccess_ReadWrite, &stream);
	EdsDownload(object, dirItemInfo.size, stream);
	EdsDownloadComplete(object);
	EdsRelease(stream);
	stream = NULL;
	if (object)
		EdsRelease(object);
}

//EdsError EDSCALLBACK handleObjectEvent(EdsObjectEvent inEvent, EdsBaseRef inRef, EdsVoid * inContext)
//{
//	download_img(inRef, inContext);
//	return EDS_ERR_OK;
//}

void init_camera(EdsCameraRef & camera)
{
	EdsError err = EDS_ERR_OK;
	EdsCameraListRef cameraList = NULL;
	EdsUInt32 count = 0;
	camera = NULL;

	err = EdsInitializeSDK();
	err = EdsGetCameraList(&cameraList);
	err = EdsGetChildCount(cameraList, &count);
	if (count > 0)
	{
		err = EdsGetChildAtIndex(cameraList, 0, &camera);
		EdsRelease(cameraList);
	}
	EdsSetObjectEventHandler(camera, kEdsObjectEvent_DirItemCreated, handleObjectEvent, NULL);
	EdsSendStatusCommand(camera, kEdsCameraStatusCommand_UIUnLock, 0);
}

void start_liveview(EdsCameraRef camera)
{
	EdsOpenSession(camera);
	EdsUInt32 device = kEdsPropID_Evf_OutputDevice;
	EdsGetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0, sizeof(device), &device);

	device |= kEdsEvfOutputDevice_PC;
	EdsSetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0, sizeof(device), &device);
}

void stop_liveview(EdsCameraRef camera)
{
	EdsUInt32 device;
	EdsGetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0, sizeof(device), &device);
	device &= ~kEdsEvfOutputDevice_PC;
	EdsSetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0,  sizeof(device), &device);
	EdsCloseSession(camera);
}

void take_video(EdsCameraRef camera, int length)
{
	EdsOpenSession(camera);
	EdsUInt32 record_start = 4; // Begin movie shooting 
	EdsSetPropertyData(camera, kEdsPropID_Record, 0, sizeof(record_start), &record_start);
	//this_thread::sleep_for(chrono::milliseconds(length));
	EdsUInt32 record_stop = 0; // End movie shooting 
	EdsSetPropertyData(camera, kEdsPropID_Record, 0, sizeof(record_stop), &record_stop);
	EdsCloseSession(camera);
}

void update_data(EdsCameraRef camera)
{
	EdsOpenSession(camera);
	// here event happens
	EdsCloseSession(camera);
}

void take_photo(EdsCameraRef camera, int count, int interv_millsec)
{
	for (int i = 0; i < count; ++i) {
		EdsOpenSession(camera);
	//	cout << "shoot" << endl;
		EdsSendCommand(camera, kEdsCameraCommand_TakePicture, 0);
		//this_thread::sleep_for(chrono::milliseconds(interv_millsec));
		Sleep(DWORD(interv_millsec));
		EdsCloseSession(camera);
	}
	//update_data(camera);
}

void dispose(EdsCameraRef camera)
{
	EdsCloseSession(camera);
	EdsTerminateSDK();
}
























//SAMPLE7 获取文件对象
EdsError getVolume(EdsCameraRef camera, EdsVolumeRef* volume)
{
	EdsError err = EDS_ERR_OK;
	EdsUInt32 count = 0;
	// 获取相机卷的数量
	err = EdsGetChildCount(camera, &count);
	if(err == EDS_ERR_OK && count == 0)
	{
		err =EDS_ERR_DIR_NOT_FOUND;
	}
	// 获取初始卷
	if(err == EDS_ERR_OK)
	{
		err = EdsGetChildAtIndex(camera, 0,volume);
	}
	return err;
}

//SAMPLE8 获取 DCIM 文件夹
EdsError getDCIMFolder(EdsVolumeRef volume, EdsDirectoryItemRef *directoryItem)
{
	EdsError err = EDS_ERR_OK;
	EdsDirectoryItemRef dirItem = NULL;
	EdsDirectoryItemInfo dirItemInfo;
	EdsUInt32 count = 0;
	// 获取卷下的项目数量
	err = EdsGetChildCount(volume, &count);
	if(err == EDS_ERR_OK && count == 0)
	{
		err =EDS_ERR_DIR_NOT_FOUND;
	}
	// 获取DCIM 文件夹
	for(int i = 0 ; i < count && err == EDS_ERR_OK; i++)
	{
		// 获取指定卷下第i个项目
		if(err == EDS_ERR_OK)
		{
			err = EdsGetChildAtIndex(volume,i, &dirItem);
		}
		// 获取检索的项目信息
		if(err == EDS_ERR_OK)
		{
			err = EdsGetDirectoryItemInfo(dirItem, &dirItemInfo);
		}
		// 指示检索到的项目是否是DCIM文件夹。
		if(err == EDS_ERR_OK)
		{
			if( stricmp(dirItemInfo.szFileName,"DCIM") == 0 &&dirItemInfo.isFolder == true)
			{
				*directoryItem = dirItem;
				break;
			}
		}
		// 释放检索的项目
		if(dirItem)
		{
			EdsRelease(dirItem);
			dirItem = NULL;
		}
	}
	return err;
}









void SaveToBmp(LPCTSTR path)
{
	EdsInitializeSDK();
	//g_dlg->m_prog.SetPos(0);
	//g_dlg->SetTimer(222,500,NULL);
	EdsImageRef m_ImageRef;
	EdsStreamRef     StreamRef;
//	IplImage *outImg;
	EdsError err;
	err = EdsCreateFileStream(path,kEdsFileCreateDisposition_OpenExisting,kEdsAccess_ReadWrite,&StreamRef);
	if(err==EDS_ERR_OK)
	{
		err = EdsCreateImageRef(StreamRef,&m_ImageRef);
		if(err==EDS_ERR_OK)
		{
			EdsImageInfo	thmInfo, fullInfo;
			EdsGetImageInfo( m_ImageRef , kEdsImageSrc_RAWThumbnail , &thmInfo );
			EdsGetImageInfo( m_ImageRef , kEdsImageSrc_RAWFullView , &fullInfo );
			EdsRect rect;
			rect.point.x=0;
			rect.point.y=0;
			rect.size=fullInfo.effectiveRect.size;
			EdsStreamRef DstStreamRef;
			err = EdsCreateMemoryStream(0 , &DstStreamRef );
			EdsImageSource source=kEdsImageSrc_RAWFullView;
			CSize size;
			size.SetSize(rect.size.width,rect.size.height);
			//CDrawImage *m_pDrawImage=new CDrawImage(size);
			time_t t1=GetTickCount();
			time_t t2;
			CString Str;
			CString StrOut;
			if(err==EDS_ERR_OK)
			{
				//err = EdsSetProgressCallback(DstStreamRef, ProgressFunc, kEdsProgressOption_Periodically, this);
				err = EdsGetImage( m_ImageRef , source , kEdsTargetImageType_RGB , rect , rect.size , DstStreamRef );
				t2=GetTickCount();
				Str.Format("GetImage:%d ms\r\n",t2-t1);
				StrOut+=Str;
				EdsVoid* pBuff;
				EdsUInt32 imageLen;
				t1=GetTickCount();
				EdsGetPointer( DstStreamRef , &pBuff );//返回RGB数据指针
				EdsGetLength(DstStreamRef,&imageLen);
				ofstream out("d:\\out.txt");  
				if (out.is_open())   
				{  
					out << DstStreamRef;  
					out.close();  
				}  
				//EdsWriteStream("d:\\23.CR2",rect.size,pBuff,&imageLen);
				//byte *bmpData = new byte[imageLen*sizeof(BYTE)];
				WORD *bmpData = (WORD *)pBuff;
				


				for(int i=rect.size.height*rect.size.width-1;i>0;i--)  //swap WORD hi&lo and get max
				{
					bmpData[i] = (bmpData[i]>>8)+((bmpData[i]&0xff)<<8);  //位图中的每个像素点占2个字节(高低八位互换)

				}

				int nHeight=rect.size.height/2;
				int nWidth=rect.size.width/2;
				WORD * m_bmpDataR = new WORD[nWidth*nHeight];
				WORD* m_bmpDataG = new WORD[nWidth*nHeight]; 
				WORD* m_bmpDataB = new WORD[nWidth*nHeight];
		
				for(int i=0;i<nHeight;i++)     //将原图片中的点以4个为一组，保存的点数只有原来的四分之一 
				{
					for(int j=0;j<nWidth;j++)
					{                                                                                                   ////////////////--------=
						m_bmpDataB[i*nWidth+j]  = bmpData[i*nWidth*4+nWidth*2+j*2+1];                                   ////////////////| R | G |	
						m_bmpDataG[i*nWidth+j]  = ((bmpData[i*nWidth*4+nWidth*2+j*2]+bmpData[i*nWidth*4+j*2+1])>>1);    ////////////////| G | B |
						m_bmpDataR[i*nWidth+j]  = bmpData[i*nWidth*4+j*2];                                              ////////////////---------
					}
				}


				CString str="D:\\img.bmp";
				SaveData2Bmp(nWidth,nHeight,(byte*)m_bmpDataR,(byte*)m_bmpDataG,(byte*)m_bmpDataB,str);

				delete m_bmpDataR;
				delete m_bmpDataG;
				delete m_bmpDataB;
				delete bmpData;
				m_bmpDataR=NULL;
				m_bmpDataG=NULL;
				m_bmpDataB=NULL;
				bmpData=NULL;
				pBuff=NULL;

				//byte* rValues;
				//byte* gValues;
				//byte* bValues;
	

				//rValues = new byte[rect.size.width*rect.size.height*sizeof(BYTE)];//记得释放内存delete
				//gValues = new byte[rect.size.width*rect.size.height*sizeof(BYTE)];
				//bValues = new byte[rect.size.width*rect.size.height*sizeof(BYTE)];


				//for(int i=0;i<rect.size.height;i++)     //将原图片中的点以4个为一组，保存的点数只有原来的四分之一 
				//{
				//	for(int j=0;j<rect.size.width;j++)
				//	{               
				//		int idx=i*rect.size.width+j;
				//		rValues[idx]  = bmpData[i*3];                                  
				//		gValues[idx]  = bmpData[i*3+1];   
				//		bValues[idx]  = bmpData[i*3+2];                                     
				//	}
				//}

				//CString str="D:\\img.bmp";
				//SaveData2Bmp(rect.size.width,rect.size.height,rValues,gValues,bValues,str);
		
				//delete rValues;
				//delete gValues;
				//delete bValues;
				//delete bmpData;
				//rValues=NULL;
				//gValues=NULL;
				//bValues=NULL;
				//bmpData=NULL;
				//pBuff=NULL;



				//FILE *fp = fopen("E:\\bmpData.txt","w");
				//CString str12;
				//for(int i=0;i<rect.size.height;i++)
				//{
				//	for(int j=0;j<rect.size.width;j++)
				//	{
				//		str12.Format("%u\n",bmpData[rect.size.width*i+j]);
				//		fwrite(str12,1,strlen(str12),fp);
				//	}
				//}
				//fclose(fp);

				//t2=GetTickCount();
				//Str.Format("GetPointer:%d ms\r\n",t2-t1);
				//StrOut+=Str;
				//t1=GetTickCount();
				//m_pDrawImage->SetImage( rect.size.width , rect.size.height , pBuff );//加工成位图,再显示
				//const unsigned char *p=(const unsigned char *)pBuff;
				//outImg = cvCreateImage(cvSize(rect.size.width,rect.size.height),IPL_DEPTH_8U ,3);//8位3通道
				//for(int i = 0; i < outImg->height; i++)
				//{
				//	for( int j = 0; j < outImg->width; j++)
				//	{
				//		((uchar*)(outImg->imageData+outImg->widthStep*i))[j*outImg->nChannels + 2] = *(p++);
				//		((uchar*)(outImg->imageData+outImg->widthStep*i))[j*outImg->nChannels + 1] = *(p++);
				//		((uchar*)(outImg->imageData+outImg->widthStep*i))[j*outImg->nChannels + 0] = *(p++);
				//	}
				//}
				//t2=GetTickCount();
				//Str.Format("SetImage:%d ms\r\n",t2-t1);
				//StrOut+=Str;
				//t1=GetTickCount();
				//cvSaveImage("E:\\0.jpg",outImg);
				////m_pDrawImage->Draw(::GetDC(NULL));
				//t2=GetTickCount();
				//Str.Format("GetDraw:%d ms\r\n",t2-t1);
				//StrOut+=Str;
			}
			//EdsDirectoryItemRef _directoryItem;
			//_directoryItem = static_cast<EdsBaseRef>(m_ImageRef);
			//EdsDirectoryItemInfo outDirItemInfo;
			//outDirItemInfo.groupID = 1;
			//outDirItemInfo.isFolder = false;
			//outDirItemInfo.option = NULL;
			//outDirItemInfo.size = rect.size;
			//outDirItemInfo.format = kEdsImageType_CR2;
			//char *name = "f:\\2\\";
			//strncpy(outDirItemInfo.szFileName,name,6);

			//char *name = "00000.CR2\0";
			//strncpy(outDirItemInfo.szFileName,name,9);
			//EdsGetDirectoryItemInfo(_directoryItem,&outDirItemInfo);
			//EdsWrite();

			//OutputDebugString(StrOut);
			//delete m_pDrawImage;
		}
	}

	EdsTerminateSDK();
}


void SaveData2Bmp(int w,int h,BYTE* rValues,BYTE* gValues,BYTE* bValues,CString strFileName)
{
	BITMAPINFOHEADER bih;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = w;
	bih.biHeight = -h;
	bih.biPlanes = 1;
	bih.biBitCount = 24;
	bih.biCompression = BI_RGB;
	bih.biSizeImage =w*h * 3;
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;

	BITMAPFILEHEADER bfh;
	bfh.bfType = ((WORD)('M' << 8) | 'B');
	bfh.bfSize = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) +w*h* 3;
	bfh.bfReserved1 = 0;
	bfh.bfReserved2 = 0;
	bfh.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

	BYTE *bmpBuff = new BYTE[w*h * 3];//记得释放内存delete
	memset(bmpBuff, 0, w*h * 3);
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			int idx = i*w + j;

			bmpBuff[i*w * 3 + 3 * j] = (BYTE)(bValues[idx]);
			bmpBuff[i*w * 3 + 3 * j + 1] = (BYTE)(gValues[idx]);
			bmpBuff[i*w * 3 + 3 * j + 2] = (BYTE)(rValues[idx]);
		}
	}

	CFile file;
	if (file.Open(strFileName, CFile::modeWrite | CFile::modeCreate))
	{
		file.Write((LPSTR)&bfh, sizeof(BITMAPFILEHEADER));
		file.Write((LPSTR)&bih, sizeof(BITMAPINFOHEADER));
		file.Write(bmpBuff, w*h * 3);
		file.Close();
	}
	delete[] bmpBuff;
}


void datatorgb(WORD* pixels,int w, int h,byte* rgbValues)
{
	double avg = 0;
	double total = 0;
	double deltaSquared = 0;
	double std = 0;
	double pl;
	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			int idx=j*w+i;
			total +=double(pixels[idx]);
		}
	}

	avg = total / (w * h);//求平均值
	for (int j = 0; j <h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			int idx=j*w+i;
			deltaSquared += pow(double(avg - pixels[idx]),2);//方差和
		}
	}
	std = sqrt(double(deltaSquared / ((w * h) - 1)));//标准差

	// 将比例像素重新缩放到三个标准偏差以进行显示
	double minVal = avg - std * 3;
	if (minVal < 0) minVal = 0;
	double maxVal = avg + std * 3;
	if (maxVal > 65535) maxVal = 65535;
	double range = maxVal - minVal;
	double spread = 65535 / range;

	byte level;
	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			int idx=j*w+i;
			pl = (double)pixels[idx];
			// 展开像素值以便更好地观看
			pl = (pl - minVal) * spread;//将像素展开到0—65535之间（计算之后最小的为0最大的为65535，逐级展开）

			if (pl < 0) pl = 0;
			// 缩放像素值
			pl = (pl * 255) / 65535;
			if (pl > 255) pl = 255;
			level = (byte)pl;
			rgbValues[idx] = level;//B
		}
	}
}
std::map<EdsUInt32, const char *> _propertyTable;
std::map<EdsUInt32, const char *> _propertyTable1;
std::map<EdsUInt32, const char *> _propertyTable2;

void CTv()
{
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x0c,"Bulb"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x10,"30″"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x13,"25″"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x14,"20″"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x15,"20″"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x18,"15″"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x1B,"13″"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x1C,"10″"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x1D,"10″"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x20,"8″"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x23,"6″"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x24,"6″"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x25,"5″"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x28,"4″"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x2B,"3″2"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x2C,"3″"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x2D,"2″5"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x30,"2″"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x33,"1″6"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x34,"1″5"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x35,"1″3"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x38,"1″"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x3B,"0″8"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x3C,"0″7"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x3D,"0″6"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x40,"0″5"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x43,"0″4"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x44,"0″3"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x45,"0″3"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x48,"4"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x4B,"5"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x4C,"6"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x4D,"6"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x50,"8"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x53,"10"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x54,"10"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x55,"13"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x58,"15"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x5B,"20"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x5C,"20"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x5D,"25"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x60,"30"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x63,"40"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x64,"45"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x65,"50"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x68,"60"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x6B,"80"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x6C,"90"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x6D,"100"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x70,"125"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x73,"160"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x74,"180"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x75,"200"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x78,"250"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x7B,"320"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x7C,"350"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x7D,"400"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x80,"500"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x83,"640"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x84,"750"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x85,"800"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x88,"1000"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x8B,"1250"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x8C,"1500"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x8D,"1600"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x90,"2000"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x93,"2500"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x94,"3000"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x95,"3200"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x98,"4000"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x9B,"5000"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x9C,"6000"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0x9D,"6400"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0xA0,"8000"));
	_propertyTable2.insert( std::pair<EdsUInt32, const char *>(0xffffffff,"unknown"));
}
void CIso()
{
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0x00,"Auto"));
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0x28,"6"));
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0x30,"12"));
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0x38,"25"));
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0x40,"50"));
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0x48,"100"));
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0x4b,"125"));
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0x4d,"160"));
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0x50,"200"));
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0x53,"250"));
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0x55,"320"));
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0x58,"400"));
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0x5b,"500"));
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0x5d,"640"));
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0x60,"800"));
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0x63,"1000"));
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0x65,"1250"));
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0x68,"1600"));
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0x6b,"2000"));
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0x6d,"2500"));
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0x70,"3200"));
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0x73,"4000"));
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0x75,"5000"));
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0x78,"6400"));
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0x7b,"8000"));
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0x7d,"10000"));
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0x80,"12800"));
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0x83,"16000"));
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0x85,"20000"));
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0x88,"25600"));
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0x8b,"32000"));
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0x8d,"40000"));
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0x90,"51200"));
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0x98,"102400"));
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0xa0,"204800"));
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0xa8,"409600"));
	_propertyTable1.insert( std::pair<EdsUInt32, const char *>(0xffffffff,"unknown"));
}
void CAv()
{
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x00,"00"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x08,"1"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x0B,"1.1"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x0C,"1.2"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x0D,"1.2"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x10,"1.4"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x13,"1.6"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x14,"1.8"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x15,"1.8"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x18,"2"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x1B,"2.2"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x1C,"2.5"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x1D,"2.5"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x20,"2.8"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x23,"3.2"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x24,"3.5"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x25,"3.5"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x28,"4"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x2B,"4.5"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x2C,"4.5"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x2D,"5.0"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x30,"5.6"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x33,"6.3"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x34,"6.7"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x35,"7.1"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x38,"8"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x3B,"9"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x3C,"9.5"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x3D,"10"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x40,"11"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x43,"13"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x44,"13"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x45,"14"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x48,"16"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x4B,"18"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x4C,"19"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x4D,"20"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x50,"22"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x53,"25"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x54,"27"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x55,"29"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x58,"32"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x5B,"36"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x5C,"38"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x5D,"40"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x60,"45"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x63,"51"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x64,"54"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x65,"57"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x68,"64"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x6B,"72"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x6C,"76"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x6D,"80"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0x70,"91"));
	_propertyTable.insert( std::pair<EdsUInt32, const char *>(0xffffffff,"unknown"));




	


}

CString updateProperty(int mode,EdsUInt32 value)
{
	if(mode==0)
	{
		std::map<EdsUInt32, const char*>::iterator itr = _propertyTable.find(value);
		if (itr != _propertyTable.end())
		{		
		// Set String combo box
		return CString(itr->second);
		}
	}
	else if (mode==1)
	{
		std::map<EdsUInt32, const char*>::iterator itr = _propertyTable1.find(value);
		if (itr != _propertyTable1.end())
		{		
		// Set String combo box
		return CString(itr->second);
		}
	}
	else
	{
		std::map<EdsUInt32, const char*>::iterator itr = _propertyTable2.find(value);
		if (itr != _propertyTable2.end())
		{		
		// Set String combo box
		return CString(itr->second);
		}
	}
}