        enum class RepType: int
        {
            NormalType = 0,
            NumByDiv,
//#if defined(AltNum_EnablePIRep)
            PINum,
//#if defined(AltNum_EnablePIPowers)
            PIPower,
//#endif
//#if defined(AltNum_EnableAlternativeRepFractionals)
//#if defined(AltNum_EnableDecimaledPiFractionals)
            PiNumByDiv,//  (Value/(ExtraRep*-1))*Pi Representation
//#endif
            PiFractional,//  IntValue/DecimalHalf*Pi Representation
//#endif
//#endif
//#if defined(AltNum_EnableENum)
            ENum,
//#if defined(AltNum_EnableAlternativeRepFractionals)
//#if defined(AltNum_EnableDecimaledEFractionals)
            ENumByDiv,//(Value/(ExtraRep*-1))*e Representation
//#endif
            EFractional,//  IntValue/DecimalHalf*e Representation
//#endif
//#endif
//#if defined(AltNum_EnableImaginaryNum)
            INum,
//#if defined(AltNum_EnableAlternativeRepFractionals)
//#if defined(AltNum_EnableDecimaledIFractionals)
            INumByDiv,//(Value/(ExtraRep*-1))*i Representation
//#endif
            IFractional,//  IntValue/DecimalHalf*i Representation
//#endif
//#ifdef AltNum_EnableComplexNumbers
            ComplexIRep,
//#endif
//#endif
//#if defined(AltNum_EnableMixedFractional)
            ComplexIRep,
            MixedFrac,//IntValue +- (DecimalHalf*-1)/ExtraRep
            MixedE,
            MixedI,
//#endif
//#if defined(AltNum_EnableInfinityRep)
            ApproachingBottom,//(Approaching Towards Zero is equal to 0.000...1)
            ApproachingTop,//(Approaching Away from Zero is equal to 0.9999...)
//#if defined(AltNum_EnableApproachingDivided)
            ApproachingTopByDiv,//(Approaching Away from Zero is equal to IntValue + 0.9999.../ExtraRep if positive, IntValue - 0.9999.../ExtraRep if negative) 
//#endif
//#endif
            NaN,
            NegativeZero,
//#if defined(AltNum_EnableNearPI)
            NearPI,//(Approaching Away from Zero is equal to 0.9999...PI)
//#endif
//#if defined(AltNum_EnableNearE)
            NearE,//(Approaching Away from Zero is equal to 0.9999...e)
//#endif
//#if defined(AltNum_EnableNearI)
            NearI,//(Approaching Away from Zero is equal to 0.9999...i)
//#endif
//#if defined(AltNum_EnableUndefinedButInRange)//Such as result of Cos of infinity
            UndefinedButInRange,
//#endif
//#ifndef MixedDec_DisableIntNumByDivisor//Not needed by AltDec since using NumByDivisor with ExtraRep value instead
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