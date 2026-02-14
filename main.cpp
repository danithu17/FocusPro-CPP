#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <dwmapi.h>
#include <string>
#include <vector>
#include <shellapi.h>

#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "uxtheme.lib")

// Global UI Handles
HWND hTimer, hBtn, hEdit, hStreakText;
int timeLeft = 25 * 60;
bool isRunning = false;
int focusStreak = 0;

// GitHub SDK එකට ගැලපෙන විදිහට Glass effect එක හදමු
void ApplyGlassEffect(HWND hwnd) {
    BOOL USE_DARK_MODE = TRUE;
    DwmSetWindowAttribute(hwnd, 20, &USE_DARK_MODE, sizeof(USE_DARK_MODE));
    
    // Windows 11 Rounded Corners (Compatibility safe)
    DWORD corner = 2; // DWMWCP_ROUND
    DwmSetWindowAttribute(hwnd, 33, &corner, sizeof(corner));
}

void KillAppSilent(std::wstring processName) {
    std::wstring cmd = L"/C taskkill /F /IM " + processName + L" > nul 2>&1";
    ShellExecuteW(NULL, L"open", L"cmd.exe", cmd.c_str(), NULL, SW_HIDE);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            ApplyGlassEffect(hwnd);
            
            HFONT hFontMain = CreateFontW(75, 0, 0, 0, FW_THIN, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI Variable Display");
            HFONT hFontSub = CreateFontW(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI Variable Text");

            // Timer display
            hTimer = CreateWindowExW(0, L"STATIC", L"25:00", WS_VISIBLE | WS_CHILD | SS_CENTER, 0, 40, 450, 90, hwnd, NULL, NULL, NULL);
            SendMessage(hTimer, WM_SETFONT, (WPARAM)hFontMain, TRUE);

            // App selection
            CreateWindowExW(0, L"STATIC", L"Enter process to block (e.g. chrome.exe):", WS_VISIBLE | WS_CHILD, 50, 150, 350, 25, hwnd, NULL, NULL, NULL);
            hEdit = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"chrome.exe", WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL, 50, 180, 350, 35, hwnd, NULL, NULL, NULL);
            SendMessage(hEdit, WM_SETFONT, (WPARAM)hFontSub, TRUE);

            // Streak Text
            std::wstring streakStr = L"🔥 Focus Streak: " + std::to_wstring(focusStreak) + L" Sessions";
            hStreakText = CreateWindowExW(0, L"STATIC", streakStr.c_str(), WS_VISIBLE | WS_CHILD | SS_CENTER, 50, 240, 350, 25, hwnd, NULL, NULL, NULL);
            SendMessage(hStreakText, WM_SETFONT, (WPARAM)hFontSub, TRUE);

            // Start Button
            hBtn = CreateWindowExW(0, L"BUTTON", L"START FOCUS", WS_VISIBLE | WS_CHILD | BS_FLAT, 125, 300, 200, 50, hwnd, (HMENU)1, NULL, NULL);
            SendMessage(hBtn, WM_SETFONT, (WPARAM)hFontSub, TRUE);
            break;
        }

        case WM_COMMAND:
            if (LOWORD(wParam) == 1) {
                isRunning = !isRunning;
                SetWindowTextW(hBtn, isRunning ? L"END SESSION" : L"START FOCUS");
                if (isRunning) SetTimer(hwnd, 1, 1000, NULL);
                else KillTimer(hwnd, 1);
            }
            break;

        case WM_TIMER: {
            if (timeLeft > 0) {
                timeLeft--;
                int m = timeLeft / 60;
                int s = timeLeft % 60;
                std::wstring timeStr = (m < 10 ? L"0" : L"") + std::to_wstring(m) + L":" + (s < 10 ? L"0" : L"") + std::to_wstring(s);
                SetWindowTextW(hTimer, timeStr.c_str());

                wchar_t buffer[256];
                GetWindowTextW(hEdit, buffer, 256);
                KillAppSilent(buffer);
            } else {
                isRunning = false;
                KillTimer(hwnd, 1);
                focusStreak++;
                MessageBoxW(hwnd, L"Session Complete! Badge Earned: Focus Master 🏆", L"Apple Experience", MB_OK | MB_ICONINFORMATION);
                timeLeft = 25 * 60; // Reset
                UpdateWindow(hwnd);
            }
            break;
        }

        case WM_CTLCOLORSTATIC: {
            HDC hdcStatic = (HDC)wParam;
            SetTextColor(hdcStatic, RGB(45, 45, 45));
            SetBkMode(hdcStatic, TRANSPARENT);
            return (LRESULT)GetStockObject(WHITE_BRUSH);
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"AppleFocusUltra";
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClassW(&wc);
    HWND hwnd = CreateWindowExW(0, CLASS_NAME, L"FocusPro Ultra", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 465, 420, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}