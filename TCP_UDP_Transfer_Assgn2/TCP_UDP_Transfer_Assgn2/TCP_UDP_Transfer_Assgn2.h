#pragma once

#include "resource.h"
#define WM_SOCKET (WM_USER + 1)
#define MAX_SIZE 1024
#define PORTC 55011

typedef struct _SETTINGS{
	char* server_port;
	char* client_port;
	char* client_send_ip;
	SOCKET server_socket;
	int protocol;
	int packet_size;
	int mode;
	char* times_to_send;
	SOCKET client_socket;
}SETTINGS;

typedef struct _SOCKET_INFORMATION {
	WSABUF DataBuf;
	DWORD BytesSEND;
	DWORD BytesRECV;
	int packets;
	int packet_size;
	int total_size;
	int mode;
	int header_received;
	int totalRecv;
} SOCKET_INFORMATION, *LPSOCKET_INFORMATION;

ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
// WndProc called functions
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void				Main_OnPaint(HWND hwnd);
void				Main_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void				Main_OnDestroy(HWND hwnd);
BOOL				Main_OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
// GUI setup functions
void				DrawButtons(HWND hwndParent);
void				DrawTitleText(HDC hdc);
void				DrawDisplay(HWND hwndParent);
void				SetFont(TCHAR* font, HWND hwnd, HWND* hwndButton, int buttons);

void				Init_Settings(HWND hwnd);
INT_PTR CALLBACK	Settings(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void				set_settings(HWND hwnd);

//server testing functions
void				init_server(HWND hwnd);
int					socket_event(HWND hwnd, WPARAM wParam, LPARAM lParam);
int					read_server_data(HWND hwnd, WPARAM wParam);
void accept_data(HWND hwnd, WPARAM wParam);

void     			init_client(HWND hwnd);
int					client_connect(HWND hwnd);
void				write_client_data(HWND hwnd, WPARAM wParam, LPARAM lParam);

LPSTR grab_file(HWND hwnd, HANDLE * hf);
void save_file(HWND hwnd, char * buffer, int size);
void activity(char * buffer, int box);
void disconnect(HWND hwnd);
void process_tcp_header(HWND hwnd, SOCKET recv);