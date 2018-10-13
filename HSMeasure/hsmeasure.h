#ifndef HSMEASURE_H
#define HSMEASURE_H

#include <QtWidgets/QMainWindow>
#include "ui_hsmeasure.h"

#include <QCloseEvent>
#include <qfiledialog.h>
#include <qinputdialog.h>
#include <qdatetime.h>
#include <qsettings.h>
#include <qmutex.h>
#include <qmessagebox.h>
#include <qtextstream.h>
#include <qstandarditemmodel.h>
#include <qdebug.h>
#include <QMetaType>
#include "define.h"
#include "Lib/motionLib.hpp"
#include "Lib/communicateLib.hpp"

//#define

class HSMeasure;

class WORK_THREAD : public QObject
{
	Q_OBJECT
public:
	WORK_THREAD(HSMeasure* pHSMeasure) : mpHSMeasure(pHSMeasure)
	{
		mThread.start();
		this->moveToThread(&mThread);
	}
	~WORK_THREAD(){}

	protected slots:
	void run();
	//	void fun2();

public:
	QMutex		mMutex;
private:
	QThread		mThread;
	HSMeasure*	mpHSMeasure;
};

class HSMeasure : public QMainWindow
{
	Q_OBJECT

public:
	HSMeasure(QWidget *parent = 0);
	~HSMeasure();

	void initValue();
	void initDmcCard();
	void hsmeasure_para();
	void showValueToUi(const FIX_VALUE&);
	void showLogToUi();
	void saveValueToLog(const FIX_VALUE&);

	//
	_MOTIONCLASS::DMC5000Lib* mpDMC5000Lib = new _MOTIONCLASS::DMC5000Lib(1,8);
	_MOTIONCLASS::DMC5000Lib* mpMOTIONLib = mpDMC5000Lib;

	_MOTIONCLASS::MOTION_CARD_TYPE mCardType = _MOTIONCLASS::MOTION_CARD_TYPE::DMC5000;

	//
	_COMMUNICATECLASS::TCP_CLIENT* mpTcpClientCcd[2];
	_COMMUNICATECLASS::COM_PORT_ONE* mpSerialportCcd[2];
	//
	_COMMUNICATECLASS::TCP_CLIENT* mpTcpClientLaser[3];
	

	//
	
	bool flagSlot[SlotposNo4 + 1];
	FIX_VALUE fixValue;

	int mCardNo = 0;
	QTimer *pIoTimer;
	
	int ccdAxisNo = 0;
	int laserAxisNo = 1;
	int platformAxisNo = 2;
	int rotateAxisNo = 3;
	int unloadAxisNo = 4;

	long LaserSafePosition;
	long FeedingPosition;

	bool bNext;
	bool mbPause;
	bool mbStop;
	QStringList mStatusList;
	STATE_FLOW  mCurState = NULL_MEASURE;
	WORK_THREAD* pWorkThread;

private:
	Ui::HSMeasureClass ui;
	void init();
	void load_ini();

	QMutex m_mutex;
	
	
	bool go_home();
	
	QString create_file_path(const QString & dir_, const QString & file_);
	void write_log_to_txt(const QString &file_name);
	void closeEvent(QCloseEvent *event);



	QStandardItemModel *model_result;





	private slots:
	void show_msg(const QString &msg);
	void save_ini();
	void save_log();

	void hs_start();
	void hs_pause();
	void hs_zero();
	void hs_stop();
	//
	void onIoTimer();

signals:
	
};

#endif // HSMEASURE_H
