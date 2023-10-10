#include <Windows.h>

RECT clientRect;
int fwKeys, zDelta;
int recWidth = 200, recHeight = 200;
int recX = 100, recY = 100;
int speedX, speedY;

void Collision() {
	if (recX + speedX <= clientRect.left) {
		recX = 0;
		speedX = -speedX;
	}
	if (recY + speedY <= clientRect.top) {
		recY = 0;
		speedY = -speedY;
	}
	if (recX + recWidth + speedX >= clientRect.right) {
		recX = clientRect.right - recWidth;
		speedX = -speedX;
	}
	if (recY + recHeight + speedY >= clientRect.bottom) {
		recY = clientRect.bottom - recHeight;
		speedY = -speedY;
	}

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		GetClientRect(hWnd, &clientRect);
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		FillRect(hdc, &clientRect, (HBRUSH)GetStockObject(WHITE_BRUSH));
		//Rectangle(hdc, clientRect.left + 30, clientRect.top + 30, clientRect.right - 700, clientRect.bottom - 300);
		Rectangle(hdc, recX, recY, recX+recWidth, recY+recHeight);
		RECT box = { recX, recY, recX + recWidth, recY + recHeight};
		FillRect(hdc, &box, (HBRUSH)GetStockObject(BLACK_BRUSH));

		EndPaint(hWnd, &ps);
	}
	break;
	
	case WM_MOUSEWHEEL:
	{
		fwKeys = GET_KEYSTATE_WPARAM(wParam);
		zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		if (fwKeys == 4)
			speedX += zDelta / 20;
		else
			speedY += zDelta / 20;
	}
	break;
	case WM_LBUTTONDBLCLK:
		MessageBox(hWnd, L"Hello, World!", L"Message", MB_OK);
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
			speedX -= 1;
		else if (speedX < 0)
			speedX += 1;
		if (speedY > 0)
			speedY -= 1;
		else if (speedY < 0)
			speedY += 1;
		Collision();
		
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		recX += speedX;
		recY += speedY;
	}
	return (int)msg.wParam;
}


