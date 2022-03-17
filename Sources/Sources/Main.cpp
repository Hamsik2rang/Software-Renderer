#ifdef UNICODE
#pragma comment (linker, "/entry:wWinMainCRTStartup /subsystem:console")
#elif
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#include "framework.h"
#include "Main.h"
#include "./Core/Renderer/Renderer.h"
#include "./Core/Utility/InputManager.h"
#include "./Core/Renderer/ObjectManager.h"
#include "./Core/Utility/Random.hpp"

#define MAX_LOADSTRING 100

// Global Variables:
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HINSTANCE g_hInst;                              // current instance

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

Renderer* g_pRenderer;
Camera* g_pCamera;
ObjectManager* g_pObjectManager;

HWND g_hWnd;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SOURCES, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SOURCES));

    MSG msg;

    g_pRenderer = new Renderer(g_hWnd);
    g_pCamera = new Camera;
    g_pRenderer->SetCamera(g_pCamera);

    g_pObjectManager  = ObjectManager::GetInstance();
    g_pObjectManager->Load("TestCube.obj");
    g_pObjectManager->Load("TestModel.obj");

    std::vector<Vec3f> positions(1);
    for (int i = 0; i < positions.size(); i++)
    {
       // positions[i] = { Random::GetRandomReal(-20.0f, 20.0f),Random::GetRandomReal(-20.0f, 20.0f),Random::GetRandomReal(-10.0f, 10.0f) };
        positions[i] = { 0.0f, 0.0f, 0.0f };
    }
    for (int i = 0; i < positions.size(); i++)
    {
		RenderObject* testModel = new RenderObject;
		testModel->SetBuffer(g_pObjectManager->GetBufferAddress(1));
		testModel->m_position = positions[i];
		g_pRenderer->AddModel(testModel);
    }
    // Main Loop
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
        else
        {
            g_pRenderer->Render();
        }
    }

    if (g_pRenderer)
    {
        delete g_pRenderer;
        g_pRenderer = nullptr;
    }
    if (g_pCamera)
    {
        delete g_pCamera;
        g_pCamera = nullptr;
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SOURCES));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SOURCES);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   g_hInst = hInstance; // Store instance handle in our global variable

   g_hWnd = CreateWindowW(szWindowClass, szTitle, WS_CAPTION | WS_SYSMENU,
      CW_USEDEFAULT, 0, 960, 720, nullptr, nullptr, hInstance, nullptr);

   if (!g_hWnd)
   {
      return FALSE;
   }
   ShowWindow(g_hWnd, nCmdShow);
   UpdateWindow(g_hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_KEYDOWN:
        {
            InputManager::GetInstance()->KeyPress(wParam);
            if (wParam == 'T')
            {
                g_pRenderer->m_bWireFrame ^= true;
            }
        }
        break;
    case WM_KEYUP:
        {
            InputManager::GetInstance()->KeyRelease(wParam);
        }
        break;
    case WM_MOUSEMOVE:
        {
            InputManager::GetInstance()->MouseMove();
        }
        break;
    case WM_MOVE:
        {
            if(g_pRenderer)
				g_pRenderer->UpdateWindowPos();
        }
        break;
    case WM_RBUTTONUP:
        {
            g_pCamera->SetIgnoreInput();
            InputManager::GetInstance()->ShowCursor();
        }
        break;
    case WM_RBUTTONDOWN:
        {
            g_pCamera->UnsetIgnoreInput();
            InputManager::GetInstance()->HideCursor();
            InputManager::GetInstance()->SetCursorToCenter();
        }
        break;
    case WM_SIZE:
        {
            //Caution. not be implemented yet.
            if(g_pRenderer)
				g_pRenderer->UpdateWindowSize();
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
