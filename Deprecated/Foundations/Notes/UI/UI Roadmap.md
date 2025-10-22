The UI system consists of a few parts. 

1. Controls & UI Base Classes
2. Common Controls
3. Custom Controls System
4. Window Management System

Technically, the Editor system is a different system of the UI, even though it is a UI element. The reason why it is not included is because the UI system does have the systems and the access to the IO like Editors need. 

## Controls & UI Base Classes

This is the foundation of the UI system. It contains the Control class, and the various base classes that other controls use to organize systems. These base classes are responsible for separating the coded version of the UI from Win32, as the Control class and base classes are tasked with handling ALL of the Win32 messages, commands, and inputs. The only exception is that Controls contains an virtual void,
```C++
	protected:
	virtual void HandleOtherMessage(UINT Message, WPARAM wp, LPARAM lp) 
	{ 
		return DefWindowProc(_Base, Message, wp, lp); 
	}
```
This function can be overriden by a derived class, and therefore all controls, baseline and custom, have the ability to handle all Window Messages. 

For more, see [[Control & UI Base Classes]].

## Common Controls

The UI library also has a long list of common controls. These controls are things that will be used in almost all windows, as they are the most simple and most widespread. They can be categorized as such:

1. Command: Button, ToggleButton, RadioButton, CheckBox, RepeatButton, SplitButton
2. Informational: Label/TextBlock, ImageViewer, ProgressBar, CalendarGrid (Readonly Mode)
3. Input: TextBox, PasswordBox, NumericBox, CalendarGrid (Non-Readonly Mode), ComboBox, Slider, ToggleSwitch, DatePicker, TimePicker, DraggableItem
4. Layout: NavigationPane, Ribbon, Grid, StackPanel, ReferenceGrid, AbsoluteGrid, ScrollViewer, GridView, ListView, ListBox, TreeView, DataGrid, Border, Expander, SplitView

Each of these four groupings has its own folder in the Controls folder, and each control exists in its own .h and .cpp file within that folder. However, all controls are part of the Foundations::UI::Controls namespace. 

For more on controls, see [[Common Controls]].

## Custom Controls

This is a simpler part of the system, however there are some rules with creating custom controls. For this instance, please review the [[How to Create a Custom Control]] page for more information.

## Window Management System

This part of the UI library is in charge of handling windows, or the hosts of all controls. Windows are, in themselves, a type of control, and they have the socket to accept one child of the window. This is to help with bringing the whole system together. However, this does not mean that a child control cannot reference the window as a child. 

For more specific information about the system, see [[Window Management System]].

