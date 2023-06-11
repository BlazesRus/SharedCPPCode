        enum class RepType: int
        {
            NormalType = 0,
            NumByDiv,
//#if defined(AltDec_EnablePIRep)
            PINum,
//#if defined(AltDec_EnablePIPowers)
            PIPower,
//#endif
//#if defined(AltDec_EnableAlternativeRepFractionals)
//#if defined(AltDec_EnableDecimaledPiFractionals)
            PiNumByDiv,//  (Value/(ExtraRep*-1))*Pi Representation
//#endif
            PiFractional,//  IntValue/DecimalHalf*Pi Representation
//#endif
//#endif
//#if defined(AltDec_EnableENum)
            ENum,
//#if defined(AltDec_EnableAlternativeRepFractionals)
//#if defined(AltDec_EnableDecimaledEFractionals)
            ENumByDiv,//(Value/(ExtraRep*-1))*e Representation
//#endif
            EFractional,//  IntValue/DecimalHalf*e Representation
//#endif
//#endif
//#if defined(AltDec_EnableImaginaryNum)
            INum,
//#if defined(AltDec_EnableAlternativeRepFractionals)
//#if defined(AltDec_EnableDecimaledIFractionals)
            INumByDiv,//(Value/(ExtraRep*-1))*i Representation
//#endif
            IFractional,//  IntValue/DecimalHalf*i Representation
//#endif
//#ifdef MixedDec_EnableComplexNumbers
            ComplexIRep,
//#endif
//#endif
//#if defined(AltDec_EnableMixedFractional)
            ComplexIRep,
            MixedFrac,//IntValue +- (DecimalHalf*-1)/ExtraRep
            MixedE,
            MixedI,
//#endif
//#if defined(AltDec_EnableInfinityRep)
            ApproachingBottom,//(Approaching Towards Zero is equal to 0.000...1)
            ApproachingTop,//(Approaching Away from Zero is equal to 0.9999...)
//#if defined(AltDec_EnableApproachingDivided)
            ApproachingTopByDiv,//(Approaching Away from Zero is equal to IntValue + 0.9999.../ExtraRep if positive, IntValue - 0.9999.../ExtraRep if negative) 
//#endif
//#endif
            NaN,
            NegativeZero,
//#if defined(AltDec_EnableNearPI)
            NearPI,//(Approaching Away from Zero is equal to 0.9999...PI)
//#endif
//#if defined(AltDec_EnableNearE)
            NearE,//(Approaching Away from Zero is equal to 0.9999...e)
//#endif
//#if defined(AltDec_EnableNearI)
            NearI,//(Approaching Away from Zero is equal to 0.9999...i)
//#endif
//#if defined(AltDec_EnableUndefinedButInRange)//Such as result of Cos of infinity
            UndefinedButInRange,
//#endif
//#ifndef MixedDec_DisableIntNumByDivisor
//			//IntValue/(DecimalHalf*-1)
//			//For DecimalHalf values of between 0 and ENumBreakpoint if MediumDecV2_EnableERep toggled
//			//For DecimalHalf Values:
//			//between 0 and ApproachingBottomRep if MediumDecV2_DisableInfinityRep not toggled and MediumDecV2_EnableApproachingMidDec not toggled
//			//between 0 and MidFromBottomRep if MediumDecV2_DisableInfinityRep not toggled and MediumDecV2_EnableApproachingMidDec is toggled
//			//between 0 and -2147483648 if no infinity representations are enabled
            IntNumByDivisor,
//#endif
            UnknownType
        };