#include "bcdh_gap.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>


extern QString mPath;

BCDH_gap::BCDH_gap(HSMeasure* pHSMeasure)
	: mpHSMeasure(pHSMeasure)
{
	connect(this, SIGNAL(showMsgSignal(const QString&)), mpHSMeasure, SLOT(show_msg(const QString &)));
	connect(this, SIGNAL(showDialogSignal(const QString&)), mpHSMeasure, SLOT(show_dialog(const QString &)), Qt::BlockingQueuedConnection);
		
	mpHSMeasure->mpSerialportCcd[0]->open();
	mpHSMeasure->mpSerialportCcd[1]->open();

}

BCDH_gap::~BCDH_gap()
{
	mpHSMeasure->mpSerialportCcd[0]->close();
	mpHSMeasure->mpSerialportCcd[1]->close();

	qDebug() << "~BCDH_gap()";
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
	
	

	gapValue = 5.0;

	return true;
}

bool BCDH_gap::getGapValue(const GAP_NO gapNo, float gapValue[])
{
	if (false == getPosFromCfg())
	{
		return false;
	}

	showMsgSignal(QString("arrayGapPos[gapNo].ccdPos:%1").arg(arrayGapPos[gapNo].ccdPos));

	if (false == mpHSMeasure->mpMOTIONLib->mpDmcAxis[mpHSMeasure->mCardNo][mpHSMeasure->ccdAxisNo]->moveAndCheck(arrayGapPos[gapNo].ccdPos))
	{
		return false;
	}

	if (false == mpHSMeasure->mpMOTIONLib->mpDmcAxis[mpHSMeasure->mCardNo][mpHSMeasure->platformAxisNo]->moveAndCheck(arrayGapPos[gapNo].platPos))
	{
		return false;
	}

	QString tRecv;

	mpHSMeasure->mpSerialportCcd[0]->communicate("EXW," + QString("%1").arg(gapNo) + "\r", tRecv);
	mpHSMeasure->mpSerialportCcd[1]->communicate("EXW," + QString("%1").arg(gapNo) + "\r", tRecv);

	mpHSMeasure->mpSerialportCcd[0]->communicate("T1\r", tRecv);	
	gapValue[SlotposNo1] = tRecv.mid(3).toFloat();
	mpHSMeasure->mpSerialportCcd[1]->communicate("T1\r", tRecv);
	gapValue[SlotposNo3] = tRecv.mid(3).toFloat();

	
	//
	mpHSMeasure->mpSerialportCcd[0]->communicate("T2\r", tRecv);
	gapValue[SlotposNo2] = tRecv.mid(3).toFloat();
	mpHSMeasure->mpSerialportCcd[1]->communicate("T2\r", tRecv);
	gapValue[SlotposNo4] = tRecv.mid(3).toFloat();

	showDialogSignal(QString("pos %1...").arg(gapNo));

	return true;
}

bool BCDH_gap::getGapValue()
{
	
	for (size_t j = 0; j < GapJ + 1; j++)
	{
		
		if (GapF == j)
		{
			showDialogSignal("rotate...");

			/*
			if (false == mpHSMeasure->mpMOTIONLib->mpDmcAxis[mpHSMeasure->mCardNo][mpHSMeasure->rotateAxisNo]->moveAndCheckdone(0, DEFAUL_HOME_TIME_OUT))
			{
				return false;
			}
			*/
		}
		
		float tValue[SlotposNo4 + 1] = {-1};
				
		if (false == getGapValue((GAP_NO)j, tValue))
		{
			
			return false;
		}

		mpHSMeasure->gapValues[SlotposNo1][j] = tValue[SlotposNo1];
		mpHSMeasure->gapValues[SlotposNo2][j] = tValue[SlotposNo2];
		mpHSMeasure->gapValues[SlotposNo3][j] = tValue[SlotposNo3];
		mpHSMeasure->gapValues[SlotposNo4][j] = tValue[SlotposNo4];
	}
	
	return true;
}

bool BCDH_gap::getGapValueFront()
{
	for (; mCurStep < GapE + 1; mCurStep++)
	{

		float tValue[SlotposNo4 + 1] = { -1 };

		if (false == getGapValue((GAP_NO)mCurStep, tValue))
		{

			return false;
		}

		mpHSMeasure->gapValues[SlotposNo1][mCurStep] = tValue[SlotposNo1];
		mpHSMeasure->gapValues[SlotposNo2][mCurStep] = tValue[SlotposNo2];
		mpHSMeasure->gapValues[SlotposNo3][mCurStep] = tValue[SlotposNo3];
		mpHSMeasure->gapValues[SlotposNo4][mCurStep] = tValue[SlotposNo4];
	}

	return true;
}


bool BCDH_gap::getGapValueSide()
{
	for (; mCurStep < GapJ + 1; mCurStep++)
	{

		float tValue[SlotposNo4 + 1] = { -1 };

		if (false == getGapValue((GAP_NO)mCurStep, tValue))
		{
			return false;
		}

		mpHSMeasure->gapValues[SlotposNo1][mCurStep] = tValue[SlotposNo1];
		mpHSMeasure->gapValues[SlotposNo2][mCurStep] = tValue[SlotposNo2];
		mpHSMeasure->gapValues[SlotposNo3][mCurStep] = tValue[SlotposNo3];
		mpHSMeasure->gapValues[SlotposNo4][mCurStep] = tValue[SlotposNo4];
	}

	mCurStep = 0;
	return true;
}


