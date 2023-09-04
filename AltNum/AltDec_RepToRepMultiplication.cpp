#include "AltDec.hpp"
using AltDec = BlazesRusCode::AltDec;
using RepType = BlazesRusCode::AltDec::RepType;

void NormalRtRMultiplication(const RepType& RRep, AltDec& self, AltDec& Value)
{
	switch (RRep)
	{
		case RepType::NormalType://Fail safe for when converted before switch
			self.BasicMultOp(Value);
			break;
	#if defined(AltNum_EnablePiRep)
		case RepType::PiNum://X * (Y*Pi)
			self.BasicMultOp(Value);
			self.ExtraRep = AltDec::PiRep;
			break;
		#if defined(AltNum_EnablePiPowers)
		case RepType::PiPower://X * (Y*Pi)^-ExtraRep
			self.ExtraRep = Value.ExtraRep-1;
			self.BasicMultOp(Value);
			break;
		#endif
	#endif
    #if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
			self = AltDec::MixedFracRtRMult_WithNormal(self, Value);
			break;
        #if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi:
			self = AltDec::MixedPiFracRtRMult_WithNormal(self, Value);
			break;
        #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
			self = AltDec::MixedEFracRtRMult_WithNormal(self, Value);
			break;
        #endif
    #endif
    #if defined(AltNum_EnableImaginaryNum)
		case RepType::INum:
			self.BasicMultOp(Value);
			self.ExtraRep = AltDec::IRep;
			break;
        #if defined(AltNum_EnableAlternativeRepFractionals)
	        #if defined(AltNum_EnableDecimaledIFractionals)
		case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
            #else
		case RepType::IFractional://  IntValue/DecimalHalf*i Representation
            #endif
        #endif
        #if defined(AltNum_EnableMixedIFractional)
		case RepType::MixedI:
        #endif
        //Placeholder code(Converting to i rep for now)
        #if defined(AltNum_EnableAlternativeRepFractionals)||defined(AltNum_EnableMixedIFractional)
			Value.ConvertToNormalIRep(RRep);
			self.BasicMultOp(Value);
			self.ExtraRep = AltDec::IRep;
        #endif
    #endif
        default:
            self.CatchAllMultiplication(Value, RepType::NormalType, RRep);
            break;
    }
}

#if defined(AltNum_EnableFractionals)
void NumByDivisorRtRMultiplication(const RepType& RRep, AltDec& self, AltDec& Value)
{
    switch (RRep)
    {
        case RepType::NormalType://Later normalize fractional when integer when viable
            self.BasicMultOp(Value);
            break;
	#if defined(AltNum_EnablePiRep)&&!defined(AltNum_EnablePiPowers)
        case RepType::PiNum:
		#if defined(AltNum_EnableDecimaledPiFractionals)
            self.BasicMultOp(Value);
            self.ExtraRep *= -1;
		#else
            self.CatchAllMultiplication(Value, RepType::NumByDiv, RRep);
		#endif
            break;
	#endif
	#if defined(AltNum_EnableERep)
        case RepType::ENum:
		#if defined(AltNum_EnableDecimaledEFractionals)
            self.BasicMultOp(Value);
            self.ExtraRep *= -1;
		#else
            self.CatchAllMultiplication(Value, RepType::NumByDiv, RRep);
		#endif
            break;
	#endif
    #if defined(AltNum_EnableMixedFractional)
        case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
            self = AltDec::MixedFracRtRMult_WithNormal(self, Value);
            break;
    #endif
    #if defined(AltNum_EnableMixedPiFractional)
        case RepType::MixedPi:
			self = AltDec::MixedPiFracRtRMult_WithNormal(self, Value);
			break;
    #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
			self = AltDec::MixedEFracRtRMult_WithNormal(self, Value);
			break;
    #endif
	#if defined(AltNum_EnableImaginaryNum)
        case RepType::INum:
		#if defined(AltNum_EnableDecimaledIFractionals)
            self.BasicMultOp(Value);
            self.ExtraRep *= -1;
		#else
            self.BasicMultOp(Value);
            self.ConvertToNormType(RepType::NumByDiv);
            self.ExtraRep = AltDec::IRep;
		#endif
            break;
	#endif
        default:
            self.CatchAllMultiplication(Value, RepType::NumByDiv, RRep);
            break;
    }
}
#endif

#if defined(AltNum_EnablePiRep)
void PiNumRtRMultiplication(const RepType& RRep, AltDec& self, AltDec& Value)
{
	switch (RRep)
	{
		case RepType::NormalType:
			self.BasicMultOp(Value);
			break;
	#if defined(AltNum_EnablePiPowers)
		case RepType::PiPower://XPi * (Y*Pi)^-ExtraRep
			self.ExtraRep = Value.ExtraRep-1;
			self.BasicMultOp(Value);
			break;
	#endif
	#if defined(AltNum_EnableImaginaryNum)
		case RepType::INum:
			self.BasicMultOp(Value);
			self.ExtraRep = AltDec::IRep;
			self.BasicMultOp(AltDec::PiNum);
			break;
	#endif
	#if defined(AltNum_EnableFractionals)
		case RepType::NumByDiv:
		#if defined(AltNum_EnableDecimaledPiFractionals)//Convert result to PiNumByDiv
			self.ExtraRep = Value.ExtraRep;
			self.BasicMultOp(Value);
		#elif defined(AltNum_EnablePiFractional)
			if(self.DecimalHalf==0&&Value.DecimalHalf==0)//If both left and right side values are whole numbers, convert result into PiFractional
			{//Becoming IntValue/DecimalHalf*Pi Representation
				self.IntValue *= Value.IntValue;
				self.DecimalHalf = Value.ExtraRep;
				self.ExtraRep = PiByDivisorRep;
			}
			else
			{
				self /= Value.ExtraRep;
				self.BasicMultOp(Value);
			}
		#else//Else just keep as PiNum type
			self /= Value.ExtraRep;
			self.BasicMultOp(Value);
		#endif
		break;
	#endif
    #if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
			self = AltDec::MixedFracRtRMult_WithNormal(self, Value);
			break;
        #if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi:
			self = AltDec::MixedPiFracRtRMult_WithPiNum(self, Value);
			break;
        #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
			self = AltDec::MixedEFracRtRMult_WithNormal(self, Value);
			break;
        #endif

    #endif
        default:
            self.CatchAllMultiplication(Value, RepType::PiNum, RRep);
            break;
    }
}
#endif
#if defined(AltNum_EnablePiPowers)
void PiPowerRtRMultiplication(const RepType& RRep, AltDec& self, AltDec& Value)
{
    switch (RRep)
    {
        case RepType::NormalType:
            self.BasicMultOp(Value);
            break;
        case RepType::PiNum://(IntValue.DecimalHalf)*Pi^-ExtraRep representation
            --self.ExtraRep;
            self.BasicMultOp(Value);
            break;
    #if defined(AltNum_EnableMixedFractional)
        case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
            self = AltDec::MixedFracRtRMult_WithNormal(self, Value);
            break;
    #endif
    #if defined(AltNum_EnableMixedPiFractional)
        case RepType::MixedPi:
			self = AltDec::MixedPiFracRtRMult_WithPiPower(self, Value);
			break;
    #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
			self = AltDec::MixedEFracRtRMult_WithNormal(self, Value);
			break;
    #endif
        default:
            self.CatchAllMultiplication(Value, RepType::PiPower, RRep);
            break;
    }
}
#endif
#if defined(AltNum_EnablePiFractional)
void PiFractionalRtRMultiplication(const RepType& RRep, AltDec& self, AltDec& Value)
{
    switch (RRep)
    {
        case RepType::NormalType:
            if(Value.DecimalHalf==0)
                self.IntValue *= Value.IntValue;
            else
                self.CatchAllMultiplication(Value, LRep, RRep);
            break;
    #if defined(AltNum_EnableMixedFractional)
        case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
            self = AltDec::MixedFracRtRMult_WithNormal(self, Value);
            break;
    #endif
    #if defined(AltNum_EnableMixedPiFractional)
        case RepType::MixedPi:
			self = AltDec::MixedPiFracRtRMult_WithOther(self, Value);
			break;
    #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
			self = AltDec::MixedEFracRtRMult_WithOther(self, Value);
			break;
    #endif
        default:
            self.CatchAllMultiplication(Value, RepType::PiFractional, RRep);
            break;
    }
}
#endif

#if defined(AltNum_EnableERep)
void ENumRtRMultiplication(const RepType& RRep, AltDec& self, AltDec& Value)
{
    switch (RRep)
    {
        case RepType::NormalType://Fail safe for when converted before switch
            self.BasicMultOp(Value);
            break;
	#if defined(AltNum_EnablePiRep)
        case RepType::PiNum://Xe * (Y*Pi)
			Value.ConvertPiToNum();
			self.BasicMultOp(Value);
			break;
		#if defined(AltNum_EnablePiPowers)
        case RepType::PiPower://Xe * (Y*Pi)^-ExtraRep
			AltDec Res = Value;
			Res.BasicMultOp(self);
			Res.BasicMultOp(ENum);
			self.SetVal(Res);
			break;
		#endif
	#endif
	#if defined(AltNum_EnableFractionals)
        case RepType::NumByDiv:
		#if defined(AltNum_EnableAlternativeRepFractionals)
			#if defined(AltNum_EnableDecimaledEFractionals)//Convert result to PiNumByDiv
			self.ExtraRep = Value.ExtraRep;
			self.BasicMultOp(Value);
			#else
			if(self.DecimalHalf==0&&Value.DecimalHalf==0)//If both left and right side values are whole numbers, convert result into EFractional
			{//Becoming IntValue/DecimalHalf*e Representation
				self.IntValue *= Value.IntValue;
				self.DecimalHalf = Value.ExtraRep;
				self.ExtraRep = AltDec::EByDivisorRep;
			}
			else
			{
				self /= Value.ExtraRep;
				self.BasicMultOp(Value);
			}
			#endif
		#else//Else just keep as ENum type
			self /= Value.ExtraRep;
			self.BasicMultOp(Value);
		#endif
			break;
	#endif
	#if defined(AltNum_EnableImaginaryNum)
		case RepType::INum:
			self.BasicMultOp(Value);
			self.ConvertToNormType(RepType::ENum);
			self.ExtraRep = AltDec::IRep;
			break;
	#endif
    #if defined(AltNum_EnableMixedFractional)
        case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
            self = AltDec::MixedFracRtRMult_WithNormal(self, Value);
            break;
    #endif
    #if defined(AltNum_EnableMixedPiFractional)
        case RepType::MixedPi:
			self = AltDec::MixedPiFracRtRMult_WithNormal(self, Value);
			break;
    #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
			self = AltDec::MixedEFracRtRMult_WithNormal(self, Value);
			break;
    #endif
        default:
            self.CatchAllMultiplication(Value, RepType::ENum, RRep);
            break;
    }
}
#endif
#if defined(AltNum_EnableEFractional)
void EFractionalRtRMultiplication(const RepType& RRep, AltDec& self, AltDec& Value)
{
    switch (RRep)
    {
        case RepType::NormalType:
            if(Value.DecimalHalf==0)
                self.IntValue *= Value.IntValue;
            else
                self.CatchAllMultiplication(Value, RepType::EFractional, RRep);
            break;
    #if defined(AltNum_EnableMixedFractional)
        case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
            self = AltDec::MixedFracRtRMult_WithNormal(self, Value);
            break;
    #endif
    #if defined(AltNum_EnableMixedPiFractional)
        case RepType::MixedPi:
			self = AltDec::MixedPiFracRtRMult_WithOther(self, Value);
			break;
    #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
			self = AltDec::MixedEFracRtRMult_WithOther(self, Value);
			break;
    #endif
        default:
            self.CatchAllMultiplication(Value, RepType::EFractional, RRep);
            break;
    }
}
#endif

#if defined(AltNum_EnableDecimaledPiFractionals)|| defined(AltNum_EnableDecimaledEFractionals)
void PiOrEByDivMultiplication(const RepType& RRep, AltDec& self, AltDec& Value)
{
    switch (RRep)
    {
        case RepType::NormalType://ToDo:Normalize denom later
            self.BasicMultOp(Value);
            break;
    #if defined(AltNum_EnableMixedFractional)
        case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)/ExtraRep
            self = AltDec::MixedFracRtRMult_WithNormal(self, Value);
            break;
    #endif
    #if defined(AltNum_EnableMixedPiFractional)
        case RepType::MixedPi:
			self = AltDec::MixedPiFracRtRMult_WithNormal(self, Value);
			break;
    #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
			self = AltDec::MixedEFracRtRMult_WithNormal(self, Value);
			break;
    #endif
    #if defined(AltNum_EnableImaginaryNum)
        case RepType::INum:
            self.BasicMultOp(Value);
        #if defined(AltNum_EnableDecimaledPiFractionals)
            self.ConvertToNormType(RepType::PiNumByDiv);
        #else
            self.ConvertToNormType(RepType::ENumByDiv);
        #endif
            self.ExtraRep = AltDec::IRep;
            break;
    #endif
        default:
            Value.ConvertToNormType(RRep);
            self.BasicMultOp(Value);
            break;
    }
}
#endif

#if defined(AltNum_EnableApproachingValues)
void ApproachingBottomRtRMultiplication(const RepType& RRep, AltDec& self, AltDec& Value)
{
    switch (RRep)
    {
	#if defined(AltNum_EnableImaginaryNum)
		case RepType::INum:
			if(self.IntValue=0||self.IntValue==AltDec::NegativeRep)
			{
				self.IntValue = self.IntValue==0?AltDec::NegativeRep:0;
	#if defined(AltNum_EnableImaginaryInfinity)
				self.DecimalHalf = ApproachingImaginaryBottomRep;
	#else
				self.DecimalHalf = 1;
	#endif
				self.ExtraRep = AltDec::IRep;
			}
			else
			{
				self.DecimalHalf = 1;
				self.BasicMultOp(Value);
				self.ExtraRep = AltDec::IRep;
			}
			break;
		#if defined(AltNum_EnableAlternativeRepFractionals)
			#if defined(AltNum_EnableDecimaledIFractionals)
		case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
			#else
		case RepType::IFractional://  IntValue/DecimalHalf*i Representation	
			#endif
			#if defined(AltNum_EnableMixedIFractional)
		case RepType::MixedI:
			#endif
			if(self.IntValue=0||self.IntValue==AltDec::NegativeRep)
			{
				self.IntValue = self.IntValue==0?AltDec::NegativeRep:0;
			#if defined(AltNum_EnableImaginaryInfinity)
				self.DecimalHalf = ApproachingImaginaryBottomRep;
			#else
				self.DecimalHalf = 1;
			#endif
				self.ExtraRep = AltDec::IRep;
			}
			else
			{
				Value.ConvertToNormalIRep(RRep);
				self.DecimalHalf = 1;
				self.BasicMultOp(Value);
				self.ExtraRep = AltDec::IRep;
			}
		break;
		#endif
	#endif
		default:
			self.CatchAllMultiplication(Value, RepType::ApproachingBottom, RRep);
			break;
    }
}

	#if !defined(AltNum_DisableApproachingTop)
void ApproachingTopRtRMultiplication(const RepType& RRep, AltDec& self, AltDec& Value)
{
    //switch (RRep)
    //{
    //  default:
            self.CatchAllMultiplication(Value, RepType::ApproachingTop, RRep);
    //      break;
    //}
}
	#endif
#endif

#if defined(AltNum_EnableApproachingDivided)
void ApproachingMidLeftRtRMultiplication(const RepType& RRep, AltDec& self, AltDec& Value)
{
}

	#if !defined(AltNum_DisableApproachingTop)
void ApproachingMidRightRtRMultiplication(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
#endif


#if defined(AltNum_EnableImaginaryNum)
void INumRtRMultiplication(const RepType& RRep, AltDec& self, AltDec& Value)
{
	switch (RRep)
    {
        case RepType::NormalType:
            self.BasicMultOp(Value);
            break;
        case RepType::INum:
            self.BasicMultOp(Value);
            self.ExtraRep = 0;
            break;
    #if defined(AltNum_EnableDecimaledIFractionals)
        case RepType::INumByDiv:
    #elif defined(AltNum_EnableIFractionals)
        case RepType::IFractional:
    #endif
	#if defined(AltNum_EnableMixedIFractional)
        case RepType::MixedI:
    #endif
            Value.ConvertToNormalIRep(RRep);
            self.ExtraRep = 0;
            self.BasicMultOp(-Value);
            break;
        default:
            Value.ConvertToNormType(RRep);
            self.BasicMultOp(Value);
            break;
    }
}
#endif

#if defined(AltNum_EnableDecimaledIFractionals)
void INumByDivisorRtRMultiplication(const RepType& RRep, AltDec& self, AltDec& Value)
{
    switch (RRep)
    {
        case RepType::NormalType:
            self.BasicMultOp(Value);
            break;
        case RepType::INum://Convert result into NumByDiv
            self.BasicMultOp(-Value);
            self.ExtraRep *= -1;
            break;
	#if defined(AltNum_EnableMixedIFractional)
        case RepType::MixedI:
            Value.ConvertToNormalIRep(RepType::MixedI);
            self.BasicMultOp(-Value);
            self.ExtraRep *= -1;
            break;
    #endif
        default:
            Value.ConvertToNormType(&RRep);
            self.BasicMultOp(Value);
    }
}
#elif defined(AltNum_EnableIFractional)
void IFractionalRtRMultiplication(const RepType& RRep, AltDec& self, AltDec& Value)
{
    switch (RRep)
    {
        case RepType::NormalType:
            self.ConvertToNormalIRep(RepType::IFractional);
            self.BasicMultOp(Value);
            break;
        case RepType::INum://Convert result into NumByDiv
            self.ExtraRep = self.DecimalHalf;
            self.DecimalHalf = 0;
            self.BasicMultOp(-Value);
            break;
	#if defined(AltNum_EnableMixedIFractional)
        case RepType::MixedI:
            Value.ConvertToNormalIRep(RepType::MixedI);
            self.ExtraRep = self.DecimalHalf;
            self.DecimalHalf = 0;
            self.BasicMultOp(-Value);
            break;
    #endif
        default:
            Value.ConvertToNormType(RRep);
            self.BasicMultOp(Value);
        }
}
#endif

#if defined(AltNum_EnableApproachingI)
void ApproachingImaginaryBottomRtRMultiplication(const RepType& RRep, AltDec& self, AltDec& Value)
{
}

	#if !defined(AltNum_DisableApproachingTop)
void ApproachingImaginaryTMultOpRtRMultiplication(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	#endif
	#if defined(AltNum_EnableApproachingDivided)
void ApproachingImaginaryMidLeftRtRMultiplication(const RepType& RRep, AltDec& self, AltDec& Value)
{
}

	    #if !defined(AltNum_DisableApproachingTop)
void ApproachingImaginaryMidRightRtRMultiplication(const RepType& RRep, AltDec& self, AltDec& Value)
{
}
	    #endif
    #endif
#endif

#if defined(AltNum_EnableMixedFractional)
void MixedFracRtRMultiplication(const RepType& RRep, AltDec& self, AltDec& Value)
{
    switch (RRep)
    {
/* //Unfinished code
        case RepType::NormalType:
            if(Value.DecimalHalf==0)
            {
                self.IntValue *= Value.IntValue;
                self.DecimalHalf *= Value.IntValue;
                int invertedDecimalHalf = -self.DecimalHalf
                if(invertedDecimalHalf >= self.ExtraRep)
                {
                    int divRes = invertedDecimalHalf / self.ExtraRep;
                    int C = invertedDecimalHalf - self.ExtraRep * divRes;
                    if(C==0)//If no longer a mixed fraction so instead convert into NormalType
                    {
                        throw "ToDo:Finish this code later";
                    }
                    else
                    {
                        throw "ToDo:Finish this code later";
                    }
                }
                else
                {
                    throw "ToDo:Finish this code later";
                }
            }
            else//Convert result as NumByDiv
            {
                throw "ToDo:Finish this code later";
            }
*/
        case RepType::NormalType:
    #if defined(AltNum_EnablePiRep)
        case RepType::PiNum:
		#if defined(AltNum_EnablePiPowers)
        case RepType::PiPower:
		#endif
	#endif
	#if defined(AltNum_EnableENum)
        case RepType::ENum:
	#endif
            self = AltDec::MixedFracRtRMult_WithNormal(Value, self);
            break;
    //ToDo::replace Mixed Pi/E code with more precise code later
	#if defined(AltNum_EnableMixedPiFractional)
        case RepType::MixedPi:
            AltDec::MixedPiFracRtRMult_WithOther(Value, self);
            break;
	#else
        case RepType::MixedE:
            AltDec::MixedEFracRtRMult_WithOther(Value, self);
            break;
	#endif
	#if defined(AltNum_EnableMixedIFractional)
        case RepType::MixedI:
            throw "ToDo: Add code here later";
            break;
	#endif
        default:
            self.CatchAllMultiplication(Value, RepType::MixedFrac, RRep);
            break;
    }
}
#endif
#if defined(AltNum_EnableMixedPiFractional)||defined(AltNum_EnableMixedEFractional)
void MixedPiOrERtRMultiplication(const RepType& RRep, AltDec& self, AltDec& Value)
{
    switch (RRep)
    {
/*
        case RepType::NormalType:
            if(Value.DecimalHalf==0)
            {
                self.IntValue *= Value.IntValue;
                self.DecimalHalf *= Value.IntValue;
                if(self.DecimalHalf <= self.ExtraRep)
                {//(-4 - -3)*1 = -1;
                    int divRes = self.DecimalHalf / self.ExtraRep;
                    int C = self.DecimalHalf - self.ExtraRep * divRes;
                    if(C==0)//If no longer a mixed fraction so instead convert into PiNum/ENum
                    {
                        self.DecimalHalf = 0;
        #if defined(AltNum_EnableMixedPiFractional)
                        self.ExtraRep = PiRep;
        #else
                        self.ExtraRep = ERep;
        #endif
                        if(self.IntValue<0)
                            self.IntValue -= divRes;
                        else
                            self.IntValue += divRes;
                    }
                    else
                    {
                        throw "ToDo:Finish this code later";
                    }
                }
            }
            else//Convert result as NumByDiv, PiNumByDiv, or ENumByDiv
            {
                throw "ToDo:Finish this code later";
            }
            break;
*/
#if defined(AltNum_EnableMixedPiFractional)
        case RepType::NormalType:
	#if defined(AltNum_EnableENum)
        case RepType::ENum:
	#endif
            self = AltDec::MixedPiFracRtRMult_WithNormal(Value, self);
            break;
    #if defined(AltNum_EnablePiRep)
        case RepType::PiNum:
            self = AltDec::MixedPiFracRtRMult_WithPiNum(Value, self);
            break;
		#if defined(AltNum_EnablePiPowers)
        case RepType::PiPower:
            self = AltDec::MixedPiFracRtRMult_WithPiPower(Value, self);
            break;
		#endif
	#endif
    #if defined(AltNum_EnablePiFractional)
        case RepType::PiFractional:
            MixedPiFracRtRMult_WithOther(Value, self);
            break;
    #endif
    #if defined(AltNum_EnableEFractional)
        case RepType::EFractional:
            AltDec::MixedPiFracRtRMult_WithOther(Value, self);
            break;
    #endif
    #if defined(AltNum_EnableFractionals)
		case RepType::NumByDiv:
		#if defined(AltNum_EnableDecimaledPiFractionals)
		case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
		#elif defined(AltNum_EnableDecimaledEFractionals)
		case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
		#endif
            AltDec::MixedPiFracRtRMult_WithNormal(Value, self);
			break;
    #endif
#else
        case RepType::NormalType:
    #if defined(AltNum_EnablePiRep)
        case RepType::PiNum:
		#if defined(AltNum_EnablePiPowers)
        case RepType::PiPower:
		#endif
	#endif
	#if defined(AltNum_EnableENum)
        case RepType::ENum:
	#endif
            self = AltDec::MixedEFracRtRMult_WithNormal(Value, self);
            break;
    #if defined(AltNum_EnablePiFractional)
        case RepType::PiFractional:
            MixedEFracRtRMult_WithOther(Value, self);
            break;
    #endif
    #if defined(AltNum_EnableEFractional)
        case RepType::EFractional:
            MixedEFracRtRMult_WithOther(Value, self);
            break;
    #endif
    #if defined(AltNum_EnableFractionals)
		case RepType::NumByDiv:
		#if defined(AltNum_EnableDecimaledPiFractionals)
		case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
		#elif defined(AltNum_EnableDecimaledEFractionals)
		case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
		#endif
            MixedEFracRtRMult_WithNormal(Value, self);
			break;
    #endif
#endif
        default:
        #if defined(AltNum_EnableMixedPiFractional)
            self.ConvertToNormType(RepType::MixedPi);
        #else
            self.ConvertToNormType(RepType::MixedE);
        #endif
            Value.ConvertToNormType(RRep);
            self.BasicMultOp(Value);
    }
}
#elif defined(AltNum_EnableMixedIFractional)
void MixedIRtRMultiplication(const RepType& RRep, AltDec& self, AltDec& Value)
{
    switch (RRep)
    {
        case RepType::INum:
        #if defined(AltNum_EnableDecimaledIFractionals)
        case RepType::INumByDiv:
        #elif defined(AltNum_EnableFractionals)
        case RepType::IFractional:
        #endif
            self.CatchAllImaginaryMultiplication(Value, LRep, RRep);
        default:
            self.ConvertToNormalIRep(RepType::MixedI);
            Value.ConvertToNormType(RRep);
            self.BasicMultOp(Value);
    }
}
#endif

inline void BlazesRusCode::AltDec::RepToRepMultOp(RepType& LRep, RepType& RRep, AltDec& self, AltDec& Value)
{
    //LRep Overrides
#if defined(AltNum_EnableUndefinedButInRange)||defined(AltNum_EnableImaginaryNum)
    switch(LRep)
    {
    #if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
        case RepType::UndefinedButInRange:
			throw "UndefinedButInRange MultOperations not supported yet(from left side)"; return;
            break;
    #endif
	#if defined(AltNum_EnableImaginaryNum)
		case RepType::INum:
		#if defined(AltNum_EnableAlternativeRepFractionals)
			#if defined(AltNum_EnableDecimaledIFractionals)
		case RepType::INumByDiv://(Value/(ExtraRep*-1))*i Representation
			#else
		case RepType::IFractional://  IntValue/DecimalHalf*i Representation
            #endif
		#endif
        #if defined(AltNum_EnableMixedIFractional)
		case RepType::MixedI:
        #endif
            AltDec::MultOp_LRepImaginaryOverride(LRep, RRep, self, Value);
			break;
    #endif
		default:
			break;
	}
#endif
    //RRep Overrides before Main RtR Code
	switch (RRep)
	{
	#if defined(AltNum_EnableApproachingValues)
		case RepType::ApproachingBottom:
		{
			if (Value.IntValue == 0)
			{
				self.IntValue = self.IntValue < 0 ? NegativeRep : 0;
				self.DecimalHalf = ApproachingBottomRep;
				self.ExtraRep = 0;
				return;
			}
			else
			{
				Value.DecimalHalf = 1;
				RRep = RepType::NormalType;
			}
		}
		break;

		case RepType::ApproachingTop:
		{
			Value.DecimalHalf = 999999999;
			Value.ExtraRep = 0;
			RRep = RepType::NormalType;
		}
		break;
		#if defined(AltNum_EnableApproachingDivided)
		case RepType::ApproachingMidLeft:
			Value.ConvertToNormType(RepType::ApproachingMidLeft);
			RRep = RepType::NormalType;
			break;
		case RepType::ApproachingMidRight:
			Value.ConvertToNormType(RepType::ApproachingMidRight);
			RRep = RepType::NormalType;
			break;
		#endif
	#endif
    #if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
        case RepType::UndefinedButInRange:
			throw "UndefinedButInRange MultOperations not supported yet(from right side)";
            break;
    #endif
	#if defined(AltNum_EnableNaN)
		case RepType::Undefined:
		case RepType::NaN:
			throw "Can't perform MultOperations with NaN or Undefined number";
			break;
	#endif
		default:
			break;
	}
    switch (LRep)//Main switch block starts here
    {
		case RepType::NormalType:
			NormalRtRMultiplication(RRep, self, Value); break;
#if defined(AltNum_EnablePiRep)
		case RepType::PiNum:
			PiNumRtRMultiplication(RRep, self, Value); break;
    #if defined(AltNum_EnablePiPowers)
		case RepType::PiPower:
			PiPowerRtRMultiplication(RRep, self, Value); break;
    #endif
#endif
#if defined(AltNum_EnableERep)
		case RepType::ENum:
			ENumRtRMultiplication(RRep, self, Value); break;
#endif
#if defined(AltNum_EnableApproachingValues)
		case RepType::ApproachingBottom:
			ApproachingBottomRtRMultiplication(RRep, self, Value); break;
	#if defined(AltNum_DisableApproachingTop)
		case RepType::ApproachingTop:
            self.CatchAllMultiplication(Value, RepType::ApproachingTop, RRep); break;
			//ApproachingTopRtRMultiplication(RRep, self, Value); break;
	#endif
	#if defined(AltNum_EnableApproachingDivided)
		case RepType::ApproachingMidLeft:
			//ApproachingMidLeftRtRMultiplication(RRep, self, Value); break;
		#if defined(AltNum_DisableApproachingTop)
		case RepType::ApproachingMidRight:
			//ApproachingMidRightRtRMultiplication(RRep, self, Value); break;
		#endif
            self.CatchAllMultiplication(Value, LRep, RRep); break;
	#endif
#endif
#if defined(AltNum_EnableFractionals)
		case RepType::NumByDiv:
			NumByDivisorRtRMultiplication(RRep, self, Value); break;
	#if defined(AltNum_EnableAlternativeRepFractionals)
		#if defined(AltNum_EnablePiFractional)
		case RepType::PiFractional://  IntValue/DecimalHalf*Pi Representation
			PiFractionalRtRMultiplication(RRep, self, Value); break;
		#endif
		#if defined(AltNum_EnableEFractional)
		case RepType::EFractional://  IntValue/DecimalHalf*e Representation
			EFractionalRtRMultiplication(RRep, self, Value); break;
		#endif

		#if defined(AltNum_EnableDecimaledPiFractionals)
		case RepType::PiNumByDiv://  (Value/(-ExtraRep))*Pi Representation
			PiOrEByDivMultiplication(RRep, self, Value); break;
		#elif defined(AltNum_EnableDecimaledEFractionals)
		case RepType::ENumByDiv://(Value/(-ExtraRep))*e Representation
			PiOrEByDivMultiplication(RRep, self, Value); break;
		#endif
	#endif
#endif
#if defined(AltNum_EnableImaginaryNum)
		case RepType::INum:
			INumRtRMultiplication(RRep, self, Value); break;
    #if defined(AltNum_EnableDecimaledIFractionals)
		case RepType::INumByDiv://(Value/(-ExtraRep))*i Representation
			INumByDivisorRtRMultiplication(RRep, self, Value); break;
    #elif defined(AltNum_EnableIFractional)
		case RepType::IFractional://  IntValue/DecimalHalf*i Representation
			IFractionalRtRMultiplication(RRep, self, Value); break;
    #endif
#endif
#if defined(AltNum_EnableMixedFractional)
		case RepType::MixedFrac://IntValue +- (DecimalHalf*-1)
			MixedFracRtRMultiplication(RRep, self, Value); break;
    #if defined(AltNum_EnableMixedPiFractional)
		case RepType::MixedPi://(IntValue<0?(IntValue + DecimalHalf):(IntValue -DecimalHalf))/-ExtraRep)
			MixedPiOrERtRMultiplication(RRep, self, Value); break;
    #elif defined(AltNum_EnableMixedEFractional)
		case RepType::MixedE:
			MixedPiOrERtRMultiplication(RRep, self, Value); break;
	#elif defined(AltNum_EnableMixedIFractional)
		case RepType::MixedI:
			MixedIRtRMultiplication(RRep, self, Value); break;
    #endif
#endif
	#if defined(AltNum_EnableNaN)
		case RepType::Undefined:
		case RepType::NaN:
			throw "Can't perform RtRMultiplicationerations with NaN or Undefined number";
			break;
	#endif
		default:
			throw AltDec::RepTypeAsString(LRep)+" RepType multiplication with"+AltDec::RepTypeAsString(RRep)+"not supported yet";
			break;
	}
}