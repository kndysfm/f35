// test_f35.cpp : アプリケーションのエントリ ポイントを定義します。
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

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
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

    // TODO: ここにコードを挿入してください。

    // グローバル文字列を初期化しています。
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TEST_F35, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーションの初期化を実行します:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TEST_F35));

    MSG msg;

    // メイン メッセージ ループ:
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
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
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
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します。
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス処理を格納します。

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
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウの描画
//  WM_DESTROY  - 中止メッセージを表示して戻る
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
            // 選択されたメニューの解析:
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
            //// TODO: HDC を使用する描画コードをここに追加してください...
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

// バージョン情報ボックスのメッセージ ハンドラーです。
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
