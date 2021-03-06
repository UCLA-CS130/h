#ifndef HTTP_COMPRESSION_H
#define HTTP_COMPRESSION_H

#include <string>
#include <sstream>
#include <stdexcept>

#include <string.h>
#include <zlib.h>
#include "body_compression.h"



namespace http {
namespace server {


class Compression
{


private:
	std::string original_body_;
	std::string compressed_body_;

public:

	Compression(std::string original_body):original_body_(original_body){}

	void CompressGzip(int compressionlevel = Z_BEST_COMPRESSION);

	void CompressDeflate(int compressionlevel = Z_BEST_COMPRESSION);

	std::string GetCompressedBody(){
		return compressed_body_;
	} 

}; 

}
}


#endif