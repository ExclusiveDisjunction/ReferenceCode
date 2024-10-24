#pragma once

#include "BaseMath.h"

#include "Timekeeper.h"
#include "Container.h"
#include "BasicWindow.h"

#include <functional>
#include <chrono>
#include <future>

struct ParticleProps
{
	Vector Position;
	Vector Velocity, VelocityVariation, Gravity;
	AngleRange VelocityAngle;
	AaColor ColorBegin, ColorEnd;
	float SizeBegin, SizeEnd;
	float LifeTime = 1.0f;
};

class ParticleSystem : public TimekeeperSubscriber, public CallableTimer
{
private:
	static void TickEmit(ParticleSystem* System);
	std::thread ThisThread;

	struct Particle
	{
		Vector Pos;
		Vector Velocity, VelocityInitial;
		AaColor ColorBegin, ColorEnd;
		float SizeBegin, SizeEnd, Size;
		unsigned long long Color;

		float LifeTime = 1.0f;
		float LifeRemaining = 0.0f;

		bool Active = false;
	};

	ParticleProps Data;
	Container<Particle> Particles;
	uint PoolIndex = 1999;
	int EmitAmount;
	double EmitTimePeriod;
	//HWND TimerWindow;

	ParticleSystem(const ParticleSystem& Other) { }
public:
	ParticleSystem(ParticleProps& Data, Vector Gravity = EarthGravity);
	ParticleSystem(ParticleProps& Data, double EmitTimePeriod, int EmitCount, Vector Gravity = EarthGravity); //EmitTimePeriod is in seconds
	~ParticleSystem();

	Vector Gravity;

	void Emit();
	void OnUpdate(double ts);

	bool Active = true;

	void TimerTick(int ID) override
	{
		for (int i = 0; i < EmitAmount; i++)
			Emit();
	}
	
	void OnRender(HDC Dc, RECT Area) override;
};