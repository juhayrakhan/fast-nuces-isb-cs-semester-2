#pragma once
#include <iostream>
using namespace std;

struct PerformerOps {
	void (*greet)(void*);
	void (*perform)(void*);
	void (*taunt)(void*, void*);
	void (*cleanup)(void*);
};

struct Performer {
	PerformerOps* ops;
	char name[64];
};

struct Contortionist {
	PerformerOps* ops;
	char name[64];
	int flexibility_score;
	char signature_move[128];
};

struct FireJuggler {
	PerformerOps* ops;
	char name[64];
	int torches_in_air;
	const char** torch_names;
	int torch_count;
};

struct ShadowMimic {
	PerformerOps* ops;
	char name[64];
	void* watching;
};

struct Grudge {
	void* aggressor;
	void* target;
	Grudge* beneath;
};

extern PerformerOps contortionist_ops;
extern PerformerOps juggler_ops;
extern PerformerOps mimic_ops;

void c_greet(void* self) {
	cout << "Behold, " << ((Contortionist*)self)->name << ", the Contortionist!\n";
}
void c_perform(void* self) {
	Contortionist* c = (Contortionist*)self;
	cout << "Huzzah! " << c->name << " performs\n";
	cout << c->signature_move << '\n';
	cout << "Earning a -> " << c->flexibility_score << " <- point score!!\n";
}
void c_taunt(void* self, void* rival) {
	cout << ((Contortionist*)self)->name << " taunts " << ((Performer*)rival)->name << "\n";
}
void c_cleanup(void* self) {}

void f_greet(void* self) {
	cout << "And now, " << ((FireJuggler*)self)->name << ", the FireJuggler!\n";
}
void f_perform(void* self) {
	FireJuggler* j = (FireJuggler*)self;
	cout << "Huzzah! " << j->name << " now juggles\n";
	cout << j->torch_count << " torches at once!\n";
	for (int i = 0; i < j->torch_count; i++)
		cout << "There's " << j->torch_names[i] << "!\n";
}
void f_taunt(void* self, void* rival) {
	cout << ((FireJuggler*)self)->name << " taunts " << ((Performer*)rival)->name << "\n";
}
void f_cleanup(void* self) {
	FireJuggler* j = (FireJuggler*)self;
	if (!(j && j->torch_names)) return;
	for (int i = 0; i < j->torch_count; i++)
		delete[] j->torch_names[i];
	delete[] j->torch_names;
}

void s_greet(void* self) {
	cout << "Time for the mystical " << ((ShadowMimic*)self)->name << "!\n";
}
void s_perform(void* self) {
	ShadowMimic* m = (ShadowMimic*)self;
	if (!m->watching) {
		cout << "The Mimic stares into the void.\n";
		return;
	}
	cout << m->name << " has transformed into " << ((Performer*)(m->watching))->name << "!!\n";
	((Performer*)m->watching)->ops->perform(m->watching);
}
void s_taunt(void* self, void*) {
	ShadowMimic* m = (ShadowMimic*)self;
	if (!m->watching) {
		cout << "The Mimic stares into the void.\n";
		return;
	}
	cout << m->name << " taunts " << ((Performer*)m->watching)->name << "!!\n";
}
void s_cleanup(void* self) {}

PerformerOps contortionist_ops = { c_greet, c_perform, c_taunt, c_cleanup };
PerformerOps juggler_ops = { f_greet, f_perform, f_taunt, f_cleanup };
PerformerOps mimic_ops = { s_greet, s_perform, s_taunt, s_cleanup };

void run_show(void** roster, int count) {
	for (int i = 0; i < count; i++) {
		if (!roster[i]) continue;
		Performer* p = (Performer*)roster[i];
		p->ops->greet(roster[i]);
		p->ops->perform(roster[i]);
	}
}

void escalate_grudges(Grudge* top) {
	if (!top) return;
	((Performer*)top->aggressor)->ops->taunt(top->aggressor, top->target);
	escalate_grudges(top->beneath);
}

void tear_down_board(Grudge* top) {
	if (!top) return;
	tear_down_board(top->beneath);
	delete top;
}

void replace_performer(void** roster, int slot, void* new_performer) {
	Performer* old = (Performer*)roster[slot];
	old->ops->cleanup(old);
	roster[slot] = new_performer;
}