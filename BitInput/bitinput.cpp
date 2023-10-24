#include "bitinput.hpp"

BitInput::BitInput(std::ifstream& input) : m_input (input), m_bitbuffer (0), m_bufferlen (0) {
	// Init read_state
	read_state = std::make_shared<std::streampos>(m_input.tellg());
}

uint64_t BitInput::read_uint(size_t n, bool update_state) {
	// Get read_state
	m_input.seekg(*read_state);
	//std::cout << "[Read state] : " << (*read_state) << std::endl;

	uint8_t charbuffer;
	while (m_bufferlen < n && !m_input.eof()) {
		m_input.read(reinterpret_cast<char*>(&charbuffer),1);
		// Add charbuffer to buffer
		m_bitbuffer = (m_bitbuffer << 8) | charbuffer;
		// Increment buffer length
		m_bufferlen += 8;
		// Save read_state
		if (update_state) {
			(*read_state) = (*read_state) + static_cast<std::streampos>(1);
		}
	}
    // Clear length buffer
    m_bufferlen -= n;
	//std::cout << "m_bufferlen : " << m_bufferlen << std::endl;

	uint64_t result = (m_bitbuffer >> m_bufferlen) & ((static_cast<uint64_t>(1) << n) - 1);

    // Clear length buffer
    m_bitbuffer &= (1 << m_bufferlen) - 1;

	return result;
}

int64_t BitInput::read_int(size_t n, bool update_state) {
	int64_t buffer = this->read_uint(n, update_state);
	buffer -= ((buffer >> (n - 1)) << n);
	return buffer;
}

int64_t BitInput::read_rice_int(size_t n, bool update_state) {
	int64_t value = 0;
	while (!this->read_uint(1,update_state)) {
		value++;
	}
	value = (value << n) | this->read_uint(n,update_state);
	return (value >> 1) ^ -(value & 1);
	
}