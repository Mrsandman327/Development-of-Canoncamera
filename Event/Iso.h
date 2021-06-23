#pragma once
#include "Observer.h"
#include "ActionSource.h"
#include "CameraEvent.h"
#include "CameraController.h"


typedef void(CALLBACK *ISOCALLBACK)(EdsPropertyDesc IsoDesc,EdsUInt32 Iso,LPARAM lParam);
class CIso : public ActionSource , public Observer
{
public:
	CIso()
	{
		_IsoDesc.access=0;
		_IsoDesc.form=0;
		_IsoDesc.numElements=0;
		memset(&_IsoDesc.propDesc, 0, sizeof(_IsoDesc.propDesc));
		_Iso=0;
	}
public:
	CameraModel*		_model;
	void SetCameraModel(CameraModel* model){_model=model;}
	ISOCALLBACK			_isostate;
	LPARAM				_lParam;
	EdsPropertyDesc		_IsoDesc;
	EdsUInt32			_Iso;
	void SetCallback(ISOCALLBACK isostate,LPARAM lParam){_isostate=isostate;_lParam=lParam;}
public:
	virtual void update(Observable* from, CameraEvent *e)
	{
		std::string event = e->getEvent();

		//Update property
		if(event == "PropertyChanged")
		{
			EdsInt32 propertyID = *static_cast<EdsInt32 *>(e->getArg());

			if(propertyID == kEdsPropID_ISOSpeed)
			{
				_Iso=_model->getIso();
				_isostate(_IsoDesc,_Iso,_lParam);
			}
		}

		//Update of list that can set property
		if(event == "PropertyDescChanged")
		{
			EdsInt32 propertyID = *static_cast<EdsInt32 *>(e->getArg());

			if(propertyID == kEdsPropID_ISOSpeed)
			{
				_IsoDesc=_model->getIsoDesc();
				_isostate(_IsoDesc,_Iso,_lParam);
			}	
		}
	}
};