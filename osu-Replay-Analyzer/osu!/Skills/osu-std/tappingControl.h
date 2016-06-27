#ifndef OSUSTANDARD_TAPPINGCONTROL_H
#define OSUSTANDARD_TAPPINGCONTROL_H

#include "../../osu_standard.h"
#include "../../Filestructure/Play.h"
#include "../../../utils/mathUtils.h"


namespace OSUSTANDARD
{
	namespace TAPPING_CONTROL
	{
		extern std::vector<OSUSTANDARD::TIMING> scores, skills, diffs;

		void genScore(Play* _play);
		void genSkill(Play* _play);
		void genDiff(Play* _play);

		double getScore();
		double getSkill();
		double getDiff();

		double getScoreAt(int _time);
		double getSkillAt(int _time);
		double getDiffAt(int _time);

		double getTotalScoreAt(int _time);
		double getTotalSkillAt(int _time);
		double getTotalDiffAt(int _time);
	};
};

#endif