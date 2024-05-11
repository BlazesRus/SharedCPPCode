// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

#include "..\MediumDec\MediumDecBase.hpp"
#include "..\MediumDec\MediumDec.hpp"
#if defined(AltNum_UseBuiltinVirtualTable)
	#include "..\VirtualTableBase.hpp"
#endif

namespace BlazesRusCode
{
    class MediumDecV2Base;

    /// <summary>
    /// Separating functions that don't use static variables inside this base class for deriving
    /// Completed class inside MediumDec
	/// </summary>
    class DLL_API MediumDecV2Base : public MediumDecBase
    {
	#if defined(AltNum_UseBuiltinVirtualTable)
	protected:
		struct VirtualTable {
			RepTypeFn* VirtualTable_GetRepType;
			//String_RepTypeFn* VirtualTable_RepTypeAsString;
		};
		VirtualTable* VTable;
	#endif
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
	#if defined(AltNum_UseBuiltinVirtualTable)
			VTable = new VirtualTable;
	#endif
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

        /// <summary>
        /// Sets the value.
        /// </summary>
        /// <param name="Value">The value.</param>
        void SetVal(const MediumDecV2& Value)
        {
            IntValue = Value.IntValue;
            DecimalHalf = Value.DecimalHalf;
        } const


        constexpr auto SetAsZero = MediumDecBase::SetAsZero;

        /// <summary>
        /// Swaps the negative status.
        /// </summary>
        constexpr auto SwapNegativeStatus = MediumDecBase::SwapNegativeStatus;

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
	#if defined(AltNum_EnableImaginaryNum)
        //e*Value representation(DecimalHalf.Flag==3)
        static const unsigned int IRep = 3;
	#endif
	
	#if defined(AltNum_EnableInfinityRep)
        //When DecimalHalf.Value equals this value, it represents infinity (sign of IntValue determines if either negative or positive inifity)
		static const unsigned int InfinityRep = 1073741824;
	#endif
	#if defined(AltNum_EnableApproachingValues)
        //When DecimalHalf.Value equals this value, it represents Approaching IntValue from right towards left (IntValue.0..01)
        static const unsigned int ApproachingBottomRep = 1073741823;
        //When DecimalHalf.Value equals this value, it represents Approaching IntValue from left towards right (IntValue.9..9)
		static const unsigned int ApproachingTopRep = 1073741822;
	#endif
	#if defined(AltNum_EnableUndefinedButInRange)
		//When DecimalHalf is at this value, than value is undefined but within real number range
		//Such as result of Cos of infinity
		//https://www.cuemath.com/trigonometry/domain-and-range-of-trigonometric-functions/
        static const unsigned int UndefinedInRangeRep = 1073741821;
	#endif
	#if defined(AltNum_EnableNaN)
        //Is NaN when DecimalHalf is at this value
        static const signed int NaNRep = 1073741820;
        //Is Undefined when DecimalHalf is at this value
        static const signed int UndefinedRep = 1073741819;
	#endif
	#if defined(AltNum_EnableNil)
        //Is defined at empty value when DecimalHalf is at this value
        static const signed int NilRep = 1073741818;
	#endif
    public:
    #pragma endregion Const Representation values

    #pragma region RepType

        /// <summary>
        /// Returns representation type data that is stored in value
        /// </summary>
#if defined(AltNum_UseBuiltinVirtualTable)
        RepType VirtualTable_GetRepType()//Virtual Function for use in directly calling
#else
        RepType GetRepType()
#endif
        {
            switch(DecimalHalf.Flag)
            {
		#if defined(MediumDecV2_EnablePiRep)
                case 1:
                    {
            #if defined(MediumDecV2_EnableApproachingPi)
                        if (DecimalHalf == ApproachingBottomRep)
                            return RepType::ApproachingBottomPi;
                        else if (DecimalHalf == ApproachingTopRep)
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
                        if (DecimalHalf == ApproachingBottomRep)
                            return RepType::ApproachingBottomE;
                        else if (DecimalHalf == ApproachingTopRep)
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
                default:
                    {
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
                    }
                    break;
            }
			throw "Unknown or non-enabled representation type detected";//Should not reach this point when code is fully working
            return RepType::UnknownType;//Catch-All Value;
        }

#if defined(AltNum_UseBuiltinVirtualTable)
        /// <summary>
        /// Returns representation type data that is stored in value(Directly calling function)
        /// </summary>
        RepType GetRepType()
		{
			GetVTable(VTable)->VirtualTable_GetRepType(VTable);
		}
		
		/*
		static std::string RepTypeAsString(const RepType& repType)
		{
			GetVTable(VTable)->VirtualTable_RepTypeAsString(VTable, repType);
		}*/
#endif

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
protected:
        template<MediumDecVariant VariantType=MediumDecBaseV2>
        void SetPiValV1(const VariantType& Value)
        {
            IntValue = Value.IntValue; DecimalHalf = PartialInt(Value.DecimalHalf.Value,1);
        }
public:
        constexpr auto SetPiVal = SetPiValV1<MediumDecV2Base>;
  
        void SetPiValFromInt(int Value)
        {
            IntValue = Value.IntValue; DecimalHalf = PartialInt(0,1);
        }
    #endif
    #pragma endregion PiNum Setters

    #pragma region ENum Setters
    #if defined(MediumDecV2_EnableERep)
protected:
        template<MediumDecVariant VariantType=MediumDecBaseV2>
        void SetEValV1(const VariantType& Value)
        {
            IntValue = Value.IntValue; DecimalHalf = PartialInt(Value.DecimalHalf.Value,2);
        }
public:
        constexpr auto SetEVal = SetEValV1<MediumDecV2Base>;
		
        void SetEValFromInt(int Value)
        {
            IntValue = Value.IntValue; DecimalHalf = PartialInt(0,2);
        }
    #endif
    #pragma endregion ENum Setters

    #pragma region INum Setters
    #if defined(MediumDecV2_EnableIRep)
protected:
        template<MediumDecVariant VariantType=MediumDecBaseV2>
        void SetIValV1(const VariantType& Value)
        {
            IntValue = Value.IntValue; DecimalHalf = PartialInt(Value.DecimalHalf.Value,3);
        }
public:
        constexpr auto SetIVal = SetEValV1<MediumDecV2Base>;
		
        void SetIValFromInt(int Value)
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
        void SetAsApproachingBottom(const int& value=0)
        {
            IntValue = value; DecimalHalf = ApproachingBottomRep;
        }

		#if !defined(AltNum_DisableApproachingTop)
		//Alias:SetAsApproachingValueFromLeft, Alias:SetAsApproachingZeroFromLeft if value = 0
        //Approaching Towards (IntValue-1) from Left to right side(IntValue.999...9)
        void SetAsApproachingTop(const int& value)
        {
            IntValue = value; DecimalHalf = ApproachingTopRep;
        }
        #endif
		
		#if defined(MediumDecV2_EnableApproachingPi)
        //Approaching Towards (IntValue-1) from Left to right side(IntValue.999...9)Pi
        void SetAsApproachingTopPi(const int& value)
        {
            IntValue = value; DecimalHalf = PartialInt(ApproachingTopRep,1);
        }
		
        void SetAsApproachingBottomPi(const int& value)
        {
            IntValue = value; DecimalHalf = PartialInt(ApproachingBottomRep,1);
        }
		#endif
		
		#if defined(MediumDecV2_EnableApproachingE)
        //Approaching Towards (IntValue-1) from Left to right side(IntValue.999...9)e
        void SetAsApproachingTopE(const int& value)
        {
            IntValue = value; DecimalHalf = PartialInt(ApproachingTopRep,2);
        }
		
        void SetAsApproachingBottomE(const int& value)
        {
            IntValue = value; DecimalHalf = PartialInt(ApproachingBottomRep,2);
        }
		#endif
		
		#if defined(MediumDecV2_EnableApproachingI)
        //Approaching Towards (IntValue-1) from Left to right side(IntValue.999...9)i
        void SetAsApproachingTopI(const int& value)
        {
            IntValue = value; DecimalHalf = PartialInt(ApproachingTopRep,3);
        }
		
        void SetAsApproachingBottomI(const int& value)
        {
            IntValue = value; DecimalHalf = PartialInt(ApproachingBottomRep,3);
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

protected:

        void InitialyzeAltRepFromString(const std::string& Value)
        {
        #if defined(MediumDecV2_EnablePiRep)
            if(str.find("Pi") != std::string::npos)
                DecimalHalf.Flags = 1;
        #endif
        #if defined(MediumDecV2_EnableERep)
            if(Value.last()=='e')
                DecimalHalf.Flags = 2;
        #endif
        #if defined(MediumDecV2_EnableImaginaryNum)
            if(Value.last()=='i')
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
            ReadString(Value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="MediumDecV2Base"/> class.
        /// </summary>
        /// <param name="Value">The value.</param>
        MediumDecV2Base(const std::string& Value)
        {
            ReadString(Value);
        }

		std::string ConvertToBasicString(const RepType& repType)
		{
			auto self = ConvertAsNormType(repType);
			return self.BasicToStringOp();
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
        float toFloat()
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
        double toDouble()
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
        ldouble toDecimal()
        {
            auto self = *this;
            self.ConvertToNormTypeV2();
            return self.toDecimalV1();
        }

        /// <summary>
        /// MediumDec Variant to int explicit conversion
        /// </summary>
        /// <returns>The result of the operator.</returns>
        int toInt() {
            auto self = *this;
            self.ConvertToNormTypeV2();
            return IntValue.GetValue();
        }

        bool toBool() {
            auto self = *this;
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
    
    void ConvertPiToNum();

    #endif
    #pragma endregion Pi Conversion

    #pragma region E Conversion
    #if defined(AltNum_EnableERep)
    
    void ConvertEToNum();

    #endif
    #pragma endregion E Conversion

    #pragma region Imaginary Conversion
    #if defined(AltNum_EnableImaginaryNum)

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
                        ExtraRep = 0;
                    }
                    break;
            #endif*/
            #if defined(AltNum_EnableApproachingValues)
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
                IntValue = IsPositive()?MaxIntValue:MinIntValue; 
                DecimalHalf.Value = 999999999; 
                /*ExtraRep = 0;*/
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
      //To-Do: Test cost of making vs alternative means of making sure uses updated code after derivation    

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
		#if defined(AltNum_EnableApproachingValues)
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
		#if defined(AltNum_EnableApproachingValues)
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
				IntValue = IsPositive()?MaxIntValue:MinIntValue; 
				DecimalHalf = 999999999;
				break;
	#endif
	#if defined(AltNum_EnableApproachingValues)
			case RepType::ApproachingBottom:
				DecimalHalf = 1;
				break;
		#if !defined(AltNum_DisableApproachingTop)
			case RepType::ApproachingTop:
				DecimalHalf = 999999999;
				break;
		#endif
	#endif
	#if defined(AltNum_EnableImaginaryNum)
			case RepType::INum:
				break;
		#if defined(AltNum_EnableApproachingValues)
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
				IntValue = IsPositive()?MaxIntValue:MinIntValue; 
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
            VariantType Res = *this;
            Res.ConvertToNormTypeV2();
            return Res;
        }

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
			//Comparing if number is negative vs positive
			if (auto SignCmp = IntValue.IsPositive <=> that.IntValue.IsPositive; SignCmp != 0)
				return SignCmp;
	
			RepType LRep = GetRepType();
			RepType RRep = that.GetRepType();
    #if defined(AltNum_EnableNaN)||defined(AltNum_EnableNilRep)||defined(AltNum_EnableUndefinedButInRange)
			if(LRep^UndefinedBit||RRep^UndefinedBit)
				throw "Can't compare undefined/nil representations";
    #endif
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
	#if defined(AltNum_EnableApproachingValues)
	
	#endif
				default:
				{
					if(LRep==RRep)
						return BasicComparisonV2(that);
					else if(RRep==RepType:Infinity)
                    {
                        if(that.IntValue==1)
							return 0<=>1;//Positive Infinity is greater than real number representations
						else
							return 1<=>0;
                    }
                    else
					{
						auto lSide = *this;
						auto rSide = that;
						lSide.ConvertToNormTypeV2(); rSide.ConvertToNormTypeV2();
						return lSide.BasicComparisonV2(rSide);
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
			if(DecimalHalf.Flags==0)
			{
				return BasicIntComparison(that);
			}
			else
			{
				auto lSide = *this;
				lSide.ConvertToNormTypeV2();
				return lSide.BasicIntComparison(that);
			}
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

	/*  
		//Add comparisons to previous parent classes with more limited ranges based on supported values

    */

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

		bool operator==(const MediumDec& that) const
		{
			if (IntValue!=that.IntValue)
				return false;
			if (DecimalHalf!=that.IntValue)
				return false;
		}

		bool operator==(const MediumDecV2Base& that) const
		{
			if (IntValue!=that.IntValue)
				return false;
			if (DecimalHalf!=that.IntValue)
				return false;
		}
    #pragma endregion Comparison Operators

    #pragma region NormalRep Integer division operations
protected:
        template<IntegerType IntType=unsigned int>
        constexpr auto PartialUIntDivOp = MediumDecBase::PartialUIntDivOp<IntType>;

        template<IntegerType IntType=signed int>
        constexpr auto PartialIntDivOp = MediumDecBase::PartialIntDivOp<IntType>;

public:
        /// <summary>
        /// Basic division operation between MediumDec Variant and Integer value 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>AltDec&</returns>
        template<IntegerType IntType=unsigned int>
        constexpr auto BasicUIntDivOp = MediumDecBase::BasicUIntDivOp<IntType>;

        /// <summary>
        /// Basic division operation between MediumDec Variant and Integer value 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>AltDec&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicIntDivOp = MediumDecBase::BasicIntDivOp<IntType>;

    #pragma endregion NormalRep Integer division operations

    #pragma region NormalRep Integer Multiplication Operations
protected:
        template<IntegerType IntType=signed int>
        constexpr auto PartialIntMultOp = MediumDecBase::PartialIntMultOp<IntType>;

        template<IntegerType IntType=signed int>
        constexpr auto PartialUIntMultOp = MediumDecBase::PartialUIntMultOp<IntType>;

public:
        /// <summary>
        /// Basic multiplication operation between MediumDec Variant and Integer value 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDecV2&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicIntMultOp = MediumDecBase::BasicIntMultOp<IntType>;

        /// <summary>
        /// Basic multiplication operation between MediumDec Variant and Integer value 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDecV2&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicUIntMultOp = MediumDecBase::BasicUIntMultOp<IntType>;

    #pragma endregion NormalRep Integer Multiplication Operations

    #pragma region NormalRep Integer Addition Operations

        /// <summary>
        /// Basic addition operation between MediumDec Variant and Integer value 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>MediumDecV2&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicIntAddOp = MediumDecBase::BasicIntAddOp<AltDecBase>;

	#pragma endregion NormalRep Integer Addition Operations

    #pragma region NormalRep Integer Subtraction Operations

        /// <summary>
        /// Basic subtraction operation between MediumDec Variant and Integer value 
        /// that ignores special representation status
        /// </summary>
        /// <param name="rValue">The value.</param>
        /// <returns>AltDec&</returns>
        template<IntegerType IntType=signed int>
        constexpr auto BasicIntSubOp = MediumDecBase::BasicIntSubOp<AltDecBase>;

    #pragma endregion NormalRep Integer Subtraction Operations

    #pragma region NormalRep Integer Bitwise Operations

    #pragma endregion NormalRep Integer Bitwise Operations

    #pragma region Mixed Fraction Operations

    #pragma endregion Mixed Fraction Operations

    #pragma region NormalRep AltNum division operations

    #pragma endregion NormalRep AltNum division operations

    #pragma region NormalRep AltNum Multiplication Operations

    #pragma endregion NormalRep AltNum Multiplication Operations

	#pragma region NormalRep AltNum Addition Operations

	#pragma endregion NormalRep AltNum Addition Operations

	#pragma region NormalRep AltNum Subtraction Operations

	#pragma endregion NormalRep AltNum Subtraction Operations

	#pragma region Other division operations

		//Simplified division by 2 operation(to reduce cost of dividing)
        constexpr auto DivideByTwo = MediumDecBase::DivideByTwo;

		//Simplified division by 4 operation(to reduce cost of dividing)
        constexpr auto DivideByFour = MediumDecBase::DivideByFour;

protected:
        /// <summary>
        /// Unsigned division operation between MediumDec variant and unsigned integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2Base&</returns>
        template<IntegerType IntType= unsigned int>
        auto UIntDivOpV1(const IntType& rValue)
		{
			//Add Code here
		}

        /// <summary>
        /// Division operation between MediumDec variant and integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2Base&</returns>
        template<IntegerType IntType= signed int>
        auto IntDivOpV1(const IntType& rValue)
		{
			//Add Code here
		}

        /// <summary>
        /// Unsigned division operation between MediumDec variant and unsigned integer values
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2Base</returns>
        template<IntegerType IntType= unsigned int>
        auto DivByUIntV1(const IntType& rValue)
		{
			//Add Code here
		}

        /// <summary>
        /// Division operation between MediumDec variant and integer values
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2Base</returns>
        template<IntegerType IntType= signed int>
        constexpr auto DivByIntV1(const IntType& rValue)
		{
			//Add Code here
		}

public:

        constexpr auto UIntDivOpV1 = UIntDivOpV1<unsigned int>;
        constexpr auto IntDivOpV1 = IntDivOpV1<signed int>;
        constexpr auto UnsignedIntDivOp = UIntDivOpV1<signed int>;
        constexpr auto UInt64DivOp = UIntDivOpV1<unsigned long long>;
        constexpr auto Int64DivOp = IntDivOpV1<signed long long>;
	
        constexpr auto DivByUInt = DivByUIntV1<unsigned int>;
        constexpr auto DivByInt = DivByIntV1<signed int>;
        constexpr auto UnsignedDivByInt = DivByUInt<signed int>;
        constexpr auto DivByUInt64 = DivByUInt<;
        constexpr auto DivByInt64 = BasicDivByInt64;
        constexpr auto UnsignedDivByInt64 = DivByUInt<signed long long>;
		
        constexpr auto DivByUInt8 = BasicDivByUInt8;
        constexpr auto DivByInt8 = BasicDivByInt8;
        constexpr auto DivByUInt16 = BasicDivByUInt16;
        constexpr auto DivByInt16 = BasicDivByInt16;

        /// <summary>
        /// Unsigned division operation between MediumDec variants.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>auto&</returns>
        constexpr auto UnsignedDivOp

        /// <summary>
        /// Division operation between MediumDec variants.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>auto&</returns>
        constexpr auto DivOp

        /// <summary>
        /// Unsigned division operation between MediumDec variants.
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>auto</returns>
        constexpr auto DivByUnsigned

        /// <summary>
        /// Division operation between MediumDec variants.
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>auto</returns>
        constexpr auto DivBy

        /// <summary>
        /// Division operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecV2Base</returns>
        friend MediumDecV2Base operator/(const MediumDecV2Base& self, const MediumDecV2Base& Value) { return self.DivideBy(Value); }
		
        /// <summary>
        /// /= operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecV2Base</returns>
        friend MediumDecV2Base& operator/=(MediumDecV2Base& self, const MediumDecV2Base& Value) { return self.DivOp(Value); }
		
        /// <summary>
        /// Division operation between MediumDecV2Base and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecV2Base</returns>
        friend MediumDecV2Base operator/(const MediumDecV2Base& self, const signed int& Value) { return self.DivideByInt(Value); }
        friend MediumDecV2Base operator/(const MediumDecV2Base& self, const signed long long& Value) { return self.DivideByInt64(Value); }
        friend MediumDecV2Base operator/(const MediumDecV2Base& self, const unsigned int& Value) { return self.DivideByUInt(Value); }
        friend MediumDecV2Base operator/(const MediumDecV2Base& self, const unsigned long long& Value) { return self.DivideByUInt64(Value); }
		
        friend MediumDecV2Base operator/(const signed int& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).DivideBy(rValue); }
        friend MediumDecV2Base operator/(const signed long long& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).DivideBy(rValue); }
        friend MediumDecV2Base operator/(const unsigned int& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).DivideBy(rValue); }
        friend MediumDecV2Base operator/(const unsigned long long& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).DivideBy(rValue); }

        friend MediumDecV2Base operator/(const MediumDecV2Base& self, const signed char& Value) { return self.DivideByInt8(Value); }
        friend MediumDecV2Base operator/(const MediumDecV2Base& self, const signed short& Value) { return self.DivideByInt16(Value); }
        friend MediumDecV2Base operator/(const MediumDecV2Base& self, const unsigned char& Value) { return self.DivideByUInt8(Value); }
        friend MediumDecV2Base operator/(const MediumDecV2Base& self, const unsigned short& Value) { return self.DivideByUInt16(Value); }

        friend MediumDecV2Base operator/(const signed char& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).DivideBy(rValue); }
        friend MediumDecV2Base operator/(const signed short& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).DivideBy(rValue); }
        friend MediumDecV2Base operator/(const unsigned char& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).DivideBy(rValue); }
        friend MediumDecV2Base operator/(const unsigned short& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).DivideBy(rValue); }


        /// <summary>
        /// *= operation between MediumDecV2Base and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecV2Base</returns>
        friend MediumDecV2Base& operator/=(MediumDecV2Base& self, const signed int& Value) { return self.IntDivOp(Value); }
        friend MediumDecV2Base& operator/=(MediumDecV2Base& self, const signed long long& Value) { return self.Int64DivOp(Value); }
        friend MediumDecV2Base& operator/=(MediumDecV2Base& self, const unsigned int& Value) { return self.UIntDivOp(Value); }
        friend MediumDecV2Base& operator/=(MediumDecV2Base& self, const unsigned long long& Value) { return self.UInt64DivOp(Value); }

        friend MediumDecV2Base& operator/=(MediumDecV2Base& self, const signed char& Value) { return self.Int8DivOp(Value); }
        friend MediumDecV2Base& operator/=(MediumDecV2Base& self, const signed short& Value) { return self.Int16DivOp(Value); }
        friend MediumDecV2Base& operator/=(MediumDecV2Base& self, const unsigned char& Value) { return self.UInt8DivOp(Value); }
        friend MediumDecV2Base& operator/=(MediumDecV2Base& self, const unsigned short& Value) { return self.UInt16DivOp(Value); }	#pragma endregion Other division operations	

	#pragma endregion Other division operations	

	#pragma region Other multiplication operations

        /// <summary>
        /// Simplified multiplication by 2 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>void</returns>
        constexpr auto MultiplyByTwo = MediumDecBase:MultiplyByTwo;

        /// <summary>
        /// Simplified multiplication by 4 operation(to reduce cost of operations)
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>void</returns>
        constexpr auto MultiplyByFour = MediumDecBase:MultiplyByFour;

protected:
        /// <summary>
        /// Multiplication operation between MediumDec variant and unsigned integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        template<IntegerType IntType= unsigned int>
        auto& UIntMultOpV1(const IntType& rValue)
		{
			//Add Code here
		}

        /// <summary>
        /// Multiplication operation between MediumDec variant and integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        template<IntegerType IntType= signed int>
        auto& IntMultOpV1(const IntType& rValue)
		{
			//Add Code here
		}

        /// <summary>
        /// Multiplication operation between MediumDec variant and unsigned integer values
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase</returns>
        template<IntegerType IntType= unsigned int>
        auto MultByUIntV1(const IntType& rValue)
		{
			//Add Code here
		}

        /// <summary>
        /// Multiplication operation between MediumDec variant and integer values
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase</returns>
        template<IntegerType IntType= signed int>
        auto MultByIntV1(const IntType& rValue)
		{
			//Add Code here
		}


public:

        constexpr auto UIntMultOpV1 = UIntMultOpV1<unsigned int>;
        constexpr auto IntMultOpV1 = IntMultOpV1<signed int>;
        constexpr auto UnsignedIntMultOp = UIntMultOpV1<signed int>;
        constexpr auto UInt64MultOp = UIntMultOpV1<unsigned long long>;
        constexpr auto Int64MultOp = IntMultOpV1<signed long long>;
	
        constexpr auto MultByUInt = MultByUIntV1<unsigned int>;
        constexpr auto MultByInt = MultByIntV1<signed int>;
        constexpr auto UnsignedMultByInt = MultByUInt<signed int>;
        constexpr auto MultByUInt64 = MultByUInt<;
        constexpr auto MultByInt64 = BasicMultByInt64;
        constexpr auto UnsignedMultByInt64 = MultByUInt<signed long long>;
		
        constexpr auto MultByUInt8 = BasicMultByUInt8;
        constexpr auto MultByInt8 = BasicMultByInt8;
        constexpr auto MultByUInt16 = BasicMultByUInt16;
        constexpr auto MultByInt16 = BasicMultByInt16;

        /// <summary>
        /// Unsigned multiplication operation between MediumDec variants.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>auto&</returns>
        constexpr auto UnsignedMultOp

        /// <summary>
        /// Multiplication operation between MediumDec variants.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>auto&</returns>
        constexpr auto MultOp

        /// <summary>
        /// Unsigned multiplication operation between MediumDec variants.
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>auto</returns>
        constexpr auto MultByUnsigned

        /// <summary>
        /// Multiplication operation between MediumDec variants.
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>auto</returns>
        constexpr auto MultBy

        /// <summary>
        /// Multiplication operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecV2Base operator*(const MediumDecV2Base& self, const MediumDecV2Base& Value) { return self.MultBy(Value); }

        /// <summary>
        /// += operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecV2Base& operator*=(MediumDecV2Base& self, const MediumDecV2Base& Value) { return self.MultOp(Value); }
		
        /// <summary>
        /// Multition operation between MediumDecBase and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecV2Base operator*(const MediumDecV2Base& self, const signed int& Value) { return self.MultByInt(Value); }
        friend MediumDecV2Base operator*(const MediumDecV2Base& self, const signed long long& Value) { return self.MultByInt64(Value); }
        friend MediumDecV2Base operator*(const MediumDecV2Base& self, const unsigned int& Value) { return self.MultByUInt(Value); }
        friend MediumDecV2Base operator*(const MediumDecV2Base& self, const unsigned long long& Value) { return self.MultByUInt64(Value); }
		
        friend MediumDecV2Base operator*(const signed int& lValue, const MediumDecV2Base& rValue) { return rValue.MultByInt(lValue); }
        friend MediumDecV2Base operator*(const signed long long& lValue, const MediumDecV2Base& rValue) { return rValue.MultByInt64(lValue); }
        friend MediumDecV2Base operator*(const unsigned int& lValue, const MediumDecV2Base& rValue) { return rValue.MultByUInt(lValue); }
        friend MediumDecV2Base operator*(const unsigned long long& lValue, const MediumDecV2Base& rValue) { return rValue.MultByUInt64(lValue); }

        friend MediumDecV2Base operator*(const MediumDecV2Base& self, const signed char& Value) { return self.MultByInt8(Value); }
        friend MediumDecV2Base operator*(const MediumDecV2Base& self, const signed short& Value) { return self.MultByInt16(Value); }
        friend MediumDecV2Base operator*(const MediumDecV2Base& self, const unsigned char& Value) { return self.MultByUInt8(Value); }
        friend MediumDecV2Base operator*(const MediumDecV2Base& self, const unsigned short& Value) { return self.MultByUInt16(Value); }

        friend MediumDecV2Base operator*(const signed char& lValue, const MediumDecV2Base& rValue) { return rValue.MultByInt8(lValue); }
        friend MediumDecV2Base operator*(const signed short& lValue, const MediumDecV2Base& rValue) { return rValue.MultByInt16(lValue); }
        friend MediumDecV2Base operator*(const unsigned char& lValue, const MediumDecV2Base& rValue) { return rValue.MultByUInt8(lValue); }
        friend MediumDecV2Base operator*(const unsigned short& lValue, const MediumDecV2Base& rValue) { return rValue.MultByUInt16(lValue); }

        /// <summary>
        /// += operation between MediumDecBase and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecV2Base& operator*=(MediumDecV2Base& self, const signed int& Value) { return self.IntMultOp(Value); }
        friend MediumDecV2Base& operator*=(MediumDecV2Base& self, const signed long long& Value) { return self.Int64MultOp(Value); }
        friend MediumDecV2Base& operator*=(MediumDecV2Base& self, const unsigned int& Value) { return self.UIntMultOp(Value); }
        friend MediumDecV2Base& operator*=(MediumDecV2Base& self, const unsigned long long& Value) { return self.UInt64MultOp(Value); }

        friend MediumDecV2Base& operator*=(MediumDecV2Base& self, const signed char& Value) { return self.Int8MultOp(Value); }
        friend MediumDecV2Base& operator*=(MediumDecV2Base& self, const signed short& Value) { return self.Int16MultOp(Value); }
        friend MediumDecV2Base& operator*=(MediumDecV2Base& self, const unsigned char& Value) { return self.UInt8MultOp(Value); }
        friend MediumDecV2Base& operator*=(MediumDecV2Base& self, const unsigned short& Value) { return self.UInt16MultOp(Value); }

    #pragma endregion Other multiplication operations

    #pragma region Other addition operations
protected:

        /// <summary>
        /// Addition operation between MediumDecBase and unsigned integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        template<IntegerType IntType= unsigned int>
        auto& UIntAddOpV1(const IntType& rValue)
		{
			//Add Code here
		}

        /// <summary>
        /// Addition operation between MediumDec variant and Integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase&</returns>
        template<IntegerType IntType= signed int>
        auto& IntAddOpV1(const IntType& rValue)
		{
			//Add Code here
		}

        /// <summary>
        /// Addition operation between MediumDec variant and unsigned Integer values
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase</returns>
        template<IntegerType IntType= unsigned int>
        auto AddByUIntV1(const IntType& rValue)
		{
			//Add Code here
		}

        /// <summary>
        /// Addition operation between MediumDec variant and Integer values
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecBase</returns>
        template<IntegerType IntType= signed int>
        auto AddByIntV1(const IntType& rValue)
		{
			//Add Code here
		}

public:

        constexpr auto UIntAddOpV1 = UIntAddOpV1<unsigned int>;
        constexpr auto IntAddOpV1 = IntAddOpV1<signed int>;
        constexpr auto UnsignedIntAddOp = UIntAddOpV1<signed int>;
        constexpr auto UInt64AddOp = UIntAddOpV1<unsigned long long>;
        constexpr auto Int64AddOp = IntAddOpV1<signed long long>;
	
        constexpr auto AddByUInt = AddByUIntV1<unsigned int>;
        constexpr auto AddByInt = AddByIntV1<signed int>;
        constexpr auto UnsignedAddByInt = AddByUInt<signed int>;
        constexpr auto AddByUInt64 = AddByUInt<;
        constexpr auto AddByInt64 = BasicAddByInt64;
        constexpr auto UnsignedAddByInt64 = AddByUInt<signed long long>;
		
        constexpr auto AddByUInt8 = BasicAddByUInt8;
        constexpr auto AddByInt8 = BasicAddByInt8;
        constexpr auto AddByUInt16 = BasicAddByUInt16;
        constexpr auto AddByInt16 = BasicAddByInt16;

        /// <summary>
        /// Unsigned Addition operation between MediumDec variants.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>auto&</returns>
        auto& UnsignedAddOp(const auto& rValue)
		{
			//Add Code here
		}

        /// <summary>
        /// Addition operation between MediumDec variants.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>auto&</returns>
        auto& AddOp(const auto& rValue)
		{
			//Add Code here
		}

        /// <summary>
        /// Unsigned Addition operation between MediumDec variants.
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>auto</returns>
        auto AddByUnsigned(const auto& rValue)
		{
			//Add Code here
		}

        /// <summary>
        /// ___ operation between MediumDec variants.
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>auto</returns>
        auto AddBy(const auto& rValue)
		{
			//Add Code here
		}

        /// <summary>
        /// Addition operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecV2Base operator+(const MediumDecV2Base& self, const MediumDecV2Base& Value) { return self.AddBy(Value); }

        /// <summary>
        /// += operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecV2Base& operator+=(MediumDecV2Base& self, const MediumDecV2Base& Value) { return self.AddOp(Value); }
		
        /// <summary>
        /// Addition operation between MediumDecBase and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecV2Base operator+(const MediumDecV2Base& self, const signed int& Value) { return self.AddByInt(Value); }
        friend MediumDecV2Base operator+(const MediumDecV2Base& self, const signed long long& Value) { return self.AddByInt64(Value); }
        friend MediumDecV2Base operator+(const MediumDecV2Base& self, const unsigned int& Value) { return self.AddByUInt(Value); }
        friend MediumDecV2Base operator+(const MediumDecV2Base& self, const unsigned long long& Value) { return self.AddByUInt64(Value); }
		
        friend MediumDecV2Base operator+(const signed int& lValue, const MediumDecV2Base& rValue) { return rValue.AddByInt(lValue); }
        friend MediumDecV2Base operator+(const signed long long& lValue, const MediumDecV2Base& rValue) { return rValue.AddByInt64(lValue); }
        friend MediumDecV2Base operator+(const unsigned int& lValue, const MediumDecV2Base& rValue) { return rValue.AddByUInt(lValue); }
        friend MediumDecV2Base operator+(const unsigned long long& lValue, const MediumDecV2Base& rValue) { return rValue.AddByUInt64(lValue); }

        friend MediumDecV2Base operator+(const MediumDecV2Base& self, const signed char& Value) { return self.AddByInt8(Value); }
        friend MediumDecV2Base operator+(const MediumDecV2Base& self, const signed short& Value) { return self.AddByInt16(Value); }
        friend MediumDecV2Base operator+(const MediumDecV2Base& self, const unsigned char& Value) { return self.AddByUInt8(Value); }
        friend MediumDecV2Base operator+(const MediumDecV2Base& self, const unsigned short& Value) { return self.AddByUInt16(Value); }

        friend MediumDecV2Base operator+(const signed char& lValue, const MediumDecV2Base& rValue) { return rValue.AddByInt8(lValue); }
        friend MediumDecV2Base operator+(const signed short& lValue, const MediumDecV2Base& rValue) { return rValue.AddByInt16(lValue); }
        friend MediumDecV2Base operator+(const unsigned char& lValue, const MediumDecV2Base& rValue) { return rValue.AddByUInt8(lValue); }
        friend MediumDecV2Base operator+(const unsigned short& lValue, const MediumDecV2Base& rValue) { return rValue.AddByUInt16(lValue); }

        /// <summary>
        /// += operation between MediumDecBase and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecBase</returns>
        friend MediumDecV2Base& operator+=(MediumDecV2Base& self, const signed int& Value) { return self.IntAddOp(Value); }
        friend MediumDecV2Base& operator+=(MediumDecV2Base& self, const signed long long& Value) { return self.Int64AddOp(Value); }
        friend MediumDecV2Base& operator+=(MediumDecV2Base& self, const unsigned int& Value) { return self.UIntAddOp(Value); }
        friend MediumDecV2Base& operator+=(MediumDecV2Base& self, const unsigned long long& Value) { return self.UInt64AddOp(Value); }

        friend MediumDecV2Base& operator+=(MediumDecV2Base& self, const signed char& Value) { return self.Int8AddOp(Value); }
        friend MediumDecV2Base& operator+=(MediumDecV2Base& self, const signed short& Value) { return self.Int16AddOp(Value); }
        friend MediumDecV2Base& operator+=(MediumDecV2Base& self, const unsigned char& Value) { return self.UInt8AddOp(Value); }
        friend MediumDecV2Base& operator+=(MediumDecV2Base& self, const unsigned short& Value) { return self.UInt16AddOp(Value); }

    #pragma endregion Other addition operations

    #pragma region Other Subtraction Operations
protected:

        /// <summary>
        /// Subtraction operation between MediumDec variant and unsigned integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2Base&</returns>
        template<IntegerType IntType= unsigned int>
        auto& UIntSubOpV1(const IntType& rValue)
		{
			//Add Code here
		}

        /// <summary>
        /// Subtraction operation between MediumDec variant and integer values
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2Base&</returns>
        template<IntegerType IntType= signed int>
        auto& IntSubOpV1(const IntType& rValue)
		{
			//Add Code here
		}

        /// <summary>
        /// Subtraction operation between MediumDec variant and unsigned integer values
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2Base</returns>
        template<IntegerType IntType= unsigned int>
        auto SubByUIntV1(const IntType& rValue)
		{
			//Add Code here
		}

        /// <summary>
        /// Subtraction operation between MediumDec variant and integer values
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>MediumDecV2Base</returns>
        template<IntegerType IntType= signed int>
        auto SubByIntV1(const IntType& rValue)
		{
			//Add Code here
		}

public:

        constexpr auto UIntSubOpV1 = UIntSubOpV1<unsigned int>;
        constexpr auto IntSubOpV1 = IntSubOpV1<signed int>;
        constexpr auto UnsignedIntSubOp = UIntSubOpV1<signed int>;
        constexpr auto UInt64SubOp = UIntSubOpV1<unsigned long long>;
        constexpr auto Int64SubOp = IntSubOpV1<signed long long>;
	
        constexpr auto SubByUInt = SubByUIntV1<unsigned int>;
        constexpr auto SubByInt = SubByIntV1<signed int>;
        constexpr auto UnsignedSubByInt = SubByUInt<signed int>;
        constexpr auto SubByUInt64 = SubByUInt<;
        constexpr auto SubByInt64 = BasicSubByInt64;
        constexpr auto UnsignedSubByInt64 = SubByUInt<signed long long>;
		
        constexpr auto SubByUInt8 = BasicSubByUInt8;
        constexpr auto SubByInt8 = BasicSubByInt8;
        constexpr auto SubByUInt16 = BasicSubByUInt16;
        constexpr auto SubByInt16 = BasicSubByInt16;

        /// <summary>
        /// Unsigned subtraction operation between MediumDec variants.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>auto&</returns>
        auto& UnsignedSubOp(const auto& rValue)
		{
			//Add Code here
		}

        /// <summary>
        /// Subtraction operation between MediumDec variants.
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>auto&</returns>
        auto& SubOp(const auto& rValue)
		{
			//Add Code here
		}

        /// <summary>
        /// Unsigned subtraction operation between MediumDec variants.
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>auto</returns>
        auto SubByUnsigned(const auto& rValue)
		{
			//Add Code here
		}

        /// <summary>
        /// Subtraction operation between MediumDec variants.
        /// (Doesn't modifify owner object)
        /// </summary>
        /// <param name="rValue.">The right side Value</param>
        /// <returns>auto</returns>
        auto SubBy(const auto& rValue)
		{
			//Add Code here
		}

        /// <summary>
        /// Subtraction operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecV2Base</returns>
        friend MediumDecV2Base operator-(const MediumDecV2Base& self, const MediumDecV2Base& Value) { return self.SubtractBy(Value); }
		
        /// <summary>
        /// -= operation
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecV2Base</returns>
        friend MediumDecV2Base& operator-=(MediumDecV2Base& self, const MediumDecV2Base& Value) { return self.SubOp(Value); }
		
        /// <summary>
        /// Subtraction operation between MediumDecV2Base and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecV2Base</returns>
        friend MediumDecV2Base operator-(const MediumDecV2Base& self, const signed int& Value) { return self.SubtractByInt(Value); }
        friend MediumDecV2Base operator-(const MediumDecV2Base& self, const signed long long& Value) { return self.SubtractByInt64(Value); }
        friend MediumDecV2Base operator-(const MediumDecV2Base& self, const unsigned int& Value) { return self.SubtractByUInt(Value); }
        friend MediumDecV2Base operator-(const MediumDecV2Base& self, const unsigned long long& Value) { return self.SubtractByUInt64(Value); }
		
        friend MediumDecV2Base operator-(const signed int& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).SubtractBy(rValue); }
        friend MediumDecV2Base operator-(const signed long long& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).SubtractBy(rValue); }
        friend MediumDecV2Base operator-(const unsigned int& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).SubtractBy(rValue); }
        friend MediumDecV2Base operator-(const unsigned long long& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).SubtractBy(rValue); }

        friend MediumDecV2Base operator-(const MediumDecV2Base& self, const signed char& Value) { return self.SubtractByInt8(Value); }
        friend MediumDecV2Base operator-(const MediumDecV2Base& self, const signed short& Value) { return self.SubtractByInt16(Value); }
        friend MediumDecV2Base operator-(const MediumDecV2Base& self, const unsigned char& Value) { return self.SubtractByUInt8(Value); }
        friend MediumDecV2Base operator-(const MediumDecV2Base& self, const unsigned short& Value) { return self.SubtractByUInt16(Value); }

        friend MediumDecV2Base operator-(const signed char& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).SubtractBy(rValue); }
        friend MediumDecV2Base operator-(const signed short& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).SubtractBy(rValue); }
        friend MediumDecV2Base operator-(const unsigned char& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).SubtractBy(rValue); }
        friend MediumDecV2Base operator-(const unsigned short& lValue, const MediumDecV2Base& rValue) { return ((MediumDecV2Base)lValue).SubtractBy(rValue); }


        /// <summary>
        /// -= operation between MediumDecV2Base and Integer value.
        /// </summary>
        /// <param name="self">The left side value</param>
        /// <param name="Value">The right side value.</param>
        /// <returns>MediumDecV2Base</returns>
        friend MediumDecV2Base& operator-=(MediumDecV2Base& self, const signed int& Value) { return self.IntSubOp(Value); }
        friend MediumDecV2Base& operator-=(MediumDecV2Base& self, const signed long long& Value) { return self.Int64SubOp(Value); }
        friend MediumDecV2Base& operator-=(MediumDecV2Base& self, const unsigned int& Value) { return self.UIntSubOp(Value); }
        friend MediumDecV2Base& operator-=(MediumDecV2Base& self, const unsigned long long& Value) { return self.UInt64SubOp(Value); }

        friend MediumDecV2Base& operator-=(MediumDecV2Base& self, const signed char& Value) { return self.Int8SubOp(Value); }
        friend MediumDecV2Base& operator-=(MediumDecV2Base& self, const signed short& Value) { return self.Int16SubOp(Value); }
        friend MediumDecV2Base& operator-=(MediumDecV2Base& self, const unsigned char& Value) { return self.UInt8SubOp(Value); }
        friend MediumDecV2Base& operator-=(MediumDecV2Base& self, const unsigned short& Value) { return self.UInt16SubOp(Value); }

    #pragma endregion Other Subtraction Operations

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

        friend MediumDecV2Base operator+(const MediumDecV2Base& self, const ldouble& Value) { return self + (MediumDecV2Base)Value; }
        friend MediumDecV2Base operator-(const MediumDecV2Base& self, const ldouble& Value) { return self - (MediumDecV2Base)Value; }
        friend MediumDecV2Base operator*(const MediumDecV2Base& self, const ldouble& Value) { return self * (MediumDecV2Base)Value; }
        friend MediumDecV2Base operator/(const MediumDecV2Base& self, const ldouble& Value) { return self / (MediumDecV2Base)Value; }

        friend MediumDecV2Base operator+(const ldouble& Value, const MediumDecV2Base& self) { return (MediumDecV2Base)Value + self; }
        friend MediumDecV2Base operator-(const ldouble& Value, const MediumDecV2Base& self) { return (MediumDecV2Base)Value - self; }
        friend MediumDecV2Base operator*(const ldouble& Value, const MediumDecV2Base& self) { return (MediumDecV2Base)Value * self; }
        friend MediumDecV2Base operator/(const ldouble& Value, const MediumDecV2Base& self) { return (MediumDecV2Base)Value / self; }

    #pragma endregion Floating Operator Overrides

    #pragma region Other Operators
	
        /// <summary>
        /// Negative Unary Operator(Flips negative status)
        /// </summary>
        /// <param name="self">The self.</param>
        /// <returns>MediumDecV2Base</returns>
        MediumDecV2Base operator- ()
        {
			auto self = this;
            self.SwapNegativeStatus(); return self;
        } const

        /// <summary>
        /// ++MediumDecV2Base Operator
        /// </summary>
        /// <returns>MediumDecV2Base &</returns>
        MediumDecV2Base& operator ++()
        {
            if (DecimalHalf == 0)
                ++IntValue;
            else if (IntValue == NegativeRep)
                IntValue = MirroredInt::Zero;
            else
                ++IntValue;
            return *this;
        }

        /// <summary>
        /// ++MediumDecV2Base Operator
        /// </summary>
        /// <returns>MediumDecV2Base &</returns>
        MediumDecV2Base& operator --()
        {
            if (DecimalHalf == 0)
                --IntValue;
            else if (IntValue == 0)
                IntValue = NegativeRep;
            else
                --IntValue;
            return *this;
        }

        /// <summary>
        /// MediumDec Variant++ Operator
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
        MediumDecV2Base operator ++(int)
        {
            MediumDecV2Base tmp(*this);
            ++* this;
            return tmp;
        }

        /// <summary>
        /// MediumDec Variant-- Operator
        /// </summary>
        /// <returns>MediumDecV2Base</returns>
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
    #if !defined(AltNum_EnableImaginaryNum)
            if(IsNegative())
                throw "Can't display result of negative square root without imaginary number support";
    #endif
            if (DecimalHalf.Value == 0&&DecimalHalf.Flags==0)
            {
                auto value = this;
                bool AutoSetValue = true;
                switch (IntValue.Value)
                {
                case 1: value.IntValue = 1; break;
                case 4: value.IntValue = 2; break;
                case 9: value.IntValue = 3; break;
                case 16: value.IntValue = 4; break;
                case 25: value.IntValue = 5; break;
                case 36: value.IntValue = 6; break;
                case 49: value.IntValue = 7; break;
                case 64: value.IntValue = 8; break;
                case 81: value.IntValue = 9; break;
                case 100: value.IntValue = 10; break;
                case 121: value.IntValue = 11; break;
                case 144: value.IntValue = 12; break;
                case 169: value.IntValue = 13; break;
                case 196: value.IntValue = 14; break;
                case 225: value.IntValue = 15; break;
                case 256: value.IntValue = 16; break;
                case 289: value.IntValue = 17; break;
                case 324: value.IntValue = 18; break;
                case 361: value.IntValue = 19; break;
                case 400: value.IntValue = 20; break;
                case 1600: value.IntValue = 40; break;
                default:
                    AutoSetValue = false;
                    break;
                }
                if(AutoSetValue)
                {
    #if defined(AltNum_EnableImaginaryNum)
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
			return value.BasicSqrtOf(precision);
		}

protected:


public:

        /// <summary>
        /// Finds nTh Root of value based on https://www.geeksforgeeks.org/n-th-root-number/ code
        /// </summary>
        /// <param name="nValue">The nth root value.</param>
        /// <param name="precision">Precision level (smaller = more precise)</param>
        /// <returns>auto</returns>
        constexpr auto NthRoot = MediumDecBase::NthRoot;

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
        /// <returns>MediumDecBase</returns>
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
        /// <returns>MediumDecBase</returns>
		auto Log10Of()
		{
            auto self = x.ConvertAsNormType();
            return self.Log10OfV1();
		}
		
        /// <summary>
        /// Log Base 10 of Value
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>MediumDecBase</returns>
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
        /// <returns>MediumDecBase</returns>
        constexpr auto Log10OfInt = MediumDecBase::Log10OfInt;
		
        /// <summary>
        /// Log with Base of BaseVal of Value
        /// Based on http://home.windstream.net/okrebs/page57.html
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <param name="baseVal">The base of Log</param>
        /// <returns>MediumDecBase</returns>
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
        /// <returns>MediumDecBase</returns>
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
        /// <returns>MediumDecBase</returns>
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
        /// <returns>MediumDecBase</returns>
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
        /// <returns>MediumDecBase</returns>
        static auto Sin(const auto& Value)
        {
            if(DecimalHalf.Flags==PiRep)
            {
                auto self = Value.ConvertAsPiNum(repType);
                if(Value.IsNegative())
                {
                    IntValue.Value %= 2;
                    IntValue.Value = 2 - IntValue.Value;
                    if (Value.DecimalHalf != 0) { Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf; }
                }
                else
                    IntValue.Value %= 2;
                if(DecimalHalf==0)//0 or 1 Pi
                    return Zero;
                else if(DecimalHalf==500000000)
                {
                    if(IntValue==0)//)0.5 Pi
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
        /// <returns>MediumDecBase</returns>
        static auto Cos(const auto& Value)
        {
            if(DecimalHalf.Flags==PiRep)
            {
                auto self = Value.ConvertAsPiNum(repType);
                if(Value.IsNegative())
                {
                    IntValue.Value %= 2;
                    IntValue.Value = 2 - IntValue.Value;
                    if (Value.DecimalHalf != 0) { Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf; }
                }
                else
                    IntValue.Value %= 2;
                if(DecimalHalf==0)
                    if(IntValue==0)//)0
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
        /// <returns>MediumDecBase</returns>
        static auto Tan(const auto& Value)
        {
            if(DecimalHalf.Flags==PiRep)
            {
                auto self = Value.ConvertAsPiNum(repType);
                if(Value.IsNegative())
                {
                    IntValue.Value %= 2;
                    IntValue.Value = 2 - IntValue.Value;
                    if (Value.DecimalHalf != 0) { Value.DecimalHalf = DecimalOverflow - Value.DecimalHalf; }
                }
                else
                    IntValue.Value %= 2;
                if(DecimalHalf==0)//0 or 1 Pi
					return Zero;
                else if(DecimalHalf==500000000)//0.5 Pi or 1.5 Pi
				{
					if(IntValue==0)
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
        /// <returns>MediumDecBase</returns>
        constexpr auto ATan = MediumDecBase::ATan;

        /// <summary>
        /// atan2 calculation with self normalization
        /// Application: Used when one wants to compute the 4-quadrant arctangent of a complex number (or any number with x-y coordinates) with a self-normalizing function.
        /// Example Applications: digital FM demodulation, phase angle computations
        /// Code from http://dspguru.com/dsp/tricks/fixed-point-atan2-with-self-normalization/ with some slight edit to get working
        /// </summary>
        /// <param name="y">The y.</param>
        /// <param name="X">The x.</param>
        /// <returns>MediumDecBase</returns>
        constexpr auto ArcTan2 = MediumDecBase::ArcTan2;

        /// <summary>
        /// Get Sin from Value of angle.
        /// Formula code based on answer from https://stackoverflow.com/questions/38917692/sin-cos-funcs-without-math-h
        /// </summary>
        /// <param name="value">The target MediumDec variant value to perform function on.</param>
        /// <returns>MediumDecBase</returns>
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
        /// <returns>MediumDecBase</returns>
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
	    #if defined(AltNum_EnableApproachingValues)
        case RepType::ApproachingBottom:
			#ifdef AltNum_DisplayApproachingAsReal
            return ConvertToBasicString(RepType::ApproachingBottom);
			#else
            return (std::string)IntValue + ".0..01";
			#endif
            break;
        case RepType::ApproachingTop:
			#ifdef AltNum_DisplayApproachingAsReal
			return ConvertToBasicString(RepType::ApproachingTop);
			#else
            return (std::string)IntValue + ".9..9";
			#endif
            break;
        #endif
	#endif
	#if defined(AltNum_EnablePiRep)
        case RepType::PiNum:
            return BasicToStringOp()+"π";
            break;
	#endif
	#if defined(AltNum_EnableERep)
        case RepType::ENum:
            return BasicToStringOp()+"e";
            break;
	#endif

	#if defined(AltNum_EnableImaginaryNum)
        case RepType::INum:
            return BasicToStringOp()+"i";
            break;
	#endif
	#if defined(AltNum_EnableApproachingPi)
        case RepType::ApproachingBottomPi://equal to IntValue.0..01 Pi
			#ifdef AltNum_DisplayApproachingAsReal
			return ConvertToBasicString(RepType::ApproachingBottom)+"π";
			#else
            return (std::string)IntValue + ".0..01π";
			#endif
            break;
        case RepType::ApproachingTopPi://equal to IntValue.9..9 Pi
			#ifdef AltNum_DisplayApproachingAsReal
			return ConvertToBasicString(RepType::ApproachingTop)+"π";
			#else
            return (std::string)IntValue + ".9..9π";
			#endif
            break;
	#endif
	#if defined(AltNum_EnableApproachingE)
        case RepType::ApproachingBottomE://equal to IntValue.0..01 e
			#ifdef AltNum_DisplayApproachingAsReal
			return ConvertToBasicString(RepType::ApproachingBottom)+"e";
			#else
            return (std::string)IntValue + ".0..01e";
			#endif
            break;
        case RepType::ApproachingTopE://equal to IntValue.9..9 e
			#ifdef AltNum_DisplayApproachingAsReal
			return ConvertToBasicString(RepType::ApproachingTop)+"e";
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
			return ConvertToBasicString(RepType::ApproachingBottom)+"i";
			#else
            return (std::string)IntValue + ".0..01i";
			#endif
            break;
        case RepType::ApproachingImaginaryTop:
			#ifdef AltNum_DisplayApproachingAsReal
			return ConvertToBasicString(RepType::ApproachingTop)+"i";
			#else
            return (std::string)IntValue + ".9..9i";
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
			return ConvertToBasicString(repType);
            break;
        }
    }

    #pragma endregion String Function Source
}