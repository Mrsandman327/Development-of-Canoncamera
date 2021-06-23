#pragma once
#include "Observer.h"
#include "ActionSource.h"
#include "CameraEvent.h"
#include "CameraController.h"


typedef void(CALLBACK *AVCALLBACK)(EdsPropertyDesc AvDesc,EdsUInt32 Av,LPARAM lParam);
class CAv : public ActionSource , public Observer
{
public:
	CAv()
	{
		_AvDesc.access=0;
		_AvDesc.form=0;
		_AvDesc.numElements=0;
		memset(&_AvDesc.propDesc, 0, sizeof(_AvDesc.propDesc));
		_Av=0;
	}
public:
	CameraModel*		_model;
	void SetCameraModel(CameraModel* model){_model=model;}
	AVCALLBACK			_avstate;
	LPARAM				_lParam;
	EdsPropertyDesc		_AvDesc;
	EdsUInt32			_Av;
	void SetCallback(AVCALLBACK avstate,LPARAM lParam){_avstate=avstate;_lParam=lParam;}
public:
	virtual void update(Observable* from, CameraEvent *e)
	{
		std::string event = e->getEvent();

		//Update property
		if(event == "PropertyChanged")
		{
			EdsInt32 propertyID = *static_cast<EdsInt32 *>(e->getArg());

			if(propertyID == kEdsPropID_Av)
			{
				_Av=_model->getAv();
				_avstate(_AvDesc,_Av,_lParam);
			}
		}

		//Update of list that can set property
		if(event == "PropertyDescChanged")
		{
			EdsInt32 propertyID = *static_cast<EdsInt32 *>(e->getArg());

			if(propertyID == kEdsPropID_Av)
			{
				_AvDesc=_model->getAvDesc();
				_avstate(_AvDesc,_Av,_lParam);
			}	
		}
	}
};
