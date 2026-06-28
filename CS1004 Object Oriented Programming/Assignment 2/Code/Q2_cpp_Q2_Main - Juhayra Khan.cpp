#include "Q2_Submission.h"
using namespace std;

int main() {


	int x, y;
	cout << "Enter Initial storage and ram(atleast 100 each): ";
	cin >> x >> y;
	if (x < 100 || y < 1010) {
		cout << "Invalid\n";
		return 0;
	}

	Computer comp(x, y);

	int choice;
	char app[100], name[100], content[1000], key[100], value[100];
	double a, b;
	char op;
	int size;

	while (true) {

		cout << "\n\n---------------> COMPUTER INTERFACE <---------------\n\n1.Launch App\n2.Close App\n3.Create File\n4.View File\n5.Delete File\n6.Restore File\n7.Empty Recycle Bin\n";
		cout << "8. Calculate\n9. Export Calculations\n10. Set Setting\n11. Get Setting\n12. Display System Status\n0. Exit\n\n----> ";
		cin >> choice;
		if (choice == 0) break;

		cout << "\n----->\t";
		switch (choice) {
		case 1:
			cout << "\nFileExplorer\nRecycleBin\nCalculator\nSettings\n----->\tEnter app name: "; cin >> app;
			cout << "----->\t";
			cout << (comp.launchApp(app) ? "Launched\n" : "ERROR: Failed to Open App\n");
			break;
		case 2:
			cout << "OPEN APPS:\n";
			cout << (comp.getFileExplorer()->isRunning ? "FileExplorer\n" : "");
			cout << (comp.getRecycleBin()->isRunning ? "RecycleBin\n" : "");
			cout << (comp.getSettings()->isRunning ? "Settings\n" : "");
			cout << (comp.getCalculator()->isRunning ? "Calculator\n" : "");
			cout << "\n----->\tEnter app name: "; cin >> app;
			cout << (comp.closeApp(app) ? "Closed\n" : "ERROR: Failed to Close App\n");
			break;
		case 3:
			if (!comp.getFileExplorer()->isRunning) {
				cout << "ERROR: FileExplorer not Open\n";
				break;
			}
			cout << "File name: "; cin.getline(name,100);
			cout << "----->\tContent: ";
			cin.ignore();
			cin.getline(content, 100);
			cout << "----->\tSize: ";
			cin >> size;
			cout << (comp.createFile(name, content, size) ? "Created\n" : "ERROR: Failed to Create File\n");
			break;

		case 4:
			if (!comp.getFileExplorer()->isRunning) {
				cout << "ERROR: FileExplorer not Open\n";
				break;
			}
			cout << "----->\tFile name: "; cin.getline(name,100);
			cout << "----->\t";
			cout << (comp.getFileExplorer()->viewFile(name) ? "File Exists\n" : "ERROR: File not found\n");
			break;

		case 5:
			if (!comp.getFileExplorer()->isRunning || !comp.getRecycleBin()->isRunning) {
				if (!comp.getFileExplorer()->isRunning) {
					cout << "ERROR: FileExplorer not Open\n";
				}
				if (!comp.getRecycleBin()->isRunning) {
					cout << "ERROR: RecycleBin not Open\n";
				}
			}
			cout << "File name: ";
			cin.getline(name,100);
			cout << "----->\t";
			cout << (comp.deleteFile(name) ? "File Deleted" : "ERROR: Failed to Delete File");
			break;

		case 6:
			if (!comp.getFileExplorer()->isRunning || !comp.getRecycleBin()->isRunning) {
				if (!comp.getRecycleBin()->isRunning)
					cout << "\nERROR: RecycleBin not Open";
				if (!comp.getFileExplorer()->isRunning)
					cout << "\nERROR: FileExplorer not Open";
				cout << '\n';
				break;
			}
			cout << "File name: "; cin.getline(name,100);
			cout << "----->\t";
			cout << (comp.restoreFile(name) ? "Restored\n" : "ERROR: Failed to Restore File\n");
			break;

		case 7:
			if (!comp.getRecycleBin()->isRunning) {
				cout << "ERROR: RecycleBin not Open\n";
				break;
			}
			comp.emptyRecycleBin();
			cout << "Recycle bin emptied\n";
			break;

		case 8:
			cout << "Enter: number operator number (e.g. 3 + 4): ";
			cin >> a >> op >> b;
			cout << "----->\t";
			cout << "Result: " << comp.calculate(a, op, b) << "\n";
			break;

		case 9:
			if (!comp.getFileExplorer()->isRunning) {
				cout << "ERROR: FileExplorer not Open\n";
				break;
			}
			cout << "Export filename: ";
			cin.getline(name,100);
			cout << "----->\t";
			cout << (comp.exportCalculations(name) ? "Exported\n" : "Failed\n");
			break;

		case 10:
			if (!comp.getSettings()->isRunning) {
				cout << "ERROR: Settings not Open\n";
				break;
			}
			cout << "Key: ";
			cin.getline(key,10);
			cout << "----->\t";
			if (comp.getSetting(key)) {
				cout << "Value: ";
				cin.getline(value,10);
				cout << "----->\t";
				cout << (comp.setSetting(key, value) ? "Set\n" : "Invalid Value\n");
			}
			else
				cout << "Not found\n";
			break;

		case 11:
			if (!comp.getSettings()->isRunning) {
				cout << "ERROR: Settings not Open\n";
				break;
			}
			cout << "Key: ";
			cin >> key;
			cout << "----->\t";
			if (comp.getSetting(key))
				cout << "Value: " << comp.getSetting(key) << "\n";
			else
				cout << "Not found\n";
			break;
		case 12:
			cout << "\n";
			comp.displaySystemStatus();
			cout << "----->\n";
			break;
		default:
			cout << "----->\tInvalid choice\n";
		}

	}

	return 0;
}