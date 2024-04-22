// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include "..\MediumDec\AltNumBase.hpp"

namespace BlazesRusCode
{

    class PartialDec;

    /// <summary>
    /// Reduced version of AltDec that only adds feature of Fractionals to MediumDec
	/// and removes support for negative numbers
	/// </summary>
    class DLL_API PartialDec : AltNumBase
    {
	public:
        /// <summary>
        /// The decimal overflow
        /// </summary>
        static signed int const DecimalOverflow = 1000000000;

        /// <summary>
        /// The decimal overflow
        /// </summary>
        static signed _int64 const DecimalOverflowX = 1000000000;
		
	public:

        /// <summary>
        /// long double (Extended precision double)
        /// </summary>
        using ldouble = long double;

        /// <summary>
        /// Stores whole half of number(Including positive/negative status)
		/// (in the case of infinity is used to determine if positive vs negative infinity)
        /// </summary>
        unsigned int IntHalf;

		//Return IntHalf as signed int
        unsigned int GetIntHalf()
        {
			return IntHalf;
        }

        bool IsNegative()
        {
			return false;
        }

        /// <summary>
        /// Stores decimal section info and other special info
        /// </summary>
        unsigned int DecimalHalf;

        /// <summary>
        /// Initializes a new instance of the <see cref="PartialDec"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        PartialDec(const unsigned int& intVal = 0, const unsigned int& decVal = 0)
        {
            IntValue = intVal;
            DecimalHalf = decVal;
        }

        PartialDec(const PartialDec&) = default;

        PartialDec& operator=(const int& rhs)
        {
	#if defined(AltNum_EnableMirroredSection)
			if(rhs<0)
			{
				IntValue.Value = -rhs;
				IntValue.IsPositive = 0;
			}
			else
	#endif
				IntValue = rhs;
			DecimalHalf = 0;
            return *this;
        } const

        PartialDec& operator=(const PartialDec& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            IntValue = rhs.IntValue; DecimalHalf = rhs.DecimalHalf;
            return *this;
        } const

        //Is at either zero or negative zero IntHalf of AltNum
        bool IsAtZeroInt()
        {
	#if defined(AltNum_EnableMirroredSection)
            return IntValue.Value==0;
    #else
            return IntValue==0||IntValue==NegativeRep;
    #endif
        }

        bool IsNotAtZeroInt()
        {
	#if defined(AltNum_EnableMirroredSection)
            return IntValue.Value!=0;
    #else
            return IntValue != 0 && IntValue != NegativeRep;
    #endif
        }

        //Detect if at exactly zero(only overridden with MixedDec)
		bool IsZero()
		{
            return DecimalHalf==0&&IntValue==0;
		}

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        template<MediumDecVariant VariantType=PartialDec>
        void SetVal(VariantType Value)
        {
            IntValue = Value.IntValue;
            DecimalHalf = Value.DecimalHalf;
        }

        void SetAsZero()
        {
            IntValue = 0;
            DecimalHalf = 0;
        }
        
        /// <summary>
        /// Swaps the negative status.
        /// </summary>
        void SwapNegativeStatus()
        {
	#if defined(AltNum_EnableMirroredSection)
            IntValue.IsPositive ^= 1;
    #else
            if (IntValue == NegativeRep)
            {
                IntValue = 0;
            }
            else if (IntValue == 0)
            {
                IntValue = NegativeRep;
            }
            else
            {
                IntValue *= -1;
            }
    #endif
        }

    #pragma region Const Representation values
    protected:

    #pragma endregion Const Representation values

    #pragma region RepType

        /// <summary>
        /// Enum representing value type stored
        /// </summary>
        enum class RepType: RepTypeUnderlayer
        {
            //Sign = IntValue.IsNegative()?-1:1;
            NormalType = 0,
	//#if defined(AltNum_EnableFractionals)
            NumByDiv = 8,
	//#endif
	//#if defined(AltNum_EnablePowerOfRepresentation)
            ToPowerOf = 16,
	//#endif
	//#if defined(AltNum_EnablePiRep)
            PiNum = 1,
		//#if defined(AltNum_EnablePowerOfRepresentation)
            PiPower = 17,
		//#endif
		#if !defined(AltNum_UseIntForDecimalHalf)||defined(AltNum_EnableDecimaledPiFractionals)
            PiNumByDiv = 9,//  (Value/(ExtraRep.Value))*Pi Representation
		#endif
		#if defined(AltNum_UseIntForDecimalHalf)&&!defined(AltNum_EnableDecimaledPiFractionals)
            PiFractional = 9,//  IntValue/DecimalHalf*Pi Representation
		#endif
		//#endif
		//#if defined(AltNum_EnableApproachingPi)
            //(Enum Bits:7,1)
            //equal to IntValue.9..9 Pi
            ApproachingTopPi = 65,
		//#endif
		//#if defined(MixedDec_EnableApproachingAlternativeDiv)
		
            //(Enum Bits:7,5,1)
			ApproachingMidLeftPi = 81,
			//#if !defined(AltNum_DisableApproachingTop)
            
            //(Enum Bits:7,4,5,1)
            ApproachingMidRightPi = 89,
			//#endif
		//#endif
	//#endif
	//#if defined(AltNum_EnableERep)
            ENum = 2,
		//#if defined(AltNum_EnablePowerOfRepresentation)
            EPower = 18,
		//#endif
		#if !defined(AltNum_UseIntForDecimalHalf)||!defined(AltNum_EnableDecimaledEFractionals)
            ENumByDiv = 10,//(Value/(ExtraRep.Value))*e Representation
		#endif
		#if defined(AltNum_UseIntForDecimalHalf)&&!defined(AltNum_EnableDecimaledEFractionals)
            EFractional = 9,//  IntValue/DecimalHalf*Pi Representation
		#endif
		//#if defined(AltNum_EnableApproachingE)
            //(Enum Bits:7,2)
            //equal to IntValue.9..9 e
            ApproachingTopE = 66,
		//#endif
		//#if defined(MixedDec_EnableApproachingAlternativeDiv)
		
            //(Enum Bits:7,5,2)
			ApproachingMidLeftE = 82,
			//#if !defined(AltNum_DisableApproachingTop)
            
            //(Enum Bits:7,4,5,2)
            ApproachingMidRightE = 90,
			//#endif
		//#endif
	//#endif
	//#if defined(AltNum_EnableImaginaryNum)
            INum = 4,
		#if !defined(AltNum_UseIntForDecimalHalf)||!defined(AltNum_EnableDecimaledIFractionals)
            INumByDiv = 11,//(Value/(ExtraRep.Value))*i Representation
		#endif
		#if defined(AltNum_UseIntForDecimalHalf)&&!defined(AltNum_EnableDecimaledIFractionals)
            IFractional = 11,//  IntValue/DecimalHalf*i Representation
		#endif
	//#endif
	#if defined(AltNum_EnableMixedFractional)
            //Sign*(IntValue + (DecimalHalf.Value/ExtraRep.Value))
            MixedFrac = 32,
        #if !defined(AltNum_UseIntForDecimalHalf)||defined(AltNum_EnableMixedPiFractional)
            //Sign*(IntValue + (DecimalHalf.Value/ExtraRep.Value))
            MixedPi = 33,
		#endif
		#if !defined(AltNum_UseIntForDecimalHalf)||defined(AltNum_EnableMixedEFractional)
            //Sign*(IntValue + (DecimalHalf.Value/ExtraRep.Value))
            MixedE = 34,
		#endif
		#if !defined(AltNum_UseIntForDecimalHalf)||defined(AltNum_EnableMixedIFractional)
            //Sign*(IntValue + (DecimalHalf.Value/ExtraRep.Value))
            MixedI = 36,
		#endif
	#endif

	//#if defined(AltNum_EnableInfinityRep)
            //(Enum Bits:7,6)
            //If Positive Infinity, then convert number into MaximumValue instead when need as real number
            //If Negative Infinity, then convert number into MinimumValue instead when need as real number
			Infinity = 96,
	//#endif
	//#if defined(AltNum_EnableApproachingValues)
            //(Enum Bits:7)
            ApproachingBottom = 64,//(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)
		//#if !defined(AltNum_DisableApproachingTop)

            //(Enum Bits:7,4)
            //(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)
            ApproachingTop = 72,
		//#endif
		//#if defined(AltNum_EnableApproachingDivided)
		
            //(Enum Bits:7,5)
            //DecimalHalf:1000000000/ExtraRep - ApproachingZero (AlternativeName:ApproachingMidLeft)
			ApproachingMidLeft = 80,
			//#if !defined(AltNum_DisableApproachingTop)
            
            //(Enum Bits:7,4,5)
            //DecimalHalf:1000000000/ExtraRep + ApproachingZero (AlternativeName:ApproachingMidRight)
            ApproachingMidRight = 88,
			//#endif
		//#endif
	//#endif
	//#if defined(AltNum_EnableImaginaryInfinity)
            //(Enum Bits:7,6,3)
            ImaginaryInfinity = 100,
	//#endif
	//#if defined(AltNum_EnableApproachingI)
            //(Enum Bits:7,3)
            //(Approaching Towards Zero);(IntValue of 0 results in 0.00...1)i
            ApproachingImaginaryBottom = 68,
		//#if !defined(AltNum_DisableApproachingTop)

            //(Enum Bits:7,3,4)
            //(Approaching Away from Zero);(IntValue of 0 results in 0.99...9)i
            ApproachingImaginaryTop = 76,
		//#endif
		//#if defined(AltNum_EnableApproachingDivided)

            //(Enum Bits:7,3,5)
            //DecimalHalf:1000000000/ExtraRep - ApproachingImaginaryZero
			ApproachingImaginaryMidLeft = 84,
			//#if !defined(AltNum_DisableApproachingTop)

            //(Enum Bits:7,3,5,4)
            //DecimalHalf:1000000000/ExtraRep + ApproachingImaginaryZero
            ApproachingImaginaryMidRight = 92,
			//#endif
		//#endif
    //#endif
    //#if defined(AltNum_EnableNaN)
            //(Enum Bits:8)
            Undefined = 128,
            //(Enum Bits:8, 1)
            NaN = 129,
    //#endif
	//#if defined(AltNum_EnableUndefinedButInRange)

            //(Enum Bits:8, 2)
            //Such as result of Cos of infinity(value format part uses for +- range, DecimalHalf==UndefinedInRangeRep)
            UndefinedButInRange = 130,
		//#if defined(AltNum_EnableWithinMinMaxRange)

            //(Enum Bits:8,6)
            //Undefined except for ranged IntValue to DecimalHalf (ExtraRepValue==UndefinedInRangeMinMaxRep)
			WithinMinMaxRange = 160,
		//#endif
	//#endif
    //#if defined(AltNum_EnableNil)
            //(Enum Bits:8, 1, 2)
            Nil = 131,
    //#endif
		//#ifdef AltNum_EnableComplexNumbers

            //Enum Bits subject to change for Complec Number later(Not completely used yet)
            ComplexIRep = 255,
		//#endif
            //(Enum Bits:8, 1, 2, 3)
            UnknownType = 135
		}

/*
        /// <summary>
        /// Returns representation type data that is stored in value
        /// </summary>
        virtual RepType const GetRepType()
        {
            return RepType::UnknownType;//Virtual code replaced inside main class(Not actually used inside MediumDec class)
        }
*/

    #pragma endregion RepType

public:
    #pragma region RangeLimits

        /// <summary>
        /// Sets value to the highest non-infinite/Special Decimal State Value that it store
        /// </summary>
        virtual void SetAsMaximum()
        {
            IntValue = 2147483647;
			DecimalHalf = 999999999;
        }

        /// <summary>
        /// Sets value to the lowest non-infinite/Special Decimal State Value that it store
        /// </summary>
        virtual void SetAsMinimum()
        {
		#if defined(AltNum_EnableMirroredSection)
			IntValue = MirroredInt(2147483647,0);
		#else
            IntValue = -2147483647;
		#endif
			DecimalHalf = 999999999;
        }
	
    #pragma endregion RangeLimits

	#pragma region PiNum Setters
	//Not used for this variant(Used in PartialDecV2 and others)
	#pragma endregion PiNum Setters

	#pragma region ENum Setters
	//Not used for this variant(Used in PartialDecV2 and others)
	#pragma endregion ENum Setters

    #pragma region INum Setters
    //Not used for this variant(Used in PartialDecV2 and others)
    #pragma endregion INum Setters

	#pragma region Fractional Setters
	//Not used for this variant(Used in AltDecBase and others)
	#pragma endregion Fractional Setters
        
	#pragma region MixedFrac Setters
	//Not used for this variant(Used in AltDecBase and others)
	#pragma endregion MixedFrac Setters
		
	#pragma region Infinity Setters
	//Not used for this variant(Used in PartialDecV2 and others)
	#pragma endregion Infinity Setters
	
	#pragma region ApproachingZero Setters
	//Not used for this variant(Used in PartialDecV2 and others)
	#pragma endregion ApproachingZero Setters

	#pragma region NaN Setters
	#if defined(AltNum_EnableNaN)
        void SetAsNaN()
        {
            IntValue = 0; DecimalHalf = NaNRep;
        }

        void SetAsUndefined()
        {
            IntValue = 0; DecimalHalf = UndefinedRep;
        }
	#endif
	#pragma endregion NaN Setters

    #pragma region ValueDefines
	//Stored inside Actual MediumDec class to prevent deriving statics
    #pragma endregion ValueDefines

    #pragma region String Commands
        /// <summary>
        /// Reads the string.
        /// </summary>
        /// <param name="Value">The value.</param>
        virtual void ReadString(const std::string& Value);

        /// <summary>
        /// Gets the value from string.
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        template<MediumDecVariant VariantType=PartialDec>
        VariantType GetValueFromString(std::string Value)
        {

        }

        /// <summary>
        /// Initializes a new instance of the <see cref="PartialDec"/> class from string literal
        /// </summary>
        /// <param name="strVal">The value.</param>
        PartialDec(const char* strVal)
        {
            std::string Value = strVal;
            if (Value == "Pi")
            {
                this->SetVal(Pi);
            }
            else if (Value == "E")
            {
                this->SetVal(E);
            }
            else
            {
                this->ReadString(Value);
            }
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="PartialDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        PartialDec(const std::string& Value)
        {
            this->ReadString(Value);
        }

//private:
        //std::string BasicToStringOp();
		
		//std::string BasicToFullStringOp();
public:

        /// <summary>
        /// Converts to string.
        /// </summary>
        /// <returns>std.string</returns>
        virtual std::string ToString();

        /// <summary>
        /// Converts to string with digits filled in even when empty
        /// </summary>
        /// <returns>std.string</returns>
        virtual std::string ToFullString();

        /// <summary>
        /// Implements the operator std::string operator.
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator std::string() { return ToString(); }
    #pragma endregion String Commands

    #pragma region ConvertFromOtherTypes
		//To-Do: Add more exact conversion from floating point format to MediumDec variant
	
        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        virtual void SetFloatVal(const float& Value)
        {
	#if defined(AltNum_UseLegacyFloatingConversion)
			float lValue = Value;
            bool IsNegative = Value < 0.0f;
            if (IsNegative) { lValue *= -1.0f; }
            //Cap value if too big on initialize (preventing overflow on conversion)
            if (Value >= 2147483648.0f)
            {
                if (IsNegative)
		    #if defined(AltNum_EnableMirroredSection)
					IntValue = MirroredInt(2147483647,0);
		    #else
					IntValue = -2147483647;
		    #endif
                else
                    IntValue = 2147483647;
                DecimalHalf = 999999999;
            }
            else
            {
                signed __int64 WholeValue = (signed __int64)std::floor(Value);
                lValue -= (float)WholeValue;
                DecimalHalf = (signed int)Value * 10000000000;
                if(DecimalHalf!=0)
                    IntValue = IsNegative ? -WholeValue: WholeValue;
                else
                    IntValue = IsNegative ? NegativeRep : 0;
            }
	#elif defined(AltNum_EnableMirroredSection)//Extract number from "2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format
			//To-Do:Add code here
	#endif
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        virtual void SetDoubleVal(const double& Value)
        {
	#if defined(AltNum_UseLegacyFloatingConversion)
			double lValue = Value;
            bool IsNegative = Value < 0.0;
            if (IsNegative) { lValue *= -1.0; }
            //Cap value if too big on initialize (preventing overflow on conversion)
            if (Value >= 2147483648.0)
            {
                if (IsNegative)
		    #if defined(AltNum_EnableMirroredSection)
					IntValue = MirroredInt(2147483647,0);
		    #else
					IntValue = -2147483647;
		    #endif
                else
                    IntValue = 2147483647;
                DecimalHalf = 999999999;
            }
            else
            {
                signed __int64 WholeValue = (signed __int64)std::floor(Value);
                lValue -= (double)WholeValue;
                DecimalHalf = (signed int)Value * 10000000000;
                if(DecimalHalf!=0)
                    IntValue = IsNegative ? -WholeValue: WholeValue;
                else
                    IntValue = IsNegative ? NegativeRep : 0;
            }
	#elif defined(AltNum_EnableMirroredSection)//Extract number from "2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format
			//To-Do:Add code here
	#endif
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        virtual void SetDecimalVal(const ldouble& Value)
        {
	#if defined(AltNum_UseLegacyFloatingConversion)
			ldouble lValue = Value;
            bool IsNegative = Value < 0.0L;
            if (IsNegative) { lValue *= -1.0L; }
            //Cap value if too big on initialize (preventing overflow on conversion)
            if (lValue >= 2147483648.0L)
            {
                if (IsNegative)
		    #if defined(AltNum_EnableMirroredSection)
					IntValue = MirroredInt(2147483647,0);
		    #else
					IntValue = -2147483647;
		    #endif
                else
                    IntValue = 2147483647;
                DecimalHalf = 999999999;
            }
            else
            {
                signed __int64 WholeValue = (signed __int64)std::floor(lValue);
                lValue -= (ldouble)WholeValue;
                DecimalHalf = (signed int)lValue * 10000000000;
                if(DecimalHalf!=0)
                    IntValue = IsNegative ? -WholeValue: WholeValue;
                else
                    IntValue = IsNegative ? NegativeRep : 0;
            }
	#elif defined(AltNum_EnableMirroredSection)//Extract number from "2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format
			//To-Do:Add code here
	#endif
        }

        /// <summary>
        /// Sets the value(false equals zero; otherwise is true).
        /// </summary>
        /// <param name="Value">The value.</param>
        virtual void SetBoolVal(const bool& Value)
        {
            IntValue = Value==false ? 0 : 1;
            DecimalHalf = 0;
        }

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        virtual void SetIntVal(const int& Value)
        {
	#if defined(AltNum_EnableMirroredSection)
			if(Value<0)
			{
				IntValue.IsPositive = 0;
				IntValue.Value = -Value;
			}
			else
	#else
				IntValue = Value;
	#endif
			DecimalHalf = 0;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="PartialDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        PartialDec(const float& Value)
        {
            this->SetFloatVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="PartialDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        PartialDec(const double& Value)
        {
            this->SetDoubleVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="PartialDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        PartialDec(const ldouble& Value)
        {
            this->SetDecimalVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="PartialDec"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        PartialDec(const bool& Value)
        {
            this->SetBoolVal(Value);
        }

#if defined(AltNum_EnablePartialDecBasedSetValues)
        PartialDec(const PartialDec& Value)
        {
            this->SetVal(Value);
        }
#endif
    #pragma endregion ConvertFromOtherTypes

    #pragma region ConvertToOtherTypes
protected://Adding more exact conversion from floating point to PartialDec variant later

        /// <summary>
        /// MediumDec Variant to float explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        void float toFloatV1()
        {
	#if defined(AltNum_UseLegacyFloatingConversion)
            float Value;
            if (IntValue.IsNegative())
            {
        #if !defined(AltNum_EnableMirroredSection)
                Value = IntValue == NegativeRep ? 0.0f : (float)IntValue;
        #else
                Value = (float)-IntValue;
        #endif
                if (DecimalHalf != 0) { Value -= ((float)DecimalHalf * 0.000000001f); }
            }
            else
            {
        #if !defined(AltNum_EnableMirroredSection)
                Value = (float)IntValue;
        #else
                Value = (float)IntValue.Value;
        #endif
                if (DecimalHalf != 0) { Value += ((float)DecimalHalf * 0.000000001f); }
            }
            return Value;
	#elif defined(AltNum_EnableMirroredSection)//Convert number to "2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format
			if(IntValue.Value==0)//Exponent is negative
			{
				//To-Do:Add code here
			}
			else
			{
				//To-Do:Add code here
			}
			return 0.0f;//Placeholder
	#endif
        }

        /// <summary>
        /// MediumDec Variant to double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        void double toDoubleV1()
        {
	#if defined(AltNum_UseLegacyFloatingConversion)
		    double Value;
            if (IntValue < 0)
            {
        #if !defined(AltNum_EnableMirroredSection)
                Value = IntValue == NegativeRep ? 0.0f : (double)IntValue;
        #else
                Value = (double)-IntValue;
        #endif
                if (DecimalHalf != 0) { Value -= ((double)DecimalHalf * 0.000000001); }
            }
            else
            {
        #if !defined(AltNum_EnableMirroredSection)
                Value = (double)IntValue;
        #else
                Value = (double)IntValue.Value;
        #endif
                if (DecimalHalf != 0) { Value += ((double)DecimalHalf * 0.000000001); }
            }
            return Value;
	#elif defined(AltNum_EnableMirroredSection)//Convert number to "2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format
			if(IntValue.Value==0)//Exponent is negative
			{
				//To-Do:Add code here
			}
			else
			{
				//To-Do:Add code here
			}
			return 0.0;//Placeholder
	#endif
        }

        /// <summary>
        /// MediumDec Variant to long double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        void ldouble toDecimalV1()
        {
	#if defined(AltNum_UseLegacyFloatingConversion)
            ldouble Value;
            if (IntValue < 0)
            {
        #if !defined(AltNum_EnableMirroredSection)
                Value = IntValue == NegativeRep ? 0.0L : (float)IntValue;
        #else
                Value = (ldouble)-IntValue;
        #endif
                if (DecimalHalf != 0) { Value -= ((ldouble)DecimalHalf * 0.000000001L); }
            }
            else
            {
        #if !defined(AltNum_EnableMirroredSection)
                Value = (ldouble)IntValue;
        #else
                Value = (ldouble)IntValue.Value;
        #endif
                if (DecimalHalf != 0) { Value += ((ldouble)DecimalHalf * 0.000000001L); }
            }
            return Value;
	#elif defined(AltNum_EnableMirroredSection)//Convert number to "2^Exp + SignifNum*(2^(Exp - DenomMaxExp))" format
			if(IntValue.Value==0)//Exponent is negative
			{
				//To-Do:Add code here
			}
			else
			{
				//To-Do:Add code here
			}
			return 0.0L;//Placeholder
	#endif
        }

public:
        /// <summary>
        /// MediumDec Variant to float explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        virtual float toFloat()
        {
            return toFloatV1;
        }

        /// <summary>
        /// MediumDec Variant to double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        virtual double toDouble()
        {
            return toDoubleV1();
        }

        /// <summary>
        /// MediumDec Variant to long double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        virtual double toDecimal()
        {
            return toDecimalV1;
        }
		
        /// <summary>
        /// MediumDec Variant to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        virtual int toInt() { return IntValue.GetValue(); }

        virtual bool toBool() { return IntValue.IsZero() ? false : true; }

        /// <summary>
        /// MediumDec Variant to float explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator float() { return toFloatV1(); }
		
        /// <summary>
        /// MediumDec Variant to double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator double() { return toDoubleV1(); }
		
        /// <summary>
        /// MediumDec Variant to decimal explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator ldouble() { return toDecimalV1(); }

        /// <summary>
        /// MediumDec Variant to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator int() { return toInt(); }

        /// <summary>
        /// MediumDec Variant to bool explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        explicit operator bool() { return toBool(); }

    #pragma endregion ConvertToOtherTypes

    #pragma region Pi Conversion
	//Not used for this variant
    #pragma endregion Pi Conversion

    #pragma region E Conversion
	//Not used for this variant
    #pragma endregion E Conversion
	
    #pragma region Other RepType Conversion
    //Functions only used in variants of MediumDec

/*      //To-Do: Test cost of making virtual vs alternative means of making sure uses updated code after derivation    

        virtual void ConvertToNormType(const RepType& repType){}

protected:
		//Returns value as normal type representation
        template<MediumDecVariant VariantType=PartialDec>
        VariantType ConvertAsNormTypeV1(const RepType& repType)
        {
            VariantType Res = *this;
            Res.ConvertToNormType(repType);
            return Res;
        }
*/
		
public:
/*
		//Returns value as normal type representation
        constexpr auto ConvertAsNormType = PartialDec::ConvertAsNormTypeV1<PartialDec>;

        //Converts value to normal type representation
        void ConvertToNormTypeV2()
        {
            RepType repType = GetRepType();
            ConvertToNormType(repType);
        }
*/

protected:
		//Returns value as normal type representation
        template<MediumDecVariant VariantType=PartialDec>
        VariantType AutoConvertAsNormType()
        {
            VariantType Res = *this;
            Res.ConvertToNormTypeV2();
            return Res;
        }
public:
/*
		//Returns value as normal type representation
        constexpr auto ConvertAsNormTypeV2 = PartialDec::AutoConvertAsNormType<PartialDec>;
*/
    #pragma endregion Other RepType Conversion
	
    #pragma region Comparison Operators
protected:
		//Compare only as if in NormalType representation mode
		template<MediumDecVariant VariantType=PartialDec>
		std::strong_ordering BasicComparisonV1(const VariantType& that) const
		{
	#if defined(AltNum_EnableMirroredSection)
			//Comparing if number is negative vs positive
			if (auto SignCmp = IntValue.IsPositive <=> that.IntValue.IsPositive; SignCmp != 0)
				return SignCmp;
			if (auto IntHalfCmp = IntValue <=> that.IntValue; IntHalfCmp != 0)
				return IntHalfCmp;
			int lVal; int rVal;
	#else
			int lVal = IntValue==NegativeZero?0:IntValue;
			int rVal = that.IntValue==NegativeZero?0:that.IntValue;
	#endif
			if (auto IntHalfCmp = lVal <=> rVal; IntHalfCmp != 0)
				return IntHalfCmp;
			//Counting negative zero as same as zero IntValue but with negative DecimalHalf
	#if defined(AltNum_UseIntForDecimalHalf)
			lVal = IsNegative()?0-DecimalHalf:DecimalHalf;
			rVal = that.IsNegative()?0-that.DecimalHalf:that.DecimalHalf;
	#else
			lVal = IsNegative()?0-DecimalHalf.Value:DecimalHalf.Value;
			rVal = IsNegative()?0-that.DecimalHalf.Value:that.DecimalHalf.Value;
	#endif
			if (auto DecimalHalfCmp = lVal <=> rVal; DecimalHalfCmp != 0)
				return DecimalHalfCmp;
		}
		
#if defined(AltNum_EnableMirroredSection)
		//Compare only as if in NormalType representation mode ignoring sign(check before using)
		template<MediumDecVariant VariantType=PartialDec>
		std::strong_ordering BasicComparisonWithoutSignCheck(const VariantType& that) const
		{
			if (auto IntHalfCmp = IntValue <=> that.IntValue; IntHalfCmp != 0)
				return IntHalfCmp;

			if (auto IntHalfCmp = lVal <=> rVal; IntHalfCmp != 0)
				return IntHalfCmp;
			//Counting negative zero as same as zero IntValue but with negative DecimalHalf
	#if defined(AltNum_UseIntForDecimalHalf)
			int lVal = IsNegative()?0-DecimalHalf:DecimalHalf;
			int rVal = that.IsNegative()?0-that.DecimalHalf:that.DecimalHalf;
	#else
			int lVal = IsNegative()?0-DecimalHalf.Value:DecimalHalf.Value;
			int rVal = IsNegative()?0-that.DecimalHalf.Value:that.DecimalHalf.Value;
	#endif
			if (auto DecimalHalfCmp = lVal <=> rVal; DecimalHalfCmp != 0)
				return DecimalHalfCmp;
		}
#endif
	
		//Compare only as if in NormalType representation mode
        constexpr auto BasicComparison = PartialDec::BasicComparisonV1<PartialDec>;
		
#if defined(AltNum_EnableMirroredSection)
		//Compare only as if in NormalType representation mode ignoring sign(check before using)
        constexpr auto BasicComparisonV2 = PartialDec::BasicComparisonWithoutSignCheck<PartialDec>;
#endif
		
		//Compare only as if in NormalType representation mode
		std::strong_ordering BasicIntComparison(const int& that) const
		{
	#if defined(AltNum_EnableMirroredSection)
			//Comparing if number is negative vs positive
			if (auto SignCmp = IntValue.IsPositive <=> that.IntValue.IsPositive; SignCmp != 0)
				return SignCmp;
			if (auto IntHalfCmp = IntValue <=> that; IntHalfCmp != 0)
				return IntHalfCmp;
			int lVal;
	#else
			int lVal = IntValue==NegativeZero?0:IntValue;
	#endif
			if (auto IntHalfCmp = lVal <=> that; IntHalfCmp != 0)
				return IntHalfCmp;
			//Counting negative zero as same as zero IntValue but with negative DecimalHalf
	#if defined(AltNum_UseIntForDecimalHalf)
			lVal = DecimalHalf>0?1:0;
	#else
			lVal = DecimalHalf.Value>0?1:0;
	#endif
			if (auto DecimalHalfCmp = lVal <=> 0; DecimalHalfCmp != 0)
				return DecimalHalfCmp;
		}
		
#if defined(AltNum_EnableMirroredSection)
		//Compare only as if in NormalType representation mode ignoring sign(check before using)
		std::strong_ordering BasicIntComparisonV2(const int& that) const
		{
			if (auto IntHalfCmp = IntValue <=> that; IntHalfCmp != 0)
				return IntHalfCmp;
			//Counting negative zero as same as zero IntValue but with negative DecimalHalf
	#if defined(AltNum_UseIntForDecimalHalf)
			int lVal = DecimalHalf>0?1:0;
	#else
			int lVal = DecimalHalf.Value>0?1:0;
	#endif
			if (auto DecimalHalfCmp = lVal <=> 0; DecimalHalfCmp != 0)
				return DecimalHalfCmp;
		}
#endif
	
public:

		std::strong_ordering operator<=>(const PartialDec& that) const
		{
			return BasicComparison(that);
		}

		std::strong_ordering operator<=>(const int& that) const
		{
			return BasicIntComparison(that);
		}

		bool operator==(const int& that) const
		{
			if (IntValue!=that)
				return false;
			if (DecimalHalf!=0)
				return false;
			return true;
		}

		bool operator==(const PartialDec& that) const
		{
			if (IntValue!=that.IntValue)
				return false;
			if (DecimalHalf!=that.IntValue)
				return false;
		}
    #pragma endregion Comparison Operators

    #pragma region NormalRep Integer Division Operations
protected:
        template<IntegerType IntType=unsigned int>
        void PartialUIntDivOp(const IntType& rValue)
        {//Avoid using with special status representations such as approaching zero or result will be incorrect
#if !defined(AltNum_EnableMirroredSection)
            bool ResIsNegative = IntValue < 0;
#endif
            unsigned _int64 SelfRes;
            unsigned _int64 Res;
            unsigned _int64 IntHalfRes;
            unsigned _int64 DecimalRes;
#if !defined(AltNum_EnableMirroredSection)
            if(DecimalHalf == 0)
                SelfRes = ResIsNegative?NegDecimalOverflowX * IntValue:DecimalOverflowX * IntValue;
            else
            {
                if (ResIsNegative)
                {
                    if(IntValue==NegativeRep)
    #if defined(AltNum_UseIntForDecimalHalf)
                        SelfRes = DecimalHalf;
    #else
                        SelfRes = DecimalHalf.Value;
    #endif
                    else
    #if defined(AltNum_UseIntForDecimalHalf)
                        SelfRes = NegDecimalOverflowX * IntValue + DecimalHalf;
    #else
                        SelfRes = NegDecimalOverflowX * IntValue + DecimalHalf.Value;
    #endif
                }
                else
    #if defined(AltNum_UseIntForDecimalHalf)
                    SelfRes = DecimalOverflowX * IntValue + DecimalHalf;
    #else
                    SelfRes = DecimalOverflowX * IntValue + DecimalHalf.Value;
    #endif
            }
#else
    #if defined(AltNum_UseIntForDecimalHalf)
            SelfRes = DecimalHalf == 0? DecimalOverflowX * IntValue.Value: DecimalOverflowX * IntValue.Value + DecimalHalf;
    #else
            SelfRes = DecimalHalf == 0? DecimalOverflowX * IntValue.Value: DecimalOverflowX * IntValue.Value + DecimalHalf.Value;
    #endif
#endif
            Res = SelfRes / rValue;
            IntHalfRes = Res/DecimalOverflowX;
            DecimalRes = Res - DecimalOverflowX * IntHalfRes;
#if !defined(AltNum_EnableMirroredSection)
			if(ResIsNegative)
			{
				IntValue = IntHalfRes==0? NegativeRep: (int)(-IntHalfRes);
				DecimalHalf = (int) DecimalRes;
			}
			else
			{
				IntValue = (int)IntHalfRes;
				DecimalHalf = DecimalRes;
			}
#else
		    IntValue.Value = (unsigned int)IntHalfRes;
    #if defined(AltNum_UseIntForDecimalHalf)
			DecimalHalf = DecimalRes;
    #else
			DecimalHalf.Value = DecimalRes;
    #endif
#endif
        }

        template<typename IntType=int>
        void PartialIntDivOp(const IntType& Value)
        {
            if(Value<0)
            {
#if defined(AltNum_EnableMirroredSection)
                SwapNegativeStatus();
#endif
                PartialUIntDivOp(-Value);
            }
            else
                PartialUIntDivOp(Value);
        }

protected:
        template<MediumDecVariant VariantType=PartialDec, typename IntType>
        VariantType& BasicUIntDivOpV1(IntType& Value)
        {
            if (Value == 0)
            {
                throw "Target value can not be divided by zero";
            }
            else if (IsZero())
                return;
            PartialUIntDivOp(Value);
            if (IntValue == 0 && DecimalHalf == 0) { DecimalHalf = 1; }//Prevent Dividing into nothing
            return *this;
        }
		
        template<MediumDecVariant VariantType=PartialDec, typename IntType>
        VariantType& BasicIntDivOpV1(IntType& Value)
        {
            if (Value == 0)
            {
                throw "Target value can not be divided by zero";
            }
            else if (IsZero())
                return;
            PartialIntDivOp(Value);
            if (IntValue == 0 && DecimalHalf == 0) { DecimalHalf = 1; }//Prevent Dividing into nothing
            return *this;
        }
public:
/*
        constexpr auto BasicUIntDivOp = BasicUIntDivOpV1<PartialDec, IntType>;
		
        constexpr auto BasicIntDivOp = BasicIntDivOpV1<PartialDec, IntType>;

		virtual void BasicDivOp(signed int& Value) { BasicIntDivOp(Value); }
		virtual void BasicDivOp(unsigned int& Value) { BasicUnsignedIntDivOp(Value); }
		virtual void BasicDivOp(signed long long& Value) { BasicIntDivOp(Value); }
        virtual void BasicDivOp(unsigned long long& Value) { BasicUnsignedIntDivOp(Value); }
*/
    #pragma endregion NormalRep Integer Division Operations
		
    #pragma region NormalRep Integer Multiplication Operations
protected:
        template<typename IntType=int>
        void PartialUIntMultOp(const IntType& Value)
        {
			//Update this code
		}
		
        template<typename IntType=int>
        void PartialIntMultOp(const IntType& Value)
        {
            if(Value<0)
            {
#if defined(AltNum_EnableMirroredSection)
                SwapNegativeStatus();
#endif
                PartialUIntMultOp(-Value);
            }
            else
                PartialUIntMultOp(Value);
        }
		
protected:
        template<MediumDecVariant VariantType=PartialDec, typename IntType>
        VariantType& BasicUIntMultOpV1(const IntType& Value)
        {
            if (Value == 0)
            {
                SetAsZero();
                return *this;
            }
            else if (IsZero())
                return *this;
            PartialUIntMultOp(Value);
            return *this;
        }
		
        template<MediumDecVariant VariantType=PartialDec, typename IntType>
        VariantType& BasicIntMultOpV1(const IntType& Value)
        {
            if (Value == 0)
            {
                SetAsZero();
                return *this;
            }
            else if (IsZero())
                return *this;
			PartialIntMultOp(Value);
            return *this;
        }
public:
/*
        constexpr auto BasicUIntMultOp = BasicUIntMultOpV1<PartialDec, const unsigned int&>;
		
        constexpr auto BasicIntMultOp = BasicIntMultOpV1<PartialDec, const signed int&>;

        constexpr auto BasicUInt64MultOp = BasicUIntMultOpV1<PartialDec, const unsigned long long&>;
		
        constexpr auto BasicInt64MultOp = BasicIntMultOpV1<PartialDec, const signed long long&>;
*/
		
    #pragma endregion NormalRep Integer Multiplication Operations

	#pragma region NormalRep Integer Addition Operations
protected:
	
public:
/*
        constexpr auto BasicUIntAddOp = BasicUIntAddOpV1<PartialDec, const unsigned int&>;
		
        constexpr auto BasicIntAddOp = BasicIntAddOpV1<PartialDec, const signed int&>;

        constexpr auto BasicUInt64AddOp = BasicUIntAddOpV1<PartialDec, const unsigned long long&>;
		
        constexpr auto BasicInt64AddOp = BasicIntAddOpV1<PartialDec, const signed long long&>;
*/
	#pragma endregion NormalRep Integer Addition Operations

	#pragma region NormalRep Integer Subtraction Operations
protected:
	
public:
/*
        constexpr auto BasicUIntSubOp = BasicUIntSubOpV1<PartialDec, const unsigned int&>;
		
        constexpr auto BasicIntSubOp = BasicIntSubOpV1<PartialDec, const signed int&>;

        constexpr auto BasicUInt64SubOp = BasicUIntSubOpV1<PartialDec, const unsigned long long&>;
		
        constexpr auto BasicInt64SubOp = BasicIntSubOpV1<PartialDec, const signed long long&>;
*/
	#pragma endregion NormalRep Integer Subtraction Operations

	#pragma region NormalRep AltNum Division Operations
protected:
	
public:
/*
        constexpr auto BasicDivOp = BasicDivOpV1<PartialDec>;
*/
	#pragma endregion NormalRep AltNum Division Operations

	#pragma region NormalRep AltNum Multiplication Operations
protected:
		/// <summary>
        /// Basic Multiplication Operation that ignores special decimal status with unsigned PartialDec
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        template<MediumDecVariant VariantType=PartialDec, typename IntType>
        VariantType& BasicUnsignedMultOpV1(const PartialDec& rValue)
		{//To-Do:Update this code more towards current default format
            if (DecimalHalf == 0)
            {
                if (IntValue == 1)
                {
                    IntValue = rValue.IntValue; DecimalHalf = rValue.DecimalHalf;
                }
                else if (rValue.DecimalHalf == 0)
                {
                    IntValue *= rValue.IntValue;
                }
                else
                {
                    __int64 rRep = rValue.IntValue == 0 ? rValue.DecimalHalf : DecimalOverflowX * rValue.IntValue + rValue.DecimalHalf;
                    bool ResIsNegative = IntValue<0;
                    if(ResIsNegative)
                    {
                        ResIsNegative = false;
                        rRep *= IntHalfAsAbs();
                    }
                    else
                    {
                        ResIsNegative = true;
                        rRep *= IntValue;
                    }
                    if (rRep >= DecimalOverflowX)
                    {
                        __int64 OverflowVal = rRep / DecimalOverflowX;
                        rRep -= OverflowVal * DecimalOverflowX;
                        IntValue = (signed int)ResIsNegative ? OverflowVal * -1 : OverflowVal;
                        DecimalHalf = (signed int)rRep;
                        return *this;
                    }
                    else
                    {
                        DecimalHalf = (signed int)rRep;
                        if(DecimalHalf==0)
#if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
                            DecimalHalf = 1;
#endif
                        IntValue = ResIsNegative ? NegativeRep : 0;
                        return *this;
                    }
                }
            }
            else if (IntValue == 0)
            {
                __int64 SRep = (__int64)DecimalHalf;
                SRep *= rValue.DecimalHalf;
                SRep /= PartialDec::DecimalOverflowX;
                if (rValue.IntValue == 0)
                {
                    DecimalHalf = (signed int)SRep;
                #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
                    if (DecimalHalf == 0)
                        DecimalHalf = 1;
                #endif
                    return *this;
                }
                else
                {
                    SRep += (__int64)DecimalHalf * rValue.IntValue;
                    if (SRep >= PartialDec::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / PartialDec::DecimalOverflowX;
                        SRep -= OverflowVal * PartialDec::DecimalOverflowX;
                        IntValue = OverflowVal;
                        DecimalHalf = (signed int)SRep;
						return *this;
                    }
                    else
                    {
                        DecimalHalf = (signed int)SRep;
                #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
                        if(DecimalHalf==0)
                            DecimalHalf = 1;
                #endif
                        return *this;
                    }
                }
            }
            else
            {
                if (rValue.DecimalHalf == 0)//Y is integer
                {
                    __int64 SRep = PartialDec::DecimalOverflowX * IntValue + DecimalHalf;
                    SRep *= rValue.IntValue;
                    if (SRep >= PartialDec::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / PartialDec::DecimalOverflowX;
                        SRep -= OverflowVal * PartialDec::DecimalOverflowX;
                        IntValue = (signed int)OverflowVal;
                        DecimalHalf = (signed int)SRep;
                    }
                    else
                    {
                        DecimalHalf = (signed int)SRep;
                        if(DecimalHalf==0)
                        {
                #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
                            if(DecimalHalf==0)
                                DecimalHalf = 1;
                #endif
                        }
                        IntValue = SelfIsNegative ? PartialDec::NegativeRep : 0;
                    }
				    return *this;
                }
                else if (rValue.IntValue == 0)
                {
                    __int64 SRep = PartialDec::DecimalOverflowX * IntValue + DecimalHalf;
                    SRep *= rValue.DecimalHalf;
                    SRep /= PartialDec::DecimalOverflowX;
                    if (SRep >= PartialDec::DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / PartialDec::DecimalOverflowX;
                        SRep -= OverflowVal * PartialDec::DecimalOverflowX;
                        IntValue = (signed int)(SelfIsNegative ? -OverflowVal : OverflowVal);
                        DecimalHalf = (signed int)SRep;
                    }
                    else
                    {
                        DecimalHalf = (signed int)SRep;
                        if(DecimalHalf==0)
                        {
                #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
                            if(DecimalHalf==0)
                                DecimalHalf = 1;
                #endif
                        }
                        IntValue = SelfIsNegative ? PartialDec::NegativeRep : 0;
                    }
                    return *this;
                }
                else
                {
                    //X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                    __int64 SRep = IntValue == 0 ? DecimalHalf : PartialDec::DecimalOverflowX * IntValue + DecimalHalf;
                    SRep *= rValue.IntValue;//SRep holds __int64 version of X.Y * Z
                    //X.Y *.V
                    __int64 Temp03 = (__int64)(rValue.DecimalHalf * IntValue);//Temp03 holds __int64 version of X *.V
                    __int64 Temp04 = (__int64)DecimalHalf * (__int64)rValue.DecimalHalf;
                    Temp04 /= PartialDec::DecimalOverflow;
                    //Temp04 holds __int64 version of .Y * .V
                    __int64 IntegerRep = SRep + Temp03 + Temp04;
                    __int64 IntHalf = IntegerRep / PartialDec::DecimalOverflow;
                    IntegerRep -= IntHalf * (__int64)PartialDec::DecimalOverflow;
                    if (IntHalf == 0)
                        IntValue = (signed int)SelfIsNegative ? PartialDec::NegativeRep : 0;
                    else
                        IntValue = (signed int)SelfIsNegative ? IntHalf * -1 : IntHalf;
                    DecimalHalf = (signed int)IntegerRep;
                }
            }
#if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
            if(DecimalHalf==0&&IntValue==0)
                DecimalHalf = 1;
#endif
            return *this;
		}
public:
/*

        constexpr auto BasicMultOp = BasicMultOpV1<PartialDec>;
*/
	#pragma endregion NormalRep AltNum Multiplication Operations

    #pragma region NormalRep AltNum Addition Operations
protected:
	
public:
/*
        constexpr auto BasicAddOp = BasicAddOpV1<PartialDec>;
*/
	#pragma endregion NormalRep AltNum Addition Operations

	#pragma region NormalRep AltNum Subtraction Operations
protected:
	
public:
/*
        constexpr auto BasicSubOp = BasicSubOpV1<PartialDec>;
*/
	#pragma endregion NormalRep AltNum Subtraction Operations

	#pragma region Mixed Fraction Operations
	//Used only in AltDec/MixedDec
	#pragma endregion Mixed Fraction Operations

	#pragma region Other Division Operations

	#pragma endregion Other Division Operations	

	#pragma region Other Multiplication Operations

	#pragma endregion Other Multiplication Operations

	#pragma region Other Addition Operations

	#pragma endregion Other Addition Operations

	#pragma region Other Subtraction Operations

	#pragma endregion Other Subtraction Operations

	#pragma region Modulus Operations
	//Not supported at moment
	#pragma endregion Modulus Operations

	#pragma region Bitwise Operations
	//Not supported at moment
	#pragma endregion Bitwise Operations

    #pragma region Main Operator Overrides
        /// <summary>
        /// Division Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec operator/(PartialDec self, PartialDec Value) { return DivOp(self, Value); }

        /// <summary>
        /// /= Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec& operator/=(PartialDec& self, PartialDec Value) { return DivOp(self, Value); }

        /// <summary>
        /// Multiplication Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec operator*(PartialDec self, PartialDec Value) { return MultOp(self, Value); }

        ///// <summary>
        ///// *= Operation
        ///// </summary>
        ///// <param name="self">The self.</param>
        ///// <param name="Value">The value.</param>
        ///// <returns>PartialDec</returns>
        friend PartialDec& operator*=(PartialDec& self, PartialDec Value) { return MultOp(self, Value); }

        /// <summary>
        /// Addition Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec operator+(PartialDec self, PartialDec Value) { return AddOp(self, Value); }

        /// <summary>
        /// += Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec& operator+=(PartialDec& self, PartialDec Value) { return AddOp(self, Value); }

        /// <summary>
        /// Subtraction Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec operator-(PartialDec self, PartialDec Value) { return SubOp(self, Value); }

        /// <summary>
        /// -= Operation
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec& operator-=(PartialDec& self, PartialDec Value) { return SubOp(self, Value); }

        /// <summary>
        /// Addition Operation Between PartialDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec operator+(PartialDec self, int Value) { return IntAddOp(self, Value); }

        ///// <summary>
        ///// += Operation Between PartialDec and Integer Value
        ///// </summary>
        ///// <param name="self">The self.</param>
        ///// <param name="Value">The value.</param>
        ///// <returns>PartialDec</returns>
        template<typename IntType>
        friend PartialDec& operator+=(PartialDec& self, int Value) { return IntAddOp(self, Value); }

        //friend PartialDec operator+=(PartialDec* self, int Value) { return IntAddOp(**self, Value); }

        /// <summary>
        /// Subtraction Operation Between PartialDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec operator-(PartialDec self, int Value) { return IntSubOp(self, Value); }

        /// <summary>
        /// -= Operation Between PartialDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec& operator-=(PartialDec& self, int Value) { return IntSubOp(self, Value); }

        /// <summary>
        /// Multiplication Operation Between PartialDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec operator*(PartialDec self, int Value) { return IntMultOp(self, Value); }

        /// <summary>
        /// *= Operation Between PartialDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        template<typename IntType>
        friend PartialDec& operator*=(PartialDec& self, int Value) { return IntMultOp(self, Value); }

        /// <summary>
        /// Division Operation Between PartialDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec operator/(PartialDec self, int Value) { return IntDivOp(self, Value); }

        /// <summary>
        /// /= Operation Between PartialDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec& operator/=(PartialDec& self, int Value) { return IntDivOp(self, Value); }

        /// <summary>
        /// Addition Operation Between PartialDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec operator+(PartialDec self, signed long long Value) { return IntAddOp(self, Value); }

        ///// <summary>
        ///// += Operation Between PartialDec and Integer Value
        ///// </summary>
        ///// <param name="self">The self.</param>
        ///// <param name="Value">The value.</param>
        ///// <returns>PartialDec</returns>
        template<typename IntType>
        friend PartialDec& operator+=(PartialDec& self, signed long long Value) { return IntAddOp(self, Value); }

        /// <summary>
        /// Subtraction Operation Between PartialDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec operator-(PartialDec self, signed long long Value) { return IntSubOp(self, Value); }

        /// <summary>
        /// -= Operation Between PartialDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec& operator-=(PartialDec& self, signed long long Value) { return IntSubOp(self, Value); }

        /// <summary>
        /// Multiplication Operation Between PartialDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec operator*(PartialDec self, signed long long Value) { return IntMultOp(self, Value); }

        /// <summary>
        /// *= Operation Between PartialDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        template<typename IntType>
        friend PartialDec operator*=(PartialDec& self, signed long long Value) { return IntMultOp(self, Value); }

        /// <summary>
        /// Division Operation Between PartialDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec operator/(PartialDec self, signed long long Value) { return IntDivOp(self, Value); }

        /// <summary>
        /// /= Operation Between PartialDec and Integer Value
        /// </summary>
        /// <param name="self">The self.</param>
        /// <param name="Value">The value.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec operator/=(PartialDec& self, signed long long Value) { return IntDivOp(self, Value); }
    
        friend PartialDec operator+(PartialDec self, float Value) { return self + (PartialDec)Value; }
        friend PartialDec operator-(PartialDec self, float Value) { return self - (PartialDec)Value; }
        friend PartialDec operator*(PartialDec self, float Value) { return self * (PartialDec)Value; }
        friend PartialDec operator/(PartialDec self, float Value) { return self / (PartialDec)Value; }

        friend PartialDec operator+(float Value, PartialDec self) { return (PartialDec)Value + self; }
        friend PartialDec operator-(float Value, PartialDec self) { return (PartialDec)Value - self; }
        friend PartialDec operator*(float Value, PartialDec self) { return (PartialDec)Value * self; }
        friend PartialDec operator/(float Value, PartialDec self) { return (PartialDec)Value / self; }

        friend PartialDec operator+(PartialDec self, double Value) { return self + (PartialDec)Value; }
        friend PartialDec operator-(PartialDec self, double Value) { return self - (PartialDec)Value; }
        friend PartialDec operator*(PartialDec self, double Value) { return self * (PartialDec)Value; }
        friend PartialDec operator/(PartialDec self, double Value) { return self / (PartialDec)Value; }

        friend PartialDec operator+(PartialDec self, ldouble Value) { return self + (PartialDec)Value; }
        friend PartialDec operator-(PartialDec self, ldouble Value) { return self - (PartialDec)Value; }
        friend PartialDec operator*(PartialDec self, ldouble Value) { return self * (PartialDec)Value; }
        friend PartialDec operator/(PartialDec self, ldouble Value) { return self / (PartialDec)Value; }

        friend PartialDec operator+(ldouble Value, PartialDec self) { return (PartialDec)Value + self; }
        friend PartialDec operator-(ldouble Value, PartialDec self) { return (PartialDec)Value - self; }
        friend PartialDec operator*(ldouble Value, PartialDec self) { return (PartialDec)Value * self; }
        friend PartialDec operator/(ldouble Value, PartialDec self) { return (PartialDec)Value / self; }


        friend PartialDec operator+(PartialDec self, unsigned char Value) { return IntAddOp(self, Value); }
        friend PartialDec operator-(PartialDec self, unsigned char Value) { return IntSubOp(self, Value); }
        friend PartialDec operator*(PartialDec self, unsigned char Value) { return self.UnsignedIntMultOp(Value); }
        friend PartialDec operator/(PartialDec self, unsigned char Value) { return self.UnsignedIntDivOp(Value); }

        friend PartialDec operator+=(PartialDec& self, unsigned char Value) { return IntAddOp(self, Value); }
        friend PartialDec operator-=(PartialDec& self, unsigned char Value) { return IntSubOp(self, Value); }
        friend PartialDec operator*=(PartialDec& self, unsigned char Value) { return self.UnsignedIntMultOp(Value); }
        friend PartialDec operator/=(PartialDec& self, unsigned char Value) { return UnsignedDivOp(self, Value); }
        

        friend PartialDec operator+(PartialDec self, unsigned short Value) { return IntAddOp(self, Value); }
        friend PartialDec operator-(PartialDec self, unsigned short Value) { return IntSubOp(self, Value); }
        friend PartialDec operator*(PartialDec self, unsigned short Value) { return self.UnsignedIntMultOp(Value); }
        friend PartialDec operator/(PartialDec self, unsigned short Value) { return UnsignedDivOp(self, Value); }

        friend PartialDec operator+=(PartialDec& self, unsigned short Value) { return IntAddOp(self, Value); }
        friend PartialDec operator-=(PartialDec& self, unsigned short Value) { return IntSubOp(self, Value); }
        friend PartialDec operator*=(PartialDec& self, unsigned short Value) { return self.UnsignedIntMultOp(Value); }
        friend PartialDec operator/=(PartialDec& self, unsigned short Value) { return UnsignedDivOp(self, Value); } 

        friend PartialDec operator+(PartialDec self, unsigned int Value) { return IntAddOp(self, Value); }
        friend PartialDec operator-(PartialDec self, unsigned int Value) { return IntSubOp(self, Value); }
        friend PartialDec operator*(PartialDec self, unsigned int Value) { return self.UnsignedIntMultOp(Value); }
        friend PartialDec operator/(PartialDec self, unsigned int Value) { return UnsignedDivOp(self, Value); }
        

        friend PartialDec operator+=(PartialDec& self, unsigned int Value) { return IntAddOp(self, Value); }
        friend PartialDec operator-=(PartialDec& self, unsigned int Value) { return IntSubOp(self, Value); }
        friend PartialDec operator*=(PartialDec& self, unsigned int Value) { return self.UnsignedIntMultOp(Value); }
        friend PartialDec operator/=(PartialDec& self, unsigned int Value) { return UnsignedDivOp(self, Value); }
        
        friend PartialDec operator+(PartialDec self, unsigned __int64 Value) { return IntAddOp(self, Value); }
        friend PartialDec operator-(PartialDec self, unsigned __int64 Value) { return IntSubOp(self, Value); }
        friend PartialDec operator*(PartialDec self, unsigned __int64 Value) { return self.UnsignedIntMultOp(Value); }
        friend PartialDec operator/(PartialDec self, unsigned __int64 Value) { return UnsignedDivOp(self, Value); }

        friend PartialDec operator+=(PartialDec& self, unsigned __int64 Value) { return IntAddOp(self, Value); }
        friend PartialDec operator-=(PartialDec& self, unsigned __int64 Value) { return IntSubOp(self, Value); }
        friend PartialDec operator*=(PartialDec& self, unsigned __int64 Value) { return self.UnsignedIntMultOp(Value); }
        friend PartialDec operator/=(PartialDec& self, unsigned __int64 Value) { return UnsignedDivOp(self, Value); }

    #pragma endregion Main Operator Overrides

    #pragma endregion Other Operators
        /// <summary>
        /// Negative Unary Operator(Flips negative status)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <returns>PartialDec</returns>
        friend PartialDec& operator-(PartialDec& self)
        {
            return self;//Negative numbers not supported
        }

        /// <summary>
        /// ++PartialDec Operator
        /// </summary>
        /// <returns>PartialDec &</returns>
        PartialDec& operator ++()
        {
            ++IntValue;
            return *this;
        }

        /// <summary>
        /// ++PartialDec Operator
        /// </summary>
        /// <returns>PartialDec &</returns>
        PartialDec& operator --()
        {
			if(IntValue==0)
				throw "Can't reduce number into negative values";
            else
                --IntValue;
            return *this;
        }

        /// <summary>
        /// MediumDec Variant++ Operator
        /// </summary>
        /// <returns>PartialDec</returns>
        PartialDec operator ++(int)
        {
            PartialDec tmp(*this);
            ++* this;
            return tmp;
        }

        /// <summary>
        /// MediumDec Variant-- Operator
        /// </summary>
        /// <returns>PartialDec</returns>
        PartialDec operator --(int)
        {
            PartialDec tmp(*this);
            --* this;
            return tmp;
        }

        /// <summary>
        /// MediumDec Variant* Operator
        /// </summary>
        /// <returns>PartialDec &</returns>
        PartialDec& operator *()
        {
            return *this;
        }
    #pragma endregion Other Operators

	#pragma region Math Etc Functions
	//Not Supported at moment
	#pragma endregion Math Etc Functions

	#pragma region Pow and Sqrt Functions
	//Not Supported at moment
	#pragma endregion Pow and Sqrt Functions

	#pragma region Log Functions
protected:

public:
        /// <summary>
        /// Taylor Series Exponential function derived from https://www.pseudorandom.com/implementing-exp
        /// Does not modifier owner object
        /// </summary>
        /// <param name="x">The value to apply the exponential function to.</param>
        /// <returns>PartialDec</returns>
        auto ExpOf()
        {
            //x.ConvertToNormType();//Prevent losing imaginary number status
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
             *      - approximation of e^x in PartialDec precision
             */
             // Check that x is a valid input.
#if !defined(AltNum_EnableMirroredSection)
            assert(-709 < IntValue && IntValue < 709);
#else
            assert(IntValue.Value < 709);
#endif
            // When x = 0 we already know e^x = 1.
            if (IsZero()) {
                return One;
            }
            // Normalize x to a non-negative value to take advantage of
            // reciprocal symmetry. But keep track of the original sign
            // in case we need to return the reciprocal of e^x later.
            VariantType x0 = *this;
            x0.Abs();
            // First term of Taylor expansion of e^x at a = 0 is 1.
            // tn is the variable we we will return for e^x, and its
            // value at any time is the sum of all currently evaluated
            // Taylor terms thus far.
            VariantType tn = One;
            // Chose a truncation point for the Taylor series using the
            // heuristic bound 12 * ceil(|x| e), then work down from there
            // using Horner's method.
            int n = VariantType::CeilInt(x0 * E) * 12;
            for (int i = n; i > 0; --i) {
                tn = tn * (x0 / i) + One;
            }
            // If the original input x is less than 0, we want the reciprocal
            // of the e^x we calculated.
            if (x < 0) {
                tn = One / tn;
            }
            return tn;
        }

        /// <summary>
        /// Get the (n)th Root
        /// Code based mostly from https://rosettacode.org/wiki/Nth_root#C.23
        /// Does not modify owner object
        /// </summary>
        /// <param name="n">The n value to apply with root.</param>
        /// <returns></returns>
        auto NthRootOfV2(int n, const auto& Precision = FiveBillionth)
        {
            int nMinus1 = n - 1;
            auto x[2] = { (One / n) * ((*this*nMinus1) + (*this / PowOf(nMinus1))), targetValue };
            while (Abs(x[0] - x[1]) > Precision)
            {
                x[1] = x[0];
                x[0] = (One / n) * ((x[1]*nMinus1) + (targetValue / x[1].PowOf(nMinus1)));
            }
            return x[0];
        }
	#pragma endregion Log Functions

    #pragma region Trigonomic Etc Functions
	//Not Supported at moment
    #pragma endregion Trigonomic Etc Functions
    };

    #pragma region ValueDefine Source

    #pragma endregion ValueDefine Source

    #pragma region String Function Source
    /// <summary>
    /// Reads the string.
    /// </summary>
    /// <param name="Value">The value.</param>
    inline void MediumDec::ReadString(std::string Value)
    {
        IntValue = 0; DecimalHalf = 0;
	#if !defined(AltNum_EnableMirroredSection)
        bool IsNegative = false;
	#endif
        int PlaceNumber;
        std::string WholeNumberBuffer = "";
        std::string DecimalBuffer = "";

        bool ReadingDecimal = false;
        int TempInt;
        int TempInt02;
        for (char const& StringChar : Value)
        {
            if (VariableConversionFunctions::IsDigit(StringChar))
            {
                if (ReadingDecimal) { DecimalBuffer += StringChar; }
                else { WholeNumberBuffer += StringChar; }
            }
            else if (StringChar == '-')
            {
	#if !defined(AltNum_EnableMirroredSection)
                IsNegative = true;
	#else
				IntValue.IsPositive = 0;
	#endif
            }
            else if (StringChar == '.')
            {
                ReadingDecimal = true;
            }
            else if(StringChar!=' ')
                break;//Stop Extracting after encounter non-number character such as i
        }
        PlaceNumber = WholeNumberBuffer.length() - 1;
        for (char const& StringChar : WholeNumberBuffer)
        {
            TempInt = VariableConversionFunctions::CharAsInt(StringChar);
            TempInt02 = (TempInt * VariableConversionFunctions::PowerOfTens[PlaceNumber]);
            if (StringChar != '0')
            {
                IntValue += TempInt02;
            }
            PlaceNumber--;
        }
        PlaceNumber = 8;
        for (char const& StringChar : DecimalBuffer)
        {
            //Limit stored decimal numbers to the amount it can store
            if (PlaceNumber > -1)
            {
                TempInt = VariableConversionFunctions::CharAsInt(StringChar);
                TempInt02 = (TempInt * VariableConversionFunctions::PowerOfTens[PlaceNumber]);
                if (StringChar != '0')
                {
                    DecimalHalf += TempInt02;
                }
                PlaceNumber--;
            }
        }
	#if !defined(AltNum_EnableMirroredSection)
        if (IsNegative)
        {
            if (IntValue == 0) { IntValue = NegativeRep; }
            else { IntValue *= -1; }
        }
	#endif
    }

    /// <summary>
    /// Gets the value from string.
    /// </summary>
    /// <param name="Value">The value.</param>
    /// <returns>MediumDec</returns>
    inline MediumDec MediumDec::GetValueFromString(const std::string& Value)
    {
        MediumDec NewSelf = Zero;
        NewSelf.ReadString(Value);
        return NewSelf;
    }

    std::string MediumDec::ToString()
    {
        std::string Value = "";
        int CurrentSection = IntValue;
        unsigned __int8 CurrentDigit;
        std::string DecBuffer = "";
        if (IntValue < 0)
        {
            Value += "-";
            if (IntValue == NegativeRep) { CurrentSection = 0; }
            else { CurrentSection *= -1; }
        }
        for (__int8 Index = VariableConversionFunctions::NumberOfPlaces(CurrentSection); Index >= 0; Index--)
        {
            CurrentDigit = (unsigned __int8)(CurrentSection / VariableConversionFunctions::PowerOfTens[Index]);
            CurrentSection -= (signed int)(CurrentDigit * VariableConversionFunctions::PowerOfTens[Index]);
            Value += VariableConversionFunctions::DigitAsChar(CurrentDigit);
        }
        if (DecimalHalf != 0)
        {
            Value += ".";
            CurrentSection = DecimalHalf;
            for (__int8 Index = 8; Index >= 0; --Index)
            {
                CurrentDigit = (unsigned __int8)(CurrentSection / VariableConversionFunctions::PowerOfTens[Index]);
                CurrentSection -= (signed int)(CurrentDigit * VariableConversionFunctions::PowerOfTens[Index]);
                if (CurrentDigit != 0)
                {
                    if(!DecBuffer.empty())
                    {
                        Value += DecBuffer;
                        DecBuffer.clear();
                    }
                    Value += VariableConversionFunctions::DigitAsChar(CurrentDigit);
                }
                else
                {
                    DecBuffer += VariableConversionFunctions::DigitAsChar(CurrentDigit);
                }
            }
        }
        return Value;
    }

    std::string MediumDec::ToFullString()
    {
        std::string Value = "";
        int CurrentSection = IntValue;
        unsigned __int8 CurrentDigit;
        if (IntValue < 0)
        {
            Value += "-";
            if (IntValue == NegativeRep) { CurrentSection = 0; }
            else { CurrentSection *= -1; }
        }
        for (__int8 Index = VariableConversionFunctions::NumberOfPlaces(CurrentSection); Index >= 0; Index--)
        {
            CurrentDigit = (unsigned __int8)(CurrentSection / VariableConversionFunctions::PowerOfTens[Index]);
            CurrentSection -= (signed int)(CurrentDigit * VariableConversionFunctions::PowerOfTens[Index]);
            Value += VariableConversionFunctions::DigitAsChar(CurrentDigit);
        }
        if (DecimalHalf != 0)
        {
            Value += ".";
            bool HasDigitsUsed = false;
            CurrentSection = DecimalHalf;
            for (__int8 Index = 8; Index >= 0; --Index)
            {
                if (CurrentSection > 0)
                {
                    CurrentDigit = (unsigned __int8)(CurrentSection / VariableConversionFunctions::PowerOfTens[Index]);
                    CurrentSection -= (CurrentDigit * VariableConversionFunctions::PowerOfTens[Index]);
                    Value += VariableConversionFunctions::DigitAsChar(CurrentDigit);
                }
                else
                    Value += "0";
            }
        }
        else
        {
            Value += ".000000000";
        }
        return Value;
    }
    #pragma endregion String Function Source
}
}