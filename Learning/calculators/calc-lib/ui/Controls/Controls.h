#pragma once

//This is a joint header for quick access to the code for custom controls.

#include "ControlBase.h"

#include "ScrollViewer.h"
#include "ProgressBar.h"
#include "CheckableButton.h"
#include "Button.h"
#include "TextBox.h"
#include "Label.h"
#include "Grid.h"
#include "ComboBox.h"
#include "ToggleButton.h"
#include "ImageViewer.h"
#include "CalendarGrid.h"

namespace Mason::UI::Controls
{
	using ControlList = Core::Vector<ControlBase*>;
}