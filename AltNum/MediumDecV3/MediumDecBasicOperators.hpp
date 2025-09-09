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
      // Right-side unsigned integer add: this + rhsInt
      template<typename UIntT, typename = std::enable_if_t<!SplitFieldsMode>>
      void Bitwise_UIntAddOp(const UIntT& rhsInt) noexcept {
          if constexpr (UnsignedMode) {
              if constexpr (BinaryFractionMode) {
                  SetRawValue(RawValue() + (ValueT(rhsInt) << DECIMAL_BITS));
              } else {
                  // Decimal: integer-only add; fractional lane unchanged
                  SetValue(GetIntHalf() + static_cast<IntHalfValueT>(rhsInt), GetDecimalHalf());
              }
              return;
          }
      
          const bool signBefore = IsPositive();
      
          if constexpr (BinaryFractionMode) {
              const ValueT rhsRaw = (ValueT(rhsInt) << DECIMAL_BITS);
              const ValueT lhsRaw = RawValue();
      
              if (signBefore) {
                  SetRawValue(lhsRaw + rhsRaw);
              } else {
                  // -mag + rhsInt == rhsRaw - lhsRaw in magnitude space
                  if (rhsRaw >= lhsRaw) {
                      SetRawValue(rhsRaw - lhsRaw);
                      FlipSign(); // becomes positive
                  } else {
                      SetRawValue(lhsRaw - rhsRaw);
                      // stays negative
                  }
              }
          } else {
              // Decimal mode
              IntHalfValueT lhsInt = GetIntHalf();
              DecimalHalfValueT lhsDec = GetDecimalHalf();
              const IntHalfValueT r = static_cast<IntHalfValueT>(rhsInt);
      
              if (signBefore) {
                  // +mag + r: simple integer add, fractional unchanged
                  lhsInt += r;
                  SetValue(lhsInt, lhsDec);
              } else {
                  // -mag + r == r - (lhsInt + lhsDec/DO)
                  if (r > lhsInt || (r == lhsInt && lhsDec > 0)) {
                      // Flip to positive; mirror fractional over DecimalOverflow when lhsDec>0
                      const IntHalfValueT newInt = r - lhsInt - (lhsDec > 0 ? 1 : 0);
                      const DecimalHalfValueT newDec = (lhsDec == 0) ? 0 : (DecimalOverflow - lhsDec);
                      SetValue(newInt, newDec);
                      FlipSign();
                  } else {
                      // Remain negative; magnitude shrinks by r
                      lhsInt -= r;
                      SetValue(lhsInt, lhsDec);
                  }
              }
          }
      }
      
      // Right-side unsigned integer sub: this - rhsInt
      template<typename UIntT, typename = std::enable_if_t<!SplitFieldsMode>>
      void Bitwise_UIntSubOp(const UIntT& rhsInt) noexcept {
          if constexpr (UnsignedMode) {
              if constexpr (BinaryFractionMode) {
                  SetRawValue(RawValue() - (ValueT(rhsInt) << DECIMAL_BITS));
              } else {
                  // Decimal: subtract integer; if fractional > 0 we borrow
                  IntHalfValueT lhsInt = GetIntHalf();
                  DecimalHalfValueT lhsDec = GetDecimalHalf();
      
                  if (lhsDec > 0) {
                      // borrow 1 from integer to keep decimal invariant
                      if (lhsInt == 0) {
                          // result goes negative
                          IntHalfValueT newInt = static_cast<IntHalfValueT>(rhsInt) - 1;
                          DecimalHalfValueT newDec = DecimalOverflow - lhsDec;
                          SetValue(newInt, newDec);
                          // No sign lane in UnsignedMode; if you ever add one, flip here.
                      } else {
                          --lhsInt;
                          lhsDec = DecimalOverflow - lhsDec;
                          // then subtract rhsInt
                          if (lhsInt >= rhsInt) {
                              lhsInt -= static_cast<IntHalfValueT>(rhsInt);
                              SetValue(lhsInt, lhsDec);
                          } else {
                              // underflow to negative magnitude — only possible if you later enable sign lane
                              SetValue(0, 0);
                          }
                      }
                  } else {
                      // no borrow needed
                      if (GetIntHalf() >= rhsInt) {
                          SetValue(GetIntHalf() - static_cast<IntHalfValueT>(rhsInt), 0);
                      } else {
                          // underflow to negative magnitude — only possible if you later enable sign lane
                          SetValue(0, 0);
                      }
                  }
              }
              return;
          }
      
          const bool signBefore = IsPositive();
      
          if constexpr (BinaryFractionMode) {
              const ValueT rhsRaw = (ValueT(rhsInt) << DECIMAL_BITS);
              const ValueT lhsRaw = RawValue();
      
              // this - rhsRaw in magnitude space with explicit sign management
              if (signBefore) {
                  if (lhsRaw >= rhsRaw) {
                      SetRawValue(lhsRaw - rhsRaw);  // stays positive
                  } else {
                      SetRawValue(rhsRaw - lhsRaw);  // flips to negative
                      FlipSign();
                  }
              } else {
                  // negative - positive == -(mag + rhs)
                  SetRawValue(lhsRaw + rhsRaw); // magnitude grows, sign stays negative
              }
          } else {
              // Decimal mode
              IntHalfValueT lhsInt = GetIntHalf();
              DecimalHalfValueT lhsDec = GetDecimalHalf();
              const IntHalfValueT r = static_cast<IntHalfValueT>(rhsInt);
      
              if (signBefore) {
                  // +mag - r
                  if (r > lhsInt || (r == lhsInt && lhsDec > 0)) {
                      // Flip to negative; mirror fractional over DecimalOverflow if lhsDec>0
                      IntHalfValueT newInt = r - lhsInt - (lhsDec > 0 ? 1 : 0);
                      DecimalHalfValueT newDec = (lhsDec == 0) ? 0 : (DecimalOverflow - lhsDec);
                      SetValue(newInt, newDec);
                      FlipSign();
                  } else {
                      // Stay positive; borrow only if fractional present
                      if (lhsDec > 0) {
                          --lhsInt;
                          lhsDec = DecimalOverflow - lhsDec;
                      }
                      lhsInt -= r;
                      SetValue(lhsInt, lhsDec);
                  }
              } else {
                  // -mag - r == -(mag + r), magnitude increases; fractional unchanged
                  lhsInt += r;
                  SetValue(lhsInt, lhsDec);
                  // sign remains negative
              }
          }


      template<UnsignedIntegerType IntType=unsigned int, typename = std::enable_if_t<!SplitFieldsMode>>
      void Bitwise_UIntAddOp(const IntType& r) const noexcept {
        if (constexpr(UnsignedMode)){

        } else if constexpr (BinaryFractionMode) {
        } else {
          if(IsPositive()){
            Value() += r;
          } else
            magSubOrFlip(r, false);
        }
      }

      template<UnsignedIntegerType IntType=unsigned int, typename = std::enable_if_t<!SplitFieldsMode>>
      void Bitwise_UIntSubOp(const IntType& r) const noexcept {
        if (constexpr(UnsignedMode)){
          Value() -= r;
        } else if constexpr (BinaryFractionMode) {
        } else {
          if(IsPositive()){
            Value() += r;
          } else
            magSubOrFlip(r, false);
        }
      }

      //If signBeforeOp = true, then sign was positive before operation
      template<typename = std::enable_if_t<!SplitFieldsMode>
      void magSubOrFlip(const VariantName& rhs, bool signBeforeOp) noexcept {
          if constexpr (BinaryFractionMode) {
              // Binary fixed-point: packed RawValue is true magnitude
              auto lhsRaw = RawValue();
              auto rhsRaw = rhs.RawValue();
              if (rhsRaw > lhsRaw) {
                  SetRawValue(rhsRaw - lhsRaw);
                  FlipSign();
              } else {
                  SetRawValue(lhsRaw - rhsRaw);
              }
          } else {
              // Decimal mode
              if (rhs.GetDecimalHalf() == 0) {
                  // Right integer-only fast path
                  magIntSubOrFlip(rhs.GetIntHalf(), signBeforeOp);
                  return;
              }
      
              auto lhsInt = GetIntHalf();
              auto lhsDec = GetDecimalHalf();
              auto rhsInt = rhs.GetIntHalf();
              auto rhsDec = rhs.GetDecimalHalf();
      
              bool flip = false;
              if (rhsInt > lhsInt || (rhsInt == lhsInt && rhsDec > lhsDec)) {
                  std::swap(lhsInt, rhsInt);
                  std::swap(lhsDec, rhsDec);
                  flip = true;
              }
              if (lhsDec < rhsDec) {
                  lhsDec += DecimalOverflow;
                  --lhsInt;
              }
              lhsDec -= rhsDec;
              lhsInt -= rhsInt;
      
              SetValue(lhsInt, lhsDec);
              if (flip) FlipSign();
          }
      }
      
      // Unsigned subtract
      void Bitwise_UnsignedSubOp(const VariantName& rhs){
          if constexpr (BinaryFractionMode) {
              auto lhsRaw = RawValue();
              auto rhsRaw = rhs.RawValue();
              if constexpr (UnsignedMode) {
                if constexpr (EnableOverflowProtection) {
                  if (rhsRaw > lhsRaw) {
                   throw std::underflow_error("Unsigned underflow");
                  }
                }
                SetRawValue(lhsRaw - rhsRaw);
              } else
                //Code needs rewritten to account for signs
 
              }
          else if constexpr (UnsignedMode) {
            auto lhsRaw = RawValue();
            auto rhsRaw = rhs.RawValue();
            if constexpr (EnableOverflowProtection) {
              if (rhsRaw > lhsRaw) {
                throw std::underflow_error("Unsigned underflow");
              }
            }
            ValueT result = lhsRaw - rhsRaw;
            DecimalHalfValueT decPart = splitDecPart(result);
            if(decPart>DecimalOverflow){
              result += OneIntPackedMinusDecimalOverflow;
            }
            SetRawValue(result);
          } else {
              if (rhs.GetDecimalHalf() == 0) {
                  UIntSubOp(rhs.GetIntHalf()); // integer fast path
              } else {
                  magSubOrFlip(rhs, true);
              }
          }
      }
      
      // Signed subtract
      void Bitwise_SubOp(const VariantName& rhs){
          if constexpr (UnsignedMode) {
              Bitwise_UnsignedSubOp(rhs);
          } else {
            rightIsPositive = rhs.IsPositive();
            if (IsPositive())
              rightIsPositive ? Bitwise_UnsignedSubOp(rhs): Bitwise_UnsignedAddOp(rhs);
            else
              rightIsPositive ? Bitwise_UnsignedAddOp(rhs): Bitwise_UnsignedSubOp(rhs);
          }
      }
    #pragma region NormalRep Integer Division Operations

        template<IntegerType IntType=unsigned int>
        void PartialUIntDivOp(const IntType& rValue)
        {//Avoid using with special status representations such as approaching zero or result will be incorrect
          if constexpr (VariantName::SplitFieldsMode) {
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
				  if constexpr (Policy::SplitFieldsMode) {
            if(DecimalHalf.Value==0)
                IntHalf.NRepSkippingUIntAddOp(rValue);
            else {
                int signBeforeOp = IntHalf.Sign;
                IntHalf.UIntAddOp(rValue);
                if(signBeforeOp!=IntHalf.Sign)//Invert the decimal section
                    DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value;
            }
					} else {
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
	        if constexpr (Policy::SplitFieldsMode) {
            if(DecimalHalf.Value==0)
                IntHalf.NRepSkippingIntegerAddOp(rValue);
            else {
                int signBeforeOp = IntHalf.Sign;
                IntHalf += rValue;
                if(signBeforeOp!=IntHalf.Sign)//Invert the decimal section
                    DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value;
            }
					} else
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
          if constexpr (VariantName::SplitFieldsMode) {
            if constexpr(VariantName::UnsignedMode) {
            } else {
              if(DecimalHalf.Value==0)
                  IntHalf.NRepSkippingUIntSubOp(rValue);
              else {
                  int signBeforeOp = IntHalf.Sign;
                  IntHalf.UIntSubOp(rValue);
                  if(signBeforeOp!=IntHalf.Sign)//Invert the decimal section
                      DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value;
              }
            }
          } else if constexpr(VariantName::UnsignedMode) {
          } else {
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
          if constexpr (VariantName::SplitFieldsMode) {
            if constexpr(VariantName::UnsignedMode) {
            } else {
              if(DecimalHalf.Value==0)
                  IntHalf.NRepSkippingIntegerSubOp(rValue);
              else {
                  unsigned int signBeforeOp = IntHalf.Sign;
                  IntHalf -= rValue;
                  if(signBeforeOp!=IntHalf.Sign)//Invert the decimal section
                      DecimalHalf.Value = DecimalOverflow - DecimalHalf.Value;
              }
            }
          } else if constexpr(VariantName::UnsignedMode) {
          } else {
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
          if constexpr (Policy::SplitFieldsMode) {
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
          } else
            if constexpr (BinaryFractionMode) {
              if constexpr (UnsignedMode) {
                SetRawValue(RawValue() + rhs.RawValue());
              } else {
                if(IsPositive())
                  SetRawValue(RawValue() + rhs.RawValue());
                else {
                  //Update code
                }
              }
            } else {
                if (rhs.GetDecimalHalf() == 0) {
                    UIntAddOp(rhs.GetIntHalf()); // integer fast path
                } else {
                    auto sum = RawValue() + rhs.RawValue();
                    auto intHalf = sum >> DECIMAL_BITS;
                    auto decHalf = sum & ((ValueT(1) << DECIMAL_BITS) - 1);
                    if (decHalf >= DecimalOverflow) {
                        decHalf -= DecimalOverflow;
                        ++intHalf;
                    }
                    SetValue(intHalf, decHalf);
                }
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
          if constexpr (Policy::SplitFieldsMode) {
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
          } else {
            if constexpr (UnsignedMode) {
                Bitwise_UnsignedAddOp(rhs);
            } else {
              rightIsPositive = rhs.IsPositive();
              if (IsPositive())
                rightIsPositive ? Bitwise_UnsignedAddOp(rhs):magSubOrFlip(rhs, true);
              else
                rightIsPositive ? magSubOrFlip(rhs, false)
                                : Bitwise_UnsignedAddOp(rhs);
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
          if constexpr (Policy::SplitFieldsMode) {
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
          } else {
          }
        }

        /// <summary>
        /// Basic subtraction Operation
        /// (Modifies owner object)
        /// </summary>
        /// <param name="rValue.">The right side rValue</param>
        void BasicSubOp(const VariantName& rValue)
        {
          if constexpr (Policy::SplitFieldsMode) {
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
          } else {

          }
        }

    #pragma endregion NormalRep AltNum Addition/Subtraction Operations
    };
}