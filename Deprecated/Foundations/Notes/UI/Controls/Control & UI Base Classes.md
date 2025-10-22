
## General Concept

In the UI library, for simplicity reasons, each class that is a control or window inherits from one of the base classes of the UI Contro library. Each of these base classes inherits from the Control class. On top of this, each base class has a specific feature that makes it distinguishable from another base type. They go as follows:

1. Control
	1. The base class of all of the UI system. 
	2. Contains handlers for each window message that is commonly used
	3. Contains the virtual functions to be used later on down the chain. 
	4. Contains all of the decorative colors & fonts used for the control, but allows later classes to override paint behavior. 
2. ChildlessControl
	1. A control that lacks the ability to add a child to it. 
	2. Typically used to describe simple controls. 
3. SingleChildControl
	1. A control that has exactly one control as a child. 
4. MultiChildControl
	1. A control that can have any number of children.

Please note that events from controls, such as WM_COMMAND are processed by the owner (parent) control or the window through an exposed method from Control. If this method is not overriden, the Control will pass the WM_COMMAND mesage to it's parent. 

### Control

The control class must have members for all of the colors and styles, the corner radius of each control, as well as handle all of the window messages from the system. This is done by having a WNDCLASS and a WNDPROC.  Please note that the backend of the control is NOT loaded untill the control is placed under a parent control and/or a window. This means that the programmer can fill in the control WITHOUT filling in a HINSTANCE or a HWND, but the Control class will handle it in the background. 

The general markup of the Control class goes as follows:

```C++
class Control
{
private:
	HWND _Base;
	Control* _Parent;

	static ATOM _ThisAtom;
	static void LoadBase(HINSTANCE ins);
	static LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);

	/*
		Commands go here....
	*/
	void Paint();
protected:
	/*
		Events that can be overriden go here...
	*/
	virtual int HandleCommand(HMENU ControlID, LPARAM CommandPointer) { return -1; } //To override, make sure that the overriden function returns 0. 
	virtual void PaintContent(HDC Context, RECT Bounds) = 0;
	virtual void PaintBase(HDC Context);
	
	virtual void HandleOtherMessage(UINT Message, WPARAM wp, LPARAM lp)
	{
		return DefWindowProc(_Base, Message, wp, lp);
	}
	
public:
	Brush* Background;
	Brush* BorderBrush;
	Thickness BorderThickness;
	Thickness CornerRadius;
	
	Brush* Foreground;
	String FontFamily;
	int FontSize;
	int FontWeight;
	int FontStyle;
	Alignment HorizontalContentAlignment;
	Alignment VerticalContentAlignment;
	
	Thickness Margin;
	Thickness Padding;
	
	virtual void RemoveChild(Control* const& Child) = 0;
	virtual void AddChild(Control* const& Child) = 0;
};
```

Please note that this is not a complete markup of the Control class, but it shows the general concepts. 

Also, note that there is a function in the class, 
```C++
virtual void RemoveChild(Control* const& Child) = 0;
virtual void AddChild(Control* const& Child) = 0;
```

This is intentional, as it would be a waste of memory and time to determine which base class each control is to remove/add a child, so this function is added in. Each inheriting base class of a Control override it's behavior. 

For more information about styles and visual appearance, see [[Styles & Visual States]].

### ChildlessControl

The ChildlessControl has, as the name implies, no children. It is standalone and the "leaf" of the UI tree. This class is used to describe simpler controls that do not contain anything other than their content and some functionality. An example of a ChildlessControl is a Button, because logically a button would not need to contain any children. 

A general markup (not complete) of ChildlessControl is as follows

```C++
class ChildlessControl : public Control
{
protected:
	void PaintContent(HDC Dc, RECT Bounds) override
	{
		if (Content)
			Content->Render(Dc, Bounds);
	}
public:
	UIContent* Content; //Represents an Object that can store and render the content of the control.
	void RemoveChild(Control* const& Child) override { }
	void AddChild(Control* const& Child) override { }  
};
```


### SingleChildControl

This control contains exactly ONE child, but may or may not contain a content. For this reason, the Content field is not included in SingleChildControl. If a SingleChildControl inheriting class does have a content, that class will specifiy its own content and override the base function, Control::PaintContent(HDC, RECT). However, for simplicity, the SingleChildControl class overrides Control::PaintContent(HDC, RECT), but it does no actions.

A simple markup of the SingleChildControl goes as follows:

```C++
class SingleChildControl : public Control
{
private:
	Control* _Child;
protected:
	virtual void PaintContent(HDC Dc, RECT Bounds) override { }
	
public:
	Control* const& Child = _Child;
	void AddChild(Control* const& Child) override;
	void RemoveChild(Control* const& Child) override;
};
```


### MultiChildControl

This Control is the parent of many child controls. It usually has its own way of arranging the controls, therefore this behvaior is managed by the inheriting class. Similar to SingleChildControl, this child may or may not contain a content. Therefore, if the inheriting class needs a content feild, it will specify it and override Control::PaintContent(HDC, RECT). 

The general markup of MultiChildControl goes as follows:

```C++
class MultiChildControl : public Control
{
private:
	ControlList* _Children;
	
protected:
	virtual void PaintContent(HDC Dc, RECT Bounds) override { }
	
public:
	ControlList* const& Child = _Child;
	void AddChild(Control* const& Child) override;
	void RemoveChild(Control* const& Child) override;
	void ContainsChild(Control* const& Control) override;
}
```