#ifndef PRESSRELEASEINTERVALS_H
#define PRESSRELEASEINTERVALS_H

#include "../Analyzer.h"

class Analyzer_PressReleaseIntervals : public Analyzer
{
public:
	Analyzer_PressReleaseIntervals();
	virtual ~Analyzer_PressReleaseIntervals();

	virtual void AnalyzeStd(Play* _play);
	virtual void AnalyzeCatch(Play* _play);
	virtual void AnalyzeTaiko(Play* _play);
	virtual void AnalyzeMania(Play* _play);
	virtual void AnalyzeDodge(Play* _play);
};


#endif
