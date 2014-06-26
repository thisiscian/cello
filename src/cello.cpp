#include <cello/cello.h>
using std::string;
using std::fstream;
using std::cout;
using std::endl;

Cello::Cello() {
	initialise();
}

void Cello::start(string f, size_t w, size_t h) {
	filename=f; width=w; height=h;
	currentFrame.initialise(width, height, frameDelay);
}

int Cello::getWidth() { return width ;};
int Cello::getHeight() { return height; };
string Cello::getFilename() { return filename; };

void Cello::initialise() {
	width=0; height=0; frameDelay=5; mode=1; filename="output.gif";
}

void Cello::store() {
	currentFrame.makeImages();
	frames.push_back(currentFrame);
}

void Cello::save() {
	if(mode==STANDARD) {
		cout << "saving " << filename << " using lib_gif methods" << endl;
		standardSave();
	} else if(mode==CUSTOM) {
		cout << "saving " << filename << " using cello methods" << endl;
		customSave();
	} else if(mode==BOTH) {
		cout << "saving " << filename << " using cello and lib_gif methods" << endl;
		string title=filename;
		filename="CUSTOM_"+title;
		customSave();
		filename="STANDARD_"+title;
		standardSave();
	}
}

void Cello::customSave() {
	gout.open(filename.c_str(), fstream::out | fstream::trunc);
  gout.close(); 
  gout.open(filename.c_str(), fstream::out | fstream::app);
	cout << "\tsaving: " << filename << endl;

	writeHeader("89a");
	writeLogicalScreenDescriptor(width,height,8,0,0,globalColourMap.size(),0);	
	writeColourTable(globalColourMap.size(),globalColourMap.colours);
	writeApplicationExtension(0xFF, 11, "NETSCAPE", "2.0");
		Byte loopControl[3]={0x01,0xFF,0xFF};
		writeDataSubBlock(3,loopControl);
	writeBlockTerminator();

	for(int i=0; i<frames.size(); i++) {
		writeGraphicControlExtension(2,0,1,0,0x00);
		for(int j=0; j<frames[i].images.size(); j++) {
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

int standardWrite(GifFileType * ft, const GifByteType * data, int count) {
 	( (fstream *) ft->UserData )->write( (char*) data, count);
	return count;
}


void Cello::standardSave() {
	gout.open(filename.c_str(), fstream::out | fstream::trunc);
  gout.close(); 
  gout.open(filename.c_str(), fstream::out | fstream::app);
	cout << "\tsaving: " << filename << endl;

	int err=0;
	GifFileType *filetype=EGifOpen(&gout, &standardWrite, &err);

	int imageCount=0;
	for(int i=0; i<frames.size(); i++) {
		imageCount+=frames[i].images.size();
	}

	SavedImage savedImages[imageCount];

	filetype->ImageCount=imageCount;
	filetype->SWidth=getWidth();
	filetype->SHeight=getHeight();
	filetype->SColorMap=globalColourMap.toStandard();
	filetype->SavedImages=savedImages;
	
	for(int i=0; i<frames.size(); i++) {
		for(int j=0; j<frames[i].images.size(); j++) {
			Image *image=&(frames[i].images[j]);
			savedImages[i].ImageDesc.Left=image->left();
			savedImages[i].ImageDesc.Top=image->top();
			savedImages[i].ImageDesc.Width=image->width();
			savedImages[i].ImageDesc.Height=image->height();
			savedImages[i].ImageDesc.Interlace=image->interlace();
			savedImages[i].ImageDesc.ColorMap=image->colourMap.toStandard();
			savedImages[i].RasterBits=image->getRawData();
			cout << "cc=" << savedImages[i].ImageDesc.ColorMap->ColorCount<< endl;

			if(i==0) {
				ExtensionBlock blocks[2];
				GifByteType animationBlockData[15]={0x4e,0x45,0x54,0x53,0x43,0x41,0x50,0x45,0x32,0x2e,0x30,0x03,0x01,0xff,0xff};
				GifByteType graphicBlockData[15]={4, ((int) frames[i].delay)%256, ((int) frames[i].delay)/256, 0};
				blocks[0].ByteCount=15;
				blocks[0].Bytes=animationBlockData;
				blocks[0].Function=APPLICATION_EXT_FUNC_CODE;
				blocks[1].ByteCount=4;
				blocks[1].Bytes=graphicBlockData;
				blocks[1].Function=APPLICATION_EXT_FUNC_CODE;
				savedImages[i].ExtensionBlockCount=2;
				savedImages[i].ExtensionBlocks=blocks;
			} else if(j==0) {
				ExtensionBlock block;
				GifByteType graphicBlockData[15]={4, ((int) frames[i].delay)%256, ((int) frames[i].delay)/256, 0};
				block.ByteCount=4;
				block.Bytes=graphicBlockData;
				block.Function=APPLICATION_EXT_FUNC_CODE;
				savedImages[i].ExtensionBlockCount=1;
				savedImages[i].ExtensionBlocks=&block;
			} else {
				savedImages[i].ExtensionBlockCount=0;
				savedImages[i].ExtensionBlocks=NULL;
			}
		}
	}

	EGifSpew(filetype);
	gout.close();
}


void Cello::writeDataSubBlock(Byte size, Byte* data) {
	gout << size;
	for(int i=0; i<(int) size; i++) {
		gout << *(data+i);
	}
}

void Cello::writeBlockTerminator() {
	gout << (Byte) 0x00;
}

void Cello::writeHeader(string version) {
	gout << "GIF" << version.substr(0,3);
}

void Cello::writeLogicalScreenDescriptor(int width, int height, int resolution, int sort, int tableSize, Byte backgroundColour, Byte pixelAspectRatio) {
	int table=(tableSize!=0);	
	gout << (Byte) (width%256) << (Byte) (width/256);
	gout << (Byte) (height%256) << (Byte) (height/256);
	gout << (Byte) (128*table+(resolution-1)*16+sort*8+(tableSize-1));
	gout << backgroundColour << pixelAspectRatio;
}

void Cello::writeColourTable(int size, Byte* table) {
	if(table!=NULL && size > 0) 
		gout.write(reinterpret_cast<char *>(table), size*3);
}

void Cello::writeImageDescriptor(int left, int top, int width, int height, int interlace, int sort, int tableSize) {
	int table=(tableSize!=0);
	int size=(tableSize>>1)-1;
	gout << (Byte) 0x2c;
	gout << (Byte) (left%256) << (Byte) (left/256);
	gout << (Byte) (top%256) << (Byte) (top/256);
	gout << (Byte) (width%256) << (Byte) (width/256);
	gout << (Byte) (height%256) << (Byte) (height/256);
	gout << (Byte) (128*table+64*interlace+32*sort+size);
}

void Cello::writeTableBasedImageData(Byte minLZWSize, Byte size, Byte* data) {
	gout << minLZWSize;
	for(int i=0,j=std::min(255,size-i); i<size; j=std::min(255,size-i),i+=j) {
		writeDataSubBlock(j, data+i);
	}
	writeBlockTerminator();
}

void Cello::writeGraphicControlExtension(int disposal, int ui, int transparent, int delay, Byte transparentColour) {
	gout << (Byte) 0x21 << (Byte) 0xF9 << (Byte) 4;
	gout << (Byte) (4*disposal+2*ui+transparent);
	gout << (Byte) (delay%256) << (Byte) (delay/256);
	gout << transparentColour;
	writeBlockTerminator();
}

void Cello::writeApplicationExtension(Byte label, Byte size, string identifier, string auth) {
	gout << (Byte) 0x21 << label << size;
	gout << identifier.substr(0,8);
	gout << auth.substr(0,3);
}

void Cello::writeTrailer() {
	gout << (Byte) 0x3B;
}
