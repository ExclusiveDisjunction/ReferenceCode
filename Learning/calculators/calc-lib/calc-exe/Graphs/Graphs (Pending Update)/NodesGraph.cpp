#include "NodesGraph.h"

#include "..\UICommon.h"

NodesGraph::NodesGraph(int X, int Y, int Width, int Height, HWND Parent, HINSTANCE ins)
{
	if (!_ThisAtom)
		InitBase(ins);

	Mode = SelectionMode;

	_Base = CreateWindowEx(0l, MAKEINTATOM(_ThisAtom), NULL, WS_CHILD | WS_VISIBLE, X, Y, Width, Height, Parent, nullptr, ins, nullptr);
	SetWindowLongPtr(_Base, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	RECT WndRect;
	GetClientRect(_Base, &WndRect);

	OPos.Y = WndRect.bottom / 2.0;
	OPos.X = WndRect.right / 2.0;
}
NodesGraph::~NodesGraph()
{
	for (Box* Item : Boxes)
		delete Item;
	for (Connection* Item : Connections)
		delete Item;

	Boxes.Clear();
	Connections.Clear();
}

ATOM NodesGraph::_ThisAtom = ATOM();
void NodesGraph::InitBase(HINSTANCE ins)
{
	WNDCLASS wn = { 0 };
	wn.lpfnWndProc = NodesGraph::WndProc;
	wn.lpszClassName = TEXT("NodesGraphClass");
	wn.hCursor = Mason::MasonCursor;
	wn.hInstance = ins;
	wn.cbClsExtra = 0;
	wn.cbWndExtra = sizeof(NodesGraph*);
	wn.hbrBackground = nullptr;
	wn.lpszMenuName = nullptr;
	wn.style = CS_HREDRAW | CS_VREDRAW;

	_ThisAtom = RegisterClass(&wn);
}
LRESULT __stdcall NodesGraph::WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp)
{
	NodesGraph* Item = reinterpret_cast<NodesGraph*>(GetWindowLongPtr(Window, GWLP_USERDATA));
	if (!Item)
		return DefWindowProc(Window, Message, wp, lp);

	switch (Message)
	{
	case WM_PAINT:
		return Item->Paint();
	case WM_LBUTTONDOWN:
		return Item->MouseDown();
	case WM_LBUTTONUP:
		return Item->MouseUp();
	case WM_MOUSEWHEEL:
		return Item->MouseWheel(wp);
	case WM_MOUSEMOVE:
		return Item->MouseMove();
	case WM_CHAR:
		return Item->Char(wp);
	case WM_KEYDOWN:
		return Item->KeyDown(wp);
	default:
		return DefWindowProc(Window, Message, wp, lp);
	}
}

LRESULT NodesGraph::Paint()
{
	PAINTSTRUCT ps;
	HDC Dc = BeginPaint(_Base, &ps);

	RECT WndRect;
	GetClientRect(_Base, &WndRect);

	HBRUSH Bk = CreateSolidBrush(0x000000);
	FillRect(Dc, &WndRect, Bk);
	DeleteObject(Bk);

	Vector2d iHat = { Zoom, 0 }, jHat = { 0, Zoom };
	Vector2d TiHat = OriginToScreen(iHat, OPos, WndRect), TjHat = OriginToScreen(jHat, OPos, WndRect);

	{
		Vector<Vector2d> RenderPoints = Origin;

		HPEN BoxPen = CreatePen(PS_SOLID, 5, 0x00FF00);
		HGDIOBJ OldPen = SelectObject(Dc, BoxPen);
		HGDIOBJ OldBrush = SelectObject(Dc, GetStockObject(NULL_BRUSH));
		for (Vector2d& Obj : RenderPoints)
		{
			//Vector2d Tran = Obj.X * iHat + Obj.Y * jHat;
			Vector2d Tran = OriginToScreen(Obj, OPos, WndRect);

			if (!PtInRect(&WndRect, Tran.operator POINT()))
				continue;

			int By = 12;
			Rectangle(Dc, long(Tran.X) - By, long(Tran.Y) - By, long(Tran.X + By), long(Tran.Y + By));
		}

		DeleteObject(BoxPen);
		SelectObject(Dc, OldPen);
		SelectObject(Dc, OldBrush);
	}

	if (!IsMouseDown)
	{
		//double Precis = 1 / ((Zoom + 1) * 16);

		//int Rate = int(50 * (1 / Zoom)) + 10;
		RECT BoundRect = { 0, 0, WndRect.right, WndRect.bottom }; //The bounding edge for rendering. If a node is outside of this range, it will not be considered for rendering.

		double BoxSize = 0;

		SetTextColor(Dc, 0xFFFFFF);
		SetBkMode(Dc, TRANSPARENT);

		//PUT BOX RENDERING HERE
		{
			for (Box* Item : Boxes)
			{
				AaColor Color = (Item == CurrentBox ? AaColor(0xFFFFFFFF) : HSVtoRGB(Item->Hue, 80, 80));
				HBRUSH Base = CreateSolidBrush(HSVtoRGB(Item->Hue, 80, 80));
				HPEN Border = CreatePen(PS_SOLID, 3, Color);
				if (Item == CurrentBox) //If Item is the current box, then it will be highlighted so the user can see that it is active.
					SetTextColor(Dc, 0x000000);
				else
					SetTextColor(Dc, 0xFFFFFF);

				SelectObject(Dc, Base);
				SelectObject(Dc, Border);

				Vector2d TopLeft = OriginToScreen(Item->Point - Vector2d(Item->Size, Item->Size), OPos, WndRect); //Determines the locations of the rectangle node.
				Vector2d BottomRight = OriginToScreen(Item->Point + Vector2d(Item->Size, Item->Size), OPos, WndRect);

				int By = 30;
				BoxSize = 1;
				RECT Rect = { (int)TopLeft.X, (int)TopLeft.Y, (int)BottomRight.X, (int)BottomRight.Y };
				Rectangle(Dc, Rect.left, Rect.top, Rect.right, Rect.bottom); //Draws a rectangle representingg a node.
				DrawTextW(Dc, static_cast<LPCWSTR>(Item->Text), Item->Text.Length(), &Rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER); //Draws the text of the box into the center of the node.

				DeleteObject(Base);
				DeleteObject(Border);
			}
		}

		//PUT LINE CONNECTIONS HERE
		{
			for (Connection* Item : Connections)
			{
				AaColor Color = (Item == CurrentConnection ? AaColor(0xFFFFFF) : Item->Color);
				HPEN Border = CreatePen(PS_SOLID, 3 * Zoom, Color);
				SelectObject(Dc, Border);

				Box* Source = Item->One;
				Box* To = Item->Two;
				int BoxSize1 = Source->Size;
				int BoxSize2 = To->Size;

				CubicBezier Bez; //Sets up the cubic bezier to connect the different nodes together. Joins from bottom of Source to top of To.
				Bez.Begin = Source->Point - Vector2d(0, BoxSize1);
				Bez.A1 = Source->Point - Vector2d(0, Radius + BoxSize1);
				Bez.End = To->Point + Vector2d(0, BoxSize2);
				Bez.A2 = To->Point + Vector2d(0, Radius + BoxSize2);

				bool Begin = true; //When true, the algorithim will move to the current point instaed of drawing to it.
				for (double T = 0.0; T <= 1.05; T += 0.05) //Draws the cubic bezier path.
				{
					Vector2d Temp = Bez(T);
					Vector2d Tran = OriginToScreen(Temp, OPos, WndRect);
					if (Begin)
					{
						MoveToEx(Dc, (int)Tran.X, (int)Tran.Y, nullptr);
						Begin = false;
					}
					else
						LineTo(Dc, (int)Tran.X, (int)Tran.Y);
				}

				DeleteObject(Border);
			}
		}
	}

	//This block sets up the status for the top left corner.
	SetTextColor(Dc, 0xFFFFFF);
	String CurrentMode;
	switch (Mode)
	{
	case LinkMode:
		CurrentMode = L"Link Mode";
		break;
	case MoveMode:
		CurrentMode = L"Move Mode";
		break;
	case ConnectionSelMode:
		CurrentMode = L"Connection Selection Mode";
		break;
	case AddNodeMode:
		CurrentMode = L"Add Node Mode";
		break;
	case RadiusChangeMode:
		CurrentMode = L"Radius Change Mode, Radius = " + String(Radius);
		break;
	default:
		CurrentMode = L"Selection Mode";
		break;
	}
	if (Mode & AcceptingNumber)
		CurrentMode += ", Accepting Number Currently: \"" + RawTextIn + L"\".";
	else if (CurrentBox)
		CurrentMode += L", Selected Box #" + String(CurrentIndex + 1);
	TextOut(Dc, 10, 10, static_cast<LPCWSTR>(CurrentMode), CurrentMode.Length());

	EndPaint(_Base, &ps);
	return 0;
}
LRESULT NodesGraph::MouseDown()
{
	SetFocus(_Base);

	POINT Cursor;
	GetCursorPos(&Cursor);

	if (Mode == AddNodeMode)
	{
		RECT WndRect;
		GetClientRect(_Base, &WndRect);

		Vector2d Cur = Vector2d((double)Cursor.x, (double)Cursor.y);
		Vector2d Conv = ScreenToOrigin(Cur, OPos, WndRect);

		Box* Item = new Box(Conv);
		Boxes.Add(Item);
		Item->Text = Boxes.Size;

		RedrawWindow(_Base, nullptr, nullptr, RDW_ERASENOW | RDW_INVALIDATE);
		return 0;
	}
	else if (Mode == MoveMode && CurrentBox)
	{
		RECT WndRect;
		GetClientRect(_Base, &WndRect);

		Vector2d Cur = Vector2d((double)Cursor.x, (double)Cursor.y);
		Vector2d Conv = ScreenToOrigin(Cur, OPos, WndRect);

		CurrentBox->Point = Conv;

		RedrawWindow(_Base, nullptr, nullptr, RDW_ERASENOW | RDW_INVALIDATE);
		return 0;
	}

	RECT WndRect;
	GetClientRect(_Base, &WndRect);

	Vector2d Cur = Vector2d((double)Cursor.x, (double)Cursor.y);
	Vector2d Conv = ScreenToOrigin(Cur, OPos, WndRect);

	bool DidFindBlock = false;
	for (Box* Item : Boxes)
	{
		struct RectD
		{
			double Left, Top, Right, Bottom;
		} Bounds = { Item->Point.X - 1, Item->Point.Y - 1, Item->Point.X + 1, Item->Point.Y + 1 };

		auto PointInRectD = [](RectD Rect, Vector2d Point)
		{
			if (Point.X <= Rect.Right && Point.X >= Rect.Left && Point.Y >= Rect.Bottom && Point.Y <= Rect.Top)
				return true;
			else
				return false;
		};

		RECT Temp = RECT{ (long)Item->Point.X - 1, (long)Item->Point.Y - 1, (long)Item->Point.X + 1, (long)Item->Point.Y + 1 };

		if (PtInRect(&Temp, (POINT)Conv))//PointInRectD(Bounds, Conv))
		{
			if (Mode & LinkMode)
			{
				if (CurrentBox == nullptr)
				{
					CurrentIndex = 0;
					CurrentBox = Item;
				}
				else if (CurrentBox == Item)
				{
					MessageBox(_Base, TEXT("A box cannot be linked to itself."), TEXT("Error"), MB_OK | MB_ICONERROR);
					CurrentIndex = 0;
					CurrentBox = nullptr;
				}
				else
				{
					Box* This = Item;

					for (Connection* NItem : Connections)
					{
						if (NItem->One == CurrentBox && NItem->Two == This)
						{
							MessageBox(_Base, TEXT("A link already exists between these two nodes."), TEXT("Error:"), MB_ICONERROR | MB_OK);
							return 0;
						}
					}

					Connections.Add(new Connection(CurrentBox, This));

					CurrentIndex = 0;
					CurrentBox = nullptr;
				}
			}
			else
			{
				CurrentIndex = Boxes.IndexOf(Item);
				CurrentBox = Item;
			}

			RedrawWindow(_Base, NULL, nullptr, RDW_INVALIDATE | RDW_ERASENOW);
			DidFindBlock = true;
			break;
		}
	}

	if (DidFindBlock)
		return 0;

	CurrentIndex = 0;
	CurrentBox = nullptr;
	CurrentConnection = nullptr;

	CursorLast = { double(Cursor.x), double(Cursor.y) };
	OPosLast = OPos;

	IsMouseDown = true;
	return 0;
}
LRESULT NodesGraph::MouseUp()
{
	IsMouseDown = false;
	RedrawWindow(_Base, nullptr, nullptr, RDW_INVALIDATE | RDW_ERASENOW);
	return 0;
}
LRESULT NodesGraph::MouseMove()
{
	if (IsMouseDown)
	{
		if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
		{
			IsMouseDown = false;
			return 0;
		}

		POINT NewLoc;
		GetCursorPos(&NewLoc);

		double ChangeY = double(NewLoc.y) - CursorLast.Y;
		ChangeY *= 2;
		double ChangeX = double(NewLoc.x) - CursorLast.X;
		ChangeX *= 2;

		OPos = OPosLast + Vector2d(ChangeX, ChangeY);
		RedrawWindow(_Base, nullptr, nullptr, RDW_ERASENOW | RDW_INVALIDATE);
	}

	if (Mode == MoveMode && CurrentBox && (GetKeyState(VK_SHIFT) & 0x8000))
	{
		POINT Cursor;
		GetCursorPos(&Cursor);

		RECT WndRect;
		GetClientRect(_Base, &WndRect);

		Vector2d Cur = Vector2d((double)Cursor.x, (double)Cursor.y);
		Vector2d Conv = ScreenToOrigin(Cur, OPos, WndRect);

		CurrentBox->Point = Conv;

		RedrawWindow(_Base, nullptr, nullptr, RDW_ERASENOW | RDW_INVALIDATE);
		return 0;
	}
	return 0;
}
LRESULT NodesGraph::MouseWheel(WPARAM wp)
{
	if (CurrentBox && (GetKeyState(VK_CONTROL) & 0x8000))
	{
		int& New = CurrentBox->Hue;
		double By = GET_WHEEL_DELTA_WPARAM(wp) / 8;
		New += (int)By;

		if (New < 0) { New = 0; return 0; }
		else if (New > 360) { New = 360; return 0; }
	}
	return 0;
}
LRESULT NodesGraph::Char(WPARAM wp)
{
	wchar_t Key = wp;
	RECT WndRect;
	GetClientRect(_Base, &WndRect);

	switch (Key)
	{
	case L'O':
	case L'o':
		OPos = { (double)WndRect.right / 2, (double)WndRect.bottom / 2 };
		Zoom = 1;
		return 0;
	case L'A':
	case L'a':
	{
		if (Mode & Modes::AddNodeMode)
			Mode = 0;
		else
			Mode = AddNodeMode;
		return 0;
	}
	case L'L':
	case L'l': //Link Mode
	{
		if (Mode & LinkMode)
			Mode = 0;
		else
			Mode = LinkMode;
		return 0;
	}
	case L'M':
	case L'm': //Move Mode
	{
		if (Mode & MoveMode)
			Mode = 0;
		else
		{
			CurrentConnection = nullptr;
			Mode = MoveMode;
		}
		return 0;
	}
	case L'C':
	case L'c': //Connection Selection Mode
	{
		if (Mode & ConnectionSelMode)
			Mode = 0;
		else
		{
			CurrentBox = nullptr;
			Mode = ConnectionSelMode;
		}
		return 0;
	}
	case L'N':
	case L'n': //Number Entry
	{
		Mode |= AcceptingNumber;
		RawTextIn.Clear();
		return 0;
	}
	case L'R':
	case L'r':
	{
		if (Mode & RadiusChangeMode)
			Mode = 0;
		else
		{
			RawTextIn.Clear();
			Mode = RadiusChangeMode;
			CurrentBox = nullptr;
			CurrentConnection = nullptr;
			CurrentIndex = 0;
		}
		break;
	}
	case L'0':
	case L'1':
	case L'2':
	case L'3':
	case L'4':
	case L'5':
	case L'6':
	case L'7':
	case L'8':
	case L'9':
	{
		uint Num = wp - L'0';
		Num--;

		if (Mode & AcceptingNumber)
		{
			Num++;
			RawTextIn += String(Num);
		}
		else
		{
			if (Mode & RadiusChangeMode)
			{
				Radius = Num + 1;
				return 0;
			}

			uint Size = (Mode & ConnectionSelMode) ? Connections.Size : Boxes.Size;
			if (Num < 0 || Num >= Size)
			{
				MessageBox(_Base, TEXT("The range is invalid."), L"Bound Error:", MB_OK | MB_ICONERROR);
				break;
			}

			if (Mode & LinkMode)
			{
				if (CurrentBox == nullptr)
				{
					CurrentIndex = Num;
					CurrentBox = Boxes[Num];
				}
				else if (CurrentBox == Boxes[Num])
				{
					MessageBox(_Base, TEXT("A box cannot be linked to itself."), TEXT("Error"), MB_OK | MB_ICONERROR);
					CurrentIndex = 0;
					CurrentBox = nullptr;
				}
				else
				{
					Box* This = Boxes[Num];

					for (Connection* Item : Connections)
					{
						if (Item->One == CurrentBox && Item->Two == This)
						{
							MessageBox(_Base, TEXT("A link already exists between these two nodes."), TEXT("Error:"), MB_ICONERROR | MB_OK);
							return 0;
						}
					}

					Connections.Add(new Connection(CurrentBox, This));

					CurrentIndex = 0;
					CurrentBox = nullptr;
				}
			}
			else
			{
				if (Mode & ConnectionSelMode)
					CurrentConnection = Connections[Num];
				else
					CurrentBox = Boxes[Num];
				CurrentIndex = Num;
			}
		}
	}
	}

	RedrawWindow(_Base, nullptr, nullptr, RDW_INVALIDATE | RDW_ERASENOW);
	return 0;
}
LRESULT NodesGraph::KeyDown(WPARAM wp)
{
	WPARAM Key = wp;

	RECT WndRect;
	GetClientRect(_Base, &WndRect);

	switch (Key)
	{
	case VK_RETURN:
	{
		if (Mode & Modes::AcceptingNumber)
		{
			Mode &= ~AcceptingNumber;

			uint Num = RawTextIn.ToUInt();
			Num--;

			if (Mode & RadiusChangeMode)
			{
				Radius = Num + 1;
				return 0;
			}

			uint Size = Mode & ConnectionSelMode ? Connections.Size : Boxes.Size;
			if (Num < 0 || Num >= Size)
			{
				MessageBox(_Base, TEXT("The range is invalid."), L"Bound Error:", MB_OK | MB_ICONERROR);
				break;
			}

			if (Mode & LinkMode)
			{
				if (CurrentBox == nullptr)
				{
					CurrentIndex = Num;
					CurrentBox = Boxes[Num];
				}
				else if (CurrentBox == Boxes[Num])
				{
					MessageBox(_Base, TEXT("A box cannot be linked to itself."), TEXT("Error"), MB_OK | MB_ICONERROR);
					CurrentIndex = 0;
					CurrentBox = nullptr;
				}
				else
				{
					Box* This = Boxes[Num];

					for (Connection* Item : Connections)
					{
						if (Item->One == CurrentBox && Item->Two == This)
						{
							MessageBox(_Base, TEXT("A link already exists between these two nodes."), TEXT("Error:"), MB_ICONERROR | MB_OK);
							return 0;
						}
					}

					Connections.Add(new Connection(CurrentBox, This));

					CurrentIndex = 0;
					CurrentBox = nullptr;
				}
			}
			else
			{
				CurrentIndex = Num;
				if (Mode & ConnectionSelMode)
					CurrentConnection = Connections[Num];
				else
					CurrentBox = Boxes[Num];
			}

			RawTextIn.Clear();
			break;
		}

		if (Mode & RadiusChangeMode)
			Mode &= ~RadiusChangeMode;

		break;
	}
	case VK_BACK:
	{
		if (Mode & Modes::AcceptingNumber)
		{
			if (RawTextIn.Length() > 0)
				RawTextIn.RemoveAt(RawTextIn.Length() - 1);
		}
		break;
	}
	case VK_DELETE:
	{
		if (Mode & ConnectionSelMode)
		{
			if (!CurrentConnection)
				return 0;

			Connections.Remove(CurrentConnection);
			delete CurrentConnection;

			CurrentConnection = nullptr;
			CurrentIndex = 0;
		}
		else
		{
			if (!CurrentBox)
				return 0;

			Boxes.Remove(CurrentBox);
			Box* TempBox = CurrentBox; //DO NOT REFERENCE, simply for comparing any connections.
			delete CurrentBox;

			int Index = 1;
			for (Box* Item : Boxes)
			{
				Item->Text = Index;
				Index++;
			}

			for (int i = (int)Connections.Size - 1; i >= 0; i--)
			{
				Connection* Item = Connections[i];
				if (Item->One == TempBox || Item->Two == TempBox)
				{
					Connections.Remove(Item);
					delete Item;
				}
			}

			CurrentBox = nullptr;
			CurrentIndex = 0;
		}

		RedrawWindow(_Base, nullptr, nullptr, RDW_ERASENOW | RDW_INVALIDATE);
		return 0;
	}
	case VK_ESCAPE:
		Mode = 0;
		CurrentIndex = 0;
		RawTextIn.Clear();
		CurrentBox = nullptr;

		if (GetKeyState(VK_SHIFT) & 0x8000)
		{
			for (Box* Item : Boxes)
				delete Item;
			Boxes.Clear();
			for (Connection* Item : Connections)
				delete Item;
			Connections.Clear();
		}
		break;
	}

	RedrawWindow(_Base, nullptr, nullptr, RDW_INVALIDATE | RDW_ERASENOW);
	return 0;
}