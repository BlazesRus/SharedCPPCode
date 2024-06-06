#include "MediumDec.hpp"
using MediumDec = BlazesRusCode::MediumDec;

#pragma region class_constructors

inline BlazesRusCode::MediumDec::MediumDec(const MirroredInt& intVal, const PartialInt& decVal)
{
	IntHalf = intVal;
	DecimalHalf = decVal;
}

inline BlazesRusCode::MediumDec::MediumDec(const signed int& intVal, const PartialInt& decVal)
{
	IntHalf = intVal;
	DecimalHalf = decVal;
}

inline BlazesRusCode::MediumDec::MediumDec(const MediumDecBase& rhs)
{
	IntHalf = rhs.IntHalf; DecimalHalf = rhs.DecimalHalf;
}

#pragma endregion class_constructors

inline void BlazesRusCode::MediumDec::SetValue(MediumDec Value)
{
	IntHalf = Value.IntHalf;
	DecimalHalf = Value.DecimalHalf;
}

#pragma region ValueDefines

inline MediumDec BlazesRusCode::MediumDec::AlmostOneValue()
{
	return MediumDec(0, 999999999);
}

inline MediumDec BlazesRusCode::MediumDec::PiNumValue()
{
	return MediumDec(3, 141592654);
}

inline MediumDec BlazesRusCode::MediumDec::HundredMilPiNumValue()
{
	return MediumDec(314159265, 358979324);
}

inline MediumDec BlazesRusCode::MediumDec::TenMilPiNumValue()
{
	return MediumDec(31415926, 535897932);
}

inline MediumDec BlazesRusCode::MediumDec::OneMilPiNumValue()
{
	return MediumDec(3141592, 653589793);
}

inline MediumDec BlazesRusCode::MediumDec::TenPiNumValue()
{
	return MediumDec(31, 415926536);
}

inline MediumDec BlazesRusCode::MediumDec::ENumValue()
{
	return MediumDec(2, 718281828);
}

inline MediumDec BlazesRusCode::MediumDec::ZeroValue()
{
	return MediumDec();
}

inline MediumDec BlazesRusCode::MediumDec::OneValue()
{
	return MediumDec(1);
}

inline MediumDec BlazesRusCode::MediumDec::TwoValue()
{
	return MediumDec(MirroredInt::Two);
}

inline MediumDec BlazesRusCode::MediumDec::NegativeOneValue()
{
	return MediumDec(MirroredInt::NegativeOne);
}

inline MediumDec BlazesRusCode::MediumDec::Point5Value()
{
	return MediumDec(0, 500000000);
}

inline MediumDec BlazesRusCode::MediumDec::JustAboveZeroValue()
{
	return MediumDec(0, 1);
}

inline MediumDec BlazesRusCode::MediumDec::OneMillionthValue()
{
	return MediumDec(0, 1000);
}

inline MediumDec BlazesRusCode::MediumDec::FiveThousandthValue()
{
	return MediumDec(0, 5000000);
}

inline MediumDec BlazesRusCode::MediumDec::FiveMillionthValue()
{
	return MediumDec(0, 5000);
}

inline MediumDec BlazesRusCode::MediumDec::TenMillionthValue()
{
	return MediumDec(0, 100);
}

inline MediumDec BlazesRusCode::MediumDec::OneHundredMillionthValue()
{
	return MediumDec(0, 10);
}

inline MediumDec BlazesRusCode::MediumDec::FiveBillionthValue()
{
	return MediumDec(0, 5);
}

inline MediumDec BlazesRusCode::MediumDec::LN10Value()
{
	return MediumDec(2, 302585093);
}

inline MediumDec BlazesRusCode::MediumDec::LN10DivValue()
{
	return MediumDec(0, 434294482);
}

inline MediumDec BlazesRusCode::MediumDec::TwiceLN10DivValue()
{
	return MediumDec(0, 868588964);
}

inline MediumDec BlazesRusCode::MediumDec::MinimumValue()
{
	return MediumDec(MirroredInt::Maximum, 999999999);
}

inline MediumDec BlazesRusCode::MediumDec::MaximumValue()
{
	return MediumDec(MirroredInt::Minimum, 999999999);
}

inline MediumDec BlazesRusCode::MediumDec::NegativePointFiveValue()
{
	return MediumDec(MirroredInt::NegativeZero, 500000000);
}

#pragma endregion ValueDefines

#pragma region ValueDefine Source
MediumDec MediumDec::AlmostOne = MediumDec::AlmostOneValue();
MediumDec MediumDec::Pi = MediumDec::PiNumValue();
MediumDec MediumDec::One = MediumDec::OneValue();
MediumDec MediumDec::Two = MediumDec::TwoValue();
MediumDec MediumDec::NegativeOne = MediumDec::NegativeOneValue();
MediumDec MediumDec::Zero = MediumDec::ZeroValue();
MediumDec MediumDec::PointFive = MediumDec::Point5Value();
MediumDec MediumDec::JustAboveZero = MediumDec::JustAboveZeroValue();
MediumDec MediumDec::OneMillionth = MediumDec::OneMillionthValue();
MediumDec MediumDec::FiveThousandth = MediumDec::FiveThousandthValue();
MediumDec MediumDec::Minimum = MediumDec::MinimumValue();
MediumDec MediumDec::Maximum = MediumDec::MaximumValue();
MediumDec MediumDec::E = MediumDec::ENumValue();
MediumDec MediumDec::LN10 = MediumDec::LN10Value();
MediumDec MediumDec::LN10Div = MediumDec::LN10DivValue();
MediumDec MediumDec::TwiceLN10Div = MediumDec::TwiceLN10DivValue();
MediumDec MediumDec::TenMillionth = MediumDec::TenMillionthValue();
MediumDec MediumDec::FiveMillionth = MediumDec::FiveMillionthValue();
MediumDec MediumDec::FiveBillionth = MediumDec::FiveBillionthValue();
MediumDec MediumDec::OneGMillionth = MediumDec::OneHundredMillionthValue();

MediumDec MediumDec::PiNum = MediumDec::PiNumValue();
MediumDec MediumDec::ENum = MediumDec::ENumValue();
MediumDec MediumDec::NegativePointFive = MediumDec::NegativePointFiveValue();
#pragma endregion ValueDefine Source

#pragma region String Commands

inline MediumDec BlazesRusCode::MediumDec::GetValueFromString(std::string Value)
{
	MediumDec NewSelf = MediumDec();
	NewSelf.ReadString(Value);
	return NewSelf;
}

#pragma endregion String Commands

//Experimental result saving modulus operation(use assignment to get normal result)
	#pragma region Modulus Operations
    //Global Binary functions

  //      MediumDec& operator%=(MediumDec& lValue, const MediumDec& rValue)
		//{
  //          if(lValue.DecimalHalf.Value==0&&rValue.DecimalHalf.Value==0){
  //              if(rValue.IsNegative())
  //                  lValue.SwapNegativeStatus();
  //              lValue.IntHalf.Value %= rValue.IntHalf.Value;
  //          }
  //          else {
  //              MediumDec divRes = lValue / rValue;
  //              lValue -= rValue * divRes;
  //          }
  //          return lValue;
		//}

  //      /// <summary>
  //      /// %= operation between MediumDec variant and Integer rValue.
  //      /// </summary>
  //      /// <param name="lValue">The left side value</param>
  //      /// <param name="rValue">The right side value.</param>
  //      /// <returns>MediumDec</returns>
  //      MediumDec& operator%=(MediumDec& lValue, const signed int& rValue) { return lValue %= (MediumDec)rValue; }
  //      MediumDec& operator%=(MediumDec& lValue, const signed __int64& rValue) { return lValue%=(MediumDec)rValue; }
  //      MediumDec& operator%=(MediumDec& lValue, const unsigned int& rValue) { return lValue%=(MediumDec)rValue; }
  //      MediumDec& operator%=(MediumDec& lValue, const unsigned __int64& rValue) { return lValue%=(MediumDec)rValue; }

  //      MediumDec& operator%=(MediumDec& lValue, const signed char& rValue) { return lValue%=(MediumDec)rValue; }
  //      MediumDec& operator%=(MediumDec& lValue, const signed short& rValue) { return lValue%=(MediumDec)rValue; }
  //      MediumDec& operator%=(MediumDec& lValue, const unsigned char& rValue) { return lValue%=(MediumDec)rValue; }
  //      MediumDec& operator%=(MediumDec& lValue, const unsigned short& rValue) { return lValue%=(MediumDec)rValue; }
		
  //      using MediumDecModResult = MediumDec::ModResult;

  //      /// <summary>
  //      /// Modulus Operation:
		///// divRes member variables gives division result,
		///// modRes member variable and + operator gives mod result,
		///// bool conversion gives result of modRes==0
  //      /// </summary>
  //      /// <param name="self">The left side value</param>
  //      /// <param name="Value">The right side value.</param>
  //      /// <returns>MediumDecV2Base</returns>
  //      MediumDecModResult operator%(const MediumDec& LValue, const MediumDec& RValue)
		//{
		//	return MediumDecModResult(LValue, RValue);
		//}
//        /// <summary>
//        /// Modulus operation between MediumDec variant and Integer rValue.
//        /// </summary>
//        /// <param name="lValue">The left side value</param>
//        /// <param name="rValue">The right side value.</param>
//        /// <returns>MediumDec</returns>
//        friend ModResult operator%(const MediumDec& lValue, const signed int& rValue) { return ModResult(lValue, MediumDec(rValue)); }
//        friend ModResult operator%(const MediumDec& lValue, const signed __int64& rValue) { return ModResult(lValue, MediumDec(rValue));  }
//        friend ModResult operator%(const MediumDec& lValue, const unsigned int& rValue) { return ModResult(lValue, MediumDec(rValue));  }
//        friend ModResult operator%(const MediumDec& lValue, const unsigned __int64& rValue) { return ModResult(lValue, MediumDec(rValue));  }
//		
//        friend ModResult operator%(const MediumDec& lValue, const signed char& rValue) { return ModResult(lValue, MediumDec(rValue)); }
//        friend ModResult operator%(const MediumDec& lValue, const signed short& rValue) { return ModResult(lValue, MediumDec(rValue));  }
//        friend ModResult operator%(const MediumDec& lValue, const unsigned char& rValue) { return ModResult(lValue, MediumDec(rValue));  }
//        friend ModResult operator%(const MediumDec& lValue, const unsigned short& rValue) { return ModResult(lValue, MediumDec(rValue));  }
//		
//
//        friend ModResult operator%(const signed int& lValue, const MediumDec& rValue) { return ModResult(MediumDec(lValue), rValue);  }
//        friend ModResult operator%(const signed __int64& lValue, const MediumDec& rValue) { return ModResult(MediumDec(lValue), rValue); }
//        friend ModResult operator%(const unsigned int& lValue, const MediumDec& rValue) { return ModResult(MediumDec(lValue), rValue); }
//        friend ModResult operator%(const unsigned __int64& lValue, const MediumDec& rValue) { return ModResult(MediumDec(lValue), rValue); }
//		
//        friend ModResult operator%(const signed char& lValue, const MediumDec& rValue) { return ModResult(MediumDec(lValue), rValue); }
//        friend ModResult operator%(const signed short& lValue, const MediumDec& rValue) { return ModResult(MediumDec(lValue), rValue); }
//        friend ModResult operator%(const unsigned char& lValue, const MediumDec& rValue) { return ModResult(MediumDec(lValue), rValue); }
//        friend ModResult operator%(const unsigned short& lValue, const MediumDec& rValue) { return ModResult(MediumDec(lValue), rValue); }

	#pragma endregion Modulus Operations

/// <summary>
/// Gets the value from string.
/// </summary>
/// <param name="Value">The value.</param>
/// <returns>MediumDec</returns>