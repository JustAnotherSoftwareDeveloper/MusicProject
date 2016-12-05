

enum WaveType {SINE,SAWTOOTH,TRIANGLE,PULSE,OOPS};

#ifndef SOUNDGEN_H_
#define SOUNDGEN_H_

#include <string>
#include <vector>
#ifndef MUSICFILE_H_
#include "MusicFile.h"
#endif
class MusicFile;
class SoundGen {
public:
	SoundGen(int argc,char** argv);
	virtual ~SoundGen();
	bool genOutput();
	bool validateInput();
	void write();
	std::string toString();
private:
	int bitdepth,max,min;
	long freq,sampleRate; //frequency is in Hz.
	float peak,attack,decay,sustain,release,duration,pf; //Are we putting in floats as possible seconds values? I'm assuming so
	WaveType wave;
	std::string filename;
	MusicFile* output;
	std::vector<int>* data; //Channel will be 1
	double* unrounded_data;
	void genSine(int nSamples);
	void genTriangle(int nSamples);
	void genSawtooth(int nSamples);
	void genPulse(int nSamples);


};

#endif /* SOUNDGEN_H_ */
