#include "FlowFieldGen.h"

#include "FlowRenderable.h"

FlowFieldGen::FlowFieldGen(Renderer* Render)
{
	if (!Render)
		throw std::exception();

	this->Render = Render;
	Render->RegisterOnSizeEvent(this, FlowFieldGen::OnRendererResize);
	ConstructRenderables();
}
FlowFieldGen::~FlowFieldGen()
{
	Renderables.clear();
	if (Render)
		Render->DeRegisterOnSizeEvent(this);
}

float FlowFieldGen::DetermineAngle(const Vector& Pos)
{
	int XLo = (int)floor(Pos.x), XHi = (int)ceil(Pos.x);
	int YLo = (int)floor(Pos.y), YHi = (int)ceil(Pos.y);

	XLo = XLo >= XMax ? XMax - 1 : XLo;
	XHi = XHi >= XMax ? XMax - 1 : XHi;
	YLo = YLo >= YMax ? YMax - 1 : YLo;
	YHi = YHi >= YMax ? YMax - 1 : YHi;

	XLo = XLo < 0 ? 0 : XLo;
	XHi = XHi < 0 ? 0 : XHi;
	YLo = YLo < 0 ? 0 : YLo;
	YHi = YHi < 0 ? 0 : YHi;

	//The coordinates form a grid:
	/*
	*  (XLo, YLo) [a0] --------- (XHi, YLo) [a1]
	*  |                                  |
	*  |           * (x,y)                |
	*  |                                  |
	*  |                                  |
	*  |                                  |
	*  (XLo, YHi) [a2] --------- (XHi, YHi) [a3]
	* 
	* From this diagram, determine the four angle values (a0, a1, a2, a3), and then linear interpolate the values
	*/

	if ((XLo == (int)Pos.x || XLo == XMax - 1 || XLo == 0) || (YLo == (int)Pos.y || YLo == YMax - 1 || YLo == 0)) //Whole number location
	{
		float Return = AngleMat[XLo][YLo];

		return Return;
	}
	else
	{
		float a0 = AngleMat[XLo][YLo],
			a1 = AngleMat[XLo][YHi],
			a2 = AngleMat[XHi][YLo],
			a3 = AngleMat[XHi][XHi];

		//First interpolate x, then y, and then interpolate the results.
		float xw = (float)XLo - Pos.x;
		float yw = (float)YLo - Pos.y;

		xw = xw < 0 ? 0 : (xw > 1) ? 1 : xw; //Ensures it is in [0,1]
		yw = yw < 0 ? 0 : (yw > 1) ? 1 : yw;

		float dxy = sqrt(pow<float, float, 0>(xw, 2.0f) + pow<float, float, 0>(yw, 2.0f));

		float i01 = lerp(a0, a1, xw);
		float i23 = lerp(a2, a3, xw);

		float i02 = lerp(a0, a2, yw);
		float i13 = lerp(a1, a3, yw);

		float ix = lerp(i01, i23, yw);
		float iy = lerp(i02, i13, xw);

		return lerp(ix, iy, dxy);
	}
}
void FlowFieldGen::ClearAll()
{
	AngleMat.Resize(0, 0);
	Particles.clear();
	
	XMax = YMax = WndX = WndY = 0;
	ScaleX = ScaleY = 0.0;

	ResetRenderables();
	RequestRedraw();
}
void FlowFieldGen::ConstructRenderables()
{
	FlowRenderable* Temp = nullptr;
	Temp = new GridRenderable(this, this->Render);
	Temp->IsEnabled(false);
	Renderables.push_back(Temp);

	Temp = new PointsDispRenderable(this, this->Render);
	Temp->IsEnabled(false);
	Renderables.push_back(Temp);

	Temp = new FieldRenderable(this, this->Render);
	Temp->IsEnabled(false);
	Renderables.push_back(Temp);
}
void FlowFieldGen::ResetRenderables()
{
	for (FlowRenderable*& Obj : Renderables)
		Obj->IsEnabled(false);
}

void FlowFieldGen::OnRendererResize(void* Data, D2D1_SIZE_U NewSize)
{
	if (Data == nullptr)
		return;

	FlowFieldGen* Conv = (FlowFieldGen*)Data;

	//First set the wndsize
	Conv->WndX = NewSize.width;
	Conv->WndY = NewSize.height;

	//Then the ratio (scale)
	if (Conv->XMax && Conv->YMax)
	{
		Conv->ScaleX = (float)Conv->WndX / Conv->XMax;
		Conv->ScaleY = (float)Conv->WndY / Conv->YMax;

		Conv->Render->SetTransform(D2D1::Matrix3x2F::Scale(Conv->ScaleX, Conv->ScaleY));
	}
}

void FlowFieldGen::FillParticles(int ParticleCount)
{
	Particles.clear();
	Particles.resize(ParticleCount);

	for (int i = 0; i < ParticleCount; i++)
	{
		D2D1_COLOR_F Color = D2D1::ColorF((UINT32)lerp(D2D1::ColorF::Orange, D2D1::ColorF::Red, (float)i / ParticleCount));
		Particles[i].RandomizeValues((float)XMax, (float)YMax, Color);
	}

	//Reset Calculations
	{
		Calculations.clear();
		auto Result = std::find_if(Renderables.begin(), Renderables.end(), [](FlowRenderable*& Obj) -> bool
			{
				return Obj->GetName() == "FieldRenderable";
			});

		if (Result != Renderables.end())
			(*Result)->IsEnabled(false);
	}
}
void FlowFieldGen::DisplayParticles(bool Visible)
{
	auto Result = std::find_if(Renderables.begin(), Renderables.end(), [](FlowRenderable*& Obj) -> bool
		{
			return Obj->GetName() == "PointsDispRenderable";
		});

	if (Result != Renderables.end())
		(*Result)->IsEnabled(Visible);

	Render->RequestRedraw();
}
void FlowFieldGen::AnimateParticles()
{
	Calculations.clear();
	Calculations.resize(Particles.size());

	D2D1_SIZE_F Size = Render->GetSize(), WndSize = GetWndSize();
	float greaterAxis = max(Size.width, Size.height);
	bool xIsGreater = Size.width == greaterAxis;

	float tInc = Size.width / WndSize.width;

	auto partCurr = Particles.begin(), partEnd = Particles.end();
	for (auto curr = Calculations.begin(), end = Calculations.end(); curr != end && partCurr != partEnd; curr++, partCurr++)
	{
		std::vector<D2D1_POINT_2F>& pts = *curr;
		Particle& part = *partCurr;

		Vector& CurrentPosition = part.getCurrentPosition();

		//lets first assume that the pts will need the distance from the current particles and the outer bounds, measured from the greater axis.
		{
			float ValToTest = xIsGreater ? CurrentPosition.x : CurrentPosition.y;
			size_t Largest = max((size_t)ValToTest, (size_t)(greaterAxis - ValToTest));
			pts.reserve(Largest);
		}

		float t = 0.0;
		int step = 0;
		const int MAX_STEP = 2000;
		do
		{
			//Calculate here
			float Angle = DetermineAngle(CurrentPosition);
			CurrentPosition.x += cos(Angle);
			CurrentPosition.y += sin(Angle);

			pts.push_back(D2D1::Point2F((float)CurrentPosition.x, (float)CurrentPosition.y));
			step++;
		} while ((CurrentPosition.x >= 0.0 && CurrentPosition.x <= Size.width && CurrentPosition.y >= 0.0 && CurrentPosition.y <= Size.height) && step <= MAX_STEP);
	}

	//Display the renderable
	DisplayCalculations(true);
}
void FlowFieldGen::DisplayCalculations(bool Visible)
{
	auto Result = std::find_if(Renderables.begin(), Renderables.end(), [](FlowRenderable*& Obj) -> bool
		{
			return Obj->GetName() == "FieldRenderable";
		});

	if (Result != Renderables.end())
		(*Result)->IsEnabled(true);

	Render->RequestRedraw();
}

void FlowFieldGen::FillGrid(int XMax, int YMax)
{
	try
	{
		ClearAll();

		AngleMat.Resize(XMax, YMax);
		for (int j = 0; j < XMax; j++)
		{
			for (int i = 0; i < YMax; i++)
			{
				float x = static_cast<float>(pow(i, 2) - pow(j, 2) - 4);
				float y = static_cast<float>(2.0f * i * j);

				AngleMat[i][j] = atan2(y, x);
			}
		}

		this->XMax = XMax;
		this->YMax = YMax;
	}
	catch (std::logic_error& e)
	{
		OutputDebugStringA((std::string("Error: ") + e.what()).c_str());
		return;
	}

	if (Render)
		OnRendererResize(this, D2D1::SizeU((UINT32)Render->GetSize().width, (UINT32)Render->GetSize().height)); //Sets the scale

}
void FlowFieldGen::DisplayGrid(bool Visible)
{
	auto Result = std::find_if(Renderables.begin(), Renderables.end(), [](FlowRenderable*& Obj) -> bool
		{
			return Obj->GetName() == "GridRenderable";
		});

	if (Result != Renderables.end())
		(*Result)->IsEnabled(Visible);

	Render->RequestRedraw();
}

Vector FlowFieldGen::MapToWindow(const Vector& ToTranslate)
{
	return Vector(ToTranslate.x * ScaleX, ToTranslate.y * ScaleY);
}
Vector FlowFieldGen::MapFromWindow(const Vector& ToTranslate)
{
	return Vector(ToTranslate.x / ScaleX, ToTranslate.y / ScaleY);
}