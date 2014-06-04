#include<cello/compile.h>
#include<iostream>
#include<fstream>
using std::cout;
using std::endl;

void writeTestGif() {
	int err=0;

	Frame myFrame;
	cello::restrictPalette(myFrame);

	cout << "starting..." << endl;
	GifFileType *output = EGifOpenFileName("test.gif", cello::checkFileExists, &err);
	if(err) { cout << "err=" << err << " " << GifErrorString(err) << endl; }

//	GifByteType repeatBlock[3]={1,0,0};
	GifByteType midExt[4]={0x04,0x0A,0x00,0xff};
//	GifByteType netscape[12]="NETSCAPE2.0";
//	ExtensionBlock start={11,netscape, APPLICATION_EXT_FUNC_CODE};
//	ExtensionBlock mid={4,midExt, GRAPHICS_EXT_FUNC_CODE};
//	ExtensionBlock stop={3,repeatBlock, APPLICATION_EXT_FUNC_CODE};
//	ExtensionBlock list[3]={start,mid,stop};

	ColorMapObject* palette=GifMakeMapObject(cello::maxPaletteSize, cello::defaultPalette);
	GifImageDesc desc= {0,0,cello::width,cello::height, true, palette};
	SavedImage si={desc, myFrame.pixelArray, 0, NULL};
	output->SavedImages=&si;
	output->ImageCount=1;
	output->ExtensionBlockCount=0;
	output->ExtensionBlocks=NULL;

	EGifPutScreenDesc(output,cello::width,cello::height, 256,0,defaultPalette);
	EGifPutExtension(output, GRAPHICS_EXT_FUNC_CODE, 4, midExt);
	EGifPutImageDesc(output,0,0,cello::width,cello::height, false,NULL);
	for(int i=0; i<(cello::width.h	

	
//	if(!EGifSpew(output)) { cout << "Error: failed to spew" << endl;}
	cout << "done" << endl;
}
