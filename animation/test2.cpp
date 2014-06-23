#include<cello.h>
#include<iostream>
#include<iomanip>
using namespace std;

int main(int argc, char* argv[]) {
	int w=10, h=10;

	Cello cello;
	cello.mode=CUSTOM;
	cello.start("test2.gif",w,h);

	for(int i=0; i<3; i++) {
		for(int j=0; j<5; j++) {
			cello.currentFrame.pixels[i*w+j].colour(255,0,0);
			cello.currentFrame.pixels[i*w+j+5].colour(0,0,255);
		}
	}
	for(int i=3; i<7; i++) {
		for(int j=0; j<3; j++) {
			cello.currentFrame.pixels[i*w+j].colour(255,0,0);
			cello.currentFrame.pixels[i*w+j+7].colour(0,0,255);
		}
		for(int j=0; j<4; j++) {
			cello.currentFrame.pixels[i*w+j+3].colour(255,255,255);
		}
	}
	for(int i=7; i<10; i++) {
		for(int j=0; j<5; j++) {
			cello.currentFrame.pixels[i*w+j].colour(255,0,0);
			cello.currentFrame.pixels[i*w+j+5].colour(0,0,255);
		}
	}

	cello.store();
	cello.save();

	return 0;
}
