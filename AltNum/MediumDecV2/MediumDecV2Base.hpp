// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#ifdef BlazesSharedCode_LocalLayout
#ifndef DLL_API
#ifdef UsingBlazesSharedCodeDLL
#define DLL_API __declspec(dllimport)
#elif defined(BLAZESSharedCode_LIBRARY)
#define DLL_API __declspec(dllexport)
#else
#define DLL_API
#endif
#endif
#else
#include "..\..\DLLAPI.h"
#endif

#include "..\MediumDec\MediumDecBase.hpp"
#include "..\MediumDec\MediumDec.hpp"

namespace BlazesRusCode
{
    class MediumDecV2Base;

    /// <summary>
    /// Separating functions that don't use static variables inside this base class for deriving
    /// Completed class inside MediumDec
	/// </summary>
    class DLL_API MediumDecV2Base : public virtual MediumDecBase
    {
	public:
        /// <summary>
        /// long double (Extended precision double)
        /// </summary>
        using ldouble = long double;

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecV2Base"/> class.
        /// </summary>
        /// <param name="intVal">The whole number based half of the representation</param>
        /// <param name="decVal01">The non-whole based half of the representation(and other special statuses)</param>
        MediumDecV2Base(const IntHalfType& intVal, const DecimalHalfType& decVal = 0)
        {
            IntValue = intVal;
            DecimalHalf = decVal;
        }

        MediumDecV2Base(const MediumDecV2Base&) = default;

        MediumDecV2Base& operator=(const int& rhs)
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

        MediumDecV2Base& operator=(const MediumDec& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            IntValue = rhs.IntValue; DecimalHalf = rhs.DecimalHalf;
            return *this;
        } const

        MediumDecV2Base& operator=(const MediumDecV2Base& rhs)
        {
            // Check for self-assignment
            if (this == &rhs)      // Same object?
                return *this;        // Yes, so skip assignment, and just return *this.
            IntValue = rhs.IntValue; DecimalHalf = rhs.DecimalHalf;
            return *this;
        } const

        //Is at either zero or negative zero IntHalf of AltNum
        constexpr auto IsAtZeroInt = MediumDecBase::IsAtZeroInt;

        //alias function
        constexpr auto IsNotAtZeroInt = MediumDecBase::IsNotAtZeroInt;

        //Detect if at exactly zero
        constexpr auto IsZero = MediumDecBase::IsZero;

/*
        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        template<MediumDecVariant VariantType=MediumDecBase>
        void SetVal(VariantType Value)
        {
            IntValue = Value.IntValue;
            DecimalHalf = Value.DecimalHalf;
        }*/


        constexpr auto SetAsZero = MediumDecBase::SetAsZero;

        /// <summary>
        /// Swaps the negative status.
        /// </summary>
        constexpr auto SwapNegativeStatus = MediumDecBase::SwapNegativeStatus;

    #pragma region Const Representation values
    protected:
	#if defined(AltNum_EnablePiRep) && defined(AltNum_UseIntForDecimalHalf)
        //Pi*Value representation(when DecimalHalf.Flag==1)
        static const unsigned int PiRep = 1;
	#endif
	#if defined(AltNum_EnableERep) && defined(AltNum_UseIntForDecimalHalf)
        //e*Value representation(DecimalHalf.Flag==2)
        static const unsigned int ERep = 2;
	#endif
	#if defined(AltNum_EnableImaginaryNum) && defined(AltNum_UseIntForDecimalHalf)
        //e*Value representation(DecimalHalf.Flag==3)
        static const unsigned int IRep = 3;
	#endif
	
	#if defined(AltNum_EnableInfinityRep)
        //When DecimalHalf.Value equals this value, it represents infinity (sign of IntValue determines if either negative or positive inifity)
		#if defined(AltNum_UseIntForDecimalHalf)
        static const signed int InfinityRep = -2147483648;
		#else
		static const unsigned int InfinityRep = 1073741824;
		#endif
	#endif
	#if defined(AltNum_EnableApproachingValues)
        //When DecimalHalf.Value equals this value, it represents Approaching IntValue from right towards left (IntValue.0..01)
		#if defined(AltNum_UseIntForDecimalHalf)
        static const signed int ApproachingBottomRep = -2147483647;
		#else
        static const unsigned int ApproachingBottomRep = 1073741823;
		#endif
        //When DecimalHalf.Value equals this value, it represents Approaching IntValue from left towards right (IntValue.9..9)
		#if defined(AltNum_UseIntForDecimalHalf)
		static const signed int ApproachingTopRep = -2147483646;
		#else
		static const unsigned int ApproachingTopRep = 1073741822;
		#endif
	#endif
	#if defined(AltNum_EnableUndefinedButInRange)
		//When DecimalHalf is at this value, than value is undefined but within real number range
		//Such as result of Cos of infinity
		//https://www.cuemath.com/trigonometry/domain-and-range-of-trigonometric-functions/
		#if defined(AltNum_UseIntForDecimalHalf)
        static const signed int UndefinedInRangeRep = -2147483645;
		#else
        static const unsigned int UndefinedInRangeRep = 1073741821;
		#endif
	#endif
	#if defined(AltNum_EnableNaN)
        //Is NaN when DecimalHalf is at this value
		#if defined(AltNum_UseIntForDecimalHalf)
        static const signed int NaNRep = 2147483647;
		#else
        static const signed int NaNRep = 1073741820;
		#endif
        //Is Undefined when DecimalHalf is at this value
		#if defined(AltNum_UseIntForDecimalHalf)
        static const signed int UndefinedRep = 2147483646;
		#else
        static const signed int UndefinedRep = 1073741819;
		#endif
	#endif
	#if defined(AltNum_EnableNil)
        //Is defined at empty value when DecimalHalf is at this value
		#if defined(AltNum_UseIntForDecimalHalf)
        static const signed int NilRep = 2147483645;
		#else
        static const signed int NilRep = 1073741818;
		#endif
	#endif
    public:
    #pragma endregion Const Representation values

    #pragma region RepType

        /// <summary>
        /// Enum representing value type stored
        /// </summary>
        using RepType = MediumDecBase::RepType;

        /// <summary>
        /// Returns representation type data that is stored in value
        /// </summary>
        virtual RepType GetRepType()
        {
#if !defined(AltNum_UseIntForDecimalHalf)
            switch(DecimalHalf.Flag)
            {
#endif
		#if defined(MediumDecV2_EnablePiRep)
                case 1:
                    {
            #if defined(MediumDecV2_EnableApproachingPi)
                        if (DecimalHalf == ApproachingTopRep)
                            return RepType::ApproachingTopPi;
            #endif
                        return RepType::PiNum;
                    }
                    break;
        #endif
		#if defined(MediumDecV2_EnableERep)
                case 2:
                    {
            #if defined(MediumDecV2_EnableApproachingE)
                        if (DecimalHalf == ApproachingTopRep)
                            return RepType::ApproachingTopE;
            #endif
                        return RepType::ENum;
                    }
                    
                    break;
		#endif
        #if defined(MediumDecV2_EnableImaginaryNum)
                case 3:
                    {
            #if defined(MediumDecV2_EnableImaginaryInfinity)
                        if(DecimalHalf == InfinityRep)
                            return RepType::ImaginaryInfinity;
            #endif
            #if defined(MediumDecV2_EnableApproachingI)
                        if (DecimalHalf == ApproachingTopRep)
                            return RepType::ApproachingImaginaryTop;
                        else if (DecimalHalf == ApproachingTopRep)
                            return RepType::ApproachingImaginaryBottom;
            #endif
                        return RepType::INum;
                    }
                    break;
        #elif defined(MediumDecV2_EnableWithinMinMaxRange)
                case 3:
				    //If IntValue==???, then left side range value equals negative infinity
				    //If DecimalHalf.Value==???, then right side range value equals positive infinity
				    //IntValue represents left side minimum
				    //For DecimalHalf.Value represents right side maximum value with negative numbers represents at numbers above ???
				    return RepType::WithinMinMaxRange;
                    break;
        #endif
#if !defined(AltNum_UseIntForDecimalHalf)
                default:
                    {
#endif
		#if defined(MediumDecV2_EnableInfinityRep)
                        if(DecimalHalf == InfinityRep)
                            return RepType::Infinity;
		#endif
		#if defined(MediumDecV2_EnableApproachingValues)
                        if (DecimalHalf == ApproachingBottomRep)
                            return RepType::ApproachingBottom;
                        else if (DecimalHalf == ApproachingTopRep)
                            return RepType::ApproachingTop;
	    #endif
		#if defined(AltNum_EnableNaN)
			            if(DecimalHalf==NaNRep)
				            return RepType::NaN;
			            else if(DecimalHalf==UndefinedRep)
				            return RepType::Undefined;
		#endif
        #if defined(AltNum_EnableNil)
			            if(DecimalHalf==NilRep)
				            return RepType::Nil;
		#endif
		#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
			            if(DecimalHalf==UndefinedInRangeRep)
				            //If IntValue equals 0, than equals undefined value with range between negative infinity and positive infinity 
                            //Otherwise, indicates either negative or positive infinity (outside range of real number representation)
                            return RepType::UndefinedButInRange;
		#endif
                        return RepType::NormalType;
#if !defined(AltNum_UseIntForDecimalHalf)
                    }
                    break;
            }
#endif
			throw "Unknown or non-enabled representation type detected";//Should not reach this point when code is fully working
            return RepType::UnknownType;//Catch-All Value;
        }

    #pragma endregion RepType

public:
    #pragma region RangeLimits

        /// <summary>
        /// Sets value to the highest non-infinite/Special Decimal State Value that it store
        /// </summary>
        constexpr auto SetAsMaximum = MediumDecBase::SetAsMaximum;

        /// <summary>
        /// Sets value to the lowest non-infinite/Special Decimal State Value that it store
        /// </summary>
        constexpr auto SetAsMinimum = MediumDecBase::SetAsMinimum;

    #pragma endregion RangeLimits

    #pragma region PiNum Setters
    #if defined(MediumDecV2_EnablePiRep)
        template<MediumDecVariant VariantType=MediumDecBaseV2>
        virtual void SetPiVal(const VariantType& Value)
        {
            IntValue = Value.IntValue; DecimalHalf = PartialInt(Value.DecimalHalf.Value,1);
        }
        
        virtual void SetPiValFromInt(int Value)
        {
            IntValue = Value.IntValue; DecimalHalf = PartialInt(0,1);
        }
    #endif
    #pragma endregion PiNum Setters

    #pragma region ENum Setters
    #if defined(MediumDecV2_EnableERep)
        template<MediumDecVariant VariantType=MediumDecBaseV2>
        virtual void SetEVal(const VariantType& Value)
        {
            IntValue = Value.IntValue; DecimalHalf = PartialInt(Value.DecimalHalf.Value,2);
        }
        
        virtual void SetEValFromInt(int Value)
        {
            IntValue = Value.IntValue; DecimalHalf = PartialInt(0,2);
        }
    #endif
    #pragma endregion ENum Setters

    #pragma region INum Setters
    #if defined(MediumDecV2_EnableIRep)
        template<MediumDecVariant VariantType=MediumDecBaseV2>
        virtual void SetIVal(const VariantType& Value)
        {
            IntValue = Value.IntValue; DecimalHalf = PartialInt(Value.DecimalHalf.Value,3);
        }
        
        virtual void SetIValFromInt(int Value)
        {
            IntValue = Value.IntValue; DecimalHalf = PartialInt(0,3);
        }
    #endif
    #pragma endregion INum Setters

    #pragma region Fractional Setters
	//Not used for this variant(Used in AltDecBase and others)
    #pragma endregion Fractional Setters
    
    #pragma region MixedFrac Setters
	//Not used for this variant(Used in AltDecBase and others)
    #pragma endregion MixedFrac Setters

    #pragma region Infinity Setters
    //Infinity operations based on https://www.gnu.org/software/libc/manual/html_node/Infinity-and-NaN.html
    // and https://tutorial.math.lamar.edu/classes/calcI/typesofinfinity.aspx
    #if defined(AltNum_EnableInfinityRep)
        void SetAsInfinity()
        {
	#if defined(AltNum_EnableMirroredSection)
            IntValue.IsPositive = 1; DecimalHalf = InfinityRep;
    #else
            IntValue = 1; DecimalHalf = InfinityRep;
    #endif
        }

        void SetAsNegativeInfinity()
        {
	#if defined(AltNum_EnableMirroredSection)
            IntValue.IsPositive = 0; DecimalHalf = InfinityRep;
    #else
            IntValue = -1; DecimalHalf = InfinityRep;
    #endif
        }
	#endif
    #pragma endregion Infinity Setters

    #pragma region ApproachingZero Setters
	#if defined(AltNum_EnableApproachingValues)

		//Alias:SetAsApproachingValueFromRight, Alias:SetAsApproachingZero if value = 0
        //Approaching Towards values from right to left side(IntValue.000...1)
        void SetAsApproachingBottom(int value=0)
        {
            IntValue = value; DecimalHalf = ApproachingBottomRep;
        }

		#if !defined(AltNum_DisableApproachingTop)
		//Alias:SetAsApproachingValueFromLeft, Alias:SetAsApproachingZeroFromLeft if value = 0
        //Approaching Towards (IntValue-1) from Left to right side(IntValue.999...9)
        void SetAsApproachingTop(int value)
        {
            IntValue = value; DecimalHalf = ApproachingTopRep;
        }
        #endif
		
		#if defined(MediumDecV2_EnableApproachingPi)
        //Approaching Towards (IntValue-1) from Left to right side(IntValue.999...9)Pi
        void SetAsApproachingTopPi(int value)
        {
            IntValue = value; DecimalHalf = PartialInt(ApproachingTopRep,1);
        }
		#endif
		
		#if defined(MediumDecV2_EnableApproachingE)
        //Approaching Towards (IntValue-1) from Left to right side(IntValue.999...9)e
        void SetAsApproachingTopE(int value)
        {
            IntValue = value; DecimalHalf = PartialInt(ApproachingTopRep,2);
        }
		#endif
		
		#if defined(MediumDecV2_EnableApproachingI)
        //Approaching Towards (IntValue-1) from Left to right side(IntValue.999...9)i
        void SetAsApproachingTopE(int value)
        {
            IntValue = value; DecimalHalf = PartialInt(ApproachingTopRep,3);
        }
		#endif
    #endif
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

    #pragma endregion ValueDefines

    #pragma region String Commands
        /// <summary>
        /// Reads the string.
        /// </summary>
        /// <param name="Value">The value.</param>
        constexpr auto ReadString = MediumDecBase::ReadString;

        /// <summary>
        /// Gets the value from string.
        /// </summary>
        /// <param name="Value">The value.</param>
        /// <returns>MediumDecV2Base</returns>
        constexpr auto GetValueFromString = MediumDecBase::GetValueFromString<MediumDecV2Base>;

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecV2Base"/> class from string literal
        /// </summary>
        /// <param name="strVal">The value.</param>
        MediumDecV2Base(const char* strVal)
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
        /// Initializes a new instance of the <see cref="MediumDecV2Base"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecV2Base(std::string Value)
        {
            this->ReadString(Value);
        #if defined(AltNum_EnablePiRep)
            if(str.find("Pi") != std::string::npos)
                DecimalHalf.Flags = 1;
        #endif
        #if defined(AltNum_EnableERep)
            if(Value.last()=='e')
                DecimalHalf.Flags = 2;
        #endif
        #if defined(AltNum_EnableImaginaryNum)
            if(Value.last()=='i')
                DecimalHalf.Flags = 3;
        #endif
        }

    #pragma endregion String Commands

    #pragma region ConvertFromOtherTypes
		
        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecV2Base"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecV2Base(const float& Value)
        {
            this->SetFloatVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecV2Base"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecV2Base(const double& Value)
        {
            this->SetDoubleVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecV2Base"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecV2Base(const ldouble& Value)
        {
            this->SetDecimalVal(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecV2Base"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecV2Base(const bool& Value)
        {
            this->SetBoolVal(Value);
        }

#if defined(AltNum_EnableMediumDecV2BaseBasedSetValues)
        MediumDecV2Base(const MediumDecV2Base& Value)
        {
            this->SetVal(Value);
        }
#endif

    #pragma endregion ConvertFromOtherTypes

    #pragma region ConvertToOtherTypes
public:

        /// <summary>
        /// MediumDec Variant to float explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        virtual float toFloat()
        {
            auto self = *this;
            self.ConvertToNormTypeV2();
            return self.toFloatV1();
        }

private:
        constexpr auto toDoubleV1 = MediumDecBase::toDouble;
public:

        /// <summary>
        /// MediumDec Variant to double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        virtual double toDouble()
        {
            auto self = *this;
            self.ConvertToNormTypeV2();
            return self.toDoubleV1();
        }

private:
        constexpr auto toDecimalV1 = MediumDecBase::toDecimal;
public:

        /// <summary>
        /// MediumDec Variant to long double explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        virtual ldouble toDecimal()
        {
            MediumDecV2Base self = *this;
            self.ConvertToNormTypeV2();
            return self.toDecimalV1();
        }

        /// <summary>
        /// MediumDec Variant to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        virtual int toInt() {
            MediumDecV2Base self = *this;
            self.ConvertToNormTypeV2();
            return IntValue.GetValue();
        }

        virtual bool toBool() {
            MediumDecV2Base self = *this;
            self.ConvertToNormTypeV2();
            return IntValue.IsZero() ? false : true;
        }

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
        explicit operator ldouble() { return toDecimal(); }

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
    #if defined(AltNum_EnablePiRep)
    
    virtual void ConvertPiToNum();

    #endif
    #pragma endregion Pi Conversion

    #pragma region E Conversion
    #if defined(AltNum_EnableERep)
    
    virtual void ConvertEToNum();

    #endif
    #pragma endregion E Conversion

    #pragma region Other RepType Conversion

    #pragma endregion Other RepType Conversion

    #pragma region Comparison Operators
protected:
		//Compare only as if in NormalType representation mode
        constexpr auto BasicComparison = MediumDecBase::BasicComparisonV1<MediumDecV2Base>;

#if defined(AltNum_EnableMirroredSection)
		//Compare only as if in NormalType representation mode ignoring sign(check before using)
        constexpr auto BasicComparisonV2 = MediumDecBase::BasicComparisonWithoutSignCheck<MediumDecV2Base>;
#endif

    #if defined(AltNum_DefineInfinityAsSignedReps)
		template<MediumDecVariant VariantType=MediumDecV2Base>
		std::strong_ordering LSideInfinityComparison(const VariantType& that, const RepType& RRep) const
		{
	    #if defined(AltNum_EnableMirroredSection)
			if(IntValue.IsPositive())
				if(RRep==RepType:Infinity&&that.IntValue.IsPositive())
					return 1<=>1;
				else
					return 1<=>0;
			else
				if(RRep==RepType:Infinity&&that.IntValue.IsNegative())
					return 1<=>1;
				else
					return 0<=>1;
		#else
			if(IntValue==1)
				if(RRep==RepType:Infinity&&that.IntValue==1)
					return 1<=>1;
				else
					return 1<=>0;
			else
				if(RRep==RepType:Infinity&&that.IntValue==-1)
					return 1<=>1;
				else
					return 0<=>1;
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
	#if defined(AltNum_EnableMirroredSection)
			//Comparing if number is negative vs positive
			if (auto SignCmp = IntValue.IsPositive <=> that.IntValue.IsPositive; SignCmp != 0)
				return SignCmp;
	#endif
	
			RepType LRep = GetRepType();
			RepType RRep = that.GetRepType();
    #if defined(AltNum_EnableNaN)||defined(AltNum_EnableNilRep)||defined(AltNum_EnableUndefinedButInRange)
			if(LRep^UndefinedBit||RRep^UndefinedBit)
				throw "Can't compare undefined/nil representations";
    #endif
    //AltNum_UseIntForDecimalHalf is required to not be set for
    //imaginary numbers to be supported by MediumDecV2
    #if defined(AltNum_EnableImaginaryNum)
            if (LValue.DecimalHalf.Flags == 3)
            {
                if(RValue.Flags!=3)
                    throw "Can't compare imaginary number with real number";
				else if(RRep==RepType:ImaginaryInfinity)
                {
					if(that.IntValue==1)
						return 0<=>1;//Positive Infinity is greater than real number representations
					else
						return 1<=>0;
                }
                else
                {
	    #if defined(AltNum_EnableMirroredSection)
			    	return BasicComparisonV2(rSide);
	    #else
					return BasicComparison(rSide);
	    #endif
                }
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
	#if defined(AltNum_EnableApproachingValues)
	
	#endif
				default:
				{
					if(LRep==RRep)
	#if defined(AltNum_EnableMirroredSection)
						return BasicComparisonV2(that);
	#else
						return BasicComparison(that);
	#endif
					else if(RRep==RepType:Infinity)
                    {
                        if(that.IntValue==1)
							return 0<=>1;//Positive Infinity is greater than real number representations
						else
							return 1<=>0;
                    }
                    else
					{
						MediumDecV2Base lSide = *this;
						MediumDecV2Base rSide = that;
						lSide.ConvertToNormTypeV2(); rSide.ConvertToNormTypeV2();
	#if defined(AltNum_EnableMirroredSection)
						return lSide.BasicComparisonV2(rSide);
	#else
						return rSide.BasicComparison(rSide);
	#endif
					}
				}
			}
		}

		//Templated version of Spaceship operator to allow full version of class to inherit the spaceship operator code
		template<MediumDecVariant VariantType=MediumDecV2Base>
		std::strong_ordering CompareWithIntV1(const int& that) const
		{
			int lVal; int rVal;
			//Pi and E only enabled if imbedded flags are enabled
	#if !defined(AltNum_UseIntForDecimalHalf)
			if(DecimalHalf.Flags==0)
			{
	#endif
				return BasicIntComparison(that);
	#if !defined(AltNum_UseIntForDecimalHalf)
			}
			else
			{
				MediumDecV2Base lSide = *this;
				lSide.ConvertToNormTypeV2();
				return lSide.BasicIntComparison(that);
			}
	#endif
		}

		//Alias to prevent creating function more than once with template arguments
        constexpr auto CompareWith = MediumDecBase::CompareWithV1<MediumDecV2Base>;

		//Alias to prevent creating function more than once with template arguments
        constexpr auto CompareWithInt = MediumDecBase::CompareWithIntV1<MediumDecV2Base>;

public:
		std::strong_ordering operator<=>(const MediumDecV2Base& that) const
		{
			return CompareWith(that);
		}

		std::strong_ordering operator<=>(const int& that) const
		{
			return CompareWithInt(that);
		}

		bool operator==(const int& that) const
		{
			if (IntValue!=that)
				return false;
			if (DecimalHalf!=0)
				return false;
			return true;
		}

		bool operator==(const MediumDecV2Base& that) const
		{
			if (IntValue!=that.IntValue)
				return false;
			if (DecimalHalf!=that.IntValue)
				return false;
		}
    #pragma endregion Comparison Operators

    #pragma region NormalRep Integer Division Operations

    #pragma endregion NormalRep Integer Division Operations

    #pragma region NormalRep Integer Multiplication Operations

    #pragma endregion NormalRep Integer Multiplication Operations

    #pragma region NormalRep Integer Addition Operations

	#pragma endregion NormalRep Integer Addition Operations

    #pragma region NormalRep Integer Subtraction Operations

    #pragma endregion NormalRep Integer Subtraction Operations

    #pragma region NormalRep Integer Bitwise Operations

    #pragma endregion NormalRep Integer Bitwise Operations

    #pragma region Mixed Fraction Operations

    #pragma endregion Mixed Fraction Operations

    #pragma region NormalRep AltNum Division Operations

    #pragma endregion NormalRep AltNum Division Operations

    #pragma region NormalRep AltNum Multiplication Operations

    #pragma endregion NormalRep AltNum Multiplication Operations

	#pragma region NormalRep AltNum Addition Operations

	#pragma endregion NormalRep AltNum Addition Operations

	#pragma region NormalRep AltNum Subtraction Operations

	#pragma endregion NormalRep AltNum Subtraction Operations

    #pragma region Other Division Operations

	#pragma endregion Other Division Operations	

    #pragma region Other Multiplication Operations

    #pragma endregion Other Multiplication Operations

    #pragma region Other Addition Operations

    #pragma endregion Other Addition Operations

    #pragma region Other Subtraction Operations

    #pragma endregion Other Subtraction Operations
    }
    #pragma region String Function Source

    #pragma endregion String Function Source
}

    std::string MediumDecV2Base::ToString()
    {
        RepType repType = GetRepType();
        switch (repType)
        {
	#if defined(AltNum_EnableInfinityRep)
        case RepType::Infinity:
            return IsNegative()?"-∞":"∞";
            break;
	    #if defined(AltNum_EnableApproachingValues)
        case RepType::ApproachingBottom:
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingBottom);
            return BasicToStringOp();
			#else
            return (std::string)IntValue + ".0..01";
			#endif
            break;
        case RepType::ApproachingTop:
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingTop);
            return BasicToStringOp();
			#else
            return (std::string)IntValue + ".9..9";
			#endif
            break;
		    #if defined(AltNum_EnableApproachingDivided)
		//ToDo:work on unreal string version for the various approaching values
        case RepType::ApproachingMidRight:
        case RepType::ApproachingMidLeft:
            ConvertToNormType(repType);
			return BasicToStringOp();
			break;
            #endif
        #endif
	#endif
	/*
    #if defined(AltNum_EnableFractionals)
            return BasicToStringOp()+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(ExtraRep);
            break;
    #endif
	*/
	#if defined(AltNum_EnablePiRep)
        case RepType::PiNum:
            return BasicToStringOp()+"π";
            break;
		/*
        #if defined(AltNum_EnableDecimaledPiFractionals)
        case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
            return BasicToStringOp()+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"Ï€";
            break;
        #elif defined(AltNum_EnableAlternativeRepFractionals)
        case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
            return (std::string)IntValue+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(DecimalHalf)+"Ï€";
            break;
        #endif
		*/
	#endif
	#if defined(AltNum_EnableERep)
        case RepType::ENum:
            return BasicToStringOp()+"e";
            break;
		/*
        #if defined(AltNum_EnableDecimaledPiFractionals)
        case RepType::ENumByDiv://  (Value/(ExtraRep*-1))*e Representation
            return BasicToStringOp()+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"e";
            break;
        #elif defined(AltNum_EnableAlternativeRepFractionals)
        case RepType::EFractional://  IntValue/DecimalHalf*e Representation
            return (std::string)IntValue+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(DecimalHalf)+"e";
            break;
        #endif
		*/
	#endif

	#if defined(AltNum_EnableImaginaryNum)
        case RepType::INum:
            return BasicToStringOp()+"i";
            break;
		/*
        #if defined(AltNum_EnableDecimaledPiFractionals)
        case RepType::INumByDiv://  (Value/(ExtraRep*-1))*i Representation
            return BasicToStringOp()+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"i";
            break;
        #elif defined(AltNum_EnableAlternativeRepFractionals)
        case RepType::IFractional://  IntValue/DecimalHalf*i Representation
            return (std::string)IntValue+"/"
            +VariableConversionFunctions::UnsignedIntToStringConversion(DecimalHalf)+"i";
            break;
        #endif
		*/
	#endif
	#if defined(AltNum_EnableApproachingPi)
        case RepType::ApproachingTopPi://equal to IntValue.9..9 Pi
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingTop);
            return BasicToStringOp()+"π";
			#else
            return (std::string)IntValue + ".9..9π";
			#endif
            break;
	#endif
	#if defined(AltNum_EnableApproachingE)
        case RepType::ApproachingTopE://equal to IntValue.9..9 e
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingTop);
            return BasicToStringOp()+"e";
			#else
            return (std::string)IntValue + ".9..9e";
			#endif
            break;
	#endif
    #if defined(AltNum_EnableImaginaryInfinity)
        case RepType::ImaginaryInfinity:
            return IsNegative()?"-∞i":"∞i";
            break;
	#endif
	
	#if defined(MediumDecV2_EnableApproachingI)
        case RepType::ApproachingImaginaryBottom:
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingBottom);
            return BasicToStringOp()+"i";
			#else
            return (std::string)IntValue + ".0..01i";
			#endif
            break;
        case RepType::ApproachingImaginaryTop:
			#ifdef AltNum_DisplayApproachingAsReal
			ConvertToNormType(RepType::ApproachingTop);
            return BasicToStringOp()+"i";
			#else
            return (std::string)IntValue + ".9..9i";
			#endif
            break;
		/*#if defined(AltNum_EnableApproachingDivided)
		//ToDo:work on unreal string version for the various approaching values
        case RepType::ApproachingImaginaryMidRight:
        case RepType::ApproachingImaginaryMidLeft:
            ConvertToNormType(repType);
			return BasicToStringOp()+"i";
			break;
        #endif*/
    #endif
	/*
    #if defined(AltNum_EnableMixedFractional)
        case RepType::MixedFrac://IntValue +- (-DecimalHalf)/ExtraRep
            return (std::string)IntValue+" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
            +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(ExtraRep);
            break;
		#if defined(AltNum_EnableMixedPiFractional)
        case RepType::MixedPi://IntValue +- (-DecimalHalf/-ExtraRep)
            return (std::string)IntValue+" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
            +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"π";
            break;
		#elif defined(AltNum_EnableMixedEFractional)
        case RepType::MixedE://IntValue +- (-DecimalHalf/-ExtraRep)
            return (std::string)IntValue+" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
            +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"e";
            break;
		#elif defined(AltNum_EnableMixedIFractional)
        case RepType::MixedI://IntValue +- (-DecimalHalf/-ExtraRep)
            return (std::string)IntValue+" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
            +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"i";
            break;
		#endif
    #endif
	*/
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
		#if defined(AltNum_EnableWithinMinMaxRange)//Undefined except for ranged IntValue to DecimalHalf (ExtraRepValue==UndefinedInRangeMinMaxRep)
        case WithinMinMaxRange:
		    return "WithinMinMaxRange of "+VariableConversionFunctions::UnsignedIntToStringConversion((int)IntValue)+" to "+VariableConversionFunctions::UnsignedIntToStringConversion(DecimalHalf);
            break;
        #endif
		*/
	#endif
    #if defined(AltNum_EnableNil)
        case RepType::Nil:
            return "Nil";
    #endif
        default:
			ConvertToNormType(repType);
			return BasicToStringOp();
            break;
        }
    }