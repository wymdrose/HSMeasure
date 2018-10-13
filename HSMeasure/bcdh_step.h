#ifndef BCDH_STEP_H
#define BCDH_STEP_H

#include <QObject>
#include <array>
#include "hsmeasure.h"

class BCDH_step //: public QObject
{
//	Q_OBJECT

public:
	BCDH_step(HSMeasure* pHSMeasure);
	~BCDH_step();

	
	struct STEP_MAP
	{
		LASER_AXIS_POS laserPos;
		LASER_NO laserAxisNo;
	};

	static const STEP_MAP arrayStepMap[(SlotposNo4 + 1) * (StepJ + 1)];

	bool getPosFromCfg();
	bool getStepValue(QString, float&);
	bool getStepValue();

private:
	float posLaserAxis[laserPos11 + 1];
	HSMeasure* mpHSMeasure;
};


#endif // BCDH_STEP_H
