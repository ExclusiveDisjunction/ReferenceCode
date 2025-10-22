#pragma once

#include <Windows.h>
#include "Core\Color.h"
#include "Core\Vector.h"
#include "Core\Str.h"

#if (!defined(_WIN32) and !defined(_WIN64)) or !defined(__cplusplus)
#error Win32 or Win64 is required, and the code must be compled in C++
#endif

namespace Mason::UI::Controls
{
    Core::String GetText(HWND Window);
    void SetText(HWND Window, Core::String New);

    enum TextAlignment
    {
        TA_CenterAlignment,
        TA_LeftAlignment,
        TA_RightAlignment
    };

    struct StyleSheet
    {
        StyleSheet()
        {
            Background = 0xFF000000;
            BorderBrush = 0xFF000000;
            BorderThickness = 0;
            Radius = 0;
        }

        Core::Color Background;
        Core::Color BorderBrush;
        Core::Color BaseBackground;
        int BorderThickness;
        int Radius;
    };
    struct TextStyleSheet
    {
        Core::Color Foreground = 0xFFFFFFFF;
        Core::String FontFamily;
        int FontSize = 12, CharSpacing = 0;
        bool Bold = false;
        bool Italic = false;
        bool Underline = false;

        TextAlignment Alignment = TextAlignment::TA_LeftAlignment;
    };

    class ControlBase
    {
    protected:
        HWND _Base = nullptr;
        bool IsEnabled = true;

        static LRESULT __stdcall CommonWndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);

        virtual bool FocusOnClick() const { return true; }
    public:
        ControlBase()
        {
            _Base = nullptr;
        }
        virtual ~ControlBase()
        {
            SetWindowLongPtrW(_Base, GWLP_USERDATA, 0);
            DestroyWindow(_Base);
        }

        virtual LRESULT KeyDown(WPARAM key);
        virtual LRESULT Create() { return 0; }
        virtual LRESULT Paint() { return 0; }
        virtual LRESULT OnDestroy() { return 0; }
        virtual LRESULT Click() { return 0; }
        virtual LRESULT DoubleClick() { return SendMessageW(GetParent(_Base), WM_LBUTTONDBLCLK, 0, 0); }
        virtual LRESULT RDoubleClick() { return SendMessageW(GetParent(_Base), WM_RBUTTONDBLCLK, 0, 0); }
        virtual LRESULT MouseUp() { return 0; }
        virtual LRESULT TimerTick(LONG ID) { return 0; }
        virtual LRESULT Char(WPARAM ID) { return 0; }
        virtual LRESULT Command(WPARAM ID, LPARAM lp) { return SendMessageW(GetParent(_Base), WM_COMMAND, ID, lp); }
        virtual LRESULT Size() { return 0; }
        virtual LRESULT GotFocus() { return 0; }
        virtual LRESULT LostFocus() { return 0; }
        virtual LRESULT RightClick() { return SendMessageW(GetParent(_Base), WM_RBUTTONDOWN, 0,0); }

        bool Enable()
        {
            return IsEnabled;
        }
        void Enable(bool New)
        {
            IsEnabled = New;
            EnableWindow(_Base, New);
            Redraw();
        }

        virtual void Redraw()
        {
            RedrawWindow(_Base, NULL, NULL, RDW_INVALIDATE | RDW_ERASENOW);
        }
        virtual void Move(int X, int Y, int Width, int Height)
        {
            MoveWindow(_Base, X, Y, Width, Height, TRUE);
        }

        void Placement(RECT& WndRect) const
        {
            GetWindowRect(_Base, &WndRect);
        }
        int XCoord() const
        {
            RECT Place;
            Placement(Place);

            MapWindowPoints(HWND_DESKTOP, GetParent(_Base), reinterpret_cast<LPPOINT>(&Place), 2);

            return Place.left;
        }
        int YCoord() const
        {
            RECT Place;
            Placement(Place);

            MapWindowPoints(HWND_DESKTOP, GetParent(_Base), reinterpret_cast<LPPOINT>(&Place), 2);

            return Place.top;
        }

        operator HWND()
        {
            return _Base;
        }
    };

    class StyledControl : public ControlBase
    {
    protected:
        StyleSheet _Style;
        TextStyleSheet _TextStyle;
    public:
        StyleSheet const& Style = _Style;
        TextStyleSheet const& TextStyle = _TextStyle;

        void Background(const Core::Color& New)
        {
            _Style.Background = New;

            Redraw();
        }
        void BorderBrush(const Core::Color& New)
        {
            _Style.BorderBrush = New;

            Redraw();
        }
        void BorderThickness(int New)
        {
            _Style.BorderThickness = New;

            Redraw();
        }
        void SetStyle(const StyleSheet& New)
        {
            _Style = New;
            Redraw();
        }
        void SetTextStyle(const TextStyleSheet& New)
        {
            _TextStyle = New;
            Redraw();
        }
    };

    void GetClientRect(ControlBase* Source, LPRECT Dest);
    void GetWindowRect(ControlBase* Source, LPRECT Dest);
    
    template<std::convertible_to<ControlBase*>... Args>
    void CloseControls(Args... Obj)
    {
        for (ControlBase* Item : Obj)
            delete Item;
    }
    template<std::convertible_to<ControlBase*>... Args>
    void EnableControls(bool IsEnabled, Args... Obj)
    {
        for (ControlBase* Item : Obj)
            Item->Enable(IsEnabled);
    }
}