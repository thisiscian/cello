#include<cello/cello.h>
using gif::gout;
using std::string;

namespace cello {
	int width=100;
	int height=100;
	int delay=9;
}

void cello::start(string filename, int w, int h) {
	cello::width=w;
	cello::height=h;
	cello::delay=9;
	char loopControl[3]={0x01,0xFF,0xFF};

	// clears the file to be written to
	gout.open(filename.c_str(), std::fstream::out | std::fstream::trunc);
	gout.close();

	// reopens file, so can be appended to, and sets important flags
	gout.open(filename.c_str(), std::fstream::out | std::fstream::app);

	gif::writeHeader("89a");
	gif::writeLogicalScreenDescriptor(w,h,0,8,0,0,0,0);
	gif::writeColourTable(0,NULL);
	gif::writeApplicationExtension(0xFF, 11, "NETSCAPE", "2.0");
		gif::writeDataSubBlock(3,loopControl);
	gif::writeBlockTerminator();
}

void cello::stop() {
	gif::writeTrailer();
	gout.close();
}
