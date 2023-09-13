#include "AltDec.hpp"
using AltDec = BlazesRusCode::AltDec;
using RepType = BlazesRusCode::AltDec::RepType;

#if defined(AltDec_UseMirroredInt)
	MirroredInt AltDec::NegativeRep = MirroredInt::NegativeZero;
#endif
    #pragma region ValueDefine Source
#if defined(AltNum_EnableApproachingValues)
    AltDec AltDec::AlmostOne = AltDec::ApproachingRightRealValue();
#endif
    AltDec AltDec::Pi = AltDec::PiValue();
    AltDec AltDec::One = AltDec::OneValue();
    AltDec AltDec::Two = AltDec::TwoValue();
    AltDec AltDec::NegativeOne = AltDec::NegativeOneValue();
    AltDec AltDec::Zero = AltDec::ZeroValue();
    AltDec AltDec::PointFive = AltDec::Point5Value();
    AltDec AltDec::JustAboveZero = AltDec::JustAboveZeroValue();
    AltDec AltDec::OneMillionth = AltDec::OneMillionthValue();
    AltDec AltDec::FiveThousandth = AltDec::FiveThousandthValue();
    AltDec AltDec::Minimum = AltDec::MinimumValue();
    AltDec AltDec::Maximum = AltDec::MaximumValue();
    AltDec AltDec::E = AltDec::EValue();
    AltDec AltDec::LN10 = AltDec::LN10Value();
    AltDec AltDec::LN10Mult = AltDec::LN10MultValue();
    AltDec AltDec::HalfLN10Mult = AltDec::HalfLN10MultValue();
    AltDec AltDec::TenMillionth = AltDec::TenMillionthValue();
    AltDec AltDec::FiveMillionth = AltDec::FiveMillionthValue();
    AltDec AltDec::FiveBillionth = AltDec::FiveBillionthValue();
    AltDec AltDec::OneGMillionth = AltDec::OneHundredMillionthValue();
    #if defined(AltNum_EnableNilRep)
    AltDec AltDec::Nil = AltDec::NilValue();
    #endif

    AltDec AltDec::PiNum = AltDec::PiNumValue();
    AltDec AltDec::ENum = AltDec::ENumValue();
    
#if defined(AltNum_EnableInfinityRep)
    AltDec AltDec::Infinity = AltDec::InfinityValue();
#endif
#if defined(AltNum_EnableApproachingValues)
    AltDec AltDec::NegativeInfinity = AltDec::NegativeInfinityValue();
    AltDec AltDec::ApproachingZero = AltDec::ApproachingZeroValue();
#endif
#if defined(AltNum_EnableNaN)
    AltDec AltDec::NaN = AltDec::NaNValue();
	AltDec AltDec::Undefined = AltDec::UndefinedValue();
#endif
    #pragma endregion ValueDefine Source
	
#if defined(AltNum_EnableImaginaryNum)
#endif

    std::string AltDec::BasicToStringOp()
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

    std::string AltDec::ToString()
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

    std::string AltDec::BasicToFullStringOp()
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

    std::string AltDec::ToFullString()
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