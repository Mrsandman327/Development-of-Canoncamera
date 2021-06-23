#pragma once
#include "Observer.h"
#include "ActionSource.h"
#include "CameraEvent.h"
#include "CameraController.h"


typedef void(CALLBACK *METERINGMODECALLBACK)(EdsPropertyDesc MeteringModeDesc,EdsUInt32 MeteringMode,LPARAM lParam);
class CMeteringMode : public ActionSource , public Observer
{
public:
	CMeteringMode()
	{
		_MeteringModeDesc.access=0;
		_MeteringModeDesc.form=0;
		_MeteringModeDesc.numElements=0;
		memset(&_MeteringModeDesc.propDesc, 0, sizeof(_MeteringModeDesc.propDesc));
		_MeteringMode=0;
	}
public:
	CameraModel*		_model;
	void SetCameraModel(CameraModel* model){_model=model;}
	METERINGMODECALLBACK			_meteringmodestate;
	LPARAM				_lParam;
	EdsPropertyDesc		_MeteringModeDesc;
	EdsUInt32			_MeteringMode;
	void SetCallback(METERINGMODECALLBACK meteringmodestate,LPARAM lParam){_meteringmodestate=meteringmodestate;_lParam=lParam;}
public:
	virtual void update(Observable* from, CameraEvent *e)
	{
		std::string event = e->getEvent();

		//Update property
		if(event == "PropertyChanged")
		{
			EdsInt32 propertyID = *static_cast<EdsInt32 *>(e->getArg());

			if(propertyID == kEdsPropID_MeteringMode)
			{
				_MeteringMode=_model->getMeteringMode();
				_meteringmodestate(_MeteringModeDesc,_MeteringMode,_lParam);
			}
		}

		//Update of list that can set property
		if(event == "PropertyDescChanged")
		{
			EdsInt32 propertyID = *static_cast<EdsInt32 *>(e->getArg());

			if(propertyID == kEdsPropID_MeteringMode)
			{
				_MeteringModeDesc=_model->getMeteringModeDesc();
				_meteringmodestate(_MeteringModeDesc,_MeteringMode,_lParam);
			}	
		}
	}
};
