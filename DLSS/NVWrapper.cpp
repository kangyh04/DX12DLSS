#include "NVWrapper.h"
#include "streamline-sdk-v2.9.0/include/sl_core_types.h"
#include "streamline-sdk-v2.9.0/include/sl_security.h"
#include "streamline-sdk-v2.9.0/include/sl_core_api.h"
#include <debugapi.h>

void LogMessageCallback(sl::LogType type, const char* msg)
{
	switch (type)
	{
	case sl::LogType::eInfo:
		OutputDebugStringA("[SL INFO]: ");
		break;
	case sl::LogType::eWarn:
		OutputDebugStringA("[SL WARNING]: ");
		break;
	case sl::LogType::eError:
		OutputDebugStringA("[SL ERROR]: ");
		break;
	default:
		break;
	}
	OutputDebugStringA(msg);
	OutputDebugStringA("\n");
}

bool SuccessCheck(sl::Result result, const char* location)
{
	if (result == sl::Result::eOk)
	{
		return true;
	}

	LogMessageCallback(sl::LogType::eError, ("Error: " + static_cast<int>(result) + (location == nullptr ? "" : std::string(location))).c_str());
	return false;
}

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

	pref.applicationId = AppID;

	if (mOptions.enableLogging)
	{
		pref.showConsole = true;
		pref.logMessageCallback = &LogMessageCallback;
		pref.logLevel = sl::LogLevel::eDefault;
	}
	else
	{
		pref.logLevel = sl::LogLevel::eOff;
	}

	sl::Feature featuresToLoad[] =
	{
		sl::kFeatureDLSS,
		sl::kFeaturePCL
	};

	pref.featuresToLoad = featuresToLoad;
	pref.numFeaturesToLoad = static_cast<uint32_t>(std::size(featuresToLoad));

	pref.renderAPI = sl::RenderAPI::eD3D12;

	pref.flags |= sl::PreferenceFlags::eUseManualHooking;

	if (mOptions.useNuwSetTagAPI)
	{
		pref.flags |= sl::PreferenceFlags::eUseFrameBasedResourceTagging;
	}

	std::wstring pathDLL = L"./sl.interposer.dll";

	HMODULE interposer = {};
	interposer = LoadLibraryW(pathDLL.c_str());

	if (!interposer)
	{
		OutputDebugStringA("Failed to load sl.interposer.dll\n");
		return false;
	}

	auto initialized = SuccessCheck(slInit(pref, SDK_VERSION), "slInit");

	if (!initialized)
	{
		OutputDebugStringA("Failed to initialize SL.");
		return false;
	}

	slSetFeatureLoaded(sl::kFeatureDLSS_G, false);

	return false;
}
