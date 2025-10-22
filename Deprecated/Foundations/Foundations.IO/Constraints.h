#pragma once

namespace Core::IO
{
	/// <summary>
	/// Describes an object that has a "Next", "Last" and "Parent" structure. Any tree structure object that contains these properties is considered ElementSimilar.
	/// </summary>
	template<typename T>
	concept ElementSimilar = requires(T a)
	{
		a.Next&& a.Last&& a.Parent;
	};
}