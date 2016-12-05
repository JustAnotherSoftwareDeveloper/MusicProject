#ifndef CS229FILE_H_
#include "CS229File.h"
#endif
#include "SoundGen.h"
#include<stdlib.h>
#include <math.h>
/*!
 * \brief Default Constructor for SoundGen. Sets all the data we need for this to work
 * \param argc passed from main
 * \param argv passed from main. You know this makes everything harder to test
 */
SoundGen::SoundGen(int argc, char** argv) {
	filename="cout";
	std::string arg;
	pf=.5;
	bool found[11]={false,false,false,false,false,false,false,false,false,false,false};
	for(int i=2;i<argc;i++) {
		arg=argv[i];
		if(arg=="-o") { //Literally a switch statement. For some reason those aren't working
			i++;
			if (i==argc) { throw "Argument Error: Output";}
			filename=argv[i];
		}
		else if (arg=="--bits") { //NONE OF THESE ARE AT ALL CASE SENSITIVE
			i++;
			found[0]=true;
			if (i>=argc) { throw "Argument Error: Bits";}
			bitdepth=atoi(argv[i]);
		}
		else if (arg=="--sr") {
			found[1]=true;
			i++;
			if (i>=argc) { throw "Argument Error: Sample Rate";}
			sampleRate=atol(argv[i]);
		}
		else if (arg=="-f") {
			found[2]=true;
			i++;
			if (i>=argc) { throw "Argument Error: Frequency";}
			freq=atol(argv[i]);
		}
		else if (arg=="-t") {
			found[3]=true;
			i++;
			if (i>=argc) { throw "Argument Error: Total Duration";}
			duration=atof(argv[i]);
		}
		else if (arg=="-v") {
			found[4]=true;
			i++;
			if (i>=argc) { throw "Argument Error: Peak Volume";}
			peak=atof(argv[i]);
		}
		else if (arg=="-a") {
			found[5]=true;
			i++;
			if (i>=argc) { throw "Argument Error:Attack Time";}
			attack=atof(argv[i]);
		}
		else if (arg=="-d") {
			found[6]=true;
			i++;
			if (i>=argc) { throw "Argument Error: Decay Time";}
			decay=atof(argv[i]);
		}
		else if (arg=="-s") {
			found[7]=true;
			i++;
			if (i>=argc) { throw "Argument Error: Sustain Volume";}
			sustain=atof(argv[i]);
		}
		else if (arg=="-r") {
			i++;
			found[8]=true;
			if (i>=argc) { throw "Argument Error: Release Time";}
			release=atof(argv[i]);
		}
		else if (arg=="--sine") {
			found[9]=true;
			if (i>=argc) { throw "Argument Error: WaveForm";} //I know I probably don't need these, but I'd rather hit an error
			wave=SINE;
		}
		else if (arg=="--triangle") {
			found[9]=true;
			if (i>=argc) { throw "Argument Error: WaveForm";} //than a segfault if I screw up
			wave=TRIANGLE;
		}
		else if (arg=="--sawtooth") {
			found[9]=true;
			if (i>=argc) { throw "Argument Error: WaveForm";}
			wave=SAWTOOTH;
		}
		else if (arg=="--pulse") {
			found[9]=true;
			if (i>=argc) { throw "Argument Error: WaveForm";}
			wave=PULSE;
		}
		else if (arg=="--pf") { //If I'm reading pf correctly, it's only used in  pulse, right?
			i++;
			found[10]=true;
			if (i>=argc) { throw "Argument Error: PulseFraction";}
			pf=atof(argv[i]);
		}
		else {
			std::cout<<"Illegal Argument for "<<arg<<std::endl;
			throw;
		}
	}
	//Make Sure Every Argument Was Found:
	for(int i=0; i<10;i++) {
		if (!found[i]) {
			std::cout<<"Argument Missing"<<std::endl;
			throw;
		}
	}
	output=NULL; //This'll be created Later
	unrounded_data=NULL; //Same
	data=new std::vector<int>();
	max=(pow(2,bitdepth)/2)-1;
	min=-1*max; //If bitdepth is something stupid it'll be caught in the MusicFile creation anyway

}
/*!
 * \brief Deconstructor. Short this time!
 */
SoundGen::~SoundGen() {
	delete output;
	delete data;
	delete unrounded_data;
}
/*!
 * \brief Validates the input that I just passed. Including bitdepth because why not
 */
bool SoundGen::validateInput() {
	/*
	 * This goes in order based of PDF specs
	 */
	switch(bitdepth) {
		case 4:
		case 8:
		case 16:
		case 32:
			//do nothing
			break;
		default:
			return false;
			break;
	}
	//Sample Rate...Just make sure its >=1?
	if (sampleRate<1) { return false;}
	//Frequency just has to not be 0 right? I mean it'd be impractical but possible for .1 Hz frequency
	if (freq<=0) { return false;}
	//Duration has to be greater than 0...and also something to do with the sameple rate, but I think that's solved in rounding
	//Ex: duration: .5 seconds. 1 sample per second. Problem?
	if (duration<0) { return false;} //If it's 0 we're just rolling with it.
	//Volume: 0 <= volume <=1
	if ((peak<0)||(peak>1)) {
		return false;
	}
	//Both attack and decay just have to be >0
	if (attack<0) { return false;}
	if (decay<0) { return false;}
	// 0<= volume <= 1
	if ((sustain<0)||(sustain>1)) { return false;}
	//release time > 0
	if (release<0) { return false;}
	//PF has to be 0 <= pf <=1 . Unless you're passing fractions as 1/2 . In which case I've i'm done
	if ((0>pf)||(pf>1)) { return false;}
	return true;
}
/*!
 * \brief returns sndInfo of music object
 */
std::string SoundGen::toString() {
	return output->toString();
}
/*!
 * \brief Writes the object to file
 */
void SoundGen::write() {
	output->writefile();
}
/*!
 * \brief Creates the Output file, data included.
 */
bool SoundGen::genOutput()  {
	output=new CS229File(1,sampleRate,bitdepth,filename);
	/*
	 * So First I gotta figure out the actual time for this. That means messing
	 * with the ADSR stuff based of precedence
	 */
	float oldDecay=decay;
	float oldAttack=attack; //These'll be used to computer the slope because of the specs saying that if it's shortened, the amplitude will be more/less
	if(release>=duration) { return true;} //So Empty Sound
	while(((release+attack+decay)>=duration)&&(decay>.5)) { //We're decreasing Decay First because it has the lowest precedence
		decay*=.9; //This'll decrease decay
	} //If it gets too low it'll just move on.
	while(((release+attack+decay)>=duration)&&(attack>.5)) {
		attack=attack*.9; //Decrease Attack
	}
	if((release+attack+decay)>=duration) {
		decay=0; //Completely cut out decay
	}
	if((release+attack+decay)>=duration) {
			attack=0; //Completely cut out attack
	}
	/*
	 * Okay so now I got the timing I have to Assign a number of samples to
	 * each phase, and the total.
	 * The way i'm doing it is that the number will be NOT cumulative.
	 */
	float sustainTime=duration-(release+attack+decay); //Now we finally get to set a duration
	long nSamples[5]; //Attack,Decay,Sustain,Release,Duration
	//I'm gonna have to round these to go further
	nSamples[0]=round(sampleRate*attack); //The extra sample is for the primer
	nSamples[1]=round(sampleRate*decay);
	nSamples[2]=round(sampleRate*sustainTime);
	nSamples[3]=round(sampleRate*release);
	nSamples[4]=round(sampleRate*duration);
	/*
	 * Okay so Now we have to figure out the slope for attack,decay,and release,
	 * Per the instructions, we're using the original attack and decay, and just having an incomplete slope.
	 */
	double slopes[3]; //Attack, Decay, Release. Rise: Volume, Run: Samples
	//Attack Goes from 0,0 -> peak,Samples
	slopes[0]=peak/(round(oldAttack*sampleRate));
	//Decay Goes from peak -> Sustain
	slopes[1]=-1*(peak-sustain)/(round(oldDecay*sampleRate));
	//Release Goes from sustain -> 0
	slopes[2]=-1*(sustain)/(nSamples[3]);
	//Now we start making the data;
	int xval;
	unrounded_data=(double*) malloc(sizeof(double)*(nSamples[4]+1));
	for(int i=1;i<=nSamples[4];i++) {
		if(i<nSamples[0]) { //Attack
			unrounded_data[i]=i*slopes[0];
		}
		else if (i<(nSamples[0]+nSamples[1])) { //Decay
			xval=i-nSamples[0];
			unrounded_data[i]=peak+(slopes[1]*(xval)); //This'll jump to peak if shortened
		}
		else if (i<(nSamples[0]+nSamples[1]+nSamples[2])) { //Sustain
			unrounded_data[i]=sustain; //This one just chills
		}
		else { //Release
			xval=i-(nSamples[0]+nSamples[1]+nSamples[2]);
			unrounded_data[i]=sustain+(slopes[2]*(xval)); //Should be 0 by the end
		}
		unrounded_data[i]*=max; //To scale the whole thing to the correct depth.
	}
	/*
	 * Now you're probably thinking 'Wow this kid's an idiot: he didn't use any of the waveforms!"
	 * Well you're only half right; I split my waveform generation up and put it after making my
	 * envelope. Without Further Ado:
	 */
	switch(wave) { //I'm doing this with a switch statement. Yes I know decorator would have been better
		case SINE:
			this->genSine(nSamples[4]);
			break;
		case SAWTOOTH:
			this->genSawtooth(nSamples[4]);
			break;
		case TRIANGLE:
			this->genTriangle(nSamples[4]);
			break;
		case PULSE:
			this->genPulse(nSamples[4]);
			break;
		case OOPS:
			return false;
			break;
	}
	/*
	 * So now the data is how we want it. The only thing left to do now is round it and add it.
	 */
	for(int i=0;i<nSamples[4];i++) {
		data->push_back(round(unrounded_data[i]));
	}
	std::vector<std::vector<int>*>* toadd=new std::vector<std::vector<int>* >();
	toadd->push_back(data);
	output->addData(toadd,nSamples[4],1);
	delete toadd; //This won't delete my data...hopefully.
	return true;
}
/*
 * \brief Creates A sine wave for soundgen
 * \param nSamples: the number of samples
 */
void SoundGen::genSine(int nSamples) {
	/*
	 * The First thing we gotta do is figure out the step (time for each sample), we can do this by
	 * dividing the total duration by number of samples.
	 *
	 * We also have to correspond the index to the duration. We do this by multiplying the time by the sampleRate;
	 */
	float step=duration/nSamples;
	/*
	 * This will multiply every single thing by a correct sine value
	 */
	float temp;
	int index;
	for(float i=0;i<=duration;i+=step) {
		index=round(sampleRate*i); //This shouldn't go wrong...but you might have random samples that are 'off'
		temp=unrounded_data[index]*sin(i*freq/(2*M_PI)); //If I got my trig right this should generate a sine wave with the correct frequency
		unrounded_data[index]=temp;
	}
}
/*!
 * \brief Creates a Sawtooth wave
 */
void SoundGen::genSawtooth(int nSamples) {
	float step=duration/nSamples; //See genSine()
	/*
	 * So the logic for Sawtooth is a bit more complex. Basically I gotta do ( i*something % 2) -1
	 * That something has to make I go from 0 -> 2 in the correct frequency.
	 * I THINK this is 2*freq. The logic being the slope is rise (0->2) over run ( 1/freq)
	 */
	float temp;
	int index;
	float mult;
	for(float i=0; i<=duration; i+=step) {
		index=round(sampleRate*i);
		mult=fmod(2*freq*i,2)-1;
		temp=unrounded_data[index]*mult;
		unrounded_data[index]=temp;
	}
}
/*!
 * \brief Creates a triangle wave
 */
void SoundGen::genTriangle(int nSamples) {
	float step=duration/nSamples;
	int index;
	/*
	 * So triangle should be an absolute value function. Something like
	 * |(4*i*freq % 4 ) -2 | -1 . The logic being that the absolute value will create the triangle
	 * the mod will keep it in range, the -2 will keep the amplitude in check, and the -1 will change the starting ps
	 */
	double temp;
	float mod;
	float mult;
	double toabs;
	for(float i=0; i<=duration;i+=step) {
		index=round(sampleRate*i);
		mod=2*i*freq;
		toabs=abs(fmod(mod,2*freq)-freq);
		mult=(toabs/((double).5*freq))-1;
		temp=unrounded_data[index]*mult;;
		unrounded_data[index]=temp;
	}
}
/*!
 * \brief  Creates a pulse wave
 */
void SoundGen::genPulse(int nSamples) {
	float step=duration/nSamples;
	int index;
	/*
	 * So if I'm understanding the pulse correctly, it's like the yoda of wave: up or down, there is no inbetween
	 * As such I think its (fmod(i,1/freq)) < (1/freq)*pf ? 1 : -1; So you have the period (1/freq) as a modulus to
	 * keep the frequency, then you determine down vs up by seeing if it's < the period times the fraction. so if pf =1,
	 * it's always up. if pf = .3, it's up a third of the time.
	 */
	float temp;
	int mult; //I'm breaking it up into two because I can't see myself not screwing this up
	for(float i=0; i<=duration;i+=step) {
		index=round(sampleRate*i);
		mult= (fmod(i, (1/freq)) < (pf/freq)) ? 1: -1;
		temp=unrounded_data[index]*mult;
		unrounded_data[index]=temp;
	}
}
