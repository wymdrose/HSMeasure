#include "hsmeasure.h"
#include "Lib/fileLib.hpp"
#include "bcdh_gap.h"
#include "bcdh_step.h"

extern QString mPath;


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

void HSMeasure::initUiIo()
{
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
					pBitOutS[i][j]->isChecked() ? dmc_write_erc_pin(mCardNo, i, 0) : dmc_write_sevon_pin(mCardNo, i, 1);
					break;
				case 1: 
					
					break;
				case 2:
					
					break;
				case 3:
					pBitOutS[i][j]->isChecked() ? dmc_write_sevon_pin(mCardNo, i, 0) : dmc_write_sevon_pin(mCardNo, i, 1);
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
		
		connect(pBitOutM[i], &QCheckBox::clicked, [this, i]()
		{
			ioMutex.lock();
			pBitOutM[i]->isChecked() ? dmc_write_outbit(mCardNo, i, 0) : dmc_write_outbit(mCardNo, i, 1);
			ioMutex.unlock();
		});	
	}

	pBitInM[0]->setText(QStringLiteral("启动"));
	pBitInM[1]->setText(QStringLiteral("停止"));
	pBitInM[2]->setText(QStringLiteral("复位"));
	pBitInM[3]->setText(QStringLiteral("急停"));
	pBitInM[4]->setText(QStringLiteral("人工确认"));
	pBitInM[5]->setText(QStringLiteral("生产/复检"));
	pBitInM[6]->setText(QStringLiteral("光纤有料检测1"));
	pBitInM[7]->setText(QStringLiteral("光纤有料检测2"));
	pBitInM[8]->setText(QStringLiteral("光纤有料检测3"));
	pBitInM[9]->setText(QStringLiteral("光纤有料检测4"));
	pBitInM[10]->setText(QStringLiteral("真空吸料检测1"));
	pBitInM[11]->setText(QStringLiteral("真空吸料检测2"));
	pBitInM[12]->setText(QStringLiteral("真空吸料检测3"));
	pBitInM[13]->setText(QStringLiteral("真空吸料检测4"));
	pBitInM[14]->setText(QStringLiteral("备用（LTC0）"));
	pBitInM[15]->setText(QStringLiteral("备用（LTC1）"));
	//
	pBitOutM[0]->setText(QStringLiteral("红灯"));
	pBitOutM[1]->setText(QStringLiteral("黄灯"));
	pBitOutM[2]->setText(QStringLiteral("绿灯"));
	pBitOutM[3]->setText(QStringLiteral("蜂鸣器"));
	pBitOutM[4]->setText(QStringLiteral("OK线启动"));
	pBitOutM[5]->setText(QStringLiteral("NG线启动"));
	pBitOutM[6]->setText(QStringLiteral("治具真空打开"));
	pBitOutM[7]->setText(QStringLiteral("送料气缸进"));
	pBitOutM[8]->setText(QStringLiteral("送料气缸退"));
	pBitOutM[9]->setText(QStringLiteral("升降气缸下降"));
	pBitOutM[10]->setText(QStringLiteral("升降气缸上升"));
	pBitOutM[11]->setText(QStringLiteral("真空吸打开1"));
	pBitOutM[12]->setText(QStringLiteral("备用(CMP0)"));
	pBitOutM[13]->setText(QStringLiteral("备用(CMP1)"));
	pBitOutM[14]->setText(QStringLiteral("备用(CMP2)"));
	pBitOutM[15]->setText(QStringLiteral("备用(CMP3)"));




	//CAN
	for (size_t i = 0; i < IO_EX_NUM; i++)
	{
		for (size_t j = 0; j < IO_BIT_NUM; j++)
		{
			pBitInG[i][j] = findChild<QCheckBox*>("checkBoxInG" + QString::number(i + 1) + "_" + QString::number(j + 1));
			pBitOutG[i][j] = findChild<QCheckBox*>("checkBoxOutG" + QString::number(i + 1) + "_" + QString::number(j + 1));
	
			connect(pBitOutG[i][j], &QCheckBox::clicked, [this, i, j]()
			{
				ioMutex.lock();
				auto re = pBitOutG[i][j]->isChecked() ? dmc_write_can_outbit(mCardNo, i + 1, j, 0) : dmc_write_can_outbit(mCardNo, i + 1, j, 1);
				ioMutex.unlock();
			});
		}
	}
	
	pBitInG[0][0]->setText(QStringLiteral("治具有无放好检测"));			
	pBitInG[0][1]->setText(QStringLiteral("送料气缸进")); 		    
	pBitInG[0][2]->setText(QStringLiteral("送料气缸退"));			
	pBitInG[0][3]->setText(QStringLiteral("升降气缸下降"));			
	pBitInG[0][4]->setText(QStringLiteral("升降气缸上升"));			
	pBitInG[0][5]->setText(QStringLiteral("真空吸料检测5"));			
	pBitInG[0][6]->setText(QStringLiteral("真空吸料检测6"));			
	pBitInG[0][7]->setText(QStringLiteral("真空吸料检测7"));			
	pBitInG[0][8]->setText(QStringLiteral("真空吸料检测8"));			
	pBitInG[0][9]->setText(QStringLiteral("固定气缸夹紧"));			
	pBitInG[0][10]->setText(QStringLiteral("固定气缸松开"));			
	pBitInG[0][11]->setText(QStringLiteral("1#升降气缸下降"));		
	pBitInG[0][12]->setText(QStringLiteral("1#升降气缸上升"));		
	pBitInG[0][13]->setText(QStringLiteral("真空吸料检测9"));			
	pBitInG[0][14]->setText(QStringLiteral("真空吸料检测10"));		
	pBitInG[0][15]->setText(QStringLiteral("真空吸料检测11"));		
	//
	pBitInG[1][0]->setText(QStringLiteral("真空吸料检测12"));
	pBitInG[1][1]->setText(QStringLiteral("2.1#升降气缸下降"));
	pBitInG[1][2]->setText(QStringLiteral("2.1#升降气缸上升"));
	pBitInG[1][3]->setText(QStringLiteral("2.2#升降气缸下降"));
	pBitInG[1][4]->setText(QStringLiteral("2.2#升降气缸上升"));
	pBitInG[1][5]->setText(QStringLiteral("2.3#升降气缸下降"));
	pBitInG[1][6]->setText(QStringLiteral("2.3#升降气缸上升"));
	pBitInG[1][7]->setText(QStringLiteral("2.4#升降气缸下降"));
	pBitInG[1][8]->setText(QStringLiteral("2.4#升降气缸上升"));
	pBitInG[1][9]->setText(QStringLiteral("2#固定气缸夹紧"));
	pBitInG[1][10]->setText(QStringLiteral("2#固定气缸松开"));
	pBitInG[1][11]->setText(QStringLiteral("1#NG标记气缸下降"));
	pBitInG[1][12]->setText(QStringLiteral("1#NG标记气缸上升"));
	pBitInG[1][13]->setText(QStringLiteral("2#NG标记气缸下降"));
	pBitInG[1][14]->setText(QStringLiteral("2#NG标记气缸上升"));
	pBitInG[1][15]->setText(QStringLiteral("3#NG标记气缸下降"));
	//
	pBitInG[1][16]->setText(QStringLiteral("3#NG标记气缸上升"));
	pBitInG[2][17]->setText(QStringLiteral("阻挡气缸下降"));
	pBitInG[2][18]->setText(QStringLiteral("阻挡气缸上升"));
	pBitInG[2][19]->setText(QStringLiteral("OK线 进料对射"));
	pBitInG[2][20]->setText(QStringLiteral("OK线 出料对射1"));
	pBitInG[2][21]->setText(QStringLiteral("OK线 出料对射2"));
	pBitInG[2][22]->setText(QStringLiteral("NG线 进料反射1"));
	pBitInG[2][23]->setText(QStringLiteral("备用"));
	pBitInG[2][24]->setText(QStringLiteral("备用"));
	pBitInG[2][10]->setText(QStringLiteral("NG线 阻挡位有料对射"));
	pBitInG[2][11]->setText(QStringLiteral("NG线 出料对射"));
	pBitInG[2][12]->setText(QStringLiteral("NG接料盒检测1"));
	pBitInG[2][13]->setText(QStringLiteral("NG接料盒检测2"));
	pBitInG[2][14]->setText(QStringLiteral("光栅"));
	pBitInG[2][15]->setText(QStringLiteral("门磁"));
	pBitInG[2][16]->setText(QStringLiteral("备用"));

	//pBitOutG			
	pBitOutG[0][0]->setText(QStringLiteral("真空破打开1"));
	pBitOutG[0][1]->setText(QStringLiteral("真空吸打开2"));
	pBitOutG[0][2]->setText(QStringLiteral("真空破打开2"));
	pBitOutG[0][3]->setText(QStringLiteral("真空吸打开3"));
	pBitOutG[0][4]->setText(QStringLiteral("真空破打开3"));
	pBitOutG[0][5]->setText(QStringLiteral("真空吸打开4"));
	pBitOutG[0][6]->setText(QStringLiteral("真空破打开4"));
	pBitOutG[0][7]->setText(QStringLiteral("固定气缸夹紧"));
	pBitOutG[0][8]->setText(QStringLiteral("固定气缸松开"));
	pBitOutG[0][9]->setText(QStringLiteral("1#升降气缸下降"));
	pBitOutG[0][10]->setText(QStringLiteral("1#升降气缸上升"));
	pBitOutG[0][11]->setText(QStringLiteral("真空吸打开5"));
	pBitOutG[0][12]->setText(QStringLiteral("真空破打开5"));
	pBitOutG[0][13]->setText(QStringLiteral("真空吸打开6"));
	pBitOutG[0][14]->setText(QStringLiteral("真空破打开6"));
	pBitOutG[0][15]->setText(QStringLiteral("真空吸打开7"));
	//																	
	pBitOutG[1][0]->setText(QStringLiteral("真空破打开7"));
	pBitOutG[1][1]->setText(QStringLiteral("真空吸打开8"));
	pBitOutG[1][2]->setText(QStringLiteral("真空破打开8"));
	pBitOutG[1][3]->setText(QStringLiteral("2#固定气缸夹紧"));
	pBitOutG[1][4]->setText(QStringLiteral("2#固定气缸松开"));
	pBitOutG[1][5]->setText(QStringLiteral("2.1#升降气缸下降"));
	pBitOutG[1][6]->setText(QStringLiteral("2.1#升降气缸上升"));
	pBitOutG[1][7]->setText(QStringLiteral("2.2#升降气缸下降"));
	pBitOutG[1][8]->setText(QStringLiteral("2.2#升降气缸上升"));
	pBitOutG[1][9]->setText(QStringLiteral("2.3#升降气缸下降"));
	pBitOutG[1][10]->setText(QStringLiteral("2.3#升降气缸上升"));
	pBitOutG[1][11]->setText(QStringLiteral("2.3#升降气缸下降"));
	pBitOutG[1][12]->setText(QStringLiteral("2.3#升降气缸上升"));
	pBitOutG[1][13]->setText(QStringLiteral("2.4#升降气缸下降"));
	pBitOutG[1][14]->setText(QStringLiteral("2.4#升降气缸上升"));
	pBitOutG[1][15]->setText(QStringLiteral("真空吸打开10"));
	//
	pBitOutG[2][0]->setText(QStringLiteral("真空破打开10"));
	pBitOutG[2][1]->setText(QStringLiteral("真空吸打开11"));
	pBitOutG[2][2]->setText(QStringLiteral("真空破打开11"));
	pBitOutG[2][3]->setText(QStringLiteral("真空吸打开12"));
	pBitOutG[2][4]->setText(QStringLiteral("真空破打开12"));
	pBitOutG[2][5]->setText(QStringLiteral("1#NG标记气缸下降"));
	pBitOutG[2][6]->setText(QStringLiteral("1#NG标记气缸上升"));
	pBitOutG[2][7]->setText(QStringLiteral("2#NG标记气缸下降"));
	pBitOutG[2][8]->setText(QStringLiteral("2#NG标记气缸上升"));
	pBitOutG[2][9]->setText(QStringLiteral("备用"));
	pBitOutG[2][10]->setText(QStringLiteral("备用"));
	pBitOutG[2][11]->setText(QStringLiteral("阻挡气缸下降"));
	pBitOutG[2][12]->setText(QStringLiteral("阻挡气缸上升"));
	pBitOutG[2][13]->setText(QStringLiteral("备用"));
	pBitOutG[2][14]->setText(QStringLiteral("备用"));
	pBitOutG[2][15]->setText(QStringLiteral("备用"));
	
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



