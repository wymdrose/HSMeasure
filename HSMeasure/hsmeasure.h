#ifndef HSMEASURE_H
#define HSMEASURE_H

#include <QtWidgets/QMainWindow>
#include "ui_hsmeasure.h"
#include "laseredit.h"
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
#include <QGridLayout>
#include <QMetaType>
#include <memory>
#include <QQueue>
#include <array>

#include "define.h"
#include "defineIo.h"

#include "Lib/motionLib.hpp"
#include "Lib/communicateLib.hpp"

class HSMeasure;

class MY_THREAD : public QObject
{
	Q_OBJECT
public:
	MY_THREAD(HSMeasure* pHSMeasure) : mpHSMeasure(pHSMeasure)
	{
		this->moveToThread(&mThread);
		mThread.start();

		connect(this, &MY_THREAD::finished, &mThread, &QThread::quit);
		connect(this, &MY_THREAD::finished, this, &MY_THREAD::deleteLater);
		connect(&mThread, &QThread::finished, &mThread, &QThread::deleteLater);
		

	}
	~MY_THREAD()
	{
		qDebug() << "~MY_THREAD()";
	}

	void start()
	{
		mThread.start();
	}

signals:
	void finished();
public slots:
	void run();
	void fun();
public:
	
	QMutex		mMutex;
	QThread		mThread;
private:
	
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
	void login();
	void hsmeasure_para();
	void showValueToUi(const FIX_VALUE&);
	void showLogToUi();
	void saveValueToLog(const FIX_VALUE&);

	bool ioOut(const QString, const WORD);
	short ioGet(const QString);
	void cylinderMove(const IO_CYLINDER);
	short cylinderCheck(const IO_CYLINDER);
	void sensorOut(const IO_SENSOR, const short);
	short sensorIn(const IO_SENSOR);

	void initUiIo();
	void load_ini();

	void ok_ngResult();
	bool okUnload();
	bool ngUnload();
	void caseOpposite();
	inline void caseFlow1();
	inline void caseFlow2();
	inline void caseFlow3();
	void caseFlow4();
	int curStateFlow1 = -1;
	int curStateFlow2 = -1;
	int curStateFlow3 = -1;
	int curStateFlow4 = -1;
	int stepFiberFlag[SlotposNo4 + 1];
	
	int modeRunNullFlag;
	//
	_MOTIONCLASS::DMC5000Lib* mpDMC5000Lib = new _MOTIONCLASS::DMC5000Lib(1,8);
	_MOTIONCLASS::DMC5000Lib* mpMOTIONLib = mpDMC5000Lib;

	_MOTIONCLASS::MOTION_CARD_TYPE mCardType = _MOTIONCLASS::MOTION_CARD_TYPE::DMC5000;

	//
	std::shared_ptr<_COMMUNICATECLASS::TCP_CLIENT> mpTcpClientCcd[2];
	std::shared_ptr<_COMMUNICATECLASS::COM_PORT_ONE> mpSerialportCcd[2];
	std::shared_ptr<_COMMUNICATECLASS::TCP_CLIENT> mpTcpClientLaser[3];
	
	//ר��IO
	QCheckBox* pBitInS[AXIS_NUM][IO_A_NUM];
	QCheckBox* pBitOutS[AXIS_NUM][IO_A_NUM];

	//ͨ��IO
	QCheckBox* pBitInM[IO_BIT_NUM];
	QCheckBox* pBitOutM[IO_BIT_NUM];

	QCheckBox* pBitInG[IO_EX_NUM][IO_BIT_NUM];
	QCheckBox* pBitOutG[IO_EX_NUM][IO_BIT_NUM];
	
	//	
	bool flagSlot[SlotposNo4 + 1];

	FIX_VALUE fixValue;
	float offsetGap_L;
	float offsetGap_H;
	float offsetStep_L;
	float offsetStep_H;
	std::array<std::array<float, GapJ + 1>, SlotposNo4 + 1> gapValues;
	std::array<std::array<float, StepJ + 1>, SlotposNo4 + 1> stepValues;
	QQueue<std::array<std::array<float, GapJ + 1>, SlotposNo4 + 1>> gapQueue;
	QQueue<std::array<std::array<float, StepJ + 1>, SlotposNo4 + 1>> stepQueue;
	
	QQueue<int> ngTypeQueue;

	int mCardNo = 0;
	QTimer *pIoTimer;
	
	int ccdAxisNo = 0;
	int laserAxisNo = 1;
	int platformAxisNo = 2;
	int rotateAxisNo = 3;
	int unloadAxisNo = 4;
	SPEED_FROM_INI speedFromIni;

	long LaserSafePosition;
	long loadPositionY1;
	long unloadPositionY1;
	long loadPositionY2;
	long unloadPositionY2;
	long stepPositionY2;
	long ngPositionY2;

	bool bNext;
	bool mbPause = false;
	bool mbStop = false;
	QStringList mStatusList;
	
	std::shared_ptr <MY_THREAD> pWorkThread;
	std::shared_ptr <MY_THREAD> pIoThread;

	//
	laserEdit* m_laserEditor[6];
	QLineEdit* m_laseWave[6][4];  //��������β���λ�ã���4����ȷ����start1��end1��start2��end2����6��������
	QLabel* m_laserName[6];
	QCheckBox* m_laserTrig[6];

	//����tabҳ��layout
	QVBoxLayout* m_mainLayout;
	QHBoxLayout* m_midBarLayout;
	QHBoxLayout* m_midBar2Layout;
	QHBoxLayout* m_topLayout;
	QHBoxLayout* m_midLayout;
	QVBoxLayout* m_bottomMainLayout;
	QHBoxLayout* m_midTopLayout;
	QHBoxLayout* m_midBotLayout;
	void deal_raw_data();

private:
	Ui::HSMeasureClass ui;
	void init();

	QMutex m_mutex;
	QMutex ioMutex;
	
	bool go_home();
	
	QString create_file_path(const QString & dir_, const QString & file_);
	void write_log_to_txt(const QString &file_name);
	void closeEvent(QCloseEvent *event);

	QStandardItemModel *model_result;

	private slots:
	void show_msg(const QString &msg);
	void show_dialog(const QString &msg);

	void save_ini();
	void save_log();

	void hs_start();
	void hs_pause();
	void hs_zero();
	void hs_stop();
	void hs_RestAlm();
	void hs_Mute();
	//
	void onIoTimer();
	void onTabWidgetChanged(int);
	void onCheckBoxIo();
signals:
	
};

#endif // HSMEASURE_H
