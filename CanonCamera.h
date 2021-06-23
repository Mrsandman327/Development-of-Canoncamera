#pragma once
#include <map>
#include "EDSDK/Header/EDSDK.h"
#pragma comment (lib,"EDSDK/Library/EDSDK.lib")

#include "Camrea/CameraModel.h"
#include "Camrea/CameraModelLegacy.h"
#include "Camrea/CameraController.h"
#include "Camrea/CameraEventListener.h"
#include "Class/ActionSource.h"
#include "Event/EVfDownload.h"
#include "Event/DownloadPic.h"
#include "Event/Tv.h"
#include "Event/Av.h"
#include "Event/Iso.h"
#include "Event/AEMode.h"
#include "Event/MeteringMode.h"
#include "Event/ExposureComp.h"
#include "Event/ImageQuality.h"
#include "Event/EvfAFMode.h"

class CanonCamera
{
public:
	CanonCamera(void);
	~CanonCamera(void);
public:
	ActionSource	_TakePicture;
	ActionSource	_StartEVF;
	ActionSource	_EndEVF;
	ActionSource	_FocusNear3;
	ActionSource	_FocusNear2;
	ActionSource	_FocusNear1;
	ActionSource	_FocusFar3;
	ActionSource	_FocusFar2;
	ActionSource	_FocusFar1;
	ActionSource	_FocusUp;
	ActionSource	_FocusDown;
	ActionSource	_FocusLeft;
	ActionSource	_FocusRight;
	ActionSource	_ZoomFit;
	ActionSource    _ZoomZoom;

	//ActionSource	_PressingHalfway;//
	//ActionSource	_PressingCompletely;
	//ActionSource	_PressingOff;
	//ActionSource	_EvfAfON;
	//ActionSource	_EvfAfOFF;

	CDownloadPic    _Download;
	CEVfDownload    _DownloadEvf;
	CTv             _Tv;
	CAv				_Av;
	CIso			_Iso;
	CAEMode			_AEMode;
	CMeteringMode   _MeteringMode;//测光模式
	CExposureComp   _ExposureComp;//曝光补偿
	CImageQuality   _ImageQuality;//输出图像格式
	CEvfAFMode      _EvfAFMode;
public:
	EdsCameraRef		camera;
	CameraModel*		_model;
	CameraController*	_controller;
	bool				isSDKLoaded;
	bool				isConnected;

	//参数映射表
	std::map<EdsUInt32, const char *> _TvDesc;	
	std::map<EdsUInt32, const char *> _AvDesc;
	std::map<EdsUInt32, const char *> _IsoDesc;
	std::map<EdsUInt32, const char *> _AEDesc;
	std::map<EdsUInt32, const char *> _MeteringModeDesc;
	std::map<EdsUInt32, const char *> _ExposureCompDesc;
	std::map<EdsUInt32, const char *> _ImageQualityDesc;
	std::map<EdsUInt32, const char *> _EvfAFModeDesc;

	//获取连接转态
	bool Canon_GetCameraConnectedState(void){return isConnected;};
	//连接
	bool Canon_ListCamerasAndConnect(void);
	bool Canon_DisConnect(void);
	//设置订阅者和被订阅者
	void Canon_setupListener(ActionListener* listener);
	void Canon_setupObserver(Observable* ob);
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


	CameraController* getController(){return _controller;};
	CameraModel* getModel(){return _model;};
	BOOL IsValidate(void) {return (_controller == NULL || _model == NULL)?false:true;};
	CameraModel* cameraModelFactory(EdsCameraRef camera, EdsDeviceInfo deviceInfo)
	{
		// if Legacy protocol.
		if(deviceInfo.deviceSubType == 0)
		{
			return new CameraModelLegacy(camera);//旧协议
		}

		// PTP protocol.
		return new CameraModel(camera);
	}
};

