#include<cello/frame.h>
#include<iostream>
#include<string>
#include<sstream>
using std::cout;
using std::endl;
using std::min;
using std::set;
using std::pair;
using std::vector;
using std::map;
using std::string;

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

string tmp(vector<int> x) {
	std::stringstream s;
	for(int i=0; i<x.size(); i++) {
		s << (int) x[i];
	}
	return s.str();
}


void Frame::draw() {
	unsigned char r[3]={255,0,0}, wh[3]={255,255,255}, b[3]={0,0,255};
	Pixel red(r), white(wh), blue(b);

	gif::writeGraphicControlExtension(2,0,1,0,0x00);
	for(int i=0, m=min(height,16); i<height; m=min(height-i,16), i+=m) {
		for(int j=0, n=min(width,16); j<width; n=min(width-j,16), j+=n) {
			drawImage((char *) &(pix[3*(i*width+j)]), m,n, width, height, i,j);
		}
	}
}

typedef vector<int> code;

void drawImage(char* p, int width, int height, int fw, int fh, int x, int y) {
	char indexStream[width*height];
	char pix[width*height*3], colourTable[256*3];
	Pixel pixels[width*height], colours[256];
	set<Pixel> uniqueColours;
	for(int i=0; i<height; i++) {
		for(int j=0; j<width; j++) {
			pix[3*(i*width+j)+0]=p[3*((i+y)*fw+j+x)+0];
			pix[3*(i*width+j)+1]=p[3*((i+y)*fw+j+x)+1];
			pix[3*(i*width+j)+2]=p[3*((i+y)*fw+j+x)+2];
			pixels[i*width+j].set(&pix[3*(i*width+j)]);
		}
	}

	for(int i=0; i<256; i++) {
		colours[i].set(&colourTable[3*i]);
	}

	for(int i=0; i<height; i++) {
		for(int j=0; j<width; j++) {
			uniqueColours.insert(pixels[i*width+j]);
		}
	}
	
	int tableSize=1;
	colourTable[0]=255;
	colourTable[1]=0;
	colourTable[2]=255;
	for(set<Pixel>::iterator it=uniqueColours.begin(); it!=uniqueColours.end(); ++it) {
		colourTable[3*tableSize+0]=(*it)[0];
		colourTable[3*tableSize+1]=(*it)[1];
		colourTable[3*tableSize+2]=(*it)[2];
		colours[tableSize].set(&colourTable[3*tableSize]);
		tableSize++;
	}

	char lzwMinCodeSize=(char) ((int) ceil(log(tableSize-1)/log(2)));
	char clearCode=pow(2,lzwMinCodeSize+1);
	char eoiCode=clearCode+1;

	while(tableSize<pow(2,lzwMinCodeSize+1)) {
		colourTable[3*tableSize+0]=0;
		colourTable[3*tableSize+1]=0;
		colourTable[3*tableSize+2]=0;
		colours[tableSize].set(&colourTable[3*tableSize]);
		tableSize++;
	}

	for(int i=0; i<height; i++) {
		for(int j=0; j<width; j++) {
			for(int k=0; k<tableSize; k++) {
				if(colours[k]==pixels[i*width+j]) {
					indexStream[i*width+j]=(char) k;
					break;
				}
				indexStream[i*width+j]=0;
			}
		}
	}

	map<code,char> codeList;
	char freeCode;
	for(freeCode=0; freeCode<(char) (tableSize+2); freeCode++) {
		codeList[code(1,(int) freeCode)]=freeCode;
	}

	vector<char> lzwList;
	code inputBuffer(1,indexStream[0]);
	for(int i=1; i<width*height; i++) {
		char newIndex=indexStream[i];	
		code newBuffer=inputBuffer;
		newBuffer.push_back(newIndex);
		if(codeList.count(newBuffer)==0) {
			lzwList.push_back(codeList[inputBuffer]);
			codeList[newBuffer]=freeCode++;
			inputBuffer=code(1,newIndex);
		} else {
			inputBuffer=newBuffer;
		}
	}
	
	lzwList.push_back(codeList[inputBuffer]);


	char lzwSize=lzwList.size()+2;
	char lzw[lzwSize];
	lzw[0]=clearCode;
	lzw[lzwSize-1]=eoiCode;
	int i=1;

	for(vector<char>::iterator it=lzwList.begin(); it!=lzwList.end(); ++it) {
		lzw[i++]=(*it);
	}

	gif::writeImageDescriptor(x,y, width, height, 1, 0, 0, lzwMinCodeSize);
	gif::writeColourTable(tableSize, colourTable);
	gif::writeTableBasedImageData(lzwMinCodeSize, lzwSize, lzw);
}




