							switch(RRep)
							{
								case RepType::PiNum:{
								} break;
		#pragma region AltDecVariantExclusive
							#if defined(AltNum_EnablePowerOfRepresentation)
								case RepType::PiPower:{
								} break;
							#endif
							#if defined(AltNum_EnableFractionals)
								case RepType::PiNumByDiv:{
								} break;
							#endif
							#if defined(AltNum_EnableMixedFractional)
								case RepType::MixedPi:{
								} break;
							#endif
		#pragma endregion AltDecVariantExclusive
							#if defined(AltNum_EnableApproaching)
								case RepType::ApproachingBottomPi:{
								} break;
								#if !defined(AltNum_DisableApproachingTop)
								case RepType::ApproachingTopPi:{
								} break;
								#endif
							#endif
		#pragma region AltDecVariantExclusive
							#if defined(AltNum_EnableApproachingDivided)
								case RepType::ApproachingMidLeftPi:{
								} break;
								#if !defined(AltNum_DisableApproachingTop)
								case RepType::ApproachingMidRightPi:{
								} break;
								#endif
							#endif
		#pragma endregion AltDecVariantExclusive
							default:
								throw "Unsupported operation"; break;
							}
