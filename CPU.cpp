#include "CPU.h"
#include <stdexcept>

#include <iostream>

namespace RocketVM
{
	bool CPU::Init(MemoryMapper* memory)
	{
		m_Memory = memory;

		// Init Registers
		m_Registers[Register::RVA] = 0;
		m_Registers[Register::RVB] = 0;
		m_Registers[Register::RVC] = 0;
		m_Registers[Register::RVD] = 0;
		m_Registers[Register::RVE] = 0;
		m_Registers[Register::RVF] = 0;
		m_Registers[Register::RVG] = 0;
		m_Registers[Register::RVH] = 0;

		m_Registers[Register::RVIP] = 0;
		m_Registers[Register::RVACC] = 0;

		// MEMORY MAP STACK LATER (0x0000 - 0xffff) address space available
		m_Registers[Register::RVSP] = (0xffff - 1); // memsize = 1024, point at 1023, (use 1023 and 1024) (2bytes) for storing. -1 because indexing starts at 0 (1022, 1023)
		m_Registers[Register::RVBP] = (0xffff - 1);

		m_StackFrameSize = 0;

		return true;
	}

	uint16_t CPU::GetRegister(Register name)
	{
		if (m_Registers.find(name) == m_Registers.end())
			throw std::runtime_error("RocketVM->CPU->GetRegister: Invalid Register");

		return m_Registers[name];
	}

	void CPU::SetRegister(Register name, uint16_t value)
	{
		if (m_Registers.find(name) == m_Registers.end())
			throw std::runtime_error("RocketVM->CPU->SetRegister: Invalid Register");

		m_Registers[name] = value;
	}

	uint8_t CPU::Fetch8()
	{
		uint16_t ip = GetRegister(Register::RVIP);
		uint8_t instr = m_Memory->ReadUInt8(ip);
		SetRegister(Register::RVIP, ip + 1);
		return instr;
	}

	uint16_t CPU::Fetch16()
	{
		uint16_t ip = GetRegister(Register::RVIP);
		uint16_t instr = m_Memory->ReadUInt16(ip);
		SetRegister(Register::RVIP, ip + 2);
		return instr;
	}

	uint16_t CPU::Execute(Opcode instruction)
	{
		switch (instruction)
		{
			// MOV RVx, 0x1234
		case Opcode::MOV_REG_LIT:
		{
			uint8_t r1 = Fetch8();
			uint16_t val = Fetch16();
			SetRegister((Register)r1, val);
			break;
		}
			// MOV RVx, RVx
		case Opcode::MOV_REG_REG:
		{
			uint8_t r1 = Fetch8();
			uint8_t r2 = Fetch8();
			uint16_t r2val = GetRegister(static_cast<Register>(r2));
			SetRegister((Register)r1, r2val);
			break;
		}
			// MOV RVx, 0x1234
		case Opcode::MOV_REG_MEM:
		{
			uint8_t r1 = Fetch8();
			uint16_t addr = Fetch16();
			uint16_t memval = m_Memory->ReadUInt16(addr);
			SetRegister((Register)r1, memval);
			break;
		}
			// MOV 0x1234, RVx
		case Opcode::MOV_MEM_REG:
		{
			uint16_t addr = Fetch16();
			uint8_t r1 = Fetch8();
			uint16_t r1val = GetRegister((Register)r1);
			m_Memory->WriteUInt16(addr, r1val);
			break;
		}
			// PUSH 0x1234
		case Opcode::PUSH_LIT:
		{
			uint16_t val = Fetch16();
			PushStack16(val);
			break;
		}
			// PUSH RVx
		case Opcode::PUSH_REG:
		{
			uint8_t r1 = Fetch8();
			uint16_t r1val = GetRegister((Register)r1);
			PushStack16(r1val);
			break;
		}
			// POP RVx
		case Opcode::POP_REG:
		{
			uint8_t r1 = Fetch8();
			uint16_t sval = PopStack16();
			SetRegister((Register)r1, sval);
			break;
		}
			// CALL 0x1234
		case Opcode::CALL_LABEL:
		{
			uint16_t addr = Fetch16();
			PushCPUState();
			SetRegister(Register::RVIP, addr);
			break;
		}
			// CALL RVx
		case Opcode::CALL_REG:
		{
			uint8_t r1 = Fetch8();
			uint16_t r1val = GetRegister((Register)(r1));
			PushCPUState();
			SetRegister(Register::RVIP, r1val);
			break;
		}
			// RET
		case Opcode::RET:
		{
			PopCPUState();
			break;
		}
			// HLT
		case Opcode::HLT:
		{
			return 1;
		}
			// JNE 0x1234, 0xABCD
		case Opcode::JNE_MEM_LIT:
		{
			uint16_t addr = Fetch16();
			uint16_t val = Fetch16();
			if (GetRegister(Register::RVACC) != val)
				SetRegister(Register::RVIP, addr);
			break;
		}
			// ADD RVx, RVx
		case Opcode::ADD_REG_REG:
		{
			uint8_t r1 = Fetch8(); // Get Register OpCode from Memory (EX: ADD RVA, RVB) 0xA0 0x1, 0x2
			uint8_t r2 = Fetch8();
			uint16_t r1val = GetRegister(static_cast<Register>(r1));
			uint16_t r2val = GetRegister(static_cast<Register>(r2));
			SetRegister(Register::RVACC, (r1val + r2val));
			break;
		}
		default:
			break;
		}

		return 0;
	}

	uint16_t CPU::Step()
	{
		uint8_t instruction = Fetch8();
		return Execute((Opcode)instruction);
	}

	void CPU::Emulate()
	{
		uint16_t hlt = Step();
		while (!hlt)
			hlt = Step();
	}

	void CPU::_DumpRegisters()
	{
		printf("Register Dump: \n");
		for (const auto& [key, val] : m_Registers)
		{
			printf("0x%X : 0x%04X\n", key, val);
		}
		std::cout << std::endl;
	}

	void CPU::_DumpMemory(uint16_t addr)
	{
		printf("Memory Dump: 0x%04X -> 0x%04X\n", addr, m_Memory->ReadUInt16(addr));
		printf("0x%04X:", addr);
		for(int i = 0; i < 8; i++)
			printf(" 0x%02X", m_Memory->ReadUInt8(addr + i));
		printf("\n");
	}

	void CPU::_DumpStack()
	{
		uint16_t sp = GetRegister(Register::RVSP) + 2;
		printf("Stack Dump: 0x%04X -> 0x%04X\n", sp, (uint16_t)m_Memory->ReadUInt16(sp));
		printf("0x%04X:", sp);
		for (int i = 0; i < 2; i++)
			printf(" 0x%02X", m_Memory->ReadUInt8(sp + i));
		printf("\n\n");
	}

	void CPU::PushStack16(uint16_t value)
	{
		m_StackFrameSize += 2;
		uint16_t sp = GetRegister(Register::RVSP);
		m_Memory->WriteUInt16(sp, value);
		SetRegister(Register::RVSP, sp - 2);
	}

	uint16_t CPU::PopStack16()
	{
		m_StackFrameSize -= 2;
		uint16_t prevsp = GetRegister(Register::RVSP) + 2;
		SetRegister(Register::RVSP, prevsp);
		return m_Memory->ReadUInt16(prevsp);
	}

	void CPU::PushCPUState()
	{
		PushStack16(GetRegister(Register::RVA));
		PushStack16(GetRegister(Register::RVB));
		PushStack16(GetRegister(Register::RVC));
		PushStack16(GetRegister(Register::RVD));
		PushStack16(GetRegister(Register::RVE));
		PushStack16(GetRegister(Register::RVF));
		PushStack16(GetRegister(Register::RVG));
		PushStack16(GetRegister(Register::RVH));
		PushStack16(GetRegister(Register::RVIP));
		PushStack16(m_StackFrameSize + 2);

		SetRegister(Register::RVBP, GetRegister(Register::RVSP));
		m_StackFrameSize = 0;
	}

	void CPU::PopCPUState()
	{
		uint16_t bp = GetRegister(Register::RVBP);
		SetRegister(Register::RVSP, bp);

		uint16_t frameSize = PopStack16();
		SetRegister(Register::RVIP, PopStack16());
		SetRegister(Register::RVH, PopStack16());
		SetRegister(Register::RVG, PopStack16());
		SetRegister(Register::RVF, PopStack16());
		SetRegister(Register::RVE, PopStack16());
		SetRegister(Register::RVD, PopStack16());
		SetRegister(Register::RVC, PopStack16());
		SetRegister(Register::RVB, PopStack16());
		SetRegister(Register::RVA, PopStack16());

		uint16_t argCount = PopStack16();
		for (int i = 0; i < argCount; i++)
			PopStack16();

		SetRegister(Register::RVBP, bp + frameSize);
	}
}
