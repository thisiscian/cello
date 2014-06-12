#include<cello.h>
#include<iostream>
#include<iomanip>
using namespace std;
using gif::gout;

int main(int argc, char* argv[]) {
	int w=10, h=10;
	cello::start("test2.gif",w,h);
	Frame frame;

	for(int i=0; i<3; i++) {
		for(int j=0; j<w/2; j++) {
			frame.pixels[i*w+j    ].colour(255,0,0);
			frame.pixels[i*w+j+w/2].colour(0,0,255);
		}
	}
	for(int i=3; i<7; i++) {
		for(int j=0; j<3; j++) {
			frame.pixels[i*w+j].colour(255,0,0);
			frame.pixels[i*w+j+7].colour(0,0,255);
			frame.pixels[i*w+j+3].colour(255,255,255);
		}
		frame.pixels[i*w+6].colour(255,255,255);
	}
	for(int i=7; i<10; i++) {
		for(int j=0; j<5; j++) {
			frame.pixels[i*w+j    ].colour(255,0,0);
			frame.pixels[i*w+j+w/2].colour(0,0,255);
		}
	}

	for(int i=0; i<100; i++) {
		if(frame.pix[3*i]==0) { cout << "B"; }
		else if(frame.pix[3*i+2]==0) { cout << "R"; }
		else if(frame.pix[3*i+1]==255) { cout << "W"; }
		else { cout << "?"; }
		if(i%10==9) { cout << endl; }
	}

	frame.draw();
	cello::stop();
	return 0;
}
