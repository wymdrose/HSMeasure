#include "bcdh_gap.h"
#include <QFile>
#include <QTextStream>

extern QString mPath;

BCDH_gap::BCDH_gap(HSMeasure* pHSMeasure)
	: mpHSMeasure(pHSMeasure)
{
	
	
		
}

BCDH_gap::~BCDH_gap()
{

}


bool BCDH_gap::getPosFromCfg()
{
	QString fileName = mPath + "/cfg/gapPos.csv";

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

		float posCcd = oneLine[1].toFloat();
		float posPlat = oneLine[2].toFloat();

		arrayGapPos[i - 1] = { posCcd, posPlat };
	}

	return true;
}

bool BCDH_gap::getGapValue(const SLOTPOS_NO slotposNo, const GAP_NO gapNo, float& gapValue)
{
	if (false == getPosFromCfg())
	{
		return false;
	}

	qDebug() << "arrayGapPos[gapNo].ccdPos:" << arrayGapPos[gapNo].ccdPos;

	if (false == mpHSMeasure->mpMOTIONLib->mpDmcAxis[mpHSMeasure->mCardNo][mpHSMeasure->ccdAxisNo]->moveAndCheckdone(arrayGapPos[gapNo].ccdPos, DEFAUL_HOME_TIME_OUT))
	{
		return false;
	}

	if (false == mpHSMeasure->mpMOTIONLib->mpDmcAxis[mpHSMeasure->mCardNo][mpHSMeasure->platformAxisNo]->moveAndCheckdone(arrayGapPos[gapNo].platPos, DEFAUL_HOME_TIME_OUT))
	{
		return false;
	}
	
	Sleep(300);

	gapValue = 5.0;

	return true;
}

bool BCDH_gap::getGapValue(const GAP_NO gapNo, float gapValue[])
{
	if (false == getPosFromCfg())
	{
		return false;
	}

	qDebug() << "arrayGapPos[gapNo].ccdPos:" << arrayGapPos[gapNo].ccdPos;

	if (false == mpHSMeasure->mpMOTIONLib->mpDmcAxis[mpHSMeasure->mCardNo][mpHSMeasure->ccdAxisNo]->moveAndCheckdone(arrayGapPos[gapNo].ccdPos, DEFAUL_HOME_TIME_OUT))
	{
		return false;
	}

	if (false == mpHSMeasure->mpMOTIONLib->mpDmcAxis[mpHSMeasure->mCardNo][mpHSMeasure->platformAxisNo]->moveAndCheckdone(arrayGapPos[gapNo].platPos, DEFAUL_HOME_TIME_OUT))
	{
		return false;
	}

	Sleep(300);

	gapValue[SlotposNo1] = 1.0;
	gapValue[SlotposNo2] = 2.0;
	gapValue[SlotposNo3] = 3.0;
	gapValue[SlotposNo4] = 4.0;

	return true;
}

bool BCDH_gap::getGapValue()
{
	
	for (size_t j = 0; j < GapJ + 1; j++)
	{
		float tValue[SlotposNo4 + 1] = {0.0};
		
		
		if (false == getGapValue((GAP_NO)j, tValue))
		{
			
			return false;
		}

		mpHSMeasure->fixValue.plug[SlotposNo1].gap[j] = tValue[SlotposNo1];
		mpHSMeasure->fixValue.plug[SlotposNo2].gap[j] = tValue[SlotposNo2];
		mpHSMeasure->fixValue.plug[SlotposNo3].gap[j] = tValue[SlotposNo3];
		mpHSMeasure->fixValue.plug[SlotposNo4].gap[j] = tValue[SlotposNo4];
	}
	
	return true;
}

