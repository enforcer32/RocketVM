#pragma once

#include "Device.h"

#include <cstdint>

namespace RocketVM
{
	class Memory : public Device
	{
	public:
		virtual bool Init() override;
		virtual void Destroy() override;

		virtual uint8_t ReadUInt8(uint16_t offset) override;
		virtual void WriteUInt8(uint16_t offset, uint8_t value) override;
		virtual uint16_t ReadUInt16(uint16_t offset) override;
		virtual void WriteUInt16(uint16_t offset, uint16_t value) override;
		
		bool Allocate(uint16_t size, bool zero = false);

		inline uint8_t* GetMemory() { return m_Memory; }
		inline uint16_t GetMemorySize() { return m_MemSize; }

	private:
		uint16_t m_MemSize;
		uint8_t* m_Memory;
	};
}
