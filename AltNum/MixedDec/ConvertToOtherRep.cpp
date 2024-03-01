#include "MixedDec.hpp"
using MixedDec = BlazesRusCode::MixedDec;

#if defined(AltNum_EnablePiRep)
#if defined(AltNum_EnableDecimaledPiFractionals)
#elif defined(AltNum_EnablePiFractional)
#endif

inline void BlazesRusCode::MixedDec::ConvertPiPowerToNum()
{
	int powerExponent = -ExtraRep;
	ExtraRep = 0;
	MixedDec PiSide = PiNum;
	PiSide.IntPowOp(powerExponent);
	BasicMultOp(PiSide);
}

inline MixedDec BlazesRusCode::MixedDec::PiPowerNum(int powerExponent)
{
	ExtraRep = 0;
	MixedDec PiSide = PiNum;
	PiSide.IntPowOp(powerExponent);
	return PiSide;
}

inline void BlazesRusCode::MixedDec::ConvertPiPowerToPiRep()
{
	int powerExponent = -ExtraRep;
	if (powerExponent == 0)
		ExtraRep = 0;//Pi^0 = 1
	else
	{
		ExtraRep = PiRep;
		if (powerExponent != 1)
		{
			MixedDec PiSide = PiNum;
			powerExponent -= 1;
			PiSide.IntPowOp(powerExponent);
			BasicMultOp(PiSide);
		}
	}
}

#endif

#if defined(AltNum_EnableERep)
#if defined(AltNum_EnableDecimaledEFractionals)
inline void BlazesRusCode::MixedDec::ConvertEByDivToNumByDiv()
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
		Temp04 /= MixedDec::DecimalOverflow;
		//Temp04 holds __int64 version of .Y * .V
		__int64 IntegerRep = SRep + Temp03 + Temp04;
		__int64 IntHalf = IntegerRep / MixedDec::DecimalOverflow;
		IntegerRep -= IntHalf * (__int64)MixedDec::DecimalOverflow;
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
