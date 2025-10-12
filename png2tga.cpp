/**
* Copyright (c) 2025 The Phoenix Project Software SVG
* 
* Spectra:
* Converts a given PNG file to Targa format.
**/

#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <cstdio>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace fs = std::filesystem;

// Convert one file
bool ConvertFile(const fs::path& input, const fs::path& output)
{
	int width, height, channels;
	unsigned char* data = stbi_load(input.string().c_str(), &width, &height, &channels, 4);
	if (!data)
	{
		std::cerr << "Error loading " << input << ": " << stbi_failure_reason() << std::endl;
		return false;
	}

	// always output 24 or 32 bit for goldsrc and vgui2 support
	int success = stbi_write_tga(output.string().c_str(), width, height, 4, data);
	stbi_image_free(data);

	if (!success)
	{
		std::cerr << "Error writing " << output << std::endl;
		return false;
	}

	return true;
}

// progress bar
void PrintProgress(size_t current, size_t total)
{
	const int barWidth = 40;
	float progress = static_cast<float>(current) / static_cast<float>(total);
	int pos = static_cast<int>(barWidth * progress);

	std::cout << "\r[";
	for (int i = 0; i < barWidth; ++i)
		std::cout << (i < pos ? "=" : (i == pos ? ">" : " "));
	std::cout << "] " << int(progress * 100.0) << "%";
	std::cout.flush();
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Spectra <input.png | folder> [output.tga]\n";
		return 1;
	}

	fs::path inputPath = argv[1];
	fs::path outputPath;

	if (argc >= 3)
		outputPath = argv[2];

	if (fs::is_directory(inputPath))
	{
		std::vector<fs::path> pngFiles;
		for (auto& entry : fs::directory_iterator(inputPath))
		{
			if (entry.path().extension() == ".png" || entry.path().extension() == ".PNG")
				pngFiles.push_back(entry.path());
		}

		if (pngFiles.empty())
		{
			std::cerr << "No PNG files found in directory: " << inputPath << std::endl;
			return 0;
		}

		std::cout << "Found " << pngFiles.size() << " PNG files in directory. Converting . . .\n";

		size_t index = 0;
		for (const auto& file : pngFiles)
		{
			fs::path out = file;
			out.replace_extension(".tga");

			if (fs::exists(out))
			{
				std::cout << "\nOverwrite existing " << out << "? (y/n): ";
				char choice;
				std::cin >> choice;
				if (choice != 'y' && choice != 'Y')
				{
					++index;
					PrintProgress(index, pngFiles.size());
					continue;
				}
			}

			ConvertFile(file, out);
			++index;
			PrintProgress(index, pngFiles.size());
		}

		std::cout << "\nDONE BABY YEAH!\n";
		return 0;
	}

	// handle single file
	if (!fs::exists(inputPath))
	{
		std::cerr << "Input file does not exist: " << inputPath << std::endl;
		return 1;
	}

	// auto-generate output path if not provided
	if (outputPath.empty())
	{
		outputPath = inputPath;
		outputPath.replace_extension(".tga");
	}

	// check overwrite
	if (fs::exists(outputPath))
	{
		std::cout << "Overwrite existing " << outputPath << "? (y/n): ";
		char choice;
		std::cin >> choice;
		if (choice != 'y' && choice != 'Y')
		{
			std::cout << "Aborted by user.\n";
			return 0;
		}
	}

	if (ConvertFile(inputPath, outputPath))
	{
		std::cout << "Converted " << inputPath << " to " << outputPath << "\n";
	}
	else
		return 1;

	return 0;
}