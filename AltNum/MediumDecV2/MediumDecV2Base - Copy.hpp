// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include "MediumDecV2Preprocessors.h"
#include "MediumDecBase.hpp"
#if defined(AltNum_UseBuiltinVirtualTable)
#include "..\VirtualTableBase.hpp"
#endif

#include "..\RepType.h"

using MirroredInt = BlazesRusCode::MirroredInt;
using PartialInt = BlazesRusCode::PartialInt;

namespace BlazesRusCode
{
    class MediumDecV2Base;

    //Reduced version of MediumDecV2 result for modulus result and other stuff
    class DLL_API MediumDecV2Base : public MediumDecBase
    {
        #if defined(AltNum_UseBuiltinVirtualTable)//Experimental VTable code
    protected:
        struct VirtualTable {
            RepTypeFn* VirtualTable_GetRepType;
            //String_RepTypeFn* VirtualTable_RepTypeAsString;
        };
        #if defined(AltNum_UseNonStaticVirtualStable)
        VirtualTable* VTable;
        #else//Defaulting to non-static VTable to attempt to reduce virtual footprint
        static VirtualTable* VTable;
        #endif
        #if !defined(AltNum_UseNonStaticVirtualStable)
        VirtualTable* InitializeVTable() { return new VirtualTable; }
        #endif
        #endif
    protected:
        //BitFlag 01(1) = PiRep
        RepTypeUnderlayer PiFlag = 1;
        //BitFlag 02(2) = ERep
        RepTypeUnderlayer EFlag = 2;
        //BitFlag 03(4) = IRep
        RepTypeUnderlayer IFlag = 4;
        //BitFlag 04(8) = Fractional Rep
        RepTypeUnderlayer FractionalFlag = 8;
        //If BitFlag#7 and BitFlag#4 is set and BitFlag#6 isn't set,
        //then Approaching From Top side;
        //If BitFlag#7 is set and BitFlag#6 isn't set,
        //then Approaching From Bottom side;
        RepTypeUnderlayer ApproachFromRFlag = 8;
        //BitFlag 05 (16) = Power of flag
        RepTypeUnderlayer ToPowerOfFlag = 16;
        //If BitFlag#7 is set,
        //then Approaching but divided by ExtraRep
        RepTypeUnderlayer DividedByFlag = 16;
        //BitFlag 06 (32)= Mixed Fraction flag
        RepTypeUnderlayer MixedFracFlag = 32;
        //If BitFlag#7 is set and BitFlag#06 is set,
        //then Is Infinity Type
        RepTypeUnderlayer InfinityFlag = 32;
        //BitFlag 07 = Infinitesimal/Infinity Bit (Infinity or approaching representation)
        RepTypeUnderlayer InfTypeFlag = 64;
        //Bitflag 08= Undefined/NaN/Nil
        RepTypeUnderlayer UndefinedBit = 128;
    public:
        #pragma region DigitStorage
        #pragma endregion DigitStorage

        #pragma region class_constructors

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecV2Base"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        MediumDecV2Base(const MirroredInt& intVal, const PartialInt& decVal = PartialInt::Zero)
        {
            IntHalf = intVal;
            DecimalHalf = decVal;
            #if defined(AltNum_UseBuiltinVirtualTable)&&defined(AltNum_UseNonStaticVirtualStable)
            VTable = new VirtualTable;
            #endif
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecV2Base"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        MediumDecV2Base(const signed int& intVal = 0, const PartialInt& decVal = PartialInt::Zero)
        {
            IntHalf = intVal;
            DecimalHalf = decVal;
            #if defined(AltNum_UseBuiltinVirtualTable)&&defined(AltNum_UseNonStaticVirtualStable)
            VTable = new VirtualTable;
            #endif
        }

        MediumDecV2Base& operator=(const MediumDecV2Base& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            IntHalf = rhs.IntHalf; DecimalHalf = rhs.DecimalHalf;
            return *this;
        }

        MediumDecV2Base& operator=(const signed int& rhs)
        {
            IntHalf = rhs; DecimalHalf = 0;
            return *this;
        }

        /// <summary>
        /// Creates class from derived class into this class
        /// (subscript operator of [])
        /// </summary>
        template<MediumDecVariant VariantType>
        MediumDecV2Base operator[](VariantType variantValue) const
        {
            MediumDecV2Base newSelf = MediumDecV2Base(variantValue.IntHalf, variantValue.DecimalHalf);
            return newSelf;
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetValue(const MediumDecV2& Value)
        {
            IntHalf = Value.IntHalf;
            DecimalHalf = Value.DecimalHalf;
        } const

            #pragma endregion class_constructors

            #pragma region Negative_Status

            /// <summary>
            /// Negative Unary Operator(Flips negative status)
            /// </summary>
            /// <param name="self">The self.</param>
            /// <returns>MediumDecV2Base</returns>
            MediumDecV2Base operator-() const
        {
            MediumDecV2Base self = *this; self.SwapNegativeStatus(); return self;
        }

        #pragma endregion Negative_Status


        #pragma region Check_if_value
        #pragma endregion Check_if_value

        #pragma region Const Representation values
    protected:
        #if defined(AltNum_EnablePiRep)
        //Pi*Value representation(when DecimalHalf.Flag==1)
        static const unsigned int PiRep = 1;
        #endif
        #if defined(AltNum_EnableERep)
        //e*Value representation(DecimalHalf.Flag==2)
        static const unsigned int ERep = 2;
        #endif
        #if defined(AltNum_EnableIRep)
        //e*Value representation(DecimalHalf.Flag==3)
        static const unsigned int IRep = 3;
        #endif

        #if defined(AltNum_EnableInfinityRep)
        //When DecimalHalf.Value equals this value, it represents infinity (sign of IntHalf determines if either negative or positive inifity)
        static const unsigned int InfinityRep = 1073741805;
        #endif
        #if defined(AltNum_EnableApproaching)
        //When DecimalHalf.Value equals this value, it represents Approaching IntHalf from right towards left (IntHalf.0..01)
        static const unsigned int ApproachingBottomRep = 1073741806;
        //When DecimalHalf.Value equals this value, it represents Approaching IntHalf from left towards right (IntHalf.9..9)
        static const unsigned int ApproachingTopRep = 1073741807;
        #endif
        //If DecimalHalf is greator than NaNVariantThreshold, than number is at unknown range or indeterminate form
        static const unsigned int NaNVariantThreshold = 1073741809;
        #if defined(AltNum_EnableNil)
        //Is defined at empty value when DecimalHalf is at this value
        static const signed int NilRep = 1073741810;
        #endif
        #if defined(AltNum_EnableNaN)
        //Is NaN when DecimalHalf is at this value
        static const signed int NaNRep = 1073741811;
        //Is Undefined when DecimalHalf is at this value
        static const signed int UndefinedRep = 1073741812;
        #endif
        #if defined(AltNum_EnableUndefinedButInRange)
        //When DecimalHalf is at this value, than value is undefined but within real number range
        //Such as result of Cos of infinity
        //https://www.cuemath.com/trigonometry/domain-and-range-of-trigonometric-functions/
        static const unsigned int UndefinedInRangeRep = 1073741813;
        #endif
        #if defined(AltNum_EnableIndeterminateForms)
        //Is indeterminate form when DecimalHalf above this value
        static unsigned int IndeterminateThreshold = UndefinedInRangeRep;

        //When DecimalHalf.Value is this value, then the indeterminate form represents 0 x Infinity
        //When IntHalf.Sign==NegativeSign, then the indeterminate form represents 0 x -Infinity
        static unsigned int ZeroTimesInfinityRep = IndeterminateThreshold + 1;//1073741814;

        //When DecimalHalf.Value is this value, then the indeterminate form represents Infinity / Infinity
        //When IntHalf.Sign==NegativeSign, then the indeterminate form represents -Infinity / Infinity
        static unsigned int InfDividedByInfRep = IndeterminateThreshold + 2;

        //When DecimalHalf.Value is this value, then the indeterminate form represents Infinity - Infinity
        static unsigned int InfMinusInfRep = IndeterminateThreshold + 3;

        //When DecimalHalf.Value is this value, then the indeterminate form represents 0 to power of 0
        static unsigned int ZeroToPowerOfZeroRep = IndeterminateThreshold + 4;

        //When DecimalHalf.Value is this value, then the indeterminate form represents IntHalf/0
        static unsigned int DivideByZeroRep = IndeterminateThreshold + 9;

        static unsigned int UnknownIndeterminateRep = IndeterminateThreshold + 10;
        #endif//Maximum DecimalHalf.Value = 1073741823
    public:
        #pragma endregion Const Representation values

        #pragma region RepType

        /// <summary>
        /// Returns representation type data that is stored in value
        /// </summary>
        #if defined(AltNum_UseBuiltinVirtualTable)
        RepType VirtualTable_GetPiRepType()
            #else
        RepType GetPiRepType()
            #endif
        {
            switch (DecimalHalf.Value)
            {
                #if defined(AltNum_EnableApproaching)
            case ApproachingBottomRep:
                return RepType::ApproachingTopE; break;
            case ApproachingTopRep:
                return RepType::ApproachingTopE; break;
                #endif
            default:
                return RepType::PiNum;
            }
        }

        #if defined(AltNum_EnableERep)
        /// <summary>
        /// Returns representation type data that is stored in value
        /// </summary>
        #if defined(AltNum_UseBuiltinVirtualTable)
        RepType VirtualTable_GetERepType()
            #else
        RepType GetERepType()
            #endif
        {
            switch (DecimalHalf.Value)
            {
                #if defined(AltNum_EnableApproaching)
            case ApproachingBottomRep:
                return RepType::ApproachingTopE; break;
            case ApproachingTopRep:
                return RepType::ApproachingTopE; break;
                #endif
            default:
                return RepType::ENum;
            }
        }
        #endif

        #if defined(AltNum_EnableIRep)
        /// <summary>
        /// Returns representation type data that is stored in value
        /// </summary>
        #if defined(AltNum_UseBuiltinVirtualTable)
        RepType VirtualTable_GetIRepType()
            #else
        RepType GetIRepType()
            #endif
        {
            switch (DecimalHalf.Value)
            {
                #if defined(AltNum_EnableImaginaryInfinity)
            case InfinityRep:
                return RepType::ImaginaryInfinity; break;
                #endif
                #if defined(AltNum_EnableApproaching)
            case ApproachingBottomRep:
                return RepType::ApproachingImaginaryBottom; break;
            case ApproachingTopRep:
                return RepType::ApproachingImaginaryTop; break;
                #endif
            default:
                return RepType::INum;
            }
        }
        #endif

        /// <summary>
        /// Returns representation type data that is stored in value
        /// </summary>
        #if defined(AltNum_UseBuiltinVirtualTable)
        RepType VirtualTable_GetNormRepType()
            #else
        RepType GetNormRepType()
            #endif
        {
            switch (DecimalHalf.Value)
            {
                #if defined(AltNum_EnableApproaching)
            case ApproachingBottomRep:
                return RepType::ApproachingBottom; break;
            case ApproachingTopRep:
                return RepType::ApproachingTop; break;
                #endif
                #if defined(AltNum_EnableInfinityRep)
            case InfinityRep:
                return RepType::Infinity; break;
                #endif
                #if defined(AltNum_EnableNaN)
            case NaNRep:
                return RepType::NaN; break;
            case UndefinedRep:
                return RepType::Undefined; break;
                #endif
                #if defined(AltNum_EnableNil)
            case NilRep:
                return RepType::Nil; break;
                #endif
            default:
                #if defined(AltNum_EnableIndeterminateForms)
                if (DecimalHalf > IndeterminateRep)
                    return RepType::IndeterminateForm;
                #endif
                return RepType::NormalType; break;
            }
        }

        /// <summary>
        /// Returns representation type data that is stored in value
        /// </summary>
        #if defined(AltNum_UseBuiltinVirtualTable)
        RepType VirtualTable_GetRepType()
            #else
        RepType GetRepType()
            #endif
        {
            switch (DecimalHalf.Flag)
            {
                #if defined(AltNum_EnablePiRep)
            case 1:
                return GetPiRepType(); break;
                #endif
                #if defined(AltNum_EnableERep)
            case 2:
                return GetERepType(); break;
                #endif
                #if defined(AltNum_EnableIRep)
            case 3:
                return GetIRepType(); break;
                #elif defined(MediumDecV2_EnableWithinMinMaxRange)
            case 3:
                //If IntHalf==???, then left side range value equals negative infinity
                //If DecimalHalf.Value==???, then right side range value equals positive infinity
                //IntHalf represents left side minimum
                //For DecimalHalf.Value represents right side maximum value with negative numbers represents at numbers above ???
                return RepType::WithinMinMaxRange;
                #endif
            default:
                return GetNormRepType(); break;
            }
        }

        #if defined(AltNum_UseBuiltinVirtualTable)

        /// <summary>
        /// Returns representation type data that is stored in value(Directly calling function)
        /// </summary>
        RepType GetPiRepType()
        {
            GetVTable(VTable)->VirtualTable_GetPiRepType(VTable);
        }

        /// <summary>
        /// Returns representation type data that is stored in value(Directly calling function)
        /// </summary>
        RepType GetERepType()
        {
            GetVTable(VTable)->VirtualTable_GetERepType(VTable);
        }

        /// <summary>
        /// Returns representation type data that is stored in value(Directly calling function)
        /// </summary>
        RepType GetIRepType()
        {
            GetVTable(VTable)->VirtualTable_GetIRepType(VTable);
        }

        /// <summary>
        /// Returns representation type data that is stored in value(Directly calling function)
        /// </summary>
        RepType GetNormRepType()
        {
            GetVTable(VTable)->VirtualTable_GetNormRepType(VTable);
        }

        /// <summary>
        /// Returns representation type data that is stored in value(Directly calling function)
        /// </summary>
        RepType GetRepType()
        {
            GetVTable(VTable)->VirtualTable_GetRepType(VTable);
        }

        #endif

        #pragma endregion RepType

        #pragma region RangeLimits
        #pragma endregion RangeLimits

        #pragma region PiNum Setters
        #if defined(AltNum_EnablePiRep)
    protected:
        template<MediumDecVariant VariantType = MediumDecBaseV2>
        void SetPiValV1(const VariantType& Value)
        {
            IntHalf = Value.IntHalf; DecimalHalf = PartialInt(Value.DecimalHalf.Value, 1);
        }
    public:
        constexpr auto SetPiVal = SetPiValV1<MediumDecV2Base>;

        void SetPiValFromInt(int Value)
        {
            IntHalf = Value.IntHalf; DecimalHalf = PartialInt(0, 1);
        }
        #endif
        #pragma endregion PiNum Setters

        #pragma region ENum Setters
        #if defined(AltNum_EnableERep)
    protected:
        template<MediumDecVariant VariantType = MediumDecBaseV2>
        void SetEValV1(const VariantType& Value)
        {
            IntHalf = Value.IntHalf; DecimalHalf = PartialInt(Value.DecimalHalf.Value, 2);
        }
    public:
        constexpr auto SetEVal = SetEValV1<MediumDecV2Base>;

        void SetEValFromInt(int Value)
        {
            IntHalf = Value.IntHalf; DecimalHalf = PartialInt(0, 2);
        }
        #endif
        #pragma endregion ENum Setters

        #pragma region INum Setters
        #if defined(AltNum_EnableIRep)
    protected:
        template<MediumDecVariant VariantType = MediumDecBaseV2>
        void SetIValV1(const VariantType& Value)
        {
            IntHalf = Value.IntHalf; DecimalHalf = PartialInt(Value.DecimalHalf.Value, 3);
        }
    public:
        constexpr auto SetIVal = SetEValV1<MediumDecV2Base>;

        void SetIValFromInt(int Value)
        {
            IntHalf = Value.IntHalf; DecimalHalf = PartialInt(0, 3);
        }
        #endif
        #pragma endregion INum Setters

        #pragma region Infinity Setters
        //Infinity operations based on https://www.gnu.org/software/libc/manual/html_node/Infinity-and-NaN.html
        // and https://tutorial.math.lamar.edu/classes/calcI/typesofinfinity.aspx
        #if defined(AltNum_EnableInfinityRep)

        void SetAsInfinity()
        {
            IntHalf = 1; DecimalHalf = InfinityRep;
        }

        void SetAsNegativeInfinity()
        {
            IntHalf = MirroredInt(1, MirroredInt::NegativeSign); DecimalHalf = InfinityRep;
        }

        #if defined(AltNum_EnableIRep)

        void SetAsImaginaryInfinity()
        {
            IntHalf = 1; DecimalHalf = PartialInt(InfinityRep, 3);
        }

        void SetAsNegativeImaginaryInfinity()
        {
            IntHalf = MirroredInt(1, MirroredInt::NegativeSign); DecimalHalf = PartialInt(InfinityRep, 3);
        }

        #endif

        #endif
        #pragma endregion Infinity Setters

        #pragma region ApproachingZero Setters
        #if defined(AltNum_EnableApproaching)

        //Alias:SetAsApproachingValueFromRight, Alias:SetAsApproachingZero if value = 0
        //Approaching Towards values from right to left side(IntHalf.000...1)
        void SetAsApproachingBottom(const MirroredInt& value = MirroredInt::Zero)
        {
            IntHalf = value; DecimalHalf = ApproachingBottomRep;
        }

        #if !defined(AltNum_DisableApproachingTop)
        //Alias:SetAsApproachingValueFromLeft, Alias:SetAsApproachingZeroFromLeft if value = 0
        //Approaching Towards (IntHalf-1) from Left to right side(IntHalf.999...9)
        void SetAsApproachingTop(const MirroredInt& value = MirroredInt::Zero)
        {
            IntHalf = value; DecimalHalf = ApproachingTopRep;
        }
        #endif

        //SetAsApproachingBottom without changing sign
        void SetAsApproachingBottomVal(const unsigned int& value = 0)
        {
            IntHalf.Value = value; DecimalHalf = ApproachingBottomRep;
        }

        #if !defined(AltNum_DisableApproachingTop)
        //SetAsApproachingTop without changing sign
        void SetAsApproachingTopVal(const unsigned int& value = 0)
        {
            IntHalf.Value = value; DecimalHalf = ApproachingTopRep;
        }
        #endif

        //SetAsApproachingBottom without changing sign or flags
        void SetAsApproachingBottomValV2(const unsigned int& value = 0)
        {
            IntHalf.Value = value; DecimalHalf.Value = ApproachingBottomRep;
        }

        #if !defined(AltNum_DisableApproachingTop)
        //SetAsApproachingTop without changing sign
        void SetAsApproachingTopValV2(const unsigned int& value = 0)
        {
            IntHalf.Value = value; DecimalHalf.Value = ApproachingTopRep;
        }
        #endif

        #if defined(AltNum_EnablePiRep)

        void SetAsApproachingBottomPi(const MirroredInt& value = MirroredInt::Zero)
        {
            IntHalf = value; DecimalHalf = PartialInt(ApproachingBottomRep, 1);
        }

        #if !defined(AltNum_DisableApproachingTop)
        void SetAsApproachingTopPi(const MirroredInt& value = MirroredInt::Zero)
        {
            IntHalf = value; DecimalHalf = PartialInt(ApproachingBottomRep, 1);
        }
        #endif

        #endif

        #if defined(AltNum_EnableERep)

        void SetAsApproachingBottomE(const MirroredInt& value = MirroredInt::Zero)
        {
            IntHalf = value; DecimalHalf = PartialInt(ApproachingBottomRep, 2);
        }

        #if !defined(AltNum_DisableApproachingTop)
        //Approaching Towards (IntHalf-1) from Left to right side(IntHalf.999...9)e
        void SetAsApproachingTopE(const MirroredInt& value = MirroredInt::Zero)
        {
            IntHalf = value; DecimalHalf = PartialInt(ApproachingTopRep, 2);
        }
        #endif

        #endif

        #if defined(AltNum_EnableIRep)

        void SetAsApproachingBottomI(const MirroredInt& value = MirroredInt::Zero)
        {
            IntHalf = value; DecimalHalf = PartialInt(ApproachingBottomRep, 3);
        }

        #if !defined(AltNum_DisableApproachingTop)
        //Approaching Towards (IntHalf-1) from Left to right side(IntHalf.999...9)i
        void SetAsApproachingTopI(const MirroredInt& value = MirroredInt::Zero)
        {
            IntHalf = value; DecimalHalf = PartialInt(ApproachingTopRep, 3);
        }
        #endif

        #endif
        #endif
        #pragma endregion ApproachingZero Setters

        #pragma region NaN Setters
        #if defined(AltNum_EnableNaN)
        void SetAsNaN()
        {
            IntHalf = 0; DecimalHalf = NaNRep;
        }

        void SetAsUndefined()
        {
            IntHalf = 0; DecimalHalf = UndefinedRep;
        }
        #endif
        #pragma endregion NaN Setters

        #pragma region ValueDefines
    private:

        #if defined(AltNum_EnableNaN)
        static MediumDec NaNValue()
        {
            MediumDec NewSelf = MediumDec(0, NaNRep);
            return NewSelf;
        }

        static MediumDec UndefinedValue()
        {
            MediumDec NewSelf = MediumDec(0, UndefinedRep);
            return NewSelf;
        }
        #endif

        static MediumDecV2Base AlmostOneValue();

        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up
        /// (Stored as 3.141592654)
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base PiNumValue();

        //100,000,000xPi(Rounded to 9th decimal digit)
        static MediumDecV2Base HundredMilPiNumValue();

        //10,000,000xPi(Rounded to 9th decimal digit)
        static MediumDecV2Base TenMilPiNumValue();

        //1,000,000xPi(Rounded to 9th decimal digit)
        static MediumDecV2Base OneMilPiNumValue();

        //10xPi(Rounded to 9th decimal digit)
        static MediumDecV2Base TenPiNumValue();

        static MediumDecV2Base ENumValue();

        static MediumDecV2Base ZeroValue();

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base OneValue();

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base TwoValue();

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base NegativeOneValue();

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base Point5Value();

        static MediumDecV2Base JustAboveZeroValue();

        static MediumDecV2Base OneMillionthValue();

        static MediumDecV2Base FiveThousandthValue();

        static MediumDecV2Base FiveMillionthValue();

        static MediumDecV2Base TenMillionthValue();

        static MediumDecV2Base OneHundredMillionthValue();

        static MediumDecV2Base FiveBillionthValue();

        static MediumDecV2Base LN10Value();

        static MediumDecV2Base LN10DivValue();

        static MediumDecV2Base TwiceLN10DivValue();

        static MediumDecV2Base MinimumValue();

        static MediumDecV2Base MaximumValue();

        static MediumDecV2Base NegativePointFiveValue();

        static MediumDecV2Base NegativePointFive;

        static MediumDecV2Base AlmostOne;

        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up to 3.141592654
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base PiNum;

        /// <summary>
        /// Euler's number (Non-Alternative Representation)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base ENum;

        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) Representation
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base Pi;

        /// <summary>
        /// Euler's number (Non-Alternative Representation)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base E;

        /// <summary>
        /// Returns the value at zero
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base Zero;

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base One;

        /// <summary>
        /// Returns the value at two
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base Two;

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base PointFive;

        /// <summary>
        /// Returns the value at digit one more than zero (0.000000001)
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base JustAboveZero;

        /// <summary>
        /// Returns the value at .000000005
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base FiveBillionth;

        /// <summary>
        /// Returns the value at .000001000
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base OneMillionth;

        /// <summary>
        /// Returns the value at "0.005"
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base FiveThousandth;

        /// <summary>
        /// Returns the value at .000000010
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base OneGMillionth;

        //0e-7
        static MediumDecV2Base TenMillionth;

        /// <summary>
        /// Returns the value at "0.000005"
        /// </summary>
        static MediumDecV2Base FiveMillionth;

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        static MediumDecV2Base NegativeOne;

        /// <summary>
        /// Returns value of lowest non-infinite/Special Decimal State Value that can store
        /// (-2147483647.999999999)
        /// </summary>
        static MediumDecV2Base Minimum;

        /// <summary>
        /// Returns value of highest non-infinite/Special Decimal State Value that can store
        /// (2147483647.999999999)
        /// </summary>
        static MediumDecV2Base Maximum;

        /// <summary>
        /// 2.3025850929940456840179914546844
        /// (Based on https://stackoverflow.com/questions/35968963/trying-to-calculate-logarithm-base-10-without-math-h-really-close-just-having)
        /// </summary>
        static MediumDecV2Base LN10;

        /// <summary>
        /// (1 / Ln10) (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static MediumDecV2Base LN10Div;

        /// <summary>
        /// (1 / Ln10)*2 (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static MediumDecV2Base TwiceLN10Div;

        #pragma endregion ValueDefines

        #pragma region String Commands
    protected:

        void InitialyzeAltRepFromString(const std::string& Value)
        {
            #if defined(AltNum_EnablePiRep)
            if (str.find("Pi") != std::string::npos)
                DecimalHalf.Flags = 1;
            #endif
            #if defined(AltNum_EnableERep)
            if (Value.last() == 'e')
                DecimalHalf.Flags = 2;
            #endif
            #if defined(AltNum_EnableIRep)
            if (Value.last() == 'i')
                DecimalHalf.Flags = 3;
            #endif
        }

    public:

        /// <summary>
        /// Reads the string.
        /// </summary>
        /// <param name="Value">The value.</param>
        void ReadString(const std::string& Value)
        {
            MediumDecBase::ReadString(Value);
            InitialyzeAltRepFromString(Value);
        }

        /// <summary>
        /// Converts to string.
        /// </summary>
        /// <returns>std.string</returns>
        std::string ToBasicString() { return MediumDecBase::ToString(); }

        /// <summary>
        /// Converts to string with digits filled in even when empty
        /// </summary>
        /// <returns>std.string</returns>
        std::string ToFullBasicString() { return MediumDecBase::ToFullString(); }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class from string literal
        /// </summary>
        /// <param name="strVal">The value.</param>
        MediumDecV2Base(const char* strVal)
        {
            std::string Value = strVal;
            this->ReadString(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecV2Base(const std::string& Value)
        {
            this->ReadString(Value);
        }

        std::string ConvertToBasicString(const RepType& repType)
        {
            auto self = ConvertAsNormType(repType);
            return self.BasicToString();
        }

        /// <summary>
        /// Implements the operator std::string operator.
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator std::string() { return ToString(); }

    #pragma endregion String Commands

    #pragma region ConvertFromOtherTypes

        MediumDecV2Base(const unsigned __int64& Value){ this->SetUIntVal(Value); }
        MediumDecV2Base(const signed __int64& Value){ this->SetIntVal(Value); }
        MediumDecV2Base(const unsigned char& Value){ this->SetUIntVal(Value); }
        MediumDecV2Base(const signed char& Value){ this->SetIntVal(Value); }
        MediumDecV2Base(const unsigned short& Value){ this->SetUIntVal(Value); }
        MediumDecV2Base(const signed short& Value){ this->SetIntVal(Value); }
        MediumDecV2Base(const unsigned int& Value){ this->SetUIntVal(Value); }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecV2Base(const bool& Value){ this->SetBoolVal(Value); }

    #if defined(AltNum_EnableFloatingConversion)
	
        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecBase(const float& Value){ this->SetFloatVal(Value); }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecBase(const double& Value){ this->SetDoubleVal(Value); }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecBase(const long double& Value){ this->SetDecimalVal(Value); }
		
	#endif

    #pragma endregion ConvertFromOtherTypes

    #pragma region ConvertToOtherTypes

        /// <summary>
        /// MediumDec Variant to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        int toInt() const { return IntHalf.GetValue(); }

        /// <summary>
        /// MediumDec Variant to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        int toUInt() const { return IntHalf.IsNegative()?0:IntHalf.Value; }

        bool toBool() const { return IntHalf.IsZero() ? false : true; }

    #if defined(AltNum_EnableFloatingConversion)

        /// <summary>
        /// MediumDec Variant to float explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator float() { return toFloat(); }
		
        /// <summary>
        /// MediumDec Variant to double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator double() { return toDouble(); }
		
        /// <summary>
        /// MediumDec Variant to decimal explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator long double() { return toDecimal(); }

    #endif

        /// <summary>
        /// MediumDec Variant to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed int() { return toInt(); }

        /// <summary>
        /// MediumDec Variant to uint explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator unsigned int() { return toUInt(); }

        /// <summary>
        /// MediumDec Variant to int64 explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed __int64() { return toInt(); }

        /// <summary>
        /// MediumDec Variant to ubyte explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator unsigned char() { return toUInt(); }

        /// <summary>
        /// MediumDec Variant to byte explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed char() { return toInt(); }

        /// <summary>
        /// MediumDec Variant to ushort explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator unsigned short() { return toUInt(); }

        /// <summary>
        /// MediumDec Variant to short explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator signed short() { return toInt(); }

        /// <summary>
        /// MediumDec Variant to bool explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator bool() { return toBool(); }

    #pragma endregion ConvertToOtherTypes

    #pragma region Pi Conversion
    #if defined(AltNum_EnablePiRep)

    void ConvertPiToNum();

    #endif
    #pragma endregion Pi Conversion

    #pragma region E Conversion
    #if defined(AltNum_EnableERep)

    void ConvertEToNum();

    #endif
    #pragma endregion E Conversion

    #pragma region Imaginary Conversion
    #if defined(AltNum_EnableIRep)

        void ConvertIRepToINum(const RepType& repType)
        {//Assuming not zero(should not reach needing to convert the representation if RValue is zero)
            switch (repType)
            {
                case RepType::INum:
                    break;
			/*
            #if defined(AltNum_EnableDecimaledIFractionals)
                case RepType::INumByDiv://(Value/(ExtraRep.Value))*i Representation
                    {
                        BasicUnsignedDivOp(ExtraRep.Value);
                        ExtraRep = InitialExtraRep;
                    }
                    break;
            #endif*/
            #if defined(AltNum_EnableApproaching)
            case RepType::ApproachingImaginaryBottom:
                DecimalHalf.Value = 1;
                break;
                #if !defined(AltNum_DisableApproachingTop)
            case RepType::ApproachingImaginaryTop:
                DecimalHalf.Value = 999999999;
                break;
                #endif
                /*#if defined(AltNum_EnableApproachingDivided)
            case RepType::ApproachingImaginaryMidLeft:
                ConvertFromApproachingIMidLeftToNorm(); break;
					#if !defined(AltNum_DisableApproachingTop)
            case RepType::ApproachingImaginaryMidRight:
                ConvertFromApproachingIMidRightToNorm(); break;
					#endif
				#endif*/
			#endif
            #if defined(AltNum_EnableInfinityRep)
            case RepType::ImaginaryInfinity:
                IntHalf = IsPositive()?MaxIntHalf:MinIntHalf;
                DecimalHalf.Value = 999999999;
                /*ExtraRep = InitialExtraRep;*/
                break;
            #endif
            #ifdef AltNum_EnableComplexNumbers
                case RepType::ComplexIRep:
                {
                    throw "Conversion from complex number to real number not supported yet.";
                    break;
                }
            #endif
                default:
                    throw "Conversion not supported.";
                    break;
            }
        }

		auto ConvertAsNormalIRep(const RepType& repType)
        {
            auto Res = *this;
            Res.ConvertIRepToINum(repType);
            return Res;
        } const

    #endif
    #pragma endregion region Imaginary Conversion


    #pragma region Other RepType Conversion

        //Returns value as normal type or INum representation
        void ConvertToNormType(const RepType& repType)
        {
            switch (repType)
            {
            case RepType::NormalType:
                break;
	#if defined(AltNum_EnablePiRep)
            case RepType::PiNum:
                ConvertPiToNum(); break;
		#if defined(AltNum_EnableApproaching)
            case RepType::ApproachingBottomPi:
                DecimalHalf.Value = 1;
                break;
			#if !defined(AltNum_DisableApproachingTop)
            case RepType::ApproachingTopPi:
                DecimalHalf.Value = 999999999;
                break;
			#endif
		#endif
	#endif
	#if defined(AltNum_EnableERep)
            case RepType::ENum:
                ConvertENumToNum(); break;
		#if defined(AltNum_EnableApproaching)
            case RepType::ApproachingBottomPi:
                DecimalHalf.Value = 1;
                break;
			#if !defined(AltNum_DisableApproachingTop)
            case RepType::ApproachingTopPi:
                DecimalHalf.Value = 999999999;
                break;
			#endif
		#endif
	#endif
	#if defined(AltNum_EnableInfinityRep)
			case RepType::Infinity:
				IntHalf = IsPositive()?MaxIntHalf:MinIntHalf;
				DecimalHalf = 999999999;
				break;
	#endif
	#if defined(AltNum_EnableApproaching)
			case RepType::ApproachingBottom:
				DecimalHalf = 1;
				break;
		#if !defined(AltNum_DisableApproachingTop)
			case RepType::ApproachingTop:
				DecimalHalf = 999999999;
				break;
		#endif
	#endif
	#if defined(AltNum_EnableIRep)
			case RepType::INum:
				break;
		#if defined(AltNum_EnableApproaching)
			case RepType::ApproachingImaginaryBottom:
				DecimalHalf.Value = 1;
				break;
			#if !defined(AltNum_DisableApproachingTop)
			case RepType::ApproachingImaginaryTop:
				DecimalHalf.Value = 999999999;
				break;
			#endif
		#endif
		#if defined(AltNum_EnableInfinityRep)
			case RepType::ImaginaryInfinity:
				IntHalf = IsPositive()?MaxIntHalf:MinIntHalf;
				DecimalHalf.Value = 999999999;
				break;
		#endif
		#ifdef AltNum_EnableComplexNumbers
			case RepType::ComplexIRep:
				throw "Conversion from complex number to real number not supported yet.";
				break;
		#endif
	#endif
	#ifdef AltNum_EnableComplexNumbers
            case RepType::ComplexIRep:
                throw "Conversion from complex number to real number not supported yet.";
                break;
	#endif
            default:
                throw "Conversion to normal number not supported yet?";
                break;
            }
        } const

		//Returns value as normal type or INum representation
        auto ConvertAsNormType(const RepType& repType)
        {
            auto Res = *this;
            Res.ConvertToNormType(repType);
            return Res;
        }

        //Converts value to normal type representation
        void ConvertToNormTypeV2()
        {
            RepType repType = GetRepType();
            ConvertToNormType(repType);
        }

		//Returns value as normal type representation
        auto ConvertAsNormTypeV2()
        {
            auto Res = *this;
            Res.ConvertToNormTypeV2();
            return Res;
        }

	#if defined(AltNum_EnablePiRep)||defined(AltNum_EnableERep)||defined(AltNum_EnableIRep)
        RepType GetRepAsNormalEquavant(const RepType& repType)
        {
			switch(repType)
			{
		#if defined(AltNum_EnablePiRep)
				case RepType::PiNum:
		#endif
		#if defined(AltNum_EnableERep)
				case RepType::ENum:
		#endif
		#if defined(AltNum_EnableERep)
				case RepType::INum:
		#endif
					return RepType::NormalType; break;
		#if defined(AltNum_EnableApproaching)
			#if defined(AltNum_EnablePiRep)
				case RepType::ApproachingBottomPi:
			#endif
			#if defined(AltNum_EnableERep)
				case RepType::ApproachingBottomE:
			#endif
			#if defined(AltNum_EnableIRep)
				case RepType::ApproachingImaginaryBottom:
			#endif
					return RepType::ApproachingBottom; break;
			#if !defined(AltNum_DisableApproachingTop
				#if defined(AltNum_EnablePiRep)
				case RepType::ApproachingTopPi:
				#endif
				#if defined(AltNum_EnableERep)
				case RepType::ApproachingTopE:
				#endif
				#if defined(AltNum_EnableIRep)
				case RepType::ApproachingImaginaryBottom:
				#endif
					return RepType::ApproachingTop; break;
			#endif
		#endif
		#if defined(AltNum_EnableImaginaryInfinity)
				case RepType::ImaginaryInfinity:
					return RepType::Infinity; break;
		#endif
				default:
					return repType;
			}
		}
	#endif

	#if defined(AltNum_EnablePiRep)||defined(AltNum_EnableERep)
        RepType ConvertToNormalEquivalant(const RepType& repType)
        {
			switch(repType)
			{
		#if defined(AltNum_EnablePiRep)
				case RepType::PiNum:{
					BasicUnsignedMultOp(PiNum); DecimalHalf.Flags = 0;
					return RepType::NormalType;
				}break;
		#endif
		#if defined(AltNum_EnableERep)
				case RepType::ENum:{
					BasicUnsignedMultOp(ENum); DecimalHalf.Flags = 0;
					return RepType::NormalType;
				}	break;
		#endif
			#if defined(AltNum_EnableApproaching)
				#if defined(AltNum_EnablePiRep)
				case RepType::ApproachingBottomPi:
				#endif
				#if defined(AltNum_EnableERep)
				case RepType::ApproachingBottomE:
				#endif
					if(IntHalf.Value==0)
					{
						DecimalHalf.Flags = 0;
						return RepType::ApproachingBottom;
					}
					else
					{
						ConvertToNormType(repType);
						return RepType::NormalType;
					}
					break;
				#if !defined(AltNum_DisableApproachingTop)
					#if defined(AltNum_EnablePiRep)
				case RepType::ApproachingTopPi:
					#endif
					#if defined(AltNum_EnableERep)
				case RepType::ApproachingTopE:
					#endif
					ConvertToNormType(repType);
					return RepType::NormalType;
					break;
				#endif
			#endif
				default:
					return repType;
			}
		}

		//Returns std::pair of Value and RepType
        auto ConvertAsNormalEquivalant(const RepType& repType)
        {
            auto Res = *this;
            RepType convertedRep = ConvertToNormalEquivalant(repType, convertedRep);
            return std::make_pair(Res, convertedRep);
		}
	#endif

    #pragma endregion Other RepType Conversion

    #pragma region Comparison Operators
protected:

		std::strong_ordering BasicComparison(const MediumDecV2Base& that) const
		{
            return BasicComparisonV1(that);
        }

    #if defined(AltNum_EnableInfinityRep))
		template<MediumDecVariant VariantType=MediumDecV2Base>
		std::strong_ordering LSideInfinityComparison(const VariantType& that, const RepType& RRep) const
		{
			if(IntHalf.IsPositive())
				if(RRep==RepType:Infinity&&that.IntHalf.IsPositive())
					return 0<=>0;
				else
        #if defined(AltNum_UseInvertedSign)
					return MirroredInt::PositiveSign<=>MirroredInt::NegativeSign;
		#else
					return MirroredInt::NegativeSign<=>MirroredInt::PositiveSign;
		#endif
			else
				if(RRep==RepType:Infinity&&that.IntHalf.IsNegative())
					return 0<=>0;
				else
		#if defined(AltNum_UseInvertedSign)
					return MirroredInt::NegativeSign<=>MirroredInt::PositiveSign;
		#else
					return MirroredInt::PositiveSign<=>MirroredInt::NegativeSign;
		#endif
		}
	#endif

		//Templated version of Spaceship operator to allow full version of class to inherit the spaceship operator code
		template<MediumDecVariant VariantType=MediumDecV2Base>
		std::strong_ordering CompareWithV1(const VariantType& that) const
		{
	#if defined(MediumDecV2_EnableWithinMinMaxRange)
			if(DecimalHalf.Flag==3) {
				if(that.DecimalHalf.Flag==3) {
					//To-do compare within min-max range code here
				}
				else {
					//To-do compare within min-max range code here
				}
			}
			else if(that.DecimalHalf.Flag==3) {
				//To-do compare within min-max range code here
			}
	#endif
			//Comparing if number is negative vs positive
    #if defined(AltNum_UseInvertedSign)
			auto SignCmp = Sign <=> that.Sign;
    #else   //(inverted comparison so sign of zero==positive)
            auto SignCmp = that.Sign <=> Sign;
    #endif
	        if (SignCmp != 0)
				return SignCmp;
			RepType LRep = GetRepType();
			RepType RRep = that.GetRepType();
    #if defined(AltNum_EnableNaN)||defined(AltNum_EnableNilRep)||defined(AltNum_EnableUndefinedButInRange)
			if(LRep^UndefinedBit||RRep^UndefinedBit)
				throw "Can't compare undefined/nil representations";
    #endif
    #if defined(AltNum_EnableIRep)
            if (LValue.DecimalHalf.Flags == 3)
            {
                if(RValue.Flags!=3)
                    throw "Can't compare imaginary number with real number";
				else if(LRep==RepType:ImaginaryInfinity){
					if(RRep==RepType:ImaginaryInfinity)
						return SignCmp;
					else{
			#if defined(AltNum_UseInvertedSign)
						if(IsPositive())//+Inf i vs Any non-inf imaginary
							return MirroredInt::PositiveSign<=>MirroredInt::NegativeSign;
						else//-Inf i vs Any non-inf imaginary
							return MirroredInt::NegativeSign<=>MirroredInt::PositiveSign;
			#else//Inverted order compared to if sign of positive == 1
						if(IsPositive())
							return MirroredInt::NegativeSign<=>MirroredInt::PositiveSign;
						else
							return MirroredInt::PositiveSign<=>MirroredInt::NegativeSign;
			#endif
					}
				} else if(RRep==RepType:ImaginaryInfinity)
                {
			#if defined(AltNum_UseInvertedSign)
						if(that.IsPositive())//Any non-inf imaginary vs +Inf i
							return MirroredInt::NegativeSign<=>MirroredInt::PositiveSign;
						else//Any non-inf imaginary vs -Inf i
							return MirroredInt::PositiveSign<=>MirroredInt::NegativeSign;
			#else
						if(that.IsPositive())
							return MirroredInt::PositiveSign<=>MirroredInt::NegativeSign;
						else
							return MirroredInt::NegativeSign<=>MirroredInt::PositiveSign;
			#endif
                }
                else
			    	return BasicComparisonV2(rSide);
            }
            else if(RValue.Flags==3)
                throw "Can't compare imaginary number with real number";
    #endif
			switch(LRep)
			{
	#if defined(AltNum_EnableInfinityRep)
                case RepType:Infinity:
                    LSideInfinityComparison(that, RRep);
                    break;
	#endif
	#if defined(AltNum_EnableApproaching)

	#endif
				default:
				{
					if(LRep==RRep)
						return BasicComparisonV2(that);
					else if(RRep==RepType:Infinity)
                    {
			#if defined(AltNum_UseInvertedSign)
                        if(that.IntHalf.IsPositive())
							return MirroredInt::NegativeSign<=>MirroredInt::PositiveSign;//Positive Infinity is greater than real number representations
						else
							return MirroredInt::PositiveSign<=>MirroredInt::NegativeSign;
			#else
                        if(that.IntHalf.IsPositive())
							return MirroredInt::PositiveSign<=>MirroredInt::NegativeSign;
						else
							return MirroredInt::NegativeSign<=>MirroredInt::PositiveSign;
			#endid
                    }
                    else
					{
						auto lSide = *this;
						auto rSide = that;
						lSide.ConvertToNormTypeV2(); rSide.ConvertToNormTypeV2();
						return lSide.BasicComparison(rSide);
					}
				}
			}
		}

		//Templated version of Spaceship operator to allow full version of class to inherit the spaceship operator code
		template<MediumDecVariant VariantType=MediumDecV2Base, IntegerType IntType=signed int>
		std::strong_ordering CompareWithIntV1(const IntType& that) const
		{
    #if defined(AltNum_EnableIRep)
            if(DecimalHalf.Flags==3)
                throw "Can't compare imaginary number with real number except for checking if equal.";
    #endif
			//Pi and E only enabled if imbedded flags are enabled
			if(DecimalHalf.Flags==0)
			{
				return constexpr (std::is_unsigned<IntType>)?BasicUIntComparison(that):BasicIntComparison(that);
			}
			else
			{
				VariantType lSide = *this;
				lSide.ConvertToNormTypeV2();
				return constexpr (std::is_unsigned<IntType>)?lSide.BasicUIntComparison(that):lSide.BasicIntComparison(that);
			}
		}

        std::strong_ordering CompareWith(const MediumDecV2Base& that) const
		{
            return CompareWithV1<MediumDecV2Base>
        }

		std::strong_ordering CompareWithInt(const unsigned int& that) const
		{
            return CompareWithIntV1(that);
        }

		std::strong_ordering CompareWithInt(const signed int& that) const
		{
            return CompareWithIntV1(that);
        }

public:
		std::strong_ordering operator<=>(const MediumDecV2Base& that) const
		{
			return CompareWith(that);
		}

		std::strong_ordering operator<=>(const int& that) const
		{
    #if defined(AltNum_EnableIRep)
            if(DecimalHalf.Flags==3)
                throw "Can't compare imaginary number with real number except for checking if equal.";
    #endif
            MediumDecV2Base LValue = *this;
            LValue.ConvertToNormTypeV2();
			return LValue.CompareWithInt(that);
		}

		bool operator==(const MediumDecV2Base& that) const
		{
			if (IntHalf!=that.IntHalf)
				return false;
			if (DecimalHalf!=that.DecimalHalf)
				return false;
            return true;
		}

		bool operator!=(const MediumDecV2Base& that) const
		{
			if (IntHalf!=that.IntHalf)
				return true;
			if (DecimalHalf!=that.DecimalHalf)
				return true;
            return false;
		}

		bool operator==(const int& that) const
		{
			if (IntHalf!=that)
				return false;
			if (DecimalHalf!=0)
				return false;
			return true;
		}

		bool operator!=(const int& that) const
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

        /// <summary>
        /// Basic division operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=unsigned int>
        void BasicUIntDivOpV1(const IntType& Value)
        {
            MediumDecBase::UIntDivOpV1(Value);
        }
		
        template<IntegerType IntType=signed int>
        void BasicIntDivOpV1(const IntType& Value)
        {
            MediumDecBase::IntDivOpV1(Value);
        }

        template<MediumDecVariant VariantType=MediumDecV2Base, IntegerType IntType=unsigned int>
        VariantType& BasicUIntDivOperationV1(const IntType& rValue)
        { UIntDivOpV1(rValue); return *this; }

        template<MediumDecVariant VariantType=MediumDecV2Base, IntegerType IntType=unsigned int>
        VariantType& BasicIntDivOperationV1(const IntType& rValue)
        { BasicIntDivOpV1(rValue); return *this; }

        /// <summary>
        /// Basic division operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<MediumDecVariant VariantType=MediumDecV2Base, IntegerType IntType=unsigned int>
        const VariantType BasicDivideByUIntV1(const IntType& rValue)
        { auto self = *this; return self.BasicUIntDivOperationV1(rValue); }

        /// <summary>
        /// Basic division operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<MediumDecVariant VariantType=MediumDecV2Base, IntegerType IntType=signed int>
        const VariantType BasicDivideByIntV1(const IntType& rValue)
        { auto self = *this; return self.BasicIntDivOperationV1(rValue); }

public:

        void BasicUIntDivOp(const unsigned int& rValue) { BasicUIntDivOpV1(rValue); }
        void BasicIntDivOp(const signed int& rValue) { BasicIntDivOpV1(rValue); }
        void BasicUInt64DivOp(const unsigned __int64& rValue) { BasicUIntDivOpV1(rValue); }
        void BasicInt64DivOp(const signed __int64& rValue) { BasicIntDivOpV1(rValue); }

        void BasicUnsignedIntDivOp(const signed int& rValue) { BasicUIntDivOpV1(rValue); }
        void BasicUnsignedInt64DivOp(const signed __int64& rValue) { BasicUIntDivOpV1(rValue); }

        void BasicUInt8DivOp(const unsigned char& rValue) { BasicUIntDivOpV1(rValue); }
        void BasicInt8DivOp(const signed char& rValue) { BasicIntDivOpV1(rValue); }
        void BasicUInt16DivOp(const unsigned short& rValue) { BasicUIntDivOpV1(rValue); }
        void BasicInt16DivOp(const signed short& rValue) { BasicIntDivOpV1(rValue); }

        MediumDecV2Base& BasicUIntDivOperation(const unsigned int& rValue) { return BasicUIntDivOperationV1(rValue); }
        MediumDecV2Base& BasicIntDivOperation(const signed int& rValue) { return BasicIntDivOperationV1(rValue); }
        MediumDecV2Base& BasicUInt64DivOperation(const unsigned __int64& rValue) { return BasicUIntDivOperationV1(rValue); }
        MediumDecV2Base& BasicInt64DivOperation(const signed __int64& rValue) { return BasicIntDivOperationV1(rValue); }
        MediumDecV2Base& BasicUInt8DivOperation(const unsigned char& rValue) { return BasicUIntDivOperationV1(rValue); }
        MediumDecV2Base& BasicInt8DivOperation(const signed char& rValue) { return BasicIntDivOperationV1(rValue); }
        MediumDecV2Base& BasicUInt16DivOperation(const unsigned short& rValue) { return BasicUIntDivOperationV1(rValue); }
        MediumDecV2Base& BasicInt16DivOperation(const signed short& rValue) { return BasicIntDivOperationV1(rValue); }

        const MediumDecV2Base BasicDivideByUInt(const unsigned int& rValue) { return BasicDivideByUIntV1(rValue); }
        const MediumDecV2Base BasicDivideByInt(const signed int& rValue) { return BasicDivideByIntV1(rValue); }
        const MediumDecV2Base BasicDivideByUInt64(const unsigned __int64& rValue) { return BasicDivideByUIntV1(rValue); }
        const MediumDecV2Base BasicDivideByInt64(const signed __int64& rValue) { return BasicDivideByIntV1(rValue); }

        const MediumDecV2Base BasicUnsignedDivideByInt(const signed int& rValue) { return BasicDivideByUIntV1(rValue); }
        const MediumDecV2Base BasicUnsignedDivideByInt64(const signed __int64& rValue) { return BasicDivideByUIntV1(rValue); }

        const MediumDecV2Base BasicDivideByUInt8(const unsigned char& rValue) { return BasicDivideByUIntV1(rValue); }
        const MediumDecV2Base BasicDivideByInt8(const signed char& rValue) { return BasicDivideByIntV1(rValue); }
        const MediumDecV2Base BasicDivideByUInt16(const unsigned short& rValue) { return BasicDivideByUIntV1(rValue); }
        const MediumDecV2Base BasicDivideByInt16(const signed short& rValue) { return BasicDivideByIntV1(rValue); }

    #pragma endregion NormalRep Integer Division Operations

	#pragma region NormalRep AltNum Division Operations

		/// <summary>
        /// Basic unsigned division operation(main code block)
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The rValue</param>
        bool BasicUnsignedPartialDivOp(const MediumDecV2Base& rValue){ return MediumDecBase::UnsignedPartialDivOpV1(rValue); }
		
		/// <summary>
        /// Unsigned division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side value</param>
        void BasicUnsignedDivOp(const MediumDecV2Base& rValue){ MediumDecBase::UnsignedDivOpV1(rValue); }
		
		/// <summary>
        /// Basic division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        void BasicDivOp(const MediumDecV2Base& rValue){ MediumDecBase::DivOpV1(rValue); }

		/// <summary>
        /// Basic unsigned division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDecV2Base& BasicUnsignedDivOperation(const MediumDecV2Base& rValue)
		{ MediumDecBase::UnsignedDivOp(rValue); return *this; }

		/// <summary>
        /// Basic division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDecV2Base& BasicDivOperation(const MediumDecV2Base& rValue)
		{ MediumDecBase::DivOp(rValue); return *this; }

		/// <summary>
        /// Basic unsigned division operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        const MediumDecV2Base BasicDivideByUnsigned(const MediumDecV2Base& rValue)
        { MediumDecV2Base lValue = *this; return lValue.UnsignedDivOperation(rValue); }

		/// <summary>
        /// Basic division operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        const MediumDecV2Base BasicDivideBy(const MediumDecV2Base& rValue)
        { MediumDecV2Base lValue = *this; return lValue.DivOperation(rValue); }

	#pragma endregion NormalRep AltNum Division Operations

	#pragma region Other Division Operations

        /// <summary>
        /// Simplified division by 2 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side value</param>
        /// <returns>MediumDecV2Base&</returns>
        void DivideByTwo();

        /// <summary>
        /// Simplified division by 4 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side value</param>
        /// <returns>MediumDecV2Base&</returns>
        void DivideByFour();

        //Return copy of result divided by two
        MediumDecV2Base DividedByTwo() const;

        //Return copy of result divided by four
        MediumDecV2Base DividedByFour() const;

protected:

        /// <summary>
        /// Unsigned division operation between MediumDec variant and unsigned integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecVariant&</returns>
        template<IntegerType IntType= unsigned int>
        void UIntDivOpV1(const IntType& rValue)
		{
            if (rValue == 1)
                return;
            if (rValue == 0)
            {
                #if defined(AltNum_EnableInfinityRep)&&defined(AltNum_DefineDivideByZeroAsInfinity)
                if (IntHalf < 0)
                    SetAsNegativeInfinity();
                else
                    SetAsInfinity();
                return *this;
                #else
                throw "Target rValue can not be divided by zero";
                #endif
            }
        	switch(DecimalHalf.Flags)
        	{
        #if defined(AltNum_EnablePiRep)
        		case 1:{
                    RepType LRep = rValue.GetPiRepType();
                    switch(LRep)
                    {
                        case RepType::PiNum:{
                            BasicUIntDivOp(rValue);
                        } break;
    #pragma region AltDecVariantExclusive
    #pragma endregion AltDecVariantExclusive
            #if defined(AltNum_EnableApproaching)
                        case RepType::ApproachingBottomPi://(Approaching Towards Zero);(IntHalf of 0 results in 0.0...01)
                        {
                            if (IsAtZeroInt())
                                return *this;
                            ConvertToNormType(LRep);
                            BasicUIntDivOp(rValue);
                        }
                        break;
                        #if !defined(AltNum_DisableApproachingTop)
                        case RepType::ApproachingTopPi://(Approaching Away from Zero);(IntHalf of 0 results in 0.99...9)
                        #endif
    #pragma region AltDecVariantExclusive
    #pragma endregion AltDecVariantExclusive
                        {
                            ConvertToNormType(LRep);
                            BasicUIntDivOp(rValue);
                        } break;
            #endif
                        default:
                            throw "Unable to perform integer division on current representation.";
                    }
                } break;
        #endif
        #if defined(AltNum_EnableERep)
        		case 2:{
                    RepType LRep = rValue.GetERepType();
                    switch(LRep)
                    {
                        case RepType::ENum:{
                            BasicUIntDivOp(rValue);
                        } break;
    #pragma region AltDecVariantExclusive
    #pragma endregion AltDecVariantExclusive
            #if defined(AltNum_EnableApproaching)
                        case RepType::ApproachingBottomE://(Approaching Towards Zero);(IntHalf of 0 results in 0.0...01)
                        {
                            if (IsAtZeroInt())
                                return *this;
                            ConvertToNormType(LRep);
                            BasicUIntDivOp(rValue);
                        }
                        break;
                        #if !defined(AltNum_DisableApproachingTop)
                        case RepType::ApproachingTopE://(Approaching Away from Zero);(IntHalf of 0 results in 0.99...9)
                        #endif
    #pragma region AltDecVariantExclusive
    #pragma endregion AltDecVariantExclusive
                        {
                            ConvertToNormType(LRep);
                            BasicUIntDivOp(rValue);
                        } break;
            #endif
                        default:
                            throw "Unable to perform integer division on current representation.";
                    }
                } break;
        #endif
        #if defined(AltNum_EnableIRep)//IRep_to_integer
        		case 3:{
                    RepType LRep = rValue.GetIRepType();
                    switch(LRep){
                        case RepType::INum:{
                            BasicUIntDivOp(rValue);
                        } break;
    #pragma region AltDecVariantExclusive
    #pragma endregion AltDecVariantExclusive
            #if defined(AltNum_EnableApproaching)
                        case RepType::ApproachingImaginaryBottom://(Approaching Towards Zero);(IntHalf of 0 results in 0.00...1)i
                    #if !defined(AltNum_DisableApproachingTop)
                        case RepType::ApproachingImaginaryTop://(Approaching Away from Zero);(IntHalf of 0 results in 0.99...9)i
                    #endif
    #pragma region AltDecVariantExclusive
    #pragma endregion AltDecVariantExclusive
                        {
                            ConvertToNormalIRep(LRep);
                            BasicUIntDivOp(rValue);
                        }
                        break;
            #endif
            #if defined(AltNum_EnableImaginaryInfinity)
                        case RepType::ImaginaryInfinity:
                            return *this;
                            break;
            #endif
                        default:
                            throw "Unable to perform integer division on current representation.";
                    }
                } break;
        #endif
        		default:{
                    RepType LRep = rValue.GetNormRepType();
                    switch(LRep)
                    {
                        case RepType::NormalType:
                        {
                            BasicUIntDivOp(rValue);
                        }
                        break;
    #pragma region AltDecVariantExclusive
    #pragma endregion AltDecVariantExclusive
            #if defined(AltNum_EnableApproaching)
                        case RepType::ApproachingBottom://(Approaching Towards Zero);(IntHalf of 0 results in 0.0...01)
                        {
                            if (IsAtZeroInt())
                                return *this;
                            ConvertToNormType(LRep);
                            BasicUIntDivOp(rValue);
                        }
                        break;
                        #if !defined(AltNum_DisableApproachingTop)
                        case RepType::ApproachingTop://(Approaching Away from Zero);(IntHalf of 0 results in 0.99...9)
                        #endif
    #pragma region AltDecVariantExclusive
    #pragma endregion AltDecVariantExclusive
                        {
                            ConvertToNormType(LRep);
                            BasicUIntDivOp(rValue);
                        } break;
            #endif
            #ifdef AltNum_EnableInfinity
                        case RepType::Infinity:
                            return *this;
                            break;
            #endif
                        default:
                            throw "Unable to perform integer division on current representation.";
                    }
                } break;
        	}
		}

        /// <summary>
        /// Division operation between MediumDec variant and integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecVariant&</returns>
        template<IntegerType IntType= signed int>
        void IntDivOpV1(const IntType& rValue)
		{
            if(Value<0)
            {
                SwapNegativeStatus();
                UIntDivOpV1(-Value);
            }
            else
                UIntDivOpV1(Value);
		}

        template<IntegerType IntType=unsigned int>
        MediumDecBase& UIntDivOperationV1(const IntType& rValue)
        { UIntDivOpV1(rValue); return *this; }

        template<IntegerType IntType=unsigned int>
        MediumDecBase& IntDivOperationV1(const IntType& rValue)
        { IntDivOpV1(rValue); return *this; }

        /// <summary>
        /// Division operation between MediumDec Variant and unsigned Integer value 
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=unsigned int>
        const auto DivideByUIntV1(const IntType& rValue)
        { auto self = *this; return self.UIntDivOperationV1(rValue); }

        /// <summary>
        /// Division operation between MediumDec Variant and unsigned Integer value 
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=signed int>
        const auto DivideByIntV1(const IntType& rValue)
        { auto self = *this; return self.IntDivOperationV1(rValue); }

public:

        void UIntDivOp(const unsigned int& rValue) { UIntDivOpV1(rValue); }
        void IntDivOp(const signed int& rValue) { IntDivOpV1(rValue); }
        void UInt64DivOp(const unsigned __int64& rValue) { UIntDivOpV1(rValue); }
        void Int64DivOp(const signed __int64& rValue) { IntDivOpV1(rValue); }

        void UnsignedIntDivOp(const signed int& rValue) { UIntDivOpV1(rValue); }
        void UnsignedInt64DivOp(const signed __int64& rValue) { UIntDivOpV1(rValue); }

        void UInt8DivOp(const unsigned char& rValue) { UIntDivOpV1(rValue); }
        void Int8DivOp(const signed char& rValue) { IntDivOpV1(rValue); }
        void UInt16DivOp(const unsigned short& rValue) { UIntDivOpV1(rValue); }
        void Int16DivOp(const signed short& rValue) { IntDivOpV1(rValue); }

        MediumDecV2Base& UIntDivOperation(const unsigned int& rValue) { return UIntDivOperationV1(rValue); }
        MediumDecV2Base& IntDivOperation(const signed int& rValue) { return IntDivOperationV1(rValue); }
        MediumDecV2Base& UInt64DivOperation(const unsigned __int64& rValue) { return UIntDivOperationV1(rValue); }
        MediumDecV2Base& Int64DivOperation(const signed __int64& rValue) { return IntDivOperationV1(rValue); }
        MediumDecV2Base& UInt8DivOperation(const unsigned char& rValue) { return UIntDivOperationV1(rValue); }
        MediumDecV2Base& Int8DivOperation(const signed char& rValue) { return IntDivOperationV1(rValue); }
        MediumDecV2Base& UInt16DivOperation(const unsigned short& rValue) { return UIntDivOperationV1(rValue); }
        MediumDecV2Base& Int16DivOperation(const signed short& rValue) { return IntDivOperationV1(rValue); }

        const MediumDecV2Base DivideByUInt(const unsigned int& rValue) { return DivideByUIntV1(rValue); }
        const MediumDecV2Base DivideByInt(const signed int& rValue) { return DivideByIntV1(rValue); }
        const MediumDecV2Base DivideByUInt64(const unsigned __int64& rValue) { return DivideByUIntV1(rValue); }
        const MediumDecV2Base DivideByInt64(const signed __int64& rValue) { return DivideByIntV1(rValue); }

        const MediumDecV2Base UnsignedDivideByInt(const signed int& rValue) { return DivideByUIntV1(rValue); }
        const MediumDecV2Base UnsignedDivideByInt64(const signed __int64& rValue) { return DivideByUIntV1(rValue); }

        const MediumDecV2Base DivideByUInt8(const unsigned char& rValue) { return DivideByUIntV1(rValue); }
        const MediumDecV2Base DivideByInt8(const signed char& rValue) { return DivideByIntV1(rValue); }
        const MediumDecV2Base DivideByUInt16(const unsigned short& rValue) { return DivideByUIntV1(rValue); }
        const MediumDecV2Base DivideByInt16(const signed short& rValue) { return DivideByIntV1(rValue); }

protected:
		void UnsignedDivOp_RValueIntSwitch(const auto& rValue)
		{
			switch(rValue.IntHalf.Value)
			{
				case 2:
					if(IntHalf&1==1)//Check if number is odd
						UnsignedBasicIntDivOp(2);
					else
						IntHalf.Value /= 2;
					break;
				case 4:
					if(((IntHalf >> 2) << 2) == IntHalf)
						IntHalf.Value /= 4;
					else
						UnsignedBasicIntDivOp(4);
					break;
				case 8:
					if(((IntHalf >> 3) << 3) == IntHalf)
						IntHalf.Value /= 8;
					else
						UnsignedBasicIntDivOp(4);
					break;
				case 16:
					if(((IntHalf >> 4) << 4) == IntHalf)
						IntHalf.Value /= 16;
					else
						UnsignedBasicIntDivOp(4);
					break;
				case 32:
					if(((IntHalf >> 5) << 5) == IntHalf)
						IntHalf.Value /= 32;
					else
						UnsignedBasicIntDivOp(4);
					break;
				case 0:
					throw "Target value can not be divided by zero";
					break;
				default:
					UnsignedBasicIntDivOp(rValue.IntHalf.Value);
					break;
			}
		}

public:

		/// <summary>
        /// Unsigned division operation that ignores special decimal status
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDecV2Base& UnsignedDivOp(const MediumDecV2Base& rValue);

		/// <summary>
        /// Division operation that ignores special decimal status
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        void DivOp(const MediumDecV2Base& Value)
        {
            if(Value.IsNegative())
            {
                SwapNegativeStatus();
                UnsignedMultOp(-Value);
            }
            else
                UnsignedDivOp(Value);
        }

		/// <summary>
        /// Unsigned division operation that ignores special decimal status
        /// Return true if divide into zero
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        auto DivideByUnsigned(const MediumDecV2Base& rValue)
        {
            auto self = *this;
            return self.UnsignedDivOp(rValue);
        }

		/// <summary>
        /// Division operation that ignores special decimal status
        /// Return true if divide into zero
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        auto DivideBy(const MediumDecV2Base& rValue)
        {
            auto self = *this;
            return self.DivOp(rValue);
        }

        /// <summary>
        /// /= operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2Base</returns>
        friend MediumDecV2Base& operator/=(MediumDecV2Base& lValue, const MediumDecV2Base& rValue) { return lValue.DivOperation(rValue); }

        /// <summary>
        /// *= operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2Base</returns>
        friend MediumDecV2Base& operator/=(MediumDecV2Base& lValue, const signed int& rValue) { return lValue.IntDivOperation(rValue); }
        friend MediumDecV2Base& operator/=(MediumDecV2Base& lValue, const signed __int64& rValue) { return lValue.Int64DivOperation(rValue); }
        friend MediumDecV2Base& operator/=(MediumDecV2Base& lValue, const unsigned int& rValue) { return lValue.UIntDivOperation(rValue); }
        friend MediumDecV2Base& operator/=(MediumDecV2Base& lValue, const unsigned __int64& rValue) { return lValue.UInt64DivOperation(rValue); }

        friend MediumDecV2Base& operator/=(MediumDecV2Base& lValue, const signed char& rValue) { return lValue.Int8DivOperation(rValue); }
        friend MediumDecV2Base& operator/=(MediumDecV2Base& lValue, const signed short& rValue) { return lValue.Int16DivOperation(rValue); }
        friend MediumDecV2Base& operator/=(MediumDecV2Base& lValue, const unsigned char& rValue) { return lValue.UInt8DivOperation(rValue); }
        friend MediumDecV2Base& operator/=(MediumDecV2Base& lValue, const unsigned short& rValue) { return lValue.UInt16DivOperation(rValue); }

        /// <summary>
        /// Division operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2Base</returns>
        friend MediumDecV2Base operator/(MediumDecV2Base lValue, const MediumDecV2Base& rValue) { return lValue.DivideBy(rValue); }
		
        /// <summary>
        /// Division operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2Base</returns>
        friend MediumDecV2Base operator/(MediumDecV2Base lValue, const signed int& rValue) { return lValue.IntDivOperation(rValue); }
        friend MediumDecV2Base operator/(MediumDecV2Base lValue, const signed __int64& rValue) { return lValue.Int64DivOperation(rValue); }
        friend MediumDecV2Base operator/(MediumDecV2Base lValue, const unsigned int& rValue) { return lValue.UIntDivOperation(rValue); }
        friend MediumDecV2Base operator/(MediumDecV2Base lValue, const unsigned __int64& rValue) { return lValue.UInt64DivOperation(rValue); }

        friend MediumDecV2Base operator/(MediumDecV2Base lValue, const signed char& rValue) { return lValue.Int8DivOperation(rValue); }
        friend MediumDecV2Base operator/(MediumDecV2Base lValue, const signed short& rValue) { return lValue.Int16DivOperation(rValue); }
        friend MediumDecV2Base operator/(MediumDecV2Base lValue, const unsigned char& rValue) { return lValue.UInt8DivOperation(rValue); }
        friend MediumDecV2Base operator/(MediumDecV2Base lValue, const unsigned short& rValue) { return lValue.UInt16DivOperation(rValue); }
		
        friend MediumDecV2Base operator/(const signed int& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).DivideBy(rValue); }
        friend MediumDecV2Base operator/(const signed __int64& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).DivideBy(rValue); }
        friend MediumDecV2Base operator/(const unsigned int& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).DivideBy(rValue); }
        friend MediumDecV2Base operator/(const unsigned __int64& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).DivideBy(rValue); }

        friend MediumDecV2Base operator/(const signed char& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).DivideBy(rValue); }
        friend MediumDecV2Base operator/(const signed short& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).DivideBy(rValue); }
        friend MediumDecV2Base operator/(const unsigned char& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).DivideBy(rValue); }
        friend MediumDecV2Base operator/(const unsigned short& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).DivideBy(rValue); }

	#pragma endregion Other Division Operations

    #pragma region NormalRep Integer Multiplication Operations
protected:

        /// <summary>
        /// Basic Multiplication operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=unsigned int>
        void BasicUIntMultOpV1(const IntType& Value)
        {
            MediumDecBase::UIntMultOpV1(Value);
        }
		
        template<IntegerType IntType=signed int>
        void BasicIntMultOpV1(const IntType& Value)
        {
            MediumDecBase::IntMultOpV1(Value);
        }

        template<MediumDecVariant VariantType=MediumDecV2Base, IntegerType IntType=unsigned int>
        VariantType& BasicUIntMultOperationV1(const IntType& rValue)
        { UIntMultOpV1(rValue); return *this; }

        template<MediumDecVariant VariantType=MediumDecV2Base, IntegerType IntType=unsigned int>
        VariantType& BasicIntMultOperationV1(const IntType& rValue)
        { BasicIntMultOpV1(rValue); return *this; }

        /// <summary>
        /// Basic Multiplication operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<MediumDecVariant VariantType=MediumDecV2Base, IntegerType IntType=unsigned int>
        const VariantType BasicMultiplyByUIntV1(const IntType& rValue)
        { auto self = *this; return self.BasicUIntMultOperationV1(rValue); }

        /// <summary>
        /// Basic Multiplication operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<MediumDecVariant VariantType=MediumDecV2Base, IntegerType IntType=signed int>
        const VariantType BasicMultiplyByIntV1(const IntType& rValue)
        { auto self = *this; return self.BasicIntMultOperationV1(rValue); }

public:

        void BasicUIntMultOp(const unsigned int& rValue) { BasicUIntMultOpV1(rValue); }
        void BasicIntMultOp(const signed int& rValue) { BasicIntMultOpV1(rValue); }
        void BasicUInt64MultOp(const unsigned __int64& rValue) { BasicUIntMultOpV1(rValue); }
        void BasicInt64MultOp(const signed __int64& rValue) { BasicIntMultOpV1(rValue); }

        void BasicUnsignedIntMultOp(const signed int& rValue) { BasicUIntMultOpV1(rValue); }
        void BasicUnsignedInt64MultOp(const signed __int64& rValue) { BasicUIntMultOpV1(rValue); }

        void BasicUInt8MultOp(const unsigned char& rValue) { BasicUIntMultOpV1(rValue); }
        void BasicInt8MultOp(const signed char& rValue) { BasicIntMultOpV1(rValue); }
        void BasicUInt16MultOp(const unsigned short& rValue) { BasicUIntMultOpV1(rValue); }
        void BasicInt16MultOp(const signed short& rValue) { BasicIntMultOpV1(rValue); }

        MediumDecV2Base& BasicUIntMultOperation(const unsigned int& rValue) { return BasicUIntMultOperationV1(rValue); }
        MediumDecV2Base& BasicIntMultOperation(const signed int& rValue) { return BasicIntMultOperationV1(rValue); }
        MediumDecV2Base& BasicUInt64MultOperation(const unsigned __int64& rValue) { return BasicUIntMultOperationV1(rValue); }
        MediumDecV2Base& BasicInt64MultOperation(const signed __int64& rValue) { return BasicIntMultOperationV1(rValue); }
        MediumDecV2Base& BasicUInt8MultOperation(const unsigned char& rValue) { return BasicUIntMultOperationV1(rValue); }
        MediumDecV2Base& BasicInt8MultOperation(const signed char& rValue) { return BasicIntMultOperationV1(rValue); }
        MediumDecV2Base& BasicUInt16MultOperation(const unsigned short& rValue) { return BasicUIntMultOperationV1(rValue); }
        MediumDecV2Base& BasicInt16MultOperation(const signed short& rValue) { return BasicIntMultOperationV1(rValue); }

        const MediumDecV2Base BasicMultiplyByUInt(const unsigned int& rValue) { return BasicMultiplyByUIntV1(rValue); }
        const MediumDecV2Base BasicMultiplyByInt(const signed int& rValue) { return BasicMultiplyByIntV1(rValue); }
        const MediumDecV2Base BasicMultiplyByUInt64(const unsigned __int64& rValue) { return BasicMultiplyByUIntV1(rValue); }
        const MediumDecV2Base BasicMultiplyByInt64(const signed __int64& rValue) { return BasicMultiplyByIntV1(rValue); }

        const MediumDecV2Base BasicUnsignedMultiplyByInt(const signed int& rValue) { return BasicMultiplyByUIntV1(rValue); }
        const MediumDecV2Base BasicUnsignedMultiplyByInt64(const signed __int64& rValue) { return BasicMultiplyByUIntV1(rValue); }

        const MediumDecV2Base BasicMultiplyByUInt8(const unsigned char& rValue) { return BasicMultiplyByUIntV1(rValue); }
        const MediumDecV2Base BasicMultiplyByInt8(const signed char& rValue) { return BasicMultiplyByIntV1(rValue); }
        const MediumDecV2Base BasicMultiplyByUInt16(const unsigned short& rValue) { return BasicMultiplyByUIntV1(rValue); }
        const MediumDecV2Base BasicMultiplyByInt16(const signed short& rValue) { return BasicMultiplyByIntV1(rValue); }

    #pragma endregion NormalRep Integer Multiplication Operations

	#pragma region NormalRep AltNum Multiplication Operations

		/// <summary>
        /// Basic multiplication operation that ignores special decimal status with unsigned MediumDec
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        void BasicUnsignedMultOp(const MediumDecV2Base& rValue){ MediumDecBase::UnsignedMultOpV1(rValue); }
		
        void BasicMultOp(const MediumDecV2Base& rValue){ MediumDecBase::MultOpV1(rValue); }

		/// <summary>
        /// Basic unsigned Multiplication operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDecV2Base& BasicUnsignedMultOperation(const MediumDecV2Base& rValue)
		{ MediumDecBase::UnsignedMultOp(rValue); return *this; }

		/// <summary>
        /// Basic Multiplication operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDecV2Base& BasicMultOperation(const MediumDecV2Base& rValue)
		{ MediumDecBase::MultOp(rValue); return *this; }

		/// <summary>
        /// Basic unsigned Multiplication operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        const MediumDecV2Base BasicMultiplyByUnsigned(const MediumDecV2Base& rValue)
        { MediumDecV2Base lValue = *this; return lValue.UnsignedMultOperation(rValue); }

		/// <summary>
        /// Basic Multiplication operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        const MediumDecV2Base BasicMultiplyBy(const MediumDecV2Base& rValue)
        { MediumDecV2Base lValue = *this; return lValue.MultOperation(rValue); }

	#pragma endregion NormalRep AltNum Multiplication Operations

	#pragma region Other multiplication operations

        /// <summary>
        /// Simplified Multiplication by 2 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side value</param>
        /// <returns>MediumDecV2Base&</returns>
        void MultiplyByTwo();

        /// <summary>
        /// Simplified Multiplication by 4 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side value</param>
        /// <returns>MediumDecV2Base&</returns>
        void MultiplyByFour();

        //Return copy of result divided by two
        MediumDecV2Base MultipliedByTwo() const;

        //Return copy of result divided by four
        MediumDecV2Base MultipliedByFour() const;

protected:

        /// <summary>
        /// Unsigned multiplication operation between MediumDec variant and unsigned integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecVariant&</returns>
        template<IntegerType IntType= unsigned int>
        void UIntMultOpV1(const IntType& rValue)
		{
            if (rValue == 1)
                return *this;
            else if (rValue == 0)
            {
                SetAsZero();
                return *this;
            }
        	switch(DecimalHalf.Flags)
        	{
        #if defined(AltNum_EnablePiRep)
        		case 1:{
                    RepType LRep = rValue.GetPiRepType();
                    switch(LRep)
                    {
                        case RepType::PiNum:
                            BasicUIntMultOp(rValue);
                        break;
    #pragma region AltDecVariantExclusive
    #pragma endregion AltDecVariantExclusive
            #if defined(AltNum_EnableApproaching)
                        case RepType::ApproachingBottomPi:
        					if(IntHalf.Value!=0)
        						CatchAllUIntMultiplication(rValue, LRep);
        					break;
                        #if !defined(AltNum_DisableApproachingTop)
                        case RepType::ApproachingTopPi:
        					if(IntHalf.Value==0)//0.99.9 * 5 = ~4.9..9
        						IntHalf.Value = (int)rValue - 1;
        					else//5.9..9 * 100 = 599.9..9
        						IntHalf.Value = (IntHalf.Value+1)*(unsigned int)rValue - 1;
    					break;
                        #endif
    #pragma region AltDecVariantExclusive
    #pragma endregion AltDecVariantExclusive
            #endif
                        default:
                            throw "Unable to perform integer multiplication on current representation.";
                    }
                } break;
        #endif
        #if defined(AltNum_EnableERep)
        		case 2:{
                    RepType LRep = rValue.GetERepType();
                    switch(LRep)
                    {
                        case RepType::ENum:
                            BasicUIntMultOp(rValue);
                        break;
    #pragma region AltDecVariantExclusive
    #pragma endregion AltDecVariantExclusive
            #if defined(AltNum_EnableApproaching)
                        case RepType::ApproachingBottomE:{

                        } break;
                        #if !defined(AltNum_DisableApproachingTop)
                        case RepType::ApproachingTopE:
        					if(IntHalf.Value==0)//0.99.9 * 5 = ~4.9..9
        						IntHalf.Value = (int)rValue - 1;
        					else//5.9..9 * 100 = 599.9..9
        						IntHalf.Value = (IntHalf.Value+1)*(unsigned int)rValue - 1;
    					break;
                        #endif
    #pragma region AltDecVariantExclusive
    #pragma endregion AltDecVariantExclusive
            #endif
                        default:
                            throw "Unable to perform integer multiplication on current representation.";
                    }
                } break;
        #endif
        #if defined(AltNum_EnableIRep)//IRep_to_integer
        		case 3:{
                    RepType LRep = rValue.GetIRepType();
                    switch(LRep){
                        case RepType::INum:
                            BasicUIntMultOp(rValue);
                        break;
    #pragma region AltDecVariantExclusive
    #pragma endregion AltDecVariantExclusive
            #if defined(AltNum_EnableApproaching)
                        case RepType::ApproachingImaginaryBottom:
        					if(IntHalf.Value!=0)
        						CatchAllUIntMultiplication(rValue, LRep);
                            break;
                        #if !defined(AltNum_DisableApproachingTop)
                        case RepType::ApproachingImaginaryTop:
        					if(IntHalf.Value==0)//0.99.9 * 5 = ~4.9..9
        						IntHalf.Value = (int)rValue - 1;
        					else//5.9..9 * 100 = 599.9..9
        						IntHalf.Value = (IntHalf.Value+1)*(unsigned int)rValue - 1;
    					break;
                        #endif
    #pragma region AltDecVariantExclusive
    #pragma endregion AltDecVariantExclusive
            #endif
            #if defined(AltNum_EnableImaginaryInfinity)
                        case RepType::ImaginaryInfinity:
                            return *this;
                            break;
            #endif
                        default:
                            throw "Unable to perform integer multiplication on current representation.";
                    }
                } break;
        #endif
        		default:{
                    RepType LRep = rValue.GetNormRepType();
                    switch(LRep)
                    {
                        case RepType::NormalType:
                            BasicUIntMultOp(rValue);
                        break;
    #pragma region AltDecVariantExclusive
    #pragma endregion AltDecVariantExclusive
            #if defined(AltNum_EnableApproaching)
                        case RepType::ApproachingBottom:
        					if(IntHalf.Value!=0)
        						CatchAllUIntMultiplication(rValue, LRep);
                            break;
                        #if !defined(AltNum_DisableApproachingTop)
                        case RepType::ApproachingTop:
        					if(IntHalf.Value==0)//0.99.9 * 5 = ~4.9..9
        						IntHalf.Value = (int)rValue - 1;
        					else//5.9..9 * 100 = 599.9..9
        						IntHalf.Value = (IntHalf.Value+1)*(unsigned int)rValue - 1;
    					break;
                        #endif
    #pragma region AltDecVariantExclusive
    #pragma endregion AltDecVariantExclusive
            #endif
            #ifdef AltNum_EnableInfinity
                        case RepType::Infinity:
                            return *this;
                            break;
            #endif
                        default:
                            throw "Unable to perform integer multiplication on current representation.";
                    }
                } break;
        	}
		}

        /// <summary>
        /// Multiplication operation between MediumDec variant and integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecVariant&</returns>
        template<IntegerType IntType= signed int>
        void IntMultOpV1(const IntType& rValue)
		{
            if(Value<0)
            {
                SwapNegativeStatus();
                UIntMultOpV1(-rValue);
            }
            else
                UIntMultOpV1(rValue);
		}

        template<IntegerType IntType=unsigned int>
        MediumDecV2Base& UIntMultOperationV1(const IntType& rValue)
        { UIntMultOpV1(rValue); return *this; }

        template<IntegerType IntType=unsigned int>
        MediumDecV2Base& IntMultOperationV1(const IntType& rValue)
        { IntMultOpV1(rValue); return *this; }

        /// <summary>
        /// Unsigned Multiplication operation between MediumDec variant and unsigned integer values
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecVariant</returns>
        template<IntegerType IntType= unsigned int>
        auto MultiplyByUIntV1(const IntType& rValue)
		{
            auto self = *this;
            return self.UIntMultOpV1(rValue);
		}

        /// <summary>
        /// Multiplication operation between MediumDec variant and integer values
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecVariant</returns>
        template<IntegerType IntType= signed int>
        auto MultiplyByIntV1(const IntType& rValue)
		{
            auto self = *this;
            return self.IntMultOpV1(rValue);
		}

public:

        void UIntMultOp(const unsigned int& rValue) { UIntMultOpV1(rValue); }
        void IntMultOp(const signed int& rValue) { IntMultOpV1(rValue); }
        void UInt64MultOp(const unsigned __int64& rValue) { UIntMultOpV1(rValue); }
        void Int64MultOp(const signed __int64& rValue) { IntMultOpV1(rValue); }

        void UnsignedIntMultOp(const signed int& rValue) { UIntMultOpV1(rValue); }
        void UnsignedInt64MultOp(const signed __int64& rValue) { UIntMultOpV1(rValue); }

        void UInt8MultOp(const unsigned char& rValue) { UIntMultOpV1(rValue); }
        void Int8MultOp(const signed char& rValue) { IntMultOpV1(rValue); }
        void UInt16MultOp(const unsigned short& rValue) { UIntMultOpV1(rValue); }
        void Int16MultOp(const signed short& rValue) { IntMultOpV1(rValue); }

        MediumDecV2Base& UIntMultOperation(const unsigned int& rValue) { return UIntMultOperationV1(rValue); }
        MediumDecV2Base& IntMultOperation(const signed int& rValue) { return IntMultOperationV1(rValue); }
        MediumDecV2Base& UInt64MultOperation(const unsigned __int64& rValue) { return UIntMultOperationV1(rValue); }
        MediumDecV2Base& Int64MultOperation(const signed __int64& rValue) { return IntMultOperationV1(rValue); }
        MediumDecV2Base& UInt8MultOperation(const unsigned char& rValue) { return UIntMultOperationV1(rValue); }
        MediumDecV2Base& Int8MultOperation(const signed char& rValue) { return IntMultOperationV1(rValue); }
        MediumDecV2Base& UInt16MultOperation(const unsigned short& rValue) { return UIntMultOperationV1(rValue); }
        MediumDecV2Base& Int16MultOperation(const signed short& rValue) { return IntMultOperationV1(rValue); }

        const MediumDecV2Base MultiplyByUInt(const unsigned int& rValue) { return MultiplyByUIntV1(rValue); }
        const MediumDecV2Base MultiplyByInt(const signed int& rValue) { return MultiplyByIntV1(rValue); }
        const MediumDecV2Base MultiplyByUInt64(const unsigned __int64& rValue) { return MultiplyByUIntV1(rValue); }
        const MediumDecV2Base MultiplyByInt64(const signed __int64& rValue) { return MultiplyByIntV1(rValue); }

        const MediumDecV2Base UnsignedMultiplyByInt(const signed int& rValue) { return MultiplyByUIntV1(rValue); }
        const MediumDecV2Base UnsignedMultiplyByInt64(const signed __int64& rValue) { return MultiplyByUIntV1(rValue); }

        const MediumDecV2Base MultiplyByUInt8(const unsigned char& rValue) { return MultiplyByUIntV1(rValue); }
        const MediumDecV2Base MultiplyByInt8(const signed char& rValue) { return MultiplyByIntV1(rValue); }
        const MediumDecV2Base MultiplyByUInt16(const unsigned short& rValue) { return MultiplyByUIntV1(rValue); }
        const MediumDecV2Base MultiplyByInt16(const signed short& rValue) { return MultiplyByIntV1(rValue); }

		/// <summary>
        /// Unsigned Multiplication operation that ignores special decimal status
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDecV2Base& UnsignedMultOp(const MediumDecV2Base& rValue);

		/// <summary>
        /// Multiplication operation that ignores special decimal status
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        void MultOp(const MediumDecV2Base& Value)
        {
            if(Value.IsNegative())
            {
                SwapNegativeStatus();
                UnsignedMultOp(-Value);
            }
            else
                UnsignedMultOp(Value);
        }

		/// <summary>
        /// Unsigned Multiplication operation that ignores special decimal status
        /// Return true if divide into zero
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        auto MultiplyByUnsigned(const MediumDecV2Base& rValue)
        {
            auto self = *this;
            return self.UnsignedMultOp(rValue);
        }

		/// <summary>
        /// Multiplication operation that ignores special decimal status
        /// Return true if divide into zero
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        auto MultiplyBy(const MediumDecV2Base& rValue)
        {
            auto self = *this;
            return self.MultOp(rValue);
        }

        /// <summary>
        /// /= operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2Base</returns>
        friend MediumDecV2Base& operator*=(MediumDecV2Base& lValue, const MediumDecV2Base& rValue) { return lValue.MultOperation(rValue); }

        /// <summary>
        /// *= operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2Base</returns>
        friend MediumDecV2Base& operator*=(MediumDecV2Base& lValue, const signed int& rValue) { return lValue.IntMultOperation(rValue); }
        friend MediumDecV2Base& operator*=(MediumDecV2Base& lValue, const signed __int64& rValue) { return lValue.Int64MultOperation(rValue); }
        friend MediumDecV2Base& operator*=(MediumDecV2Base& lValue, const unsigned int& rValue) { return lValue.UIntMultOperation(rValue); }
        friend MediumDecV2Base& operator*=(MediumDecV2Base& lValue, const unsigned __int64& rValue) { return lValue.UInt64MultOperation(rValue); }

        friend MediumDecV2Base& operator*=(MediumDecV2Base& lValue, const signed char& rValue) { return lValue.Int8MultOperation(rValue); }
        friend MediumDecV2Base& operator*=(MediumDecV2Base& lValue, const signed short& rValue) { return lValue.Int16MultOperation(rValue); }
        friend MediumDecV2Base& operator*=(MediumDecV2Base& lValue, const unsigned char& rValue) { return lValue.UInt8MultOperation(rValue); }
        friend MediumDecV2Base& operator*=(MediumDecV2Base& lValue, const unsigned short& rValue) { return lValue.UInt16MultOperation(rValue); }

        /// <summary>
        /// Multiplication operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2Base</returns>
        friend MediumDecV2Base operator*(MediumDecV2Base lValue, const MediumDecV2Base& rValue) { return lValue.MultiplyBy(rValue); }
		
        /// <summary>
        /// Multiplication operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2Base</returns>
        friend MediumDecV2Base operator*(MediumDecV2Base lValue, const signed int& rValue) { return lValue.IntMultOperation(rValue); }
        friend MediumDecV2Base operator*(MediumDecV2Base lValue, const signed __int64& rValue) { return lValue.Int64MultOperation(rValue); }
        friend MediumDecV2Base operator*(MediumDecV2Base lValue, const unsigned int& rValue) { return lValue.UIntMultOperation(rValue); }
        friend MediumDecV2Base operator*(MediumDecV2Base lValue, const unsigned __int64& rValue) { return lValue.UInt64MultOperation(rValue); }

        friend MediumDecV2Base operator*(MediumDecV2Base lValue, const signed char& rValue) { return lValue.Int8MultOperation(rValue); }
        friend MediumDecV2Base operator*(MediumDecV2Base lValue, const signed short& rValue) { return lValue.Int16MultOperation(rValue); }
        friend MediumDecV2Base operator*(MediumDecV2Base lValue, const unsigned char& rValue) { return lValue.UInt8MultOperation(rValue); }
        friend MediumDecV2Base operator*(MediumDecV2Base lValue, const unsigned short& rValue) { return lValue.UInt16MultOperation(rValue); }
		
        friend MediumDecV2Base operator*(const signed int& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).MultiplyBy(rValue); }
        friend MediumDecV2Base operator*(const signed __int64& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).MultiplyBy(rValue); }
        friend MediumDecV2Base operator*(const unsigned int& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).MultiplyBy(rValue); }
        friend MediumDecV2Base operator*(const unsigned __int64& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).MultiplyBy(rValue); }

        friend MediumDecV2Base operator*(const signed char& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).MultiplyBy(rValue); }
        friend MediumDecV2Base operator*(const signed short& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).MultiplyBy(rValue); }
        friend MediumDecV2Base operator*(const unsigned char& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).MultiplyBy(rValue); }
        friend MediumDecV2Base operator*(const unsigned short& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).MultiplyBy(rValue); }

    #pragma endregion Other multiplication operations

	#pragma region NormalRep Integer Addition Operations
protected:
		
        /// <summary>
        /// Basic addition operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        void BasicUIntAddOpV1(const IntType& rValue)
        {
			MediumDecBase::UIntAddOpV1(rValue);
        }

        /// <summary>
        /// Basic addition operation between MediumDec Variant and Integer value 
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        void BasicIntAddOpV1(const IntType& rValue)
        {
			MediumDecBase::IntAddOpV1(rValue);
        }

        template<IntegerType IntType=unsigned int>
        auto& BasicUIntAddOperationV1(const IntType& rValue)
        { BasicUIntAddOpV1(rValue); return *this; }

        template<IntegerType IntType=signed int>
        auto& BasicIntAddOperationV1(const IntType& rValue)
        { BasicIntAddOpV1(rValue); return *this; }

		/// <summary>
        /// Basic addition operation between MediumDec variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        auto BasicAddByUIntV1(const IntType& rValue)
        { auto self = *this; return self.BasicUIntAddOperationV1(rValue); }

		/// <summary>
        /// Basic addition operation between MediumDec variant and Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        auto BasicAddByIntV1(const IntType& rValue)
        { auto self = *this; return self.IntAddOperationV1(rValue); }

public:

        /// <summary>
        /// Basic addition operation between MediumDec Variant and unsigned MirroredInt
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        void BasicUnsignedIntegerAddition(const MirroredInt& rValue)
        { MediumDecBase::UnsignedIntegerAddition(rValue); }

        /// <summary>
        /// Basic addition operation between MediumDec Variant and MirroredInt
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        void BasicIntegerAddition(const MirroredInt& rValue)
        { MediumDecBase::IntegerAddition(rValue); }

        void BasicUIntAddOp(const unsigned int& rValue){ MediumDecBase::UIntAddOp(rValue); }

        void BasicIntAddOp(const signed int& rValue) { MediumDecBase::IntAddOpV1(rValue); }
        void BasicUInt64AddOp(const unsigned __int64& rValue) { MediumDecBase::UIntAddOpV1(rValue); }
        void BasicInt64AddOp(const signed __int64& rValue) { MediumDecBase::IntAddOpV1(rValue); }

        void BasicUnsignedIntAddOp(const signed int& rValue) { MediumDecBase::UIntAddOpV1(rValue); }
        void BasicUnsignedInt64AddOp(const signed __int64& rValue) { MediumDecBase::UIntAddOpV1(rValue); }

        void BasicUInt8AddOp(const unsigned char& rValue) { MediumDecBase::UIntAddOpV1(rValue); }
        void BasicInt8AddOp(const signed char& rValue) { MediumDecBase::IntAddOpV1(rValue); }
        void BasicUInt16AddOp(const unsigned short& rValue) { MediumDecBase::UIntAddOpV1(rValue); }
        void BasicInt16AddOp(const signed short& rValue) { MediumDecBase::IntAddOpV1(rValue); }

        MediumDecV2Base& BasicUIntAddOperation(const unsigned int& rValue){ return MediumDecBase::UIntAddOp(rValue); return *this; }

        MediumDecV2Base& BasicIntAddOperation(const signed int& rValue) { return BasicIntAddOperationV1(rValue); }
        MediumDecV2Base& BasicUInt64AddOperation(const unsigned __int64& rValue) { return BasicUIntAddOperationV1(rValue); }
        MediumDecV2Base& BasicInt64AddOperation(const signed __int64& rValue) { return BasicIntAddOperationV1(rValue); }
		
        MediumDecV2Base& BasicUInt8AddOperation(const unsigned char& rValue) { return BasicUIntAddOperationV1(rValue); }
        MediumDecV2Base& BasicInt8AddOperation(const signed char& rValue) { return BasicIntAddOperationV1(rValue); }
        MediumDecV2Base& BasicUInt16AddOperation(const unsigned short& rValue) { return BasicUIntAddOperationV1(rValue); }
        MediumDecV2Base& BasicInt16AddOperation(const signed short& rValue) { return BasicIntAddOperationV1(rValue); }
        MediumDecV2Base BasicAddByUInt(const unsigned int& rValue) { return BasicAddByUIntV1(rValue); }
        MediumDecV2Base BasicAddByInt(const signed int& rValue) { return BasicAddByIntV1(rValue); }
        MediumDecV2Base BasicAddByUInt64(const unsigned __int64& rValue) { return BasicAddByUIntV1(rValue); }
        MediumDecV2Base BasicAddByInt64(const signed __int64& rValue) { return BasicAddByIntV1(rValue); }

        MediumDecV2Base BasicUnsignedAddByInt(const signed int& rValue) { return BasicAddByUIntV1(rValue); }
        MediumDecV2Base BasicUnsignedAddByInt64(const signed __int64& rValue) { return BasicAddByUIntV1(rValue); }

        MediumDecV2Base BasicAddByUInt8(const unsigned char& rValue) { return BasicAddByUIntV1(rValue); }
        MediumDecV2Base BasicAddByInt8(const signed char rValue) { return BasicAddByIntV1(rValue); }
        MediumDecV2Base BasicAddByUInt16(const unsigned short& rValue) { return BasicAddByUIntV1(rValue); }
        MediumDecV2Base BasicAddByInt16(const signed short& rValue) { return BasicAddByIntV1(rValue); }
    	
	#pragma endregion NormalRep Integer Addition Operations

	#pragma region NormalRep Integer Subtraction Operations
protected:
		
        /// <summary>
        /// Basic subtraction operation between MediumDec Variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        void BasicUIntSubOpV1(const IntType& rValue)
        {
			MediumDecBase::UIntSubOpV1(rValue);
        }

        /// <summary>
        /// Basic subtraction operation between MediumDec Variant and Integer value 
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        void BasicIntSubOpV1(const IntType& rValue)
        {
			MediumDecBase::IntSubOpV1(rValue);
        }

        template<IntegerType IntType=unsigned int>
        auto& BasicUIntSubOperationV1(const IntType& rValue)
        { BasicUIntSubOpV1(rValue); return *this; }

        template<IntegerType IntType=signed int>
        auto& BasicIntSubOperationV1(const IntType& rValue)
        { BasicIntSubOpV1(rValue); return *this; }

		/// <summary>
        /// Basic subtraction operation between MediumDec variant and unsigned Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        auto BasicSubtractByUIntV1(const IntType& rValue)
        { auto self = *this; return self.BasicUIntSubOperationV1(rValue); }

		/// <summary>
        /// Basic subtraction operation between MediumDec variant and Integer value 
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        auto BasicSubtractByIntV1(const IntType& rValue)
        { auto self = *this; return self.IntSubOperationV1(rValue); }

public:

        /// <summary>
        /// Basic subtraction operation between MediumDec Variant and unsigned MirroredInt
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        void BasicUnsignedIntegerSubtraction(const MirroredInt& rValue)
        { MediumDecBase::UnsignedIntegerSubtraction(rValue); }

        /// <summary>
        /// Basic subtraction operation between MediumDec Variant and MirroredInt
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        void BasicIntegerSubtraction(const MirroredInt& rValue)
        { MediumDecBase::IntegerSubtraction(rValue); }

        void BasicUIntSubOp(const unsigned int& rValue){ MediumDecBase::UIntSubOp(rValue); }

        void BasicIntSubOp(const signed int& rValue) { MediumDecBase::IntSubOpV1(rValue); }
        void BasicUInt64SubOp(const unsigned __int64& rValue) { MediumDecBase::UIntSubOpV1(rValue); }
        void BasicInt64SubOp(const signed __int64& rValue) { MediumDecBase::IntSubOpV1(rValue); }

        void BasicUnsignedIntSubOp(const signed int& rValue) { MediumDecBase::UIntSubOpV1(rValue); }
        void BasicUnsignedInt64SubOp(const signed __int64& rValue) { MediumDecBase::UIntSubOpV1(rValue); }

        void BasicUInt8SubOp(const unsigned char& rValue) { MediumDecBase::UIntSubOpV1(rValue); }
        void BasicInt8SubOp(const signed char& rValue) { MediumDecBase::IntSubOpV1(rValue); }
        void BasicUInt16SubOp(const unsigned short& rValue) { MediumDecBase::UIntSubOpV1(rValue); }
        void BasicInt16SubOp(const signed short& rValue) { MediumDecBase::IntSubOpV1(rValue); }

        MediumDecV2Base& BasicUIntSubOperation(const unsigned int& rValue){ return MediumDecBase::UIntSubOp(rValue); return *this; }

        MediumDecV2Base& BasicIntSubOperation(const signed int& rValue) { return BasicIntSubOperationV1(rValue); }
        MediumDecV2Base& BasicUInt64SubOperation(const unsigned __int64& rValue) { return BasicUIntSubOperationV1(rValue); }
        MediumDecV2Base& BasicInt64SubOperation(const signed __int64& rValue) { return BasicIntSubOperationV1(rValue); }
		
        MediumDecV2Base& BasicUInt8SubOperation(const unsigned char& rValue) { return BasicUIntSubOperationV1(rValue); }
        MediumDecV2Base& BasicInt8SubOperation(const signed char& rValue) { return BasicIntSubOperationV1(rValue); }
        MediumDecV2Base& BasicUInt16SubOperation(const unsigned short& rValue) { return BasicUIntSubOperationV1(rValue); }
        MediumDecV2Base& BasicInt16SubOperation(const signed short& rValue) { return BasicIntSubOperationV1(rValue); }
        MediumDecV2Base BasicSubtractByUInt(const unsigned int& rValue) { return BasicSubtractByUIntV1(rValue); }
        MediumDecV2Base BasicSubtractByInt(const signed int& rValue) { return BasicSubtractByIntV1(rValue); }
        MediumDecV2Base BasicSubtractByUInt64(const unsigned __int64& rValue) { return BasicSubtractByUIntV1(rValue); }
        MediumDecV2Base BasicSubtractByInt64(const signed __int64& rValue) { return BasicSubtractByIntV1(rValue); }

        MediumDecV2Base BasicUnsignedSubtractByInt(const signed int& rValue) { return BasicSubtractByUIntV1(rValue); }
        MediumDecV2Base BasicUnsignedSubtractByInt64(const signed __int64& rValue) { return BasicSubtractByUIntV1(rValue); }

        MediumDecV2Base BasicSubtractByUInt8(const unsigned char& rValue) { return BasicSubtractByUIntV1(rValue); }
        MediumDecV2Base BasicSubtractByInt8(const signed char rValue) { return BasicSubtractByIntV1(rValue); }
        MediumDecV2Base BasicSubtractByUInt16(const unsigned short& rValue) { return BasicSubtractByUIntV1(rValue); }
        MediumDecV2Base BasicSubtractByInt16(const signed short& rValue) { return BasicSubtractByIntV1(rValue); }
    	
	#pragma endregion NormalRep Integer Subtraction Operations

    #pragma region NormalRep AltNum Addition/Subtraction Operations

        /// <summary>
        /// Basic addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void UnsignedAddOp(const MediumDecV2Base& rValue){ MediumDecBase::UnsignedAddOpV1(rValue); }

	    /// <summary>
        /// Basic addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void AddOp(const MediumDecV2Base& rValue){ MediumDecBase::AddOpV1(rValue); }

	    /// <summary>
        /// Basic subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void UnsignedSubOp(const MediumDecV2Base& rValue){ MediumDecBase::UnsignedSubOpV1(rValue); }
		
	    /// <summary>
        /// Basic subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void SubOp(const MediumDecV2Base& rValue){ MediumDecBase::SubOpV1(rValue); }

        //Basic addition operation
        MediumDecV2Base& UnsignedAddOperation(const MediumDecV2Base& rValue)
        { MediumDecBase::UnsignedAddOp(rValue); return *this; }
		
        //Basic addition operation
        MediumDecV2Base& AddOperation(const MediumDecV2Base& rValue)
        { MediumDecBase::AddOp(rValue); return *this; }

		/// <summary>
        /// Unsigned Addition operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDecV2Base AddByUnsigned(const MediumDecV2Base& rValue)
        { MediumDecV2Base lValue = *this; return lValue.UnsignedAddOperation(rValue); } const

		/// <summary>
        /// Addition operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        MediumDecV2Base AddBy(const MediumDecV2Base& rValue)
        { MediumDecV2Base lValue = *this; return lValue.AddOperation(rValue); } const

        //Basic subtraction operation
        MediumDecV2Base& UnsignedSubOperation(const MediumDecV2Base& rValue)
        { MediumDecBase::UnsignedSubOp(rValue); return *this; }

        //Basic subtraction operation
        MediumDecV2Base& SubOperation(const MediumDecV2Base& rValue)
        { MediumDecBase::SubOp(rValue); return *this; }

		/// <summary>
        /// Basic unsigned Subtraction operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        MediumDecV2Base SubtractByUnsigned(const MediumDecV2Base& rValue)
        { MediumDecV2Base lValue = *this; return lValue.UnsignedSubOperation(rValue); } const

		/// <summary>
        /// Basic Subtraction operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param> 
        MediumDecV2Base SubtractBy(const MediumDecV2Base& rValue)
        { MediumDecV2Base lValue = *this; return lValue.SubOperation(rValue); } const

    #pragma endregion NormalRep AltNum Addition/Subtraction Operations

	#pragma region Other addition operations

        /// <summary>
        /// += operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2Base</returns>
        friend MediumDecV2Base& operator+=(MediumDecV2Base& lValue, const MediumDecV2Base& rValue) { return lValue.AddOperation(rValue); }

        /// <summary>
        /// += operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        friend MediumDecV2Base& operator+=(MediumDecV2Base& lValue, const signed int& rValue) { return lValue.IntAddOperation(rValue); }
        friend MediumDecV2Base& operator+=(MediumDecV2Base& lValue, const signed __int64& rValue) { return lValue.Int64AddOperation(rValue); }
        friend MediumDecV2Base& operator+=(MediumDecV2Base& lValue, const unsigned int& rValue) { return lValue.UIntAddOperation(rValue); }
        friend MediumDecV2Base& operator+=(MediumDecV2Base& lValue, const unsigned __int64& rValue) { return lValue.UInt64AddOperation(rValue); }

        friend MediumDecV2Base& operator+=(MediumDecV2Base& lValue, const signed char& rValue) { return lValue.Int8AddOperation(rValue); }
        friend MediumDecV2Base& operator+=(MediumDecV2Base& lValue, const signed short& rValue) { return lValue.Int16AddOperation(rValue); }
        friend MediumDecV2Base& operator+=(MediumDecV2Base& lValue, const unsigned char& rValue) { return lValue.UInt8AddOperation(rValue); }
        friend MediumDecV2Base& operator+=(MediumDecV2Base& lValue, const unsigned short& rValue) { return lValue.UInt16AddOperation(rValue); }

        /// <summary>
        /// Addition operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2Base</returns>
        friend MediumDecV2Base operator+(MediumDecV2Base lValue, const MediumDecV2Base& rValue) { return lValue.AddOperation(rValue); }
		
        /// <summary>
        /// Addition operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        friend MediumDecV2Base operator+(MediumDecV2Base lValue, const signed int& rValue) { return lValue.IntAddOperation(rValue); }
        friend MediumDecV2Base operator+(MediumDecV2Base lValue, const signed __int64& rValue) { return lValue.Int64AddOperation(rValue); }
        friend MediumDecV2Base operator+(MediumDecV2Base lValue, const unsigned int& rValue) { return lValue.UIntAddOperation(rValue); }
        friend MediumDecV2Base operator+(MediumDecV2Base lValue, const unsigned __int64& rValue) { return lValue.UInt64AddOperation(rValue); }
		
        friend MediumDecV2Base operator+(MediumDecV2Base lValue, const signed char& rValue) { return lValue.Int8AddOperation(rValue); }
        friend MediumDecV2Base operator+(MediumDecV2Base lValue, const signed short& rValue) { return lValue.Int16AddOperation(rValue); }
        friend MediumDecV2Base operator+(MediumDecV2Base lValue, const unsigned char& rValue) { return lValue.UInt8AddOperation(rValue); }
        friend MediumDecV2Base operator+(MediumDecV2Base lValue, const unsigned short& rValue) { return lValue.UInt16AddOperation(rValue); }
		
        friend MediumDecV2Base operator+(const signed int& lValue, MediumDecV2Base rValue)  { return rValue.IntAddOperation(lValue); }
        friend MediumDecV2Base operator+(const signed __int64& lValue, MediumDecV2Base rValue)  { return rValue.Int64AddOperation(lValue); }
        friend MediumDecV2Base operator+(const unsigned int& lValue, MediumDecV2Base rValue)  { return rValue.UIntAddOperation(lValue); }
        friend MediumDecV2Base operator+(const unsigned __int64& lValue, MediumDecV2Base rValue)  { return rValue.UInt64AddOperation(lValue); }
		
        friend MediumDecV2Base operator+(const signed char& lValue, MediumDecV2Base rValue)  { return rValue.Int8AddOperation(lValue); }
        friend MediumDecV2Base operator+(const signed short& lValue, MediumDecV2Base rValue)  { return rValue.Int16AddOperation(lValue); }
        friend MediumDecV2Base operator+(const unsigned char& lValue, MediumDecV2Base rValue)  { return rValue.UInt8AddOperation(lValue); }
        friend MediumDecV2Base operator+(const unsigned short& lValue, MediumDecV2Base rValue)  { return rValue.UInt16AddOperation(lValue); }

	#pragma endregion Other addition operations

	#pragma region Other subtraction operations

        /// <summary>
        /// -= operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2Base</returns>
        friend MediumDecV2Base& operator-=(MediumDecV2Base& lValue, const MediumDecV2Base& rValue) { return lValue.SubOperation(rValue); }

        /// <summary>
        /// -= operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2Base</returns>
        friend MediumDecV2Base& operator-=(MediumDecV2Base& lValue, const signed int& rValue) { return lValue.IntSubOperation(rValue); }
        friend MediumDecV2Base& operator-=(MediumDecV2Base& lValue, const signed __int64& rValue) { return lValue.Int64SubOperation(rValue); }
        friend MediumDecV2Base& operator-=(MediumDecV2Base& lValue, const unsigned int& rValue) { return lValue.UIntSubOperation(rValue); }
        friend MediumDecV2Base& operator-=(MediumDecV2Base& lValue, const unsigned __int64& rValue) { return lValue.UInt64SubOperation(rValue); }

        friend MediumDecV2Base& operator-=(MediumDecV2Base& lValue, const signed char& rValue) { return lValue.Int8SubOperation(rValue); }
        friend MediumDecV2Base& operator-=(MediumDecV2Base& lValue, const signed short& rValue) { return lValue.Int16SubOperation(rValue); }
        friend MediumDecV2Base& operator-=(MediumDecV2Base& lValue, const unsigned char& rValue) { return lValue.UInt8SubOperation(rValue); }
        friend MediumDecV2Base& operator-=(MediumDecV2Base& lValue, const unsigned short& rValue) { return lValue.UInt16SubOperation(rValue); }

        /// <summary>
        /// Subtraction operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2Base</returns>
        friend MediumDecV2Base operator-(MediumDecV2Base lValue, const MediumDecV2Base& rValue) { return lValue.SubOperation(rValue); }
		
        /// <summary>
        /// Subtraction operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2Base</returns>
        friend MediumDecV2Base operator-(MediumDecV2Base lValue, const signed int& rValue) { return lValue.IntSubOperation(rValue); }
        friend MediumDecV2Base operator-(MediumDecV2Base lValue, const signed __int64& rValue) { return lValue.Int64SubOperation(rValue); }
        friend MediumDecV2Base operator-(MediumDecV2Base lValue, const unsigned int& rValue) { return lValue.UIntSubOperation(rValue); }
        friend MediumDecV2Base operator-(MediumDecV2Base lValue, const unsigned __int64& rValue) { return lValue.UInt64SubOperation(rValue); }
		
        friend MediumDecV2Base operator-(MediumDecV2Base lValue, const signed char& rValue) { return lValue.Int8SubOperation(rValue); }
        friend MediumDecV2Base operator-(MediumDecV2Base lValue, const signed short& rValue) { return lValue.Int16SubOperation(rValue); }
        friend MediumDecV2Base operator-(MediumDecV2Base lValue, const unsigned char& rValue) { return lValue.UInt8SubOperation(rValue); }
        friend MediumDecV2Base operator-(MediumDecV2Base lValue, const unsigned short& rValue) { return lValue.UInt16SubOperation(rValue); }
		
        friend MediumDecV2Base operator-(const signed int& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).SubtractBy(rValue); }
        friend MediumDecV2Base operator-(const signed __int64& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).SubtractBy(rValue); }
        friend MediumDecV2Base operator-(const unsigned int& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).SubtractBy(rValue); }
        friend MediumDecV2Base operator-(const unsigned __int64& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).SubtractBy(rValue); }
		
        friend MediumDecV2Base operator-(const signed char& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).SubtractBy(rValue); }
        friend MediumDecV2Base operator-(const signed short& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).SubtractBy(rValue); }
        friend MediumDecV2Base operator-(const unsigned char& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).SubtractBy(rValue); }
        friend MediumDecV2Base operator-(const unsigned short& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).SubtractBy(rValue); }

	#pragma endregion Other subtraction operations

	#pragma region Modulus Operations
    //Defined inside full version of class object
	#pragma endregion Modulus Operations

    #pragma region Floating Operator Overrides

        friend MediumDecV2Base operator+(const MediumDecV2Base& self, const float& Value) { return self + (MediumDecV2Base)Value; }
        friend MediumDecV2Base operator-(const MediumDecV2Base& self, const float& Value) { return self - (MediumDecV2Base)Value; }
        friend MediumDecV2Base operator*(const MediumDecV2Base& self, const float& Value) { return self * (MediumDecV2Base)Value; }
        friend MediumDecV2Base operator/(const MediumDecV2Base& self, const float& Value) { return self / (MediumDecV2Base)Value; }

        friend MediumDecV2Base operator+(const float& Value, const MediumDecV2Base& self) { return (MediumDecV2Base)Value + self; }
        friend MediumDecV2Base operator-(const float& Value, const MediumDecV2Base& self) { return (MediumDecV2Base)Value - self; }
        friend MediumDecV2Base operator*(const float& Value, const MediumDecV2Base& self) { return (MediumDecV2Base)Value * self; }
        friend MediumDecV2Base operator/(const float& Value, const MediumDecV2Base& self) { return (MediumDecV2Base)Value / self; }

        friend MediumDecV2Base operator+(const MediumDecV2Base& self, const double& Value) { return self + (MediumDecV2Base)Value; }
        friend MediumDecV2Base operator-(const MediumDecV2Base& self, const double& Value) { return self - (MediumDecV2Base)Value; }
        friend MediumDecV2Base operator*(const MediumDecV2Base& self, const double& Value) { return self * (MediumDecV2Base)Value; }
        friend MediumDecV2Base operator/(const MediumDecV2Base& self, const double& Value) { return self / (MediumDecV2Base)Value; }

        friend MediumDecV2Base operator+(const MediumDecV2Base& self, const long double& Value) { return self + (MediumDecV2Base)Value; }
        friend MediumDecV2Base operator-(const MediumDecV2Base& self, const long double& Value) { return self - (MediumDecV2Base)Value; }
        friend MediumDecV2Base operator*(const MediumDecV2Base& self, const long double& Value) { return self * (MediumDecV2Base)Value; }
        friend MediumDecV2Base operator/(const MediumDecV2Base& self, const long double& Value) { return self / (MediumDecV2Base)Value; }

        friend MediumDecV2Base operator+(const long double& Value, const MediumDecV2Base& self) { return (MediumDecV2Base)Value + self; }
        friend MediumDecV2Base operator-(const long double& Value, const MediumDecV2Base& self) { return (MediumDecV2Base)Value - self; }
        friend MediumDecV2Base operator*(const long double& Value, const MediumDecV2Base& self) { return (MediumDecV2Base)Value * self; }
        friend MediumDecV2Base operator/(const long double& Value, const MediumDecV2Base& self) { return (MediumDecV2Base)Value / self; }

    #pragma endregion Floating Operator Overrides

    #pragma region Other Operators

        /// <summary>
        /// Negative Unary Operator(Flips negative status)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <returns>MediumDecVariant</returns>
        MediumDecV2Base operator- ()
        {
			auto self = *this;
            self.SwapNegativeStatus(); return self;
        } const

        /// <summary>
        /// ++MediumDecV2Base Operator
        /// </summary>
        /// <returns>MediumDecV2Base &</returns>
        MediumDecV2Base& operator ++()
        {
            if (DecimalHalf == 0)
                ++IntHalf;
            else if (IntHalf == NegativeRep)
                IntHalf = MirroredInt::Zero;
            else
                ++IntHalf;
            return *this;
        }

        /// <summary>
        /// ++MediumDecV2Base Operator
        /// </summary>
        /// <returns>MediumDecV2Base &</returns>
        MediumDecV2Base& operator --()
        {
            if (DecimalHalf == 0)
                --IntHalf;
            else if (IntHalf == 0)
                IntHalf = NegativeRep;
            else
                --IntHalf;
            return *this;
        }

        /// <summary>
        /// MediumDec Variant++ Operator
        /// </summary>
        /// <returns>MediumDecVariant</returns>
        MediumDecV2Base operator ++(int)
        {
            MediumDecV2Base tmp(*this);
            ++* this;
            return tmp;
        }

        /// <summary>
        /// MediumDec Variant-- Operator
        /// </summary>
        /// <returns>MediumDecVariant</returns>
        MediumDecV2Base operator --(int)
        {
            MediumDecV2Base tmp(*this);
            --* this;
            return tmp;
        }

        /// <summary>
        /// MediumDec Variant* Operator
        /// </summary>
        /// <returns>MediumDecV2Base &</returns>
        MediumDecV2Base& operator *()
        {
            return *this;
        }

    #pragma endregion Other Operators

	#pragma region Math Etc Functions

	#pragma endregion Math Etc Functions

	#pragma region Pow and Sqrt Functions

        /// <summary>
        /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
        /// </summary>
        auto SqrtOf(const int& precision=7)
        {
    #if !defined(AltNum_EnableIRep)
            if(IsNegative())
                throw "Can't display result of negative square root without imaginary number support";
    #endif
            if (DecimalHalf.Value == 0&&DecimalHalf.Flags==0)
            {
                auto value = this;
                bool AutoSetValue = true;
                switch (IntHalf.Value)
                {
                case 1: value.IntHalf = 1; break;
                case 4: value.IntHalf = 2; break;
                case 9: value.IntHalf = 3; break;
                case 16: value.IntHalf = 4; break;
                case 25: value.IntHalf = 5; break;
                case 36: value.IntHalf = 6; break;
                case 49: value.IntHalf = 7; break;
                case 64: value.IntHalf = 8; break;
                case 81: value.IntHalf = 9; break;
                case 100: value.IntHalf = 10; break;
                case 121: value.IntHalf = 11; break;
                case 144: value.IntHalf = 12; break;
                case 169: value.IntHalf = 13; break;
                case 196: value.IntHalf = 14; break;
                case 225: value.IntHalf = 15; break;
                case 256: value.IntHalf = 16; break;
                case 289: value.IntHalf = 17; break;
                case 324: value.IntHalf = 18; break;
                case 361: value.IntHalf = 19; break;
                case 400: value.IntHalf = 20; break;
                case 1600: value.IntHalf = 40; break;
                default:
                    AutoSetValue = false;
                    break;
                }
                if(AutoSetValue)
                {
    #if defined(AltNum_EnableIRep)
                    if(IsNegative())
                        DecimalHalf.Flags = 3;
    #endif
                    return value;//Techically both positive and negative numbers of same equal the result
                }
            }

            auto number = this;
            auto start = 0, end = number;
            auto mid;

            // variable to store the answer
            auto ans;

            // for computing integral part
            // of square root of number
            while (start <= end) {
                mid = (start + end) / 2;
                if (mid * mid == number) {
                    ans = mid;
                    break;
                }

                // incrementing start if integral
                // part lies on right side of the mid
                if (mid * mid < number) {
                    start = mid + 1;
                    ans = mid;
                }

                // decrementing end if integral part
                // lies on the left side of the mid
                else {
                    end = mid - 1;
                }
            }

            // For computing the fractional part
            // of square root up to given precision
            auto increment = "0.1";
            for (int i = 0; i < precision; ++i) {
                while (ans * ans <= number) {
                    ans += increment;
                }

                // loop terminates when ans * ans > number
                ans = ans - increment;
                increment = increment / 10;
            }
            return ans;
        }

		/// <summary>
        /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
        /// </summary>
		static auto Sqrt(const auto& value, const int& precision=7)
		{
			return value.SqrtOf(precision);
		}

protected:

        /// <summary>
        /// Applies Power of operation (for unsigned integer exponents)
        /// without flipping of negative status and other checks
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<IntegerType IntType=unsigned int>
        auto PartialUIntPowOp(const IntType& expValue)
        {
            if (DecimalHalf.Value == 0 && IntHalf.Value == 10)
            {
                IntHalf.Value = VariableConversionFunctions::PowerOfTens[expValue];
                DecimalHalf.Value = 0; ResetDivisor();
            }
            else
            {
                //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                auto self = AbsOf();
                IntHalf = 1; DecimalHalf = 0;// Initialize result
                while (expValue > 0)
                {
                    // If expValue is odd, multiply self with result
                    if (expValue % 2 == 1)
                        this *= self;
                    // n must be even now
                    expValue = expValue >> 1; // y = y/2
                    self = self * self; // Change x to x^2
                }
            }
            return *this;
        }

        /// <summary>
        /// Applies Power of operation on references(for integer exponents)
        /// without flipping of negative status and other checks
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<IntegerType IntType=signed int>
        auto PartialIntPowOfOp(const IntType& expValue)
        {
            if (expValue < 0)//Negative Pow
            {
                IntType exp = expValue * -1;
				//Code(Reversed in application) based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
				auto self = AbsOf();
				IntHalf = 1; DecimalHalf = 0;// Initialize result
				while (expValue > 0)
				{
					// If expValue is odd, multiply self with result
					if (exp & 1 == 1)
						*this /= self;
					// n must be even now
					expValue = expValue >> 1; // y = y/2
					self *= self; // Change x to x^2
				}
                return *this;
            }
            else if (DecimalHalf.Value == 0 && IntHalf.Value == 10)
            {
                IntHalf.Value = VariableConversionFunctions::PowerOfTens[expValue];
                DecimalHalf.Value = 0; ResetDivisor();
            }
            else
            {
                //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                auto self = AbsOf();
                IntHalf = 1; DecimalHalf = 0;// Initialize result
                while (expValue > 0)
                {
                    // If expValue is odd, multiply self with result
                    if (expValue % 2 == 1)
                        this *= self;
                    // n must be even now
                    expValue = expValue >> 1; // y = y/2
                    self = self * self; // Change x to x^2
                }
            }
            return *this;
        }

        /// <summary>
        /// Applies Power of operation (for unsigned integer exponents)
        /// without checking for specific representation type
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<IntegerType IntType=unsigned int>
        auto BasicUIntPowOpV1(const IntType& expValue)
        {
            auto convertedVal = ConvertAsNormTypeV2();
            if (convertedVal.DecimalHalf == 0 && convertedVal.IntHalf.Value == 10)
            {
                if(IsNegative()&&exp&1==1)
                    IntHalf.Sign = 1;
                IntHalf.Value = VariableConversionFunctions::PowerOfTens[expValue];
                DecimalHalf = 0; ResetDivisor();
            }
            else
            {
                //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                bool IsNegative = IsPositive()?false:exp&1==1?false:true;
                auto self = AbsOf();
                IntHalf = 1; DecimalHalf = 0;// Initialize result
                while (expValue > 0)
                {
                    // If expValue is odd, multiply self with result
                    if (expValue % 2 == 1)
                        this *= self;
                    // n must be even now
                    expValue = expValue >> 1; // y = y/2
                    self *= self; // Change x to x^2
                }
                if(IsNegative)
                    IntHalf.Sign = 0;
            }
            return *this;
        }

        /// <summary>
        /// Applies Power of operation (for integer exponents)
        /// without checking for specific representation type
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<IntegerType IntType=signed int>
        auto BasicIntPowOfOpV1(const IntType& expValue)
        {
            auto convertedVal = ConvertAsNormTypeV2();
            if (expValue < 0)//Negative Pow
            {
                auto convertedVal = ConvertAsNormTypeV2();
                IntType exp = expValue * -1;
                if (convertedVal.DecimalHalf.Value == 0 && convertedVal.IntHalf == 10 && expValue >= -9)
                {
                    IntHalf = 0; DecimalHalf = DecimalOverflow / VariableConversionFunctions::PowerOfTens[exp];
                    if(IsNegative()&&exp&1==1)
                        Sign = 1;
                }
                else
                {
                    //Code(Reversed in application) based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                    //Switches from negative to positive if exp is odd number
                    bool IsNegative = IsPositive()?false:exp&1==1?false:true;
                    auto self = AbsOf();
                    IntHalf = 1; DecimalHalf = 0;// Initialize result
                    while (expValue > 0)
                    {
                        // If expValue is odd, divide self with result
                        if (exp & 1 == 1)
                            *this /= self;
                        // n must be even now
                        expValue = expValue >> 1; // y = y/2
                        self *= self; // Change x to x^2
                    }
                    if(IsNegative)
                        IntHalf.Sign = 0;
                }
                return *this;
            }
            else if (convertedVal.DecimalHalf.Value == 0 && convertedVal.IntHalf.Value == 10)
            {
                if(IsNegative()&&exp&1==1)
                    IntHalf.Sign = 1;
                IntHalf.Value = VariableConversionFunctions::PowerOfTens[expValue];
                DecimalHalf = 0; ResetDivisor();
            }
            else
            {
                //Code based on https://www.geeksforgeeks.org/write-an-iterative-olog-y-function-for-powx-y/
                bool IsNegative = IsPositive()?false:exp&1==1?false:true;
                auto self = AbsOf();
                IntHalf = 1; DecimalHalf = 0;// Initialize result
                while (expValue > 0)
                {
                    // If expValue is odd, multiply self with result
                    if (expValue % 2 == 1)
                        this *= self;
                    // n must be even now
                    expValue = expValue >> 1; // y = y/2
                    self = self * self; // Change x to x^2
                }
                if(IsNegative)
                    IntHalf.Sign = 0;
            }
            return *this;
        }

        /// <summary>
        /// Applies Power of operation(for unsigned integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        auto BasicUIntPowOfV1(const ValueType& expValue)
        {
            auto self = this;
            return self.BasicUIntPowOpV1();
		}

        /// <summary>
        /// Applies Power of operation(for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        auto BasicIntPowOfV1(const ValueType& expValue)
        {
            auto self = this;
            return self.BasicIntPowOpV1();
		}

public:

        constexpr auto BasicUIntPowOfOp = BasicUIntPowOfOpV1<unsigned int>;
        constexpr auto BasicIntPowOfOp = BasicIntPowOfOpV1<signed int>;
        constexpr auto BasicUInt64PowOfOp = BasicUIntPowOfOpV1<UInt64>;
        constexpr auto BasicInt64PowOfOp = BasicIntPowOpOfV1<Int64>;

        constexpr auto BasicUIntPowOf = BasicUIntPowOfV1<unsigned int>;
        constexpr auto BasicIntPowOf = BasicIntPowOfV1<signed int>;
        constexpr auto BasicUInt64PowOf = BasicUIntPowOfV1<UInt64>;
        constexpr auto BasicInt64PowOf = BasicIntPowOfV1<Int64>;

        /// <summary>
        /// Finds nTh Root of value based on https://www.geeksforgeeks.org/n-th-root-number/ code
        /// </summary>
        /// <param name="nValue">The nth root value.</param>
        /// <param name="precision">Precision level (smaller = more precise)</param>
        /// <returns>auto</returns>
        constexpr auto NthRootOf = MediumDecBase::NthRootOf;

	#if defined(AltNum_EnablePiRep)
        /// <summary>
        /// Multiply by Pi exp times
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        auto MultiplyByPiPower(const ValueType& exp)
        {
            auto PiPower = PiNum.BasicPow(exp);
            BasicUnsignedMultOp(PiPower);
        }

        /// <summary>
        /// Divide by Pi exp times
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        auto DivideByPiPower(const ValueType& exp)
        {
            auto PiPower = PiNum.BasicPow(exp);
            BasicUnsignedDivOp(PiPower);
        }
    #endif

	#if defined(AltNum_EnableERep)
        /// <summary>
        /// Multiply by E exp times
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        auto MultiplyByEPower(const ValueType& exp)
        {
            auto EPower = ENum.BasicPow(exp);
            BasicUnsignedMultOp(PiPower);
        }

        /// <summary>
        /// Divide by E exp times
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        auto DivideByEPower(const ValueType& exp)
        {
            auto EPower = ENum.BasicPow(exp);
            BasicUnsignedMultOp(PiPower);
        }
    #endif

protected:

        /// <summary>
        /// Applies Power of operation (for unsigned integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        auto UIntPowOpV1(const ValueType& expValue)
        {
			if (expValue == 1)
				return *this;//Return self
			else if (expValue == 0)
				SetAsOne(); return *this;
		#if defined(AltNum_EnablePiRep)
			else if(DecimalHalf.Flags==1)
			{
				BasicUIntPowOp(expValue);
				MultiplyByPiPower(exp-1);
			}
		#endif
		#if defined(AltNum_EnableERep)
			else if(DecimalHalf.Flags==2)
			{
				if (expValue == 1)
					return *this;//Return self
				else if (expValue == 0)
					SetAsOne(); return *this;
				BasicUIntPowOp(expValue);
				MultiplyByEPower(exp-1);
			}
		#endif
		#if defined(AltNum_EnableIRep)
            else if(DecimalHalf.Flags==3)
            {
                //Add code here
            }
        #endif
        #if defined(AltNum_EnableInfinityRep)
            else if(DecimalHalf.Value==InfinityRep)
            {
            }
        #endif
        #if defined(AltNum_EnableApproaching)
            else if (DecimalHalf == ApproachingBottomRep)
            {
            }
            else if (DecimalHalf == ApproachingTopRep)
            {
            }
        #endif
        #if defined(AltNum_EnableUndefinedButInRange)
            else if(DecimalHalf.Value==UndefinedInRangeRep)
            {
            }
        #endif
        #if defined(MediumDecV2_EnableWithinMinMaxRange)
            else if(DecimalHalf.Flags==3)
            {
            }
        #endif
            else
                BasicUIntPowOpV1(expValue);
			return *this;
        }

        /// <summary>
        /// Applies Power of operation on references(for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        auto IntPowOfOpV1(const ValueType& exp)
        {
			if (expValue == 1)
				return *this;//Return self
			else if (expValue == 0)
				SetAsOne();
		#if defined(AltNum_EnablePiRep)
			else if(DecimalHalf.Flags==1)
			{
				BasicIntPowOp(expValue);
                if(exp<0)
                    DivideByPiPower(exp);
                else
				    MultiplyByPiPower(exp-1);
			}
		#endif
		#if defined(AltNum_EnableERep)
			else if(DecimalHalf.Flags==2)
			{
				if (expValue == 1)
					return *this;//Return self
				else if (expValue == 0)
					SetAsOne(); return *this;
				BasicIntPowOp(expValue);
                if(exp<0)
                    DivideByEPower(exp);
                else
				    MultiplyByEPower(exp-1);
			}
		#endif
		#if defined(AltNum_EnableIRep)
            else if(DecimalHalf.Flags==3)
            {
                //Add code here
            }
        #endif
        #if defined(AltNum_EnableInfinityRep)
            else if(DecimalHalf.Value==InfinityRep)
            {
            }
        #endif
        #if defined(AltNum_EnableApproaching)
            else if (DecimalHalf == ApproachingBottomRep)
            {
            }
            else if (DecimalHalf == ApproachingTopRep)
            {
            }
        #endif
        #if defined(AltNum_EnableUndefinedButInRange)
            else if(DecimalHalf.Value==UndefinedInRangeRep)
            {
            }
        #endif
        #if defined(MediumDecV2_EnableWithinMinMaxRange)
            else if(DecimalHalf.Flags==3)
            {
            }
        #endif
            else
                BasicIntPowOpV1(expValue);
			return *this;
        }

        /// <summary>
        /// Applies Power of operation(for unsigned integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        auto UIntPowOfV1(const ValueType& expValue)
        {
            auto self = this;
            return self.UIntPowOpV1();
		}

        /// <summary>
        /// Applies Power of operation(for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        template<typename ValueType>
        auto IntPowOfV1(const ValueType& expValue)
        {
            auto self = this;
            return self.IntPowOpV1();
		}

public:

        /// <summary>
        /// Applies Power of operation (for unsigned integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        constexpr auto UIntPowOfOp = UIntPowOfOpV1<unsigned int>;

        /// <summary>
        /// Applies Power of operation on references(for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        constexpr auto IntPowOfOp = IntPowOfOpV1<signed int>;
        constexpr auto UInt64PowOfOp = UIntPowOfOpV1<UInt64>;
        constexpr auto Int64PowOfOp = IntPowOpOfV1<Int64>;

        /// <summary>
        /// Applies Power of operation (for unsigned integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        constexpr auto UIntPowOf = UIntPowOfV1<unsigned int>;

        /// <summary>
        /// Applies Power of operation on references(for integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        constexpr auto IntPowOf = IntPowOfV1<signed int>;
        constexpr auto UInt64PowOf = UIntPowOfV1<UInt64>;
        constexpr auto Int64PowOf = IntPowOfV1<Int64>;

	#pragma endregion Pow and Sqrt Functions

	#pragma region Log Functions
protected:

        constexpr auto ExpOfV1 = MediumDecBase::ExpOf;
        constexpr auto NthRootOfV1 = MediumDecBase::NthRootOf;

public:
        /// <summary>
        /// Taylor Series Exponential function derived from https://www.pseudorandom.com/implementing-exp
        /// Does not modify owner object
        /// </summary>
        /// <returns>MediumDecVariant</returns>
        auto ExpOf()
        {
            auto self = x.ConvertAsNormType();//Prevent losing imaginary number status
            return self.ExpOfV1();
        }

        /// <summary>
        /// Taylor Series Exponential function derived from https://www.pseudorandom.com/implementing-exp
        /// Does not modify owner object
        /// </summary>
        /// <param name="x">The value to apply the exponential function to.</param>
        /// <returns>BlazesRusCode::MediumDecBase</returns>
        static auto Exp(const auto& x)
        {
			return x.ExpOf();
        }

        /// <summary>
        /// Get the (n)th Root
        /// Code based mostly from https://rosettacode.org/wiki/Nth_root#C.23
        /// Does not modify owner object
        /// </summary>
        /// <param name="n">The n value to apply with root.</param>
        /// <returns></returns>
        auto NthRootOf(const int& n, const auto& Precision = FiveBillionth)
        {
            auto self = x.ConvertAsNormTypeV2();
            return self.NthRootOfV1();
        }

        /// <summary>
        /// Get the (n)th Root
        /// Code based mostly from https://rosettacode.org/wiki/Nth_root#C.23
        /// Does not modify owner object
        /// </summary>
        /// <param name="n">The n value to apply with root.</param>
        /// <returns></returns>
        static auto NthRoot(const auto& value, const int& n, const auto& Precision = FiveBillionth)
        {
            return value.NthRootOf(n, Precision);
        }

protected:

        constexpr auto LnRef_Part02V1 = MediumDecBase::LnRef_Part02;

		auto LnRef_Part02()
		{
            auto self = x.ConvertAsNormType();
            return self.LnRef_Part02V1();
		}

        constexpr auto NaturalLogOfV1 = MediumDecBase::NaturalLogOf;
        constexpr auto Log10OfV1 = MediumDecBase::Log10Of;

public:

		/// <summary>
		/// Natural log (Equivalent to Log_E(value))
		/// </summary>
		/// <returns>BlazesRusCode::MediumDecBase</returns>
		auto NaturalLogOf()
		{
            auto self = x.ConvertAsNormType();
            return self.NaturalLogOfV1();
		}

        /// <summary>
        /// Natural log (Equivalent to Log_E(value))
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>BlazesRusCode::MediumDecBase</returns>
        static auto Ln(const auto& value)
        {
			return value.NaturalLogOf();
        }

        /// <summary>
        /// Log Base 10 of Value
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>MediumDecVariant</returns>
		auto Log10Of()
		{
            auto self = x.ConvertAsNormType();
            return self.Log10OfV1();
		}

        /// <summary>
        /// Log Base 10 of Value
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>MediumDecVariant</returns>
        static auto Log10(const auto& value)
        {
			return value.Log10Of();
        }

protected:


public:

        /// <summary>
        /// Log Base 10 of Value(integer value variant)
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>MediumDecVariant</returns>
        constexpr auto Log10OfInt = MediumDecBase::Log10OfInt;

        /// <summary>
        /// Log with Base of BaseVal of Value
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <param name="baseVal">The base of Log</param>
        /// <returns>MediumDecVariant</returns>
        auto LogOf(const auto& baseVal)
        {
            auto self = x.ConvertAsNormType();
            if (self.IsOne())
                return Zero;
            return Log10Of() / baseVal.Log10Of();
        }

        /// <summary>
        /// Log with Base of BaseVal of Value
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <param name="baseVal">The base of Log</param>
        /// <returns>MediumDecVariant</returns>
        static auto Log(const auto& value, const auto& baseVal)
        {
            return value.LogOf(baseVal);
        }

protected:


public:

        /// <summary>
        /// Log with Base of BaseVal of Value
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <param name="BaseVal">The base of Log</param>
        /// <returns>MediumDecVariant</returns>
        auto LogOfInt(const int& baseVal, const auto& threshold = FiveBillionth)
        {
            //Calculate Base log first
            auto baseTotalRes;
            bool lnMultLog = LogOfInt_BaseCalculation(baseTotalRes.ConvertAsNormTypeV2());
            return LogOf_Section02(lnMultLog, baseTotalRes.ConvertAsNormTypeV2(), threshold);
        }

        /// <summary>
        /// Log with Base of BaseVal of Value
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <param name="BaseVal">The base of Log</param>
        /// <returns>MediumDecVariant</returns>
        auto LogOfV2(const auto& baseVal, const auto& threshold = FiveBillionth)
        {
            //Calculate Base log first
            auto baseTotalRes;
            bool lnMultLog = LogOf_BaseCalculation(baseTotalRes.ConvertAsNormTypeV2());
            return LogOf_Section02(lnMultLog, baseTotalRes.ConvertAsNormTypeV2(), threshold);
        }

	#pragma endregion Log Functions

    #pragma region Trigonomic Functions
protected:

        constexpr auto SinV1 = MediumDecBase::Sin;
        constexpr auto CosV1 = MediumDecBase::Cos;
        constexpr auto TanV1 = MediumDecBase::Tan;
        constexpr auto SinFromAngleV1 = MediumDecBase::SinFromAngle;
        constexpr auto CosFromAngleV1 = MediumDecBase::CosFromAngle;
        constexpr auto TanFromAngleV1 = MediumDecBase::TanFromAngle;

public:
       /// <summary>
        /// Calculate Sine from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>MediumDecVariant</returns>
        static auto Sin(const auto& Value)
        {
            if(DecimalHalf.Flags==PiRep)
            {
                auto self = Value.ConvertAsPiNum(repType);
                if(Value.IsNegative())
                {
                    IntHalf.Value %= 2;
                    IntHalf.Value = 2 - IntHalf.Value;
                    if (Value.DecimalHalf != 0) { Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf; }
                }
                else
                    IntHalf.Value %= 2;
                if(DecimalHalf==0)//0 or 1 Pi
                    return Zero;
                else if(DecimalHalf==500000000)
                {
                    if(IntHalf==0)//)0.5 Pi
                        return One;
                    else//1.5 Pi
                        return NegativeOne;
                }
                else
                {
                    self = Value.ConvertToNormType(repType);
                    return self.SinV1(Value);
                }
            }
            else
            {
                auto self = Value.ConvertAsNormType(repType);
                return self.SinV1(Value);
            }
        }

        /// <summary>
        /// Get Cos from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>MediumDecVariant</returns>
        static auto Cos(const auto& Value)
        {
            if(DecimalHalf.Flags==PiRep)
            {
                auto self = Value.ConvertAsPiNum(repType);
                if(Value.IsNegative())
                {
                    IntHalf.Value %= 2;
                    IntHalf.Value = 2 - IntHalf.Value;
                    if (Value.DecimalHalf != 0) { Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf; }
                }
                else
                    IntHalf.Value %= 2;
                if(DecimalHalf==0)
                    if(IntHalf==0)//)0
                        return One;
                    else//1 Pi
                        return NegativeOne;
                else if(DecimalHalf==500000000)//0.5 Pi or 1.5 Pi
					return Zero;
                else
                {
                    self = Value.ConvertToNormType(repType);
                    return self.CosV1(Value);
                }
            }
            else
            {
                auto self = Value.ConvertAsNormType(repType);
                return self.CosV1(Value);
            }
        }

        /// <summary>
        /// Get Tangent from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="Value">The value in Radians.</param>
        /// <returns>MediumDecVariant</returns>
        static auto Tan(const auto& Value)
        {
            if(DecimalHalf.Flags==PiRep)
            {
                auto self = Value.ConvertAsPiNum(repType);
                if(Value.IsNegative())
                {
                    IntHalf.Value %= 2;
                    IntHalf.Value = 2 - IntHalf.Value;
                    if (Value.DecimalHalf != 0) { Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf; }
                }
                else
                    IntHalf.Value %= 2;
                if(DecimalHalf==0)//0 or 1 Pi
					return Zero;
                else if(DecimalHalf==500000000)//0.5 Pi or 1.5 Pi
				{
					if(IntHalf==0)
					#if defined(AltNum_EnableInfinityRep)
						return PositiveInfinity;
					#else
						return Maximum;
					#endif
					else
					#if defined(AltNum_EnableInfinityRep)
						return NegativeInfinity;
					#else
						return Minimum;
					#endif
				}
                else
                {
                    self = Value.ConvertToNormType(repType);
                    return self.TanV1(Value);
                }
            }
            else
            {
                auto self = Value.ConvertAsNormType(repType);
                return self.TanV1(Value);
            }
        }

        /// <summary>
        /// Gets Inverse Tangent from Value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>MediumDecVariant</returns>
        constexpr auto ATan = MediumDecBase::ATan;

        /// <summary>
        /// atan2 calculation with self normalization
        /// Application: Used when one wants to compute the 4-quadrant arctangent of a complex number (or any number with x-y coordinates) with a self-normalizing function.
        /// Example Applications: digital FM demodulation, phase angle computations
        /// Code from http://dspguru.com/dsp/tricks/fixed-point-atan2-with-self-normalization/ with some slight edit to get working
        /// </summary>
        /// <param name="y">The y.</param>
        /// <param name="X">The x.</param>
        /// <returns>MediumDecVariant</returns>
        constexpr auto ArcTan2 = MediumDecBase::ArcTan2;

        /// <summary>
        /// Get Sin from Value of angle.
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>MediumDecVariant</returns>
        static auto SinFromAngle(const auto& Value)
        {
            auto self = Value.ConvertAsNormType(repType);
            return self.SinFromAngleV1(Value);
        }

        /// <summary>
        /// Get Cos() from Value of Angle
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns></returns>
        static auto CosFromAngle(const auto& Value)
        {
            auto self = Value.ConvertAsNormType(repType);
            return self.CosFromAngleV1(Value);
        }

        /// <summary>
        /// Get Tangent from Value in Degrees (SlopeInPercent:http://communityviz.city-explained.com/communityviz/s360webhelp4-2/formulas/function_library/atan_function.htm)
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>MediumDecVariant</returns>
        static auto TanFromAngle(const auto& Value)
        {
            auto self = Value.ConvertAsNormType(repType);
            return self.TanFromAngleV1(Value);
        }

    #pragma endregion Trigonomic Functions
    };
    #pragma region String Function Source

    std::string MediumDecV2Base::ToString()
    {
        RepType repType = GetRepType();
        switch (repType)
        {
	#if defined(AltNum_EnableInfinityRep)
        case RepType::Infinity:
            return IsNegative()?"-∞":"∞";
            break;
	    #if defined(AltNum_EnableApproaching)
        case RepType::ApproachingBottom:
			#ifdef AltNum_DisplayApproachingAsReal
            return ConvertToBasicString(RepType::ApproachingBottom);
			#else
            return (std::string)IntHalf + ".0..01";
			#endif
            break;
        case RepType::ApproachingTop:
			#ifdef AltNum_DisplayApproachingAsReal
			return ConvertToBasicString(RepType::ApproachingTop);
			#else
            return (std::string)IntHalf + ".9..9";
			#endif
            break;
        #endif
	#endif
	#if defined(AltNum_EnablePiRep)
        case RepType::PiNum:
            return BasicToString()+"π";
            break;
	#endif
	#if defined(AltNum_EnableERep)
        case RepType::ENum:
            return BasicToString()+"e";
            break;
	#endif

	#if defined(AltNum_EnableIRep)
        case RepType::INum:
            return BasicToString()+"i";
            break;
	#endif
	#if defined(AltNum_EnableApproachingPi)
        case RepType::ApproachingBottomPi://equal to IntHalf.0..01 Pi
			#ifdef AltNum_DisplayApproachingAsReal
			return ConvertToBasicString(RepType::ApproachingBottom)+"π";
			#else
            return (std::string)IntHalf + ".0..01π";
			#endif
            break;
        case RepType::ApproachingTopPi://equal to IntHalf.9..9 Pi
			#ifdef AltNum_DisplayApproachingAsReal
			return ConvertToBasicString(RepType::ApproachingTop)+"π";
			#else
            return (std::string)IntHalf + ".9..9π";
			#endif
            break;
	#endif
	#if defined(AltNum_EnableApproachingE)
        case RepType::ApproachingBottomE://equal to IntHalf.0..01 e
			#ifdef AltNum_DisplayApproachingAsReal
			return ConvertToBasicString(RepType::ApproachingBottom)+"e";
			#else
            return (std::string)IntHalf + ".0..01e";
			#endif
            break;
        case RepType::ApproachingTopE://equal to IntHalf.9..9 e
			#ifdef AltNum_DisplayApproachingAsReal
			return ConvertToBasicString(RepType::ApproachingTop)+"e";
			#else
            return (std::string)IntHalf + ".9..9e";
			#endif
            break;
	#endif
    #if defined(AltNum_EnableImaginaryInfinity)
        case RepType::ImaginaryInfinity:
            return IsNegative()?"-∞i":"∞i";
            break;
	#endif

	#if defined(AltNum_EnableApproachingI)
        case RepType::ApproachingImaginaryBottom:
			#ifdef AltNum_DisplayApproachingAsReal
			return ConvertToBasicString(RepType::ApproachingBottom)+"i";
			#else
            return (std::string)IntHalf + ".0..01i";
			#endif
            break;
        case RepType::ApproachingImaginaryTop:
			#ifdef AltNum_DisplayApproachingAsReal
			return ConvertToBasicString(RepType::ApproachingTop)+"i";
			#else
            return (std::string)IntHalf + ".9..9i";
			#endif
            break;
    #endif
	#if defined(AltNum_EnableNaN)
        case RepType::Undefined:
            return "Undefined";
        case RepType::NaN:
            return "NaN";
	#endif
	#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity(value format part uses for +- range, ExtraRepValue==UndefinedInRangeRep)
        case UndefinedButInRange:
            return "UndefinedButInRange";
            break;
		/*
		#if defined(MediumDecV2_EnableWithinMinMaxRange)//Undefined except for ranged IntHalf to DecimalHalf (ExtraRepValue==UndefinedInRangeMinMaxRep)
        case WithinMinMaxRange:
		    return "WithinMinMaxRange of "+VariableConversionFunctions::UnsignedIntToStringConversion((int)IntHalf)+" to "+VariableConversionFunctions::UnsignedIntToStringConversion(DecimalHalf);
            break;
        #endif
		*/
	#endif
    #if defined(AltNum_EnableNil)
        case RepType::Nil:
            return "Nil";
    #endif
        default:
			return ConvertToBasicString(repType);
            break;
        }
    }

    #pragma endregion String Function Source
}