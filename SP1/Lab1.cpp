#include <Windows.h>

RECT clientRect;
float zDelta;
int recWidth = 300, recHeight = 200;
int fwKeys, recX = 100, recY = 100;
float speedX, speedY;
bool dragged = false;
int tempX=0, tempY=0;

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
			speedX += zDelta / 130;
		else
			speedY += zDelta / 130;
	}
	break;
	case WM_LBUTTONDOWN:
	{
		POINT mousePos = { 0, 0 };
		//GetCursorPos(&mousePos);
		ClientToScreen(hWnd, &mousePos);
		if (mousePos.x > recX && mousePos.x<recX + recWidth && mousePos.y>recY && mousePos.y < recY + recHeight)
		{
			dragged = true;
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
	case WM_LBUTTONUP:
	{
		dragged = false;
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
		if (!dragged) {
			recX += speedX;
			recY += speedY;
		}
		else {
			POINT mousePos = { 0, 0 };
			ClientToScreen(hWnd, &mousePos);
			if (mousePos.x != tempX) {
				tempX = 0;
			}
			recX += mousePos.x-tempX;
			recY += mousePos.y-tempY;
			tempX = mousePos.x;
			tempY = mousePos.y;
			
		}
		if (speedX<1 && speedX>-1)
			speedX = 0;
		if (speedY<1 && speedY>-1)
			speedY = 0;
		
	}
	return (int)msg.wParam;
}


