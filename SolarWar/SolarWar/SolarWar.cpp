#include <windows.h>
#include "resource.h"
#include <atlimage.h>
#include <math.h>
#include <time.h>
HINSTANCE g_hlnst;
LPCTSTR lpszClass = "Solar War";
HWND hWnd;
#define width 1000
#define height 600
#define PI 3.14
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hlnstance, HINSTANCE hPrevlnstance, LPSTR lpszCmdParam, int nCmdShow)
{

	MSG Message;
	WNDCLASSEX WndClass;
	g_hlnst = hlnstance;

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hlnstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_THICKFRAME | WS_SYSMENU, 0, 0, width, height, NULL, (HMENU)NULL, hlnstance, NULL);
	/*WS_OVERLAPPED*/
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}
HBITMAP GetRotatedBitmap(HDC hdc, HBITMAP hBitmap, int source_x, int source_y, int dest_width, int dest_height, float angle, COLORREF bkColor, int animation)
{
	HDC sourceDC = CreateCompatibleDC(hdc);                              // ȸ���� ��Ʈ�� ������ ������ DC
	HDC destDC = CreateCompatibleDC(hdc);                              // ȸ���� ��Ʈ���� ��¹��� DC
	HBITMAP hbmResult = CreateCompatibleBitmap(hdc, dest_width, dest_height);      // ȸ���� ��Ʈ���� ��¹��� �޸𸮺�Ʈ�� �ڵ�

	HBITMAP hbmOldSource = (HBITMAP)SelectObject(sourceDC, hBitmap);               // ȸ���� ��Ʈ�� ������ DC�� ����
	HBITMAP hbmOldDest = (HBITMAP)SelectObject(destDC, hbmResult);               // ȸ���� ��Ʈ���� ��¹��� DC��, ȸ���� ��Ʈ���� ��¹��� �޸𸮺�Ʈ�� ����

	HBRUSH hbrBack = CreateSolidBrush(bkColor);                           // ȸ������ ����, ���鿵���� ĥ�� �귯���ڵ� ����
	HBRUSH hbrOld = (HBRUSH)SelectObject(destDC, hbrBack);                  // �귯���ڵ� ����
	PatBlt(destDC, 0, 0, dest_width, dest_height, PATCOPY);                     // ���õ� �귯�÷�, ȸ���� ��Ʈ���� ��¹��� DC��, �̸� ������ ä�� ����
	DeleteObject(SelectObject(destDC, hbrOld));                              // �귯�� ����

																			 //angle = (float)(angle * 3.14 / 180);                              // ȸ���� ����(360�й�)��, ���ȴ����� ������ ����
	float cosine = (float)cos(angle);                                    // ȸ���̵���ȯ ��Ŀ� �� cos��Ÿ ���� ����      
	float sine = (float)sin(angle);                                    // ȸ���̵���ȯ ��Ŀ� �� sin��Ÿ ���� ����

	SetGraphicsMode(destDC, GM_ADVANCED);                                    // ������ ��ǥ���� ȸ���� ���Ͽ�, �׷��ȸ�带 Ȯ����� �����մϴ�.(�䱸����:��98,NT�̻�)

																			 // ȸ���̵���ȯ ����� ����.
																			 // (�������� API���� ������ 2�� Page(1279) ����.)
																			 // (���� ���α׷��Ӹ� ���� ���ʼ��а� ���� Page(196) ����.)

	XFORM xform;                                                      // �������� ǥ���ϴ� 3��3���� ��� ����
	xform.eM11 = cosine;                                                // 1�� 1�� ���� ���� (ȸ������)
	xform.eM12 = sine;                                                   // 1�� 2�� ���� ���� (ȸ������)
	xform.eM21 = -sine;                                                   // 2�� 1�� ���� ���� (ȸ������)
	xform.eM22 = cosine;                                                // 2�� 2�� ���� ���� (ȸ������)
	xform.eDx = (FLOAT)dest_width / 2.0f;                                    // 3�� 1�� ���� ���� (X�� �̵� ����)
	xform.eDy = (FLOAT)dest_height / 2.0f;                                    // 3�� 2�� ���� ���� (Y�� �̵� ����)

																			  // ������ ��ķ�, �μ�DC�� �������� ��ǥ�踦 ��ȯ.
																			  // �μ�DC�� ��´������ �޸𸮺�Ʈ���� �����Ƿ�,
																			  // �����ϰ� �ִ� �޸𸮺�Ʈ���� ��ǥü�谡 ȸ���ȴ�.
	SetWorldTransform(destDC, &xform);

	// ȸ���� �޸�DC��, ȸ���� �̹����� ���

	TransparentBlt(destDC, -(dest_width / 2), -(dest_height / 2), dest_width, dest_height, sourceDC, source_x, source_y, 435, 435, RGB(255, 255, 255));


	// ���� �ڿ� ����

	SelectObject(sourceDC, hbmOldSource);
	SelectObject(destDC, hbmOldDest);
	DeleteDC(sourceDC);
	DeleteDC(destDC);

	// ȸ���� ��Ʈ�� �ڵ� ����

	return hbmResult;
}

void RotateBlt(HDC hdc, HDC hOffScreen, int dest_x, int dest_y, int dest_right, int dest_bottom, int dest_width, int dest_height,
	HBITMAP hBit, int source_x, int source_y, DWORD dFlag, float angle, COLORREF bkColor, int animation)
{
	static HDC hTempMemDC;            // ȸ���� ��Ʈ���� ������ �޸�DC
	HBITMAP hTempBitmap, hOldBitmap;   // ȸ���� ��Ʈ���� ������, ��Ʈ�� �ڵ�

									   // ���� �ѹ��� ��Ʈ��DC�� �����մϴ�.

	if (!hTempMemDC) hTempMemDC = CreateCompatibleDC(hdc);

	// ��Ʈ���� ȸ���ϴ� �Լ��� ȣ���Ͽ�, ���ϵǴ� ȸ���� ��Ʈ�� �ڵ��� ����

	hTempBitmap = GetRotatedBitmap(hdc, hBit, source_x, source_y, dest_width, dest_height, angle, bkColor, animation);

	// ȸ���� ��Ʈ�� �ڵ��� �����ϰ�,

	hOldBitmap = (HBITMAP)SelectObject(hTempMemDC, hTempBitmap);

	// ������ũ��DC��, ȸ���� ��Ʈ�� �ڵ��� ������, �޸�DC�� �̹����� ����մϴ�.
	// ���� �ּ��� �����ϸ�, �����ǥ�� �߽����� �ϴ� ȸ���̹����� �˴ϴ�.

	/////////���⼭ ������ ����//////////////
	TransparentBlt(hOffScreen, dest_x, dest_y, dest_right, dest_bottom,
		hTempMemDC, 0, 0, 435, 435, RGB(255, 255, 255));

	SelectObject(hTempMemDC, hOldBitmap);
	DeleteObject(hTempBitmap);

}

float GetRadian(float nAngle)
{
	nAngle = (float)(nAngle * 180 / 3.1415);
	return nAngle;
}

float CalcAngle(float px, float py, float px2, float py2, int Kind, int Array)
{
	float x, y, nTan, ang;
	switch (Kind)
	{
	case 0:
		x = (float)px - ((float)px2) + 0.001f;
		y = (float)py - ((float)py2) + 0.001f;
		nTan = (float)x / y;
		ang = GetRadian(-atan(nTan));
		if (x > 0 && y > 0)
			return ang + 180;
		else if (x < 0 && y>0)
			return ang + 180;
		else if (x < 0 && y < 0)
			return ang;
		else if (x > 0 && y < 0)
			return ang + 360;
		break;
	}
	return 180.f;
}


float TanGetAngle(float x1, float y1, float x2, float y2)
{
	float fX, fY;
	float fAngle;

	fX = x2 - x1;
	fY = y2 - y1;

	fAngle = atan(-fY / fX);
	if (fX < 0) fAngle += PI;
	if (x2 >= x1 && y2 >= y1) fAngle += 2 * PI;

	return fAngle;
}

typedef struct _ARMY_INFO
{
	float x, y;	// ��ǥ
	float degree; // ��� ������ �̵��Ұ���
	RECT pnt; // DrawText�� ���� �뵵
	BOOL Move; // �����̴°� �̳����� üũ
	int popula; // ������ ����
	int max_popula;
	
}ARMY;

typedef struct PLANET_INFO
{
	BOOL More_BIG;
	RECT Rect;		// ��ǥ
	int Occ_State; //���� ���� 0 = �� �༺ 1 = �Ʊ� �༺ 2= �����༺
	int Occ_Time; // ���ɿ� �ʿ��� �ð�
	ARMY Army;
	ARMY E_Army;
	RECT Time_See; //------------------------------------------------>>>>>>>>>>.��������

}PLANET;


typedef struct _MISSILE_PLANET
{
	POINT shot;
	RECT Rect;		// ��ǥ
	int Attack_Time;  // ���� ����
	int Occ_State; // ���� ���� 
	ARMY Army;
	ARMY E_Army;
}MISSILE;


HDC hdc, memdc, tmp_dc;
HBITMAP hBit, oldBit;

RECT rectView;

HBITMAP Bg_img, MyPlanet_img, Enemy_img, Empty_img, Army_img, Army_Enemy_img, 
Select_img, Missile_img, Missile_My_img, Missile_Enemy_img, CC_img,CC_My_img,CC_Enemy_img,Start_Main_img;
BITMAP Bg_size, Planet_size, Army_size, Select_size, Missile_size, CC_size, Start_Main_size;
PLANET Planet[8];
MISSILE Missile;
BOOL Missile_Occ; // �̻����� �����Ϸ��°��� üũ

int mx, my, Endx, Endy;
int Start_Planet, End_Planet;
float Agl, speed;		//��Ʈ�� ȸ�� ���� 0.05
BOOL Mouse_ON, Double_Army;

int Stage_P_count, Random_move;
BOOL Stage1 , Stage2, Stage3, Game_Start;


void Stage1_init_()
{
	Stage_P_count = 5;

	//-----------------�ʱ� ��ġ ����------------------//
	Planet[0].Rect.left = 50; Planet[0].Rect.right = 150;
	Planet[0].Rect.top = 50; Planet[0].Rect.bottom = 150;
	Planet[0].Occ_State = 1; Planet[0].More_BIG = FALSE;
	Planet[1].Rect.left = 800; Planet[1].Rect.right = 900;
	Planet[1].Rect.top = 300; Planet[1].Rect.bottom = 400;
	Planet[1].Occ_State = 2; Planet[1].More_BIG = FALSE;
	Planet[2].Rect.left = 450; Planet[2].Rect.right = 550;
	Planet[2].Rect.top = 80; Planet[2].Rect.bottom = 180;
	Planet[2].Occ_State = 0; Planet[2].More_BIG = FALSE;
	Planet[3].Rect.left = 150; Planet[3].Rect.right = 250;
	Planet[3].Rect.top = 300; Planet[3].Rect.bottom = 400;
	Planet[3].Occ_State = 0; Planet[3].More_BIG = FALSE;
	Planet[4].Rect.left = 650; Planet[4].Rect.right = 750;
	Planet[4].Rect.top = 420; Planet[4].Rect.bottom = 520;
	Planet[4].Occ_State = 0;  Planet[4].More_BIG = FALSE; 

	//----------------------------------------------------//
	//-------------------------�ʱ� ���� ----------------------------//
	for (int i = 0; i < Stage_P_count; i++)
	{
		Planet[i].Army.x = Planet[i].Rect.left + ((Planet[i].Rect.right - Planet[i].Rect.left) / 2);
		Planet[i].Army.y = Planet[i].Rect.top + ((Planet[i].Rect.bottom - Planet[i].Rect.top) / 2);
		Planet[i].E_Army.x = Planet[i].Rect.left + ((Planet[i].Rect.right - Planet[i].Rect.left) / 2);
		Planet[i].E_Army.y = Planet[i].Rect.top + ((Planet[i].Rect.bottom - Planet[i].Rect.top) / 2);
		Planet[i].Army.popula = 0;
		Planet[i].E_Army.popula = 0;
		Planet[i].Army.Move = FALSE;
		Planet[i].E_Army.Move = FALSE;

		if (Planet[i].Occ_State == 0)
		{
			Planet[i].Occ_Time = 50;  // �� �༺ ���ɿ� �ʿ��� �ð� 5s 
			if (Planet[i].More_BIG)
			{
				Planet[i].Occ_Time = 160;
				Planet[i].Army.max_popula = 500;
				Planet[i].E_Army.max_popula = 500;
			}
			else
			{
				Planet[i].Occ_Time = 80;
				// �̹� ���ɵ� ���� 8s
				Planet[i].Army.max_popula = 300;
				Planet[i].E_Army.max_popula = 300;
			}
		}
		else
		{
			if (Planet[i].More_BIG)
			{
				Planet[i].Occ_Time = 160;
				Planet[i].Army.max_popula = 500;
				Planet[i].E_Army.max_popula = 500;
			}
			else
			{
				Planet[i].Occ_Time = 80;
				// �̹� ���ɵ� ���� 8s
				Planet[i].Army.max_popula = 300;
				Planet[i].E_Army.max_popula = 300;
			}
		}
			

		Planet[i].Time_See.left = Planet[i].Rect.left;
		Planet[i].Time_See.right = Planet[i].Rect.right;
		Planet[i].Time_See.top = Planet[i].Rect.bottom;
		Planet[i].Time_See.bottom = Planet[i].Rect.bottom +10;

		Missile = { NULL, };

	}
	//-----------------------------------------------------------------//

	Mouse_ON = FALSE;
	Double_Army = FALSE;
	Start_Planet = -1;
	End_Planet = -1;

}
void Stage2_init_()
{
	Stage_P_count = 6;
	int size = 85;
	//-----------------�ʱ� ��ġ ����------------------//
	Planet[0].Rect.left = 150; Planet[0].Rect.right = Planet[0].Rect.left + size;
	Planet[0].Rect.top = 180; Planet[0].Rect.bottom = Planet[0].Rect.top + size;
	Planet[0].Occ_State = 1; Planet[0].More_BIG = FALSE;

	Planet[1].Rect.left = 800; Planet[1].Rect.right = Planet[1].Rect.left + size;
	Planet[1].Rect.top = 380; Planet[1].Rect.bottom = Planet[1].Rect.top + size;
	Planet[1].Occ_State = 2; Planet[1].More_BIG = FALSE;

	Planet[2].Rect.left = 230; Planet[2].Rect.right = Planet[2].Rect.left + size;
	Planet[2].Rect.top = 400; Planet[2].Rect.bottom = Planet[2].Rect.top + size;
	Planet[2].Occ_State = 0; Planet[2].More_BIG = FALSE;

	Planet[3].Rect.left = 380; Planet[3].Rect.right = Planet[3].Rect.left + size;
	Planet[3].Rect.top = 45; Planet[3].Rect.bottom = Planet[3].Rect.top + size;
	Planet[3].Occ_State = 0; Planet[3].More_BIG = FALSE;

	Planet[4].Rect.left = 770; Planet[4].Rect.right = Planet[4].Rect.left + size;
	Planet[4].Rect.top = 180; Planet[4].Rect.bottom = Planet[4].Rect.top + size;
	Planet[4].Occ_State = 0; Planet[4].More_BIG = FALSE;

	Planet[5].Rect.left = 590; Planet[5].Rect.right = Planet[5].Rect.left + size;
	Planet[5].Rect.top = 410; Planet[5].Rect.bottom = Planet[5].Rect.top + size;
	Planet[5].Occ_State = 0; Planet[5].More_BIG = FALSE;

	Missile.Rect.left = 445;  Missile.Rect.right = Missile.Rect.left + 260;   
	Missile.Rect.top = 120;  Missile.Rect.bottom = Missile.Rect.top +260;
	//----------------------------------------------------//
	//-------------------------�ʱ� ���� ----------------------------//
	for (int i = 0; i < Stage_P_count; i++)
	{
		Planet[i].Army.x = Planet[i].Rect.left + ((Planet[i].Rect.right - Planet[i].Rect.left) / 2);
		Planet[i].Army.y = Planet[i].Rect.top + ((Planet[i].Rect.bottom - Planet[i].Rect.top) / 2);
		Planet[i].E_Army.x = Planet[i].Rect.left + ((Planet[i].Rect.right - Planet[i].Rect.left) / 2);
		Planet[i].E_Army.y = Planet[i].Rect.top + ((Planet[i].Rect.bottom - Planet[i].Rect.top) / 2);
		Planet[i].Army.popula = 0;
		Planet[i].E_Army.popula = 0;
		Planet[i].Army.Move = FALSE;
		Planet[i].E_Army.Move = FALSE;

		if (Planet[i].Occ_State == 0)
		{
			Planet[i].Occ_Time = 50;  // �� �༺ ���ɿ� �ʿ��� �ð� 5s 
			if (Planet[i].More_BIG)
			{
				Planet[i].Occ_Time = 160;
				Planet[i].Army.max_popula = 500;
				Planet[i].E_Army.max_popula = 500;
			}
			else
			{
				Planet[i].Occ_Time = 80;
				// �̹� ���ɵ� ���� 8s
				Planet[i].Army.max_popula = 300;
				Planet[i].E_Army.max_popula = 300;
			}
		}
		else
		{
			if (Planet[i].More_BIG)
			{
				Planet[i].Occ_Time = 160;
				Planet[i].Army.max_popula = 500;
				Planet[i].E_Army.max_popula = 500;
			}
			else
			{
				Planet[i].Occ_Time = 80;
				// �̹� ���ɵ� ���� 8s
				Planet[i].Army.max_popula = 300;
				Planet[i].E_Army.max_popula = 300;
			}
		}

		Missile.Occ_State = 0;
		Missile.Attack_Time = 20;
		Missile.Army.popula = 0;
		Missile.E_Army.popula = 0;

		Planet[i].Time_See.left = Planet[i].Rect.left;
		Planet[i].Time_See.right = Planet[i].Rect.right;
		Planet[i].Time_See.top = Planet[i].Rect.bottom;
		Planet[i].Time_See.bottom = Planet[i].Rect.bottom + 30;

	}
	//-----------------------------------------------------------------//

	Mouse_ON = FALSE;
	Double_Army = FALSE;
	Start_Planet = -1;
	End_Planet = -1;

}
void Stage3_init_()
{
	Stage_P_count = 8;
	int size = 75;
	int Big_size = 75;
	//-----------------�ʱ� ��ġ ����------------------//
	Planet[0].Rect.left = rectView.left+200; Planet[0].Rect.right = Planet[0].Rect.left + size;
	Planet[0].Rect.top = rectView.top + 70; Planet[0].Rect.bottom = Planet[0].Rect.top + size;
	Planet[0].Occ_State = 0; Planet[0].More_BIG = FALSE;

	Planet[1].Rect.left = rectView.left + 200; Planet[1].Rect.right = Planet[1].Rect.left + size;
	Planet[1].Rect.top = rectView.bottom - 140; Planet[1].Rect.bottom = Planet[1].Rect.top + size;
	Planet[1].Occ_State = 0; Planet[1].More_BIG = FALSE;

	Planet[2].Rect.left = rectView.right - 250; Planet[2].Rect.right = Planet[2].Rect.left + size;
	Planet[2].Rect.top = rectView.top + 70; Planet[2].Rect.bottom = Planet[2].Rect.top + size;
	Planet[2].Occ_State = 0; Planet[2].More_BIG = FALSE;

	Planet[3].Rect.left = rectView.right - 250; Planet[3].Rect.right = Planet[3].Rect.left + size;
	Planet[3].Rect.top = rectView.bottom - 140; Planet[3].Rect.bottom = Planet[3].Rect.top + size;
	Planet[3].Occ_State = 0; Planet[3].More_BIG = FALSE;


	Planet[4].Rect.left = rectView.left + 60; Planet[4].Rect.right = Planet[4].Rect.left + Big_size;
	Planet[4].Rect.top = rectView.top + 100; Planet[4].Rect.bottom = Planet[4].Rect.top + Big_size;
	Planet[4].Occ_State = 1; Planet[4].More_BIG = TRUE;

	Planet[5].Rect.left = rectView.left + 60; Planet[5].Rect.right = Planet[5].Rect.left + Big_size;
	Planet[5].Rect.top = rectView.bottom - 170; Planet[5].Rect.bottom = Planet[5].Rect.top + Big_size;
	Planet[5].Occ_State = 0; Planet[5].More_BIG = TRUE;

	Planet[6].Rect.left = rectView.right - 150; Planet[6].Rect.right = Planet[6].Rect.left + Big_size;
	Planet[6].Rect.top = rectView.top + 100; Planet[6].Rect.bottom = Planet[6].Rect.top + Big_size;
	Planet[6].Occ_State = 0; Planet[6].More_BIG = TRUE;

	Planet[7].Rect.left = rectView.right - 150; Planet[7].Rect.right = Planet[7].Rect.left + Big_size;
	Planet[7].Rect.top = rectView.bottom - 170; Planet[7].Rect.bottom = Planet[7].Rect.top + Big_size;
	Planet[7].Occ_State = 2; Planet[7].More_BIG = TRUE;
	
	Missile.Rect.left = 370;  Missile.Rect.right = Missile.Rect.left + 260;
	Missile.Rect.top = 150;  Missile.Rect.bottom = Missile.Rect.top + 260;
	//----------------------------------------------------//
	//-------------------------�ʱ� ���� ----------------------------//
	for (int i = 0; i < Stage_P_count; i++)
	{
		Planet[i].Army.x = Planet[i].Rect.left + ((Planet[i].Rect.right - Planet[i].Rect.left) / 2);
		Planet[i].Army.y = Planet[i].Rect.top + ((Planet[i].Rect.bottom - Planet[i].Rect.top) / 2);
		Planet[i].E_Army.x = Planet[i].Rect.left + ((Planet[i].Rect.right - Planet[i].Rect.left) / 2);
		Planet[i].E_Army.y = Planet[i].Rect.top + ((Planet[i].Rect.bottom - Planet[i].Rect.top) / 2);
		Planet[i].Army.popula = 0;
		Planet[i].E_Army.popula = 0;
		Planet[i].Army.Move = FALSE;
		Planet[i].E_Army.Move = FALSE;

		if (Planet[i].Occ_State == 0)
		{
			Planet[i].Occ_Time = 50;  // �� �༺ ���ɿ� �ʿ��� �ð� 5s 
			if (Planet[i].More_BIG)
			{
				Planet[i].Occ_Time = 160;
				Planet[i].Army.max_popula = 500;
				Planet[i].E_Army.max_popula = 500;
			}
			else
			{
				Planet[i].Occ_Time = 80;
				// �̹� ���ɵ� ���� 8s
				Planet[i].Army.max_popula = 300;
				Planet[i].E_Army.max_popula = 300;
			}
		}
		else
		{
			if (Planet[i].More_BIG)
			{
				Planet[i].Occ_Time = 160;
				Planet[i].Army.max_popula = 500;
				Planet[i].E_Army.max_popula = 500;
			}
			else
			{
				Planet[i].Occ_Time = 80;
				// �̹� ���ɵ� ���� 8s
				Planet[i].Army.max_popula = 300;
				Planet[i].E_Army.max_popula = 300;
			}
		}

		Missile.Occ_State = 0;
		Missile.Attack_Time = 20;
		Missile.Army.popula = 0;
		Missile.E_Army.popula = 0;


		Planet[i].Time_See.left = Planet[i].Rect.left;
		Planet[i].Time_See.right = Planet[i].Rect.right;
		Planet[i].Time_See.top = Planet[i].Rect.bottom;
		Planet[i].Time_See.bottom = Planet[i].Rect.bottom + 30;

	}
	//-----------------------------------------------------------------//

	Mouse_ON = FALSE;
	Double_Army = FALSE;
	Start_Planet = -1;
	End_Planet = -1;

}

void Create_Army()
{
	char pop[8];

	SetBkMode(memdc, TRANSPARENT);
	for (int i = 0; i < Stage_P_count; i++)
	{
		Planet[i].Army.pnt.left = Planet[i].Army.x - 20;
		Planet[i].Army.pnt.right = Planet[i].Army.x+20;
		Planet[i].Army.pnt.top = Planet[i].Army.y - 20;
		Planet[i].Army.pnt.bottom = Planet[i].Army.y + 20;

		Planet[i].E_Army.pnt.left = Planet[i].E_Army.x - 20;
		Planet[i].E_Army.pnt.right = Planet[i].E_Army.x + 20;
		Planet[i].E_Army.pnt.top = Planet[i].E_Army.y - 20;
		Planet[i].E_Army.pnt.bottom = Planet[i].E_Army.y + 20;

		
		if (Planet[i].Occ_State == 1)
		{
			SelectObject(tmp_dc, Army_img);
			if(Planet[i].Army.Move)
				TransparentBlt(memdc, Planet[i].Army.pnt.left, Planet[i].Army.pnt.top, Planet[i].Army.pnt.right - Planet[i].Army.pnt.left, Planet[i].Army.pnt.bottom - Planet[i].Army.pnt.top, tmp_dc, 0, 0, Army_size.bmWidth, Army_size.bmWidth, RGB(255, 255, 255));
		}
		else if (Planet[i].Occ_State == 2)
		{
			SelectObject(tmp_dc, Army_Enemy_img);
			if (Planet[i].E_Army.Move)
				TransparentBlt(memdc, Planet[i].E_Army.pnt.left, Planet[i].E_Army.pnt.top, Planet[i].E_Army.pnt.right - Planet[i].E_Army.pnt.left, Planet[i].E_Army.pnt.bottom - Planet[i].E_Army.pnt.top, tmp_dc, 0, 0, Army_size.bmWidth, Army_size.bmWidth, RGB(255, 255, 255));
		}
			
		if (Planet[i].Occ_State != 0)	// �� �༺�� �ƴ� ��쿡�� ���� �� ����
		{
			if (Planet[i].Occ_State == 1)  //�Ʊ� ���¸� �Ʊ����� ����
			{
				// �����̰� ���� �ʰ� �ִ������ ������ ����
				if (!Planet[i].Army.Move && Planet[i].Army.popula < Planet[i].Army.max_popula) 
					Planet[i].Army.popula++;

				itoa(Planet[i].Army.popula / 10, pop, 10);
				SetTextColor(memdc, RGB(255, 255, 255));
				DrawText(memdc, pop, -1, &Planet[i].Army.pnt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				itoa(Planet[i].Occ_Time, pop, 10);
				DrawText(memdc, pop, -1, &Planet[i].Time_See, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}

			else if (Planet[i].Occ_State == 2) // ���� ���¸� ���� ���� ����
			{
				if (!Planet[i].E_Army.Move && Planet[i].E_Army.popula < Planet[i].E_Army.max_popula) // �����̰� ���� �ʴٸ�
					Planet[i].E_Army.popula++;

				itoa(Planet[i].E_Army.popula / 10, pop, 10);
				SetTextColor(memdc, RGB(255, 255, 255));
				DrawText(memdc, pop, -1, &Planet[i].E_Army.pnt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				itoa(Planet[i].Occ_Time, pop, 10);
				DrawText(memdc, pop, -1, &Planet[i].Time_See, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
		}
	}

	BitBlt(hdc, 0, 0, rectView.right, rectView.bottom, memdc, 0, 0, SRCCOPY);

	DeleteObject(Army_img); DeleteObject(Army_Enemy_img);

}

void Enemy_Move()
{
	float Line_Length = 0, Min_Length = 1200;
	int Min_Planet = -1, Empty_Planet_Count = 0, Min_Army = 1200, E_Army_speed = 6;

	int dx, dy;
	double rad;

	//--------------------- �Ÿ��� ���� ª���� ���ϱ�----------------------
	for (int i = 0; i < Stage_P_count; i++)
	{
		if (Planet[i].Occ_State == 2 && !Planet[i].E_Army.Move)	 //���� ������ ������ �༺�̰�, �� ���簡 �����̰����� �ʴٸ�
		{
			for (int j = 0; j < Stage_P_count; j++)
			{
				if (Planet[j].Occ_State == 0) // �� �༺�̶��
				{
					Empty_Planet_Count++;
					if (Planet[j].E_Army.x < Planet[i].E_Army.x)
					{
						if (Planet[j].E_Army.y < Planet[i].E_Army.y)
						{
							Line_Length = sqrt(pow(Planet[i].E_Army.x - Planet[j].E_Army.x, 2) + pow(Planet[i].E_Army.y - Planet[j].E_Army.y, 2));
							if (Min_Length > Line_Length)
							{
								Min_Length = Line_Length;
								Min_Planet = j;

								Planet[i].E_Army.degree = (atan2f(Planet[j].E_Army.y - Planet[i].E_Army.y, Planet[j].E_Army.x - Planet[i].E_Army.x) / 360) * 360.0;

							}
						}
						else
						{
							Line_Length = sqrt(pow(Planet[i].E_Army.x - Planet[j].E_Army.x, 2) + pow(Planet[j].E_Army.y - Planet[i].E_Army.y, 2));
							if (Min_Length > Line_Length)
							{
								Min_Length = Line_Length;
								Min_Planet = j;
							}
						}
					}
					else
					{
						if (Planet[j].E_Army.y < Planet[i].E_Army.y)
						{
							Line_Length = sqrt(pow(Planet[j].E_Army.x - Planet[i].E_Army.x, 2) + pow(Planet[i].E_Army.y - Planet[j].E_Army.y, 2));
							if (Min_Length > Line_Length)
							{
								Min_Length = Line_Length;
								Min_Planet = j;
							}
						}
						else
						{
							Line_Length = sqrt(pow(Planet[j].E_Army.x - Planet[i].E_Army.x, 2) + pow(Planet[j].E_Army.y - Planet[i].E_Army.y, 2));
							if (Min_Length > Line_Length)
							{
								Min_Length = Line_Length;
								Min_Planet = j;
							}
						}
					}
				}
			}

			if (Empty_Planet_Count == 0) //�� �༺�� ���ٸ�
			{
				Min_Army = 1200;
				for (int j = 0; j < Stage_P_count; j++)
				{
					if (Planet[j].Occ_State == 1 && Planet[j].Army.popula < Min_Army) //�Ʊ����ɻ����̰� �Ʊ����簡 ���� ������
					{
						Min_Army = Planet[j].Army.popula;
						Min_Planet = j;
					}
				}
			}
			
			if (Planet[i].E_Army.popula != 0 && Planet[i].E_Army.popula % Random_move <= 10 && Planet[i].E_Army.popula > 50)
			{
				// ���� ����� �༺�� ��������  ������ ���ϰ� ��� 

				if (!Stage1 && Planet[i].E_Army.popula >= 100 &&  Missile.Occ_State != 2) //10�� �̻��ְ� 
				{
						Missile_Occ = TRUE;
						Planet[i].E_Army.degree = TanGetAngle(Planet[i].Army.x, Planet[i].Army.y, Missile.shot.x, Missile.shot.y);
						Planet[i].E_Army.Move = TRUE;
				}
				else {
					Planet[i].E_Army.degree = TanGetAngle(Planet[i].Army.x, Planet[i].Army.y, Planet[Min_Planet].E_Army.x, Planet[Min_Planet].E_Army.y);
					Planet[i].E_Army.Move = TRUE;
				}
				
				Min_Planet = -1;
				Min_Length = 1200;
			}

		}
	}
	//----------------------------------------------------------------------

	//------------------�� ���� �̵� ------------------------------------

	for (int i = 0; i < Stage_P_count; i++)	// �����̴� ���� i
	{
		if (Planet[i].E_Army.Move)	// ���� ������ ���ؼ� �̵�
		{
			Planet[i].E_Army.x += cos(Planet[i].E_Army.degree) * E_Army_speed;
			Planet[i].E_Army.y -= sin(Planet[i].E_Army.degree) * E_Army_speed;
		}

		for (int j = 0; j < Stage_P_count; j++)	// �ٸ��༺ j
		{
			// �����̰� �ִ� ������ �༺�� �ƴϰ� �ٸ� �༺�� �浹�ߴµ� 
			if (i != j &&  Planet[j].Rect.left < Planet[i].E_Army.x &&Planet[i].E_Army.x < Planet[j].Rect.right && Planet[j].Rect.top < Planet[i].E_Army.y && Planet[i].E_Army.y < Planet[j].Rect.bottom)
			{
				if (Planet[j].Occ_State == 2)	// ���� ������ �༺�̶��
				{
					Planet[j].E_Army.popula += Planet[i].E_Army.popula;		//���� ���� �����ְ�
					Planet[i].E_Army.popula = 0;	  // �����̴������ �ٽ� �������
					Planet[i].E_Army.x = Planet[i].Rect.left + ((Planet[i].Rect.right - Planet[i].Rect.left) / 2);
					Planet[i].E_Army.y = Planet[i].Rect.top + ((Planet[i].Rect.bottom - Planet[i].Rect.top) / 2);
					Planet[i].E_Army.Move = FALSE;
				}
				if (Planet[j].Occ_State == 1)  // 1 . �Ʊ��� ������ �༺�̰�
				{
					for (int z = 0; z < Stage_P_count; z++)	//�Ʊ��༺������ �̹� ���ִ� ������������ �����ְ� ����
					{
						if (i != z && Planet[z].Occ_State == 2
							&& Planet[j].Rect.left <Planet[z].E_Army.x && Planet[j].Rect.right >Planet[z].E_Army.x
							&& Planet[j].Rect.top < Planet[z].E_Army.y && Planet[j].Rect.bottom > Planet[z].E_Army.y)
						{
							Planet[i].E_Army.popula += Planet[z].E_Army.popula;		//���� ���� �����ְ�
							Planet[z].E_Army.popula = 0;	  // �����̴������ �ٽ� �������
							Planet[z].E_Army.x = Planet[i].Rect.left + ((Planet[i].Rect.right - Planet[i].Rect.left) / 2);
							Planet[z].E_Army.y = Planet[i].Rect.top + ((Planet[i].Rect.bottom - Planet[i].Rect.top) / 2);
							Planet[z].E_Army.Move = FALSE;
						}
					}
					if (!Planet[j].Army.Move) // ������ �༺�� ���簡 �������� �ʰ��ִٸ�
					{
						if (Planet[j].Army.popula < Planet[i].E_Army.popula)  // �� ���簡 �� ���ٸ� 
						{
							Planet[i].E_Army.popula -= Planet[j].Army.popula;
							Planet[j].Army.popula = 0;
							if (Planet[j].Occ_Time != 0)
							{
								// ���ɽð��� ���ҽ�Ű��  0�̵ɋ����� �浹�� ���縦 ���ڸ��� �ӹ����� �Ѵ�
								Planet[j].Occ_Time--;
								Planet[i].E_Army.x = Planet[j].Rect.left + ((Planet[j].Rect.right - Planet[j].Rect.left) / 2);
								Planet[i].E_Army.y = Planet[j].Rect.top + ((Planet[j].Rect.bottom - Planet[j].Rect.top) / 2);

							}
							else if (Planet[j].Occ_Time <= 0)
							{
								// ���ɽð��� �������� ���¸� �ٲ��ְ� ���縦 �ٽ� �� ��ġ���� ������Ų��.
								Planet[j].Occ_Time = 80;
								Planet[j].Occ_State = 2;
								Planet[j].Army.popula += Planet[i].E_Army.popula;		//�༺ ���� �����ְ�
								Planet[i].E_Army.popula = 0;	  // �����̴������ �ٽ� �������
								Planet[i].E_Army.x = Planet[i].Rect.left + ((Planet[i].Rect.right - Planet[i].Rect.left) / 2);
								Planet[i].E_Army.y = Planet[i].Rect.top + ((Planet[i].Rect.bottom - Planet[i].Rect.top) / 2);
								Planet[i].E_Army.Move = FALSE;
							}
						}
						else if ((Planet[j].Army.popula >= Planet[i].E_Army.popula)) // �� ���簡 �� ���ų� ���ٸ�
						{
							Planet[j].Army.popula -= Planet[i].E_Army.popula;
							Planet[i].E_Army.popula = 0;	  // �����̴������ �ٽ� �������
							Planet[i].E_Army.x = Planet[i].Rect.left + ((Planet[i].Rect.right - Planet[i].Rect.left) / 2);
							Planet[i].E_Army.y = Planet[i].Rect.top + ((Planet[i].Rect.bottom - Planet[i].Rect.top) / 2);
							Planet[i].E_Army.Move = FALSE;
							Planet[j].Occ_Time = 80; // �� �� �׿����� ���ɽð� �ʱ�ȭ
						}
					}
					else //������ �༺�� ���簡 �����̰��ִٸ�
					{
						if (Planet[j].Occ_Time != 0)
						{
							// ���ɽð��� ���ҽ�Ű��  0�̵ɋ����� �浹�� ���縦 ���ڸ��� �ӹ����� �Ѵ�
							Planet[j].Occ_Time--;
							Planet[i].E_Army.x = Planet[j].Rect.left + ((Planet[j].Rect.right - Planet[j].Rect.left) / 2);
							Planet[i].E_Army.y = Planet[j].Rect.top + ((Planet[j].Rect.bottom - Planet[j].Rect.top) / 2);

						}
						else if (Planet[j].Occ_Time <= 0)
						{
							// ���ɽð��� �������� ���¸� �ٲ��ְ� ���縦 �ٽ� �� ��ġ���� ������Ų��.
							Planet[j].Occ_Time = 80;
							Planet[j].Occ_State = 2;
							Planet[j].Army.popula += Planet[i].E_Army.popula;		//�༺ ���� �����ְ�
							Planet[i].E_Army.popula = 0;	  // �����̴������ �ٽ� �������
							Planet[i].E_Army.x = Planet[i].Rect.left + ((Planet[i].Rect.right - Planet[i].Rect.left) / 2);
							Planet[i].E_Army.y = Planet[i].Rect.top + ((Planet[i].Rect.bottom - Planet[i].Rect.top) / 2);
							Planet[i].E_Army.Move = FALSE;
						}
					}
				}

				else if (Planet[j].Occ_State == 0)	// 2. �� �༺�ε�
				{
					for (int z = 0; z < Stage_P_count; z++)
					{
						// �Ʊ��� �����鼭 
						if (Planet[z].Occ_State == 1
							&& Planet[j].Rect.left <Planet[z].Army.x && Planet[j].Rect.right >Planet[z].Army.x
							&& Planet[j].Rect.top < Planet[z].Army.y && Planet[j].Rect.bottom > Planet[z].Army.y)
						{
							Planet[j].Occ_Time = 80;
							if (Planet[i].E_Army.popula < Planet[z].Army.popula) //�Ʊ� ���簡 �� ������
							{
								Planet[z].Army.popula -= Planet[i].E_Army.popula;
								Planet[i].E_Army.popula = 0;	  // �����̴������ �ٽ� �������
								Planet[i].E_Army.x = Planet[i].Rect.left + ((Planet[i].Rect.right - Planet[i].Rect.left) / 2);
								Planet[i].E_Army.y = Planet[i].Rect.top + ((Planet[i].Rect.bottom - Planet[i].Rect.top) / 2);
								Planet[i].E_Army.Move = FALSE;
							}
							else
							{
								Planet[i].E_Army.popula -= Planet[z].Army.popula;
								Planet[z].Army.popula = 0;	  // �Ʊ��� ������ġ �ٽ� ��������
								Planet[z].Army.x = Planet[z].Rect.left + ((Planet[z].Rect.right - Planet[z].Rect.left) / 2);
								Planet[z].Army.y = Planet[z].Rect.top + ((Planet[z].Rect.bottom - Planet[z].Rect.top) / 2);
								Planet[z].Army.Move = FALSE;
							}

						}
						// �̹� �ٸ� �� ���簡 �����߿� ������
						else if (i != z && Planet[z].Occ_State == 2
							&& Planet[j].Rect.left <Planet[z].E_Army.x && Planet[j].Rect.right >Planet[z].E_Army.x
							&& Planet[j].Rect.top < Planet[z].E_Army.y && Planet[j].Rect.bottom > Planet[z].E_Army.y)
						{
							Planet[i].E_Army.popula += Planet[z].E_Army.popula;		//���� ���� �����ְ�
							Planet[z].E_Army.popula = 0;	  // �����̴������ �ٽ� �������
							Planet[z].E_Army.x = Planet[i].Rect.left + ((Planet[i].Rect.right - Planet[i].Rect.left) / 2);
							Planet[z].E_Army.y = Planet[i].Rect.top + ((Planet[i].Rect.bottom - Planet[i].Rect.top) / 2);
							Planet[z].E_Army.Move = FALSE;
						}
					}
					if (Planet[j].Occ_Time != 0)
					{
						// ���ɽð��� ���ҽ�Ű��  0�̵ɋ����� �浹�� ���縦 ���ڸ��� �ӹ����� �Ѵ�
						Planet[j].Occ_Time--;
						Planet[i].E_Army.x = Planet[j].Rect.left + ((Planet[j].Rect.right - Planet[j].Rect.left) / 2);
						Planet[i].E_Army.y = Planet[j].Rect.top + ((Planet[j].Rect.bottom - Planet[j].Rect.top) / 2);

					}
					else if (Planet[j].Occ_Time <= 0)
					{
						Planet[j].Occ_Time = 80;
						Planet[j].Occ_State = 2;
						Planet[j].E_Army.popula += Planet[i].E_Army.popula;		//�༺ ���� �����ְ�
						Planet[i].E_Army.popula = 0;	  // �����̴������ �ٽ� �������
						Planet[i].E_Army.x = Planet[i].Rect.left + ((Planet[i].Rect.right - Planet[i].Rect.left) / 2);
						Planet[i].E_Army.y = Planet[i].Rect.top + ((Planet[i].Rect.bottom - Planet[i].Rect.top) / 2);
						Planet[i].E_Army.Move = FALSE;
					}
				}
				else if (Planet[j].Occ_State == 2 && !Planet[j].E_Army.Move)
				{
					Planet[j].Army.popula += Planet[i].E_Army.popula;		//�༺ ���� �����ְ�
					Planet[i].E_Army.popula = 0;	  // �����̴������ �ٽ� �������
					Planet[i].E_Army.x = Planet[i].Rect.left + ((Planet[i].Rect.right - Planet[i].Rect.left) / 2);
					Planet[i].E_Army.y = Planet[i].Rect.top + ((Planet[i].Rect.bottom - Planet[i].Rect.top) / 2);
					Planet[i].E_Army.Move = FALSE;
				}
			}
		}
	}

	//�̻��� �༺ �浹üũ
	
	for (int m = 0; m < Stage_P_count; m++)
	{
		if (!Stage1 && Missile_Occ && Missile.shot.x - 20 < Planet[m].E_Army.x && Planet[m].E_Army.x < Missile.shot.x + 20 && Missile.shot.y - 20 < Planet[m].E_Army.y &&Planet[m].E_Army.y < Missile.shot.y + 20)
		{
			Missile.Occ_State = 2;
			Missile_Occ = FALSE;
			Planet[m].E_Army.x = Planet[m].Rect.left + ((Planet[m].Rect.right - Planet[m].Rect.left) / 2);
			Planet[m].E_Army.y = Planet[m].Rect.top + ((Planet[m].Rect.bottom - Planet[m].Rect.top) / 2);
			Planet[m].E_Army.Move = FALSE;
		}
	}
	//-------------------------------------------------------------
}

void Stage()
{
	HPEN hPen, oldPen;

	Bg_img = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP4));
	GetObject(Bg_img, sizeof(BITMAP), &Bg_size);

	MyPlanet_img = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP1));
	Enemy_img = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP3));
	Empty_img = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP2));
	GetObject(MyPlanet_img, sizeof(BITMAP), &Planet_size);

	Army_img = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP9));
	GetObject(Army_img, sizeof(BITMAP), &Army_size);
	Army_Enemy_img = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP10));
	

	Missile_img = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP7));
	GetObject(Missile_img, sizeof(BITMAP), &Missile_size);

	Missile_My_img = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP12));
	Missile_Enemy_img = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP11));

	CC_img = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP8));
	GetObject(CC_img, sizeof(BITMAP), &CC_size);
	CC_My_img = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP14));
	CC_Enemy_img = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP13));

	Select_img = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP5));
	GetObject(Select_img, sizeof(BITMAP), &Select_size);


	hPen = CreatePen(PS_SOLID, 5, RGB(255, 255, 255));	// Select True . ���� �� ����
	oldPen = (HPEN)SelectObject(memdc, hPen);

	//����̹��� ---------------------
	SelectObject(tmp_dc, Bg_img);

	SetStretchBltMode(memdc, COLORONCOLOR);
	StretchBlt(memdc, rectView.left, rectView.top, rectView.right, rectView.bottom, 
		tmp_dc, 0, 0, Bg_size.bmWidth, Bg_size.bmHeight, SRCCOPY);
	//-----


	for (int i = 0; i < Stage_P_count; i++)
	{
		if (Mouse_ON)
		{
			// ------------ ���ý� �̹������ ó��/��---------------//
			SelectObject(tmp_dc, Select_img);

			if (Planet[i].Rect.left < mx && mx < Planet[i].Rect.right && Planet[i].Rect.top < my && my < Planet[i].Rect.bottom)
			{
				TransparentBlt(memdc, mx - ((Planet[i].Rect.right - Planet[i].Rect.left) / 2) - 10, my - ((Planet[i].Rect.bottom - Planet[i].Rect.top) / 2) - 10, Planet[i].Rect.right - Planet[i].Rect.left + 20, Planet[i].Rect.bottom - Planet[i].Rect.top + 20, tmp_dc, 0, 0, Select_size.bmWidth, Select_size.bmHeight, RGB(255, 255, 255));
				TransparentBlt(memdc, Endx - ((Planet[i].Rect.right - Planet[i].Rect.left) / 2) - 10, Endy - ((Planet[i].Rect.bottom - Planet[i].Rect.top) / 2) - 10, Planet[i].Rect.right - Planet[i].Rect.left + 20, Planet[i].Rect.bottom - Planet[i].Rect.top + 20, tmp_dc, 0, 0, Select_size.bmWidth, Select_size.bmHeight, RGB(255, 255, 255));

				// ��ó�� 2���� ���ִ� ������  �ϳ��θ� �ϸ� ��������ġ���� �׷����⶧���� �ϳ��ۿ� �ȳ��´�.
				//------------------------------------------------------//

				// ���� �� �׸��� (�༺�� �߾Ӻ��� �߾ӱ��� - �༺ ũ�⿡ ���� ��ǥ���� �޶����ڳ�??????)
				MoveToEx(memdc, mx, my, NULL);
				LineTo(memdc, Endx, Endy);
				// ���ἱ�� ���콺�� Ŭ���������� �׷����°Ŵϱ� ����
			}
		}
	}

	hPen = CreatePen(PS_DOT, 1, RGB(255, 0, 0));	// Select True . ���� �� ����
	oldPen = (HPEN)SelectObject(memdc, hPen);
	SelectObject(memdc, GetStockObject(NULL_BRUSH));
		
	//----------------------------��Ʈ�� ȸ��--------------------------//
	// �ݺ������� �׸��� �ε巴�� ����

	for (int i = 0; i < Stage_P_count; i++)
	{
		// ��� �ߵǳ�??
		if (Planet[i].Occ_State == 1)
		{
			if (Planet[i].More_BIG)
			{
				SelectObject(tmp_dc, CC_My_img);
				TransparentBlt(memdc, Planet[i].Rect.left-6, Planet[i].Rect.top + 12, 90, 60, tmp_dc, 0, 0, CC_size.bmWidth, CC_size.bmHeight, RGB(255, 255, 255));
			}
			else
				RotateBlt(hdc, memdc, Planet[i].Rect.left, Planet[i].Rect.top, Planet[i].Rect.right - Planet[i].Rect.left, Planet[i].Rect.bottom - Planet[i].Rect.top, Planet_size.bmWidth, Planet_size.bmHeight, MyPlanet_img, 0, 0, SRCCOPY, Agl, RGB(255, 255, 255), 0);
		}
		else if (Planet[i].Occ_State == 2)
		{
			if (Planet[i].More_BIG)
			{
				SelectObject(tmp_dc, CC_Enemy_img);
				TransparentBlt(memdc, Planet[i].Rect.left-6, Planet[i].Rect.top + 12, 90, 60, tmp_dc, 0, 0, CC_size.bmWidth, CC_size.bmHeight, RGB(255, 255, 255));
			}
			else
				RotateBlt(hdc, memdc, Planet[i].Rect.left, Planet[i].Rect.top, Planet[i].Rect.right - Planet[i].Rect.left, Planet[i].Rect.bottom - Planet[i].Rect.top, Planet_size.bmWidth, Planet_size.bmHeight, Enemy_img, 0, 0, SRCCOPY, Agl, RGB(255, 255, 255), 0);
		}
		else if (Planet[i].Occ_State == 0)
		{
			SelectObject(tmp_dc, CC_img);
			if(Planet[i].More_BIG)
				TransparentBlt(memdc, Planet[i].Rect.left-6, Planet[i].Rect.top + 12, 90, 60, tmp_dc, 0, 0, CC_size.bmWidth, CC_size.bmHeight, RGB(255, 255, 255));
			else
				RotateBlt(hdc, memdc, Planet[i].Rect.left, Planet[i].Rect.top, Planet[i].Rect.right - Planet[i].Rect.left, Planet[i].Rect.bottom - Planet[i].Rect.top, Planet_size.bmWidth, Planet_size.bmHeight, Empty_img, 0, 0, SRCCOPY, Agl, RGB(255, 255, 255), 0);
		}
			
	}

	//----------------------------------------------------------------//

	// �̻��� �༺ �׷��ִ� �κ�

	if (Stage2 || Stage3)
	{
		Missile.shot.x = Missile.Rect.left + ((Missile.Rect.right - Missile.Rect.left) / 2);
		Missile.shot.y = Missile.Rect.top + ((Missile.Rect.bottom - Missile.Rect.top) / 2);
		//���� �������� �׸�
		Ellipse(memdc, Missile.Rect.left, Missile.Rect.top, Missile.Rect.right, Missile.Rect.bottom);
		
		if (Missile.Occ_State == 0)
			SelectObject(tmp_dc, Missile_img);
		else if (Missile.Occ_State == 1)
			SelectObject(tmp_dc, Missile_My_img);
		else if (Missile.Occ_State == 2)
			SelectObject(tmp_dc, Missile_Enemy_img);
		TransparentBlt(memdc, Missile.shot.x - 50, Missile.shot.y - 35, 110, 70, tmp_dc, 0, 0, Missile_size.bmWidth, Missile_size.bmHeight, RGB(255, 255, 255));
	}

	BitBlt(hdc, 0, 0, rectView.right, rectView.bottom, memdc, 0, 0, SRCCOPY);

	SelectObject(memdc, oldPen);
	DeleteObject(hPen);

	DeleteObject(Bg_img);
	DeleteObject(MyPlanet_img); DeleteObject(Enemy_img); DeleteObject(Empty_img); 
	 DeleteObject(Select_img); DeleteObject(CC_img); DeleteObject(Missile_img);
	 DeleteObject(CC_My_img); DeleteObject(CC_Enemy_img);
}

void Attack_Missile()
{
	HPEN hPen, oldPen;
	int draw_count = 0;

	hPen = CreatePen(PS_SOLID, 4, RGB(255, 255, 255));
	oldPen = (HPEN)SelectObject(memdc, hPen);

	Missile.Attack_Time++;
	for (int j = 0; j < Stage_P_count; j++)
	{
		if (!Stage1 && Missile.Occ_State == 0) // �� �����̸�
		{
			if (Missile.Attack_Time >= 30)
			{
				if (Missile.Rect.left < Planet[j].Army.x && Planet[j].Army.x < Missile.Rect.right && Missile.Rect.top < Planet[j].Army.y && Planet[j].Army.y < Missile.Rect.bottom)
				{
					while (draw_count < 100)
					{
						draw_count++;
						MoveToEx(memdc, Missile.shot.x, Missile.shot.y, NULL);
						LineTo(memdc, Planet[j].Army.x, Planet[j].Army.y);
						BitBlt(hdc, 0, 0, rectView.right, rectView.bottom, memdc, 0, 0, SRCCOPY);
					}
					Planet[j].Army.popula -= 100;
					if (Planet[j].Army.popula < 10)
					{
						Planet[j].Army.popula = 0;	  // �����̴������ �ٽ� �������
						Planet[j].Army.x = Planet[j].Rect.left + ((Planet[j].Rect.right - Planet[j].Rect.left) / 2);
						Planet[j].Army.y = Planet[j].Rect.top + ((Planet[j].Rect.bottom - Planet[j].Rect.top) / 2);
						Planet[j].Army.Move = FALSE;
					}
					Missile.Attack_Time = 0;
					draw_count = 0;
					break;
				}
				else if (Missile.Rect.left < Planet[j].E_Army.x && Planet[j].E_Army.x < Missile.Rect.right && Missile.Rect.top < Planet[j].E_Army.y && Planet[j].E_Army.y < Missile.Rect.bottom)
				{
					while (draw_count < 50)
					{
						draw_count++;
						MoveToEx(memdc, Missile.shot.x, Missile.shot.y, NULL);
						LineTo(memdc, Planet[j].E_Army.x, Planet[j].E_Army.y);
						BitBlt(hdc, 0, 0, rectView.right, rectView.bottom, memdc, 0, 0, SRCCOPY);
					}
					Planet[j].E_Army.popula -= 100;
					if (Planet[j].E_Army.popula < 10)
					{
						Planet[j].E_Army.popula = 0;	  // �����̴������ �ٽ� �������
						Planet[j].E_Army.x = Planet[j].Rect.left + ((Planet[j].Rect.right - Planet[j].Rect.left) / 2);
						Planet[j].E_Army.y = Planet[j].Rect.top + ((Planet[j].Rect.bottom - Planet[j].Rect.top) / 2);
						Planet[j].E_Army.Move = FALSE;
					}
					Missile.Attack_Time = 0;
					draw_count = 0;
					break;
				}
			}
		}
		if (Missile.Occ_State == 1) // �Ʊ��� ������ �����̸�
		{
			if (Missile.Attack_Time >= 30)
			{
				if (Missile.Rect.left < Planet[j].E_Army.x && Planet[j].E_Army.x < Missile.Rect.right && Missile.Rect.top < Planet[j].E_Army.y && Planet[j].E_Army.y < Missile.Rect.bottom)
				{
					while (draw_count < 50)
					{
						draw_count++;
						MoveToEx(memdc, Missile.shot.x, Missile.shot.y, NULL);
						LineTo(memdc, Planet[j].E_Army.x, Planet[j].E_Army.y);
						BitBlt(hdc, 0, 0, rectView.right, rectView.bottom, memdc, 0, 0, SRCCOPY);
					}
					Planet[j].E_Army.popula -= 100;
					if (Planet[j].E_Army.popula < 10)
					{
						Planet[j].E_Army.popula = 0;	  // �����̴������ �ٽ� �������
						Planet[j].E_Army.x = Planet[j].Rect.left + ((Planet[j].Rect.right - Planet[j].Rect.left) / 2);
						Planet[j].E_Army.y = Planet[j].Rect.top + ((Planet[j].Rect.bottom - Planet[j].Rect.top) / 2);
						Planet[j].E_Army.Move = FALSE;
					}
					Missile.Attack_Time = 0;
					draw_count = 0;
					break;
				}
			}
		}
		if (Missile.Occ_State == 2)	//������ ������ ���¸�
		{
			if (Missile.Attack_Time >= 30)
			{
				if (Missile.Rect.left < Planet[j].Army.x && Planet[j].Army.x < Missile.Rect.right && Missile.Rect.top < Planet[j].Army.y && Planet[j].Army.y < Missile.Rect.bottom)
				{
					while (draw_count < 100)
					{
						draw_count++;
						MoveToEx(memdc, Missile.shot.x, Missile.shot.y, NULL);
						LineTo(memdc, Planet[j].Army.x, Planet[j].Army.y);
						BitBlt(hdc, 0, 0, rectView.right, rectView.bottom, memdc, 0, 0, SRCCOPY);
					}
					Planet[j].Army.popula -= 100;
					if (Planet[j].Army.popula < 10)
					{
						Planet[j].Army.popula = 0;	  // �����̴������ �ٽ� �������
						Planet[j].Army.x = Planet[j].Rect.left + ((Planet[j].Rect.right - Planet[j].Rect.left) / 2);
						Planet[j].Army.y = Planet[j].Rect.top + ((Planet[j].Rect.bottom - Planet[j].Rect.top) / 2);
						Planet[j].Army.Move = FALSE;
					}
					Missile.Attack_Time = 0;
					draw_count = 0;
					break;
				}
			}
		}
	}
	SelectObject(memdc, oldPen);
	DeleteObject(hPen);
	
}

void Print_Max()
{
	RECT Max_rect, Now_rect;
	int Max = 0, Now = 0;
	char buffer_max[10], buffer_now[10];

	Max_rect.left = 525;  Max_rect.right = Max_rect.left + 20;
	Max_rect.top = 35;  Max_rect.bottom = Max_rect.top + 20;

	Now_rect.left = 475;  Now_rect.right = Now_rect.left + 20;
	Now_rect.top = 35;  Now_rect.bottom = Now_rect.top + 20;

	for (int i = 0; i < Stage_P_count; i++)
	{
		if (Planet[i].Occ_State == 1)
		{
			if (Planet[i].More_BIG == 1)
			{
				Max += 500;
				Now += Planet[i].Army.popula;
			}
			else
			{
				Max += 300;
				Now += Planet[i].Army.popula;
			}
		}
	}
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(255, 255, 255));
	itoa(Now/10, buffer_now, 10);
	itoa(Max/10, buffer_max, 10);
	DrawText(hdc, buffer_now, -1, &Now_rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	TextOut(hdc, 500, 35, "/", 1);
	DrawText(hdc, buffer_max, -1, &Max_rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	/*BitBlt(hdc, 0, 0, rectView.right, rectView.bottom, memdc, 0, 0, SRCCOPY);*/
}

void Clear_check()
{
	int Occ_count = 0;
	int Lose_count = 0;
	for (int i = 0; i < Stage_P_count; i++)
	{
		if (Planet[i].Occ_State == 1)
			Occ_count++;
		else if(Planet[i].Occ_State == 2)
			Lose_count++;
	}
	if (Occ_count == Stage_P_count)
	{
		if (Stage1)
		{
			Stage1 = FALSE; Stage2 = TRUE;
			Stage2_init_();
		}
		else if (Stage2)
		{
			Stage2 = FALSE; Stage3 = TRUE;
			Stage3_init_();
		}
		else if (Stage3)
		{
			PostQuitMessage(0);
			return;
		}
	}
	else if (Lose_count == Stage_P_count)
	{
		Game_Start = FALSE;
		Stage1 = FALSE; Stage2 = FALSE; Stage3 = FALSE;
	}
	else
	{
		Occ_count = 0;
		Lose_count = 0;
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;

	switch (iMessage)
	{
	case WM_CREATE:
		GetClientRect(hWnd, &rectView);

		Game_Start = FALSE;
		
		SetTimer(hWnd, 1, 1, NULL);
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		memdc = CreateCompatibleDC(hdc);
		tmp_dc = CreateCompatibleDC(memdc);
		hBit = CreateCompatibleBitmap(hdc, rectView.right, rectView.bottom);
		oldBit = (HBITMAP)SelectObject(memdc, hBit);

		Start_Main_img = (HBITMAP)LoadBitmap(g_hlnst, MAKEINTRESOURCE(IDB_BITMAP15));
		GetObject(Start_Main_img, sizeof(BITMAP), &Start_Main_size);
		if (!Game_Start)
		{
			SetStretchBltMode(memdc, COLORONCOLOR);
			SelectObject(tmp_dc, Start_Main_img);
			StretchBlt(memdc, rectView.left, rectView.top,rectView.right,rectView.bottom, tmp_dc, 0, 0, Start_Main_size.bmWidth, Start_Main_size.bmHeight, SRCCOPY);
			BitBlt(hdc, 0, 0, rectView.right, rectView.bottom, memdc, 0, 0, SRCCOPY);
		}
		else
		{
			Stage();

			srand((unsigned)time(NULL));
			Random_move = 50 + rand() % 90;

			Enemy_Move();

			Create_Army();

			Attack_Missile();

			Print_Max();

			Clear_check();
		}

		SelectObject(memdc, oldBit);
		DeleteObject(hBit);

		DeleteObject(Start_Main_img);

		DeleteDC(tmp_dc); DeleteDC(memdc);
		EndPaint(hWnd, &ps);

		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_STAGE_STAGE1:
			Stage1_init_();
			Stage1 = TRUE; Stage2 = FALSE; Stage3 = FALSE;
			break;
		case ID_STAGE_STAGE2:
			Stage2_init_();
			Stage1 = FALSE; Stage2 = TRUE; Stage3 = FALSE;
			break;
		case ID_STAGE_STAGE3:
			Stage3_init_();
			Stage1 = FALSE; Stage2 = FALSE; Stage3 = TRUE;
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);

		for (int i = 0; i < Stage_P_count; i++)
		{
			// �Ʊ� ������ �༺ ���ο� ��ǥ�� ���� 
			if (Planet[i].Occ_State == 1 && !Planet[i].Army.Move && Planet[i].Rect.left < mx && mx < Planet[i].Rect.right && Planet[i].Rect.top < my && my < Planet[i].Rect.bottom)
			{
				Mouse_ON = TRUE;
				Start_Planet = i;
				mx = Planet[i].Rect.left + ((Planet[i].Rect.right - Planet[i].Rect.left) / 2);;
				my = Planet[i].Rect.top + ((Planet[i].Rect.bottom - Planet[i].Rect.top) / 2);

				Endx = mx; // Endx�� mx�� ���� �߾� �ʱ�ȭ  (�����ָ� �̻�����)
				Endy = my;

				break;
			}
		}
		
		break;

	case WM_MOUSEMOVE:

		Endx = LOWORD(lParam);
		Endy = HIWORD(lParam);
		for (int i = 0; i < Stage_P_count; i++)
		{
			if (Planet[i].Rect.left < Endx && Endx < Planet[i].Rect.right && Planet[i].Rect.top < Endy && Endy < Planet[i].Rect.bottom)
			{
				End_Planet = i;
				Endx = Planet[i].Rect.left + ((Planet[i].Rect.right - Planet[i].Rect.left) / 2);
				Endy = Planet[i].Rect.top + ((Planet[i].Rect.bottom - Planet[i].Rect.top) / 2);

				// End ��ǥ�� �� �߾����� ����� ���� �ؽ�Ʈ�� �߾����� �����°� ���ϰ� �ҵ�
				break;
			}
			else
				End_Planet = Start_Planet;
		}

		//���콺 �巡�װ� �̻����� �����״ٸ�_����Ʈ�̹��� �׸������ѿ뵵
		if (!Stage1 && Missile.shot.x - 25 < Endx && Endx < Missile.shot.x + 25 && Missile.shot.y - 25 < Endy < Missile.shot.y + 25)
		{
			if (Stage2)
			{
				Endx = Missile.Rect.left + ((Missile.Rect.right - Missile.Rect.left) / 2);
				Endy = Missile.Rect.top + ((Missile.Rect.bottom - Missile.Rect.top) / 2);
			}
		}
		
		break;
	case WM_LBUTTONUP:
		Mouse_ON = FALSE;
		if (Start_Planet > -1 && End_Planet > -1 && Start_Planet != End_Planet) // ���� �༺�̶� �� �༺�� �ٸ���
		{
			if (!Planet[Start_Planet].Army.Move)	// ������ �����ϴ� �༺�� ����� startplanet�� �޾ƿ��ϱ�
			{										// ���� �ݺ������� üũ ���� �ʿ䰡 ����
				Planet[Start_Planet].Army.degree = TanGetAngle(mx,my,Endx,Endy);
				
				Planet[Start_Planet].Army.Move = TRUE;
			}
		}
		
		// ���콺 �巡�װ� �̻����༺�� ����Ų�ٸ�_ ������ ���ϱ� ���� �뵵
		if (!Stage1 && Missile.shot.x - 15 < Endx && Endx < Missile.shot.x + 15 && Missile.shot.y - 15 < Endy < Missile.shot.y + 15)
		{
			Planet[Start_Planet].Army.degree = TanGetAngle(Planet[Start_Planet].Army.x, Planet[Start_Planet].Army.y, Missile.shot.x, Missile.shot.y);
			Planet[Start_Planet].Army.Move = TRUE;
			Missile_Occ = TRUE;
		}

		Start_Planet = -1;
		End_Planet = -1;
		
		break;
	case WM_TIMER:
		Agl += 0.05;	// ��Ʈ�� ȸ�� ���� 
		speed = 5;
		//�� �༺�� �浹üũ
		for (int i = 0; i < Stage_P_count; i++)
		{
			// ���콺�� ������ ���� �� ����ؾ� army.move =true �ϱ�  ���ǹ� �̴�� �ָ�ɵ�
			if (Planet[i].Army.Move) // �༺ ���縦 �̵�
			{
				Planet[i].Army.x += (cos(Planet[i].Army.degree)) * speed;	// �������������� ������ �����ؼ� �����̴ϱ� = ������
				Planet[i].Army.y -= (sin(Planet[i].Army.degree)) * speed;	// ����� �̵����ؾ��ϴ°Ŷ� +=

				for (int j = 0; j < Stage_P_count; j++)
				{
					// �����̰� �ִ� ������ �༺�� �ƴϰ� �ٸ� �༺�� �浹�ߴٸ�
					if (i != j &&  Planet[j].Rect.left < Planet[i].Army.x &&Planet[i].Army.x < Planet[j].Rect.right && Planet[j].Rect.top < Planet[i].Army.y && Planet[i].Army.y < Planet[j].Rect.bottom)
					{
						if (Planet[j].Occ_State == 2)  // 1 . ������ ������ �༺
						{
							if (!Planet[j].E_Army.Move)
							{
								if (Planet[j].E_Army.popula < Planet[i].Army.popula)  // �� ���簡 �� ���ٸ� 
								{
									Planet[i].Army.popula -= Planet[j].E_Army.popula;
									Planet[j].E_Army.popula = 0;

									if (Planet[j].Occ_Time != 0)
									{
										// ���ɽð��� ���ҽ�Ű��  0�̵ɋ����� �浹�� ���縦 ���ڸ��� �ӹ����� �Ѵ�
										Planet[j].Occ_Time--;
										Planet[i].Army.x = Planet[j].Rect.left + ((Planet[j].Rect.right - Planet[j].Rect.left) / 2);
										Planet[i].Army.y = Planet[j].Rect.top + ((Planet[j].Rect.bottom - Planet[j].Rect.top) / 2);

									}
									else if (Planet[j].Occ_Time <= 0)
									{
										if(Planet[j].More_BIG)
											Planet[j].Occ_Time = 160;
										else
											Planet[j].Occ_Time = 80;
										Planet[j].Occ_State = 1;
										Planet[j].Army.popula += Planet[i].Army.popula;		//�༺ ���� �����ְ�
										Planet[i].Army.popula = 0;	  // �����̴������ �ٽ� �������
										Planet[i].Army.x = Planet[i].Rect.left + ((Planet[i].Rect.right - Planet[i].Rect.left) / 2);
										Planet[i].Army.y = Planet[i].Rect.top + ((Planet[i].Rect.bottom - Planet[i].Rect.top) / 2);
										Planet[i].Army.Move = FALSE;
									}
								}
								else if ((Planet[j].E_Army.popula >= Planet[i].Army.popula)) // �� ���簡 �� ���ų� ���ٸ�
								{
									Planet[j].E_Army.popula -= Planet[i].Army.popula;
									Planet[i].Army.popula = 0;	  // �����̴������ �ٽ� �������
									Planet[i].Army.x = Planet[i].Rect.left + ((Planet[i].Rect.right - Planet[i].Rect.left) / 2);
									Planet[i].Army.y = Planet[i].Rect.top + ((Planet[i].Rect.bottom - Planet[i].Rect.top) / 2);
									Planet[i].Army.Move = FALSE;
								}
							}
							else	//������ ���簡 �����̰��ִٸ�
							{
								if (Planet[j].Occ_Time != 0)
								{
									// ���ɽð��� ���ҽ�Ű��  0�̵ɋ����� �浹�� ���縦 ���ڸ��� �ӹ����� �Ѵ�
									Planet[j].Occ_Time--;
									Planet[i].Army.x = Planet[j].Rect.left + ((Planet[j].Rect.right - Planet[j].Rect.left) / 2);
									Planet[i].Army.y = Planet[j].Rect.top + ((Planet[j].Rect.bottom - Planet[j].Rect.top) / 2);

								}
								else if (Planet[j].Occ_Time <= 0)
								{
									if (Planet[j].More_BIG)
										Planet[j].Occ_Time = 160;
									else
										Planet[j].Occ_Time = 80;
									Planet[j].Occ_State = 1;
									Planet[j].Army.popula += Planet[i].Army.popula;		//�༺ ���� �����ְ�
									Planet[i].Army.popula = 0;	  // �����̴������ �ٽ� �������
									Planet[i].Army.x = Planet[i].Rect.left + ((Planet[i].Rect.right - Planet[i].Rect.left) / 2);
									Planet[i].Army.y = Planet[i].Rect.top + ((Planet[i].Rect.bottom - Planet[i].Rect.top) / 2);
									Planet[i].Army.Move = FALSE;
								}
							}
						}
						else if (Planet[j].Occ_State == 0)	// 2. �� �༺
						{
							if (Planet[j].Occ_Time != 0)
							{
								// ���ɽð��� ���ҽ�Ű��  0�̵ɋ����� �浹�� ���縦 ���ڸ��� �ӹ����� �Ѵ�
								Planet[j].Occ_Time--;
								Planet[i].Army.x = Planet[j].Rect.left + ((Planet[j].Rect.right - Planet[j].Rect.left) / 2);
								Planet[i].Army.y = Planet[j].Rect.top + ((Planet[j].Rect.bottom - Planet[j].Rect.top) / 2);

							}
							else if (Planet[j].Occ_Time <= 0)
							{
								if (Planet[j].More_BIG)
									Planet[j].Occ_Time = 160;
								else
									Planet[j].Occ_Time = 80;
								Planet[j].Occ_State = 1;
								Planet[j].Army.popula += Planet[i].Army.popula;		//�༺ ���� �����ְ�
								Planet[i].Army.popula = 0;	  // �����̴������ �ٽ� �������
								Planet[i].Army.x = Planet[i].Rect.left + ((Planet[i].Rect.right - Planet[i].Rect.left) / 2);
								Planet[i].Army.y = Planet[i].Rect.top + ((Planet[i].Rect.bottom - Planet[i].Rect.top) / 2);
								Planet[i].Army.Move = FALSE;
							}
						}
						else		// 2�� �ƴϰ� 0 �� �ƴϸ� �Ʊ� �༺�̴ϱ� �����ְ� �׳� ���ư���
						{
							Planet[j].Army.popula += Planet[i].Army.popula;		//�༺ ���� �����ְ�
							Planet[i].Army.popula = 0;	  // �����̴������ �ٽ� �������
							Planet[i].Army.x = Planet[i].Rect.left + ((Planet[i].Rect.right - Planet[i].Rect.left) / 2);
							Planet[i].Army.y = Planet[i].Rect.top + ((Planet[i].Rect.bottom - Planet[i].Rect.top) / 2);
							Planet[i].Army.Move = FALSE;
						}
					}
				}
			}
		}

		//�̻��� - �Ʊ����� �浹üũ 
		for (int m = 0; m < Stage_P_count; m++)
		{
			if (!Stage1 && Missile_Occ && Missile.shot.x - 20 < Planet[m].Army.x && Planet[m].Army.x < Missile.shot.x + 20 && Missile.shot.y - 20 < Planet[m].Army.y &&Planet[m].Army.y < Missile.shot.y + 20)
			{
				Missile.Occ_State = 1;
				Missile_Occ = FALSE;
				Planet[m].Army.x = Planet[m].Rect.left + ((Planet[m].Rect.right - Planet[m].Rect.left) / 2);
				Planet[m].Army.y = Planet[m].Rect.top + ((Planet[m].Rect.bottom - Planet[m].Rect.top) / 2);
				Planet[m].Army.Move = FALSE;
			}
		}

		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_SPACE:
			if (!Game_Start)
			{
				Game_Start = TRUE;
				Stage1 = TRUE;
				Stage2 = FALSE; Stage3 = FALSE;
				Stage1_init_();
			}
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}
