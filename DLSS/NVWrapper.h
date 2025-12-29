#pragma once

struct SLOptions
{
	bool checkSignature = true;
	bool enableLogging = false;
	bool useNuwSetTagAPI = true;
	bool allowSMSCG = false;
};

class NVWrapper
{
public:
	static NVWrapper& GetInstance()
	{
		static NVWrapper instance;
		return instance;
	}

	void SetSLOptions(const bool checkSig, const bool enableLog, const bool useNewSetTagAPI, const bool allowSMSCG);
	bool Initialize_preDevice();
	bool Initialize_postDevice();

private:
	SLOptions mOptions;
};
