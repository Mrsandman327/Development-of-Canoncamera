#pragma once
#include "Observer.h"
#include "ActionSource.h"
#include "CameraEvent.h"
#include "CameraController.h"


typedef void(CALLBACK *TVCALLBACK)(EdsPropertyDesc TvDesc,EdsUInt32 Tv,LPARAM lParam);
class CTv : public ActionSource , public Observer
{
public:
	CTv()
	{
		_TvDesc.access=0;
		_TvDesc.form=0;
		_TvDesc.numElements=0;
		memset(&_TvDesc.propDesc, 0, sizeof(_TvDesc.propDesc));
		_Tv=0;
	}
public:
	CameraModel*		_model;
	void SetCameraModel(CameraModel* model){_model=model;}
	TVCALLBACK _tvstate;
	LPARAM _lParam;
	EdsPropertyDesc _TvDesc;
	EdsUInt32 _Tv;
	void SetCallback(TVCALLBACK tvstate,LPARAM lParam){_tvstate=tvstate;_lParam=lParam;}
public:
	virtual void update(Observable* from, CameraEvent *e)
	{
		std::string event = e->getEvent();

		//Update property
		if(event == "PropertyChanged")
		{
			EdsInt32 propertyID = *static_cast<EdsInt32 *>(e->getArg());

			if(propertyID == kEdsPropID_Tv)
			{
				_Tv=_model->getTv();
				_tvstate(_TvDesc,_Tv,_lParam);
			}
		}

		//Update of list that can set property
		if(event == "PropertyDescChanged")
		{
			EdsInt32 propertyID = *static_cast<EdsInt32 *>(e->getArg());

			if(propertyID == kEdsPropID_Tv)
			{
				_TvDesc=_model->getTvDesc();
				_tvstate(_TvDesc,_Tv,_lParam);
			}	
		}
	}
};