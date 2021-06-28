#include "CanonCamera.h"
 CanonCamera canoncam;

class DemoDlg
{
private:
    /* data */
public:
    DemoDlg(/* args */);
    ~DemoDlg();
public:
	static void CALLBACK ShowEvf(CComPtr<IStream> stream, LPARAM lParam);
	static void CALLBACK ShowDownloadState(EdsUInt32 percent,int state,LPARAM lParam);
	static void CALLBACK ShowTvState(EdsPropertyDesc TvDesc,EdsUInt32 Tv,LPARAM lParam);
	static void CALLBACK ShowAvState(EdsPropertyDesc AvDesc,EdsUInt32 Av,LPARAM lParam);
	static void CALLBACK ShowIsoState(EdsPropertyDesc IsoDesc,EdsUInt32 Iso,LPARAM lParam);
public:
    void Connect();
    void Disconnect();
    void Shot();
    void Evf(bool evf);
};

DemoDlg::DemoDlg(/* args */)
{
}

DemoDlg::~DemoDlg()
{
}



void DemoDlg::ShowTvState(EdsPropertyDesc TvDesc,EdsUInt32 Tv,LPARAM lParam)
{
    DemoDlg *pDlg =(DemoDlg *)lParam;
	for(int i = 0; i < TvDesc.numElements; i++)
	{
		std::map<EdsUInt32, const char *>::iterator itr = canoncam._TvDesc.find((EdsUInt32)TvDesc.propDesc[i]);
		if (itr != canoncam._TvDesc.end())
		{
			//get propDesc
		}
	}
	std::map<EdsUInt32, const char*>::iterator itr = canoncam._TvDesc.find(Tv);
	if (itr != canoncam._TvDesc.end())
	{		
		//get prop
	}
}

void DemoDlg::ShowAvState(EdsPropertyDesc AvDesc,EdsUInt32 Av,LPARAM lParam)
{
     DemoDlg *pDlg =(DemoDlg *)lParam;
	for(int i = 0; i < AvDesc.numElements; i++)
	{
		std::map<EdsUInt32, const char *>::iterator itr = canoncam._AvDesc.find((EdsUInt32)AvDesc.propDesc[i]);

		if (itr != canoncam._AvDesc.end())
		{
			//get propDesc
		}
	}
	std::map<EdsUInt32, const char*>::iterator itr = canoncam._AvDesc.find(Av);
	if (itr != canoncam._AvDesc.end())
	{		
		//get prop
	}
}

void DemoDlg::ShowIsoState(EdsPropertyDesc IsoDesc,EdsUInt32 Iso,LPARAM lParam)
{
     DemoDlg *pDlg =(DemoDlg *)lParam;
	for(int i = 0; i < IsoDesc.numElements; i++)
	{
		std::map<EdsUInt32, const char *>::iterator itr = canoncam._IsoDesc.find((EdsUInt32)IsoDesc.propDesc[i]);

		if (itr != canoncam._IsoDesc.end())
		{
            //get propDesc
		}
	}
	std::map<EdsUInt32, const char*>::iterator itr = canoncam._IsoDesc.find(Iso);
	if (itr != canoncam._IsoDesc.end())
	{		
		//get propDesc
	}
}


void DemoDlg::ShowEvf(CComPtr<IStream> stream, LPARAM lParam)
{
     DemoDlg *pDlg =(DemoDlg *)lParam;
    //get stream
}


void DemoDlg::ShowDownloadState(EdsUInt32 percent,int state,LPARAM lParam)
{
     DemoDlg *pDlg =(DemoDlg *)lParam;
	if (state==SATRTDOWN)
	{	
	} 
	else if (state==ENDDOWN)
	{
	}
	else if (state==DOWNING)
	{
	}
	else
	{
	}
}

void DemoDlg::Connect()
{
    if (!canoncam.Canon_GetCameraConnectedState())
	{
		if(canoncam.Canon_ListCamerasAndConnect())
		{
			canoncam.Canon_SetTvCallback(ShowTvState,LPARAM(this));
			canoncam.Canon_SetAvCallback(ShowAvState,LPARAM(this));
			canoncam.Canon_SetIsoCallback(ShowIsoState,LPARAM(this));
			canoncam.Canon_SetDownloadEvfCallback(ShowEvf,LPARAM(this));
			canoncam.Canon_SetDownLoadCallback(ShowDownloadState,LPARAM(this));
		}
	}
}

void DemoDlg::Disconnect()
{
    if (canoncam.Canon_GetCameraConnectedState())
        canoncam.Canon_DisConnect();
}


void DemoDlg::Shot()
{
	if(canoncam.Canon_GetCameraConnectedState())	
	    canoncam.Canon_TakeAnImage();
}


void DemoDlg::Evf(bool evf)
{
	if(canoncam.Canon_GetCameraConnectedState())
    {
        if (evf)
        {
            canoncam.Canon_StartEVF();
        }
        else
        {
            canoncam.Canon_EndEVF();
        }
    }
}