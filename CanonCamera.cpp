#include "CanonCamera.h"

CanonCamera::CanonCamera(void)
{
	_model=NULL;
	_controller=NULL;
	camera = NULL;
	isSDKLoaded = false;
	isConnected =false;

	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x0c,"Bulb"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x10,"30″"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x13,"25″"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x14,"20″"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x15,"20″"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x18,"15″"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x1B,"13″"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x1C,"10″"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x1D,"10″"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x20,"8″"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x23,"6″"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x24,"6″"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x25,"5″"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x28,"4″"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x2B,"3″2"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x2C,"3″"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x2D,"2″5"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x30,"2″"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x33,"1″6"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x34,"1″5"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x35,"1″3"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x38,"1″"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x3B,"0″8"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x3C,"0″7"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x3D,"0″6"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x40,"0″5"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x43,"0″4"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x44,"0″3"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x45,"0″3"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x48,"4"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x4B,"5"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x4C,"6"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x4D,"6"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x50,"8"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x53,"10"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x54,"10"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x55,"13"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x58,"15"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x5B,"20"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x5C,"20"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x5D,"25"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x60,"30"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x63,"40"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x64,"45"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x65,"50"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x68,"60"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x6B,"80"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x6C,"90"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x6D,"100"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x70,"125"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x73,"160"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x74,"180"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x75,"200"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x78,"250"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x7B,"320"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x7C,"350"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x7D,"400"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x80,"500"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x83,"640"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x84,"750"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x85,"800"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x88,"1000"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x8B,"1250"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x8C,"1500"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x8D,"1600"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x90,"2000"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x93,"2500"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x94,"3000"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x95,"3200"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x98,"4000"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x9B,"5000"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x9C,"6000"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0x9D,"6400"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0xA0,"8000"));
	_TvDesc.insert( std::pair<EdsUInt32, const char *>(0xffffffff,"unknown"));

	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x00,"00"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x08,"1"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x0B,"1.1"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x0C,"1.2"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x0D,"1.2"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x10,"1.4"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x13,"1.6"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x14,"1.8"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x15,"1.8"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x18,"2"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x1B,"2.2"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x1C,"2.5"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x1D,"2.5"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x20,"2.8"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x23,"3.2"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x24,"3.5"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x25,"3.5"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x28,"4"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x2B,"4.5"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x2C,"4.5"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x2D,"5.0"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x30,"5.6"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x33,"6.3"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x34,"6.7"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x35,"7.1"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x38,"8"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x3B,"9"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x3C,"9.5"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x3D,"10"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x40,"11"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x43,"13"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x44,"13"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x45,"14"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x48,"16"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x4B,"18"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x4C,"19"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x4D,"20"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x50,"22"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x53,"25"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x54,"27"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x55,"29"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x58,"32"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x5B,"36"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x5C,"38"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x5D,"40"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x60,"45"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x63,"51"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x64,"54"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x65,"57"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x68,"64"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x6B,"72"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x6C,"76"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x6D,"80"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0x70,"91"));
	_AvDesc.insert( std::pair<EdsUInt32, const char *>(0xffffffff,"unknown"));

	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0x00,"Auto"));
	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0x28,"6"));
	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0x30,"12"));
	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0x38,"25"));
	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0x40,"50"));
	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0x48,"100"));
	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0x4b,"125"));
	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0x4d,"160"));
	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0x50,"200"));
	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0x53,"250"));
	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0x55,"320"));
	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0x58,"400"));
	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0x5b,"500"));
	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0x5d,"640"));
	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0x60,"800"));
	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0x63,"1000"));
	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0x65,"1250"));
	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0x68,"1600"));
	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0x6b,"2000"));
	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0x6d,"2500"));
	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0x70,"3200"));
	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0x73,"4000"));
	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0x75,"5000"));
	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0x78,"6400"));
	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0x7b,"8000"));
	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0x7d,"10000"));
	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0x80,"12800"));
	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0x83,"16000"));
	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0x85,"20000"));
	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0x88,"25600"));
	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0x8b,"32000"));
	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0x8d,"40000"));
	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0x90,"51200"));
	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0x98,"102400"));
	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0xa0,"204800"));
	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0xa8,"409600"));
	_IsoDesc.insert( std::pair<EdsUInt32, const char *>(0xffffffff,"unknown"));

	_AEDesc.insert( std::pair<EdsUInt32, const char *>(0,"P"));
	_AEDesc.insert( std::pair<EdsUInt32, const char *>(1,"Tv"));
	_AEDesc.insert( std::pair<EdsUInt32, const char *>(2,"Av"));
	_AEDesc.insert( std::pair<EdsUInt32, const char *>(3,"M"));
	_AEDesc.insert( std::pair<EdsUInt32, const char *>(4,"Bulb"));
	_AEDesc.insert( std::pair<EdsUInt32, const char *>(5,"A-DEP"));
	_AEDesc.insert( std::pair<EdsUInt32, const char *>(6,"DEP"));
	_AEDesc.insert( std::pair<EdsUInt32, const char *>(7,"C1"));
	_AEDesc.insert( std::pair<EdsUInt32, const char *>(16,"C2"));
	_AEDesc.insert( std::pair<EdsUInt32, const char *>(17,"C3"));
	_AEDesc.insert( std::pair<EdsUInt32, const char *>(8,"Lock"));
	_AEDesc.insert( std::pair<EdsUInt32, const char *>(9,"GreenMode"));
	_AEDesc.insert( std::pair<EdsUInt32, const char *>(10,"Night Portrait"));
	_AEDesc.insert( std::pair<EdsUInt32, const char *>(11,"Sports"));
	_AEDesc.insert( std::pair<EdsUInt32, const char *>(13,"LandScape"));
	_AEDesc.insert( std::pair<EdsUInt32, const char *>(14,"Close Up"));
	_AEDesc.insert( std::pair<EdsUInt32, const char *>(15,"No Strobo"));
	_AEDesc.insert( std::pair<EdsUInt32, const char *>(12,"Portrait"));
	_AEDesc.insert( std::pair<EdsUInt32, const char *>(19,"Creative Auto"));
	_AEDesc.insert( std::pair<EdsUInt32, const char *>(20,"Movies"));
	_AEDesc.insert( std::pair<EdsUInt32, const char *>(25,"SCN"));
	_AEDesc.insert( std::pair<EdsUInt32, const char *>(22,"SceneIntelligentAuto"));
	_AEDesc.insert( std::pair<EdsUInt32, const char *>(0xffffffff,"unknown"));

	_MeteringModeDesc.insert( std::pair<EdsUInt32, const char *>(1,"Spot"));
	_MeteringModeDesc.insert( std::pair<EdsUInt32, const char *>(3,"Evaluative"));
	_MeteringModeDesc.insert( std::pair<EdsUInt32, const char *>(4,"Partial"));
	_MeteringModeDesc.insert( std::pair<EdsUInt32, const char *>(5,"Center-Weighted Average"));
	_MeteringModeDesc.insert( std::pair<EdsUInt32, const char *>(0xffffffff,"unknown"));

	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0x28,"+5"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0x25,"+4 2/3"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0x24,"+4 1/2"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0x23,"+4 1/3"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0x20,"+4"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0x1D,"+3 2/3"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0x1C,"+3 1/2"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0x1B,"+3 1/3"));

	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0x18,"+3"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0x15,"+2 2/3"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0x14,"+2 1/2"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0x13,"+2 1/3"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0x10,"+2"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0x0d,"+1 2/3"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0x0c,"+1 1/2"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0x0b,"+1 1/3"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0x08,"+1"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0x05,"+2/3"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0x04,"+1/2"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0x03,"+1/3"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0x00,"0"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0xfd,"-1/3"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0xfc,"-1/2"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0xfb,"-2/3"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0xf8,"-1"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0xf5,"-1 1/3"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0xf4,"-1 1/2"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0xf3,"-1 2/3"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0xf0,"-2"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0xed,"-2 1/3"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0xec,"-2 1/2"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0xeb,"-2 2/3"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0xe8,"-3"));

	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0xE5,"-3 1/3"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0xE4,"-3 1/2"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0xE3,"-3 2/3"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0xE0,"-4"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0xDD,"-4 1/3"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0xDC,"-4 1/2"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0xDB,"-4 2/3"));
	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0xD8,"-5"));

	_ExposureCompDesc.insert( std::pair<EdsUInt32, const char *>(0xffffffff,"unknown"));

	// PTP Camera
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_LR,     "RAW"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_LRLJF,  "RAW + Large Fine Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_LRMJF,  "RAW + Middle Fine Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_LRSJF,  "RAW + Small Fine Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_LRLJN,  "RAW + Large Normal Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_LRMJN,  "RAW + Middle Normal Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_LRSJN,  "RAW + Small Normal Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_LRS1JF, "RAW + Small1 Fine Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_LRS1JN, "RAW + Small1 Normal Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_LRS2JF, "RAW + Small2 Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_LRS3JF, "RAW + Small3 Jpeg"));

	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_LRLJ,   "RAW + Large Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_LRM1J,  "RAW + Middle1 Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_LRM2J,  "RAW + Middle2 Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_LRSJ,   "RAW + Small Jpeg"));

	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_MR,     "Middle Raw(Small RAW1)"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_MRLJF,  "Middle Raw(Small RAW1) + Large Fine Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_MRMJF,  "Middle Raw(Small RAW1) + Middle Fine Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_MRSJF,  "Middle Raw(Small RAW1) + Small Fine Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_MRLJN,  "Middle Raw(Small RAW1) + Large Normal Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_MRMJN,  "Middle Raw(Small RAW1) + Middle Normal Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_MRSJN,  "Middle Raw(Small RAW1) + Small Normal Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_MRS1JF, "Middle RAW + Small1 Fine Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_MRS1JN, "Middle RAW + Small1 Normal Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_MRS2JF, "Middle RAW + Small2 Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_MRS3JF, "Middle RAW + Small3 Jpeg"));

	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_MRLJ,   "Middle Raw + Large Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_MRM1J,  "Middle Raw + Middle1 Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_MRM2J,  "Middle Raw + Middle2 Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_MRSJ,   "Middle Raw + Small Jpeg"));

	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_SR,     "Small RAW(Small RAW2)"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_SRLJF,  "Small RAW(Small RAW2) + Large Fine Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_SRMJF,  "Small RAW(Small RAW2) + Middle Fine Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_SRSJF,  "Small RAW(Small RAW2) + Small Fine Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_SRLJN,  "Small RAW(Small RAW2) + Large Normal Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_SRMJN,  "Small RAW(Small RAW2) + Middle Normal Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_SRSJN,  "Small RAW(Small RAW2) + Small Normal Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_SRS1JF, "Small RAW + Small1 Fine Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_SRS1JN, "Small RAW + Small1 Normal Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_SRS2JF, "Small RAW + Small2 Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_SRS3JF, "Small RAW + Small3 Jpeg"));

	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_SRLJ,   "Small RAW + Large Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_SRM1J,  "Small RAW + Middle1 Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_SRM2J,  "Small RAW + Middle2 Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_SRSJ,   "Small RAW + Small Jpeg"));

	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_LJF,    "Large Fine Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_LJN,    "Large Normal Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_MJF,    "Middle Fine Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_MJN,    "Middle Normal Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_SJF,    "Small Fine Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_SJN,    "Small Normal Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_S1JF,   "Small1 Fine Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_S1JN,   "Small1 Normal Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_S2JF,   "Small2 Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_S3JF,   "Small3 Jpeg"));

	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_LJ,     "Large Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_M1J,    "Middle1 Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_M2J,    "Middle2 Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(EdsImageQuality_SJ,     "Small Jpeg"));

	// Legacy Camera
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(kEdsImageQualityForLegacy_LR,    "RAW"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(kEdsImageQualityForLegacy_LRLJF, "RAW + Large Fine Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(kEdsImageQualityForLegacy_LRMJF, "RAW + Middle Fine Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(kEdsImageQualityForLegacy_LRSJF, "RAW + Small Fine Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(kEdsImageQualityForLegacy_LRLJN, "RAW + Large Normal Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(kEdsImageQualityForLegacy_LRMJN, "RAW + Middle Normal Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(kEdsImageQualityForLegacy_LRSJN, "RAW + Small Normal Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(kEdsImageQualityForLegacy_LJF,   "Large Fine Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(kEdsImageQualityForLegacy_MJF,   "Middle Fine Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(kEdsImageQualityForLegacy_SJF,   "Small Fine Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(kEdsImageQualityForLegacy_LJN,   "Large Normal Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(kEdsImageQualityForLegacy_MJN,   "Middle Normal Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(kEdsImageQualityForLegacy_SJN,   "Small Normal Jpeg"));

	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(kEdsImageQualityForLegacy_LR2,   "RAW"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(kEdsImageQualityForLegacy_LR2LJ, "RAW + Large Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(kEdsImageQualityForLegacy_LR2M1J,"RAW + Middle1 Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(kEdsImageQualityForLegacy_LR2M2J,"RAW + Middle2 Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(kEdsImageQualityForLegacy_LR2SJ, "RAW + Small Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(kEdsImageQualityForLegacy_LJ,    "Large Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(kEdsImageQualityForLegacy_M1J,   "Middle1 Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(kEdsImageQualityForLegacy_M2J,   "Middle2 Jpeg"));
	_ImageQualityDesc.insert( std::pair<EdsUInt32, const char *>(kEdsImageQualityForLegacy_SJ,    "Small Jpeg"));

	_EvfAFModeDesc.insert( std::pair<EdsUInt32, const char *>(0x00,"Quick mode"));
	_EvfAFModeDesc.insert( std::pair<EdsUInt32, const char *>(0x01,"Live mode"));
	_EvfAFModeDesc.insert( std::pair<EdsUInt32, const char *>(0x02,"Live face detection mode"));
	_EvfAFModeDesc.insert( std::pair<EdsUInt32, const char *>(0x03,"Live FlexiZone-Multi mode"));
	_EvfAFModeDesc.insert( std::pair<EdsUInt32, const char *>(0xffffffff,"unknown"));
}


CanonCamera::~CanonCamera(void)
{
	if(_model != NULL)
	{
		delete _model;
		_model = NULL;
	}


	if(_controller != NULL)
	{
		delete _controller;
		_controller = NULL;
	}
}


bool CanonCamera::Canon_ListCamerasAndConnect(void)
{

	EdsError	 err = EDS_ERR_OK;
	EdsCameraListRef cameraList = NULL;
	EdsUInt32	 count = 0;	


	// Initialization of SDK
	err = EdsInitializeSDK();//初始化库

	if(err == EDS_ERR_OK)
	{
		isSDKLoaded = true;
	}
	else
	{
		isConnected=false;
		goto LLL;
	}

	//Acquisition of camera list
	if(err == EDS_ERR_OK)
	{
		err = EdsGetCameraList(&cameraList);//获取相机列表对象。
	}
	else
	{
		isConnected=false;
		goto LLL;
	}

	//Acquisition of number of Cameras
	if(err == EDS_ERR_OK)
	{
		err = EdsGetChildCount(cameraList, &count);
		if(count == 0)
		{
			err = EDS_ERR_DEVICE_NOT_FOUND;
		}
	}
	else
	{
		isConnected=false;
		goto LLL;
	}


	//Acquisition of camera at the head of the list
	if(err == EDS_ERR_OK)
	{	
		err = EdsGetChildAtIndex(cameraList , 0 , &camera);	
	}
	else
	{
		isConnected=false;
		goto LLL;
	}

	//Acquisition of camera information
	EdsDeviceInfo deviceInfo;
	if(err == EDS_ERR_OK)
	{	
		err = EdsGetDeviceInfo(camera , &deviceInfo);	
		if(err == EDS_ERR_OK && camera == NULL)
		{
			err = EDS_ERR_DEVICE_NOT_FOUND;
		}
	}
	else
	{
		isConnected=false;
		goto LLL;
	}


	//Release camera list
	if(cameraList != NULL)
	{
		EdsRelease(cameraList);
	}

	//Create Camera model
	if(err == EDS_ERR_OK )
	{
		_model = cameraModelFactory(camera, deviceInfo);
		if(_model == NULL)
		{
			err = EDS_ERR_DEVICE_NOT_FOUND;
		}
	}
	else
	{
		isConnected=false;
		goto LLL;
	}

	if(err != EDS_ERR_OK)
	{
		//Cannot detect camera
		isConnected=false;
		goto LLL;
	}
		

	if(err == EDS_ERR_OK )
	{
		//Create CameraController
		_controller = new CameraController();

		_controller->setCameraModel(_model);
		// Send Model Event to view	

		//Set Property Event Handler
		if(err == EDS_ERR_OK)
		{
			err = EdsSetPropertyEventHandler( camera, kEdsPropertyEvent_All, CameraEventListener::handlePropertyEvent , (EdsVoid *)_controller);
		}

		//Set Object Event Handler
		if(err == EDS_ERR_OK)
		{
			err = EdsSetObjectEventHandler( camera, kEdsObjectEvent_All, CameraEventListener::handleObjectEvent , (EdsVoid *)_controller);
		}

		//Set State Event Handler
		if(err == EDS_ERR_OK)
		{
			err = EdsSetCameraStateEventHandler( camera, kEdsStateEvent_All, CameraEventListener::handleStateEvent , (EdsVoid *)_controller);
		}
	}
	else
	{
		isConnected=false;
		goto LLL;
	}
	

	// 打开相机会话(貌似会影响图像下载)
	//if(err == EDS_ERR_OK)
	//{
	//	err = EdsOpenSession(camera);
	//}
	//else
	//	return false;

	Canon_setupObserver(_model);
	Canon_setupListener(_controller);

	isConnected=true;
LLL:
	return isConnected;
}

bool  CanonCamera::Canon_DisConnect(void)
{
	//关闭会话
	//EdsCloseSession(camera);


	//Release Camera
	if( camera != NULL )
	{
		EdsRelease(camera);
		camera = NULL;
	}

	//Termination of SDK
	if(isSDKLoaded)
	{
		EdsTerminateSDK();//终止使用库。调用此函数将释放由库分配的所有资源。
		isSDKLoaded = false;
	}
	isConnected=false;

	return true;
}

void CanonCamera::Canon_setupListener(ActionListener* listener)
{
	_TakePicture.addActionListener(listener);
	_TakePicture.setActionCommand("TakePicture");
	_StartEVF.setActionCommand("startEVF");
	_StartEVF.addActionListener(listener);
	_EndEVF.setActionCommand("endEVF");
	_EndEVF.addActionListener(listener);
	_FocusNear3.setActionCommand("focus_Near3");
	_FocusNear3.addActionListener(listener);
	_FocusNear2.setActionCommand("focus_Near2");
	_FocusNear2.addActionListener(listener);
	_FocusNear1.setActionCommand("focus_Near1");
	_FocusNear1.addActionListener(listener);
	_FocusFar3.setActionCommand("focus_Far3");
	_FocusFar3.addActionListener(listener);
	_FocusFar2.setActionCommand("focus_Far2");
	_FocusFar2.addActionListener(listener);
	_FocusFar1.setActionCommand("focus_Far1");
	_FocusFar1.addActionListener(listener);
	_FocusUp.setActionCommand("focus_Up");
	_FocusUp.addActionListener(listener);
	_FocusLeft.setActionCommand("focus_Left");
	_FocusLeft.addActionListener(listener);
	_FocusRight.setActionCommand("focus_Right");
	_FocusRight.addActionListener(listener);
	_FocusDown.setActionCommand("focus_Down");
	_FocusDown.addActionListener(listener);	
	_ZoomFit.addActionListener(listener);
	_ZoomFit.setActionCommand("zoom_Fit");
	_ZoomZoom.addActionListener(listener);
	_ZoomZoom.setActionCommand("zoom_Zoom");
	_DownloadEvf.setActionCommand("downloadEVF");
	_DownloadEvf.addActionListener(listener);
	_Download.setActionCommand("download");
	_Download.addActionListener(listener);
	_Tv.setActionCommand("set_Tv");
	_Tv.addActionListener(listener);
	_Av.setActionCommand("set_Av");
	_Av.addActionListener(listener);
	_Iso.setActionCommand("set_ISOSpeed");
	_Iso.addActionListener(listener);
	_AEMode.setActionCommand("set_AEMode");
	_AEMode.addActionListener(listener);
	_MeteringMode.setActionCommand("set_MeteringMode");
	_MeteringMode.addActionListener(listener);
	_ImageQuality.setActionCommand("set_ImageQuality");
	_ImageQuality.addActionListener(listener);
	_EvfAFMode.setActionCommand("set_EvfAFMode");
	_EvfAFMode.addActionListener(listener);	
	_controller->run();
}

void CanonCamera::Canon_setupObserver(Observable* ob)
{
	ob->addObserver(static_cast<Observer*>(&_Tv));
	ob->addObserver(static_cast<Observer*>(&_Av));
	ob->addObserver(static_cast<Observer*>(&_Iso));
	ob->addObserver(static_cast<Observer*>(&_DownloadEvf));
	ob->addObserver(static_cast<Observer*>(&_Download));

	//不需要监视暂时注释
	//ob->addObserver(static_cast<Observer*>(&_MeteringMode));
	//ob->addObserver(static_cast<Observer*>(&_AEMode));
	//ob->addObserver(static_cast<Observer*>(&_ExposureComp));
	//ob->addObserver(static_cast<Observer*>(&_ImageQuality));
	//ob->addObserver(static_cast<Observer*>(&_EvfAFMode));
}



bool CanonCamera::Canon_TakeAnImage(void)
{
	_TakePicture.fireEvent();
	return true;
}

bool CanonCamera::Canon_StartEVF(void)
{
	_StartEVF.fireEvent();
	_DownloadEvf.fireEvent();
	return true;
}

bool CanonCamera::Canon_EndEVF(void)
{
	_EndEVF.fireEvent();
	return true;
}

bool CanonCamera::Canon_ZoomFit(void)
{
	_ZoomFit.fireEvent();
	return true;
}

bool CanonCamera::Canon_ZoomZoom(void)
{
	_ZoomZoom.fireEvent();
	return true;
}

bool CanonCamera::Canon_Focus_Near(int value)
{
	switch(value)
	{
	case 1:_FocusNear1.fireEvent();
	case 2:_FocusNear2.fireEvent();
	case 3:_FocusNear3.fireEvent();
	}
	return true;
}

bool CanonCamera::Canon_Focus_Far(int value)
{
	switch(value)
	{
	case 1:_FocusFar1.fireEvent();
	case 2:_FocusFar2.fireEvent();
	case 3:_FocusFar3.fireEvent();
	}
	return true;
}

bool CanonCamera::Canon_Focus_Direction(int value)
{
	switch(value)
	{
	case 1:_FocusUp.fireEvent();
	case 2:_FocusDown.fireEvent();
	case 3:_FocusLeft.fireEvent();
	case 4:_FocusRight.fireEvent();
	}
	return true;
}

bool CanonCamera::Canon_SetTv(int sel)
{
	_Tv.fireEvent(&sel);
	return true;
}

bool CanonCamera::Canon_SetAv(int sel)
{
	_Av.fireEvent(&sel);
	return true;
}

bool CanonCamera::Canon_SetIso(int sel)
{
	_Iso.fireEvent(&sel);
	return true;
}

bool CanonCamera::Canon_SetAEMode(int sel)
{
	_AEMode.fireEvent(&sel);
	return true;
}

bool CanonCamera::Canon_SetAMeteringMode(int sel)
{
	_MeteringMode.fireEvent(&sel);
	return true;
}

bool CanonCamera::Canon_SetExposureComp(int sel)
{
	_ExposureComp.fireEvent(&sel);
	return true;
}

bool CanonCamera::Canon_SetImageQuality(int sel)
{
	_ImageQuality.fireEvent(&sel);
	return true;
}

bool CanonCamera::Canon_SetEvfAFMode(int sel)
{
	_EvfAFMode.fireEvent(&sel);
	return true;
}

bool CanonCamera::Canon_SetDownloadEvfCallback(IMAGEDATACALLBACK imgdata,LPARAM lParam)
{
	_DownloadEvf.SetCallback(imgdata,lParam);
	return true;
}

void CanonCamera::Canon_SetDownLoadCallback(DOWNLOADPROGRESSCALLBACK downloadstate,LPARAM lParam)
{
	_Download.SetCallback(downloadstate,lParam);
}

bool CanonCamera::Canon_SetTvCallback(TVCALLBACK tvstate,LPARAM lParam)
{
	_Tv.SetCameraModel(_model);
	_Tv.SetCallback(tvstate,lParam);
	return true;
}

bool CanonCamera::Canon_SetAvCallback(AVCALLBACK avstate,LPARAM lParam)
{
	_Av.SetCameraModel(_model);
	_Av.SetCallback(avstate,lParam);
	return true;
}

bool CanonCamera::Canon_SetIsoCallback(ISOCALLBACK isostate,LPARAM lParam)
{
	_Iso.SetCameraModel(_model);
	_Iso.SetCallback(isostate,lParam);
	return true;
}

bool CanonCamera::Canon_SetAEModeCallback(AECALLBACK aestate,LPARAM lParam)
{
	_AEMode.SetCameraModel(_model);
	_AEMode.SetCallback(aestate,lParam);
	return true;
}

bool CanonCamera::Canon_SetMeteringModeCallback(METERINGMODECALLBACK meteringmodestate,LPARAM lParam)
{
	_MeteringMode.SetCameraModel(_model);
	_MeteringMode.SetCallback(meteringmodestate,lParam);
	return true;
}

bool CanonCamera::Canon_SetExposureCompCallback(EXPOSURECOMPCALLBACK exposurecompstate,LPARAM lParam)
{
	_ExposureComp.SetCameraModel(_model);
	_ExposureComp.SetCallback(exposurecompstate,lParam);
	return true;
}

bool CanonCamera::Canon_SetImageQualityCallback(IMGQUALITYCALLBACK imgqualitystate,LPARAM lParam)
{
	_ImageQuality.SetCameraModel(_model);
	_ImageQuality.SetCallback(imgqualitystate,lParam);
	return true;
}

bool CanonCamera::Canon_SetEvfAFModeCallback(EVFAFMODECALLBACK evfafmodestate,LPARAM lParam)
{
	_EvfAFMode.SetCameraModel(_model);
	_EvfAFMode.SetCallback(evfafmodestate,lParam);
	return true;
}