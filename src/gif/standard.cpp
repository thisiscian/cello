#include<cello/gif/standard.h>
using std::string;
using std::fstream;

namespace cello {
	namespace standard {
		fstream gout;
		GifFileType* filetype=NULL;
		ColorMapObject* globalColourMap=NULL;	
		int imageCount=0;
	}
}

int cello::standard::writeFunction(GifFileType *gf, const GifByteType *gb, int count) {
	((fstream *) gf->UserData)->write((char *) gb, count);
	return count;
}

void cello::standard::start(string filename, int w, int h) {
	gout.open(filename.c_str(), fstream::out | fstream::trunc);
	gout.close();
	gout.open(filename.c_str(), fstream::out | fstream::app);
	int err;
	filetype=EGifOpen(&gout, &cello::standard::writeFunction, &err);
	filetype->ImageCount=0;
	filetype->SWidth=w;
	filetype->SHeight=h;
	filetype->SColorMap=NULL;
}

void cello::standard::writeGraphicControlExtension(int disposal, int ui, int transparent, int delay, byte transparentColour) {
	cello::standard::imageCount=0;
	byte* graphicData=new byte[4];
	graphicData[0]=4*disposal+2*ui+transparent;
	graphicData[1]=delay%256;
	graphicData[2]=delay/256;
	graphicData[3]=transparentColour;
	graphicControl={4,graphicData,GRAPHICS_EXT_FUNC_CODE};	
};

void cello::standard::writeImageDescriptor(int left, int top, int width, int height, int table, int interlace, int sort, int tableSize) {	
	currentImageDesc={left,top,width,height,(interlace==1), NULL};
};

void cello::standard::writeColourTable(int size, byte* table) {
	if(size>0 && table!=NULL) {
		ColorMapObject *localMap=GifMakeMapObject(size,table);
		currentImageDesc.ColorMap=localMap;
	}
};

void cello::standard::writeTableBasedImageData(byte minLZWSize, byte size, byte* data) {
	if(cello::standard::filetype->ImageCount==0) {
		ExtensionBlock blocks[2]={animationControl,graphicControl};
		SavedImage im={currentImageDesc, data, 2, blocks};
		cello::standard::savedImages.push_back(im); 
	} else if(cello::standard::imageCount==0) {
		ExtensionBlock blocks=*graphicControl;
		SavedImage im={currentImageDesc, data, 1, &blocks};
		cello::standard::savedImages.push_back(im); 
	} else {
		ExtensionBlock blocks[2]={animationControl,graphicControl};
		SavedImage im={currentImageDesc, data, 0, NULL};
		cello::standard::savedImages.push_back(im); 
	}
};

void cello::standard::stop() {
	EGifSpew(filetype);
	gout.close();
}
