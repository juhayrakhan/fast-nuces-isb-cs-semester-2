#include <iostream>
using namespace std;

class Organism;
char Sprawler = 'S', Hunter = 'H';

class Tile {
public: 
	float nutrientLevel;
	float toxicity;
	Organism* occupant;

	Tile() {
		nutrientLevel = rand() % 101;
		toxicity = rand() % 101;
		occupant = NULL;
	}
};

class Organism {
public:
	int x, y;
	int width, height;
	bool** internalShape;

	Organism(int x, int y, int w, int h): x(x), y(y), width(w), height(h) { allocateShape(); }
	virtual ~Organism() {deallocateShape();}

	void allocateShape() {
		internalShape = new bool* [height];
		for (int i = 0; i < height; i++) {
			internalShape[i] = new bool[width];
			for (int j = 0; j < width; j++)
				internalShape[i][j] = true;
		}
	}

	void deallocateShape() {
		for (int i = 0; i < height; i++) delete[] internalShape[i];
		delete[] internalShape;
	}

	float getAverageNutrients(Tile** world, int worldW, int worldH) {
		float sum = 0;
		int count = 0;

		for (int i = 0; i < height; i++) for (int j = 0; j < width; j++)
			if (internalShape[i][j]) {
				int wx = x + j, wy = y + i;
				if (wx >= 0 && wy >= 0 && wx < worldW && wy < worldH) {
					sum += world[wy][wx].nutrientLevel;
					count++;
				}
		}

		if (count == 0) return 0;
		return sum / count;
	}

	virtual void update(Tile** world, int w, int h, Organism**& population, int& popSize) = 0;
	virtual char getSymbol() = 0;
};

class FractalSprawler : public Organism {
public:
	FractalSprawler(int x, int y) : Organism(x, y, 2, 2) {}
	FractalSprawler(int x, int y, int w, int h) : Organism(x, y, w, h) {}

	void resize(int newW, int newH) {
		deallocateShape();
		width = newW;
		height = newH;
		allocateShape();
	}

	void update(Tile** world, int w, int h, Organism**& population, int& popSize) {
		float avg = getAverageNutrients(world, w, h);

		if (avg > 70) resize(width + 2, height + 2);
		else if (avg < 30)
			if (width > 1 && height > 1) resize(width - 1, height - 1);
			else width = height = 0;

		if (width >= 5 && height >= 5) {
			Organism** newPop = new Organism * [popSize + 1];

			for (int i = 0; i < popSize; i++) newPop[i] = population[i];

			newPop[popSize] = new FractalSprawler(x + 2, y + 2);

			delete[] population;
			population = newPop;
			popSize++;
			resize(2, 2);
		}
	}

	char getSymbol() { return Sprawler; }
};

class KineticHunter : public Organism {
public:
	KineticHunter(int x, int y) : Organism(x, y, 2, 2) {}
	KineticHunter(int x, int y, int w, int h) : Organism(x, y, w, h) {}

	void update(Tile** world, int w, int h, Organism**& population, int& popSize) {

		Organism* target = NULL;
		// abhi ke liye meney just first sprawler ko target kiya hey, can u add the radius(square) logic here
		for (int i = 0; i < popSize; i++) {
			if (population[i]->getSymbol()==Sprawler) {
				target = population[i];
				break;
			}
		}

		if (target) {
			if (target->x > x)
				x++;
			else if (target->x < x)
				x--;

			if (target->y > y)
				y++;
			else if (target->y < y)
				y--;
		}
		for (int i = 0; i < popSize; i++) {
			FractalSprawler* fs = (FractalSprawler*)population[i];
			if (fs&& abs(fs->x - x) < 2 && abs(fs->y - y) < 2&& fs->width > 1)
				fs->resize(fs->width - 1, fs->height - 1);
		}
	}

	char getSymbol() { return Hunter; }
};

class World {
public:
	Tile** grid;
	int width, height;

	Organism** population;
	int popSize;
	int maxpopSize;

	World(int w, int h): width(h), height(h) {
		grid = new Tile * [height];
		for (int i = 0; i < height; i++) grid[i] = new Tile[width];
		popSize = 0, maxpopSize = 8;
		population = new Organism * [maxpopSize];
	}

	void addOrganism(Organism* org) {
		population[popSize] = org;
		popSize++;
		if (popSize == maxpopSize) {
			maxpopSize *= 2;
			Organism** newpop = new Organism * [maxpopSize];
			for (int i = 0; i < popSize; i++)
				newpop[i] = population[i];
			delete[] population;
			population = newpop;
		}
	}

	~World() {
		for (int i = 0; i < height; i++)
			delete[] grid[i];
		delete[] grid;

		for (int i = 0; i < popSize; i++)
			delete population[i];
		delete[] population;
	}

	void clearOccupants() {
		for (int i = 0; i < height; i++) for (int j = 0; j < width; j++)
			grid[i][j].occupant = nullptr;
	}

	void mapOrganisms() {
		for (int k = 0; k < popSize; k++) {
			Organism* org = population[k];
			for (int i = 0; i < org->height; i++) for (int j = 0; j < org->width; j++)
				if (org->internalShape[i][j]) {
					int wx = org->x + j, wy = org->y + i;
					if (wx >= 0 && wy >= 0 && wx < width && wy < height) grid[wy][wx].occupant = org;
				}
		}
	}

	void runIteration() {
		for (int i = 0; i < popSize; i++)
			population[i]->update(grid, width, height, population, popSize);
		clearOccupants();
		mapOrganisms();
	}

	void display() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++)
				if (grid[i][j].occupant == NULL) cout << "\033[40m  \033[0m";
				else cout << (grid[i][j].occupant->getSymbol() == 'H' ? "\033[41m  \033[0m" : "\033[42m  \033[0m");
			cout << endl;
		}
		float tox = 0;
		for (int i = 0; i < height; i++) for (int j = 0; j < width; j++)
			tox += grid[i][j].toxicity;

		cout << "Population: " << popSize << endl;
		cout << "Avg Toxicity: " << tox / (width * height) << endl;
	}
};

