#include <iostream>
#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>

#include <algorithm>

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

std::string capitalize(const std::string& s) {
	std::string sout = s;
	for (size_t i = 1; i < sout.length(); i++) {
		sout[i] = tolower(s[i]);
	}
	sout[0] = toupper(s[0]);
	return sout;
}

void decode_file(std::ifstream& input, std::ofstream* output) {
	BitInput inp = BitInput(input);
	uint64_t header = inp.read_uint(32);
	if (header != 0x664C6143) {
		std::cerr << "Invalid magic string" << std::endl;
	}
	uint8_t type;
	size_t length;
	uint32_t samplerate = -1;
	uint16_t numchannels;
	uint16_t sampledepth;
	uint64_t numsamples;

	uint64_t vendor_length = 0;
	uint64_t comment_length = 0;
	std::string vendor_string = "";
	std::string comment_string = "";
	uint64_t user_comment_list_length;
	std::vector<std::string> user_comment_list;

	uint64_t picture_type;
	uint64_t mime_length = 0;
	std::string mime_string = "No_Mime";
	uint64_t desc_length = 0;
	std::string desc_string = "No_Desc";
	uint64_t width = 0;
	uint64_t height = 0;
	uint64_t depth = 0;
	uint64_t channels = 0;
	uint64_t data_length = 0;
	std::string data_string = ""; // Data -> TODO : Binary

	char charbuffer;

	bool last = false;
	
	while (!last) {
		last = inp.read_uint(1) != 0;
		type = inp.read_uint(7);
		length = inp.read_uint(24);
		if (type == 0) { 																						// METADATA_BLOCK_STREAMINFO
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
		} else if (type == 4) {																					// METADATA_BLOCK_VORBIS_COMMENT
			vendor_length = __builtin_bswap32(inp.read_uint(32)); // /!\ little-endian reverse
			vendor_string.clear();
			for (uint64_t i = 0; i < vendor_length; ++i) {
				// Read UTF-8 characters
				charbuffer = static_cast<char>(inp.read_uint(8));
				vendor_string.push_back(charbuffer);
			}

			user_comment_list_length = __builtin_bswap32(inp.read_uint(32));
			user_comment_list = std::vector<std::string>(user_comment_list_length,"");
			
			for (uint64_t i = 0; i < user_comment_list_length; ++i) {
				comment_length = __builtin_bswap32(inp.read_uint(32));
				comment_string.clear();
				for (uint64_t j = 0; j < comment_length; ++j) {
					// Read UTF-8 characters
					charbuffer = static_cast<char>(inp.read_uint(8));
					comment_string.push_back(charbuffer);
				}
				user_comment_list[i] = std::move(comment_string);
			}
		} else if (type == 6) {
			picture_type = inp.read_uint(32);
			mime_length = inp.read_uint(32);
			mime_string.clear();
			for (uint64_t i = 0; i < mime_length; ++i) {
				// Read UTF-8 characters
				charbuffer = static_cast<char>(inp.read_uint(8));
				mime_string.push_back(charbuffer);
			}
			desc_length = inp.read_uint(32);
			desc_string.clear();
			for (uint64_t i = 0; i < desc_length; ++i) {
				// Read UTF-8 characters
				charbuffer = static_cast<char>(inp.read_uint(8));
				desc_string.push_back(charbuffer);
			}
			width = inp.read_uint(32);
			height = inp.read_uint(32);
			depth = inp.read_uint(32);
			channels = inp.read_uint(32);
			data_length =  inp.read_uint(32);
			data_string.clear();
			for (uint64_t i = 0; i < data_length; ++i) {
				// Read UTF-8 characters
				charbuffer = static_cast<char>(inp.read_uint(8));
				data_string.push_back(charbuffer);
			}
		} else {																								// OTHER_BLOCKS
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
	int sec = numsamples/samplerate;
	std::string comment;
	std::string comment_head;
	std::string comment_tail;

	std::cout << std::endl;
	std::cout << "Encoding infos :" << std::endl;
	std::cout << "    - Sample rate : " << samplerate/1000. << "kHz" << std::endl;
	std::cout << "    - Channels    : " << numchannels << " channel(s)" << std::endl;
	std::cout << "    - Bit rate    : " << sampledepth << "-bit" << std::endl;
	std::cout << "    - Duration    : " << std::setw(2) << std::setfill('0') << sec/60 << ":" << (sec % 60) << " min" << std::endl;
	std::cout << std::endl;

	std::cout << std::endl;
	std::cout << "Music infos :" << std::endl;
	std::cout << "    - Vendor      : " << vendor_string << std::endl;
	std::cout << "    - Comments    : " << std::endl;
	for (uint32_t i = 0; i < user_comment_list_length; ++i) {
		comment = user_comment_list[i];
		comment_head = comment.substr(0, comment.find('='));
		comment_tail = comment.substr(comment.find('=')+1, comment.length()-1);
		comment_head = capitalize(comment_head);
		std::cout << "        - " << std::setw(12) << std::left << std::setfill(' ') << comment_head << ": " << comment_tail << std::endl;
	}
	std::cout << std::endl;

	std::cout << std::endl;
	std::cout << "Picture infos :" << std::endl;
	std::cout << "    - Type      : " << picture_type << std::endl;
	std::cout << "    - Mime      : " << mime_string << std::endl;
	std::cout << "    - Desc      : " << desc_string << std::endl;
	std::cout << "    - Width     : " << width << std::endl;
	std::cout << "    - Height    : " << height << std::endl;
	std::cout << "    - Depth     : " << depth << std::endl;
	std::cout << "    - Channel   : " << channels << std::endl;
	std::cout << "    - Data size : " << data_length << std::endl;
	std::cout << std::endl;
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

		decode_file(input,&output);
	} else if (argc == 2) {
		std::cout << "Input file : " << pwd.string() << "/" << argv[1] << std::endl;
		std::cout << "Output file : " << pwd.string() << "/" << "out.wav" << std::endl;

		std::ifstream input(pwd.string() + "/" + argv[1], std::ios::binary);
		std::ofstream output(pwd.string() + "/out.wav" , std::ios::binary);

		decode_file(input,&output);
	}
	return 0;
}