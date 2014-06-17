#include<cello/gif.h>
using std::string;

namespace cello {
	int width=100;
	int height=100;
	int delay=9;
	bool useStandard=false;

	void (*stop)()=NULL;
	void (*writeHeader)(std::string version)=NULL;
	void (*writeApplicationExtension)(byte label, byte size, std::string identifier, std::string auth)=NULL;
	void (*writeGraphicControlExtension)(int disposal, int ui, int transparent, int delay, byte transparentColour)=NULL;
	void (*writeImageDescriptor)(int left, int top, int width, int height, int table, int interlace, int sort, int tableSize)=NULL;
	void (*writeColourTable)(int size, byte* table)=NULL;
	void (*writeLogicalScreenDescriptor)(int width, int height, int table, int resolution, int sort, int tableSize, byte backgroundColour, byte pixelAspectRatio)=NULL;
	void (*writeDataSubBlock)(byte size, byte* data)=NULL;
	void (*writeTableBasedImageData)(byte minLZWSize, byte size, byte* data)=NULL;
	void (*writeBlockTerminator)()=NULL;
	void (*writeTrailer)()=NULL;
}

void cello::setFunctionsToStandard() {
	writeGraphicControlExtension=&cello::standard::writeGraphicControlExtension;
	writeImageDescriptor=&cello::standard::writeImageDescriptor;
	writeColourTable=&cello::standard::writeColourTable;
	writeTableBasedImageData=&cello::standard::writeTableBasedImageData;
	stop=&cello::standard::stop;
}

void cello::setFunctionsToCustom() {
	writeHeader=&cello::custom::writeHeader;
	writeApplicationExtension=&cello::custom::writeApplicationExtension;
	writeGraphicControlExtension=&cello::custom::writeGraphicControlExtension;
	writeImageDescriptor=&cello::custom::writeImageDescriptor;
	writeColourTable=&cello::custom::writeColourTable;
	writeLogicalScreenDescriptor=&cello::custom::writeLogicalScreenDescriptor;
	writeDataSubBlock=&cello::custom::writeDataSubBlock;
	writeTableBasedImageData=&cello::custom::writeTableBasedImageData;
	writeBlockTerminator=&cello::custom::writeBlockTerminator;
	writeTrailer=&cello::custom::writeTrailer;
	stop=&cello::custom::stop;
}

void cello::start(string filename, int w, int h) {
	if(cello::useStandard) {
		cello::setFunctionsToStandard();
		cello::standard::start(filename, w, h);
	} else {
		cello::setFunctionsToStandard();
		cello::custom::start(filename, w, h);
	}
}
