#include "hsmeasure.h"
#include "Lib/fileLib.hpp"
#include "bcdh_gap.h"
#include "bcdh_step.h"

extern QString mPath;

void HSMeasure::showValueToUi(const FIX_VALUE& tFixValue)
{
	static unsigned int lineNo = 0;

	
	for (size_t i = 0; i <= SlotposNo4; i++)
	{
		QStringList oneLine;

		for (size_t j = 0; j <= GapJ; j++)
		{
			oneLine.push_back(QString("%1").arg(tFixValue.plug[i].gap[j]));
		}

		for (size_t j = 0; j <= StepJ; j++)
		{
			oneLine.push_back(QString("%1").arg(tFixValue.plug[i].step[j]));
		}

		for (size_t j = 0; j < oneLine.size(); j++)
		{
			model_result->setItem(lineNo, j, new QStandardItem(oneLine[j]));
		}
		
		lineNo++;
	}
	
	QApplication::processEvents();
}

void HSMeasure::showLogToUi()
{
	static unsigned int lineNo = 0;

	_FILECLASS::CSV_FILE tCsv(mPath + "/Log/result_" + QDate::currentDate().toString("yyyy-MM-dd") + ".csv");

	QStringList oneLine = tCsv.getLast().split(",");

	for (size_t j = 0; j < oneLine.size(); j++)
	{
		model_result->setItem(lineNo, j, new QStandardItem(oneLine[j]));
	}

	lineNo+=4;

	QApplication::processEvents();
}


void HSMeasure::saveValueToLog(const FIX_VALUE& fixValue)
{
	_FILECLASS::CSV_FILE tCsv(mPath + "/Log/result_" + QDate::currentDate().toString("yyyy-MM-dd") + ".csv");

	QStringList tList;

	for (size_t i = 0; i < SlotposNo4 + 1; i++)
	{
		tList.clear();

		if (false == flagSlot[i])
		{
			for (size_t j = 0; j < GapJ + StepJ + 2; j++)
			{
				tList.push_back("#");
			}		
		}
		else
		{
			for (size_t j = 0; j < GapJ + 1; j++)
			{
				tList.push_back(QString("%1").arg(fixValue.plug[i].gap[j]));
			}

			for (size_t j = 0; j < StepJ + 1; j++)
			{
				tList.push_back(QString("%1").arg(fixValue.plug[i].step[j]));
			}
		}

		tList.push_back(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

		tCsv.append(tList);
	}

}


void HSMeasure::onIoTimer()
{
	WORD portno;

	dmc_read_inport(mCardNo, portno);
	QApplication::processEvents();
//	dmc_write_outport(mCardNo, 0, 0);

}



