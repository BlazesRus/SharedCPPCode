#include "AltDecBase.hpp"
using AltDecBase = BlazesRusCode::AltDecBase;

#if defined(AltNum_EnablePiRep)
#if defined(AltNum_EnableDecimaledPiFractionals)
#elif defined(AltNum_EnablePiFractional)
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
#if defined(AltNum_EnableDecimaledEFractionals)
inline void BlazesRusCode::AltDecBase::ConvertEByDivToNumByDiv()
{
	BasicIntDivOp(-ExtraRep);
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

#elif defined(AltNum_EnableEFractional)
#endif
#endif


#if defined(AltNum_EnableImaginaryNum)

#endif
