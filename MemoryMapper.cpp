#include "MemoryMapper.h"

#include <stdexcept>

namespace RocketVM
{
	bool MemoryMapper::Map(Device* device, uint16_t start, uint16_t end, bool remap)
	{
		m_Regions.insert(m_Regions.begin(), { device, start, end, remap });
		return true;
	}

	uint8_t MemoryMapper::ReadUInt8(uint16_t address)
	{
		MemoryMapRegion* region = GetRegion(address);
		uint16_t addr = region->Remap ? address - region->StartAddress : address;
		return region->Device->ReadUInt8(addr);
	}

	void MemoryMapper::WriteUInt8(uint16_t address, uint8_t value)
	{
		MemoryMapRegion* region = GetRegion(address);
		uint16_t addr = region->Remap ? address - region->StartAddress : address;
		region->Device->WriteUInt8(addr, value);
	}

	uint16_t MemoryMapper::ReadUInt16(uint16_t address)
	{
		MemoryMapRegion* region = GetRegion(address);
		uint16_t addr = region->Remap ? address - region->StartAddress : address;
		return region->Device->ReadUInt16(addr);
	}

	void MemoryMapper::WriteUInt16(uint16_t address, uint16_t value)
	{
		MemoryMapRegion* region = GetRegion(address);
		uint16_t addr = region->Remap ? address - region->StartAddress : address;
		region->Device->WriteUInt16(addr, value);
	}

	MemoryMapRegion* MemoryMapper::GetRegion(uint16_t address)
	{
		for (auto& region : m_Regions)
			if (address >= region.StartAddress && address <= region.EndAddress)
				return &region;
		throw std::runtime_error("RocketVM->MemoryMapper->GetRegion: Invalid Region Address");
	}
}
