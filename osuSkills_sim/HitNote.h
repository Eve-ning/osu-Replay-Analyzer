#ifndef HITNOTE_H
#define HITNOTE_H

#include <math.h>
#include <vector>
#include <tuple>

#include "GuiObj.h"
#include "Window.h"
#include "Hitobject.h"
#include "Beatmap.h"

class HitNote : public GuiObj
{
public:
	HitNote(Beatmap* _beatmap, Hitobject* _hitobject, int* _viewTime, double* _zoom);

private:
	Beatmap *beatmap;
	Hitobject *hitobject;
	int *viewTime;
	double *zoom;

	SColor edgeCol;

	enum STATE
	{
		IDLE = 0,
		HIGHLIGHTED = 1,
		SELECTED = 2
	}state;

	const SColor IDLE_COLOR = SColor(255, 150, 150, 150);
	const SColor HIGHLIGHTED_COLOR = SColor(255, 255, 255, 255);
	const SColor SELECTED_COLOR = SColor(255, 255, 100, 100);

	void IdleLogic(Window &_win);
	void HighlightedLogic(Window &_win);
	void SelectedLogic(Window &_win);

	virtual void UpdateInternal(Window &_win);
	virtual void UpdateAbsPos(Window& _win);
	virtual void UpdateAbsDim(Window& _win);

	virtual void Draw(Window &_win);
};

#endif