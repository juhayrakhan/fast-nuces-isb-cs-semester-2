#include "Q1_Submission.h"
using namespace std;

int main(){

	Contortionist a = { &contortionist_ops, "Mirela", 9, "AAA" };
	Contortionist b = { &contortionist_ops, "SomeOther", 4, "EEE" };
	const char** torchnames = new const char* [2];
	torchnames[0] = "torch1";
	torchnames[1] = "torch2";
	const char** torchnames3 = new const char* [2];
	torchnames3[0] = "torchA";
	torchnames3[1] = "torchB";
	FireJuggler c = { &juggler_ops, "Drazenko", 2,torchnames,2 };
	FireJuggler d = { &juggler_ops, "Dracula_idk", 2,torchnames3,2 };
	ShadowMimic e = { &mimic_ops, "The Mimic", &c };

	const char** torchnames2 = new const char* [4];
	torchnames2[0] = "torch1";
	torchnames2[1] = "torch2";
	torchnames[2] = "torch3";
	torchnames[3] = "torch4";
	Contortionist x = { &contortionist_ops, "Grayson", 9, "AAA" };
	FireJuggler y = { &juggler_ops, "Valeria", 4,torchnames2,4 };

	const int perfcount = 5;
	void* myroster[perfcount] = { &a,&b,&c,&d,&e };

	Grudge* card1 = new Grudge{ &d,&c,nullptr };
	Grudge* card2 = new Grudge{ &a,&d,card1 };
	Grudge* card3 = new Grudge{ &e,&a,card2 };
	int input;
	while (true) {
		cout << "\n-------> THE CURSED CARNIVAL <-------\n\n1.\tView Performers\n2.\tRun Show\n3.\tGo Through Grudges\n4.\tReplace Performer\nOther.\tExit\n\n---> ";
		cin >> input; cout << '\n';
		switch (input) {
		default: return 0;
		case 1:
			for (int i = 0; i < perfcount; i++)
				((Performer*)myroster[i])->ops->greet(myroster[i]);
			break;
		case 2:
			for (int i = 0; i < perfcount; i++)
				((Performer*)myroster[i])->ops->perform(myroster[i]);
			break;
		case 3:
			escalate_grudges(card3);
			break;
		case 4:
			cout << "Other Performers Unused: \n1. ";
			((Performer*)&x)->ops->greet(&x); cout << "\n2. ";
			((Performer*)&y)->ops->greet(&y);
			cout << "----> "; cin >> input;
			Performer* p=nullptr;
			switch(input){
			default: break;
			case 1: p = ((Performer*)&x); break;
			case 2: p = ((Performer*)&y);
			}
			if (!p) {
				cout << "\nInvalid\n";
				break;
			}
			cout << "Which index are you replacing?\n----> ";
			cin >> input;
			if (input < 0 || input>4) {
				cout << "\nInvalid, between 0 and 4\n";
				break;
			}
			replace_performer(myroster, input, p);
			cout << "\n\nReplacement Successful\n";
		}
	}

}