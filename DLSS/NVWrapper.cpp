#include "NVWrapper.h"
#include "streamline-sdk-v2.9.0/include/sl_core_types.h"

void NVWrapper::SetSLOptions(const bool checkSig, const bool enableLog, const bool useNewSetTagAPI, const bool allowSMSCG)
{
	mOptions.checkSignature = checkSig;
	mOptions.enableLogging = enableLog;
	mOptions.useNuwSetTagAPI = useNewSetTagAPI;
	mOptions.allowSMSCG = allowSMSCG;
}

bool NVWrapper::Initialize_preDevice()
{
	sl::Preferences pref;

	return false;
}
