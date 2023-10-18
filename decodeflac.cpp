#include <iostream>
#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>

#include <algorithm>

#include "BitInput/bitinput.hpp"

#include "Bloc/bloc.hpp"
#include "Bloc/Metadata/metadata.hpp"
#include "Bloc/Vorbis/vorbis.hpp"
#include "Bloc/Image/image.hpp"

#include "Frame/frame.hpp"

void read_flac(std::ifstream& input) {
	std::shared_ptr<BitInput> inp = std::make_shared<BitInput>(input);

	uint64_t header = inp->read_uint(32);

	if (header != 0x664C6143) {
		std::cerr << "Invalid magic string" << std::endl;
	}

	/**
	 * TODO : create a constructor for sub_blocs
	 * 		It must take a shared_ptr<Block> as entry
	 * 		and initialize the sub_bloc with this bloc.
	 * 	
	 * 		See how to chose a sub_bloc class depending
	 * 		on its type.
	*/

	MetadataBloc metadata = MetadataBloc(inp);
	metadata.read_metadata();
	metadata.print_metadata();

	VorbisBloc vorbis = VorbisBloc(inp);
	vorbis.read_vorbis();
	vorbis.print_vorbis();

	ImageBloc image = ImageBloc(inp);
	image.read_image();
	image.print_image();

	Bloc padding = Bloc(inp);
	while (!padding.is_last()) {
		padding.read_header();
		padding.print_info();
		padding.read_body();
	}
	
	Frame frame = Frame(inp);
	frame.read_header();
	frame.print_info();
}

int main(int argc, char** argv) {
	std::cout << "Usage : " << argv[0] << " input.flac " << " output.wav" << std::endl;
	std::filesystem::path pwd = std::filesystem::current_path();
	std::cout << "Current path : " << pwd.string() << std::endl;
	std::cout << "Number of arguments : " << argc << std::endl;
	if (argc >= 3) {
		std::cout << "Input file : " << pwd.string() << "/" << argv[1] << std::endl;
		std::cout << "Output file : " << pwd.string() << "/" << argv[2] << std::endl;

		std::ifstream input(pwd.string() + "/" + argv[1], std::ios::binary);
		std::ofstream output(pwd.string() + "/" + argv[2], std::ios::binary);

		//decode_file(input,&output);
	} else if (argc == 2) {
		std::cout << "Input file : " << pwd.string() << "/" << argv[1] << std::endl;
		std::cout << "Output file : " << pwd.string() << "/" << "out.wav" << std::endl;

		std::ifstream input(pwd.string() + "/" + argv[1], std::ios::binary);
		std::ofstream output(pwd.string() + "/out.wav" , std::ios::binary);

		std::cout << std::endl;

		//decode_file(input,&output);
		read_flac(input);
	}
	return 0;
}