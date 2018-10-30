#include "bcdh_step.h"

extern QString mPath;

const BCDH_step::STEP_MAP BCDH_step::arrayStepMap[(SlotposNo4 + 1) * (StepJ + 1)] = {
	{ laserPos1, laserNo1 }, { laserPos1, laserNo6 },
	{ laserPos2, laserNo1 }, { laserPos2, laserNo6 }, { laserPos2, laserNo2 }, { laserPos2, laserNo5 },
	{ laserPos3, laserNo2 }, { laserPos3, laserNo5 },
	{ laserPos4, laserNo1 }, { laserPos4, laserNo6 }, { laserPos4, laserNo2 }, { laserPos4, laserNo5 },{ laserPos4, laserNo3 }, { laserPos4, laserNo4 },
	{ laserPos5, laserNo2 }, { laserPos5, laserNo5 }, { laserPos5, laserNo3 }, { laserPos5, laserNo4 },
	{ laserPos6, laserNo1 }, { laserPos6, laserNo6 }, { laserPos6, laserNo2 }, { laserPos6, laserNo5 }, { laserPos6, laserNo3 }, { laserPos6, laserNo4 },
	{ laserPos7, laserNo2 }, { laserPos7, laserNo5 }, { laserPos7, laserNo3 }, { laserPos7, laserNo4 },
	{ laserPos8, laserNo2 }, { laserPos8, laserNo5 }, { laserPos8, laserNo3 }, { laserPos8, laserNo4 },
	{ laserPos9, laserNo2 }, { laserPos9, laserNo5 }, { laserPos9, laserNo3 }, { laserPos9, laserNo4 },
	{ laserPos10, laserNo3 }, { laserPos10, laserNo4 },
	{ laserPos11, laserNo3 }, { laserPos11, laserNo4 },

};

const QStringList POS_STEP =
{
	"StepC_1", "StepH_1",
	"StepC_2", "StepH_2", "StepD_1", "StepG_1",
	"StepB_1", "StepI_1",
	"StepC_3", "StepH_3", "StepD_2", "StepG_2", "StepE_1", "StepF_1",
	"StepB_2", "StepI_2", "StepA_1", "StepJ_1",
	"StepC_4", "StepH_4", "StepD_3", "StepG_3", "StepE_2", "StepF_2",
	"StepB_3", "StepI_3", "StepA_2", "StepJ_2",
	"StepD_4", "StepG_4", "StepE_3", "StepF_3",
	"StepB_4", "StepI_4", "StepA_3", "StepJ_3",
	"StepE_4", "StepF_4",
	"StepA_4", "StepJ_4"
};

BCDH_step::BCDH_step(HSMeasure* pHSMeasure)
	: mpHSMeasure(pHSMeasure)
{
	connect(this, SIGNAL(showMsgSignal(const QString&)), mpHSMeasure, SLOT(show_msg(const QString &)));


	auto a = arrayStepMap[(SlotposNo3 + 1) * (StepD + 1)];
	
	m_pLoadDllfunc = CLoadLJDllFunc::GetInstance();

	laser1_config.IPAddress.S_un.S_un_b.s_b1 = 192;
	laser1_config.IPAddress.S_un.S_un_b.s_b2 = 168;
	laser1_config.IPAddress.S_un.S_un_b.s_b3 = 0;
	laser1_config.IPAddress.S_un.S_un_b.s_b4 = 20;

	laser2_config.IPAddress.S_un.S_un_b.s_b1 = 192;
	laser2_config.IPAddress.S_un.S_un_b.s_b2 = 168;
	laser2_config.IPAddress.S_un.S_un_b.s_b3 = 70;
	laser2_config.IPAddress.S_un.S_un_b.s_b4 = 10;

	laser3_config.IPAddress.S_un.S_un_b.s_b1 = 192;
	laser3_config.IPAddress.S_un.S_un_b.s_b2 = 168;
	laser3_config.IPAddress.S_un.S_un_b.s_b3 = 80;
	laser3_config.IPAddress.S_un.S_un_b.s_b4 = 10;

}

BCDH_step::~BCDH_step()
{
	qDebug() << "~BCDH_step()";
}

void BCDH_step::GetOneHeadData(LJIF_PROFILETARGET head, LJIF_OPENPARAM_ETHERNET config, float* buf)
{
	int iLSRet;
	int a = 0;
	LJIF_PROFILE_INFO profileInfo;

	iLSRet = m_pLoadDllfunc->LJIF_OpenDeviceETHER(&config);

	iLSRet = m_pLoadDllfunc->LJIF_GetProfileData(head, &profileInfo, buf, LSCOUNT);

	m_pLoadDllfunc->LJIF_CloseDevice();


	for (int i = 0; i < LSCOUNT; i++)
	{
		a = (_isnan((double)buf[i]));
		if (a)
		{
			buf[i] = -999.99;
		}

	}
}

void BCDH_step::GetTwoHeadData(LJIF_OPENPARAM_ETHERNET config, float* buf1, float* buf2)
{
	int iLSRet;
	int a = 0, b = 0;
	LJIF_PROFILE_INFO profileInfo;
	iLSRet = m_pLoadDllfunc->LJIF_OpenDeviceETHER(&config);

	iLSRet = m_pLoadDllfunc->LJIF_GetProfileData(LJIF_PROFILETARGET_HEAD_A, &profileInfo, buf1, LSCOUNT);

	iLSRet = m_pLoadDllfunc->LJIF_GetProfileData(LJIF_PROFILETARGET_HEAD_B, &profileInfo, buf2, LSCOUNT);

	m_pLoadDllfunc->LJIF_CloseDevice();


	for (int i = 0; i < LSCOUNT; i++)
	{
		a = (_isnan((double)buf1[i]));
		b = (_isnan((double)buf2[i]));
		if (a)
		{
			buf1[i] = -999.9999;
		}
		if (b)
		{
			buf2[i] = -999.9999;
		}

	}
}

bool BCDH_step::getPosFromCfg()
{
	QString fileName = mPath + "/cfg/stepPos.csv";

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

	for (size_t i = 1; i < CSVList.size(); i++)
	{
		QStringList oneLine = CSVList[i].split(",");

		float posLaser = oneLine[1].toFloat();
		posPlat = oneLine[2].toFloat();

		posLaserAxis[i - 1] = posLaser;
	}

	return true;
}

bool BCDH_step::getStepValue(QString posStep, float& stepValue)
{
	getPosFromCfg();

	auto a = POS_STEP.indexOf(posStep);

	showMsgSignal(QString("posLaserAxis[arrayStepMap[POS_STEP.indexOf(posStep)].laserPos]:%1").arg(posLaserAxis[arrayStepMap[POS_STEP.indexOf(posStep)].laserPos]));

	if (false == mpHSMeasure->mpMOTIONLib->mpDmcAxis[mpHSMeasure->mCardNo][mpHSMeasure->platformAxisNo]->moveAndCheckdone(posPlat, DEFAUL_MOVE_TIME_OUT))
	{
		return false;
	}
	
	if (false == mpHSMeasure->mpMOTIONLib->mpDmcAxis[mpHSMeasure->mCardNo][mpHSMeasure->laserAxisNo]->moveAndCheckdone(posLaserAxis[arrayStepMap[a].laserPos], DEFAUL_MOVE_TIME_OUT))
	{
		return false;
	}
	

	Sleep(300);
	
	//arrayStepMap[a].laserNo
	//
	float buf1[800];

	GetOneHeadData(LJIF_PROFILETARGET_HEAD_A, laser1_config, buf1);

	for (size_t i = 0; i < 800; i += 10)
	{
		if (0 == i % 100)
		{
			qDebug() << "\r";
		}

		qDebug() << buf1[i] << " " << buf1[i + 1] << " " << buf1[i + 2] << " " << buf1[i + 3] << " " << buf1[i + 4] << " " << buf1[i + 5] << " " << buf1[i + 6] << " " << buf1[i + 7] << " " << buf1[i + 8] << " " << buf1[i + 9];

	}

	stepValue = buf1[500];

	return true;
}

bool BCDH_step::getStepValue()
{

	float tValue(0);

	for (size_t i = 0; i < POS_STEP.size(); i++)
	{
		if (false == getStepValue(POS_STEP[i], tValue))
		{
			return false;
		}

		int slotNo = POS_STEP[i].right(1).toInt() - 1;

		QByteArray tByteArray = POS_STEP[i].mid(4, 1).toLocal8Bit();
		int stepNo = tByteArray[0] - 'A';

		mpHSMeasure->fixValue.plug[slotNo].step[stepNo] = tValue;
	}
	

	return true;
}
