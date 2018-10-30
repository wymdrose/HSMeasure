
#pragma once

#define AXIS_NUM  5
#define USA_UP_VAL  0.080
#define DEFAUL_HOME_TIME_OUT  100000
#define DEFAUL_MOVE_TIME_OUT  100000
#define DMC_SERV_ENABLE  0
#define DMC_SERV_DISABLE  1

#define IO_A_NUM 4		//一个轴专用IO数量
#define	IO_EX_NUM 3
#define IO_BIT_NUM 16

enum SLOTPOS_NO
{
	SlotposNo1, SlotposNo2, SlotposNo3, SlotposNo4
};

enum GAP_NO
{
	GapA, GapB, GapC, GapD, GapE, GapF, GapG, GapH, GapI, GapJ
};

enum LASER_AXIS_POS
{
	laserPos1, laserPos2, laserPos3, laserPos4, laserPos5, laserPos6, laserPos7, laserPos8, laserPos9, laserPos10, laserPos11
};

enum LASER_NO
{
	laserNo1, laserNo2, laserNo3, laserNo4, laserNo5, laserNo6
};


enum STEP_NO
{
	StepA, StepB, StepC, StepD, StepE, StepF, StepG, StepH, StepI, StepJ
};


struct ONE_PLUG_VALUE
{
	float gap[GapJ + 1];
	float step[StepJ + 1];
};

struct FIX_VALUE
{
	ONE_PLUG_VALUE plug[SlotposNo4 + 1];
};


enum
{
	NULL_MEASURE = 0, GAP_MEASURE, STEP_MEASURE, UNLOAD, FINISH
};

typedef unsigned int STATE_FLOW;

struct SPEED_FROM_INI
{
	double  ccdHomeSpeed;
	double	laserHomeSpeed;
	double	platHomeSpeed;
	double	rotateHomeSpeed;
	double	unloadHomeSpeed;

	double	ccdJogSpeed;
	double	laserJogSpeed;
	double	platJogSpeed;
	double	rotateJogSpeed;
	double	unloadJogSpeed;
	int		jogPulse;

	double	ccdAutoSpeed;
	double	laserAutoSpeed;
	double	platAutoSpeed;
	double	rotateAutoSpeed;
	double	unloadAutoSpeed;
};


