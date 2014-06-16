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

typedef vector<int> Code;

vector<byte> compressIndexStream(Code indexStream, int minCodeSize) {
	vector<int> bitStream;
	vector<byte> outStream;
	int codeSize=minCodeSize;

	for(int i=0; i<indexStream.size(); i++) {
		while(	pow(2,codeSize)<= indexStream[i] ) {codeSize++; }
		for(int j=0; j<codeSize; j++) {
			int bit=((indexStream[i] & (1<<j)) == pow(2,j));
			bitStream.push_back(bit);
		}
		cout << codeSize << ":" << indexStream[i] << "=";
		for(int j=codeSize-1; j>=0; j--) {
			int bit=((indexStream[i] & (1<<j)) == pow(2,j));
			cout << bit;
		}
		cout << endl;
	}

	byte code=0;
	for(int i=0; i<bitStream.size(); i++) {
		code+=bitStream[i]*pow(2,(i+0)%8);
		cout << bitStream[i]*pow(2,i%8) << "+";
		if(i%8==7) {
			outStream.push_back(code);
			cout << "=" << (int) code << endl;
			code=0;
		}
	}	
	return outStream;
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
		cout << "colour->" << (int) (*it)[0] << "," << (int) (*it)[1] << "," << (int) (*it)[2] << endl;
	}

	// translate pixels from image into an stream of indices relating to the values in the colour table
	byte colourTableSize=(byte) ceil(log(tableSize)/log(2)-1);
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
	int clearCode=pow(2,tableSize);
	int eoiCode=(clearCode+1);
	int freeCode=(eoiCode+1);
	byte lzwMinCodeSize=(byte) max(2.0,floor(log(eoiCode)/log(2)));
	cout << "clearCode=" << clearCode << endl;
	cout << "eoiCode=" << eoiCode << endl;
	cout << "first freeCode=" << freeCode << endl;

	// write the indexes that could be found in the indexStream
	map<Code,int> codeList;
	for(int i=0; i<freeCode; i++) {
		codeList[Code(1,i)]=i;
	}

	// lzw compression
	Code lzwList(1,clearCode);
	Code inputBuffer(1,indexStream[0]);
	int maxUsedCode=0;
	for(int i=1; i<width*height; i++) {
		int newIndex=indexStream[i];	
		Code newBuffer=inputBuffer;
		newBuffer.push_back(newIndex);
		if(codeList.count(newBuffer)==0) {
			int cd=codeList[inputBuffer];
			lzwList.push_back(cd);
			maxUsedCode=(cd > maxUsedCode)? cd: maxUsedCode;
			codeList[newBuffer]=freeCode++;
			inputBuffer=Code(1,newIndex);
		} else {
			inputBuffer=newBuffer;
		}
	}
	lzwList.push_back(codeList[inputBuffer]);
	lzwList.push_back(eoiCode);


	cout << "lzwMinCodeSize=" << (int) lzwMinCodeSize << endl;
	vector<byte> lzwCompress= compressIndexStream(lzwList, lzwMinCodeSize);
	
	byte lzwSize=lzwCompress.size();
	byte lzw[lzwSize];
	int i=0;

	for(vector<byte>::iterator it=lzwCompress.begin(); it!=lzwCompress.end(); ++it) {
		lzw[i++]=(*it);
	}

	gif::writeImageDescriptor(x,y, width, height, 1, 0, 0, colourTableSize);
	gif::writeColourTable(tableSize, colourTable);
	gif::writeTableBasedImageData(lzwMinCodeSize, lzwSize, lzw);
}
