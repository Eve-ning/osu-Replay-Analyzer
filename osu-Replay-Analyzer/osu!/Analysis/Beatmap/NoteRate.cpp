#include "NoteRate.h"
#include "../../../utils/geometry.h"

#include "../../osu_standard.h"
#include "../../osu_mania.h"

Analyzer_NoteRate::Analyzer_NoteRate() : Analyzer("note rate (notes/s)") {}
Analyzer_NoteRate::~Analyzer_NoteRate() {}

void Analyzer_NoteRate::AnalyzeStd(Play* _play)
{
	std::vector<Hitobject*>& hitobjects = _play->beatmap->getHitobjects();
	osu::TIMING timing;
	timing.data = 0;

	int i = 0, previ = 0;
	double delay = 10.0;

	for (int ms = 0; ms < hitobjects[hitobjects.size() - 1]->getTime(); ms += delay)
	{
		// Let index catch up to the time. Bail if out of bounds
		while (hitobjects[i]->getEndTime() < ms) i++;
		if (i >= hitobjects.size()) break;

		// Out of bound check
		if (!BTWN(1, i, hitobjects.size() - 1))
			continue;

		// previ - i needs to be at least 1 second apart
		while (hitobjects[i]->getTime() - hitobjects[previ]->getTime() > 1000.0)
		{
			if (previ < hitobjects.size() - 1) previ++;
			else break;
		}

		timing.data = i - previ;
		timing.pos = vector2df(hitobjects[i]->getPos().X, hitobjects[i]->getPos().Y);
		timing.press = false;
		timing.time = ms;

		data.push_back(timing);
	}
}

void Analyzer_NoteRate::AnalyzeCatch(Play* _play)
{

}

void Analyzer_NoteRate::AnalyzeTaiko(Play* _play)
{

}

void Analyzer_NoteRate::AnalyzeMania(Play* _play)
{
	std::vector<Hitobject*>& hitobjects = _play->beatmap->getHitobjects();
	int KEYS = _play->beatmap->getMods().getCS();
	
	osu::TIMING timing;
		timing.data = 0;

	long currTime = hitobjects[0]->getTime();
	std::vector<long> holdnoteEnds;
		holdnoteEnds.resize(KEYS);

	data.push_back({ currTime - 1, 0 }); // make the min be 0

	int noteCount = 0;

	for (int i = 0; i < hitobjects.size(); i++)
	{
		if (hitobjects[i]->isHitobjectLong())
			holdnoteEnds[OSUMANIA::getKey(hitobjects[i]->getPos().X, KEYS)] = hitobjects[i]->getEndTime();

		long hitobjectTime = hitobjects[i]->getTime();
		if (hitobjectTime > currTime)
		{
			noteCount++;
			if (i <= 1)
				noteCount--;

			// See if there are any hold notes at this time as well
			for (long holdnoteEnd : holdnoteEnds)
			{
				if (BTWN(currTime, hitobjectTime, holdnoteEnd))
					noteCount++;
			}

			timing.data = (double)noteCount / ((double)hitobjectTime - (double)currTime) * 1000.0;
			timing.time = hitobjectTime;

			// True = just presses, False = Presses and Releases
			timing.press = true;
			for (long holdnoteEnd : holdnoteEnds)
				timing.press &= (holdnoteEnd == hitobjectTime);

			data.push_back(timing);

			currTime = hitobjects[i]->getTime();
			noteCount = 0;
		}
		else
		{
			noteCount++;
		}
	}
}

void Analyzer_NoteRate::AnalyzeDodge(Play* _play)
{

}