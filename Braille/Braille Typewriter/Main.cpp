#include "Main.h"

#include <gdiplus.h>
using namespace Gdiplus;

ATOM Main::_ThisAtom = ATOM();
void Main::InitBase(HINSTANCE ins)
{
	WNDCLASSW wn = { 0 };
	wn.lpfnWndProc = Main::WndProc;
	wn.hInstance = ins;
	wn.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wn.hbrBackground = NULL;
	wn.lpszMenuName = NULL;
	wn.lpszClassName = L"Main";
	wn.hIcon = LoadIconW(NULL, IDI_APPLICATION);
	wn.cbClsExtra = 0;
	wn.cbWndExtra = sizeof(Main*);
	wn.style = CS_HREDRAW | CS_VREDRAW;

	_ThisAtom = RegisterClassW(&wn);
}
LRESULT __stdcall Main::WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp)
{
	Main* Item = reinterpret_cast<Main*>(GetWindowLongPtrW(Window, GWLP_USERDATA));
	if (!Item)
		return DefWindowProcW(Window, Message, wp, lp);

	switch (Message)
	{
	case WM_PAINT:
		return Item->Paint();
	case WM_KEYDOWN:
		return Item->KeyDown(wp);
	case WM_KEYUP:
		return Item->KeyUp(wp);
	case WM_DESTROY:
		return Item->Destroy();
	default:
		return DefWindowProcW(Window, Message, wp, lp);
	}
}

LRESULT Main::KeyDown(WPARAM wp)
{
	if (wp == VK_ESCAPE)
	{
		CloseControls(Letters);
		CurrentX = 10;
		CurrentY = 10;

		return 0;
	}
	else if (wp == VK_RETURN)
	{
		if (Letters.Size() == 0)
			return 0;

		CurrentY += 5 + Letters[0]->Height();
		CurrentX = 10;
		return 0;
	}
	else if (wp == VK_BACK)
	{
		if (CurrentX == 10)
			return 0;
         
		BrailleCell* Target = Letters[Letters.Size() - 1];
		CurrentX -= Target->Width() + 1;
		Letters.Remove(Target);
		CloseControl(Target);
	}
	else if (wp == VK_TAB)
	{
		String Text = InterpretText(Letters);
		MessageBoxW(_Base, static_cast<LPCWSTR>(Text), L"Output:", MB_OK);
	}

	bool ProperKey = wp == 0x53 || wp == 0x44 || wp == 0x46 || wp == VK_SPACE || wp == 0x4A || wp == 0x4B || wp == 0x4C;
	if (!ProperKey)
		return SendMessageW(GetParent(_Base), WM_KEYDOWN, wp, 0);

	KeyMode = true;
	if (!DownKeys.Contains(wp))
		DownKeys.Add(wp);

	return 0;
}
LRESULT Main::KeyUp(WPARAM wp)
{
	bool ProperKey = wp == 0x53 || wp == 0x44 || wp == 0x46 || wp == VK_SPACE || wp == 0x4A || wp == 0x4B || wp == 0x4C;
	if (!ProperKey)
		return SendMessageW(GetParent(_Base), WM_KEYDOWN, wp, 0);

	if (!UpKeys.Contains(wp))
		UpKeys.Add(wp);

	if (UpKeys.Size() == DownKeys.Size())
	{
		KeyMode = false;
		FillKey();

		UpKeys.Clear();
		DownKeys.Clear();
	}

	return 0;
}
void Main::FillKey()
{
	bool Three = UpKeys.Find([](WPARAM& Item) { return Item == 0x53; }); //S, 3
	bool Two = UpKeys.Find([](WPARAM& Item) { return Item == 0x44; }); //D, 2
	bool One = UpKeys.Find([](WPARAM& Item) { return Item == 0x46; }); //F, 1

	bool Four = UpKeys.Find([](WPARAM& Item) { return Item == 0x4A; }); //J, 4
	bool Five = UpKeys.Find([](WPARAM& Item) { return Item == 0x4B; }); //K, 5
	bool Six = UpKeys.Find([](WPARAM& Item) { return Item == 0x4C; }); //L, 6

	HINSTANCE ins = reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(_Base, GWLP_HINSTANCE));

	BrailleCell* New = new BrailleCell(CurrentX, CurrentY, 0xFF313131, 0xFFAAAAAA, ins, _Base);
	New->SetState(One, Two, Three, Four, Five, Six);
	Letters.Add(New);

	CurrentX += New->Width() + 1;
}

LRESULT Main::Paint()
{
	PAINTSTRUCT ps = { 0 };
	HDC Dc = BeginPaint(_Base, &ps);

	Graphics* Graph = Graphics::FromHDC(Dc);

	RECT WndRect;
	GetClientRect(_Base, &WndRect);
	Rect Area(WndRect.left, WndRect.top, WndRect.right, WndRect.bottom);

	SolidBrush Bk(0xFF313131);
	Graph->FillRectangle(&Bk, Area);

	delete Graph;
	EndPaint(_Base, &ps);
	return 0;
}
LRESULT Main::Destroy()
{
	CloseControls(Letters);

	PostQuitMessage(0);
	return 0;
}

String InterpretText(Vector<BrailleCell*>& Items)
{
	String Return;
	bool LastUppercase = false;
	bool NumericMode = false;
	int LastY = 10;
	for (uint i = 0; i < Items.Size(); i++)
	{
		BrailleCell*& Item = Items[i];
		if (Item->YCoord() != LastY)
		{
			int LineDiff = (Item->YCoord() - LastY) / (Item->Height() + 5);
			for (int i = 0; i < LineDiff; i++)
				Return += L'\n';
			LastY = Item->YCoord();
		}
		wchar_t Current = L'\0';

		if (Item->IsInState(false, false, false, false, false, true))
			LastUppercase = true;
		else if (Item->IsInState(false, true, false, true, true, true))
			NumericMode = true;
		else if (Item->IsInState(false, false, false, true, false, true))
			NumericMode = false;

		else if (Item->IsInState(true, false, false, false, false, false))
			Current = NumericMode ? L'1' : L'a';
		else if (Item->IsInState(true, false, true, false, false, false))
			Current = NumericMode ? L'2' : L'b';
		else if (Item->IsInState(true, true, false, false, false, false))
			Current = NumericMode ? L'3' : L'c';
		else if (Item->IsInState(true, true, false, true, false, false))
			Current = NumericMode ? L'4' : L'd';
		else if (Item->IsInState(true, false, false, true, false, false))
			Current = NumericMode ? L'5' : L'e';
		else if (Item->IsInState(true, true, true, false, false, false))
			Current = NumericMode ? L'6' : L'f';
		else if (Item->IsInState(true, true, true, true, false, false))
			Current = NumericMode ? L'7' : L'g';
		else if (Item->IsInState(true, false, true, true, false, false))
			Current = NumericMode ? L'8' : L'h';
		else if (Item->IsInState(false, true, true, false, false, false))
			Current = NumericMode ? L'9' : L'i';
		else if (Item->IsInState(false, true, true, true, false, false))
			Current = NumericMode ? L'0' : L'j';

		else if (Item->IsInState(true, false, false, false, true, false))
			Current = L'k';
		else if (Item->IsInState(true, false, true, false, true, false))
			Current = L'l';
		else if (Item->IsInState(true, true, false, false, true, false))
			Current = L'm';
		else if (Item->IsInState(true, true, false, true, true, false))
			Current = L'n';
		else if (Item->IsInState(true, false, false, true, true, false))
			Current = L'o';
		else if (Item->IsInState(true, true, true, false, true, false))
			Current = L'p';
		else if (Item->IsInState(true, true, true, true, true, false))
			Current = L'q';
		else if (Item->IsInState(true, false, true, true, true, false))
			Current = L'r';

		else if (Item->IsInState(false, true, true, false, true, false))
			Current = L's';
		else if (Item->IsInState(false, true, true, true, true, false))
			Current = L't';
		else if (Item->IsInState(true, false, false, false, true, true))
			Current = L'u';
		else if (Item->IsInState(true, false, true, false, true, true))
			Current = L'v';
		else if (Item->IsInState(false, true, true, true, false, true))
			Current = L'w';
		else if (Item->IsInState(true, true, false, false, true, true))
			Current = L'x';
		else if (Item->IsInState(true, true, false, true, true, true))
			Current = L'y';
		else if (Item->IsInState(true, false, false, true, true, true))
			Current = L'z';

		else if (Item->IsInState(false, false, false, false, false, false))
			Current = L' ';
		else if (Item->IsInState(false, false, true, false, false, false))
			Current = L',';
		else if (Item->IsInState(false, false, false, false, true, false))
			Current = L'\'';

		if (Current > L'j' && Current <= L'z' && !Item->IsInState(false, false, false, true, false, false))
			NumericMode = false;

		if (Current == L'\0')
			continue;
		else if (LastUppercase && Current >= L'a' && Current <= L'z')
		{
			Current -= 32;
			LastUppercase = false;
		}

		Return += Current;
	}

	return Return;
}
