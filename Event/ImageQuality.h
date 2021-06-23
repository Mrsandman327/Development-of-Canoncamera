#pragma once
#include "Observer.h"
#include "ActionSource.h"
#include "CameraEvent.h"
#include "CameraController.h"


typedef void(CALLBACK *IMGQUALITYCALLBACK)(EdsPropertyDesc ImageQualityDesc,EdsUInt32 ImageQuality,LPARAM lParam);
class CImageQuality : public ActionSource , public Observer
{
public:
	CImageQuality()
	{
		_ImageQualityDesc.access=0;
		_ImageQualityDesc.form=0;
		_ImageQualityDesc.numElements=0;
		memset(&_ImageQualityDesc.propDesc, 0, sizeof(_ImageQualityDesc.propDesc));
		_ImageQuality=0;
	}
public:
	CameraModel*		_model;
	void SetCameraModel(CameraModel* model){_model=model;}
	IMGQUALITYCALLBACK			_imgqualitystate;
	LPARAM				_lParam;
	EdsPropertyDesc		_ImageQualityDesc;
	EdsUInt32			_ImageQuality;
	void SetCallback(IMGQUALITYCALLBACK imgqualitystate,LPARAM lParam){_imgqualitystate=imgqualitystate;_lParam=lParam;}
public:
	virtual void update(Observable* from, CameraEvent *e)
	{
		std::string event = e->getEvent();

		//Update property
		if(event == "PropertyChanged")
		{
			EdsInt32 propertyID = *static_cast<EdsInt32 *>(e->getArg());

			if(propertyID == kEdsPropID_ImageQuality)
			{
				_ImageQuality=_model->getImageQuality();
				_imgqualitystate(_ImageQualityDesc,_ImageQuality,_lParam);
			}
		}

		//Update of list that can set property
		if(event == "PropertyDescChanged")
		{
			EdsInt32 propertyID = *static_cast<EdsInt32 *>(e->getArg());

			if(propertyID == kEdsPropID_ImageQuality)
			{
				_ImageQualityDesc=_model->getImageQualityDesc();
				_imgqualitystate(_ImageQualityDesc,_ImageQuality,_lParam);
			}	
		}
	}
};
