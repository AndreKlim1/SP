#include <Windows.h>

RECT clientRect, clientFrame;
float zDelta;
int recWidth = 200, recHeight = 200;
int fwKeys, recX = 100, recY = 100;
float speedX, speedY;
bool dragged = false;
int tempX=0, tempY=0;
HDC hdc;
HANDLE handleBack;
const int BITMAP_SIZE = 200;
POINT imageCord;
BITMAP bm;
HANDLE handleBitmap;



void Collision() {
	if (recX + speedX <= clientRect.left) {
		recX = 0;
		speedX = -speedX;
		if (speedX == 0)
			speedX = 2;
	}
	if (recY + speedY <= clientRect.top) {
		recY = 0;
		speedY = -speedY;
		if (speedY == 0)
			speedY = 2;
	}
	if (recX + recWidth + speedX >= clientRect.right) {
		recX = clientRect.right - recWidth;
		speedX = -speedX;
		if (speedX == 0)
			speedX = -2;
	}
	if (recY + recHeight + speedY >= clientRect.bottom) {
		recY = clientRect.bottom - recHeight;
		speedY = -speedY;
		if (speedY == 0)
			speedY = -2;
	}

}

void DrawImage(HANDLE& hBmp, BITMAP& bitmap)
{
	
	RestoreDC(hdc, -1);
	SaveDC(hdc);
	FillRect(hdc, &clientFrame, (HBRUSH)(COLOR_WINDOW + 1));
	RECT imageRect{ recX, recY, recX + bitmap.bmWidth, recY + bitmap.bmHeight };
	HBRUSH hBrush = CreateSolidBrush(RGB(250, 118, 107));
	FillRect(hdc, &imageRect, hBrush);
	HDC hMemDC = CreateCompatibleDC(hdc);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBmp);
	if (hOldBitmap)
	{
		SetMapMode(hMemDC, GetMapMode(hdc));
		TransparentBlt(hdc, recX, recY, BITMAP_SIZE, BITMAP_SIZE, hMemDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, RGB(192, 255, 255));
		SelectObject(hMemDC, hOldBitmap);
	}
	DeleteDC(hMemDC);

	RestoreDC(hdc, -1);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	
	switch (message)
	{
	case WM_CREATE:
	{
		handleBitmap = LoadImage(NULL, L"cup.bmp", IMAGE_BITMAP, BITMAP_SIZE, BITMAP_SIZE, LR_LOADFROMFILE | LR_LOADTRANSPARENT);
		GetObject(handleBitmap, sizeof(bm), &bm);

		imageCord = { 100, 100 };
		break;
	}
	break;
	case WM_PAINT:
	{
		GetClientRect(hWnd, &clientRect);
		PAINTSTRUCT ps;
		hdc = BeginPaint(hWnd, &ps);
		FillRect(hdc, &clientRect, CreateSolidBrush(RGB(255, 255, 255)));
		DrawImage(handleBitmap, bm);
		EndPaint(hWnd, &ps);
		break;
	}
	break;
	
	case WM_MOUSEWHEEL:
	{
		fwKeys = GET_KEYSTATE_WPARAM(wParam);
		zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		if (fwKeys == 4)
			speedX += zDelta / 130;
		else
			speedY += zDelta / 130;
	}
	break;
	case WM_LBUTTONDOWN:
	{
		POINT mousePos = { 0, 0 };
		mousePos.x = LOWORD(lParam);
		mousePos.y = HIWORD(lParam);
		if (mousePos.x > recX && mousePos.x<recX + recWidth && mousePos.y>recY && mousePos.y < recY + recHeight)
		{
			dragged = true;
			tempX = mousePos.x;
			tempY = mousePos.y;
		}
	}
	break;
	case WM_LBUTTONUP:
	{
		dragged = false;
	}
	break;
	case WM_MOUSEMOVE:
	{
		POINT mousePos = { 0, 0 };
		mousePos.x = LOWORD(lParam);
		mousePos.y = HIWORD(lParam);
		if (dragged == true) {
			recX += mousePos.x - tempX;
			recY += mousePos.y - tempY;
			tempX = mousePos.x;
			tempY = mousePos.y;
		}
	}
	break;
	case WM_KEYDOWN:
	{
		fwKeys=GET_KEYSTATE_WPARAM(wParam);
		if (fwKeys == 'A')
			speedX = -5;
		if (fwKeys == 'W')
			speedY = -5;
		if (fwKeys == 'D')
			speedX = 5;
		if (fwKeys == 'S')
			speedY = 5;
	}
	break;
	
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex; HWND hWnd; MSG msg;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_DBLCLKS;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"HelloWorldClass";
	wcex.hIconSm = wcex.hIcon;
	RegisterClassEx(&wcex);
	hWnd = CreateWindow(L"HelloWorldClass", L"Hello, World!",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0,
		CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
		if (speedX > 0)
			speedX -= 0.1;
		else if (speedX < 0)
			speedX += 0.1;
		if (speedY > 0)
			speedY -= 0.1;
		else if (speedY < 0)
			speedY += 0.1;
		Collision();
		
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		recX += speedX;
		recY += speedY;
		
		
		if (speedX<1 && speedX>-1)
			speedX = 0;
		if (speedY<1 && speedY>-1)
			speedY = 0;
		
	}
	return (int)msg.wParam;
}


