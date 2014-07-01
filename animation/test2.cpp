#include<cello.h>
#include<iostream>
#include<iomanip>
using namespace std;

int main(int argc, char* argv[]) {
	int w=10, h=10;

	Cello cello;
	cello.mode=BOTH;
	cello.start("test2.gif",w,h);

	for(int i=0; i<h; i++) {
		for(int j=0; j<w; j++) {
			if( (i+j) % 2 == 0) {
				cello.currentFrame.pixels[i*w+j].colour(255,255,255);
			} else {
				cello.currentFrame.pixels[i*w+j].colour(0,0,0);
			}
		}
	}

	cello.store();
	cello.save();

	return 0;
}
