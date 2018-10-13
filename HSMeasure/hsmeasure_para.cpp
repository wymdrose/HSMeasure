#include "hsmeasure.h"

void HSMeasure::hsmeasure_para()
{
	QString fileName;
	fileName = QCoreApplication::applicationDirPath();
	fileName += "/cfg/config.ini";

	QSettings settings(fileName, QSettings::IniFormat);
	QString str_path = settings.value("path_name/path").toString();

	ui.lineEditCcdIp1->setText(settings.value("CCD_CFG/IP1").toString());
	ui.lineEditCcdIp2->setText(settings.value("CCD_CFG/IP2").toString());

	ui.lineEditCcdUart1->setText(settings.value("CCD_CFG/UART1").toString());
	ui.lineEditCcdUart2->setText(settings.value("CCD_CFG/UART2").toString());
	
	ui.lineEditLaserIp1->setText(settings.value("LASER_CFG/IP1").toString());
	ui.lineEditLaserIp2->setText(settings.value("LASER_CFG/IP2").toString());
	ui.lineEditLaserIp3->setText(settings.value("LASER_CFG/IP3").toString());

	//
	mpTcpClientCcd[0] = new _COMMUNICATECLASS::TCP_CLIENT(ui.lineEditCcdIp1->text(), settings.value("CCD_CFG/PORT1").toInt());
	mpTcpClientCcd[1] = new _COMMUNICATECLASS::TCP_CLIENT(ui.lineEditCcdIp2->text(), settings.value("CCD_CFG/PORT2").toInt());

	mpTcpClientLaser[0] = new _COMMUNICATECLASS::TCP_CLIENT(ui.lineEditLaserIp1->text(), settings.value("LASER_CFG/PORT1").toInt());
	mpTcpClientLaser[1] = new _COMMUNICATECLASS::TCP_CLIENT(ui.lineEditLaserIp2->text(), settings.value("LASER_CFG/PORT2").toInt());
	mpTcpClientLaser[2] = new _COMMUNICATECLASS::TCP_CLIENT(ui.lineEditLaserIp3->text(), settings.value("LASER_CFG/PORT3").toInt());

	//
	mpSerialportCcd[0] = new _COMMUNICATECLASS::COM_PORT_ONE(ui.lineEditCcdUart1->text().toInt(), settings.value("CCD_CFG/baudRate").toInt());
	mpSerialportCcd[1] = new _COMMUNICATECLASS::COM_PORT_ONE(ui.lineEditCcdUart2->text().toInt(), settings.value("CCD_CFG/baudRate").toInt());

	//
	connect(ui.pushButtonCcdSend, &QPushButton::clicked, [this](){		
		//
		mpTcpClientCcd[0]->connect();
		mpTcpClientCcd[1]->connect();

		mpTcpClientCcd[0]->send(ui.textEditCcdSend->toPlainText() + "\r");
		mpTcpClientCcd[0]->getRec();
		
		mpTcpClientCcd[1]->send(ui.textEditCcdSend->toPlainText() + "\r");	
		mpTcpClientCcd[1]->getRec();

		//
		if (false == mpSerialportCcd[0]->open())
		{
			return;
		}
		mpSerialportCcd[1]->open();

		mpSerialportCcd[0]->send(ui.textEditCcdSend->toPlainText() + "\r");
		mpSerialportCcd[0]->recData();

		mpSerialportCcd[1]->send(ui.textEditCcdSend->toPlainText() + "\r");	
		mpSerialportCcd[1]->recData();


	});
	//
	
	connect(ui.pushButtonLaserSend, &QPushButton::clicked, [this](){
		//
		mpTcpClientLaser[0]->connect();
		mpTcpClientLaser[1]->connect();
		mpTcpClientLaser[2]->connect();

		mpTcpClientLaser[0]->send(ui.textEditCcdSend->toPlainText() + "\r");
		mpTcpClientLaser[0]->getRec();
		
		mpTcpClientLaser[1]->send(ui.textEditCcdSend->toPlainText() + "\r");
		mpTcpClientLaser[1]->getRec();

		mpTcpClientLaser[2]->send(ui.textEditCcdSend->toPlainText() + "\r");
		mpTcpClientLaser[2]->getRec();

	});


	//
	connect(ui.pushButtonPjogCcd, &QPushButton::pressed, [this](){

		mpDMC5000Lib->mpDmcAxis[mCardNo][ccdAxisNo]->setMovePara(0, 5000, 10000);

		auto nPulse = ui.lineEditJogSpeedCcd->text().toInt();

		if (nPulse < 1000)
		{
			nPulse = 1000;
		}

		mpMOTIONLib->move(mCardNo, ccdAxisNo, nPulse);

		ui.lineEditCurPositionCcd->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][ccdAxisNo]->curPosition()));

	});

	connect(ui.pushButtonNjogCcd, &QPushButton::pressed, [this](){

	
		mpDMC5000Lib->mpDmcAxis[mCardNo][ccdAxisNo]->setMovePara(0, 5000, 10000);
		mpMOTIONLib->move(mCardNo, ccdAxisNo, -1000);

		ui.lineEditCurPositionCcd->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][ccdAxisNo]->curPosition()));

	});

	connect(ui.pushButtonMoveCcd, &QPushButton::clicked, [this](){

		auto nPulse = ui.lineEditTargetPositionCcd->text().toInt();
		
		mpDMC5000Lib->mpDmcAxis[mCardNo][ccdAxisNo]->setMovePara(1, 5000, 10000);
		mpMOTIONLib->move(mCardNo, ccdAxisNo, nPulse);


		DWORD tSystemTime = GetTickCount();

		while (1 == mpDMC5000Lib->mpDmcAxis[mCardNo][ccdAxisNo]->checkDone(tSystemTime, DEFAUL_MOVE_TIME_OUT))
		{
			ui.lineEditCurPositionCcd->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][ccdAxisNo]->curPosition()));
		}	

	});
	
	connect(ui.pushButtonHomeCcd, &QPushButton::clicked, [this](){

		mpDMC5000Lib->mpDmcAxis[mCardNo][ccdAxisNo]->setHomePara();
		mpDMC5000Lib->mpDmcAxis[mCardNo][ccdAxisNo]->home();

		DWORD tSystemTime = GetTickCount();

		while (1 == mpDMC5000Lib->mpDmcAxis[mCardNo][ccdAxisNo]->checkDone(tSystemTime, DEFAUL_MOVE_TIME_OUT))
		{
			ui.lineEditCurPositionCcd->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][ccdAxisNo]->curPosition()));
		}
		
	});

	connect(ui.pushButtonPoweronCcd, &QPushButton::clicked, [this](){
	
		mpDMC5000Lib->mpDmcAxis[mCardNo][ccdAxisNo]->sevonOnOff(0);
	});
	
	connect(ui.pushButtonPoweroffCcd, &QPushButton::clicked, [this](){

		dmc_stop(mCardNo, ccdAxisNo, 0);
		mpDMC5000Lib->mpDmcAxis[mCardNo][ccdAxisNo]->sevonOnOff(1);
	});

	//LASER
	connect(ui.pushButtonPjogLaser, &QPushButton::pressed, [this](){

		mpDMC5000Lib->mpDmcAxis[mCardNo][laserAxisNo]->setMovePara(0, 5000, 10000);

		auto nPulse = ui.lineEditJogSpeedLaser->text().toInt();

		if (nPulse < 1000)
		{
			nPulse = 1000;
		}

		mpMOTIONLib->move(mCardNo, laserAxisNo, nPulse);

		ui.lineEditCurPositionLaser->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][laserAxisNo]->curPosition()));

	});

	connect(ui.pushButtonNjogLaser, &QPushButton::pressed, [this](){


		mpDMC5000Lib->mpDmcAxis[mCardNo][laserAxisNo]->setMovePara(0, 5000, 10000);
		mpMOTIONLib->move(mCardNo, laserAxisNo, -1000);

		ui.lineEditCurPositionLaser->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][laserAxisNo]->curPosition()));

	});

	connect(ui.pushButtonMoveLaser, &QPushButton::clicked, [this](){

		auto nPulse = ui.lineEditTargetPositionLaser->text().toInt();

		mpDMC5000Lib->mpDmcAxis[mCardNo][laserAxisNo]->setMovePara(1, 5000, 10000);
		mpMOTIONLib->move(mCardNo, laserAxisNo, nPulse);

		DWORD tSystemTime = GetTickCount();

		while (1 == mpDMC5000Lib->mpDmcAxis[mCardNo][laserAxisNo]->checkDone(tSystemTime, DEFAUL_MOVE_TIME_OUT))
		{
			ui.lineEditCurPositionLaser->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][laserAxisNo]->curPosition()));
		}
	});

	connect(ui.pushButtonHomeLaser, &QPushButton::clicked, [this](){

		mpDMC5000Lib->mpDmcAxis[mCardNo][laserAxisNo]->setHomePara();
		mpDMC5000Lib->mpDmcAxis[mCardNo][laserAxisNo]->home();

		DWORD tSystemTime = GetTickCount();

		while (1 == mpDMC5000Lib->mpDmcAxis[mCardNo][laserAxisNo]->checkDone(tSystemTime, DEFAUL_MOVE_TIME_OUT))
		{
			ui.lineEditCurPositionLaser->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][laserAxisNo]->curPosition()));
		}
	});

	connect(ui.pushButtonPoweronLaser, &QPushButton::clicked, [this](){

		mpDMC5000Lib->mpDmcAxis[mCardNo][laserAxisNo]->sevonOnOff(0);
	});

	connect(ui.pushButtonPoweroffLaser, &QPushButton::clicked, [this](){

		dmc_stop(mCardNo, laserAxisNo, 0);
		mpDMC5000Lib->mpDmcAxis[mCardNo][laserAxisNo]->sevonOnOff(1);
	});

	//platform
	connect(ui.pushButtonPjogPlatform, &QPushButton::pressed, [this](){

		mpDMC5000Lib->mpDmcAxis[mCardNo][platformAxisNo]->setMovePara(0, 5000, 10000);

		auto nPulse = ui.lineEditJogSpeedPlatform->text().toInt();

		if (nPulse < 1000)
		{
			nPulse = 1000;
		}

		mpMOTIONLib->move(mCardNo, platformAxisNo, nPulse);

		ui.lineEditCurPositionPlatform->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][platformAxisNo]->curPosition()));

	});

	connect(ui.pushButtonNjogPlatform, &QPushButton::pressed, [this](){


		mpDMC5000Lib->mpDmcAxis[mCardNo][platformAxisNo]->setMovePara(0, 5000, 10000);
		mpMOTIONLib->move(mCardNo, platformAxisNo, -1000);

		ui.lineEditCurPositionPlatform->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][platformAxisNo]->curPosition()));

	});

	connect(ui.pushButtonMovePlatform, &QPushButton::clicked, [this](){

		auto nPulse = ui.lineEditTargetPositionPlatform->text().toInt();

		mpDMC5000Lib->mpDmcAxis[mCardNo][platformAxisNo]->setMovePara(1, 5000, 10000);
		mpMOTIONLib->move(mCardNo, platformAxisNo, nPulse);

		DWORD tSystemTime = GetTickCount();

		while (1 == mpDMC5000Lib->mpDmcAxis[mCardNo][platformAxisNo]->checkDone(tSystemTime, DEFAUL_MOVE_TIME_OUT))
		{
			ui.lineEditCurPositionPlatform->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][platformAxisNo]->curPosition()));
		}
	});

	connect(ui.pushButtonHomePlatform, &QPushButton::clicked, [this](){

		mpDMC5000Lib->mpDmcAxis[mCardNo][platformAxisNo]->setHomePara();
		mpDMC5000Lib->mpDmcAxis[mCardNo][platformAxisNo]->home();
		DWORD tSystemTime = GetTickCount();

		while (1 == mpDMC5000Lib->mpDmcAxis[mCardNo][platformAxisNo]->checkDone(tSystemTime, DEFAUL_MOVE_TIME_OUT))
		{
			ui.lineEditCurPositionPlatform->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][platformAxisNo]->curPosition()));
		}
	});

	connect(ui.pushButtonPoweronPlatform, &QPushButton::clicked, [this](){

		mpDMC5000Lib->mpDmcAxis[mCardNo][platformAxisNo]->sevonOnOff(0);
	});

	connect(ui.pushButtonPoweroffPlatform, &QPushButton::clicked, [this](){
		
		dmc_stop(mCardNo, platformAxisNo, 0);
		mpDMC5000Lib->mpDmcAxis[mCardNo][platformAxisNo]->sevonOnOff(1);
	});

	//ROTATE
	connect(ui.pushButtonPjogRotate, &QPushButton::pressed, [this](){

		mpDMC5000Lib->mpDmcAxis[mCardNo][rotateAxisNo]->setMovePara(0, 5000, 10000);

		auto nPulse = ui.lineEditJogSpeedRotate->text().toInt();

		if (nPulse < 1000)
		{
			nPulse = 1000;
		}

		mpMOTIONLib->move(mCardNo, rotateAxisNo, nPulse);

		ui.lineEditCurPositionRotate->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][rotateAxisNo]->curPosition()));

	});

	connect(ui.pushButtonNjogRotate, &QPushButton::pressed, [this](){


		mpDMC5000Lib->mpDmcAxis[mCardNo][rotateAxisNo]->setMovePara(0, 5000, 10000);
		mpMOTIONLib->move(mCardNo, rotateAxisNo, -1000);

		ui.lineEditCurPositionRotate->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][rotateAxisNo]->curPosition()));

	});

	connect(ui.pushButtonMoveRotate, &QPushButton::clicked, [this](){

		auto nPulse = ui.lineEditTargetPositionRotate->text().toInt();

		mpDMC5000Lib->mpDmcAxis[mCardNo][rotateAxisNo]->setMovePara(1, 5000, 10000);
		mpMOTIONLib->move(mCardNo, rotateAxisNo, nPulse);

		DWORD tSystemTime = GetTickCount();

		while (1 == mpDMC5000Lib->mpDmcAxis[mCardNo][rotateAxisNo]->checkDone(tSystemTime, DEFAUL_MOVE_TIME_OUT))
		{
			ui.lineEditCurPositionRotate->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][rotateAxisNo]->curPosition()));
		}
	});

	connect(ui.pushButtonHomeRotate, &QPushButton::clicked, [this](){

		mpDMC5000Lib->mpDmcAxis[mCardNo][rotateAxisNo]->setHomePara();
		mpDMC5000Lib->mpDmcAxis[mCardNo][rotateAxisNo]->home();
		DWORD tSystemTime = GetTickCount();

		while (1 == mpDMC5000Lib->mpDmcAxis[mCardNo][rotateAxisNo]->checkDone(tSystemTime, DEFAUL_MOVE_TIME_OUT))
		{
			ui.lineEditCurPositionRotate->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][rotateAxisNo]->curPosition()));
		}
	});

	connect(ui.pushButtonPoweronRotate, &QPushButton::clicked, [this](){

		mpDMC5000Lib->mpDmcAxis[mCardNo][rotateAxisNo]->sevonOnOff(0);
	});

	connect(ui.pushButtonPoweroffRotate, &QPushButton::clicked, [this](){
		
		dmc_stop(mCardNo, rotateAxisNo, 0);
		mpDMC5000Lib->mpDmcAxis[mCardNo][rotateAxisNo]->sevonOnOff(1);
	});

	//UNLOAD
	connect(ui.pushButtonPjogUnload, &QPushButton::pressed, [this](){

		mpDMC5000Lib->mpDmcAxis[mCardNo][unloadAxisNo]->setMovePara(0, 5000, 10000);

		auto nPulse = ui.lineEditJogSpeedUnload->text().toInt();

		if (nPulse < 1000)
		{
			nPulse = 1000;
		}

		mpMOTIONLib->move(mCardNo, unloadAxisNo, nPulse);

		ui.lineEditCurPositionUnload->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][unloadAxisNo]->curPosition()));

	});

	connect(ui.pushButtonNjogUnload, &QPushButton::pressed, [this](){


		mpDMC5000Lib->mpDmcAxis[mCardNo][unloadAxisNo]->setMovePara(0, 5000, 10000);
		mpMOTIONLib->move(mCardNo, unloadAxisNo, -1000);

		ui.lineEditCurPositionUnload->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][unloadAxisNo]->curPosition()));

	});

	connect(ui.pushButtonMoveUnload, &QPushButton::clicked, [this](){

		auto nPulse = ui.lineEditTargetPositionUnload->text().toInt();

		mpDMC5000Lib->mpDmcAxis[mCardNo][unloadAxisNo]->setMovePara(1, 5000, 10000);
		mpMOTIONLib->move(mCardNo, unloadAxisNo, nPulse);
		DWORD tSystemTime = GetTickCount();

		while (1 == mpDMC5000Lib->mpDmcAxis[mCardNo][unloadAxisNo]->checkDone(tSystemTime, DEFAUL_MOVE_TIME_OUT))
		{
			ui.lineEditCurPositionUnload->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][unloadAxisNo]->curPosition()));
		}
	});

	connect(ui.pushButtonHomeUnload, &QPushButton::clicked, [this](){

		mpDMC5000Lib->mpDmcAxis[mCardNo][unloadAxisNo]->setHomePara();
		mpDMC5000Lib->mpDmcAxis[mCardNo][unloadAxisNo]->home();
		
		DWORD tSystemTime = GetTickCount();

		while (1 == mpDMC5000Lib->mpDmcAxis[mCardNo][unloadAxisNo]->checkDone(tSystemTime, DEFAUL_MOVE_TIME_OUT))
		{
			ui.lineEditCurPositionUnload->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][unloadAxisNo]->curPosition()));
		}
	});

	connect(ui.pushButtonPoweronUnload, &QPushButton::clicked, [this](){

		mpDMC5000Lib->mpDmcAxis[mCardNo][unloadAxisNo]->sevonOnOff(0);
	});

	connect(ui.pushButtonPoweroffUnload, &QPushButton::clicked, [this](){

		dmc_stop(mCardNo, unloadAxisNo, 0);
		mpDMC5000Lib->mpDmcAxis[mCardNo][unloadAxisNo]->sevonOnOff(1);
	});

}

