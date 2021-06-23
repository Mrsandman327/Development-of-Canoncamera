#pragma once
#include "Observer.h"
#include "ActionSource.h"
#include "CameraEvent.h"
#include "CameraController.h"


typedef void(CALLBACK *EVFAFMODECALLBACK)(EdsPropertyDesc EvfAFModeDesc,EdsUInt32 EvfAFMode,LPARAM lParam);
class CEvfAFMode : public ActionSource , public Observer
{
public:
	CEvfAFMode()
	{
		_EvfAFModeDesc.access=0;
		_EvfAFModeDesc.form=0;
		_EvfAFModeDesc.numElements=0;
		memset(&_EvfAFModeDesc.propDesc, 0, sizeof(_EvfAFModeDesc.propDesc));
		_EvfAFMode=0;
	}
public:
	CameraModel*		_model;
	void SetCameraModel(CameraModel* model){_model=model;}
	EVFAFMODECALLBACK			_evfafmodestate;
	LPARAM				_lParam;
	EdsPropertyDesc		_EvfAFModeDesc;
	EdsUInt32			_EvfAFMode;
	void SetCallback(EVFAFMODECALLBACK evfafmodestate,LPARAM lParam){_evfafmodestate=evfafmodestate;_lParam=lParam;}
public:
	virtual void update(Observable* from, CameraEvent *e)
	{
		std::string event = e->getEvent();

		//Update property
		if(event == "PropertyChanged")
		{
			EdsInt32 propertyID = *static_cast<EdsInt32 *>(e->getArg());

			if(propertyID == kEdsPropID_Evf_AFMode)
			{
				_EvfAFMode=_model->getEvfAFMode();
				_evfafmodestate(_EvfAFModeDesc,_EvfAFMode,_lParam);
			}
		}

		//Update of list that can set property
		if(event == "PropertyDescChanged")
		{
			EdsInt32 propertyID = *static_cast<EdsInt32 *>(e->getArg());

			if(propertyID == kEdsPropID_Evf_AFMode)
			{
				_EvfAFModeDesc=_model->getEvfAFModeDesc();
				_evfafmodestate(_EvfAFModeDesc,_EvfAFMode,_lParam);
			}	
		}
	}
};
