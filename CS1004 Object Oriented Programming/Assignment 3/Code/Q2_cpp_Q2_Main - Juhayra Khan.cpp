#include <iostream>
#include "Q2_Submission.h"
#include <fstream>
using namespace std;

int main() {
    Kingdom k1, k2;
    AethelGardEngine engine(&k1,&k2);

    for (int i = 0; i < 10; i++) {
        engine.realms[i] = nullptr;
    }

    loadAll("scenario_alpha.csv", engine);

    printKingdoms(engine);
    printRelations(engine);
    printAssassins(engine);
    printLords(engine);

    cout << "\nThreatMax: " << engine.threatMax << endl;

    return 0;
}