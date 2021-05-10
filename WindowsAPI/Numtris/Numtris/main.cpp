#include <iostream>
#include <random>
#include <time.h>
#include <string.h>
#include <string>
#include "header.h"
#include "Numtris.h"

#define MAX_LOADSTRING 100

//#define X_BLOCKS 5
//#define Y_BLOCKS 6

#define DOWNSPEED 5

enum BLOCK_NUM_TYPE {
	A = 2, B = 4, C = 8, D = 16, E = 32, F = 64, G = 128
} block_num_type;

enum DIRECT {
	LEFT = 101, TOP = 102, RIGHT = 103, BOTTOM = 104
} direct;

enum BLOCK_STATE {
	IDLE = 111, DROP = 112, CHECK = 113
} block_state;


typedef struct Pos
{
	int x; int y;
}DOT;

typedef struct Size
{
	int witdh = 0; int height = 0;
};

RECT NewSetRect(RECT rect, Pos _pos, int witdh, int height)
{
	rect.left = _pos.x - witdh / 2;
	rect.right = _pos.x + witdh / 2;
	rect.top = _pos.y - height / 2;
	rect.bottom = _pos.y + height / 2;

	return rect;
}

struct Block
{
	//public:
	Pos		center_pos = { 0,0 };
	RECT	rect;
	int		num = 0;
	int		state = IDLE;

	int SetBlockNumType()
	{
		int value = rand() % 100;

		if (value < 20)			num = A;
		else if (value < 40)	num = B;
		else if (value < 60)	num = C;
		else if (value < 75)	num = D;
		else if (value < 90)	num = E;
		else if (value < 95)	num = F;
		else if (value < 100)	num = G;

		return num;
	}

	void BlockMove(int _direct)
	{
		int witdh = rect.right - rect.left;
		int height = rect.bottom - rect.top;
		center_pos.x = rect.right - witdh / 2;
		center_pos.y = rect.bottom - height / 2;

		if (_direct == LEFT)		center_pos.x -= witdh;
		else if (_direct == RIGHT)	center_pos.x += witdh;

		rect = NewSetRect(rect, { center_pos.x, center_pos.y }, witdh, height);
	}

	void BlockDown(int speed = DOWNSPEED)
	{
		int witdh = rect.right - rect.left;
		int height = rect.bottom - rect.top;
		center_pos.x = rect.right - witdh / 2;
		center_pos.y = rect.bottom - height / 2 + speed;

		rect = NewSetRect(rect, { center_pos.x, center_pos.y }, witdh, height);
	}
};


class GameBoard
{
	Pos		center_pos;
	RECT	rect;
	RECT	rect_game_board;

	Size	block_size;
	int		count = 0;
	Pos		MAX_BLOCKS = { 5,6 };

public:
	// ★ 블록을 list 형태로 사용
	Block	rect_game_block[63] = {};

	void SetGameBoard(Pos _pos, int witdh, int height)
	{
		rect_game_board = NewSetRect(rect_game_board, _pos, witdh, height);

		block_size.witdh = witdh / MAX_BLOCKS.x;
		block_size.height = height / MAX_BLOCKS.y;

	}
	void SetMaxBlocks(int x = 5, int y = 6) { MAX_BLOCKS.x = x; MAX_BLOCKS.y = y; }


	RECT	GetGameBoard() { return rect_game_board; };
	Size	GetBlockSize() { return block_size; };
	Pos		GetMaxBlocks() { return MAX_BLOCKS; };
	int		GetCount() {
		return count;
	};

	//void DrawGameBoard();
	void CountUp(int v = 1) { count += v; }
};




static GameBoard Play_Board;


// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

												// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_NUMTRIS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_NUMTRIS));

	MSG msg;

	// 기본 메시지 루프입니다.
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NUMTRIS));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_NUMTRIS);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		200, 50, 600, 800, nullptr, nullptr, hInstance, nullptr);

	srand(time(NULL));

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}


//1번 타이머 :: 블록 생성
void CALLBACK BlockCreate(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime) 
{
	HDC hdc;
	hdc = GetDC(hWnd);

	// 생성 블록 렉트 세팅하기
	//for (int i = 0; i <= Play_Board.GetCount(); ++i)
	if (Play_Board.GetCount() < Play_Board.GetMaxBlocks().x * Play_Board.GetMaxBlocks().y) {
		Play_Board.rect_game_block[Play_Board.GetCount()].rect = NewSetRect(Play_Board.rect_game_block[Play_Board.GetCount()].rect,
			{ Play_Board.GetGameBoard().right / 2 + Play_Board.GetBlockSize().witdh / 2 + 10 , Play_Board.GetGameBoard().top - Play_Board.GetBlockSize().height / 2 },
			Play_Board.GetBlockSize().witdh,
			Play_Board.GetBlockSize().height);
		Play_Board.rect_game_block[Play_Board.GetCount()].SetBlockNumType();
	}

	Play_Board.CountUp();

	InvalidateRect(hWnd, NULL, TRUE);
	ReleaseDC(hWnd, hdc);
}

//2번 타이머 :: 블록 드랍
void CALLBACK BlockDrop(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime) 
{
	HDC hdc;
	hdc = GetDC(hWnd);


	for (int i = Play_Board.GetCount(); i >= 0; --i)
	{
		if (Play_Board.rect_game_block[Play_Board.GetCount()].rect.bottom < Play_Board.GetGameBoard().bottom) {
			if (i == Play_Board.GetCount())
			{
				Play_Board.rect_game_block[Play_Board.GetCount()].BlockDown(10);
				continue;
			}
			// 충돌
			if (Play_Board.rect_game_block[Play_Board.GetCount()].rect.bottom > Play_Board.rect_game_block[i].rect.top &&
				Play_Board.rect_game_block[Play_Board.GetCount()].center_pos.x > Play_Board.rect_game_block[i].rect.left &&
				Play_Board.rect_game_block[Play_Board.GetCount()].center_pos.x < Play_Board.rect_game_block[i].rect.right)
			{
				if (Play_Board.rect_game_block[Play_Board.GetCount()].num != Play_Board.rect_game_block[i].num)
				{
					Play_Board.CountUp();
					KillTimer(hWnd, 2);
				}
				else if (Play_Board.rect_game_block[Play_Board.GetCount()].num == Play_Board.rect_game_block[i].num) // 블럭끼리 합쳐짐
				{
					//Play_Board.CountUp();
					// now block 을 초기화
					Play_Board.rect_game_block[Play_Board.GetCount()].rect = NewSetRect(Play_Board.rect_game_block[Play_Board.GetCount()].rect, { 0,0 }, 0, 0);
					Play_Board.rect_game_block[Play_Board.GetCount()].num = 0;

					Play_Board.rect_game_block[i].num *= 2;

					// ★ 작성 해야할 내용 ::
					//		블럭을 Y_BLOCK 만큼 저장해 놓고 높이가 걸리는 곳 마다 합친다


					KillTimer(hWnd, 2);
				}
			}
		}
		else {
			Play_Board.CountUp();
			KillTimer(hWnd, 2);
		}
	}


	InvalidateRect(hWnd, NULL, TRUE);
	ReleaseDC(hWnd, hdc);
}



//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{



	switch (message)
	{
	case WM_CREATE:
	{
		Play_Board.SetGameBoard({ 300,400 }, 400, 500);
		//std::cout << Play_Board.GetGameBoard().bottom;


	}

	break;
	case WM_KEYDOWN:
	{
		//HDC hdc = GetDC(hWnd);

		if (wParam == VK_RETURN) {
			//SetTimer(hWnd, 1, 1000, BlockCreate);

			if (Play_Board.GetCount() < Play_Board.GetMaxBlocks().x * Play_Board.GetMaxBlocks().y) {
				Play_Board.rect_game_block[Play_Board.GetCount()].rect = NewSetRect(Play_Board.rect_game_block[Play_Board.GetCount()].rect,
					{ Play_Board.GetGameBoard().right / 2 + Play_Board.GetBlockSize().witdh / 2 + 10 , Play_Board.GetGameBoard().top - Play_Board.GetBlockSize().height / 2 },
					Play_Board.GetBlockSize().witdh,
					Play_Board.GetBlockSize().height);
				Play_Board.rect_game_block[Play_Board.GetCount()].SetBlockNumType();
			}
			SetTimer(hWnd, 2, 100, (TIMERPROC)BlockDrop);


		}

		if (wParam == 'A') {
			Play_Board.rect_game_block[Play_Board.GetCount()].BlockMove(LEFT);
		}

		if (wParam == 'D') {
			Play_Board.rect_game_block[Play_Board.GetCount()].BlockMove(RIGHT);
		}

		if (wParam == 'Z') {
			Play_Board.SetMaxBlocks();
			Play_Board.SetGameBoard({ 300,400 }, 400, 500);
			KillTimer(hWnd, 2);
		}
		if (wParam == 'X') {
			Play_Board.SetMaxBlocks(6,8);
			Play_Board.SetGameBoard({ 300,400 }, 400, 500);
			KillTimer(hWnd, 2);
		}
		if (wParam == 'C') {
			Play_Board.SetMaxBlocks(7,9);
			Play_Board.SetGameBoard({ 300,400 }, 400, 500);
			KillTimer(hWnd, 2);
		}



		InvalidateRect(hWnd, NULL, TRUE);
		//ReleaseDC(hWnd, hdc);
		break;
	}
	break;

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 메뉴 선택을 구문 분석합니다.
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.


		// game board 그리기
		Rectangle(hdc, Play_Board.GetGameBoard().left, Play_Board.GetGameBoard().top,
			Play_Board.GetGameBoard().right, Play_Board.GetGameBoard().bottom);

		// game board 위에 줄 긋기
		for (int i = 1; i < Play_Board.GetMaxBlocks().x; ++i) {
			MoveToEx(hdc, Play_Board.GetGameBoard().left + i * Play_Board.GetBlockSize().witdh, Play_Board.GetGameBoard().top, NULL);
			LineTo(hdc, Play_Board.GetGameBoard().left + i * Play_Board.GetBlockSize().witdh, Play_Board.GetGameBoard().bottom);
		}
		for (int i = 1; i < Play_Board.GetMaxBlocks().y; ++i) {
			MoveToEx(hdc, Play_Board.GetGameBoard().left, Play_Board.GetGameBoard().top + i * Play_Board.GetBlockSize().height, NULL);
			LineTo(hdc, Play_Board.GetGameBoard().right, Play_Board.GetGameBoard().top + i * Play_Board.GetBlockSize().height);
		}

		// block 그리기
		for (int i = 0; i <= Play_Board.GetCount(); ++i) {
			TCHAR str[5];
			wsprintf(str, TEXT("%d"), Play_Board.rect_game_block[i].num);

			Rectangle(hdc, Play_Board.rect_game_block[i].rect.left, Play_Board.rect_game_block[i].rect.top,
				Play_Board.rect_game_block[i].rect.right, Play_Board.rect_game_block[i].rect.bottom);
			//DrawText(hdc, (LPCWSTR)Play_Board.rect_game_block[i].num, lstrlen((LPCWSTR)Play_Board.rect_game_block[i].num), &Play_Board.rect_game_block[i].rect, DT_CENTER | DT_VCENTER);
			DrawText(hdc, str, lstrlen(str), &Play_Board.rect_game_block[i].rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		}


		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}




// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
