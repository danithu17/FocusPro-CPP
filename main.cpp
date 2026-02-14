#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <dwmapi.h>
#include <string>
#include <vector>
#include <shellapi.h>

#pragma comment(lib, "dwmapi.lib")

// Global Data for Dashboard
int totalMinutesFocused = 0;
int sessionsCompleted = 0;
int timeLeft = 25 * 60;
bool isRunning = false;

HWND hTimer, hBtn, hEdit, hDashboard;

// Apple-style Focus Tones using Beep (Stability high)
void PlayFocusTone(bool start) {
    if (start) {
        Beep(700, 200); Beep(900, 300); // Startup melody
    } else {
        Beep(1000, 150); Beep(800, 150); Beep(600, 400); // Success melody
    }
}

void ApplyGlassEffect(HWND hwnd) {
    BOOL USE_DARK_MODE = TRUE;
    DwmSetWindowAttribute(hwnd, 20, &USE_DARK_MODE, sizeof(USE_DARK_MODE));
    DWORD corner = 2; 
    DwmSetWindowAttribute(hwnd, 33, &corner, sizeof(corner));
}

void KillAppSilent(std::wstring processName) {
    std::wstring cmd = L"/C taskkill /F /IM " + processName + L" > nul 2>&1";
    ShellExecuteW(NULL, L"open", L"cmd.exe", cmd.c_str(), NULL, SW_HIDE);
}

void UpdateDashboard(HWND hwnd) {
    std::wstring dbStr = L"📊 DASHBOARD\nFocus Time: " + std::to_wstring(totalMinutesFocused) + 
                         L" mins  |  Sessions: " + std::to_wstring(sessionsCompleted);
    SetWindowTextW(hDashboard, dbStr.c_str());
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            ApplyGlassEffect(hwnd);
            HFONT hFontMain = CreateFontW(80, 0, 0, 0, FW_THIN, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI Variable Display");
            HFONT hFontSub = CreateFontW(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI Variable Text");

            // Main Timer
            hTimer = CreateWindowExW(0, L"STATIC", L"25:00", WS_VISIBLE | WS_CHILD | SS_CENTER, 0, 30, 480, 100, hwnd, NULL, NULL, NULL);
            SendMessage(hTimer, WM_SETFONT, (WPARAM)hFontMain, TRUE);

            // Input Area
            CreateWindowExW(0, L"STATIC", L"Restrict App:", WS_VISIBLE | WS_CHILD, 60, 140, 150, 20, hwnd, NULL, NULL, NULL);
            hEdit = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"chrome.exe", WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL, 60, 165, 360, 35, hwnd, NULL, NULL, NULL);
            SendMessage(hEdit, WM_SETFONT, (WPARAM)hFontSub, TRUE);

            // Dashboard Area (Apple Card Style)
            hDashboard = CreateWindowExW(0, L"STATIC", L"📊 DASHBOARD\nFocus Time: 0 mins  |  Sessions: 0", WS_VISIBLE | WS_CHILD | SS_CENTER, 40, 220, 400, 60, hwnd, NULL, NULL, NULL);
            SendMessage(hDashboard, WM_SETFONT, (WPARAM)hFontSub, TRUE);

            // Start Button
            hBtn = CreateWindowExW(0, L"BUTTON", L"START FOCUS", WS_VISIBLE | WS_CHILD | BS_FLAT, 140, 310, 200, 50, hwnd, (HMENU)1, NULL, NULL);
            SendMessage(hBtn, WM_SETFONT, (WPARAM)hFontSub, TRUE);
            break;
        }

        case WM_COMMAND:
            if (LOWORD(wParam) == 1) {
                isRunning = !isRunning;
                if (isRunning) {
                    PlayFocusTone(true);
                    SetWindowTextW(hBtn, L"END SESSION");
                    SetTimer(hwnd, 1, 1000, NULL);
                } else {
                    KillTimer(hwnd, 1);
                    SetWindowTextW(hBtn, L"START FOCUS");
                }
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

                // Every minute, update dashboard
                if (timeLeft % 60 == 0 && timeLeft != 1500) {
                    totalMinutesFocused++;
                    UpdateDashboard(hwnd);
                }
            } else {
                isRunning = false;
                KillTimer(hwnd, 1);
                sessionsCompleted++;
                UpdateDashboard(hwnd);
                PlayFocusTone(false);
                MessageBoxW(hwnd, L"Focus session complete! 🏆 You're a Focus Master.", L"Apple FocusPro", MB_OK | MB_ICONINFORMATION);
                timeLeft = 25 * 60;
            }
            break;
        }

        case WM_CTLCOLORSTATIC: {
            HDC hdcStatic = (HDC)wParam;
            SetTextColor(hdcStatic, RGB(45, 45, 45));
            SetBkMode(hdcStatic, TRANSPARENT);
            return (LRESULT)GetStockObject(WHITE_BRUSH);
        }

        case WM_DESTROY: PostQuitMessage(0); break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSW wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"AppleFocusDashboard";
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClassW(&wc);
    HWND hwnd = CreateWindowExW(0, L"AppleFocusDashboard", L"FocusPro Dashboard", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 500, 420, NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, nCmdShow);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) { TranslateMessage(&msg); DispatchMessage(&msg); }
    return 0;
}