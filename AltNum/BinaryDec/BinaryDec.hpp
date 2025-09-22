// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include <string>
#include <cmath>

#include <type_traits>
#include <cstddef>
#include <compare>//used for C++20 feature of spaceship operator
#include "..\PolicyConcepts.hpp"

#include "..\AlternativeInt\MirroredInt.hpp"
using MirroredInt = BlazesRusCode::MirroredInt;

namespace BlazesRusCode
{
  namespace BinaryDecCode
  {
    struct EmptyStruct{};

	  template<typename Policy>
    concept UseCustomMode   = requires { { Policy::UseCustomMode   } -> std::convertible_to<bool>; };
  }
  
  class DefaultBinaryUDecIntHalfStorage{
    /// <summary>
    /// Stores whole half of number
    /// </summary>
    unsigned int IntHalf;
  };

  class DefaultBinaryDecDecimalStorage{
  public:
  #pragma region DigitStorage

    /// <summary>
    /// Stores decimal section info (mixed binary fraction representation)
    /// </summary>
    unsigned int DecimalHalf;

    //Return IntHalf as signed int
    inline unsigned int GetIntHalf() const
    {
      return IntHalf;
    }
    
  #pragma endregion DigitStorage
	
    /// <summary>
    /// The decimal overflow
    /// </summary>
		static constexpr unsigned int DecimalOverflow = 1 << 32;
		
    /// <summary>
    /// The decimal overflow in _int64 so don't need to widen
    /// </summary>
		static constexpr unsigned _int64 DecimalOverflow = 1 << 32;
		
    static constexpr DecimalHalfT DecimalMax = DecimalOverflow - 1;
	};

  class DefaultBinaryUDecStorage : DefaultBinaryUDecIntHalfStorage, DefaultBinaryDecDecimalStorage{
	};

	template<typename Policy>
  CustomBinaryUDecStorage{
  public:
  #pragma region DigitStorage

    /// <summary>
    /// Stores whole half of number
    /// </summary>
    unsigned int IntHalf:Policy::INT_BITS;

    /// <summary>
    /// Stores decimal section info (mixed binary fraction representation)
    /// </summary>
    unsigned int DecimalHalf:Policy::DEC_BITS;

    //Return IntHalf as signed int
    inline unsigned int GetIntHalf() const
    {
      return IntHalf;
    }
    
  #pragma endregion DigitStorage
	
    /// <summary>
    /// The decimal overflow
    /// </summary>
		static constexpr unsigned int DecimalOverflow = 1 << DEC_BITS;
		
    /// <summary>
    /// The decimal overflow in _int64 so don't need to widen
    /// </summary>
		static constexpr unsigned _int64 DecimalOverflow = 1 << DEC_BITS;
		
    static constexpr unsigned int DecimalMax = DecimalOverflow - 1;
	}

	template<typename Policy>
  BinaryDecSignedPolicies{
  protected:
    // PositiveSign and NegativeSign are logical convention constants (1/0 or true/false).
    // They define whether a sign bit value of 1 means "positive" (this is inverted from the usual two's-complement convention).
    // The "sign bit" here refers to the logical sign indicator for this type, not necessarily a literal bit in all modes.
    // Defaults: PositiveSign = 1, NegativeSign = 0 if neither is provided in the policy.
    // Exactly one may be defined in the policy; the other is derived to be its opposite.
    // In signed mode, negative values do NOT have their magnitude bits flipped — only the sign indicator changes.
    // This avoids magnitude inversion overhead and enhances multiplication/division speed for negative values.
    static inline constexpr u32 PositiveSign   = Has_PositiveSign ? Policy::PositiveSign : (Has_NegativeSign?0:1);
    static inline constexpr u32 NegativeSign   = Has_PositiveSign ? 0 : (Has_NegativeSign?1:0);
	}

  class DefaultBinaryDecIntHalfStorage{
    /// <summary>
    /// Stores whole half of number(Including positive/negative status)
    /// </summary>
    MirroredInt IntHalf;
  };

  class DefaultBinaryDecStorage : public DefaultBinaryDecIntHalfStorage, public DefaultBinaryDecDecimalStorage, protected BinaryDecSignedPolicies{
	};

	template<typename Policy>
	struct BinaryDecStorageSelector : 
	std::conditional_t<Has_UseCustomDenom<Policy>, RestrictedFloatExtraCustomDenomSupport<Policy>, BinaryDecCode::EmptyStruct>,
	{
	protected:
    static_assert(!(Has_UnsignedMode && Has_SignedMode),
      "Policy cannot define both UnsignedMode and SignedMode");
    static inline constexpr bool SignedMode = Has_SignedMode ? Policy::SignedMode : false;

		using DefaultPart = std::conditional_t<!UseCustomMode&&!SignedMode,RestrictedFloatExtraCustomDenomSupport<Policy>, BinaryDecCode::EmptyStruct>;
		using CustomPart = std::conditional_t<UseCustomMode&&!SignedMode,RestrictedFloatExtraCustomDenomSupport<Policy>, BinaryDecCode::EmptyStruct>;
		using DefaultSignedPart = std::conditional_t<!UseCustomMode&&SignedMode,RestrictedFloatExtraCustomDenomSupport<Policy>, BinaryDecCode::EmptyStruct>;
		//Use MediumUDecV3Variant later if needed signed BinaryDec with custom widths
	public:	
		BinaryDecStorageSelector() noexcept : DefaultPart, CustomPart, DefaultSignedPart, CustomSignedPart() {}
	};

  //Fixed number representation with binary based fractional tail
  template<class VariantClass, typename Policy:BinaryDecCode::EmptyStruct>
  class BinaryDec : public BinaryDecStorageSelector<VariantClass, Policy>
  {
  public:
  #pragma region class_constructors

		/// <summary>
		/// Initializes a new instance of the unsigned <see cref="BinaryDec"/> class using signed integer.
		/// </summary>
		/// <param name="intVal">The whole number based half of the representation</param>
		/// <param name="decVal">The non-whole based half of the representation (and other special statuses)</param>
    template<typename = std::enable_if_t<!SignedMode>>
		constexpr BinaryDec(const unsigned int& intVal = 0, const unsigned int& decVal = 0)
		: IntHalf(intVal), DecimalHalf(decVal) {}

		/// <summary>
		/// Initializes a new instance of the signed <see cref="BinaryDec"/> class using IntHalf's type field.
		/// </summary>
		/// <param name="intVal">The whole number based half of the representation</param>
		/// <param name="decVal">The non-whole based half of the representation (and other special statuses)</param>
    template<typename = std::enable_if_t<SignedMode>>
		constexpr BinaryDec(const MirroredInt& intVal, const unsigned int& decVal = 0)
		: IntHalf(intVal), DecimalHalf(decVal) {}

		/// <summary>
		/// Initializes a new instance of the signed <see cref="BinaryDec"/> class using signed integer.
		/// </summary>
		/// <param name="intVal">The whole number based half of the representation</param>
		/// <param name="decVal">The non-whole based half of the representation (and other special statuses)</param>
    template<typename = std::enable_if_t<SignedMode>>
		constexpr BinaryDec(const signed int& intVal = 0, const unsigned int& decVal = 0)
		: IntHalf(intVal), DecimalHalf(decVal) {}

    BinaryDec& operator=(const BinaryDec& rhs)
    {
      // Check for self-assignment
      if (this == &rhs)    // Same object?
        return *this;    // Yes, so skip assignment, and just return *this.
      IntHalf = rhs.IntHalf; DecimalHalf = rhs.DecimalHalf;
      return *this;
    }

    BinaryDec& operator=(const signed int& rhs)
    {
      IntHalf = rhs; DecimalHalf = 0;
      return *this;
    }

    /// <summary>
    /// Sets the value.
    /// </summary>
    /// <param name="Value">The value.</param>
    void SetValue(const BinaryDec& rValue)
    {
      IntHalf = rValue.IntHalf;
      DecimalHalf.SetValueV2(rValue.DecimalHalf);
    }

  #pragma endregion class_constructors

  #pragma region Check_if_value

    //Set value as exactly zero
    void SetAsZero();

    //Set value as exactly one
    void SetAsOne();

    //Set as +-1 while keeping current sign
    void SetAsOneVal();

    void SetAsValues(const unsigned int& intVal = 0, const unsigned int& decVal = unsigned int::Zero);

    //Is at either zero or negative zero IntHalf of AltNum
    bool IsAtZeroInt() const;

    bool IsNotAtZeroInt() const;

    bool IsAtOneInt() const;

    bool IsNotAtOneInt() const;

    //Detect if at exactly zero(only overridden with MixedDec)
    bool IsZero() const;

    bool IsOne() const;

    bool IsOneVal() const;

  #pragma endregion Check_if_value

  #pragma region RangeLimits

    /// <summary>
    /// Sets value to the highest non-infinite/Special Decimal State tValue that it store
    /// </summary>
    void SetAsMaximum();

    /// <summary>
    /// Sets value to the lowest non-infinite/Special Decimal State tValue that it store
    /// </summary>
    void SetAsMinimum();

  #pragma endregion RangeLimits

  #pragma region ValueSetters
protected://Work around for not allowing to use incomplete class statics during forming of class
    static const unsigned int LN10Div_DecSection = 434294482;
    static const unsigned int TwiceLN10Div_DecSection = 868588964;

public:

    /// <summary>
    /// Sets value to Pi(3.1415926535897932384626433) with tenth digit rounded up
    /// (Stored as 3.141592654)
    /// </summary>
    void  SetValueToPiNum();

    //100,000,000xPi(Rounded to 9th decimal digit)
    void  SetValueToHundredMilPiNum();

    //10,000,000xPi(Rounded to 9th decimal digit)
    void  SetValueToTenMilPiNum();

    //1,000,000xPi(Rounded to 9th decimal digit)
    void  SetValueToOneMilPiNum();

    //10xPi(Rounded to 9th decimal digit)
    void  SetValueToTenPiNum();

    /// <summary>
    /// Euler's number rounded to 9th digit(2.718281828)
    /// Irrational number equal to about (1 + 1/n)^n
    /// (about 2.71828182845904523536028747135266249775724709369995)
    /// </summary>
    void  SetValueToENum();

    //Sets value to value at 0.5
    void  SetValueToPoint5();

    void  SetValueToJustAboveZero();

    /// <summary>
    /// Sets the value at .000001000
    /// </summary>
    void  SetValueToOneMillionth();

    /// <summary>
    /// Sets the value at "0.005"
    /// </summary>
    /// <returns>BinaryDec</returns>
    void  SetValueToFiveThousandth();

    /// <summary>
    /// Sets the value at "0.000005"
    /// </summary>
    void  SetValueToFiveMillionth();

    //0e-7
    void  SetValueToTenMillionth();

    /// <summary>
    /// Sets the value to .000000010
    /// </summary>
    void  SetValueToOneHundredMillionth();

    /// <summary>
    /// 2.3025850929940456840179914546844
    /// (Based on https://stackoverflow.com/questions/35968963/trying-to-calculate-logarithm-base-10-without-math-h-really-close-just-having)
    /// </summary>
    void  SetValueToLN10();

    /// <summary>
    /// (1 / Ln10) (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
    /// </summary>
    void  SetValueToLN10Div();

    /// <summary>
    /// (1 / Ln10)*2 (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
    /// </summary>
    void  SetValueToTwiceLN10Div();

    void SetValueToPointOne();

  #pragma endregion ValueSetters

  #pragma region ValueDefines
public:

    static BinaryDec AlmostOneValue();

    /// <summary>
    /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up
    /// (Stored as 3.141592654)
    /// </summary>
    /// <returns>BinaryDec</returns>
    static BinaryDec PiNumValue();

    //100,000,000xPi(Rounded to 9th decimal digit)
    static BinaryDec HundredMilPiNumValue();

    //10,000,000xPi(Rounded to 9th decimal digit)
    static BinaryDec TenMilPiNumValue();

    //1,000,000xPi(Rounded to 9th decimal digit)
    static BinaryDec OneMilPiNumValue();

    //10xPi(Rounded to 9th decimal digit)
    static BinaryDec TenPiNumValue();

    static BinaryDec ENumValue();

    static BinaryDec ZeroValue();

    /// <summary>
    /// Returns the value at one
    /// </summary>
    /// <returns>BinaryDec</returns>
    static BinaryDec OneValue();

    /// <summary>
    /// Returns the value at one
    /// </summary>
    /// <returns>BinaryDec</returns>
    static BinaryDec TwoValue();

    /// <summary>
    /// Returns the value at 0.5
    /// </summary>
    /// <returns>BinaryDec</returns>
    static BinaryDec Point5Value();

    static BinaryDec JustAboveZeroValue();

    static BinaryDec OneMillionthValue();

    static BinaryDec FiveThousandthValue();

    static BinaryDec FiveMillionthValue();

    static BinaryDec TenMillionthValue();

    static BinaryDec OneHundredMillionthValue();

    static BinaryDec FiveBillionthValue();

    static BinaryDec LN10Value();

    static BinaryDec LN10DivValue();

    static BinaryDec TwiceLN10DivValue();

    static BinaryDec MinimumValue();

    static BinaryDec MaximumValue();

    static BinaryDec PointOneValue();

    static const BinaryDec AlmostOne;

    /// <summary>
    /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up to 3.141592654
    /// </summary>
    /// <returns>BinaryDec</returns>
    static const BinaryDec PiNum;

    /// <summary>
    /// Euler's number (Non-Alternative Representation)
    /// Irrational number equal to about (1 + 1/n)^n
    /// (about 2.71828182845904523536028747135266249775724709369995)
    /// </summary>
    /// <returns>BinaryDec</returns>
    static const BinaryDec ENum;

    /// <summary>
    /// Returns Pi(3.1415926535897932384626433) Representation
    /// </summary>
    /// <returns>BinaryDec</returns>
    static const BinaryDec Pi;

    /// <summary>
    /// Euler's number (Non-Alternative Representation)
    /// Irrational number equal to about (1 + 1/n)^n
    /// (about 2.71828182845904523536028747135266249775724709369995)
    /// </summary>
    /// <returns>BinaryDec</returns>
    static const BinaryDec E;

    /// <summary>
    /// Returns the value at zero
    /// </summary>
    /// <returns>BinaryDec</returns>
    static const BinaryDec Zero;

    /// <summary>
    /// Returns the value at one
    /// </summary>
    /// <returns>BinaryDec</returns>
    static const BinaryDec One;

    /// <summary>
    /// Returns the value at two
    /// </summary>
    /// <returns>BinaryDec</returns>
    static const BinaryDec Two;

    /// <summary>
    /// Returns the value at 0.5
    /// </summary>
    /// <returns>BinaryDec</returns>
    static const BinaryDec PointFive;

    /// <summary>
    /// Returns the value at digit one more than zero (0.000000001)
    /// </summary>
    /// <returns>BinaryDec</returns>
    static const BinaryDec JustAboveZero;

    /// <summary>
    /// Returns the value at .000000005
    /// </summary>
    /// <returns>BinaryDec</returns>
    static const BinaryDec FiveBillionth;

    /// <summary>
    /// Returns the value at .000001000
    /// </summary>
    /// <returns>BinaryDec</returns>
    static const BinaryDec OneMillionth;

    /// <summary>
    /// Returns the value at "0.005"
    /// </summary>
    /// <returns>BinaryDec</returns>
    static const BinaryDec FiveThousandth;

    /// <summary>
    /// Returns the value at .000000010
    /// </summary>
    /// <returns>BinaryDec</returns>
    static const BinaryDec OneGMillionth;

    //0e-7
    static const BinaryDec TenMillionth;

    /// <summary>
    /// Returns the value at "0.000005"
    /// </summary>
    static const BinaryDec FiveMillionth;

    /// <summary>
    /// Returns value of lowest non-infinite/Special Decimal State tValue that can store
    /// (-2147483647.999999999)
    /// </summary>
    static const BinaryDec Minimum;

    /// <summary>
    /// Returns value of highest non-infinite/Special Decimal State tValue that can store
    /// (2147483647.999999999)
    /// </summary>
    static const BinaryDec Maximum;

    /// <summary>
    /// 2.3025850929940456840179914546844
    /// (Based on https://stackoverflow.com/questions/35968963/trying-to-calculate-logarithm-base-10-without-math-h-really-close-just-having)
    /// </summary>
    static const BinaryDec LN10;

    /// <summary>
    /// (1 / Ln10) (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
    /// </summary>
    static const BinaryDec LN10Div;

    /// <summary>
    /// (1 / Ln10)*2 (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
    /// </summary>
    static const BinaryDec TwiceLN10Div;

    static const BinaryDec PointOne;

  #pragma endregion ValueDefines

  #pragma region String Commands

    /// <summary>
    /// Reads the string.
    /// </summary>
    /// <param name="Value">The value.</param>
    void ReadString(const std::string& Value);

    /// <summary>
    /// Initializes a new instance of the <see cref="BinaryDec"/> class from string literal
    /// </summary>
    /// <param name="strVal">The value.</param>
    BinaryDec(const char* strVal);

    /// <summary>
    /// Initializes a new instance of the <see cref="BinaryDec"/> class.
    /// </summary>
    /// <param name="Value">The value.</param>
    BinaryDec(const std::string& Value);

public:

    /// <summary>
    /// Converts to string.
    /// </summary>
    /// <returns>std.string</returns>
    std::string ToString();

    /// <summary>
    /// Converts to string with digits filled in even when empty
    /// </summary>
    /// <returns>std.string</returns>
    std::string ToFullString();

    /// <summary>
    /// Implements the operator std::string operator.
    /// </summary>
    /// <returns>The result of the operator.</returns>
    explicit operator std::string() { return ToString(); }

  #pragma endregion String Commands

  #pragma region ConvertFromOtherTypes

  #if defined(AltNum_EnableFloatingConversion)

    /// <summary>
    /// Sets the value.
    /// </summary>
    /// <param name="Value">The value.</param>
    void SetFloatVal(const float& tValue);

    /// <summary>
    /// Sets the value.
    /// </summary>
    /// <param name="Value">The value.</param>
    void SetDoubleVal(const double& tValue);

    /// <summary>
    /// Sets the value.
    /// </summary>
    /// <param name="Value">The value.</param>
    void SetDecimalVal(const long double& tValue);

  #endif

    /// <summary>
    /// Sets the value(false equals zero; otherwise is true).
    /// </summary>
    /// <param name="Value">The value.</param>
    void SetBoolVal(const bool& Value);

    /// <summary>
    /// Sets the value.
    /// </summary>
    /// <param name="Value">The value.</param>
    void SetIntVal(const unsigned int& Value);

  #if defined(AltNum_EnableFloatingConversion)

    /// <summary>
    /// Initializes a new instance of the <see cref="BinaryDec"/> class.
    /// </summary>
    /// <param name="Value">The value.</param>
    BinaryDec(const float& tValue){ this->SetFloatVal(tValue); }

    /// <summary>
    /// Initializes a new instance of the <see cref="BinaryDec"/> class.
    /// </summary>
    /// <param name="Value">The value.</param>
    BinaryDec(const double& tValue){ this->SetDoubleVal(tValue); }

    /// <summary>
    /// Initializes a new instance of the <see cref="BinaryDec"/> class.
    /// </summary>
    /// <param name="Value">The value.</param>
    BinaryDec(const long double& tValue){ this->SetDecimalVal(tValue); }

  #endif

  #pragma endregion ConvertFromOtherTypes

  #pragma region ConvertToOtherTypes

  #if defined(AltNum_EnableFloatingConversion)

    /// <summary>
    /// BinaryDec Variant to float explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    float toFloat() const;

    /// <summary>
    /// BinaryDec Variant to double explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    double toDouble() const;

    /// <summary>
    /// BinaryDec Variant to long double explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    long double toDecimal() const;

  #endif

    /// <summary>
    /// BinaryDec Variant to int explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    signed int toInt() const { return IntHalf; }

    /// <summary>
    /// BinaryDec Variant to int explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    unsigned int toUInt() const { return IntHalf; }

    bool toBool() const { return IntHalf==0 ? false : true; }

  #if defined(AltNum_EnableFloatingConversion)

    /// <summary>
    /// BinaryDec Variant to float explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    explicit operator float() { return toFloat(); }

    /// <summary>
    /// BinaryDec Variant to double explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    explicit operator double() { return toDouble(); }

    /// <summary>
    /// BinaryDec Variant to decimal explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    explicit operator long double() { return toDecimal(); }

  #endif

    /// <summary>
    /// BinaryDec Variant to int explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    explicit operator signed int() { return toInt(); }

    /// <summary>
    /// BinaryDec Variant to uint explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    explicit operator unsigned int() { return toUInt(); }

    /// <summary>
    /// BinaryDec Variant to int64 explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    explicit operator signed __int64() { return toInt(); }

    /// <summary>
    /// BinaryDec Variant to ubyte explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    explicit operator unsigned char() { return toUInt(); }

    /// <summary>
    /// BinaryDec Variant to byte explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    explicit operator signed char() { return toInt(); }

    /// <summary>
    /// BinaryDec Variant to ushort explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    explicit operator unsigned short() { return toUInt(); }

    /// <summary>
    /// BinaryDec Variant to short explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    explicit operator signed short() { return toInt(); }

    /// <summary>
    /// BinaryDec Variant to bool explicit conversion
    /// </summary>
    /// <returns>The result of the operator.</returns>
    explicit operator bool() { return toBool(); }

  #pragma endregion ConvertToOtherTypes

  #pragma region Comparison Operators
protected:
    //Compare only as if in NormalType representation mode
    template<BinaryDecVariant VariantType=BinaryDec>
    std::strong_ordering BasicComparisonV1(const VariantType& that) const
    {
      if (auto IntHalfCmp = IntHalf <=> that.IntHalf; IntHalfCmp != 0)
        return IntHalfCmp;
      if (auto DecimalHalfCmp = DecimalHalf.Value <=> that.DecimalHalf.Value; DecimalHalfCmp != 0)
        return DecimalHalfCmp;
    }

    std::strong_ordering BasicComparison(const BinaryDec& that) const
    {
      return BasicComparisonV1(that);
    }

    //Compare only as if in NormalType representation mode
    std::strong_ordering BasicUIntComparison(const unsigned int& that) const
    {
      if (auto IntHalfCmp = IntHalf <=> that; IntHalfCmp != 0)
        return IntHalfCmp;
      if (auto DecimalHalfCmp = DecimalHalf.Value <=> 0; DecimalHalfCmp != 0)
        return DecimalHalfCmp;
    }

    //Compare only as if in NormalType representation mode
    std::strong_ordering BasicIntComparison(const signed int& that) const
    {
      if (that < 0) {
        auto IntHalfCmp = 0 <=> 1;
        return IntHalfCmp;
      }
      else
      {
        if (auto IntHalfCmp = IntHalf <=> (unsigned int)that; IntHalfCmp != 0)
          return IntHalfCmp;
        if (auto DecimalHalfCmp = DecimalHalf.Value <=> 0; DecimalHalfCmp != 0)
          return DecimalHalfCmp;
      }
    }

public:

    std::strong_ordering operator<=>(const BinaryDec& that) const
    {//return BasicComparison(that);
      if (auto IntHalfCmp = IntHalf <=> that.IntHalf; IntHalfCmp != 0)
        return IntHalfCmp;
      if (auto DecimalHalfCmp = DecimalHalf.Value <=> that.DecimalHalf.Value; DecimalHalfCmp != 0)
        return DecimalHalfCmp;
    }

    std::strong_ordering operator<=>(const unsigned int& that) const
    {
      return BasicUIntComparison(that);
    }

    std::strong_ordering operator<=>(const signed int& that) const
    {
      return BasicIntComparison(that);
    }

    bool operator==(const BinaryDec& that) const
    {
      if (IntHalf!=that.IntHalf)
        return false;
      if (DecimalHalf!=that.DecimalHalf)
        return false;
      return true;
    }

    bool operator!=(const BinaryDec& that) const
    {
      if (IntHalf!=that.IntHalf)
        return true;
      if (DecimalHalf!=that.DecimalHalf)
        return true;
      return false;
    }

    bool operator==(const unsigned int& that) const
    {
      if (IntHalf!=that)
        return false;
      if (DecimalHalf!=0)
        return false;
      return true;
    }

    bool operator!=(const unsigned int& that) const
    {
      if (IntHalf!=that)
        return true;
      if (DecimalHalf!=0)
        return true;
      return false;
    }

    bool operator==(const signed int& that) const
    {
      if (IntHalf!=that)
        return false;
      if (DecimalHalf!=0)
        return false;
      return true;
    }

    bool operator!=(const signed int& that) const
    {
      if (IntHalf!=that)
        return true;
      if (DecimalHalf!=0)
        return true;
      return false;
    }

  #pragma endregion Comparison Operators

  #pragma region NormalRep Integer Division Operations
protected:

    template<IntegerType IntType=unsigned int>
    void unsigned intDivOpV1(const IntType& rValue)
    {//Avoid using with special status representations such as approaching zero or result will be incorrect
      unsigned _int64 SelfRes;
      unsigned _int64 Res;
      unsigned _int64 IntHalfRes;
      unsigned _int64 DecimalRes;
      SelfRes = DecimalHalf == 0? DecimalOverflowX * IntHalf: DecimalOverflowX * IntHalf + DecimalHalf.Value;
      Res = SelfRes / rValue;

      IntHalfRes = Res/DecimalOverflowX;
      DecimalRes = Res - DecimalOverflowX * IntHalfRes;
      IntHalf = (unsigned int)IntHalfRes;
      DecimalHalf.Value = (unsigned int)DecimalRes;
    }

public:
    void unsigned intDivOp(const unsigned int& rValue) { unsigned intDivOpV1(rValue); }
    void PartialUInt64DivOp(const unsigned int& rValue) { unsigned intDivOpV1(rValue); }

    void unsigned intDivOp(const signed int& rValue) { unsigned intDivOpV1(rValue); }
    void unsigned int64DivOp(const signed __int64& rValue) { unsigned intDivOpV1(rValue); }

protected:

    /// <summary>
    /// Basic division operation between BinaryDec Variant and unsigned Integer value
    /// that ignores special representation status
    /// (Doesn't modify owner object)
    /// </summary>
    /// <param name="rValue">The right side value</param>
    template<IntegerType IntType=unsigned int>
    void IntDivOpV1(const IntType& Value)
    {
      if (Value == 0)
      {
        throw "Target value can not be divided by zero";
      }
      else if (IsZero())
        return;
      unsigned intDivOpV1(Value);
      if (IntHalf == 0 && DecimalHalf == 0)
        DecimalHalf = 1;//Prevent Dividing into nothing
    }

    template<IntegerType IntType=unsigned int>
    auto& IntDivOperationV1(const IntType& rValue)
    { IntDivOpV1(rValue); return *this; }

    /// <summary>
    /// Basic division operation between BinaryDec Variant and unsigned Integer value
    /// that ignores special representation status
    /// (Doesn't modify owner object)
    /// </summary>
    /// <param name="rValue">The right side value</param>
    template<IntegerType IntType=unsigned int>
    const auto DivideByIntV1(const IntType& rValue)
    { auto self = *this; return self.IntDivOperationV1(rValue); }

public:

    void UIntDivOp(const unsigned int& rValue) { IntDivOpV1(rValue); }
    void IntDivOp(const signed int& rValue) { IntDivOpV1(rValue); }
    void UInt64DivOp(const unsigned __int64& rValue) { IntDivOpV1(rValue); }
    void Int64DivOp(const signed __int64& rValue) { IntDivOpV1(rValue); }

    void UInt8DivOp(const unsigned char& rValue) { IntDivOpV1(rValue); }
    void Int8DivOp(const signed char& rValue) { IntDivOpV1(rValue); }
    void UInt16DivOp(const unsigned short& rValue) { IntDivOpV1(rValue); }
    void Int16DivOp(const signed short& rValue) { IntDivOpV1(rValue); }

    BinaryDec& UIntDivOperation(const unsigned int& rValue) { return IntDivOperationV1(rValue); }
    BinaryDec& IntDivOperation(const signed int& rValue) { return IntDivOperationV1(rValue); }
    BinaryDec& UInt64DivOperation(const unsigned __int64& rValue) { return IntDivOperationV1(rValue); }
    BinaryDec& Int64DivOperation(const signed __int64& rValue) { return IntDivOperationV1(rValue); }
    BinaryDec& UInt8DivOperation(const unsigned char& rValue) { return IntDivOperationV1(rValue); }
    BinaryDec& Int8DivOperation(const signed char& rValue) { return IntDivOperationV1(rValue); }
    BinaryDec& UInt16DivOperation(const unsigned short& rValue) { return IntDivOperationV1(rValue); }
    BinaryDec& Int16DivOperation(const signed short& rValue) { return IntDivOperationV1(rValue); }

    const BinaryDec DivideByUInt(const unsigned int& rValue) { return DivideByIntV1(rValue); }
    const BinaryDec DivideByInt(const signed int& rValue) { return DivideByIntV1(rValue); }
    const BinaryDec DivideByUInt64(const unsigned __int64& rValue) { return DivideByIntV1(rValue); }
    const BinaryDec DivideByInt64(const signed __int64& rValue) { return DivideByIntV1(rValue); }

    const BinaryDec DivideByUInt8(const unsigned char& rValue) { return DivideByIntV1(rValue); }
    const BinaryDec DivideByInt8(const signed char& rValue) { return DivideByIntV1(rValue); }
    const BinaryDec DivideByUInt16(const unsigned short& rValue) { return DivideByIntV1(rValue); }
    const BinaryDec DivideByInt16(const signed short& rValue) { return DivideByIntV1(rValue); }

  #pragma endregion NormalRep Integer Division Operations

  #pragma region NormalRep AltNum Division Operations
protected:

    /// <summary>
    /// Basic unsigned division operation(main code block)
    /// Return true if divide into zero
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue.">The right side value</param>
    template<BinaryDecVariant VariantType=BinaryDec>
    bool PartialDivOpV1(const VariantType& rValue)
    {
      unsigned _int64 SelfRes = DecimalOverflowX * IntHalf + DecimalHalf.Value;
      unsigned _int64 ValueRes = DecimalOverflowX * rValue.IntHalf + rValue.DecimalHalf.Value;

      unsigned _int64 IntHalfRes = SelfRes / ValueRes;
      unsigned _int64 DecimalRes = SelfRes - ValueRes * IntHalfRes;
      IntHalf = (unsigned int) IntHalfRes;
      DecimalHalf.Value = DecimalRes;
      if (IntHalfRes == 0 && DecimalRes == 0)
        return true;
      else
        return false;
    }

    /// <summary>
    /// Unsigned division operation that ignores special decimal status
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue.">The right side value</param>
    template<BinaryDecVariant VariantType=BinaryDec>
    void DivOpV1(const VariantType& rValue)
    {
      if(DecimalHalf==0)
      {
        if(rValue.DecimalHalf==0)
        {
          switch(rValue.IntHalf)
          {
            case 2:
              if((IntHalf&1)==1)//Check if number is odd
                UIntDivOp(2);
              else
                IntHalf /= 2;
              break;
            case 4:
              if(((IntHalf >> 2) << 2) == IntHalf)
                IntHalf /= 4;
              else
                UIntDivOp(4);
              break;
            case 8:
              if(((IntHalf >> 3) << 3) == IntHalf)
                IntHalf /= 8;
              else
                UIntDivOp(4);
              break;
            case 16:
              if(((IntHalf >> 4) << 4) == IntHalf)
                IntHalf /= 16;
              else
                UIntDivOp(4);
              break;
            case 32:
              if(((IntHalf >> 5) << 5) == IntHalf)
                IntHalf /= 32;
              else
                UIntDivOp(4);
              break;
            case 0:
              throw "Target rValue can not be divided by zero";
              break;
            default:
              UIntDivOp(rValue.IntHalf);
              break;
          }
        }
#if !defined(AltNum_DisableDivideDownToNothingPrevention)
        else if (PartialDivOp(rValue))//Prevent Dividing into nothing
            DecimalHalf.Value = 1;
#else
        else
          PartialDivOp(rValue);
#endif
      }
#if !defined(AltNum_DisableDivideDownToNothingPrevention)
      else if (PartialDivOp(rValue))//Prevent Dividing into nothing
        DecimalHalf.Value = 1;
#else
      else
        PartialDivOp(rValue);
#endif
    }

    template<BinaryDecVariant VariantType = BinaryDec>
    static VariantType DivisionV1(VariantType lValue, const VariantType& rValue)
    {
      lValue.DivOpV1(rValue); return lValue;
    }

public:

    /// <summary>
    /// Basic unsigned division operation(main code block)
    /// Return true if divide into zero
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue.">The rValue</param>
    bool PartialDivOp(const BinaryDec& rValue){ return PartialDivOpV1(rValue); }

    /// <summary>
    /// Unsigned division operation that ignores special decimal status
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue.">The right side value</param>
    void DivOp(const BinaryDec& rValue){ DivOpV1(rValue); }

    /// <summary>
    /// Basic unsigned division operation that ignores special decimal status
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue.">The right side tValue</param>
    BinaryDec& DivOperation(const BinaryDec& rValue)
    { DivOp(rValue); return *this; }

    /// <summary>
    /// Basic unsigned division operation that ignores special decimal status
    /// (Doesn't modify owner object)
    /// </summary>
    /// <param name="rValue.">The right side tValue</param>
    BinaryDec DivideBy(const BinaryDec& rValue) const
    { BinaryDec lValue = *this; return lValue.DivOperation(rValue); }

  #pragma endregion NormalRep AltNum Division Operations

  #pragma region Other Division Operations

    /// <summary>
    /// Simplified division by 2 operation(to reduce cost of operations)
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue.">The right side value</param>
    /// <returns>BinaryDec&</returns>
    void DivideByTwo();

    /// <summary>
    /// Simplified division by 4 operation(to reduce cost of operations)
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue.">The right side value</param>
    /// <returns>BinaryDec&</returns>
    void DivideByFour();

    //Return copy of result divided by two
    BinaryDec DividedByTwo() const;

    //Return copy of result divided by four
    BinaryDec DividedByFour() const;

    /// <summary>
    /// /= operation
    /// </summary>
    /// <param name="lValue">The left side value</param>
    /// <param name="rValue">The right side value.</param>
    /// <returns>BinaryDec</returns>
    friend BinaryDec& operator/=(BinaryDec& lValue, const BinaryDec& rValue) { return lValue.DivOperation(rValue); }

    /// <summary>
    /// *= operation between BinaryDec variant and Integer rValue.
    /// </summary>
    /// <param name="lValue">The left side value</param>
    /// <param name="rValue">The right side value.</param>
    /// <returns>BinaryDec</returns>
    friend BinaryDec& operator/=(BinaryDec& lValue, const signed int& rValue) { return lValue.IntDivOperation(rValue); }
    friend BinaryDec& operator/=(BinaryDec& lValue, const signed __int64& rValue) { return lValue.Int64DivOperation(rValue); }
    friend BinaryDec& operator/=(BinaryDec& lValue, const unsigned int& rValue) { return lValue.UIntDivOperation(rValue); }
    friend BinaryDec& operator/=(BinaryDec& lValue, const unsigned __int64& rValue) { return lValue.UInt64DivOperation(rValue); }

    friend BinaryDec& operator/=(BinaryDec& lValue, const signed char& rValue) { return lValue.Int8DivOperation(rValue); }
    friend BinaryDec& operator/=(BinaryDec& lValue, const signed short& rValue) { return lValue.Int16DivOperation(rValue); }
    friend BinaryDec& operator/=(BinaryDec& lValue, const unsigned char& rValue) { return lValue.UInt8DivOperation(rValue); }
    friend BinaryDec& operator/=(BinaryDec& lValue, const unsigned short& rValue) { return lValue.UInt16DivOperation(rValue); }

    /// <summary>
    /// Division operation
    /// </summary>
    /// <param name="lValue">The left side value</param>
    /// <param name="rValue">The right side value.</param>
    /// <returns>BinaryDec</returns>
    friend BinaryDec operator/(BinaryDec lValue, const BinaryDec& rValue) { return lValue.DivideBy(rValue); }

    /// <summary>
    /// Division operation between BinaryDec variant and Integer rValue.
    /// </summary>
    /// <param name="lValue">The left side value</param>
    /// <param name="rValue">The right side value.</param>
    /// <returns>BinaryDec</returns>
    friend BinaryDec operator/(BinaryDec lValue, const signed int& rValue) { return lValue.IntDivOperation(rValue); }
    friend BinaryDec operator/(BinaryDec lValue, const signed __int64& rValue) { return lValue.Int64DivOperation(rValue); }
    friend BinaryDec operator/(BinaryDec lValue, const unsigned int& rValue) { return lValue.UIntDivOperation(rValue); }
    friend BinaryDec operator/(BinaryDec lValue, const unsigned __int64& rValue) { return lValue.UInt64DivOperation(rValue); }

    friend BinaryDec operator/(BinaryDec lValue, const signed char& rValue) { return lValue.Int8DivOperation(rValue); }
    friend BinaryDec operator/(BinaryDec lValue, const signed short& rValue) { return lValue.Int16DivOperation(rValue); }
    friend BinaryDec operator/(BinaryDec lValue, const unsigned char& rValue) { return lValue.UInt8DivOperation(rValue); }
    friend BinaryDec operator/(BinaryDec lValue, const unsigned short& rValue) { return lValue.UInt16DivOperation(rValue); }

    friend BinaryDec operator/(const signed int& lValue, const BinaryDec& rValue) { return ((BinaryDec)lValue).DivideBy(rValue); }
    friend BinaryDec operator/(const signed __int64& lValue, const BinaryDec& rValue) { return ((BinaryDec)lValue).DivideBy(rValue); }
    friend BinaryDec operator/(const unsigned int& lValue, const BinaryDec& rValue) { return ((BinaryDec)lValue).DivideBy(rValue); }
    friend BinaryDec operator/(const unsigned __int64& lValue, const BinaryDec& rValue) { return ((BinaryDec)lValue).DivideBy(rValue); }

    friend BinaryDec operator/(const signed char& lValue, const BinaryDec& rValue) { return ((BinaryDec)lValue).DivideBy(rValue); }
    friend BinaryDec operator/(const signed short& lValue, const BinaryDec& rValue) { return ((BinaryDec)lValue).DivideBy(rValue); }
    friend BinaryDec operator/(const unsigned char& lValue, const BinaryDec& rValue) { return ((BinaryDec)lValue).DivideBy(rValue); }
    friend BinaryDec operator/(const unsigned short& lValue, const BinaryDec& rValue) { return ((BinaryDec)lValue).DivideBy(rValue); }

  #pragma endregion Other Division Operations

  #pragma region NormalRep Integer Multiplication Operations
protected:

    /// <summary>
    /// Partial version of UIntMultOpV1 without zero checks
    /// (Modifies owner object)
    /// </summary>
    /// <param name="Value">The value.</param>
    /// <returns>BinaryDec</returns>
    template<IntegerType IntType=int>
    void PartialUIntMultOpV1(const IntType& rValue)
    {
      if (DecimalHalf == 0)
        IntHalf *= rValue;
      else
      {
        unsigned __int64 SRep = IntHalf == 0 ? DecimalHalf.Value : DecimalOverflowX * IntHalf + DecimalHalf.Value;
        SRep *= rValue;
        if (SRep >= DecimalOverflowX)
        {
          unsigned __int64 OverflowVal = SRep / DecimalOverflowX;
          SRep -= OverflowVal * DecimalOverflowX;
          IntHalf = (unsigned int)OverflowVal;
          DecimalHalf.Value = (unsigned int)SRep;
        }
        else
        {
          IntHalf = 0;
          DecimalHalf.Value = (unsigned int)SRep;
        }
      }
    }

    //Partial version of BasicIntMultOpV1 without zero checks
    //Modifies owner object
    template<IntegerType IntType = signed int>
    void unsigned intMultOpV1(const IntType& rValue)
    {
      if (rValue < 0)
        throw "Returns unsupported negative number.";
      else
        PartialUIntMultOp(rValue);
    }

public:

    void PartialUIntMultOp(const unsigned int& rValue) { PartialUIntMultOpV1(rValue); }
    void unsigned intMultOp(const signed int& rValue) { unsigned intMultOpV1(rValue); }
    void PartialUInt64MultOp(const unsigned __int64& rValue) { PartialUIntMultOpV1(rValue); }
    void unsigned int64MultOp(const signed __int64& rValue) { unsigned intMultOpV1(rValue); }

    void Unsignedunsigned intMultOp(const signed int& rValue) { PartialUIntMultOpV1(rValue); }
    void Unsignedunsigned int64MultOp(const signed __int64& rValue) { PartialUIntMultOpV1(rValue); }

protected:
    template<IntegerType IntType=signed int>
    void UIntMultOpV1(const IntType& rValue)
    {
      if (rValue == 0)
      {
        SetAsZero();
        return;
      }
      else if (IsZero())
        return;
      PartialUIntMultOpV1(rValue);
    }

    template<IntegerType IntType=signed int>
    void IntMultOpV1(const IntType& Value)
    {
      if (Value == 0)
      {
        SetAsZero();
        return;
      }
      else if (IsZero())
        return;
      unsigned intMultOpV1(Value);
    }

    template<IntegerType IntType=unsigned int>
    auto& UIntMultOperationV1(const IntType& Value)
    { UIntMultOpV1(Value); return *this; }

    template<IntegerType IntType=unsigned int>
    auto& IntMultOperationV1(const IntType& Value)
    { IntMultOpV1(Value); return *this; }

    /// <summary>
    /// Basic multiplication operation between BinaryDec variant and unsigned Integer value
    /// that ignores special representation status
    /// (Doesn't modify owner object)
    /// </summary>
    /// <param name="rValue">The right side value</param>
    template<IntegerType IntType=unsigned int>
    const BinaryDec MultiplyByUIntV1(const IntType& rValue)
    { auto self = *this; return self.UIntMultOperationV1(rValue); }

    /// <summary>
    /// Basic multiplication operation between BinaryDec variant and Integer value
    /// that ignores special representation status
    /// (Doesn't modify owner object)
    /// </summary>
    /// <param name="rValue">The right side value</param>
    template<IntegerType IntType=signed int>
    const BinaryDec MultiplyByIntV1(const IntType& rValue)
    { auto self = *this; return self.IntMultOperationV1(rValue); }

    template<BinaryDecVariant VariantType=BinaryDec, IntegerType IntType=unsigned int>
    static VariantType UIntMultiplicationV1(VariantType lValue, const IntType& rValue)
    { return lValue.UIntMultOperationV1(rValue); }

public:

    void UIntMultOp(const unsigned int& rValue) { UIntMultOpV1(rValue); }
    void IntMultOp(const signed int& rValue) { IntMultOpV1(rValue); }
    void UInt64MultOp(const unsigned __int64& rValue) { UIntMultOpV1(rValue); }
    void Int64MultOp(const signed __int64& rValue) { IntMultOpV1(rValue); }

    void UnsignedIntegerMultOp(const signed int& rValue) { UIntMultOpV1(rValue); }

    void UInt8MultOp(const unsigned char& rValue) { UIntMultOpV1(rValue); }
    void Int8MultOp(const signed char& rValue) { IntMultOpV1(rValue); }
    void UInt16MultOp(const unsigned short& rValue) { UIntMultOpV1(rValue); }
    void Int16MultOp(const signed short& rValue) { IntMultOpV1(rValue); }

    BinaryDec& UIntMultOperation(const unsigned int& rValue) { return UIntMultOperationV1(rValue); }
    BinaryDec& IntMultOperation(const signed int& rValue) { return IntMultOperationV1(rValue); }
    BinaryDec& UInt64MultOperation(const unsigned __int64& rValue) { return UIntMultOperationV1(rValue); }
    BinaryDec& Int64MultOperation(const signed __int64& rValue) { return IntMultOperationV1(rValue); }
    BinaryDec& UInt8MultOperation(const unsigned char& rValue) { return UIntMultOperationV1(rValue); }
    BinaryDec& Int8MultOperation(const signed char& rValue) { return IntMultOperationV1(rValue); }
    BinaryDec& UInt16MultOperation(const unsigned short& rValue) { return UIntMultOperationV1(rValue); }
    BinaryDec& Int16MultOperation(const signed short& rValue) { return IntMultOperationV1(rValue); }

    const BinaryDec MultiplyByUInt(const unsigned int& rValue) { return MultiplyByUIntV1(rValue); }
    const BinaryDec MultiplyByInt(const signed int& rValue) { return MultiplyByIntV1(rValue); }
    const BinaryDec MultiplyByUInt64(const unsigned __int64& rValue) { return MultiplyByUIntV1(rValue); }
    const BinaryDec MultiplyByInt64(const signed __int64& rValue) { return MultiplyByIntV1(rValue); }

    const BinaryDec UnsignedMultiplyByInt(const signed int& rValue) { return MultiplyByUIntV1(rValue); }
    const BinaryDec UnsignedMultiplyByInt64(const signed __int64& rValue) { return MultiplyByUIntV1(rValue); }

    const BinaryDec MultiplyByUInt8(const unsigned char& rValue) { return MultiplyByUIntV1(rValue); }
    const BinaryDec MultiplyByInt8(const signed char& rValue) { return MultiplyByIntV1(rValue); }
    const BinaryDec MultiplyByUInt16(const unsigned short& rValue) { return MultiplyByUIntV1(rValue); }
    const BinaryDec MultiplyByInt16(const signed short& rValue) { return MultiplyByIntV1(rValue); }

  #pragma endregion NormalRep Integer Multiplication Operations

  #pragma region NormalRep AltNum Multiplication Operations
protected:

    /// <summary>
    /// Basic multiplication operation that ignores special decimal status with unsigned BinaryDec
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue.">The right side tValue</param>
    template<BinaryDecVariant VariantType=BinaryDec>
    void MultOpV1(const VariantType& rValue)
    {
      if (DecimalHalf == 0)
      {
        if (IntHalf == 1)
        {
          IntHalf = rValue.IntHalf;
          DecimalHalf = rValue.DecimalHalf;
        }
        else if (rValue.DecimalHalf == 0)
          IntHalf *= rValue.IntHalf;
        else {
          __int64 rRep = rValue.IntHalf == 0 ? rValue.DecimalHalf.Value : DecimalOverflowX * rValue.IntHalf + rValue.DecimalHalf.Value;
          if (rRep >= DecimalOverflowX)
          {
            __int64 OverflowVal = rRep / DecimalOverflowX;
            rRep -= OverflowVal * DecimalOverflowX;
            IntHalf = (unsigned int)OverflowVal;
            DecimalHalf.Value = (unsigned int)rRep;
            return;
          }
          else
          {
            DecimalHalf = (signed int)rRep;
          #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
            if(DecimalHalf==0)
              DecimalHalf.Value = 1;
          #endif
            IntHalf = 0;
            return;
          }
        }
      }
      else if (IntHalf == 0)
      {
        __int64 SRep = (__int64)DecimalHalf;
        SRep *= rValue.DecimalHalf.Value;
        SRep /= DecimalOverflowX;
        if (rValue.IntHalf == 0)
        {
          DecimalHalf = (signed int)SRep;
        #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
          if (DecimalHalf == 0)
            DecimalHalf.Value = 1;
        #endif
          return;
        }
        else
        {
          SRep += (__int64)DecimalHalf.Value * rValue.IntHalf;
          if (SRep >= DecimalOverflowX)
          {
            __int64 OverflowVal = SRep / DecimalOverflowX;
            SRep -= OverflowVal * DecimalOverflowX;
            IntHalf = OverflowVal;
            DecimalHalf.Value = (signed int)SRep;
            return;
          }
          else
          {
            DecimalHalf.Value = (unsigned int)SRep;
        #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
            if(DecimalHalf==0)
              DecimalHalf.Value = 1;
        #endif
            return;
          }
        }
      }
      else
      {
        if (rValue.DecimalHalf == 0)//Y is integer
        {
          __int64 SRep = DecimalOverflowX * IntHalf + DecimalHalf.Value;
          SRep *= rValue.IntHalf;
          if (SRep >= DecimalOverflowX)
          {
            __int64 OverflowVal = SRep / DecimalOverflowX;
            SRep -= OverflowVal * DecimalOverflowX;
            IntHalf = (unsigned int)OverflowVal;
            DecimalHalf.Value = (unsigned int)SRep;
          }
          else
          {
            DecimalHalf.Value = (unsigned int)SRep;
            if(DecimalHalf==0)
            {
        #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
              if(DecimalHalf==0)
                DecimalHalf.Value = 1;
        #endif
            }
            IntHalf = 0;
          }
          return;
        }
        else if (rValue.IntHalf == 0)
        {
          __int64 SRep = DecimalOverflowX * IntHalf + DecimalHalf.Value;
          SRep *= rValue.DecimalHalf.Value;
          SRep /= DecimalOverflowX;
          if (SRep >= DecimalOverflowX)
          {
            __int64 OverflowVal = SRep / DecimalOverflowX;
            SRep -= OverflowVal * DecimalOverflowX;
            IntHalf = (unsigned int)OverflowVal;
            DecimalHalf.Value = (unsigned int)SRep;
          }
          else
          {
            DecimalHalf.Value = (unsigned int)SRep;
            if(DecimalHalf==0)
            {
        #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
              if(DecimalHalf==0)
                DecimalHalf.Value = 1;
        #endif
            }
            IntHalf = 0;
          }
          return;
        }
        else
        {
          //X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
          unsigned __int64 SRep = IntHalf == 0 ? DecimalHalf.Value : DecimalOverflowX * IntHalf + DecimalHalf.Value;
          SRep *= rValue.IntHalf;//SRep holds __int64 version of X.Y * Z
          //X.Y *.V
          unsigned __int64 Temp03 = (__int64)(rValue.DecimalHalf * IntHalf);//Temp03 holds __int64 version of X *.V
          unsigned __int64 Temp04 = (__int64)DecimalHalf.Value * (__int64)rValue.DecimalHalf.Value;
          Temp04 /= DecimalOverflow;
          //Temp04 holds __int64 version of .Y * .V
          unsigned __int64 IntegerRep = SRep + Temp03 + Temp04;
          unsigned __int64 intHalf = IntegerRep / DecimalOverflow;
          IntegerRep -= intHalf * DecimalOverflowX;
          IntHalf = (unsigned int) intHalf;
          DecimalHalf.Value = (unsigned int)IntegerRep;
        }
      }
      #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
      if(DecimalHalf==0&&IntHalf==0)
        DecimalHalf.Value = 1;
      #endif
    }

    template<BinaryDecVariant VariantType=BinaryDec>
    static VariantType MultiplicationV1(VariantType lValue, const VariantType& rValue)
    {
      lValue.MultOpV1(rValue); return lValue;
    }

public:

    /// <summary>
    /// Basic multiplication operation that ignores special decimal status with unsigned BinaryDec
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue.">The right side tValue</param>
    void MultOp(const BinaryDec& rValue){ MultOpV1(rValue); }

    /// <summary>
    /// Basic unsigned multiplication operation that ignores special decimal status
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue.">The right side tValue</param>
    BinaryDec& MultOperation(const BinaryDec& rValue)
    { MultOp(rValue); return *this; }

    /// <summary>
    /// Basic unsigned multiplication operation that ignores special decimal status
    /// (Doesn't modify owner object)
    /// </summary>
    /// <param name="rValue.">The right side tValue</param>
    BinaryDec MultiplyBy(const BinaryDec& rValue) const
    { BinaryDec lValue = *this; return lValue.MultOperation(rValue); }

  #pragma endregion NormalRep AltNum Multiplication Operations

  #pragma region Other multiplication operations

    /// <summary>
    /// Simplified multiplication by 2 operation(to reduce cost of operations)
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue.">The right side value</param>
    /// <returns>void</returns>
    void MultiplyByTwo();

    /// <summary>
    /// Simplified multiplication by 4 operation(to reduce cost of operations)
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue.">The right side value</param>
    /// <returns>void</returns>
    void MultiplyByFour();

    //Return copy of result divided by two
    BinaryDec MultipliedByTwo() const;

    //Return copy of result divided by four
    BinaryDec MultipliedByFour() const;

    /// <summary>
    /// *= operation
    /// </summary>
    /// <param name="lValue">The left side value</param>
    /// <param name="rValue">The right side value.</param>
    /// <returns>BinaryDec</returns>
    friend BinaryDec& operator*=(BinaryDec& lValue, const BinaryDec& rValue) { return lValue.MultOperation(rValue); }

    /// <summary>
    /// *= operation between BinaryDec variant and Integer rValue.
    /// </summary>
    /// <param name="lValue">The left side value</param>
    /// <param name="rValue">The right side value.</param>
    friend BinaryDec& operator*=(BinaryDec& lValue, const signed int& rValue) { return lValue.IntMultOperation(rValue); }
    friend BinaryDec& operator*=(BinaryDec& lValue, const signed __int64& rValue) { return lValue.Int64MultOperation(rValue); }
    friend BinaryDec& operator*=(BinaryDec& lValue, const unsigned int& rValue) { return lValue.UIntMultOperation(rValue); }
    friend BinaryDec& operator*=(BinaryDec& lValue, const unsigned __int64& rValue) { return lValue.UInt64MultOperation(rValue); }

    friend BinaryDec& operator*=(BinaryDec& lValue, const signed char& rValue) { return lValue.Int8MultOperation(rValue); }
    friend BinaryDec& operator*=(BinaryDec& lValue, const signed short& rValue) { return lValue.Int16MultOperation(rValue); }
    friend BinaryDec& operator*=(BinaryDec& lValue, const unsigned char& rValue) { return lValue.UInt8MultOperation(rValue); }
    friend BinaryDec& operator*=(BinaryDec& lValue, const unsigned short& rValue) { return lValue.UInt16MultOperation(rValue); }

    /// <summary>
    /// Multiplication operation
    /// </summary>
    /// <param name="lValue">The left side value</param>
    /// <param name="rValue">The right side value.</param>
    /// <returns>BinaryDec</returns>
    friend BinaryDec operator*(BinaryDec lValue, const BinaryDec& rValue) { return lValue.MultOperation(rValue); }

    /// <summary>
    /// Multiplication operation between BinaryDec variant and Integer rValue.
    /// </summary>
    /// <param name="self">The left side value</param>
    /// <param name="rValue">The right side value.</param>
    friend BinaryDec operator*(BinaryDec lValue, const signed int& rValue) { return lValue.IntMultOperation(rValue); }
    friend BinaryDec operator*(BinaryDec lValue, const signed __int64& rValue) { return lValue.Int64MultOperation(rValue); }
    friend BinaryDec operator*(BinaryDec lValue, const unsigned int& rValue) { return lValue.UIntMultOperation(rValue); }
    friend BinaryDec operator*(BinaryDec lValue, const unsigned __int64& rValue) { return lValue.UInt64MultOperation(rValue); }

    friend BinaryDec operator*(BinaryDec lValue, const signed char& rValue) { return lValue.Int8MultOperation(rValue); }
    friend BinaryDec operator*(BinaryDec lValue, const signed short& rValue) { return lValue.Int16MultOperation(rValue); }
    friend BinaryDec operator*(BinaryDec lValue, const unsigned char& rValue) { return lValue.UInt8MultOperation(rValue); }
    friend BinaryDec operator*(BinaryDec lValue, const unsigned short& rValue) { return lValue.UInt16MultOperation(rValue); }

    friend BinaryDec operator*(signed int lValue, BinaryDec rValue)  { return rValue.IntMultOperation(lValue); }
    friend BinaryDec operator*(signed __int64 lValue, BinaryDec& rValue)  { return rValue.Int64MultOperation(lValue); }
    friend BinaryDec operator*(unsigned int lValue, BinaryDec& rValue)  { return rValue.UIntMultOperation(lValue); }
    friend BinaryDec operator*(unsigned __int64 lValue, BinaryDec& rValue)  { return rValue.UInt64MultOperation(lValue); }

    friend BinaryDec operator*(signed char lValue, BinaryDec& rValue)  { return rValue.Int8MultOperation(lValue); }
    friend BinaryDec operator*(signed short lValue, BinaryDec& rValue)  { return rValue.Int16MultOperation(lValue); }
    friend BinaryDec operator*(unsigned char lValue, BinaryDec& rValue)  { return rValue.UInt8MultOperation(lValue); }
    friend BinaryDec operator*(unsigned short lValue, BinaryDec& rValue)  { return rValue.UInt16MultOperation(lValue); }

  #pragma endregion Other multiplication operations

  #pragma region NormalRep Integer Addition Operations
protected:

    /// <summary>
    /// Basic addition operation between variant and unsigned Integer value
    /// that ignores special representation status
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue">The right side value</param>
    template<IntegerType IntType=unsigned int>
    void UIntAddOpV1(const IntType& rValue)
    {
      IntHalf += rValue;
    }

    /// <summary>
    /// Basic addition operation between variant and Integer value
    /// that ignores special representation status
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue">The right side value</param>
    template<IntegerType IntType=signed int>
    void IntAddOpV1(const IntType& rValue)
    {
      if (rValue < 0) {
        if ((unsigned int)(-rValue) > IntHalf)
          throw("Underflow error into non-supported negative number.");
        IntHalf += rValue;
      }
      else
        IntHalf += rValue;
    }

    template<IntegerType IntType=unsigned int>
    auto& UIntAddOperationV1(const IntType& rValue)
    { UIntAddOpV1(rValue); return *this; }

    template<IntegerType IntType=signed int>
    auto& IntAddOperationV1(const IntType& rValue)
    { IntAddOpV1(rValue); return *this; }

    /// <summary>
    ///  addition operation between variant and unsigned Integer value
    /// that ignores special representation status
    /// (Doesn't modify owner object)
    /// </summary>
    /// <param name="rValue">The right side value</param>
    template<IntegerType IntType=unsigned int>
    auto AddByUIntV1(const IntType& rValue) const
    { auto self = *this; return self.UIntAddOperationV1(rValue); }

    /// <summary>
    /// Basic addition operation between variant and Integer value
    /// that ignores special representation status
    /// (Doesn't modify owner object)
    /// </summary>
    /// <param name="rValue">The right side value</param>
    template<IntegerType IntType=signed int>
    auto AddByIntV1(const IntType& rValue) const
    { auto self = *this; return self.IntAddOperationV1(rValue); }

public:

    void UInt8AddOp(const unsigned char& rValue) { UIntAddOpV1(rValue); }
    void UInt16AddOp(const unsigned short& rValue) { UIntAddOpV1(rValue); }
    void UIntAddOp(const unsigned int& rValue) { UIntAddOpV1(rValue); }
    void UInt64AddOp(const unsigned __int64& rValue) { UIntAddOpV1(rValue); }

    BinaryDec& UInt8AddOperation(const unsigned char& rValue) { return UIntAddOperationV1(rValue); }
    BinaryDec& UInt16AddOperation(const unsigned short& rValue) { return UIntAddOperationV1(rValue); }
    BinaryDec& UIntAddOperation(const unsigned int& rValue) { return UIntAddOperationV1(rValue); }
    BinaryDec& UInt64AddOperation(const unsigned __int64& rValue) { return UIntAddOperationV1(rValue); }

    BinaryDec AddByUInt8(const unsigned char& rValue) { return AddByUIntV1(rValue); }
    BinaryDec AddByUInt16(const unsigned short& rValue) { return AddByUIntV1(rValue); }
    BinaryDec AddByUInt(const unsigned int& rValue) { return AddByUIntV1(rValue); }
    BinaryDec AddByUInt64(const unsigned __int64& rValue) { return AddByUIntV1(rValue); }

    void Int8AddOp(const signed char& rValue) { IntAddOpV1(rValue); }
    void Int16AddOp(const signed short& rValue) { IntAddOpV1(rValue); }
    void IntAddOp(const signed int& rValue) { IntAddOpV1(rValue); }
    void Int64AddOp(const signed __int64& rValue) { IntAddOpV1(rValue); }

    BinaryDec& Int8AddOperation(const signed char& rValue) { return IntAddOperationV1(rValue); }
    BinaryDec& Int16AddOperation(const signed short& rValue) { return IntAddOperationV1(rValue); }
    BinaryDec& IntAddOperation(const signed int& rValue) { return IntAddOperationV1(rValue); }
    BinaryDec& Int64AddOperation(const signed __int64& rValue) { return IntAddOperationV1(rValue); }

    BinaryDec AddByInt8(const signed char rValue) { return AddByIntV1(rValue); }
    BinaryDec AddByInt16(const signed short& rValue) { return AddByIntV1(rValue); }
    BinaryDec AddByInt(const signed int& rValue) { return AddByIntV1(rValue); }
    BinaryDec AddByInt64(const signed __int64& rValue) { return AddByIntV1(rValue); }

  #pragma endregion NormalRep Integer Addition Operations

  #pragma region NormalRep Integer Subtraction Operations
protected:

    /// <summary>
    /// Basic Subtraction operation between variant and unsigned Integer value
    /// that ignores special representation status
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue">The right side value</param>
    template<IntegerType IntType=unsigned int>
    void UIntSubOpV1(const IntType& rValue)
    {
      if(rValue>IntHalf)
        throw("Underflow error into non-existent negative number.");
      IntHalf -= rValue;
    }

    /// <summary>
    /// Basic Subtraction operation between variant and Integer value
    /// that ignores special representation status
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue">The right side value</param>
    template<IntegerType IntType=signed int>
    void IntSubOpV1(const IntType& rValue)
    {
      if(rValue>(signed IntType)IntHalf)
        throw("Underflow error into non-existent negative number.");
      IntHalf -= rValue;
    }

    template<IntegerType IntType=unsigned int>
    auto& UIntSubOperationV1(const IntType& rValue)
    { UIntSubOpV1(rValue); return *this; }

    template<IntegerType IntType=signed int>
    auto& IntSubOperationV1(const IntType& rValue)
    { IntSubOpV1(rValue); return *this; }

    /// <summary>
    /// Basic Subtraction operation between variant and unsigned Integer value
    /// that ignores special representation status
    /// (Doesn't modify owner object)
    /// </summary>
    /// <param name="rValue">The right side value</param>
    template<IntegerType IntType=unsigned int>
    auto SubtractByUIntV1(const IntType& rValue)
    { auto self = *this; return self.UIntSubOperationV1(rValue); }

    /// <summary>
    /// Basic Subtraction operation between variant and Integer value
    /// that ignores special representation status
    /// (Doesn't modify owner object)
    /// </summary>
    /// <param name="rValue">The right side value</param>
    template<IntegerType IntType=signed int>
    auto SubtractByIntV1(const IntType& rValue)
    { auto self = *this; return self.IntSubOperationV1(rValue); }

public:

    void UInt8SubOp(const unsigned char& rValue) { UIntSubOpV1(rValue); }
    /// <summary>
    /// Basic Subtraction operation between variant and unsigned Integer value
    /// that ignores special representation status
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue">The right side value</param>
    void UIntSubOp(const unsigned int& rValue) { UIntSubOpV1(rValue); }
    void UInt16SubOp(const unsigned short& rValue) { UIntSubOpV1(rValue); }
    void UInt64SubOp(const unsigned __int64& rValue) { UIntSubOpV1(rValue); }

    void Int8SubOp(const signed char& rValue) { IntSubOpV1(rValue); }
    void IntSubOp(const signed int& rValue) { IntSubOpV1(rValue); }
    void Int16SubOp(const signed short& rValue) { IntSubOpV1(rValue); }
    void Int64SubOp(const signed __int64& rValue) { IntSubOpV1(rValue); }

    BinaryDec& UInt8SubOperation(const unsigned char& rValue) { return UIntSubOperationV1(rValue); }
    BinaryDec& UInt16SubOperation(const unsigned short& rValue) { return UIntSubOperationV1(rValue); }
    BinaryDec& UIntSubOperation(const unsigned int& rValue) { return UIntSubOperationV1(rValue); }
    BinaryDec& UInt64SubOperation(const unsigned __int64& rValue) { return UIntSubOperationV1(rValue); }

    BinaryDec SubtractByUInt8(const unsigned char& rValue) { return SubtractByUIntV1(rValue); }
    BinaryDec SubtractByUInt16(const unsigned short& rValue) { return SubtractByUIntV1(rValue); }
    BinaryDec SubtractByUInt(const unsigned int& rValue) { return SubtractByUIntV1(rValue); }
    BinaryDec SubtractByUInt64(const unsigned __int64& rValue) { return SubtractByUIntV1(rValue); }

    BinaryDec& Int8SubOperation(const signed char& rValue) { return IntSubOperationV1(rValue); }
    BinaryDec& Int16SubOperation(const signed short& rValue) { return IntSubOperationV1(rValue); }
    BinaryDec& IntSubOperation(const signed int& rValue) { return IntSubOperationV1(rValue); }
    BinaryDec& Int64SubOperation(const signed __int64& rValue) { return IntSubOperationV1(rValue); }

    BinaryDec SubtractByInt8(const signed char rValue) { return SubtractByIntV1(rValue); }
    BinaryDec SubtractByInt16(const signed short& rValue) { return SubtractByIntV1(rValue); }
    BinaryDec SubtractByInt(const signed int& rValue) { return SubtractByIntV1(rValue); }
    BinaryDec SubtractByInt64(const signed __int64& rValue) { return SubtractByIntV1(rValue); }

  #pragma endregion NormalRep Integer Subtraction Operations

  #pragma region NormalRep AltNum Addition/Subtraction Operations
protected:

    /// <summary>
    /// Basic addition Operation
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue.">The right side rValue</param>
    template<BinaryDecVariant VariantType = BinaryDec>
    void AddOpV1(const VariantType& rValue)
    {
      if (rValue.DecimalHalf == 0)
        IntHalf += rValue.IntHalf;
      else
      {
        IntHalf += rValue.IntHalf;

        unsigned int decResult = DecimalHalf.Value + rValue.DecimalHalf.Value;
        if (decResult == DecimalOverflow) {//5.4 + 4.6
          ++IntHalf;
          if (IntHalf == 0)
            SetAsZero();
          else
            DecimalHalf.Value = 0;
        }
        else if (decResult > DecimalOverflow) {//5.4 + 4.7
          ++IntHalf;
          DecimalHalf.Value = decResult - DecimalOverflow;
        }
        else if (signBeforeOp != IntHalf.Sign)
          DecimalHalf.Value = DecimalOverflow - decResult;
        else
          DecimalHalf.Value = decResult;
      }
    }

    /// <summary>
    /// Basic subtraction Operation
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue.">The right side rValue</param>
    template<BinaryDecVariant VariantType = BinaryDec>
    void SubOpV1(const VariantType& rValue)
    {
      if (rValue.DecimalHalf == 0)
        IntHalf -= rValue.IntHalf;
      else {
        IntHalf -= rValue.IntHalf;
        //5.XX - B
        if (DecimalHalf.Value == rValue.DecimalHalf.Value) {//5.5 - 5.5 = 10
          if (IntHalf == 0)
            SetAsZero();
          else
            DecimalHalf.Value = 0;
        }
        else if (rValue.DecimalHalf.Value > DecimalHalf.Value) {
          --IntHalf;
          if (signBeforeOp != IntHalf.Sign)//5.4 - 5.7 = -0.3
            DecimalHalf.Value = rValue.DecimalHalf.Value - DecimalHalf.Value;
          else//5.4 - 3.6 = 1.8
            DecimalHalf.Value = DecimalOverflow + DecimalHalf.Value - rValue.DecimalHalf.Value;
        }
        else if (signBeforeOp != IntHalf.Sign)//5.3 - 7.2 = -1.9
          DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value + rValue.DecimalHalf.Value;
        else//5.4 - 5.3 = 0.1
          DecimalHalf.Value -= rValue.DecimalHalf.Value;
      }
    }

    template<BinaryDecVariant VariantType=BinaryDec>
    static VariantType AdditionV1(VariantType lValue, const VariantType& rValue)
    {
      lValue.AddOpV1(rValue); return lValue;
    }

    template<BinaryDecVariant VariantType=BinaryDec>
    static VariantType SubtractionV1(VariantType lValue, const VariantType& rValue)
    {
      lValue.SubOpV1(rValue); return lValue;
    }

public:

    /// <summary>
    /// Basic addition Operation
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue.">The right side rValue</param>
    void AddOp(const BinaryDec& rValue){ AddOpV1(rValue); }

    /// <summary>
    /// Basic subtraction Operation
    /// (Modifies owner object)
    /// </summary>
    /// <param name="rValue.">The right side rValue</param>
    void SubOp(const BinaryDec& rValue){ SubOpV1(rValue); }

    //Basic addition operation
    BinaryDec& AddOperation(const BinaryDec& rValue)
    { AddOp(rValue); return *this; }

    /// <summary>
    /// Unsigned Addition operation that ignores special decimal status
    /// (Doesn't modify owner object)
    /// </summary>
    /// <param name="rValue.">The right side tValue</param>
    BinaryDec AddBy(const BinaryDec& rValue)
    { BinaryDec lValue = *this; return lValue.AddOperation(rValue); } const

    //Basic subtraction operation
    BinaryDec& SubOperation(const BinaryDec& rValue)
    { SubOp(rValue); return *this; }

    /// <summary>
    /// Basic unsigned Subtraction operation that ignores special decimal status
    /// (Doesn't modify owner object)
    /// </summary>
    /// <param name="rValue.">The right side tValue</param>
    BinaryDec SubtractBy(const BinaryDec& rValue)
    { BinaryDec lValue = *this; return lValue.SubOperation(rValue); } const

  #pragma endregion NormalRep AltNum Addition/Subtraction Operations

  #pragma region Other addition operations

    /// <summary>
    /// += operation
    /// </summary>
    /// <param name="lValue">The left side value</param>
    /// <param name="rValue">The right side value.</param>
    /// <returns>BinaryDec</returns>
    friend BinaryDec& operator+=(BinaryDec& lValue, const BinaryDec& rValue) { lValue.AddOp(rValue); return *lValue; }

    /// <summary>
    /// += operation between BinaryDec variant and Integer rValue.
    /// </summary>
    /// <param name="lValue">The left side value</param>
    /// <param name="rValue">The right side value.</param>
    friend BinaryDec& operator+=(BinaryDec& lValue, const signed int& rValue) { return lValue.IntAddOperation(rValue); }
    friend BinaryDec& operator+=(BinaryDec& lValue, const signed __int64& rValue) { return lValue.Int64AddOperation(rValue); }
    friend BinaryDec& operator+=(BinaryDec& lValue, const unsigned int& rValue) { return lValue.UIntAddOperation(rValue); }
    friend BinaryDec& operator+=(BinaryDec& lValue, const unsigned __int64& rValue) { return lValue.UInt64AddOperation(rValue); }

    friend BinaryDec& operator+=(BinaryDec& lValue, const signed char& rValue) { return lValue.Int8AddOperation(rValue); }
    friend BinaryDec& operator+=(BinaryDec& lValue, const signed short& rValue) { return lValue.Int16AddOperation(rValue); }
    friend BinaryDec& operator+=(BinaryDec& lValue, const unsigned char& rValue) { return lValue.UInt8AddOperation(rValue); }
    friend BinaryDec& operator+=(BinaryDec& lValue, const unsigned short& rValue) { return lValue.UInt16AddOperation(rValue); }

    /// <summary>
    /// Addition operation
    /// </summary>
    /// <param name="lValue">The left side value</param>
    /// <param name="rValue">The right side value.</param>
    /// <returns>BinaryDec</returns>
    friend BinaryDec operator+(BinaryDec lValue, const BinaryDec& rValue) { return lValue.AddOperation(rValue); }

    /// <summary>
    /// Addition operation between BinaryDec variant and Integer rValue.
    /// </summary>
    /// <param name="self">The left side value</param>
    /// <param name="rValue">The right side value.</param>
    friend BinaryDec operator+(BinaryDec lValue, const signed int& rValue) { return lValue.IntAddOperation(rValue); }
    friend BinaryDec operator+(BinaryDec lValue, const signed __int64& rValue) { return lValue.Int64AddOperation(rValue); }
    friend BinaryDec operator+(BinaryDec lValue, const unsigned int& rValue) { return lValue.UIntAddOperation(rValue); }
    friend BinaryDec operator+(BinaryDec lValue, const unsigned __int64& rValue) { return lValue.UInt64AddOperation(rValue); }

    friend BinaryDec operator+(BinaryDec lValue, const signed char& rValue) { return lValue.Int8AddOperation(rValue); }
    friend BinaryDec operator+(BinaryDec lValue, const signed short& rValue) { return lValue.Int16AddOperation(rValue); }
    friend BinaryDec operator+(BinaryDec lValue, const unsigned char& rValue) { return lValue.UInt8AddOperation(rValue); }
    friend BinaryDec operator+(BinaryDec lValue, const unsigned short& rValue) { return lValue.UInt16AddOperation(rValue); }

    friend BinaryDec operator+(const signed int& lValue, BinaryDec rValue)  { return rValue.IntAddOperation(lValue); }
    friend BinaryDec operator+(const signed __int64& lValue, BinaryDec rValue)  { return rValue.Int64AddOperation(lValue); }
    friend BinaryDec operator+(const unsigned int& lValue, BinaryDec rValue)  { return rValue.UIntAddOperation(lValue); }
    friend BinaryDec operator+(const unsigned __int64& lValue, BinaryDec rValue)  { return rValue.UInt64AddOperation(lValue); }

    friend BinaryDec operator+(const signed char& lValue, BinaryDec rValue)  { return rValue.Int8AddOperation(lValue); }
    friend BinaryDec operator+(const signed short& lValue, BinaryDec rValue)  { return rValue.Int16AddOperation(lValue); }
    friend BinaryDec operator+(const unsigned char& lValue, BinaryDec rValue)  { return rValue.UInt8AddOperation(lValue); }
    friend BinaryDec operator+(const unsigned short& lValue, BinaryDec rValue)  { return rValue.UInt16AddOperation(lValue); }

  #pragma endregion Other addition operations

  #pragma region Other subtraction operations

    /// <summary>
    /// -= operation
    /// </summary>
    /// <param name="lValue">The left side value</param>
    /// <param name="rValue">The right side value.</param>
    /// <returns>BinaryDec</returns>
    friend BinaryDec& operator-=(BinaryDec& lValue, const BinaryDec& rValue) { lValue.SubOp(rValue); return *lValue; }

    /// <summary>
    /// -= operation between BinaryDec variant and Integer rValue.
    /// </summary>
    /// <param name="lValue">The left side value</param>
    /// <param name="rValue">The right side value.</param>
    /// <returns>BinaryDec</returns>
    friend BinaryDec& operator-=(BinaryDec& lValue, const signed int& rValue) { return lValue.IntSubOperation(rValue); }
    friend BinaryDec& operator-=(BinaryDec& lValue, const signed __int64& rValue) { return lValue.Int64SubOperation(rValue); }
    friend BinaryDec& operator-=(BinaryDec& lValue, const unsigned int& rValue) { return lValue.UIntSubOperation(rValue); }
    friend BinaryDec& operator-=(BinaryDec& lValue, const unsigned __int64& rValue) { return lValue.UInt64SubOperation(rValue); }

    friend BinaryDec& operator-=(BinaryDec& lValue, const signed char& rValue) { return lValue.Int8SubOperation(rValue); }
    friend BinaryDec& operator-=(BinaryDec& lValue, const signed short& rValue) { return lValue.Int16SubOperation(rValue); }
    friend BinaryDec& operator-=(BinaryDec& lValue, const unsigned char& rValue) { return lValue.UInt8SubOperation(rValue); }
    friend BinaryDec& operator-=(BinaryDec& lValue, const unsigned short& rValue) { return lValue.UInt16SubOperation(rValue); }

    /// <summary>
    /// Subtraction operation
    /// </summary>
    /// <param name="lValue">The left side value</param>
    /// <param name="rValue">The right side value.</param>
    /// <returns>BinaryDec</returns>
    friend BinaryDec operator-(BinaryDec lValue, const BinaryDec& rValue) { return lValue.SubOperation(rValue); }

    /// <summary>
    /// Subtraction operation between BinaryDec variant and Integer rValue.
    /// </summary>
    /// <param name="lValue">The left side value</param>
    /// <param name="rValue">The right side value.</param>
    /// <returns>BinaryDec</returns>
    friend BinaryDec operator-(BinaryDec lValue, const signed int& rValue) { return lValue.IntSubOperation(rValue); }
    friend BinaryDec operator-(BinaryDec lValue, const signed __int64& rValue) { return lValue.Int64SubOperation(rValue); }
    friend BinaryDec operator-(BinaryDec lValue, const unsigned int& rValue) { return lValue.UIntSubOperation(rValue); }
    friend BinaryDec operator-(BinaryDec lValue, const unsigned __int64& rValue) { return lValue.UInt64SubOperation(rValue); }

    friend BinaryDec operator-(BinaryDec lValue, const signed char& rValue) { return lValue.Int8SubOperation(rValue); }
    friend BinaryDec operator-(BinaryDec lValue, const signed short& rValue) { return lValue.Int16SubOperation(rValue); }
    friend BinaryDec operator-(BinaryDec lValue, const unsigned char& rValue) { return lValue.UInt8SubOperation(rValue); }
    friend BinaryDec operator-(BinaryDec lValue, const unsigned short& rValue) { return lValue.UInt16SubOperation(rValue); }

    friend BinaryDec operator-(const signed int& lValue, const BinaryDec& rValue) { return ((BinaryDec)lValue).SubtractBy(rValue); }
    friend BinaryDec operator-(const signed __int64& lValue, const BinaryDec& rValue) { return ((BinaryDec)lValue).SubtractBy(rValue); }
    friend BinaryDec operator-(const unsigned int& lValue, const BinaryDec& rValue) { return ((BinaryDec)lValue).SubtractBy(rValue); }
    friend BinaryDec operator-(const unsigned __int64& lValue, const BinaryDec& rValue) { return ((BinaryDec)lValue).SubtractBy(rValue); }

    friend BinaryDec operator-(const signed char& lValue, const BinaryDec& rValue) { return ((BinaryDec)lValue).SubtractBy(rValue); }
    friend BinaryDec operator-(const signed short& lValue, const BinaryDec& rValue) { return ((BinaryDec)lValue).SubtractBy(rValue); }
    friend BinaryDec operator-(const unsigned char& lValue, const BinaryDec& rValue) { return ((BinaryDec)lValue).SubtractBy(rValue); }
    friend BinaryDec operator-(const unsigned short& lValue, const BinaryDec& rValue) { return ((BinaryDec)lValue).SubtractBy(rValue); }

  #pragma endregion Other subtraction operations

  #pragma region Modulus Operations
  protected:

    template<BinaryDecVariant VariantType=BinaryDec>
    void ModulusOpV1(const VariantType& rValue)
    {
      if(DecimalHalf.Value==0&&rValue.DecimalHalf.Value==0)
        IntHalf %= rValue.IntHalf;
      else {
        auto divRes = DivideBy(rValue);
        SubOp(divRes.MultiplyBy(rValue));
      }
    }

  public:

    void UIntModulusOp(const unsigned int& rValue)
    {
      if(DecimalHalf.Value==0)
        IntHalf %= rValue;
      else {
        auto divRes = DivideByIntV1(rValue);
        SubOp(divRes.MultiplyByUInt(rValue));
      }
    }

    void IntModulusOp(const signed int& rValue)
    {
      if (rValue < 0)
        throw("Negative number operation not supported by unsigned version.");
      else
        UIntModulusOp(rValue);
    }

    void UInt64ModulusOp(const unsigned __int64& rValue)
    {
      if(DecimalHalf.Value==0){
        unsigned __int64 result = IntHalf;
        result %= rValue;
        IntHalf = (unsigned int) result;
      } else {
        auto divRes = DivideByIntV1(rValue);
        SubOp(divRes.MultiplyByUInt64(rValue));
      }
    }

    void Int64ModulusOp(const signed __int64& rValue)
    {
      if (rValue < 0)
        throw("Negative number operation not supported by unsigned version.");
      else
        UInt64ModulusOp(rValue);
    }

    void ModulusOp(const BinaryDec& rValue){ ModulusOpV1(rValue); }

    BinaryDec& ModulusOperation(const BinaryDec& rValue){
      ModulusOp(rValue); return *this;
    }

    friend BinaryDec& operator%=(BinaryDec& lValue, const BinaryDec& rValue) { return lValue.ModulusOperation(rValue); }
    friend BinaryDec& operator%=(BinaryDec& lValue, const signed int& rValue) { lValue.IntModulusOp(rValue); return lValue; }
    friend BinaryDec& operator%=(BinaryDec& lValue, const unsigned int& rValue) { lValue.UIntModulusOp(rValue); return lValue; }
    friend BinaryDec& operator%=(BinaryDec& lValue, const unsigned __int64& rValue) { lValue.UInt64ModulusOp(rValue); return lValue;}
    friend BinaryDec& operator%=(BinaryDec& lValue, const signed __int64& rValue) { lValue.Int64ModulusOp(rValue); return lValue;}

  #pragma region Modulus Operations

  #pragma region Bitwise Operations

  #pragma endregion Bitwise Operations

  #pragma region Other Operators

    /// <summary>
    /// ++BinaryDec Operator
    /// </summary>
    /// <returns>BinaryDec &</returns>
    BinaryDec& operator ++()
    {
    #if defined(AltNum_PreventUnsignedUnderflowCheck)
      ++IntHalf;
    #else
      if (DecimalHalf == 0)
        ++IntHalf;
      else if (IntHalf == 0)
        IntHalf = 1;
      else
        ++IntHalf;
      return *this;
    #endif
    }

    /// <summary>
    /// ++BinaryDec Operator
    /// </summary>
    /// <returns>BinaryDec &</returns>
    BinaryDec& operator --()
    {
    #if defined(AltNum_PreventUnsignedUnderflowCheck)
      --IntHalf;
    #else
      if (DecimalHalf == 0)
        --IntHalf;
      else if (IntHalf == 0)
        throw("Unsigned underflow");
      else
        --IntHalf;
    #endif
      return *this;
    }

    /// <summary>
    /// BinaryDec Variant++ Operator
    /// </summary>
    /// <returns>BinaryDec</returns>
    BinaryDec operator ++(int)
    {
      BinaryDec tmp(*this);
      ++* this;
      return tmp;
    }

    /// <summary>
    /// BinaryDec Variant-- Operator
    /// </summary>
    /// <returns>BinaryDec</returns>
    BinaryDec operator --(int)
    {
      BinaryDec tmp(*this);
      --* this;
      return tmp;
    }

    /// <summary>
    /// BinaryDec Variant* Operator
    /// </summary>
    /// <returns>BinaryDec &</returns>
    BinaryDec& operator *()
    {
      return *this;
    }

  #pragma endregion Other Operators
  };

  /// <summary>
  /// Unsigned mixed fraction
  /// representing 0 - (4294967295+BinaryFraction)
  /// (8 bytes worth of Variable Storage inside class for each instance)
  /// </summary>
  class DLL_API MediumBinaryUDec : BinaryDec<MediumBinaryUDec>{
	}

  /// <summary>
  /// Unsigned mixed fraction
  /// representing +-(2147483647+BinaryFraction)
  /// (8 bytes worth of Variable Storage inside class for each instance)
  /// </summary>
  class DLL_API MediumBinaryDec : BinaryDec<MediumBinaryUDec>{
	}
}


