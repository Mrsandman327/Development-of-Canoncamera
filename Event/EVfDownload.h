#pragma once

#include "Observer.h"
#include "ActionSource.h"
#include "CameraEvent.h"
#include "CameraController.h"


typedef void(CALLBACK *IMAGEDATACALLBACK)(CComPtr<IStream> stream,LPARAM lParam);
class CEVfDownload : public ActionSource , public Observer
{
public:
	CEVfDownload(void)
	{
		active = FALSE;
		memset(&m_focusInfo, 0, sizeof(EdsFocusInfo));
	}
	//~CEVFPicture(void);
public:
	virtual void update(Observable* from, CameraEvent *e)
	{

		std::string event = e->getEvent();

		if(event == "EvfDataChanged")
		{
			EVF_DATASET data = *static_cast<EVF_DATASET *>(e->getArg());

			//The update processing can be executed from another thread. 
			SaveImage(data,m_path);

			EdsInt32 propertyID = kEdsPropID_FocusInfo;
			fireEvent("get_Property", &propertyID);

			// Download image data.
			fireEvent("downloadEVF");
		}

		if (event == "PropertyChanged")
		{
			EdsInt32 proeprtyID = *static_cast<EdsInt32 *>(e->getArg());
			if(proeprtyID == kEdsPropID_Evf_OutputDevice)
			{
				CameraModel* model = (CameraModel *)from;
				EdsUInt32 device = model->getEvfOutputDevice();

				// PC live view has started.
				if (!active && (device & kEdsEvfOutputDevice_PC) != 0)
				{
					active = TRUE;
					// Start download of image data.
					fireEvent("downloadEVF");
				}

				// PC live view has ended.
				if(active && (device & kEdsEvfOutputDevice_PC) == 0)
				{
					active = FALSE;
				}
			}
			if(proeprtyID == kEdsPropID_FocusInfo)
			{
				//CameraModel* model = (CameraModel *)from;
				//float xRatio = 1;
				//float yRatio = 1;
				//CRect rect;
				//GetWindowRect(&rect);

				//EdsFocusInfo focusInfo = {0};
				//focusInfo = model->getFocusInfo();
				//m_focusInfo = focusInfo;
				//xRatio = (float)(rect.Width())/(float)(focusInfo.imageRect.size.width);
				//yRatio = (float)(rect.Height())/(float)(focusInfo.imageRect.size.height);
				//for(EdsUInt32 i = 0; i < focusInfo.pointNumber; i++)
				//{
				//	m_focusInfo.focusPoint[i].rect.point.x = (EdsUInt32)(m_focusInfo.focusPoint[i].rect.point.x * xRatio);
				//	m_focusInfo.focusPoint[i].rect.point.y = (EdsUInt32)(m_focusInfo.focusPoint[i].rect.point.y * yRatio);
				//	m_focusInfo.focusPoint[i].rect.size.width = (EdsUInt32)(m_focusInfo.focusPoint[i].rect.size.width * xRatio);
				//	m_focusInfo.focusPoint[i].rect.size.height = (EdsUInt32)(m_focusInfo.focusPoint[i].rect.size.height * yRatio);
				//}
			}
			if(proeprtyID==kEdsPropID_Evf_AFMode)
			{
				CameraModel* model = (CameraModel *)from;
				m_bDrawZoomFrame = model->getEvfAFMode()!=2;
			}
		}	
	}
private:
	BOOL active;
	EdsFocusInfo	m_focusInfo;
	EdsBool			m_bDrawZoomFrame;
	LPCTSTR			m_path;

public:	
	IMAGEDATACALLBACK _imagedata;
	LPARAM			  _lParam;
	void SetCallback(IMAGEDATACALLBACK imagedata,LPARAM lParam){_imagedata=imagedata;_lParam=lParam;};


	void SaveImage(EVF_DATASET data,LPCTSTR path)
	{
		EdsUInt64 size;

		unsigned char* pbyteImage = NULL; 

		// Get image (JPEG) pointer.
		EdsGetPointer(data.stream, (EdsVoid**)&pbyteImage );

		if (pbyteImage!=NULL)
		{
			EdsGetLength(data.stream, &size);


			CComPtr<IStream> stream;
			stream = NULL;

			HGLOBAL hMem = ::GlobalAlloc(GHND, size); //从堆中分配一定数目的字节数 GHND为分配方式（可移动且初始为0）返回一个新分配的内存对象的句柄。
			LPVOID pBuff = ::GlobalLock(hMem);//锁定内存中指定的内存块，并返回一个地址值，令其指向内存块的起始处。除非用 GlobalUnlock 函数将内存块解锁，否则地址会一直保持有效。

			memcpy(pBuff, pbyteImage, size);

			::GlobalUnlock(hMem);//解锁内存块
			CreateStreamOnHGlobal(hMem, TRUE, &stream);//从指定内存创建流对象 TRUE：自动释放内存

			_imagedata(stream,_lParam);




			//CImage image;
			//image.Load(stream);

			// Drawing
			//SetStretchBltMode(pDC->GetSafeHdc() , COLORONCOLOR);
			//image.StretchBlt(pDC->GetSafeHdc(),  rect.left,rect.top,rect.Width(),rect.Height(),0,0,image.GetWidth(), image.GetHeight(),SRCCOPY);

			//Save
			//image.Save(path);
			//image.Destroy();

			::GlobalFree(hMem);//释放指定的全局内存块
		}
	}
};

