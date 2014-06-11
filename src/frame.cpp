#include<cello/frame.h>
#include<iostream>
#include<cmath>
using std::min;
Frame::Frame() {
	width=gif::width;
	height=gif::height;
	pixels = new unsigned char[width*height*3]();
	delay=gif::delay;
}

void Frame::draw() {
	for(int i=0, m=min(height,16); i<height; m=min(height-i,16), i+=m) {
		for(int j=0, n=min(width,16); j<width; n=min(width-j,16), j+=n) {
			std::cout << "on image[" << i*width+j+1 << "/" << width*height/(16*16)+1 << "]: " << i << "(" << m << ")," << j << "(" << n << ")" << std::endl;
			Image(&pixels[i*width+j], width, height).draw(i,j,m,n);
		}
	}
}


