#include "AltDecBase.hpp"
using AltDecBase = BlazesRusCode::AltDecBase;

#if defined(AltNum_EnablePiRep)
    #if defined(AltNum_EnableFractionals)
//Convert from PiByDiv to NumByDivisor representation
inline void ConvertPiByDivToNumByDiv()
{
	__int64 SRep;
	__int64 divRes;
	if (DecimalHalf == 0)
	{
	    #if !defined(AltNum_EnableMirroredSection)
		bool IsNegative = IntHalf < 0;
		if (IsNegative)
			IntHalf *= -1;
	    #endif
		SRep = 3141592654;
		SRep *= IntHalf;
		//__int64 divRes = SRep / DecimalOverflowX;
		//__int64 C = SRep - DecimalOverflowX * divRes;
		divRes = SRep / DecimalOverflowX;
	    #if defined(AltNum_UseIntForDecimalHalf)
		DecimalHalf = (int)(SRep - DecimalOverflowX * divRes);
	    #else
		DecimalHalf.Value = (unsigned int)(SRep - DecimalOverflowX * divRes);
	    #endif
		if (divRes == 0 && IsNegative)
		{
			if (DecimalHalf == 0)
				IntHalf = 0;
			else
				IntHalf = NegativeRep;
		}
		else if (IsNegative)
			IntHalf = (int)-divRes;
		else
			IntHalf = (int)divRes;
	}
	else if (IntHalf == 0)
	{
		SRep = 3141592654;
		SRep *= DecimalHalf;
		divRes = SRep / 1000000000000000000;
	    #if defined(AltNum_UseIntForDecimalHalf)
		DecimalHalf = (int)((SRep - 1000000000000000000 * divRes) / DecimalOverflowX);
	    #else
		DecimalHalf.Value = (unsigned int)((SRep - 1000000000000000000 * divRes) / DecimalOverflowX);
	    #endif
	}
	    #if !defined(AltNum_EnableMirroredSection)
	else if (IntHalf == NegativeRep)
	{
		SRep = 3141592654;
		SRep *= DecimalHalf;
		divRes = SRep / 1000000000000000000;
		    #if defined(AltNum_UseIntForDecimalHalf)
		DecimalHalf = (int)((SRep - 1000000000000000000 * divRes) / DecimalOverflowX);
		    #else
		DecimalHalf.Value = (unsigned int)((SRep - 1000000000000000000 * divRes) / DecimalOverflowX);
		    #endif
		if (divRes == 0)
			IntHalf = NegativeRep;
		else
			IntHalf = (int)-divRes;
	}
	    #endif
	else
	{
	    #if !defined(AltNum_EnableMirroredSection)
		bool IsNegative = IntHalf < 0;
		if (IsNegative)
			IntHalf *= -1;
	    #endif
		SRep = DecimalOverflowX * IntHalf + DecimalHalf;
		SRep *= 3ll;//SRep holds __int64 version of X.Y * Z
		//X.Y *.V
		__int64 Temp03 = (__int64)IntHalf * 141592654ll;//Temp03 holds __int64 version of X *.V
		__int64 Temp04 = (__int64)DecimalHalf * 141592654ll;
		Temp04 /= FlaggedDec::DecimalOverflow;
		//Temp04 holds __int64 version of .Y * .V
		__int64 IntegerRep = SRep + Temp03 + Temp04;
		__int64 IntHalf = IntegerRep / FlaggedDec::DecimalOverflow;
		IntegerRep -= IntHalf * (__int64)FlaggedDec::DecimalOverflow;
	    #if defined(AltNum_UseIntForDecimalHalf)
		DecimalHalf = (signed int)IntegerRep;
	    #else
		DecimalHalf.Value = (unsigned int)IntegerRep;
	    #endif
	    #if !defined(AltNum_EnableMirroredSection)
		if (IntHalf == 0 && IsNegative)
		{
			IntHalf = NegativeRep;
		}
		else if (IsNegative)
			IntHalf = (int)-IntHalf;
		else
			IntHalf = (int)IntHalf;
	    #else
		IntHalf.Value = (unsigned int)IntHalf;
	    #endif
	}
	ExtraRep.Flag = 0;
}
    #endif

//Convert from PiByDiv to NormalType representation
void ConvertPiByDivToNorm()
{
	BasicIntDivOp(ExtraRep.Value);
    ConvertPiByDivToNumByDiv();
}
    #endif

#if defined(AltNum_EnablePowerOfRepresentation)
/* //Keep in header for template output
inline AltDecBase BlazesRusCode::AltDecBase::PiPowerNum(int powerExponent)
{
	ExtraRep = InitialExtraRep;
	AltDecBase PiSide = PiNum;
	PiSide.IntPowOp(powerExponent);
	return PiSide;
}
*/

inline void BlazesRusCode::AltDecBase::ConvertPiPowerToNum()
{
	int powerExponent = ExtraRep.Value;
	ExtraRep = InitialExtraRep;
	auto PiSide = PiPowerNum(powerExponent);
	BasicMultOp(PiSide);
}

inline void BlazesRusCode::AltDecBase::ConvertPiPowerToPiRep()
{
    #if defined(AltNum_EnableNegativePowerRep)
	if(ExtraRep<0){//Negative PiPowers
		int powerExponent = -ExtraRep.Value;
		SetAsOne();
		auto PiSide = PiPowerNum(powerExponent);
		BasicUnsignedDivOp(PiSide);
	} else {
	    int powerExponent = ExtraRep.Value - 1;
		DecimalHalf.Flags = 1;
        //Pi^-1 = 0.31
        //Pi^-1 = Pi^-2 * Pi
		auto PiSide = PiPowerNum(powerExponent);
		BasicUnsignedMultOp(PiSide);
	}
	#else
        unsigned int powerExponent = ExtraRep.Value - 1;
		DecimalHalf.Flags = 1;
        ExtraRep.IsAltRep = 0;
		auto PiSide = PiPowerNum(powerExponent);
		BasicUnsignedMultOp(PiSide);
    #endif
}
#endif

#endif

#if defined(AltNum_EnableERep)
    #if defined(AltNum_EnableFractionals)
//Convert from PiByDiv to NumByDivisor representation
inline void BlazesRusCode::AltDecBase::ConvertEByDivToNumByDiv()
{
	__int64 SRep;
	__int64 divRes;
	if (DecimalHalf == 0)
	{
	    #if !defined(AltNum_EnableMirroredSection)
		bool IsNegative = IntHalf < 0;
		if (IsNegative)
			IntHalf *= -1;
	    #endif
		SRep = 2718281828;
		SRep *= IntHalf;
		//__int64 divRes = SRep / DecimalOverflowX;
		//__int64 C = SRep - DecimalOverflowX * divRes;
		divRes = SRep / DecimalOverflowX;
	    #if defined(AltNum_UseIntForDecimalHalf)
		DecimalHalf = (int)(SRep - DecimalOverflowX * divRes);
	    #else
		DecimalHalf.Value = (unsigned int)(SRep - DecimalOverflowX * divRes);
	    #endif
		if (divRes == 0 && IsNegative)
		{
			if (DecimalHalf == 0)
				IntHalf = 0;
			else
				IntHalf = NegativeRep;
		}
		else if (IsNegative)
			IntHalf = (int)-divRes;
		else
			IntHalf = (int)divRes;
	}
	else if (IntHalf == 0)
	{
		SRep = 2718281828;
		SRep *= DecimalHalf;
		divRes = SRep / 1000000000000000000;
	    #if defined(AltNum_UseIntForDecimalHalf)
		DecimalHalf = (int)((SRep - 1000000000000000000 * divRes) / DecimalOverflowX);
	    #else
		DecimalHalf.Value = (unsigned int)((SRep - 1000000000000000000 * divRes) / DecimalOverflowX);
	    #endif
	}
	    #if !defined(AltNum_EnableMirroredSection)
	else if (IntHalf == NegativeRep)
	{
		SRep = 2718281828;
		SRep *= DecimalHalf;
		divRes = SRep / 1000000000000000000;
		    #if defined(AltNum_UseIntForDecimalHalf)
		DecimalHalf = (int)((SRep - 1000000000000000000 * divRes) / DecimalOverflowX);
		    #else
		DecimalHalf.Value = (unsigned int)((SRep - 1000000000000000000 * divRes) / DecimalOverflowX);
		    #endif
		if (divRes == 0)
			IntHalf = NegativeRep;
		else
			IntHalf = (int)-divRes;
	}
	    #endif
	else
	{
	    #if !defined(AltNum_EnableMirroredSection)
		bool IsNegative = IntHalf < 0;
		if (IsNegative)
			IntHalf *= -1;
	    #endif
		SRep = DecimalOverflowX * IntHalf + DecimalHalf;
		SRep *= 2ll;//SRep holds __int64 version of X.Y * Z
		//X.Y *.V
		__int64 Temp03 = (__int64)IntHalf * 718281828ll;//Temp03 holds __int64 version of X *.V
		__int64 Temp04 = (__int64)DecimalHalf * 718281828ll;
		Temp04 /= DecimalOverflow;
		//Temp04 holds __int64 version of .Y * .V
		__int64 IntegerRep = SRep + Temp03 + Temp04;
		__int64 IntHalf = IntegerRep / DecimalOverflow;
		IntegerRep -= IntHalf * DecimalOverflow;
	    #if defined(AltNum_UseIntForDecimalHalf)
		DecimalHalf = (signed int)IntegerRep;
	    #else
		DecimalHalf.Value = (unsigned int)IntegerRep;
	    #endif
	    #if !defined(AltNum_EnableMirroredSection)
		if (IntHalf == 0 && IsNegative)
		{
			IntHalf = NegativeRep;
		}
		else if (IsNegative)
			IntHalf = (int)-IntHalf;
		else
			IntHalf = (int)IntHalf;
	    #else
		IntHalf.Value = (unsigned int)IntHalf;
	    #endif
	}
	ExtraRep.Flag = 0;
}

//Convert from EByDiv to NormalType representation
void ConvertEByDivToNorm()
{
	BasicIntDivOp(ExtraRep.Value);
    ConvertEByDivToNumByDiv();
}
    #endif

#if defined(AltNum_EnablePowerOfRepresentation)
/*
inline AltDecBase BlazesRusCode::AltDecBase::EPowerNum(int powerExponent)
{
	AltDecBase ESide = ENum;
	ESide.IntPowOp(powerExponent);
	return ESide;
}
*/

inline void BlazesRusCode::AltDecBase::ConvertEPowerToNum()
{
	int powerExponent = ExtraRep.Value;
	ExtraRep = InitialExtraRep;
	AltDecBase ESide = EPowerNum(powerExponent);
	BasicMultOp(ESide);
}

inline void BlazesRusCode::AltDecBase::ConvertEPowerToERep()
{
    #if defined(AltNum_EnableNegativePowerRep)
	if(ExtraRep<0){//Negative PiPowers
		int powerExponent = -ExtraRep.Value;
		SetAsOne();
		auto ESide = EPowerNum(powerExponent);
		BasicUnsignedDivOp(ESide);
	} else {
	    int powerExponent = ExtraRep.Value - 1;
		DecimalHalf.Flags = 1;
		auto ESide = EPowerNum(powerExponent);
		BasicUnsignedMultOp(PiSide);
	}
	#else
        unsigned int powerExponent = ExtraRep.Value - 1;
		DecimalHalf.Flags = 1;
        ExtraRep.IsAltRep = 0;
		auto ESide = EPowerNum(powerExponent);
		BasicUnsignedMultOp(ESide);
    #endif
}
#endif

#endif

#if defined(AltNum_EnableIRep)

#endif
#if defined(AltNum_EnablePowerOfRepresentation)
	//To-Do:Code in ToPowerOf
	
/*
inline void BlazesRusCode::AltDecBase::ConvertToPowerOfToNormal()
{
    #if defined(AltNum_EnableNegativePowerRep)
	if(ExtraRep<0){//Negative PiPowers

	} else {

	}
	#else

    #endif
}
*/
#endif