#include<cello/frame.h>
#include<iostream>
#include<string>
#include<sstream>
using std::cout;
using std::endl;

using std::min;
using std::max;
using std::set;
using std::pair;
using std::vector;
using std::map;
using std::string;

void Frame::initialise(int w, int h, int d) {
	width=w;
	height=h;
	delay=d;

	pix=new byte[width*height*3]();
	pixels=new Pixel[height*width];
	for(int i=0; i<height; i++) {
		for(int j=0; j<width; j++) {
			pixels[i*width+j].set(&pix[3*(i*width+j)]);
		}
	}	

	for(int i=0, j=min(16,height); i<j; i++, j=min(16,height-j*16)) {
		for(int k=0, l=min(16,width); k<l; k++, l=min(16,width-k*16)) {
			images.push_back(Image(&(pix[3*(i*width+k)]), l, j, width));
		}
	}
}
