#include "Control.h"

namespace Core::UI::Controls
{
    String GetText(HWND Window)
    {
        int TextLen = static_cast<int>(GetWindowTextLengthW(Window)) + 1;
        wchar_t* Chars = new wchar_t[TextLen];

        GetWindowTextW(Window, Chars, TextLen);
        String Return(Chars);

        delete[] Chars;
        return Return;
    }
    void SetText(HWND Window, String New)
    {
        wchar_t* Chars = const_cast<wchar_t*>(static_cast<const wchar_t*>(New));
        SendMessageW(Window, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(Chars));
    }

    Control::Control(HINSTANCE ins)
    {
        _Base = nullptr;
        if (!Control::_ThisAtom)
            RegisterAtom(ins);

        if (!Control::_ThisAtom)
            throw std::logic_error("The atom for control could not be loaded!");
    }
    Control::~Control()
    {
        SetWindowLongPtrW(_Base, GWLP_USERDATA, 0);
        DestroyWindow(_Base);
    }

    LRESULT __stdcall Control::CommonWndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp)
    {
        LONG_PTR Value = GetWindowLongPtrW(Window, GWLP_USERDATA);
        if (!Value)
            return DefWindowProcW(Window, Message, wp, lp);

        Control* Data = reinterpret_cast<Control*>(Value);

        switch (Message)
        {
        case WM_PAINT:
            return Data->Paint();
        case WM_CREATE:
            return Data->Create();
        case WM_DESTROY:
        {
            LRESULT Return = Data->OnDestroy();
            delete Data;
            return Return;
        }
        case WM_KEYDOWN:
            return Data->KeyDown(wp);
        case WM_LBUTTONDOWN:
        {
            bool Focus = Data->FocusOnClick();
            LRESULT Return = Data->Click();
            if (Focus)
                SetFocus(Window);
            //SendMessageW(GetParent(Window), Message, wp, lp);
            return Return;
        }
        case WM_RBUTTONDOWN:
        {
            LRESULT Return = Data->RightClick();
            return Return;
        }
        case WM_LBUTTONUP:
            return Data->MouseUp();
        case WM_LBUTTONDBLCLK:
            return Data->DoubleClick();
        case WM_RBUTTONDBLCLK:
            return Data->RDoubleClick();
        case WM_TIMER:
            return Data->TimerTick(static_cast<LONG>(wp));
        case WM_CHAR:
        {
            WORD VKCode = LOWORD(wp);

            WORD KeyFlags = HIWORD(lp);
            int ScanCode = LOBYTE(KeyFlags);
            bool IsExtendedKey = (KeyFlags & KF_EXTENDED) == KF_EXTENDED;

            if (IsExtendedKey)
                ScanCode = MAKEWORD(ScanCode, 0xE0);

            bool WasKeyDown = (KeyFlags & KF_REPEAT) == KF_REPEAT;
            WORD RepeatCount = LOWORD(lp);
            bool IsKeyReleased = (KeyFlags & KF_UP) == KF_UP;

            switch (VKCode)
            {
            case VK_SHIFT:
            case VK_CONTROL:
            case VK_MENU:
                VKCode = LOWORD(MapVirtualKey(ScanCode, MAPVK_VSC_TO_VK_EX));
            }

            Data->Char(static_cast<wchar_t>(VKCode), static_cast<int>(RepeatCount), static_cast<int>(ScanCode), IsExtendedKey, WasKeyDown, IsKeyReleased);
            return 0;
        }
        case WM_COMMAND:
        {
            LRESULT Return = Data->Command(wp, lp);
            return Return;
        }
        case WM_SIZE:
            return Data->Size();
        case WM_SETFOCUS:
            return Data->GotFocus();
        case WM_KILLFOCUS:
        {
            LRESULT Return = Data->LostFocus();
            SendMessageW(GetParent(Window), Message, wp, lp);
            return Return;
        }
        case WM_ENABLE:
            Data->IsEnabled = wp;
        case WM_SHOWWINDOW:
            RedrawWindow(Window, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);
            return 0;
        case WM_ERASEBKGND:
            return 1;

        default:
            return DefWindowProcW(Window, Message, wp, lp);
        }
    }

    LRESULT Control::KeyDown(WPARAM key)
    {
        SendMessageW(GetParent(_Base), WM_KEYDOWN, key, 0);
        return 0;
    }

    bool Control::Enabled() { return IsEnabled; }
    void Control::Enabled(bool New)
    {
        IsEnabled = New;
        EnableWindow(_Base, New);
        Redraw();
    }

    void Control::Redraw()
    {
        RedrawWindow(_Base, nullptr, nullptr, RDW_INVALIDATE | RDW_ERASENOW);
    }
}
