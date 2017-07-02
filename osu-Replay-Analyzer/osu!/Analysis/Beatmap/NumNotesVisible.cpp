#include "NumNotesVisible.h"

#include "../../osuCalc.h"
#include "../../osu_standard.h"
#include "../../osu_mania.h"

#include "../../../utils/Geometry.h"

Analyzer_NumNotesVisible::Analyzer_NumNotesVisible() : Analyzer("Number of objects visible") {}
Analyzer_NumNotesVisible::~Analyzer_NumNotesVisible() {}

void Analyzer_NumNotesVisible::AnalyzeStd(Play* _play)
{
	Database<Hitobject>& hitobjects = _play->beatmap->getHitobjects();
	osu::TIMING timing;
		timing.data = 0;

	double arMs = AR2ms(_play->getMod()->getAR());
	double csPx = CS2px(_play->getMod()->getCS());

	osu::TIMING currTime;
		currTime = OSUSTANDARD::getNextTickPoint(hitobjects, hitobjects[0]->getTime() - 1);

	while(currTime.time < hitobjects[hitobjects.size() - 1]->getTime())
	{
		int numvisible = OSUSTANDARD::getNumHitobjectsVisibleAt(_play, hitobjects.Find(currTime.time, true)[0], 0.3);

		timing.data = numvisible;
		timing.pos = currTime.pos;
		timing.press = false;
		timing.time = currTime.time;

		data.push_back(timing);

		currTime = OSUSTANDARD::getNextTickPoint(hitobjects, currTime.time);
		if (currTime.isInvalid()) break;
	}
}

void Analyzer_NumNotesVisible::AnalyzeCatch(Play* _play){}

void Analyzer_NumNotesVisible::AnalyzeTaiko(Play* _play){}

void Analyzer_NumNotesVisible::AnalyzeMania(Play* _play){}

void Analyzer_NumNotesVisible::AnalyzeDodge(Play* _play)
{

}