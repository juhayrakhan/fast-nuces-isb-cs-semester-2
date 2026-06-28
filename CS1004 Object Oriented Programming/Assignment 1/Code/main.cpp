// Question 4: Ironclad (INCOMPLETE)

#include "submission.h"
using namespace std;

int main(){

	char password[9]="fastfast";
	// cin.getline(password, 8);
	// password[8] = '\0';

	ifstream names("file_names.txt");
	int count; names >> count;
	char* filename= new char[256];

	for(int n = 0;n < count;n++){

		unsigned char keymodifier = modifier(password);
		unsigned char key = Key(password);

		firstpartofName(names, filename);
		secondpartofName(names, filename);
		cout << filename;
		ifstream currentfile(filename,ios::binary);
		int size = 256;
		char* x = new char[size];
		readfile(currentfile, x, size);

		int bytecount = 0;
		for(; x[bytecount] != '\0';bytecount++);
		
		int wordcount = bytecount / 4;
		int nextindex = (wordcount / 2) * 4;
		cout << "---" << bytecount << "---" << nextindex << "---";
		char* decrypted = new char[size]; int pos = 0;

		decrypthelper(x, decrypted, pos, key, nextindex * 4, wordcount);

		decrypted[pos] = '\0';

		cout << " \t: " << decrypted << "-done!" << endl;

		delete[] x; x = nullptr;
		delete[] decrypted; decrypted = nullptr;
	}

	delete[] filename;
	return 0;
}