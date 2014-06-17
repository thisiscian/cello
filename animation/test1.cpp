#include<cello.h>
#include<iostream>
#include<iomanip>
using namespace std;

int main(int argc, char* argv[]) {
	int w=10, h=10;
	cello::start("test1.gif",w,h);
	Frame frame;

	for(int i=0; i<h; i++) {
		for(int j=0; j<w; j++) {
			frame.pixels[i*w+j].colour(255,255,255);
		}
	}
	frame.draw();
	cello::stop();
	return 0;
}
