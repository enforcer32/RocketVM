#include "ScreenDevice.h"

#include <cmath>
#include <cstdio>

namespace RocketVM
{
	bool ScreenDevice::Init()
	{
		return true;
	}

	void ScreenDevice::Destroy()
	{
	}

	uint8_t ScreenDevice::ReadUInt8(uint16_t address)
	{
		return 0;
	}

	void ScreenDevice::WriteUInt8(uint16_t address, uint8_t value)
	{
	}

	uint16_t ScreenDevice::ReadUInt16(uint16_t address)
	{
		return 0;
	}

	void ScreenDevice::WriteUInt16(uint16_t address, uint16_t value)
	{
		uint8_t cmd = ((value & 0xff00) >> 8);
		uint8_t ch = (value & 0x00ff);
		//uint8_t x = (address % 16) + 1;
		//uint8_t y = std::floor((address / 16)) + 1;
		//printf("\x1b[${%d};${%d}H", y, (x * 2));
		printf("%c", ch);
	}
}
