#include "DateTime.h" 

#include <sstream>
#include <fstream>

using namespace std;
using namespace std::chrono;

namespace Core
{
	DateTime::DateTime() : _Dur(0) { }
	DateTime::DateTime(int Month, int Day, int Year)
	{
		if (!(Month >= 1 && Month <= 12))
			Month = 1;
		if (!(Day >= 1 && Day <= 31))
			Day = 1;

		Month--;
		Day--;

		years Y(Year);
		months M(Month);
		days D(Day);

		_Dur = Y + M + D;
	}
	DateTime::DateTime(int Hour, int Minute, int Second, int Millisecond)
	{
		if (Hour > 23 || Hour < 0)
			Hour = 1;
		if (Minute > 59 || Minute < 0)
			Minute = 1;
		if (Second > 59 || Minute < 0)
			Second = 1;

		hours H(Hour);
		minutes Mi = minutes(Minute);
		seconds S(Second);
		milliseconds Ms(Millisecond);

		_Dur = H + Mi + S + Ms;
	}
	DateTime::DateTime(int Month, int Day, int Year, int Hour, int Minute, int Second, int Millisecond)
	{
		if (!(Month >= 1 && Month <= 12))
			Month = 1;
		if (!(Day >= 1 && Day <= 31))
			Day = 1;
		if (Hour > 23 || Hour < 0)
			Hour = 1;
		if (Minute > 59 || Minute < 0)
			Minute = 1;
		if (Second > 59 || Minute < 0)
			Second = 1;

		Month--;
		Day--;
		Year -= 1970;

		years Y(Year);
		months M(Month);
		days D(Day);
		hours H(Hour);
		minutes Mi = minutes(Minute);
		seconds S(Second);
		milliseconds Ms(Millisecond);

		_Dur = Y + M + D + H + Mi + S + Ms;
	}
	DateTime::DateTime(const duration<double>& Dur)
	{
		_Dur = Dur;
	}

	DateTime DateTime::Today()
	{
		DateTime Return;

		auto Point = system_clock::now();
		auto Zone = current_zone();
		auto Zoned = zoned_time(current_zone(), Point);

		Return._Dur = Zoned.get_local_time().time_since_epoch();
		Return = Return.DayParts();

		return Return;
	}
	DateTime DateTime::Now()
	{
		DateTime Return;

		auto Point = system_clock::now();
		auto Zoned = zoned_time(current_zone(), Point);

		Return._Dur = Zoned.get_local_time().time_since_epoch();

		return Return;
	}

	int DateTime::Year() const
	{
		duration<double> This = this->_Dur;
		return Year(This) + 1970;
	}
	int DateTime::Year(duration<double>& Remain) const
	{
		years Return = duration_cast<years>(Remain);
		Remain -= Return;

		return Return.count();
	}
	int DateTime::Month() const
	{
		duration<double> This = this->_Dur;
		Year(This);
		return Month(This) + 1;
	}
	int DateTime::Month(std::chrono::duration<double>& Remain) const
	{
		months Return = duration_cast<months>(Remain);
		Remain -= Return;

		return Return.count();
	}
	int DateTime::Day() const
	{
		duration<double> This = this->_Dur;
		Month(This);
		return Day(This) + 1;
	}
	int DateTime::Day(std::chrono::duration<double>& Remain) const
	{
		days Return = duration_cast<days>(Remain);
		Remain -= Return;

		return Return.count();
	}
	int DateTime::Hour() const
	{
		duration<double> This = this->_Dur;
		Day(This);
		return Hour(This);
	}
	int DateTime::Hour(std::chrono::duration<double>& Remain) const
	{
		hours Return = duration_cast<hours>(Remain);
		Remain -= Return;

		return Return.count();
	}
	int DateTime::Minute() const
	{
		duration<double> This = this->_Dur;
		Hour(This);
		return Minute(This);
	}
	int DateTime::Minute(std::chrono::duration<double>& Remain) const
	{
		minutes Return = duration_cast<minutes>(Remain);
		Remain -= Return;

		return Return.count();
	}
	int DateTime::Second() const
	{
		duration<double> This = this->_Dur;
		Minute(This);
		return Second(This);
	}
	int DateTime::Second(std::chrono::duration<double>& Remain) const
	{
		seconds Return = duration_cast<seconds>(Remain);
		Remain -= Return;

		return static_cast<int>(Return.count());
	}
	int DateTime::Millisecond() const
	{
		duration<double> This = this->_Dur;
		Second(This);
		return Millisecond(This);
	}
	int DateTime::Millisecond(duration<double>& Remain) const
	{
		milliseconds Return = duration_cast<milliseconds>(Remain);
		Remain -= Return;

		return static_cast<int>(Return.count());
	}

	DateTime DateTime::DayParts() const
	{
		DateTime New;

		duration<double>& Dur = New._Dur;
		Dur = _Dur;
		years Y = duration_cast<years>(Dur);
		Dur -= Y;
		months M = duration_cast<months>(Dur);
		Dur -= M;
		days D = duration_cast<days>(Dur);
		Dur -= D;

		Dur = Y + M + D; //Once Y:M:D is extracted, Dur is set to the sum of these three parts, excluding H:M:S:MI
		return New;
	}
	DateTime DateTime::TimeParts() const
	{
		DateTime New;

		duration<double>& Dur = New._Dur;
		Dur = _Dur;
		years Y = duration_cast<years>(Dur);
		Dur -= Y;
		months M = duration_cast<months>(Dur);
		Dur -= M;
		days D = duration_cast<days>(Dur);
		Dur -= D;

		return New; //All that is left is simply H:M:S:MS. No further action must take place.
	}

	std::string DateTime::ToString(DateStringFormat Format) const
	{
		int Year = this->Year(), Month = this->Month(), Day = this->Day(), Hour = this->Hour(), Minute = this->Minute(), Second = this->Second(), Millisecond = this->Millisecond();

		stringstream Return;

		if (*this == DateTime())
			return "No Data";

		switch (Format)
		{
		case DateStringFormat::Date:
		{
			if (Day != 0 && Year != 0 && Month != 0)
				Return << Month << "/" << Day << "/" << Year;
			else
				return "No Data";
			break;
		}
		case DateStringFormat::Duration:
		{
			if (Year == 0 && Month == 0 && Day == 0 && Hour == 0 && Minute == 0 && Second == 0)
				return "No Data";

			std::string Y = Year == 0 ? "" : to_string(Year) + (Year == 1 ? "Year" : "Years"),
						M = Month == 0 ? "" : to_string(Month) + (Month == 1 ? "Month" : "Months"),
						D = Day == 0 ? "" : to_string(Day) + (Day == 1 ? "Day" : "Days"),
						H = Hour == 0 ? "" : to_string(Hour) + (Hour == 1 ? "Hour" : "Hours"),
						Mi = Minute == 0 ? "" : to_string(Minute) + (Minute == 1 ? "Minute" : "Minutes"),
						S = Second == 0 ? "" : to_string(Second) + (Second == 1 ? "Second" : "Seconds");

			Return << Y << (Y.length() == 0 ? "" : ", ") <<
				M << (!M.length() ? "" : ", ") <<
				D << (!D.length() ? "" : ", ") <<
				H << (!H.length() ? "" : ", ") <<
				Mi << (!Mi.length() ? "" : ", ") <<
				S << (!S.length() ? "" : ".");
			break;
		}
		case DateStringFormat::LongDate:
		{
			if (Month != 0 && Year != 0 && Day != 0)
			{
				switch (Month)
				{
				case 1:
					Return << "January";
					break;
				case 2:
					Return << "February";
					break;
				case 3:
					Return << "March";
					break;
				case 4:
					Return << "April";
					break;
				case 5:
					Return << "May";
					break;
				case 6:
					Return << "June";
					break;
				case 7:
					Return << "July";
					break;
				case 8:
					Return << "August";
					break;
				case 9:
					Return << "September";
					break;
				case 10:
					Return << "October";
					break;
				case 11:
					Return << "November";
					break;
				case 12:
					Return << "December";
					break;
				}

				Return << ' ' << Day;
				switch (Day)
				{
				case 1:
				case 21:
				case 31:
					Return << "st";
					break;

				case 2:
				case 22:
					Return << "nd";
					break;

				case 3:
				case 23:
					Return << "rd";
					break;

				default:
					Return << "th";
					break;
				}

				Return << ", " << Year;
			}

			if (Hour != 0 && Minute != 0)
			{
				if (Year == 0 || Month == 0 || Day == 0)
					Return << " at ";

				Return << (Hour > 12 ? Hour - 12 : Hour) << ':' << Minute << ' ' << (Hour < 12 ? "AM" : "PM");
			}
			break;
		}
		case DateStringFormat::ShortDate:
		{
			if (Day == 0 && Year == 0 && Month == 0 && Hour == 0 && Minute == 0)
				return "No Data";

			if (Day != 0 && Year != 0 && Month != 0)
				Return << Month << '/' << Day << '/' << Year;

			if (Hour != 0 && Minute != 0)
			{
				if (Year == 0 || Month == 0 || Day == 0)
					Return << " at ";

				Return << (Hour > 12 ? Hour - 12 : Hour) << ':' << Minute << ' ' << (Hour < 12 ? "AM" : "PM");
			}
			break;
		}
		case DateStringFormat::Time:
		{
			if (Hour != 0 && Minute != 0)
				Return << (Hour > 12 ? Hour - 12 : Hour) << ':' << Minute << ' ' << (Hour < 12 ? "AM" : "PM");
			else
				return "No Time";
			break;
		}
		}

		return Return.str();
	}
	std::string DateTime::ToBackString() const
	{
		return to_string(_Dur.count());
	}
	DateTime DateTime::FromBackString(const std::string& Value)
	{
		stringstream Stream(Value);
		double NumVal = 0.0;
		Stream >> NumVal;

		return DateTime(duration<double>(NumVal));
	}

	bool DateTime::operator==(const DateTime& Two) const
	{
		return _Dur == Two._Dur;
	}
	bool DateTime::operator!=(const DateTime& Two) const
	{
		return _Dur != Two._Dur;
	}

	bool DateTime::operator<(const DateTime& Two) const
	{
		return _Dur < Two._Dur;
	}
	bool DateTime::operator>(const DateTime& Two) const
	{
		return _Dur > Two._Dur;
	}

	DateTime DateTime::operator+(const DateTime& Two) const
	{
		return DateTime(_Dur + Two._Dur);
	}
	DateTime DateTime::operator-(const DateTime& Two) const
	{
		return DateTime(_Dur - Two._Dur);
	}
	DateTime& DateTime::operator+=(const DateTime& Two)
	{
		_Dur += Two._Dur;
		return *this;
	}
	DateTime& DateTime::operator-=(const DateTime& Two)
	{
		_Dur -= Two._Dur;
		return *this;
	}

	std::ostream& operator<<(std::ostream& out, const DateTime& Obj)
	{
		out << Obj.ToString(DateStringFormat::ShortDate);
		return out;
	}
	std::ofstream& operator<<(std::ofstream& out, const DateTime& Obj)
	{
		out << Obj._Dur.count();
		return out;
	}
	std::istream& operator>>(std::istream& in, DateTime& Obj)
	{
		double DurVal;
		in >> DurVal;

		Obj._Dur = duration<double>(DurVal);

		return in;
	}
}