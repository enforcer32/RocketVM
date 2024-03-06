#pragma once

#include <cstdint>
#include <unordered_map>
#include <string>

#include "MemoryMapper.h"

namespace RocketVM
{
	enum class Register : uint8_t
	{
		// Special Registers
		RVIP = 0x0A,
		RVACC = 0x0B,
		RVSP = 0x0C,
		RVBP = 0x0D,

		// General Purpose Registers
		RVA = 0x01,
		RVB = 0x02,
		RVC = 0x03,
		RVD = 0x04,
		RVE = 0x05,
		RVF = 0x06,
		RVG = 0x07,
		RVH = 0x08,
	};

	enum class Opcode : uint8_t
	{
		// Data Manipulation
		MOV_REG_LIT = 0x30,
		MOV_REG_REG = 0x31,
		MOV_REG_MEM = 0x32,
		MOV_MEM_LIT = 0x33,
		MOV_MEM_REG = 0x34,	

		// Stack
		PUSH_LIT = 0x38,
		PUSH_REG = 0x39,
		POP_REG = 0x3B,

		// Control Flow
		CALL_LABEL = 0x3E,
		CALL_REG = 0x3F,
		RET = 0x40,
		HLT = 0x41,

		JNE_MEM_LIT = 0x50,


		// PUSH 0xB0, POP 0xC0, LOAD, 0xD0

		// Arithmetic
		ADD_REG_REG = 0xE0,
		ADD_REG_LIT = 0xE1,
	};

	class CPU
	{
	public:
		bool Init(MemoryMapper* memory);

		uint16_t GetRegister(Register name);
		void SetRegister(Register name, uint16_t value);

		uint8_t Fetch8();
		uint16_t Fetch16();
		
		uint16_t Execute(Opcode instruction);
		uint16_t Step();
		void Emulate();

		void _DumpRegisters();
		void _DumpMemory(uint16_t addr);
		void _DumpStack();

	private:
		void PushStack16(uint16_t value);
		uint16_t PopStack16();

		void PushCPUState();
		void PopCPUState();

	private:
		MemoryMapper* m_Memory;
		std::unordered_map<Register, uint16_t> m_Registers;
		uint16_t m_StackFrameSize;
	};
}
