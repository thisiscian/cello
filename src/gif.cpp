#include<cello/gif.h>
#include<iostream>
#include<cstdio>
#include<cello/pixel.h>
using std::hex;
using std::setfill;
using std::string;
using std::left;
using std::right;
using std::setw;
using std::set;
std::fstream gif::gout;




namespace gif {
	char backgroundColour=0;
	int colourResolution=255;
	char applicationBlock[2]={0x21,0xFF};
	char graphicBlock[2]={0x21,0xF7};
	char imageBlock=0x2c;
	char endBlock=0x00;
	char noTransparency=0x08;
	char transparency=0x00;
	char transparencyColour=0x00;
	int delay=9;
}


void gif::init(string filename, int w, int h) {
	gif::width=w;
	gif::height=h;
	char size[4]={w%256, w/256, h%256, h/256};
	char globalColourTable=0x00;
//	char quality[3]={backgroundColour, resolution%256, resolution/256};
	char loopCode[12]={11,0x4E,0x45,0x54,0x53,0x43,0x41,0x50,0x45,0x32,0x2E,0x30}; // translates to NETSCAPE2.0
	char loopControl[4]={3,0x01,0xFF,0xFF};
	
	// clears the file to be written to
	gif::gout.open(filename.c_str(), std::fstream::out | std::fstream::trunc);
	gif::gout.close();

	// reopens file, so can be appended to, and sets important flags
	gif::gout.open(filename.c_str(), std::fstream::out | std::fstream::app);
	gif::gout.flags(std::ios::left | std::ios::hex);
	gif::gout << setfill('0');

	// parts required for standard
	gif::gout << "GIF89a";
	gif::gout.write(size,4);
	gif::gout << globalColourTable;

	// writes parts needed for animation to loop
	gif::gout << gif::applicationBlock;
		gif::gout.write(loopCode, loopCode[0]+1);
		gif::gout.write(loopControl, loopControl[0]+1);
	gif::gout << gif::endBlock;
}

void gif::drawFrame(unsigned char* frame) {
	int pos[2]={0,0};
	char graphicBlockContent[5]={0x04,gif::noTransparency,gif::delay%256, gif::delay/256, gif::transparentColour};
	char position[4]={pos[0]%256,pos[0]/256,pos[1]%256,pos[1]/256};
	char size[4]={gif::width%256, gif::width/256, gif::height%256, gif::height/256};
	gif::gout << gif::graphicBlock; 
		gif::gout.write(graphicBlockContent,graphicBlockContent[0]+1);
	gif::gout << gif::endBlock;
	gif::gout << gif::imageBlock;
		gif::gout.write(position, 4);
		gif::gout.write(size, 4);
		gif::writeTableAndFrame(frame);
	gif::gout << gif::endBlock;
}

void gif::writeTableAndFrame(unsigned char* frame) {
	set<char*, PixelCompare> uniqueColours;
	unsigned char colourTable[256*3] = {0};
	for(int i=0; i<gif::width*gif::height; i++) {
	}
	//gif::gout.write(frame, gif::width*gif::height*3);
}
