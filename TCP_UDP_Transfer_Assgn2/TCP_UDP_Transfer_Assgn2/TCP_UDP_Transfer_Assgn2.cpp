/*--------------------------------------------------------------------------------------------------------------------
--	SOURCE: Assignment1Term4SPII.cpp
--
--	PROGRAM : Raw Terminal Input through Forks and Pipes
--
--	FUNCTIONS :
--		int main()
--		void process_input(int rawp[2], int transp[2], pid_t * children)
--		void translate_input(int frmtp[2], int transp[2])
--		void print_output(int rawp[2], int frmtp[2])
--		void fatal_error(char * error)
--
--	DATE: January 20, 2014
--	REVISIONS : none
--
--	DESIGNER : Ramzi Chennafi
--  PROGRAMMER : Ramzi Chennafi
--
--	NOTES :
--	A program which disables terminal text processing and handles it instead.Several methods of character input have
--	changed, as well as methods of terminating the program.Prints out one line of raw input, then upon typing "E",
--	prints out a formatted line of text.All 'a' characters are changed to 'z', all 'z' characters are changed to 'a',
--	and control letters such as X, E, K and T are not printed in the formatted text.
--
--
----------------------------------------------------------------------------------------------------------------------*/
#include "stdafx.h"
#include "TCP_UDP_Transfer_Assgn2.h"

// Global Variables:
#define MAX_LOADSTRING 100
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
WPARAM glbwParam;
LPARAM glblParam;
static int mode;
/*------------------------------------------------------------------------------------------------------------------
--      FUNCTION: main
--
--      DATE: January 20, 2014
--      REVISIONS: none
--
--      DESIGNER: Ramzi Chennafi
--      PROGRAMMER: Ramzi Chennafi
--
--      INTERFACE: void main(void)
--
--      RETURNS: void
--
--      NOTES:
--      Program entry point. Disables terminal processing, creates 3 pipes and 2 children processes. These processes
--		are directed into their respective function paths.
----------------------------------------------------------------------------------------------------------------------*/
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TCP_UDP_TRANSFER_ASSGN2, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TCP_UDP_TRANSFER_ASSGN2));

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}
/*------------------------------------------------------------------------------------------------------------------
--      FUNCTION: main
--
--      DATE: January 20, 2014
--      REVISIONS: none
--
--      DESIGNER: Ramzi Chennafi
--      PROGRAMMER: Ramzi Chennafi
--
--      INTERFACE: void main(void)
--
--      RETURNS: void
--
--      NOTES:
--      Program entry point. Disables terminal processing, creates 3 pipes and 2 children processes. These processes
--		are directed into their respective function paths.
----------------------------------------------------------------------------------------------------------------------*/
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = sizeof(SETTINGS) + (sizeof(char) * MAX_SIZE);
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TCP_UDP_TRANSFER_ASSGN2));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(60, 60, 60));
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_TCP_UDP_TRANSFER_ASSGN2);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}
/*------------------------------------------------------------------------------------------------------------------
--      FUNCTION: main
--
--      DATE: January 20, 2014
--      REVISIONS: none
--
--      DESIGNER: Ramzi Chennafi
--      PROGRAMMER: Ramzi Chennafi
--
--      INTERFACE: void main(void)
--
--      RETURNS: void
--
--      NOTES:
--      Program entry point. Disables terminal processing, creates 3 pipes and 2 children processes. These processes
--		are directed into their respective function paths.
----------------------------------------------------------------------------------------------------------------------*/
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance;

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 940, 600, NULL, NULL, hInstance, NULL);

	if (!hWnd)
		return FALSE;
	 
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}
/*------------------------------------------------------------------------------------------------------------------
--      FUNCTION: main
--
--      DATE: January 20, 2014
--      REVISIONS: none
--
--      DESIGNER: Ramzi Chennafi
--      PROGRAMMER: Ramzi Chennafi
--
--      INTERFACE: void main(void)
--
--      RETURNS: void
--
--      NOTES:
--      Program entry point. Disables terminal processing, creates 3 pipes and 2 children processes. These processes
--		are directed into their respective function paths.
----------------------------------------------------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	glblParam = lParam;
	glbwParam = wParam;

	switch (message){
		HANDLE_MSG(hWnd, WM_CREATE, Main_OnCreate);
		HANDLE_MSG(hWnd, WM_COMMAND, Main_OnCommand);
		HANDLE_MSG(hWnd, WM_PAINT, Main_OnPaint);
		HANDLE_MSG(hWnd, WM_DESTROY, Main_OnDestroy);
		case WM_SOCKET:
			socket_event(hWnd, wParam, lParam);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			
	}
}
/*------------------------------------------------------------------------------------------------------------------
--      FUNCTION: main
--
--      DATE: January 20, 2014
--      REVISIONS: none
--
--      DESIGNER: Ramzi Chennafi
--      PROGRAMMER: Ramzi Chennafi
--
--      INTERFACE: void main(void)
--
--      RETURNS: void
--
--      NOTES:
--      Program entry point. Disables terminal processing, creates 3 pipes and 2 children processes. These processes
--		are directed into their respective function paths.
----------------------------------------------------------------------------------------------------------------------*/
BOOL Main_OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct){
	
	SETTINGS * st = (SETTINGS*) malloc(sizeof(SETTINGS));
	st->client_port = "5150";
	st->server_port = "5150";
	st->protocol = 0;
	st->packet_size = 0;
	st->times_to_send = "1";
	st->client_send_ip = "127.0.0.1";
	SetClassLongPtr(hwnd, 0, (LONG)st);

	DrawDisplay(hwnd);
	DrawButtons(hwnd);
	UpdateWindow(hwnd);
	
	return TRUE;
}
/*------------------------------------------------------------------------------------------------------------------
--      FUNCTION: main
--
--      DATE: January 20, 2014
--      REVISIONS: none
--
--      DESIGNER: Ramzi Chennafi
--      PROGRAMMER: Ramzi Chennafi
--
--      INTERFACE: void main(void)
--
--      RETURNS: void
--
--      NOTES:
--      Program entry point. Disables terminal processing, creates 3 pipes and 2 children processes. These processes
--		are directed into their respective function paths.
----------------------------------------------------------------------------------------------------------------------*/
void Main_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify){
	
	SETTINGS * st = (SETTINGS*)GetClassLongPtr(hwnd, 0);

	switch (id)
	{
	case BT_SEND:
		write_data(hwnd, glbwParam, glblParam);
		activity("Sending data.\n", EB_STATUSBOX);
		break;
	case BT_DISCONNECT:
		if (mode == CLIENT){
			disconnect(hwnd);
			activity("Connection disconnected with server.\n", EB_STATUSBOX);
		}
		if (mode == SERVER){
			FreeSocketInformation(st->server_socket);
			activity("Connection disconnected.", EB_STATUSBOX);
		}
		break;
	case BT_CONNECT:
		client_connect(hwnd);
		activity("Connected to server.\n", EB_STATUSBOX);
		break;
	case ID_FILE_SETTINGS:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_SETTINGS), hwnd, Settings);
		break;
	case ID_CONNECT_SERVERMODE:
		init_server(hwnd);
		mode = SERVER;
		activity("Server mode intialized.\n", EB_STATUSBOX);
		break;
	case ID_CONNECT_CLIENTMODE:
		init_client(hwnd);
		mode = CLIENT;
		activity("Client mode intialized.\n", EB_STATUSBOX);
		break;
	case ID_HELP_README:
		break;
	case ID_HELP_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);
		break;
	case IDM_EXIT:
		DestroyWindow(hwnd);
		break;
	}
}
/*------------------------------------------------------------------------------------------------------------------
--      FUNCTION: main
--
--      DATE: January 20, 2014
--      REVISIONS: none
--
--      DESIGNER: Ramzi Chennafi
--      PROGRAMMER: Ramzi Chennafi
--
--      INTERFACE: void main(void)
--
--      RETURNS: void
--
--      NOTES:
--      Program entry point. Disables terminal processing, creates 3 pipes and 2 children processes. These processes
--		are directed into their respective function paths.
----------------------------------------------------------------------------------------------------------------------*/
void Main_OnPaint(HWND hwnd){
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	DrawTitleText(hdc);
	EndPaint(hwnd, &ps);
}
/*------------------------------------------------------------------------------------------------------------------
--      FUNCTION: main
--
--      DATE: January 20, 2014
--      REVISIONS: none
--
--      DESIGNER: Ramzi Chennafi
--      PROGRAMMER: Ramzi Chennafi
--
--      INTERFACE: void main(void)
--
--      RETURNS: void
--
--      NOTES:
--      Program entry point. Disables terminal processing, creates 3 pipes and 2 children processes. These processes
--		are directed into their respective function paths.
----------------------------------------------------------------------------------------------------------------------*/
void Main_OnDestroy(HWND hwnd){
	PostQuitMessage(0);
}
/*------------------------------------------------------------------------------------------------------------------
--      FUNCTION: main
--
--      DATE: January 20, 2014
--      REVISIONS: none
--
--      DESIGNER: Ramzi Chennafi
--      PROGRAMMER: Ramzi Chennafi
--
--      INTERFACE: void main(void)
--
--      RETURNS: void
--
--      NOTES:
--      Program entry point. Disables terminal processing, creates 3 pipes and 2 children processes. These processes
--		are directed into their respective function paths.
----------------------------------------------------------------------------------------------------------------------*/
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}
/*------------------------------------------------------------------------------------------------------------------
--      FUNCTION: main
--
--      DATE: January 20, 2014
--      REVISIONS: none
--
--      DESIGNER: Ramzi Chennafi
--      PROGRAMMER: Ramzi Chennafi
--
--      INTERFACE: void main(void)
--
--      RETURNS: void
--
--      NOTES:
--      Program entry point. Disables terminal processing, creates 3 pipes and 2 children processes. These processes
--		are directed into their respective function paths.
----------------------------------------------------------------------------------------------------------------------*/
INT_PTR CALLBACK Settings(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		Init_Settings(hDlg);
		return TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case BT_SETTINGCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		case BT_SETTINGOK:
			set_settings(hDlg);			
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
	}
	return FALSE;
}