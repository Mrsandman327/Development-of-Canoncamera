#pragma once
class Canon
{
public:
	Canon(void);
	~Canon(void);
};

#include "EDSDK.h"
#include "EDSDKErrors.h"
#include "EDSDKTypes.h"
#include <map>
//using namespace std;
void CAv();//
void CTv();//
void CIso();//
CString updateProperty(int mode,EdsUInt32 value);//
extern std::map<EdsUInt32, const char *> _propertyTable;//
extern std::map<EdsUInt32, const char *> _propertyTable1;//
extern std::map<EdsUInt32, const char *> _propertyTable2;//

void InitStruct();//

void applicationRun();//
void applicationEnd();//
EdsError getFirstCamera(EdsCameraRef *camera);//
EdsError getVolume(EdsCameraRef camera, EdsVolumeRef * volume);
EdsError getDCIMFolder(EdsVolumeRef volume, EdsDirectoryItemRef * directoryItem);

void OnDrawImage(CDC *pDC, unsigned char* pbyteImage, int size,CRect rect);//

EdsError getPropertyCommand(EdsCameraRef camera,EdsPropertyID propertyID);//
EdsError SetPropertyCommand(EdsCameraRef camera,EdsUInt32 propertyID, EdsUInt32 value);//
EdsError PressShutterButtonCommand(EdsCameraRef camera, EdsUInt32 status);//
EdsError TakePictureCommand(EdsCameraRef camera);//
EdsError DownloadCommand(EdsDirectoryItemRef directoryItem);//
EdsError StartEvfCommand(EdsCameraRef camera);//
EdsError DownloadEvfCommand(EdsCameraRef camera,CDC *pDC,CRect rect);//
EdsError EndEvfCommand(EdsCameraRef camera);//
EdsError DriveLensCommand(EdsCameraRef camera, EdsUInt32 driveLens);//
EdsError DoEvfAFCommand(EdsCameraRef camera, EdsUInt32 status);
EdsError getPropertyDescCommand(EdsCameraRef camera,EdsUInt32 propertyID,EdsPropertyDesc *Desc);//
EdsError SaveSettingCommand();//

EdsError getPropertyDesc(EdsPropertyID propertyID);
void download_img(EdsBaseRef & object, EdsVoid * & context);
void dispose(EdsCameraRef camera);
void take_photo(EdsCameraRef camera, int count, int interv_millsec);
void update_data(EdsCameraRef camera);
void take_video(EdsCameraRef camera, int length);
void stop_liveview(EdsCameraRef camera);
void start_liveview(EdsCameraRef camera);
void init_camera(EdsCameraRef & camera);



void SaveData2Bmp(int w,int h,BYTE* rValues,BYTE* gValues,BYTE* bValues,CString strFileName);
void SaveToBmp(LPCTSTR path);
void datatorgb(WORD* pixels,int w, int h,byte* rgbValues);
static EdsError EDSCALLBACK  handleObjectEvent( 
	EdsUInt32			inEvent,
	EdsBaseRef			inRef,
	EdsVoid *			inContext				
	);//
static EdsError EDSCALLBACK  handlePropertyEvent(
	EdsUInt32			inEvent,
	EdsUInt32			inPropertyID,
	EdsUInt32			inParam, 
	EdsVoid *			inContext				
	);//
static EdsError EDSCALLBACK  handleStateEvent(
	EdsUInt32			inEvent,
	EdsUInt32			inParam, 
	EdsVoid *			inContext				
	);//


struct CameraData
{
	long AvDesc[128];
	long TvDesc[128];
	long IsoDesc[128];
	long curAv;
	long curTv;
	long curIso;
	BOOL isOpenSession;
	BOOL isEvf;
	bool isfinsh;
};


extern CameraData Cameradata;
extern EdsCameraRef camera;