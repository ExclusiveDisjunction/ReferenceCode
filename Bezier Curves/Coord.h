#pragma once

#include <Windows.h>

#include "BaseMath.h"
#include "Container.h"

class TrackedCoord
{
protected:
	Vector<Vector2d*> Modify;
	HWND _Base;
	bool _Track;
	COLORREF Border, Bk;

	static LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);
	static ATOM _ThisAtom;
	static void InitBase(HINSTANCE ins);

	LRESULT Paint();
	LRESULT MouseDown();
	LRESULT MouseUp();
	LRESULT Timer(WPARAM wp);

	virtual void OnMove(Vector2d NewPlace) {}
public:
	TrackedCoord(Vector<Vector2d*> Bind, int Size, HWND Parent, COLORREF Border, COLORREF Bk, HINSTANCE ins);

	Vector<Vector2d*> Binding() const { return Modify; }
	void Binding(Vector<Vector2d*> New);
	void AddToBind(Vector2d* New);

	operator HWND() const { return _Base; }
};

class Anchor;
class Modifyer;

class Anchor : public TrackedCoord
{
private:
	Modifyer* M1, *M2;
	Vector2d LastPoint;

	void OnMove(Vector2d NewPlace) override;
public:
	Anchor(Vector<Vector2d*> Bind, int Size, HWND Parent, COLORREF Border, COLORREF Bk, HINSTANCE ins, Modifyer* Modifyer1 = nullptr, Modifyer* Modifyer2 = nullptr) : TrackedCoord(Bind, Size, Parent, Border, Bk, ins)
	{
		M1 = Modifyer1;
		M2 = Modifyer2;
		LastPoint = Bind.Size() != 0 ? *Bind[0] : Vector2d();
	}

	std::pair<Modifyer*, Modifyer*> Modifyers() const { return { M1, M2 }; }
	void Modifyers(Modifyer* One, Modifyer* Two) { Modifyers({ One, Two }); }
	void Modifyers(std::pair<Modifyer*, Modifyer*> New);
};
class Modifyer : public TrackedCoord
{
private:
	Modifyer* Pair;
	Anchor* Root;

	void OnMove(Vector2d NewPlace) override;
public:
	Modifyer(Vector<Vector2d*> Bind, int Size, HWND Parent, COLORREF Border, COLORREF Bk, HINSTANCE ins, Anchor* Anc = nullptr, Modifyer* Pair = nullptr) : TrackedCoord(Bind, Size, Parent, Border, Bk, ins)
	{
		Pair = Pair;
		Root = Anc;
	}

	Modifyer* PairedTo() const { return Pair; }
	void PairedTo(Modifyer* New);
	Anchor* ThisAnchor() const { return Root; }
	void ThisAnchor(Anchor* New);
};