#include "MediumDecV2Base.hpp"
using MediumDecV2Base = BlazesRusCode::MediumDecV2Base;

#if defined(AltNum_EnablePiRep)
//3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844 *selfNum
inline void BlazesRusCode::MediumDecV2Base::ConvertPiToNum()
{
	if (IntHalf.Value >= 683565275 && DecimalHalf.Value >= 168866626)//Exceeding Storage limit of NormalRep
	{
		throw "Conversion of Pi multiplication into MediumDec format resulted in overflow(setting value to maximum MediumDec value)";
		IntHalf = 2147483647; DecimalHalf = 999999999;//set value as maximum value(since not truely infinite just bit above storage range)
	}
	DecimalHalf.Flags = 0;
	__int64 SRep;
	__int64 divRes;
	if (DecimalHalf == 0)
	{
		SRep = 3141592654;
		SRep *= IntHalf.Value;
		//__int64 divRes = SRep / DecimalOverflowX;
		//__int64 C = SRep - DecimalOverflowX * divRes;
		divRes = SRep / DecimalOverflowX;
		DecimalHalf.Value = (unsigned int)(SRep - DecimalOverflowX * divRes);
		IntHalf.Value = (unsigned int)divRes;
	}
	else if (IntHalf == 0)
	{
		SRep = 3141592654;
		SRep *= DecimalHalf.Value;
		divRes = SRep / 1000000000000000000;
		DecimalHalf.Value = (unsigned int)((SRep - 1000000000000000000 * divRes) / DecimalOverflowX);
	}
	else
	{
		SRep = DecimalOverflowX * IntHalf.Value + DecimalHalf.Value;
		SRep *= 3ll;//SRep holds __int64 version of X.Y * Z
		//X.Y *.V
		__int64 Temp03 = (__int64)IntHalf * 141592654ll;//Temp03 holds __int64 version of X *.V
		__int64 Temp04 = (__int64)DecimalHalf * 141592654ll;
		Temp04 /= MediumDecV2Base::DecimalOverflow;
		//Temp04 holds __int64 version of .Y * .V
		__int64 IntegerRep = SRep + Temp03 + Temp04;
		__int64 intHalfRep = IntegerRep / MediumDecV2Base::DecimalOverflow;
		IntegerRep -= intHalfRep * (__int64)MediumDecV2Base::DecimalOverflow;
		DecimalHalf.Value = (unsigned int) IntegerRep;
		IntHalf.Value = (unsigned int)intHalfRep;
	}
}

#endif

#if defined(AltNum_EnableERep)
//2.71828 18284 59045 23536 02874 71352 66249 77572 47093 69995 * selfNum
inline void BlazesRusCode::MediumDecV2Base::ConvertENumToNum()
{
	if(IntHalf.Value>=790015084&&DecimalHalf>=351050349)//Exceeding Storage limit of NormalRep
	{
		throw "Conversion of e multiplication into MediumDec format resulted in overflow(setting value to maximum MediumDec value)";
		IntHalf = 2147483647; DecimalHalf = 999999999;//set value as maximum value(since not truely infinite just bit above storage range)
	}
	DecimalHalf.Flags = 0;
	__int64 SRep;
	__int64 divRes;
	if(DecimalHalf==0)
	{
		bool IsNegative = IntHalf<0;
		if(IsNegative)
			IntHalf *= -1;
		SRep = 2718281828;
		SRep *= IntHalf.Value;
		divRes = SRep / DecimalOverflowX;
		DecimalHalf.Value = (unsigned int)(SRep - DecimalOverflowX * divRes);
		IntHalf = (unsigned int)divRes;
	}
	else if(IntHalf==0)
	{
		SRep = 2718281828;
		SRep *= DecimalHalf;
		divRes = SRep / 1000000000000000000;
		DecimalHalf.Value = ((SRep - 1000000000000000000 * divRes) / DecimalOverflowX);
	}
	else
	{
		SRep = DecimalOverflowX * IntHalf + DecimalHalf;
		SRep *= 2ll;//SRep holds __int64 version of X.Y * Z
		//X.Y *.V
		unsigned __int64 Temp03 = (__int64)IntHalf * 718281828ll;//Temp03 holds __int64 version of X *.V
		unsigned __int64 Temp04 = (__int64)DecimalHalf * 718281828ll;
		Temp04 /= MediumDecV2Base::DecimalOverflow;
		//Temp04 holds __int64 version of .Y * .V
		unsigned __int64 IntegerRep = SRep + Temp03 + Temp04;
		unsigned __int64 intHalfRep = IntegerRep / MediumDecV2Base::DecimalOverflow;
		IntegerRep -= intHalfRep * MediumDecV2Base::DecimalOverflowX;
		DecimalHalf.Value = (unsigned int) IntegerRep;
		IntHalf.Value = (unsigned int)intHalfRep;
	}
}

#endif
