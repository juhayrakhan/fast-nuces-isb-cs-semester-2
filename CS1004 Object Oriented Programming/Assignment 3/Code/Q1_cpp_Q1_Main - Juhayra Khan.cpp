#include "Q1_Submission.h"
using namespace std;

void wait(double seconds) {
	if (!seconds) return;
	clock_t start = clock();
	clock_t ticks = (clock_t)(seconds * CLOCKS_PER_SEC);

	while ((clock() - start) < ticks);
}

int main() {
    srand(time(0));
    World w(20, 20);

    w.addOrganism(new FractalSprawler(2, 2, 2, 3));
    w.addOrganism(new FractalSprawler(15, 15, 4, 4));
    w.addOrganism(new KineticHunter(10, 5, 1, 1));

    for (int i = 0; i < 30; i++) {
        w.runIteration();
        w.display();
        wait(1);
        system("cls");
    }

    return 0;
}