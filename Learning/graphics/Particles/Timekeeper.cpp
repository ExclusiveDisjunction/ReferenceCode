#include "Timekeeper.h"

#include <vector>
using namespace std;

namespace TimekeeperInfo
{
	struct Segment
	{
		Segment* Next, * Last;
		TimekeeperSubscriber* Value;
	};

	double CurrentTime = 0.0, LastUpdate = 0.0, FPS = 12 / 60;
	bool Running = false;
	std::atomic<Segment*> Subscribers;

	std::atomic<HWND> ToRender;

	std::thread MasterTime, UpdateTime;

	void MasterTimekeeper()
	{
		while (Running)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			CurrentTime += 0.1;
		}
	}
	void UpdateRenderCycle()
	{
		while (Running)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(FPS * 100)));

			if (Subscribers == nullptr)
				continue;

			HDC Dc = GetDC(ToRender);
			RECT Area;
			GetClientRect(ToRender, &Area);

			HBRUSH Bk = CreateSolidBrush(0x000000);
			FillRect(Dc, &Area, Bk);
			DeleteObject(Bk);

			double DeltaTime = CurrentTime - LastUpdate;
			for (Segment* This = Subscribers; This && This != nullptr && Running; This = This->Next)
			{
				TimekeeperSubscriber* Current = This->Value;

				Current->OnUpdate(DeltaTime);
				Current->OnRender(Dc, Area);
			}

			LastUpdate = CurrentTime;
			ReleaseDC(ToRender, Dc);
		}
	}
}

using namespace TimekeeperInfo;

void Timekeeper::Init(HWND ToRender)
{
	TimekeeperInfo::ToRender = ToRender;
	Running = true;
	FPS = 8.0 / 60.0;

	MasterTime = std::thread(MasterTimekeeper);
	UpdateTime = std::thread(UpdateRenderCycle);

	//MasterTime.detach();
	//UpdateTime.detach();
}
void Timekeeper::Close()
{
    Running = false;
	
	if (MasterTime.joinable())
		MasterTime.join();

	if (UpdateTime.joinable())
		UpdateTime.join();
}

void Timekeeper::ForceRender()
{
	HDC Dc = GetDC(TimekeeperInfo::ToRender);
	RECT Area;
	GetClientRect(ToRender, &Area);

	HBRUSH Bk = CreateSolidBrush(0x000000);
	FillRect(Dc, &Area, Bk);

	for (Segment* This = Subscribers; This && This != nullptr; This = This->Next)
		This->Value->OnRender(Dc, Area);

	ReleaseDC(ToRender, Dc);
}

void Timekeeper::FramesPerSecond(int New)
{
	if (New > 60 || New <= 0)
		return;

	FPS = New / 60.0;
}
void Timekeeper::Subscribe(TimekeeperSubscriber* New)
{
	if (Subscribers == nullptr)
	{
		Subscribers = new Segment();
		Subscribers.load()->Value = New;
	}

	Segment* Last = nullptr;
	for (Segment* This = Subscribers; This && This->Next != nullptr; This = This->Next)
	{
		if (This->Value == New)
			return;
		Last = This;
	}

	Segment* NewSeg = new Segment();
	NewSeg->Last = Last;
	if (Last)
		Last->Next = NewSeg;
	NewSeg->Value = New;
}
void Timekeeper::Desubscribe(TimekeeperSubscriber* Obj)
{
	if (Subscribers == nullptr)
		return;

	if (Subscribers.load()->Value == Obj)
	{
		Segment* Next = Subscribers.load()->Next;
		if (Next)
			Next->Last = nullptr;

		delete Subscribers.load();
		Subscribers = Next;
	}

	Segment* Target = nullptr;
	for (Segment* This = Subscribers; This && This != nullptr; This = This->Next)
	{
		if (This->Value == Obj)
		{
			Target = This;
			break;
		}
	}

	if (!Target)
		return;

	Segment* Prev = Target->Last, * Next = Target->Next;
	if (Prev)
		Prev->Next = Next;
	if (Next)
		Next->Last = Prev;

	delete Target;
}