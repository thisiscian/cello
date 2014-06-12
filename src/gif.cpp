#include<cello/gif.h>
#include<iostream>
using std::string;
using std::set;
using std::min;
using gif::gout;

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
		for(int i=0; i<size*3; i++) {
			gout << (char) *(table+i);
		}	
}

void gif::writeImageDescriptor(int left, int top, int width, int height, int table, int interlace, int sort, int tableSize) {
	gout << (char) 0x2c;
	gout << (char) (left%256) << (char) (left/256);
	gout << (char) (top%256) << (char) (top/256);
	gout << (char) (width%256) << (char) (width/256);
	gout << (char) (height%256) << (char) (height/256);
	gout << (char) (128*table+64*interlace+32*sort+tableSize);
}

void gif::writeTableImageData(char minLZWSize, char size, char* data) {
	gout << minLZWSize;
	for(int i=0,j=min(255,size-i); i<size; j=min(255,size-i),i+=j) {
		writeDataSubBlock(j, data+i);
	}
	gif::writeBlockTerminator();
}

void gif::writeGraphicControlExtension(char label, char size, int disposal, int ui, char transparent, int delay, char transparentColour) {
	gout << (char) 0x21 << label << size;
	gout << (char) (transparent+2*ui+4*disposal);
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


