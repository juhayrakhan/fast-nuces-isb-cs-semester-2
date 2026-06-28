#include "Q5_Submission.h"
using namespace std;

int main(int argc, char** argv) {
	int input;
	int choice;
	int selected = 0;
	Player* allplayers;
	Player* squad = new Player[15];
	int totalcount = loadSquad(allplayers, "players.csv");
	if (totalcount == 0) {
		cout << "\nError: players.csv not in same filepath as code. Please Try Again.\n";
		return 0;
	}
	while (true) {
		cout << "\n==========================================================\n\t\tPAKISTAN HEAD COACH SYSTEM\n\t\tICC T20 WORLD CUP 2026\n==========================================================";
		cout << "\n[1] Load and View Full Squad\n[2] Select World Cup Squad\n[3] Enter WC MODE\n[4] Exit\n\n-----> ";
		cin >> choice;
		switch (choice) {
		default: cout << "\nInvalid\n"; break;
		case 4: return 0;
		case 1: {
			printPlayers(allplayers, totalcount);
			break;
		}
		case 2: {
			int select;
			while (true) {
				selected = 0;
				for (; selected < 15;) {
					cout << "\n[1] View Selected Roster\n[2] Add to Roster\n[3] View Not Selected\n\n-----> "; cin >> choice;
					switch (choice) {
					default: cout << "\nInvalid\n"; break;
					case 1:
						printPlayers(squad, selected); break;
					case 2: {
						cout << "\nSelect Player from Number\n-----> "; cin >> select;
						while (select > totalcount) { cout << "Invalid: Re-Input\n-----> "; cin >> select; }
						squad[selected] = allplayers[select - 1];
						if (validateSquad(squad, selected))
							selected++;
						else cout << "\nFailed\n";
						break;
					}
					case 3: {
						cout << left << " # " << setw(18) << "Name" << setw(4) << "Age" << setw(14) << "Role" << setw(8) << "BatAvg"
							<< setw(8) << "BowlAvg" << setw(6) << "Econ" << setw(6) << "Form" << setw(6) << "Fit"
							<< setw(6) << "Exp" << setw(6) << "Play" << setw(6) << "Infl" << setw(6) << "Pres"
							<< setw(6) << "Fat" << setw(6) << "Adapt" << setw(7) << "Part" << '\n';
						for (int i = 0; i < totalcount - selected; i++)
							if (!compare(squad, selected, allplayers[i].name))
								printPlayer(allplayers[i], i);
					}
					}
				}
				printPlayers(squad, selected);
			}
		}
		}
	}
}