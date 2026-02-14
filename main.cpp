#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <commctrl.h>
#include <string>
#include <vector>

#pragma comment(lib, "comctl32.lib")

// Global UI Handles
HWND hTimerText, hBtn, hList;
int timeLeft = 25 * 60;
bool isRunning = false;

struct BlockableApp {
    std::wstring name;
    std::string processName;
    bool shouldBlock;
};

std::vector<BlockableApp> appList = {
    {L"Google Chrome", "chrome.exe", false},
    {L"Microsoft Edge", "msedge.exe", false},
    {L"Discord App", "discord.exe", false},
    {L"Spotify", "spotify.exe", false}
};

// Function to block apps
void KillApps() {
    for (const auto& app : appList) {
        if (app.shouldBlock) {
            std::string cmd = "taskkill /F /IM " + app.processName + " > nul 2>&1";
            system(cmd.c_str());
        }
    }
}

void UpdateTimerDisplay() {
    int m = timeLeft / 60;
    int s = timeLeft % 60;
    std::wstring timeStr = (m < 10 ? L"0" : L"") + std::to_wstring(m) + L":" + (s < 10 ? L"0" : L"") + std::to_wstring(s);
    SetWindowTextW(hTimerText, timeStr.c_str());
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            // Apple Style Fonts
            HFONT hFontMain = CreateFontW(70, 0, 0, 0, FW_LIGHT, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI Variable Display");
            HFONT hFontSmall = CreateFontW(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");

            // Timer Display (Big and Minimal)
            hTimerText = CreateWindowExW(0, L"STATIC", L"25:00", WS_VISIBLE | WS_CHILD | SS_CENTER, 0, 40, 400, 100, hwnd, NULL, NULL, NULL);
            SendMessage(hTimerText, WM_SETFONT, (WPARAM)hFontMain, TRUE);

            // Instructions
            HWND hInfo = CreateWindowExW(0, L"STATIC", L"Select apps to restrict:", WS_VISIBLE | WS_CHILD, 35, 150, 330, 20, hwnd, NULL, NULL, NULL);
            SendMessage(hInfo, WM_SETFONT, (WPARAM)hFontSmall, TRUE);

            // App List (Checkbox List)
            hList = CreateWindowExW(0, WC_LISTVIEWW, L"", WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT | LVS_NOCOLUMNHEADER, 35, 180, 330, 120, hwnd, NULL, NULL, NULL);
            ListView_SetExtendedListViewStyle(hList, LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);
            
            LVCOLUMNW lvc = {0};
            lvc.mask = LVCF_WIDTH;
            lvc.cx = 310;
            ListView_InsertColumn(hList, 0, &lvc);

            for (int i = 0; i < (int)appList.size(); i++) {
                LVITEMW lvi = {0};
                lvi.mask = LVIF_TEXT;
                lvi.iItem = i;
                lvi.pszText = (LPWSTR)appList[i].name.c_str();
                ListView_InsertItem(hList, &lvi);
            }

            // Start Button
            hBtn = CreateWindowExW(0, L"BUTTON", L"Start Focus", WS_VISIBLE | WS_CHILD | BS_FLAT, 125, 320, 150, 45, hwnd, (HMENU)1, NULL, NULL);
            SendMessage(hBtn, WM_SETFONT, (WPARAM)hFontSmall, TRUE);
            break;
        }

        case WM_COMMAND:
            if (LOWORD(wParam) == 1) {
                if (!isRunning) {
                    for (int i = 0; i < (int)appList.size(); i++) {
                        appList[i].shouldBlock = ListView_GetCheckState(hList, i);
                    }
                    isRunning = true;
                    SetWindowTextW(hBtn, L"End Session");
                    SetTimer(hwnd, 1, 1000, NULL);
                } else {
                    isRunning = false;
                    KillTimer(hwnd, 1);
                    SetWindowTextW(hBtn, L"Start Focus");
                }
            }
            break;

        case WM_TIMER:
            if (timeLeft > 0) {
                timeLeft--;
                UpdateTimerDisplay();
                KillApps();
            }
            break;

        case WM_CTLCOLORSTATIC: {
            HDC hdcStatic = (HDC)wParam;
            SetTextColor(hdcStatic, RGB(45, 45, 45));
            SetBkColor(hdcStatic, RGB(255, 255, 255));
            return (LRESULT)GetStockObject(WHITE_BRUSH);
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"AppleFocusClass";
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClassW(&wc);
    HWND hwnd = CreateWindowExW(0, CLASS_NAME, L"FocusPro", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 415, 430, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}