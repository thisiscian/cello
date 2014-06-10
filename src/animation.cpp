#include<cello/animation.h>
using std::cout;
using std::cerr;
using std::endl;

int writeAnimation() {
	int err=0;
	Frame frame;
	GifFileType *output = EGifOpenFileName(cello::fileName.c_str(), cello::checkFileExists, &err);
	if(err) {
		cerr << "Error: libgif failed with message \"" << GifErrorString(err) << "\"... quitting" << endl;
		return err;
	}

	EGifPutScreenDesc(output, cello::width, cello::height, 8,0, NULL);

	// initiate infiniteLoop
	if(cello::infiniteLoop) {
		char loopCode[11]={0x4E,0x45,0x54,0x53,0x43,0x41,0x50,0x45,0x32,0x2E,0x30};
		unsigned char loopControl[4]={0x01, 0xFF,0xFF,0x00};
		EGifPutExtensionLeader(output, APPLICATION_EXT_FUNC_CODE);
		EGifPutExtensionBlock(output, 11,loopCode);
		EGifPutExtensionBlock(output, 3,loopControl);
		EGifPutExtensionTrailer(output);
	}

	// write frames to file
	for(cello::currentFrame=0; cello::currentFrame<cello::period; cello::currentFrame++) {
		char frameControl[4]={0x04,0x00,0x00,0xff};
		frameControl[1]=(int)(frame.delay*100) % 256;
		frameControl[2]=(int)(frame.delay*100) / 256;
		EGifPutExtension(output, GRAPHICS_EXT_FUNC_CODE, 4, frameControl);

		frame.draw();
		frame.defaultRestrictPalette();
		const GifColorType* colourMap=frame.getColourMap();
		ColorMapObject* palette=GifMakeMapObject(cello::maxPaletteSize, colourMap);
		EGifPutImageDesc(output,0,0,cello::width,cello::height,false,palette);
		
		for(int i=0; i<cello::height; i++) {
			EGifPutLine(output, &(frame.pixels[i]), cello::width);
		}
	}

	EGifCloseFile(output);
	cout << "Saved gif to " << cello::fileName << endl;
}
