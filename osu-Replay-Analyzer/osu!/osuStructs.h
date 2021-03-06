#ifndef OSUSTRUCTS_H
#define OSUSTRUCTS_H

#include <vector>
#include "../irrlicht/include/vector2d.h"

namespace osu
{
	struct TIMING
	{
		long time;		// The what time this occurs at
		double data;	// generic data associated with this timing
		int key;		// generic integral data (can be used for which key pressed, timing state, etc)
		bool press;		// generic boolean data (can be used to determine slider or not, pressed key or not, etc)

		irr::core::vector2df pos; // additional timing info for 2d coordinates
	};

	int FindTimingAt(std::vector<TIMING>& _timings, double _time);
	void SortByTime(std::vector<TIMING>& _timings);
};

#endif // !OSUSTRUCTS_H