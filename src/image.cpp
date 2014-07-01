#include <cello/image.h>
#include <iostream>
#include <iomanip>
using std::cout;
using std::endl;
using std::map;

Image::Image(Byte* input, size_t w, size_t h, size_t f) {
	width(w);
	height(h);
	frameWidth(f);	
	left(0);
	top(0);
	interlace(false);
	sort(false);
	data=input;

	_indexData=new Byte[_height*_width];
	for(int i=0; i<height(); i++) {
		for(int j=0; j<width(); j++) {
			int k=colourMap.add( &(input[3*(i*frameWidth()+j)]) );
			if(k>=0) { _indexData[i*width()+j]=k; }	
			else { cout << "ERROR::" << endl; }
		}
	}

	_minimumCodeSize=(Byte) pow(2,ceil(log(colourMap.size())/log(2)));
}

void Image::left(size_t left) { _left=left; };
void Image::top(size_t top) { _top=top; };
void Image::width(size_t width) { _width=width; };
void Image::height(size_t height) { _height=height; };
void Image::frameWidth(size_t frameWidth) { _frameWidth=frameWidth; };
void Image::minimumCodeSize(size_t minimumCodeSize) { _minimumCodeSize=minimumCodeSize; };
void Image::interlace(bool interlace) { _interlace=interlace; };
void Image::sort(bool sort) { _sort=sort; };

size_t Image::left() { return _left; }
size_t Image::top() { return _top; }
size_t Image::width() { return _width; }
size_t Image::height() { return _height; }
size_t Image::frameWidth() { return _frameWidth; }
size_t Image::minimumCodeSize() { return _minimumCodeSize; }
bool Image::interlace() { return _sort; }
bool Image::sort() { return _sort; }
size_t Image::table() { return (colourMap.size()!=0); }

Byte* Image::getIndexData() {
	return _indexData;
}

LZWStream Image::getCompressedData() {
	LZWStream output;
	map<Code,Byte> codeMap;
	Byte *indexStream=getIndexData();
	int indexSize=minimumCodeSize();
	Byte code=0;
	int bitMultiplier=0;

	// populate the codemap with all colours, and the clear & exit codes
	for(int i=0; i<colourMap.size(); i++) {
		codeMap[Code(1,i)]=i;
	}
	Byte clearCode=pow(2,colourMap.size());
	Byte exitCode=clearCode+1;
	Byte freeValue=exitCode+1;
	Code current(1,indexStream[0]);

	compressCode(output, clearCode, code, indexSize, bitMultiplier);
	for(int i=1; i<width()*height(); i++) {
		Code next=current;
		next.push_back(indexStream[i]);
		if(codeMap.count(next)!=0) {
			current=next;
		} else {
			compressCode(output, codeMap[current], code, indexSize, bitMultiplier);
			codeMap[next]=freeValue++;
			current=Code(1,indexStream[i]);
		}
	}
	compressCode(output, codeMap[current], code, indexSize, bitMultiplier);
	compressCode(output, exitCode, code, indexSize, bitMultiplier);
	
	if(code!=0) {
		compressCode(output, code, code, indexSize, bitMultiplier);
	}

	return output;
}

void Image::compressCode(LZWStream &output, Byte input, Byte &code, int &indexSize, int &bitMultiplier) {
	while(pow(2,indexSize)<=input) {indexSize++;}
	for(int j=0; j<indexSize; j++) {
		int bit=(input>>j) & 1;
		cout << bit;
		code+=bit<<bitMultiplier++;
		if(bitMultiplier==8) {
			cout << endl;
			bitMultiplier=0;
			output.push_back(code);
			cout << std::hex << "code->0x" << (int) code << std::dec << "(" << indexSize << ")" << endl;
			code=0;
		}
	}
//	cout << "input->" << std::hex <<  (int) input << std::dec << "(" << (int) code << "," << bitMultiplier << ")[" << indexSize << "]" << endl;
}
