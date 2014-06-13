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

Frame::Frame() {
	width=cello::width;
	height=cello::height;
	pix = new byte[width*height*3]();
	pixels = new Pixel[height*width];
	for(int i=0; i<height; i++) {
		for(int j=0; j<width; j++) {
			pixels[i*width+j].set(&pix[3*(i*width+j)]);
		}
	}	
	delay=cello::delay;
}

typedef vector<int> code;

vector<char> convertCompressLZW(code lzw, int lzwMinCodeSize) {
	vector<int> b;
	vector<char> o;
	for(int i=0; i<lzw.size(); i++) {
		int j=max((int) log((lzw[i])/log(2)), lzwMinCodeSize);
		int k=lzw[i];
		cout << "converting " << k << " (" << j << ") to ";	
		for(int l=0; l<=j; l++) {
			b.push_back( (k & (1<<l)) == pow(2,l));
			cout << (int) ((k & (1<<l)) == pow(2,l));	
		}
		cout << endl;
	}
	int j=0;	
	byte O=0;
	for(int i=0; i<b.size(); i++) {
		O+=pow(2,j++)*b[i];
		if(j==8) {
			j=0;
			o.push_back(O);
			O=0;
		}
	}
	o.push_back(O);
	return o;
}


void Frame::draw() {
	byte r[3]={255,0,0}, wh[3]={255,255,255}, b[3]={0,0,255};
	Pixel red(r), white(wh), blue(b);

	gif::writeGraphicControlExtension(2,0,1,0,0x00);
	for(int i=0, m=min(height,16); i<height; m=min(height-i,16), i+=m) {
		for(int j=0, n=min(width,16); j<width; n=min(width-j,16), j+=n) {
			drawImage((byte *) &(pix[3*(i*width+j)]), m,n, width, height, i,j);
		}
	}
}


void drawImage(byte* p, int width, int height, int fw, int fh, int x, int y) {
	int indexStream[width*height]={};
	byte pix[width*height*3]={}, colourTable[256*3]={};
	Pixel pixels[width*height], colours[256];
	set<Pixel> uniqueColours;

	// create array of subimage pixels
	for(int i=0; i<height; i++) {
		for(int j=0; j<width; j++) {
			pix[3*(i*width+j)+0]=p[3*((i+y)*fw+j+x)+0];
			pix[3*(i*width+j)+1]=p[3*((i+y)*fw+j+x)+1];
			pix[3*(i*width+j)+2]=p[3*((i+y)*fw+j+x)+2];
			pixels[i*width+j].set(&pix[3*(i*width+j)]);
		}
	}

	// similarly, initialise colours list
	for(int i=0; i<256; i++) {
		colours[i].set(&colourTable[3*i]);
	}


	// collect unique colours from image
	for(int i=0; i<height; i++) {
		for(int j=0; j<width; j++) {
			uniqueColours.insert(pixels[i*width+j]);
		}
	}
	
	// create colour table, with the first entry being the transparent value
	colourTable[0]=255;
	colourTable[1]=0;
	colourTable[2]=255;
	int tableSize=1;
	for(set<Pixel>::iterator it=uniqueColours.begin(); it!=uniqueColours.end(); ++it) {
		colours[tableSize++].colour(*it);
	}

	// translate pixels from image into an stream of indices relating to the values in the colour table
	byte colourTableSize=(byte) ((int) ceil(log(tableSize-1)/log(2)));
	for(int i=0; i<height; i++) {
		for(int j=0; j<width; j++) {
			for(int k=0; k<pow(2,colourTableSize+1); k++) {
				if(colours[k]==pixels[i*width+j]) {
					indexStream[i*width+j]=k;
					break;
				}
			}
		}
	}
	
	// we need these variables for the gif
	byte lzwMinCodeSize=8; // this defines how much precision we can expect from the definition of each pixel, 8 is the easiest
	int clearCode=pow(2,lzwMinCodeSize);
	int eoiCode=(clearCode+1);
	int freeCode=(eoiCode+1);
	cout << "lzwMinCodeSize=" << (int) lzwMinCodeSize << endl;
	cout << "clearCode=" << clearCode << endl;
	cout << "eoiCode=" << eoiCode << endl;
	cout << "first freeCode=" << freeCode << endl;

	// write the indexes that could be found in the indexStream
	map<code,int> codeList;
	for(int i=0; i<clearCode; i++) {
		codeList[code(1,i)]=i;
	}

	// lzw compression
	code lzwList(1,clearCode);
	code inputBuffer(1,indexStream[0]);
	for(int i=1; i<width*height; i++) {
		int newIndex=indexStream[i];	
		code newBuffer=inputBuffer;
		newBuffer.push_back(newIndex);
		if(codeList.count(newBuffer)==0) {
			lzwList.push_back(codeList[inputBuffer]);
			cout << "freeCode was " << freeCode;
			codeList[newBuffer]=freeCode++;
			cout << " and is now " << freeCode << " (" << lzwList[lzwList.size()-1] << ")" << endl;
			inputBuffer=code(1,newIndex);
		} else {
			inputBuffer=newBuffer;
		}
	}
	lzwList.push_back(codeList[inputBuffer]);
	lzwList.push_back(eoiCode);

	vector<char> lzwCompress= convertCompressLZW(lzwList, lzwMinCodeSize);
	
	byte lzwSize=lzwCompress.size();
	byte lzw[lzwSize];
	int i=1;

	for(vector<char>::iterator it=lzwCompress.begin(); it!=lzwCompress.end(); ++it) {
		lzw[i++]=(*it);
	}

	gif::writeImageDescriptor(x,y, width, height, 1, 0, 0, colourTableSize);
	gif::writeColourTable(tableSize, colourTable);
	gif::writeTableBasedImageData(lzwMinCodeSize, lzwSize, lzw);
}




