#include "Interpreter.h"
#include "Q3_Submission.h"
using namespace std;

int main() {

    Mainboard board;         
    MemoryModule ram;
    Keyboard keyboard;
    GraphicsAdapter gpu;
    Processor cpu(&board);
    board.attachCPU(&cpu);
    board.attachRAM(&ram);
    board.attachKeyboard(&keyboard);
    board.attachGPU(&gpu);
    Interpreter interpreter;
    
    char filename[100];
    cin >> filename;
    interpreter.loadProgramAndFlash(filename, ram);
    cout << "[SYSTEM] Starting ONYX-16 Execution...\n";
    cpu.run();
    gpu.flush();
    cout << "\n[SYSTEM] Execution Complete.\n";

    return 0;
}