// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

namespace BlazesRusCode
{
    template<class VariantName>
    class MediumDecBasicOperators
    {
protected:
    #pragma region NormalRep Integer Division Operations

        template<IntegerType IntType=unsigned int>
        void PartialUIntDivOp(const IntType& rValue)
        {//Avoid using with special status representations such as approaching zero or result will be incorrect
          if constexpr (VariantName::DisableBitwiseMaskMode) {
            unsigned _int64 SelfRes;
            unsigned _int64 Res;
            unsigned _int64 IntHalfRes;
            unsigned _int64 DecimalRes;
            SelfRes = DecimalHalf == 0? DecimalOverflowX * IntHalf.Value: DecimalOverflowX * IntHalf.Value + DecimalHalf.Value;
            Res = SelfRes / rValue;

            IntHalfRes = Res/DecimalOverflowX;
            DecimalRes = Res - DecimalOverflowX * IntHalfRes;
            IntHalf.Value = (unsigned int)IntHalfRes;
            DecimalHalf.Value = (unsigned int)DecimalRes;
          } else {
            Value() /= rValue;
          }
        }

        template<IntegerType IntType=signed int>
        void PartialIntDivOp(const IntType& Value)
        {
          if constexpr (VariantName::SignedMode) {
            if(Value<0)
            {
                SwapNegativeStatus();
                PartialUIntDivOp(-Value);
            }
            else
                PartialUIntDivOp(Value);
          } else PartialUIntDivOp(Value);
        }

        /// <summary>
        /// Basic division operation between MediumDecV3Variant and unsigned Integer value
        /// that ignores special representation status
        /// (Doesn't modify owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        void BasicUIntDivOp(const IntType& Value)
        {
            if (Value == 0)
            { throw "Target value can not be divided by zero"; }
            else if (IsZero())
                return;
            PartialUIntDivOpV1(Value);
            if (GetIntHalf() == 0 && DecimalHalf() == 0)
                DecimalHalf = 1;//Prevent Dividing into nothing
        }

        template<IntegerType IntType=signed int>
        void BasicIntDivOp(const IntType& Value)
        {
            if (Value == 0)
            { throw "Target value can not be divided by zero"; }
            else if (IsZero())
                return;
            PartialIntDivOpV1(Value);
            if (GetIntHalf() == 0 && GetDecimalHalf() == 0)
                DecimalHalf = 1;//Prevent Dividing into nothing
        }

    #pragma endregion NormalRep Integer Division Operations

    #pragma region NormalRep AltNum Division Operations
protected:

        /// <summary>
        /// Basic unsigned division operation(main code block)
        /// Return true if divide into zero
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side value</param>
        bool UnsignedPartialDivOp(const VariantName& rValue)
        {
					ValueT SelfRes = Value();
					ValueT ValueRes = rValue.Value();

					ValueT IntHalfRes = SelfRes / ValueRes;
					ValueT DecimalRes = SelfRes - ValueRes * IntHalfRes;
					SetValue(IntHalfRes, DecimalRes);
					if(IntHalfRes==0&&DecimalRes==0)
						  return true;
					return false;
        }

        /// <summary>
        /// Unsigned division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side value</param>
        template<VariantNameVariant VariantType=VariantName>
        void UnsignedDivOpV1(const VariantType& rValue)
        {
            if(DecimalHalf==0)
            {
                if(rValue.DecimalHalf==0)
                {
                    switch(rValue.IntHalf.Value)
                    {
                        case 2:
                            if((IntHalf.Value&1)==1)//Check if number is odd
                                UIntDivOp(2);
                            else
                                IntHalf.Value /= 2;
                            break;
                        case 4:
                            if(((IntHalf.Value >> 2) << 2) == IntHalf.Value)
                                IntHalf.Value /= 4;
                            else
                                UIntDivOp(4);
                            break;
                        case 8:
                            if(((IntHalf.Value >> 3) << 3) == IntHalf.Value)
                                IntHalf.Value /= 8;
                            else
                                UIntDivOp(4);
                            break;
                        case 16:
                            if(((IntHalf.Value >> 4) << 4) == IntHalf.Value)
                                IntHalf.Value /= 16;
                            else
                                UIntDivOp(4);
                            break;
                        case 32:
                            if(((IntHalf.Value >> 5) << 5) == IntHalf.Value)
                                IntHalf.Value /= 32;
                            else
                                UIntDivOp(4);
                            break;
                        case 0:
                            throw "Target rValue can not be divided by zero";
                            break;
                        default:
                            UIntDivOp(rValue.IntHalf.Value);
                            break;
                    }
                }
#if !defined(AltNum_DisableDivideDownToNothingPrevention)
                else if (UnsignedPartialDivOp(rValue))//Prevent Dividing into nothing
                        DecimalHalf.Value = 1;
#else
                else
                    UnsignedPartialDivOp(rValue);
#endif
            }
#if !defined(AltNum_DisableDivideDownToNothingPrevention)
            else if (UnsignedPartialDivOp(rValue))//Prevent Dividing into nothing
                DecimalHalf.Value = 1;
#else
            else
                UnsignedPartialDivOp(rValue);
#endif
        }

        /// <summary>
        /// Basic division operation that ignores special decimal status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        template<VariantNameVariant VariantType=VariantName>
        void DivOpV1(const VariantType& rValue)
        {
            if(rValue.IsNegative())
            {
                SwapNegativeStatus();
                UnsignedDivOp(-rValue);
            }
            else
                UnsignedDivOp(rValue);
        }

public:

    #pragma endregion NormalRep AltNum Division Operations

    #pragma region NormalRep Integer Multiplication Operations
protected:
        /// <summary>
        /// Partial version of UIntMultOpV1 without zero checks
        /// (Modifies owner object)
        /// </summary>
        /// <param name="tValue">The value.</param>
        /// <returns>VariantName</returns>
        template<IntegerType IntType=int>
        void PartialUIntMultOpV1(const IntType& rValue)
        {
            if (DecimalHalf == 0)
                IntHalf.Value *= rValue;
            else
            {
                __int64 SRep = IntHalf == 0 ? DecimalHalf.Value : DecimalOverflowX * IntHalf.Value + DecimalHalf.Value;
                SRep *= rValue;
                if (SRep >= DecimalOverflowX)
                {
                    __int64 OverflowVal = SRep / DecimalOverflowX;
                    SRep -= OverflowVal * DecimalOverflowX;
                    IntHalf.Value = (unsigned int)OverflowVal;
                    DecimalHalf.Value = (unsigned int)SRep;
                }
                else
                {
                    IntHalf.Value = 0;
                    DecimalHalf.Value = (unsigned int)SRep;
                }
            }
        }

        //Partial version of BasicIntMultOpV1 without zero checks
        //Modifies owner object
        template<IntegerType IntType=signed int>
        void PartialIntMultOpV1(const IntType& rValue)
        {
            if(rValue<0)
            {
                SwapNegativeStatus();
                PartialUIntMultOpV1(-rValue);
            }
            else
                PartialUIntMultOp(rValue);
        }
public:

protected:
        template<IntegerType IntType=signed int>
        void UIntMultOpV1(const IntType& rValue)
        {
            if (rValue == 0)
            {
                SetAsZero();
                return;
            }
            else if (IsZero())
                return;
            PartialUIntMultOpV1(rValue);
        }

        template<IntegerType IntType=signed int>
        void IntMultOpV1(const IntType& Value)
        {
            if (Value == 0)
            {
                SetAsZero();
                return;
            }
            else if (IsZero())
                return;
            PartialIntMultOpV1(Value);
        }

public:

    #pragma endregion NormalRep Integer Multiplication Operations

    #pragma region NormalRep AltNum Multiplication Operations
protected:

        /// <summary>
        /// Basic multiplication operation that ignores special decimal status with unsigned VariantName
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side tValue</param>
        template<VariantNameVariant VariantType=VariantName>
        void UnsignedMultOpV1(const VariantType& rValue)
        {
            if (DecimalHalf == 0)
            {
                if (IntHalf.Value == 1)
                {
                    if(IntHalf.IsNegative())
                        IntHalf = -rValue.IntHalf;
                    else
                        IntHalf = rValue.IntHalf;
                    DecimalHalf = rValue.DecimalHalf;
                }
                else if (rValue.DecimalHalf == 0)
                    IntHalf *= rValue.IntHalf;
                else {
                    __int64 rRep = rValue.IntHalf == 0 ? rValue.DecimalHalf.Value : DecimalOverflowX * rValue.IntHalf.Value + rValue.DecimalHalf.Value;
                    if (rRep >= DecimalOverflowX)
                    {
                        __int64 OverflowVal = rRep / DecimalOverflowX;
                        rRep -= OverflowVal * DecimalOverflowX;
                        IntHalf.Value = (unsigned int)OverflowVal;
                        DecimalHalf.Value = (unsigned int)rRep;
                        return;
                    }
                    else
                    {
                        DecimalHalf = (signed int)rRep;
                    #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
                        if(DecimalHalf==0)
                            DecimalHalf.Value = 1;
                    #elif !defined(AltNum_AllowNegativeZero)
                        if(DecimalHalf==0){
                            SetAsZero(); return; }
                    #endif
                        IntHalf.Value = 0;
                        return;
                    }
                }
            }
            else if (IntHalf.Value == 0)
            {
                __int64 SRep = (__int64)DecimalHalf;
                SRep *= rValue.DecimalHalf.Value;
                SRep /= DecimalOverflowX;
                if (rValue.IntHalf == 0)
                {
                    DecimalHalf = (signed int)SRep;
                #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
                    if (DecimalHalf == 0)
                        DecimalHalf.Value = 1;
                #elif !defined(AltNum_AllowNegativeZero)
                    if(DecimalHalf==0){
                        SetAsZero(); return; }
                #endif
                    return;
                }
                else
                {
                    SRep += (__int64)DecimalHalf.Value * rValue.IntHalf.Value;
                    if (SRep >= DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / DecimalOverflowX;
                        SRep -= OverflowVal * DecimalOverflowX;
                        IntHalf.Value = OverflowVal;
                        DecimalHalf.Value = (signed int)SRep;
                        return;
                    }
                    else
                    {
                        DecimalHalf.Value = (unsigned int)SRep;
                #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
                        if(DecimalHalf==0)
                            DecimalHalf.Value = 1;
                #elif !defined(AltNum_AllowNegativeZero)
                        if(DecimalHalf==0){
                            SetAsZero(); return; }
                #endif
                        return;
                    }
                }
            }
            else
            {
                if (rValue.DecimalHalf == 0)//Y is integer
                {
                    __int64 SRep = DecimalOverflowX * IntHalf.Value + DecimalHalf.Value;
                    SRep *= rValue.IntHalf.Value;
                    if (SRep >= DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / DecimalOverflowX;
                        SRep -= OverflowVal * DecimalOverflowX;
                        IntHalf.Value = (unsigned int)OverflowVal;
                        DecimalHalf.Value = (unsigned int)SRep;
                    }
                    else
                    {
                        DecimalHalf.Value = (unsigned int)SRep;
                        if(DecimalHalf==0)
                        {
                #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
                            if(DecimalHalf==0)
                                DecimalHalf.Value = 1;
                #elif !defined(AltNum_AllowNegativeZero)
                        if(DecimalHalf==0){
                            SetAsZero(); return; }
                #endif
                        }
                        IntHalf.Value = 0;
                    }
                    return;
                }
                else if (rValue.IntHalf == 0)
                {
                    __int64 SRep = DecimalOverflowX * IntHalf.Value + DecimalHalf.Value;
                    SRep *= rValue.DecimalHalf.Value;
                    SRep /= DecimalOverflowX;
                    if (SRep >= DecimalOverflowX)
                    {
                        __int64 OverflowVal = SRep / DecimalOverflowX;
                        SRep -= OverflowVal * DecimalOverflowX;
                        IntHalf.Value = (unsigned int)OverflowVal;
                        DecimalHalf.Value = (unsigned int)SRep;
                    }
                    else
                    {
                        DecimalHalf.Value = (unsigned int)SRep;
                        if(DecimalHalf==0)
                        {
                #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
                            if(DecimalHalf==0)
                                DecimalHalf.Value = 1;
                #elif !defined(AltNum_AllowNegativeZero)
                        if(DecimalHalf==0){
                            SetAsZero(); return; }
                #endif
                        }
                        IntHalf.Value = 0;
                    }
                    return;
                }
                else
                {
                    //X.Y * Z.V == ((X * Z) + (X * .V) + (.Y * Z) + (.Y * .V))
                    unsigned __int64 SRep = IntHalf == 0 ? DecimalHalf.Value : DecimalOverflowX * IntHalf.Value + DecimalHalf.Value;
                    SRep *= rValue.IntHalf.Value;//SRep holds __int64 version of X.Y * Z
                    //X.Y *.V
                    unsigned __int64 Temp03 = (__int64)(rValue.DecimalHalf * IntHalf.Value);//Temp03 holds __int64 version of X *.V
                    unsigned __int64 Temp04 = (__int64)DecimalHalf.Value * (__int64)rValue.DecimalHalf.Value;
                    Temp04 /= DecimalOverflow;
                    //Temp04 holds __int64 version of .Y * .V
                    unsigned __int64 IntegerRep = SRep + Temp03 + Temp04;
                    unsigned __int64 intHalf = IntegerRep / DecimalOverflow;
                    IntegerRep -= intHalf * DecimalOverflowX;
                    IntHalf.Value = (unsigned int) intHalf;
                    DecimalHalf.Value = (unsigned int)IntegerRep;
                }
            }
            #if !defined(AltNum_DisableMultiplyDownToNothingPrevention)
            if(DecimalHalf==0&&IntHalf==0)
                DecimalHalf.Value = 1;
            #elif !defined(AltNum_AllowNegativeZero)
            if(DecimalHalf==0)
                SetAsZero();
            #endif
        }

        template<VariantNameVariant VariantType=VariantName>
        void MultOpV1(const VariantType& rValue)
        {
            if(rValue.IsNegative())
            {
                SwapNegativeStatus();
                UnsignedMultOp(-rValue);
            }
            else
                UnsignedMultOp(rValue);
        }

public:

    #pragma endregion NormalRep AltNum Multiplication Operations

    #pragma region NormalRep Integer Addition Operations
protected:

        /// <summary>
        /// Basic addition operation between VariantName Variant and unsigned Integer value
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        void UIntAddOpV1(const IntType& rValue)
        {
            if(DecimalHalf.Value==0)
                IntHalf.NRepSkippingUIntAddOp(rValue);
            else {
                int signBeforeOp = IntHalf.Sign;
                IntHalf.UIntAddOp(rValue);
                if(signBeforeOp!=IntHalf.Sign)//Invert the decimal section
                    DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value;
            }
        }

        /// <summary>
        /// Basic addition operation between VariantName Variant and Integer value
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        void IntAddOpV1(const IntType& rValue)
        {
            if(DecimalHalf.Value==0)
                IntHalf.NRepSkippingIntegerAddOp(rValue);
            else {
                int signBeforeOp = IntHalf.Sign;
                IntHalf += rValue;
                if(signBeforeOp!=IntHalf.Sign)//Invert the decimal section
                    DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value;
            }
        }

public:

    #pragma endregion NormalRep Integer Addition Operations

    #pragma region NormalRep Integer Subtraction Operations
protected:

        /// <summary>
        /// Basic Subtraction operation between VariantName Variant and unsigned Integer value
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=unsigned int>
        void UIntSubOp(const IntType& rValue)
        {
            if(DecimalHalf.Value==0)
                IntHalf.NRepSkippingUIntSubOp(rValue);
            else {
                int signBeforeOp = IntHalf.Sign;
                IntHalf.UIntSubOp(rValue);
                if(signBeforeOp!=IntHalf.Sign)//Invert the decimal section
                    DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value;
            }
        }

        /// <summary>
        /// Basic Subtraction operation between VariantName Variant and Integer value
        /// that ignores special representation status
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue">The right side value</param>
        template<IntegerType IntType=signed int>
        void IntSubOp(const IntType& rValue)
        {
            if(DecimalHalf.Value==0)
                IntHalf.NRepSkippingIntegerSubOp(rValue);
            else {
                unsigned int signBeforeOp = IntHalf.Sign;
                IntHalf -= rValue;
                if(signBeforeOp!=IntHalf.Sign)//Invert the decimal section
                    DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value;
            }
        }

public:

    #pragma endregion NormalRep Integer Subtraction Operations

    #pragma region NormalRep AltNum Addition/Subtraction Operations
protected:

        /// <summary>
        /// Basic addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void UnsignedAddOpV1(const VariantName& rValue)
        {
            if(rValue.DecimalHalf==0)
                UnsignedMirroredAddOp(rValue.IntHalf);
            else
            {
                int signBeforeOp = IntHalf.Sign;
                IntHalf += rValue.IntHalf;
                if (signBeforeOp==MirroredInt::NegativeSign)
                {
                    if(DecimalHalf.Value==rValue.DecimalHalf.Value){//5.5 + -4.5
                        if(IntHalf.Value==0)
                            SetAsZero();
                        else
                            DecimalHalf.Value = 0;
                    }
                    else if(rValue.DecimalHalf.Value>DecimalHalf.Value)
                    {
                        ++IntHalf;
                        if(signBeforeOp!=IntHalf.Sign)//-1.6 + 2.7 = 1.1
                            DecimalHalf.Value = rValue.DecimalHalf.Value - DecimalHalf.Value;
                        else//-1.6 + .7 = -0.9
                            DecimalHalf.Value = DecimalOverflow + DecimalHalf.Value - rValue.DecimalHalf.Value;//10-7+6 = 9
                    } else if(signBeforeOp!=IntHalf.Sign)//-1.6 + 2.5 = 0.9
                        DecimalHalf.Value = DecimalOverflow + rValue.DecimalHalf.Value - DecimalHalf.Value;//10 - (6-5) == 10
                    else
                        DecimalHalf.Value -= rValue.DecimalHalf.Value;
                } else {
                    unsigned int decResult = DecimalHalf.Value + rValue.DecimalHalf.Value;
                    if(decResult==DecimalOverflow){//5.4 + 4.6
                        ++IntHalf;
                        if(IntHalf.Value==0)
                            SetAsZero();
                        else
                            DecimalHalf.Value = 0;
                    } else if(decResult>DecimalOverflow){//5.4 + 4.7
                        ++IntHalf;
                        DecimalHalf.Value = decResult - DecimalOverflow;
                    }
                    else if(signBeforeOp!=IntHalf.Sign)
                        DecimalHalf.Value = DecimalOverflow - decResult;
                    else
                        DecimalHalf.Value = decResult;
                }
            }
        }

        /// <summary>
        /// Basic addition Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void AddOpV1(const VariantName& rValue)
        {
            if (rValue.DecimalHalf == 0)
                MirroredAddOp(rValue.IntHalf);
            else {
                int signBeforeOp = IntHalf.Sign;
                IntHalf += rValue.IntHalf;

                if (signBeforeOp==MirroredInt::NegativeSign){
                    if(rValue.IsPositive()){
                        if(DecimalHalf.Value==rValue.DecimalHalf.Value){
                            if(IntHalf.Value==0)
                                SetAsZero();
                            else
                                DecimalHalf.Value = 0;
                        } else if(rValue.DecimalHalf.Value>DecimalHalf.Value){
                            ++IntHalf;
                            if(signBeforeOp!=IntHalf.Sign)//-1.6 + 2.7 = 1.1
                                DecimalHalf.Value = rValue.DecimalHalf.Value - DecimalHalf.Value;
                            else//-1.6 + .7 = -0.9
                                DecimalHalf.Value = DecimalOverflow + DecimalHalf.Value - rValue.DecimalHalf.Value;//10-7+6 = 9
                        } else if(signBeforeOp!=IntHalf.Sign)//-1.6 + 2.5 = 0.9
                            DecimalHalf.Value = DecimalOverflow + rValue.DecimalHalf.Value - DecimalHalf.Value;//10 - (6-5) == 10
                        else
                            DecimalHalf.Value -= rValue.DecimalHalf.Value;
                    } else {
                        unsigned int decResult = DecimalHalf.Value + rValue.DecimalHalf.Value;
                        if (decResult == DecimalOverflow) {//-5.4 + - 5.6
                            --IntHalf;
                            if (IntHalf.Value == 0)
                                SetAsZero();
                            else
                                DecimalHalf.Value = 0;
                        } else if (decResult > DecimalOverflow) {//-5.4 - 5.7 = -11.1
                            --IntHalf;
                            DecimalHalf.Value = decResult - DecimalOverflow;
                        } else//-5.2 - 5.2 = -10.4
                            DecimalHalf.Value = decResult;
                    }
                } else {
                    if(rValue.IsPositive()){
                        unsigned int decResult = DecimalHalf.Value + rValue.DecimalHalf.Value;
                        if(decResult==DecimalOverflow){//5.5 + 4.5 = 10
                            ++IntHalf;
                            if(IntHalf.Value==0)
                                SetAsZero();
                            else
                                DecimalHalf.Value = 0;
                        } else if(decResult>DecimalOverflow){//5.5 + 4.6 = 10.1
                            ++IntHalf;
                            DecimalHalf.Value = decResult - DecimalOverflow;
                        } else//5.4 + 5.3 = 10.7
                            DecimalHalf.Value = decResult;
                    } else {
                        if(DecimalHalf.Value==rValue.DecimalHalf.Value){//5.5 + -5.5
                            if(IntHalf.Value==0)
                                SetAsZero();
                            else
                                DecimalHalf.Value = 0;
                        } else if(rValue.DecimalHalf.Value>DecimalHalf.Value){
                            --IntHalf;
                            if(signBeforeOp!=IntHalf.Sign)//4.3 - 5.4 = -1.1
                                DecimalHalf.Value = rValue.DecimalHalf.Value - DecimalHalf.Value;
                            else//4.3 - 2.4 = 1.9
                                DecimalHalf.Value = DecimalOverflow + DecimalHalf.Value - rValue.DecimalHalf.Value;
                        } else if(signBeforeOp!=IntHalf.Sign)//5.4 + - 6.3 = -0.9
                            DecimalHalf.Value = DecimalOverflow + rValue.DecimalHalf.Value - DecimalHalf.Value;//10 + 3 - 4
                        else//4.4 + -2.3 = 2.1
                            DecimalHalf.Value -= rValue.DecimalHalf.Value;
                    }
                }
            }
        }

        /// <summary>
        /// Basic subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void BasicUnsignedSubOp(const VariantName& rValue)
        {
            if (rValue.DecimalHalf == 0)
                UnsignedMirroredSubOp(rValue.IntHalf);
            else {
                int signBeforeOp = IntHalf.Sign;
                IntHalf -= rValue.IntHalf;
                if (signBeforeOp==MirroredInt::NegativeSign){//-5 - B
                    unsigned int decResult = DecimalHalf.Value + rValue.DecimalHalf.Value;
                    if (decResult == DecimalOverflow){//-5.4 - 5.6
                        --IntHalf;
                        if (IntHalf.Value == 0)
                            SetAsZero();
                        else
                            DecimalHalf.Value = 0;
                    } else if (decResult > DecimalOverflow) {//-5.4 - 5.7 = -11.1
                        --IntHalf;
                        DecimalHalf.Value = decResult - DecimalOverflow;
                    } else//-5.2 - 5.2 = -10.4
                        DecimalHalf.Value = decResult;
                } else {//5.XX - B
                    if(DecimalHalf.Value==rValue.DecimalHalf.Value){//5.5 - 5.5 = 10
                        if(IntHalf.Value==0)
                            SetAsZero();
                        else
                            DecimalHalf.Value = 0;
                    } else if(rValue.DecimalHalf.Value>DecimalHalf.Value){
                        --IntHalf;
                        if(signBeforeOp!=IntHalf.Sign)//5.4 - 5.7 = -0.3
                            DecimalHalf.Value = rValue.DecimalHalf.Value - DecimalHalf.Value;
                        else//5.4 - 3.6 = 1.8
                            DecimalHalf.Value = DecimalOverflow + DecimalHalf.Value - rValue.DecimalHalf.Value;
                    } else if(signBeforeOp!=IntHalf.Sign)//5.3 - 7.2 = -1.9
                        DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value + rValue.DecimalHalf.Value;
                    else//5.4 - 5.3 = 0.1
                        DecimalHalf.Value -= rValue.DecimalHalf.Value;
                }
            }
        }

        /// <summary>
        /// Basic subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void BasicSubOp(const VariantName& rValue)
        {
            if(rValue.DecimalHalf==0)
                MirroredSubOp(rValue.IntHalf);
            else
            {
                int signBeforeOp = IntHalf.Sign;
                IntHalf -= rValue.IntHalf;
                if (signBeforeOp==MirroredInt::NegativeSign)
                {
                    if(rValue.IsPositive())
                    {
                        unsigned int decResult = DecimalHalf.Value + rValue.DecimalHalf.Value;
                        if(decResult==DecimalOverflow){//-5.4 - 5.6
                            --IntHalf;
                            if(IntHalf.Value==0)
                                SetAsZero();
                            else
                                DecimalHalf.Value = 0;
                        } else if(decResult>DecimalOverflow){//-5.4 - 5.7 = -11.1
                            --IntHalf;
                            DecimalHalf.Value = decResult - DecimalOverflow;
                        } else//-5.2 - 5.2 = -10.4
                            DecimalHalf.Value = decResult;
                    } else {
                        if(DecimalHalf.Value==rValue.DecimalHalf.Value){//-5.4 - -4.4
                            if(IntHalf.Value==0)
                                SetAsZero();
                            else
                                DecimalHalf.Value = 0;
                        } else if(rValue.DecimalHalf.Value>DecimalHalf.Value){
                            ++IntHalf;
                            if(signBeforeOp!=IntHalf.Sign)//-5.4 - -6.5 = 1.1
                                DecimalHalf = DecimalOverflow - DecimalHalf.Value - rValue.DecimalHalf;
                            else//-5.4 - -4.5 = -0.9 == -5.4 + 4.5
                                DecimalHalf.Value += rValue.DecimalHalf.Value;
                        } else if(signBeforeOp!=IntHalf.Sign)//-5.4 - -7.3 = 1.9
                            DecimalHalf.Value = DecimalOverflow + rValue.DecimalHalf.Value - DecimalHalf.Value;
                        else//-5.4 - -3.3 = -2.1
                            DecimalHalf.Value -= rValue.DecimalHalf.Value;
                    }
                } else {
                    if(rValue.IsPositive()){
                        if (DecimalHalf.Value == rValue.DecimalHalf.Value) {//5.5 - 5.5 = 10
                            if (IntHalf.Value == 0)
                                SetAsZero();
                            else
                                DecimalHalf.Value = 0;
                        }
                        else if (rValue.DecimalHalf.Value > DecimalHalf.Value) {
                            --IntHalf;
                            if (signBeforeOp != IntHalf.Sign)//5.4 - 5.7 = -0.3
                                DecimalHalf.Value = rValue.DecimalHalf.Value - DecimalHalf.Value;
                            else//5.4 - 3.6 = 1.8
                                DecimalHalf.Value = DecimalOverflow + DecimalHalf.Value - rValue.DecimalHalf.Value;
                        }
                        else if (signBeforeOp != IntHalf.Sign)//5.3 - 7.2 = -1.9
                            DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value + rValue.DecimalHalf.Value;
                        else//5.4 - 5.3 = 0.1
                            DecimalHalf.Value -= rValue.DecimalHalf.Value;
                    } else {
                        if(DecimalHalf.Value==rValue.DecimalHalf.Value){//5.5 - -5.5 = 11
                            ++IntHalf;
                            if(IntHalf.Value==0)
                                SetAsZero();
                            else
                                DecimalHalf.Value = 0;
                        } else if(rValue.DecimalHalf.Value>DecimalHalf.Value){//5.4 - -5.7 = 11.1
                            ++IntHalf;
                            DecimalHalf.Value = DecimalHalf.Value + rValue.DecimalHalf.Value - DecimalOverflow;
                        } else//5.4 - -5.3 = 10.7
                            DecimalHalf.Value += rValue.DecimalHalf.Value;
                    }
                }
            }
        }

    #pragma endregion NormalRep AltNum Addition/Subtraction Operations
    };
}