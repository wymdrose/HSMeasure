#ifndef BCDH_STEP_H
#define BCDH_STEP_H

#include <QObject>
#include <array>
#include "hsmeasure.h"

class BCDH_step : public QObject
{
	Q_OBJECT

public:
	BCDH_step(HSMeasure* pHSMeasure);
	~BCDH_step();

	const int LSCOUNT = 800;

	struct STEP_MAP
	{
		LASER_AXIS_POS laserPos;
		LASER_NO laserNo;
	};

	LJIF_OPENPARAM_ETHERNET laser1_config;
	LJIF_OPENPARAM_ETHERNET laser2_config;
	LJIF_OPENPARAM_ETHERNET laser3_config;

	void BCDH_step::GetOneHeadData(LJIF_PROFILETARGET head, LJIF_OPENPARAM_ETHERNET config, float* buf);
	void BCDH_step::GetTwoHeadData(LJIF_OPENPARAM_ETHERNET config, float* buf1, float* buf2);

	static const STEP_MAP arrayStepMap[(SlotposNo4 + 1) * (StepJ + 1)];

	bool getPosFromCfg();
	bool getStepValue(QString, float&);
	bool getStepValue();
signals:
	void showMsgSignal(const QString&);
private:
	CLoadLJDllFunc * m_pLoadDllfunc;
	float posLaserAxis[laserPos11 + 1];
	float posPlat;

	HSMeasure* mpHSMeasure;
};


#endif // BCDH_STEP_H
