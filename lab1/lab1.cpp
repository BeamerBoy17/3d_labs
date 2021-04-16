#include "framework.h"

#include "lab1.h"
#include "Functions.h"

const int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);   //Разрешение экрана по вертикали
const int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);  //Разрешение экрана по горизонтали

const int Space = 100;  //Отступ от краев экрана

const int FormWidth = ScreenWidth - 2 * Space;    //Ширина формы
const int FormHeight = ScreenHeight - 2 * Space;  //Высота формы

#define MAX_LOADSTRING 100


// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна



ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB1));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
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
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB1));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB1);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd;

    hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

    hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        Space, Space, FormWidth, FormHeight, NULL, NULL, hInstance, NULL);

    if (!hWnd) return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

const int Count = 7;

POINT3 WPT[Count]; //Мировые координаты углов пирамиды
POINT SPT[Count];  //Экранные координаты углов пирамиды

double R = 100; //Расстояние от точки наблюдения до начала мировой системы координат
double A = 90;  //Полярный угол в горизонтальной плоскости
double B = 0;   //Полярный угол в вертикальной плоскости
double d;       //Расстояние от точки наблюдения до экранной плоскости


//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработка меню приложения
//  WM_PAINT    -Закрасить главное окно
//  WM_DESTROY   - ввести сообщение о выходе и вернуться.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;

    //Вычисление координат пирамиды относительно размеров формы

    const int z = 500;
    const int w = FormWidth;
    const int h = FormHeight;

    WPT[0] = PointEx(Round(w / 2), Round(h / 10), z);
    WPT[1] = PointEx(Round(w / 3), Round(h / 2), z);
    WPT[2] = PointEx(Round(w / 3) + (Round(w / 2) - Round(w / 3)) * 2, Round(h / 2), z);
    WPT[3] = PointEx(w - Round(w / 8), h - Round(h / 3), z);
    WPT[4] = PointEx(w - Round(w / 3), h - Round(h / 6), z);
    WPT[5] = PointEx(Round(w / 3), h - Round(h / 6), z);
    WPT[6] = PointEx(Round(w / 8), h - Round(h / 3), z);

    int size_img = FormWidth * FormHeight; //Размер изображения
    int size_object = (WPT[3].x - WPT[6].x) * (WPT[4].y - WPT[0].y); //Размер объекта
    d = R * ((double)size_img / (double)size_object);

    const double RotateAngle = 1;

    switch (message)
    {
    case WM_COMMAND:
        wmId = LOWORD(wParam);
        wmEvent = HIWORD(wParam);
        // Разобрать выбор в меню:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case 'A':
        {
            A += RotateAngle;
            InvalidateRect(hWnd, NULL, TRUE);
            UpdateWindow(hWnd);
        } break;
        case 'D':
        {
            A -= RotateAngle;
            InvalidateRect(hWnd, NULL, TRUE);
            UpdateWindow(hWnd);
        }
        case 'W':
        {
            B += RotateAngle;
            InvalidateRect(hWnd, NULL, TRUE);
            UpdateWindow(hWnd);
        } break;
        case 'S':
        {
            B -= RotateAngle;
            InvalidateRect(hWnd, NULL, TRUE);
            UpdateWindow(hWnd);
        } break;
        case 0x31:
        {
            R += R / 5;
            InvalidateRect(hWnd, NULL, TRUE);
            UpdateWindow(hWnd);
        } break;
        case 0x32:
        {
            R -= R / 5;
            InvalidateRect(hWnd, NULL, TRUE);
            UpdateWindow(hWnd);
        } break;
        case 0x33:
        {
            d += d / 5;
            InvalidateRect(hWnd, NULL, TRUE);
            UpdateWindow(hWnd);
        } break;
        case 0x34:
        {
            d -= d / 5;
            InvalidateRect(hWnd, NULL, TRUE);
            UpdateWindow(hWnd);
        } break;
        }
    } break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        // TODO: добавьте любой код отрисовки...
        for (int i = 0; i < Count; i++)
        {
            POINT3 EPT = GlobalPointToScreen(WPT[i], A, B, R);
            SPT[i] = Perspective(EPT, d);
        }
        Piramid(hdc, SPT, Count);
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}