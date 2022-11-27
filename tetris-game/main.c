#include "main.h"
#include "entity.h"
#include "game-helper.h"
#include "draw.h"
#include "tetromino.h"

GameStatus currentGameStatus = GAME_OVER;
Tetromino currentTetromino;
int score;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TETRISGAME, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    initializateDrawCoordinates();

    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TETRISGAME));
    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TETRISGAME));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    int wndWight = (BLOCK_SIZE_IN_PIXEL * PLAY_FIELD_WIDTH_IN_BLOCKS) + (INDENT_IN_PIXELS * 2) + SUPPORT_FIELD_WIDTH_IN_PIXEL;
    int wndHeight = (BLOCK_SIZE_IN_PIXEL * PLAY_FIELD_HEIGHT_IN_BLOCKS) + (INDENT_IN_PIXELS * 4);

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME | WS_VISIBLE | WS_CAPTION,
        CW_USEDEFAULT, CW_USEDEFAULT, wndWight, wndHeight, NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int buttonX = PLAY_FIELD_WIDTH_IN_BLOCKS * BLOCK_SIZE_IN_PIXEL + INDENT_IN_PIXELS * 3;
    const RECT playFieldRect = { 0, 0, PLAY_FIELD_WIDTH_IN_BLOCKS * BLOCK_SIZE_IN_PIXEL + INDENT_IN_PIXELS, PLAY_FIELD_HEIGHT_IN_BLOCKS * BLOCK_SIZE_IN_PIXEL + INDENT_IN_PIXELS };

    switch (message)
    {
    case WM_CREATE:
        CreateWindowW(L"Button", L"Start Game", WS_VISIBLE | WS_CHILD, buttonX, 40, 85, 25, hWnd, (HMENU)ID_BUTTON_START, 0, NULL);
        hScore = CreateWindowW(L"static", L"Score: ", WS_VISIBLE | WS_CHILD | WS_BORDER, buttonX, 80, 85, 25, hWnd, (HMENU)ID_BUTTON_SCORE, 0, NULL);
        break;
    case WM_COMMAND:
        if (LOWORD(wParam) == ID_BUTTON_START)
        {
            initializeGame(&currentTetromino, &currentGameStatus, &score, hWnd);
            SetFocus(hWnd);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        drawWithBuffer(hdc);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_TIMER:
    {
        switch (currentGameStatus)
        {
        case PLAYING:
            downTetromino(&currentTetromino, &currentGameStatus, &score);
            updateScore(&score, hScore);
            break;
        case GAME_OVER:
            KillTimer(hWnd, SPEED);
            int ret = MessageBoxW(hWnd, L"Game Over! Do you want to play again?", L"Message", MB_YESNO);
            switch (ret)
            {
            case IDNO:
                SendMessage(hWnd, WM_CLOSE, 0, 0);
                break;
            case IDYES:
                SendMessage(hWnd, WM_COMMAND, (HMENU)ID_BUTTON_START, 0);
                break;
            }
            break;
        }
        InvalidateRect(hWnd, &playFieldRect, FALSE);
    }
    break;
    case WM_KEYDOWN:
        if (keyProc(hWnd, wParam) == TRUE)
        {
            InvalidateRect(hWnd, &playFieldRect, FALSE);
        }
        break;
    case WM_SIZE:
        switch (wParam)
        {
        case SIZE_MINIMIZED:
            KillTimer(hWnd, SPEED);
            break;
        case SIZE_RESTORED:
            if (currentGameStatus == PLAYING)
            {
                SetTimer(hWnd, SPEED, SPEED, NULL);
            }
            break;
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

BOOL keyProc(HWND hWnd, WPARAM wParam)
{
    if (currentGameStatus == GAME_OVER)
    {
        return FALSE;
    }

    switch (wParam)
    {
    case VK_RIGHT:
        moveTetromino(&currentTetromino, MOVE_TO_RIGHT);
        break;
    case VK_LEFT:
        moveTetromino(&currentTetromino, MOVE_TO_LEFT);
        break;
    case VK_DOWN:
        downTetromino(&currentTetromino, &currentGameStatus, &score);
        break;
    case VK_UP:
        rotateTetromino(&currentTetromino, CLOCKWISE);
        break;
    case VK_SPACE:
        rotateTetromino(&currentTetromino, COUNTER_CLOCKWISE);
        break;
    case VK_ESCAPE:
    {
        int ret = MessageBoxW(hWnd, L"Are you sure to quit?", L"Message", MB_OKCANCEL);
        if (ret == IDOK)
        {
            SendMessage(hWnd, WM_CLOSE, 0, 0);
        }
    }
    }
    return TRUE;
}