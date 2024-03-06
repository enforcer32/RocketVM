#include "Memory.h"

#include <cstring>

namespace RocketVM
{
	bool Memory::Init()
	{
		return true;
	}

	void Memory::Destroy()
	{
		delete[] m_Memory;
		m_Memory = nullptr;
		m_MemSize = 0;
	}

	uint8_t Memory::ReadUInt8(uint16_t offset)
	{
		return m_Memory[offset];
	}

	void Memory::WriteUInt8(uint16_t offset, uint8_t value)
	{
		m_Memory[offset] = value;
	}

	uint16_t Memory::ReadUInt16(uint16_t offset)
	{
		return ((uint16_t*)(m_Memory + offset))[0];
	}

	void Memory::WriteUInt16(uint16_t offset, uint16_t value)
	{
		((uint16_t*)(m_Memory + offset))[0] = value;
	}

	bool Memory::Allocate(uint16_t size, bool zero)
	{
		m_MemSize = size;
		m_Memory = new uint8_t[size];
		if (zero) memset(m_Memory, 0x00, size);
		return true;
	}
}
