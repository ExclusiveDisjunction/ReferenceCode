#include "EditorRegistry.h"

#include "Editor.h"
#include "EditorButtonHost.h"
#include "EditorPopout.h"
#include "EditorButton.h"
#include "..\IO\IOCommon.h"

namespace Core::Editors
{    
    using namespace IO;
    using namespace UI;
    using namespace UI::Windows;

    Vector<Editor*> EditorRegistry::Info = Vector<Editor*>();
    Vector<EditorButtonHost*> EditorRegistry::ButtonHosts;
    Vector<FooterHost*> EditorRegistry::FooterHosts;
    EditorHost* EditorRegistry::BaseHost = nullptr;

    void EditorRegistry::SetHost(EditorHost* BaseHost)
    {
        EditorRegistry::BaseHost = BaseHost;
        if (dynamic_cast<EditorButtonHost*>(BaseHost) != nullptr)
            Track(dynamic_cast<EditorButtonHost*>(BaseHost));
    }
    void EditorRegistry::Track(const Vector<EditorButtonHost*>& ButtonHosts, const Vector<FooterHost*>& RibbonHosts)
    {
        EditorRegistry::ButtonHosts = ButtonHosts;
        EditorRegistry::FooterHosts = RibbonHosts;
    }
    void EditorRegistry::Track(EditorButtonHost* ButtonHost)
    {
        ButtonHosts.Add(ButtonHost);
        for (EditorButton* i : ButtonHost->EditorButtons)
            delete i;

        EditorButtonHost* ToRef = ButtonHosts.Size == 0 ? nullptr : ButtonHosts[0];
        if (!ToRef)
            return;

        for (uint i = 0; i < ToRef->EditorButtons.Size; i++)
            ButtonHost->AddEditorButton(ToRef->EditorButtons[i]->Current());
    }
    void EditorRegistry::Track(FooterHost* FooterHost)
    {
        FooterHosts.Add(FooterHost);
    }
    void EditorRegistry::RemoveTracking(EditorButtonHost* Host)
    {
        ButtonHosts.Remove(Host);
    }
    void EditorRegistry::RemoveTracking(FooterHost* Ribbon)
    {
        FooterHosts.Remove(Ribbon);
    }
    void EditorRegistry::ExitRegistry()
    {
        for (Editor* Current : Info)
            delete Current;
        Info.Clear();

        for (uint i = 0; i < ButtonHosts.Size; i++)
        {
            EditorButtonHost* Host = ButtonHosts[i];
            if (Host != BaseHost)
                delete Host;
        }

        ButtonHosts.Clear();
        FooterHosts.Clear();
        BaseHost = nullptr;
    }

    Vector<Editor*> EditorRegistry::CurrentApplyableEditors()
    {
        Vector<Editor*> Return;
        for (uint i = 0; i < Info.Size; i++)
        {
            Editor* This = Info[i];
            if (This->IsApplyable())
                Return.Add(This);
        }

        return Return;
    }
    Vector<Editor*> EditorRegistry::CurrentOpenEditors()
    {
        return Vector<Editor*>(Info);
    }

    Editor* EditorRegistry::GetEditor(unsigned long long EditorType, EditorHost* Parent)
    {
        for (uint i = 0; i < Info.Size; i++)
        {
            Editor* Current = Info[i];
            if ((EditorType & Current->EditorType()) && (!Parent ? true : Current->Host == Parent))
                return Current;
        }

        return nullptr;
    }
    Vector<Editor*> EditorRegistry::GetEditors(unsigned long long EditorType, EditorHost* Parent)
    {
        Vector<Editor*> Return;
        for (uint i = 0; i < Info.Size; i++)
        {
            Editor* Current = Info[i];
            if ((EditorType & Current->EditorType()) && (!Parent ? true : Current->Host == Parent))
                Return.Add(Current);
        }

        return Return;
    }

    void EditorRegistry::MoveEditor(Editor* editor, EditorHost* toHost, bool ToPopout)
    {
        HINSTANCE ins = reinterpret_cast<HINSTANCE>(GetWindowLongPtr(*editor, GWLP_HINSTANCE));

        if (ToPopout)
            toHost = new EditorPopout(ins);
        if (!toHost)
            toHost = EditorRegistry::BaseHost;

        EditorHost* fromHost = editor->Host;
        if (fromHost == toHost)
            return;
        if (!fromHost || !toHost)
            return;

        EditorButtonHost* ConvHost = dynamic_cast<EditorButtonHost*>(toHost);
        if (ConvHost)
        {
            if (ConvHost->Current)
                ShowWindow(*ConvHost->Current, SW_HIDE);
            ConvHost->LastEditor = ConvHost->Current;
            ConvHost->Current = editor;
            editor->_Host = ConvHost;
        }
        else
        {
            if (toHost->Current != nullptr)
                return;
            
            toHost->Current = editor;
            editor->_Host = toHost;
        }

        HWND ToParent = toHost->EditorParent();
        RECT ToPlacement = toHost->EditorPlacement;

        HWND FromParent = fromHost->EditorParent();
        HWND ThisHWnd = *editor;

        editor->_Placement = ToPlacement;
        SetParent(ThisHWnd, ToParent);
        editor->ReSize();

        EditorButtonHost* ConvFrom = dynamic_cast<EditorButtonHost*>(fromHost);
        if (ConvFrom)
        {
            ConvFrom->Current = nullptr;
            ConvFrom->OpenLastEditor();
        }
        else
        {
            delete fromHost;
            fromHost = nullptr;
        }

        HWND Parent = *toHost;

        SetWindowPos(Parent, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
        ShowWindow(ThisHWnd, SW_SHOW);

        if (GetWindowLongPtrW(Parent, GWL_STYLE) & WS_MINIMIZE)
            ShowWindow(Parent, SW_RESTORE);

        for (EditorButtonHost* Hosts : ButtonHosts)
            RedrawWindow(*Hosts, NULL, NULL, RDW_INVALIDATE | RDW_ERASENOW);
    }

    void EditorRegistry::ResetEditorOfType(unsigned long long Types)
    {
        for (uint i = 0; i < Info.Size; i++)
        {
            Editor* Current = Info[i];
            if (Types & Current->EditorType())
                Current->Reset();
        }
    }
    void EditorRegistry::ResetEditorOfType(unsigned long long Types, Vector<void*> Condition)
    {
        for (uint i = 0; i < Info.Size; i++)
        {
            Editor* Current = Info[i];
            if ((Types & Current->EditorType()) && Current->TestOnCondition(Condition))
                Current->Reset();
        }
    }

    bool EditorRegistry::IsOpen(unsigned long long Types, EditorHost* Host)
    {
        for (Editor* Frame : Info)
        {
            if ((Types & Frame->EditorType()) && Frame->Host == Host)
                return true;
        }

        return false;
    }
    bool EditorRegistry::IsOpen(unsigned long long Types, Vector<void*> Args, EditorHost* Host)
    {
        for (Editor* Frame : Info)
        {
            if ((Types & Frame->EditorType()) && Frame->TestOnCondition(Args) && (!Host ? true: Frame->Host == Host))
                return true;
        }

        return false;
    }

    Editor* EditorRegistry::CanMesh(int State, Editor*& Source)
    {
        if (!Source)
            return nullptr;

        unsigned long long editor = Source->EditorType();
        Vector<void*> Args = Source->CondenseArgs();

        Vector<class Editor*> Others = EditorRegistry::GetEditors(editor);
        Editor* Current = nullptr;
        if (Others.Size == 0)
            return nullptr;

        bool Return = false;

        switch (State)
        {
        case 0: //Applyable & Object Specific
        case 1: //Object Specific
        {
            for (uint i = 0; i < Others.Size; i++)
            {
                Editor* Temp = Others[i];
                if (Temp->TestOnCondition(Args))
                {
                    Current = Temp;
                    Return = true;
                    break;
                }
            }
            break;
        }
        case 2: //Nonapplyble
            Return = true;
            Current = Others[0];
            break;
        }

        if (Return)
        {
            EditorHost* Host = Current->Host;
            EditorButtonHost* BttnHost = dynamic_cast<EditorButtonHost*>(Host);

            if (BttnHost)
            {
                if (BttnHost->Current)
                    ShowWindow(*BttnHost->Current, SW_HIDE);
                ShowWindow(*Current, SW_SHOW);
                BttnHost->LastEditor = BttnHost->Current;
                BttnHost->Current = Current;
            }

            HWND Parent = *Host;

            SetWindowPos(Parent, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

            if (GetWindowLongPtrW(Parent, GWL_STYLE) & WS_MINIMIZE)
                ShowWindow(Parent, SW_RESTORE);
        }

        if (!Return)
            Current = nullptr;
        else
        {
            if (Source->Host->SingleHostEditor())
                delete Source->Host;
            delete Source;
            Source = nullptr;
        }
        return Current;
    }
    Editor* EditorRegistry::OpenEditor(Editor* Frame, EditorHost* Host)
    {
        if (!Frame)
            return nullptr;

        if (!Host)
        {
            if (!BaseHost)
                return nullptr;

            Host = BaseHost;
        }

        HWND Parent = Host->EditorParent();
        RECT Place = Host->EditorPlacement;
        Frame->_Host = Host;
        Frame->GenerateBase(Parent, Place);

        int Type = 0;
        if (Frame->IsApplyable())
            Type = 0;
        else if (Frame->ConditionSpecific())
            Type = 1;
        else
            Type = 2;
        
        Editor* Mesh = CanMesh(Type, Frame);
        if (Mesh)
            return Mesh; //CanMesh handles deleting of variables

        for (EditorButtonHost* BttnHost : ButtonHosts)
            BttnHost->AddEditorButton(Frame);
        Host->ProcessOpenEditor(Frame);

        Info.Add(Frame);
        /*
        for (FooterHost* Footer : FooterHosts)
            Footer->SetFooterText(L"Opened Editor \"" + Frame->GetName() + L"\".");
        */
        return Frame;
    }

    bool EditorRegistry::CloseEditor(Editor* Editor, bool AskForAppend)
    {
        if (Editor == nullptr)
            return true;

        if (Editor->IsApplyable() && AskForAppend)
        {
            String Title = L"Close \"" + Editor->GetName() + L"\":";
            auto Result = MessageBoxW(GetAncestor(*Editor, GA_ROOT), L"Do you want to apply before closing?", static_cast<LPCWSTR>(Title), MB_YESNOCANCEL | MB_ICONWARNING);
            if (Result == IDYES)
            {
                if (!Editor->Apply(IO::LoadedProject, true))
                {
                    MessageBoxW(GetAncestor(*Editor, GA_ROOT), L"The editor could not be applied, and it will remain open.", static_cast<LPCWSTR>(Title), MB_OK | MB_ICONERROR);
                    return false;
                }
            }
            if (Result == IDCANCEL)
                return false;
        }

        String EditorName = Editor->GetName();
        Info.Remove(Editor);

        EditorHost* Host = Editor->Host;

        for (EditorButtonHost* EditorButtonH : ButtonHosts)
            EditorButtonH->RemoveEditorButton(Editor);

        for (EditorButtonHost* EditorButtonH : ButtonHosts)
            EditorButtonH->OpenLastEditor();
         
        delete Editor;
        Host->ProcessCloseCurrent();

        /*
        for (FooterHost* Footer : FooterHosts)
            Footer->SetFooterText(L"Closed Editor \"" + EditorName + L"\".");
        */
        return true;
    }
    bool EditorRegistry::CloseAllEditors() 
    {
        return CloseEditors(Info);
    }
    bool EditorRegistry::CloseEditors(Vector<Editor*>& Info)
    {        
        Vector<Editor*> Applyable;
        for (uint i = 0; i < Info.Size; i++)
        {
            Editor* Current = Info[i];
            if (Current->IsApplyable())
                Applyable.Add(Current);
        }

        bool DoAppylyable = false;
        if (Applyable.Size != 0)
        {
            String Prompt = L"Do you want to apply the following editors?\n\n";
            for (Editor* Current : Applyable)
                Prompt += Current->GetName() + L"\n";

            int Result = MessageBoxW(*BaseHost, static_cast<LPCWSTR>(Prompt), L"Close Editors:", MB_YESNOCANCEL | MB_ICONWARNING);
            DoAppylyable = Result == IDYES;
        }

        Vector<Editor*> ToRemove;
        bool Return = true;
        for (Editor* Current : Info)
        {
            if (Current->IsApplyable())
            {
                if (!DoAppylyable)
                {
                    Return &= false;
                    continue;
                }

                bool Result = Current->Apply(LoadedProject);
                if (Result)
                {
                    ToRemove.Add(Current);
                    Current->ClearState();
                }
                else
                    Current->CurrentState(Current->CurrentState() | EDS_AppendError);

                Return &= Result;
            }
            else
            {
                Return &= true;
                ToRemove.Add(Current);
            }
        }

        for (Editor* This : ToRemove)
        {
            Info.Remove(This);
            EditorRegistry::Info.Remove(This);
            for (EditorButtonHost* EditorButtonH : ButtonHosts)
            {
                if (EditorButtonH->Current == This)
                    EditorButtonH->Current = nullptr;
                if (EditorButtonH->LastEditor == This)
                    EditorButtonH->LastEditor = nullptr;

                EditorButtonH->RemoveEditorButton(This);
            }

            delete This;
        }

        for (EditorButtonHost* EditorButtonH : ButtonHosts)
            EditorButtonH->OpenLastEditor();

        /*
        for (FooterHost* Footer : FooterHosts)
            Footer->SetFooterText(L"Closed " + String(ToRemove.Size) + L" editors.");
        */
        return Return;
    }
    bool EditorRegistry::ForceCloseAllEditors()
    {
        for (uint i = 0; i < Info.Size; i++)
        {
            Editor* Editor = Info[i];
            EditorHost* Host = Editor->Host;

            for (EditorButtonHost* BttnHost : ButtonHosts)
                BttnHost->RemoveEditorButton(Editor);

            delete Editor;
            Host->Current = nullptr;

            if (Host->SingleHostEditor())
                delete Host;
        }

        Info.Clear();
        return true;
    }
    bool EditorRegistry::PopEditor(Editor* Obj)
    {
        return Info.Remove(Obj);
    }
}