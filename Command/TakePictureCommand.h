/******************************************************************************
*                                                                             *
*   PROJECT : EOS Digital Software Development Kit EDSDK                      *
*      NAME : TakePictureCommand.h	                                          *
*                                                                             *
*   Description: This is the Sample code to show the usage of EDSDK.          *
*                                                                             *
*                                                                             *
*******************************************************************************
*                                                                             *
*   Written and developed by Camera Design Dept.53                            *
*   Copyright Canon Inc. 2006 All Rights Reserved                             *
*                                                                             *
*******************************************************************************/

#pragma once

#include "Command.h"
#include "CameraEvent.h"
#include "EDSDK.h"

class TakePictureCommand : public Command
{

public:
	TakePictureCommand(CameraModel *model) : Command(model){}


	// Execute command	
	virtual bool execute()
	{
		EdsError err = EDS_ERR_OK;
		bool	 locked = false;
		
		//Taking a picture
		err = EdsSendCommand(_model->getCameraObject(), kEdsCameraCommand_PressShutterButton, kEdsCameraCommand_ShutterButton_Completely);
		      EdsSendCommand(_model->getCameraObject(), kEdsCameraCommand_PressShutterButton, kEdsCameraCommand_ShutterButton_OFF);
		

		//Notification of error
		if(err != EDS_ERR_OK)
		{
			// It retries it at device busy
			if(err == EDS_ERR_DEVICE_BUSY)
			{
				CameraEvent e("DeviceBusy");
				_model->notifyObservers(&e);
				return true;
			}
			
			CameraEvent e("error", &err); 
			_model->notifyObservers(&e);
		}

		return true;
	}


};