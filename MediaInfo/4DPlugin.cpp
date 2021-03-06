/* --------------------------------------------------------------------------------
 #
 #	4DPlugin.c
 #	source generated by 4D Plugin Wizard
 #	Project : MediaInfo
 #	author : miyako
 #	11/25/13
 #
 # --------------------------------------------------------------------------------*/

#include "4DPlugin.h"

void PluginMain(int32_t selector, PA_PluginParameters params)
{
	try
	{
		int32_t pProcNum = selector;
		sLONG_PTR *pResult = (sLONG_PTR *)params->fResult;
		PackagePtr pParams = (PackagePtr)params->fParameters;

		CommandDispatcher(pProcNum, pResult, pParams); 
	}
	catch(...)
	{

	}
}

void CommandDispatcher (int32_t pProcNum, sLONG_PTR *pResult, PackagePtr pParams)
{
	switch(pProcNum)
	{
// --- MediaInfo

		case 1 :
			MediaInfo(pResult, pParams);
			break;

	}
}

// ----------------------------------- MediaInfo ----------------------------------

void MediaInfo(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_BLOB Param1;
	C_LONGINT Param2;
	C_TEXT returnValue;

	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);

	MediaInfoLib::MediaInfo MI;	
	MI.Open_Buffer_Init();
	
	size_t			size;
	const uint8_t	*ptr;
	
	ptr		= Param1.getBytesPtr();
	size	= STREAM_BUFFER_SIZE;
	
	do
	{
		PA_YieldAbsolute();
		if (!(MI.Open_Buffer_Continue(ptr, size)))
		{
			ptr = Param1.getBytesPtrForSize((uint32_t *)&size);
		}else{
			size = 0;
		}
	}
	
	while (size > 0);
	
	MI.Open_Buffer_Finalize();

	switch (Param2.getIntValue()) {
		case MediaInfo_Inform_XML:
			MediaInfoLib::Config.Inform_Set(__T("XML"));
			break;
		case MediaInfo_Inform_HTML:
			MediaInfoLib::Config.Inform_Set(__T("HTML"));
			break;
		case MediaInfo_Inform_CSV:
			MediaInfoLib::Config.Inform_Set(__T("CSV"));
			break;			
		default:
			MediaInfoLib::Config.Inform_Set(__T(""));
			break;
	}
	
	std::wstring inform = std::wstring((const wchar_t *)MI.Inform().c_str());
	
uint32_t len;	

#if VERSIONMAC	
PA_4DCharSet eVTC_WCHAR_T = eVTC_UTF_32;
#else
PA_4DCharSet eVTC_WCHAR_T = eVTC_UTF_16;
#endif

	len = (uint32_t)(inform.size() * sizeof(wchar_t)) + sizeof(PA_Unichar);
	std::vector<char> buf(len);
	
	uint32_t ulen = PA_ConvertCharsetToCharset(
						(char *)inform.c_str(), 
						inform.size() * sizeof(wchar_t), 
						eVTC_WCHAR_T, 
						(char *)&buf[0],
						len, 
						eVTC_UTF_16
						);	

	returnValue.setUTF16String((const PA_Unichar *)&buf[0], ulen);
	returnValue.setReturn(pResult);
	
	MI.Close();
}

