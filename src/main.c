#include <windows.h>
#include "resource.h"

#define TIMER_ID 1
#define MOUSE_MOVE_INTERVAL  120000 // 2 minutes in milliseconds 
#define WM_TRAYICON (WM_USER + 1)
#define ID_TRAY_EXIT 1001
#define ID_TRAY_ABOUT 1002

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            SetTimer(hwnd, TIMER_ID, MOUSE_MOVE_INTERVAL, NULL);
            break;
        case WM_TIMER:
            if (wParam == TIMER_ID) {
                POINT p;
                GetCursorPos(&p);
                SetCursorPos(p.x + 1, p.y);
            }
            break;
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case ID_TRAY_EXIT:
                    DestroyWindow(hwnd);
                    break;
                case ID_TRAY_ABOUT:
                    MessageBox(hwnd, "Mouse mover by Lucas Neves e Copilot\nCIT SENAC 2025", "   Sobre", MB_OK | MB_ICONINFORMATION);
                    break;
            }
            break;
        case WM_TRAYICON:
            if (lParam == WM_RBUTTONUP) {
                POINT p;
                GetCursorPos(&p);
                HMENU hMenu = CreatePopupMenu();
                InsertMenu(hMenu, 0, MF_BYPOSITION | MF_STRING, ID_TRAY_ABOUT, "Sobre");
                InsertMenu(hMenu, 1, MF_BYPOSITION | MF_STRING, ID_TRAY_EXIT, "Exit");
                SetForegroundWindow(hwnd);
                TrackPopupMenu(hMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, p.x, p.y, 0, hwnd, NULL);
                DestroyMenu(hMenu);
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    HWND hwnd;
    MSG msg;
    WNDCLASS wc = {0};
    NOTIFYICONDATA nid = {0};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "MouseMoverClass";
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

    RegisterClass(&wc);

    hwnd = CreateWindowEx(
        0,
        "MouseMoverClass",
        "Mouse Mover",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, hInstance, NULL
    );

    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = IDI_ICON1;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_TRAYICON;
    nid.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    strcpy(nid.szTip, "Mouse Mover");

    Shell_NotifyIcon(NIM_ADD, &nid);

    ShowWindow(hwnd, SW_HIDE);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    Shell_NotifyIcon(NIM_DELETE, &nid);

    return 0;
}