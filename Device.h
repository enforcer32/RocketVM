#pragma once

#include <cstdint>

namespace RocketVM
{
	class Device
	{
	public:
		virtual ~Device() = default;
		virtual bool Init() = 0;
		virtual void Destroy() = 0;

		virtual uint8_t ReadUInt8(uint16_t address) = 0;
		virtual void WriteUInt8(uint16_t address, uint8_t value) = 0;
		virtual uint16_t ReadUInt16(uint16_t address) = 0;
		virtual void WriteUInt16(uint16_t address, uint16_t value) = 0;
	};
}
