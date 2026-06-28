#pragma once
#include <fstream>
#include <iostream>
#include <iomanip>
using namespace std;

struct Player {
	char name[50];
	int age;
	char role[20];
	float battingAvg, bowlingAvg, economy, recentForm;
	int fitnessLevel;
	float experienceScore;
	int matchesPlayed;
	float influenceScore, pressureIndex;
	int fatigueLevel;
	float adaptabilityRating, partnershipScore;
	bool selected;
	bool operator==(const Player& other) {
		if (name == other.name) return true;
		return false;
	}
};

int counting(const char* filename) {
	ifstream file(filename);
	if (!file) return -1;
	int count = 0;
	char* line = new char[500] {0};
	while (file.getline(line, 500)) count++;
	delete[]line;
	file.close();
	return count;
}

char* getstr(ifstream& mainfile, char check) {
	char* str = new char[200];
	mainfile.getline(str, 200, check);
	return str;
}

void copying(ifstream& filename, char* val, char check) {
	char* str = getstr(filename, check);
	int i = 0;
	for (; str[i] != '\0'; i++)
		val[i] = str[i];
	val[i] = '\0';
	delete[] str;
}
void copying(ifstream& filename, int& val) {
	char* str = getstr(filename, ',');
	int i = 0, sum = 0;
	for (; str[i] != '\0'; i++) {
		int x = str[i] - 48;
		sum *= 10; sum += x;
	}
	val = sum;
	delete[] str;
}
void copying(ifstream& filename, float& val, char check) {
	char* str = getstr(filename, check);
	int i = 0, j = 0; float sum = 0; bool flag = false;
	for (; str[i] != '\0'; i++) {
		if (str[i] == '.') {
			flag = true;
			j++;
			continue;
		}
		float x = str[i] - 48;
		if (flag) {
			for (int k = 0; k < j; k++)x /= 10;
			sum += x;
			j++;
		}
		else {
			sum *= 10; sum += x;
		}
	}
	val = sum;
	delete[] str;
}

void printPlayer(Player& squad, int i) {
	cout << right << setw(2) << i + 1 << left << ' ' << fixed << setprecision(1) << setw(18) << squad.name << setw(4) << squad.age
		<< setw(14) << squad.role << setw(8) << squad.battingAvg << setw(8) << squad.bowlingAvg << setw(6) << squad.economy
		<< setw(6) << squad.recentForm << setw(6) << squad.fitnessLevel << setw(6) << squad.experienceScore << setw(6) << squad.matchesPlayed
		<< setw(6) << squad.influenceScore << setw(6) << squad.pressureIndex << setw(6) << squad.fatigueLevel << setw(6) << squad.adaptabilityRating
		<< setw(7) << squad.partnershipScore << '\n';
}

int length(const char* str) {
	int len;
	for (len = 0; str[len] != '\0'; len++);
	return len;
}
bool compare(const char* str1, const char* str2) {
	if (length(str1) != length(str2)) return false;
	for (int i = 0; i < length(str1); i++)
		if (str1[i] != str2[i]) return false;
	return true;
}

bool compare(Player*& squad, int count, const char* name) {
	for (int i = 0; i < count; i++)
		if (compare(squad[i].name, name)) return true;
	return false;
}

void printPlayers(Player*& squad, int count) {
	if (count == 0) return;
	cout << left << " # " << setw(18) << "Name" << setw(4) << "Age" << setw(14) << "Role" << setw(8) << "BatAvg"
		<< setw(8) << "BowlAvg" << setw(6) << "Econ" << setw(6) << "Form" << setw(6) << "Fit"
		<< setw(6) << "Exp" << setw(6) << "Play" << setw(6) << "Infl" << setw(6) << "Pres"
		<< setw(6) << "Fat" << setw(6) << "Adapt" << setw(7) << "Part" << '\n';
	for (int i = 0; i < count; i++)
		printPlayer(squad[i], i);
}


int loadSquad(Player*& squad, const char* filename) {
	int count = counting(filename);
	if (count == -1) {
		cout << "Failed to Open File";
		return 0;
	}
	squad = new Player[count];
	ifstream mainfile(filename);
	if (!mainfile) {
		cout << "Failed to Open File";
		return 0;
	}
	for (int k = 0; k < count; k++) {
		copying(mainfile, squad[k].name, ',');
		copying(mainfile, squad[k].age);
		copying(mainfile, squad[k].role, ',');
		copying(mainfile, squad[k].battingAvg, ',');
		copying(mainfile, squad[k].bowlingAvg, ',');
		copying(mainfile, squad[k].economy, ',');
		copying(mainfile, squad[k].recentForm, ',');
		copying(mainfile, squad[k].fitnessLevel);
		copying(mainfile, squad[k].experienceScore, ',');
		copying(mainfile, squad[k].matchesPlayed);
		copying(mainfile, squad[k].influenceScore, ',');
		copying(mainfile, squad[k].pressureIndex, ',');
		copying(mainfile, squad[k].fatigueLevel);
		copying(mainfile, squad[k].adaptabilityRating, ',');
		copying(mainfile, squad[k].partnershipScore, '\n');
		squad[k].selected = false;
	}

	mainfile.close();
	return count;
}

bool validateSquad(Player squad[], int size) {
	if (size < 15) {
		cout << "\nError: size <15\n";
		return false;
	}
	for (int i = 0; i < size - 1; i++)
		if (squad[size - 1].name == squad[i].name) {
			cout << "\nError: Player already Selcted\n";
			return false;
		}
	int batcount = 0, bowlcount = 0, roundcount = 0, fatiguetotal = 0, pressureabove = 0;
	int inf = 0;
	for (int i = 0; i < size; i++) {
		if (compare(squad[i].role, "Batsman")) batcount++;
		if (compare(squad[i].role, "Bowler")) bowlcount++;
		if (compare(squad[i].role, "AllRounder")) roundcount++;
		fatiguetotal += squad[i].fatigueLevel;
		inf += squad[i].influenceScore;
		if (squad[i].pressureIndex > 7.5)pressureabove++;
	}
	fatiguetotal /= 15; inf /= 15;
	cout << inf << endl;
	if (batcount < 5) { cout << "Error -> batsman count < 5\n"; return false; }
	if (bowlcount < 3) {
		cout << "Error -> bowler count < 3\n";
		return false;
	}
	if (!roundcount) {
		cout << "Error -> need atleast 1 allrounder\n";
		return false;
	}
	if (pressureabove < 3) {
		cout << "Error -> players with pressure index above 7.5 < 3\n";
		return false;
	}
	if (fatiguetotal > 58) {
		cout << "Error -> average fatiguelevel > 58\n";
		return false;
	}
	return true;
}

class PlayerCard {
private:
	Player p;
	float compositeScore;
	float computeComposite() const {
		float c = p.battingAvg * 0.15;
		c += (50 - p.bowlingAvg) * 0.1;
		c += (10 - p.economy) * 0.05;
		c += p.recentForm * 2;
		c += p.fitnessLevel * 0.05;
		c += p.experienceScore * 1.5;
		c += p.influenceScore * 2;
		c += p.pressureIndex * 2;
		c += p.adaptabilityRating * 1.5;
		c += p.partnershipScore * 1.5;
		c -= p.fatigueLevel * 0.05;

		return c;
	}
public:
	PlayerCard(float val) { compositeScore = val; }
	PlayerCard(Player player) {
		p = player;
		compositeScore = computeComposite();
	}
	PlayerCard operator+(const PlayerCard& other) const {
		float c = (p.battingAvg + other.p.battingAvg) / 2;
		c *= (p.recentForm + other.p.recentForm) / 2;
		c += p.partnershipScore * other.p.partnershipScore;
		PlayerCard newc(c);
		return newc;
	}
	PlayerCard operator-(const PlayerCard& other) const {
		float c = (p.recentForm - other.p.recentForm) * 3;
		c += (p.pressureIndex - other.p.pressureIndex) * 2.5;
		c -= p.fatigueLevel * other.p.fatigueLevel;
		PlayerCard newc(c);
		return newc;
	}
	PlayerCard operator*(const PlayerCard& other) const {
		float c = 2500 / (p.bowlingAvg * other.p.bowlingAvg);
		c += (p.pressureIndex * other.p.pressureIndex) / 10;
		c -= float(p.fatigueLevel + other.p.fatigueLevel) / 25;
		PlayerCard newc(c);
		return newc;
	}
	PlayerCard operator/(const PlayerCard& other) const {
		float c = (p.battingAvg + other.p.battingAvg) / 2;
		c *= (p.recentForm + other.p.recentForm) / 2;
		c += p.partnershipScore * other.p.partnershipScore;
		PlayerCard newc(c);
		return newc;
	}

	bool operator>(const PlayerCard& other) const {
		if (compositeScore > other.compositeScore) return true;
		return false;
	}
	bool operator<(const PlayerCard& other) const {
		if (compositeScore < other.compositeScore) return true;
		return false;
	}
	bool operator==(const PlayerCard& other) const {
		if (compositeScore == other.compositeScore) return true;
		return false;
	}

	void display() const;
	float getComposite() const { return compositeScore; }
	Player getPlayer() const { return p; }
};


bool maxed(int num) {
	int maxtime = 90;
	if (num > maxtime) return true;
	return false;
}

struct SessionPlayer {
	Player p;
	int time;
	float priority;
	bool inXI;
	bool scheduled;
};

int getBaseTime(Player& p) {
	if (compare(p.role, "Batsman")) return 10;
	if (compare(p.role, "Bowler")) return 15;
	if (compare(p.role, "AllRounder")) return 12;
	if (compare(p.role, "WicketKeeper")) return 8;
	return 10;
}

int adjustForFatigue(int base, int fatigue) {
	if (fatigue > 80) return 0;
	if (fatigue > 65) {
		float factor = 1.0f - (fatigue - 65) / 50.0f;
		if (factor < 0.3f) factor = 0.3f;
		return (int)(base * factor);
	}
	return base;
}

bool isInXI(Player& p, Player* xi, int xiSize) {
	for (int i = 0; i < xiSize; i++)
		if (compare(p.name, xi[i].name)) return true;
	return false;
}

float computePriority(Player& p, Player* xi, int xiSize) {
	float score = 0;
	if (p.recentForm < 5) score += (5 - p.recentForm) * 3;
	if (isInXI(p, xi, xiSize)) score += 10;
	if (p.fitnessLevel < 60) score += 5;
	if (p.pressureIndex < 5) score += (5 - p.pressureIndex) * 2;
	if (p.adaptabilityRating < 5) score += (5 - p.adaptabilityRating) * 2;
	score -= p.fatigueLevel * 0.1f;
	return score;
}

struct ScheduledPlayer {
	Player p;
	int time;
	bool mentorshipBonus;
};

struct ExcludedPlayer {
	Player p;
	char* reason=new char[100];
};

int getlength(const char* str) {
	if (str == nullptr) return 0;
	int i = 0;
	for (; str[i] != '\0'; i++);
	return i;
}
char* deepcopy(const char* str) {
	if (str == nullptr) return nullptr;
	char* newstr = new char[getlength(str) + 1];
	int i = 0;
	for (; str[i] != '\0'; i++)
		newstr[i] = str[i];
	newstr[i] = '\0';
	return newstr;
}

struct Temp {
	Player p;
	int time;
	float priority;
	bool inXI;
	bool selected;
};

bool isInXI(Player& p, Player* xi) {
	for (int i = 0; i < 11; i++)
		if (compare(p.name, xi[i].name)) return true;
	return false;
};
int getBaseTime(Player& p) {
	if (compare(p.role, "Batsman")) return 10;
	if (compare(p.role, "Bowler")) return 15;
	if (compare(p.role, "AllRounder")) return 12;
	if (compare(p.role, "WicketKeeper")) return 8;
	return 10;
	};
int adjust(int base, int fatigue) {
	if (fatigue > 80) return 0;
	if (fatigue > 65) {
		float f = 1.0f - (fatigue - 65) / 50.0f;
		if (f < 0.3f) f = 0.3f;
		return (int)(base * f);
	}
	return base;
	};
float priority(Player& p, Player* xi) {
	float s = 0;

	if (p.recentForm < 5) s += (5 - p.recentForm) * 3;
	if (isInXI(p,xi)) s += 10;
	if (p.fitnessLevel < 60) s += 5;
	if (p.pressureIndex < 5) s += (5 - p.pressureIndex) * 2;
	if (p.adaptabilityRating < 5) s += (5 - p.adaptabilityRating) * 2;

	s -= p.fatigueLevel * 0.1f;
	return s;
	};

void scheduleTraining(Player* squad, int squadSize, Player* xi, int xiSize, ScheduledPlayer*& scheduled, int& scheduledCount, ExcludedPlayer*& excluded, int& excludedCount) {

	Temp* list = new Temp[squadSize];
	int validCount = 0;


	for (int i = 0; i < squadSize; i++) {

		if (squad[i].fatigueLevel > 80) {
			continue;
		}

		int base = getBaseTime(squad[i]);
		int t = adjust(base, squad[i].fatigueLevel);

		if (t <= 0) continue;

		list[validCount].p = squad[i];
		list[validCount].time = t;
		list[validCount].priority = priority(squad[i], xi);
		list[validCount].inXI = isInXI(squad[i], xi);
		list[validCount].selected = false;

		validCount++;
	}
	for (int i = 0; i < validCount - 1; i++)
		for (int j = 0; j < validCount - i - 1; j++) {

			bool swap = false;

			if (list[j].p.fatigueLevel > 65 && list[j + 1].p.fatigueLevel <= 65)
				swap = true;
			else if (list[j].p.fatigueLevel <= 65 && list[j + 1].p.fatigueLevel > 65)
				swap = false;
			else if (list[j].priority < list[j + 1].priority)
				swap = true;

			if (swap) {
				Temp temp = list[j];
				list[j] = list[j + 1];
				list[j + 1] = temp;
			}
	}
	int total = 0, xiTime = 0;
	for (int i = 0; i < validCount; i++) {
		if (total + list[i].time > 90) continue;

		list[i].selected = true;
		total += list[i].time;

		if (list[i].inXI)
			xiTime += list[i].time;
	}
	delete[] list;
	scheduled = new ScheduledPlayer[validCount];
	excluded = new ExcludedPlayer[squadSize];

	scheduledCount = 0;
	excludedCount = 0;

	Player prev;
	bool hasPrev = false;

	for (int i = 0; i < validCount; i++) {

		if (!list[i].selected) continue;

		scheduled[scheduledCount].p = list[i].p;
		scheduled[scheduledCount].time = list[i].time;
		scheduled[scheduledCount].mentorshipBonus = false;

		if (hasPrev && prev.influenceScore > 8 && list[i].p.influenceScore < 6)
			scheduled[scheduledCount].mentorshipBonus = true;

		prev = list[i].p;
		hasPrev = true;

		scheduledCount++;
	}

	for (int i = 0; i < squadSize; i++) {

		bool found = false;

		for (int j = 0; j < validCount; j++) {
			if (compare(squad[i].name, list[j].p.name)) {
				found = true;

				if (!list[j].selected) {
					excluded[excludedCount].p = squad[i];
					excluded[excludedCount].reason = deepcopy("Time limit");
					excludedCount++;
				}
			}
		}

		if (!found && squad[i].fatigueLevel > 80) {
			excluded[excludedCount].p = squad[i];
			excluded[excludedCount].reason = deepcopy("Fatigue > 80");
			excludedCount++;
		}
	}


}