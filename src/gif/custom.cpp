#include<cello/gif/custom.h>
#include<iostream>
using std::string;
using std::fstream;
using std::set;
using std::min;
using cello::custom::gout;
using std::cout;
using std::endl;

namespace cello {
	namespace custom {
		fstream gout;
	}
}

void cello::custom::start() {
	byte loopControl[3]={0x01,0xFF,0xFF};

	// clears the file to be written to
	gout.open(cello::filename.c_str(), std::fstream::out | std::fstream::trunc);
	gout.close();

	// reopens file, so can be appended to, and sets important flags
	gout.open(cello::filename.c_str(), std::fstream::out | std::fstream::app);

	writeHeader("89a");
	writeLogicalScreenDescriptor(cello::width,cello::height,0,8,0,0,0,0);
	writeColourTable(0,NULL);
	writeApplicationExtension(0xFF, 11, "NETSCAPE", "2.0");
		writeDataSubBlock(3,loopControl);
	writeBlockTerminator();
}

void cello::custom::stop() {
	writeTrailer();
//	gout << (byte) 0x0a;
	gout.close();
}

void cello::custom::writeDataSubBlock(byte size, byte* data) {
	gout << size;
	for(int i=0; i<(int) size; i++) {
		gout << *(data+i);
	}
}

void cello::custom::writeBlockTerminator() {
	gout << (byte) 0x00;
}

void cello::custom::writeHeader(string version) {
	gout << "GIF" << version.substr(0,3);
}

void cello::custom::writeLogicalScreenDescriptor(int width, int height, int table, int resolution, int sort, int tableSize, byte backgroundColour, byte pixelAspectRatio) {
	gout << (byte) (width%256) << (byte) (width/256);
	gout << (byte) (height%256) << (byte) (height/256);
	gout << (byte) (128*table+(resolution-1)*16+sort*8+(tableSize-1));
	gout << backgroundColour << pixelAspectRatio;
}

void cello::custom::writeColourTable(int size, byte* table) {
	if(table!=NULL && size > 0) 
		gout.write(reinterpret_cast<char *>(table), size*3);
}

void cello::custom::writeImageDescriptor(int left, int top, int width, int height, int table, int interlace, int sort, int tableSize) {
	gout << (byte) 0x2c;
	gout << (byte) (left%256) << (byte) (left/256);
	gout << (byte) (top%256) << (byte) (top/256);
	gout << (byte) (width%256) << (byte) (width/256);
	gout << (byte) (height%256) << (byte) (height/256);
	gout << (byte) (128*table+64*interlace+32*sort+tableSize);
}

void cello::custom::writeTableBasedImageData(byte minLZWSize, byte size, byte* data) {
	gout << minLZWSize;
	for(int i=0,j=min(255,size-i); i<size; j=min(255,size-i),i+=j) {
		writeDataSubBlock(j, data+i);
	}
	writeBlockTerminator();
}

void cello::custom::writeGraphicControlExtension(int disposal, int ui, int transparent, int delay, byte transparentColour) {
	gout << (byte) 0x21 << (byte) 0xF9 << (byte) 4;
	gout << (byte) (4*disposal+2*ui+transparent);
	gout << (byte) (delay%256) << (byte) (delay/256);
	gout << transparentColour;
	writeBlockTerminator();
}

void cello::custom::writeApplicationExtension(byte label, byte size, string identifier, string auth) {
	gout << (byte) 0x21 << label << size;
	gout << identifier.substr(0,8);
	gout << auth.substr(0,3);
}

void cello::custom::writeTrailer() {
	gout << (byte) 0x3B;
}


