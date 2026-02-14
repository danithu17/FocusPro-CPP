#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <dwmapi.h> // Glass effect එකට
#include <string>
#include <vector>
#include <shellapi.h>

#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "uxtheme.lib")

// Global UI Handles
HWND hTimer, hBtn, hEdit;
int timeLeft = 25 * 60;
bool isRunning = false;
int focusStreak = 0; // Challenge feature

// Modern Glass/Acrylic Effect apply කරන function එක
void ApplyGlassEffect(HWND hwnd) {
    DWM_SYSTEM_BACKDROP_TYPE backdropType = DWMSBT_TRANSIENTBACKDROP; // Acrylic style
    DwmSetWindowAttribute(hwnd, DWMWA_SYSTEM_BACKDROP_TYPE, &backdropType, sizeof(backdropType));
    
    // Rounded Corners (Windows 11 look for Apple Feel)
    DWM_WINDOW_CORNER_PREFERENCE corner = DWMWCP_ROUND;
    DwmSetWindowAttribute(hwnd, DWMWA_WINDOW_CORNER_PREFERENCE, &corner, sizeof(corner));
}

// CMD එක පේන්නේ නැතුව Apps අයින් කරන විදිහ
void KillAppSilent(std::wstring processName) {
    std::wstring cmd = L"/C taskkill /F /IM " + processName + L" > nul 2>&1";
    ShellExecuteW(NULL, L"open", L"cmd.exe", cmd.c_str(), NULL, SW_HIDE);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            ApplyGlassEffect(hwnd);
            
            HFONT hFontMain = CreateFontW(80, 0, 0, 0, FW_THIN, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI Variable Display");
            HFONT hFontSub = CreateFontW(20, 0, 0, 0, FW_REGULAR, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI Variable Text");

            // Timer display
            hTimer = CreateWindowExW(0, L"STATIC", L"25:00", WS_VISIBLE | WS_CHILD | SS_CENTER, 0, 60, 450, 100, hwnd, NULL, NULL, NULL);
            SendMessage(hTimer, WM_SETFONT, (WPARAM)hFontMain, TRUE);

            // App Name Input (Apple Search Style)
            CreateWindowExW(0, L"STATIC", L"Type App to Block (e.g. chrome.exe):", WS_VISIBLE | WS_CHILD, 50, 180, 350, 25, hwnd, NULL, NULL, NULL);
            hEdit = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"chrome.exe", WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL, 50, 210, 350, 35, hwnd, NULL, NULL, NULL);
            SendMessage(hEdit, WM_SETFONT, (WPARAM)hFontSub, TRUE);

            // Streak/Challenge Text
            std::wstring streakStr = L"🔥 Current Focus Streak: " + std::to_wstring(focusStreak);
            HWND hStreak = CreateWindowExW(0, L"STATIC", streakStr.c_str(), WS_VISIBLE | WS_CHILD | SS_CENTER, 50, 270, 350, 25, hwnd, (HMENU)2, NULL, NULL);
            SendMessage(hStreak, WM_SETFONT, (WPARAM)hFontSub, TRUE);

            // Modern Rounded Button (We simulate with a Flat Button for now)
            hBtn = CreateWindowExW(0, L"BUTTON", L"START FOCUS", WS_VISIBLE | WS_CHILD | BS_FLAT, 125, 330, 200, 50, hwnd, (HMENU)1, NULL, NULL);
            SendMessage(hBtn, WM_SETFONT, (WPARAM)hFontSub, TRUE);
            break;
        }

        case WM_COMMAND:
            if (LOWORD(wParam) == 1) { // Start/Stop Button
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

                // App Blocking Logic (Silent)
                wchar_t buffer[256];
                GetWindowTextW(hEdit, buffer, 256);
                KillAppSilent(buffer);
            } else {
                isRunning = false;
                KillTimer(hwnd, 1);
                focusStreak++; // Increase challenge streak
                MessageBoxW(hwnd, L"Session Complete! Badge Earned: Focus Master 🏆", L"Apple Experience", MB_OK | MB_ICONINFORMATION);
            }
            break;
        }

        case WM_CTLCOLORSTATIC: {
            HDC hdcStatic = (HDC)wParam;
            SetTextColor(hdcStatic, RGB(30, 30, 30));
            SetBkMode(hdcStatic, TRANSPARENT);
            return (LRESULT)GetStockObject(NULL_BRUSH);
        }

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            // මෙතන අපිට ඕන නම් gradient එකක් අඳින්න පුළුවන් Apple style එක ගන්න
            EndPaint(hwnd, &ps);
            break;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"AppleUltraFocus";
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClassW(&wc);
    // Window size එක පොඩ්ඩක් ලොකු කළා modern පෙනුමට
    HWND hwnd = CreateWindowExW(WS_EX_NOREDIRECTIONBITMAP, CLASS_NAME, L"FocusPro Ultra", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 465, 480, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}