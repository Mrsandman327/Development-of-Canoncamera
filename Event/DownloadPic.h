#pragma once
#include "Observer.h"
#include "ActionSource.h"
#include "CameraEvent.h"
#include "CameraController.h"

#define SATRTDOWN 0
#define ENDDOWN	  1
#define DOWNING   2

typedef void(CALLBACK *DOWNLOADPROGRESSCALLBACK)(EdsUInt32 percent,int state,LPARAM lParam);
class CDownloadPic : public ActionSource , public Observer
{
public:
	//CDownloadPic(void);
	//~CDownloadPic(void);

	DOWNLOADPROGRESSCALLBACK _downloadstate;
	LPARAM					_lParam;
	void SetCallback(DOWNLOADPROGRESSCALLBACK downloadstate,LPARAM lParam){_downloadstate=downloadstate;_lParam=lParam;}

	virtual void update(Observable* from, CameraEvent *e)
	{
		std::string event = e->getEvent();

		if(event == "DownloadStart")
		{
			_downloadstate(0,0,_lParam);
		}
		if(event == "DownloadComplete")
		{
			_downloadstate(100,1,_lParam);
		}

		if(event == "ProgressReport")
		{
			EdsInt32 percent = *static_cast<EdsInt32 *>(e->getArg());
			_downloadstate(percent,2,_lParam);
		}

		/*if(event == "error")
		{
		_downloadstate(0,3,_lParam);
		}*/
	}
};

