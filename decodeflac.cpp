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
#include "Bloc/Padding/padding.hpp"

#include "Frame/frame.hpp"

void read_flac(std::ifstream& input) {
	std::shared_ptr<BitInput> inp = std::make_shared<BitInput>(input);

	uint64_t header = inp->read_uint(32);

	if (header != 0x664C6143) {
		std::cerr << "Invalid magic string" << std::endl;
	}

	std::shared_ptr<Bloc> bloc = std::make_shared<Bloc>(inp);
	uint64_t bloc_type;
	VorbisBloc vorbis = VorbisBloc(bloc);
	ImageBloc image = ImageBloc(bloc);
	PaddingBloc padding = PaddingBloc(bloc);

	MetadataBloc metadata = MetadataBloc(bloc);
	metadata.read_metadata();
	metadata.print_metadata();

	uint64_t bitrate = metadata.get_bitrate();

	while (!bloc.get()->is_last()) {
		bloc_type = bloc.get()->silent_read_type();
		switch (bloc_type) {
			case 4:
				vorbis.read_vorbis();
				vorbis.print_vorbis();
				break;
			case 6:
				image.read_image();
				image.print_image();
				break;
			default:
				padding.read_padding();
				break;
		}
	}
	
	Frame frame = Frame(inp, bitrate);
	int i = 0;
	while (!frame.is_last()) {
		std::cout << "Frame n" << i << " : " << std::endl;
		i++;
		frame.read_header();
		frame.print_info();
		frame.read_frame();
		frame.read_footer();
	}

	std::cout << std::endl << "[INFO] : The entire Flac file has been read" << std::endl;
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