#include <iostream>
#include <string>
#include "linenoise.hpp"

#include "CPU.h"
#include "Memory.h"
#include "MemoryMapper.h"
#include "ScreenDevice.h"

void io_program()
{
	fflush(stdin);
	fflush(stdout);

	uint16_t IP = 0;
	RocketVM::Memory* memory = new RocketVM::Memory;
	memory->Allocate(0xFFFF, true);

	// PROGRAM START

	const char* text = "HELLO";
	for (uint16_t i = 0; i < strlen(text); i++)
	{
		memory->WriteUInt8(IP++, (uint8_t)RocketVM::Opcode::MOV_REG_LIT);
		memory->WriteUInt8(IP++, (uint8_t)RocketVM::Register::RVA);
		memory->WriteUInt16(IP++, text[i]);
		IP++;

		memory->WriteUInt8(IP++, (uint8_t)RocketVM::Opcode::MOV_MEM_REG);
		memory->WriteUInt16(IP++, 0x3000 + (i * 2));
		IP++;
		memory->WriteUInt8(IP++, (uint8_t)RocketVM::Register::RVA);
	}

	memory->WriteUInt8(IP++, (uint8_t)RocketVM::Opcode::HLT);
	// PROGRAM END

	RocketVM::MemoryMapper* memorymapper = new RocketVM::MemoryMapper;
	memorymapper->Map(memory, 0x0000, 0xffff);
	memorymapper->Map(new RocketVM::ScreenDevice(), 0x3000, 0x30ff, true);

	RocketVM::CPU cpu;
	cpu.Init(memorymapper);
	cpu.Emulate();


	/*
	std::cout << "Intial State: " << std::endl;
	cpu._DumpRegisters();
	cpu._DumpStack();

	int i = 0;
	bool quit = false;
	while (!quit)
	{
		std::cout << std::endl;
		std::string line;
		quit = linenoise::Readline(("Step->" + std::to_string(i++)).c_str(), line);
		if (quit) break;

		quit = cpu.Step();
		cpu._DumpRegisters();
		cpu._DumpStack();
	}


	std::cout << "CPU Halted\n";*/
	delete memory;
}


int main()
{
	io_program();
}
