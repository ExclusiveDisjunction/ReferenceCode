#ifndef DATETIME_H
#define DATETIME_H

#include <chrono>
#include <string>

#include "Common.h"

namespace Core
{
	enum class DateStringFormat
	{
		LongDate = 0,
		ShortDate = 1,
		Time = 2,
		Date = 3,
		Duration = 4
	};

	class CORE_API DateTime
	{
	private:
		std::chrono::duration<double> _Dur;

	public:
		static DateTime Today();
		static DateTime Now();

		DateTime();
		DateTime(int Month, int Day, int Year);
		DateTime(int Hour, int Minute, int Second, int Millisecond);
		DateTime(int Month, int Day, int Year, int Hour, int Minute, int Second, int Millisecond);
		DateTime(const std::string& BackString);
		DateTime(const std::chrono::duration<double>& Tm);

		bool HasValue = true, HasDay = true, HasTime = true;

		int Year() const;
		int Year(std::chrono::duration<double>& Remain) const;
		int Month() const;
		int Month(std::chrono::duration<double>& Remain) const;
		int Day() const;
		int Day(std::chrono::duration<double>& Remain) const;
		int Hour() const;
		int Hour(std::chrono::duration<double>& Remain) const;
		int Minute() const;
		int Minute(std::chrono::duration<double>& Remain) const;
		int Second() const;
		int Second(std::chrono::duration<double>& Remain) const;
		int Millisecond() const;
		int Millisecond(std::chrono::duration<double>& Remain) const;

		DateTime DayParts() const;
		DateTime TimeParts() const;

		std::string ToString(DateStringFormat Format) const;
		std::string ToBackString() const; //A BackString is what gets outputted to a file.
		static DateTime FromBackString(const std::string& Value); //Reads from a file using Backstring format.

		friend CORE_API std::ostream& operator<<(std::ostream& out, const DateTime& Obj); //USES PRESENTABLE STRING USING DateStringFormat::ShortDate
		friend CORE_API std::ofstream& operator<<(std::ofstream& out, const DateTime& Obj); //USES BACKSTRING
		friend CORE_API std::istream& operator>>(std::istream& in, DateTime& Obj); //USES BACKSTRING

		bool operator==(const DateTime& Obj) const;
		bool operator!=(const DateTime& Obj) const;
		bool operator<(const DateTime& Obj) const;
		bool operator>(const DateTime& Obj) const;

		DateTime operator-(const DateTime& Obj) const;
		DateTime operator+(const DateTime& Obj) const;
		DateTime& operator+=(const DateTime& Obj);
		DateTime& operator-=(const DateTime& Obj);
	};
}

#endif