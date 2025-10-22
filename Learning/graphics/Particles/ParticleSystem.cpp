#include "Particle.h"

#include <Windows.h>
#include <Gdiplus.h>
#include <random>

#include "BasicWindow.h"

ParticleSystem::ParticleSystem(ParticleProps& Data, Vector Gravity)
{
	this->Data = Data;
	this->Gravity = Gravity;
	Particles.Resize(2000);

	Timekeeper::Subscribe(this);
}
ParticleSystem::ParticleSystem(ParticleProps& Data, double EmitTimePeriod, int EmitCount, Vector Gravity) : ParticleSystem(Data, Gravity)
{
	ThisThread = std::thread(ParticleSystem::TickEmit, this);
	this->EmitAmount = EmitCount;
	this->EmitTimePeriod = EmitTimePeriod;

	//TimerWindow = CreateTimerWindow(this, EmitTimePeriod);
}
ParticleSystem::~ParticleSystem()
{
	//DestroyWindow(TimerWindow);
	Timekeeper::Desubscribe(this);

	if (ThisThread.joinable())
		ThisThread.join();
}

void ParticleSystem::TickEmit(ParticleSystem* System)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(System->EmitTimePeriod * 1000)));
	for (int i = 0; i < System->EmitAmount; i++)
		System->Emit();
}

void ParticleSystem::Emit()
{
	Particle& particle = Particles[PoolIndex];
	particle.Active = true;
	particle.Pos = Data.Position;
	
	particle.Velocity = Data.Velocity;

	std::uniform_real_distribution<float> dist(Data.VelocityAngle.AngleBegin, Data.VelocityAngle.AngleEnd);
	std::uniform_real_distribution<float> velocdist(-0.5f, 0.5f);

	float AngleBetween = dist(RandomEngine);

	Vector& Start = particle.Velocity;
	float Magnitude = Start.Magnitude();
	Start.X = Magnitude * cos(AngleBetween);
	Start.Y = Magnitude * sin(AngleBetween);

	Start.X += Data.VelocityVariation.X * velocdist(RandomEngine);
	Start.Y += Data.VelocityVariation.Y * velocdist(RandomEngine);

	particle.ColorBegin = Data.ColorBegin;
	particle.ColorEnd = Data.ColorEnd;
	particle.Color = particle.ColorBegin;

	particle.LifeTime = Data.LifeTime;
	particle.LifeRemaining = Data.LifeTime;
	particle.SizeBegin = Data.SizeBegin;
	particle.SizeEnd = Data.SizeEnd;
	particle.Size = particle.SizeBegin;

	PoolIndex = PoolIndex-- & Particles.Size();
}
void ParticleSystem::OnUpdate(double ts)
{
	for (uint i = 0; i < Particles.Size(); i++)
	{
		Particle& particle = Particles[i];

		if (!particle.Active)
			continue;

		if (particle.LifeRemaining <= 0.0f)
		{
			particle.Active = false;
			continue;
		}

		particle.LifeRemaining -= ts;

		float Life = particle.LifeRemaining / particle.LifeTime;

		BYTE A1 = particle.ColorEnd.A(), A2 = particle.ColorBegin.A();
		BYTE R1 = particle.ColorEnd.R(), R2 = particle.ColorBegin.R();
		BYTE G1 = particle.ColorEnd.G(), G2 = particle.ColorBegin.G();
		BYTE B1 = particle.ColorEnd.B(), B2 = particle.ColorBegin.B();

		float LA = Lerp(A1, A2, Life), LR = Lerp(R1, R2, Life), LG = Lerp(G1, G2, Life), LB = Lerp(B1, B2, Life);

		if (particle.VelocityInitial != Vector(0, 0))
			particle.Velocity = particle.VelocityInitial + Gravity * ts;
		else
			particle.VelocityInitial = particle.Velocity;

		particle.Pos += particle.Velocity * ts;
		particle.Size = Lerp(particle.SizeEnd, particle.SizeBegin, Life);
		particle.Color = AaColor(LA, LR, LG, LB);
	}
}

void ParticleSystem::OnRender(HDC Dc, RECT Area)
{
	for (uint i = 0; i < Particles.Size(); i++)
	{
		Particle& Particle = Particles[i];
		if (!Particle.Active)
			continue;

		Vector Pos = TranslateCoord(Particle.Pos, Area);

		int Size = Particle.Size;
		RECT Render = { Pos.X - Size, Pos.Y - Size, Pos.X + Size, Pos.Y + Size };

		HBRUSH ThisColor = CreateSolidBrush(Particle.Color);
		FillRect(Dc, &Render, ThisColor);

		DeleteObject(ThisColor);
	}
}