#ifndef OSU_MANIA_RENDERER
#define OSU_MANIA_RENDERER

#include "Window.h"
#include "GuiObj.h"
#include "HitNote.h"

class OsuManiaRenderer : public GuiObj
{
	public:
		OsuManiaRenderer(Play* _play, int* _viewTime, GuiObj* _parent);
		virtual ~OsuManiaRenderer();

		enum Layer
		{
			VISIBLE   = 1 << 0,
			// non
			DENSITY   = 1 << 2,
			REPLAY	  = 1 << 3,
			TIMINGS   = 1 << 4,
			HITIMINGS = 1 << 5
		};

		void SetLayers(int _layer);

		int getStartTime();
		int getEndTime();

	private:
		void GenerateHitNotes();

		virtual void UpdateAbsDim(Window& _win);
		virtual void UpdateAbsPos(Window& _win);
		virtual void UpdateInternal(Window& _win);
		void UpdateUserControls(Window& _win);
		void UpdateZoom(Window& _win);

		virtual void Draw(Window& _win);

		void RenderVisible(Window& _win);
		void RenderDensities(Window& _win);
		void RenderReplay(Window& _win);
		void RenderTimings(Window& _win);
		void RenderHitTimings(Window& _win);

		int Time2px(double _time);
		int px2Time(double _px);

		std::vector<HitNote*> hitNotes;
		std::vector<bool> visiblityPool;

		Play* play;
		int* viewTime;
		Layer layerState;
		double zoom;
};

#endif