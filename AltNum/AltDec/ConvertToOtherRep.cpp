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
		bool IsNegative = IntValue < 0;
		if (IsNegative)
			IntValue *= -1;
	    #endif
		SRep = 3141592654;
		SRep *= IntValue;
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
				IntValue = 0;
			else
				IntValue = NegativeRep;
		}
		else if (IsNegative)
			IntValue = (int)-divRes;
		else
			IntValue = (int)divRes;
	}
	else if (IntValue == 0)
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
	else if (IntValue == NegativeRep)
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
			IntValue = NegativeRep;
		else
			IntValue = (int)-divRes;
	}
	    #endif
	else
	{
	    #if !defined(AltNum_EnableMirroredSection)
		bool IsNegative = IntValue < 0;
		if (IsNegative)
			IntValue *= -1;
	    #endif
		SRep = DecimalOverflowX * IntValue + DecimalHalf;
		SRep *= 3ll;//SRep holds __int64 version of X.Y * Z
		//X.Y *.V
		__int64 Temp03 = (__int64)IntValue * 141592654ll;//Temp03 holds __int64 version of X *.V
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
			IntValue = NegativeRep;
		}
		else if (IsNegative)
			IntValue = (int)-IntHalf;
		else
			IntValue = (int)IntHalf;
	    #else
		IntValue.Value = (unsigned int)IntHalf;
	    #endif
	}
	ExtraRep.Flag = 0;
}

//Convert from PiByDiv to NormalType representation
void ConvertPiByDivToNorm()
{
	BasicIntDivOp(ExtraRep.Value);
    ConvertPiByDivToNumByDiv();
}
    #endif

inline void BlazesRusCode::AltDecBase::ConvertPiPowerToNum()
{
	int powerExponent = -ExtraRep;
	ExtraRep = 0;
	AltDecBase PiSide = PiNum;
	PiSide.IntPowOp(powerExponent);
	BasicMultOp(PiSide);
}

inline AltDecBase BlazesRusCode::AltDecBase::PiPowerNum(int powerExponent)
{
	ExtraRep = 0;
	AltDecBase PiSide = PiNum;
	PiSide.IntPowOp(powerExponent);
	return PiSide;
}

inline void BlazesRusCode::AltDecBase::ConvertPiPowerToPiRep()
{
	int powerExponent = -ExtraRep;
	if (powerExponent == 0)
		ExtraRep = 0;//Pi^0 = 1
	else
	{
		ExtraRep = PiRep;
		if (powerExponent != 1)
		{
			AltDecBase PiSide = PiNum;
			powerExponent -= 1;
			PiSide.IntPowOp(powerExponent);
			BasicMultOp(PiSide);
		}
	}
}

#endif

#if defined(AltNum_EnableERep)
    #if defined(AltNum_EnableFractionals)
inline void BlazesRusCode::AltDecBase::ConvertEByDivToNumByDiv()
{
	BasicIntDivOp(ExtraRep.Value);
	ExtraRep = 0;
	__int64 SRep;
	__int64 divRes;
	if(DecimalHalf==0)
	{
		bool IsNegative = IntValue<0;
		if(IsNegative)
			IntValue *= -1;
		SRep = 2718281828;				       
		SRep *= IntValue;
		divRes = SRep / DecimalOverflowX;
		DecimalHalf = (int)(SRep - DecimalOverflowX * divRes);
		if(divRes==0&&IsNegative)
		{
			if(DecimalHalf==0)
				IntValue = 0;
			else
				IntValue = NegativeRep;
		}
		else if(IsNegative)
			IntValue = (int)-divRes;
		else
			IntValue = (int)divRes;
	}
	else if(IntValue==0)
	{
		SRep = 2718281828;
		SRep *= DecimalHalf;
		divRes = SRep / 1000000000000000000;
		DecimalHalf = (int)((SRep - 1000000000000000000 * divRes)/DecimalOverflowX);
	}
	else if(IntValue==NegativeRep)
	{
		SRep = 2718281828;
		SRep *= DecimalHalf;
		divRes = SRep / 1000000000000000000;
		DecimalHalf = (int)((SRep - 1000000000000000000 * divRes)/DecimalOverflowX);
		if(divRes==0)
			IntValue = NegativeRep;
		else
			IntValue = (int)-divRes;
	}
	else
	{
		bool IsNegative = IntValue<0;
		if(IsNegative)
			IntValue *= -1;
		SRep = DecimalOverflowX * IntValue + DecimalHalf;
		SRep *= 2ll;//SRep holds __int64 version of X.Y * Z
		//X.Y *.V
		__int64 Temp03 = (__int64)IntValue * 718281828ll;//Temp03 holds __int64 version of X *.V
		__int64 Temp04 = (__int64)DecimalHalf * 718281828ll;
		Temp04 /= AltDecBase::DecimalOverflow;
		//Temp04 holds __int64 version of .Y * .V
		__int64 IntegerRep = SRep + Temp03 + Temp04;
		__int64 IntHalf = IntegerRep / AltDecBase::DecimalOverflow;
		IntegerRep -= IntHalf * (__int64)AltDecBase::DecimalOverflow;
		DecimalHalf = (signed int)IntegerRep;
		if(IntHalf == 0&&IsNegative)
		{
			IntValue = NegativeRep;
		}
		else if(IsNegative)
			IntValue = (int)-IntHalf;
		else
			IntValue = (int)IntHalf;
	}
	ExtraRep = -ExtraRep;
}


inline void BlazesRusCode::AltDecBase::ConvertFromEByDivToNorm()
{
	__int64 SRep;
	__int64 divRes;
	if (DecimalHalf == 0)
	{
		bool IsNegative = IntValue<0;
		if (IsNegative)
			IntValue *= -1;
		SRep = 2718281828;
		SRep *= IntValue;
		divRes = SRep / AltDecBase::DecimalOverflowX;
		DecimalHalf = (int)(SRep - DecimalOverflowX * divRes);
		if (divRes == 0 && IsNegative)
		{
			if (DecimalHalf == 0)
				IntValue = 0;
			else
				IntValue = NegativeRep;
		}
		else if (IsNegative)
			IntValue = (int)-divRes;
		else
			IntValue = (int)divRes;
	}
	else if (IntValue == 0)
	{
		SRep = 2718281828;
		SRep *= DecimalHalf;
		divRes = SRep / 1000000000000000000;
		DecimalHalf = (int)((SRep - 1000000000000000000 * divRes) / DecimalOverflowX);
	}
	else if (IntValue == NegativeRep)
	{
		SRep = 2718281828;
		SRep *= DecimalHalf;
		divRes = SRep / 1000000000000000000;
		DecimalHalf = (int)((SRep - 1000000000000000000 * divRes) / DecimalOverflowX);
		if (divRes == 0)
			IntValue = NegativeRep;
		else
			IntValue = (int)-divRes;
	}
	else
	{
		bool IsNegative = IntValue<0;
		if (IsNegative)
			IntValue *= -1;
		SRep = DecimalOverflowX * IntValue + DecimalHalf;
		SRep *= 2ll;//SRep holds __int64 version of X.Y * Z
		//X.Y *.V
		__int64 Temp03 = (__int64)IntValue * 718281828ll;//Temp03 holds __int64 version of X *.V
		__int64 Temp04 = (__int64)DecimalHalf * 718281828ll;
		Temp04 /= AltDecBase::DecimalOverflow;
		//Temp04 holds __int64 version of .Y * .V
		__int64 IntegerRep = SRep + Temp03 + Temp04;
		__int64 IntHalf = IntegerRep / AltDecBase::DecimalOverflow;
		IntegerRep -= IntHalf * (__int64)AltDecBase::DecimalOverflow;
		DecimalHalf = (signed int)IntegerRep;
		if (IntHalf == 0 && IsNegative)
		{
			IntValue = NegativeRep;
		}
		else if (IsNegative)
			IntValue = (int)-IntHalf;
		else
			IntValue = (int)IntHalf;
	}
}
    #endif
#endif


#if defined(AltNum_EnableImaginaryNum)

#endif
