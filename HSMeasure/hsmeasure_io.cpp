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
	//ר��IO
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

	pBitInS[0][0]->setText(QStringLiteral("X1�ᣨCCD�ᣩ����"));
	pBitInS[0][1]->setText(QStringLiteral("X1�ᣨCCD�ᣩ�Ҽ���"));
	pBitInS[0][2]->setText(QStringLiteral("X1�ᣨCCD�ᣩԭ��"));
	pBitInS[0][3]->setText(QStringLiteral("X1�ᣨCCD�ᣩ����"));

	pBitInS[1][0]->setText(QStringLiteral("X2�ᣨ�����ᣩ����"));
	pBitInS[1][1]->setText(QStringLiteral("X2�ᣨ�����ᣩ�Ҽ���"));
	pBitInS[1][2]->setText(QStringLiteral("X2�ᣨ�����ᣩԭ��"));
	pBitInS[1][3]->setText(QStringLiteral("X2�ᣨ�����ᣩ����"));

	pBitInS[2][0]->setText(QStringLiteral("X3�ᣨƤ������������"));
	pBitInS[2][1]->setText(QStringLiteral("X3�ᣨƤ���������Ҽ���"));
	pBitInS[2][2]->setText(QStringLiteral("X3�ᣨƤ��������ԭ��"));
	pBitInS[2][3]->setText(QStringLiteral("X3�ᣨƤ������������"));

	pBitInS[3][0]->setText(QStringLiteral("Y2�ᣨ�������ᣩ����"));
	pBitInS[3][1]->setText(QStringLiteral("Y2�ᣨ�������ᣩ�Ҽ���"));
	pBitInS[3][2]->setText(QStringLiteral("Y2�ᣨ�������ᣩԭ��"));
	pBitInS[3][3]->setText(QStringLiteral("Y2�ᣨ�������ᣩ����"));

	pBitInS[4][0]->setText(QStringLiteral("Y2�ᣨ�������ᣩ����"));
	pBitInS[4][1]->setText(QStringLiteral("Y2�ᣨ�������ᣩ�Ҽ���"));
	pBitInS[4][2]->setText(QStringLiteral("Y2�ᣨ�������ᣩԭ��"));
	pBitInS[4][3]->setText(QStringLiteral("Y2�ᣨ�������ᣩ����"));
	//
	pBitOutS[0][0]->setText(QStringLiteral("X1�ᣨCCD�ᣩ�ŷ�ʹ��"));
	pBitOutS[0][1]->setText(QStringLiteral("X1�ᣨCCD�ᣩ�ŷ�������λ"));
	pBitOutS[0][2]->setText(QStringLiteral("X1�ᣨCCD�ᣩ�ŷ��������"));
	pBitOutS[0][3]->setText(QStringLiteral("X1�ᣨCCD�ᣩ�ŷ�����"));
								   
	pBitOutS[1][0]->setText(QStringLiteral("X2�ᣨ�����ᣩ�ŷ�ʹ��"));
	pBitOutS[1][1]->setText(QStringLiteral("X2�ᣨ�����ᣩ�ŷ�������λ"));
	pBitOutS[1][2]->setText(QStringLiteral("X2�ᣨ�����ᣩ�ŷ��������"));
	pBitOutS[1][3]->setText(QStringLiteral("X2�ᣨ�����ᣩ�ŷ�����"));
							  
	pBitOutS[2][0]->setText(QStringLiteral("X3�ᣨƤ���������ŷ�ʹ��"));
	pBitOutS[2][1]->setText(QStringLiteral("X3�ᣨƤ���������ŷ�������λ"));
	pBitOutS[2][2]->setText(QStringLiteral("X3�ᣨƤ���������ŷ��������"));
	pBitOutS[2][3]->setText(QStringLiteral("X3�ᣨƤ���������ŷ�����"));
							   
	pBitOutS[3][0]->setText(QStringLiteral("Y1�ᣨ�����ᣩ�ŷ�ʹ��"));
	pBitOutS[3][1]->setText(QStringLiteral("Y1�ᣨ�����ᣩ�ŷ�������λ"));
	pBitOutS[3][2]->setText(QStringLiteral("Y1�ᣨ�����ᣩ�ŷ��������"));
	pBitOutS[3][3]->setText(QStringLiteral("Y1�ᣨ�����ᣩ�ŷ�����"));
							   
	pBitOutS[4][0]->setText(QStringLiteral("Y2�ᣨ�������ᣩ�ŷ�ʹ��"));
	pBitOutS[4][1]->setText(QStringLiteral("Y2�ᣨ�������ᣩ�ŷ�������λ"));
	pBitOutS[4][2]->setText(QStringLiteral("Y2�ᣨ�������ᣩ�ŷ��������"));
	pBitOutS[4][3]->setText(QStringLiteral("Y2�ᣨ�������ᣩ�ŷ�����"));


	//ͨ��IO ����
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

	pBitInM[0]->setText(QStringLiteral("����"));
	pBitInM[1]->setText(QStringLiteral("ֹͣ"));
	pBitInM[2]->setText(QStringLiteral("��λ"));
	pBitInM[3]->setText(QStringLiteral("��ͣ"));
	pBitInM[4]->setText(QStringLiteral("�˹�ȷ��"));
	pBitInM[5]->setText(QStringLiteral("����/����"));
	pBitInM[6]->setText(QStringLiteral("�������ϼ��1"));
	pBitInM[7]->setText(QStringLiteral("�������ϼ��2"));
	pBitInM[8]->setText(QStringLiteral("�������ϼ��3"));
	pBitInM[9]->setText(QStringLiteral("�������ϼ��4"));
	pBitInM[10]->setText(QStringLiteral("������ϼ��1"));
	pBitInM[11]->setText(QStringLiteral("������ϼ��2"));
	pBitInM[12]->setText(QStringLiteral("������ϼ��3"));
	pBitInM[13]->setText(QStringLiteral("������ϼ��4"));
	pBitInM[14]->setText(QStringLiteral("���ã�LTC0��"));
	pBitInM[15]->setText(QStringLiteral("���ã�LTC1��"));
	//
	pBitOutM[0]->setText(QStringLiteral("���"));
	pBitOutM[1]->setText(QStringLiteral("�Ƶ�"));
	pBitOutM[2]->setText(QStringLiteral("�̵�"));
	pBitOutM[3]->setText(QStringLiteral("������"));
	pBitOutM[4]->setText(QStringLiteral("OK������"));
	pBitOutM[5]->setText(QStringLiteral("NG������"));
	pBitOutM[6]->setText(QStringLiteral("�ξ���մ�"));
	pBitOutM[7]->setText(QStringLiteral("�������׽�"));
	pBitOutM[8]->setText(QStringLiteral("����������"));
	pBitOutM[9]->setText(QStringLiteral("���������½�"));
	pBitOutM[10]->setText(QStringLiteral("������������"));
	pBitOutM[11]->setText(QStringLiteral("�������1"));
	pBitOutM[12]->setText(QStringLiteral("����(CMP0)"));
	pBitOutM[13]->setText(QStringLiteral("����(CMP1)"));
	pBitOutM[14]->setText(QStringLiteral("����(CMP2)"));
	pBitOutM[15]->setText(QStringLiteral("����(CMP3)"));




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
	
	pBitInG[0][0]->setText(QStringLiteral("�ξ����޷źü��"));			
	pBitInG[0][1]->setText(QStringLiteral("�������׽�")); 		    
	pBitInG[0][2]->setText(QStringLiteral("����������"));			
	pBitInG[0][3]->setText(QStringLiteral("���������½�"));			
	pBitInG[0][4]->setText(QStringLiteral("������������"));			
	pBitInG[0][5]->setText(QStringLiteral("������ϼ��5"));			
	pBitInG[0][6]->setText(QStringLiteral("������ϼ��6"));			
	pBitInG[0][7]->setText(QStringLiteral("������ϼ��7"));			
	pBitInG[0][8]->setText(QStringLiteral("������ϼ��8"));			
	pBitInG[0][9]->setText(QStringLiteral("�̶����׼н�"));			
	pBitInG[0][10]->setText(QStringLiteral("�̶������ɿ�"));			
	pBitInG[0][11]->setText(QStringLiteral("1#���������½�"));		
	pBitInG[0][12]->setText(QStringLiteral("1#������������"));		
	pBitInG[0][13]->setText(QStringLiteral("������ϼ��9"));			
	pBitInG[0][14]->setText(QStringLiteral("������ϼ��10"));		
	pBitInG[0][15]->setText(QStringLiteral("������ϼ��11"));		
	//
	pBitInG[1][0]->setText(QStringLiteral("������ϼ��12"));
	pBitInG[1][1]->setText(QStringLiteral("2.1#���������½�"));
	pBitInG[1][2]->setText(QStringLiteral("2.1#������������"));
	pBitInG[1][3]->setText(QStringLiteral("2.2#���������½�"));
	pBitInG[1][4]->setText(QStringLiteral("2.2#������������"));
	pBitInG[1][5]->setText(QStringLiteral("2.3#���������½�"));
	pBitInG[1][6]->setText(QStringLiteral("2.3#������������"));
	pBitInG[1][7]->setText(QStringLiteral("2.4#���������½�"));
	pBitInG[1][8]->setText(QStringLiteral("2.4#������������"));
	pBitInG[1][9]->setText(QStringLiteral("2#�̶����׼н�"));
	pBitInG[1][10]->setText(QStringLiteral("2#�̶������ɿ�"));
	pBitInG[1][11]->setText(QStringLiteral("1#NG��������½�"));
	pBitInG[1][12]->setText(QStringLiteral("1#NG�����������"));
	pBitInG[1][13]->setText(QStringLiteral("2#NG��������½�"));
	pBitInG[1][14]->setText(QStringLiteral("2#NG�����������"));
	pBitInG[1][15]->setText(QStringLiteral("3#NG��������½�"));
	//
	pBitInG[1][16]->setText(QStringLiteral("3#NG�����������"));
	pBitInG[2][17]->setText(QStringLiteral("�赲�����½�"));
	pBitInG[2][18]->setText(QStringLiteral("�赲��������"));
	pBitInG[2][19]->setText(QStringLiteral("OK�� ���϶���"));
	pBitInG[2][20]->setText(QStringLiteral("OK�� ���϶���1"));
	pBitInG[2][21]->setText(QStringLiteral("OK�� ���϶���2"));
	pBitInG[2][22]->setText(QStringLiteral("NG�� ���Ϸ���1"));
	pBitInG[2][23]->setText(QStringLiteral("����"));
	pBitInG[2][24]->setText(QStringLiteral("����"));
	pBitInG[2][10]->setText(QStringLiteral("NG�� �赲λ���϶���"));
	pBitInG[2][11]->setText(QStringLiteral("NG�� ���϶���"));
	pBitInG[2][12]->setText(QStringLiteral("NG���Ϻм��1"));
	pBitInG[2][13]->setText(QStringLiteral("NG���Ϻм��2"));
	pBitInG[2][14]->setText(QStringLiteral("��դ"));
	pBitInG[2][15]->setText(QStringLiteral("�Ŵ�"));
	pBitInG[2][16]->setText(QStringLiteral("����"));

	//pBitOutG			
	pBitOutG[0][0]->setText(QStringLiteral("����ƴ�1"));
	pBitOutG[0][1]->setText(QStringLiteral("�������2"));
	pBitOutG[0][2]->setText(QStringLiteral("����ƴ�2"));
	pBitOutG[0][3]->setText(QStringLiteral("�������3"));
	pBitOutG[0][4]->setText(QStringLiteral("����ƴ�3"));
	pBitOutG[0][5]->setText(QStringLiteral("�������4"));
	pBitOutG[0][6]->setText(QStringLiteral("����ƴ�4"));
	pBitOutG[0][7]->setText(QStringLiteral("�̶����׼н�"));
	pBitOutG[0][8]->setText(QStringLiteral("�̶������ɿ�"));
	pBitOutG[0][9]->setText(QStringLiteral("1#���������½�"));
	pBitOutG[0][10]->setText(QStringLiteral("1#������������"));
	pBitOutG[0][11]->setText(QStringLiteral("�������5"));
	pBitOutG[0][12]->setText(QStringLiteral("����ƴ�5"));
	pBitOutG[0][13]->setText(QStringLiteral("�������6"));
	pBitOutG[0][14]->setText(QStringLiteral("����ƴ�6"));
	pBitOutG[0][15]->setText(QStringLiteral("�������7"));
	//																	
	pBitOutG[1][0]->setText(QStringLiteral("����ƴ�7"));
	pBitOutG[1][1]->setText(QStringLiteral("�������8"));
	pBitOutG[1][2]->setText(QStringLiteral("����ƴ�8"));
	pBitOutG[1][3]->setText(QStringLiteral("2#�̶����׼н�"));
	pBitOutG[1][4]->setText(QStringLiteral("2#�̶������ɿ�"));
	pBitOutG[1][5]->setText(QStringLiteral("2.1#���������½�"));
	pBitOutG[1][6]->setText(QStringLiteral("2.1#������������"));
	pBitOutG[1][7]->setText(QStringLiteral("2.2#���������½�"));
	pBitOutG[1][8]->setText(QStringLiteral("2.2#������������"));
	pBitOutG[1][9]->setText(QStringLiteral("2.3#���������½�"));
	pBitOutG[1][10]->setText(QStringLiteral("2.3#������������"));
	pBitOutG[1][11]->setText(QStringLiteral("2.3#���������½�"));
	pBitOutG[1][12]->setText(QStringLiteral("2.3#������������"));
	pBitOutG[1][13]->setText(QStringLiteral("2.4#���������½�"));
	pBitOutG[1][14]->setText(QStringLiteral("2.4#������������"));
	pBitOutG[1][15]->setText(QStringLiteral("�������10"));
	//
	pBitOutG[2][0]->setText(QStringLiteral("����ƴ�10"));
	pBitOutG[2][1]->setText(QStringLiteral("�������11"));
	pBitOutG[2][2]->setText(QStringLiteral("����ƴ�11"));
	pBitOutG[2][3]->setText(QStringLiteral("�������12"));
	pBitOutG[2][4]->setText(QStringLiteral("����ƴ�12"));
	pBitOutG[2][5]->setText(QStringLiteral("1#NG��������½�"));
	pBitOutG[2][6]->setText(QStringLiteral("1#NG�����������"));
	pBitOutG[2][7]->setText(QStringLiteral("2#NG��������½�"));
	pBitOutG[2][8]->setText(QStringLiteral("2#NG�����������"));
	pBitOutG[2][9]->setText(QStringLiteral("����"));
	pBitOutG[2][10]->setText(QStringLiteral("����"));
	pBitOutG[2][11]->setText(QStringLiteral("�赲�����½�"));
	pBitOutG[2][12]->setText(QStringLiteral("�赲��������"));
	pBitOutG[2][13]->setText(QStringLiteral("����"));
	pBitOutG[2][14]->setText(QStringLiteral("����"));
	pBitOutG[2][15]->setText(QStringLiteral("����"));
	
}

void HSMeasure::onIoTimer()
{
	//����ͨ��IO
	for (size_t i = 0; i < IO_BIT_NUM; i++)
	{
		ioMutex.lock();
		dmc_read_inbit(mCardNo, i) ? pBitInM[i]->setChecked(false) : pBitInM[i]->setChecked(true);
		dmc_read_outbit(mCardNo, i) ? pBitOutM[i]->setChecked(false) : pBitOutM[i]->setChecked(true);
		ioMutex.unlock();
	}

	//��չģ��IO
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



