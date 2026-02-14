#include <windows.h>
#include <string>
#include <vector>

// Global Variables
HWND hTimerText, hBtn;
int timeLeft = 25 * 60; // 25 Minutes
bool isRunning = false;
std::vector<std::string> appsToBlock = {"chrome.exe", "msedge.exe", "discord.exe"};

// Function to kill apps
void KillApps() {
    for (const auto& app : appsToBlock) {
        std::string cmd = "taskkill /F /IM " + app + " > nul 2>&1";
        system(cmd.c_str());
    }
}

// Timer Logic
void UpdateTimerDisplay() {
    int m = timeLeft / 60;
    int s = timeLeft % 60;
    std::wstring timeStr = (m < 10 ? L"0" : L"") + std::to_wstring(m) + L":" + (s < 10 ? L"0" : L"") + std::to_wstring(s);
    SetWindowTextW(hTimerText, timeStr.c_str());
}

// Window Process logic
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            // Timer Text Label
            hTimerText = CreateWindowW(L"STATIC", L"25:00", WS_VISIBLE | WS_CHILD | SS_CENTER, 50, 50, 200, 50, hwnd, NULL, NULL, NULL);
            // Start/Stop Button
            hBtn = CreateWindowW(L"BUTTON", L"START FOCUS", WS_VISIBLE | WS_CHILD, 75, 120, 150, 40, hwnd, (HMENU)1, NULL, NULL);
            
            // Set Font to Arial
            HFONT hFont;
            hFont = CreateFontW(40, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
            SendMessage(hTimerText, WM_SETFONT, (WPARAM)hFont, TRUE);
            break;

        case WM_COMMAND:
            if (LOWORD(wParam) == 1) { // Button clicked
                isRunning = !isRunning;
                SetWindowTextW(hBtn, isRunning ? L"STOP" : L"START FOCUS");
                if (isRunning) SetTimer(hwnd, 1, 1000, NULL);
                else KillTimer(hwnd, 1);
            }
            break;

        case WM_TIMER:
            if (timeLeft > 0) {
                timeLeft--;
                UpdateTimerDisplay();
                KillApps();
            } else {
                KillTimer(hwnd, 1);
                MessageBoxW(hwnd, L"Focus Session Finished!", L"FocusPro", MB_OK | MB_ICONINFORMATION);
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"FocusProWindowClass";
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClassW(&wc);
    HWND hwnd = CreateWindowExW(0, CLASS_NAME, L"FocusPro v4.0 (Native)", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 320, 250, NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) return 0;
    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}