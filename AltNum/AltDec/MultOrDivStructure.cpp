#include "AltDecBase.hpp"
using MediumDecVariant = BlazesRusCode::AltDecBase;
using RepType = BlazesRusCode::RepType;

void CatchAllPiOp(const auto& rValue, const RepType& RRep)
{
	RepType RRep = rValue.GetPiRepType();
	auto RValue = rValue.ConvertAsNormType(RRep);
	BasicUnsigned___Op(RValue);
}

void CatchAllEOp(const auto& rValue, const RepType& RRep)
{
	RepType RRep = rValue.GetERepType();
	auto RValue = rValue.ConvertAsNormType(RRep);
	BasicUnsigned___Op(RValue);
}

//UnsignedMultOp or UnsignedDivOp
auto& MediumDecVariant::Unsigned___Op(const auto& rValue)
{
	switch(DecimalHalf.Flags)
	{
#if defined(AltNum_EnablePiRep)//PiRep_to_others
		case 1:
		{
			RepType LRep = GetPiRepType();
			switch(rValue.DecimalHalf.Flags)
			{
#pragma region PiRep_to_PiRep
				case 1:{
					RepType RRep = rValue.GetPiRepType();
					if(LRep==RRep)
					{
                        switch(LRep)
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
					}
					else
					{
                        switch(LRep)
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
					}
				} break;
#pragma endregion PiRep_to_PiRep
	#if defined(AltNum_EnableERep)
				case 2:
					CatchAllEOp(rValue, RRep); break;
	#endif
	#if defined(AltNum_EnableIRep)
				case 3:{
					RepType RRep = rValue.GetIRepType();
					switch(LRep)
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
				} break;
	#endif
#pragma region PiRep_to_NormRep
				default:{
					RepType RRep = rValue.GetNormRepType();
					switch(LRep)
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
				} break;
#pragma endregion PiRep_to_NormRep
		} break;
#endif
#if defined(AltNum_EnableERep)//ERep_to_others
		case 2:
		{
			RepType LRep = GetERepType();
			switch(rValue.DecimalHalf.Flags)
			{
	#if defined(AltNum_EnablePiRep)
				case 1:
					CatchAllPiOp(rValue, RRep); break;
	#endif
#pragma region ERep_to_ERep
				case 2:
				{
					RepType RRep = rValue.GetERepType();
					if(LRep==RRep)
					{
                        switch(LRep)
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
					}
					else
					{
                        switch(LRep)
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
					}
				} break;
#pragma endregion ERep_to_ERep
	#if defined(AltNum_EnableIRep)
				case 3:{//ERep_to_IRep
					RepType RRep = rValue.GetIRepType();
					switch(LRep)
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
				} break;
	#endif
				default:{//ERep_to_NormalRep
					RepType RRep = rValue.GetNormRepType();
					switch(LRep)
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
				} break;
		} break;
#endif
#if defined(AltNum_EnableIRep)//IRep_to_others
		case 3:
		{
			RepType LRep = GetIRepType();
			switch(rValue.DecimalHalf.Flags)
			{
	#if defined(AltNum_EnablePiRep)
				case 1:
					CatchAllPiOp(rValue, RRep); break;
	#endif
				case 2:
					CatchAllEOp(rValue, RRep); break;
#pragma region IRep_to_IRep
				case 3:
				{
					RepType RRep = rValue.GetIRepType();
					if(LRep==RRep)
					{
						switch(LRep)
						{
							default:
								throw "Unsupported operation";
						}
					}
					else
					{
						switch(LRep)
						{
							default:
								throw "Unsupported operation";
						}
					}
				} break;
#pragma endregion IRep_to_IRep
#pragma region IRep_to_NormRep
				default:
				{
					RepType RRep = rValue.GetNormRepType();
    				switch(LRep)
    				{
    					case RepType::INum:{
    						switch(RRep){
    						}
    					} break;
    #pragma region AltDecVariantExclusive
    				#if defined(AltNum_EnableFractionals)
    					case RepType::INumByDiv:{
    						switch(RRep){
    						}
    					} break;
    				#endif
    				#if defined(AltNum_EnableMixedFractional)
    					case RepType::MixedI:{
    						switch(RRep){
    						}
    					} break;
    				#endif
    #pragma endregion AltDecVariantExclusive
    				#if defined(AltNum_EnableApproaching)
    					case RepType::ApproachingImaginaryBottom:{
    						switch(RRep){
    						}
    					} break;
    					#if !defined(AltNum_DisableApproachingTop)
    					case RepType::ApproachingImaginaryTop:{
    						switch(RRep){
    						}
    					} break;
    					#endif
    				#endif
    #pragma region AltDecVariantExclusive
    				#if defined(AltNum_EnableApproachingDivided)
    					case RepType::ApproachingImaginaryMidLeft:{
    						switch(RRep){
    						}
    					} break;
    					#if !defined(AltNum_DisableApproachingTop)
    					case RepType::ApproachingImaginaryMidRight:{
    						switch(RRep){
    						}
    					} break;
    					#endif
    				#endif
    #pragma endregion AltDecVariantExclusive
    #if defined(AltNum_EnableImaginaryInfinity)
    					case RepType::ImaginaryInfinity:{
    						switch(RRep){
    						}
    					} break;
    #endif
    				default:
    					throw "Unsupported operation"; break;
    				}
				} break;
#pragma region IRep_to_NormRep
		} break;
#endif
		default:
		{
			RepType LRep = GetNormRepType();
			switch(rValue.DecimalHalf.Flags)
			{
	#if defined(AltNum_EnablePiRep)
#pragma region NormRep_to_PiRep
				case 1:
				{
					RepType RRep = rValue.GetPiRepType();
					switch(LRep)
					{
						case RepType::NormalType:{
							switch(RRep){
							}
						} break;
#pragma region AltDecVariantExclusive
				#if defined(AltNum_EnableFractionals)
						case RepType::NumByDiv:{
							switch(RRep){
							}
						}; break;
				#endif
				#if defined(AltNum_EnablePowerOfRepresentation)
						case RepType::ToPowerOf:{
							switch(RRep){
							}
						}; break;
				#endif
				#if defined(AltNum_EnableMixedFractional)
						case RepType::MixedFrac:{
							switch(RRep){
							}
						}; break;
				#endif
#pragma endregion AltDecVariantExclusive
						case RepType::ApproachingBottomRep:{
							switch(RRep){
							}
						} break;
						case RepType::ApproachingTopRep:{
							switch(RRep){
							}
						} break;
#pragma region AltDecVariantExclusive
				#if defined(AltNum_EnableApproachingDivided)
						case RepType::ApproachingMidLeft:{
							switch(RRep){
							}
						} break;
					#if !defined(AltNum_DisableApproachingTop)
						case RepType::ApproachingMidRight:{
							switch(RRep){
							}
						} break;
					#endif
				#endif
#pragma endregion AltDecVariantExclusive
						case RepType::InfinityRep:{
							switch(RRep){
							}
						} break;
						default:
							throw "Unsupported operation";
					}
				} break;
#pragma endregion NormRep_to_PiRep
	#endif
	#if defined(AltNum_EnableERep)
#pragma region NormRep_to_ERep
				case 2:
				{
					RepType RRep = rValue.GetERepType();
					switch(LRep)
					{
						case RepType::NormalType:{
							switch(RRep){
							}
						} break;
#pragma region AltDecVariantExclusive
				#if defined(AltNum_EnableFractionals)
						case RepType::NumByDiv:{
							switch(RRep){
							}
						}; break;
				#endif
				#if defined(AltNum_EnablePowerOfRepresentation)
						case RepType::ToPowerOf:{
							switch(RRep){
							}
						}; break;
				#endif
				#if defined(AltNum_EnableMixedFractional)
						case RepType::MixedFrac:{
							switch(RRep){
							}
						}; break;
				#endif
#pragma endregion AltDecVariantExclusive
						case RepType::ApproachingBottomRep:{
							switch(RRep){
							}
						} break;
						case RepType::ApproachingTopRep:{
							switch(RRep){
							}
						} break;
#pragma region AltDecVariantExclusive
				#if defined(AltNum_EnableApproachingDivided)
						case RepType::ApproachingMidLeft:{
							switch(RRep){
							}
						} break;
					#if !defined(AltNum_DisableApproachingTop)
						case RepType::ApproachingMidRight:{
							switch(RRep){
							}
						} break;
					#endif
				#endif
#pragma endregion AltDecVariantExclusive
						case RepType::InfinityRep:{
							switch(RRep){
							}
						} break;
						default:
							throw "Unsupported operation";
					}
				} break;
#pragma endregion NormRep_to_ERep
	#endif
	#if defined(AltNum_EnableIRep)
#pragma endregion NormRep_to_IRep
				case 3:
				{
					RepType RRep = rValue.GetIRepType();
					switch(LRep)
					{
						case RepType::NormalType:{
							switch(RRep){
							}
						} break;
#pragma region AltDecVariantExclusive
				#if defined(AltNum_EnableFractionals)
						case RepType::NumByDiv:{
							switch(RRep){
							}
						}; break;
				#endif
				#if defined(AltNum_EnablePowerOfRepresentation)
						case RepType::ToPowerOf:{
							switch(RRep){
							}
						}; break;
				#endif
				#if defined(AltNum_EnableMixedFractional)
						case RepType::MixedFrac:{
							switch(RRep){
							}
						}; break;
				#endif
#pragma endregion AltDecVariantExclusive
						case RepType::ApproachingBottomRep:{
							switch(RRep){
							}
						} break;
						case RepType::ApproachingTopRep:{
							switch(RRep){
							}
						} break;
#pragma region AltDecVariantExclusive
				#if defined(AltNum_EnableApproachingDivided)
						case RepType::ApproachingMidLeft:{
							switch(RRep){
							}
						} break;
					#if !defined(AltNum_DisableApproachingTop)
						case RepType::ApproachingMidRight:{
							switch(RRep){
							}
						} break;
					#endif
				#endif
#pragma endregion AltDecVariantExclusive
						case RepType::InfinityRep:{
							switch(RRep){
							}
						} break;
						default:
							throw "Unsupported operation";
					}
				} break;
#pragma endregion NormRep_to_IRep
	#endif
#pragma region NormRep_to_NormRep
				default:
				{
					RepType RRep = rValue.GetNormRepType();
					if(LRep==RRep)
					{
						switch(LRep)
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
					}
					else
					{
						switch(LRep)
						{
							case RepType::NormalType:{
								switch(RRep){
								}
							} break;
	#pragma region AltDecVariantExclusive
					#if defined(AltNum_EnableFractionals)
							case RepType::NumByDiv:{
								switch(RRep){
								}
							}; break;
					#endif
					#if defined(AltNum_EnablePowerOfRepresentation)
							case RepType::ToPowerOf:{
								switch(RRep){
								}
							}; break;
					#endif
					#if defined(AltNum_EnableMixedFractional)
							case RepType::MixedFrac:{
								switch(RRep){
								}
							}; break;
					#endif
	#pragma endregion AltDecVariantExclusive
							case RepType::ApproachingBottomRep:{
								switch(RRep){
								}
							} break;
							case RepType::ApproachingTopRep:{
								switch(RRep){
								}
							} break;
	#pragma region AltDecVariantExclusive
					#if defined(AltNum_EnableApproachingDivided)
							case RepType::ApproachingMidLeft:{
								switch(RRep){
								}
							} break;
						#if !defined(AltNum_DisableApproachingTop)
							case RepType::ApproachingMidRight:{
								switch(RRep){
								}
							} break;
						#endif
					#endif
	#pragma endregion AltDecVariantExclusive
							case RepType::InfinityRep:{
								switch(RRep){
								}
							} break;
							default:
								throw "Unsupported operation";
						}
					}
				} break;
			}
		} break;
#pragma endregion NormRep_to_NormRep
	}
	return *this;
}
