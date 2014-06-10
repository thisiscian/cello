#include <cello.hpp>
using namespace std;

void Frame::draw() {
	int i;
	int x=cello::height*cello::width*1.5;
	int max=cello::height*cello::width*3;
	cout<< "cello::height:" << cello::height << " cello::width:" << cello::width  << "(" << x << "," << max << ")" <<endl;
	for(i=0; i<x; i+=3) {
		pixels[i]=0; // red
		pixels[i+1]=0; //blue
		pixels[i+2]=255; //green rbg?
	}
	for(i=x; i<max; i+=3) {
		pixels[i]=255;
		pixels[i+1]=0;
		pixels[i+2]=0;
	}
}

int main(int argc, char* argv[]) {
	Settings s(argc, argv);	
	cello::fileName="test1.gif";
	cello::infiniteLoop=true;
	cello::period=2;
	writeAnimation();
	return 0;
}
