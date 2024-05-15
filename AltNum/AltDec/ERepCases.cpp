					switch(RRep)
					{
						case RepType::ENum:{
							switch(RRep){
							}
						} break;
#pragma region AltDecVariantExclusive
					#if defined(AltNum_EnablePowerOfRepresentation)
						case RepType::EPower:{
							switch(RRep){
							}
						} break;
					#endif
					#if defined(AltNum_EnableFractionals)
						case RepType::ENumByDiv:{
							switch(RRep){
							}
						} break;
					#endif
					#if defined(AltNum_EnableMixedFractional)
						case RepType::MixedE:{
							switch(RRep){
							}
						} break;
					#endif
#pragma endregion AltDecVariantExclusive
					#if defined(AltNum_EnableApproaching)
						case RepType::ApproachingBottomE:{
							switch(RRep){
							}
						} break;
						#if !defined(AltNum_DisableApproachingTop)
						case RepType::ApproachingTopE:{
							switch(RRep){
							}
						} break;
						#endif
					#endif
#pragma region AltDecVariantExclusive
					#if defined(AltNum_EnableApproachingDivided)
						case RepType::ApproachingMidLeftE:{
							switch(RRep){
							}
						} break;
						#if !defined(AltNum_DisableApproachingTop)
						case RepType::ApproachingMidRightE:{
							switch(RRep){
							}
						} break;
						#endif
					#endif
#pragma endregion AltDecVariantExclusive
					default:
						throw "Unsupported operation"; break;
					}