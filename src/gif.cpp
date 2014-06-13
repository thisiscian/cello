#include<cello/gif.h>
#include<iomanip>
#include<iostream>
using std::string;
using std::set;
using std::min;
using gif::gout;
using std::cout;
using std::endl;

namespace gif {
	std::fstream gout;
}

void gif::writeDataSubBlock(char size, char* data) {
	gout << size;
	for(int i=0; i<(int) size; i++) {
		gout << *(data+i);
	}
}

void gif::writeBlockTerminator() {
	gout << (char) 0x00;
}

void gif::writeHeader(string version) {
	gout << "GIF" << version.substr(0,3);
}

void gif::writeLogicalScreenDescriptor(int width, int height, int table, int resolution, int sort, int tableSize, char backgroundColour, char pixelAspectRatio) {
	gout << (char) (width%256) << (char) (width/256);
	gout << (char) (height%256) << (char) (height/256);
	gout << (char) (128*table+(resolution-1)*16+sort*8+(tableSize-1));
	gout << backgroundColour << pixelAspectRatio;
}

void gif::writeColourTable(int size, char* table) {
	if(table!=NULL && size > 0) 
		gout.write(table, size*3);
}

void gif::writeImageDescriptor(int left, int top, int width, int height, int table, int interlace, int sort, int tableSize) {
	gout << (char) 0x2c;
	gout << (char) (left%256) << (char) (left/256);
	gout << (char) (top%256) << (char) (top/256);
	gout << (char) (width%256) << (char) (width/256);
	gout << (char) (height%256) << (char) (height/256);
	gout << (char) (128*table+64*interlace+32*sort+tableSize);
}

void gif::writeTableBasedImageData(char minLZWSize, char size, char* data) {
	gout << minLZWSize;
	for(int i=0,j=min(255,size-i); i<size; j=min(255,size-i),i+=j) {
		writeDataSubBlock(j, data+i);
	}
	gif::writeBlockTerminator();
}

void gif::writeGraphicControlExtension(int disposal, int ui, int transparent, int delay, char transparentColour) {
	cout << std::hex << std::showbase << 0x21 << " " << 0xF9 << " " << 4;
	cout << std::hex << std::showbase << " " << (4*disposal+2*ui+transparent) << std::dec << "(" <<  4*disposal << "+" << 2*ui << "+" << transparent << ")" ;
	cout << std::hex << std::showbase << " " << (delay%256) << " " << (delay/256);
	cout << std::hex << std::showbase << " " << transparentColour;
	cout << std::dec << endl;

	gout << (char) 0x21 << (char) 0xF9 << (char) 4;
	gout << (char) (4*disposal+2*ui+transparent);
	gout << (char) (delay%256) << (char) (delay/256);
	gout << transparentColour;
	writeBlockTerminator();
}

void gif::writeApplicationExtension(char label, char size, string identifier, string auth) {
	gout << (char) 0x21 << label << size;
	gout << identifier.substr(0,8);
	gout << auth.substr(0,3);
}

void gif::writeTrailer() {
	gout << (char) 0x3B;
}


