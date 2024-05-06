#include "../pch.h"
#include "../include/D3D11Window.hpp"
#include "../include/FileManager.h"
#include <chrono>

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

float HSV_RAINBOW_SPEED = 0.001f;
static float HSV_RAINBOW_HUE = 0;
void SV_RAINBOW(float saturation, float value, float opacity)
{
	using namespace DX11_Base;
	HSV_RAINBOW_HUE -= HSV_RAINBOW_SPEED;
	if (HSV_RAINBOW_HUE < -1.f) HSV_RAINBOW_HUE += 1.f;
	for (int i = 0; i < 860; i++)
	{
		float hue = HSV_RAINBOW_HUE + (1.f / (float)860) * i;
		if (hue < 0.f) hue += 1.f;
		g_Menu->dbg_RAINBOW = ImColor::HSV(hue, (saturation / 255), (value / 255), (opacity / 255));
	}
}

typedef BOOL(WINAPI* hk_SetCursorPos)(int, int);
hk_SetCursorPos origSetCursorPos = NULL;

BOOL WINAPI HOOK_SetCursorPos(int X, int Y)
{
	if (DX11_Base::g_GameVariables->m_ShowMenu)
		return FALSE;

	return origSetCursorPos(X, Y);
}

bool HookCursor()
{
	if (MH_CreateHook(&SetCursorPos, &HOOK_SetCursorPos, reinterpret_cast<LPVOID*>(&origSetCursorPos)) != MH_OK)
		return FALSE;

	if (MH_EnableHook(&SetCursorPos) != MH_OK)
		return FALSE;

	return TRUE;
}

void NoClip(WPARAM wParam)
{
	if (Config.pPlayerLocationAddress && Config.pPlayerCameraRotationAddress)
	{
		Vector3D_Data* LocationData = (Vector3D_Data*)Config.pPlayerLocationAddress;
		Vector3D_Data* RotationData = (Vector3D_Data*)Config.pPlayerCameraRotationAddress;

		switch (wParam)
		{
		//Key W
		case 0x57:
			LocationData->X += RotationData->X / 3 * Config.fPlayerSpeed;
			LocationData->Z += RotationData->Z / 3 * Config.fPlayerSpeed;
			LocationData->Y += RotationData->Y / 3 * Config.fPlayerSpeed;
			break;
		//Key S
		case 0x53:
			LocationData->X += RotationData->X / 3 * Config.fPlayerSpeed * -1;
			LocationData->Z += RotationData->Z / 3 * Config.fPlayerSpeed * -1;
			LocationData->Y += RotationData->Y / 3 * Config.fPlayerSpeed * -1;
			break;
		//Key A
		case 0x41:
			LocationData->X -= RotationData->Z / 3 * Config.fPlayerSpeed;
			LocationData->Z += RotationData->X / 3 * Config.fPlayerSpeed;
			break;
		//Key D
		case 0x44:
			LocationData->X -= RotationData->Z / 3 * Config.fPlayerSpeed * -1;
			LocationData->Z += RotationData->X / 3 * Config.fPlayerSpeed * -1;
			break;
		}
	}
}

namespace DX11_Base {
	static uint64_t* MethodsTable = NULL;

	static bool bWndProc_Focus = true;
	LRESULT D3D11Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		//Out of focus
		case WM_KILLFOCUS:
			bWndProc_Focus = false;
			break;
		case WM_SETFOCUS:
			bWndProc_Focus = true;
			break;
		//Minimized
		case WM_SIZE:
			if (wParam == SIZE_MINIMIZED)
			{
				bWndProc_Focus = false;
			}
			else
			{
				bWndProc_Focus = true;
			}
			break;
		case WM_KEYDOWN:
			if (wParam == 0x57 || wParam == 0x53 || wParam == 0x41 || wParam == 0x44)
			{
				if (Config.bNoClip)
				{
					NoClip(wParam);
					return TRUE;
				}
			}
			break;
		}

		if (!bWndProc_Focus)
		{
			return CallWindowProc((WNDPROC)g_D3D11Window->m_OldWndProc, hWnd, msg, wParam, lParam);
		}

		if (g_GameVariables->m_ShowMenu)
		{
			ImGui_ImplWin32_WndProcHandler((HWND)g_D3D11Window->m_OldWndProc, msg, wParam, lParam);
			return TRUE;
		}

		return CallWindowProc((WNDPROC)g_D3D11Window->m_OldWndProc, hWnd, msg, wParam, lParam);
	}

	/// <summary>
	/// INITIALIZE
	/// </summary>
	bool D3D11Window::Hook()
	{
		if (InitHook()) {
			HookCursor();
			CreateHook(8, (void**)&oIDXGISwapChainPresent, HookPresent);
			CreateHook(12, (void**)&oID3D11DrawIndexed, MJDrawIndexed);
			Sleep(1000);

			return TRUE;
		}

		return FALSE;
	}

	bool D3D11Window::CreateHook(uint16_t Index, void** Original, void* Function)
	{
		assert(Index >= 0 && Original != NULL && Function != NULL);
		void* target = (void*)MethodsTable[Index];
		if (MH_CreateHook(target, Function, Original) != MH_OK || MH_EnableHook(target) != MH_OK) {
			return FALSE;
		}
		return TRUE;
	}

	bool D3D11Window::InitHook()
	{
		if (!InitWindow())
			return FALSE;

		HMODULE D3D11Module = GetModuleHandleA("d3d11.dll");

		D3D_FEATURE_LEVEL FeatureLevel;
		const D3D_FEATURE_LEVEL FeatureLevels[] = { D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0 };

		DXGI_RATIONAL RefreshRate;
		RefreshRate.Numerator = 30;
		RefreshRate.Denominator = 1;

		DXGI_MODE_DESC BufferDesc;
		BufferDesc.Width = 100;
		BufferDesc.Height = 100;
		BufferDesc.RefreshRate = RefreshRate;
		BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		DXGI_SAMPLE_DESC SampleDesc;
		SampleDesc.Count = 1;
		SampleDesc.Quality = 0;

		DXGI_SWAP_CHAIN_DESC SwapChainDesc;
		SwapChainDesc.BufferDesc = BufferDesc;
		SwapChainDesc.SampleDesc = SampleDesc;
		SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SwapChainDesc.BufferCount = 1;
		SwapChainDesc.OutputWindow = WindowHwnd;
		SwapChainDesc.Windowed = 1;
		SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		IDXGISwapChain* SwapChain;
		ID3D11Device* Device;
		ID3D11DeviceContext* Context;
		if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, FeatureLevels, 1, D3D11_SDK_VERSION, &SwapChainDesc, &SwapChain, &Device, &FeatureLevel, &Context) < 0)
		{
			DeleteWindow();
			return FALSE;
		}

		MethodsTable = (uint64_t*)::calloc(205, sizeof(uint64_t));
		memcpy(MethodsTable, *(uint64_t**)SwapChain, 18 * sizeof(uint64_t));
		memcpy(MethodsTable + 18, *(uint64_t**)Device, 43 * sizeof(uint64_t));
		memcpy(MethodsTable + 18 + 43, *(uint64_t**)Context, 144 * sizeof(uint64_t));
		Sleep(1000);

		//	INIT NOTICE
		Beep(300, 300);

		MH_Initialize();
		SwapChain->Release();
		SwapChain = NULL;
		Device->Release();
		Device = NULL;
		Context->Release();
		Context = NULL;
		DeleteWindow();
		return TRUE;
	}

	bool D3D11Window::InitWindow()
	{
		WindowClass.cbSize = sizeof(WNDCLASSEX);
		WindowClass.style = CS_HREDRAW | CS_VREDRAW;
		WindowClass.lpfnWndProc = DefWindowProc;
		WindowClass.cbClsExtra = 0;
		WindowClass.cbWndExtra = 0;
		WindowClass.hInstance = GetModuleHandle(NULL);
		WindowClass.hIcon = NULL;
		WindowClass.hCursor = NULL;
		WindowClass.hbrBackground = NULL;
		WindowClass.lpszMenuName = NULL;
		WindowClass.lpszClassName = L"MJ";
		WindowClass.hIconSm = NULL;
		RegisterClassEx(&WindowClass);
		WindowHwnd = CreateWindow(WindowClass.lpszClassName, L"DX11 Window", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, WindowClass.hInstance, NULL);
		if (WindowHwnd == NULL) {
			return FALSE;
		}

		return TRUE;
	}

	bool D3D11Window::DeleteWindow()
	{
		DestroyWindow(WindowHwnd);
		UnregisterClass(WindowClass.lpszClassName, WindowClass.hInstance);
		if (WindowHwnd != NULL) {
			return FALSE;
		}

		return TRUE;
	}

	void D3D11Window::LoadFont()
	{
		std::string Path = g_FileManager.GetFontFile();
		if (Path.empty())
		{
			bFontLoaded = true;
			return;
		}

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		float fFontSize = g_FileManager.Language["FontSize"].get<float>();

		ImFontConfig config;
		config.OversampleV = 2;
		config.OversampleH = 2;

		ImVector<ImWchar> ranges;
		ImFontGlyphRangesBuilder builder;
		builder.AddRanges(io.Fonts->GetGlyphRangesKorean());
		builder.AddRanges(io.Fonts->GetGlyphRangesJapanese());
		builder.AddRanges(io.Fonts->GetGlyphRangesChineseFull());
		builder.AddRanges(io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
		builder.AddRanges(io.Fonts->GetGlyphRangesCyrillic());
		builder.AddRanges(io.Fonts->GetGlyphRangesThai());
		builder.AddRanges(io.Fonts->GetGlyphRangesGreek());
		builder.AddRanges(io.Fonts->GetGlyphRangesVietnamese());
		builder.BuildRanges(&ranges);

		try
		{
			io.Fonts->AddFontFromFileTTF(Path.c_str(), fFontSize, &config, ranges.Data);
			io.Fonts->Build();
#ifdef DEBUG_CONSOLE
			printf("[D3D11Window][LoadFont] Successfully added new font\n");
#endif
		}
		catch (const std::exception& ex)
		{
#ifdef DEBUG_CONSOLE
			printf("[D3D11Window][LoadFont] Failed to AddFontFromFileTTF. Exception: %s\n", ex.what());
#endif
		}

		bFontLoaded = true;
	}

	bool D3D11Window::Init(IDXGISwapChain* swapChain)
	{
		if (SUCCEEDED(swapChain->GetDevice(__uuidof(ID3D11Device), (void**)&m_Device))) {
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.WantCaptureMouse || io.WantTextInput || io.WantCaptureKeyboard;

			if (!bFontLoaded) LoadFont();

			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			io.IniFilename = NULL;
			m_Device->GetImmediateContext(&m_DeviceContext);

			DXGI_SWAP_CHAIN_DESC Desc;
			swapChain->GetDesc(&Desc);
			g_GameVariables->g_GameWindow = Desc.OutputWindow;

			ID3D11Texture2D* BackBuffer;
			swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&BackBuffer);
			m_Device->CreateRenderTargetView(BackBuffer, NULL, &m_RenderTargetView);
			BackBuffer->Release();

			ImGui_ImplWin32_Init(g_GameVariables->g_GameWindow);
			ImGui_ImplDX11_Init(m_Device, m_DeviceContext);
			ImGui_ImplDX11_CreateDeviceObjects();
			//ImGui::GetIO().ImeWindowHandle = g_GameVariables->g_GameWindow;
			m_OldWndProc = (WNDPROC)SetWindowLongPtr(g_GameVariables->g_GameWindow, GWLP_WNDPROC, (__int3264)(LONG_PTR)WndProc);
			b_ImGui_Initialized = TRUE;
			pImGui = GImGui;
			pViewport = pImGui->Viewports[0];

			return 1;
		}
		b_ImGui_Initialized = FALSE;
		return 0;
	}

	/// <summary>
	/// RENDER LOOP
	/// </summary>
	HRESULT APIENTRY D3D11Window::HookPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
	{
		if (g_KillSwitch) {
			g_Hooking->Unhook();
			g_D3D11Window->oIDXGISwapChainPresent(pSwapChain, SyncInterval, Flags);
			g_Running = FALSE;
			return 0;
		}
		g_D3D11Window->Overlay(pSwapChain);
		return g_D3D11Window->oIDXGISwapChainPresent(pSwapChain, SyncInterval, Flags);
	}

	void APIENTRY D3D11Window::MJDrawIndexed(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation) {
		return g_D3D11Window->oID3D11DrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
	}

	void D3D11Window::Overlay(IDXGISwapChain* pSwapChain)
	{
		if (!b_ImGui_Initialized)
			Init(pSwapChain);

		SV_RAINBOW(169, 169, 200);
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		//ImGui::GetIO().MouseDrawCursor = g_GameVariables->m_ShowMenu;

		//Render Menu Loop
		g_Menu->Draw();
		ImGui::EndFrame();
		ImGui::Render();
		m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, NULL);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	/// <summary>
	/// UNHOOK
	/// </summary>
	void D3D11Window::Unhook()
	{
		SetWindowLongPtr(g_GameVariables->g_GameWindow, GWLP_WNDPROC, (LONG_PTR)m_OldWndProc);
		DisableAll();
		return;
	}

	void D3D11Window::DisableHook(uint16_t Index)
	{
		assert(Index >= 0);
		MH_DisableHook((void*)MethodsTable[Index]);
		return;
	}

	void D3D11Window::DisableAll()
	{
		DisableHook(8);
		DisableHook(12);
		free(MethodsTable);
		MethodsTable = NULL;
		return;
	}

	D3D11Window::~D3D11Window()
	{
		Unhook();
	}
}