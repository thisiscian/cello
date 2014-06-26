#include <cello/image.h>
#include <iostream>
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

	colourMap=makeColourMap(input, width(), height(), frameWidth());
	minimumCodeSize((size_t) std::max(2.0,ceil(log(colourMap.size())/log(2))));

	_rawData=new Byte[3*width()*height()];
	for(int i=0; i<height(); i++) {
		for(int j=0; j<width(); j++) {
			_rawData[3*(i*width()+j)+0]=data[3*(i*frameWidth()+j)+0];
			_rawData[3*(i*width()+j)+1]=data[3*(i*frameWidth()+j)+1];
			_rawData[3*(i*width()+j)+2]=data[3*(i*frameWidth()+j)+2];
		}
	}

}

ColourMap Image::makeColourMap(Byte* input, size_t width, size_t height, size_t frameWidth) {
	ColourMap colorMap;
	for(int i=0; i<height; i++) {
		for(int j=0; j<width; j++) {
			colorMap.add( &(input[3*(i*frameWidth+j)]) );
		}
	}
	return colorMap;
};

IndexStream Image::makeIndexStream(Byte* data, size_t width, size_t height, size_t frameWidth, ColourMap colourMap) {
	IndexStream indexStream;
	int value;
	for(int i=0; i<height; i++) {
		for(int j=0; j<width; j++) {
			int k=colourMap.contains(&(data[3*(frameWidth*i+j)]));
			if(k>=0) { indexStream.push_back(k); }	
		}
	}

	IndexStream output;
	map<Code,Byte> codeMap;

	for(int i=0; i<colourMap.size()+2; i++) {
		codeMap[Code(1,i)]=i;
	}

	Byte freeValue=colourMap.size()+2;
	Code current(1,indexStream[0]);
	for(int i=1; i<indexStream.size(); i++) {
		Code next=current;
		next.push_back(indexStream[i]);
		if(codeMap.count(next)!=0) {
			current=next;
		} else {
			codeMap[current]=freeValue++;
			output.push_back(codeMap[current]);
			current=Code(1,indexStream[i]);
		}
	}
	output.push_back(codeMap[current]);
	return output;
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

Byte* Image::getRawData() { 
	return _rawData;
}

IndexStream Image::getCompressedData() {
	IndexStream stream=makeIndexStream(data, width(), height(), frameWidth(), colourMap);
	IndexStream output=compressIndexStream(minimumCodeSize(), stream);
	return output;
};

IndexStream Image::compressIndexStream(size_t minimumCodeSize, IndexStream stream) {
	int indexSize=minimumCodeSize;
	IndexStream outStream;
	Byte code=0;
	Byte bitMultiplier=0;
	for(int i=0; i<stream.size(); i++) {
		while(pow(2,indexSize)<=stream[i]) {indexSize++; }
		for(int j=0; j<indexSize; j++) {
			int bit=(stream[i]>>j) & 1;
			code+=bit<<bitMultiplier++;
			//code+=pow(2,bitMultiplier++)*bit; //bit<<bitMultiplier++;
			if(bitMultiplier==8) {
				bitMultiplier=0;
				outStream.push_back(code);
				code=0;
			}
		}
	}
	outStream.push_back(code);
	return outStream;
}
