#pragma once
#include "StageToolGui.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx9.h"

#include "Export_System.h"

#include "StageTool.h"
#include "ObjectLoad.h"


CStageToolGui::CStageToolGui()
{
}

CStageToolGui::CStageToolGui(HWND hWnd, LPDIRECT3DDEVICE9 pGraphicDev)
{
    Ready_ImGuiTools(hWnd, pGraphicDev);
}

CStageToolGui::~CStageToolGui()
{
    Free();
}

HRESULT CStageToolGui::Ready_ImGuiTools(HWND hWnd, LPDIRECT3DDEVICE9 pGraphicDev)
{
    m_bIsOpend = false;
    m_iSelectedStageIndex = 0;
    m_vPickingPos = { 0, 0, 0 };

    m_pGraphicDev = pGraphicDev;

    vecObjCounts = CObjectLoad::GetInstance()->Get_VecObjCounts();
    m_mapStage = CObjectLoad::GetInstance()->Get_MapStage();
    m_umapObjectType = CObjectLoad::GetInstance()->Get_UmapObjectType();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX9_Init(m_pGraphicDev);

    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    return S_OK;
}

void CStageToolGui::Update_ImGuiTools()
{
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    int iListSize = m_mapStage.size();
    vector<string> vecString(iListSize);

    int i = 0;

    for_each(m_mapStage.begin(), m_mapStage.end(),
        [&i, &vecString](auto& iter) {
            vecString[i] = iter.second;
            ++i;
        });

    vector<const char*> items;

    if (0 < vecString.size()) {
        for (const string& str : vecString) {
            items.push_back(str.c_str());
        }
    }

    //매 업데이트마다 돌아감
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("StageTool");                          // Create a window called "Hello, world!" and append into it.

    static CHAR szSelectedStage[MAX_PATH] = "";

    ImGui::Text("Stage List");

    //ImGui::SetNextItemWidth(100.f);
    ImGui::BeginListBox("##");
    if (0 < vecString.size())
        ImGui::ListBox("##", &m_iSelectedStageIndex, items.data(), vecString.size());
    ImGui::EndListBox();

    ImGui::NewLine();
    ImGui::Text("Position: ");
    ImGui::Text("x: %f", m_vPickingPos.x);
    ImGui::Text("y: %f", m_vPickingPos.y);
    ImGui::Text("z: %f", m_vPickingPos.z);

    // 현재까지 지정한 맵 정보를 저장한다.
    if (ImGui::Button("Save"))
    {
        string strDirPath = "../../Dat/";
        string strFileName = vecString[m_iSelectedStageIndex] + "_Design";
        string strFileExtension = ".dat";

        strFileName = strDirPath + strFileName + strFileExtension;

        // 파일 스트림을 엽니다.
        ofstream fout(strFileName, ios::binary);

        for_each(m_PlacementObjList.begin(), m_PlacementObjList.end(),
            [strFileName, &fout](auto& iter) {
                fout << iter.iType << ",";
                fout << iter.iIndex << ",";
                fout << iter.x << ",";
                fout << iter.y << ",";
                fout << iter.z << endl;
            });

        // 파일 스트림을 닫습니다.
        fout.close();
    }

    ImGui::SameLine();

    if (ImGui::Button("Load"))
    {
        m_pTargetScene->Clear_Placement_Object();
        m_PlacementObjList.clear();
        Load_Stage_Design();
    }

    ImGui::SameLine();

    if (ImGui::Button("Clear"))
    {
        m_pTargetScene->Clear_Placement_Object();
        m_PlacementObjList.clear();
    }

    ImGui::End();

    Popup_Object_Gui();

}

void CStageToolGui::Render_ImGuiTools()
{
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void CStageToolGui::Popup_Object_Gui()
{
    ImGui::Begin("Object Placement");

    ImGui::Text("Please select an object to place.");

    Create_Image_Buttons();

    ImGui::End();
}

void CStageToolGui::Create_Image_Buttons()
{
    static int iTest = 0;

    for (auto& item : m_umapObjectType)
    {
        ImGui::NewLine();

        string str = CObjectLoad::GetInstance()->Trans_Object_Type_To_String(item.first);
        ImGui::Text("%s", str.c_str());

        int i = 0;

        for (auto& image : item.second)
        {
            string strButtonName = str + to_string(i);

            D3DSURFACE_DESC my_image_desc;
            image->GetLevelDesc(0, &my_image_desc);
            int width = my_image_desc.Width * TEXTURE_SIZE;
            int height = my_image_desc.Height * TEXTURE_SIZE;

            if (ImGui::ImageButton(strButtonName.c_str(), (void*)image, ImVec2(width, height)))
            {
                iTest = i;
                m_pTargetScene->Set_Cursor_Image(item.first, i);
            }
            ImGui::SameLine();

            i++;
        }
    }

    ImGui::Text("%i", iTest);
}

void CStageToolGui::Push_Placement_Obj_List(int iType, int iIndex, float x, float y, float z)
{
    PlacementObjInfo tTemp{ iType , iIndex, x, y, z };

    m_PlacementObjList.push_front(tTemp);
}

void CStageToolGui::Load_Stage_Design()
{
    // 0번은 사용하지 않는 방을 의미하며, 인덱스로 처리하기 때문에 1번부터 시작하니 +1 해줬음
    string strFilePath = "../../Dat/" + 
        m_mapStage[m_iSelectedStageIndex + 1] + "_Design.dat";

    ifstream fin(strFilePath);

    string strGetLine = "";

    while (getline(fin, strGetLine))
    {
        vector<string> vecStr;
        int iIndex = 0;

        while (true) {
            // , 위치 찾기
            int pos = strGetLine.find_first_of(',', iIndex);

            // ,를 찾지 못하면 종료
            if (pos == string::npos) {
                vecStr.push_back(strGetLine.substr(iIndex));
                break;
            }

            // 분리된 문자열 출력
            vecStr.push_back(strGetLine.substr(iIndex, pos - iIndex));
            iIndex = pos + 1;
        }

        Push_Placement_Obj_List(stoi(vecStr[0]), stoi(vecStr[1]), stof(vecStr[2]), stof(vecStr[3]), stof(vecStr[4]));
        m_pTargetScene->Create_Placement_Object(stoi(vecStr[0]), stoi(vecStr[1]), stof(vecStr[2]), stof(vecStr[3]), stof(vecStr[4]));

        vecStr.clear();
    }

    fin.close();
}

void CStageToolGui::Free()
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}
