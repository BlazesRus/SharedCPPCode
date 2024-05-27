    #if defined(AltNum_EnableInfinityRep)
        //Is Infinity Representation when DecimalHalf==-2147483648 (IntValue==1 for positive infinity;IntValue==-1 for negative Infinity)
        //(other values only used if AltNum_EnableInfinityPowers is enabled)
        //If AltNum_EnableImaginaryInfinity is enabled and ExtraRep = IRep, then represents either negative or positive imaginary infinity
		#if !defined(AltNum_UsePositiveInfinityRep)
        static const signed int InfinityRep = -2147483648;
		#else
        static const signed int InfinityRep = 2147483647;
		#endif
    #endif
    #if defined(AltNum_EnableApproaching)
        //Is Approaching Bottom when DecimalHalf==-2147483647:
        //If ExtraRep==1, it represents Approaching IntValue from right towards left (IntValue.0..1)
        //If ExtraRep above 1 and 2147483645 and AltNum_EnableApproachingDivided enabled, Represents approaching 1/ExtraRep point
        //If ExtraRep=PiRep, then it represents Approaching IntValue from right towards left (IntValue.0..1)Pi
        static const signed int ApproachingBottomRep = -2147483647;
        //Is Approaching Top i when DecimalHalf==-2147483646:
        //If ExtraRep==1, it represents Approaching IntValue+1 from left towards right (IntValue.9__9)
        //If ExtraRep above 1 and AltNum_EnableApproachingDivided enabled, Represents approaching 1/ExtraRep point
        //If ExtraRep=PiRep, then it represents Approaching IntValue+1 from left towards right (IntValue.9__9)Pi
        static const signed int ApproachingTopRep = -2147483646;
    #endif
    #if defined(AltNum_EnableUndefinedButInRange)
        //Such as result of Cos of infinity
        //https://www.cuemath.com/trigonometry/domain-and-range-of-trigonometric-functions/
        static const signed int UndefinedInRangeRep = -2147483642;
        
        #if defined(AltNum_EnableWithinMinMaxRange)
        //Undefined but in ranged of IntValue to DecimalHalf
        static const signed int WithinMinMaxRangeRep = -2147483642;
        #endif
    #endif
    #if defined(AltNum_EnableNaN)
        //Is NaN when DecimalHalf==2147483647
        static const signed int NaNRep = 2147483647;
        //Is NaN when DecimalHalf==2147483646
        static const signed int UndefinedRep = 2147483646;
    #endif
    #if defined(AltNum_EnableNil)
        //When both IntValue and DecimalHalf equal -2147483648 it is Nil
        static signed int const NilRep = -2147483648;
    #endif
#endif
    #if defined(AltNum_EnableApproaching)
        #if defined(AltNum_EnableApproachingI)
        //Is Approaching Bottom i when DecimalHalf==-2147483645:
        //If ExtraRep==1, it represents Approaching IntValue from right towards left (IntValue.0..1)i
        static const signed int ApproachingImaginaryBottomRep = -2147483645;
        //Is Approaching Top i when DecimalHalf==-2147483644:
        //If ExtraRep==1, it represents Approaching IntValue+1 from left towards right (IntValue.9__9)i
        static const signed int ApproachingImaginaryTopRep = -2147483644;
        #endif
    #endif
    #if defined(AltNum_EnablePiRep)
        //Is Pi*Value representation when ExtraRep==-2147483648
        static const signed int PiRep = -2147483648;
        #ifdef AltNum_EnableAlternativeRepFractionals
        //If AltNum_EnableIRep is enabled and ExtraRep== -2147483645, then represents (IntValue/DecimalHalf)*Pi
        static const signed int PiByDivisorRep = -2147483645;
        #endif
    #endif
    #if defined(AltNum_EnableIRep)
        //If AltNum_EnableIRep is enabled and ExtraRep== -2147483646, then represents Value*i
        static const signed int IRep = -2147483647;
        #ifdef AltNum_EnableAlternativeRepFractionals
        //If AltNum_EnableIRep is enabled and ExtraRep== -2147483644, then represents (IntValue/DecimalHalf)*i
        static const signed int IByDivisorRep = -2147483644;
        #endif
    #endif
    #if defined(AltNum_EnableERep)&&defined(AltNum_EnableAlternativeRepFractionals)
        //If AltNum_EnableIRep is enabled and ExtraRep== -2147483646, then represents Value*e
        static const signed int ERep = -2147483646;
        #ifdef AltNum_EnableAlternativeRepFractionals
        //If AltNum_EnableIRep is enabled and ExtraRep== -2147483643, then represents (IntValue/DecimalHalf)*e
        static const signed int EByDivisorRep = -2147483643;
        #endif
    #endif