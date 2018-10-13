#include "bcdh_step.h"

extern QString mPath;

const BCDH_step::STEP_MAP BCDH_step::arrayStepMap[(SlotposNo4 + 1) * (StepJ + 1)] = {
	{ laserPos1, laserNo1 }, { laserPos1, laserNo6 },
	{ laserPos2, laserNo1 }, { laserPos2, laserNo6 }, { laserPos2, laserNo2 }, { laserPos2, laserNo5 },
	{ laserPos3, laserNo2 }, { laserPos3, laserNo5 },
	{ laserPos4, laserNo1 }, { laserPos4, laserNo6 }, { laserPos4, laserNo2 }, { laserPos4, laserNo5 },{ laserPos4, laserNo3 }, { laserPos4, laserNo4 },
	{ laserPos5, laserNo2 }, { laserPos5, laserNo5 }, { laserPos5, laserNo3 }, { laserPos5, laserNo4 },
	{ laserPos6, laserNo1 }, { laserPos6, laserNo6 }, { laserPos6, laserNo2 }, { laserPos6, laserNo5 }, { laserPos6, laserNo3 }, { laserPos6, laserNo4 },
	{ laserPos7, laserNo2 }, { laserPos7, laserNo5 }, { laserPos7, laserNo3 }, { laserPos7, laserNo4 },
	{ laserPos8, laserNo2 }, { laserPos8, laserNo5 }, { laserPos8, laserNo3 }, { laserPos8, laserNo4 },
	{ laserPos9, laserNo2 }, { laserPos9, laserNo5 }, { laserPos9, laserNo3 }, { laserPos9, laserNo4 },
	{ laserPos10, laserNo3 }, { laserPos10, laserNo4 },
	{ laserPos11, laserNo3 }, { laserPos11, laserNo4 },

};

const QStringList POS_STEP =
{
	"StepC_1", "StepH_1",
	"StepC_2", "StepH_2", "StepD_1", "StepG_1",
	"StepB_1", "StepI_1",
	"StepC_3", "StepH_3", "StepD_2", "StepG_2", "StepE_1", "StepF_1",
	"StepB_2", "StepI_2", "StepA_1", "StepJ_1",
	"StepC_4", "StepH_4", "StepD_3", "StepG_3", "StepE_2", "StepF_2",
	"StepB_3", "StepI_3", "StepA_2", "StepJ_2",
	"StepD_4", "StepG_4", "StepE_3", "StepF_3",
	"StepB_4", "StepI_4", "StepA_3", "StepJ_3",
	"StepE_4", "StepF_4",
	"StepA_4", "StepJ_4"
};

BCDH_step::BCDH_step(HSMeasure* pHSMeasure)
	: mpHSMeasure(pHSMeasure)
{

	auto a = arrayStepMap[(SlotposNo3 + 1) * (StepD + 1)];
	




}

BCDH_step::~BCDH_step()
{

}



bool BCDH_step::getPosFromCfg()
{
	QString fileName = mPath + "/cfg/stepPos.csv";

	QFile csvFile(fileName);

	QStringList CSVList;

	if (!csvFile.open(QIODevice::ReadOnly))
	{
		return false;
	}

	QTextStream stream(&csvFile);

	while (!stream.atEnd())
	{
		CSVList.push_back(stream.readLine());
	}
	csvFile.close();

	for (size_t i = 1; i < CSVList.size(); i++)
	{
		QStringList oneLine = CSVList[i].split(",");

		float posLaser = oneLine[1].toFloat();
		float posPlat = oneLine[2].toFloat();

		posLaserAxis[i - 1] = posLaser;
	}

	return true;
}

bool BCDH_step::getStepValue(QString posStep, float& stepValue)
{
	getPosFromCfg();

	auto a = POS_STEP.indexOf(posStep);

	qDebug() << "posLaserAxis[arrayStepMap[POS_STEP.indexOf(posStep)].laserPos]:" << posLaserAxis[arrayStepMap[POS_STEP.indexOf(posStep)].laserPos];

	if (false == mpHSMeasure->mpMOTIONLib->mpDmcAxis[mpHSMeasure->mCardNo][mpHSMeasure->laserAxisNo]->moveAndCheckdone(posLaserAxis[arrayStepMap[a].laserPos], 100000))
	{
		return false;
	}
	
	Sleep(300);
	//
	stepValue = 6.0;


	return true;
}

bool BCDH_step::getStepValue()
{

	float tValue(0);

	for (size_t i = 0; i < POS_STEP.size(); i++)
	{
		if (false == getStepValue(POS_STEP[i], tValue))
		{
			return false;
		}

		int slotNo = POS_STEP[i].right(1).toInt() - 1;

		QByteArray tByteArray = POS_STEP[i].mid(4, 1).toLocal8Bit();
		int stepNo = tByteArray[0] - 'A';

		mpHSMeasure->fixValue.plug[slotNo].step[stepNo] = tValue;
	}
	

	return true;
}
