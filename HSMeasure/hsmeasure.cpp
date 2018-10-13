#include "hsmeasure.h"
#include "bcdh_gap.h"
#include "bcdh_step.h"

QString mPath;
BCDH_gap* mpBCDH_gap;
BCDH_step* mpBCDH_step
;
HSMeasure::HSMeasure(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setWindowTitle(QStringLiteral("大族激光IT装备DH测量软件 v1.0.0"));
	init();
	initValue();
	load_ini();
	//
	mPath = QCoreApplication::applicationDirPath();

	initDmcCard();
	hsmeasure_para();

	//
	pIoTimer = new QTimer();
	connect(pIoTimer, SIGNAL(timeout()), this, SLOT(onIoTimer()));
	pIoTimer->start(300);

	//
}

HSMeasure::~HSMeasure()
{
	delete mpTcpClientCcd[0];
	delete mpTcpClientCcd[1];

	delete mpSerialportCcd[0];
	delete mpSerialportCcd[1];

	delete mpDMC5000Lib;
}

void HSMeasure::init()
{
	this->setFixedSize(985, 650);
	ui.tabWidget->setTabText(0, QStringLiteral("主界面"));
	ui.tabWidget->setTabText(1, QStringLiteral("系统参数"));
	ui.tabWidget->setTabText(2, QStringLiteral("I/O监控"));
	ui.tabWidget->setCurrentIndex(0);
	ui.tabWidget->setStyleSheet("QTabWidget:pane{border-top:0px solid #e8f3f9;background:#e8f3f9;}"); 
	
	//默认启用4个槽位
	ui.checkBox_cave1->setCheckState(Qt::Checked);
	ui.checkBox_cave2->setCheckState(Qt::Checked);
	ui.checkBox_cave3->setCheckState(Qt::Checked);
	ui.checkBox_cave4->setCheckState(Qt::Checked);

	//默认产品国别 
	ui.comboBoxProduct->addItem(QStringLiteral("美国"));
	ui.comboBoxProduct->addItem(QStringLiteral("中国"));
	ui.comboBoxProduct->addItem(QStringLiteral("英国"));

	//产线编号  1....20
	ui.comboBoxLine->addItem(QStringLiteral("Line_1"));
	ui.comboBoxLine->addItem(QStringLiteral("Line_2"));
	ui.comboBoxLine->addItem(QStringLiteral("Line_3"));


	//
	//默认权限 操作员
	ui.comboBoxPower->addItem(QStringLiteral("操作员"));
	ui.comboBoxPower->addItem(QStringLiteral("技术员"));
	ui.comboBoxPower->addItem(QStringLiteral("工程师"));
	
	
	//table 1  此规格OK产品对应的上、下限值
	QStandardItemModel *model_product = new QStandardItemModel(this);
	model_product->setColumnCount(3);
	model_product->setHeaderData(0, Qt::Horizontal, QStringLiteral("检测项目"));
	model_product->setHeaderData(1, Qt::Horizontal, QStringLiteral("上限值"));
	model_product->setHeaderData(2, Qt::Horizontal, QStringLiteral("下限值"));
	ui.productTableView->setModel(model_product);
	ui.productTableView->setColumnWidth(0, 119);
	ui.productTableView->setColumnWidth(1, 120);
	ui.productTableView->setColumnWidth(2, 120);
	model_product->setItem(0, 0, new QStandardItem(QStringLiteral("缝隙（Gap）")));
	model_product->setItem(1, 0, new QStandardItem(QStringLiteral("段差（Step）")));
	model_product->setItem(0, 1, new QStandardItem(QStringLiteral("0.080mm")));
	model_product->setItem(1, 1, new QStandardItem(QStringLiteral("0.110mm")));
	model_product->setItem(0, 2, new QStandardItem(QStringLiteral("0.000mm")));
	model_product->setItem(1, 2, new QStandardItem(QStringLiteral("-0.050mm")));
	ui.productTableView->verticalHeader()->hide();

	//table 2  检测值
	model_result = new QStandardItemModel(this);
	model_result->setColumnCount(20);
	model_result->setHeaderData(0, Qt::Horizontal, QStringLiteral("Gap_A"));
	model_result->setHeaderData(1, Qt::Horizontal, QStringLiteral("Gap_B"));
	model_result->setHeaderData(2, Qt::Horizontal, QStringLiteral("Gap_C"));
	model_result->setHeaderData(3, Qt::Horizontal, QStringLiteral("Gap_D"));
	model_result->setHeaderData(4, Qt::Horizontal, QStringLiteral("Gap_E"));
	model_result->setHeaderData(5, Qt::Horizontal, QStringLiteral("Gap_F"));
	model_result->setHeaderData(6, Qt::Horizontal, QStringLiteral("Gap_G"));
	model_result->setHeaderData(7, Qt::Horizontal, QStringLiteral("Gap_H"));
	model_result->setHeaderData(8, Qt::Horizontal, QStringLiteral("Gap_I"));
	model_result->setHeaderData(9, Qt::Horizontal, QStringLiteral("Gap_J"));

	model_result->setHeaderData(10, Qt::Horizontal, QStringLiteral("Step_A"));
	model_result->setHeaderData(11, Qt::Horizontal, QStringLiteral("Step_B"));
	model_result->setHeaderData(12, Qt::Horizontal, QStringLiteral("Step_C"));
	model_result->setHeaderData(13, Qt::Horizontal, QStringLiteral("Step_D"));
	model_result->setHeaderData(14, Qt::Horizontal, QStringLiteral("Step_E"));
	model_result->setHeaderData(15, Qt::Horizontal, QStringLiteral("Step_F"));
	model_result->setHeaderData(16, Qt::Horizontal, QStringLiteral("Step_G"));
	model_result->setHeaderData(17, Qt::Horizontal, QStringLiteral("Step_H"));
	model_result->setHeaderData(18, Qt::Horizontal, QStringLiteral("Step_I"));
	model_result->setHeaderData(19, Qt::Horizontal, QStringLiteral("Step_J"));
	ui.tableViewResult->setModel(model_result);
	ui.tableViewResult->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.tableViewResult->verticalHeader()->setDefaultSectionSize(18);
	ui.tableViewResult->setAlternatingRowColors(true); 
	ui.tableViewResult->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可编辑
	
	/*
	QString para = "0.0823";
	model_result->setItem(0, 1, new QStandardItem(para));
	model_result->setItem(1, 1, new QStandardItem(para));
	model_result->setItem(2, 1, new QStandardItem(para));
	model_result->setItem(3, 1, new QStandardItem(para));
	model_result->setItem(4, 1, new QStandardItem(para));
	model_result->setItem(5, 1, new QStandardItem(para));
	model_result->setItem(6, 1, new QStandardItem(para));
	model_result->setItem(7, 1, new QStandardItem(para));
	*/

	connect(ui.pushButtonSaveLog, SIGNAL(clicked()), this, SLOT(save_log()));
	connect(ui.pushButtonStart, SIGNAL(clicked()), this, SLOT(hs_start()));
	connect(ui.pushButtonPause, SIGNAL(clicked()), this, SLOT(hs_pause()));
	connect(ui.pushButtonZero, SIGNAL(clicked()), this, SLOT(hs_zero()));
	connect(ui.pushButtonStop, SIGNAL(clicked()), this, SLOT(hs_stop()));
	
	show_msg("the program is running!");

	//
	for (size_t i = 0; i <= SlotposNo4; i++)
	{
		flagSlot[i] = true;
	}

	connect(ui.checkBox_cave1, &QCheckBox::clicked, [this]()
	{
		if (true == ui.checkBox_cave1->isChecked())
		{
			flagSlot[0] = true;
		}
		else
		{
			flagSlot[0] = false;
		}
	});
	
	connect(ui.checkBox_cave2, &QCheckBox::clicked, [this]()
	{
		if (true == ui.checkBox_cave2->isChecked())
		{
			flagSlot[1] = true;
		}
		else
		{
			flagSlot[1] = false;
		}
	});

	connect(ui.checkBox_cave3, &QCheckBox::clicked, [this]()
	{
		if (true == ui.checkBox_cave3->isChecked())
		{
			flagSlot[2] = true;
		}
		else
		{
			flagSlot[2] = false;
		}
	});

	connect(ui.checkBox_cave4, &QCheckBox::clicked, [this]()
	{
		if (true == ui.checkBox_cave4->isChecked())
		{
			flagSlot[3] = true;
		}
		else
		{
			flagSlot[3] = false;
		}
	});

//	qRegisterMetaType<STATE_FLOW>("STATE_FLOW");
//	connect(this, SIGNAL(stateSignal(STATE_FLOW)), this, SLOT(stateSlot(STATE_FLOW)));


//	ui.pushButtonStart->setEnabled(false);
	ui.pushButtonStop->setEnabled(false);
	ui.pushButtonPause->setEnabled(false);

	ui.pushButtonZero->setEnabled(true);	
}

void HSMeasure::initDmcCard()
{

	
	if (false == mpMOTIONLib->initial())
	{
		qDebug() << QStringLiteral("没有找到运动控制卡，或者控制卡异常 \n");
		QMessageBox::warning(this, "", QStringLiteral("运动控制卡初始化失败，请检查后重启!"));
		return;
	}

	qDebug() << QStringLiteral("找到%1张运动控制卡 \n").arg(mpMOTIONLib->mCardNum);

	mCardNo = mpMOTIONLib->mInitialPara.CardIdList[0];


	
	for(int Axis = 0; Axis < AXIS_NUM; Axis++)
	{
		dmc_write_sevon_pin(mCardNo, Axis, 0);
	}

	for (int Axis = 0; Axis < AXIS_NUM; Axis++)
	{
		dmc_set_pulse_outmode(mCardNo, Axis, 1);
	}

	for (int Axis = 0; Axis < AXIS_NUM; Axis++)
	{
		dmc_set_el_mode(mCardNo, Axis, 2, 3, 0);
	}


	int Org_logic = 1;   
	int Filter = 0;       
	int Home_dir = 1;   
	int Vel_mode = 0;	
	int Mode = 2;        
	int EZ_count = 0; 

	for (int Axis = 0; Axis < AXIS_NUM; Axis++)
	{
		dmc_set_home_pin_logic(mCardNo, Axis, Org_logic, Filter);
		dmc_set_homemode(mCardNo, Axis, Home_dir, Vel_mode, Mode, EZ_count);
	}
	
	for (int Axis = 0; Axis < AXIS_NUM; Axis++)
	{
		dmc_set_s_profile(mCardNo, Axis, 0, 0.01);
	}

	for (int Axis = 0; Axis < AXIS_NUM; Axis++)
	{
		dmc_set_axis_io_map(mCardNo, Axis, 3, 3, 0, 1);
		dmc_set_emg_mode(mCardNo, Axis, 1, 0);
	}

	
	for (int Axis = 0; Axis < AXIS_NUM; Axis++)
	{
		mpDMC5000Lib->mpDmcAxis[0][Axis]->setMovePara(5000,10000);
	}
	
}

void HSMeasure::initValue()
{
	

	mCurState = NULL_MEASURE;;

	for (size_t i = 0; i <= SlotposNo4; i++)
	{
		
		for (size_t j = 0; j <= GapJ; j++)
		{
			fixValue.plug[i].gap[j] = 0;
		}

		for (size_t j = 0; j <= StepJ; j++)
		{
			fixValue.plug[i].step[j] = 0;
		}
	}
}

void HSMeasure::hs_start()  //启动button clicked
{
	//
	WORK_THREAD* pWorkThread = new WORK_THREAD(this);
	QTimer::singleShot(0, pWorkThread, SLOT(run()));

	this->pWorkThread = pWorkThread;

	for (int Axis = 0; Axis < AXIS_NUM; Axis++)
	{
		mpMOTIONLib->mpDmcAxis[mCardNo][Axis]->mpMutex = &pWorkThread->mMutex;
	}
	

	mpBCDH_gap = new BCDH_gap(this);
	mpBCDH_step = new BCDH_step(this);


	ui.pushButtonStart->setEnabled(false);
	ui.pushButtonZero->setEnabled(false);
	
	ui.pushButtonPause->setEnabled(true);
	ui.pushButtonStop->setEnabled(true);

	/*
	_COMMUNICATECLASS::COM_PORT_ONE testCom(1, QSerialPort::BaudRate::Baud9600);

	auto bb = testCom.open();
	auto aa = testCom.send("1234");

	auto re = testCom.getRec();
	*/
}

void HSMeasure::hs_pause()  //暂停button clicked
{
	static bool flag = false;

	flag = !flag;

	if (flag)
	{
		mbPause = true;
		pWorkThread->mMutex.lock();
		ui.pushButtonPause->setStyleSheet("QPushButton{background:yellow}");
		ui.pushButtonPause->setText(QStringLiteral("继续"));
	}
	else
	{
		mbPause = false;
		pWorkThread->mMutex.unlock();
		ui.pushButtonPause->setStyleSheet("QPushButton{background:}");
		ui.pushButtonPause->setText(QStringLiteral("暂停"));	
	}
	
}

void HSMeasure::hs_stop()
{
	mbStop = true;
	ui.pushButtonStop->setEnabled(false);
	ui.pushButtonStop->setStyleSheet("QPushButton{background:red}");
	pWorkThread->mMutex.lock();
	mpDMC5000Lib->emgStop();
}

void HSMeasure::hs_zero()  //回原button clicked
{
	ui.pushButtonStop->setEnabled(true);

	if (false == go_home())
	{

		ui.pushButtonZero->setStyleSheet("QPushButton{background:red}");
		return;
	}

	ui.pushButtonZero->setStyleSheet("QPushButton{background:green}");
	ui.pushButtonStart->setEnabled(true);
}


void WORK_THREAD::run()  //运动流程
{

	while (true)
	{

		if (true == this->mpHSMeasure->mbStop)
		{
			break;
		}

		WORD portno = 0;

		DWORD ioStates = dmc_read_inport(this->mpHSMeasure->mCardNo, portno);

		QSettings settings(mPath +"/cfg/config.ini", QSettings::IniFormat);
		int bStart = settings.value("DEBUG/BSTART").toInt();

		if ((1 == ioStates && 1 == ioStates) || 1 == bStart)
		{
			settings.setValue("DEBUG/BSTART","0");
			
			this->mpHSMeasure->mCurState = GAP_MEASURE;			
		}
		
		switch (this->mpHSMeasure->mCurState)
		{
		case GAP_MEASURE:
			if (false == mpBCDH_gap->getGapValue())
			{
				return;
			}
			break;
		case STEP_MEASURE:
			mpBCDH_step->getStepValue();
			this->mpHSMeasure->saveValueToLog(this->mpHSMeasure->fixValue);
			this->mpHSMeasure->showValueToUi(this->mpHSMeasure->fixValue);
			break;
		default:
			break;

		}
		this->mpHSMeasure->mCurState++;

		if (this->mpHSMeasure->mCurState > FINISH)
		{
			this->mpHSMeasure->mCurState = NULL_MEASURE;
		}

		qDebug() << "runing...";
		QApplication::processEvents();
		Sleep(1000);
	}

}

bool HSMeasure::go_home()  //回原流程
{
	
	for (int Axis = 0; Axis < AXIS_NUM; Axis++)
	{
		mpDMC5000Lib->stop(mCardNo, Axis, _MOTIONCLASS::DMC5000Lib::STOP_MODE_EMG);
	}

	for (int Axis = 0; Axis < AXIS_NUM; Axis++)
	{
		dmc_set_position(mCardNo, Axis, 0);
	}

	for (int Axis = 0; Axis < AXIS_NUM; Axis++)
	{
		dmc_write_sevon_pin(mCardNo, Axis, DMC_SERV_ENABLE);
	}


	int homeStep = 0;
	bool bReturn;

	while (true)
	{
		ui.pushButtonZero ->setStyleSheet("QPushButton{background:yellow}");

		if (mbPause)
		{
			continue;
		}

		if (mbStop)
		{
			return false;
		}

		DWORD tSystemTime = GetTickCount();

		switch (homeStep)
		{
		case 0:
			bReturn = mpDMC5000Lib->mpDmcAxis[mCardNo][ccdAxisNo]->home(); if (!bReturn) { return false; }
			bReturn = mpDMC5000Lib->mpDmcAxis[mCardNo][laserAxisNo]->home(); if (!bReturn) { return false; }
			
			bReturn = mpDMC5000Lib->mpDmcAxis[mCardNo][laserAxisNo]->checkHome(tSystemTime, DEFAUL_HOME_TIME_OUT); if (!bReturn) { return false; }

			break;
		case 1:
			bReturn = mpDMC5000Lib->mpDmcAxis[mCardNo][laserAxisNo]->moveAndCheckdone(LaserSafePosition, DEFAUL_HOME_TIME_OUT); if (!bReturn) { return false; }

			break;
		case 2:
			bReturn = mpDMC5000Lib->mpDmcAxis[mCardNo][ccdAxisNo]->checkHome(tSystemTime, DEFAUL_HOME_TIME_OUT); if (!bReturn) { return false; }
			break;
		case 3:
			bReturn = mpDMC5000Lib->mpDmcAxis[mCardNo][platformAxisNo]->home(); if (!bReturn) { return false; }
			bReturn = mpDMC5000Lib->mpDmcAxis[mCardNo][platformAxisNo]->checkHome(tSystemTime, DEFAUL_HOME_TIME_OUT); if (!bReturn) { return false; }
			break;

		case 4:
			bReturn = mpDMC5000Lib->mpDmcAxis[mCardNo][platformAxisNo]->moveAndCheckdone(LaserSafePosition, DEFAUL_HOME_TIME_OUT); if (!bReturn) { return false; }

		case 5:
			break;
		case 6:
			/*
			for (int InPut = 0; InPut < 10; InPut++)
			{
				if (dmc_read_inbit(mCardNo, InPut))
				{
					QMessageBox::warning(this, "", QString("Input %1 failed").arg(InPut));
				}
			}
			*/
			break;
		default:
			break;
		}
		
		homeStep++;

		if (homeStep > 6)
		{
			return true;
		}

		Sleep(300);
	}
}

void HSMeasure::show_msg(const QString &msg)
{
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_time = current_date_time.toString("yyyy-MM-dd HH:mm:ss:zzz");
	ui.plainTextEditShowMsg->appendPlainText("["+ current_time +"] " + msg);
	qDebug() << msg;
}

void HSMeasure::load_ini()
{
	QString fileName;
	fileName = QCoreApplication::applicationDirPath();
	fileName += "/cfg/config.ini";

	QSettings settings(fileName, QSettings::IniFormat);
	QString str_path = settings.value("path_name/path").toString();

	ccdAxisNo = settings.value("AXIS_NO_CFG/ccdAxisNo").toInt();
	laserAxisNo = settings.value("AXIS_NO_CFG/laserAxisNo").toInt();
	platformAxisNo = settings.value("AXIS_NO_CFG/platformAxisNo").toInt();
	rotateAxisNo = settings.value("AXIS_NO_CFG/rotateAxisNo").toInt();
	unloadAxisNo = settings.value("AXIS_NO_CFG/unloadAxisNo").toInt();

	LaserSafePosition = settings.value("POSITION/LaserSafePosition").toInt();
	FeedingPosition = settings.value("POSITION/FeedingPosition").toInt();
}

void HSMeasure::save_ini()
{
	QString fileName;
	fileName = QCoreApplication::applicationDirPath();
	fileName += "/cfg/config.ini";

	QSettings settings(fileName, QSettings::IniFormat);
	QString str_path = settings.value("path_name/path").toString();

	settings.setValue("CCD_CFG/IP1", ui.lineEditCcdIp1->text());
	settings.setValue("CCD_CFG/IP2", ui.lineEditCcdIp2->text());
	settings.setValue("CCD_CFG/UART1", ui.lineEditCcdUart1->text());
	settings.setValue("CCD_CFG/UART2", ui.lineEditCcdUart2->text());

	settings.setValue("LASER_CFG/IP1", ui.lineEditLaserIp1->text());
	settings.setValue("LASER_CFG/IP2", ui.lineEditLaserIp2->text());
	settings.setValue("LASER_CFG/IP3", ui.lineEditLaserIp3->text());
}

void HSMeasure::save_log()
{
	QString path = create_file_path("D:/qdir/", "log.txt");  //file name = dir_ + nowtime + file_
	write_log_to_txt(path);
}

QString HSMeasure::create_file_path(const QString & dir_, const QString & file_)
{
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_time = current_date_time.toString("yyyyMMdd_HHmmss_");
	QString fileName;
	fileName = dir_;
	if (fileName.endsWith("/"))
	{ 
	}
	else
	{
		fileName = fileName.append("/");
	}

	QDir dir(fileName);
	if (dir.exists())
	{
	}
	else
	{
		dir.mkpath(fileName);
	}

	fileName += current_time + file_;
	return fileName;	
}

void HSMeasure::write_log_to_txt(const QString &file_name)
{
	QFile file(file_name);//文件命名  
	if (!file.open(QFile::WriteOnly | QFile::Text))     //检测文件是否打开  
	{
		QMessageBox::information(this, "Error Message", "Please close the Text File!");
		return;
	}
	QTextStream out(&file);   
	m_mutex.lock();
	out << ui.plainTextEditShowMsg->toPlainText();   
	ui.plainTextEditShowMsg->setPlainText("");  //清空msg
	m_mutex.unlock();
	show_msg(QStringLiteral("log保存路径：") + file_name);
	file.close();
}

void HSMeasure::closeEvent(QCloseEvent *event)
{

	save_ini();



	dmc_board_close();
}
