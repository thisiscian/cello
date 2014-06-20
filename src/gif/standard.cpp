#include<cello/gif/standard.h>
#include<iostream>
using std::cout;
using std::endl;

using std::string;
using std::fstream;
using std::vector;

namespace cello {
	namespace standard {
		fstream gout;
		GifFileType* filetype=NULL;
		ColorMapObject* globalColourMap=NULL;	
		int imageCount=0;
		ExtensionBlock animationControl;
		ExtensionBlock graphicControl;
		GifImageDesc currentImageDesc;
		vector<SavedImage> savedImages;
	}
}

int cello::standard::writeFunction(GifFileType *gf, const GifByteType *gb, int count) {
	((fstream *) gf->UserData)->write((char *) gb, count);
	return count;
}

void cello::standard::start() {
	gout.open(cello::filename.c_str(), fstream::out | fstream::trunc);
	gout.close();
	gout.open(cello::filename.c_str(), fstream::out | fstream::app);
	int err;
	filetype=EGifOpen(&gout, &writeFunction, &err);
	filetype->ImageCount=0;
	filetype->SWidth=cello::width;
	filetype->SHeight=cello::height;
	filetype->SColorMap=NULL;
}

void cello::standard::writeGraphicControlExtension(int disposal, int ui, int transparent, int delay, byte transparentColour) {
	cout << "writing Graphic Control Extension" << endl;
	imageCount=0;
	byte* graphicData=new byte[4];
	graphicData[0]=4*disposal+2*ui+transparent;
	graphicData[1]=delay%256;
	graphicData[2]=delay/256;
	graphicData[3]=transparentColour;

	graphicControl.ByteCount=4;
	graphicControl.Bytes=graphicData;
	graphicControl.Function=GRAPHICS_EXT_FUNC_CODE;
};

void cello::standard::writeImageDescriptor(int left, int top, int width, int height, int table, int interlace, int sort, int tableSize) {	
	currentImageDesc.Left=left;
	currentImageDesc.Top=top;
	currentImageDesc.Width=width;
	currentImageDesc.Height=height;
	currentImageDesc.Interlace=(interlace==1);
	currentImageDesc.ColorMap=NULL;
};

void cello::standard::writeColourTable(int size, byte* table) {
	if(size>0 && table!=NULL) {
		GifColorType colourTable[size];
		for(int i=0; i<size; i++) {
			colourTable[i].Red=table[3*i];		
			colourTable[i].Blue=table[3*i+1];		
			colourTable[i].Green=table[3*i+2];		
		}

		ColorMapObject *localMap=GifMakeMapObject(size,colourTable);
		currentImageDesc.ColorMap=localMap;
	}
};

void cello::standard::writeTableBasedImageData(byte minLZWSize, byte size, byte* data) {
	cout << "okay: " << filetype->ImageCount << "," <<  imageCount << endl;
	if(currentFrame==0) {
		ExtensionBlock blocks[2]={animationControl,graphicControl};
		SavedImage im={currentImageDesc, data, 2, blocks};
		savedImages.push_back(im); 
	} else if(imageCount==0) {
		ExtensionBlock blocks=graphicControl;
		SavedImage im={currentImageDesc, data, 1, &blocks};
		savedImages.push_back(im); 
	} else {
		ExtensionBlock blocks[2]={animationControl,graphicControl};
		SavedImage im={currentImageDesc, data, 0, NULL};
		savedImages.push_back(im); 
	}
};

void cello::standard::stop() {
	EGifSpew(filetype);
	gout.close();
}
