// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include <string>
#include <cmath>

#include <type_traits>
#include <cstddef>
#include <bit>//used for C++20 std::bit_cast
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
    
    struct SignedModePolicy{
       static constexpr bool SignedMode = true;
    }

    template<unsigned TotalBits>
    using PackedTypeForBits =
        std::conditional_t<TotalBits <= 32, std::uint32_t,
        std::conditional_t<TotalBits <= 64, std::uint64_t,
    #if defined(__SIZEOF_INT128__)
        std::conditional_t<TotalBits <= 128, unsigned __int128,
    #else
        std::conditional_t<TotalBits <= 128, std::array<std::uint64_t, 2>, // fallback if no 128-bit
    #endif
        std::array<std::uint64_t, (TotalBits + 63) / 64>>>>;

    template<unsigned TotalBits>
    using SignedTypeForBits =
        std::conditional_t<TotalBits <= 32, std::int32_t,
        std::conditional_t<TotalBits <= 64, std::int64_t,
    #if defined(__SIZEOF_INT128__)
        std::conditional_t<TotalBits <= 128, signed __int128,
    #else
        std::conditional_t<TotalBits <= 128, void, // fallback if no 128-bit
    #endif
        void>>>;

    // Native-word detection (effective only if PackedT is a single integer type)
    template<typename T>
    constexpr bool is_native_word_v =
        std::is_same_v<T, std::uint32_t> ||
        std::is_same_v<T, std::uint64_t>
    #if defined(__SIZEOF_INT128__)
        || std::is_same_v<T, unsigned __int128>
    #endif
        ;
    
    template<typename T>
    constexpr unsigned bit_width_of_v = sizeof(T) * 8;
  }
  
  class DefaultBinaryUDecIntHalfStorage{
    /// <summary>
    /// Stores whole half of number
    /// </summary>
    unsigned int IntHalf;

    static constexpr unsigned int IntHalfMax = 4'294'967'295;

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
  protected:
    using IntHalfT = BinaryDecCode::PackedTypeForBits(Policy::INT_BITS);
    using SignedIntHalfT = BinaryDecCode::SignedTypeForBits(Policy::INT_BITS); 
  public:
  #pragma region DigitStorage

    /// <summary>
    /// Stores whole half of number
    /// </summary>
    unsigned int IntHalf:Policy::INT_BITS;

    static constexpr unsigned int IntHalfMax = (unsigned int)((1 << INT_BITS) - 1);

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
 
    using DecimalOverflowT = BinaryDecCode::PackedTypeForBits(Policy::DEC_BITS+1);
    using DecimalHalfT = BinaryDecCode::PackedTypeForBits(Policy::DEC_BITS);

    /// <summary>
    /// The decimal overflow (Every value is used by DecimalMax so need to store in next biggest storage)
    /// </summary>
    static constexpr DecimalOverflowT DecimalOverflow = DecimalOverflowT(1) << DEC_BITS;
    
    /// <summary>
    /// The decimal overflow in _int64 so don't need to widen
    /// </summary>
    static constexpr unsigned _int64 DecimalOverflowX = unsigned _int64(DecimalOverflow);

    static constexpr DecimalHalfT DecimalMax = (DecimalHalfT)DecimalOverflow - 1;
    static constexpr DecimalHalfT HalfOverflow = DecimalMax/2;
  }

  class DefaultBinaryDecIntHalfStorage{
    /// <summary>
    /// Stores whole half of number(Including positive/negative status)
    /// </summary>
    MirroredInt IntHalf;
  };

  class DefaultBinaryDecStorage : public DefaultBinaryDecIntHalfStorage, public DefaultBinaryDecDecimalStorage{
  };

  template<typename Policy>
  struct BinaryDecStorageSelector : 
  std::conditional_t<!Has_UseCustomMode<Policy>&&!Has_SignedMode<Policy>,DefaultBinaryUDecStorage, BinaryDecCode::EmptyStruct>,
  std::conditional_t<Has_UseCustomMode<Policy>&&!Has_SignedMode<Policy>,CustomBinaryUDecStorage<Policy>, BinaryDecCode::EmptyStruct>,
  std::conditional_t<!Has_UseCustomMode<Policy>&&Has_SignedMode<Policy>,DefaultBinaryDecStorage, BinaryDecCode::EmptyStruct>
  {
  protected:
    static inline constexpr bool SignedMode = Has_SignedMode<Policy> ? Policy::SignedMode : false;
    static inline constexpr bool UseCustomMode = Has_UseCustomMode<Policy> ? Policy::UseCustomMode : false;

    using DefaultPart = std::conditional_t<!UseCustomMode&&!SignedMode,DefaultBinaryUDecStorage, BinaryDecCode::EmptyStruct>;
    using CustomPart = std::conditional_t<UseCustomMode&&!SignedMode,CustomBinaryUDecStorage<Policy>, BinaryDecCode::EmptyStruct>;
    using DefaultSignedPart = std::conditional_t<!UseCustomMode&&SignedMode,DefaultBinaryDecStorage, BinaryDecCode::EmptyStruct>;
    // Signed + custom width storage intentionally omitted.
    // If required later, implement using MediumUDecV3Variant rather than a BinaryDec variant.
    // For similar modes already supported in MediumDecV3Variant, prefer BinaryDec or BinaryUDec postfix in classname.
    // Naming convention optional: BinaryDec/BinaryUDec postfix recommended for clarity,
    // but other styles acceptable if mode is documented in class comment or policy name.
  public:  
    BinaryDecStorageSelector() noexcept : DefaultPart, CustomPart, DefaultSignedPart() {}
  };

	// Fixed-point number with binary fractional tail.
	// Fractional tail stored as DecimalHalf / (1 << DEC_BITS), default DEC_BITS = 32.
	// Tail is integer-based, not a floating-point mantissa.
	// CRTP base: VariantClass is the final derived type.
  template<class VariantClass, typename Policy:BinaryDecCode::EmptyStruct>
  class BinaryDec : public BinaryDecStorageSelector<VariantClass, Policy>
  {
  public:
  #pragma region class_constructors

    /// <summary>
    /// Initializes a new instance of the unsigned <see cref="BinaryDec"/> class using unsigned integer.
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

    BinaryDec& operator=(const unsigned int& rhs)
    requires (!SignedMode)
    {
      IntHalf = rhs; DecimalHalf = 0;
      return *this;
    }

    template<typename = std::enable_if_t<SignedMode>>
    BinaryDec& operator=(const signed int& rhs)
    requires (SignedMode)
    {
      IntHalf = rhs; DecimalHalf = 0;
      return *this;
    }

    /// <summary>
    /// Sets the value.
    /// </summary>
    constexpr void SetValue(const BinaryDec& rValue)
    {
      IntHalf = rValue.IntHalf;
      DecimalHalf = rValue.DecimalHalf;
    }

  #pragma endregion class_constructors

  #pragma region Value Cast
    
    using PackedT = UseCustomMode ? BinaryDecCode::PackedTypeForBits<INT_BITS + DEC_BITS> : std::uint64_t;
    
    // Bitfield-safe layout guard
    static constexpr bool ValueSafe =
        UseCustomMode
            ? (std::is_trivially_copyable_v<BinaryDec> &&
               sizeof(BinaryDec) == sizeof(IntHalf) + sizeof(DecimalHalf) &&
               (INT_BITS + DEC_BITS) <= bit_width_of_v<PackedT>)
            : (std::is_trivially_copyable_v<BinaryDec> &&
               sizeof(BinaryDec) == sizeof(IntHalf) + sizeof(DecimalHalf));
    
    static constexpr bool IsValueEffective = ValueSafe ?
    (UseCustomMode? (BinaryDecCode::is_native_word_v<PackedT> && (INT_BITS + DEC_BITS) <= BinaryDecCode::bit_width_of_v<PackedT>)
    : true):false;
    
    // Default unsigned mode
    constexpr auto GetValue() const noexcept
    requires (ValueSafe && !SignedMode && !UseCustomMode)
    {
        struct Packed { decltype(IntHalf) intHalf; decltype(DecimalHalf) decHalf; };
        return std::bit_cast<PackedT>(Packed{IntHalf, DecimalHalf});
    }
    
    // CustomMode (unsigned, bitfields inside)
    constexpr auto GetValue() const noexcept
    requires (ValueSafe && !SignedMode && UseCustomMode)
    {
        struct Packed { decltype(IntHalf) intHalf; decltype(DecimalHalf) decHalf; };
        return std::bit_cast<PackedT>(Packed{IntHalf, DecimalHalf});
    }
    
    // Signed mode (magnitude-only: 31-bit Value; sign exists in container)
    constexpr auto GetValue() const noexcept
    requires (ValueSafe && SignedMode)
    {
        using MagT = std::remove_cv_t<decltype(IntHalf.Value)>;
        struct Packed { MagT intHalfMag; decltype(DecimalHalf) decHalf; };
        return std::bit_cast<PackedT>(Packed{IntHalf.Value, DecimalHalf});
    }
  
  #pragma endregion Value Cast

  #pragma region Negative_Status

    bool IsPositive() const noexempt
    requires (SignedMode)
    { return IntHalf.IsPositive(); }

    bool IsNegative() const
    requires (SignedMode)
    { return IntHalf.IsNegative(); }

    /// <summary>
    /// Swaps the negative status.
    /// </summary>
    void SwapNegativeStatus()
    requires (SignedMode)
    { IntHalf.Sign ^= 1; }

    /// <summary>
    /// Negative Unary Operator(Flips negative status)
    /// </summary>
    /// <param name="self">The self.</param>
    /// <returns>MediumDec</returns>
    VariantClass operator-() const
    requires (SignedMode)
    { VariantClass self = *this; self.SwapNegativeStatus(); return self; }

  #pragma endregion Negative_Status

  #pragma region Check_if_value

    //Set value as exactly zero
    void SetAsZero()
    {
      IntHalf = constexpr (Policy::SignedMode)?MirroredInt::Zero:0;
      DecimalHalf = 0; }

    //Set value as exactly one
    void SetAsOne()
    { 
      IntHalf = constexpr (Policy::SignedMode)?MirroredInt::One:1;
      DecimalHalf = 0; }

    //Set as +-1 while keeping current sign
    template<typename = std::enable_if_t<SignedMode>>
    void SetAsOneVal()
    { IntHalf.Value = 1; DecimalHalf = 0; }

    template<typename = std::enable_if_t<!SignedMode>>
    void SetAsValues(const unsigned int& intVal = 0, const unsigned int& decVal = unsigned int::Zero)
    {
      IntHalf = intVal; DecimalHalf = decVal;
    }

    template<typename = std::enable_if_t<SignedMode>>
    void SetAsValues(const MirroredInt& intVal = MirroredInt::Zero, const unsigned int& decVal = 0)
    {
      IntHalf = intVal; DecimalHalf = decVal;
    }

    //Is at either zero or negative zero IntHalf of AltNum
    template<typename = std::enable_if_t<SignedMode>>
    bool IsAtZeroInt() const noexcept
    {
      return IntHalf.Value == 0;
    }

    template<typename = std::enable_if_t<SignedMode>>
    bool IsNotAtZeroInt() const noexcept
    {
      return IntHalf.Value != 0;
    }

    template<typename = std::enable_if_t<SignedMode>>
    bool IsAtOneInt() const noexcept
    {
      return IntHalf.Value == 1;
    }

    template<typename = std::enable_if_t<SignedMode>>
    bool IsNotAtOneInt() const noexcept
    {
      return IntHalf.Value != 1;
    }

    bool IsZero() const noexcept
    {
      return DecimalHalf == 0 && IntHalf == constexpr (Policy::SignedMode)? MirroredInt::Zero:0;
    }

    bool IsOne() const noexcept
    {
      return DecimalHalf == 0 && IntHalf == constexpr (Policy::SignedMode)?MirroredInt::One:1;
    }

    template<typename = std::enable_if_t<SignedMode>>
    bool IsOneVal() const noexcept
    {
      return DecimalHalf == 0 && IntHalf == MirroredInt::One;
    }

  #pragma endregion Check_if_value

  #pragma region RangeLimits

    /// <summary>
    /// Sets value to the highest value that it store
    /// </summary>
    inline void SetAsMaximum() noexcept
    {
      IntHalf = IntHalfMax;
      DecimalHalf = DecimalMax;
    }

    /// <summary>
    /// Sets value to the lowest value that it store
    /// </summary>
    inline void SetAsMinimum() noexcept
    {
      IntHalf = constexpr (Policy::SignedMode)?MirroredInt::Minimum : 0;
      DecimalHalf = DecimalMax;
    }

  #pragma endregion RangeLimits

  #pragma region ValueSetters
  #pragma endregion ValueSetters

  #pragma region ValueDefines
public:
    #pragma region Integer constants
    static constexpr VariantClass Zero = VariantClass(constexpr (Policy::SignedMode)?MirroredInt::Zero:0);
    static constexpr VariantClass One = VariantClass(constexpr (Policy::SignedMode)?MirroredInt::One:1);
    static constexpr VariantClass Two = VariantClass(constexpr (Policy::SignedMode)?MirroredInt::Two:2);
    template<typename = std::enable_if_t<SignedMode>>
    static constexpr VariantClass NegativeOne = VariantClass(MirroredInt::NegativeZero);
    #pragma endregion Integer constants

    #pragma region Core fractional constants
    //-0.5
    static constexpr VariantClass NegativePointFive = VariantClass(MirroredInt::NegativeZero, HalfOverflow);
    //Almost one defined as nearest representable real number
    static constexpr VariantClass AlmostOne = VariantClass(constexpr (Policy::SignedMode)?MirroredInt::Zero:0, DecimalMax);
    static constexpr VariantClass PointFive = VariantClass(constexpr (Policy::SignedMode)?MirroredInt::Zero:0, HalfOverflow);
    static constexpr VariantClass JustAboveZero = VariantClass(constexpr (Policy::SignedMode)?MirroredInt::Zero:0, 1);
    #pragma endregion Core fractional constants

    #pragma region Range limit constants
    static constexpr VariantClass Minimum = VariantClass(constexpr (Policy::SignedMode)?MirroredInt::Maximum:IntHalfMax, DecimalMax);
    static constexpr VariantClass Maximum = VariantClass(constexpr (Policy::SignedMode)?MirroredInt::Minimum:0, DecimalMax);
    #pragma endregion Range limit constants
    
    //frac = round( (ValueRepresentation - IntHalfRepresentation) * DecimalMax )
    #pragma region Mathematical constants
    // Pi
    // π = ~ 3.141'592'653'589'793'238'462'643'3
    template<typename = std::enable_if_t<!UseCustomMode>>
    static constexpr VariantClass PiNum = VariantClass(3, 605070158u);
    // Euler's number (Non-Alternative Representation)
    // Irrational number equal to about (1 + 1/n)^n
    // e = ~2.71828182845904523536028747135266249775724709369995
    template<typename = std::enable_if_t<!UseCustomMode>>
    static constexpr VariantClass ENum = VariantClass(2, 308884356u);
    // π * e
    // = ~8.539'734'222'673567065463550869546574495034888535765084881233717265981654348037954472832304065619300439
    template<typename = std::enable_if_t<!UseCustomMode>>
    static constexpr VariantClass PiENum = VariantClass(8, 2315469002u);
    
    //Pi, and e are different in VariantClassV2 and higher variants(using FlagState multiplier constants)
    //static constexpr VariantClass Pi = PiNum;
    //static constexpr VariantClass E = ENum;
    //PiE constant only defined if PiE FlagState enabled
    
    template<typename = std::enable_if_t<!UseCustomMode>>
    static constexpr VariantClass LN10 = VariantClass(2, 1300902225u);
    
    //template<typename = std::enable_if_t<!UseCustomMode>>
    //static constexpr VariantClass LN10Div = VariantClass(0, ???);
    //template<typename = std::enable_if_t<!UseCustomMode>>
    //static constexpr VariantClass TwiceLN10Div = VariantClass(0, ???);
    
    //0.693147180559945309417232121458176568075500134360255254120680009493393621969694715605863326996418688
    template<typename = std::enable_if_t<!UseCustomMode>>
    static constexpr VariantClass Ln2 = VariantClass(0, 2975105830u);
    template<typename = std::enable_if_t<SignedMode>>
    static constexpr VariantClass NegLn2 = VariantClass(MirroredInt::NegativeZero, 2975105830u);
    #pragma endregion Mathematical constants
    
    #pragma region Pi Multipliers constants
    #pragma endregion Pi Multipliers constants
    
    #pragma region Small magnitude constants
    //Power of 10 magnitudes not perfectly representable in binary based fractions
    #pragma endregion Small magnitude constants
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

  #pragma endregion ConvertFromOtherTypes

  #pragma region ConvertToOtherTypes

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
public:

    std::strong_ordering operator<=>(const BinaryDec& that) const
    {
      if constexpr (IsValueEffective&&!SignedMode) {
        if (auto ValueCmp = GetValue() <=> that.GetValue(); ValueCmp != 0)
          return ValueCmp;
      } else {
        if (auto IntHalfCmp = IntHalf <=> that.IntHalf; IntHalfCmp != 0)
          return IntHalfCmp;
        if constexpr (!SignedMode) {
          if (auto DecimalHalfCmp = DecimalHalf <=> that.DecimalHalf; DecimalHalfCmp != 0)
            return DecimalHalfCmp;
        } else {
          //Counting negative zero as same as zero IntHalf but with negative DecimalHalf
          unsigned int lVal = IsNegative()?DecimalMax-DecimalHalf:DecimalHalf;
          unsigned int rVal = that.IsNegative()?DecimalMax-that.DecimalHalf:that.DecimalHalf;
          if (auto DecimalHalfCmp = lVal <=> rVal; DecimalHalfCmp != 0)
            return DecimalHalfCmp;
        }
      }
      return std::strong_ordering::equal;
    }

    std::strong_ordering operator<=>(const unsigned int& that) const
    requires (!UseCustomMode)
    {
      if (auto IntHalfCmp = IntHalf <=> that; IntHalfCmp != 0)
          return IntHalfCmp;
      if(DecimalHalf > 0)
        return std::strong_ordering::greater;
      return std::strong_ordering::equal;
    }

    std::strong_ordering operator<=>(const signed int& that) const noexcept
    requires (!UseCustomMode)
    {
        if constexpr (!SignedMode) {
            // Unsigned mode
            if (that < 0)
                return std::strong_ordering::greater;
    
            if (auto IntHalfCmp = IntHalf <=> that;
                IntHalfCmp != 0)
                return IntHalfCmp;
    
            return (DecimalHalf > 0)
                ? std::strong_ordering::greater
                : std::strong_ordering::equal;
    
        } else {
            // Signed mode
            if (auto IntHalfCmp = IntHalf <=> that; IntHalfCmp != 0)
                return IntHalfCmp;
    
            if (IsNegative()) {
                // e.g. -3.5 < -3
                return (DecimalHalf > 0)
                    ? std::strong_ordering::less
                    : std::strong_ordering::equal;
            } else {
                // e.g. 3.5 > 3
                return (DecimalHalf > 0)
                    ? std::strong_ordering::greater
                    : std::strong_ordering::equal;
            }
        }
    }

    std::strong_ordering operator<=>(const IntHalfT& that) const
    requires (UseCustomMode)
    {
      if (auto IntHalfCmp = IntHalf <=> that; IntHalfCmp != 0)
          return IntHalfCmp;
      if(DecimalHalf > 0)
        return std::strong_ordering::greater;
      return std::strong_ordering::equal;
    }

    std::strong_ordering operator<=>(const SignedIntHalfT& that) const noexcept
#if defined(__SIZEOF_INT128__)
    requires (UseCustomMode && POLICY::INT_BITS<=128)
#else
    requires (UseCustomMode && POLICY::INT_BITS<=64)
#endif
    {
        if constexpr (!SignedMode) {
            // Unsigned mode
            if (that < 0)
                return std::strong_ordering::greater;
    
            if (auto IntHalfCmp = IntHalf <=> that;
                IntHalfCmp != 0)
                return IntHalfCmp;
    
            return (DecimalHalf > 0)
                ? std::strong_ordering::greater
                : std::strong_ordering::equal;
    
        } else {
            // Signed mode
            if (auto IntHalfCmp = IntHalf <=> that; IntHalfCmp != 0)
                return IntHalfCmp;
    
            if (IsNegative()) {
                // e.g. -3.5 < -3
                return (DecimalHalf > 0)
                    ? std::strong_ordering::less
                    : std::strong_ordering::equal;
            } else {
                // e.g. 3.5 > 3
                return (DecimalHalf > 0)
                    ? std::strong_ordering::greater
                    : std::strong_ordering::equal;
            }
        }
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
    requires (!UseCustomMode)
    {
      if (IntHalf!=that)
        return false;
      if (DecimalHalf!=0)
        return false;
      return true;
    }

    bool operator!=(const unsigned int& that) const
    requires (!UseCustomMode)
    {
      if (IntHalf!=that)
        return true;
      if (DecimalHalf!=0)
        return true;
      return false;
    }

    bool operator==(const signed int& that) const
    requires (!UseCustomMode)
    {
      if (IntHalf!=that)
        return false;
      if (DecimalHalf!=0)
        return false;
      return true;
    }

    bool operator!=(const signed int& that) const
    requires (!UseCustomMode)
    {
      if (IntHalf!=that)
        return true;
      if (DecimalHalf!=0)
        return true;
      return false;
    }

    bool operator==(const unsigned int& that) const
    requires (UseCustomMode)
    {
      if (IntHalf!=that)
        return false;
      if (DecimalHalf!=0)
        return false;
      return true;
    }

    bool operator!=(const unsigned int& that) const
    requires (UseCustomMode)
    {
      if (IntHalf!=that)
        return true;
      if (DecimalHalf!=0)
        return true;
      return false;
    }

    bool operator==(const SignedIntHalfT& that) const
#if defined(__SIZEOF_INT128__)
    requires (UseCustomMode && POLICY::INT_BITS<=128)
#else
    requires (UseCustomMode && POLICY::INT_BITS<=64)
#endif
    {
      if (IntHalf!=that)
        return false;
      if (DecimalHalf!=0)
        return false;
      return true;
    }

    bool operator!=(const SignedIntHalfT& that) const
#if defined(__SIZEOF_INT128__)
    requires (UseCustomMode && POLICY::INT_BITS<=128)
#else
    requires (UseCustomMode && POLICY::INT_BITS<=64)
#endif
    {
      if (IntHalf!=that)
        return true;
      if (DecimalHalf!=0)
        return true;
      return false;
    }

  #pragma endregion Comparison Operators

  #pragma region NormalRep Integer Division Operations
public:

    template<IntegerType IntType=unsigned int>
    inline void PartialUDivOp(const IntType& rValue)
    {//Avoid using with special status representations such as approaching zero or result will be incorrect
      PackedT Res, IntHalfRes, DecimalRes;
      if constexpr (IsValueEffective) {
        Res = GetValue();
      } else {
        Res = DecimalHalf == 0 ? DecimalOverflow * IntHalf : DecimalOverflow * IntHalf + DecimalHalf;
      }
      Res /= rValue;
      if constexpr (IsValueEffective) {
        constexpr unsigned int ShiftAmount = UseCustomMode ? Policy::DEC_BITS : 32;
      	IntHalfRes = Res >> ShiftAmount;
      	DecimalRes = Res & DecimalMax;// mask out lower bits
      } else {
      	IntHalfRes = Res/DecimalOverflow;
      	DecimalRes = Res - DecimalOverflow * IntHalfRes;
      }
      if constexpr (SignedMode)
          IntHalf.Value = static_cast<unsigned int>(IntHalfRes);
      else
          IntHalf = static_cast<IntHalfT>(IntHalfRes);
      DecimalHalf = (DecHalfT)DecimalRes;
    }

    template<IntegerType IntType=signed int>
    inline void PartialIntDivOp(const IntType& Value)
    {
      if constexpr (SignedMode){
        if(Value<0)
        {
            SwapNegativeStatus();
            PartialUIntDivOp(-Value);
        }
        else
            PartialUIntDivOp(Value);
      } else
      {
        // Forward directly to unsigned logic
        PartialUDivOp(static_cast<std::make_unsigned_t<IntType>>(Value));
      }
    }

public:

    /// <summary>
    /// Basic division operation between BinaryDec Variant and unsigned Integer value
    /// that ignores special representation status
    /// (Doesn't modify owner object)
    /// </summary>
    /// <param name="rValue">The right side value</param>
    template<IntegerType IntType=unsigned int>
    inline void UIntDivOp(const IntType& Value)
    {
        if (Value == 0)
        {
            throw "Target value can not be divided by zero";
        }
        else if (IsZero())
            return;
        PartialUIntDivOpV1(Value);
        if (IntHalf == 0 && DecimalHalf == 0)
            DecimalHalf = 1;//Prevent Dividing into nothing
    }

    template<IntegerType IntType=signed int>
    inline void IntDivOp(const IntType& Value)
    {
      if constexpr (SignedMode)
      {
        if (Value == 0)
        {
            throw "Target value can not be divided by zero";
        }
        else if (IsZero())
            return;
        PartialIntDivOpV1(Value);
        if (IntHalf == 0 && DecimalHalf == 0)
            DecimalHalf = 1;//Prevent Dividing into nothing
      } else
      {
        // Forward directly to unsigned logic
        UIntDivOp(static_cast<std::make_unsigned_t<IntType>>(Value));
      }
    }

    template<IntegerType IntType=unsigned int>
    inline VariantClass& UIntDivOperation(const IntType& rValue)
    { UIntDivOpV1(rValue); return *this; }

    template<IntegerType IntType=unsigned int>
    inline VariantClass& IntDivOperation(const IntType& rValue)
    { IntDivOpV1(rValue); return *this; }

    /// <summary>
    /// Basic division operation between BinaryDec Variant and unsigned Integer value
    /// that ignores special representation status
    /// (Doesn't modify owner object)
    /// </summary>
    /// <param name="rValue">The right side value</param>
    template<IntegerType IntType=unsigned int>
    inline constexpr VariantClass DivideByUInt(const IntType& rValue)
    { auto self = *this; return self.UIntDivOperation(rValue); }

    /// <summary>
    /// Basic division operation between BinaryDec Variant and signed Integer value
    /// that ignores special representation status
    /// (Doesn't modify owner object)
    /// </summary>
    /// <param name="rValue">The right side value</param>
    template<IntegerType IntType=unsigned int>
    inline constexpr VariantClass DivideByInt(const IntType& rValue)
    { auto self = *this; return self.IntDivOperationV1(rValue); }

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
      unsigned _int64 SelfRes = DecimalOverflowX * IntHalf + DecimalHalf;
      unsigned _int64 ValueRes = DecimalOverflowX * rValue.IntHalf + rValue.DecimalHalf;

      unsigned _int64 IntHalfRes = SelfRes / ValueRes;
      unsigned _int64 DecimalRes = SelfRes - ValueRes * IntHalfRes;
      IntHalf = (unsigned int) IntHalfRes;
      DecimalHalf = DecimalRes;
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
            DecimalHalf = 1;
#else
        else
          PartialDivOp(rValue);
#endif
      }
#if !defined(AltNum_DisableDivideDownToNothingPrevention)
      else if (PartialDivOp(rValue))//Prevent Dividing into nothing
        DecimalHalf = 1;
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
        unsigned __int64 SRep = IntHalf == 0 ? DecimalHalf : DecimalOverflowX * IntHalf + DecimalHalf;
        SRep *= rValue;
        if (SRep >= DecimalOverflowX)
        {
          unsigned __int64 OverflowVal = SRep / DecimalOverflowX;
          SRep -= OverflowVal * DecimalOverflowX;
          IntHalf = (unsigned int)OverflowVal;
          DecimalHalf = (unsigned int)SRep;
        }
        else
        {
          IntHalf = 0;
          DecimalHalf = (unsigned int)SRep;
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
          __int64 rRep = rValue.IntHalf == 0 ? rValue.DecimalHalf : DecimalOverflowX * rValue.IntHalf + rValue.DecimalHalf;
          if (rRep >= DecimalOverflowX)
          {
            __int64 OverflowVal = rRep / DecimalOverflowX;
            rRep -= OverflowVal * DecimalOverflowX;
            IntHalf = (unsigned int)OverflowVal;
            DecimalHalf = (unsigned int)rRep;
            return;
          }
          else
          {
            DecimalHalf = (signed int)rRep;
          #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
            if(DecimalHalf==0)
              DecimalHalf = 1;
          #endif
            IntHalf = 0;
            return;
          }
        }
      }
      else if (IntHalf == 0)
      {
        __int64 SRep = (__int64)DecimalHalf;
        SRep *= rValue.DecimalHalf;
        SRep /= DecimalOverflowX;
        if (rValue.IntHalf == 0)
        {
          DecimalHalf = (signed int)SRep;
        #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
          if (DecimalHalf == 0)
            DecimalHalf = 1;
        #endif
          return;
        }
        else
        {
          SRep += (__int64)DecimalHalf * rValue.IntHalf;
          if (SRep >= DecimalOverflowX)
          {
            __int64 OverflowVal = SRep / DecimalOverflowX;
            SRep -= OverflowVal * DecimalOverflowX;
            IntHalf = OverflowVal;
            DecimalHalf = (signed int)SRep;
            return;
          }
          else
          {
            DecimalHalf = (unsigned int)SRep;
        #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
            if(DecimalHalf==0)
              DecimalHalf = 1;
        #endif
            return;
          }
        }
      }
      else
      {
        if (rValue.DecimalHalf == 0)//Y is integer
        {
          __int64 SRep = DecimalOverflowX * IntHalf + DecimalHalf;
          SRep *= rValue.IntHalf;
          if (SRep >= DecimalOverflowX)
          {
            __int64 OverflowVal = SRep / DecimalOverflowX;
            SRep -= OverflowVal * DecimalOverflowX;
            IntHalf = (unsigned int)OverflowVal;
            DecimalHalf = (unsigned int)SRep;
          }
          else
          {
            DecimalHalf = (unsigned int)SRep;
            if(DecimalHalf==0)
            {
        #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
              if(DecimalHalf==0)
                DecimalHalf = 1;
        #endif
            }
            IntHalf = 0;
          }
          return;
        }
        else if (rValue.IntHalf == 0)
        {
          __int64 SRep = DecimalOverflowX * IntHalf + DecimalHalf;
          SRep *= rValue.DecimalHalf;
          SRep /= DecimalOverflowX;
          if (SRep >= DecimalOverflowX)
          {
            __int64 OverflowVal = SRep / DecimalOverflowX;
            SRep -= OverflowVal * DecimalOverflowX;
            IntHalf = (unsigned int)OverflowVal;
            DecimalHalf = (unsigned int)SRep;
          }
          else
          {
            DecimalHalf = (unsigned int)SRep;
            if(DecimalHalf==0)
            {
        #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
              if(DecimalHalf==0)
                DecimalHalf = 1;
        #endif
            }
            IntHalf = 0;
          }
          return;
        }
        else
        {
          //X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
          unsigned __int64 SRep = IntHalf == 0 ? DecimalHalf : DecimalOverflowX * IntHalf + DecimalHalf;
          SRep *= rValue.IntHalf;//SRep holds __int64 version of X.Y * Z
          //X.Y *.V
          unsigned __int64 Temp03 = (__int64)(rValue.DecimalHalf * IntHalf);//Temp03 holds __int64 version of X *.V
          unsigned __int64 Temp04 = (__int64)DecimalHalf * (__int64)rValue.DecimalHalf;
          Temp04 /= DecimalOverflow;
          //Temp04 holds __int64 version of .Y * .V
          unsigned __int64 IntegerRep = SRep + Temp03 + Temp04;
          unsigned __int64 intHalf = IntegerRep / DecimalOverflow;
          IntegerRep -= intHalf * DecimalOverflowX;
          IntHalf = (unsigned int) intHalf;
          DecimalHalf = (unsigned int)IntegerRep;
        }
      }
      #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
      if(DecimalHalf==0&&IntHalf==0)
        DecimalHalf = 1;
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

        DecimalOverflowT decResult = DecimalHalf + rValue.DecimalHalf;
        if (decResult == DecimalOverflow) {//5.4 + 4.6
          ++IntHalf;
          if (IntHalf == 0)
            SetAsZero();
          else
            DecimalHalf = 0;
        }
        else if (decResult > DecimalOverflow) {//5.4 + 4.7
          ++IntHalf;
          DecimalHalf = (DecimalHalfT)(decResult - DecimalOverflow);
        }
        else if (signBeforeOp != IntHalf.Sign)
          DecimalHalf = (DecimalHalfT)(DecimalOverflow - decResult);
        else
          DecimalHalf = (DecimalHalfT)(decResult);
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
        if (DecimalHalf == rValue.DecimalHalf) {//5.5 - 5.5 = 10
          if (IntHalf == 0)
            SetAsZero();
          else
            DecimalHalf = 0;
        }
        else if (rValue.DecimalHalf > DecimalHalf) {
          --IntHalf;
          if (signBeforeOp != IntHalf.Sign)//5.4 - 5.7 = -0.3
            DecimalHalf = rValue.DecimalHalf - DecimalHalf;
          else//5.4 - 3.6 = 1.8
            DecimalHalf = (DecimalHalfT)(DecimalOverflow + DecimalHalf - rValue.DecimalHalf);
        }
        else if (signBeforeOp != IntHalf.Sign)//5.3 - 7.2 = -1.9
          DecimalHalf = (DecimalHalfT)(DecimalOverflow - DecimalHalf + rValue.DecimalHalf);
        else//5.4 - 5.3 = 0.1
          DecimalHalf -= rValue.DecimalHalf;
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
      if(DecimalHalf==0&&rValue.DecimalHalf==0)
        IntHalf %= rValue.IntHalf;
      else {
        auto divRes = DivideBy(rValue);
        SubOp(divRes.MultiplyBy(rValue));
      }
    }

  public:

    void UIntModulusOp(const unsigned int& rValue)
    {
      if(DecimalHalf==0)
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
      if(DecimalHalf==0){
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

	// Naming convention:
	//   BinaryDec   → signed fixed-point with binary fractional tail
	//   BinaryUDec  → unsigned fixed-point with binary fractional tail
	//   Prefix (e.g., Medium) indicates width category:
	//       Medium → 32-bit integer half (uint32_t / int32_t)
	// Derived concrete types below:
	//   MediumBinaryUDec → unsigned, medium-width (uint32_t)
	//   MediumBinaryDec  → signed, medium-width (int32_t)

  /// <summary>
  /// Unsigned mixed fraction
  /// representing 0 - (4294967295 + BinaryFraction)
  /// (8 bytes worth of Variable Storage inside class for each instance)
  /// </summary>
  class DLL_API MediumBinaryUDec : BinaryDec<MediumBinaryUDec>{
  };

  /// <summary>
  /// Signed mixed fraction
  /// representing ±(2147483647 + BinaryFraction)
  /// (8 bytes worth of variable storage inside class for each instance)
  /// </summary>
  class DLL_API MediumBinaryDec : BinaryDec<MediumBinaryDec, BinaryDecCode::SignedModePolicy>{
  };
}

