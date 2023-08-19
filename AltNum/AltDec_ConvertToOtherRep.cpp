#include "AltDec.hpp"
using AltDec = BlazesRusCode::AltDec;

#if defined(AltNum_EnablePiRep)
//3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844 *selfNum
inline void BlazesRusCode::AltDec::ConvertPiToNum()
{
	ExtraRep = 0;
	// Can only convert to up 683565275.1688666254437963172038917047964296646843381624484789109135725652864987887127902610635528943x PiRepresentation
	//Can Represent up ? before hitting Maximum AltDec value on reconversion when AltNum_UseLowerPrecisionPi is enabled
	//otherwise can represent up to ???(when adding up value from each decimal place of IntValue + (PiNum*DecimalHalf/1000000000))
	//Calculations from HiPer Calc
	//683565275.168866625 x 3.141592654 = 2147483646.99999999860577275
	//683565275.168866626 x 3.141592654 = 2147483647.000000001747365404
	if (IntValue >= 683565275 && DecimalHalf >= 168866626)//Exceeding Storage limit of NormalRep
	{
		throw "Conversion of Pi multiplication into MediumDec format resulted in overflow(setting value to maximum MediumDec value)";
		IntValue = 2147483647; DecimalHalf = 999999999;//set value as maximum value(since not truely infinite just bit above storage range)
	}
	else if (IntValue <= -683565275 && DecimalHalf >= 168866626)//Exceeding Storage limit of NormalRep
	{
		throw "Conversion of Pi multiplication into MediumDec format resulted in underflow(setting value to minimum MediumDec value)";
		IntValue = -2147483647; DecimalHalf = 999999999;//set value as minimum value(since not truely infinite just bit above storage range)
	}
	//Maximum result value              = 2147483647.999999999
	//Pi * 2147483647              =   6,746,518,849.1194168257096980859855
	//Int32 Max=
	//2147483647
	//683565275.168866625 x 3.141592654 = 2147483646.99999999860577275
	//Pi * 683565275                    = 2147483646.189086752242857401518383790899531254705429038668838768350655406412544182854647248094561223
	//Int64Max                 = 9223372036854775807
	//                     9,223,372,036,854,775,807
	//3,141,592653.5897932384626433832795 * 2147483647
	//                          =6746518849119416825.7096980859855
	//Pi =
	//3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844
	//For X.0 * Pi results
	//3141592653 * 683565275    =2147483645785924575
	//IntValue = 2147483645 (Res / 1000000000)
	//DecimalHalf = 785924575
	//3141592654 * 683565275    =2147483646469489850
	//IntValue = 2147483646 (Res / 1000000000)
	//DecimalHalf = 469489850
	//For 0.X * Pi results
	//Pi * 0.999999999        = 3.141592650448200584872850144816859500917666515177936421599838771332871813978392592341825826714082243
	//3141592654 * 999999999    =3141592650858407346
	//IntValue = 3 (Res / 1000000000000000000)
	//DecimalHalf = 141592650 (Rounded up techically equals 141592651) ((SRep - 1000000000000000000 * divRes)/DecimalOverflowX)
	__int64 SRep;
	__int64 divRes;
	if (DecimalHalf == 0)
	{
		bool IsNegative = IntValue < 0;
		if (IsNegative)
			IntValue *= -1;
		SRep = 3141592654;
		SRep *= IntValue;
		//__int64 divRes = SRep / DecimalOverflowX;
		//__int64 C = SRep - DecimalOverflowX * divRes;
		divRes = SRep / DecimalOverflowX;
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
		SRep = 3141592654;
		SRep *= DecimalHalf;
		divRes = SRep / 1000000000000000000;
		DecimalHalf = (int)((SRep - 1000000000000000000 * divRes) / DecimalOverflowX);
	}
	else if (IntValue == NegativeRep)
	{
		SRep = 3141592654;
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
		bool IsNegative = IntValue < 0;
		if (IsNegative)
			IntValue *= -1;
		SRep = DecimalOverflowX * IntValue + DecimalHalf;
		SRep *= 3ll;//SRep holds __int64 version of X.Y * Z
		//X.Y *.V
		__int64 Temp03 = (__int64)141592654ll * IntValue;//Temp03 holds __int64 version of X *.V
		__int64 Temp04 = (__int64)DecimalHalf * 141592654ll;
		Temp04 /= AltDec::DecimalOverflow;
		//Temp04 holds __int64 version of .Y * .V
		__int64 IntegerRep = SRep + Temp03 + Temp04;
		__int64 IntHalf = IntegerRep / AltDec::DecimalOverflow;
		IntegerRep -= IntHalf * (__int64)AltDec::DecimalOverflow;
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

	#if defined(AltNum_EnableDecimaledPiFractionals)
inline void BlazesRusCode::AltDec::ConvertPiByDivToNumByDiv()
{
	__int64 SRep;
	__int64 divRes;
	if (DecimalHalf == 0)
	{
		bool IsNegative = IntValue < 0;
		if (IsNegative)
			IntValue *= -1;
		SRep = 3141592654;
		SRep *= IntValue;
		//__int64 divRes = SRep / DecimalOverflowX;
		//__int64 C = SRep - DecimalOverflowX * divRes;
		divRes = SRep / DecimalOverflowX;
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
		SRep = 3141592654;
		SRep *= DecimalHalf;
		divRes = SRep / 1000000000000000000;
		DecimalHalf = (int)((SRep - 1000000000000000000 * divRes) / DecimalOverflowX);
	}
	else if (IntValue == NegativeRep)
	{
		SRep = 3141592654;
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
		bool IsNegative = IntValue < 0;
		if (IsNegative)
			IntValue *= -1;
		SRep = DecimalOverflowX * IntValue + DecimalHalf;
		SRep *= 3ll;//SRep holds __int64 version of X.Y * Z
		//X.Y *.V
		__int64 Temp03 = (__int64)141592654ll * IntValue;//Temp03 holds __int64 version of X *.V
		__int64 Temp04 = (__int64)DecimalHalf * 141592654ll;
		Temp04 /= AltDec::DecimalOverflow;
		//Temp04 holds __int64 version of .Y * .V
		__int64 IntegerRep = SRep + Temp03 + Temp04;
		__int64 IntHalf = IntegerRep / AltDec::DecimalOverflow;
		IntegerRep -= IntHalf * (__int64)AltDec::DecimalOverflow;
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
	ExtraRep = -ExtraRep;
}

inline void BlazesRusCode::AltDec::ConvertFromPiByDivToNorm()
{
	PartialIntDiv(-ExtraRep);
	ExtraRep = 0;
	__int64 SRep;
	__int64 divRes;
	if (DecimalHalf == 0)
	{
		bool IsNegative = IntValue < 0;
		if (IsNegative)
			IntValue *= -1;
		SRep = 3141592654;
		SRep *= GetIntHalf();
		divRes = SRep / DecimalOverflowX;
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
		SRep = 3141592654;
		SRep *= DecimalHalf;
		divRes = SRep / 1000000000000000000;
		DecimalHalf = (int)((SRep - 1000000000000000000 * divRes) / DecimalOverflowX);
	}
	else if (IntValue == NegativeRep)
	{
		SRep = 3141592654;
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
		bool IsNegative = IntValue < 0;
		if (IsNegative)
			IntValue *= -1;
		SRep = DecimalOverflowX * IntValue + DecimalHalf;
		SRep *= 3ll;//SRep holds __int64 version of X.Y * Z
		//X.Y *.V
		__int64 Temp03 = (__int64)141592654ll * IntValue;//Temp03 holds __int64 version of X *.V
		__int64 Temp04 = (__int64)DecimalHalf * 141592654ll;
		Temp04 /= AltDec::DecimalOverflow;
		//Temp04 holds __int64 version of .Y * .V
		__int64 IntegerRep = SRep + Temp03 + Temp04;
		__int64 IntHalf = IntegerRep / AltDec::DecimalOverflow;
		IntegerRep -= IntHalf * (__int64)AltDec::DecimalOverflow;
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
	#elif defined(AltNum_EnablePiFractional)
inline void BlazesRusCode::AltDec::ConvertFromPiFractionalToNorm()
{
	int divisor = DecimalHalf;
	DecimalHalf = 0;
	ExtraRep = 0;
	bool IsNegative = IntValue<0;
	if(IsNegative)
		IntValue *= -1;
	__int64 SRep = 3141592654ll;
	SRep *= IntValue;
	__int64 divRes = SRep / DecimalOverflowX;
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
	BasicIntDivOp(divisor);
}
	#endif

inline void BlazesRusCode::AltDec::ConvertPiPowerToNum()
{
	int powerExponent = -ExtraRep;
	ExtraRep = 0;
	AltDec PiSide = PiNum;
	PiSide.Int32PowOp(powerExponent);
	BasicMultOp(PiSide);
}

inline AltDec BlazesRusCode::AltDec::PiPowerNum(int powerExponent)
{
	ExtraRep = 0;
	AltDec PiSide = PiNum;
	PiSide.Int32PowOp(powerExponent);
	return PiSide;
}

inline void BlazesRusCode::AltDec::ConvertPiPowerToPiRep()
{
	int powerExponent = -ExtraRep;
	if (powerExponent == 0)
		ExtraRep = 0;//Pi^0 = 1
	else
	{
		ExtraRep = PiRep;
		if (powerExponent != 1)
		{
			AltDec PiSide = PiNum;
			powerExponent -= 1;
			PiSide.Int32PowOp(powerExponent);
			BasicMultOp(PiSide);
		}
	}
}

#endif

#if defined(AltNum_EnableERep)
//2.71828 18284 59045 23536 02874 71352 66249 77572 47093 69995 * selfNum
inline void BlazesRusCode::AltDec::ConvertENumToNum()
{
	if(IntValue>=790015084&&DecimalHalf>=351050349)//Exceeding Storage limit of NormalRep
	{
		throw "Conversion of e multiplication into MediumDec format resulted in overflow(setting value to maximum MediumDec value)";
		IntValue = 2147483647; DecimalHalf = 999999999;//set value as maximum value(since not truely infinite just bit above storage range)
	}
	else if(IntValue<=-790015084&&DecimalHalf>=351050349)//Exceeding Storage limit of NormalRep
	{
		throw "Conversion of e multiplication into MediumDec format resulted in underflow(setting value to minimum MediumDec value)";
		IntValue = -2147483647; DecimalHalf = 999999999;//set value as minimum value(since not truely infinite just bit above storage range)
	}
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
		Temp04 /= AltDec::DecimalOverflow;
		//Temp04 holds __int64 version of .Y * .V
		__int64 IntegerRep = SRep + Temp03 + Temp04;
		__int64 IntHalf = IntegerRep / AltDec::DecimalOverflow;
		IntegerRep -= IntHalf * (__int64)AltDec::DecimalOverflow;
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
	ExtraRep = 0;
}

	#if defined(AltNum_EnableDecimaledEFractionals)
inline void BlazesRusCode::AltDec::ConvertEByDivToNumByDiv()
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
		Temp04 /= AltDec::DecimalOverflow;
		//Temp04 holds __int64 version of .Y * .V
		__int64 IntegerRep = SRep + Temp03 + Temp04;
		__int64 IntHalf = IntegerRep / AltDec::DecimalOverflow;
		IntegerRep -= IntHalf * (__int64)AltDec::DecimalOverflow;
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

inline void BlazesRusCode::AltDec::ConvertFromEByDivToNorm()
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
		divRes = SRep / AltDec::DecimalOverflowX;
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
		Temp04 /= AltDec::DecimalOverflow;
		//Temp04 holds __int64 version of .Y * .V
		__int64 IntegerRep = SRep + Temp03 + Temp04;
		__int64 IntHalf = IntegerRep / AltDec::DecimalOverflow;
		IntegerRep -= IntHalf * (__int64)AltDec::DecimalOverflow;
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

	#elif defined(AltNum_EnableEFractional)
inline void BlazesRusCode::AltDec::ConvertFromEFractionalToNorm()
{
	int divisor = DecimalHalf;
	DecimalHalf = 0;
	ExtraRep = 0;
	bool IsNegative = IntValue < 0;
	if (IsNegative)
		IntValue *= -1;
	__int64 SRep = 2718281828ll;
	SRep *= IntValue;
	__int64 divRes = SRep / AltDec::DecimalOverflowX;
	DecimalHalf = (int)(SRep - AltDec::DecimalOverflowX * divRes);
	if (divRes == 0 && IsNegative)
	{
		if (DecimalHalf == 0)
			IntValue = 0;
		else
			IntValue = AltDec::NegativeRep;
	}
	else if (IsNegative)
		IntValue = (int)-divRes;
	else
		IntValue = (int)divRes;
	BasicIntDivOp(&divisor);
}
	#endif
#endif

inline void BlazesRusCode::AltDec::ConvertToNormTypeV2()
{
	RepType repType = GetRepType();
	ConvertToNormType(repType);
}

inline AltDec BlazesRusCode::AltDec::ConvertAsNormTypeV2()
{
	AltDec Res = *this;
	Res.ConvertToNormTypeV2();
	return Res;
}

#if defined(AltNum_EnableImaginaryNum)

inline void BlazesRusCode::AltDec::ConvertToNormalIRep()
{
	RepType repType = GetRepType();
	ConvertToNormalIRep(repType);
}

inline AltDec BlazesRusCode::AltDec::ConvertAsNormalIRep()
{
	AltDec Res = *this;
	Res.ConvertToNormalIRep();
	return Res;
}
#endif
