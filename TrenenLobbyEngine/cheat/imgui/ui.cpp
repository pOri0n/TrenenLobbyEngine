#include <pch.h>

Singleton_CPP(UI);


void UI::Setup(IDirect3DDevice9* pDevice)
{
	IsSetup = true;

	while (!(hWindow = FindWindowA("Valve001", NULL)))
		std::this_thread::sleep_for(0.2s);

	ImGui_ImplDX9_Init(hWindow, pDevice);

	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Text] = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.09f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.04f, 0.04f, 0.04f, 0.88f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.24f, 0.40f, 0.95f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.24f, 0.40f, 0.95f, 0.59f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.59f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.24f, 0.40f, 0.95f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.24f, 0.40f, 0.95f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.40f, 0.95f, 0.59f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.24f, 0.40f, 0.95f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.24f, 0.40f, 0.95f, 0.59f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.70f, 0.02f, 0.60f, 0.22f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.24f, 0.40f, 0.95f, 1.00f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.24f, 0.40f, 0.95f, 0.59f);

	style.WindowRounding = 0.f;
	style.FramePadding = ImVec2(4, 1);
	style.ScrollbarSize = 10.f;
	style.ScrollbarRounding = 0.f;
	style.GrabMinSize = 5.f;
}

void UI::Render(IDirect3DDevice9* pDevice)
{
	if (!IsSetup)
		Setup(pDevice);

	ImGui_ImplDX9_NewFrame();

	ImGui::GetIO().MouseDrawCursor = false;
	
	//ImGui::GetIO().MouseDrawCursor = true;
	//int pX, pY;
	//I.InputSystem->GetCursorPosition(&pX, &pY);
	//ImGuiIO& io = ImGui::GetIO();
	//io.MousePos.x = (float)(pX);
	//io.MousePos.y = (float)(pY);

	if (IsOpen)
	{
		ImGui::Begin("TrenenLobbyEngine", &IsOpen, ImVec2(300, 200), 0.9f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
		{
			ImGui::Checkbox("Modify Chat Messages", &CFG->LobbyChat_Enable);

			const char* ChatColours[] = { "Standard", "Red", "Green", "Yellow" };

			ImGui::Combo("Chat Colour", &CFG->LobbyChat_ColourIndex, ChatColours, ARRAYSIZE(ChatColours));

			ImGui::Checkbox("Prepend Name", &CFG->LobbyChat_PrependName);
			ImGui::Checkbox("Endline Spam", &CFG->LobbyChat_EndlineSpam);

			ImGui::Separator();

			ImGui::Checkbox("Hide Invite Names", &CFG->LobbyInvite_HideInviteNames);

			ImGui::Separator();

			ImGui::Checkbox("Queue Bomb", &CFG->LobbyNuke_Enable);
			ImGui::SliderFloat("Exponent", &CFG->LobbyNuke_Exponent, 0.f, 5.f, "%.2f");

		}
		ImGui::End();
	}

	ImGui::Render();
}

extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool UI::OnWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_SYSKEYUP || uMsg == WM_KEYUP)
	{
		if (wParam == VK_INSERT)
		{
			IsOpen = !IsOpen;
		}
	}

	if (IsOpen && ImGui_ImplDX9_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return false;
}