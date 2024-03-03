#include "AltDecBase.hpp"
using AltDecBase = BlazesRusCode::AltDecBase;
using RepType = BlazesRusCode::AltDecBase::RepType;

#if defined(AltDec_UseMirroredInt)
	MirroredInt AltDecBase::NegativeRep = MirroredInt::NegativeZero;
#endif
    #pragma region ValueDefine Source
#if defined(AltNum_EnableApproachingValues)
    AltDecBase AltDecBase::AlmostOne = AltDecBase::ApproachingRightRealValue();
#endif
    AltDecBase AltDecBase::Pi = AltDecBase::PiValue();
    AltDecBase AltDecBase::One = AltDecBase::OneValue();
    AltDecBase AltDecBase::Two = AltDecBase::TwoValue();
    AltDecBase AltDecBase::NegativeOne = AltDecBase::NegativeOneValue();
    AltDecBase AltDecBase::Zero = AltDecBase::ZeroValue();
    AltDecBase AltDecBase::PointFive = AltDecBase::Point5Value();
    AltDecBase AltDecBase::JustAboveZero = AltDecBase::JustAboveZeroValue();
    AltDecBase AltDecBase::OneMillionth = AltDecBase::OneMillionthValue();
    AltDecBase AltDecBase::FiveThousandth = AltDecBase::FiveThousandthValue();
    AltDecBase AltDecBase::Minimum = AltDecBase::MinimumValue();
    AltDecBase AltDecBase::Maximum = AltDecBase::MaximumValue();
    AltDecBase AltDecBase::E = AltDecBase::EValue();
    AltDecBase AltDecBase::LN10 = AltDecBase::LN10Value();
    AltDecBase AltDecBase::LN10Mult = AltDecBase::LN10MultValue();
    AltDecBase AltDecBase::HalfLN10Mult = AltDecBase::HalfLN10MultValue();
    AltDecBase AltDecBase::TenMillionth = AltDecBase::TenMillionthValue();
    AltDecBase AltDecBase::FiveMillionth = AltDecBase::FiveMillionthValue();
    AltDecBase AltDecBase::FiveBillionth = AltDecBase::FiveBillionthValue();
    AltDecBase AltDecBase::OneGMillionth = AltDecBase::OneHundredMillionthValue();
    #if defined(AltNum_EnableNilRep)
    AltDecBase AltDecBase::Nil = AltDecBase::NilValue();
    #endif

    AltDecBase AltDecBase::PiNum = AltDecBase::PiNumValue();
    AltDecBase AltDecBase::ENum = AltDecBase::ENumValue();
    
#if defined(AltNum_EnableInfinityRep)
    AltDecBase AltDecBase::Infinity = AltDecBase::InfinityValue();
#endif
#if defined(AltNum_EnableApproachingValues)
    AltDecBase AltDecBase::NegativeInfinity = AltDecBase::NegativeInfinityValue();
    AltDecBase AltDecBase::ApproachingZero = AltDecBase::ApproachingZeroValue();
#endif
#if defined(AltNum_EnableNaN)
    AltDecBase AltDecBase::NaN = AltDecBase::NaNValue();
	AltDecBase AltDecBase::Undefined = AltDecBase::UndefinedValue();
#endif
    #pragma endregion ValueDefine Source
	
#if defined(AltNum_EnableImaginaryNum)
#endif

    std::string AltDecBase::BasicToStringOp()
    {
        std::string Value = IntHalfAsString();
        if (DecimalHalf != 0)
        {
            unsigned __int8 CurrentDigit;
            std::string DecBuffer = "";
            Value += ".";
            int CurrentSection = DecimalHalf;
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

    std::string AltDecBase::ToString()
    {
        RepType repType = GetRepType();
        switch (repType)
        {
			case RepType::NormalType:
				return BasicToStringOp();
				break;
        #if defined(AltNum_EnableFractionals)
            case RepType::NumByDiv:
                return BasicToStringOp()+"/"
                +VariableConversionFunctions::UnsignedIntToStringConversion(ExtraRep);
                break;
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
        #if defined(AltNum_EnableDecimaledPiFractionals)
            case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
                return BasicToStringOp()+"/"
                +VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"π";
                break;
        #elif defined(AltNum_EnablePiFractional)
            case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
                return IntHalfAsString() +"/"
                +VariableConversionFunctions::UnsignedIntToStringConversion(DecimalHalf)+"π";
                break;
        #endif
        #if defined(AltNum_EnableDecimaledEFractionals)
            case RepType::ENumByDiv://  (Value/(ExtraRep*-1))*e Representation
                return BasicToStringOp()+"/"
                +VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"e";
                break;
        #elif defined(AltNum_EnableEFractional)
            case RepType::EFractional://  IntValue/DecimalHalf*e Representation
                return IntHalfAsString() +"/"
                +VariableConversionFunctions::UnsignedIntToStringConversion(DecimalHalf)+"e";
                break;
        #endif
        #if defined(AltNum_EnableInfinityRep)
            case RepType::PositiveInfinity:
                return "∞";
                break;
            case RepType::NegativeInfinity:
                return "-∞";
                break;
        #endif
        #if defined(AltNum_EnableApproachingValues)
            case RepType::ApproachingBottom:
                #ifdef AltNum_DisplayApproachingAsReal
                ConvertToNormType(RepType::ApproachingBottom);
                return BasicToStringOp();
                #else
                return IntHalfAsString() + ".0..1";
                #endif
                break;
            #if !defined(AltNum_DisableApproachingTop)
            case RepType::ApproachingTop:
                #ifdef AltNum_DisplayApproachingAsReal
                ConvertToNormType(RepType::ApproachingTop);
                return BasicToStringOp();
                #else
                return IntHalfAsString() + ".9..9";
                #endif
                break;
            #endif
        #endif
        #if defined(AltNum_EnableApproachingDivided)
            case RepType::ApproachingMidLeft:
                #ifdef AltNum_DisplayApproachingAsReal
                ConvertToNormType(RepType::ApproachingMidLeft);
                return BasicToStringOp();
                #else
            {
				int DecimalPart = DecimalOverflowX/ExtraRep;//ToDo:check for truncated digits later
				return IntHalfAsString()+VariableConversionFunctions::UnsignedIntToStringConversion(DecimalPart)+"9..9";
			}
                #endif
				break;
            #if !defined(AltNum_DisableApproachingTop)
            case RepType::ApproachingMidRight:
                #ifdef AltNum_DisplayApproachingAsReal
                ConvertToNormType(RepType::ApproachingMidRight);
                return BasicToStringOp();
                #else
            {
				int DecimalPart = DecimalOverflowX/ExtraRep;//ToDo:check for truncated digits later
				return IntHalfAsString()+VariableConversionFunctions::UnsignedIntToStringConversion(DecimalPart)+"0..1";
			}
                #endif
				break;
            #endif
        #endif
        #if defined(AltNum_EnableImaginaryNum)
            case RepType::INum:
                return BasicToStringOp()+"i";
                break;
        #endif
        #if defined(AltNum_EnableDecimaledIFractionals)
            case RepType::INumByDiv://  (Value/(ExtraRep*-1))*i Representation
                return BasicToStringOp()+"/"
                +VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"i";
                break;
        #elif defined(AltNum_EnableIFractional)
            case RepType::IFractional://  IntValue/DecimalHalf*i Representation
                return IntHalfAsString() +"/"
                +VariableConversionFunctions::UnsignedIntToStringConversion(DecimalHalf)+"i";
                break;
        #endif
        #if defined(AltNum_EnableApproachingPi)
            case RepType::ApproachingTopPi://equal to IntValue.9..9 Pi
                #ifdef AltNum_DisplayApproachingAsReal
                ConvertToNormType(RepType::ApproachingTop);
                return BasicToStringOp()+"π";
                #else
                return IntHalfAsString() + ".9..9π";
                #endif
                break;
        #endif
        #if defined(AltNum_EnableApproachingE)
            case RepType::ApproachingTopE://equal to IntValue.9..9 e
                #ifdef AltNum_DisplayApproachingAsReal
                ConvertToNormType(RepType::ApproachingTop);
                return BasicToStringOp()+"e";
                #else
                return IntHalfAsString() + ".9..9e";
                #endif
                break;
        #endif
        #if defined(AltNum_EnableMixedFractional)
            case RepType::MixedFrac://IntValue +- (-DecimalHalf)/ExtraRep
                return IntHalfAsString()+" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
                +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(ExtraRep);
                break;
            #if defined(AltNum_EnableMixedPiFractional)
            case RepType::MixedPi://IntValue +- (-DecimalHalf/-ExtraRep)
                return IntHalfAsString()+" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
                +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"π";
                break;
            #elif defined(AltNum_EnableMixedEFractional)
            case RepType::MixedE://IntValue +- (-DecimalHalf/-ExtraRep)
                return IntHalfAsString()+" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
                +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"e";
                break;
            #elif defined(AltNum_EnableMixedIFractional)
            case RepType::MixedI://IntValue +- (-DecimalHalf/-ExtraRep)
                return IntHalfAsString()+" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
                +"/"+VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"i";
                break;
            #endif
        #endif
		#if defined(AltNum_EnableImaginaryInfinity)
			case RepType::PositiveImaginaryInfinity:
				return "∞";
				break;
			case RepType::NegativeImaginaryInfinity:
				return "-∞";
				break;
		#endif
    #if defined(AltNum_EnableApproachingI)
			case RepType::ApproachingImaginaryBottom:
				#ifdef AltNum_DisplayApproachingAsReal
				ConvertIRepToNormal(RepType::ApproachingImaginaryBottom);
				return BasicToStringOp()+"i";
				#else
				return IntHalfAsString() + ".0..1i";
				#endif
				break;
			#if !defined(AltNum_DisableApproachingTop)
			case RepType::ApproachingImaginaryTop:
				#ifdef AltNum_DisplayApproachingAsReal
				ConvertIRepToNormal(RepType::ApproachingImaginaryTop);
				return BasicToStringOp()+"i";
				#else
				return IntHalfAsString() + ".9..9i";
				#endif
				break;
			#endif
        #if defined(AltNum_EnableApproachingDivided)
            case RepType::ApproachingImaginaryMidLeft:
            #ifdef AltNum_DisplayApproachingAsReal
                ConvertIRepToNormal(RepType::ApproachingImaginaryMidLeft);
                return BasicToStringOp()+"i";
            #else
            {
				int DecimalPart = DecimalOverflowX/ExtraRep-1;//ToDo:check for truncated digits later
				return IntHalfAsString()+VariableConversionFunctions::UnsignedIntToStringConversion(DecimalPart)+"9..9i";
			}
            #endif
			break;
            #if !defined(AltNum_DisableApproachingTop)
            case RepType::ApproachingImaginaryMidRight:
            #ifdef AltNum_DisplayApproachingAsReal
                ConvertIRepToNormal(RepType::ApproachingImaginaryMidRight);
                return BasicToStringOp()+"i";
            #else
            {
				int DecimalPart = DecimalOverflowX/ExtraRep;//ToDo:check for truncated digits later
				return IntHalfAsString()+VariableConversionFunctions::UnsignedIntToStringConversion(DecimalPart)+"0..1i";
			}
            #endif
				break;
		#endif
    #endif
        #if defined(AltNum_EnableNaN)
            case RepType::Undefined:
                return "Undefined"; break;
            case RepType::NaN:
                return "NaN"; break;
        #endif
        #if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity(value format part uses for +- range, ExtraRepValue==UndefinedInRangeRep)
            case UndefinedButInRange:
                return "UndefinedButInRange";
                break;
            #if defined(AltNum_EnableWithinMinMaxRange)//Undefined except for ranged IntValue to DecimalHalf (ExtraRepValue==UndefinedInRangeMinMaxRep)
            case WithinMinMaxRange:
                return "WithinMinMaxRange";
                break;
            #endif
        #endif
        #if defined(AltNum_EnableNilRep)
            case RepType::Nil:
                return "Nil"; break;
        #endif
            default:
                ConvertToNormType(repType);
                return BasicToStringOp();
                break;
        }
    }

    std::string AltDecBase::BasicToFullStringOp()
    {
        std::string Value = IntHalfAsString();
        if (DecimalHalf != 0)
        {
            unsigned __int8 CurrentDigit;
            Value += ".";
            bool HasDigitsUsed = false;
            int CurrentSection = DecimalHalf;
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

    std::string AltDecBase::ToFullString()
    {
        RepType repType = GetRepType();
        switch (repType)
        {
			case RepType::NormalType:
				return BasicToFullStringOp();
				break;
		#if defined(AltNum_EnableFractionals)
            case RepType::NumByDiv:
				return BasicToFullStringOp()+"/"
				+VariableConversionFunctions::UnsignedIntToStringConversion(ExtraRep);
				break;
		#endif
		#if defined(AltNum_EnablePiRep)
			case RepType::PiNum:
				return BasicToFullStringOp()+"π";
				break;
		#endif
		#if defined(AltNum_EnableERep)
			case RepType::ENum:
				return BasicToFullStringOp()+"e";
				break;
		#endif
		#if defined(AltNum_EnableDecimaledEFractionals)
			case RepType::ENumByDiv://  (Value/(ExtraRep*-1))*e Representation
				return BasicToFullStringOp()+"/"
				+VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"e";
				break;
		#elif defined(AltNum_EnableEFractional)
			case RepType::EFractional://  IntValue/DecimalHalf*e Representation
				return IntHalfAsString()+"/"
				+VariableConversionFunctions::UnsignedIntToStringConversion(DecimalHalf)+"e";
				break;
		#endif
		#if defined(AltNum_EnableDecimaledPiFractionals)
			case RepType::PiNumByDiv://  (Value/(ExtraRep*-1))*Pi Representation
				return BasicToFullStringOp()+"/"
				+VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"π";
				break;
		#elif defined(AltNum_EnablePiFractional)
			case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
				return IntHalfAsString()+"/"
				+VariableConversionFunctions::UnsignedIntToStringConversion(DecimalHalf)+"π";
				break;
		#endif
		#if defined(AltNum_EnableInfinityRep)
			case RepType::PositiveInfinity:
				return "∞";
				break;
			case RepType::NegativeInfinity:
				return "-∞";
				break;
		#endif
		#if defined(AltNum_EnableApproachingValues)
			case RepType::ApproachingBottom:
				#ifdef AltNum_DisplayApproachingAsReal
				ConvertToNormType(RepType::ApproachingBottom);
				return BasicToFullStringOp();
				#else
				return IntHalfAsString() + ".0..1";
				#endif
				break;
			case RepType::ApproachingTop:
				#ifdef AltNum_DisplayApproachingAsReal
				ConvertToNormType(RepType::ApproachingTop);
				return BasicToFullStringOp();
				#else
				return IntHalfAsString() + ".9..9";
				#endif
				break;
		#endif
        #if defined(AltNum_EnableApproachingDivided)
            case RepType::ApproachingMidLeft:
                #ifdef AltNum_DisplayApproachingAsReal
                ConvertToNormType(RepType::ApproachingMidLeft);
                return BasicToStringOp();
                #else
            {
				int DecimalPart = DecimalOverflowX/ExtraRep-1;//ToDo:check for truncated digits later
				return IntHalfAsString()+VariableConversionFunctions::UnsignedIntToStringConversion(DecimalPart)+"9..9";
			}
                #endif
				break;
            #if !defined(AltNum_DisableApproachingTop)
            case RepType::ApproachingMidRight:
                #ifdef AltNum_DisplayApproachingAsReal
                ConvertToNormType(RepType::ApproachingMidRight);
                return BasicToStringOp();
                #else
            {
				int DecimalPart = DecimalOverflowX/ExtraRep;//ToDo:check for truncated digits later
				return IntHalfAsString()+VariableConversionFunctions::UnsignedIntToStringConversion(DecimalPart)+"0..1";
			}
                #endif
				break;
            #endif
        #endif
		#if defined(AltNum_EnableImaginaryNum)
			case RepType::INum:
				return BasicToFullStringOp()+"i";
				break;
		#endif
		#if defined(AltNum_EnableDecimaledIFractionals)
			case RepType::INumByDiv://  (Value/(ExtraRep*-1))*i Representation
				return BasicToFullStringOp()+"/"
				+VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"i";
				break;
		#elif defined(AltNum_EnableIFractional)
			case RepType::IFractional://  IntValue/DecimalHalf*i Representation
				return IntHalfAsString() +"/"
				+VariableConversionFunctions::UnsignedIntToStringConversion(DecimalHalf)+"i";
				break;
		#endif
		#if defined(AltNum_EnableApproachingPi)
			case RepType::ApproachingTopPi://equal to IntValue.9..9 Pi
				#ifdef AltNum_DisplayApproachingAsReal
				ConvertToNormType(RepType::ApproachingTop);
				return BasicToFullStringOp()+"π";
				#else
				return IntHalfAsString() + ".9..9π";
				#endif
				break;
		#endif
		#if defined(AltNum_EnableApproachingE)
			case RepType::ApproachingTopE://equal to IntValue.9..9 e
				#ifdef AltNum_DisplayApproachingAsReal
				ConvertToNormType(RepType::ApproachingTop);
				return BasicToFullStringOp()+"e";
				#else
				return IntHalfAsString() + ".9..9e";
				#endif
				break;
		#endif
		#if defined(AltNum_EnableImaginaryInfinity)
			case RepType::PositiveImaginaryInfinity:
				return "∞";
				break;
			case RepType::NegativeImaginaryInfinity:
				return "-∞";
				break;
		#endif
    #if defined(AltNum_EnableApproachingI)
			case RepType::ApproachingImaginaryBottom:
				#ifdef AltNum_DisplayApproachingAsReal
				ConvertIRepToNormal(RepType::ApproachingImaginaryBottom);
				return BasicToFullStringOp()+"i";
				#else
				return IntHalfAsString() + ".0..1i";
				#endif
				break;
			#if !defined(AltNum_DisableApproachingTop)
			case RepType::ApproachingImaginaryTop:
				#ifdef AltNum_DisplayApproachingAsReal
				ConvertIRepToNormal(RepType::ApproachingImaginaryTop);
				return BasicToFullStringOp()+"i";
				#else
				return IntHalfAsString() + ".9..9i";
				#endif
				break;
			#endif
        #if defined(AltNum_EnableApproachingDivided)
            case RepType::ApproachingImaginaryMidLeft:
            #ifdef AltNum_DisplayApproachingAsReal
                ConvertIRepToNormal(RepType::ApproachingImaginaryMidLeft);
                return BasicToFullStringOp()+"i";
            #else
            {
				int DecimalPart = DecimalOverflowX/ExtraRep-1;//ToDo:check for truncated digits later
				return IntHalfAsString()+VariableConversionFunctions::UnsignedIntToStringConversion(DecimalPart)+"9..9i";
			}
            #endif
			break;
            #if !defined(AltNum_DisableApproachingTop)
            case RepType::ApproachingImaginaryMidRight:
            #ifdef AltNum_DisplayApproachingAsReal
                ConvertIRepToNormal(RepType::ApproachingImaginaryMidRight);
                return BasicToFullStringOp()+"i";
            #else
            {
				int DecimalPart = DecimalOverflowX/ExtraRep;//ToDo:check for truncated digits later
				return IntHalfAsString()+VariableConversionFunctions::UnsignedIntToStringConversion(DecimalPart)+"0..1i";
			}
            #endif
				break;
		#endif
    #endif
		#if defined(AltNum_EnableMixedFractional)
			case RepType::MixedFrac://IntValue +- (-DecimalHalf)/ExtraRep
				return IntHalfAsString() +" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
				+"/"+VariableConversionFunctions::UnsignedIntToStringConversion(ExtraRep);
				break;
			#if defined(AltNum_EnableMixedPiFractional)
			case RepType::MixedPi://IntValue +- (-DecimalHalf/-ExtraRep)
				return IntHalfAsString() +" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
				+"/"+VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"π";
				break;
			#elif defined(AltNum_EnableMixedEFractional)
			case RepType::MixedE://IntValue +- (-DecimalHalf/-ExtraRep)
				return IntHalfAsString() +" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
				+"/"+VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"e";
				break;
			#elif defined(AltNum_EnableMixedIFractional)
			case RepType::MixedI://IntValue +- (-DecimalHalf/-ExtraRep)
				return IntHalfAsString() +" "+VariableConversionFunctions::UnsignedIntToStringConversion(-DecimalHalf)
				+"/"+VariableConversionFunctions::UnsignedIntToStringConversion(-ExtraRep)+"i";
				break;
			#endif
		#endif
		#if defined(AltNum_EnableNaN)
			case RepType::Undefined:
				return "Undefined"; break;
			case RepType::NaN:
				return "NaN"; break;
		#endif
		#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity(value format part uses for +- range, ExtraRepValue==UndefinedInRangeRep)
			case UndefinedButInRange:
				return "UndefinedButInRange";
				break;
			#if defined(AltNum_EnableWithinMinMaxRange)//Undefined except for ranged IntValue to DecimalHalf (ExtraRepValue==UndefinedInRangeMinMaxRep)
			case WithinMinMaxRange:
				return "WithinMinMaxRange";
				break;
			#endif
		#endif
		#if defined(AltNum_EnableNilRep)
			case RepType::Nil:
				return "Nil"; break;
		#endif
			default:
				ConvertToNormType(repType);
				return BasicToFullStringOp();
				break;
        }
    }
    #pragma endregion String Function Source