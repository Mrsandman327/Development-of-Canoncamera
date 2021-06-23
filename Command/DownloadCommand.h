/******************************************************************************
*                                                                             *
*   PROJECT : EOS Digital Software Development Kit EDSDK                      *
*      NAME : DownloadCommand.h	                                              *
*                                                                             *
*   Description: This is the Sample code to show the usage of EDSDK.          *
*                                                                             *
*                                                                             *
*******************************************************************************
*                                                                             *
*   Written and developed by Camera Design Dept.53                            *
*   Copyright Canon Inc. 2006-2008 All Rights Reserved                        *
*                                                                             *
*******************************************************************************/

#pragma once

#include "Command.h"
#include "CameraEvent.h"
#include "EDSDK.h"

class DownloadCommand : public Command
{
private:
	EdsDirectoryItemRef _directoryItem;

public:
	DownloadCommand(CameraModel *model, EdsDirectoryItemRef dirItem) 
			: _directoryItem(dirItem), Command(model){}


	virtual ~DownloadCommand()
	{
		//Release item
		if(_directoryItem != NULL)
		{
			EdsRelease( _directoryItem);
			_directoryItem = NULL;
		}
	}


	// Execute command 	
	virtual bool execute()
	{
		EdsError				err = EDS_ERR_OK;
		EdsStreamRef			stream = NULL;

		//Acquisition of the downloaded image information
		EdsDirectoryItemInfo	dirItemInfo;
		err = EdsGetDirectoryItemInfo( _directoryItem, &dirItemInfo);
	
		// Forwarding beginning notification	
		if(err == EDS_ERR_OK)
		{
			CameraEvent e("DownloadStart");
			_model->notifyObservers(&e);
		}
#if 0
		/*************************************指定图像存储路径*************************************/
		TCHAR sFilePath[MAX_PATH];
		GetModuleFileName(NULL,sFilePath,MAX_PATH);
		CString strFile = sFilePath;
		CString imagepath= strFile.Left(strFile.ReverseFind('\\')) + "/Image";//用来检查和创建文件夹
		DWORD dwAttr = GetFileAttributes(imagepath);
		if(dwAttr==0xFFFFFFFF)  //若文件夹不存在就创建
		{
			CreateDirectory(imagepath,NULL);
		}
		imagepath=imagepath+"\\";

		char Filepath[500];
		char FileName[500];
		for(int i=0;i<	imagepath.GetLength();i++)
		{
			Filepath[i]=imagepath.GetAt(i);
		}
		for(int i=0;i<12;i++)
		{
			FileName[i]=dirItemInfo.szFileName[i];
		}
		for (int i=0;i<	imagepath.GetLength()+12;i++)
		{
			if (i<	imagepath.GetLength())
			{
				dirItemInfo.szFileName[i]=Filepath[i];
			}
			else
			{
				dirItemInfo.szFileName[i]=FileName[i-imagepath.GetLength()];
			}

		}
		/*****************************************************************************************/
#endif
		//Make the file stream at the forwarding destination
		if(err == EDS_ERR_OK)
		{	
			err = EdsCreateFileStream(dirItemInfo.szFileName, kEdsFileCreateDisposition_CreateAlways, kEdsAccess_ReadWrite, &stream);
		}	

		//Set Progress
		if(err == EDS_ERR_OK)
		{
			err = EdsSetProgressCallback(stream, ProgressFunc, kEdsProgressOption_Periodically, this);
		}


		//Download image
		if(err == EDS_ERR_OK)
		{
			err = EdsDownload( _directoryItem, dirItemInfo.size, stream);
		}

		//Forwarding completion
		if(err == EDS_ERR_OK)
		{
			err = EdsDownloadComplete( _directoryItem);
		}

		//Release Item
		if(_directoryItem != NULL)
		{
			err = EdsRelease( _directoryItem);
			_directoryItem = NULL;
		}

		//Release stream
		if(stream != NULL)
		{
			err = EdsRelease(stream);
			stream = NULL;
		}		
		
		// Forwarding completion notification
		if( err == EDS_ERR_OK)
		{
			CameraEvent e("DownloadComplete", &err);
			_model->notifyObservers(&e);
		}

		//Notification of error
		if( err != EDS_ERR_OK)
		{
			CameraEvent e("error", &err);
			_model->notifyObservers(&e);
		}

		return true;
	}

private:
	static EdsError EDSCALLBACK ProgressFunc (
						EdsUInt32	inPercent,
						EdsVoid *	inContext,
						EdsBool	*	outCancel
						)
	{
		Command *command = (Command *)inContext;
		CameraEvent e("ProgressReport", &inPercent);
		command->getCameraModel()->notifyObservers(&e);
		return EDS_ERR_OK;
	}


};