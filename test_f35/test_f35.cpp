// test_f35.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "test_f35.h"

#include <f35/GraphicsRenderer.h>
#include <f35/TestGraphics.h>
#include <f35/SimpleChartGraphics.h>
#include <f35/SampleChartDataSeries.h>
#include <f35/SimpleLineChartLegend.h>


#define _USE_MATH_DEFINES
#include <math.h>

#define MAX_LOADSTRING 100

// �O���[�o���ϐ�:
HINSTANCE hInst;                                // ���݂̃C���^�[�t�F�C�X
WCHAR szTitle[MAX_LOADSTRING];                  // �^�C�g�� �o�[�̃e�L�X�g
WCHAR szWindowClass[MAX_LOADSTRING];            // ���C�� �E�B���h�E �N���X��

// ���̃R�[�h ���W���[���Ɋ܂܂��֐��̐錾��]�����܂�:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

fxxxv::GraphicsRenderer *pRenderer = NULL;
fxxxv::TestGraphics graphs[100];
fxxxv::GraphicsContainer container;
fxxxv::SimpleChartGraphics chart_graph;
fxxxv::SampleChartDataSeries chart_data;
fxxxv::SimpleLineChartLegend chart_legend;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: �����ɃR�[�h��}�����Ă��������B

    // �O���[�o������������������Ă��܂��B
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TEST_F35, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // �A�v���P�[�V�����̏����������s���܂�:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TEST_F35));

    MSG msg;

    // ���C�� ���b�Z�[�W ���[�v:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  �֐�: MyRegisterClass()
//
//  �ړI: �E�B���h�E �N���X��o�^���܂��B
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TEST_F35));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TEST_F35);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   �֐�: InitInstance(HINSTANCE, int)
//
//   �ړI: �C���X�^���X �n���h����ۑ����āA���C�� �E�B���h�E���쐬���܂��B
//
//   �R�����g:
//
//        ���̊֐��ŁA�O���[�o���ϐ��ŃC���X�^���X �n���h����ۑ����A
//        ���C�� �v���O���� �E�B���h�E���쐬����ѕ\�����܂��B
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // �O���[�o���ϐ��ɃC���X�^���X�������i�[���܂��B

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

static void init_graphics(HWND hWnd)
{
	pRenderer = new fxxxv::GraphicsRenderer(hWnd);
	pRenderer->Init();
	pRenderer->EnableAutoErase(D2D1::ColorF(D2D1::ColorF::DarkBlue, 0.1f));
	for (auto &g : graphs) container.AddGraphics(&g);
	pRenderer->AddGraphics(&container);

	auto size = pRenderer->GetSize();
	for (auto &g : graphs)
	{
		FLOAT x = size.width * ::rand() / RAND_MAX;
		FLOAT y = size.height * ::rand() / RAND_MAX;
		g.SetPosition(x, y);
		FLOAT angle = (::rand() > RAND_MAX / 2) ? 0 : 90;
		g.SetRotation(angle);
		FLOAT scale = 1.0f + 3.0f * ::rand() / RAND_MAX;
		g.SetScale(scale);
	}
	container.SetPosition(size.width / 2, size.height / 2);

	chart_data.SetLength(20);
	chart_legend.SetFillColor(D2D1::ColorF::Olive);
	chart_legend.SetLineColor(D2D1::ColorF::WhiteSmoke);
	chart_legend.ShowMarker(TRUE);
	chart_graph.AddDataSeries(_T("sample"), &chart_data, &chart_legend);
	chart_graph.SetChartAreaSize(size.width / 2, size.height / 2);
	chart_graph.SetPosition(-size.width / 4, -size.height / 4);

	chart_graph.SetForegroundColor(D2D1::ColorF::DarkGreen);

	container.AddGraphics(&chart_graph);
}

static void paint_graphics()
{
	if (pRenderer) {
		chart_data.Update();
		//auto min = chart_data.GetMinDataValues();
		//auto max = chart_data.GetMaxDataValues();
		//chart_graph.SetChartAxisX(min.x, max.x);
		//chart_graph.SetChartAxisY(min.y, max.y);
		chart_graph.AdjustChartAxesAuto();
	
		SYSTEMTIME t; ::GetSystemTime(&t);
		FLOAT deg = 6.0f * (t.wSecond + 0.001f * t.wMilliseconds);
		container.SetRotation(deg);
		pRenderer->Update();
		pRenderer->Render();
	}
}

static void destroy_graphic()
{
	pRenderer->Destroy();
	delete pRenderer;
	pRenderer = NULL;

}

//
//  �֐�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �ړI:    ���C�� �E�B���h�E�̃��b�Z�[�W���������܂��B
//
//  WM_COMMAND  - �A�v���P�[�V���� ���j���[�̏���
//  WM_PAINT    - ���C�� �E�B���h�E�̕`��
//  WM_DESTROY  - ���~���b�Z�[�W��\�����Ė߂�
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		init_graphics(hWnd);
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // �I�����ꂽ���j���[�̉��:
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
			paint_graphics();
			//PAINTSTRUCT ps;
            //HDC hdc = BeginPaint(hWnd, &ps);
            //// TODO: HDC ���g�p����`��R�[�h�������ɒǉ����Ă�������...
            //EndPaint(hWnd, &ps);
        }
        break;
	case WM_SIZE:
	{
		if (pRenderer) pRenderer->Resize();
		break;
	}
    case WM_DESTROY:
        PostQuitMessage(0);
		destroy_graphic();
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// �o�[�W�������{�b�N�X�̃��b�Z�[�W �n���h���[�ł��B
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
