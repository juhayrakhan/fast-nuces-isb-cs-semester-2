#include "Q6_Submission.h"
using namespace std;


int main() {
	int choice;
	Archive* arch = createArchive(3);

	Movie* movies[100];
	Actor* actors[100];
	int movieCount = 0, actorCount = 0;

	while (true) {
		cout << "\n====== MOVIE ARCHIVE MENU ======\n1. Add Movie\n2. Insert Movie into Archive\n3. Search Movie\n4. Range Query\n5. Delete Movie\n";
		cout << "6. Retitle Movie\n7. Add Flagged Title\n8. Add Actor\n9. Add Rival\n10. Cast Actor\n0. Exit\n\n----> ";
		cin >> choice;

		switch (choice) {
		default: cout << "Invalid\n"; break;
		case 0: return 0;
		case 1: {
			int id, year;
			char title[100];

			cout << "ID -----> "; cin >> id;
			cout << "Title --> "; cin >> title;
			cout << "Year ---> "; cin >> year;

			movies[movieCount++] = addMovie(id, title, year, 0);
			cout << "Movie created.\n";
			break;
		}
		case 2: {
			if (movieCount == 0) {
				cout << "No movies in archive\n"; break;
			}

			cout << "Movie Index (Between 0 and " << movieCount << " ----> "; cin >> choice;

			if (choice >= 0 && choice < movieCount) {
				archiveInsert(arch, movies[choice]);
				cout << "Inserted into archive.\n";
			}
			else cout << "Error: Index must be between 0 and " << movieCount << endl;
			break;
		}
		case 3: {
			char title[100];
			cout << "Enter title to search: ";
			cin >> title;

			Movie* m = archiveSearch(arch, title);
			if (m) cout << "Found " << m->title << endl;
			else cout << "Not found\n";
			break;
		}
		case 4: {
			char from[100], to[100];
			cout << "From: "; cin >> from;
			cout << "To: "; cin >> to;
			archiveRangeQuery(arch, from, to);
			break;
		}
		case 5: {
			char title[100];
			cout << "Enter Titke: ";
			cin >> title;
			if (!archiveDelete(arch, title)) cout << "Deleted\n";
			else cout << "Not found\n";
			break;
		}
		case 6: {
			char oldTitle[100], newTitle[100];
			cout << "Old title: "; cin >> oldTitle;
			cout << "New title: "; cin >> newTitle;

			Movie* m = archiveSearch(arch, oldTitle);
			if (m) {
				retitleMovie(arch, m, newTitle);
				cout << "Retitled.\n";
			}
			else cout << "Movie not found\n";
			break;
		}
		case 7: {
			char title[100], flag[100];
			cout << "Movie title: "; cin >> title;
			cout << "Flagged title: "; cin >> flag;
			Movie* m = archiveSearch(arch, title);
			if (m) {
				addFlaggedTitle(m, flag);
				cout << "Flag added\n";
			}
			break;
		}
		case 8: {
			int id, age;
			char name[100];

			cout << "Enter ID: "; cin >> id;
			cout << "Enter Name: "; cin >> name;
			cout << "Enter Age: "; cin >> age;

			actors[actorCount++] = addActor(id, name, age);
			cout << "Actor added\n"; break;
		}
		case 9: {
			int a, b;
			cout << "Actor index A: "; cin >> a;
			cout << "Actor index B: "; cin >> b;
			if (a < actorCount && b < actorCount) {
				addRival(actors[a], actors[b]);
				cout << "Rival added\n";
			}
			break;
		}
		case 10: {
			int a, m;
			cout << "Actor index: "; cin >> a;
			cout << "Movie index: "; cin >> m;
			if (a < actorCount && m < movieCount)
				if (!castActor(actors[a], movies[m])) cout << "Cast successful\n";
				else cout << "Blocked due to rival\n";
			break;
		}
		}
	}
	deleteArchive(arch);
	return 0;
}