							switch(RRep)
							{
								case RepType::NormalType:{
								}; break;
		#pragma region AltDecVariantExclusive
						#if defined(AltNum_EnableFractionals)
								case RepType::NumByDiv:{
								}; break;
						#endif
						#if defined(AltNum_EnablePowerOfRepresentation)
								case RepType::ToPowerOf:{
								}; break;
						#endif
						#if defined(AltNum_EnableMixedFractional)
								case RepType::MixedFrac:{
								}; break;
						#endif
		#pragma endregion AltDecVariantExclusive
								case RepType::ApproachingBottomRep:{
								} break;
								case RepType::ApproachingTopRep:{
								} break;
		#pragma region AltDecVariantExclusive
						#if defined(AltNum_EnableApproachingDivided)
								case RepType::ApproachingMidLeft:{
								} break;
							#if !defined(AltNum_DisableApproachingTop)
								case RepType::ApproachingMidRight:{
								} break;
							#endif
						#endif
		#pragma endregion AltDecVariantExclusive
								case RepType::InfinityRep:{
								} break;
								default:
									throw "Unsupported operation";
							}