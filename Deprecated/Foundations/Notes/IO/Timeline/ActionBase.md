
# General Concept

The ActionBase is an abstract base class used by the UndoHistory object, and allows for an abstract way to represent Actions. This is because there are multiple types of actions, and this allows for a more dynamic system that can be upgraded over time. This also lets the developer that is using Foundations be able to put in their own actions into the Timeline and Action system. 

# Class Layout

```C++
class ActionBase
{
private:
	Timeline* _Owner;
	ActionBase* _Next = nullptr, * _Previous = nullptr;
	ActionBase* _Top = nullptr;
	
	void DeleteChain();
protected:

public:
	ActionBase(Timeline* Owner);
	ActionBase(Timeline* Owner, ActionBase* Next, ActionBase* Previous);
	ActionBase(const ActionBase& Obj) = delete;
	ActionBase(ActionBase&& Obj) = delete;
	
	ActionBase& operator=(const ActionBase& Obj) = delete;
	ActionBase& operator=(ActionBase&& Obj) = delete;
	
	Timeline* const& Owner = _Owner;
	ActionBase* const& Next = _Next;
	ActionBase* const& Previous = _Previous;
	ActionBase* const& Top = _Top;
	
	ActionID ID;
	
	
	virtual bool ApplyChanges() = 0; 
}
```
