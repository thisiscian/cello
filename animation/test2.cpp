#include <cello.hpp>
#include<iostream>
using namespace std;
using gif::gout;

int main(int argc, char* argv[]) {
	gif::init("hello.gif",10,10);
	Frame frame;
	for(int i=0; i<3; i++) {
		for(int j=0; j<5; j++) {
			frame.pixels[3*i*10+3*j+0]=255;
			frame.pixels[3*i*10+3*(j+5)+2]=255;
		}
	}
	for(int i=3; i<7; i++) {
		for(int j=0; j<3; j++) {
			frame.pixels[3*i*10+3*j+0]=255;
			frame.pixels[3*i*10+3*(j+7)+2]=255;
			frame.pixels[3*i*10+3*(j+3)+0]=255;
			frame.pixels[3*i*10+3*(j+3)+1]=255;
			frame.pixels[3*i*10+3*(j+3)+2]=255;
		}
		frame.pixels[3*i*10+3*(3+3)+0]=255;
		frame.pixels[3*i*10+3*(3+3)+1]=255;
		frame.pixels[3*i*10+3*(3+3)+2]=255;
	}
	for(int i=7; i<10; i++) {
		for(int j=0; j<5; j++) {
			frame.pixels[3*i*10+3*j+0]=255;
			frame.pixels[3*i*10+3*(j+5)+2]=255;
		}
	}

	for(int i=0; i<100; i++) {
		if(frame.pixels[3*i]==0) { cout << "B"; }
		else if(frame.pixels[3*i+2]==0) { cout << "R"; }
		else if(frame.pixels[3*i+1]==255) { cout << "W"; }
		else { cout << "?"; }
		if(i%10==9) { cout << endl; }
	}

	frame.draw();
	gout.close();
	return 0;
}
