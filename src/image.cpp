#include <cello/image.h>

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

IndexStream makeIndexStream(Byte* data, size_t width, size_t height, size_t frameWidth, ColourMap colourMap) {
	IndexStream output;
	for(int i=0; i<height; i++) {
		for(int j=0; j<width; j++) {
			int k=colourMap.contains(&(data[3*(frameWidth*i+j)]));
			if(k>0) { output.push_back(k); }
		}
	}
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
	Byte *output=new Byte[3*width()*height()];
	for(int i=0; i<height(); i++) {
		for(int j=0; j<width(); j++) {
			output[3*(i*width()+j)+0]=data[3*(i*frameWidth()+j)+0];
			output[3*(i*width()+j)+1]=data[3*(i*frameWidth()+j)+1];
			output[3*(i*width()+j)+2]=data[3*(i*frameWidth()+j)+2];
		}
	}
	return output;
}

IndexStream Image::getCompressedData() {
	IndexStream stream=makeIndexStream(data, width(), height(), frameWidth(), colourMap);
	return compressIndexStream(minimumCodeSize(), stream);
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
			if(bitMultiplier==8) {
				bitMultiplier=0;
				outStream.push_back(code);
				code=0;
			}
		}
	}
	return outStream;
}
