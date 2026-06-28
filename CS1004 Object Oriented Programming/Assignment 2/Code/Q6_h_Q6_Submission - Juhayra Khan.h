#pragma once
#include <iostream>
using namespace std;

int length(const char* str) {
	int len = 0;
	for (; str[len] != '\0'; len++);
	return len;
}
int compare(const char* a, const char* b) {
	int i = 0;
	while (a[i] && b[i]) {
		if (a[i] != b[i]) return a[i] - b[i];
		i++;
	}
	return a[i] - b[i];
}

struct Movie;

struct Actor {
	int id;
	const char* name;
	int age;
	Movie** filmography;
	int filmCount;
	Movie* latestRole;
	Actor** rival;
	int rivalcount;
	void addmetomovie(Movie* movie) {
		latestRole = movie;
		if (!filmography) filmography = new Movie * [1];
		else {
			Movie** temp = new Movie * [filmCount + 1];
			for (int i = 0; i < filmCount; i++) temp[i] = filmography[i];
			delete[] filmography; filmography = temp;
		}
		filmography[filmCount] = movie;
		filmCount++;
	}
	void reduce() {
		filmCount--;
		if (!filmCount) filmography = nullptr;
		Movie** resized = new Movie * [filmCount];
		for (int i = 0; i < filmCount; i++)
			resized[i] = filmography[i];
		delete[] filmography; filmography = resized;
		if (!filmCount) latestRole = nullptr;
		else latestRole = filmography[filmCount - 1];
	}
	bool isRival(Actor* a) {
		for (int i = 0; i < rivalcount; i++)
			if (rival[i] == a) return true;
		return false;
	}
};

struct Movie {
	int id;
	const char* title;
	const char** flaggedTitles;
	int flaggedcount;
	Actor** cast;
	int castCount;
	Actor* leadActor;
	int year;
	void addtomovie(Actor* actor) {
		if (!cast) {
			cast = new Actor * [1];
			leadActor = actor;
		}
		cast[castCount] = actor;
		castCount++;
	}
	void reduce() {
		castCount--;
		if (!castCount) cast = nullptr;
		Actor** resized = new Actor * [castCount];
		for (int i = 0; i < castCount; i++)
			resized[i] = cast[i];
		cast = resized;
		if (!castCount)leadActor = nullptr;
		else leadActor = cast[0];
	}
	bool foundrival(Actor* a) {
		for (int i = 0; i < castCount; i++)
			if (cast[i]->isRival(a)) return true;
			else if (a->isRival(cast[i])) return true;
		return false;
	}
};

Actor* addActor(int id, const char* name, int age) {
	if (name == nullptr || age == 0) return nullptr;
	Actor* newactor = new Actor;
	newactor->filmCount = 0;
	newactor->filmography = nullptr;
	newactor->name = name;
	newactor->age = age;
	newactor->id = id;
	newactor->rival = nullptr;
	newactor->rivalcount = 0;

	return newactor;
}
Movie* addMovie(int id, const char* title, int year, float rating) {
	if (title == nullptr || year == 0) return nullptr;
	Movie* newmovie = new Movie;
	newmovie->castCount = 0;
	newmovie->flaggedTitles = nullptr;
	newmovie->flaggedcount = 0;
	newmovie->title = title;
	newmovie->year = year;
	newmovie->id = id;
	newmovie->cast = nullptr;
	return newmovie;
}

int castActor(Actor* a, Movie* m) {
	if (m->foundrival(a)) return -1;

	m->addtomovie(a);
	a->addmetomovie(m);

	return 0;

}

int removeActorFromMovie(Actor* a, Movie* m) {
	int i;

	for (i = 0; i < m->castCount; i++)
		if (m->cast[i] == a)	break;
	if (i == m->castCount) return -1;
	i++;
	for (; i < m->castCount; i++)
		m->cast[i - 1] = m->cast[i];
	m->reduce();

	for (i = 0; i < a->filmCount; i++)
		if (a->filmography[i] == m)	break;
	if (i == a->filmCount) return 0;
	for (; i < a->filmCount; i++)
		a->filmography[i - 1] = a->filmography[i];
	a->reduce();
	return 0;
}

void addRival(Actor* a, Actor* rival) {
	a->rivalcount++;
	Actor** newrival = new Actor * [a->rivalcount];
	for (int i = 0; i < a->rivalcount - 1; i++)
		newrival[i] = a->rival[i];
	newrival[a->rivalcount - 1] = rival;
	a->rival = newrival;
}

void swapLeads(Movie* m1, Movie* m2) {
	Actor* temp = m1->leadActor;
	m1->leadActor = m2->leadActor;
	m2->leadActor = temp;
	removeActorFromMovie(m1->leadActor, m2);
	removeActorFromMovie(m2->leadActor, m1);
}

void addFlaggedTitle(Movie* m, const char* title) {
	m->flaggedcount++;
	const char** resized = new const char* [m->flaggedcount];
	for (int i = 0; i < m->flaggedcount - 1; i++)
		resized[i] = m->flaggedTitles[i];
	resized[m->flaggedcount - 1] = title;
	if (m->flaggedTitles) delete[] m->flaggedTitles;
	m->flaggedTitles = resized;
}

Movie* findflaggedtitle(Movie* m, const char* title) {
	for (int i = 0; i < m->flaggedcount; i++)
		if (!compare(m->flaggedTitles[i], title)) return m;
	return nullptr;
}

struct Vault {
	bool isledger;
	int titlecount;
	int max;
	char** titles;

	Movie** movies;
	Vault* nextledger;
	Vault* prevledger;

	Vault** outgoing;
	int outgoingcount;
};

struct Archive {
	int order;
	Vault* root;
};

Vault* createVault(bool isledger, int order) {
	Vault* v = new Vault;
	v->isledger = isledger;
	v->titlecount = 0;
	v->max = order;

	v->titles = new char* [order + 1];

	if (isledger) {
		v->movies = new Movie * [order + 1];
		v->nextledger = v->prevledger = nullptr;
	}
	else {
		v->outgoing = new Vault * [order + 2];
		v->outgoingcount = 0;
	}

	return v;
}
Archive* createArchive(int order) {
	Archive* a = new Archive;
	a->order = order;
	a->root = createVault(true, order);
	return a;
}

Vault* traverseArchive(Vault* root, const char* title) {
	Vault* current = root;
	while (!current->isledger) {
		int i;
		for (i = 0; i < current->titlecount != 0 && compare(title, current->titles[i]) >= 0; i++);
		current = current->outgoing[i];
	}
	return current;
}
Vault* findprevious(Vault* current, Vault* next) {
	if (current->isledger) return nullptr;
	for (int i = 0; i <= current->titlecount; i++) {
		if (current->outgoing[i] == next)
			return current;
		Vault* res = findprevious(current->outgoing[i], next);
		if (res != nullptr) return res;
	}
	return nullptr;
}

void splitledger(Archive*, Vault*);
void addabove(Archive* arch, Vault* left, char* key, Vault* right) {

	if (arch->root == left) {
		Vault* newRoot = createVault(false, arch->order);
		newRoot->titles[0] = key;
		newRoot->outgoing[0] = left;
		newRoot->outgoing[1] = right;
		newRoot->titlecount = 1;

		arch->root = newRoot;
		return;
	}

	Vault* prev = findprevious(arch->root, left);

	int i;
	for (i = 0; i < prev->titlecount && compare(prev->titles[i], key) < 0; i++);

	for (int j = prev->titlecount; j > i; j--) {
		prev->titles[j] = prev->titles[j - 1];
		prev->outgoing[j + 1] = prev->outgoing[j];
	}
	prev->titles[i] = key;
	prev->outgoing[i + 1] = right;
	prev->titlecount++;

	if (prev->titlecount > prev->max)
		splitledger(arch, prev);
}
void insertIntoledger(Vault* ledger, Movie* m) {
	int i = 0;
	while (i < ledger->titlecount && compare(ledger->titles[i], m->title) < 0)
		i++;

	for (int j = ledger->titlecount; j > i; j--) {
		ledger->titles[j] = ledger->titles[j - 1];
		ledger->movies[j] = ledger->movies[j - 1];
	}

	ledger->titles[i] = (char*)m->title;
	ledger->movies[i] = m;
	ledger->titlecount++;
}
void splitledger(Archive* arch, Vault* ledger) {

	Vault* newledger = createVault(true, ledger->max);

	for (int i = ledger->titlecount / 2, j = 0; i < ledger->titlecount; i++, j++) {
		newledger->titles[j] = ledger->titles[i];
		newledger->movies[j] = ledger->movies[i];
		newledger->titlecount++;
	}

	ledger->titlecount = ledger->titlecount / 2;;
	newledger->nextledger = ledger->nextledger;
	if (newledger->nextledger)
		newledger->nextledger->prevledger = newledger;
	ledger->nextledger = newledger;
	newledger->prevledger = ledger;

	char* key = newledger->titles[0];
	addabove(arch, ledger, key, newledger);
}

void archiveInsert(Archive* arch, Movie* m) {
	Vault* ledger = traverseArchive(arch->root, m->title);
	insertIntoledger(ledger, m);
	if (ledger->titlecount > ledger->max)
		splitledger(arch, ledger);
}
Movie* archiveSearch(Archive* arch, const char* title) {
	Vault* ledger = traverseArchive(arch->root, title);
	for (int i = 0; i < ledger->titlecount; i++)
		if (!compare(ledger->titles[i], title))
			return ledger->movies[i];
	Vault* cur = ledger;

	//flaggedtitle
	while (cur->prevledger != nullptr) cur = cur->prevledger;
	while (cur != nullptr) {
		for (int i = 0; i < cur->titlecount; i++)
			if (findflaggedtitle(cur->movies[i], title))
				return cur->movies[i];
		cur = cur->nextledger;
	}

	return nullptr;
}

void archiveRangeQuery(Archive* arch, const char* from, const char* to) {
	Vault* ledger = traverseArchive(arch->root, from);

	while (ledger) {
		for (int i = 0; i < ledger->titlecount; i++) {
			if (compare(ledger->titles[i], from) >= 0 &&
				compare(ledger->titles[i], to) <= 0) {

				cout << ledger->movies[i]->title << endl;
			}
			else if (compare(ledger->titles[i], to) > 0)
				return;
		}
		ledger = ledger->nextledger;
	}
}

void removeFromledger(Vault* ledger, const char* title) {
	int i;
	for (i = 0; i < ledger->titlecount && compare(ledger->titles[i], title) != 0; i++);
	if (i == ledger->titlecount) return;
	for (; i < ledger->titlecount - 1; i++) {
		ledger->titles[i] = ledger->titles[i + 1];
		ledger->movies[i] = ledger->movies[i + 1];
	}
	ledger->titlecount--;
}
bool borrowFromLeft(Vault* prev, int idx) {
	Vault* left = prev->outgoing[idx - 1];
	Vault* cur = prev->outgoing[idx];

	if (left->titlecount > (left->max + 1) / 2) {
		for (int i = cur->titlecount; i > 0; i--) {
			cur->titles[i] = cur->titles[i - 1];
			cur->movies[i] = cur->movies[i - 1];
		}
		cur->titles[0] = left->titles[left->titlecount - 1];
		cur->movies[0] = left->movies[left->titlecount - 1];
		cur->titlecount++;	left->titlecount--;
		prev->titles[idx - 1] = cur->titles[0];
		return true;
	}
	return false;
}
void mergeLeaves(Vault* left, Vault* right) {
	for (int i = 0; i < right->titlecount; i++) {
		left->titles[left->titlecount] = right->titles[i];
		left->movies[left->titlecount] = right->movies[i];
		left->titlecount++;
	}
	left->nextledger = right->nextledger;
	if (right->nextledger)
		right->nextledger->prevledger = left;
	delete right; right = nullptr;
}

int archiveDelete(Archive* arch, const char* title) {
	Vault* ledger = traverseArchive(arch->root, title);
	int before = ledger->titlecount;
	removeFromledger(ledger, title);
	if (ledger->titlecount == before) return -1;
	if (ledger == arch->root) {
		if (ledger->titlecount == 0) {
			delete ledger; ledger = nullptr;
			arch->root = createVault(true, arch->order);
		}
		return 0;
	}
	if (ledger->titlecount >= (ledger->max + 1) / 2) return 0;

	Vault* prev = findprevious(arch->root, ledger);

	int i;
	for (i = 0; prev->outgoing[i] != ledger; i++);
	if (i > 0 && borrowFromLeft(prev, i)) return 0;

	if (i > 0) mergeLeaves(prev->outgoing[i - 1], ledger);
	else mergeLeaves(ledger, prev->outgoing[i + 1]);

	return 0;
}

void deletenexts(Vault* v) {
	if (v == nullptr) return;
	if (!v->isledger) {
		for (int i = 0; i <= v->titlecount; i++)
			deletenexts(v->outgoing[i]);
		delete[] v->outgoing;
	}
	else delete[] v->movies;
	delete[] v->titles;
	delete v;
}
void deleteArchive(Archive* arch) {
	deletenexts(arch->root);
	delete arch;
}

void retitleMovie(Archive* arch, Movie* m, const char* newTitle) {
	if (!arch || !m || !newTitle) return;
	const char* oldTitle = m->title;
	addFlaggedTitle(m, oldTitle);
	archiveDelete(arch, oldTitle);
	m->title = newTitle;
	archiveInsert(arch, m);
}