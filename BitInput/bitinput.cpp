#include "bitinput.hpp"

BitInput::BitInput(std::ifstream& input) : m_input (input), m_bitbuffer (0), m_bufferlen (0) {
	// Init read_state
	read_state = std::make_shared<std::streampos>(m_input.tellg());
}

uint64_t BitInput::read_uint(size_t n) {
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
		(*read_state) = (*read_state) + static_cast<std::streampos>(1);
	}
    // Clear length buffer
    m_bufferlen -= n;
	//std::cout << "m_bufferlen : " << m_bufferlen << std::endl;

	uint64_t result = (m_bitbuffer >> m_bufferlen) & ((static_cast<uint64_t>(1) << n) - 1);

    // Clear length buffer
    m_bitbuffer &= (1 << m_bufferlen) - 1;

	return result;
}