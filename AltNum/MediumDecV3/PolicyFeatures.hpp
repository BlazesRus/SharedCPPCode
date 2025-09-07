// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once

//PolicyFeatures.hpp
namespace BlazesRusCode
{
  //Undefined Policy important flags will be set to default values if needed(no need to inherit from DefaultPolicy)

	struct CoreMediumDecFeatures {
    static constexpr bool PositiveSign = true; // 1 means positive
    using StoreT      = u64;
		
    //Defines default type for IntHalf portion(IntHalfValueT is for split fields .Value field)
    using IntHalfT      = u32;
    //Defines default type for DecimalHalf portion
    using DecimalHalfT      = u32;
		
    static constexpr IntHalfT IntHalfZero = 0;
    static constexpr DecimalHalfT DecimalHalfZero = 0;
    static constexpr IntHalfT IntHalfOne = 1;
    static constexpr DecimalHalfT DecimalHalfOne = 1;
	};
	
	struct SignedFeatures {
    static constexpr bool UnsignedMode = false;
		
    static constexpr unsigned IntHalfBits = 31;
    static constexpr unsigned DecimalHalfBits = 30;
    static constexpr unsigned ValueBits = 61;
		
	};
	
	struct ClassicMediumDecFeatures {
    static constexpr bool PositiveSign = true;//Not Actually handled by masks but by MirroredInt but defining for debug purposes
    static constexpr bool UnsignedMode = false;
    static constexpr bool SplitFieldsMode = true;
    //Defines default type for IntHalf portion
    using IntHalfT      = MirroredInt;
    using IntHalfValueT      = u32;
    //Defines default type for DecimalHalf portion
    using DecimalHalfT      = PartialInt;
    using DecimalHalfValueT      = u32;
    static constexpr IntHalfT IntHalfZero = MirroredInt::Zero;
    static constexpr DecimalHalfT DecimalHalfZero = PartialInt::Zero;
    static constexpr IntHalfT IntHalfOne = MirroredInt::One;
    static constexpr DecimalHalfT DecimalHalfOne = PartialInt::One;
	};
	
	struct UnsignedFeatures {
    static constexpr bool UnsignedMode = true;

    static constexpr unsigned IntHalfBits = 32;
    static constexpr unsigned DecimalHalfBits = 30;
    static constexpr unsigned ValueBits = 62;
		
    static constexpr unsigned MaxIntHalf = 4294967295; 
    static constexpr unsigned MinIntHalf = 0;
	};
	
	struct MediumDecV2MainFeatures {
    static constexpr bool EnablePi = true;
    static constexpr bool EnableE = true;
		//static constexpr bool EnableI = true; if need imaginary numbers
		
    //Pi,e,i state flags
    static constexpr unsigned stateFlagBits = 2;
	};
	
	struct ApproachingFeatures {
    static constexpr bool EnableApproaching       = true;
	  //EnableApproachingDivided requires EnableExtraRep (will be adjusted if EnableMaskedExtraRep not set)
    static constexpr bool EnableApproachingDivided= true;
	};
	
	struct AltDecFeatures {
    static constexpr bool EnableMaskedExtraRep    = true;
    using ExtraRepT                                = u32;
    static constexpr ExtraRepHalfT DecimalHalfZero = 0;
	};
	
	struct MixedDecFeatures {
    static constexpr bool EnableMaskedExtraRep    = true;
    using ExtraRepT                                = u32;
    static constexpr ExtraRepHalfT DecimalHalfZero = 0;
	};
	
	struct ExperimentalFeatures {
	 //Treat ^ as meaning power of instead of XOR
	 static constexpr bool CaretIsPower = true;
	 
	 //Implies ExtraRep is enabled(sets to default policy values if not defined by policy)
	 static constexpr bool EnablePowerOf = true;
	 static constexpr bool EnableWithinMinMaxRange = true;
   static constexpr bool EnableIndeterminateForms = true;
   static constexpr bool EnableNaN = true;
   static constexpr bool EnableInfinity = true;
	 //Experimental +- Result sign(requires reserving a bit) for results like square root of negative number which are techically both positive and negative at same time
	 static constexpr bool EnableDualPositiveNegative = true;
	};
	
  template<typename T>
  concept IsAltDecType =
    (requires { { T::EnableExtraRep } -> std::convertible_to<bool>; } && T::EnableExtraRep);
	
  template<typename T>
  concept IsMixedDecType =
    (requires { { T::HasTail } -> std::convertible_to<bool>; } && T::HasTail);
}