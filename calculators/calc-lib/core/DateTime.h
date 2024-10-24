#pragma once

#include <chrono>

#include "Types\BasicObject.h"

namespace Core
{
	class CORE_API String;

	enum class DateStringFormat
	{
		LongDate = 0,
		ShortDate = 1,
		Time = 2,
		Date = 3,
		Duration = 4
	};

	class CORE_API DateTime : public BasicObject
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
		DateTime(std::chrono::duration<double> Tm);

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
		
		String ToString() const override;
		String ToUIString() const override;
		String TypeName() const override;

		BasicObject* DefaultValue() const override;
		BasicObject* Clone() const override;
		bool OverrideFrom(BasicObject* Obj) override;
		void FillFromString(const String& Obj) override;

		bool HasModifyer() const override;
		BasicObjectModifyer* ConstructModifyer() const override;

		/**
		* Converts this DateTime to the following format: MM/DD/YYYY HH:MM:SS [AM/PM]
		*/
		String ToString(DateStringFormat Format) const;
		String ToBackString() const;
		static DateTime FromBackString(const String& Value);

		friend CORE_API std::wostream& operator<<(std::wostream& out, const DateTime& Obj);

		friend bool operator==(const DateTime& One, const DateTime& Two);
		friend bool operator!=(const DateTime& One, const DateTime& Two);
		bool operator<(const DateTime& Two) const;
		bool operator>(const DateTime& Two) const;

		friend DateTime operator-(const DateTime& One, const DateTime& Two);
		friend DateTime operator+(const DateTime& One, const DateTime& Two);
		DateTime& operator+=(const DateTime& Other);
		DateTime& operator-=(const DateTime& Other);
	};
}