
## General Use & Concept

The Footer Host is a simplification of the Footer, a common feature of Main and Side windows. The footer displays messages from the application that do not need to be placed in a message box, due to low importance, but are prevelent to the user. 

## Class Markup

```C++
class FooterHost
{
private:
	StackPanel* FooterParent = nullptr;
	Label* FooterText = nullptr;
	std::queue<String> Msgs;
	std::queue<int> MsgDur;
protected:
	void LoadFooterCtrls();
	
	AaColor FooterBackground;
	AaColor FooterForeground;
public:
	void SetFooterText(String Text, int Duration = 3000);
	void ResetFooter();
};
```