#pragma once

#include <string>
#include <vector>

#include "Common.h"

std::string::const_iterator CORE_API StrContains(const std::string& obj, char Item);
std::string::const_iterator CORE_API StrContains(const std::string& obj, const std::string& Other);
bool CORE_API StrBeginsWith(const std::string& obj, char val);
bool CORE_API StrBeginsWith(const std::string& obj, const std::string& strseq);

std::string CORE_API StrToLower(const std::string& Obj);
std::string CORE_API StrToUpper(const std::string& Obj);
std::string CORE_API StrToCamelCase(const std::string& Obj);

std::vector<std::string> CORE_API StrSplit(const std::string& Obj, char at);
std::string CORE_API StrCombine(const std::vector<std::string>& Objs, char with);
std::string CORE_API StrReplace(const std::string& obj, char replace, char with);

std::string CORE_API StrTrimFront(const std::string& obj);
std::string CORE_API StrTrimEnd(const std::string& obj);
std::string CORE_API StrTrim(const std::string& obj);
int CORE_API StrTabIndex(const std::string& obj);