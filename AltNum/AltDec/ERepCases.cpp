					switch(RRep)
					{
						case RepType::ENum:{
						} break;
#pragma region AltDecVariantExclusive
					#if defined(AltNum_EnablePowerOfRepresentation)
						case RepType::EPower:{
						} break;
					#endif
					#if defined(AltNum_EnableFractionals)
						case RepType::ENumByDiv:{
						} break;
					#endif
					#if defined(AltNum_EnableMixedFractional)
						case RepType::MixedE:{
						} break;
					#endif
#pragma endregion AltDecVariantExclusive
					#if defined(AltNum_EnableApproaching)
						case RepType::ApproachingBottomE:{
						} break;
						#if !defined(AltNum_DisableApproachingTop)
						case RepType::ApproachingTopE:{
						} break;
						#endif
					#endif
#pragma region AltDecVariantExclusive
					#if defined(AltNum_EnableApproachingDivided)
						case RepType::ApproachingMidLeftE:{
						} break;
						#if !defined(AltNum_DisableApproachingTop)
						case RepType::ApproachingMidRightE:{
						} break;
						#endif
					#endif
#pragma endregion AltDecVariantExclusive
					default:
						throw "Unsupported operation"; break;
					}