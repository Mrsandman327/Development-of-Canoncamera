#pragma once
#include "Observer.h"
#include "ActionSource.h"
#include "CameraEvent.h"
#include "CameraController.h"


typedef void(CALLBACK *EXPOSURECOMPCALLBACK)(EdsPropertyDesc ExposureCompDesc,EdsUInt32 ExposureComp,LPARAM lParam);
class CExposureComp : public ActionSource , public Observer
{
public:
	CExposureComp()
	{
		_ExposureCompDesc.access=0;
		_ExposureCompDesc.form=0;
		_ExposureCompDesc.numElements=0;
		memset(&_ExposureCompDesc.propDesc, 0, sizeof(_ExposureCompDesc.propDesc));
		_ExposureComp=0;
	}
public:
	CameraModel*		_model;
	void SetCameraModel(CameraModel* model){_model=model;}
	EXPOSURECOMPCALLBACK			_exposurecompstate;
	LPARAM				_lParam;
	EdsPropertyDesc		_ExposureCompDesc;
	EdsUInt32			_ExposureComp;
	void SetCallback(EXPOSURECOMPCALLBACK exposurecompstate,LPARAM lParam){_exposurecompstate=exposurecompstate;_lParam=lParam;}
public:
	virtual void update(Observable* from, CameraEvent *e)
	{
		std::string event = e->getEvent();

		//Update property
		if(event == "PropertyChanged")
		{
			EdsInt32 propertyID = *static_cast<EdsInt32 *>(e->getArg());

			if(propertyID == kEdsPropID_ExposureCompensation)
			{
				_ExposureComp=_model->getMeteringMode();
				_exposurecompstate(_ExposureCompDesc,_ExposureComp,_lParam);
			}
		}

		//Update of list that can set property
		if(event == "PropertyDescChanged")
		{
			EdsInt32 propertyID = *static_cast<EdsInt32 *>(e->getArg());

			if(propertyID == kEdsPropID_ExposureCompensation)
			{
				_ExposureCompDesc=_model->getMeteringModeDesc();
				_exposurecompstate(_ExposureCompDesc,_ExposureComp,_lParam);
			}	
		}
	}
};
