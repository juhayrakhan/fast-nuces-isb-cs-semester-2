#include "Q7_Submission.h"
using namespace std;

int cursorpos = 0;
struct cursordata {
	int x, y;
	int index;
};

cursordata* layout = nullptr;
int layoutCount = 0;

void BuildLayout(HDC hdc, RECT pageRect) {
	delete[] layout;
	layout = new cursordata[textLen + 1];
	layoutCount = 0;

	int x = pageRect.left + padding;
	int y = pageRect.top + padding;

	TEXTMETRIC tm;
	GetTextMetrics(hdc, &tm);
	int lineHeight = tm.tmHeight;

	for (int i = 0; i < textLen; i++) {
		wchar_t ch = textBuffer[i];

		layout[layoutCount++] = {x, y,i};
		if (ch == L'\n') {
			x = pageRect.left + padding;
			y += lineHeight;
			continue;
		}
		SIZE sz;
		GetTextExtentPoint32W(hdc, &ch, 1, &sz);
		if (x + sz.cx > pageRect.right - padding) {
			x = pageRect.left + padding;
			y += lineHeight;
		}
		x += sz.cx;
	}

	layout[layoutCount++] = { x, y, textLen };
}

void rendercursor(HDC hdc) {
	TEXTMETRIC tm; GetTextMetrics(hdc, &tm);
	for (int i = 0; i < layoutCount; i++) {
		if (layout[i].index == cursorpos) {
			TextOutW(hdc, layout[i].x - tm.tmAveCharWidth/2, layout[i].y, L"|", 1);
			return;
		}
	}
}

void RenderText(HDC hdc, RECT rightPage, RECT leftPage) {
	//int x = pageRect.left;
	//int y = pageRect.top;
	char* temp = new char[textLen + 1];
	for (int i = 0; i < textLen; i++)
		temp[i] = (char)textBuffer[i];
	temp[textLen] = '\0';
	TEXTMETRIC tm;
	GetTextMetrics(hdc, &tm);
	//int lineHeight = tm.tmHeight;
	int maxPixelWidth = rightPage.right - rightPage.left-padding;
	int maxLines = (rightPage.bottom - rightPage.top) / tm.tmHeight;
	int pageCount = 0;
	Page* pages = paginate(temp, pageCount, maxPixelWidth, maxLines, hdc);
	//totalpagecount = pageCount;
	//if (pageCount > 0) {
	//	Page& p = pages[0];
	//	for (int i = 0; i < p.getLineCount(); i++) {
	//		p.getLine(i).draw(hdc, x, y);
	//		y += lineHeight;
	//	}
	//}
	//delete[] temp; delete[] pages;

	int lineHeight = tm.tmHeight;

	Page p; RECT r;

	if (pageCount > 0) {
		p = pages[0];
		r = leftPage;
		int index = 0;
		int y = leftPage.top + padding;
		for (int i = 0; i < p.getLineCount(); i++) {
			p.getLine(i).draw(hdc, leftPage.left + padding, y);
			y += lineHeight;
		}
	}

	if (pageCount > 1) {
		p = pages[1];
		r = rightPage;
		int y = rightPage.top + padding;
		int index = 0;
		for (int i = 0; i < p.getLineCount(); i++) {
			p.getLine(i).draw(hdc, rightPage.left + padding, y);
			y += lineHeight;
		}
	}

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	static int lastY = -1;
	switch (msg) {
	case WM_CHAR: {
		if (wParam == VK_BACK) {
			if (cursorpos > 0) {
				for (int i = cursorpos - 1; i < textLen - 1; i++)
					textBuffer[i] = textBuffer[i + 1];

				textLen--;
				cursorpos--;
			}
		}
		else if (wParam >= 32 && textLen < 999999) {
			for (int i = textLen; i > cursorpos; i--)
				textBuffer[i] = textBuffer[i - 1];

			textBuffer[cursorpos] = (wchar_t)wParam;
			textLen++;
			cursorpos++;
		}

		textBuffer[textLen] = L'\0';
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;
	}
	case WM_KEYDOWN: {

		if (wParam == VK_LEFT) {
			if (cursorpos > 0) cursorpos--;
		}
		else if (wParam == VK_RIGHT) {
			if (cursorpos < textLen) cursorpos++;
		}
		else if (wParam == VK_UP || wParam == VK_DOWN) {

			int currentX = 0, currentY = 0;

			for (int i = 0; i < layoutCount; i++) {
				if (layout[i].index == cursorpos) {
					currentX = layout[i].x;
					currentY = layout[i].y;
					break;
				}
			}

			TEXTMETRIC tm;
			HDC hdc = GetDC(hwnd);
			GetTextMetrics(hdc, &tm);
			ReleaseDC(hwnd, hdc);

			int targetY = currentY + (wParam == VK_DOWN ? tm.tmHeight : -tm.tmHeight);

			int bestIndex = cursorpos;
			int bestDist = 1e9;

			for (int i = 0; i < layoutCount; i++) {
				int dy = layout[i].y - targetY;

				if (abs(dy) < tm.tmHeight / 2) {
					int dx = layout[i].x - currentX;
					int dist = dx * dx;

					if (dist < bestDist) {
						bestDist = dist;
						bestIndex = layout[i].index;
					}
				}
			}

			cursorpos = bestIndex;
		}

		InvalidateRect(hwnd, NULL, TRUE);
		return 0;
	}
	case WM_MOUSEMOVE: {
		ScaleScreen(wParam, lParam, lastY);
		if (wParam & MK_MBUTTON) InvalidateRect(hwnd, NULL, TRUE);
		return 0;
	}
	case WM_LBUTTONDOWN: {
		int mouseX = LOWORD(lParam);
		int mouseY = HIWORD(lParam);

		int bestIndex = 0;
		int bestDist = 1e9;

		for (int i = 0; i < layoutCount; i++) {
			int dx = mouseX - layout[i].x;
			int dy = mouseY - layout[i].y;
			int dist = dx * dx + dy * dy;

			if (dist < bestDist) {
				bestDist = dist;
				bestIndex = layout[i].index;
			}
		}

		cursorpos = bestIndex;
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;
	}
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		RECT fullScreenRect; GetClientRect(hwnd, &fullScreenRect);
		HBRUSH bgBrush = CreateSolidBrush(RGB(20, 20, 40));
		FillRect(hdc, &fullScreenRect, bgBrush);
		DeleteObject(bgBrush);

		SetBkMode(hdc, TRANSPARENT); SetTextColor(hdc, RGB(255, 255, 255));
		HFONT font = CreateFont(fontSize, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Consolas");
		SelectObject(hdc, font);

		RECT outerRect;
		outerRect.left = leftmargin;
		outerRect.top = topmargin;
		outerRect.right = fullScreenRect.right - rightmargin;
		outerRect.bottom = fullScreenRect.bottom - bottommargin;
		bgBrush = CreateSolidBrush(RGB(35, 35, 64));
		//FillRect(hdc, &outerRect, bgBrush);
		DeleteObject(bgBrush);

		RECT pageRect;
		pageRect.left = leftmargin + padding;
		pageRect.top = topmargin + padding;
		pageRect.right = fullScreenRect.right - rightmargin - padding;
		pageRect.bottom = fullScreenRect.bottom - bottommargin - padding;

		int betweenpages = 20; 
		int pageWidth = (fullScreenRect.right - fullScreenRect.left - betweenpages - leftmargin - rightmargin) / 2;
		int pageHeight = fullScreenRect.bottom - fullScreenRect.top - topmargin - bottommargin;

		RECT leftPage;
		leftPage.left = leftmargin;
		leftPage.top = topmargin;
		leftPage.right = leftPage.left + pageWidth;
		leftPage.bottom = topmargin + pageHeight;

		RECT rightPage;
		rightPage.left = leftPage.right + betweenpages;
		rightPage.top = topmargin;
		rightPage.right = rightPage.left + pageWidth;
		rightPage.bottom = topmargin + pageHeight;

		bgBrush = CreateSolidBrush(RGB(35, 35, 64));
		FillRect(hdc, &leftPage, bgBrush);
		FillRect(hdc, &rightPage, bgBrush);
		DeleteObject(bgBrush);

		drawfooter(leftPage, hdc, 1);
		drawfooter(rightPage, hdc, 2);

		RenderText(hdc, rightPage,leftPage);

		BuildLayout(hdc, leftPage);
		rendercursor(hdc);

		DeleteObject(font);
		EndPaint(hwnd, &ps);

		return 0;
	}
	case WM_SIZE: {
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;
	}
	case WM_GETMINMAXINFO: {
		MINMAXINFO* mmi = (MINMAXINFO*)lParam;
		mmi->ptMinTrackSize.x = minscreenwidth;
		mmi->ptMinTrackSize.y = minscreenheight;
		return 0;
	}
	case WM_CLOSE:  DestroyWindow(hwnd); return 0;
	case WM_DESTROY: PostQuitMessage(0); return 0;
	default: return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"myWindowClass";
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc)) return 0;

	hwnd = CreateWindow(L"myWindowClass", L"The title of my window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 400, NULL, NULL, hInstance, NULL);

	if (hwnd == NULL) return 0;

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&Msg, NULL, 0, 0) > 0) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}