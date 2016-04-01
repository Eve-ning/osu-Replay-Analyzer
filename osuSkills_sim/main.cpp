#include <vector>

#include "irrlicht/include/irrlicht.h"
#include "Window.h"
#include "Hitcircle.h"
#include "Slider.h"
#include "DebugRender.h"
#include "SkillsCalc.h"
#include "mathUtils.h"
#include "osuStats.h"

using namespace std;

using namespace irr;
using namespace core;
using namespace video;
using namespace gui;

#ifdef _IRR_WINDOWS_
	#pragma comment(lib, "irrlicht/lib/Win32-visualstudio/Irrlicht.lib")
	//#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

void DrawDebug(Window &_win)
{
	double click = _win.reciever.GetMouseState().wheelMove;

	/*win.font->draw(core::stringw(pos.X) + ", " + core::stringw(pos.Y),
		core::rect<s32>(130, 10, 300, 50),
		video::SColor(255, 255, 255, 255));*/

	_win.font->draw(core::stringw(click),
		core::rect<s32>(10, 10, 100, 10),
		video::SColor(255, 255, 255, 255));
}


int main()
{
	const int RESX = 171+640;
	const int RESY =  50+480;

	Window win(RESX, RESY);
	win.device->setWindowCaption(L"osu!skill Formula Simulator");

	double CS = 4;
	double AR = 10.3;
	
	//int CS_px = 50;
	int time_ms = 0;	

	vector<Hitcircle> circles;
		circles =
		{ 
			//         x    y    t
			Hitcircle(231, 275, 300),
			Hitcircle(398, 362, 491),
			Hitcircle(389, 169, 682),
			Hitcircle(500, 300, 873),
			Hitcircle(373, 354, 1063),
			Hitcircle(440, 249, 1255),
			Hitcircle(342,  98, 1446),
			Hitcircle(175, 203, 1637),
			Hitcircle(312, 227, 2210),
			Hitcircle(312, 227, 2783),
			Hitcircle(272,  60, 3356),
			Hitcircle(240, 324, 3547),
			Hitcircle(272,  60, 3929),
			Hitcircle(120, 172, 4120),
			Hitcircle(388, 208, 4311)
		};

	Slider csSlider(660, 80, 90, 10);
		csSlider.setRange(0, 10);

	Slider arSlider(660, 120, 90, 10);
		arSlider.setRange(0, 11);

	Slider resSlider(660, 160, 90, 10);
		resSlider.setRange(0.155, 5);
		double res;

	while (win.device->run())
	{
		// update stuff
		AR = arSlider.getVal();
		CS = csSlider.getVal();
		res = resSlider.getVal();


		// skill calculation
		int time2index = getHitcircleAt(circles, time_ms) + 1;
		if (BTWN(1, time2index, circles.size() - 2))
			CalcChaos(circles, time2index, CS, AR);


		// mouse wheel time control
		double const step = -10; // amount of px to move by
		double newTime_ms = time_ms + (step / res) * win.reciever.getWheel();
		if (newTime_ms >= 0)
			time_ms = newTime_ms;


		// render stuff
		win.driver->beginScene(true, true, SColor(255, 0, 0, 0));

			win.driver->draw2DRectangleOutline(recti(0, 0, 640, 480));
			for (int i = 0; i < circles.size(); i++)
				circles[i].Draw(win, 0, 0, time_ms, AR, CS);
		
			//DrawLine(win, circles[0], circles[1]);
			if (BTWN(1, time2index, circles.size() - 2))
				DrawAngle(win, circles[time2index - 1], circles[time2index], circles[time2index + 1]);
			
			drawTimingGraphs(win, 0, 510, circles, true, time_ms, res, CS, AR);

			arSlider.Draw(win);
			csSlider.Draw(win);
			resSlider.Draw(win);
		
			DrawDebug(win);
		win.driver->endScene();
	}

	win.device->drop();
	return 0;
}
