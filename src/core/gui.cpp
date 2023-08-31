#include "gui.h"
#include <stdio.h>
#include "../lib/helper.h"

extern scene mainScene;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter
);

long __stdcall WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter)
{
	if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter))
		return true;

	switch (message)
	{
	case WM_SIZE: {
		if (dgui::device && wideParameter != SIZE_MINIMIZED)
		{
			dgui::presentParameters.BackBufferWidth = LOWORD(longParameter);
			dgui::presentParameters.BackBufferHeight = HIWORD(longParameter);
			dgui::ResetDevice();
		}
	}return 0;

	case WM_SYSCOMMAND: {
		if ((wideParameter & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
	}break;

	case WM_DESTROY: {
		PostQuitMessage(0);
	}return 0;

	case WM_LBUTTONDOWN: {
		dgui::position = MAKEPOINTS(longParameter); // set click points
	}return 0;

	case WM_MOUSEMOVE: {
		if (wideParameter == MK_LBUTTON)
		{
			const auto points = MAKEPOINTS(longParameter);
			auto rect = ::RECT{ };

			GetWindowRect(dgui::window, &rect);

			rect.left += points.x - dgui::position.x;
			rect.top += points.y - dgui::position.y;

			if (dgui::position.x >= 0 &&
				dgui::position.x <= dgui::_WIDTH &&
				dgui::position.y >= 0 && dgui::position.y <= 19)
				SetWindowPos(
					dgui::window,
					HWND_TOPMOST,
					rect.left,
					rect.top,
					0, 0,
					SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER
				);
		}

	}return 0;

	}

	return DefWindowProc(window, message, wideParameter, longParameter);
}

void dgui::CreateHWindow(char* windowName) noexcept
{
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_CLASSDC;
	windowClass.lpfnWndProc = WindowProcess;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandleA(0);
	windowClass.hIcon = 0;
	windowClass.hCursor = 0;
	windowClass.hbrBackground = 0;
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = (LPCWSTR)"class002";
	windowClass.hIconSm = 0;

	RegisterClassEx(&windowClass);

	window = CreateWindowEx(
		0,
		(LPCWSTR)"class002",
		(LPCWSTR)windowName,
		WS_POPUP,
		100,
		100,
		_WIDTH,
		_HEIGHT,
		0,
		0,
		windowClass.hInstance,
		0
	);

	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);
}

void dgui::DestroyHWindow() noexcept
{
	DestroyWindow(window);
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

bool dgui::CreateDevice() noexcept
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	if (!d3d)
		return false;

	ZeroMemory(&presentParameters, sizeof(presentParameters));

	presentParameters.Windowed = TRUE;
	presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
	presentParameters.EnableAutoDepthStencil = TRUE;
	presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
	presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	if (d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		window,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&presentParameters,
		&device) < 0)
		return false;

	return true;
}

void dgui::ResetDevice() noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();

	const auto result = device->Reset(&presentParameters);

	if (result == D3DERR_INVALIDCALL)
		IM_ASSERT(0);

	ImGui_ImplDX9_CreateDeviceObjects();
}

void dgui::DestroyDevice() noexcept
{
	if (device)
	{
		device->Release();
		device = nullptr;
	}

	if (d3d)
	{
		d3d->Release();
		d3d = nullptr;
	}
}

void dgui::CreateImGui() noexcept
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ::ImGui::GetIO();

	io.IniFilename = NULL;

	ImGui::StyleColorsDark();
	ImGui::GetStyle().ItemInnerSpacing = ImVec2(2, 10);
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);
}

void dgui::DestroyImGui() noexcept
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void dgui::BeginRender() noexcept
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void dgui::EndRender() noexcept
{
	ImGui::EndFrame();

	device->SetRenderState(D3DRS_ZENABLE, FALSE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	if (device->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		device->EndScene();
	}

	device->Present(0, 0, 0, 0);
}

std::string curr{ std::string() };
std::string currComp{ std::string() };

bool CInputFloat(const char* label, const char* comment, float *v, float step1, float step2)
{
	ImGui::Text(label);
	ImGui::SameLine();

	std::string labelID = "##";
	labelID += comment;

	return ImGui::InputFloat(labelID.c_str(), v, step1, step2);
}

bool CInputInt(const char* label, const char* comment, int* v, int step1, int step2)
{
	ImGui::Text(label);
	ImGui::SameLine();

	std::string labelID = "##";
	labelID += comment;

	return ImGui::InputInt(labelID.c_str(), v, step1, step2);
}

bool CInputText(const char* label, const char* comment, const char* v, int buff_size)
{
	ImGui::Text(label);
	ImGui::SameLine();

	std::string labelID = "##";
	labelID += comment;

	return ImGui::InputText(labelID.c_str(), (char*)v, buff_size);
}

bool CBeginCombo(const char* label, const char* comment, const char *preview_value)
{
	ImGui::Text(label);
	ImGui::SameLine();

	std::string labelID = "##";
	labelID += comment;

	return ImGui::BeginCombo(labelID.c_str(), preview_value);
}

void dgui::Render() noexcept
{
	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::SetNextWindowSize({ _WIDTH, _HEIGHT });
	ImGui::Begin(
		"Dog Engine 2D",
		&isRunning,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove
	);

	std::vector<std::string> list = mainScene.getObjectList();
	if (list.size() > 0) {
		if (CBeginCombo("Scene objects", "Scene objects", curr.c_str())) {
			for (std::string name : list) {
				bool is_selected = curr == name;
				if (ImGui::Selectable(name.c_str())) {
					curr = name;
				}
				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}

	if (curr != std::string()) {
		object *currentGameObject = mainScene.getObjectByName(curr);
		int layer = currentGameObject->get_layer();
		ImGui::PushItemWidth(_WIDTH / 3); {
			if (CInputInt("Layer", "Object layer", &layer, 1, 3)) {
				currentGameObject->set_layer(layer);
			}
		}
		std::vector<base_component*> components = currentGameObject->getComponentsList();
		std::vector<std::string> nameList = std::vector<std::string>();
		for (auto& i : components) {
			nameList.push_back(i->toString());
		}
		if (CBeginCombo("Object components", "Object components", currComp.c_str())) {
			for (std::string name : nameList) {
				bool is_selected = currComp == name;
				if (ImGui::Selectable(name.c_str())) {
					currComp = name;
				}
				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		if (currComp != std::string()) {
			base_component* component = nullptr;
			for (auto& i : components) {
				if (i->toString() == currComp)
					component = i;
			}

			if (transform * trans = dynamic_cast<transform*>(component)) {
				int x = trans->getPos().x;
				int y = trans->getPos().y;
				float scalex = trans->getScale().x;
				float scaley = trans->getScale().y;
				int width = trans->getWidth();
				int height = trans->getHeight();
				float rotationAngle = trans->getRotationAngle();
				ImGui::PushItemWidth(_WIDTH / 3); {
					{
						if (CInputInt("Width", "Width", &width, 1, 10)) {
							trans->setWidth(width);
						}
						ImGui::SameLine();
						if (CInputInt("Height", "Height", &height, 1, 10)) {
							trans->setHeight(height);
						}
					}
					ImGui::Text("Position"); ImGui::SameLine(); {
						if (CInputInt("X", "Position X", &x, 1, 10)) {
							trans->setPos(vec2d(x, y));
						}
						ImGui::SameLine();
						if (CInputInt("Y", "Position Y", &y, 1, 10)) {
							trans->setPos(vec2d(x, y));
						}
					}
					ImGui::Text("Scale"); ImGui::SameLine(); {
						if (CInputFloat("X", "Scale X", &scalex, 0.5, 2)) {
							trans->setScale(vec2d(scalex, scaley));
						}
						ImGui::SameLine();
						if (CInputFloat("Y", "Scale Y", &scaley, 0.5, 2)) {
							trans->setScale(vec2d(scalex, scaley));
						}
					}
				}

				ImGui::PopItemWidth();

				if (CInputFloat("Rotation Z", "Rotation Z", &rotationAngle, 1, 5)) {
					if (rotationAngle >= 360 || rotationAngle <= -360)
						rotationAngle = 0;
					trans->setRotationAngle(rotationAngle);
				}
			}
			else if (sprite* spr = dynamic_cast<sprite*>(component)) {
				vec2d size = spr->getScale();
				int width = size.x;
				int height = size.y;
				const char* text_buff = spr->filename.c_str();
				CInputText("File name", "Sprite file name", text_buff, 128);

				ImGui::PushItemWidth(_WIDTH / 3); {
					{
						ImGui::Text("Scale"); ImGui::SameLine(); {
							if (CInputInt("Width", "Width", &width, 0.5, 2)) {
								spr->setScale(vec2d(width, height));
							}
							ImGui::SameLine();
							if (CInputInt("Height", "Height", &height, 0.5, 2)) {
								spr->setScale(vec2d(width, height));
							}
						}
					}
				}

				if (ImGui::Button("Update")) {
					spr->fromFile(text_buff);
				}
			}
		}
	}

	ImGui::End();
}