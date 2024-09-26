#include "pch.h"
#include "playerInstance.h"
#include <chrono>
#include <thread>

playerInstance::playerInstance(const conFig& config) {
	this->config = config;
	this->hFfplay = NULL;
}

static BOOL CALLBACK EnumWindowsProc(_In_ HWND hwnd, _In_ LPARAM Lparam) {
	HWND hDefView = FindWindowEx(hwnd, 0, L"SHELLDLL_DefView", 0);
	if (hDefView != 0) {
		HWND hWorkerw = FindWindowEx(0, hwnd, L"WorkerW", 0);
		ShowWindow(hWorkerw, SW_HIDE);
		return FALSE;
	}
	return TRUE;
}

HWND findWindowTimeOut(const wchar_t* name, int timeoutMillis) {
	auto start = std::chrono::steady_clock::now();
	HWND hwnd = nullptr;

	while (true) {
		// ʹ�� FindWindowW ���Ҵ���
		hwnd = FindWindowW(name, 0);

		// ����ҵ����ڣ��򷵻�
		if (hwnd != nullptr) {
			return hwnd;
		}

		// ���㵱ǰ������ʱ��
		auto now = std::chrono::steady_clock::now();
		int elapsedMillis = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();

		// �����ʱ���򷵻� nullptr
		if (elapsedMillis > timeoutMillis) {
			return nullptr;
		}

		// �ȴ�һ��ʱ�䣨����100���룩�ٳ��Բ���
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

BOOL playerInstance::showWindow(LPCWSTR lpParameter) {
	if (this->hFfplay != NULL) {
		DWORD dwPID = 0;
		GetWindowThreadProcessId(hFfplay, &dwPID);
		char strCmd[MAX_PATH] = { 0 };
		sprintf_s(strCmd, "taskkill /pid %d -f", dwPID);
		system(strCmd);
	}
	STARTUPINFO si{ 0 };
	//si.dwFlags = STARTF_USESHOWWINDOW;
	//si.wShowWindow = SW_HIDE;
	PROCESS_INFORMATION pi{ 0 };
	if (CreateProcess(this->config.ffpath.c_str(), (LPWSTR)lpParameter, 0, 0, 0, CREATE_NO_WINDOW, 0, 0, &si, &pi)) {
		//Sleep(600);//�ȴ���Ƶ�������������
		//HWND hProgman = FindWindow(L"Progman", 0);// �ҵ�PI����
		HWND hProgman = findWindowTimeOut(L"Progman",2000);
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		if (hProgman == nullptr) return FALSE;
		SendMessageTimeout(hProgman, 0x052c, 0, 0, 0, 300, 0);// ������������Ϣ
		//this->hFfplay = FindWindowW(L"SDL_app", 0);// �ҵ���Ƶ����
		this->hFfplay = findWindowTimeOut(L"SDL_app", 2000);
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		if (this->hFfplay == nullptr) return FALSE;
		SetParent(hFfplay, hProgman);// ����Ƶ�������ΪPM���Ӵ���
		int systemWidth = GetSystemMetrics(0);
		int systemHeight = GetSystemMetrics(1);
		RECT cRct;
		GetWindowRect(hFfplay, &cRct);
		//if (horw) {
			int width = cRct.right - cRct.left;
			int x = (systemWidth - width) / 2;
			MoveWindow(hFfplay, 0, 0, systemWidth, systemHeight, 1);
		/* }
		else {
			int height = cRct.bottom - cRct.top;
			int x = (sheight - height) / 2;
			MoveWindow(hFfplay, 0, x, cRct.right - cRct.left, cRct.bottom - cRct.top, 0);
		}*/
		EnumWindows(EnumWindowsProc, 0);// �ҵ��ڶ���workerw���ڲ�������
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return TRUE;
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return FALSE;
}

BOOL playerInstance::generate() {
	std::wstring fcmd = L" \"";
	fcmd += this->config.videoPath + L"\"";
	fcmd += L" -noborder -loop 0";
	if (this->config.mute) {
		fcmd += L" -an";
	}
	fcmd += L" -fs";
	return this->showWindow(fcmd.c_str());
}

void playerInstance::exit() {
	if (this->hFfplay != NULL) {
		DWORD dwPID = 0;
		GetWindowThreadProcessId(this->hFfplay, &dwPID);
		char strCmd[MAX_PATH] = { 0 };
		sprintf_s(strCmd, "taskkill /pid %d -f", dwPID);
		system(strCmd);
	}
}