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

void gif::writeDataSubBlock(byte size, byte* data) {
	gout << size;
	for(int i=0; i<(int) size; i++) {
		gout << *(data+i);
	}
}

void gif::writeBlockTerminator() {
	gout << (byte) 0x00;
}

void gif::writeHeader(string version) {
	gout << "GIF" << version.substr(0,3);
}

void gif::writeLogicalScreenDescriptor(int width, int height, int table, int resolution, int sort, int tableSize, byte backgroundColour, byte pixelAspectRatio) {
	gout << (byte) (width%256) << (byte) (width/256);
	gout << (byte) (height%256) << (byte) (height/256);
	gout << (byte) (128*table+(resolution-1)*16+sort*8+(tableSize-1));
	gout << backgroundColour << pixelAspectRatio;
}

void gif::writeColourTable(int size, byte* table) {
	if(table!=NULL && size > 0) 
		gout.write(reinterpret_cast<char *>(table), size*3);
}

void gif::writeImageDescriptor(int left, int top, int width, int height, int table, int interlace, int sort, int tableSize) {
	gout << (byte) 0x2c;
	gout << (byte) (left%256) << (byte) (left/256);
	gout << (byte) (top%256) << (byte) (top/256);
	gout << (byte) (width%256) << (byte) (width/256);
	gout << (byte) (height%256) << (byte) (height/256);
	gout << (byte) (128*table+64*interlace+32*sort+tableSize);
}

void gif::writeTableBasedImageData(byte minLZWSize, byte size, byte* data) {
	gout << minLZWSize;
	for(int i=0,j=min(255,size-i); i<size; j=min(255,size-i),i+=j) {
		writeDataSubBlock(j, data+i);
	}
	gif::writeBlockTerminator();
}

void gif::writeGraphicControlExtension(int disposal, int ui, int transparent, int delay, byte transparentColour) {
	gout << (byte) 0x21 << (byte) 0xF9 << (byte) 4;
	gout << (byte) (4*disposal+2*ui+transparent);
	gout << (byte) (delay%256) << (byte) (delay/256);
	gout << transparentColour;
	writeBlockTerminator();
}

void gif::writeApplicationExtension(byte label, byte size, string identifier, string auth) {
	gout << (byte) 0x21 << label << size;
	gout << identifier.substr(0,8);
	gout << auth.substr(0,3);
}

void gif::writeTrailer() {
	gout << (byte) 0x3B;
}


