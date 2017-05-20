#include <windows.h>
#include <stdio.h>
#include <Shellapi.h>

void execAsAdmin(const char *file, bool addPause) {
	char tempDir[MAX_PATH], filePath[MAX_PATH], curDir[MAX_PATH];
	RPC_CSTR uuidStr;
	GUID guidReference;
	FILE *f;

	GetTempPathA(MAX_PATH, tempDir);
	CoCreateGuid(&guidReference);
	UuidToStringA(&guidReference, &uuidStr);
	sprintf(filePath, "%s\\OPEN%s.bat", tempDir, uuidStr);
	RpcStringFreeA(&uuidStr);
	GetCurrentDirectoryA(MAX_PATH, curDir);

	f = fopen(filePath, "wt");
	fprintf(f, "@cd \"%s\"\n", curDir);
	fprintf(f, "@%s\n", file);
	if (addPause)
		fputs("pause\n", f);
	fclose(f);

	ShellExecuteA(NULL, "runas", filePath, "", NULL, SW_SHOW);
	Sleep(100);
	DeleteFileA(filePath);
}

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	char *commandLine = lpCmdLine;
	bool addPause = false;
	if (*commandLine == '-')  {
		if (*++commandLine == ' ')
			commandLine++;
		addPause = true;
	} else if (*commandLine == '\0') {
		printf("Usage: sudo [-] COMMAND\n    With - prefix to show the output Window (for debugging).\n");
		return 0;
	}

	execAsAdmin(commandLine, addPause);
	return 0;
}
