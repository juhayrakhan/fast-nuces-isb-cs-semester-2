#pragma once
#include<iostream>
#include <fstream>
using namespace std;

class Entity {
protected:
	int id; char* name;
	int getlength(const char* str) {
		int i = 0;
		while (str[i] != '\0') i++;
		return i;
	}
public:
	Entity(int id, const char* name) : id(id) {
		int len = getlength(name);
		this->name = new char[len + 1];
		for (int i = 0; i <= len; i++)
			this->name[i] = name[i];
	}
	Entity(){}
	int getId() { return id; }
	char* getName() { return name; }
};
class LandedLord;
class Person : public Entity {
protected:
	int age; bool isAlive;
public:
	Person(int id=0, const char* name=" ", int age=0, bool isAlive=true) :
		Entity(id, name), age(age), isAlive(isAlive) {
	}

	int getAge() { return age; }
	
};
class Asset : public Entity {
protected:
	int  maxHp, armor; double maintenanceCost;
public:
	int hp;
	Asset(int id, const char* name, int hp, int maxHp, int armor, double maintenanceCost) :
		Entity(id, name), hp(hp), maxHp(maxHp), armor(armor), maintenanceCost(maintenanceCost) {
	}
	Asset() {}
	int getHP() {
		return hp;
	}
};

class MilitaryUnit : public Asset {
protected:
	int attackPower;
public:
	MilitaryUnit(int id, const char* name, int hp, int maxHp, int armor, double maintenanceCost, int attackPower) :
		Asset(id, name, hp, maxHp, armor, maintenanceCost), attackPower(attackPower) {
	}
	double calDamage(LandedLord* l);
	
};
class MeleeUnit : public MilitaryUnit {
protected:
	bool isOnFrontline;
public:
	MeleeUnit(int id, const char* name, int hp, int maxHp, int armor, double maintenanceCost, int attackPower, bool isOnFrontline) :
		MilitaryUnit(id, name, hp, maxHp, armor, maintenanceCost, attackPower), isOnFrontline(isOnFrontline) {
	}
};
class MountedUnit : public MilitaryUnit {
protected:
	int chargeMultipler, terrainPenality;
public:
	MountedUnit(int id, const char* name, int hp, int maxHp, int armor, double maintenanceCost, int attackPower, int chargeMultipler, int terrainPenality) :
		MilitaryUnit(id, name, hp, maxHp, armor, maintenanceCost, attackPower), chargeMultipler(chargeMultipler), terrainPenality(terrainPenality) {
	}
};
class Cavalry : public MountedUnit {
public:
	
	Cavalry(int id, const char* name, int hp, int hpmax, int arm, double maintaence, int attack, int charge, int terrainpen) : MountedUnit(id, name, hp, hpmax, arm, maintaence, attack, charge, terrainpen) {};
	void reduceHP(double d) {
		hp -= d;
	}
};
class FootSoldier : public MeleeUnit {
public:
	FootSoldier(int id, const char* name, int hp, int hpmax, int arm, double maint, int attack, bool f) : MeleeUnit(id, name, hp, hpmax, arm, maint, attack, f) {};
	void reduceHP(double d) {
		hp -= d;
	}
};
class Knight : public MeleeUnit {
protected:
	bool shieldBlock;
public:
	Knight(int id, const char* name, int hp, int hpmax, int arm, double maint, int attack, bool f, bool b) : MeleeUnit(id, name, hp, hpmax, arm, maint, attack, f) {};
	void reduceHP(double d) {
		hp -= d;
	}
};
class SeigeEngine : virtual public Asset {
protected:
	int seigeDamege;
	float speed;
public:
	SeigeEngine(const char* name, int id, int hp, int hpmax, int arm, double maint, int sieg, float speed) : Asset(id, name, hp, hpmax, arm, maint), seigeDamege(sieg), speed(speed) {};
};

class BatteringRam : public SeigeEngine {
public:
	BatteringRam(const char* name, int id, int hp, int hpmax, int arm, double maint, int seig, float speed): SeigeEngine(name, id, hp, hpmax, arm, maint, seig, speed) {}
};
class Catapult : public SeigeEngine {
	int aoeDamage, aoeTargets;
public:
	Catapult(const char* name, int id, int hp, int hpmax, int arm, double maint, int seig, float speed, int aoedam,int aoetar): SeigeEngine(name, id, hp, hpmax, arm, maint, seig, speed){}
};
class Transport : virtual public Asset {
	int troopCapacity;
	double knotsSpeed;
public:
	Transport(const char* name, int id, int hp, int hpmax, int arm, double maint, int troop, double knot) : Asset(id, name, hp, hpmax, arm, maint), troopCapacity(troop), knotsSpeed(knot) {};

};
class Noble {
public:
	int real;
	int ambitionStat, diplomacystat;
	double healthMeter;
	int fatherId, childCount;
	int* childrenId;
	Noble() {
		healthMeter = 0.0;
	}
	int getAmbitionStat() {
		return ambitionStat;
	}
};
class LandedLord : public Noble, public Person{
public:
	int strategyStat;
	int modifier;
	int AgeModifier() {
		if (age < 40)
			return 5;
		else
			return -5;
	}
	LandedLord(const char* name, int id, int age, bool al) : Person(id, name, age, al), Noble() {};
	int getStrategyStat() const { return strategyStat; }
};
double MilitaryUnit::calDamage(LandedLord* l) {
	double effectDam = attackPower * (1.0 + l->strategyStat / 100.0);
	return effectDam;
}

class LandlessLord : public Noble{
protected:
	double assasinationPlotProgress;
public:
	LandlessLord() : Noble() {};
	int getassasinationPlot() {
		return assasinationPlotProgress;
	}
	void setAssasination(int n) {
		assasinationPlotProgress += n;
	}
};
class Sage : public Person {
protected:
	double HealingPower, wisdom;
public:
	Sage(const char* name, int id, int age, bool al) : Person(id, name, age, al) {};
	double getHealing() { return HealingPower; }
	double getWisdom() { return wisdom; }
};
class Assassin : public Person {
protected:
	double stealthStat;
	int clientIDx;
public:
	Assassin(const char* name, int id, int age, bool al) : Person(id, name, age, al) {};
	Assassin() : Person() {}
	int getStealthStat() { return stealthStat; }
	int getClient() { return clientIDx; }
	void setClient(int val) { clientIDx = val; }
	void setStealth(int val) { stealthStat = val; }
};
class WarriorLord : public Noble, public Knight {
protected:
public:
	WarriorLord(const char* name, int id, int hp, int hpmax, int arm, double maint, int attack, bool b, bool s) : Knight(id, name, hp, hpmax, arm, maint, attack, b, s) {};
};
class Warship : public Transport, public SeigeEngine {
public:
	
	void takeHullDamage() {

	}
	void takeWeaponDamage() {

	}
};
class Kingdom {				//kingdom
public:
	float tax;
	double wealth,defenceStat;
	LandedLord* currentLord;
	int* heirsIds;
	LandlessLord** local;
	int localcount;
	Knight** kng;
	int kngCount;
	FootSoldier** fs;
	Cavalry** cav;
	BatteringRam** bat;
	Catapult** cat;
	Warship** war;
	Sage** ledgers;
	WarriorLord** warlor;
	int warlorcount;
	int fsCount;
	int cavCount;
	int sageCount;
	int assassinCount;
	int batcount;
	int catcount;


public:
	void performAction() {

	}
	double cavDamage() {
		int i = 0; double damg=0;
		while (this->cav[i] != nullptr) {
			if (cav[i]->getHP() > 0)
				damg += cav[i]->calDamage(currentLord);
			i++;
		}
		return damg;
	}

};
class FrostPeaksKingdom : public Kingdom {
protected:
 	int coldAttritionModifier;
	MilitaryUnit** arr;
	int num_arr ;
	FrostPeaksKingdom() {
		coldAttritionModifier = 5;
		arr = nullptr;
		num_arr = 0;
	}
public: 
	void applyColdWeather() {
		for (int i = 0; i < num_arr; i++) {
			if (arr[i])
				arr[i]->hp -= coldAttritionModifier;
		}
	}
	void performAction() {
		applyColdWeather();
		Kingdom::performAction();

	}
	
};
class ThePeaksOfFrost : public FrostPeaksKingdom {
public:
	double civilWartension;
	ThePeaksOfFrost(double c = 0) {
		civilWartension = c;
		if (civilWartension > 100)
			civilWartension = 100.0;

		if (civilWartension < 0)
			civilWartension = 0.0;
	}
};
 //Verdant kigdom
class VerdantKingdom : public Kingdom {
public:
	int foodSurplusBonus;
public:
	void applyFoodBonus() {
		wealth += foodSurplusBonus;
	}
	void performAction() {
		applyFoodBonus();
		Kingdom::performAction();
	}
};
class TheSkyCitadel : public VerdantKingdom {
public:
public:
	double civilWartension;
	TheSkyCitadel(double c = 0) {
		civilWartension = c;
		if (civilWartension > 100)
			civilWartension = 100.0;

		if (civilWartension < 0)
			civilWartension = 0.0;
	}
};
class TheVerdantExpanse : public VerdantKingdom {
public:
	double civilWartension;
	TheVerdantExpanse(double c = 0) {
		civilWartension = c;
		if (civilWartension > 100)
			civilWartension = 100.0;

		if (civilWartension < 0)
			civilWartension = 0.0;
	}
};
class TheCrimsonSands : public VerdantKingdom {
public:
	double civilWartension;
	TheCrimsonSands(double c = 0) {
		civilWartension = c;
		if (civilWartension > 100)
			civilWartension = 100.0;

		if (civilWartension < 0)
			civilWartension = 0.0;
	}
};
class TheAzureRiverlands : public VerdantKingdom {
public:
	double civilWartension;
	TheAzureRiverlands(double c = 0) {
		civilWartension = c;
		if (civilWartension > 100)
			civilWartension = 100.0;

		if (civilWartension < 0)
			civilWartension = 0.0;
	}
};
class CoastalKingdom : public Kingdom {
public:
protected:
	double navalTradeBonus, baseTaxIncome, shipCount;
public:
	double applyTradeBonus() {
		return navalTradeBonus * baseTaxIncome * shipCount / 100;
	}
	void performAction() {
		wealth+=applyTradeBonus();
		Kingdom::performAction();
	}
};
class TheSunlandEmpire : public CoastalKingdom {
public:
	double civilWartension;
	TheSunlandEmpire(double c = 0) {
		civilWartension = c;
		if (civilWartension > 100)
			civilWartension = 100.0;

		if (civilWartension < 0)
			civilWartension = 0.0;
	}
};
class TheCorsairIsles : public CoastalKingdom {
public:
	double civilWartension;
	TheCorsairIsles(double c = 0) {
		civilWartension = c;
		if (civilWartension > 100)
			civilWartension = 100.0;

		if (civilWartension < 0)
			civilWartension = 0.0;
	}
};
class TheTempestCoast : public CoastalKingdom {
public:
	double civilWartension;
	TheTempestCoast(double c = 0) {
		civilWartension = c;
		if (civilWartension > 100)
			civilWartension = 100.0;

		if (civilWartension < 0)
			civilWartension = 0.0;
	}
};
class TheImperialSeat : public CoastalKingdom {
public:
	double civilWartension;
	TheImperialSeat(double c = 0) {
		civilWartension = c;
		if (civilWartension > 100)
			civilWartension = 100.0;

		if (civilWartension < 0)
			civilWartension = 0.0;
	}
};
class TheVoidRift : public Kingdom {
protected:
	double threatLevel;
public:
	void performAction() {
		threatLevel += 1;
	}
};
class rebelfraction {};
class AethelGardEngine{
public:
	Kingdom* k1;
	Kingdom* k2;
	LandedLord* currentLord;
	MilitaryUnit* mil;
	LandlessLord** local;
	Knight** kng;
	FootSoldier** fs;
	Cavalry** cav;
	int cavCount;
	BatteringRam** bat;
	Catapult** cat;
	Warship** war;
	Sage** ledgers;
	WarriorLord** warlor;
	Assassin** asas;
	ThePeaksOfFrost** peak;
	TheSkyCitadel** sky;
	TheVerdantExpanse** ver;
	TheCrimsonSands** crim;
	TheAzureRiverlands** azur;
	TheSunlandEmpire** sun;
	TheCorsairIsles** corsair;
	TheTempestCoast** tempest;
	TheImperialSeat** impr;
	TheVoidRift** rift;
	rebelfraction** rebel;
	int realmcurlordcount;
	int threatMax;
	int assassinCount;
	Kingdom* realms[10];
	void declare() {
		realms[0] = new ThePeaksOfFrost();
		realms[1] = *sky;
		realms[2] = *ver;
		realms[3] = *crim;
		realms[4] = *azur;
		realms[5] = *sun;
		realms[6]= * corsair;
		realms[7] = *tempest;
		realms[8] = *impr;
		realms[9] = *rift;
	}
	AethelGardEngine(Kingdom* k1, Kingdom* k2) {
		this->k1 = k1;
		this->k2 = k2;
	}
	int relations[10][10];
	void relation() {
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				relations[i][j] = 100;
			}
		}
	}
	void HourlyWar(Kingdom* k1, Kingdom* k2) {
		double cavdamage1 = 0, cavdamage2 = 0;
		for (int i = 0; i < k1->cavCount; i++) {
			if (k1->cav[i]->getHP() > 0)
				cavdamage1 += k1->cav[i]->calDamage(k1->currentLord);
		}
		for (int i = 0; i < k2->cavCount; i++) {
			if (k2->cav[i]->getHP() > 0)
				cavdamage2 += k2->cav[i]->calDamage(k2->currentLord);
		}
		int i; i = 0;
		while (k2->kng[i] != nullptr) {
			k2->kng[i]->reduceHP(cavdamage1);
			i++;
		}
		i = 0;
		while (k2->fs[i] != nullptr) {
			k2->fs[i]->reduceHP(cavdamage1);
			i++;
		}
		i = 0;
		while (k1->kng[i] != nullptr) {
			k1->kng[i]->reduceHP(cavdamage2);
			i++;
		}
		i = 0;
		while (k1->fs[i] != nullptr) {
			k1->fs[i]->reduceHP(cavdamage2);
			i++;
		}
		/* now splitting for next knoghts and footsoldiers and applying damage to them*/
		double knDam1 = 0, knDam2 = 0;
		while (k1->kng[i] != nullptr) {
			if (k1->kng[i]->getHP() > 0)
				knDam1 += k1->kng[i]->calDamage(k1->currentLord);
			i++;

		}
		i = 0;
		while (k2->kng[i] != nullptr) {
			if (k2->kng[i]->getHP() > 0)
				knDam2 += k2->kng[i]->calDamage(k2->currentLord);
			i++;
		}
		i = 0;
		while (k2->fs[i] != nullptr) {
			k2->fs[i]->reduceHP(knDam1);
			i++;
		}
		i = 0;
		while (k1->fs[i] != nullptr) {
			k1->fs[i]->reduceHP(knDam2);
			i++;
		}
	}
		//2.4.2
	void civilWar(ThePeaksOfFrost* peak,
		TheSkyCitadel* sky,
		TheVerdantExpanse* ver,
		TheCrimsonSands* crim,
		TheAzureRiverlands* azur,
		TheSunlandEmpire* sun,
		TheCorsairIsles* corsair,
		TheTempestCoast* tempest,
		TheImperialSeat* impr,
		TheVoidRift* rift) {
		if (sky->currentLord == nullptr) {
			sky->civilWartension += 30.0;
		}
		if (sky->wealth < 0) {
			sky->civilWartension += 5.0;
		}
		int i = 0;
		while (sky->local[i] != nullptr) {
			if (sky->local[i]->ambitionStat > 50) {
				sky->civilWartension += (sky->local[i]->ambitionStat) / 10.0;
			}
			if (sky->currentLord->strategyStat > 70) {
				sky->civilWartension -= 3.0;
			}
			if (sky->civilWartension > 100.0)
				sky->civilWartension = 100.0;
			if (sky->civilWartension < 0.0)
				sky->civilWartension = 0.0;
			i++;
		}

		if (peak->currentLord == nullptr) {
			peak->civilWartension += 30.0;
		}
		if (peak->wealth < 0) {
			peak->civilWartension += 5.0;
		}
		i = 0;
		while (peak->local[i] != nullptr) {
			if (peak->local[i]->ambitionStat > 50) {
				peak->civilWartension += (peak->local[i]->ambitionStat) / 10.0;
			}
		}
		if (peak->currentLord->strategyStat > 70) {
			peak->civilWartension -= 3.0;
		}

		if (ver->currentLord == nullptr) {
			ver->civilWartension += 30.0;
		}
		if (ver->wealth < 0) {
			ver->civilWartension += 5.0;
		}
		i = 0; while (ver->local[i] != nullptr) {
			if (ver->local[i]->ambitionStat > 50) {
				ver->civilWartension += (ver->local[i]->ambitionStat) / 10.0;
			}
		}
		if (ver->currentLord->strategyStat > 70) {
			ver->civilWartension -= 3.0;
		}

		if (crim->currentLord == nullptr) {
			crim->civilWartension += 30.0;
		}
		if (crim->wealth < 0) {
			crim->civilWartension += 5.0;
		}
		 i = 0; while (crim->local[i] != nullptr) {
			if (crim->local[i]->ambitionStat > 50) {
				crim->civilWartension += (crim->local[i]->ambitionStat) / 10.0;
			}
		}
		if (crim->currentLord->strategyStat > 70) {
			crim->civilWartension -= 3.0;
		}
		if (crim->civilWartension > 100.0)
			crim->civilWartension = 100.0;
		if (crim->civilWartension < 0.0)
			crim->civilWartension = 0.0;

		// just copy all other realsms
		if (azur->currentLord == nullptr) {
			azur->civilWartension += 30.0;
		}
		if (azur->wealth < 0) {
			azur->civilWartension += 5.0;
		}
		 i = 0;
		while (azur->local[i] != nullptr) {
			if (azur->local[i]->ambitionStat > 50) {
				azur->civilWartension += (azur->local[i]->ambitionStat) / 10.0;
				i++;
			}


			if (azur->currentLord->strategyStat > 70) {
				azur->civilWartension -= 3.0;
			}

			if (azur->civilWartension > 100.0)
				azur->civilWartension = 100.0;
			if (azur->civilWartension < 0.0)
				azur->civilWartension = 0.0;
		}

		if (sun->currentLord == nullptr) {
			sun->civilWartension += 30.0;
		}
		if (sun->wealth < 0) {
			sun->civilWartension += 5.0;
		}
		 i = 0;
		while (sun->local[i] != nullptr) {
			if (sun->local[i]->ambitionStat > 50) {
				sun->civilWartension += (sun->local[i]->ambitionStat) / 10.0;
				i++;
			}


			if (sun->currentLord->strategyStat > 70) {
				sun->civilWartension -= 3.0;
			}

			if (sun->civilWartension > 100.0)
				sun->civilWartension = 100.0;
			if (sun->civilWartension < 0.0)
				sun->civilWartension = 0.0;
			//next
			if (corsair->currentLord == nullptr) {
				corsair->civilWartension += 30.0;
			}
			if (corsair->wealth < 0) {
				corsair->civilWartension += 5.0;
			}
			i = 0;
			while (corsair->local[i] != nullptr) {
				if (corsair->local[i]->ambitionStat > 50) {
					corsair->civilWartension += (corsair->local[i]->ambitionStat) / 10.0;
					i++;
				}
			}


				if (corsair->currentLord->strategyStat > 70) {
					corsair->civilWartension -= 3.0;
				}

				if (corsair->civilWartension > 100.0)
					corsair->civilWartension = 100.0;
				if (corsair->civilWartension < 0.0)
					corsair->civilWartension = 0.0;
				//next
				if (tempest->currentLord == nullptr) {
					tempest->civilWartension += 30.0;
				}
				if (tempest->wealth < 0) {
					tempest->civilWartension += 5.0;
				}
				i = 0;
				while (tempest->local[i] != nullptr) {
					if (tempest->local[i]->ambitionStat > 50) {
						tempest->civilWartension += (tempest->local[i]->ambitionStat) / 10.0;
						i++;
					}
				}


				if (tempest->currentLord->strategyStat > 70) {
					tempest->civilWartension -= 3.0;
				}

				if (tempest->civilWartension > 100.0)
				tempest->civilWartension = 100.0;
				if (tempest->civilWartension < 0.0)
					tempest->civilWartension = 0.0;
				//impr
				if (impr->currentLord == nullptr) {
					impr->civilWartension += 30.0;
				}
				if (impr->wealth < 0) {
					impr->civilWartension += 5.0;
				}
				i = 0;
				while (impr->local[i] != nullptr) {
					if (impr->local[i]->ambitionStat > 50) {
						impr->civilWartension += (impr->local[i]->ambitionStat) / 10.0;
						i++;
					}
				}


				if (impr->currentLord->strategyStat > 70) {
					impr->civilWartension -= 3.0;
				}

				if (impr->civilWartension > 100.0)
					impr->civilWartension = 100.0;
				if (impr->civilWartension < 0.0)
					impr->civilWartension = 0.0;
				
		}
	}
	
	
	float *ticks;
	void InternalCoup() {
		int i = 0;
		while (local[i] != nullptr) {
			if (local[i]->ambitionStat > 60)
				ticks[i] = ((local[i]->ambitionStat) - 60) / 5.0;
			i++;
		}
		if (currentLord->strategyStat > 50) {
			currentLord->strategyStat -= currentLord->strategyStat / 25.0;
		}
		for (int j = 0; j < 10; j++) {
			for(int k=0; k<10; k++){
				if (relations[j][k] <= 0) {
					ticks += 2;
					break;
				}
			}
		}
	}
	void checkRelations() {
		bool check = false;
		for (int i = 0; i < 10; i++) {
			check = false;
			for (int j = 0; j < 10; j++) {
				if (relations[i][j] <= 0) {
					check = true; break;
				}
			}
			if (check == true&& local[i]!=nullptr)
				local[i]->setAssasination(2);
		}
		
	}
	void checkdefence() {
		if (k1->defenceStat > 50)
			k1->defenceStat -= 1;
		if (k2->defenceStat > 50)
			k2->defenceStat -= 1;
	}
	double assasinPower;
	double targetDefense;
	void checkStrike() {
		for (int i = 0; i < 10; i++) {
			int j = 0;
			while (realms[i]->local[i] != nullptr) {
				if (peak[i]->civilWartension >= 100.0) {
					assasinPower = currentLord->ambitionStat + (peak[i]->wealth / 100);
					targetDefense = currentLord->strategyStat + (peak[i]->defenceStat / 10) + currentLord->AgeModifier();

				}
				if (assasinPower > targetDefense)
					realms[i]->currentLord = nullptr;
				else {
					realms[i]->local[j] = nullptr;
					for (int k = 0; k < 30; k++)
						relations[i][k] -= 30;
				}
			}
			j++;
		}
	}
	
	void assasin_activate(int realmid) {
		Assassin a;
		for (int i = 0; i < 10; i++) {
			if (peak[i]->civilWartension >= 100 && relations[0][i] <= -50)
				Assassin a;
			if (a.getStealthStat() > currentLord->strategyStat)
				currentLord = nullptr;
			else relations[i][0] -= 20;
		}
	}
};

bool compare(const char* a, const char* b) {
	int i = 0;
	while (a[i] != '\0' && b[i] != '\0') {
		if (a[i] != b[i]) return false;
		i++;
	}
	return a[i] == '\0' && b[i] == '\0';
}
bool startingsame(const char* line, const char* key) {
	int i = 0;
	while (key[i] != '\0') {
		if (line[i] != key[i]) return false;
		i++;
	}
	return true;
}
int integer(const char* str) {
	int num = 0, i = 0, sign = 1;

	if (str[0] == '-') {
		sign = -1;
		i++;
	}

	while (str[i] != '\0') {
		num = num * 10 + (str[i] - '0');
		i++;
	}
	return num * sign;
}
void getpart(const char* line, int& index, char* out) {
	int j = 0;

	while (line[index] == ' ' || line[index] == ',') index++;

	while (line[index] != ',' && line[index] != '\0' && line[index] != '\n') {
		out[j++] = line[index++];
	}

	out[j] = '\0';

	if (line[index] == ','|| line[index] == '\n') index++;
}


void printKingdoms(AethelGardEngine& engine) {
	cout << "\n=== KINGDOMS ===\n";
	for (int i = 0; i < 10; i++) {
		if (engine.realms[i] != nullptr) {
			cout << "Realm " << i
				<< " | Wealth: " << engine.realms[i]->wealth
				<< " | Tax: " << engine.realms[i]->tax
				<< " | Defense: " << engine.realms[i]->defenceStat;

			if (engine.realms[i]->currentLord != nullptr)
				cout << " | Lord exists";
			else
				cout << " | No Lord";

			cout << endl;
		}
	}
}
void printRelations(AethelGardEngine& engine) {
	cout << "\n=== RELATIONS MATRIX ===\n";
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			cout << engine.relations[i][j] << " ";
		}
		cout << endl;
	}
}
void printLords(AethelGardEngine& engine) {

	cout << "\n===== LANDED LORDS =====\n";

	for (int i = 0; i < 10; i++) {
		cout << "ID: " << engine.realms[i]->currentLord->getId() << " ";
		cout << "Age: " << engine.realms[i]->currentLord->getAge() << " ";
		cout << "Strategy: " << engine.realms[i]->currentLord->getStrategyStat() << "\n";
	}

	cout << "\n===== LANDLESS LORDS =====\n";
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < engine.realms[i]->localcount; i++) {
			cout << "Ambition: " << engine.realms[i]->local[j]->getAmbitionStat() << " ";
			cout << "Plot: " << engine.realms[i]->local[j]->getassasinationPlot() << "\n";
		}
		cout << " \n";
	}

	cout << "\n===== WARRIOR LORDS =====\n";
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < engine.realmcurlordcount; i++) {
			cout << "Ambition: " << engine.realms[i]->warlor[j]->getAmbitionStat() << " ";
			cout << "ID: " << engine.realms[i]->warlor[j]->getId() << "\n";
			cout << "Name: " << engine.realms[i]->warlor[j]->getName() << "\n";
		}
		cout << " \n";
	}
}
void printSages(Sage** sages, int count) {
	cout << "\n===== SAGES =====\n";

	for (int i = 0; i < count; i++) {
		if (sages[i]) {
			cout << "Sage #" << i << " ";
			cout << "Healing: " << sages[i]->getHealing() << " ";
			cout << "Wisdom: " << sages[i]->getWisdom() << "\n";
		}
	}
}
void printAssassins(AethelGardEngine& engine) {
	cout << "\n===== ASSASSINS =====\n";

	for (int i = 0; i < engine.assassinCount; i++) {
		if (engine.asas[i]) {
			cout << "Assassin #" << i << " ";
			cout << "Stealth: " << engine.asas[i]->getStealthStat() << "\n";
		}
	}
}

void add(Knight**& arr, int& count, Knight* k) {
	Knight** temp = new Knight * [count + 1];

	for (int i = 0; i < count; i++)
		temp[i] = arr[i];

	temp[count] = k;

	delete[] arr;
	arr = temp;
	count++;
}
void add(Cavalry**& arr, int& count, Cavalry* c) {
	Cavalry** temp = new Cavalry * [count + 1];

	for (int i = 0; i < count; i++)
		temp[i] = arr[i];

	temp[count] = c;

	delete[] arr;
	arr = temp;
	count++;
}
void add(FootSoldier**& arr, int& count, FootSoldier* fs) {
	FootSoldier** temp = new FootSoldier * [count + 1];

	for (int i = 0; i < count; i++)
		temp[i] = arr[i];

	temp[count] = fs;

	delete[] arr;
	arr = temp;
	count++;
}
void add(BatteringRam**& arr, int& count, BatteringRam* br) {
	BatteringRam** temp = new BatteringRam * [count + 1];

	for (int i = 0; i < count; i++)
		temp[i] = arr[i];

	temp[count] = br;

	delete[] arr;
	arr = temp;
	count++;
}
void add(Catapult**& arr, int& count, Catapult* cat) {
	Catapult** temp = new Catapult * [count + 1];

	for (int i = 0; i < count; i++)
		temp[i] = arr[i];

	temp[count] = cat;

	delete[] arr;
	arr = temp;
	count++;
}
void add(Sage**& arr, int& count, Sage* s) {
	Sage** temp = new Sage * [count + 1];
	for (int i = 0; i < count; i++) temp[i] = arr[i];
	delete[] arr;
	arr = temp;
	arr[count] = s;
	count++;
}
void add(Assassin**& arr, int& count, Assassin* a) {
	Assassin** temp = new Assassin * [count + 1];
	for (int i = 0; i < count; i++)	temp[i] = arr[i];
	delete[] arr;
	arr = temp;
	arr[count] = a;
	count++;
}

void loadKingdoms(ifstream& file, AethelGardEngine& engine) {
	char line[256];

	file.getline(line, 256);

	while (file.getline(line, 256)) {

		if (line[0] == ',') break;
		if (line[0] == '\0') continue;

		int idx = 0;
		char token[100];

		getpart(line, idx, token);
		int id = integer(token);
		getpart(line, idx, token);
		char name[100];
		int k = 0;
		while (token[k] != '\0') {
			name[k] = token[k];
			k++;
		}
		name[k] = '\0';

		getpart(line, idx, token);
		int wealth = integer(token);
		getpart(line, idx, token);
		int tax = integer(token);
		getpart(line, idx, token);
		int defense = integer(token);
		getpart(line, idx, token);
		if (compare(name, "The Sunland Empire"))
			engine.realms[id] = new TheSunlandEmpire();
		else if (compare(name, "The Peaks of Frost"))
			engine.realms[id] = new ThePeaksOfFrost();
		else if (compare(name, "The Sky Citadel"))
			engine.realms[id] = new TheSkyCitadel();
		else if (compare(name, "The Corsair Isles"))
			engine.realms[id] = new TheCorsairIsles();
		else if (compare(name, "The Verdant Expanse"))
			engine.realms[id] = new TheVerdantExpanse();
		else if (compare(name, "The Crimson Sands"))
			engine.realms[id] = new TheCrimsonSands();
		else if (compare(name, "The Tempest Coast"))
			engine.realms[id] = new TheTempestCoast();
		else if (compare(name, "The Azure Riverlands"))
			engine.realms[id] = new TheAzureRiverlands();
		else if (compare(name, "The Imperial Seat"))
			engine.realms[id] = new TheImperialSeat();
		else if (compare(name, "The Void Rift"))
			engine.realms[id] = new TheVoidRift();

		engine.realms[id]->wealth = wealth;
		engine.realms[id]->tax = tax;
		engine.realms[id]->defenceStat = defense;
	}
}
void loadRelations(ifstream& fin, AethelGardEngine& engine) {
	char line[256];
	fin.getline(line, 256);


	for (int i = 0; i < 11; i++) {
		fin.getline(line, 256);
		if (line[0] == ',' || line[0] == '\n') break;
		int j = 0;
		int k = 0;

		while (line[k] != '\0' && line[k] != '\n' && j < 10) {

			while (line[k] == ' ' || line[k] == ',') k++;

			int sign = 1;
			if (line[k] == '-') {
				sign = -1;
				k++;
			}

			int num = 0;
			bool found = false;

			while (line[k] >= '0' && line[k] <= '9') {
				num = num * 10 + (line[k] - '0');
				k++;
				found = true;
			}
			if (found) {
				engine.relations[i][j] = num * sign;
				j++;
			}
		}
	}
}
void loadLords(ifstream& file, AethelGardEngine& engine) {
	char line[256];

	file.getline(line, 256);

	while (file.getline(line, 256)) {

		if (line[0] == '#') continue;
		if (line[0] == ',') break;
		if (line[0] == '\0') continue;

		int idx = 0;
		char token[100];

		getpart(line, idx, token);
		int id = integer(token);
		getpart(line, idx, token);
		char name[100];
		int i = 0;
		while (token[i] != '\0') { name[i] = token[i]; i++; }
		name[i] = '\0';
		getpart(line, idx, token);
		int age = integer(token);

		getpart(line, idx, token);

		getpart(line, idx, token);
		char type[50];
		i = 0;
		while (token[i] != '\0') { type[i] = token[i]; i++; }
		type[i] = '\0';

		getpart(line, idx, token);
		int realm = integer(token);
		getpart(line, idx, token);
		int strat = integer(token);
		getpart(line, idx, token);
		int amb = integer(token);
		getpart(line, idx, token);
		int dip = integer(token);

		if (compare(type, "LandedLord")) {
			LandedLord* l = new LandedLord(name, id, age, true);
			l->strategyStat = strat;
			l->ambitionStat = amb;
			l->diplomacystat = dip;

			engine.realms[realm]->currentLord = l;
			engine.realmcurlordcount++;
		}

		getpart(line, idx, token);
	}
}
int loadConfig(ifstream& file) {
	char line[256];
	int threatMax = 0;
	while (file.getline(line, 256)) {
		if (line[0] == '[') break;
		int i = 0;
		while (line[i] != '=' && line[i] != '\0') i++;
		if (line[i] == '=') {
			i++;
			int num = 0;
			while (line[i] >= '0' && line[i] <= '9') {
				num = num * 10 + (line[i] - '0');
				i++;
			}
			threatMax = num;
		}
	}
	return threatMax;
}
void loadMilitary(ifstream& file, AethelGardEngine& engine) {

	char line[256];
	file.getline(line, 256);

	while (file.getline(line, 256)) {

		if (line[0] == ',') break;
		if (line[0] == '\0') continue;

		int idx = 0;
		char token[50];

		getpart(line, idx, token);
		int realm = integer(token);

		getpart(line, idx, token);
		char type[50];
		int i = 0;
		while (token[i] != '\0') { type[i] = token[i]; i++; }
		type[i] = '\0';

		getpart(line, idx, token);
		int count = integer(token);
		getpart(line, idx, token);
		int attack = integer(token);
		getpart(line, idx, token);
		int hp = integer(token);
		getpart(line, idx, token);
		int siege = integer(token);

		getpart(line, idx, token);

		for (int c = 0; c < count; c++) {
			if (compare(type, "Knight")) {
				Knight* k = new Knight(0, "Knight", hp, hp, 10, 1.0, attack, true, true);
				add(engine.realms[realm]->kng,	engine.realms[realm]->kngCount,	k);
			}
			else if (compare(type, "Cavalry")) {
				Cavalry* cav = new Cavalry(0, "Cavalry", hp, hp, 5, 1.0, attack, 2, 1);
				add(engine.realms[realm]->cav,engine.realms[realm]->cavCount,	cav);
			}
			else if (compare(type, "FootSoldier")) {
				FootSoldier* fs = new FootSoldier(0, "Foot", hp, hp, 5, 1.0, attack, true);
				add(engine.realms[realm]->fs,engine.realms[realm]->fsCount,fs);
			}
			else if (compare(type, "BatteringRam")) {
				BatteringRam* br = new BatteringRam("Ram", 0, hp, hp, 20, 2.0, siege, 1.0);
				add(engine.realms[realm]->bat,engine.realms[realm]->batcount,br);
			}
			else if (compare(type, "Catapult")) {
				Catapult* cat = new Catapult("Catapult", 0, hp, hp, 10, 2.0, siege, 1.0, 50, 3);
				add(engine.realms[realm]->cat,engine.realms[realm]->catcount,cat);
			}
		}
	}
}
void loadSages(ifstream& file, AethelGardEngine& engine) {

	char line[256];
	file.getline(line, 256);

	while (file.getline(line, 256)) {

		if (line[0] == ',') break;
		if (line[0] == '\0') continue;

		int idx = 0;
		char token[100];

		getpart(line, idx, token);
		int realm = integer(token);
		getpart(line, idx, token);
		char name[100];
		int i = 0;
		while (token[i] != '\0') { name[i] = token[i]; i++; }
		name[i] = '\0';
		getpart(line, idx, token);
		int heal = integer(token);
		getpart(line, idx, token);
		int wisdom = integer(token);

		Sage* s = new Sage(name, 0, 30, true);

		add(engine.realms[realm]->ledgers,engine.realms[realm]->sageCount,s);
	}
}
void loadAssassins(ifstream& file, AethelGardEngine& engine) {

	char line[256];
	file.getline(line, 256);

	while (file.getline(line, 256)) {

		if (line[0] == ',') break;
		if (line[0] == '\0') continue;

		int idx = 0;
		char token[100];

		getpart(line, idx, token);
		int id = integer(token);
		getpart(line, idx, token);
		char name[100];
		int i = 0;
		while (token[i] != '\0') { name[i] = token[i]; i++; }
		name[i] = '\0';
		getpart(line, idx, token);
		int age = integer(token);
		getpart(line, idx, token);
		int stealth = integer(token);
		getpart(line, idx, token);
		int client = integer(token);

		Assassin* a = new Assassin(name, id, age, true);

		a->setStealth(stealth);
		a->setClient(client);

		add(engine.asas, engine.assassinCount, a);
	}
}

void loadAll(const char* filename, AethelGardEngine& engine) {
	ifstream file(filename);
	if (!file) return;

	char line[256];
	while (file.getline(line, 256)) {
		if (line[0] == '\0') continue;
		if (line[0] == '[') {
			switch (line[1]) {
			case 'K': loadKingdoms(file, engine); break;
			case 'R': loadRelations(file, engine); break;
			case 'L': loadLords(file, engine); break;
			case 'M': loadMilitary(file, engine); break;
			case 'S': loadSages(file, engine); break;
			case 'A': loadAssassins(file, engine); break;
			case 'C': engine.threatMax = loadConfig(file); break;
			}
		}
	}
}