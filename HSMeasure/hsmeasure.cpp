#include "hsmeasure.h"
#include "bcdh_gap.h"
#include "bcdh_step.h"


QString mPath;
BCDH_gap* mpBCDH_gap;
BCDH_step* mpBCDH_step;

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
	login();
	hsmeasure_para();

	//
	pIoTimer = new QTimer();
	connect(pIoTimer, SIGNAL(timeout()), this, SLOT(onIoTimer()));
	pIoTimer->start(300);

	//
	mpBCDH_gap = new BCDH_gap(this);
	mpBCDH_step = new BCDH_step(this);

	
}

HSMeasure::~HSMeasure()
{

	delete mpBCDH_gap;
	delete mpBCDH_step;

	delete mpDMC5000Lib;
}

void HSMeasure::init()
{
//	this->setFixedSize(985, 650);
	ui.tabWidget->resize(1085, 640);
	ui.tabWidget->setTabText(0, QStringLiteral("主界面"));
	ui.tabWidget->setTabText(1, QStringLiteral("系统参数"));
	ui.tabWidget->setTabText(2, QStringLiteral("I/O主板"));
	ui.tabWidget->setTabText(3, QStringLiteral("I/O扩展"));
	ui.tabWidget->setCurrentIndex(0);
	ui.tabWidget->setStyleSheet("QTabWidget:pane{border-top:0px solid #e8f3f9;background:#e8f3f9;}");
	ui.tabWidget->setTabEnabled(0, true);
//	ui.tabWidget->setTabEnabled(1, false);
//	ui.tabWidget->setTabEnabled(2, false);
	connect(ui.tabWidget, SIGNAL(currentChanged(int)), this, SLOT(onTabWidgetChanged(int)));

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

	//io
	initUiIo();
	
	//
	connect(ui.pushButtonSaveLog, SIGNAL(clicked()), this, SLOT(save_log()));
	connect(ui.pushButtonStart, SIGNAL(clicked()), this, SLOT(hs_start()));
	connect(ui.pushButtonPause, SIGNAL(clicked()), this, SLOT(hs_pause()));
	connect(ui.pushButtonZero, SIGNAL(clicked()), this, SLOT(hs_zero()));
	connect(ui.pushButtonStop, SIGNAL(clicked()), this, SLOT(hs_stop()));
	connect(ui.pushButtonRestAlm, SIGNAL(clicked()), this, SLOT(hs_RestAlm()));
	connect(ui.pushButtonMute, SIGNAL(clicked()), this, SLOT(hs_Mute()));

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

	//
	for (int i = 0; i < 6; i++)
	{
		m_laserEditor[i] = new laserEdit(this);
		m_laserEditor[i]->set_x_label(QString("laser%1").arg(i + 1));
	}

	m_mainLayout = new QVBoxLayout(this);
	m_topLayout = new QHBoxLayout(this);
	m_midLayout = new QHBoxLayout(this);
	m_bottomMainLayout = new QVBoxLayout(this);
	m_midBarLayout = new QHBoxLayout(this);
	m_midBar2Layout = new QHBoxLayout(this);
	m_midTopLayout = new QHBoxLayout(this);
	m_midBotLayout = new QHBoxLayout(this);

	m_mainLayout->addLayout(m_topLayout);
	m_mainLayout->addLayout(m_midLayout);
	m_mainLayout->addLayout(m_bottomMainLayout);
	m_mainLayout->setSpacing(5);
	m_mainLayout->setContentsMargins(5, 5, 5, 5);

	m_topLayout->addWidget(m_laserEditor[0]);
	m_topLayout->addWidget(m_laserEditor[1]);
	m_topLayout->addWidget(m_laserEditor[2]);
	m_midLayout->addWidget(m_laserEditor[5]);
	m_midLayout->addWidget(m_laserEditor[4]);
	m_midLayout->addWidget(m_laserEditor[3]);

	m_bottomMainLayout->addLayout(m_midBarLayout);
	m_bottomMainLayout->addLayout(m_midBar2Layout);
	m_bottomMainLayout->addLayout(m_midTopLayout);
	m_bottomMainLayout->addLayout(m_midBotLayout);
	m_bottomMainLayout->setContentsMargins(5, 10, 10, 55);

	m_laserName[0] = new QLabel(QStringLiteral("#1检测位："), this);
	m_laserName[1] = new QLabel(QStringLiteral(" #2检测位："), this);
	m_laserName[2] = new QLabel(QStringLiteral(" #3检测位："), this);
	m_laserName[3] = new QLabel(QStringLiteral(" #4检测位："), this);
	m_laserName[4] = new QLabel(QStringLiteral(" #5检测位："), this);
	m_laserName[5] = new QLabel(QStringLiteral("#6检测位："), this);

	m_laserTrig[0] = new QCheckBox(QStringLiteral("Laser1实时显示"), this);
	m_laserTrig[1] = new QCheckBox(QStringLiteral("Laser2实时显示"), this);
	m_laserTrig[2] = new QCheckBox(QStringLiteral("Laser3实时显示"), this);
	m_laserTrig[3] = new QCheckBox(QStringLiteral("Laser4实时显示"), this);
	m_laserTrig[4] = new QCheckBox(QStringLiteral("Laser5实时显示"), this);
	m_laserTrig[5] = new QCheckBox(QStringLiteral("Laser6实时显示"), this);

	m_laseWave[0][0] = new QLineEdit(this);//#1
	m_laseWave[0][1] = new QLineEdit(this);
	m_laseWave[0][2] = new QLineEdit(this);
	m_laseWave[0][3] = new QLineEdit(this);

	m_laseWave[1][0] = new QLineEdit(this);//#2
	m_laseWave[1][1] = new QLineEdit(this);
	m_laseWave[1][2] = new QLineEdit(this);
	m_laseWave[1][3] = new QLineEdit(this);

	m_laseWave[2][0] = new QLineEdit(this);//#3
	m_laseWave[2][1] = new QLineEdit(this);
	m_laseWave[2][2] = new QLineEdit(this);
	m_laseWave[2][3] = new QLineEdit(this);

	m_laseWave[3][0] = new QLineEdit(this);//#4
	m_laseWave[3][1] = new QLineEdit(this);
	m_laseWave[3][2] = new QLineEdit(this);
	m_laseWave[3][3] = new QLineEdit(this);

	m_laseWave[4][0] = new QLineEdit(this);//#5
	m_laseWave[4][1] = new QLineEdit(this);
	m_laseWave[4][2] = new QLineEdit(this);
	m_laseWave[4][3] = new QLineEdit(this);

	m_laseWave[5][0] = new QLineEdit(this);//#6
	m_laseWave[5][1] = new QLineEdit(this);
	m_laseWave[5][2] = new QLineEdit(this);
	m_laseWave[5][3] = new QLineEdit(this);

	for (int i = 0; i < 6; i++)
	{
		m_laserTrig[i]->setObjectName("checkBox");
		for (int j = 0; j < 4; j++)
			m_laseWave[i][j]->setObjectName("lineEdit");
	}

	m_midBarLayout->addWidget(m_laserTrig[0]);
	m_midBarLayout->addWidget(m_laserTrig[1]);
	m_midBarLayout->addWidget(m_laserTrig[2]);
	m_midBar2Layout->addWidget(m_laserTrig[5]);
	m_midBar2Layout->addWidget(m_laserTrig[4]);
	m_midBar2Layout->addWidget(m_laserTrig[3]);

	//laser #1 lineEdit
	m_midTopLayout->addWidget(m_laserName[0]);
	m_midTopLayout->addWidget(m_laseWave[0][0]);
	m_midTopLayout->addWidget(m_laseWave[0][1]);
	m_midTopLayout->addWidget(new QLabel(QStringLiteral("^"), this));
	m_midTopLayout->addWidget(m_laseWave[0][2]);
	m_midTopLayout->addWidget(m_laseWave[0][3]);

	//laser #2 lineEdit
	m_midTopLayout->addWidget(m_laserName[1]);
	m_midTopLayout->addWidget(m_laseWave[1][0]);
	m_midTopLayout->addWidget(m_laseWave[1][1]);
	m_midTopLayout->addWidget(new QLabel(QStringLiteral("^"), this));
	m_midTopLayout->addWidget(m_laseWave[1][2]);
	m_midTopLayout->addWidget(m_laseWave[1][3]);

	//laser #3 lineEdit
	m_midTopLayout->addWidget(m_laserName[2]);
	m_midTopLayout->addWidget(m_laseWave[2][0]);
	m_midTopLayout->addWidget(m_laseWave[2][1]);
	m_midTopLayout->addWidget(new QLabel(QStringLiteral("^"), this));
	m_midTopLayout->addWidget(m_laseWave[2][2]);
	m_midTopLayout->addWidget(m_laseWave[2][3]);

	// //laser #6 lineEdit
	m_midBotLayout->addWidget(m_laserName[5]);
	m_midBotLayout->addWidget(m_laseWave[5][0]);
	m_midBotLayout->addWidget(m_laseWave[5][1]);
	m_midBotLayout->addWidget(new QLabel(QStringLiteral("^"), this));
	m_midBotLayout->addWidget(m_laseWave[5][2]);
	m_midBotLayout->addWidget(m_laseWave[5][3]);

	//laser #5 lineEdit
	m_midBotLayout->addWidget(m_laserName[4]);
	m_midBotLayout->addWidget(m_laseWave[4][0]);
	m_midBotLayout->addWidget(m_laseWave[4][1]);
	m_midBotLayout->addWidget(new QLabel(QStringLiteral("^"), this));
	m_midBotLayout->addWidget(m_laseWave[4][2]);
	m_midBotLayout->addWidget(m_laseWave[4][3]);

	//laser #4 lineEdit
	m_midBotLayout->addWidget(m_laserName[3]);
	m_midBotLayout->addWidget(m_laseWave[3][0]);
	m_midBotLayout->addWidget(m_laseWave[3][1]);
	m_midBotLayout->addWidget(new QLabel(QStringLiteral("^"), this));
	m_midBotLayout->addWidget(m_laseWave[3][2]);
	m_midBotLayout->addWidget(m_laseWave[3][3]);

	QWidget* laserWidget = new QWidget(this);
	laserWidget->setObjectName("laserWidget");
	laserWidget->setLayout(m_mainLayout);

	ui.tabWidget->insertTab(2, laserWidget, QStringLiteral("激光参数"));  //激光参数tab页

	deal_raw_data();
	
//	ui.pushButtonStart->setEnabled(false);
	ui.pushButtonZero->setEnabled(true);	
}

void HSMeasure::onTabWidgetChanged(int tabIndex)
{
	int b = tabIndex;

	if (1 == tabIndex)
	{
		ui.lineEditJogSpeedCcd->setText(QString("%1").arg(speedFromIni.ccdJogSpeed));
		

	}
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
		mpDMC5000Lib->mpDmcAxis[0][Axis]->setMovePara();
	}
	
	//
	for (int Axis = 0; Axis < AXIS_NUM; Axis++)
	{
		mpMOTIONLib->mpDmcAxis[mCardNo][Axis]->mpStopFlag = &mbStop;
	}

	mpDMC5000Lib->mpDmcAxis[mCardNo][ccdAxisNo]->setMovePara(speedFromIni.ccdAutoSpeed, ABSOLUTE_MOTION);
	mpDMC5000Lib->mpDmcAxis[mCardNo][laserAxisNo]->setMovePara(speedFromIni.laserAutoSpeed, ABSOLUTE_MOTION);
	mpDMC5000Lib->mpDmcAxis[mCardNo][platformAxisNo]->setMovePara(speedFromIni.platAutoSpeed, ABSOLUTE_MOTION);

}

void HSMeasure::initValue()
{
	mbStop = false;

	for (size_t i = 0; i <= SlotposNo4; i++)
	{
		fixValue.plug[i].ok_ng = 0;

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

void HSMeasure::hs_pause()  //暂停button clicked
{
	static bool flag = false;

	flag = !flag;

	if (flag)
	{
		mbPause = true;

		ui.pushButtonPause->setStyleSheet("QPushButton{background:yellow}");
		ui.pushButtonPause->setText(QStringLiteral("继续"));
	}
	else
	{
		mbPause = false;
		
		ui.pushButtonPause->setStyleSheet("QPushButton{background:}");
		ui.pushButtonPause->setText(QStringLiteral("暂停"));	
	}
	
}

void HSMeasure::hs_stop()
{
	pWorkThread->mMutex.lock();
	mbStop = true;
	pWorkThread->mMutex.unlock();

	Sleep(1000);

	ui.pushButtonStart->setEnabled(true);
	ui.pushButtonZero->setEnabled(true);

	ui.pushButtonStart->setStyleSheet("QPushButton{background:}");
	ui.pushButtonStop->setStyleSheet("QPushButton{background:red}");
}

void HSMeasure::hs_RestAlm()
{
	sensorOut(RedLight, IO_OFF);
	sensorOut(YellowLight, IO_ON);
}

void HSMeasure::hs_Mute()
{
	sensorOut(Buzzer, IO_OFF);
}

bool HSMeasure::go_home()  //回原流程
{
	ui.pushButtonZero->setStyleSheet("QPushButton{background:}");
	ui.pushButtonStop->setStyleSheet("QPushButton{background:}");

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
		ui.pushButtonZero->setStyleSheet("QPushButton{background:yellow}");

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
			mpDMC5000Lib->mpDmcAxis[mCardNo][laserAxisNo]->setMovePara(speedFromIni.laserAutoSpeed, ABSOLUTE_MOTION);
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
			mpDMC5000Lib->mpDmcAxis[mCardNo][platformAxisNo]->setMovePara(speedFromIni.laserAutoSpeed, ABSOLUTE_MOTION);
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

		qDebug() << "homeStep:" << homeStep;

		if (homeStep > 6)
		{
			return true;
		}

		Sleep(300);
	}
}

void HSMeasure::hs_zero()  //回原button clicked
{
	mbStop = false;


	mpDMC5000Lib->mpDmcAxis[mCardNo][ccdAxisNo]->setHomePara(speedFromIni.ccdHomeSpeed);
	mpDMC5000Lib->mpDmcAxis[mCardNo][laserAxisNo]->setHomePara(speedFromIni.laserHomeSpeed);
	mpDMC5000Lib->mpDmcAxis[mCardNo][platformAxisNo]->setHomePara(speedFromIni.platHomeSpeed);

	if (false == go_home())
	{
		ui.pushButtonZero->setStyleSheet("QPushButton{background:red}");
		return;
	}

	ui.pushButtonZero->setStyleSheet("QPushButton{background:green}");
	ui.pushButtonStart->setEnabled(true);
}

void HSMeasure::ok_ngResult()
{
	std::array<std::array<float, GapJ + 1>, SlotposNo4 + 1> tGap = gapQueue.dequeue();
	std::array<std::array<float, StepJ + 1>, SlotposNo4 + 1> tStep = stepQueue.dequeue();
	
	for (size_t i = 0; i < SlotposNo4 + 1; i++)
	{
		for (size_t j = 0; j < GapJ + 1; j++)
		{	
			if (tGap[i][j] < offsetGap_L || tGap[i][j] > offsetGap_H)
			{
				fixValue.plug[i].ok_ng |= (1<<0);
			}

			if (tStep[i][j] < offsetStep_L || tStep[i][j] > offsetStep_H)
			{
				fixValue.plug[i].ok_ng |= (1<<1);
			}
			
			fixValue.plug[i].gap[j] = tGap[i][j];
			fixValue.plug[i].step[j] = tStep[i][j];
		}
	}
}

bool HSMeasure::okUnload()
{
	if (fixValue.plug[0].ok_ng != 0 && fixValue.plug[1].ok_ng != 0 && fixValue.plug[2].ok_ng != 0 && fixValue.plug[3].ok_ng != 0)
	{
		return true;  //全部ng
	}

	if (IO_ON == sensorIn(Opposite1))	//进料对射
	{
		if (IO_ON == sensorIn(Opposite3))
		{
			sensorOut(RedLight, IO_ON);
			sensorOut(Buzzer, IO_ON);
		}

		return false;
	}



	cylinderMove(UPDOWN_C_1I);
	cylinderMove(UPDOWN_C_2I);
	cylinderMove(UPDOWN_C_3I);
	cylinderMove(UPDOWN_C_4I);

	if (cylinderCheck(UPDOWN_C_1I) != IO_ON || cylinderCheck(UPDOWN_C_2I) != IO_ON || cylinderCheck(UPDOWN_C_3I) != IO_ON || cylinderCheck(UPDOWN_C_4I) != IO_ON)
	{
		return false;
	}

	sensorOut(Vacuum9, IO_OFF);
	sensorOut(Vacuum10, IO_OFF);
	sensorOut(Vacuum11, IO_OFF);
	sensorOut(Vacuum12, IO_OFF);

	if (sensorIn(Vacuum9) != IO_OFF || sensorIn(Vacuum10) != IO_OFF || sensorIn(Vacuum11) != IO_OFF || sensorIn(Vacuum12) != IO_OFF)
	{
		return false;
	}

	return true;
}

bool HSMeasure::ngUnload()
{
	if (fixValue.plug[0].ok_ng == 0 && fixValue.plug[1].ok_ng == 0 && fixValue.plug[2].ok_ng == 0 && fixValue.plug[3].ok_ng == 0)
	{
		return true;  //全部ok
	}
	//Y2运动至ng位
	if (false == mpMOTIONLib->mpDmcAxis[mCardNo][unloadAxisNo]->moveAndCheck(ngPositionY2))
	{
		return false;
	}

	if (fixValue.plug[0].ok_ng != 0)	{ cylinderMove(UPDOWN_C_1I); }
	if (fixValue.plug[1].ok_ng != 0)	{ cylinderMove(UPDOWN_C_2I); }
	if (fixValue.plug[2].ok_ng != 0)	{ cylinderMove(UPDOWN_C_3I); }
	if (fixValue.plug[3].ok_ng != 0)	{ cylinderMove(UPDOWN_C_4I); }
	

	if (fixValue.plug[0].ok_ng != 0 && cylinderCheck(UPDOWN_C_1I) != IO_ON || cylinderCheck(UPDOWN_C_2I) != IO_ON || cylinderCheck(UPDOWN_C_3I) != IO_ON || cylinderCheck(UPDOWN_C_4I) != IO_ON)
	{
		return false;
	}

	sensorOut(Vacuum9, IO_OFF);
	sensorOut(Vacuum10, IO_OFF);
	sensorOut(Vacuum11, IO_OFF);
	sensorOut(Vacuum12, IO_OFF);

	if (sensorIn(Vacuum9) != IO_OFF || sensorIn(Vacuum10) != IO_OFF || sensorIn(Vacuum11) != IO_OFF || sensorIn(Vacuum12) != IO_OFF)
	{
		return false;
	}

	for (size_t i = 0; i < SlotposNo4 + 1; i++)
	{	
		ngTypeQueue.enqueue(fixValue.plug[i].ok_ng);	
	}

	return true;
}

void HSMeasure::caseOpposite()
{
	if (IO_ON == sensorIn(Opposite3))
	{
		sensorOut(LineUnloadOkRun, IO_OFF);
	}
	else if (IO_ON == sensorIn(Opposite1))
	{
		sensorOut(LineUnloadOkRun, IO_ON);
	}
}

inline void HSMeasure::caseFlow1()
{
	
	switch (curStateFlow1)
	{
	case 0:
		if (IO_ON == sensorIn(START))	
			curStateFlow1++;
		break;

	case 1:
		sensorOut(FixtureVacuum, IO_ON);		//治具开真空
		curStateFlow1++;
		break;

	case 2:
		if (IO_ON == sensorIn(FixtureVacuum))
			curStateFlow1++;
		break;

	case 3:
		//光纤信号
		if (IO_ON == sensorIn(Fiber1) || IO_ON == sensorIn(Fiber2) || IO_ON == sensorIn(Fiber3) || IO_ON == sensorIn(Fiber4))
			curStateFlow1++;
		break;

	case 4:
		cylinderMove(UPDOWN_A_o);		//1~4气缸上升
		curStateFlow1++;
		break;

	case 5:
		if (IO_ON == cylinderCheck(UPDOWN_A_o))
			curStateFlow1++;
		break;
		
	case 6:
		cylinderMove(FEED_o);		//送料气缸缩回
		curStateFlow1++;
		break;
	
	case 7:
		if (IO_ON == cylinderCheck(FEED_o))
			curStateFlow1++;
		break;
	
	case 8:
		cylinderMove(UPDOWN_A_I);	//1~4升降气缸伸出
		curStateFlow1++;
		break;
	
	case 9:
		sensorOut(Vacuum1, IO_ON);	//开启1~4路真空
		sensorOut(Vacuum2, IO_ON);
		sensorOut(Vacuum3, IO_ON);
		sensorOut(Vacuum4, IO_ON);
		curStateFlow1++;
		break;

	case 10:
		if (IO_ON == sensorIn(Vacuum1) && IO_ON == sensorIn(Vacuum2) && IO_ON == sensorIn(Vacuum3) && IO_ON == sensorIn(Vacuum4))
			curStateFlow1++;
		break;
	
	case 11:
		cylinderMove(UPDOWN_A_o);	//1~4升降气缸缩回
		curStateFlow1++;
		break;
	
	case 12:
		if (IO_ON == cylinderCheck(UPDOWN_A_o))	//1~4升降气缸缩回到位
			curStateFlow1++;
		break;

	case 13:
		if (sensorIn(LightCurtain) != IO_ON)	//光幕无信号
			curStateFlow1++;
		break;

	case 14:
		cylinderMove(FEED_I);		//送料气缸伸出
		curStateFlow1++;
		break;

	case 15:
		if (IO_ON == cylinderCheck(FEED_I))	////送料气缸伸出完成
			curStateFlow1++;
		break;

	case 16:	
		curStateFlow1 = -1;
		
		if (-1 == curStateFlow2)
		{
			curStateFlow2 = 0;
		}

	default:
		break;
	}
}

inline void HSMeasure::caseFlow2()
{
	switch (curStateFlow2)
	{
	case 0:
		cylinderMove(ROTATE_o);	//转动气缸缩回
		curStateFlow2++;
		break;
	case 1:
		if (IO_ON == cylinderCheck(CLAMP1_o) || modeRunNullFlag)	//判断GAP 松开信号
			curStateFlow2++;
		break;
	case 2:
		//Y1轴运动至上料位
		if (true == mpMOTIONLib->mpDmcAxis[mCardNo][platformAxisNo]->moveAndCheck(loadPositionY1))
		{
			curStateFlow2++;
		}
		
		break;
	case 3:
		cylinderMove(UPDOWN_A_I);	//1~4升降气缸伸出
		curStateFlow2++;
		break;
	case 4:
		sensorOut(Vacuum1, IO_OFF);	//关闭1~4路真空
		sensorOut(Vacuum2, IO_OFF);
		sensorOut(Vacuum3, IO_OFF);
		sensorOut(Vacuum4, IO_OFF);
		curStateFlow2++;
		break;
	case 5:
		cylinderMove(UPDOWN_A_o);	//1~4升降气缸缩回
		curStateFlow2++;
		break;
	case 6:
		if(IO_ON == cylinderCheck(UPDOWN_A_o))	//判断缩回信号
			curStateFlow2++;
		break;
	case 7:
		cylinderMove(CLAMP1_I);	//GAP夹紧
		curStateFlow2++;
		break;
	case 8:
		mpBCDH_gap->getGapValueFront();		//GAP正面检测
		curStateFlow2++;
		break;
	case 9:
		cylinderMove(ROTATE_I);	//转动气缸
		curStateFlow2++;
		break;
	case 10:
		if(IO_ON == cylinderCheck(ROTATE_I))	//判断
			curStateFlow2++;
		break;

	case 11:
		mpBCDH_gap->getGapValueSide();		//Gap侧面

		gapQueue.enqueue(gapValues);

		curStateFlow2++;
		break;
	case 12:
		cylinderMove(ROTATE_o);	//转动气缸
		curStateFlow2++;
		break;
	case 13:
		if (IO_ON == cylinderCheck(ROTATE_o))	//判断
			curStateFlow2++;
		break;
	case 14:
		//Y1轴运动至下料位
		mpMOTIONLib->mpDmcAxis[mCardNo][platformAxisNo]->moveAndCheckdone(unloadPositionY1, AUTO_MOVE_TIME_OUT);
		curStateFlow2++;
		break;
	case 15:
		//X2轴安全位置
		mpMOTIONLib->mpDmcAxis[mCardNo][laserAxisNo]->moveAndCheckdone(LaserSafePosition, AUTO_MOVE_TIME_OUT);
		curStateFlow2++;
		break;
	case 16:
		//Y2轴运动到取料位
		mpMOTIONLib->mpDmcAxis[mCardNo][unloadAxisNo]->moveAndCheckdone(loadPositionY2, AUTO_MOVE_TIME_OUT);
		curStateFlow2++;
		break;
	case 17:
		stepFiberFlag[0] = sensorIn(Fiber1);
		curStateFlow2++;
		break;
	case 18:
		cylinderMove(UPDOWN_B_I);	//5~8升降气缸下降
		if (stepFiberFlag[SlotposNo1])	cylinderMove(UPDOWN_C_1I);	
		if (stepFiberFlag[SlotposNo2])	cylinderMove(UPDOWN_C_2I);	
		if (stepFiberFlag[SlotposNo3])	cylinderMove(UPDOWN_C_3I);	
		if (stepFiberFlag[SlotposNo4])	cylinderMove(UPDOWN_C_4I);	//9~12升降气缸下降		
		curStateFlow2++;
		break;
	case 19:
		sensorOut(Vacuum5, IO_ON);	//开启5~8路真空
		sensorOut(Vacuum6, IO_ON);
		sensorOut(Vacuum7, IO_ON);
		sensorOut(Vacuum8, IO_ON);
		if (stepFiberFlag[SlotposNo1])	sensorOut(Vacuum9, IO_ON);	//开启9路真空
		if (stepFiberFlag[SlotposNo2])	sensorOut(Vacuum10, IO_ON);	//
		if (stepFiberFlag[SlotposNo3])	sensorOut(Vacuum11, IO_ON);	//
		if (stepFiberFlag[SlotposNo4])	sensorOut(Vacuum12, IO_ON);	//开启12路真空	
		curStateFlow2++;
		break;
	case 20:
		if (IO_ON == sensorIn(Vacuum5) && IO_ON == sensorIn(Vacuum6) && IO_ON == sensorIn(Vacuum7) && IO_ON == sensorIn(Vacuum8) || modeRunNullFlag)
			curStateFlow2++;
		break;	
	case 21:
		if (stepFiberFlag[SlotposNo1] && IO_ON == sensorIn(Vacuum9) || modeRunNullFlag)
			curStateFlow2++;
		break;
	case 22:
		if (stepFiberFlag[SlotposNo2] && IO_ON == sensorIn(Vacuum10) || modeRunNullFlag)
			curStateFlow2++;
		break;
	case 23:
		if (stepFiberFlag[SlotposNo3] && IO_ON == sensorIn(Vacuum11) || modeRunNullFlag)
			curStateFlow2++;
		break;
	case 24:
		if (stepFiberFlag[SlotposNo4] && IO_ON == sensorIn(Vacuum12) || modeRunNullFlag)
			curStateFlow2++;
		break;
	case 25:
		cylinderMove(UPDOWN_B_o);	//5~8升降气缸上升
		cylinderMove(UPDOWN_C_1o);
		cylinderMove(UPDOWN_C_2o);
		cylinderMove(UPDOWN_C_3o);
		cylinderMove(UPDOWN_C_4o);	//9~12升降气缸上升		
		curStateFlow2++;
		break;
	case 26:
		if (IO_ON == cylinderCheck(UPDOWN_B_o) 
			&& IO_ON == cylinderCheck(UPDOWN_C_1o) && IO_ON == cylinderCheck(UPDOWN_C_2o) && IO_ON == cylinderCheck(UPDOWN_C_3o) && IO_ON == cylinderCheck(UPDOWN_C_4o)
			|| modeRunNullFlag)
		{
			curStateFlow2++;
		}
		break;
	case 27:
		curStateFlow2 = 0;
		if (-1 == curStateFlow3)
		{
			curStateFlow3 = 0;
		}

	default:
		break;
	}
}

inline void HSMeasure::caseFlow3()
{
	switch (curStateFlow3)
	{
	case 0:
		//Y2轴运动至STEP放料位
		if (true == mpMOTIONLib->mpDmcAxis[mCardNo][unloadAxisNo]->moveAndCheck(stepPositionY2))
			curStateFlow3++;	
		break;
	case 1:
		if (IO_ON == sensorIn(Vacuum5) && IO_ON == sensorIn(Vacuum6) && IO_ON == sensorIn(Vacuum7) && IO_ON == sensorIn(Vacuum8) || modeRunNullFlag)
			curStateFlow3++;
		break;
	case 2:
		sensorOut(Vacuum5, IO_OFF);	//关闭5~8路真空
		sensorOut(Vacuum6, IO_OFF);
		sensorOut(Vacuum7, IO_OFF);
		sensorOut(Vacuum8, IO_OFF);
		curStateFlow3++;
		break;
	case 3:
		cylinderMove(UPDOWN_B_o);	//5~8升降气缸上升
		curStateFlow3++;
		break;
	case 4:
		if (IO_ON == cylinderCheck(UPDOWN_B_o) || modeRunNullFlag)
			curStateFlow3++;
		break;
	case 5:
		cylinderMove(CLAMP2_I);	//STEP气缸夹紧
		curStateFlow3++;
		break;
	case 6:
		//Y2轴在安全位置？		
		curStateFlow3++;
	
		break;
	case 7:
		//
		if (-1 == curStateFlow4)
		{
			curStateFlow4 = 0;
		}
		
		if (false == mpBCDH_step->getStepValue())
		{
			break;
		}

		stepQueue.enqueue(gapValues);
		saveValueToLog(fixValue);
		showValueToUi(fixValue);
		curStateFlow3++;
		break;
	}

}

void HSMeasure::caseFlow4()	//下料过程
{
	switch (curStateFlow4)
	{
	case 0:
		if (stepQueue.isEmpty())	
			break;	
		curStateFlow4++;
		break;
	case 1:
		ok_ngResult();
		curStateFlow4++;
		break;
	case 2:
		//Y2运动至下料位 ok料
		if (true == mpMOTIONLib->mpDmcAxis[mCardNo][unloadAxisNo]->moveAndCheck(unloadPositionY2))
		{
			curStateFlow4++;
		}	
		break;
	case 3:
		if (!sensorIn(Opposite1))
			curStateFlow4++;
		break;
	case 4:
		if (IO_ON == sensorIn(Vacuum9) && IO_ON == sensorIn(Vacuum10) && IO_ON == sensorIn(Vacuum11) && IO_ON == sensorIn(Vacuum12) || modeRunNullFlag)
			curStateFlow4++;
		break;
	case 5:
		if (true == ngUnload())
			curStateFlow4++;
		break;	
	case 6:
		cylinderMove(UPDOWN_C_1o);
		cylinderMove(UPDOWN_C_2o);
		cylinderMove(UPDOWN_C_3o);
		cylinderMove(UPDOWN_C_4o);
		if (IO_ON == cylinderCheck(UPDOWN_C_1o) && IO_ON == cylinderCheck(UPDOWN_C_2o) && IO_ON == cylinderCheck(UPDOWN_C_3o) && IO_ON == cylinderCheck(UPDOWN_C_4o))
			curStateFlow4++;
		break;
	case 7:
		if (true == okUnload())
			curStateFlow4++;
		break;
	case 8:
		cylinderMove(UPDOWN_C_1o);
		cylinderMove(UPDOWN_C_2o);
		cylinderMove(UPDOWN_C_3o);
		cylinderMove(UPDOWN_C_4o);
		if (IO_ON == cylinderCheck(UPDOWN_C_1o) && IO_ON == cylinderCheck(UPDOWN_C_2o) && IO_ON == cylinderCheck(UPDOWN_C_3o) && IO_ON == cylinderCheck(UPDOWN_C_4o))
			curStateFlow4++;
		break;

	case 9:
		curStateFlow4 = 0;
		break;
	default:
		break;
	}
}

void MY_THREAD::run()  //运动流程
{
	
	while (true)
	{
		short ioStart = mpHSMeasure->sensorIn(START);
		short ioConfirm = mpHSMeasure->sensorIn(CONFIRM);
		short ioStop = mpHSMeasure->sensorIn(STOP);
		short ioEmgStop = mpHSMeasure->sensorIn(EMGSTOP);
		short ioCurMode = mpHSMeasure->sensorIn(SWITCH_MODE);

		if (IO_ON == ioEmgStop)			//急停
		{
			this->mpHSMeasure->mbStop = true;
			break;
		}

		mMutex.lock();
		if (true == this->mpHSMeasure->mbStop)
		{
			mMutex.unlock();
			continue;
		}

		if (true == this->mpHSMeasure->mbPause)
		{
			mMutex.unlock();
			continue;
		}
		mMutex.unlock();

		mpHSMeasure->caseFlow1();
		mpHSMeasure->caseFlow2();
		mpHSMeasure->caseFlow3();
		mpHSMeasure->caseFlow4();

		//ng打点






		//io
		mpHSMeasure->caseOpposite();





		QSettings settings(mPath +"/cfg/config.ini", QSettings::IniFormat);
		int bStart = settings.value("DEBUG/BSTART").toInt();

		if ((IO_ON == ioStart && IO_ON == ioConfirm) || 1 == bStart)
		{
			settings.setValue("DEBUG/BSTART","0");
			
			if (-1 == mpHSMeasure->curStateFlow1)
			{
				mpHSMeasure->curStateFlow1 = 0;
			}		
		}
		
	
		
		qDebug() << "runing..." << QThread::currentThreadId();
		QApplication::processEvents();
		QThread::msleep(1000);
	}

}

void HSMeasure::hs_start()  //启动button clicked
{
	initValue();
	//
	std::shared_ptr <MY_THREAD> pWorkThread = std::make_shared<MY_THREAD>(this);
	std::shared_ptr <MY_THREAD> pIoThread = std::make_shared<MY_THREAD>(this);

	connect(&pWorkThread->mThread, &QThread::started, &*pWorkThread, &MY_THREAD::run);
	connect(&pIoThread->mThread, &QThread::started, &*pIoThread, &MY_THREAD::fun);

	this->pWorkThread = pWorkThread;
	this->pIoThread = pIoThread;

	ui.pushButtonStart->setStyleSheet("QPushButton{background:lightgreen}");
	ui.pushButtonStop->setStyleSheet("QPushButton{background:}");

	ui.pushButtonStart->setEnabled(false);
	ui.pushButtonZero->setEnabled(false);

	
}

void HSMeasure::show_msg(const QString &msg)
{
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_time = current_date_time.toString("yyyy-MM-dd HH:mm:ss:zzz");
	ui.plainTextEditShowMsg->appendPlainText("["+ current_time +"] " + msg);
	qDebug() << msg;
}

void HSMeasure::show_dialog(const QString &msg)
{
	QMessageBox::information(this, "", msg);
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
	loadPositionY1 = settings.value("POSITION/loadPositionY1").toInt();
	unloadPositionY1 = settings.value("POSITION/unloadPositionY1").toInt();
	loadPositionY2 = settings.value("POSITION/loadPositionY2").toInt();
	unloadPositionY2 = settings.value("POSITION/unloadPositionY2").toInt();
	stepPositionY2 = settings.value("POSITION/stepPositionY2").toInt();
	ngPositionY2 = settings.value("POSITION/ngPositionY2").toInt();
	//
	speedFromIni.ccdHomeSpeed = settings.value("SPEED/ccdHomeSpeed").toDouble();
	speedFromIni.laserHomeSpeed = settings.value("SPEED/laserHomeSpeed").toDouble();
	speedFromIni.platHomeSpeed = settings.value("SPEED/platHomeSpeed").toDouble();
	speedFromIni.rotateHomeSpeed = settings.value("SPEED/rotateHomeSpeed").toDouble();
	speedFromIni.unloadHomeSpeed = settings.value("SPEED/unloadHomeSpeed").toDouble();
	
	speedFromIni.ccdJogSpeed = settings.value("SPEED/ccdJogSpeed").toDouble();
	speedFromIni.laserJogSpeed = settings.value("SPEED/laserJogSpeed").toDouble();
	speedFromIni.platJogSpeed = settings.value("SPEED/platJogSpeed").toDouble();
	speedFromIni.rotateJogSpeed = settings.value("SPEED/rotateJogSpeed").toDouble();
	speedFromIni.unloadJogSpeed = settings.value("SPEED/unloadJogSpeed").toDouble();
	speedFromIni.jogPulse = settings.value("SPEED/jogPulse").toInt();

	speedFromIni.ccdAutoSpeed = settings.value("SPEED/ccdAutoSpeed").toDouble();
	speedFromIni.laserAutoSpeed = settings.value("SPEED/laserAutoSpeed").toDouble();
	speedFromIni.platAutoSpeed = settings.value("SPEED/platAutoSpeed").toDouble();
	speedFromIni.rotateAutoSpeed = settings.value("SPEED/rotateAutoSpeed").toDouble();
	speedFromIni.unloadAutoSpeed = settings.value("SPEED/unloadAutoSpeed").toDouble();
	//
	offsetGap_L = settings.value("OFFSET/offsetGap_L").toFloat();
	offsetGap_H = settings.value("OFFSET/offsetGap_H").toFloat();
	offsetStep_L = settings.value("OFFSET/offsetStep_L").toFloat();
	offsetStep_H = settings.value("OFFSET/offsetStep_H").toFloat();

	//
	modeRunNullFlag = settings.value("DEBUG/modeRunNullFlag").toInt();
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

void HSMeasure::deal_raw_data()
{
	int inum = 0;
	QVector<double> x(800), y(800);
	QString dbFileName = "C:\\data1.txt";
	QFile sptabFile(dbFileName);
	if (sptabFile.exists())
	{
		int j = 0;
	}
	else
	{
		int k = 0;
	}

	if (!sptabFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "Can't open the file!" << endl;
	}
	while (!sptabFile.atEnd())
	{

		//QCoreApplication::processEvents();
		QString line = sptabFile.readLine();
		y[inum] = line.toFloat();
		x[inum] = inum;
		inum++;

	}  //end while		
	m_laserEditor[0]->show_line(x, y);
}

