#pragma once

//This is a joint header for quick access to the code for custom controls.

#include "Control.h"

#include "Controls\ScrollViewer.h"
#include "Controls\ProgressBar.h"
#include "Controls\CheckableButton.h"
#include "Controls\Button.h"
#include "Controls\TextBox.h"
#include "Controls\Label.h"
#include "Controls\Grid.h"
#include "Controls\ComboBox.h"
#include "Controls\ToggleButton.h"
#include "Controls\ImageViewer.h"
#include "Controls\CalendarGrid.h"

namespace Core::UI::Controls
{
	using ControlList = Vector<Control*>;
}