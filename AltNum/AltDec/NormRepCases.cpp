						switch(RRep){
	//						case RepType::NormalType:{
	//						}; break;
	#pragma region AltDecVariantExclusive
					#if defined(AltNum_EnableFractionals)
	//						case RepType::NumByDiv:{
	//						}; break;
					#endif
					#if defined(AltNum_EnablePowerOfRepresentation)
	//						case RepType::ToPowerOf:{
	//						}; break;
					#endif
					#if defined(AltNum_EnableMixedFractional)
	//						case RepType::MixedFrac:{
	//						}; break;
					#endif
	#pragma endregion AltDecVariantExclusive
	//						case RepType::ApproachingBottomRep:{
	//						} break;
						#if !defined(AltNum_DisableApproachingTop)
	//						case RepType::ApproachingTopRep:{
	//						} break;
						#endif
	#pragma region AltDecVariantExclusive
					#if defined(AltNum_EnableApproachingDivided)
	//						case RepType::ApproachingMidLeft:{
	//						} break;
	//						case RepType::ApproachingMidRight:{
	//						} break;
					#endif
	#pragma endregion AltDecVariantExclusive
					#if defined(AltNum_EnableInfinityRep)
							case RepType::InfinityRep:
								SetValue(rValue);
					#endif
					#if defined(AltNum_EnableNaN)
							case RepType::Undefined:
							case RepType::NaN:
					#endif
					#if defined(AltNum_EnableNil)
							case RepType::Nil:
					#endif
					#if defined(AltNum_EnableNil)||defined(AltNum_EnableNaN)
								throw "Unsupported operation";
					#endif
					#if defined(AltNum_EnableUndefinedButInRange)
							case RepType::UndefinedButInRange:
								throw "Unsupported operation";
						#if defined(AltNum_EnableWithinMinMaxRange)
							case RepType::WithinMinMaxRange:
								throw "WithinMinMaxRange needs to be updated";
						#endif
					#endif
                            default:
                                CatchAllAddition(rValue, LRep, RRep); break;
						}