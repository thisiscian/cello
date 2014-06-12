#include<cello/frame.h>
#include<iostream>
using std::min;
using gif::gout;
using std::set;
using std::pair;

Frame::Frame() {
	width=cello::width;
	height=cello::height;
	pix = new unsigned char[width*height*3]();
	pixels = new Pixel[height*width];
	for(int i=0; i<height; i++) {
		for(int j=0; j<width; j++) {
			pixels[i*width+j].set(&pix[3*(i*width+j)]);
		}
	}	
	delay=cello::delay;
}

void Frame::draw() {
	gif::writeGraphicControlExtension(0xF9, 0x4, 0,0,0, delay, 0x00);
	for(int i=0, m=min(height,16); i<height; m=min(height-i,16), i+=m) {
		for(int j=0, n=min(width,16); j<width; n=min(width-j,16), j+=n) {
			drawImage((char *) &(pix[3*(i*width+j)]), m,n, width, height, i,j);
		}
	}
}

void drawImage(char* p, int width, int height, int fw, int fh, int x, int y) {
	char indexStream[width*height];
	char pix[width*height*3];
	Pixel pixels[width*height];
	/

	char colourTable[256*3];
	Pixel colours[256];
	for(int i=0; i<256; i++) {
		colours[i].set(&colourTable[3*i]);
	}

	set<Pixel> uniqueColours;

	for(int i=0; i<height; i++) {
		for(int j=0; j<width; j++) {
			pair<set<Pixel>::iterator, bool> tmp=uniqueColours.insert(pixels(i*width+j));
			int id=uniqueColours.size()-1;
			if(tmp.second) {
				colourTable[3*id]=(*(tmp.first))[0];
				colourTable[3*id+1]=(*(tmp.first))[1];
				colourTable[3*id+2]=(*(tmp.first))[2];
			}
			int k=0;
			set<Pixel>::iterator it=uniqueColours.begin();
			while (it!=tmp.first && it++!=uniqueColours.end()) {
				k++;
			}
			indexStream[i*width+j]=(char) k;
		}
	}
	
	std::cout << uniqueColours.size() << std::endl;

	char lzwMinCodeSize=ceil(log(uniqueColours.size())/log(2));
	char clearCode=pow(2,lzwMinCodeSize);
	char eoiCode=clearCode+1;

	gif::writeImageDescriptor(x,y, width, height, 1, 0, 0, lzwMinCodeSize);
	gif::writeColourTable(255, colourTable);
	gif::writeTableImageData(lzwMinCodeSize, width*height, pix);
}


