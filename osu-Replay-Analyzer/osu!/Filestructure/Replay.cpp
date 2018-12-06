#include "Replay.h"
#include "Beatmap.h"
#include "filereader.h"
#include "LZMA.h"
#include "../../utils/mathUtils.h"


#include <fstream>
#include <assert.h>

Replay::Replay(){}

Replay::~Replay()
{
	ClearReplay();
}

void Replay::ProcessReplay(std::string _filepath, Beatmap* _beatmap)
{
	// Can't have a replay without a beatmap
	if (_beatmap == nullptr)	return;
	if (!_beatmap->isValid())	return;

	std::ifstream replayFile(_filepath, std::ios::binary);
	if (replayFile.is_open())
	{
		bool success = this->ParseFile(replayFile);
		assert(success == true);
		replayFile.close();
	}

	// make sure the replay is of the same gamemode as the map
	if (!isGamemode(_beatmap->getGamemode()))
	{
		ClearReplay();
		return;
	}

	mod = _beatmap->getMods();
	this->ValidateMods();
	this->ApplyMods();

	/// \TODO: Make sure the stream data is ordered time-wise
}

osu::TIMING Replay::getFrameAt(long _time)
{
	if(replayStream.size() == 0)	return osu::TIMING();  // make sure there is a replay to search
	int frame = osu::FindTimingAt(replayStream, _time);

	if(_time == -1 || frame == -1)	return osu::TIMING();  // make sure we found something to return
	return replayStream[frame];
}

osu::TIMING Replay::getFrame(int _frame) const
{
	if(_frame < replayStream.size())
		return replayStream[_frame];
	else
		return osu::TIMING();
}

std::vector<osu::TIMING>& const Replay::getReplayStream()
{
	return replayStream;
}

int Replay::getNumFrames()
{
	return replayStream.size();
}

Mods Replay::getMods()
{
	return mod;
}

bool Replay::isValid()
{
	return (replayStream.size() != 0);
}

bool Replay::isBeatmap(std::string* _MD5)
{
	return (beatmapMD5 == *_MD5);
}

bool Replay::isGamemode(GAMEMODE _gamemode)
{
	return (gameMode == _gamemode);
}

// ------- [PRIVATE] --------

void Replay::ClearReplay()
{
	replayStream.clear();
	std::vector<osu::TIMING>().swap(replayStream);
}

std::string Replay::ReadOsuStr(std::ifstream &_replayFile)
{
	char chr;
	std::string str;

	// check if there is something to read first
	_replayFile.read(&chr, sizeof(chr));
	if (chr != '\0')
	{
		// If it begins with this, play by peppy string rules
		if (chr == 0x0b)
		{
			// get the first part of the peppy string; size of the string
			std::string size_str;
			do
			{
				_replayFile.read(&chr, sizeof(chr));
				size_str += chr;
			} while (chr < 0);

			// read the peppy string
			int size = decodeULEB128((const uint8_t*)size_str.data());
			char* tmp = new char[size+1];
				_replayFile.read(tmp, size);
				tmp[size] = '\0';
				str.assign(tmp);
			delete tmp;
		}
		else // otherwise, take it as a normal null terminated string
		{
			str += chr;
			_replayFile.read(&chr, sizeof(chr));
			
			while (chr != '\0')
			{
				str += chr;
				_replayFile.read(&chr, sizeof(chr));
			}
		}
	}

	return str;
}

bool Replay::ParseFile(std::ifstream &_replayFile)
{
	_replayFile.read(&gameMode,       sizeof(gameMode));
	_replayFile.read((char*)&version, sizeof(version));
	
	beatmapMD5 = ReadOsuStr(_replayFile);
	player     = ReadOsuStr(_replayFile);
	replayMD5  = ReadOsuStr(_replayFile);

	_replayFile.read((char*)&num300,  sizeof(num300));
	_replayFile.read((char*)&num200,  sizeof(num200));
	_replayFile.read((char*)&num50,   sizeof(num50));
	_replayFile.read((char*)&numMax,  sizeof(numMax));
	_replayFile.read((char*)&num100,  sizeof(num100));
	_replayFile.read((char*)&numMiss, sizeof(numMiss));
	_replayFile.read((char*)&score,   sizeof(score));
	_replayFile.read((char*)&combo,   sizeof(combo));
	_replayFile.read((char*)&perfect, sizeof(perfect));
	_replayFile.read((char*)&mods,    sizeof(mods));
	
	graph = ReadOsuStr(_replayFile);
	
	_replayFile.read((char*)&timestamp, sizeof(timestamp));


	// Stream data
	int replaySize;
		_replayFile.read((char*)&replaySize, sizeof(replaySize));
		
	char* data = new char[replaySize];
		_replayFile.read(data, replaySize);

	// Data generation from compressed stream data
	// \TODO: Put this in its own function
	LZMA::DataStream stream;
		stream.inData = (const unsigned char*)data;
		stream.inLen = replaySize;
		stream.outData = nullptr;
		stream.outLen = 0;

	LZMA::DecompressStreamData(&stream);
	delete data;

	//ParseReplayData(stream.outData, stream.outLen, gameMode);

	return true;
}

/*
std::string mods2str(MODS mods)
{
std::string modStr;

if (mods & EZ) modStr += "EZ";
if (mods & HR) modStr += "HR";
if (mods & HT) modStr += "HT";
if (mods & DT) modStr += "DT";
if (mods & FL) modStr += "FL";
if (mods & HD) modStr += "HD";

return modStr;
}
*/

void Replay::ParseReplayData(unsigned char* _data)
{
	std::string Replaydata = std::string((const char*)_data);
	osu::TIMING frame;
	int time = 0;
	int prevKey = 0;

	std::vector<std::string> dataPoints;
	FileReader::tokenize(Replaydata, dataPoints, ",");

	for (std::string dataPoint : dataPoints)
	{
		std::vector<std::string> data;
		FileReader::tokenize(dataPoint, data, "|");

		if (data.size() >= 4) // there must be at least 4 data indentifiers for a complete datapoint
		{
			time += atoi(data[0].data());

			frame.time = time;						// record time
			frame.pos.X = atoi(data[1].data());		// x pos of cursor
			frame.pos.Y = atoi(data[2].data());		// y pos of cursor
			frame.key = atoi(data[3].data());		// which key is being pressed
			frame.press = (frame.key != prevKey);	// key event flag

			if (frame.key != -12345) // Record if it's not this...
				replayStream.push_back(frame);

			prevKey = frame.key;
		}
	}
}

void Replay::ValidateMods()
{
	// If one of these asserts triggers, then someone has been hacking the replay
	assert(((this->mods & EZ) != true) && ((this->mods & HR) != true));
	assert(((this->mods & DT) != true) && ((this->mods & HT) != true));
	assert(((this->mods & HD) != true) && ((this->mods & FL) != true));

	mod.setTM(1.0);

	// Set mod flags
	if (mods & EZ) mod.setModifier(Mods::MODS::EZ);
	if (mods & HR) mod.setModifier(Mods::MODS::HR);
	if (mods & HT) mod.setModifier(Mods::MODS::HT);
	if (mods & DT) mod.setModifier(Mods::MODS::DT);
	if (mods & FL) mod.setModifier(Mods::MODS::FL);
	if (mods & HD) mod.setModifier(Mods::MODS::HD);
	if (mods & FI) mod.setModifier(Mods::MODS::FI);
}

void Replay::ApplyMods()
{
	// Get speed divisor
	double divisor = mod.getTM();
	if (divisor != 1.0)
	{
		// Adjust frame timings due to DT or HT mod
		for (auto &frame : replayStream)
			frame.time /= divisor;
	}

	if (mod.getModifier().HR)
	{

	}
}