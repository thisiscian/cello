#ifndef __cello_image__
#define __cello_image__
	#include <cstddef>
	#include <vector>
	#include <map>
	#include <cmath>
	#include <cello/colourMap.h>

	typedef unsigned char Byte;
	typedef std::vector<Byte> IndexStream;
	typedef std::vector<int> Code;
	typedef std::vector<Code> codeStream;
	class Image {
		public:
			Image(Byte* data, size_t width, size_t height, size_t frameWidth);

			void setProperties(size_t left, size_t top);
			void setProperties(size_t left, size_t top, bool interlace, bool sort);

			ColourMap colourMap;

			size_t left();
			size_t top();
			size_t width();
			size_t height();
			size_t frameWidth();
			bool interlace();
			bool sort();
			size_t table();
			size_t minimumCodeSize();

			Byte* getRawData();
			IndexStream getCompressedData();

		private:
			size_t _left, _top, _width, _height, _minimumCodeSize, _frameWidth;
			bool _interlace, _sort;
			Byte *data, *_rawData;
			IndexStream _indexStream;

			ColourMap makeColourMap(Byte* data, size_t width, size_t height, size_t frameWidth);
			IndexStream makeIndexStream(Byte* data, size_t width, size_t height, size_t frameWidth, ColourMap colourMap);

			void left(size_t left);
			void top(size_t top);
			void width(size_t width);
			void height(size_t height);
			void frameWidth(size_t frameWidth);
			void interlace(bool interlace);
			void sort(bool sort);
			void minimumCodeSize(size_t minimumCodeSize);

			IndexStream compressIndexStream(size_t minimumCodeSize, IndexStream stream);
	};

#endif
