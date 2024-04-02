#include "MediumDecV2Base.hpp"
using MediumDecV2Base = BlazesRusCode::MediumDecV2Base;

#if defined(AltNum_EnablePiRep)
//3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844 *selfNum
inline void BlazesRusCode::MediumDecV2Base::ConvertPiToNum()
{
	if (IntValue >= 683565275 && DecimalHalf.Value >= 168866626)//Exceeding Storage limit of NormalRep
	{
		throw "Conversion of Pi multiplication into MediumDec format resulted in overflow(setting value to maximum MediumDec value)";
		IntValue = 2147483647; DecimalHalf = 999999999;//set value as maximum value(since not truely infinite just bit above storage range)
	}
	#if !defined(AltNum_EnableMirroredSection)
	else if (IntValue <= -683565275 && DecimalHalf.Value >= 168866626)//Exceeding Storage limit of NormalRep
	{
		throw "Conversion of Pi multiplication into MediumDec format resulted in underflow(setting value to minimum MediumDec value)";
		IntValue = -2147483647; DecimalHalf = 999999999;//set value as minimum value(since not truely infinite just bit above storage range)
	}
	#endif
    #if !defined(AltNum_UseIntForDecimalHalf)
	DecimalHalf.Flags = 0;
    #endif
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
	#if !defined(AltNum_EnableMirroredSection)
		SRep *= IntValue;
	#else
		SRep *= IntValue.Value;
	#endif
		//__int64 divRes = SRep / DecimalOverflowX;
		//__int64 C = SRep - DecimalOverflowX * divRes;
		divRes = SRep / DecimalOverflowX;
    #if defined(AltNum_UseIntForDecimalHalf)
		DecimalHalf = (int)(SRep - DecimalOverflowX * divRes);
    #else
		DecimalHalf.Value = (unsigned int)(SRep - DecimalOverflowX * divRes);
    #endif
	#if !defined(AltNum_EnableMirroredSection)
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
	#else
		IntValue.Value = (unsigned int)divRes;
	#endif
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
		Temp04 /= MediumDecV2Base::DecimalOverflow;
		//Temp04 holds __int64 version of .Y * .V
		__int64 IntegerRep = SRep + Temp03 + Temp04;
		__int64 IntHalf = IntegerRep / MediumDecV2Base::DecimalOverflow;
		IntegerRep -= IntHalf * (__int64)MediumDecV2Base::DecimalOverflow;
    #if defined(AltNum_UseIntForDecimalHalf)
		DecimalHalf = (signed int)IntegerRep;
    #else
		DecimalHalf.Value = (unsigned int) IntegerRep;
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
}

#endif

#if defined(AltNum_EnableERep)
//2.71828 18284 59045 23536 02874 71352 66249 77572 47093 69995 * selfNum
inline void BlazesRusCode::MediumDecV2Base::ConvertENumToNum()
{
	if(IntValue>=790015084&&DecimalHalf>=351050349)//Exceeding Storage limit of NormalRep
	{
		throw "Conversion of e multiplication into MediumDec format resulted in overflow(setting value to maximum MediumDec value)";
		IntValue = 2147483647; DecimalHalf = 999999999;//set value as maximum value(since not truely infinite just bit above storage range)
	}
    #if !defined(AltNum_EnableMirroredSection)
	else if(IntValue<=-790015084&&DecimalHalf>=351050349)//Exceeding Storage limit of NormalRep
	{
		throw "Conversion of e multiplication into MediumDec format resulted in underflow(setting value to minimum MediumDec value)";
		IntValue = -2147483647; DecimalHalf = 999999999;//set value as minimum value(since not truely infinite just bit above storage range)
	}
    #endif
    #if !defined(AltNum_UseIntForDecimalHalf)
	DecimalHalf.Flags = 0;
    #endif
	__int64 SRep;
	__int64 divRes;
	if(DecimalHalf==0)
	{
		bool IsNegative = IntValue<0;
		if(IsNegative)
			IntValue *= -1;
		SRep = 2718281828;
	#if !defined(AltNum_EnableMirroredSection)			       
		SRep *= IntValue;
    #else
		SRep *= IntValue.Value;
	#endif
		divRes = SRep / DecimalOverflowX;
    #if defined(AltNum_UseIntForDecimalHalf)
		DecimalHalf = (int)(SRep - DecimalOverflowX * divRes);
    #else
		DecimalHalf.Value = (unsigned int)(SRep - DecimalOverflowX * divRes);
    #endif
	#if !defined(AltNum_EnableMirroredSection)
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
	#else
		IntValue = (unsigned int)divRes;
	#endif
	}
	else if(IntValue==0)
	{
		SRep = 2718281828;
		SRep *= DecimalHalf;
		divRes = SRep / 1000000000000000000;
    #if defined(AltNum_UseIntForDecimalHalf)
		DecimalHalf = (int)((SRep - 1000000000000000000 * divRes) / DecimalOverflowX);
    #else
		DecimalHalf.Value = ((SRep - 1000000000000000000 * divRes) / DecimalOverflowX);
    #endif
	}
	#if !defined(AltNum_EnableMirroredSection)
	else if(IntValue==NegativeRep)
	{
		SRep = 2718281828;
		SRep *= DecimalHalf;
		divRes = SRep / 1000000000000000000;
        #if defined(AltNum_UseIntForDecimalHalf)
		DecimalHalf = (int)((SRep - 1000000000000000000 * divRes) / DecimalOverflowX);
        #else
		DecimalHalf.Value = (unsigned int)((SRep - 1000000000000000000 * divRes) / DecimalOverflowX);
        #endif
		if(divRes==0)
			IntValue = NegativeRep;
		else
			IntValue = (int)-divRes;
	}
    #endif
	else
	{
	#if !defined(AltNum_EnableMirroredSection)
		bool IsNegative = IntValue<0;
		if(IsNegative)
			IntValue *= -1;
    #endif
		SRep = DecimalOverflowX * IntValue + DecimalHalf;
		SRep *= 2ll;//SRep holds __int64 version of X.Y * Z
		//X.Y *.V
		__int64 Temp03 = (__int64)IntValue * 718281828ll;//Temp03 holds __int64 version of X *.V
		__int64 Temp04 = (__int64)DecimalHalf * 718281828ll;
		Temp04 /= MediumDecV2Base::DecimalOverflow;
		//Temp04 holds __int64 version of .Y * .V
		__int64 IntegerRep = SRep + Temp03 + Temp04;
		__int64 IntHalf = IntegerRep / MediumDecV2Base::DecimalOverflow;
		IntegerRep -= IntHalf * (__int64)MediumDecV2Base::DecimalOverflow;
    #if defined(AltNum_UseIntForDecimalHalf)
		DecimalHalf = (signed int)IntegerRep;
    #else
		DecimalHalf.Value = (unsigned int) IntegerRep;
    #endif
	#if !defined(AltNum_EnableMirroredSection)
		if(IntHalf == 0&&IsNegative)
		{
			IntValue = NegativeRep;
		}
		else if(IsNegative)
			IntValue = (int)-IntHalf;
		else
			IntValue = (int)IntHalf;
    #else
		IntValue.Value = (unsigned int)IntHalf;
    #endif
	}
}

#endif


#if defined(AltNum_EnableImaginaryNum)

#endif
