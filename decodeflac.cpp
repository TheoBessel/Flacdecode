#include <iostream>
#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>

#include <algorithm>

#include <chrono>
#include <thread>

class BitInput {
public:
	BitInput(std::ifstream& input) : m_input (input) {
		m_bitbuffer = 0;
		m_bufferlen = 0;
	}
	uint64_t read_uint(size_t n) {
		int tmp;
		while (m_bufferlen < n) {
			this->m_input.read((char *)&tmp,1);
			if (sizeof(tmp) == 0) {
				std::cerr << "EOF error" << std::endl;
			}
			// On ajoute tmp au buffer
			this->m_bitbuffer = (this->m_bitbuffer << 8) | tmp;
			// On incrémente la taille du buffer
			this->m_bufferlen += 8;
		}
		this->m_bufferlen -= n;
		uint64_t result = (this->m_bitbuffer >> this->m_bufferlen) & ((static_cast<uint64_t>(1) << n) - 1);
		// Clear buffer
		this->m_bitbuffer &= (1 << this->m_bufferlen) - 1;
		return result;
	}
private:
	std::ifstream& m_input;
	long long unsigned int m_bitbuffer;
	size_t m_bufferlen;
};

void decode_file(std::ifstream& input, std::ofstream* output) {
	BitInput inp = BitInput(input);
	uint64_t header = inp.read_uint(32);
	if (header != 0x664C6143) {
		std::cerr << "Invalid magic string" << std::endl;
	}
	int type;
	size_t length;
	int samplerate = -1;
	int numchannels;
	int sampledepth;
	int numsamples;

	uint32_t vendor_length;
	std::string vendor_string = {};

	uint32_t user_comment_list_length;

	uint32_t comment_length;

	char tmp;

	bool last = false;
	while (!last) {
		last = inp.read_uint(1) != 0;
		type = inp.read_uint(7);
		length = inp.read_uint(24);
		if (type == 0) { 							// METADATA_BLOCK_STREAMINFO
			// Skip sizes
			inp.read_uint(16);
			inp.read_uint(16);
			inp.read_uint(24);
			inp.read_uint(24);
			// Get important infos
			samplerate = inp.read_uint(20);
			numchannels = inp.read_uint(3) + 1;
			sampledepth = inp.read_uint(5) + 1;
			numsamples = inp.read_uint(36);
			// Read the end of the block
			inp.read_uint(128);
		} else if (type == 4) {						// METADATA_BLOCK_VORBIS_COMMENT
			vendor_length = __builtin_bswap32(inp.read_uint(32)); // /!\ little endian reverse
			vendor_string.clear();
			for (uint32_t i = 0; i < vendor_length; ++i) {
				// Read UTF-8 characters
				tmp = inp.read_uint(8);
				vendor_string.push_back(static_cast<char>(tmp));
			}
			user_comment_list_length = __builtin_bswap32(inp.read_uint(32));

			for (uint32_t i = 0; i < user_comment_list_length; ++i) {
				comment_length = __builtin_bswap32(inp.read_uint(32));
				std::cout << "Length : " << comment_length << " octets" << std::endl;

				for (uint32_t j = 0; j < comment_length; ++j) {
					tmp = (char) inp.read_uint(8);
				}
			}
		} else {
			for (size_t i = 0; i < length; ++i) {
				inp.read_uint(8);
			}
		}
	}
	if (samplerate == -1) {
		std::cerr << "Stream info metadata block absent" << std::endl;
	}
	if (sampledepth % 8 != 0) {
		std::cerr << "Sample depth not supported" << std::endl;
	}
	// Write WAV header
	// Cout the informations
	std::cout << std::endl << "Encoding infos :" << std::endl;
	std::cout << "    - " << samplerate/1000. << "kHz" << std::endl;
	std::cout << "    - " << numchannels << " channel(s)" << std::endl;
	std::cout << "    - " << sampledepth << "-bit" << std::endl;
	//std::cout << "\t - " << numsamples << std::endl;

	std::cout << std::endl << "Music infos :" << std::endl;
	std::cout << "    - vendor_length : " << vendor_length << " octets" << std::endl;
	std::cout << "    - vendor : " << vendor_string << std::endl;
	std::cout << "    - comments : " << std::endl;
	/*for (uint32_t i = 0; i < user_comment_list_length; ++i) {
		std::cout << "        -- " << user_comment_list[i] << std::endl;
	}*/
}

int main(int argc, char** argv) {
	std::cout << "Usage : " << argv[0] << " input.flac " << " output.wav" << std::endl;
	std::filesystem::path pwd = std::filesystem::current_path();
	std::cout << "Current path : " << pwd.string() << std::endl;
	std::cout << "Number of arguments : " << argc << std::endl;
	if (argc >= 3) {
		std::cout << "Input file" << pwd.string() << "/" << argv[1] << std::endl;
		std::cout << "Output file" << pwd.string() << "/" << argv[2] << std::endl;

		std::ifstream input(pwd.string() + "/" + argv[1], std::ios::binary);
		std::ofstream output(pwd.string() + "/" + argv[2], std::ios::binary);
		//std::cout << input.rdbuf() << std::endl;
		decode_file(input,&output);
	}
	return 0;
}