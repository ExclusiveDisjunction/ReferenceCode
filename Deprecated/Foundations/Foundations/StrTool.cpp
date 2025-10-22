#include "StrTool.h"

using namespace std;

std::string::const_iterator CORE_API StrContains(const string& obj, char Item)
{
	for (auto curr = obj.begin(), end = obj.end(); curr != end; curr++)
	{
		if ((*curr) == Item)
			return curr;
	}

	return obj.end();
}
std::string::const_iterator CORE_API StrContains(const string& obj, const std::string& Other)
{
	if (Other == "" || obj.length() < Other.length())
		return obj.end();

	bool Streak = false;
	string::const_iterator ocurr = Other.begin(), oend = Other.end();
	string::const_iterator Return = obj.end();

	for (auto curr = obj.begin(), end = obj.end(); curr != end; curr++)
	{
		if (Streak)
		{
			if (ocurr != oend && *curr == *ocurr ) //Ensures that we are not eos on Other, and then compares. If they are equal, continue the streak by incrementing.
			{
				ocurr++;
				continue;
			}
			else if (ocurr == oend)
			{
				break; //The search is over, the entire Other was found in obj.
			}
			else //Streak broken, reset
			{
				Return = obj.end();
				Streak = false;
				ocurr = Other.begin();
			}
		}
		else if (*curr == *ocurr) //First char found
		{
			size_t numPassed = curr - obj.begin(); //Number of iterations.
			size_t numLeft = obj.size() - numPassed;

			if (numLeft < Other.size()) //If the number of chars in the obj string left is less than the number of strings in other, then it will never contain the other string.
				return obj.end();

			Streak = true;
			ocurr++;
			Return = curr;
			continue;
		}
	}

	return Return;
}
bool CORE_API StrBeginsWith(const std::string& obj, char val)
{
	return obj.length() && obj[0] == val;
}
bool CORE_API StrBeginsWith(const std::string& obj, const std::string& strseq)
{
	if (obj.length() == strseq.length())
		return obj == strseq;

	auto curr = obj.begin(), end = obj.end();
	auto ocurr = strseq.begin(), oend = strseq.end();

	for (; curr != end; curr++, ocurr++)
	{
		if (ocurr == oend) //Since we already checked for zero length string, this means that we are eos for strseq and therefore the whole string was found.
			return true;

		if (*curr == *ocurr) //Continue streak
			continue; 
		else  //Character variation found.
			return false;
	}

	return true;
}

std::string CORE_API StrToLower(const std::string& Obj)
{
	string Return(Obj.length(), ' ');

	string::const_iterator curr = Obj.begin(), end = Obj.end();
	string::iterator rcurr = Return.begin(), rend = Return.end();

	for (; curr != end && rcurr != rend; curr++, rcurr++)
		*rcurr = tolower(*curr);

	return Return;
}
std::string CORE_API StrToUpper(const std::string& Obj)
{
	string Return(Obj.length(), ' ');

	string::const_iterator curr = Obj.begin(), end = Obj.end();
	string::iterator rcurr = Return.begin(), rend = Return.end();

	for (; curr != end && rcurr != rend; curr++, rcurr++)
		*rcurr = toupper(*curr);

	return Return;
}
std::string CORE_API StrToCamelCase(const std::string& Obj)
{
	string Return(Obj.length(), ' ');

	string::const_iterator curr = Obj.begin(), end = Obj.end();
	string::iterator rcurr = Return.begin(), rend = Return.end();

	bool WordStarted = false;
	for (; curr != end && rcurr != rend; curr++, rcurr++)
	{
		char currch = *curr;
		if (isspace(currch) || iscntrl(currch) || isdigit(currch) || ispunct(currch))
		{
			WordStarted = false;
			*rcurr = currch;
			continue;
		}
		else if (isalpha(currch))
		{
			if (WordStarted)
				*rcurr = tolower(currch);
			else
			{
				*rcurr = toupper(currch);
				WordStarted = true;
			}
		}
	}

	return Return;
}

vector<string> CORE_API StrSplit(const std::string& str, char at)
{
	std::vector<string> strings;
	int start = 0, end = 0;
	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] == at || i == str.size())
		{
			end = i;
			string temp;
			temp.append(str, start, end - start);
			strings.push_back(temp);
			start = end + 1;
		}
	}

	return strings;
}
std::string CORE_API StrCombine(const std::vector<std::string>& Objs, char with)
{
	std::string Return;

	size_t i = 0, size = Objs.size() - 1;
	for (vector<string>::const_iterator curr = Objs.begin(), end = Objs.end(); curr != end; curr++, i++)
		Return += *curr + (i != size ? string(1, with) : "");

	return Return;
}
std::string CORE_API StrReplace(const std::string& obj, char replace, char with)
{
	string Return = obj;
	for (string::iterator curr = Return.begin(), end = Return.end(); curr != end; curr++)
	{
		if (*curr == replace)
			*curr = with;
	}

	return Return;
}

std::string CORE_API StrTrimFront(const std::string& obj)
{
	size_t first = 0;
	for (size_t i = 0; i < obj.size(); i++)
	{
		if (!isspace(obj[i]))
		{
			first = i;
			break;
		}
	}

	//Now that the index of the first non string is found, return that.
	return obj.substr(first);
}
std::string CORE_API StrTrimEnd(const std::string& obj)
{
	size_t first = 0;
	for (auto curr = obj.rbegin(), end = obj.rend(); curr != end; curr++)
	{
		if (!isspace(*curr))
			first = obj.rbegin() - curr;
	}

	return obj.substr(0, first + 1);
}
std::string CORE_API StrTrim(const std::string& obj)
{
	size_t first = 0, last = 0;

	//First, find the first letter thats not whitespace.
	for (size_t i = 0; i < obj.size(); i++)
	{
		if (!isspace(obj[i]))
		{
			first = i;
			break;
		}
	} 

	//Next, find the last letter thats not whitespace.
	for (auto curr = obj.rbegin(), end = obj.rend(); curr != end; curr++)
	{
		if (!isspace(*curr))
			first = obj.rbegin() - curr;
	}

	return obj.substr(first, last - first + 1);

	//Now that the index of the first non string is found, return that.
	return obj.substr(first);
}
int CORE_API StrTabIndex(const std::string& Str)
{
	int Return = 0;
	for (auto begin = Str.begin(), end = Str.end(); begin != end; begin++)
	{
		if ((*begin) == '\t')
			Return++;
		else
			return Return;
	}

	return Return;
}