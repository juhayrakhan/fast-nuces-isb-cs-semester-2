#pragma once
#include <iostream>
using namespace std;

class addressessBus {
public:
	unsigned short addressess;
	addressessBus() : addressess(0) {}
};

class DataBus {
public:
	unsigned char data;
	DataBus() : data(0) {}
};

const int NONE = 0, READ_ENABLE = 1, WRITE_ENABLE = 2;

class ControlBus {
private:
	int signal;
public:
	ControlBus() : signal(NONE) {}
	int getSignal() const { return signal; }
	void setSignal(int newsig) { signal=newsig; }
};

class MemoryModule {
private:
	unsigned char memory[3840];

public:
	MemoryModule() {
		for (int i = 0; i < 3840; i++) memory[i] = 0;
	}

	unsigned char read(unsigned short address) {
		if (address >= 0x0F00) {
			cout << "[FAULT] Read OOB\n";
			return 0xFF;
		}
		return memory[address];
	}

	void write(unsigned short address, unsigned char val) {
		if (address >= 0x0F00) {
			cout << "[FAULT] Write OOB\n";
			return;
		}
		memory[address] = val;
	}
	void loadRawBinary(unsigned short address, unsigned short word) {
		memory[address] = (word >> 8) & 0xFF;
		memory[address + 1] = word & 0xFF;
	}
};

class Keyboard {
private:
	char buffer[256];
	int start, end;

public:
	Keyboard() : start(0), end(0) { for (int i = 0; i < 256; i++) buffer[i] = '\0'; }

	void pushInput(const char* str) {
		for (int i = 0; str[i] != '\0'; i++)
			buffer[end++] = str[i];
		buffer[end++] = ' ';
	}

	unsigned char readChar() {
		if (start == end) {
			cout << "[INPUT]: ";
			char temp[100];
			cin >> temp;
			pushInput(temp);
		}
		start++;
		return buffer[start-1];
	}
};

class PhosphorDisplay {
private:
	char buffer[1024];
	int index;

public:
	PhosphorDisplay() : index(0) { for (int i = 0; i < 1024; i++) buffer[i] = '\0'; }

	void append(char c) {
		if (index < 1023) buffer[index] = c, index++;
	}

	void flush() {
		buffer[index] = '\0';
		cout << "\n==== DISPLAY ====\n";
		cout << buffer << endl;
	}
};

class GraphicsAdapter {
private:
	PhosphorDisplay display;

public:
	void writeChar(unsigned char val) {	display.append(val);}
	void writeInt(unsigned char val) {
		int num = val;

		if (num == 0) {
			display.append('0');
			return;
		}

		char temp[10];
		int i = 0;

		while (num > 0) {
			temp[i] = '0' + (num % 10);
			num /= 10;
			i++;
		}
		for (int j = i - 1; j >= 0; j--) display.append(temp[j]);
	}

	void flush() {	display.flush();}
};

class RegisterBank {
private:
	unsigned short R[8];
	unsigned short PC;
	unsigned short IR;
	unsigned char FLAGS;

public:
	RegisterBank() {
		for (int i = 0; i < 8; i++) R[i] = 0;
		PC = IR = FLAGS = 0;
	}

	unsigned short readR(int i) { return R[i]; }
	void writeR(int i, unsigned short val) { R[i] = val; }
	unsigned short getPC() { return PC; }
	void setPC(unsigned short v) { PC = v; }
	unsigned short getIR() { return IR; }
	void setIR(unsigned short v) { IR = v; }
	unsigned char getFLAGS() { return FLAGS; }
	void setFLAGS(unsigned char v) { FLAGS = v; }
};

class ALU {
public:
	unsigned short execute(unsigned char opcode, unsigned short A, unsigned short B, unsigned char& FLAGS) {

		unsigned short result = 0;
		switch (opcode) {
		case 0x01: result = A + B; break;
		case 0x02: result = A - B; break;
		case 0x03: result = A * B; break;
		case 0x04: result = (B == 0) ? 0 : A / B; break;
		case 0x0A:
			if (A == B) FLAGS = 0x01;
			else if (A < B) FLAGS = 0x02;
			else FLAGS = 0x04;
			return 0;
		}
		if (result == 0) FLAGS = 0x01;
		else FLAGS = 0;

		return result;
	}
};

class Processor;
class Mainboard {
private:
	addressessBus addressBus;
	DataBus dataBus;
	ControlBus ctrlBus;
	Processor* cpu;
	MemoryModule* ram;
	Keyboard* keyboard;
	GraphicsAdapter* gpu;

public:
	Mainboard() : cpu(0), ram(0), keyboard(0), gpu(0) {}

	void attachCPU(Processor* p) { cpu = p; }
	void attachRAM(MemoryModule* m) { ram = m; }
	void attachKeyboard(Keyboard* k) { keyboard = k; }
	void attachGPU(GraphicsAdapter* g) { gpu = g; }

	void pulseClock() {
		unsigned short address = addressBus.addressess;

		if (ctrlBus.getSignal() == READ_ENABLE) {
			if (address == 0x0FF0) dataBus.data = keyboard->readChar();
			else dataBus.data = ram->read(address);
			ctrlBus.setSignal(NONE);
		}

		else if (ctrlBus.getSignal() == WRITE_ENABLE) {
			if (address == 0x0FF1)	gpu->writeChar(dataBus.data);
			else if (address == 0x0FF2)	gpu->writeInt(dataBus.data);
			else	ram->write(address, dataBus.data);
			ctrlBus.setSignal(NONE);
		}
	}

	addressessBus* getaddressessBus() { return &addressBus; }
	DataBus* getDataBus() { return &dataBus; }
	ControlBus* getControlBus() { return &ctrlBus; }
};

class Processor {
private:
	RegisterBank regs;
	ALU alu;

	addressessBus* addressBus;
	DataBus* dataBus;
	ControlBus* ctrlBus;
	Mainboard* board;

public:
	Processor(Mainboard* mb) {
		board = mb;
		addressBus = mb->getaddressessBus();
		dataBus = mb->getDataBus();
		ctrlBus = mb->getControlBus();
	}

	unsigned char readByte(unsigned short address) {
		addressBus->addressess = address;
		ctrlBus->setSignal(READ_ENABLE);
		board->pulseClock();
		return dataBus->data;
	}
	void writeByte(unsigned short address, unsigned char val) {
		addressBus->addressess = address;
		dataBus->data = val;
		ctrlBus->setSignal(WRITE_ENABLE);
		board->pulseClock();
	}

	void run() {
		while (true) {

			unsigned short pc = regs.getPC();
			unsigned char high = readByte(pc);
			unsigned char low = readByte(pc + 1);
			unsigned short instruction = (high << 8) | low;
			regs.setIR(instruction);
			regs.setPC(pc + 2);

			unsigned char opcode = (instruction >> 8) & 0xFF;
			unsigned char regto = (instruction >> 4) & 0x0F;
			unsigned char regfrm = instruction & 0x0F;

			switch (opcode) {
			case 1: case 2: case 3: case 4: {
				unsigned short A = regs.readR(regto);
				unsigned short B = regs.readR(regfrm);
				unsigned char flags = regs.getFLAGS();

				unsigned short result = alu.execute(opcode, A, B, flags);
				regs.writeR(regto, result);
				break;
			}
			case 0x0A: {
				unsigned short A = regs.readR(regto);
				unsigned short B = regs.readR(regfrm);

				unsigned char flags = regs.getFLAGS();
				alu.execute(opcode, A, B, flags);
				regs.setFLAGS(flags);
				break;
			}
			case 0x20: {
				unsigned short address = regs.readR(regfrm);
				unsigned char val = readByte(address);
				regs.writeR(regto, val);
				break;
			}
			case 0x21: {
				unsigned short address = regs.readR(regfrm);
				unsigned short val = regs.readR(regto);
				writeByte(address, (unsigned char)val);
				break;
			}
			case 0x10: {
				regs.setPC(regs.readR(regfrm));
				break;
			}
			case 0x1A: {
				unsigned char regto = (instruction >> 4) & 0x0F;
				unsigned char imm = instruction & 0x0F;

				regs.writeR(regto, imm);
				break;
			}
			case 0x11: {
				unsigned char regfrm = instruction & 0x0F;
				unsigned char flags = regs.getFLAGS();
				if (flags & 0x01) {
					unsigned short target = regs.readR(regfrm);
					regs.setPC(target);
				}
				break;
			}
			case 0x1B: {
				unsigned char regto = (instruction >> 4) & 0x0F;
				unsigned short pc = regs.getPC();
				unsigned char high = readByte(pc);
				unsigned char low = readByte(pc + 1);
				unsigned short imm = (high << 8) | low;

				regs.setPC(pc + 2);

				regs.writeR(regto, imm);
				break;
			}
			case 0x12: {
				unsigned char regfrm = instruction & 0x0F;
				unsigned char flags = regs.getFLAGS();

				if ((flags & 0x01) == 0) {
					unsigned short target = regs.readR(regfrm);
					regs.setPC(target);
				}
				break;
			}
			case 0x00:
				cout << "[END]\n";
				return;
			}
		}
	}
};