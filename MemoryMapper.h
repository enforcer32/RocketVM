#pragma once

#include "Device.h"

#include <vector>

namespace RocketVM
{
	struct MemoryMapRegion
	{
		Device* Device;
		uint16_t StartAddress;
		uint16_t EndAddress;
		bool Remap;
	};

	class MemoryMapper
	{
	public:
		bool Map(Device* device, uint16_t start, uint16_t end, bool remap = true);
		//Unmap

		uint8_t ReadUInt8(uint16_t address);
		void WriteUInt8(uint16_t address, uint8_t value);
		uint16_t ReadUInt16(uint16_t address);
		void WriteUInt16(uint16_t address, uint16_t value);

	private:
		MemoryMapRegion* GetRegion(uint16_t address);

	private:
		std::vector<MemoryMapRegion> m_Regions;
	};
}
