#include "hsmeasure.h"
#include "Lib/fileLib.hpp"
#include "bcdh_gap.h"
#include "bcdh_step.h"

extern QString mPath;
QMap<QString, QString> ioMap;

void MY_THREAD::fun()
{
	while (true)
	{
		if (true == this->mpHSMeasure->mbStop)
		{
			break;
		}

		if (true == this->mpHSMeasure->mbPause)
		{
			continue;
		}

		qDebug() << "fun..." << QThread::currentThreadId();

		QThread::msleep(500);

		QApplication::processEvents();
	}
	
	
}

void HSMeasure::onCheckBoxIo() //????
{
	Sleep(0);
	dmc_write_outport(mCardNo, 0, 0);
}

bool getIoMapFromCfg()
{
	QString fileName = QCoreApplication::applicationDirPath() +"/cfg/ioMap.csv";

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

	for (size_t i = 0; i < CSVList.size(); i++)
	{
		QStringList oneLine = CSVList[i].split(",");

		ioMap[oneLine[1]] = oneLine[0];
	}

	return true;
}

bool HSMeasure::ioOut(const QString io, const WORD on_off)
{
	if (io.left(3) != "OUT" || io.size() < 4)
	{
		return false;
	}

	if (io.size() < 5)
	{
		dmc_write_outbit(mCardNo, io.right(1).toInt(), on_off);
	}
	else
	{
		dmc_write_can_outbit(mCardNo, io.mid(3,1).toInt(), io.right(1).toInt(0,16), on_off);
	}

	return true;
}

short HSMeasure::ioGet(const QString io)
{
	if (io.left(2) == "IN")
	{
		if (3 == io.size())
		{
			return dmc_read_inbit(mCardNo, io.right(1).toInt(0, 16));
		}
		else
		{
			return dmc_read_can_inbit(mCardNo, io.mid(3, 1).toInt(), io.right(1).toInt(0, 16));
		}
	}

	else if(io.left(3) == "OUT")
	{
		if (3 == io.size())
		{
			return dmc_read_outbit(mCardNo, io.right(1).toInt(0, 16));
		}
		else
		{
			return dmc_read_can_outbit(mCardNo, io.mid(3, 1).toInt(), io.right(1).toInt(0, 16));
		}
	}

	else
		return -1;
}

void HSMeasure::cylinderMove(const IO_CYLINDER name)	//气缸运动
{
	switch (name)
	{
	case FEED_o:
		ioOut("OUT7", IO_OFF);
		ioOut("OUT8", IO_ON);
		break;

	case FEED_I:
		ioOut("OUT8", IO_OFF);
		ioOut("OUT7", IO_ON);
		break;
	case CLAMP1_o:
		ioOut("OUT13", IO_OFF);
		ioOut("OUT14", IO_ON);
		break;
	case CLAMP1_I:
		ioOut("OUT14", IO_OFF);
		ioOut("OUT13", IO_ON);
		break;
	case UPDOWN_A_o:
		ioOut("OUT9", IO_OFF);
		ioOut("OUTA", IO_ON);
		break;
	case UPDOWN_A_I:
		ioOut("OUTA", IO_OFF);
		ioOut("OUT9", IO_ON);
		break;

	case UPDOWN_B_o :
		ioOut("OUT15", IO_OFF);
		ioOut("OUT16", IO_ON);
		break;
	case UPDOWN_B_I:
		ioOut("OUT16", IO_OFF);
		ioOut("OUT15", IO_ON);
		break;

	case UPDOWN_C_1o:
		ioOut("OUT1D", IO_OFF);
		ioOut("OUT1E", IO_ON);
		break;
	case UPDOWN_C_1I:
		ioOut("OUT1E", IO_OFF);
		ioOut("OUT1D", IO_ON);
		break;

	case UPDOWN_C_2o:
		ioOut("OUT1F", IO_OFF);
		ioOut("OUT20", IO_ON);
		break;
	case UPDOWN_C_2I:
		ioOut("OUT20", IO_OFF);
		ioOut("OUT1F", IO_ON);
		break;

	case UPDOWN_C_3o:
		ioOut("OUT21", IO_OFF);
		ioOut("OUT22", IO_ON);
		break;
	case UPDOWN_C_3I:
		ioOut("OUT22", IO_OFF);
		ioOut("OUT21", IO_ON);
		break;


	case UPDOWN_C_4o:
		ioOut("OUT23", IO_OFF);
		ioOut("OUT24", IO_ON);
		break;
	case UPDOWN_C_4I:
		ioOut("OUT24", IO_OFF);
		ioOut("OUT23", IO_ON);
		break;

	default:
		break;
	}
}

short HSMeasure::cylinderCheck(const IO_CYLINDER name)		//气缸到位检查
{
	switch (name)
	{

	case FEED_o:
		return ioGet("IN11");
	case FEED_I:
		return ioGet("IN12");
	case CLAMP1_o:
		return ioGet("IN1C");
	case CLAMP1_I:
		return ioGet("IN1B");
	case UPDOWN_A_o:
		return ioGet("IN13") && ioGet("IN14") && ioGet("IN15") && ioGet("IN16");
	default:
		break;
	}

	return -1;
}

void HSMeasure::sensorOut(const IO_SENSOR name, const short on_off)
{
	switch (name)
	{
	case RedLight:		ioOut("OUT0", on_off);		break;
	case YellowLight:	ioOut("OUT1", on_off);		break;
	case GreenLight:	ioOut("OUT2", on_off);		break;
	case Buzzer:		ioOut("OUT3", on_off);		break;
	
	case FixtureVacuum:	ioOut("OUT6", on_off);	break;
	
	case Vacuum1:		ioOut("OUT17", on_off);		break;
	case Vacuum2:		ioOut("OUT18", on_off);		break;
	case Vacuum3:		ioOut("OUT19", on_off);		break;
	case Vacuum4:		ioOut("OUT1A", on_off);		break;

	case Vacuum5:		ioOut("OUT21", on_off);		break;
	case Vacuum6:		ioOut("OUT22", on_off);		break;
	case Vacuum7:		ioOut("OUT23", on_off);		break;
	case Vacuum8:		ioOut("OUT24", on_off);		break;

	case Vacuum9:		ioOut("OUT25", on_off);		break;
	case Vacuum10:		ioOut("OUT26", on_off);		break;
	case Vacuum11:		ioOut("OUT27", on_off);		break;
	case Vacuum12:		ioOut("OUT28", on_off);		break;
	
	default:
		break;
	}
}

short HSMeasure::sensorIn(const IO_SENSOR name)
{
	
	switch (name)
	{
	case START:			return ioGet("IN0");	
	case STOP:			return ioGet("IN1");
	case RESET:			return ioGet("IN2");
	case EMGSTOP:			return ioGet("IN3");
	case CONFIRM:			return ioGet("IN4");		
	case SWITCH_MODE:		return ioGet("IN5");
	
	case	LightCurtain:	return ioGet("IN33");
	
	case Fiber1:	return ioGet("IN6");
	case Fiber2:	return ioGet("IN7");
	case Fiber3:	return ioGet("IN8");
	case Fiber4:	return ioGet("IN9");
	
	case Opposite1: return ioGet("IN2E");
	case Opposite2: return ioGet("IN2F");
	case Opposite3: return ioGet("IN30");

	case FixtureVacuum:	return ioGet("IN10");
	
	case Vacuum1:		return ioGet("INA");
	case Vacuum2:		return ioGet("INB");
	case Vacuum3:		return ioGet("INC");
	case Vacuum4:		return ioGet("IND");
						
	case Vacuum5:		return ioGet("IN17");
	case Vacuum6:		return ioGet("IN18");
	case Vacuum7:		return ioGet("IN19");
	case Vacuum8:		return ioGet("IN1A");
						
	case Vacuum9:		return ioGet("IN21");
	case Vacuum10:		return ioGet("IN22");
	case Vacuum11:		return ioGet("IN23");
	case Vacuum12:		return ioGet("IN24");


	default:
		break;
	}

	return -1;
}

void HSMeasure::initUiIo()
{
	getIoMapFromCfg();

	//专用IO
	for (size_t i = 0; i < AXIS_NUM; i++)
	{
		for (size_t j = 0; j < IO_A_NUM; j++)
		{
			pBitInS[i][j] = findChild<QCheckBox*>("checkBoxInA" + QString::number(i + 1) + "_" + QString::number(j + 1));
			pBitOutS[i][j] = findChild<QCheckBox*>("checkBoxOutA" + QString::number(i + 1) + "_" + QString::number(j + 1));
			
			connect(pBitOutS[i][j], &QCheckBox::clicked, [this, i, j]()
			{
				switch (j)
				{
				case 0:	//
					pBitOutS[i][j]->isChecked() ? dmc_write_erc_pin(mCardNo, i, IO_ON) : dmc_write_sevon_pin(mCardNo, i, IO_OFF);
					break;
				case 1: 
					
					break;
				case 2:
					
					break;
				case 3:
					pBitOutS[i][j]->isChecked() ? dmc_write_sevon_pin(mCardNo, i, IO_ON) : dmc_write_sevon_pin(mCardNo, i, IO_OFF);
					break;
				
				default:
					break;
				}
			});
		
		}
	}

	pBitInS[0][0]->setText(QStringLiteral("X1轴（CCD轴）左极限"));
	pBitInS[0][1]->setText(QStringLiteral("X1轴（CCD轴）右极限"));
	pBitInS[0][2]->setText(QStringLiteral("X1轴（CCD轴）原点"));
	pBitInS[0][3]->setText(QStringLiteral("X1轴（CCD轴）报警"));

	pBitInS[1][0]->setText(QStringLiteral("X2轴（激光轴）左极限"));
	pBitInS[1][1]->setText(QStringLiteral("X2轴（激光轴）右极限"));
	pBitInS[1][2]->setText(QStringLiteral("X2轴（激光轴）原点"));
	pBitInS[1][3]->setText(QStringLiteral("X2轴（激光轴）报警"));

	pBitInS[2][0]->setText(QStringLiteral("X3轴（皮带步进）左极限"));
	pBitInS[2][1]->setText(QStringLiteral("X3轴（皮带步进）右极限"));
	pBitInS[2][2]->setText(QStringLiteral("X3轴（皮带步进）原点"));
	pBitInS[2][3]->setText(QStringLiteral("X3轴（皮带步进）报警"));

	pBitInS[3][0]->setText(QStringLiteral("Y2轴（上下料轴）左极限"));
	pBitInS[3][1]->setText(QStringLiteral("Y2轴（上下料轴）右极限"));
	pBitInS[3][2]->setText(QStringLiteral("Y2轴（上下料轴）原点"));
	pBitInS[3][3]->setText(QStringLiteral("Y2轴（上下料轴）报警"));

	pBitInS[4][0]->setText(QStringLiteral("Y2轴（上下料轴）左极限"));
	pBitInS[4][1]->setText(QStringLiteral("Y2轴（上下料轴）右极限"));
	pBitInS[4][2]->setText(QStringLiteral("Y2轴（上下料轴）原点"));
	pBitInS[4][3]->setText(QStringLiteral("Y2轴（上下料轴）报警"));
	//
	pBitOutS[0][0]->setText(QStringLiteral("X1轴（CCD轴）伺服使能"));
	pBitOutS[0][1]->setText(QStringLiteral("X1轴（CCD轴）伺服报警复位"));
	pBitOutS[0][2]->setText(QStringLiteral("X1轴（CCD轴）伺服脉冲输出"));
	pBitOutS[0][3]->setText(QStringLiteral("X1轴（CCD轴）伺服方向"));
								   
	pBitOutS[1][0]->setText(QStringLiteral("X2轴（激光轴）伺服使能"));
	pBitOutS[1][1]->setText(QStringLiteral("X2轴（激光轴）伺服报警复位"));
	pBitOutS[1][2]->setText(QStringLiteral("X2轴（激光轴）伺服脉冲输出"));
	pBitOutS[1][3]->setText(QStringLiteral("X2轴（激光轴）伺服方向"));
							  
	pBitOutS[2][0]->setText(QStringLiteral("X3轴（皮带步进）伺服使能"));
	pBitOutS[2][1]->setText(QStringLiteral("X3轴（皮带步进）伺服报警复位"));
	pBitOutS[2][2]->setText(QStringLiteral("X3轴（皮带步进）伺服脉冲输出"));
	pBitOutS[2][3]->setText(QStringLiteral("X3轴（皮带步进）伺服方向"));
							   
	pBitOutS[3][0]->setText(QStringLiteral("Y1轴（搬运轴）伺服使能"));
	pBitOutS[3][1]->setText(QStringLiteral("Y1轴（搬运轴）伺服报警复位"));
	pBitOutS[3][2]->setText(QStringLiteral("Y1轴（搬运轴）伺服脉冲输出"));
	pBitOutS[3][3]->setText(QStringLiteral("Y1轴（搬运轴）伺服方向"));
							   
	pBitOutS[4][0]->setText(QStringLiteral("Y2轴（上下料轴）伺服使能"));
	pBitOutS[4][1]->setText(QStringLiteral("Y2轴（上下料轴）伺服报警复位"));
	pBitOutS[4][2]->setText(QStringLiteral("Y2轴（上下料轴）伺服脉冲输出"));
	pBitOutS[4][3]->setText(QStringLiteral("Y2轴（上下料轴）伺服方向"));


	//通用IO 主板
	for (size_t i = 0; i < IO_BIT_NUM; i++)
	{
		pBitInM[i] = findChild<QCheckBox*>("checkBoxInM_" + QString::number(i + 1));
		pBitOutM[i] = findChild<QCheckBox*>("checkBoxOutM_" + QString::number(i + 1));
		
		pBitInM[i]->setText(ioMap.value("IN" + QString::number(i, 16).toUpper()));
		pBitOutM[i]->setText(ioMap.value("OUT" + QString::number(i, 16).toUpper()));

		connect(pBitOutM[i], &QCheckBox::clicked, [this, i]()
		{
			ioMutex.lock();
			pBitOutM[i]->isChecked() ? dmc_write_outbit(mCardNo, i, IO_ON) : dmc_write_outbit(mCardNo, i, IO_OFF);
			ioMutex.unlock();
		});	
	}

	//CAN
	for (size_t i = 0; i < IO_EX_NUM; i++)
	{
		for (size_t j = 0; j < IO_BIT_NUM; j++)
		{
			pBitInG[i][j] = findChild<QCheckBox*>("checkBoxInG" + QString::number(i + 1) + "_" + QString::number(j + 1));
			pBitOutG[i][j] = findChild<QCheckBox*>("checkBoxOutG" + QString::number(i + 1) + "_" + QString::number(j + 1));
	
			
			pBitInG[i][j]->setText(ioMap.value("IN" + QString("%1").arg(i + 1) + QString::number(j, 16).toUpper()));
			pBitOutG[i][j]->setText(ioMap.value("OUT" + QString("%1").arg(i + 1) + QString::number(j, 16).toUpper()));

			connect(pBitOutG[i][j], &QCheckBox::clicked, [this, i, j]()
			{
				ioMutex.lock();
				auto re = pBitOutG[i][j]->isChecked() ? dmc_write_can_outbit(mCardNo, i + 1, j, IO_ON) : dmc_write_can_outbit(mCardNo, i + 1, j, IO_OFF);
				ioMutex.unlock();
			});
		}
	}
	
}

void HSMeasure::onIoTimer()
{
	//主板通用IO
	for (size_t i = 0; i < IO_BIT_NUM; i++)
	{
		ioMutex.lock();
		dmc_read_inbit(mCardNo, i) ? pBitInM[i]->setChecked(false) : pBitInM[i]->setChecked(true);
		dmc_read_outbit(mCardNo, i) ? pBitOutM[i]->setChecked(false) : pBitOutM[i]->setChecked(true);
		ioMutex.unlock();
	}

	//扩展模块IO
	for (size_t i = 0; i < IO_EX_NUM; i++)
	{
		for (size_t j = 0; j < IO_BIT_NUM; j++)
		{		
			ioMutex.lock();
			dmc_read_can_inbit(mCardNo, i + 1, j) ? pBitInG[i][j]->setChecked(false) : pBitInG[i][j]->setChecked(true);
			dmc_read_can_outbit(mCardNo, i + 1, j) ? pBitOutG[i][j]->setChecked(false) : pBitOutG[i][j]->setChecked(true);
			ioMutex.unlock();
		}
	}
	
	QApplication::processEvents();
}

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



