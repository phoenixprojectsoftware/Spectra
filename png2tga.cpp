/**
* Copyright (c) 2025 The Phoenix Project Software SVG
* 
* png2tga:
* Converts a given PNG file to Targa format/
**/

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cout << "Usage: png2tga input.png output.tga\n";
		return 1;
	}

	const char* input_file = argv[1];
	const char* output_file = argv[2];

	int width, height, channels;
	unsigned char* data = stbi_load(input_file, &width, &height, &channels, 4);

	if (!data)
	{
	std::cerr << "Error loading png file: " << stbi_failure_reason() << "\n";
	return 1;
	}

	int success = stbi_write_tga(output_file, width, height, 4, data);
	if (!success)
	{
		std::cerr << "Failed to write tga\n";
		stbi_image_free(data);
		return 1;
	}

	std::cout << "Successfully gobbled up "<< input_file << " and then farted out " << output_file << "\n";

	stbi_image_free(data);
	return 0;
}