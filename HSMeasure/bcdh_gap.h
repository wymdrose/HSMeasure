#ifndef BCDH_GAP_H
#define BCDH_GAP_H

#include <QObject>
#include <array>
#include "hsmeasure.h"

class BCDH_gap : public QObject
{
	Q_OBJECT

public:
	BCDH_gap(HSMeasure* pHSMeasure);
	~BCDH_gap();


	struct GAP_POS
	{
		float ccdPos;
		float platPos;
	};

	std::array<GAP_POS, GapJ + 1> arrayGapPos;

	bool getPosFromCfg();
	bool getGapValue(SLOTPOS_NO, GAP_NO, float&);
	bool getGapValue(const GAP_NO gapNo, float gapValue[]);
	bool getGapValue();	

signals:
	void showMsgSignal(const QString&);
	void showDialogSignal(const QString&);

private:
	HSMeasure* mpHSMeasure;
};

#endif // BCDH_GAP_H
