#pragma once
#include "ImGuiTools.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx9.h"

#include "Export_System.h"


CImGuiTools::CImGuiTools()
{
}

CImGuiTools::CImGuiTools(HWND hWnd, LPDIRECT3DDEVICE9 pGraphicDev)
{
    Ready_ImGuiTools(hWnd, pGraphicDev);
}

CImGuiTools::~CImGuiTools()
{
    Free();
}

HRESULT CImGuiTools::Ready_ImGuiTools(HWND hWnd, LPDIRECT3DDEVICE9 pGraphicDev)
{
    m_bIsOpend = false;
    m_iSelectedStageIndex = 0;

    m_pGraphicDev = pGraphicDev;

    string strFilePath = "../Dat/MapLevel.dat";

    ifstream fin(strFilePath);

    string strGetLine = "";
    //코드를 한 줄 읽어온다. (스테이지에 대한 정보는 한줄로 저장하게 만들어뒀기 때문에 가능)

    int iIndex = 0;
    while (getline(fin, strGetLine))
    {
        vector<string> vecStr;

        while (true) {
            // , 위치 찾기
            int pos = strGetLine.find_first_of(',', iIndex);

            // ,를 찾지 못하면 종료
            if (pos == string::npos) {
                break;
            }

            // 분리된 문자열 출력
            
            vecStr.push_back(strGetLine.substr(iIndex, pos - iIndex));

            // 다음 구분자 위치를 시작 위치로 설정
            iIndex = pos + 1;
        }

        m_mapStage.emplace(pair<int, string>(stoi(vecStr[1]),  vecStr[0]));

        vecStr.clear();
    }

    fin.close();


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX9_Init(m_pGraphicDev);

    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

	return S_OK;
}

void CImGuiTools::Update_ImGuiTools()
{
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    //매 업데이트마다 돌아감
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("MapTool");                          // Create a window called "Hello, world!" and append into it.

    static CHAR	szStageName[MAX_PATH] = "";
    static CHAR	szStageKey[MAX_PATH] = "";

    ImGui::Text("Stage Name: ");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(90.f);
    ImGui::InputText("##Stage Name", szStageName, MAX_PATH, ImGuiInputTextFlags_None);

    ImGui::Text("Key: ");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(100.f);
    ImGui::InputText("##Key", szStageKey, MAX_PATH);
    ImGui::SameLine();

    //스테이지 리스트를 보여주는 map에 방금 작성한 스테이지를 Add한다.
    // Key값이 중복되지 않을 경우에만 emplace가 실행되게 예외처리 적용
    if (ImGui::Button("Add"))
    {
        if (m_mapStage.find(stoi(szStageKey)) == m_mapStage.end())
        {
            string strStageFullName = szStageName;
            strStageFullName = strStageFullName + "_" + szStageKey;

            m_mapStage.emplace(pair<int, string>(stoi(szStageKey), strStageFullName));
        }
    }   

    ImGui::NewLine();

    static CHAR szSelectedStage[MAX_PATH] = "";

    int iListSize = m_mapStage.size();
    vector<string> vecString(iListSize);

    int i = 0;

    for_each(m_mapStage.begin(), m_mapStage.end(),
        [&i, &vecString](auto& iter) {
            vecString[i] = iter.second;
            ++i;
        });

    std::vector<const char*> items;

    if (0 < vecString.size()) {
        // items 벡터의 요소를 vecString의 요소로 채웁니다.
        for (const string& str : vecString) {
            items.push_back(str.c_str());
        }
    }

    ImGui::Text("Stage List");
    ImGui::SameLine();

    if (ImGui::Button("Save"))
    {

    }
    ImGui::SameLine();

    if (ImGui::Button("Load"))
    {
        //TODO: 이 곳에 ADD 클릭 시 스테이지 목록을 추가하는 함수를 작성한다.
        // 파일 스트림을 엽니다.

        //ifstream fin(strFilePath);

        //string strKeys = "";
        ////코드를 한 줄 읽어온다. (스테이지에 대한 정보는 한줄로 저장하게 만들어뒀기 때문에 가능)
        //while (getline(fin, strKeys))
        //{
        //    //내용
        //}

        //fin.close();
    }

    ImGui::SetNextItemWidth(100.f);
    //TODO: Need ListBox Load
    ImGui::BeginListBox("##");
    if (0 < vecString.size())
        ImGui::ListBox("##", &m_iSelectedStageIndex, items.data(), vecString.size());
    ImGui::EndListBox();

    //리스트 박스 클릭 시 실행되는 이벤트(0 = left, 1 = right, 2 = middle)
    if (ImGui::IsItemClicked())
    {
        m_bIsOpend = true;
    }

    //if (ImGui::Button("Open"))
    //{
    //    m_bIsOpend = true;
    //}
     
    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
    ImGui::Checkbox("Another Window", &show_another_window);

    //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

    if (m_bIsOpend)
    {
        Popup_Stage_Connection(vecString[m_iSelectedStageIndex].c_str());
    }

    //ImGui::SameLine();
    //ImGui::Text("counter = %d", counter);

   // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();

}

void CImGuiTools::Render_ImGuiTools()
{
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
    //m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
    //m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    //m_pGraphicDev->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
    //D3DCOLOR clear_col_dx = D3DCOLOR_RGBA(255, 255, 255, 255);
    //m_pGraphicDev->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
    //if (m_pGraphicDev->BeginScene() >= 0)
    //{
    //    ImGui::Render();
    //    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
    //    m_pGraphicDev->EndScene();
    //}

    //HRESULT result = m_pGraphicDev->Present(nullptr, nullptr, nullptr, nullptr);

}

void CImGuiTools::Popup_Stage_Connection(const char* items)
{
    ImGui::Begin("Stage Connection Relationship");

    //ImGui의 Input박스에 출력할 데이터는 static이 아니면 업데이트 때 마다 초기화되기 때문에 static으로 선언했음
    static CHAR	szSKeyLeftRoom[MAX_PATH] = "";
    static CHAR	szSKeyRightRoom[MAX_PATH] = "";
    static CHAR	szSKeyTopRoom[MAX_PATH] = "";
    static CHAR	szSKeyBottomRoom[MAX_PATH] = "";

    //.dat 파일을 불려올 경로를 설정해준다.
    string strFilePath = "../Dat/";
    string strExtension = ".dat";
    strFilePath = strFilePath + items + strExtension;

    ifstream fin(strFilePath);

    string strKeys = "";
    //코드를 한 줄 읽어온다. (스테이지에 대한 정보는 한줄로 저장하게 만들어뒀기 때문에 가능)
    getline(fin, strKeys);

    fin.close();

    ImGui::Text("Selected Stage Name: %s", items);

    ImGui::NewLine();

    ImGui::Text("Please enter a key value.");

    ImGui::Text("Left: ");
    ImGui::SameLine();
    if (strKeys != "")
    {
        ImGui::Text("%c", strKeys[0]);
        ImGui::SameLine();
    }

    ImGui::SetNextItemWidth(30.f);
    ImGui::InputText("##Left", szSKeyLeftRoom, MAX_PATH);

    ImGui::Text("Right: ");
    ImGui::SameLine();
    if (strKeys != "")
    {
        ImGui::Text("%c", strKeys[1]);
        ImGui::SameLine();
    }
    ImGui::SetNextItemWidth(30.f);
    ImGui::InputText("##Right", szSKeyRightRoom, MAX_PATH);

    ImGui::Text("Top: "); 
    ImGui::SameLine();
    if (strKeys != "")
    {
        ImGui::Text("%c", strKeys[2]);
        ImGui::SameLine();
    }
    ImGui::SetNextItemWidth(30.f);
    ImGui::InputText("##Top", szSKeyTopRoom, MAX_PATH);
    
    ImGui::Text("Bottom: "); 
    ImGui::SameLine();
    if (strKeys != "")
    {
        ImGui::Text("%c", strKeys[3]);
        ImGui::SameLine();
    }
    ImGui::SetNextItemWidth(30.f);
    ImGui::InputText("##Bottom", szSKeyBottomRoom, MAX_PATH);

    if (ImGui::Button("SAVE"))
    {
        // 파일 스트림을 엽니다.
        ofstream fout(strFilePath.c_str(), ios::binary);

        // 파일에 데이터를 씁니다.
        fout << szSKeyLeftRoom;
        fout << szSKeyRightRoom;
        fout << szSKeyTopRoom;
        fout << szSKeyBottomRoom;

        // 파일 스트림을 닫습니다.
        fout.close();
    }

    if (ImGui::Button("Close"))
        m_bIsOpend = false;

    ImGui::End();
}

void CImGuiTools::Free()
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}
