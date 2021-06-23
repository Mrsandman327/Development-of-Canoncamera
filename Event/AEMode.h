#pragma once
#include "Observer.h"
#include "ActionSource.h"
#include "CameraEvent.h"
#include "CameraController.h"

typedef void(CALLBACK *AECALLBACK)(EdsPropertyDesc AEDesc,EdsUInt32 Tv,LPARAM lParam);
class CAEMode :  public ActionSource , public Observer  
{
public:
	CAEMode()
	{
		_AEDesc.access=0;
		_AEDesc.form=0;
		_AEDesc.numElements=0;
		memset(&_AEDesc.propDesc, 0, sizeof(_AEDesc.propDesc));
		_AEMode=0;
	}

public:
	CameraModel*		_model;
	void SetCameraModel(CameraModel* model){_model=model;}

	AECALLBACK		_aestate;
	LPARAM			_lParam;
	EdsPropertyDesc _AEDesc;
	EdsUInt32		_AEMode;
	void SetCallback(AECALLBACK aestate,LPARAM lParam){_aestate=aestate;_lParam=lParam;}
public:
	void update(Observable* from, CameraEvent *e)
	{

		std::string event = e->getEvent();

		//Update property
		if(event == "PropertyChanged")
		{
			EdsInt32 propertyID = *static_cast<EdsInt32 *>(e->getArg());

			if(propertyID == kEdsPropID_AEModeSelect)
			{
				_AEMode=_model->getAEMode();
				_aestate(_AEDesc,_AEMode,_lParam);
			}
		}

		//Update of list that can set property
		if(event == "PropertyDescChanged")
		{
			EdsInt32 propertyID = *static_cast<EdsInt32 *>(e->getArg());

			if(propertyID == kEdsPropID_AEModeSelect)
			{
				_AEDesc=_model->getAEModeDesc();
				_aestate(_AEDesc,_AEMode,_lParam);
			}	
		}
	}
};