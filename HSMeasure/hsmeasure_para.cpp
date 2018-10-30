#include "hsmeasure.h"
#include "bcdh_step.h"

extern BCDH_step* mpBCDH_step;

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
	mpTcpClientCcd[0] = std::make_shared<_COMMUNICATECLASS::TCP_CLIENT>(ui.lineEditCcdIp1->text(), settings.value("CCD_CFG/PORT1").toInt());
	mpTcpClientCcd[1] = std::make_shared<_COMMUNICATECLASS::TCP_CLIENT>(ui.lineEditCcdIp2->text(), settings.value("CCD_CFG/PORT2").toInt());

	mpTcpClientLaser[0] = std::make_shared<_COMMUNICATECLASS::TCP_CLIENT>(ui.lineEditLaserIp1->text(), settings.value("LASER_CFG/PORT1").toInt());
	mpTcpClientLaser[1] = std::make_shared<_COMMUNICATECLASS::TCP_CLIENT>(ui.lineEditLaserIp2->text(), settings.value("LASER_CFG/PORT2").toInt());
	mpTcpClientLaser[2] = std::make_shared<_COMMUNICATECLASS::TCP_CLIENT>(ui.lineEditLaserIp3->text(), settings.value("LASER_CFG/PORT3").toInt());

	//
	mpSerialportCcd[0] = std::make_shared<_COMMUNICATECLASS::COM_PORT_ONE>(ui.lineEditCcdUart1->text().toInt(), settings.value("CCD_CFG/baudRate").toInt());
	mpSerialportCcd[1] = std::make_shared<_COMMUNICATECLASS::COM_PORT_ONE>(ui.lineEditCcdUart2->text().toInt(), settings.value("CCD_CFG/baudRate").toInt());

	//
	connect(ui.pushButtonCcdSendIp1, &QPushButton::clicked, [this](){
		//
		mpTcpClientCcd[0]->connect();
		mpTcpClientCcd[0]->send(ui.textEditCcdSend->toPlainText() + "\r");
		mpTcpClientCcd[0]->getRec();
	
	});

	connect(ui.pushButtonCcdSendIp2, &QPushButton::clicked, [this](){
	
		mpTcpClientCcd[1]->connect();
		mpTcpClientCcd[1]->send(ui.textEditCcdSend->toPlainText() + "\r");
		mpTcpClientCcd[1]->getRec();


	});
	
	connect(ui.pushButtonCcdSendUart1, &QPushButton::clicked, [this](){
		
		if (false == mpSerialportCcd[0]->open())
		{
			return;
		}
		mpSerialportCcd[0]->send(ui.textEditCcdSend->toPlainText() + "\r");
		mpSerialportCcd[0]->recData();
	});

	connect(ui.pushButtonCcdSendUart2, &QPushButton::clicked, [this](){
		
		mpSerialportCcd[1]->open();

		mpSerialportCcd[1]->send(ui.textEditCcdSend->toPlainText() + "\r");
		mpSerialportCcd[1]->recData();


	});
	
	//
	
	connect(ui.pushButtonLaserSendIp1, &QPushButton::clicked, [this](){
		
		float buf1[800] = { 0 };
		float buf2[800] = { 0 };

		mpBCDH_step->GetTwoHeadData(mpBCDH_step->laser1_config, buf1, buf2);

		qDebug() << "laser1   HEAD 1";

		for (size_t i = 0; i < 800; i += 10)
		{
			if (0 == i % 100)
			{
				qDebug() << "\r";
			}

			qDebug() << buf1[i] << " " << buf1[i + 1] << " " << buf1[i + 2] << " " << buf1[i + 3] << " " << buf1[i + 4] << " " << buf1[i + 5] << " " << buf1[i + 6] << " " << buf1[i + 7] << " " << buf1[i + 8] << " " << buf1[i + 9];

		}

		qDebug() << "\r";
		qDebug() << "\r";

		qDebug() << "HEAD 2";

		for (size_t i = 0; i < 800; i += 10)
		{
			if (0 == i % 100)
			{
				qDebug() << "\r";
			}

			qDebug() << buf2[i] << " " << buf2[i + 1] << " " << buf2[i + 2] << " " << buf2[i + 3] << " " << buf2[i + 4] << " " << buf2[i + 5] << " " << buf2[i + 6] << " " << buf2[i + 7] << " " << buf2[i + 8] << " " << buf2[i + 9];
		}


	});

	connect(ui.pushButtonLaserSendIp2, &QPushButton::clicked, [this](){
	
		float buf1[800] = { 0 };
		float buf2[800] = { 0 };

		mpBCDH_step->GetTwoHeadData(mpBCDH_step->laser2_config, buf1, buf2);

		qDebug() << "laser2   HEAD 1";

		for (size_t i = 0; i < 800; i += 10)
		{
			if (0 == i % 100)
			{
				qDebug() << "\r";
			}

			qDebug() << buf1[i] << " " << buf1[i + 1] << " " << buf1[i + 2] << " " << buf1[i + 3] << " " << buf1[i + 4] << " " << buf1[i + 5] << " " << buf1[i + 6] << " " << buf1[i + 7] << " " << buf1[i + 8] << " " << buf1[i + 9];

		}

		qDebug() << "\r";
		qDebug() << "\r";

		qDebug() << "HEAD 2";

		for (size_t i = 0; i < 800; i += 10)
		{
			if (0 == i % 100)
			{
				qDebug() << "\r";
			}

			qDebug() << buf2[i] << " " << buf2[i + 1] << " " << buf2[i + 2] << " " << buf2[i + 3] << " " << buf2[i + 4] << " " << buf2[i + 5] << " " << buf2[i + 6] << " " << buf2[i + 7] << " " << buf2[i + 8] << " " << buf2[i + 9];
		}

	});

	connect(ui.pushButtonLaserSendIp3, &QPushButton::clicked, [this](){
		
		float buf1[800] = {0};
		float buf2[800] = { 0 };
		
		mpBCDH_step->GetTwoHeadData(mpBCDH_step->laser3_config, buf1, buf2);

		qDebug() << "laser3  HEAD 1";

		for (size_t i = 0; i < 800; i+=10)
		{
			if (0 == i % 100)
			{
				qDebug() << "\r";
			}

			qDebug() << buf1[i] << " " << buf1[i+1] << " " << buf1[i+2] << " " << buf1[i+3] << " " << buf1[i+4] << " " << buf1[i+5] << " " << buf1[i+6] << " " << buf1[i+7] << " " << buf1[i+8] << " " << buf1[i+9];
			
		}
	
		qDebug() << "\r";
		qDebug() << "\r";

		qDebug() << "HEAD 2";

		for (size_t i = 0; i < 800; i += 10)
		{
			if (0 == i % 100)
			{
				qDebug() << "\r";
			}

			qDebug() << buf2[i] << " " << buf2[i + 1] << " " << buf2[i + 2] << " " << buf2[i + 3] << " " << buf2[i + 4] << " " << buf2[i + 5] << " " << buf2[i + 6] << " " << buf2[i + 7] << " " << buf2[i + 8] << " " << buf2[i + 9];		
		}


	});
	//
	connect(ui.pushButtonPjogCcd, &QPushButton::pressed, [this](){

		long curPosition = mpDMC5000Lib->mpDmcAxis[mCardNo][ccdAxisNo]->curPosition();

		mpDMC5000Lib->mpDmcAxis[mCardNo][ccdAxisNo]->setMovePara(speedFromIni.ccdJogSpeed, ABSOLUTE_MOTION);

		mpMOTIONLib->move(mCardNo, ccdAxisNo, curPosition + speedFromIni.jogPulse);

		ui.lineEditCurPositionCcd->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][ccdAxisNo]->curPosition()));

	});

	connect(ui.pushButtonNjogCcd, &QPushButton::pressed, [this](){

		long curPosition = mpDMC5000Lib->mpDmcAxis[mCardNo][ccdAxisNo]->curPosition();

		mpDMC5000Lib->mpDmcAxis[mCardNo][ccdAxisNo]->setMovePara(speedFromIni.ccdJogSpeed, ABSOLUTE_MOTION);
		mpMOTIONLib->move(mCardNo, ccdAxisNo, curPosition - speedFromIni.jogPulse);

		ui.lineEditCurPositionCcd->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][ccdAxisNo]->curPosition()));

	});

	connect(ui.pushButtonMoveCcd, &QPushButton::clicked, [this](){

		auto nPulse = ui.lineEditTargetPositionCcd->text().toInt();
		
		mpDMC5000Lib->mpDmcAxis[mCardNo][ccdAxisNo]->setMovePara(speedFromIni.ccdAutoSpeed, ABSOLUTE_MOTION);
		
		mpMOTIONLib->move(mCardNo, ccdAxisNo, nPulse);

		DWORD tSystemTime = GetTickCount();

		while (1 == mpDMC5000Lib->mpDmcAxis[mCardNo][ccdAxisNo]->checkDone(tSystemTime, DEFAUL_MOVE_TIME_OUT))
		{
			ui.lineEditCurPositionCcd->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][ccdAxisNo]->curPosition()));
		}	

	});
	
	connect(ui.pushButtonHomeCcd, &QPushButton::clicked, [this](){

		mpDMC5000Lib->mpDmcAxis[mCardNo][ccdAxisNo]->setHomePara(speedFromIni.ccdHomeSpeed);
		mpDMC5000Lib->mpDmcAxis[mCardNo][ccdAxisNo]->home();

		DWORD tSystemTime = GetTickCount();

		ui.lineEditCurPositionCcd->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][ccdAxisNo]->curPosition()));

		if (false == mpDMC5000Lib->mpDmcAxis[mCardNo][ccdAxisNo]->checkHome(tSystemTime, DEFAUL_MOVE_TIME_OUT))
		{
			QMessageBox::warning(this, "", "Failed");
			return;
		}

		QMessageBox::information(this, "", "OK");
		
	});

	connect(ui.pushButtonPoweronCcd, &QPushButton::clicked, [this](){
	
		mpDMC5000Lib->mpDmcAxis[mCardNo][ccdAxisNo]->sevonOnOff(DMC_SERV_ENABLE);
	});
	
	connect(ui.pushButtonPoweroffCcd, &QPushButton::clicked, [this](){

		dmc_stop(mCardNo, ccdAxisNo, 0);
		mpDMC5000Lib->mpDmcAxis[mCardNo][ccdAxisNo]->sevonOnOff(DMC_SERV_DISABLE);
	});

	//LASER
	connect(ui.pushButtonPjogLaser, &QPushButton::pressed, [this](){

		long curPosition = mpDMC5000Lib->mpDmcAxis[mCardNo][laserAxisNo]->curPosition();

		mpDMC5000Lib->mpDmcAxis[mCardNo][laserAxisNo]->setMovePara(speedFromIni.laserJogSpeed, ABSOLUTE_MOTION);

		mpMOTIONLib->move(mCardNo, laserAxisNo, curPosition + speedFromIni.jogPulse);

		ui.lineEditCurPositionLaser->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][laserAxisNo]->curPosition()));

	});

	connect(ui.pushButtonNjogLaser, &QPushButton::pressed, [this](){

		long curPosition = mpDMC5000Lib->mpDmcAxis[mCardNo][laserAxisNo]->curPosition();

		mpDMC5000Lib->mpDmcAxis[mCardNo][laserAxisNo]->setMovePara(speedFromIni.laserJogSpeed, ABSOLUTE_MOTION);
		
		mpMOTIONLib->move(mCardNo, laserAxisNo, curPosition - speedFromIni.jogPulse);

		ui.lineEditCurPositionLaser->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][laserAxisNo]->curPosition()));

	});

	connect(ui.pushButtonMoveLaser, &QPushButton::clicked, [this](){

		auto nPulse = ui.lineEditTargetPositionLaser->text().toInt();

		mpDMC5000Lib->mpDmcAxis[mCardNo][laserAxisNo]->setMovePara(speedFromIni.laserAutoSpeed, ABSOLUTE_MOTION);
		mpMOTIONLib->move(mCardNo, laserAxisNo, nPulse);

		DWORD tSystemTime = GetTickCount();

		while (1 == mpDMC5000Lib->mpDmcAxis[mCardNo][laserAxisNo]->checkDone(tSystemTime, DEFAUL_MOVE_TIME_OUT))
		{
			ui.lineEditCurPositionLaser->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][laserAxisNo]->curPosition()));
		}
	});

	connect(ui.pushButtonHomeLaser, &QPushButton::clicked, [this](){

		mpDMC5000Lib->mpDmcAxis[mCardNo][laserAxisNo]->setHomePara(speedFromIni.laserHomeSpeed);
		mpDMC5000Lib->mpDmcAxis[mCardNo][laserAxisNo]->home();

		DWORD tSystemTime = GetTickCount();

		ui.lineEditCurPositionLaser->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][laserAxisNo]->curPosition()));

		if (false == mpDMC5000Lib->mpDmcAxis[mCardNo][laserAxisNo]->checkHome(tSystemTime, DEFAUL_MOVE_TIME_OUT))
		{
			QMessageBox::warning(this, "", "Failed");
			return;
		}

		QMessageBox::information(this, "", "OK");
	});

	connect(ui.pushButtonPoweronLaser, &QPushButton::clicked, [this](){

		mpDMC5000Lib->mpDmcAxis[mCardNo][laserAxisNo]->sevonOnOff(DMC_SERV_ENABLE);
	});

	connect(ui.pushButtonPoweroffLaser, &QPushButton::clicked, [this](){

		dmc_stop(mCardNo, laserAxisNo, 0);
		mpDMC5000Lib->mpDmcAxis[mCardNo][laserAxisNo]->sevonOnOff(DMC_SERV_DISABLE);
	});

	//platform
	connect(ui.pushButtonPjogPlatform, &QPushButton::pressed, [this](){

		long curPosition = mpDMC5000Lib->mpDmcAxis[mCardNo][platformAxisNo]->curPosition();
		mpDMC5000Lib->mpDmcAxis[mCardNo][platformAxisNo]->setMovePara(speedFromIni.platJogSpeed, ABSOLUTE_MOTION);

		mpMOTIONLib->move(mCardNo, platformAxisNo, curPosition + speedFromIni.jogPulse);

		ui.lineEditCurPositionPlatform->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][platformAxisNo]->curPosition()));

	});

	connect(ui.pushButtonNjogPlatform, &QPushButton::pressed, [this](){

		long curPosition = mpDMC5000Lib->mpDmcAxis[mCardNo][platformAxisNo]->curPosition();
		mpDMC5000Lib->mpDmcAxis[mCardNo][platformAxisNo]->setMovePara(speedFromIni.platJogSpeed, ABSOLUTE_MOTION);
		mpMOTIONLib->move(mCardNo, platformAxisNo, curPosition - speedFromIni.jogPulse);

		ui.lineEditCurPositionPlatform->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][platformAxisNo]->curPosition()));

	});

	connect(ui.pushButtonMovePlatform, &QPushButton::clicked, [this](){

		auto nPulse = ui.lineEditTargetPositionPlatform->text().toInt();

		mpDMC5000Lib->mpDmcAxis[mCardNo][platformAxisNo]->setMovePara(speedFromIni.platAutoSpeed, ABSOLUTE_MOTION);
		mpMOTIONLib->move(mCardNo, platformAxisNo, nPulse);

		DWORD tSystemTime = GetTickCount();

		while (1 == mpDMC5000Lib->mpDmcAxis[mCardNo][platformAxisNo]->checkDone(tSystemTime, DEFAUL_MOVE_TIME_OUT))
		{
			ui.lineEditCurPositionPlatform->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][platformAxisNo]->curPosition()));
		}
	});

	connect(ui.pushButtonHomePlatform, &QPushButton::clicked, [this](){

		mpDMC5000Lib->mpDmcAxis[mCardNo][platformAxisNo]->setHomePara(speedFromIni.platHomeSpeed);
		mpDMC5000Lib->mpDmcAxis[mCardNo][platformAxisNo]->home();
		DWORD tSystemTime = GetTickCount();

		ui.lineEditCurPositionPlatform->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][platformAxisNo]->curPosition()));

		if (false == mpDMC5000Lib->mpDmcAxis[mCardNo][platformAxisNo]->checkHome(tSystemTime, DEFAUL_MOVE_TIME_OUT))
		{
			QMessageBox::warning(this, "", "Failed");
			return;
		}
		
		QMessageBox::information(this, "", "OK");

	});

	connect(ui.pushButtonPoweronPlatform, &QPushButton::clicked, [this](){

		mpDMC5000Lib->mpDmcAxis[mCardNo][platformAxisNo]->sevonOnOff(DMC_SERV_ENABLE);
	});

	connect(ui.pushButtonPoweroffPlatform, &QPushButton::clicked, [this](){
	
		mpDMC5000Lib->mpDmcAxis[mCardNo][platformAxisNo]->sevonOnOff(DMC_SERV_DISABLE);
	});

	//ROTATE
	connect(ui.pushButtonPjogRotate, &QPushButton::pressed, [this](){
		long curPosition = mpDMC5000Lib->mpDmcAxis[mCardNo][rotateAxisNo]->curPosition();
		mpDMC5000Lib->mpDmcAxis[mCardNo][rotateAxisNo]->setMovePara(speedFromIni.rotateJogSpeed, ABSOLUTE_MOTION);

		mpMOTIONLib->move(mCardNo, rotateAxisNo, curPosition + speedFromIni.jogPulse);

		ui.lineEditCurPositionRotate->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][rotateAxisNo]->curPosition()));

	});

	connect(ui.pushButtonNjogRotate, &QPushButton::pressed, [this](){

		long curPosition = mpDMC5000Lib->mpDmcAxis[mCardNo][rotateAxisNo]->curPosition();
		mpDMC5000Lib->mpDmcAxis[mCardNo][rotateAxisNo]->setMovePara(speedFromIni.rotateJogSpeed, ABSOLUTE_MOTION);
		mpMOTIONLib->move(mCardNo, rotateAxisNo, curPosition - speedFromIni.jogPulse);

		ui.lineEditCurPositionRotate->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][rotateAxisNo]->curPosition()));

	});

	connect(ui.pushButtonMoveRotate, &QPushButton::clicked, [this](){

		auto nPulse = ui.lineEditTargetPositionRotate->text().toInt();

		mpDMC5000Lib->mpDmcAxis[mCardNo][rotateAxisNo]->setMovePara(speedFromIni.rotateJogSpeed, ABSOLUTE_MOTION);
		mpMOTIONLib->move(mCardNo, rotateAxisNo, nPulse);

		DWORD tSystemTime = GetTickCount();

		while (1 == mpDMC5000Lib->mpDmcAxis[mCardNo][rotateAxisNo]->checkDone(tSystemTime, DEFAUL_MOVE_TIME_OUT))
		{
			ui.lineEditCurPositionRotate->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][rotateAxisNo]->curPosition()));
		}
	});

	connect(ui.pushButtonHomeRotate, &QPushButton::clicked, [this](){

		mpDMC5000Lib->mpDmcAxis[mCardNo][rotateAxisNo]->setHomePara(speedFromIni.rotateHomeSpeed);
		mpDMC5000Lib->mpDmcAxis[mCardNo][rotateAxisNo]->home();
		DWORD tSystemTime = GetTickCount();

		if (false == mpDMC5000Lib->mpDmcAxis[mCardNo][ccdAxisNo]->checkHome(tSystemTime, DEFAUL_HOME_TIME_OUT))
		{

		}
	});

	connect(ui.pushButtonPoweronRotate, &QPushButton::clicked, [this](){

		mpDMC5000Lib->mpDmcAxis[mCardNo][rotateAxisNo]->sevonOnOff(DMC_SERV_ENABLE);
	});

	connect(ui.pushButtonPoweroffRotate, &QPushButton::clicked, [this](){
		
		dmc_stop(mCardNo, rotateAxisNo, 0);
		mpDMC5000Lib->mpDmcAxis[mCardNo][rotateAxisNo]->sevonOnOff(DMC_SERV_DISABLE);
	});

	//UNLOAD
	connect(ui.pushButtonPjogUnload, &QPushButton::pressed, [this](){
		long curPosition = mpDMC5000Lib->mpDmcAxis[mCardNo][unloadAxisNo]->curPosition();
		mpDMC5000Lib->mpDmcAxis[mCardNo][unloadAxisNo]->setMovePara(speedFromIni.unloadJogSpeed, ABSOLUTE_MOTION);

		mpMOTIONLib->move(mCardNo, unloadAxisNo, curPosition + speedFromIni.jogPulse);

		ui.lineEditCurPositionUnload->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][unloadAxisNo]->curPosition()));

	});

	connect(ui.pushButtonNjogUnload, &QPushButton::pressed, [this](){
		long curPosition = mpDMC5000Lib->mpDmcAxis[mCardNo][unloadAxisNo]->curPosition();

		mpDMC5000Lib->mpDmcAxis[mCardNo][unloadAxisNo]->setMovePara(speedFromIni.unloadJogSpeed, ABSOLUTE_MOTION);
		mpMOTIONLib->move(mCardNo, unloadAxisNo, curPosition - speedFromIni.jogPulse);

		ui.lineEditCurPositionUnload->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][unloadAxisNo]->curPosition()));

	});

	connect(ui.pushButtonMoveUnload, &QPushButton::clicked, [this](){

		auto nPulse = ui.lineEditTargetPositionUnload->text().toInt();

		mpDMC5000Lib->mpDmcAxis[mCardNo][unloadAxisNo]->setMovePara(speedFromIni.unloadJogSpeed, ABSOLUTE_MOTION);
		mpMOTIONLib->move(mCardNo, unloadAxisNo, nPulse);
		DWORD tSystemTime = GetTickCount();

		while (1 == mpDMC5000Lib->mpDmcAxis[mCardNo][unloadAxisNo]->checkDone(tSystemTime, DEFAUL_MOVE_TIME_OUT))
		{
			ui.lineEditCurPositionUnload->setText(QString("%1").arg(mpDMC5000Lib->mpDmcAxis[mCardNo][unloadAxisNo]->curPosition()));
		}
	});

	connect(ui.pushButtonHomeUnload, &QPushButton::clicked, [this](){

		mpDMC5000Lib->mpDmcAxis[mCardNo][unloadAxisNo]->setHomePara(speedFromIni.unloadHomeSpeed);
		mpDMC5000Lib->mpDmcAxis[mCardNo][unloadAxisNo]->home();
		
		DWORD tSystemTime = GetTickCount();

		if (false == mpDMC5000Lib->mpDmcAxis[mCardNo][ccdAxisNo]->checkHome(tSystemTime, DEFAUL_HOME_TIME_OUT))
		{

		}
	});

	connect(ui.pushButtonPoweronUnload, &QPushButton::clicked, [this](){

		mpDMC5000Lib->mpDmcAxis[mCardNo][unloadAxisNo]->sevonOnOff(DMC_SERV_ENABLE);
	});

	connect(ui.pushButtonPoweroffUnload, &QPushButton::clicked, [this](){

		dmc_stop(mCardNo, unloadAxisNo, 0);
		mpDMC5000Lib->mpDmcAxis[mCardNo][unloadAxisNo]->sevonOnOff(DMC_SERV_DISABLE);
	});

}

