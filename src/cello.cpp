#include <cello/cello.h>
using std::string;
using std::fstream;

Cello::Cello() {
	initialise();
}

void Cello::start(string filename, size_t width, size_t height) {
	filename=filename; width=width; height=height;
  gout.open(filename.c_str(), fstream::out | fstream::trunc);
  gout.close(); 
  gout.open(filename.c_str(), fstream::out | fstream::app);
}

int Cello::getWidth() { return width ;};
int Cello::getHeight() { return height; };
string Cello::getFilename() { return filename; };

void Cello::initialise() {
	width=0; height=0; frameDelay=5; mode=1; filename="output.gif";
}

void Cello::addFrame(Frame frame) {
	frames.push_back(frame);
}

void Cello::save() {
	if(mode==STANDARD) {
		standardSave();
	} else if(mode==CUSTOM) {
		customSave();
	}
}

void Cello::customSave() {
	writeHeader("89a");
	writeLogicalScreenDescriptor(width,height,8,0,0,globalColourMap.size(),0);
	writeColourTable(globalColourMap.size(),globalColourMap.colours);
	writeApplicationExtension(0xFF, 11, "NETSCAPE", "2.0");
		Byte loopControl[3]={0x01,0xFF,0xFF};
		writeDataSubBlock(3,loopControl);
	writeBlockTerminator();

	for(int i=0; i<frames.size(); i++) {
		writeGraphicControlExtension(2,0,1,0,0x00);
		for(int j=0; i<frames[i].images.size(); j++) {
			Image *image=&(frames[i].images[j]);

			writeImageDescriptor(image->left(),image->top(), image->width(), image->height(), image->interlace(), image->sort(), image->colourMap.size());
			writeColourTable(image->colourMap.size(), image->colourMap.colours);

			IndexStream compressed=image->getCompressedData();
			int dataSize=compressed.size();
			
			Byte data[dataSize];
			for(int i=0; i<dataSize; i++) {
				data[i]=compressed[i];
			}

			writeTableBasedImageData(image->minimumCodeSize(), dataSize, data);
		}
	}
	writeTrailer();
	gout.close();	
}

int Cello::standardWrite(GifFileType * ft, const GifByteType * data, int count) {
 	( (fstream *) ft->UserData )->write( (char*) data, count);
	return count;
}


void Cello::standardSave() {
	int err=0;
	GifFileType *filetype=EGifOpen(&gout, &standardWrite, err);

	filetype->ImageCount=images.size();
	filetype->SWidth=Cello::getWidth();
	filetype->SHeight=Cello::getHeight();
	filetype->SColorMap=globalColourMap.toStandard();

	int imageCount=0;
	for(int i=0; i<frames.size(); i++) {
		imageCount+=frames[i].images.size();
	}

	SavedImage savedImages[imagesCount];

	for(int i=0; i<frames.size(); i++) {
		for(int j=0; i<frames[i].images.size(); j++) {
			savedImages[i].ImageDesc.Left=image->left();
			savedImages[i].ImageDesc.Top=image->top();
			savedImages[i].ImageDesc.Width=image->width();
			savedImages[i].ImageDesc.Height=image->height();
			savedImages[i].ImageDesc.Interlace=image->interlace();
			savedImages[i].Colors=image->colourMap.toStandard();
			savedImages[i].RasterBits=images[i].getRawData();
			savedImages[i].ExtensionBlockCount=(i==0)+(j==0);
			savedImages[i].ExtensionBlocks=???;
		}
	}

	EGifSpew(filetype);
	gout.close();
}



void cello::writeDataSubBlock(Byte size, Byte* data) {
	gout << size;
	for(int i=0; i<(int) size; i++) {
		gout << *(data+i);
	}
}

void cello::writeBlockTerminator() {
	gout << (Byte) 0x00;
}

void cello::writeHeader(string version) {
	gout << "GIF" << version.substr(0,3);
}

void cello::writeLogicalScreenDescriptor(int width, int height, int table, int resolution, int sort, int tableSize, Byte backgroundColour, Byte pixelAspectRatio) {
	int table=(tableSize!=0);	
	gout << (Byte) (width%256) << (Byte) (width/256);
	gout << (Byte) (height%256) << (Byte) (height/256);
	gout << (Byte) (128*table+(resolution-1)*16+sort*8+(tableSize-1));
	gout << backgroundColour << pixelAspectRatio;
}

void cello::writeColourTable(int size, Byte* table) {
	if(table!=NULL && size > 0) 
		gout.write(reinterpret_cast<char *>(table), size*3);
}

void cello::writeImageDescriptor(int left, int top, int width, int height, int interlace, int sort, int tableSize) {
	int table=(tableSize!=0);
	gout << (Byte) 0x2c;
	gout << (Byte) (left%256) << (Byte) (left/256);
	gout << (Byte) (top%256) << (Byte) (top/256);
	gout << (Byte) (width%256) << (Byte) (width/256);
	gout << (Byte) (height%256) << (Byte) (height/256);
	gout << (Byte) (128*table+64*interlace+32*sort+tableSize);
}

void cello::writeTableBasedImageData(Byte minLZWSize, Byte size, Byte* data) {
	gout << minLZWSize;
	for(int i=0,j=min(255,size-i); i<size; j=min(255,size-i),i+=j) {
		writeDataSubBlock(j, data+i);
	}
	writeBlockTerminator();
}

void cello::writeGraphicControlExtension(int disposal, int ui, int transparent, int delay, Byte transparentColour) {
	gout << (Byte) 0x21 << (Byte) 0xF9 << (Byte) 4;
	gout << (Byte) (4*disposal+2*ui+transparent);
	gout << (Byte) (delay%256) << (Byte) (delay/256);
	gout << transparentColour;
	writeBlockTerminator();
}

void cello::writeApplicationExtension(Byte label, Byte size, string identifier, string auth) {
	gout << (Byte) 0x21 << label << size;
	gout << identifier.substr(0,8);
	gout << auth.substr(0,3);
}

void cello::writeTrailer() {
	gout << (Byte) 0x3B;
}


