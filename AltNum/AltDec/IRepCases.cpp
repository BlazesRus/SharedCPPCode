						switch(RRep){
							//case RepType::INum:{
							//} break;
		#pragma region AltDecVariantExclusive
						#if defined(AltNum_EnableFractionals)
							//case RepType::INumByDiv:{
							//} break;
						#endif
						#if defined(AltNum_EnableMixedFractional)
							//case RepType::MixedI:{
							//} break;
						#endif
		#pragma endregion AltDecVariantExclusive
						#if defined(AltNum_EnableApproaching)
							//case RepType::ApproachingImaginaryBottom:{
							//} break;
							#if !defined(AltNum_DisableApproachingTop)
							//case RepType::ApproachingImaginaryTop:{
							//} break;
							#endif
						#endif
		#pragma region AltDecVariantExclusive
						#if defined(AltNum_EnableApproachingDivided)
							//case RepType::ApproachingImaginaryMidLeft:{
							//} break;
							//case RepType::ApproachingImaginaryMidRight:{
							//} break;
						#endif
		#pragma endregion AltDecVariantExclusive
		#if defined(AltNum_EnableImaginaryInfinity)
							//case RepType::ImaginaryInfinity:
							//	break;
		#endif
							default:
								CatchAllMultiplication(rValue, LRep, RRep); break;
						}