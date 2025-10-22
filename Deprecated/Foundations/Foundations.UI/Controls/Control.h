#pragma once

#include <Windows.h>
#include <string>
#include <vector>

#include "..\UICommon.h"
#include "..\Style\Style.h"

//Dll Entry Point
bool __stdcall DllMain(HINSTANCE, DWORD, LPVOID);

namespace Core::UI::Controls
{
    String CORE_API GetText(HWND Window);
    void CORE_API SetText(HWND Window, String New);

    enum TextAlignment
    {
        TA_CenterAlignment,
        TA_LeftAlignment,
        TA_RightAlignment
    };

    class CORE_API Control
    {
    private:
        static LRESULT __stdcall CommonWndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);
        static void RegisterAtom(HINSTANCE ins);
        static ATOM _ThisAtom;

    protected:
        HWND _Base = nullptr;
        bool IsEnabled = true;

        Style ThisStyle;
        StyleSchematic StyleSchema;

        ATOM GetAtom() const;

        virtual bool FocusOnClick() const { return true; }
        virtual void ProcessExtraMessage(UINT Message, WPARAM wp, LPARAM lp);
    public:
        Control(HINSTANCE ins);
        virtual ~Control();

        // !! IMPORTANT !! The DllMain function is responsible for cleaning starting and closing out the ATOM.
        friend bool __stdcall ::DllMain(HINSTANCE, DWORD, LPVOID);

        //Basic Management
        virtual void Create() { }
        virtual void Paint() { }
        virtual void OnDestroy() { }

        //User Input
        virtual void KeyDown(WPARAM key);
        virtual void Char(wchar_t Character, int RepeatCount, int ScanCode, bool ExtendedKey, bool PrevStatePressed, bool IsBeingReleased) { }
        virtual void Click() {  }
        virtual void DoubleClick() { SendMessage(GetParent(_Base), WM_LBUTTONDBLCLK, 0, 0); }
        virtual void RDoubleClick() { SendMessage(GetParent(_Base), WM_RBUTTONDBLCLK, 0, 0); }
        virtual void MouseUp() { }
        virtual void RightClick() { SendMessage(GetParent(_Base), WM_RBUTTONDOWN, 0, 0); }

        //Windows Events
        virtual void TimerTick(long TimerID) { }
        virtual void OnSize();
        virtual void OnMove();
        virtual void GotFocus() { }
        virtual void LostFocus() { }
        virtual void Command(WPARAM ID, LPARAM lp) { SendMessage(GetParent(_Base), WM_COMMAND, ID, lp); }

        bool Enabled();
        void Enabled(bool New);

        virtual void Redraw();

        Style& GetStyle() const;
        void SetStyle(const StyleSheet& New);

        explicit operator HWND() const
        {
            return _Base;
        }
    };
}