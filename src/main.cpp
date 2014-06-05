#include <cello.hpp>
using namespace std;

void Frame::draw() {
	for(int i=0; i<cello::height*cello::width*3; i++) {
		pixels[i]=(cello::currentFrame%2==0)?(125+30*(i%3-2)):125;
	}
}

int main(int argc, char* argv[]) {
	Settings s(argc, argv);	
	cello::infiniteLoop=true;
	cello::period=2;
	writeAnimation();
	return 0;
}
