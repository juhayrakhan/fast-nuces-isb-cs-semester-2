#include <windows.h>
#include <iostream>
using namespace std;

int fontSize = 28;
int scrollY = 10;
int footerspace = 40;
int totalpagecount = 1;

int leftmargin = 100, rightmargin = 100, topmargin = 50, bottommargin = 100;
int padding = 20;

int minspace = 100;
int minscreenwidth = 2 * (leftmargin + rightmargin + minspace + 2 * padding),
minscreenheight = 2 * (topmargin + bottommargin + minspace + 2 * padding);


struct Entry {
	wchar_t word[200];
	int count;
	wchar_t pos[20];
	wchar_t definition[1000];
};

struct Dictionary {
	Entry entries[176010];
	int size = 0;
} dictionary;

void towchar(const char* src, wchar_t* dest) {
	int i = 0;
	for (; src[i] != '\0'; i++)
		dest[i] = (wchar_t)src[i];
	dest[i] = L'\0';
}

void copying(char* src, int& i, char* dest, char delimiter) {
	int j = 0;
	while (src[i] != delimiter && src[i] != '\0') dest[j++] = src[i++];
	dest[j] = '\0';
	if (src[i] == delimiter) i++;
}

void copying(char* src, int& i, char* dest) {
	int j = 0;
	if (src[i] == '"') i++;
	while (src[i] != '\0') {
		if (src[i] == '"') {
			if (src[i + 1] == '"') i += 2;
			else { i++; break; }
		}
		else dest[j++] = src[i++];
	}
	dest[j] = '\0';
	if (src[i] == ',') i++;
}

void loadDictionary() {

	HANDLE file = CreateFileW(L"OPTED-Dictionary.csv", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD fileSize = GetFileSize(file, NULL); DWORD bytesRead;
	char* buffer = new char[fileSize + 1];

	ReadFile(file, buffer, fileSize, &bytesRead, NULL);
	buffer[fileSize] = '\0';

	int i = 0;
	for (; buffer[i] != '\n'; i++);
	i++;

	while (buffer[i] != '\0' && dictionary.size < 176009) {
		Entry& e = dictionary.entries[dictionary.size];

		char tempWord[500], tempPos[100], tempDef[1000], temp[10];

		copying(buffer, i, tempWord, ',');
		copying(buffer, i, temp, ',');
		i += 3;	copying(buffer, i, tempPos);
		i += 3;	copying(buffer, i, tempDef);

		e.count = 0;
		for (int k = 0; temp[k] != '\0'; k++)
			e.count = e.count * 10 + temp[k] - 48;

		towchar(tempWord, e.word);
		towchar(tempPos, e.pos);
		towchar(tempDef, e.definition);

		for (; buffer[i] != '\n' && buffer[i] != '\0'; i++);
		if (buffer[i] == '\n') i++;

		dictionary.size++;
	}

	if (buffer) delete[] buffer;
}






bool compare(const wchar_t* a, const wchar_t* b) {
	if (!a || !b) return false;
	int i = 0;
	while (a[i] && b[i]) {
		wchar_t ca = a[i];
		wchar_t cb = b[i];

		if (ca >= L'A' && ca <= L'Z') ca += L'a' - L'A';
		if (cb >= L'A' && cb <= L'Z') cb += L'a' - L'A';

		if (ca != cb) return false;
		i++;
	}
	return a[i] == b[i];
}

bool isInDictionary(const wchar_t* word) {
	if (dictionary.size == 0) loadDictionary();
	for (int i = 0; i < dictionary.size; i++) {
		if (compare(dictionary.entries[i].word, word)) return true;
	}
	return false;
}

int strLength(const wchar_t* str) {
	int i = 0;
	while (str[i] != '\0') i++;
	return i;
}

void strCopy(char* dest, const char* src) {
	int i = 0;
	while (src[i] != '\0') {
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
}

bool isPunct(const char c) {
	return (c == '.' || c == ',' || c == '!' || c == '?' ||
		c == ';' || c == ':' || c == '(' || c == ')' ||
		c == '[' || c == ']' || c == '{' || c == '}' ||
		c == '"' || c == '\'');
}

wchar_t textBuffer[1000000];
int textLen = 0;

class Word {
	wchar_t* text;
public:
	Word() : text(nullptr) {}
	Word(const wchar_t* str) {
		int len = strLength(str);
		text = new wchar_t[len + 1];
		for (int i = 0; i <= len; i++) text[i] = str[i];
	}
	Word(const Word& other) {
		int len = strLength(other.text);
		text = new wchar_t[len + 1];
		for (int i = 0; i <= len; i++) text[i] = other.text[i];
	}
	Word& operator=(const Word& other) {
		if (this != &other) {
			delete[] text;
			int len = strLength(other.text);
			text = new wchar_t[len + 1];
			for (int i = 0; i <= len; i++) text[i] = other.text[i];
		}
		return *this;
	}
	~Word() {
		if (text) delete[] text;
	}
	int length() const { return strLength(text); }
	const wchar_t* getText() const { return text; }
	int getPixelWidth(HDC hdc) const {
		SIZE sz;
		GetTextExtentPoint32A(hdc, (char*)text, length(), &sz);
		return sz.cx;
	}
};

class Line {
	Word* words;
	int capacity;
	int wordCount;
	int currentLength;
	void resize() {
		capacity *= 2;
		Word* temp = new Word[capacity];
		for (int i = 0; i < wordCount; i++) temp[i] = words[i];
		delete[] words;
		words = temp;
	}
public:
	Line() {
		capacity = 4;
		wordCount = 0;
		currentLength = 0;
		words = new Word[capacity];
	}
	Line(const Line& other) {
		capacity = other.capacity;
		wordCount = other.wordCount;
		currentLength = other.currentLength;
		words = new Word[capacity];
		for (int i = 0; i < wordCount; i++) words[i] = other.words[i];
	}
	Line& operator=(const Line& other) {
		if (this != &other) {
			delete[] words;
			capacity = other.capacity;
			wordCount = other.wordCount;
			currentLength = other.currentLength;
			words = new Word[capacity];
			for (int i = 0; i < wordCount; i++) words[i] = other.words[i];
		}
		return *this;
	}
	~Line() {
		delete[] words;
	}
	bool canAddWord(Word* w, int maxWidth, HDC hdc) {
		int wordWidth = w->getPixelWidth(hdc);
		int spacing = 0;
		if (wordCount > 0) {
			SIZE sz;
			GetTextExtentPoint32A(hdc, " ", 1, &sz);
			spacing = sz.cx;
		}
		return (currentLength + spacing + wordWidth) <= maxWidth;
	}
	Word& getWord(int x) {
		return words[x];
	}
	void addWord(Word* w, HDC hdc) {
		if (wordCount == capacity) resize();

		if (wordCount > 0) {
			SIZE sz;
			GetTextExtentPoint32A(hdc, " ", 1, &sz);
			currentLength += sz.cx;
		}

		currentLength += w->getPixelWidth(hdc);
		words[wordCount++] = *w;
	}
	void draw(HDC& hdc, int x, int y) const {
		int cx = x;

		SIZE spaceSize;
		GetTextExtentPoint32W(hdc, L" ", 1, &spaceSize);

		for (int i = 0; i < wordCount; i++) {
			const wchar_t* txt = words[i].getText();

			if (!isPunct(txt[0]) && i != 0) {
				cx += spaceSize.cx;
			}
			if (!isInDictionary(txt)&&!isPunct(*txt))
				SetTextColor(hdc, RGB(255, 150, 150));
			else
				SetTextColor(hdc, RGB(255, 255, 255));

			int len = 0;
			while (txt[len]) len++;
			TextOutW(hdc, cx, y, txt, len);
			SIZE sz;
			GetTextExtentPoint32W(hdc, txt, len, &sz);
			cx += sz.cx;
		}
	}
};

void renderLine(HDC hdc, int x, int y, int& cursorPos, int maxX) {
	int cx = x;
	int charIndex = 0;

	while (charIndex < textLen) {
		wchar_t ch = textBuffer[charIndex];
		TEXTMETRIC tm; GetTextMetrics(hdc, &tm);
		int lineHeight = tm.tmHeight;

		SIZE sz;
		GetTextExtentPoint32W(hdc, &ch, 1, &sz);
		if (cx + sz.cx > maxX) { 
			cx = x;
			y += lineHeight;
		}
		if (charIndex == cursorPos) 
			TextOutW(hdc, cx, y, L"|", 1);
		TextOutW(hdc, cx, y, &ch, 1);
		cx += sz.cx;
		charIndex++;
	}
	if (cursorPos == textLen)
		TextOutW(hdc, cx, y, L"|", 1);
}

class Page {
	Line* lines;
	int capacity;
	int lineCount;

	void resize() {
		capacity *= 2;
		Line* temp = new Line[capacity];
		for (int i = 0; i < lineCount; i++) temp[i] = lines[i];
		delete[] lines;
		lines = temp;
	}

public:
	Page() {
		capacity = 4;
		lineCount = 0;
		lines = new Line[capacity];
	}

	Page(const Page& other) {
		capacity = other.capacity;
		lineCount = other.lineCount;
		lines = new Line[capacity];
		for (int i = 0; i < lineCount; i++) lines[i] = other.lines[i];
	}

	Page& operator=(const Page& other) {
		if (this != &other) {
			delete[] lines;
			capacity = other.capacity;
			lineCount = other.lineCount;
			lines = new Line[capacity];
			for (int i = 0; i < lineCount; i++) lines[i] = other.lines[i];
		}
		return *this;
	}

	~Page() {
		delete[] lines;
	}

	void addLine(const Line& l) {
		if (lineCount == capacity) resize();
		lines[lineCount++] = l;
	}

	int getLineCount() const { return lineCount; }
	Line& getLine(int i) { return lines[i]; }
};

Page* paginate(const char* text, int& pageCount, int maxWidth, int maxlines, HDC hdc) {
	int pageCap = 2;
	Page* pages = new Page[pageCap];
	pageCount = 0;
	Page currentPage;Line currentLine;

	int i = 0;
	while (text[i] != '\0') {
		while (text[i] == ' ') i++;
		if (text[i] == '\0') break;
		wchar_t buffer[100];
		int j = 0;
		if (isPunct(text[i])) {
			buffer[0] = text[i++];
			buffer[1] = '\0';
		}
		else {
			while (text[i] && text[i] != ' ' && !isPunct(text[i]))
				buffer[j++] = text[i++];
			buffer[j] = '\0';
		}
		Word w(buffer);
		if (!currentLine.canAddWord(&w, maxWidth, hdc)) {
			currentPage.addLine(currentLine);
			if (currentPage.getLineCount() == maxlines) {
				if (pageCount == pageCap) {
					pageCap *= 2;
					Page* temp = new Page[pageCap];
					for (int k = 0; k < pageCount; k++) temp[k] = pages[k];
					delete[] pages;
					pages = temp;
				}
				pages[pageCount++] = currentPage;
				currentPage = Page();
			}
			currentLine = Line();
		}
		currentLine.addWord(&w,hdc);
	}
	currentPage.addLine(currentLine);
	if (pageCount == pageCap) {
		pageCap *= 2;
		Page* temp = new Page[pageCap];
		for (int k = 0; k < pageCount; k++) temp[k] = pages[k];
		delete[] pages;
		pages = temp;
	}
	pages[pageCount++] = currentPage;
	return pages;
}





void appendNumber(wchar_t* buffer, int& index, int num) {
	wchar_t temp[10];
	int i = 0;
	int n = num;
	wchar_t rev[10];
	int j = 0;
	for (; n > 0; j++, n /= 10) rev[j] = (n % 10) + L'0';
	for (; j > 0; i++, j--) temp[i] = rev[j - 1];
	for (int k = 0; k < i; index++, k++) buffer[index] = temp[k];
}

void drawfooter(RECT pageRect, HDC hdc, int pageNumber) {
	int footerHeight = fontSize - 15;
	int footerY = pageRect.bottom + footerHeight;

	wchar_t footer[64];
	int len = 0;

	footer[len++] = L'-';
	footer[len++] = L' ';

	appendNumber(footer, len, pageNumber);

	footer[len++] = L' ';
	footer[len++] = L'-';
	footer[len] = L'\0';

	SIZE sz;
	GetTextExtentPoint32W(hdc, footer, len, &sz);

	int footerX = pageRect.left + ((pageRect.right - pageRect.left) - sz.cx) / 2;

	TextOutW(hdc, footerX, footerY, footer, len);
}

void ScaleScreen(WPARAM wParam, LPARAM lParam, int& lastY) {
	if (wParam & MK_MBUTTON) {
		int y = HIWORD(lParam);
		if (lastY != -1) {
			int dy = y - lastY;
			fontSize += dy / 5;
			if (fontSize < 12) fontSize = 12;
			if (fontSize > 36) fontSize = 36;
		}
		lastY = y;
	}
	else lastY = -1;
}