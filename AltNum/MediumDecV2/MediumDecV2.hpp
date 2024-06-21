// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include "MediumDecV2Preprocessors.h"
#include "..\MediumDec\MediumDec.hpp"
#include "..\MediumDec\MediumDec.hpp"
#if defined(AltNum_UseBuiltinVirtualTable)
    #include "..\VirtualTableBase.hpp"
#endif

#include "..\RepType.h"

using MirroredInt = BlazesRusCode::MirroredInt;
using PartialInt = BlazesRusCode::PartialInt;

namespace BlazesRusCode
{
    class MediumDecV2;

    //Reduced version of MediumDecV2 result for modulus result and other stuff
    class DLL_API MediumDecV2 : public MediumDec
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
		VirtualTable* InitializeVTable(){ return new VirtualTable; }
		#endif
	#endif
	protected:
		//BitFlag 01(1) = PiRep
		static const RepTypeUnderlayer PiFlag = 1;
		//BitFlag 02(2) = ERep
		static const RepTypeUnderlayer EFlag = 2;
		//BitFlag 03(4) = IRep
		static const RepTypeUnderlayer IFlag = 4;
		//BitFlag 04(8) = Fractional Rep
		static const RepTypeUnderlayer FractionalFlag = 8;
		//If BitFlag#7 and BitFlag#4 is set and BitFlag#6 isn't set,
		//then Approaching From Top side;
		//If BitFlag#7 is set and BitFlag#6 isn't set,
		//then Approaching From Bottom side;
		static const RepTypeUnderlayer ApproachFromRFlag = 8;
		//BitFlag 05 (16) = Power of flag
		static const RepTypeUnderlayer ToPowerOfFlag = 16;
		//If BitFlag#7 is set,
		//then Approaching but divided by ExtraRep
		static const RepTypeUnderlayer DividedByFlag = 16;
		//BitFlag 06 (32)= Mixed Fraction flag
		static const RepTypeUnderlayer MixedFracFlag = 32;
		//If BitFlag#7 is set and BitFlag#06 is set,
		//then Is Infinity Type
		static const RepTypeUnderlayer InfinityFlag = 32;
		//BitFlag 07 = Infinitesimal/Infinity Bit (Infinity or approaching representation)
		static const RepTypeUnderlayer InfTypeFlag = 64;
		//Bitflag 08= Undefined/NaN/Nil
		static const RepTypeUnderlayer UndefinedBit = 128;
public:
    #pragma region DigitStorage
    #pragma endregion DigitStorage

    #pragma region class_constructors

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecV2"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        MediumDecV2(const MirroredInt& intVal, const PartialInt& decVal = PartialInt::Zero)
        {
            IntHalf = intVal;
            DecimalHalf = decVal;
	#if defined(AltNum_UseBuiltinVirtualTable)&&defined(AltNum_UseNonStaticVirtualStable)
			VTable = new VirtualTable;
	#endif
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecV2"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        MediumDecV2(const signed int& intVal = 0, const PartialInt& decVal = PartialInt::Zero)
        {
            IntHalf = intVal;
            DecimalHalf = decVal;
	#if defined(AltNum_UseBuiltinVirtualTable)&&defined(AltNum_UseNonStaticVirtualStable)
			VTable = new VirtualTable;
	#endif
        }

        MediumDecV2& operator=(const MediumDecV2& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            IntHalf = rhs.IntHalf; DecimalHalf = rhs.DecimalHalf;
            return *this;
        }

        MediumDecV2& operator=(const signed int& rhs)
        {
            IntHalf = rhs; DecimalHalf = 0;
            return *this;
        }

        /// <summary>
        /// Creates class from derived class into this class
        /// (subscript operator of [])
        /// </summary>
        template<MediumDecVariant VariantType>
        MediumDecV2 operator[](VariantType variantValue) const
        {
            MediumDecV2 newSelf = MediumDecV2(variantValue.IntHalf, variantValue.DecimalHalf);
            return newSelf;
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="tValue">The value.</param>
        void SetValue(const MediumDecV2& Value); const

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="tValue">The value.</param>
        void SetMediumDecValue(const MediumDec& Value); const

        unsigned int GetFlags() const;

    #pragma endregion class_constructors

    #pragma region Negative_Status

        /// <summary>
        /// Negative Unary Operator(Flips negative status)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <returns>MediumDecV2</returns>
        MediumDecV2 operator-() const
        { MediumDecV2 self = *this; self.SwapNegativeStatus(); return self; }

    #pragma endregion Negative_Status


    #pragma region Check_if_value
    #pragma endregion Check_if_value

    #pragma region Const Representation values
    protected:
	#if defined(AltNum_EnablePiRep)
        //Pi*tValue representation(when DecimalHalf.Flags==1)
        static const unsigned int PiRep = PartialInt::PiRep;
	#endif
	#if defined(AltNum_EnableERep)
        //e*tValue representation(DecimalHalf.Flags==2)
        static const unsigned int ERep = PartialInt::ERep;
	#endif
	#if defined(AltNum_EnableIRep)
        //e*tValue representation(DecimalHalf.Flags==3)
        static const unsigned int IRep = PartialInt::IRep;
	#endif

	#if defined(AltNum_EnableInfinityRep)
        //When DecimalHalf.tValue equals this value, it represents infinity (sign of IntHalf determines if either negative or positive inifity)
		static const unsigned int InfinityRep = PartialInt::InfinityRep;
	#endif
	#if defined(AltNum_EnableApproaching)
        //When DecimalHalf.tValue equals this value, it represents Approaching IntHalf from right towards left (IntHalf.0..01)
        static const unsigned int ApproachingBottomRep = PartialInt::ApproachingBottomRep;
        //When DecimalHalf.tValue equals this value, it represents Approaching IntHalf from left towards right (IntHalf.9..9)
		static const unsigned int ApproachingTopRep = PartialInt::ApproachingTopRep;
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

		//When DecimalHalf.tValue is this value, then the indeterminate form represents 0 x Infinity
		//When IntHalf.Sign==NegativeSign, then the indeterminate form represents 0 x -Infinity
		static unsigned int ZeroTimesInfinityRep = IndeterminateThreshold+1;//1073741814;

		//When DecimalHalf.tValue is this value, then the indeterminate form represents Infinity / Infinity
		//When IntHalf.Sign==NegativeSign, then the indeterminate form represents -Infinity / Infinity
		static unsigned int InfDividedByInfRep = IndeterminateThreshold+2;

		//When DecimalHalf.tValue is this value, then the indeterminate form represents Infinity - Infinity
		static unsigned int InfMinusInfRep = IndeterminateThreshold+3;

		//When DecimalHalf.tValue is this value, then the indeterminate form represents 0 to power of 0
		static unsigned int ZeroToPowerOfZeroRep = IndeterminateThreshold+4;

		//When DecimalHalf.tValue is this value, then the indeterminate form represents IntHalf/0
		static unsigned int DivideByZeroRep = IndeterminateThreshold+9;

		static unsigned int UnknownIndeterminateRep = IndeterminateThreshold+10;
	#endif//Maximum DecimalHalf.tValue = 1073741823
    public:
    #pragma endregion Const Representation values

    #pragma region RepType

        /// <summary>
        /// Returns representation type data that is stored in value
        /// </summary>
    #if defined(AltNum_UseBuiltinVirtualTable)
        RepType VirtualTable_GetPiRepType() const
    #else
        RepType GetPiRepType() const
    #endif
        {
            switch(DecimalHalf.Value)
            {
		#if defined(AltNum_EnableApproaching)
				case ApproachingBottomRep:
					return RepType::ApproachingTopPi; break;
				case ApproachingTopRep:
					return RepType::ApproachingTopPi; break;
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
        RepType VirtualTable_GetERepType() const
    #else
        RepType GetERepType() const
    #endif
        {
            switch(DecimalHalf.tValue)
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
        RepType VirtualTable_GetIRepType() const
    #else
        RepType GetIRepType() const
    #endif
        {
            switch(DecimalHalf.tValue)
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
        RepType VirtualTable_GetNormRepType() const
    #else
        RepType GetNormRepType() const
    #endif
        {
            switch(DecimalHalf.Value)
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
					if(DecimalHalf>IndeterminateRep)
						return RepType::IndeterminateForm;
		#endif
                    return RepType::NormalType; break;
            }
        }

        /// <summary>
        /// Returns representation type data that is stored in value
        /// </summary>
#if defined(AltNum_UseBuiltinVirtualTable)
        RepType VirtualTable_GetRepType() const
#else
        RepType GetRepType() const
#endif
        {
            switch(DecimalHalf.Flags)
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
				    //If DecimalHalf.tValue==???, then right side range value equals positive infinity
				    //IntHalf represents left side minimum
				    //For DecimalHalf.tValue represents right side maximum value with negative numbers represents at numbers above ???
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
        RepType GetPiRepType() const
		{
			GetVTable(VTable)->VirtualTable_GetPiRepType(VTable);
		}

        /// <summary>
        /// Returns representation type data that is stored in value(Directly calling function)
        /// </summary>
        RepType GetERepType() const
		{
			GetVTable(VTable)->VirtualTable_GetERepType(VTable);
		}

        /// <summary>
        /// Returns representation type data that is stored in value(Directly calling function)
        /// </summary>
        RepType GetIRepType() const
		{
			GetVTable(VTable)->VirtualTable_GetIRepType(VTable);
		}

        /// <summary>
        /// Returns representation type data that is stored in value(Directly calling function)
        /// </summary>
        RepType GetNormRepType() const
		{
			GetVTable(VTable)->VirtualTable_GetNormRepType(VTable);
		}

        /// <summary>
        /// Returns representation type data that is stored in value(Directly calling function)
        /// </summary>
        RepType GetRepType() const
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
        template<MediumDecVariant VariantType=MediumDecV2>
        void SetPiValV1(const VariantType& Value)
        {
            IntHalf = Value.IntHalf; DecimalHalf = PartialInt(Value.DecimalHalf.Value,1);
        }
public:
        void SetPiVal(const MediumDecV2& Value){ SetPiValV1(Value); };

        void SetPiValFromInt(const MirroredInt& Value)
        {
            IntHalf = Value; DecimalHalf = PartialInt(0,1);
        }
    #endif
    #pragma endregion PiNum Setters

    #pragma region ENum Setters
    #if defined(AltNum_EnableERep)
protected:
        template<MediumDecVariant VariantType=MediumDecV2>
        void SetEValV1(const VariantType& tValue)
        {
            IntHalf = tValue.IntHalf; DecimalHalf = PartialInt(tValue.DecimalHalf.tValue,2);
        }
public:
        void SetEVal(const MediumDecV2& tValue){ SetEValV1(tValue); };

        void SetEValFromInt(const MirroredInt& tValue)
        {
            IntHalf = tValue; DecimalHalf = PartialInt(0,2);
        }
    #endif
    #pragma endregion ENum Setters

    #pragma region INum Setters
    #if defined(AltNum_EnableIRep)
protected:
        template<MediumDecVariant VariantType=MediumDecV2>
        void SetIValV1(const VariantType& tValue)
        {
            IntHalf = tValue.IntHalf; DecimalHalf = PartialInt(tValue.DecimalHalf.tValue,3);
        }
public:
        void SetIVal(const MediumDecV2& tValue){ SetIValV1(tValue); };

        void SetIValFromInt(const MirroredInt& tValue)
        {
            IntHalf = tValue; DecimalHalf = PartialInt(0,3);
        }
    #endif
    #pragma endregion INum Setters

    #pragma region Infinity Setters
    //Infinity operations based on https://www.gnu.org/software/libc/manual/html_node/Infinity-and-NaN.html
    // and https://tutorial.math.lamar.edu/classes/calcI/typesofinfinity.aspx
    #if defined(AltNum_EnableInfinityRep)

        void SetAsInfinity()
        {
            IntHalf = MirroredInt::One; DecimalHalf = InfinityRep;
        }

        void SetAsNegativeInfinity()
        {
            IntHalf = MirroredInt::NegativeOne; DecimalHalf = InfinityRep;
        }

    #if defined(AltNum_EnableIRep)

        void SetAsImaginaryInfinity()
        {
            IntHalf = 1; DecimalHalf = PartialInt(InfinityRep,3);
        }

        void SetAsNegativeImaginaryInfinity()
        {
            IntHalf = MirroredInt::NegativeOne; DecimalHalf = PartialInt(InfinityRep,3);
        }

	#endif

	#endif
    #pragma endregion Infinity Setters

    #pragma region ApproachingZero Setters
	#if defined(AltNum_EnableApproaching)

		//Alias:SetAsApproachingValueFromRight, Alias:SetAsApproachingZero if value = 0
        //Approaching Towards values from right to left side(IntHalf.000...1)
        void SetAsApproachingBottom(const MirroredInt& value=MirroredInt::Zero)
        {
            IntHalf = value; DecimalHalf = ApproachingBottomRep;
        }

		//Alias:SetAsApproachingValueFromLeft, Alias:SetAsApproachingZeroFromLeft if value = 0
        //Approaching Towards (IntHalf-1) from Left to right side(IntHalf.999...9)
        void SetAsApproachingTop(const MirroredInt& value=MirroredInt::Zero)
        {
            IntHalf = value; DecimalHalf = ApproachingTopRep;
        }

		//SetAsApproachingBottom without changing sign
        void SetAsApproachingBottomVal(const unsigned int& value=0)
        {
            IntHalf.Value = value; DecimalHalf = ApproachingBottomRep;
        }

		//SetAsApproachingTop without changing sign
        void SetAsApproachingTopVal(const unsigned int& value=0)
        {
            IntHalf.Value = value; DecimalHalf = ApproachingTopRep;
        }

		//SetAsApproachingBottom without changing sign or flags
        void SetAsApproachingBottomValV2(const unsigned int& value=0)
        {
            IntHalf.Value = value; DecimalHalf.Value = ApproachingBottomRep;
        }

		//SetAsApproachingTop without changing sign
        void SetAsApproachingTopValV2(const unsigned int& value=0)
        {
            IntHalf.Value = value; DecimalHalf.Value = ApproachingTopRep;
        }

		#if defined(AltNum_EnablePiRep)

        void SetAsApproachingBottomPi(const MirroredInt& value=MirroredInt::Zero)
        {
            IntHalf = value; DecimalHalf = PartialInt(ApproachingBottomRep,1);
        }

        void SetAsApproachingTopPi(const MirroredInt& value=MirroredInt::Zero)
        {
            IntHalf = value; DecimalHalf = PartialInt(ApproachingBottomRep,1);
        }

		#endif

		#if defined(AltNum_EnableERep)

        void SetAsApproachingBottomE(const MirroredInt& value=MirroredInt::Zero)
        {
            IntHalf = value; DecimalHalf = PartialInt(ApproachingBottomRep,2);
        }

        //Approaching Towards (IntHalf-1) from Left to right side(IntHalf.999...9)e
        void SetAsApproachingTopE(const MirroredInt& value=MirroredInt::Zero)
        {
            IntHalf = value; DecimalHalf = PartialInt(ApproachingTopRep,2);
        }

		#endif

		#if defined(AltNum_EnableIRep)

        void SetAsApproachingBottomI(const MirroredInt& value=MirroredInt::Zero)
        {
            IntHalf = value; DecimalHalf = PartialInt(ApproachingBottomRep,3);
        }

        //Approaching Towards (IntHalf-1) from Left to right side(IntHalf.999...9)i
        void SetAsApproachingTopI(const MirroredInt& value=MirroredInt::Zero)
        {
            IntHalf = value; DecimalHalf = PartialInt(ApproachingTopRep,3);
        }

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
public:

	#if defined(AltNum_EnableNaN)
        static MediumDecV2 NaNValue()
        {
            auto NewSelf = MediumDecV2(0, NaNRep);
            return NewSelf;
        }

        static MediumDecV2 UndefinedValue()
        {
            auto NewSelf = MediumDecV2(0, UndefinedRep);
            return NewSelf;
        }
	#endif

        static MediumDecV2 AlmostOneValue();

        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up
        /// (Stored as 3.141592654)
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 PiNumValue();

        //100,000,000xPi(Rounded to 9th decimal digit)
        static MediumDecV2 HundredMilPiNumValue();

        //10,000,000xPi(Rounded to 9th decimal digit)
        static MediumDecV2 TenMilPiNumValue();

        //1,000,000xPi(Rounded to 9th decimal digit)
        static MediumDecV2 OneMilPiNumValue();

        //10xPi(Rounded to 9th decimal digit)
        static MediumDecV2 TenPiNumValue();

        static MediumDecV2 ENumValue();

        static MediumDecV2 ZeroValue();

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 OneValue();

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 TwoValue();

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 NegativeOneValue();

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 Point5Value();

        static MediumDecV2 JustAboveZeroValue();

        static MediumDecV2 OneMillionthValue();

        static MediumDecV2 FiveThousandthValue();

        static MediumDecV2 FiveMillionthValue();

        static MediumDecV2 TenMillionthValue();

        static MediumDecV2 OneHundredMillionthValue();

        static MediumDecV2 FiveBillionthValue();

        static MediumDecV2 LN10Value();

        static MediumDecV2 LN10DivValue();

        static MediumDecV2 TwiceLN10DivValue();

        static MediumDecV2 MinimumValue();

        static MediumDecV2 MaximumValue();

        static MediumDecV2 NegativePointFiveValue();

		static MediumDecV2 PointOneValue();

        static MediumDecV2 ImaginaryMinimumValue();

        static MediumDecV2 ImaginaryMaximumValue();

        static MediumDecV2 NegativePointFive;

        static MediumDecV2 AlmostOne;

        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) with tenth digit rounded up to 3.141592654
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 PiNum;

        /// <summary>
        /// Euler's number (Non-Alternative Representation)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 ENum;

        /// <summary>
        /// Returns Pi(3.1415926535897932384626433) Representation
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 Pi;

        /// <summary>
        /// Euler's number (Non-Alternative Representation)
        /// Irrational number equal to about (1 + 1/n)^n
        /// (about 2.71828182845904523536028747135266249775724709369995)
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 E;

        /// <summary>
        /// Returns the value at zero
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 Zero;

        /// <summary>
        /// Returns the value at one
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 One;

        /// <summary>
        /// Returns the value at two
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 Two;

        /// <summary>
        /// Returns the value at 0.5
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 PointFive;

        /// <summary>
        /// Returns the value at digit one more than zero (0.000000001)
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 JustAboveZero;

        /// <summary>
        /// Returns the value at .000000005
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 FiveBillionth;

        /// <summary>
        /// Returns the value at .000001000
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 OneMillionth;

        /// <summary>
        /// Returns the value at "0.005"
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 FiveThousandth;

        /// <summary>
        /// Returns the value at .000000010
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 OneGMillionth;

        //0e-7
        static MediumDecV2 TenMillionth;

        /// <summary>
        /// Returns the value at "0.000005"
        /// </summary>
        static MediumDecV2 FiveMillionth;

        /// <summary>
        /// Returns the value at negative one
        /// </summary>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 NegativeOne;

        /// <summary>
        /// Returns value of lowest non-infinite/Special Decimal State tValue that can store
        /// (-2147483647.999999999)
        /// </summary>
        static MediumDecV2 Minimum;

        /// <summary>
        /// Returns value of highest non-infinite/Special Decimal State tValue that can store
        /// (2147483647.999999999)
        /// </summary>
        static MediumDecV2 Maximum;

        /// <summary>
        /// 2.3025850929940456840179914546844
        /// (Based on https://stackoverflow.com/questions/35968963/trying-to-calculate-logarithm-base-10-without-math-h-really-close-just-having)
        /// </summary>
        static MediumDecV2 LN10;

        /// <summary>
        /// (1 / Ln10) (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static MediumDecV2 LN10Div;

        /// <summary>
        /// (1 / Ln10)*2 (Ln10 operation as division as recommended by https://helloacm.com/fast-integer-log10/ for speed optimization)
        /// </summary>
        static MediumDecV2 TwiceLN10Div;

        static MediumDecV2 PointOne;

        static MediumDecV2 ImaginaryMinimum;

        static MediumDecV2 ImaginaryMaximum;

    #pragma endregion ValueDefines

    #pragma region String Commands
protected:

        void InitialyzeAltRepFromString(const std::string& Value)
        {
            #if defined(AltNum_EnablePiRep)
            if (Value.find("Pi") != std::string::npos)
                DecimalHalf.Flags = 1;
            #endif
            #if defined(AltNum_EnableERep)
            if (tValue.last() == 'e')
                DecimalHalf.Flags = 2;
            #endif
            #if defined(AltNum_EnableIRep)
            if (tValue.last() == 'i')
                DecimalHalf.Flags = 3;
            #endif
        }

public:

        /// <summary>
        /// Reads the string.
        /// </summary>
        /// <param name="tValue">The value.</param>
        void ReadString(const std::string& Value)
        {
            MediumDec::ReadString(Value);
            InitialyzeAltRepFromString(Value);
        }

        /// <summary>
        /// Converts to string.
        /// </summary>
        /// <returns>std.string</returns>
        std::string ToBasicString() { return MediumDec::ToString(); }

        /// <summary>
        /// Converts to string with digits filled in even when empty
        /// </summary>
        /// <returns>std.string</returns>
        std::string ToFullBasicString() { return MediumDec::ToFullString(); }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class from string literal
        /// </summary>
        /// <param name="strVal">The value.</param>
        MediumDecV2(const char* strVal)
        {
            std::string Value = strVal;
            this->ReadString(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="tValue">The value.</param>
        MediumDecV2(const std::string& Value)
        {
            this->ReadString(Value);
        }

        std::string ConvertToBasicString(const RepType& repType)
        {
            auto self = ConvertAsNormType(repType);
            return self.ToBasicString();
        }


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

        MediumDecV2(const unsigned __int64& Value){ this->SetUIntVal(Value); }
        MediumDecV2(const signed __int64& Value){ this->SetIntVal(Value); }
        MediumDecV2(const unsigned char& Value){ this->SetUIntVal(Value); }
        MediumDecV2(const signed char& Value){ this->SetIntVal(Value); }
        MediumDecV2(const unsigned short& Value){ this->SetUIntVal(Value); }
        MediumDecV2(const signed short& Value){ this->SetIntVal(Value); }
        MediumDecV2(const unsigned int& Value){ this->SetUIntVal(Value); }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="tValue">The value.</param>
        MediumDecV2(const bool& Value){ this->SetBoolVal(Value); }

    #if defined(AltNum_EnableFloatingConversion)

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="tValue">The value.</param>
        MediumDec(const float& tValue){ this->SetFloatVal(tValue); }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="tValue">The value.</param>
        MediumDec(const double& tValue){ this->SetDoubleVal(tValue); }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDec"/> class.
        /// </summary>
        /// <param name="tValue">The value.</param>
        MediumDec(const long double& tValue){ this->SetDecimalVal(tValue); }

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

        void ConvertIRepToINumV2(const RepTypeEnum& repType)
        {//Assuming not zero(should not reach needing to convert the representation if RValue is zero)
            switch (repType)
            {
                case RepTypeEnum::INum:
                    break;
			/*
            #if defined(AltNum_EnableDecimaledIFractionals)
                case RepTypeEnum::INumByDiv://(tValue/(ExtraRep.tValue))*i Representation
                    {
                        BasicUnsignedDivOp(ExtraRep.tValue);
                        ExtraRep = InitialExtraRep;
                    }
                    break;
            #endif*/
            #if defined(AltNum_EnableApproaching)
            case RepTypeEnum::ApproachingImaginaryBottom:
                DecimalHalf.tValue = 1;
                break;
            case RepTypeEnum::ApproachingImaginaryTop:
                DecimalHalf.tValue = 999999999;
                break;
                /*#if defined(AltNum_EnableApproachingDivided)
            case RepTypeEnum::ApproachingImaginaryMidLeft:
                ConvertFromApproachingIMidLeftToNorm(); break;
            case RepTypeEnum::ApproachingImaginaryMidRight:
                ConvertFromApproachingIMidRightToNorm(); break;
				#endif*/
			#endif
            #if defined(AltNum_EnableInfinityRep)
            case RepTypeEnum::ImaginaryInfinity:
                IntHalf = IsPositive()?MaxIntHalf:MinIntHalf;
                DecimalHalf.tValue = 999999999;
                /*ExtraRep = InitialExtraRep;*/
                break;
            #endif
            #ifdef AltNum_EnableComplexNumbers
                case RepTypeEnum::ComplexIRep:
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

        void ConvertIRepToINum(const RepType& repType)
        {
            ConvertIRepToINumV2(repType);
        }

		auto ConvertAsNormalIRepV2(const RepTypeEnum& repType) const
        {
            auto Res = *this;
            Res.ConvertIRepToINumV2(repType);
            return Res;
        }

		auto ConvertAsNormalIRep(const RepType& repType) const
        {
            auto Res = *this;
            Res.ConvertIRepToINum(repType);
            return Res;
        }

    #endif
    #pragma endregion region Imaginary Conversion


    #pragma region Other RepType Conversion
protected:
        //Returns value as normal type or INum representation
        template<MediumDecVariant VariantType=MediumDecV2>
        static void ConvertToNormTypeV3(VariantType& lValue, const RepTypeEnum& repType)
        {
            switch (repType)
            {
            case RepTypeEnum::NormalType:
                break;
	#if defined(AltNum_EnablePiRep)
            case RepTypeEnum::PiNum:
                lValue.ConvertPiToNum(); break;
		#if defined(AltNum_EnableApproaching)
            case RepTypeEnum::ApproachingBottomPi:
                lValue.DecimalHalf.Value = 1;
                break;
            case RepTypeEnum::ApproachingTopPi:
                lValue.DecimalHalf.Value = 999999999;
                break;
		#endif
	#endif
	#if defined(AltNum_EnableERep)
            case RepTypeEnum::ENum:
                lValue.ConvertENumToNum(); break;
		#if defined(AltNum_EnableApproaching)
            case RepTypeEnum::ApproachingBottomPi:
                lValue.DecimalHalf.tValue = 1;
                break;
            case RepTypeEnum::ApproachingTopPi:
                lValue.DecimalHalf.tValue = 999999999;
                break;
		#endif
	#endif
	#if defined(AltNum_EnableInfinityRep)
			case RepTypeEnum::Infinity:
				lValue.IntHalf = IsPositive()?MaxIntHalf:MinIntHalf;
				lValue.DecimalHalf = 999999999;
				break;
	#endif
	#if defined(AltNum_EnableApproaching)
			case RepTypeEnum::ApproachingBottom:
				lValue.DecimalHalf = 1;
				break;
			case RepTypeEnum::ApproachingTop:
				lValue.DecimalHalf = 999999999;
				break;
	#endif
	#if defined(AltNum_EnableIRep)
			case RepTypeEnum::INum:
				break;
		#if defined(AltNum_EnableApproaching)
			case RepTypeEnum::ApproachingImaginaryBottom:
				lValue.DecimalHalf.tValue = 1;
				break;
			case RepTypeEnum::ApproachingImaginaryTop:
				lValue.DecimalHalf.tValue = 999999999;
				break;
		#endif
		#if defined(AltNum_EnableInfinityRep)
			case RepTypeEnum::ImaginaryInfinity:
				lValue.IntHalf = IsPositive()?MaxIntHalf:MinIntHalf;
				lValue.DecimalHalf.tValue = 999999999;
				break;
		#endif
	#endif
	#ifdef AltNum_EnableComplexNumbers
            case RepTypeEnum::ComplexIRep:
                throw "Conversion from complex number to real number not supported yet.";
                break;
	#endif
            default:
                throw "Conversion to normal number not supported yet?";
                break;
            }
        }

public:
        void ConvertToNormfromEnum(const RepTypeEnum& repType)
        {
            ConvertToNormTypeV3(*this, repType);
        }

        //Returns value as normal type or INum representation
        void ConvertToNormType(const RepType& repType)
        {
            ConvertToNormfromEnum(repType.Value);
        }

protected:

        template<MediumDecVariant VariantType=MediumDecV2>
        VariantType ConvertAsNormTypeV1(const RepType& repType) const
        {
            VariantType Res = *this;
            Res.ConvertToNormType(repType);
            return Res;
        }

public:
		//Returns value as normal type or INum representation
        MediumDecV2 ConvertAsNormType(const RepType& repType) const
        { return ConvertAsNormTypeV1<MediumDecV2>(repType); }

        //Converts value to normal type representation
        void ConvertToNormTypeV2()
        {
            RepType repType = GetRepType();
            ConvertToNormType(repType);
        }

		//Returns value as normal type representation
        MediumDecV2 ConvertAsNormTypeV2() const
        {
            MediumDecV2 Res = *this;
            Res.ConvertToNormTypeV2();
            return Res;
        }

	#if defined(AltNum_EnablePiRep)||defined(AltNum_EnableERep)

        RepType ConvertToNormalEquivalantV2(const RepTypeEnum& repType)
        {
			switch(repType)
			{
		#if defined(AltNum_EnablePiRep)
				case RepTypeEnum::PiNum:{
					BasicUnsignedMultOp(PiNum); DecimalHalf.Flags = 0;
					return RepType::NormalType;
				}break;
		#endif
		#if defined(AltNum_EnableERep)
				case RepTypeEnum::ENum:{
					BasicUnsignedMultOp(ENum); DecimalHalf.Flags = 0;
					return RepType::NormalType;
				}	break;
		#endif
			#if defined(AltNum_EnableApproaching)
				#if defined(AltNum_EnablePiRep)
				case RepTypeEnum::ApproachingBottomPi:
				#endif
				#if defined(AltNum_EnableERep)
				case RepTypeEnum::ApproachingBottomE:
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
					#if defined(AltNum_EnablePiRep)
				case RepTypeEnum::ApproachingTopPi:
					#endif
					#if defined(AltNum_EnableERep)
				case RepTypeEnum::ApproachingTopE:
					#endif
					ConvertToNormType(repType);
					return RepType::NormalType;
					break;
			#endif
				default:
					return repType;
			}
		}

        RepType ConvertToNormalEquivalant(const RepType& repType)
        {
            ConvertToNormalEquivalantV2(repType.Value);
        }

        template<MediumDecVariant VariantType=MediumDecV2>
        static std::pair<VariantType, RepType> ConvertAsNormalEquivalantV2(VariantType tValue, const RepTypeEnum& repType)
        {
            RepType convertedRep = tValue.ConvertToNormalEquivalantV2(repType);
            return std::make_pair(tValue, convertedRep);
		}

        template<MediumDecVariant VariantType=MediumDecV2>
        static std::pair<VariantType, RepType> ConvertAsNormalEquivalantV1(VariantType tValue, const RepType& repType)
        {
            RepType convertedRep = tValue.ConvertToNormalEquivalant(repType);
            return std::make_pair(tValue, convertedRep);
		}

		//Returns std::pair of tValue and RepType
        std::pair<MediumDecV2, RepType> ConvertAsNormalEquivalantFromEnum(const RepTypeEnum& repType) const
        {
            return ConvertAsNormalEquivalantV2(*this, repType);
		}

		//Returns std::pair of tValue and RepType
        std::pair<MediumDecV2, RepType> ConvertAsNormalEquivalant(const RepType& repType) const
        {
            return ConvertAsNormalEquivalantV1(*this, repType);
		}
	#endif

    #pragma endregion Other RepType Conversion

    #pragma region Comparison Operators
protected:

		std::strong_ordering BasicComparison(const MediumDecV2& that) const
		{
            return BasicComparisonV1(that);
        }

    #if defined(AltNum_EnableInfinityRep)
		template<MediumDecVariant VariantType=MediumDecV2>
		static std::strong_ordering LSideInfinityComparison(const VariantType lValue, const VariantType& that, const RepType& RRep)
		{
			if(lValue.IntHalf.IsPositive())
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

		template<MediumDecVariant VariantType=MediumDecV2>
		static std::strong_ordering CompareWithV1(const VariantType lValue, const MediumDecV2& that)
		{
	#if defined(MediumDecV2_EnableWithinMinMaxRange)
			if(lValue.DecimalHalf.Flags==3) {
				if(that.DecimalHalf.Flags==3) {
					//To-do compare within min-max range code here
				}
				else {
					//To-do compare within min-max range code here
				}
			}
			else if(that.DecimalHalf.Flags==3) {
				//To-do compare within min-max range code here
			}
	#endif
			//Comparing if number is negative vs positive
    #if defined(AltNum_UseInvertedSign)
			auto SignCmp = lValue.IntHalf.Sign <=> that.IntHalf.Sign;
    #else   //(inverted comparison so sign of zero==positive)
            auto SignCmp = that.IntHalf.Sign <=> lValue.IntHalf.Sign;
    #endif
	        if (SignCmp != 0)
				return SignCmp;
			RepType LRep = lValue.GetRepType();
			RepType RRep = that.GetRepType();
    #if defined(AltNum_EnableNaN)||defined(AltNum_EnableNilRep)||defined(AltNum_EnableUndefinedButInRange)
			if(LRep.Value^UndefinedBit||RRep.Value^UndefinedBit)
				throw "Can't compare undefined/nil representations yet";
    #endif
    #if defined(AltNum_EnableIRep)
            if (lValue.DecimalHalf.Flags == 3)
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
			    	return BasicComparison(rSide);
            }
            else if(RValue.Flags==3)
                throw "Can't compare imaginary number with real number";
    #endif
	#if defined(AltNum_EnableInfinityRep)
            if(LRep==RepType:Infinity)
                return lValue.LSideInfinityComparison(that, RRep);
	#endif
			if(LRep==RRep)
				return lValue.BasicComparison(that);
    #if defined(AltNum_EnableInfinityRep)
			else if(RRep==RepTypeEnum::Infinity)
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
		#endif
            }
    #endif
            else
			{
				VariantType lSide = lValue;
				VariantType rSide = that;
				lSide.ConvertToNormTypeV2(); rSide.ConvertToNormTypeV2();
				return lSide.BasicComparison(rSide);
			}
		}

		//Templated version of Spaceship operator to allow full version of class to inherit the spaceship operator code
		template<MediumDecVariant VariantType=MediumDecV2, IntegerType IntType=signed int>
		static std::strong_ordering CompareWithUIntV1(const VariantType& lValue, const IntType& that)
		{
    #if defined(AltNum_EnableIRep)
            if(lValue.GetFlags()==3)
                throw "Can't compare imaginary number with real number except for checking if equal.";
    #endif
			if(lValue.GetFlags() == 0)
			{
				return lValue.BasicUIntComparison(that);
			}
			else
			{
				VariantType lSide = lValue;
				lSide.ConvertToNormTypeV2();
				return lSide.BasicUIntComparison(that);
			}
		}

		template<MediumDecVariant VariantType=MediumDecV2, IntegerType IntType=signed int>
		static std::strong_ordering CompareWithIntV1(const VariantType& lValue,const IntType& that)
		{
    #if defined(AltNum_EnableIRep)
            if(lValue.GetFlags()==3)
                throw "Can't compare imaginary number with real number except for checking if equal.";
    #endif
			if(lValue.GetFlags()==0)
			{
				return lValue.BasicIntComparison(that);
			}
			else
			{
				VariantType lSide = lValue;
				lSide.ConvertToNormTypeV2();
				return lSide.BasicIntComparison(that);
			}
		}

        std::strong_ordering CompareWith(const MediumDecV2& that) const
		{
            return CompareWithV1(*this, that);
        }

		std::strong_ordering CompareWithInt(const unsigned int& that) const
		{
            return CompareWithUIntV1(*this, that);
        }

		std::strong_ordering CompareWithInt(const signed int& that) const
		{
            return CompareWithIntV1(*this, that);
        }

public:
		std::strong_ordering operator<=>(const MediumDecV2& that) const
		{
			return CompareWith(that);
		}

		std::strong_ordering operator<=>(const int& that) const
		{
    #if defined(AltNum_EnableIRep)
            if(DecimalHalf.Flags==3)
                throw "Can't compare imaginary number with real number except for checking if equal.";
    #endif
            MediumDecV2 LValue = *this;
            LValue.ConvertToNormTypeV2();
			return LValue.CompareWithInt(that);
		}

		bool operator==(const MediumDecV2& that) const
		{
			if (IntHalf!=that.IntHalf)
				return false;
			if (DecimalHalf!=that.DecimalHalf)
				return false;
            return true;
		}

		bool operator!=(const MediumDecV2& that) const
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
            MediumDec::UIntDivOpV1(Value);
        }

        template<IntegerType IntType=signed int>
        void BasicIntDivOpV1(const IntType& Value)
        {
            MediumDec::IntDivOpV1(Value);
        }

        template<MediumDecVariant VariantType=MediumDecV2, IntegerType IntType=unsigned int>
        VariantType& BasicUIntDivOperationV1(const IntType& rValue)
        { UIntDivOpV1(rValue); return *this; }

        template<MediumDecVariant VariantType=MediumDecV2, IntegerType IntType=unsigned int>
        VariantType& BasicIntDivOperationV1(const IntType& rValue)
        { BasicIntDivOpV1(rValue); return *this; }

        /// <summary>
        /// Basic division operation between MediumDec Variant and unsigned Integer value
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<MediumDecVariant VariantType=MediumDecV2, IntegerType IntType=unsigned int>
        const VariantType BasicDivideByUIntV1(const IntType& rValue)
        { auto self = *this; return self.BasicUIntDivOperationV1(rValue); }

        /// <summary>
        /// Basic division operation between MediumDec Variant and unsigned Integer value
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<MediumDecVariant VariantType=MediumDecV2, IntegerType IntType=signed int>
        const VariantType BasicDivideByIntV1(const IntType& rValue)
        { auto self = *this; return self.BasicIntDivOperationV1(rValue); }

public:

        void BasicUIntDivOp(const unsigned int& rValue) { BasicUIntDivOpV1(rValue); }
        void BasicIntDivOp(const signed int& rValue) { BasicIntDivOpV1(rValue); }
        void BasicUInt64DivOp(const unsigned __int64& rValue) { BasicUIntDivOpV1(rValue); }
        void BasicInt64DivOp(const signed __int64& rValue) { BasicIntDivOpV1(rValue); }

        void BasicUnsignedIntegerDivOp(const signed int& rValue) { BasicUIntDivOpV1(rValue); }

        void BasicUInt8DivOp(const unsigned char& rValue) { BasicUIntDivOpV1(rValue); }
        void BasicInt8DivOp(const signed char& rValue) { BasicIntDivOpV1(rValue); }
        void BasicUInt16DivOp(const unsigned short& rValue) { BasicUIntDivOpV1(rValue); }
        void BasicInt16DivOp(const signed short& rValue) { BasicIntDivOpV1(rValue); }

        MediumDecV2& BasicUIntDivOperation(const unsigned int& rValue) { return BasicUIntDivOperationV1(rValue); }
        MediumDecV2& BasicIntDivOperation(const signed int& rValue) { return BasicIntDivOperationV1(rValue); }
        MediumDecV2& BasicUInt64DivOperation(const unsigned __int64& rValue) { return BasicUIntDivOperationV1(rValue); }
        MediumDecV2& BasicInt64DivOperation(const signed __int64& rValue) { return BasicIntDivOperationV1(rValue); }
        MediumDecV2& BasicUInt8DivOperation(const unsigned char& rValue) { return BasicUIntDivOperationV1(rValue); }
        MediumDecV2& BasicInt8DivOperation(const signed char& rValue) { return BasicIntDivOperationV1(rValue); }
        MediumDecV2& BasicUInt16DivOperation(const unsigned short& rValue) { return BasicUIntDivOperationV1(rValue); }
        MediumDecV2& BasicInt16DivOperation(const signed short& rValue) { return BasicIntDivOperationV1(rValue); }

        const MediumDecV2 BasicDivideByUInt(const unsigned int& rValue) { return BasicDivideByUIntV1(rValue); }
        const MediumDecV2 BasicDivideByInt(const signed int& rValue) { return BasicDivideByIntV1(rValue); }
        const MediumDecV2 BasicDivideByUInt64(const unsigned __int64& rValue) { return BasicDivideByUIntV1(rValue); }
        const MediumDecV2 BasicDivideByInt64(const signed __int64& rValue) { return BasicDivideByIntV1(rValue); }

        const MediumDecV2 BasicUnsignedDivideByInt(const signed int& rValue) { return BasicDivideByUIntV1(rValue); }
        const MediumDecV2 BasicUnsignedDivideByInt64(const signed __int64& rValue) { return BasicDivideByUIntV1(rValue); }

        const MediumDecV2 BasicDivideByUInt8(const unsigned char& rValue) { return BasicDivideByUIntV1(rValue); }
        const MediumDecV2 BasicDivideByInt8(const signed char& rValue) { return BasicDivideByIntV1(rValue); }
        const MediumDecV2 BasicDivideByUInt16(const unsigned short& rValue) { return BasicDivideByUIntV1(rValue); }
        const MediumDecV2 BasicDivideByInt16(const signed short& rValue) { return BasicDivideByIntV1(rValue); }

    #pragma endregion NormalRep Integer Division Operations

	#pragma region NormalRep AltNum Division Operations

		/// <summary>
        /// Basic unsigned division operation(main code block)
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The rValue</param>
        bool BasicUnsignedPartialDivOp(const MediumDecV2& rValue){ return MediumDec::UnsignedPartialDivOpV1<MediumDecV2>(rValue); }

		/// <summary>
        /// Unsigned division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side value</param>
        void BasicUnsignedDivOp(const MediumDecV2& rValue){ MediumDec::UnsignedDivOpV1<MediumDecV2>(rValue); }

		/// <summary>
        /// Basic division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        void BasicDivOp(const MediumDecV2& rValue){ MediumDec::DivOpV1<MediumDecV2>(rValue); }

		/// <summary>
        /// Basic unsigned division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        MediumDecV2& BasicUnsignedDivOperation(const MediumDecV2& rValue)
		{ BasicUnsignedDivOp(rValue); return *this; }

		/// <summary>
        /// Basic division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        MediumDecV2& BasicDivOperation(const MediumDecV2& rValue)
		{ BasicDivOp(rValue); return *this; }

		/// <summary>
        /// Basic unsigned division operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        const MediumDecV2 BasicDivideByUnsigned(const MediumDecV2& rValue)
        { MediumDecV2 lValue = *this; return lValue.BasicUnsignedDivOperation(rValue); }

		/// <summary>
        /// Basic division operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        const MediumDecV2 BasicDivideBy(const MediumDecV2& rValue)
        { MediumDecV2 lValue = *this; return lValue.BasicDivOperation(rValue); }

	#pragma endregion NormalRep AltNum Division Operations

	#pragma region Other Division Operations
protected:
        template<IntegerType IntType=signed int>
		void CatchAllUIntDivision(const IntType& rValue, const RepType& LRep)
		{
            ConvertToNormType(LRep);
			BasicUIntDivOp(rValue);
		}

        template<IntegerType IntType=unsigned int>
		void CatchAllIntDivision(const IntType& rValue, const RepType& LRep)
		{
            ConvertToNormType(LRep);
			BasicIntDivOp(rValue);
		}

        template<IntegerType IntType=signed int>
        void CatchAllUIntDivisionV2(const IntType& rValue)
        {
			ConvertToNormTypeV2();
			BasicUIntDivOp(rValue);
        }

        template<IntegerType IntType=signed int>
        void CatchAllIntDivisionV2(const IntType& rValue)
        {
			ConvertToNormTypeV2();
			BasicIntDivOp(rValue);
        }

        template<MediumDecVariant VariantType=MediumDecV2>
		void CatchAllUnsignedDivision(const VariantType& rValue, const RepType& LRep, const RepType& RRep)
		{
			ConvertToNormType(LRep);
			auto convertedRVal = rValue.ConvertAsNormType(RRep);
			BasicUnsignedDivOp(convertedRVal);
		}

        template<MediumDecVariant VariantType=MediumDecV2>
		void CatchAllUnsignedDivisionV2(const VariantType& rValue, const RepType& SameRep)
		{
            ConvertToNormType(SameRep);
			auto convertedRVal = rValue.ConvertAsNormType(SameRep);
			BasicUnsignedDivOp(convertedRVal);
		}

        template<MediumDecVariant VariantType=MediumDecV2>
        void CatchAllUnsignedDivisionV3(const VariantType& rValue)
        {
	        ConvertToNormTypeV2();
            auto convertedRVal = rValue.ConvertAsNormTypeV2();
	        BasicUnsignedDivOp(convertedRVal);
        }

public:

        /// <summary>
        /// Simplified division by 2 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        void DivideByTwo();

        /// <summary>
        /// Simplified division by 4 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        void DivideByFour();

        //Return copy of result divided by two
        MediumDecV2 DividedByTwo() const;

        //Return copy of result divided by four
        MediumDecV2 DividedByFour() const;

protected:

        /// <summary>
        /// Unsigned division operation between MediumDec variant and unsigned integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
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
                #else
                throw "Target rValue can not be divided by zero";
                #endif
                return;
            }
            RepType LRep = GetNormRepType();
            switch(LRep)
            {
                case RepTypeEnum::NormalType:
                {
                    BasicUIntDivOp(rValue);
                }
                break;
#pragma region AltDecVariantExclusive
#pragma endregion AltDecVariantExclusive
    #if defined(AltNum_EnableApproaching)
                case RepTypeEnum::ApproachingBottom:
                    if (IsAtZeroInt())
                        return;
                    ConvertToNormType(LRep.ConvertFromNormalRep(GetFlags()));
                    BasicUIntDivOp(rValue);
                break;
                case RepTypeEnum::ApproachingTop:
                    ConvertToNormType(LRep.ConvertFromNormalRep(GetFlags()));
                    BasicUIntDivOp(rValue);
                    break;
#pragma region AltDecVariantExclusive
#pragma endregion AltDecVariantExclusive
    #endif
    #ifdef AltNum_EnableInfinity
                case RepTypeEnum::Infinity:
                    return;
                    break;
    #endif
                default:
                    throw "Unable to perform integer division on current representation.";
            }
        }

        /// <summary>
        /// Division operation between MediumDec variant and integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        template<IntegerType IntType= signed int>
        void IntDivOpV1(const IntType& rValue)
		{
            if(rValue<0)
            {
                SwapNegativeStatus();
                UIntDivOpV1(-rValue);
            }
            else
                UIntDivOpV1(rValue);
		}

        template<IntegerType IntType=unsigned int, MediumDecVariant VariantType=MediumDecV2>
        VariantType& UIntDivOperationV1(const IntType& rValue)
        { UIntDivOpV1(rValue); return *this; }

        template<IntegerType IntType=unsigned int, MediumDecVariant VariantType=MediumDecV2>
        VariantType& IntDivOperationV1(const IntType& rValue)
        { IntDivOpV1(rValue); return *this; }

        /// <summary>
        /// Division operation between MediumDec Variant and unsigned Integer value
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=unsigned int, MediumDecVariant VariantType=MediumDecV2>
        const auto DivideByUIntV1(const IntType& rValue) const
        { auto self = *this; return self.UIntDivOperationV1(rValue); }

        /// <summary>
        /// Division operation between MediumDec Variant and unsigned Integer value
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<IntegerType IntType=signed int, MediumDecVariant VariantType=MediumDecV2>
        const auto DivideByIntV1(const IntType& rValue) const
        { auto self = *this; return self.IntDivOperationV1(rValue); }

public:

        void UIntDivOp(const unsigned int& rValue) { UIntDivOpV1(rValue); }
        void IntDivOp(const signed int& rValue) { IntDivOpV1(rValue); }
        void UInt64DivOp(const unsigned __int64& rValue) { UIntDivOpV1(rValue); }
        void Int64DivOp(const signed __int64& rValue) { IntDivOpV1(rValue); }

        void UnsignedIntegerDivOp(const signed int& rValue) { UIntDivOpV1(rValue); }

        void UInt8DivOp(const unsigned char& rValue) { UIntDivOpV1(rValue); }
        void Int8DivOp(const signed char& rValue) { IntDivOpV1(rValue); }
        void UInt16DivOp(const unsigned short& rValue) { UIntDivOpV1(rValue); }
        void Int16DivOp(const signed short& rValue) { IntDivOpV1(rValue); }

        MediumDecV2& UIntDivOperation(const unsigned int& rValue) { return UIntDivOperationV1(rValue); }
        MediumDecV2& IntDivOperation(const signed int& rValue) { return IntDivOperationV1(rValue); }
        MediumDecV2& UInt64DivOperation(const unsigned __int64& rValue) { return UIntDivOperationV1(rValue); }
        MediumDecV2& Int64DivOperation(const signed __int64& rValue) { return IntDivOperationV1(rValue); }
        MediumDecV2& UInt8DivOperation(const unsigned char& rValue) { return UIntDivOperationV1(rValue); }
        MediumDecV2& Int8DivOperation(const signed char& rValue) { return IntDivOperationV1(rValue); }
        MediumDecV2& UInt16DivOperation(const unsigned short& rValue) { return UIntDivOperationV1(rValue); }
        MediumDecV2& Int16DivOperation(const signed short& rValue) { return IntDivOperationV1(rValue); }

        const MediumDecV2 DivideByUInt(const unsigned int& rValue) { return DivideByUIntV1(rValue); }
        const MediumDecV2 DivideByInt(const signed int& rValue) { return DivideByIntV1(rValue); }
        const MediumDecV2 DivideByUInt64(const unsigned __int64& rValue) { return DivideByUIntV1(rValue); }
        const MediumDecV2 DivideByInt64(const signed __int64& rValue) { return DivideByIntV1(rValue); }

        const MediumDecV2 UnsignedDivideByInt(const signed int& rValue) { return DivideByUIntV1(rValue); }
        const MediumDecV2 UnsignedDivideByInt64(const signed __int64& rValue) { return DivideByUIntV1(rValue); }

        const MediumDecV2 DivideByUInt8(const unsigned char& rValue) { return DivideByUIntV1(rValue); }
        const MediumDecV2 DivideByInt8(const signed char& rValue) { return DivideByIntV1(rValue); }
        const MediumDecV2 DivideByUInt16(const unsigned short& rValue) { return DivideByUIntV1(rValue); }
        const MediumDecV2 DivideByInt16(const signed short& rValue) { return DivideByIntV1(rValue); }

protected:
		void UnsignedDivOp_RValueIntSwitch(const MediumDecV2& rValue);

public:

		/// <summary>
        /// Unsigned division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        void UnsignedDivOp(const MediumDecV2& rValue);

		/// <summary>
        /// Division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        void DivOp(const MediumDecV2& Value)
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
        /// Basic unsigned division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        MediumDecV2& UnsignedDivOperation(const MediumDecV2& rValue)
		{ UnsignedDivOp(rValue); return *this; }

		/// <summary>
        /// Basic division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        MediumDecV2& DivOperation(const MediumDecV2& rValue)
		{ DivOp(rValue); return *this; }

		/// <summary>
        /// Unsigned division operation that ignores special decimal status
        /// Return true if divide into zero
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        MediumDecV2 DivideByUnsigned(const MediumDecV2& rValue)
        {
            auto self = *this;
            return self.UnsignedDivOperation(rValue);
        }

		/// <summary>
        /// Division operation that ignores special decimal status
        /// Return true if divide into zero
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        MediumDecV2 DivideBy(const MediumDecV2& rValue)
        {
            auto self = *this;
            return self.DivOperation(rValue);
        }

        /// <summary>
        /// /= operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2& operator/=(MediumDecV2& lValue, const MediumDecV2& rValue) { return lValue.DivOperation(rValue); }

        /// <summary>
        /// *= operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2& operator/=(MediumDecV2& lValue, const signed int& rValue) { return lValue.IntDivOperation(rValue); }
        friend MediumDecV2& operator/=(MediumDecV2& lValue, const signed __int64& rValue) { return lValue.Int64DivOperation(rValue); }
        friend MediumDecV2& operator/=(MediumDecV2& lValue, const unsigned int& rValue) { return lValue.UIntDivOperation(rValue); }
        friend MediumDecV2& operator/=(MediumDecV2& lValue, const unsigned __int64& rValue) { return lValue.UInt64DivOperation(rValue); }

        friend MediumDecV2& operator/=(MediumDecV2& lValue, const signed char& rValue) { return lValue.Int8DivOperation(rValue); }
        friend MediumDecV2& operator/=(MediumDecV2& lValue, const signed short& rValue) { return lValue.Int16DivOperation(rValue); }
        friend MediumDecV2& operator/=(MediumDecV2& lValue, const unsigned char& rValue) { return lValue.UInt8DivOperation(rValue); }
        friend MediumDecV2& operator/=(MediumDecV2& lValue, const unsigned short& rValue) { return lValue.UInt16DivOperation(rValue); }

        /// <summary>
        /// Division operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2 operator/(MediumDecV2 lValue, const MediumDecV2& rValue) { return lValue.DivideBy(rValue); }

        /// <summary>
        /// Division operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2 operator/(MediumDecV2 lValue, const signed int& rValue) { return lValue.IntDivOperation(rValue); }
        friend MediumDecV2 operator/(MediumDecV2 lValue, const signed __int64& rValue) { return lValue.Int64DivOperation(rValue); }
        friend MediumDecV2 operator/(MediumDecV2 lValue, const unsigned int& rValue) { return lValue.UIntDivOperation(rValue); }
        friend MediumDecV2 operator/(MediumDecV2 lValue, const unsigned __int64& rValue) { return lValue.UInt64DivOperation(rValue); }

        friend MediumDecV2 operator/(MediumDecV2 lValue, const signed char& rValue) { return lValue.Int8DivOperation(rValue); }
        friend MediumDecV2 operator/(MediumDecV2 lValue, const signed short& rValue) { return lValue.Int16DivOperation(rValue); }
        friend MediumDecV2 operator/(MediumDecV2 lValue, const unsigned char& rValue) { return lValue.UInt8DivOperation(rValue); }
        friend MediumDecV2 operator/(MediumDecV2 lValue, const unsigned short& rValue) { return lValue.UInt16DivOperation(rValue); }

        friend MediumDecV2 operator/(signed int lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).DivideBy(rValue); }
        friend MediumDecV2 operator/(signed __int64 lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).DivideBy(rValue); }
        friend MediumDecV2 operator/(const unsigned int& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).DivideBy(rValue); }
        friend MediumDecV2 operator/(const unsigned __int64& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).DivideBy(rValue); }

        friend MediumDecV2 operator/(const signed char& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).DivideBy(rValue); }
        friend MediumDecV2 operator/(const signed short& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).DivideBy(rValue); }
        friend MediumDecV2 operator/(const unsigned char& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).DivideBy(rValue); }
        friend MediumDecV2 operator/(const unsigned short& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).DivideBy(rValue); }

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
            MediumDec::UIntMultOpV1(Value);
        }

        template<IntegerType IntType=signed int>
        void BasicIntMultOpV1(const IntType& Value)
        {
            MediumDec::IntMultOpV1(Value);
        }

        template<MediumDecVariant VariantType=MediumDecV2, IntegerType IntType=unsigned int>
        VariantType& BasicUIntMultOperationV1(const IntType& rValue)
        { UIntMultOpV1(rValue); return *this; }

        template<MediumDecVariant VariantType=MediumDecV2, IntegerType IntType=unsigned int>
        VariantType& BasicIntMultOperationV1(const IntType& rValue)
        { BasicIntMultOpV1(rValue); return *this; }

        /// <summary>
        /// Basic Multiplication operation between MediumDec Variant and unsigned Integer value
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<MediumDecVariant VariantType=MediumDecV2, IntegerType IntType=unsigned int>
        const VariantType BasicMultiplyByUIntV1(const IntType& rValue)
        { auto self = *this; return self.BasicUIntMultOperationV1(rValue); }

        /// <summary>
        /// Basic Multiplication operation between MediumDec Variant and unsigned Integer value
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        /// <returns>MediumDec&</returns>
        template<MediumDecVariant VariantType=MediumDecV2, IntegerType IntType=signed int>
        const VariantType BasicMultiplyByIntV1(const IntType& rValue)
        { auto self = *this; return self.BasicIntMultOperationV1(rValue); }

public:

        void BasicUIntMultOp(const unsigned int& rValue) { BasicUIntMultOpV1(rValue); }
        void BasicIntMultOp(const signed int& rValue) { BasicIntMultOpV1(rValue); }
        void BasicUInt64MultOp(const unsigned __int64& rValue) { BasicUIntMultOpV1(rValue); }
        void BasicInt64MultOp(const signed __int64& rValue) { BasicIntMultOpV1(rValue); }

        void BasicUnsignedIntegerMultOp(const signed int& rValue) { BasicUIntMultOpV1(rValue); }

        void BasicUInt8MultOp(const unsigned char& rValue) { BasicUIntMultOpV1(rValue); }
        void BasicInt8MultOp(const signed char& rValue) { BasicIntMultOpV1(rValue); }
        void BasicUInt16MultOp(const unsigned short& rValue) { BasicUIntMultOpV1(rValue); }
        void BasicInt16MultOp(const signed short& rValue) { BasicIntMultOpV1(rValue); }

        MediumDecV2& BasicUIntMultOperation(const unsigned int& rValue) { return BasicUIntMultOperationV1(rValue); }
        MediumDecV2& BasicIntMultOperation(const signed int& rValue) { return BasicIntMultOperationV1(rValue); }
        MediumDecV2& BasicUInt64MultOperation(const unsigned __int64& rValue) { return BasicUIntMultOperationV1(rValue); }
        MediumDecV2& BasicInt64MultOperation(const signed __int64& rValue) { return BasicIntMultOperationV1(rValue); }
        MediumDecV2& BasicUInt8MultOperation(const unsigned char& rValue) { return BasicUIntMultOperationV1(rValue); }
        MediumDecV2& BasicInt8MultOperation(const signed char& rValue) { return BasicIntMultOperationV1(rValue); }
        MediumDecV2& BasicUInt16MultOperation(const unsigned short& rValue) { return BasicUIntMultOperationV1(rValue); }
        MediumDecV2& BasicInt16MultOperation(const signed short& rValue) { return BasicIntMultOperationV1(rValue); }

        const MediumDecV2 BasicMultiplyByUInt(const unsigned int& rValue) { return BasicMultiplyByUIntV1(rValue); }
        const MediumDecV2 BasicMultiplyByInt(const signed int& rValue) { return BasicMultiplyByIntV1(rValue); }
        const MediumDecV2 BasicMultiplyByUInt64(const unsigned __int64& rValue) { return BasicMultiplyByUIntV1(rValue); }
        const MediumDecV2 BasicMultiplyByInt64(const signed __int64& rValue) { return BasicMultiplyByIntV1(rValue); }

        const MediumDecV2 BasicUnsignedMultiplyByInt(const signed int& rValue) { return BasicMultiplyByUIntV1(rValue); }
        const MediumDecV2 BasicUnsignedMultiplyByInt64(const signed __int64& rValue) { return BasicMultiplyByUIntV1(rValue); }

        const MediumDecV2 BasicMultiplyByUInt8(const unsigned char& rValue) { return BasicMultiplyByUIntV1(rValue); }
        const MediumDecV2 BasicMultiplyByInt8(const signed char& rValue) { return BasicMultiplyByIntV1(rValue); }
        const MediumDecV2 BasicMultiplyByUInt16(const unsigned short& rValue) { return BasicMultiplyByUIntV1(rValue); }
        const MediumDecV2 BasicMultiplyByInt16(const signed short& rValue) { return BasicMultiplyByIntV1(rValue); }

    #pragma endregion NormalRep Integer Multiplication Operations

	#pragma region NormalRep AltNum Multiplication Operations

		/// <summary>
        /// Basic multiplication operation that ignores special decimal status with unsigned MediumDec
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        void BasicUnsignedMultOp(const MediumDecV2& rValue){ MediumDec::UnsignedMultOpV1<MediumDecV2>(rValue); }

        void BasicMultOp(const MediumDecV2& rValue){ MediumDec::MultOpV1<MediumDecV2>(rValue); }

		/// <summary>
        /// Basic unsigned Multiplication operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        MediumDecV2& BasicUnsignedMultOperation(const MediumDecV2& rValue)
		{ BasicUnsignedMultOp(rValue); return *this; }

		/// <summary>
        /// Basic Multiplication operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        MediumDecV2& BasicMultOperation(const MediumDecV2& rValue)
		{ BasicMultOp(rValue); return *this; }

		/// <summary>
        /// Basic unsigned Multiplication operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        const MediumDecV2 BasicMultiplyByUnsigned(const MediumDecV2& rValue)
        { MediumDecV2 lValue = *this; return lValue.UnsignedMultOperation(rValue); }

		/// <summary>
        /// Basic Multiplication operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        const MediumDecV2 BasicMultiplyBy(const MediumDecV2& rValue)
        { MediumDecV2 lValue = *this; return lValue.MultOperation(rValue); }

	#pragma endregion NormalRep AltNum Multiplication Operations

	#pragma region Other multiplication operations

protected:
        template<IntegerType IntType=signed int>
		void CatchAllUIntMultiplication(const IntType& rValue, const RepType& LRep)
		{
            ConvertToNormType(LRep);
			BasicUIntMultOp(rValue);
		}

        template<IntegerType IntType=unsigned int>
		void CatchAllIntMultiplication(const IntType& rValue, const RepType& LRep)
		{
            ConvertToNormType(LRep);
			BasicIntMultOp(rValue);
		}

        template<IntegerType IntType=signed int>
        void CatchAllUIntMultiplicationV2(const IntType& rValue)
        {
			ConvertToNormTypeV2();
			BasicUIntMultOp(rValue);
        }

        template<IntegerType IntType=signed int>
        void CatchAllIntMultiplicationV2(const IntType& rValue)
        {
			ConvertToNormTypeV2();
			BasicIntMultOp(rValue);
        }

		void CatchAllUnsignedMultiplication(const MediumDecV2& rValue, const RepType& LRep, const RepType& RRep)
		{
			ConvertToNormType(LRep);
			auto convertedRVal = rValue.ConvertAsNormType(RRep);
			BasicUnsignedMultOp(convertedRVal);
		}

		void CatchAllUnsignedMultiplicationV2(const MediumDecV2& rValue, const RepType& SameRep)
		{
            ConvertToNormType(SameRep);
			auto convertedRVal = rValue.ConvertAsNormType(SameRep);
			BasicUnsignedMultOp(convertedRVal);
		}

		void CatchAllUnsignedMultiplicationV3(const MediumDecV2& rValue)
		{
			ConvertToNormTypeV2();
            auto convertedRVal = rValue.ConvertAsNormTypeV2();
			BasicUnsignedMultOp(convertedRVal);
		}

public:

        /// <summary>
        /// Simplified Multiplication by 2 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        void MultiplyByTwo();

        /// <summary>
        /// Simplified Multiplication by 4 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        void MultiplyByFour();

        //Return copy of result divided by two
        MediumDecV2 MultipliedByTwo() const;

        //Return copy of result divided by four
        MediumDecV2 MultipliedByFour() const;

protected:

        /// <summary>
        /// Unsigned multiplication operation between MediumDec variant and unsigned integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        template<IntegerType IntType= unsigned int>
        void UIntMultOpV1(const IntType& rValue)
		{
            if (rValue == 1)
                return;
            else if (rValue == 0)
            {
                SetAsZero();
                return;
            }
            RepType LRep = GetNormRepType();
            switch(LRep)
            {
                case RepTypeEnum::NormalType:
                    BasicUIntMultOp(rValue);
                break;
#pragma region AltDecVariantExclusive
#pragma endregion AltDecVariantExclusive
    #if defined(AltNum_EnableApproaching)
                case RepTypeEnum::ApproachingBottom:
					if(IntHalf.Value!=0)
						CatchAllUIntMultiplication(rValue, LRep.ConvertFromNormalRep(GetFlags()));
                    break;
                case RepTypeEnum::ApproachingTop:
					if(IntHalf.Value==0)//0.99.9 * 5 = ~4.9..9
						IntHalf.Value = (int)rValue - 1;
					else//5.9..9 * 100 = 599.9..9
						IntHalf.Value = (IntHalf.Value+1)*(unsigned int)rValue - 1;
	                break;
#pragma region AltDecVariantExclusive
#pragma endregion AltDecVariantExclusive
    #endif
    #ifdef AltNum_EnableInfinity
                case RepTypeEnum::Infinity:
                    break;
    #endif
                default:
                    throw "Unable to perform integer multiplication on current representation.";
            }
        }

        /// <summary>
        /// Multiplication operation between MediumDec variant and integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        /// <returns>MediumDecVariant&</returns>
        template<IntegerType IntType= signed int>
        void IntMultOpV1(const IntType& rValue)
		{
            if(rValue<0)
            {
                SwapNegativeStatus();
                UIntMultOpV1(-rValue);
            }
            else
                UIntMultOpV1(rValue);
		}

        template<IntegerType IntType=unsigned int>
        auto& UIntMultOperationV1(const IntType& rValue)
        { UIntMultOpV1(rValue); return *this; }

        template<IntegerType IntType=unsigned int>
        auto& IntMultOperationV1(const IntType& rValue)
        { IntMultOpV1(rValue); return *this; }

        /// <summary>
        /// Unsigned Multiplication operation between MediumDec variant and unsigned integer values
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        /// <returns>MediumDecVariant</returns>
        template<IntegerType IntType= unsigned int>
        MediumDecV2 MultiplyByUIntV1(const IntType& rValue)
		{
            auto self = *this;
            return self.UIntMultOperationV1(rValue);
		}

        /// <summary>
        /// Multiplication operation between MediumDec variant and integer values
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        /// <returns>MediumDecVariant</returns>
        template<IntegerType IntType= signed int>
        MediumDecV2 MultiplyByIntV1(const IntType& rValue)
		{
            auto self = *this;
            return self.IntMultOperationV1(rValue);
		}

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

        MediumDecV2& UIntMultOperation(const unsigned int& rValue) { return UIntMultOperationV1(rValue); }
        MediumDecV2& IntMultOperation(const signed int& rValue) { return IntMultOperationV1(rValue); }
        MediumDecV2& UInt64MultOperation(const unsigned __int64& rValue) { return UIntMultOperationV1(rValue); }
        MediumDecV2& Int64MultOperation(const signed __int64& rValue) { return IntMultOperationV1(rValue); }
        MediumDecV2& UInt8MultOperation(const unsigned char& rValue) { return UIntMultOperationV1(rValue); }
        MediumDecV2& Int8MultOperation(const signed char& rValue) { return IntMultOperationV1(rValue); }
        MediumDecV2& UInt16MultOperation(const unsigned short& rValue) { return UIntMultOperationV1(rValue); }
        MediumDecV2& Int16MultOperation(const signed short& rValue) { return IntMultOperationV1(rValue); }

        const MediumDecV2 MultiplyByUInt(const unsigned int& rValue) { return MultiplyByUIntV1(rValue); }
        const MediumDecV2 MultiplyByInt(const signed int& rValue) { return MultiplyByIntV1(rValue); }
        const MediumDecV2 MultiplyByUInt64(const unsigned __int64& rValue) { return MultiplyByUIntV1(rValue); }
        const MediumDecV2 MultiplyByInt64(const signed __int64& rValue) { return MultiplyByIntV1(rValue); }

        const MediumDecV2 UnsignedMultiplyByInt(const signed int& rValue) { return MultiplyByUIntV1(rValue); }
        const MediumDecV2 UnsignedMultiplyByInt64(const signed __int64& rValue) { return MultiplyByUIntV1(rValue); }

        const MediumDecV2 MultiplyByUInt8(const unsigned char& rValue) { return MultiplyByUIntV1(rValue); }
        const MediumDecV2 MultiplyByInt8(const signed char& rValue) { return MultiplyByIntV1(rValue); }
        const MediumDecV2 MultiplyByUInt16(const unsigned short& rValue) { return MultiplyByUIntV1(rValue); }
        const MediumDecV2 MultiplyByInt16(const signed short& rValue) { return MultiplyByIntV1(rValue); }

		/// <summary>
        /// Unsigned Multiplication operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        void UnsignedMultOp(const MediumDecV2& rValue);

		/// <summary>
        /// Multiplication operation that ignores special decimal status
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        void MultOp(const MediumDecV2& Value)
        {
            if(Value.IsNegative())
            {
                SwapNegativeStatus();
                UnsignedMultOp(-Value);
            }
            else
                UnsignedMultOp(Value);
        }

        MediumDecV2& UnsignedMultOperation(const MediumDecV2& rValue)
        { UnsignedMultOp(rValue); return *this; }

        MediumDecV2& MultOperation(const MediumDecV2& rValue)
        { MultOp(rValue); return *this; }

		/// <summary>
        /// Unsigned Multiplication operation that ignores special decimal status
        /// Return true if divide into zero
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        MediumDecV2 MultiplyByUnsigned(const MediumDecV2& rValue)
        {
            auto self = *this;
            return self.UnsignedMultOperation(rValue);
        }

		/// <summary>
        /// Multiplication operation that ignores special decimal status
        /// Return true if divide into zero
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        MediumDecV2 MultiplyBy(const MediumDecV2& rValue)
        {
            auto self = *this;
            return self.MultOperation(rValue);
        }

        /// <summary>
        /// /= operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2& operator*=(MediumDecV2& lValue, const MediumDecV2& rValue) { return lValue.MultOperation(rValue); }

        /// <summary>
        /// *= operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2& operator*=(MediumDecV2& lValue, const signed int& rValue) { return lValue.IntMultOperation(rValue); }
        friend MediumDecV2& operator*=(MediumDecV2& lValue, const signed __int64& rValue) { return lValue.Int64MultOperation(rValue); }
        friend MediumDecV2& operator*=(MediumDecV2& lValue, const unsigned int& rValue) { return lValue.UIntMultOperation(rValue); }
        friend MediumDecV2& operator*=(MediumDecV2& lValue, const unsigned __int64& rValue) { return lValue.UInt64MultOperation(rValue); }

        friend MediumDecV2& operator*=(MediumDecV2& lValue, const signed char& rValue) { return lValue.Int8MultOperation(rValue); }
        friend MediumDecV2& operator*=(MediumDecV2& lValue, const signed short& rValue) { return lValue.Int16MultOperation(rValue); }
        friend MediumDecV2& operator*=(MediumDecV2& lValue, const unsigned char& rValue) { return lValue.UInt8MultOperation(rValue); }
        friend MediumDecV2& operator*=(MediumDecV2& lValue, const unsigned short& rValue) { return lValue.UInt16MultOperation(rValue); }

        /// <summary>
        /// Multiplication operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2 operator*(MediumDecV2 lValue, const MediumDecV2& rValue) { return lValue.MultiplyBy(rValue); }

        /// <summary>
        /// Multiplication operation between MediumDec variant and Integer rValue.
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2 operator*(MediumDecV2 lValue, const signed int& rValue) { return lValue.IntMultOperation(rValue); }
        friend MediumDecV2 operator*(MediumDecV2 lValue, const signed __int64& rValue) { return lValue.Int64MultOperation(rValue); }
        friend MediumDecV2 operator*(MediumDecV2 lValue, const unsigned int& rValue) { return lValue.UIntMultOperation(rValue); }
        friend MediumDecV2 operator*(MediumDecV2 lValue, const unsigned __int64& rValue) { return lValue.UInt64MultOperation(rValue); }

        friend MediumDecV2 operator*(MediumDecV2 lValue, const signed char& rValue) { return lValue.Int8MultOperation(rValue); }
        friend MediumDecV2 operator*(MediumDecV2 lValue, const signed short& rValue) { return lValue.Int16MultOperation(rValue); }
        friend MediumDecV2 operator*(MediumDecV2 lValue, const unsigned char& rValue) { return lValue.UInt8MultOperation(rValue); }
        friend MediumDecV2 operator*(MediumDecV2 lValue, const unsigned short& rValue) { return lValue.UInt16MultOperation(rValue); }

        friend MediumDecV2 operator*(const signed int& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).MultiplyBy(rValue); }
        friend MediumDecV2 operator*(const signed __int64& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).MultiplyBy(rValue); }
        friend MediumDecV2 operator*(const unsigned int& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).MultiplyBy(rValue); }
        friend MediumDecV2 operator*(const unsigned __int64& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).MultiplyBy(rValue); }

        friend MediumDecV2 operator*(const signed char& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).MultiplyBy(rValue); }
        friend MediumDecV2 operator*(const signed short& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).MultiplyBy(rValue); }
        friend MediumDecV2 operator*(const unsigned char& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).MultiplyBy(rValue); }
        friend MediumDecV2 operator*(const unsigned short& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).MultiplyBy(rValue); }

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
			MediumDec::UIntAddOpV1(rValue);
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
			MediumDec::IntAddOpV1(rValue);
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
        void BasicUIntAddition(const MirroredInt& rValue)
        { MediumDec::UnsignedMirroredAddOp(rValue); }

        /// <summary>
        /// Basic addition operation between MediumDec Variant and MirroredInt
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        void BasicIntegerAddition(const MirroredInt& rValue)
        { MediumDec::MirroredAddOp(rValue); }

        void BasicUIntAddOp(const unsigned int& rValue){ MediumDec::UIntAddOp(rValue); }

        void BasicIntAddOp(const signed int& rValue) { MediumDec::IntAddOpV1(rValue); }
        void BasicUInt64AddOp(const unsigned __int64& rValue) { MediumDec::UIntAddOpV1(rValue); }
        void BasicInt64AddOp(const signed __int64& rValue) { MediumDec::IntAddOpV1(rValue); }

        void BasicUnsignedIntegerAddOp(const signed int& rValue) { MediumDec::UIntAddOpV1(rValue); }

        void BasicUInt8AddOp(const unsigned char& rValue) { MediumDec::UIntAddOpV1(rValue); }
        void BasicInt8AddOp(const signed char& rValue) { MediumDec::IntAddOpV1(rValue); }
        void BasicUInt16AddOp(const unsigned short& rValue) { MediumDec::UIntAddOpV1(rValue); }
        void BasicInt16AddOp(const signed short& rValue) { MediumDec::IntAddOpV1(rValue); }

        MediumDecV2& BasicUIntAddOperation(const unsigned int& rValue){ BasicUIntAddOp(rValue); return *this; }

        MediumDecV2& BasicIntAddOperation(const signed int& rValue) { return BasicIntAddOperationV1(rValue); }
        MediumDecV2& BasicUInt64AddOperation(const unsigned __int64& rValue) { return BasicUIntAddOperationV1(rValue); }
        MediumDecV2& BasicInt64AddOperation(const signed __int64& rValue) { return BasicIntAddOperationV1(rValue); }

        MediumDecV2& BasicUInt8AddOperation(const unsigned char& rValue) { return BasicUIntAddOperationV1(rValue); }
        MediumDecV2& BasicInt8AddOperation(const signed char& rValue) { return BasicIntAddOperationV1(rValue); }
        MediumDecV2& BasicUInt16AddOperation(const unsigned short& rValue) { return BasicUIntAddOperationV1(rValue); }
        MediumDecV2& BasicInt16AddOperation(const signed short& rValue) { return BasicIntAddOperationV1(rValue); }
        MediumDecV2 BasicAddByUInt(const unsigned int& rValue) { return BasicAddByUIntV1(rValue); }
        MediumDecV2 BasicAddByInt(const signed int& rValue) { return BasicAddByIntV1(rValue); }
        MediumDecV2 BasicAddByUInt64(const unsigned __int64& rValue) { return BasicAddByUIntV1(rValue); }
        MediumDecV2 BasicAddByInt64(const signed __int64& rValue) { return BasicAddByIntV1(rValue); }

        MediumDecV2 BasicUnsignedAddByInt(const signed int& rValue) { return BasicAddByUIntV1(rValue); }
        MediumDecV2 BasicUnsignedAddByInt64(const signed __int64& rValue) { return BasicAddByUIntV1(rValue); }

        MediumDecV2 BasicAddByUInt8(const unsigned char& rValue) { return BasicAddByUIntV1(rValue); }
        MediumDecV2 BasicAddByInt8(const signed char rValue) { return BasicAddByIntV1(rValue); }
        MediumDecV2 BasicAddByUInt16(const unsigned short& rValue) { return BasicAddByUIntV1(rValue); }
        MediumDecV2 BasicAddByInt16(const signed short& rValue) { return BasicAddByIntV1(rValue); }

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
			MediumDec::UIntSubOpV1(rValue);
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
			MediumDec::IntSubOpV1(rValue);
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
        void BasicUIntSubtraction(const MirroredInt& rValue)
        { MediumDec::UnsignedMirroredSubOp(rValue); }

        /// <summary>
        /// Basic subtraction operation between MediumDec Variant and MirroredInt
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        void BasicIntegerSubtraction(const MirroredInt& rValue)
        { MediumDec::MirroredSubOp(rValue); }

        void BasicUIntSubOp(const unsigned int& rValue){ MediumDec::UIntSubOp(rValue); }

        void BasicIntSubOp(const signed int& rValue) { MediumDec::IntSubOpV1(rValue); }
        void BasicUInt64SubOp(const unsigned __int64& rValue) { MediumDec::UIntSubOpV1(rValue); }
        void BasicInt64SubOp(const signed __int64& rValue) { MediumDec::IntSubOpV1(rValue); }

        void BasicUnsignedIntegerSubOp(const signed int& rValue) { MediumDec::UIntSubOpV1(rValue); }

        void BasicUInt8SubOp(const unsigned char& rValue) { MediumDec::UIntSubOpV1(rValue); }
        void BasicInt8SubOp(const signed char& rValue) { MediumDec::IntSubOpV1(rValue); }
        void BasicUInt16SubOp(const unsigned short& rValue) { MediumDec::UIntSubOpV1(rValue); }
        void BasicInt16SubOp(const signed short& rValue) { MediumDec::IntSubOpV1(rValue); }

        MediumDecV2& BasicUIntSubOperation(const unsigned int& rValue){ BasicUIntSubOp(rValue); return *this; }

        MediumDecV2& BasicIntSubOperation(const signed int& rValue) { return BasicIntSubOperationV1(rValue); }
        MediumDecV2& BasicUInt64SubOperation(const unsigned __int64& rValue) { return BasicUIntSubOperationV1(rValue); }
        MediumDecV2& BasicInt64SubOperation(const signed __int64& rValue) { return BasicIntSubOperationV1(rValue); }

        MediumDecV2& BasicUInt8SubOperation(const unsigned char& rValue) { return BasicUIntSubOperationV1(rValue); }
        MediumDecV2& BasicInt8SubOperation(const signed char& rValue) { return BasicIntSubOperationV1(rValue); }
        MediumDecV2& BasicUInt16SubOperation(const unsigned short& rValue) { return BasicUIntSubOperationV1(rValue); }
        MediumDecV2& BasicInt16SubOperation(const signed short& rValue) { return BasicIntSubOperationV1(rValue); }
        MediumDecV2 BasicSubtractByUInt(const unsigned int& rValue) { return BasicSubtractByUIntV1(rValue); }
        MediumDecV2 BasicSubtractByInt(const signed int& rValue) { return BasicSubtractByIntV1(rValue); }
        MediumDecV2 BasicSubtractByUInt64(const unsigned __int64& rValue) { return BasicSubtractByUIntV1(rValue); }
        MediumDecV2 BasicSubtractByInt64(const signed __int64& rValue) { return BasicSubtractByIntV1(rValue); }

        MediumDecV2 BasicUnsignedSubtractByInt(const signed int& rValue) { return BasicSubtractByUIntV1(rValue); }
        MediumDecV2 BasicUnsignedSubtractByInt64(const signed __int64& rValue) { return BasicSubtractByUIntV1(rValue); }

        MediumDecV2 BasicSubtractByUInt8(const unsigned char& rValue) { return BasicSubtractByUIntV1(rValue); }
        MediumDecV2 BasicSubtractByInt8(const signed char rValue) { return BasicSubtractByIntV1(rValue); }
        MediumDecV2 BasicSubtractByUInt16(const unsigned short& rValue) { return BasicSubtractByUIntV1(rValue); }
        MediumDecV2 BasicSubtractByInt16(const signed short& rValue) { return BasicSubtractByIntV1(rValue); }

	#pragma endregion NormalRep Integer Subtraction Operations

    #pragma region Unsigned Addition/Subtraction Operations
protected:

        template<IntegerType IntType=unsigned int>
		void CatchAllUIntAddition(const IntType& rValue, const RepType& LRep)
		{
            ConvertToNormType(LRep);
			BasicUIntAddOp(rValue);
		}

        template<IntegerType IntType=unsigned int>
        void CatchAllUIntAdditionV2(const IntType& rValue)
        {
			ConvertToNormTypeV2();
			BasicUIntAddOp(rValue);
        }

        template<IntegerType IntType=unsigned int>
		void CatchAllUIntSubtraction(const IntType& rValue, const RepType& LRep)
		{
            ConvertToNormType(LRep);
			BasicUIntSubOp(rValue);
		}

        template<IntegerType IntType=unsigned int>
        void CatchAllUIntSubtractionV2(const IntType& rValue)
        {
			ConvertToNormTypeV2();
			BasicUIntSubOp(rValue);
        }

        /// <summary>
        /// Addition operation between MediumDec variant and unsigned integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        /// <returns>MediumDecVariant&</returns>
        template<IntegerType IntType= unsigned int>
        void UIntAddOpV1(const IntType& rValue)
		{
            if (rValue == 0)
                return;
        	switch(DecimalHalf.Flags)
        	{
        #if defined(AltNum_EnablePiRep)
        		case 1:{
                    RepType LRep = GetPiRepType();
                    CatchAllUIntAddition(rValue, LRep);
                } break;
        #endif
        #if defined(AltNum_EnableERep)
        		case 2:{
                    RepType LRep = GetERepType();
                    CatchAllUIntAddition(rValue, LRep);
                } break;
        #endif
        #if defined(AltNum_EnableIRep)//IRep_to_integer
        		case 3:
                    throw "Can't convert into complex number at moment";
                break;
        #endif
        		default:{
                    RepType LRep = GetNormRepType();
                    switch(LRep)
                    {
                        case RepTypeEnum::NormalType:
                            BasicUIntAddOp(rValue);
                        break;
    #pragma region AltDecVariantExclusive
    #pragma endregion AltDecVariantExclusive
            #if defined(AltNum_EnableApproaching)
                        case RepTypeEnum::ApproachingBottom:
                        case RepTypeEnum::ApproachingTop:
    #pragma region AltDecVariantExclusive
    #pragma endregion AltDecVariantExclusive
        					if(IsNegative())
        					{
                                if(rValue>IntHalf.Value)
								{
                                    IntHalf.Sign = 1;
                                    IntHalf.Value = rValue - IntHalf.Value;
								}
								else
									IntHalf.Value -= rValue;
                            }
                            else
                                IntHalf.Value += rValue;
                        break;
            #endif
            #ifdef AltNum_EnableInfinity
                        case RepTypeEnum::Infinity:
                            break;
            #endif
                        default:
                            throw "Unable to perform integer division on current representation.";
                    }
                } break;
        	}
		}

        /// <summary>
        /// Subtraction operation between MediumDec variant and unsigned integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        /// <returns>MediumDecVariant&</returns>
        template<IntegerType IntType= unsigned int>
        void UIntSubOpV1(const IntType& rValue)
		{
            if (rValue == 0)
                return;
        	switch(DecimalHalf.Flags)
        	{
        #if defined(AltNum_EnablePiRep)
        		case 1:{
                    RepType LRep = GetPiRepType();
                    CatchAllUIntSubtraction(rValue, LRep);
                } break;
        #endif
        #if defined(AltNum_EnableERep)
        		case 2:{
                    RepType LRep = GetERepType();
                    CatchAllUIntSubtraction(rValue, LRep);
                } break;
        #endif
        #if defined(AltNum_EnableIRep)//IRep_to_integer
        		case 3:
                    throw "Can't convert into complex number at moment";
                break;
        #endif
        		default:{
                    RepType LRep = GetNormRepType();
                    switch(LRep)
                    {
                        case RepTypeEnum::NormalType:
                            BasicUIntSubOpV1(rValue);
                        break;
    #pragma region AltDecVariantExclusive
    #pragma endregion AltDecVariantExclusive
            #if defined(AltNum_EnableApproaching)
                        case RepTypeEnum::ApproachingBottom:
                        case RepTypeEnum::ApproachingTop:
    #pragma region AltDecVariantExclusive
    #pragma endregion AltDecVariantExclusive
        					if(IsPositive())
        					{
                                if(rValue>IntHalf.Value)
								{
                                    IntHalf.Sign = 0;
                                    IntHalf.Value = rValue - IntHalf.Value;
								}
								else
									IntHalf.Value -= rValue;
                            }
                            else
                                IntHalf.Value += rValue;
                            break;
            #endif
            #ifdef AltNum_EnableInfinity
                        case RepTypeEnum::Infinity:
                            break;
            #endif
                        default:
                            throw "Unable to perform integer subtraction on current representation.";
                    }
                } break;
        	}
		}

        template<IntegerType IntType=unsigned int>
        auto& UIntAddOperationV1(const IntType& rValue)
        { UIntAddOpV1(rValue); return *this; }

        template<IntegerType IntType=unsigned int>
        auto& UIntSubOperationV1(const IntType& rValue)
        { UIntSubOpV1(rValue); return *this; }

        /// <summary>
        /// Addition operation between MediumDec variant and unsigned Integer values
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        /// <returns>MediumDecVariant</returns>
        template<IntegerType IntType= unsigned int>
        auto AddByUIntV1(const IntType& rValue){
            auto self = *this;
            return self.UIntAddOperationV1(rValue);
		}

        /// <summary>
        /// Subtraction operation between MediumDec variant and unsigned Integer values
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        /// <returns>MediumDecVariant</returns>
        template<IntegerType IntType= unsigned int>
        auto SubtractByUIntV1(const IntType& rValue){
            auto self = *this;
            return self.UIntSubOperationV1(rValue);
		}

public:

/*
        /// <summary>
        /// Basic addition operation between MediumDec Variant and unsigned MirroredInt
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        void UnsignedMirroredAddOp(const MirroredInt& rValue);
*/
        void UInt8AddOp(const unsigned char& rValue) { UIntAddOpV1(rValue); }
        void UInt16AddOp(const unsigned short& rValue) { UIntAddOpV1(rValue); }
        void UIntAddOp(const unsigned int& rValue){ UIntAddOpV1(rValue); }
        void UInt64AddOp(const unsigned __int64& rValue) { UIntAddOpV1(rValue); }

        void UnsignedIntegerAddOp(const signed int& rValue) { UIntAddOpV1(rValue); }

        MediumDecV2& UInt8AddOperation(const unsigned char& rValue) { return UIntAddOperationV1(rValue); }
        MediumDecV2& UInt16AddOperation(const unsigned short& rValue) { return UIntAddOperationV1(rValue); }
        MediumDecV2& UIntAddOperation(const unsigned int& rValue){ return UIntAddOperationV1(rValue); }
        MediumDecV2& UInt64AddOperation(const unsigned __int64& rValue) { return UIntAddOperationV1(rValue); }

        MediumDecV2 UnsignedAddByInt(const signed int& rValue) { return AddByUIntV1(rValue); }
        MediumDecV2 UnsignedAddByInt64(const signed __int64& rValue) { return AddByUIntV1(rValue); }

        MediumDecV2 AddByUInt8(const unsigned char& rValue) { return AddByUIntV1(rValue); }
        MediumDecV2 AddByUInt16(const unsigned short& rValue) { return AddByUIntV1(rValue); }
        MediumDecV2 AddByUInt(const unsigned int& rValue) { return AddByUIntV1(rValue); }
        MediumDecV2 AddByUInt64(const unsigned __int64& rValue) { return AddByUIntV1(rValue); }

/*
        /// <summary>
        /// Subtraction operation between MediumDec Variant and unsigned MirroredInt
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        void UnsignedMirroredSubOp(const MirroredInt& rValue);
*/

        void UInt8SubOp(const unsigned char& rValue) { UIntSubOpV1(rValue); }
        void UIntSubOp(const unsigned int& rValue){ UIntSubOpV1(rValue); }
        void UInt16SubOp(const unsigned short& rValue) { UIntSubOpV1(rValue); }
        void UInt64SubOp(const unsigned __int64& rValue) { UIntSubOpV1(rValue); }

        void UnsignedIntegerSubOp(const signed int& rValue) { UIntSubOpV1(rValue); }

        MediumDecV2& UInt8SubOperation(const unsigned char& rValue) { return UIntSubOperationV1(rValue); }
        MediumDecV2& UInt16SubOperation(const unsigned short& rValue) { return UIntSubOperationV1(rValue); }
        MediumDecV2& UIntSubOperation(const unsigned int& rValue){ return UIntSubOperationV1(rValue); }
        MediumDecV2& UInt64SubOperation(const unsigned __int64& rValue) { return UIntSubOperationV1(rValue); }

        MediumDecV2 UnsignedSubtractByInt(const signed int& rValue) { return SubtractByUIntV1(rValue); }
        MediumDecV2 UnsignedSubtractByInt64(const signed __int64& rValue) { return SubtractByUIntV1(rValue); }

        MediumDecV2 SubtractByUInt8(const unsigned char& rValue) { return SubtractByUIntV1(rValue); }
        MediumDecV2 SubtractByUInt16(const unsigned short& rValue) { return SubtractByUIntV1(rValue); }
        MediumDecV2 SubtractByUInt(const unsigned int& rValue) { return SubtractByUIntV1(rValue); }
        MediumDecV2 SubtractByUInt64(const unsigned __int64& rValue) { return SubtractByUIntV1(rValue); }

        friend MediumDecV2& operator+=(MediumDecV2& lValue, const unsigned char& rValue) { return lValue.UInt8AddOperation(rValue); }
        friend MediumDecV2& operator+=(MediumDecV2& lValue, const unsigned short& rValue) { return lValue.UInt16AddOperation(rValue); }
        friend MediumDecV2& operator+=(MediumDecV2& lValue, const unsigned int& rValue) { return lValue.UIntAddOperation(rValue); }
        friend MediumDecV2& operator+=(MediumDecV2& lValue, const unsigned __int64& rValue) { return lValue.UInt64AddOperation(rValue); }

        friend MediumDecV2 operator+(MediumDecV2 lValue, const unsigned char& rValue) { return lValue.UInt8AddOperation(rValue); }
        friend MediumDecV2 operator+(MediumDecV2 lValue, const unsigned short& rValue) { return lValue.UInt16AddOperation(rValue); }
        friend MediumDecV2 operator+(MediumDecV2 lValue, const unsigned int& rValue) { return lValue.UIntAddOperation(rValue); }
        friend MediumDecV2 operator+(MediumDecV2 lValue, const unsigned __int64& rValue) { return lValue.UInt64AddOperation(rValue); }

        friend MediumDecV2 operator+(const unsigned char& lValue, MediumDecV2 rValue)  { return rValue.UInt8AddOperation(lValue); }
        friend MediumDecV2 operator+(const unsigned short& lValue, MediumDecV2 rValue)  { return rValue.UInt16AddOperation(lValue); }
        friend MediumDecV2 operator+(const unsigned int& lValue, MediumDecV2 rValue)  { return rValue.UIntAddOperation(lValue); }
        friend MediumDecV2 operator+(const unsigned __int64& lValue, MediumDecV2 rValue)  { return rValue.UInt64AddOperation(lValue); }

        friend MediumDecV2& operator-=(MediumDecV2& lValue, const unsigned char& rValue) { return lValue.UInt8SubOperation(rValue); }
        friend MediumDecV2& operator-=(MediumDecV2& lValue, const unsigned short& rValue) { return lValue.UInt16SubOperation(rValue); }
        friend MediumDecV2& operator-=(MediumDecV2& lValue, const unsigned int& rValue) { return lValue.UIntSubOperation(rValue); }
        friend MediumDecV2& operator-=(MediumDecV2& lValue, const unsigned __int64& rValue) { return lValue.UInt64SubOperation(rValue); }

        friend MediumDecV2 operator-(MediumDecV2 lValue, const unsigned char& rValue) { return lValue.UInt8SubOperation(rValue); }
        friend MediumDecV2 operator-(MediumDecV2 lValue, const unsigned short& rValue) { return lValue.UInt16SubOperation(rValue); }
        friend MediumDecV2 operator-(MediumDecV2 lValue, const unsigned int& rValue) { return lValue.UIntSubOperation(rValue); }
        friend MediumDecV2 operator-(MediumDecV2 lValue, const unsigned __int64& rValue) { return lValue.UInt64SubOperation(rValue); }

    #pragma endregion Unsigned integer addition/subtraction

	#pragma region Signed integer addition/subtraction
protected:

        /// <summary>
        /// Addition operation between MediumDec variant and Integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        /// <returns>MediumDecVariant&</returns>
        template<IntegerType IntType= signed int>
        void IntAddOpV1(const IntType& rValue)
		{
            if(rValue<0)
                UIntSubOpV1(-rValue);
            else
                UIntAddOpV1(rValue);
		}

        /// <summary>
        /// Subtraction operation between MediumDec variant and Integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        /// <returns>MediumDecVariant&</returns>
        template<IntegerType IntType= signed int>
        void IntSubOpV1(const IntType& rValue)
		{
            if(rValue<0)
                UIntAddOpV1(-rValue);
            else
                UIntSubOpV1(rValue);
		}

        template<IntegerType IntType=signed int>
        auto& IntAddOperationV1(const IntType& rValue)
        { IntAddOpV1(rValue); return *this; }

        template<IntegerType IntType=signed int>
        auto& IntSubOperationV1(const IntType& rValue)
        { IntSubOpV1(rValue); return *this; }

        /// <summary>
        /// Addition operation between MediumDec variant and Integer values
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        /// <returns>MediumDecVariant</returns>
        template<IntegerType IntType= signed int>
        auto AddByIntV1(const IntType& rValue){
            auto self = *this;
            return self.IntAddOperationV1(rValue);
		}

        /// <summary>
        /// Subtraction operation between MediumDec variant and Integer values
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        /// <returns>MediumDecVariant</returns>
        template<IntegerType IntType= signed int>
        auto SubtractByIntV1(const IntType& rValue){
            auto self = *this;
            return self.IntSubOperationV1(rValue);
		}

public:

/*
        /// <summary>
        /// Addition operation between MediumDec Variant and MirroredInt
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        void MirroredAddOp(const MirroredInt& rValue);
*/

        void Int8AddOp(const signed char& rValue) { IntAddOpV1(rValue); }
        void Int16AddOp(const signed short& rValue) { IntAddOpV1(rValue); }
        void IntAddOp(const signed int& rValue) { IntAddOpV1(rValue); }
        void Int64AddOp(const signed __int64& rValue) { IntAddOpV1(rValue); }

        MediumDecV2& Int8AddOperation(const signed char& rValue) { return IntAddOperationV1(rValue); }
        MediumDecV2& Int16AddOperation(const signed short& rValue) { return IntAddOperationV1(rValue); }
        MediumDecV2& IntAddOperation(const signed int& rValue) { return IntAddOperationV1(rValue); }
        MediumDecV2& Int64AddOperation(const signed __int64& rValue) { return IntAddOperationV1(rValue); }

        MediumDecV2 AddByInt8(const signed char rValue) { return AddByIntV1(rValue); }
        MediumDecV2 AddByInt16(const signed short& rValue) { return AddByIntV1(rValue); }
        MediumDecV2 AddByInt(const signed int& rValue) { return AddByIntV1(rValue); }
        MediumDecV2 AddByInt64(const signed __int64& rValue) { return AddByIntV1(rValue); }

/*
        /// <summary>
        /// Subtraction operation between MediumDec Variant and MirroredInt
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        void MirroredSubOp(const MirroredInt& rValue);
*/

        void Int8SubOp(const signed char& rValue) { IntSubOpV1(rValue); }
        void IntSubOp(const signed int& rValue) { IntSubOpV1(rValue); }
        void Int16SubOp(const signed short& rValue) { IntSubOpV1(rValue); }
        void Int64SubOp(const signed __int64& rValue) { IntSubOpV1(rValue); }

        MediumDecV2& Int8SubOperation(const signed char& rValue) { return IntSubOperationV1(rValue); }
        MediumDecV2& Int16SubOperation(const signed short& rValue) { return IntSubOperationV1(rValue); }
        MediumDecV2& IntSubOperation(const signed int& rValue) { return IntSubOperationV1(rValue); }
        MediumDecV2& Int64SubOperation(const signed __int64& rValue) { return IntSubOperationV1(rValue); }

        MediumDecV2 SubtractByInt8(const signed char rValue) { return SubtractByIntV1(rValue); }
        MediumDecV2 SubtractByInt16(const signed short& rValue) { return SubtractByIntV1(rValue); }
        MediumDecV2 SubtractByInt(const signed int& rValue) { return SubtractByIntV1(rValue); }
        MediumDecV2 SubtractByInt64(const signed __int64& rValue) { return SubtractByIntV1(rValue); }

        friend MediumDecV2& operator+=(MediumDecV2& lValue, const signed char& rValue) { return lValue.Int8AddOperation(rValue); }
        friend MediumDecV2& operator+=(MediumDecV2& lValue, const signed short& rValue) { return lValue.Int16AddOperation(rValue); }
        friend MediumDecV2& operator+=(MediumDecV2& lValue, const signed int& rValue) { return lValue.IntAddOperation(rValue); }
        friend MediumDecV2& operator+=(MediumDecV2& lValue, const signed __int64& rValue) { return lValue.Int64AddOperation(rValue); }

        friend MediumDecV2 operator+(MediumDecV2 lValue, const signed char& rValue) { return lValue.Int8AddOperation(rValue); }
        friend MediumDecV2 operator+(MediumDecV2 lValue, const signed short& rValue) { return lValue.Int16AddOperation(rValue); }
        friend MediumDecV2 operator+(MediumDecV2 lValue, const signed int& rValue) { return lValue.IntAddOperation(rValue); }
        friend MediumDecV2 operator+(MediumDecV2 lValue, const signed __int64& rValue) { return lValue.Int64AddOperation(rValue); }

        friend MediumDecV2 operator+(const signed int& lValue, MediumDecV2 rValue)  { return rValue.IntAddOperation(lValue); }
        friend MediumDecV2 operator+(const signed __int64& lValue, MediumDecV2 rValue)  { return rValue.Int64AddOperation(lValue); }
        friend MediumDecV2 operator+(const signed char& lValue, MediumDecV2 rValue)  { return rValue.Int8AddOperation(lValue); }
        friend MediumDecV2 operator+(const signed short& lValue, MediumDecV2 rValue)  { return rValue.Int16AddOperation(lValue); }

        friend MediumDecV2& operator-=(MediumDecV2& lValue, const signed char& rValue) { return lValue.Int8SubOperation(rValue); }
        friend MediumDecV2& operator-=(MediumDecV2& lValue, const signed short& rValue) { return lValue.Int16SubOperation(rValue); }
        friend MediumDecV2& operator-=(MediumDecV2& lValue, const signed int& rValue) { return lValue.IntSubOperation(rValue); }
        friend MediumDecV2& operator-=(MediumDecV2& lValue, const signed __int64& rValue) { return lValue.Int64SubOperation(rValue); }


        friend MediumDecV2 operator-(MediumDecV2 lValue, const signed char& rValue) { return lValue.Int8SubOperation(rValue); }
        friend MediumDecV2 operator-(MediumDecV2 lValue, const signed short& rValue) { return lValue.Int16SubOperation(rValue); }
        friend MediumDecV2 operator-(MediumDecV2 lValue, const signed int& rValue) { return lValue.IntSubOperation(rValue); }
        friend MediumDecV2 operator-(MediumDecV2 lValue, const signed __int64& rValue) { return lValue.Int64SubOperation(rValue); }

	#pragma endregion Signed integer addition/subtraction

	#pragma region Basic_addition

		/// <summary>
        /// Basic addition operation that ignores special decimal status with unsigned MediumDec
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        void BasicUnsignedAddOp(const MediumDecV2& rValue);

        void BasicAddOp(const MediumDecV2& rValue);

		/// <summary>
        /// Basic unsigned addition operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        MediumDecV2& BasicUnsignedAddOperation(const MediumDecV2& rValue)
		{ BasicUnsignedAddOp(rValue); return *this; }

		/// <summary>
        /// Basic addition operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        MediumDecV2& BasicAddOperation(const MediumDecV2& rValue)
		{ BasicAddOp(rValue); return *this; }

		/// <summary>
        /// Basic unsigned addition operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        const MediumDecV2 BasicAddByUnsigned(const MediumDecV2& rValue)
        { MediumDecV2 lValue = *this; return lValue.UnsignedAddOperation(rValue); }

		/// <summary>
        /// Basic addition operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        const MediumDecV2 BasicAddBy(const MediumDecV2& rValue)
        { MediumDecV2 lValue = *this; return lValue.AddOperation(rValue); }

	#pragma endregion Basic_addition

	#pragma region Basic_subtraction

		/// <summary>
        /// Basic subtraction operation that ignores special decimal status with unsigned MediumDec
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        void BasicUnsignedSubOp(const MediumDecV2& rValue){ MediumDec::UnsignedSubOpV1<MediumDecV2>(rValue); }

        void BasicSubOp(const MediumDecV2& rValue){ MediumDec::SubOpV1<MediumDecV2>(rValue); }

		/// <summary>
        /// Basic unsigned subtraction operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        MediumDecV2& BasicUnsignedSubOperation(const MediumDecV2& rValue)
		{ BasicUnsignedSubOp(rValue); return *this; }

		/// <summary>
        /// Basic subtraction operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        MediumDecV2& BasicSubOperation(const MediumDecV2& rValue)
		{ BasicSubOp(rValue); return *this; }

		/// <summary>
        /// Basic unsigned subtraction operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        const MediumDecV2 BasicSubtractByUnsigned(const MediumDecV2& rValue)
        { MediumDecV2 lValue = *this; return lValue.UnsignedSubOperation(rValue); }

		/// <summary>
        /// Basic subtraction operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        const MediumDecV2 BasicSubtractBy(const MediumDecV2& rValue)
        { MediumDecV2 lValue = *this; return lValue.SubOperation(rValue); }

	#pragma endregion Basic_subtraction

	#pragma region Other_addition&subtraction

        template<MediumDecVariant VariantType=MediumDecV2>
		void CatchAllUnsignedAddition(const VariantType& rValue, const RepType& LRep, const RepType& RRep)
		{
			ConvertToNormType(LRep);
			auto convertedRVal = rValue.ConvertAsNormType(RRep);
			BasicUnsignedAddOp(convertedRVal);
		}

        template<MediumDecVariant VariantType=MediumDecV2>
		void CatchAllUnsignedSubtraction(const VariantType& rValue, const RepType& LRep, const RepType& RRep)
		{
			ConvertToNormType(LRep);
			auto convertedRVal = rValue.ConvertAsNormType(RRep);
			BasicUnsignedSubOp(convertedRVal);
		}

private:

		void AddOp_SameRep_ApproachingBottom(const MediumDecV2& rValue, const RepType& LRep);

		void AddOp_SameRep_ApproachingTop(const MediumDecV2& rValue, const RepType& LRep);

		void SubOp_SameRep_ApproachingBottom(const MediumDecV2& rValue, const RepType& LRep);

		void SubOp_SameRep_ApproachingTop(const MediumDecV2& rValue, const RepType& LRep);

public:

        /// <summary>
        /// Basic addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void UnsignedAddOp(const MediumDecV2& rValue);

	    /// <summary>
        /// Basic subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void UnsignedSubOp(const MediumDecV2& rValue);

        //Basic addition operation
        MediumDecV2& UnsignedAddOperation(const MediumDecV2& rValue)
        { MediumDec::UnsignedAddOp(rValue); return *this; }

		/// <summary>
        /// Unsigned Addition operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        MediumDecV2 AddByUnsigned(const MediumDecV2& rValue)
        { MediumDecV2 lValue = *this; return lValue.UnsignedAddOperation(rValue); } const

        //Basic subtraction operation
        MediumDecV2& UnsignedSubOperation(const MediumDecV2& rValue)
        { MediumDec::UnsignedSubOp(rValue); return *this; }

		/// <summary>
        /// Basic unsigned Subtraction operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        MediumDecV2 SubtractByUnsigned(const MediumDecV2& rValue)
        { MediumDecV2 lValue = *this; return lValue.UnsignedSubOperation(rValue); } const

        /// <summary>
        /// Basic addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void AddOp(const MediumDecV2& rValue){
            if(rValue.IsNegative())
                UnsignedSubOp(-rValue);
            else
                UnsignedAddOp(rValue);
        }

	    /// <summary>
        /// Basic subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void SubOp(const MediumDecV2& rValue){
            if(rValue.IsNegative())
                UnsignedAddOp(-rValue);
            else
                UnsignedSubOp(rValue);
        }

        //Basic addition operation
        MediumDecV2& AddOperation(const MediumDecV2& rValue)
        { AddOp(rValue); return *this; }

		/// <summary>
        /// Unsigned Addition operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        MediumDecV2 AddBy(const MediumDecV2& rValue) const
        { MediumDecV2 lValue = *this; return lValue.AddOperation(rValue); }

        //Basic subtraction operation
        MediumDecV2& SubOperation(const MediumDecV2& rValue)
        { SubOp(rValue); return *this; }

		/// <summary>
        /// Basic unsigned Subtraction operation that ignores special decimal status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        MediumDecV2 SubtractBy(const MediumDecV2& rValue) const
        { MediumDecV2 lValue = *this; return lValue.SubOperation(rValue); }

        friend MediumDecV2 operator-(const unsigned char& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).SubtractBy(rValue); }
        friend MediumDecV2 operator-(const unsigned short& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).SubtractBy(rValue); }
        friend MediumDecV2 operator-(const unsigned int& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).SubtractBy(rValue); }
        friend MediumDecV2 operator-(const unsigned __int64& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).SubtractBy(rValue); }
        friend MediumDecV2 operator-(const signed char& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).SubtractBy(rValue); }
        friend MediumDecV2 operator-(const signed short& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).SubtractBy(rValue); }
        friend MediumDecV2 operator-(const signed int& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).SubtractBy(rValue); }
        friend MediumDecV2 operator-(const signed __int64& lValue, const MediumDecV2& rValue) { return ((MediumDecV2)lValue).SubtractBy(rValue); }

        /// <summary>
        /// += operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2& operator+=(MediumDecV2& lValue, const MediumDecV2& rValue) { return lValue.AddOperation(rValue); }

        /// <summary>
        /// -= operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2& operator-=(MediumDecV2& lValue, const MediumDecV2& rValue) { return lValue.SubOperation(rValue); }

        /// <summary>
        /// Addition operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2 operator+(MediumDecV2 lValue, const MediumDecV2& rValue) { return lValue.AddOperation(rValue); }

        /// <summary>
        /// Subtraction operation
        /// </summary>
        /// <param name="lValue">The left side value</param>
        /// <param name="rValue">The right side value.</param>
        /// <returns>MediumDecV2</returns>
        friend MediumDecV2 operator-(MediumDecV2 lValue, const MediumDecV2& rValue) { return lValue.SubOperation(rValue); }

	#pragma endregion Other_addition&subtraction

	#pragma region Modulus Operations

        void UnsignedModulusOp(const MediumDecV2& rValue){ UnsignedModulusOpV1<MediumDecV2>(rValue); }

        void ModulusOp(const MediumDecV2& rValue){ ModulusOpV1<MediumDecV2>(rValue); }

		MediumDecV2& UnsignedModulusOperation(const MediumDecV2& rValue){
			UnsignedModulusOp(rValue); return *this;
		}

		MediumDecV2& ModulusOperation(const MediumDecV2& rValue){
			ModulusOp(rValue); return *this;
		}

        friend MediumDecV2& operator%=(MediumDecV2& lValue, const MediumDecV2& rValue) { return lValue.ModulusOperation(rValue); }
        friend MediumDecV2& operator%=(MediumDecV2& lValue, const signed int& rValue) { lValue.IntModulusOp(rValue); return lValue; }
        friend MediumDecV2& operator%=(MediumDecV2& lValue, const unsigned int& rValue) { lValue.UIntModulusOp(rValue); return lValue; }
        friend MediumDecV2& operator%=(MediumDecV2& lValue, const unsigned __int64& rValue) { lValue.UInt64ModulusOp(rValue); return lValue;}
        friend MediumDecV2& operator%=(MediumDecV2& lValue, const signed __int64& rValue) { lValue.Int64ModulusOp(rValue); return lValue;}

	#pragma endregion Modulus Operations

	#pragma region Bitwise Operations
	#pragma endregion Bitwise Operations

    /*
    #pragma region Floating Operator Overrides

        friend MediumDecV2 operator+(const MediumDecV2& self, const float& tValue) { return self + (MediumDecV2)tValue; }
        friend MediumDecV2 operator-(const MediumDecV2& self, const float& tValue) { return self - (MediumDecV2)tValue; }
        friend MediumDecV2 operator*(const MediumDecV2& self, const float& tValue) { return self * (MediumDecV2)tValue; }
        friend MediumDecV2 operator/(const MediumDecV2& self, const float& tValue) { return self / (MediumDecV2)tValue; }

        friend MediumDecV2 operator+(const float& tValue, const MediumDecV2& self) { return (MediumDecV2)tValue + self; }
        friend MediumDecV2 operator-(const float& tValue, const MediumDecV2& self) { return (MediumDecV2)tValue - self; }
        friend MediumDecV2 operator*(const float& tValue, const MediumDecV2& self) { return (MediumDecV2)tValue * self; }
        friend MediumDecV2 operator/(const float& tValue, const MediumDecV2& self) { return (MediumDecV2)tValue / self; }

        friend MediumDecV2 operator+(const MediumDecV2& self, const double& tValue) { return self + (MediumDecV2)tValue; }
        friend MediumDecV2 operator-(const MediumDecV2& self, const double& tValue) { return self - (MediumDecV2)tValue; }
        friend MediumDecV2 operator*(const MediumDecV2& self, const double& tValue) { return self * (MediumDecV2)tValue; }
        friend MediumDecV2 operator/(const MediumDecV2& self, const double& tValue) { return self / (MediumDecV2)tValue; }

        friend MediumDecV2 operator+(const MediumDecV2& self, const long double& tValue) { return self + (MediumDecV2)tValue; }
        friend MediumDecV2 operator-(const MediumDecV2& self, const long double& tValue) { return self - (MediumDecV2)tValue; }
        friend MediumDecV2 operator*(const MediumDecV2& self, const long double& tValue) { return self * (MediumDecV2)tValue; }
        friend MediumDecV2 operator/(const MediumDecV2& self, const long double& tValue) { return self / (MediumDecV2)tValue; }

        friend MediumDecV2 operator+(const long double& tValue, const MediumDecV2& self) { return (MediumDecV2)tValue + self; }
        friend MediumDecV2 operator-(const long double& tValue, const MediumDecV2& self) { return (MediumDecV2)tValue - self; }
        friend MediumDecV2 operator*(const long double& tValue, const MediumDecV2& self) { return (MediumDecV2)tValue * self; }
        friend MediumDecV2 operator/(const long double& tValue, const MediumDecV2& self) { return (MediumDecV2)tValue / self; }

    #pragma endregion Floating Operator Overrides
    */

    #pragma region Other Operators

        /// <summary>
        /// ++MediumDecV2 Operator
        /// </summary>
        /// <returns>MediumDecV2 &</returns>
        MediumDecV2& operator ++()
        {
            if (DecimalHalf == 0)
                ++IntHalf;
            else if (IntHalf == MirroredInt::NegativeZero)
                IntHalf = MirroredInt::Zero;
            else
                ++IntHalf;
            return *this;
        }

        /// <summary>
        /// ++MediumDecV2 Operator
        /// </summary>
        /// <returns>MediumDecV2 &</returns>
        MediumDecV2& operator --()
        {
            if (DecimalHalf == 0)
                --IntHalf;
            else if (IntHalf == MirroredInt::Zero)
                IntHalf = MirroredInt::NegativeZero;
            else
                --IntHalf;
            return *this;
        }

        /// <summary>
        /// MediumDec Variant++ Operator
        /// </summary>
        /// <returns>MediumDecV2</returns>
        MediumDecV2 operator ++(int)
        {
            MediumDecV2 tmp(*this);
            ++* this;
            return tmp;
        }

        /// <summary>
        /// MediumDec Variant-- Operator
        /// </summary>
        /// <returns>MediumDecV2</returns>
        MediumDecV2 operator --(int)
        {
            MediumDecV2 tmp(*this);
            --* this;
            return tmp;
        }

        /// <summary>
        /// MediumDec Variant* Operator
        /// </summary>
        /// <returns>MediumDecV2 &</returns>
        MediumDecV2& operator *()
        {
            return *this;
        }

    #pragma endregion Other Operators

	#pragma region Truncation Functions

        /// <summary>
        /// Forces Number into non-negative
        /// </summary>
        static MediumDecV2 Abs(const MediumDecV2& tValue);

        /// <summary>
        /// Returns a copy of the number converted into non-negative version
        /// </summary>
        MediumDecV2 AbsOf() const;

		static MediumDecV2 Ceil(const MediumDecV2& tValue);

        MediumDecV2 CeilOf() const;

        /// <summary>
        /// Returns the largest integer that is smaller than or equal to tValue (Rounds downs to integer value).
        /// </summary>
        /// <returns>MediumDecV2&</returns>
		static signed int FloorInt(const MediumDecV2& tValue);

        /// <summary>
        /// Returns the smallest integer that is greater than or equal to tValue (Rounds up to integer value).
        /// </summary>
        /// <returns>MediumDecV2&</returns>
		static signed int CeilInt(const MediumDecV2& tValue);

		static MediumDecV2 Trunc(const MediumDecV2& tValue);

	#pragma endregion Truncation Functions

	#pragma region Pow and Sqrt Functions
protected:

        /// <summary>
        /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
        /// </summary>
        template<MediumDecVariant VariantType = MediumDecV2>
        static VariantType SqrtV1(VariantType value,const unsigned int& precision=7)
        {
    //Sqrt(Infinity) = Infinity but not bothering to check for infinity
    #if !defined(AltNum_EnableIRep)
            if(value.IsNegative())
                throw "Can't display result of negative square root without imaginary number support";
    #else
            if(value.DecimalHalf.Flags==3)
                throw "Square root of imaginary numbers not supported yet.";//Requires complex number support for some results
            if(value.IsNegative()){
                value = MediumDec::UnsignedSqrt<VariantType>(-value, precision);
                value.DecimalHalf.Flags = 3;
                return value;
            }
    #endif
            return MediumDec::UnsignedSqrtV1<VariantType>(value, precision);
        }

public:

        /// <summary>
        /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
        /// </summary>
        static MediumDecV2 Sqrt(const auto& value, const int& precision = 7);

        /// <summary>
        /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
        /// </summary>
        MediumDecV2 SqrtOf(const int& precision = 7) const;

        /// <summary>
        /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
        /// </summary>
        static MediumDecV2 UnsignedSqrt(const auto& value, const int& precision = 7)
        {
            return UnsignedSqrtV1<MediumDecV2>(value, precision);
        }

        /// <summary>
        /// Perform square root on this instance.(Code other than switch statement from https://www.geeksforgeeks.org/find-square-root-number-upto-given-precision-using-binary-search/)
        /// </summary>
        MediumDecV2 UnsignedSqrtOf(const int& precision = 7) {
            return UnsignedSqrt(*this, precision);
        }

        /// <summary>
        /// Applies Power of operation (for unsigned integer exponents)
        /// </summary>
        /// <param name="tValue">The target value to perform power of operation.</param>
        /// <param name="expValue">The exponent value.</param>
        static MediumDecV2 UIntPow(const MediumDecV2& tValue, const unsigned int& expValue)
        { return IntPowV1<MediumDecV2>(tValue, expValue); }
        static MediumDecV2 UInt64Pow(const MediumDecV2& tValue, const unsigned __int64& expValue)
        { return UIntPowV1<MediumDecV2>(tValue, expValue); }

        /// <summary>
        /// Applies Power of operation (for signed integer exponents)
        /// </summary>
        /// <param name="tValue">The target value to perform power of operation.</param>
        /// <param name="expValue">The exponent value.</param>
        static MediumDecV2 IntPow(const MediumDecV2& tValue, const signed int& expValue)
		{ return IntPowV1<MediumDecV2>(tValue, expValue); }
        static MediumDecV2 Int64Pow(const MediumDecV2& tValue, const signed __int64& expValue)
		{ return IntPowV1<MediumDecV2>(tValue, expValue); }

        MediumDecV2 UnsignedNegIntPower(const MediumDecV2& tValue, const unsigned int& expValue)
		{ return UnsignedNegIntPowerV1<MediumDecV2>(tValue, expValue); }


        /// <summary>
        /// Applies Power of operation (for unsigned integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        MediumDecV2 UIntPowOf(const unsigned int& expValue) const
        { return UIntPow(*this, expValue); }
        MediumDecV2 UInt64PowOf(const unsigned __int64& expValue) const
        { return UInt64Pow(*this, expValue); }

        /// <summary>
        /// Applies Power of operation (for signed integer exponents)
        /// </summary>
        /// <param name="expValue">The exponent value.</param>
        MediumDecV2 IntPowOf(const signed int& expValue) const
        { return IntPow(*this, expValue); }
        MediumDecV2 Int64PowOf(const signed __int64& expValue) const
        { return Int64Pow(*this, expValue); }

        static MediumDecV2 UnsignedMirroredIntPow(const MediumDecV2& tValue, const MirroredInt& expValue)
        {   return UnsignedMirroredIntPowV1<MediumDecV2>(tValue, expValue); }

        static MediumDecV2 MirroredIntPow(const MediumDecV2& tValue, const MirroredInt& expValue)
        {   return MirroredIntPowV1<MediumDecV2>(tValue, expValue); }

protected:

        /// <summary>
        /// Finds nTh Root of value based on https://www.geeksforgeeks.org/n-th-root-number/ code
        /// </summary>
        /// <param name="tValue">The target value(radicand) to perform operation on.</param>
        /// <param name="nValue">The nth root degree value.</param>
        /// <param name="precision">Precision level (smaller = more precise)</param>
		template<MediumDecVariant VariantType=MediumDecV2>
        static VariantType NthRootV1(const VariantType& tValue, const unsigned int& n, const VariantType& precision)
        {
        #if !defined(AltNum_EnableIRep)
            if (tValue.IsNegative())
                throw "Nth root of a negative number requires imaginary number support";
        else
            if (tValue.IsNegative())
            {
                VariantType result =  MediumDec::NthRootV1(tValue, n, precision);
                result.DecimalHalf.Flags = 3;
                return result;
            }
        #endif
            else
                return MediumDec::NthRootV1(tValue, n, precision);
        }

public:

        /// <summary>
        /// Finds nTh Root of value based on https://www.geeksforgeeks.org/n-th-root-number/ code
        /// </summary>
        /// <param name="tValue">The target value(radicand) to perform operation on.</param>
        /// <param name="nValue">The nth root degree value.</param>
        /// <param name="precision">Precision level (smaller = more precise)</param>
        static MediumDecV2 UnsignedNthRoot(const MediumDecV2& tValue, const unsigned int& n, const MediumDecV2& precision = MediumDecV2::JustAboveZero);

        /// <summary>
        /// Finds nTh Root of value based on https://www.geeksforgeeks.org/n-th-root-number/ code
        /// </summary>
        /// <param name="tValue">The target value(radicand) to perform operation on.</param>
        /// <param name="nValue">The nth root degree value.</param>
        /// <param name="precision">Precision level (smaller = more precise)</param>
        static MediumDecV2 NthRoot(const MediumDecV2& tValue, const unsigned int& n, const MediumDecV2& precision = MediumDecV2::JustAboveZero);

        MediumDecV2 NthRootOf(const unsigned int& n, const MediumDecV2& precision = MediumDecV2::JustAboveZero) const;

        //Alternative formula version of NthRoot
        static MediumDecV2 AlternativeNthRoot(const MediumDecV2& tValue, const unsigned int& n, const MediumDecV2& precision = MediumDecV2::FiveBillionth);

protected:

        /// <summary>
        /// Calculate value to a fractional power based on https://study.com/academy/lesson/how-to-convert-roots-to-fractional-exponents.html
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="Frac">The exponent value to raise the value to power of.</param>
		template<MediumDecVariant VariantType=MediumDecV2>
        static VariantType FractionalPowV1(const auto& value, const boost::rational<unsigned int>& Frac)
		{
			VariantType targetVal = UIntPowOf(Frac.numerator());
			VariantType CalcVal = MediumDecVariant::NthRoot(targetVal, Frac.denominator());
			return CalcVal;
		}

        /// <summary>
        /// Calculate value to a fractional power based on https://study.com/academy/lesson/how-to-convert-roots-to-fractional-exponents.html
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <param name="expNum">The numerator of the exponent value.</param>
        /// <param name="expDenom">The denominator of the exponent value.</param>
		template<MediumDecVariant VariantType=MediumDecV2>
        VariantType FractionalPowV2(const VariantType& value, const signed int& expNum, const unsigned int& expDenom)
		{
			auto targetVal = IntPowOf(expNum);
			auto CalcVal = MediumDecVariant::NthRoot(targetVal, expDenom);
			return CalcVal;
		}

        /// <summary>
        /// Calculate to power of unsigned expValue
		/// (Doesn't modify owner object)
        /// </summary>
		template<MediumDecVariant VariantType=MediumDecV2>
        VariantType UnsignedPowOfV1(const auto& expValue)
		{
			boost::rational<unsigned int> Frac = boost::rational<unsigned int>(expValue.DecimalHalf, MediumDecVariant::DecimalOverflow);
			if(expValue.IntHalf.Value==0)
				return FractionalPowV1(Frac);
			else {
				VariantType CalcVal = UIntPowOp(expValue.IntHalf.Value);
				CalcVal *= FractionalPowV1(Frac);
				return CalcVal;
			}
		}

        /// <summary>
        /// Calculate to power of expValue
		/// (Doesn't modify owner object)
        /// </summary>
		template<MediumDecVariant VariantType=MediumDecV2>
        VariantType PowOfV1(const auto& expValue)
		{
			boost::rational<unsigned int> Frac = boost::rational<unsigned int>(expValue.DecimalHalf, MediumDecVariant::DecimalOverflow);
			if (expValue.IntHalf.IsNegative()){//Negative Exponent
				if(expValue.IntHalf.Value==0)
					return VariantType::One/FractionalPowV1(Frac);
				else {
					VariantType CalcVal = One / UIntPowOf(expValue.IntHalf.Value);
					CalcVal /= FractionalPowV1(Frac);
					return CalcVal;
				}
			} else {
				if(expValue.IntHalf.Value==0)
					return FractionalPowV1(Frac);
				else {
					VariantType CalcVal = UIntPowOp(expValue.IntHalf.Value);
					CalcVal *= FractionalPowV1(Frac);
					return CalcVal;
				}
			}
		}

public:

        /// <summary>
        /// Calculate to power of expValue
		/// (Doesn't modify owner object)
        /// </summary>
        MediumDecV2 UnsignedPowOf(const auto& expValue)
		{ return UnsignedPowOfV1(expValue); }

        /// <summary>
        /// Calculate to power of expValue
		/// (Doesn't modify owner object)
        /// </summary>
        MediumDecV2 PowOf(const auto& expValue)
		{ return PowOfV1(expValue); }

	#pragma endregion Pow and Sqrt Functions

	#pragma region Log Functions
protected:

        /// <summary>
        /// Taylor Series Exponential function derived from https://www.pseudorandom.com/implementing-exp
        /// </summary>
        /// <param name="x">The value to apply the exponential function to.</param>
        /// <returns>VariantType</returns>
		template<MediumDecVariant VariantType=MediumDecV2>
        static VariantType ExpV1(const VariantType& x)
        {
            /*
             * Evaluates f(x) = e^x for any x in the interval [-709, 709].
             * If x < -709 or x > 709, raises an assertion error. Implemented
             * using the truncated Taylor series of e^x with ceil(|x| * e) * 12
             * terms. Achieves at least 14 and at most 16 digits of precision
             * over the entire interval.
             * Performance - There are exactly 36 * ceil(|x| * e) + 5
             * operations; 69,413 in the worst case (x = 709 or -709):
             * - (12 * ceil(|x| * e)) + 2 multiplications
             * - (12 * ceil(|x| * e)) + 1 divisions
             * - (12 * ceil(|x| * e)) additions
             * - 1 rounding
             * - 1 absolute value
             * Accuracy - Over a sample of 10,000 linearly spaced points in
             * [-709, 709] we have the following error statistics:
             * - Max relative error = 8.39803e-15
             * - Min relative error = 0.0
             * - Avg relative error = 0.0
             * - Med relative error = 1.90746e-15
             * - Var relative error = 0.0
             * - 0.88 percent of the values have less than 15 digits of precision
             * Args:
             *      - x: power of e to evaluate
             * Returns:
             *      - approximation of e^x in VariantType precision
             */
             // Check that x is a valid input.
            assert(x.IntHalf.Value < 709);
            // When x = 0 we already know e^x = 1.
            if (x.IsZero()) {
                return VariantType::One;
            }
            // Normalize x to a non-negative value to take advantage of
            // reciprocal symmetry. But keep track of the original sign
            // in case we need to return the reciprocal of e^x later.
            VariantType x0 = VariantType::Abs(x);
            // First term of Taylor expansion of e^x at a = 0 is 1.
            // tn is the variable we we will return for e^x, and its
            // value at any time is the sum of all currently evaluated
            // Taylor terms thus far.
            VariantType tn = VariantType::One;
            // Chose a truncation point for the Taylor series using the
            // heuristic bound 12 * ceil(|x| e), then work down from there
            // using Horner's method.
            int n = VariantType::CeilInt(x0 * VariantType::E) * 12;
            for (int i = n; i > 0; --i) {
                tn = tn * (x0 / i) + VariantType::One;
            }
            // If the original input x is less than 0, we want the reciprocal
            // of the e^x we calculated.
            if (x.IsNegative()) {
                tn = VariantType::One / tn;
            }
            return tn;
        }

		//Common log calculations for when value is between 0 and one
		template<MediumDecVariant VariantType=MediumDecV2>
		VariantType LogZeroRangePart02(const VariantType& AccuracyLevel=VariantType::JustAboveZero) const
		{
			VariantType TotalRes = (*this - 1)/ (*this + 1);
			VariantType WSquared = TotalRes * TotalRes;
			VariantType LastPow = -TotalRes;
			int WPow = 3;
			VariantType AddRes;

			do
			{
				LastPow *= WSquared;
				AddRes = LastPow / WPow;
				TotalRes -= AddRes;
				WPow += 2;
			} while (AddRes > VariantType::JustAboveZero);
			return TotalRes;
		}

		//Common natural log calculations for range one to two
		template<MediumDecVariant VariantType=MediumDecV2>
        VariantType LnOfOneSection(const VariantType& threshold = VariantType::FiveBillionth) const
        {
            VariantType base = *this - 1;        // Base of the numerator; exponent will be explicit
            bool posSign = true;             // Used to swap the sign of each term
            VariantType term = base;       // First term
            VariantType prev;          // Previous sum
            VariantType result = term;     // Kick it off
            // den = Denominator of the nth term
            for(unsigned int den = 2;VariantType::Abs(prev - result) > threshold;++den){
                posSign = !posSign;
                term *= base;
                prev = result;
                if (posSign)
                    result += term / den;
                else
                    result -= term / den;
            }
            return result;
        }

		//Common log calculations for when value is greater than one
		template<MediumDecVariant VariantType=MediumDecV2>
		VariantType LogGreaterRangePart02(const VariantType& AccuracyLevel=VariantType::JustAboveZero) const
		{
			//Increasing iterations brings closer to accurate result(Larger numbers need more iterations to get accurate level of result)
			VariantType TotalRes = (*this - 1) / (*this + 1);
			VariantType LastPow = TotalRes;
			VariantType WSquared = TotalRes * TotalRes;
			VariantType AddRes;
			int WPow = 3;
			do
			{
				LastPow *= WSquared;
				AddRes = LastPow / WPow;
				TotalRes += AddRes; WPow += 2;
			} while (AddRes > AccuracyLevel);
			return TotalRes;
		}

		template<MediumDecVariant VariantType=MediumDecV2, IntegerType IntType = unsigned int>
		static VariantType LogGreaterRangeIntPart02(const IntType& value, const VariantType& AccuracyLevel=VariantType::JustAboveZero)
		{
			VariantType tValue = VariantType(value);
			return tValue.LogGreaterRangePart02(AccuracyLevel);
		}

        /// <summary>
        /// Natural log (Equivalent to Log_E(value))
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <returns>MediumDec variant</returns>
        template<MediumDecVariant VariantType = MediumDecV2>
        #if defined(AltNum_UseCustomLnAccuracy)
        VariantType LnOfV1(const VariantType& threshold = VariantType::FiveMillionth) const
        #else
        VariantType LnOfV1() const
        #endif
        {//Negative values for natural log return value of LnV1(-value) * i
            //if (value <= 0) {}else//Error if equal or less than 0
            if (IsOne())
                return VariantType::Zero;
            if (IntHalf == MirroredInt::Zero)//Returns a negative number derived from (http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                #if defined(AltNum_UseCustomLnAccuracy)&&!defined(AltNum_UseSeparateLnAccuracyRanges)
                return LogZeroRangePart02(threshold).MultipliedByTwo();
                #else
                return LogZeroRangePart02().MultipliedByTwo();
                #endif
            }
            else if (IntHalf == MirroredInt::One)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer(for values between 1 and 2)
                #if defined(AltNum_UseCustomLnAccuracy)
                return LnOfOneSection(threshold);
                #else
                return LnOfOneSection();
                #endif
            }
            else
            {//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
                #if defined(AltNum_UseCustomLnAccuracy)&&!defined(AltNum_UseSeparateLnAccuracyRanges)
                return LogGreaterRangePart02(threshold).MultipliedByTwo();
                #else
                return LogGreaterRangePart02().MultipliedByTwo();
                #endif
            }
        }

        /// <summary>
        /// Natural log (Equivalent to Log_E(value))
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <returns>MediumDec variant</returns>
        template<MediumDecVariant VariantType = MediumDecV2>
        static VariantType LnV1(const VariantType& value)
        {
            return value.LnOfV1();
        }

        /// <summary>
        /// Log Base 10 of tValue
        /// </summary>
        /// <param name="tValue">The value.</param>
        /// <returns>MediumDec</returns>
        template<MediumDecVariant VariantType = MediumDecV2>
        #if defined(AltNum_UseCustomLnAccuracy)
        VariantType Log10OfV1(const VariantType& threshold = VariantType::FiveMillionth) const
        #else
        VariantType Log10OfV1() const
        #endif
        {
            if (IsOne())
                return VariantType::Zero;
            #if !defined(AltNum_PreventLog10IntegerLoop)
            if (DecimalHalf == 0 && IntHalf.Value % 10 == 0)
            {//Might not be worth using checking to use this alternative code since since 10s aren't that common
                for (int index = 1; index < 9; ++index)
                {
                    if (IntHalf.Value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                        return VariantType(index, 0);
                }
                return VariantType(9, 0);
            }
            #endif
            const VariantType lnMultiplier = VariantType(0, LN10Div_DecSection);
            if (IntHalf == MirroredInt::Zero)//Returns a negative number derived from (http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                #if defined(AltNum_UseCustomLnAccuracy)&&!defined(AltNum_UseSeparateLnAccuracyRanges)
                return LogZeroRangePart02(threshold).MultiplyByUnsigned(lnMultiplier);
                #else
                return LogZeroRangePart02().MultiplyByUnsigned(lnMultiplier);
                #endif
            }
            else if (IntHalf == MirroredInt::One)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer for values between 1 & 2
                #if defined(AltNum_UseCustomLnAccuracy)
                return LnOfOneSection(threshold).MultiplyByUnsigned(lnMultiplier);
                #else
                return LnOfOneSection().MultiplyByUnsigned(lnMultiplier);
                #endif
            }
            else//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                #if defined(AltNum_UseCustomLnAccuracy)&&!defined(AltNum_UseSeparateLnAccuracyRanges)
                return LogGreaterRangePart02(threshold).MultiplyByUnsigned(lnMultiplier);
                #else
                return LogGreaterRangePart02().MultiplyByUnsigned(lnMultiplier);
                #endif
            }
        }

        /// <summary>
        /// Log Base 10 of tValue
        /// </summary>
        /// <param name="tValue">The value.</param>
        /// <returns>MediumDec</returns>
        template<MediumDecVariant VariantType = MediumDecV2>
        static VariantType Log10V1(const VariantType& value)
        {
            return value.Log10OfV1();
        }

        /// <summary>
        /// Log Base 10 of tValue(integer value variant)
        /// </summary>
        /// <param name="tValue">The value.</param>
        /// <returns>MediumDec</returns>
		template<MediumDecVariant VariantType=MediumDecV2, IntegerType IntType = unsigned int>
        static VariantType Log10OfIntV1(const IntType& value)
        {
            if (value == 1)
                return VariantType::Zero;
            if (value % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                        return VariantType(index);
                }
                return VariantType(9);
            }
            else//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                VariantType lnMultiplier = VariantType(0, TwiceLN10Div_DecSection);
                return LogGreaterRangeIntPart02(value).MultiplyByUnsigned(lnMultiplier);
            }
        }

        /// <summary>
        /// Log with Base of BaseVal of tValue
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="value">The value.</param>
        /// <param name="baseVal">The base of Log</param>
        /// <returns>MediumDec Variant</returns>
		template<MediumDecVariant VariantType=MediumDecV2>
        static VariantType LogV1(const VariantType& value, const VariantType& baseVal)
        {
            if (value == VariantType::One)
                return VariantType::Zero;
            return Log10V1(value) / Log10V1(baseVal);
        }

        /// <summary>
        /// Log with Base of BaseVal of tValue
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="tValue">The value.</param>
        /// <param name="BaseVal">The base of Log</param>
        /// <returns>VariantType</returns>
		template<MediumDecVariant VariantType=MediumDecV2, IntegerType IntType = unsigned int>
        static VariantType LogOfIntV1(const VariantType& value, const IntType& baseVal)
        {
            //Calculate Base log first
            VariantType baseTotalRes;
            bool lnMultLog = true;
            if (baseVal % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (baseVal == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                    {
                        baseTotalRes = VariantType(index, 0);
                        break;
                    }
                }
                baseTotalRes = VariantType(9, 0); lnMultLog = false;
            }
            else//Returns a positive baseVal(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                baseTotalRes = LogGreaterRangeIntPart02(baseVal);
            }
            VariantType lnMultiplier = VariantType(0, TwiceLN10Div_DecSection);
            //Now calculate other log
            if (value.DecimalHalf == 0 && value.IntHalf.Value % 10 == 0)
            {
                for (int index = 1; index < 9; ++index)
                {
                    if (value == BlazesRusCode::VariableConversionFunctions::PowerOfTens[index])
                        return lnMultLog ? VariantType(index, 0) / (baseTotalRes * lnMultiplier): VariantType(index, 0)/ baseTotalRes;
                }
                return lnMultLog? VariantType(9, 0) / (baseTotalRes.MultiplyByUnsigned(lnMultiplier)):VariantType(9, 0)/baseTotalRes;
            }
			if(value.IntHalf==MirroredInt::Zero)//Not tested this block but should work
			{
                VariantType TotalRes = value.LogZeroRangePart02();
				if(lnMultLog)
					return TotalRes.DivideByUnsigned(baseTotalRes);
				else
					return (TotalRes.MultiplyByUnsigned(lnMultiplier)).DivideByUnsigned(baseTotalRes);
			}
            else if (value.IntHalf==MirroredInt::One)//Threshold between 0 and 2 based on Taylor code series from https://stackoverflow.com/questions/26820871/c-program-which-calculates-ln-for-a-given-variable-x-without-using-any-ready-f
            {//This section gives accurate answer for values between 1 & 2
				if(lnMultLog)
					return value.LnOfOneSection()/baseTotalRes;
				else
					return (value.LnOfOneSection().MultipliedByTwo())/ baseTotalRes;
            }
            else//Returns a positive value(http://www.netlib.org/cephes/qlibdoc.html#qlog)
            {
                VariantType TotalRes = value.LogGreaterRangePart02();
				if(lnMultLog)
					return TotalRes.DivideByUnsigned(baseTotalRes);
				else
					return (TotalRes.MultiplyByUnsigned(lnMultiplier)).DivideByUnsigned(baseTotalRes);
            }
        }

public:

        /// <summary>
        /// Taylor Series Exponential function derived from https://www.pseudorandom.com/implementing-exp
        /// </summary>
        /// <param name="x">The value to apply the exponential function to.</param>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 Exp(const MediumDecV2& x) { return ExpV1(x); }

        /// <summary>
        /// Natural log (Equivalent to Log_E(value))
        /// </summary>
        /// <param name="value">The target value.</param>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 Ln(const MediumDecV2& value)
        { return LnV1(value); }

        /// <summary>
        /// Log Base 10 of tValue
        /// </summary>
        /// <param name="tValue">The value.</param>
        /// <returns>MediumDec</returns>
        static MediumDecV2 Log10(const MediumDecV2& value)
        { return Log10V1(value); }

        /// <summary>
        /// Log Base 10 of tValue(integer value variant)
        /// </summary>
        /// <param name="tValue">The value.</param>
        /// <returns>MediumDec</returns>
        static MediumDecV2 Log10OfInt(const unsigned int& value)
        { return Log10OfIntV1(value); }

        /// <summary>
        /// Log with Base of BaseVal of tValue
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="value">The value.</param>
        /// <param name="baseVal">The base of Log</param>
        /// <returns>MediumDec Variant</returns>
        static MediumDecV2 Log(const MediumDecV2& value, const MediumDecV2& baseVal)
        { return LogV1(value, baseVal); }

        /// <summary>
        /// Log with Base of BaseVal of tValue
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="tValue">The value.</param>
        /// <param name="BaseVal">The base of Log</param>
        /// <returns>MediumDecV2</returns>
        static MediumDecV2 LogOfInt(const MediumDecV2& value, const unsigned int& baseVal)
        { return LogOfIntV1(value, baseVal); }

	#pragma endregion Log Functions

    #pragma region Trigonomic Functions
#if defined(AltNum_EnablePiRep)
protected:

    	template<MediumDecVariant VariantType=MediumDecV2>
        static VariantType NormalizePiForTrig(VariantType value)
        {
            if (value.IsNegative())
            {
                if (value.IntHalf == NegativeRep)
                    value.IntHalf = 1;
                else
                {
                    value.IntHalf *= -1;
                    value.IntHalf %= 2;
                    value.IntHalf = 2 - value.IntHalf;
                }
                if(value.DecimalHalf==MediumDecV2::ApproachingBottomRep)
                    value.DecimalHalf.Value = MediumDecV2::ApproachingTopRep;
                else if(value.DecimalHalf==MediumDecV2::ApproachingTopRep)
                    value.DecimalHalf.Value = MediumDecV2::ApproachingBottomRep;
                else
                    value.DecimalHalf.Value = DecimalOverflow - value.DecimalHalf.Value;
            }
            else
            {
                value.IntHalf %= 2;
            }
            return value;
        }

        /// <summary>
        /// Calculate Sine from tValue in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The value in Radians.</param>
		template<MediumDecVariant VariantType=MediumDecV2>
        static VariantType SinV1(const VariantType& tValue)
        {
            if (tValue.GetFlags() == 1)
            {
                VariantType value = NormalizePiForTrig(tValue);
                if (value.DecimalHalf == 0)//0 and Pi
                    return VariantType::Zero;
                else if (value.DecimalHalf == 500000000)
                    return value.IntHalf==0?VariantType::One:VariantType::NegativeOne;
                else
                    return MediumDec::SinV1<VariantType>(value);
            }
            else
                return MediumDec::SinV1<VariantType>(tValue);
        }

        /// <summary>
        /// Get Cos from tValue in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The value in Radians.</param>
		template<MediumDecVariant VariantType=MediumDecV2>
        static VariantType CosV1(const VariantType& tValue)
        {
            if (tValue.GetFlags() == 1)
            {
                VariantType value = NormalizePiForTrig(tValue);
				if(value.DecimalHalf == 0)
                    return value.IntHalf==0?VariantType::One:VariantType::NegativeOne;
				else if(value.DecimalHalf == 500000000)
					return VariantType::Zero;
				else
					return MediumDec::CosV1<VariantType>(value);
            }
            else
                return MediumDec::CosV1<VariantType>(tValue);
        }

        /// <summary>
        /// Get Tan from tValue in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The value in Radians.</param>
		template<MediumDecVariant VariantType=MediumDecV2>
        static VariantType TanV1(const VariantType& tValue)
        {
            if (tValue.GetFlags() == 1)
            {
                VariantType value = NormalizePiForTrig(tValue);
				if(value.DecimalHalf == 0)//0 Pi = 0; 1Pi = 0
					return VariantType::Zero;
				else if(value.DecimalHalf== 500000000){
				#if defined(AltNum_EnableInfinityRep)
					if(value.IntHalf.Value==0)
						return VariantType::PositiveInfinity;
					else
						return VariantType::NegativeInfinity;
				#else
					if(value.IntHalf.Value==0)
						return VariantType::Maximum;
					else
						return VariantType::Minimum;
				#endif
				} else
					return MediumDec::TanV1<VariantType>(value);
            }
            else
                return MediumDec::TanV1<VariantType>(tValue);
        }

		template<MediumDecVariant VariantType=MediumDecV2>
        static VariantType ATanV1(const VariantType& tValue)
        {
            if (tValue.GetFlags() == 1)
            {
                VariantType value = NormalizePiForTrig(tValue);
				if(value.DecimalHalf== 500000000)
					return VariantType::Zero;
				else
					return MediumDec::ATanV1<VariantType>(value);
            }
            else
                return MediumDec::ATanV1<VariantType>(tValue);
        }

        /// <summary>
        /// Get Sin from tValue of angle.
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The target value in degrees to perform operation on.</param>
		template<MediumDecVariant VariantType=MediumDecV2>
        static VariantType SinFromAngleV1(const VariantType& tValue)
        {
            if(tValue.GetFlags()==1||tValue.GetFlags()==2)
            {
                VariantType Radius = PiNum * tValue / 180;
                return MediumDec::SinV1<VariantType>(Radius);
            }
			VariantType value = NormalizeForTrig(tValue);
            if(value.DecimalHalf==0)
            {
                switch(value.IntHalf.Value)
                {
                    case 0:
                    case 180://Pi Radians
                        return VariantType::Zero;
                        break;
                    case 90://0.5 Pi Radians
                        if (value.GetFlags() == 3)
                            return VariantType(1,PartialInt(0,3));
                        else
                            return VariantType::One;
                        break;
                    case 270://1.5 Pi Radians
                        if (value.GetFlags() == 3)
                            return VariantType(MirroredInt::NegativeOne,PartialInt(0,3));
                        else
                            return VariantType::NegativeOne;
                        break;
                    case 30://0.1666666666 Pi Radians
                    case 150://0.833333333 Pi Radians
                        if (value.GetFlags() == 3)
                            return VariantType(0,PartialInt(500000000,3));
                        else
                            return VariantType::PointFive;
                    case 210:
                    case 330:
                        if (value.GetFlags() == 3)
                            return VariantType(MirroredInt::NegativeZero,PartialInt(500000000,3));
                        else
                            return VariantType::NegativePointFive;
                    default://Angle as Radian
						if (value.GetFlags() == 3)
						{
							value = value * PiNum / 180;
							return MediumDec::SinV1<VariantType>(value);
						}
						else
						{
							value /= 180;
							value.DecimalHalf.Flags = 1;
							return MediumDec::SinV1<VariantType>(value);
						}
                        break;
                }
            }
			else if (value.GetFlags() == 3)
			{
				value = value * PiNum / 180;
				return MediumDec::SinV1<VariantType>(value);
			}
            else
            {
                VariantType Radius = value / 180;
                Radius.DecimalHalf.Flags = 1;
                return MediumDec::SinV1<VariantType>(Radius);
            }
        }

        /// <summary>
        /// Get Cos() from tValue of Angle
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The target value in degrees to perform operation on.</param>
		template<MediumDecVariant VariantType=MediumDecV2>
        static VariantType CosFromAngleV1(const VariantType& tValue)
        {
            if(tValue.GetFlags()==1||tValue.GetFlags()==2)
            {
                VariantType Radius = PiNum * tValue / 180;
                return MediumDec::CosV1<VariantType>(Radius);
            }
			VariantType value = NormalizeForTrig(tValue);
            if(value.DecimalHalf==0)
            {
                switch(value.IntHalf.Value)
                {
                    case 0:
                        if (value.GetFlags() == 3)
                            return VariantType(1,PartialInt(0,3));
                        else
                            return VariantType::One;
                        break;
                    case 60:
                        if (value.GetFlags() == 3)
                            return VariantType(0,PartialInt(500000000,3));
                        else
                            return VariantType::PointFive;
                        break;
                    case 90://0.5 Pi Radians
                    case 270://1.5 Pi Radians
                        return VariantType::Zero;
                        break;
                    case 180://Pi Radians
                        if (value.GetFlags() == 3)
                            return VariantType(MirroredInt::NegativeOne,PartialInt(0,3));
                        else
                            return VariantType::NegativeOne;
                        break;
                    case 120:
                    case 240:
                        if (value.GetFlags() == 3)
                            return VariantType(MirroredInt::NegativeZero,PartialInt(500000000,3));
                        else
                            return VariantType::NegativePointFive;
                    default://Angle as Radian
						if (value.GetFlags() == 3)
						{
							value = value * PiNum / 180;
							return MediumDec::CosV1<VariantType>(value);
						}
						else
						{
							value /= 180;
							value.DecimalHalf.Flags = 1;
							return MediumDec::CosV1<VariantType>(value);
						}
                        break;
                }
            }
			else if (value.GetFlags() == 3)
			{
				value = value * PiNum / 180;
				return MediumDec::CosV1<VariantType>(value);
			}
            else
            {
                VariantType Radius = value / 180;
                Radius.DecimalHalf.Flags = 1;
                return MediumDec::CosV1<VariantType>(Radius);
            }
        }

        /// <summary>
        /// Get Tangent from tValue in Degrees (SlopeInPercent:http://communityviz.city-explained.com/communityviz/s360webhelp4-2/formulas/function_library/atan_function.htm)
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The target value in degrees to perform operation on.</param>
		template<MediumDecVariant VariantType=MediumDecV2>
        static VariantType TanFromAngleV1(const VariantType& tValue)
        {
            if(tValue.GetFlags()==1||tValue.GetFlags()==2)
            {
                VariantType Radius = PiNum * tValue / 180;
                return MediumDec::TanV1<VariantType>(Radius);
            }
			VariantType value = NormalizeForTrig(tValue);
            if(value.DecimalHalf==0)
            {
                switch(value.IntHalf.Value)
                {
                    case 0:
                    case 180://Pi Radians
                        return VariantType::Zero;
                        break;
                    case 90://0.5 Pi Radians
						if (value.GetFlags() == 3)
					#if defined(AltNum_EnableInfinityRep)
    						return VariantType::PositiveImaginaryInfinity;
    					#else
                            return VariantType::ImaginaryMaximum;//Positive Infinity
    					#endif
                        else
    					#if defined(AltNum_EnableInfinityRep)
    						return VariantType::PositiveInfinity;
    					#else
                            return VariantType::Maximum;//Positive Infinity
    					#endif
                        break;
                    case 270://1.5 Pi Radians
						if (value.GetFlags() == 3)
    					#if defined(AltNum_EnableInfinityRep)
    						return VariantType::NegativeImaginaryInfinity;
    					#else
                            return VariantType::ImaginaryMinimum;//Negative Infinity
    					#endif
                        else
    					#if defined(AltNum_EnableInfinityRep)
    						return VariantType::NegativeInfinity;
    					#else
                            return VariantType::Minimum;//Negative Infinity
    					#endif
                        break;
                    default://Angle as Radian
						if (value.GetFlags() == 3)
						{
							value = value * PiNum / 180;
							return MediumDec::TanV1<VariantType>(value);
						}
						else
						{
							value /= 180;
							value.DecimalHalf.Flags = 1;
							return MediumDec::TanV1<VariantType>(value);
						}
                        break;
                }
            }
			else if (value.GetFlags() == 3)
			{
				value = value * PiNum / 180;
				return MediumDec::TanV1<VariantType>(value);
			}
            else
            {
                VariantType Radius = value / 180;
                Radius.DecimalHalf.Flags = 1;
                return MediumDec::TanV1<VariantType>(Radius);
            }
        }
#endif
public:

        /// <summary>
        /// Calculate Sine from tValue in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The value in Radians.</param>
        static MediumDecV2 Sin(const MediumDecV2& tValue)
        {
            #if defined(AltNum_EnableInfinityRep)
            if(DecimalHalf==InfinityRep)
                throw "Operation results in undefined value between 1 and -1";
            #endif
			return SinV1<MediumDecV2>(tValue);
		}

        /// <summary>
        /// Get cosine from tValue in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The tValue in Radians.</param>
        static MediumDecV2 Cos(const MediumDecV2& tValue)
        {
            #if defined(AltNum_EnableInfinityRep)
            if(DecimalHalf==InfinityRep)
                throw "Operation results in undefined value between 1 and -1";
            #endif
			return CosV1<MediumDecV2>(tValue);
		}

        /// <summary>
        /// Get Tan from value in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The tValue in Radians.</param>
        static MediumDecV2 Tan(const MediumDecV2& tValue)
        { return TanV1<MediumDecV2>(tValue); }

        /// <summary>
        /// Gets Inverse Tangent from tValue in Radians
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The tValue in Radians.</param>
        static MediumDecV2 ATan(const MediumDecV2& tValue)
        { return ATanV1<MediumDecV2>(tValue); }

        /// <summary>
        /// Get Sin from value of angle.
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The target value in degrees to perform operation on.</param>
        static MediumDecV2 SinFromAngle(const MediumDecV2& tValue)
        { return SinFromAngleV1<MediumDecV2>(tValue); }

        /// <summary>
        /// Get Cos() from value of Angle
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The target value in degrees to perform operation on.</param>
        static MediumDecV2 CosFromAngle(const MediumDecV2& tValue)
        { return CosFromAngleV1<MediumDecV2>(tValue); }

        /// <summary>
        /// Get Tangent from value in Degrees (SlopeInPercent:http://communityviz.city-explained.com/communityviz/s360webhelp4-2/formulas/function_library/atan_function.htm)
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="tValue">The target value in degrees to perform operation on.</param>
        static MediumDecV2 TanFromAngle(const MediumDecV2& tValue)
        { return TanFromAngleV1<MediumDecV2>(tValue); }

		MediumDecV2 SinOf()
        { return Sin(*this); }

		MediumDecV2 CosOf()
        { return Cos(*this); }

		MediumDecV2 TanOf()
        { return Tan(*this); }

		MediumDecV2 ATanOf()
        { return ATan(*this);; }

        /// <summary>
        /// atan2 calculation with self normalization
        /// Application: Used when one wants to compute the 4-quadrant arctangent of a complex number (or any number with x-y coordinates) with a self-normalizing function.
        /// Example Applications: digital FM demodulation, phase angle computations
        /// Code from http://dspguru.com/dsp/tricks/fixed-point-atan2-with-self-normalization/ with some slight edit to get working
        /// </summary>
        /// <param name="y">The y.</param>
        /// <param name="X">The x.</param>
        static MediumDecV2 ArcTan2(const MediumDecV2& y, const MediumDecV2& x)
        { return ArcTan2V1<MediumDecV2>(y, x); }

    #pragma endregion Trigonomic Functions
    };

    }