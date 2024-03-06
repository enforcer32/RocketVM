#pragma once

#include "Device.h"

namespace RocketVM
{
	class ScreenDevice : public Device
	{
	public:
		virtual bool Init() override;
		virtual void Destroy() override;

		virtual uint8_t ReadUInt8(uint16_t address) override;
		virtual void WriteUInt8(uint16_t address, uint8_t value) override;
		virtual uint16_t ReadUInt16(uint16_t address) override;
		virtual void WriteUInt16(uint16_t address, uint16_t value) override;
	};
}
