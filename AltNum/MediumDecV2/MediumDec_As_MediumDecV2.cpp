#include "MediumDecV2.hpp"
using MediumDecV2 = BlazesRusCode::MediumDecV2;

#pragma region class_constructors

/// <summary>
/// Initializes a new instance of the <see cref="MediumDecV2"/> class.
/// </summary>
/// <param name="intVal">The whole number based half of the representation</param>
/// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>

inline BlazesRusCode::MediumDecV2::MediumDecV2(const MirroredInt& intVal, const PartialInt& decVal)
{
	IntHalf = intVal;
	DecimalHalf = decVal;
}

/// <summary>
/// Initializes a new instance of the <see cref="MediumDecV2"/> class.
/// </summary>
/// <param name="intVal">The whole number based half of the representation</param>
/// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>

inline BlazesRusCode::MediumDecV2::MediumDecV2(const signed int& intVal, const PartialInt& decVal)
{
	IntHalf = intVal;
	DecimalHalf = decVal;
}

/// <summary>
/// Initializes a new instance of the <see cref="MediumDecV2"/> class.
/// </summary>
/// <param name="Value">The value.</param>

inline BlazesRusCode::MediumDecV2::MediumDecV2(const MediumDecV2Base& rhs)
{
	IntHalf = rhs.IntHalf; DecimalHalf = rhs.DecimalHalf;
}

#pragma endregion class_constructors

inline void BlazesRusCode::MediumDecV2::SetValue(MediumDecV2 Value)
{
	IntHalf = Value.IntHalf;
	DecimalHalf = Value.DecimalHalf;
}

#pragma region ValueDefines

inline MediumDecV2 BlazesRusCode::MediumDecV2::AlmostOneValue()
{
	return MediumDecV2(0, 999999999);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::PiNumValue()
{
	return MediumDecV2(3, 141592654);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::HundredMilPiNumValue()
{
	return MediumDecV2(314159265, 358979324);
}

//10,000,000xPi(Rounded to 9th decimal digit)

inline MediumDecV2 BlazesRusCode::MediumDecV2::TenMilPiNumValue()
{
	return MediumDecV2(31415926, 535897932);
}

//1,000,000xPi(Rounded to 9th decimal digit)

inline MediumDecV2 BlazesRusCode::MediumDecV2::OneMilPiNumValue()
{
	return MediumDecV2(3141592, 653589793);
}

//10xPi(Rounded to 9th decimal digit)

inline MediumDecV2 BlazesRusCode::MediumDecV2::TenPiNumValue()
{
	return MediumDecV2(31, 415926536);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::ENumValue()
{
	return MediumDecV2(2, 718281828);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::ZeroValue()
{
	return MediumDecV2();
}

/// <summary>
/// Returns the value at one
/// </summary>
/// <returns>MediumDecV2</returns>

inline MediumDecV2 BlazesRusCode::MediumDecV2::OneValue()
{
	return MediumDecV2(1);
}

/// <summary>
/// Returns the value at one
/// </summary>
/// <returns>MediumDecV2</returns>

inline MediumDecV2 BlazesRusCode::MediumDecV2::TwoValue()
{
	return MediumDecV2(MirroredInt::Two);
}

/// <summary>
/// Returns the value at negative one
/// </summary>
/// <returns>MediumDecV2</returns>

inline MediumDecV2 BlazesRusCode::MediumDecV2::NegativeOneValue()
{
	return MediumDecV2(MirroredInt::NegativeOne);
}

/// <summary>
/// Returns the value at 0.5
/// </summary>
/// <returns>MediumDecV2</returns>

inline MediumDecV2 BlazesRusCode::MediumDecV2::Point5Value()
{
	return MediumDecV2(0, 500000000);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::JustAboveZeroValue()
{
	return MediumDecV2(0, 1);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::OneMillionthValue()
{
	return MediumDecV2(0, 1000);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::FiveThousandthValue()
{
	return MediumDecV2(0, 5000000);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::FiveMillionthValue()
{
	return MediumDecV2(0, 5000);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::TenMillionthValue()
{
	return MediumDecV2(0, 100);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::OneHundredMillionthValue()
{
	return MediumDecV2(0, 10);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::FiveBillionthValue()
{
	return MediumDecV2(0, 5);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::LN10Value()
{
	return MediumDecV2(2, 302585093);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::LN10DivValue()
{
	return MediumDecV2(0, 434294482);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::TwiceLN10DivValue()
{
	return MediumDecV2(0, 868588964);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::MinimumValue()
{
	return MediumDecV2(MirroredInt::Maximum, 999999999);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::MaximumValue()
{
	return MediumDecV2(MirroredInt::Minimum, 999999999);
}

inline MediumDecV2 BlazesRusCode::MediumDecV2::NegativePointFiveValue()
{
	return MediumDecV2(MirroredInt::NegativeZero, 500000000);
}


#pragma endregion ValueDefines

#pragma endregion ValueDefines

#pragma region ValueDefine Source
MediumDecV2 MediumDecV2::AlmostOne = MediumDecV2::AlmostOneValue();
MediumDecV2 MediumDecV2::Pi = MediumDecV2::PiNumValue();
MediumDecV2 MediumDecV2::One = MediumDecV2::OneValue();
MediumDecV2 MediumDecV2::Two = MediumDecV2::TwoValue();
MediumDecV2 MediumDecV2::NegativeOne = MediumDecV2::NegativeOneValue();
MediumDecV2 MediumDecV2::Zero = MediumDecV2::ZeroValue();
MediumDecV2 MediumDecV2::PointFive = MediumDecV2::Point5Value();
MediumDecV2 MediumDecV2::JustAboveZero = MediumDecV2::JustAboveZeroValue();
MediumDecV2 MediumDecV2::OneMillionth = MediumDecV2::OneMillionthValue();
MediumDecV2 MediumDecV2::FiveThousandth = MediumDecV2::FiveThousandthValue();
MediumDecV2 MediumDecV2::Minimum = MediumDecV2::MinimumValue();
MediumDecV2 MediumDecV2::Maximum = MediumDecV2::MaximumValue();
MediumDecV2 MediumDecV2::E = MediumDecV2::ENumValue();
MediumDecV2 MediumDecV2::LN10 = MediumDecV2::LN10Value();
MediumDecV2 MediumDecV2::LN10Div = MediumDecV2::LN10DivValue();
MediumDecV2 MediumDecV2::TwiceLN10Div = MediumDecV2::TwiceLN10DivValue();
MediumDecV2 MediumDecV2::TenMillionth = MediumDecV2::TenMillionthValue();
MediumDecV2 MediumDecV2::FiveMillionth = MediumDecV2::FiveMillionthValue();
MediumDecV2 MediumDecV2::FiveBillionth = MediumDecV2::FiveBillionthValue();
MediumDecV2 MediumDecV2::OneGMillionth = MediumDecV2::OneHundredMillionthValue();

MediumDecV2 MediumDecV2::PiNum = MediumDecV2::PiNumValue();
MediumDecV2 MediumDecV2::ENum = MediumDecV2::ENumValue();
MediumDecV2 MediumDecV2::NegativePointFive = MediumDecV2::NegativePointFiveValue();
#pragma endregion ValueDefine Source

#pragma region String Commands

inline MediumDecV2 BlazesRusCode::MediumDecV2::GetValueFromString(std::string Value)
{
	MediumDecV2 NewSelf = MediumDecV2();
	NewSelf.ReadString(Value);
	return NewSelf;
}

#pragma endregion String Commands

//Experimental result saving modulus operation(use assignment to get normal result)
	#pragma region Modulus Operations
    //Global Binary functions

  //      MediumDecV2& operator%=(MediumDecV2& lValue, const MediumDecV2& rValue)
		//{
  //          if(lValue.DecimalHalf.Value==0&&rValue.DecimalHalf.Value==0){
  //              if(rValue.IsNegative())
  //                  lValue.SwapNegativeStatus();
  //              lValue.IntHalf.Value %= rValue.IntHalf.Value;
  //          }
  //          else {
  //              MediumDecV2 divRes = lValue / rValue;
  //              lValue -= rValue * divRes;
  //          }
  //          return lValue;
		//}

  //      /// <summary>
  //      /// %= operation between MediumDecV2 variant and Integer rValue.
  //      /// </summary>
  //      /// <param name="lValue">The left side value</param>
  //      /// <param name="rValue">The right side value.</param>
  //      /// <returns>MediumDecV2</returns>
  //      MediumDecV2& operator%=(MediumDecV2& lValue, const signed int& rValue) { return lValue %= (MediumDecV2)rValue; }
  //      MediumDecV2& operator%=(MediumDecV2& lValue, const signed __int64& rValue) { return lValue%=(MediumDecV2)rValue; }
  //      MediumDecV2& operator%=(MediumDecV2& lValue, const unsigned int& rValue) { return lValue%=(MediumDecV2)rValue; }
  //      MediumDecV2& operator%=(MediumDecV2& lValue, const unsigned __int64& rValue) { return lValue%=(MediumDecV2)rValue; }

  //      MediumDecV2& operator%=(MediumDecV2& lValue, const signed char& rValue) { return lValue%=(MediumDecV2)rValue; }
  //      MediumDecV2& operator%=(MediumDecV2& lValue, const signed short& rValue) { return lValue%=(MediumDecV2)rValue; }
  //      MediumDecV2& operator%=(MediumDecV2& lValue, const unsigned char& rValue) { return lValue%=(MediumDecV2)rValue; }
  //      MediumDecV2& operator%=(MediumDecV2& lValue, const unsigned short& rValue) { return lValue%=(MediumDecV2)rValue; }
		
  //      using MediumDecV2ModResult = MediumDecV2::ModResult;

  //      /// <summary>
  //      /// Modulus Operation:
		///// divRes member variables gives division result,
		///// modRes member variable and + operator gives mod result,
		///// bool conversion gives result of modRes==0
  //      /// </summary>
  //      /// <param name="self">The left side value</param>
  //      /// <param name="Value">The right side value.</param>
  //      /// <returns>MediumDecV2V2Base</returns>
  //      MediumDecV2ModResult operator%(const MediumDecV2& LValue, const MediumDecV2& RValue)
		//{
		//	return MediumDecV2ModResult(LValue, RValue);
		//}
//        /// <summary>
//        /// Modulus operation between MediumDecV2 variant and Integer rValue.
//        /// </summary>
//        /// <param name="lValue">The left side value</param>
//        /// <param name="rValue">The right side value.</param>
//        /// <returns>MediumDecV2</returns>
//        friend ModResult operator%(const MediumDecV2& lValue, const signed int& rValue) { return ModResult(lValue, MediumDecV2(rValue)); }
//        friend ModResult operator%(const MediumDecV2& lValue, const signed __int64& rValue) { return ModResult(lValue, MediumDecV2(rValue));  }
//        friend ModResult operator%(const MediumDecV2& lValue, const unsigned int& rValue) { return ModResult(lValue, MediumDecV2(rValue));  }
//        friend ModResult operator%(const MediumDecV2& lValue, const unsigned __int64& rValue) { return ModResult(lValue, MediumDecV2(rValue));  }
//		
//        friend ModResult operator%(const MediumDecV2& lValue, const signed char& rValue) { return ModResult(lValue, MediumDecV2(rValue)); }
//        friend ModResult operator%(const MediumDecV2& lValue, const signed short& rValue) { return ModResult(lValue, MediumDecV2(rValue));  }
//        friend ModResult operator%(const MediumDecV2& lValue, const unsigned char& rValue) { return ModResult(lValue, MediumDecV2(rValue));  }
//        friend ModResult operator%(const MediumDecV2& lValue, const unsigned short& rValue) { return ModResult(lValue, MediumDecV2(rValue));  }
//		
//
//        friend ModResult operator%(const signed int& lValue, const MediumDecV2& rValue) { return ModResult(MediumDecV2(lValue), rValue);  }
//        friend ModResult operator%(const signed __int64& lValue, const MediumDecV2& rValue) { return ModResult(MediumDecV2(lValue), rValue); }
//        friend ModResult operator%(const unsigned int& lValue, const MediumDecV2& rValue) { return ModResult(MediumDecV2(lValue), rValue); }
//        friend ModResult operator%(const unsigned __int64& lValue, const MediumDecV2& rValue) { return ModResult(MediumDecV2(lValue), rValue); }
//		
//        friend ModResult operator%(const signed char& lValue, const MediumDecV2& rValue) { return ModResult(MediumDecV2(lValue), rValue); }
//        friend ModResult operator%(const signed short& lValue, const MediumDecV2& rValue) { return ModResult(MediumDecV2(lValue), rValue); }
//        friend ModResult operator%(const unsigned char& lValue, const MediumDecV2& rValue) { return ModResult(MediumDecV2(lValue), rValue); }
//        friend ModResult operator%(const unsigned short& lValue, const MediumDecV2& rValue) { return ModResult(MediumDecV2(lValue), rValue); }

	#pragma endregion Modulus Operations

/// <summary>
/// Gets the value from string.
/// </summary>
/// <param name="Value">The value.</param>
/// <returns>MediumDecV2</returns>