#include "MediumDecV2Base.hpp"
using MediumDecV2Base = BlazesRusCode::MediumDecV2Base;
using RepType = BlazesRusCode::RepType;

auto& MediumDecV2Base::UnsignedDivOp(const auto& rValue)
{
	switch(DecimalHalf.Flags)
	{
#if defined(AltNum_EnablePiRep)
		case 1:
		{
			RepType LRep = GetPiRepType();
			switch(rValue.DecimalHalf.Flags)
			{
				case 1:
				{
					RepType RRep = GetPiRepType();
					if(LRep==RRep)
					{
                        switch(LRep)
                        {
                    		case RepType::PiNum:
                            {
                            } break;
                    	#if defined(AltNum_EnablePowerOfRepresentation)
                    		case RepType::PiPower:
                            {
                            } break;
                    	#endif
                    	#if defined(AltNum_EnableFractionals)
                    		case RepType::PiNumByDiv:
                            {
                            } break;
                    	#endif
                    	#if defined(AltNum_EnableApproaching)
                    		ApproachingBottomPi:
                            {
                            } break;
                    		#if !defined(AltNum_DisableApproachingTop)
                    		case RepType::ApproachingTopPi:
                            {
                            } break;
                    		#endif
                    	#endif
                    	#if defined(AltNum_EnableApproachingDivided)
                    		case RepType::ApproachingMidLeftPi:
                            {
                            } break;
                    		#if !defined(AltNum_DisableApproachingTop)
                    		case RepType::ApproachingMidRightPi:
                            {
                            } break;,
                    		#endif
						#endif
            			default:
            				throw "Unsupported operation"; break;
                        }
					}
					else
					{
                        switch(LRep)
                        {
                    		case RepType::PiNum:
                            {
                                RepType RRep = GetPiRepType();
                                switch(RRep)
                                {
                            	#if defined(AltNum_EnablePowerOfRepresentation)
                            		case RepType::PiPower:
                                    {
                                    } break;
                            	#endif
                            	#if defined(AltNum_EnableFractionals)
                            		case RepType::PiNumByDiv:
                                    {

                                    } break;
                            	#endif
                            	#if defined(AltNum_EnableApproaching)
                            		case RepType::ApproachingBottomPi:
                                    {
                                    } break;
                            		#if !defined(AltNum_DisableApproachingTop)
                            		case RepType::ApproachingTopPi:
                                    {
                                    } break;
                            		#endif
                            	#endif
                            	#if defined(AltNum_EnableApproachingDivided)
                            		case RepType::ApproachingMidLeftPi:
                                    {
                                    } break;
                            		#if !defined(AltNum_DisableApproachingTop)
                            		case RepType::ApproachingMidRightPi:
                                    {
                                    } break;,
                            		#endif
                            	#endif
                    			default:
                    				throw "Unsupported operation"; break;
                                }
                            } break;
                    	#if defined(AltNum_EnablePowerOfRepresentation)
                    		case RepType::PiPower:
                            {
                                RepType RRep = GetPiRepType();
                                switch(RRep)
                                {
                            		case RepType::PiNum:
                                    {
                                    } break;
                            	#if defined(AltNum_EnableFractionals)
                            		case RepType::PiNumByDiv:
                                    {

                                    } break;
                            	#endif
                            	#if defined(AltNum_EnableApproaching)
                            		case RepType::ApproachingBottomPi:
                                    {
                                    } break;
                            		#if !defined(AltNum_DisableApproachingTop)
                            		case RepType::ApproachingTopPi:
                                    {
                                    } break;
                            		#endif
                            	#endif
                            	#if defined(AltNum_EnableApproachingDivided)
                            		case RepType::ApproachingMidLeftPi:
                                    {
                                    } break;
                            		#if !defined(AltNum_DisableApproachingTop)
                            		case RepType::ApproachingMidRightPi:
                                    {
                                    } break;,
                            		#endif
                            	#endif
                    			default:
                    				throw "Unsupported operation"; break;
                                }
                            } break;
                    	#endif
                    	#if defined(AltNum_EnableFractionals)
                    		case RepType::PiNumByDiv:
                            {
                                RepType RRep = GetPiRepType();
                                switch(RRep)
                                {
                            		case RepType::PiNum:
                                    {
                                    } break;
                            	#if defined(AltNum_EnablePowerOfRepresentation)
                            		case RepType::PiPower:
                                    {
                                    } break;
                            	#endif
                            	#if defined(AltNum_EnableApproaching)
                            		case RepType::ApproachingBottomPi:
                                    {
                                    } break;
                            		#if !defined(AltNum_DisableApproachingTop)
                            		case RepType::ApproachingTopPi:
                                    {
                                    } break;
                            		#endif
                            	#endif
                            	#if defined(AltNum_EnableApproachingDivided)
                            		case RepType::ApproachingMidLeftPi:
                                    {
                                    } break;
                            		#if !defined(AltNum_DisableApproachingTop)
                            		case RepType::ApproachingMidRightPi:
                                    {
                                    } break;,
                            		#endif
                            	#endif
                    			default:
                    				throw "Unsupported operation"; break;
                                }
                            } break;
                    	#endif
                    	#if defined(AltNum_EnableApproaching)
                    		case RepType::ApproachingBottomPi:
                            {
                                RepType RRep = GetPiRepType();
                                switch(RRep)
                                {
                            		case RepType::PiNum:
                                    {
                                    } break;
                            	#if defined(AltNum_EnablePowerOfRepresentation)
                            		case RepType::PiPower:
                                    {
                                    } break;
                            	#endif
                            	#if defined(AltNum_EnableFractionals)
                            		case RepType::PiNumByDiv:
                                    {

                                    } break;
                            	#endif
                            	#if defined(AltNum_EnableApproaching)
                            		#if !defined(AltNum_DisableApproachingTop)
                            		case RepType::ApproachingTopPi:
                                    {
                                    } break;
                            		#endif
                            	#endif
                            	#if defined(AltNum_EnableApproachingDivided)
                            		case RepType::ApproachingMidLeftPi:
                                    {
                                    } break;
                            		#if !defined(AltNum_DisableApproachingTop)
                            		case RepType::ApproachingMidRightPi:
                                    {
                                    } break;,
                            		#endif
                            	#endif
                    			default:
                    				throw "Unsupported operation"; break;
                                }
                            } break;
                    		#endif
                    		#if !defined(AltNum_DisableApproachingTop)
                    		case RepType::ApproachingTopPi:
                            {
                                RepType RRep = GetPiRepType();
                                switch(RRep)
                                {
                            		case RepType::PiNum:
                                    {
                                    } break;
                            	#if defined(AltNum_EnablePowerOfRepresentation)
                            		case RepType::PiPower:
                                    {
                                    } break;
                            	#endif
                            	#if defined(AltNum_EnableFractionals)
                            		case RepType::PiNumByDiv:
                                    {

                                    } break;
                            	#endif
                            		case RepType::ApproachingBottomPi:
                                    {
                                    } break;
                            		#if !defined(AltNum_DisableApproachingTop)
                            		case RepType::ApproachingTopPi:
                                    {
                                    } break;
                            		#endif
                            	#if defined(AltNum_EnableApproachingDivided)
                            		case RepType::ApproachingMidLeftPi:
                                    {
                                    } break;
                            		#if !defined(AltNum_DisableApproachingTop)
                            		case RepType::ApproachingMidRightPi:
                                    {
                                    } break;,
                            		#endif
                            	#endif
                    			default:
                    				throw "Unsupported operation"; break;
                                }
                            } break;
                    		#endif
                    	#endif
            			default:
            				throw "Unsupported operation"; break;
                        }
					}
				} break;
	#if defined(AltNum_EnableERep)
				case 2:
				{
					RepType RRep = GetERepType();
					ConvertToNormType(LRep);
					auto RValue = rValue.ConvertAsNormType(LRep);
					BasicUnsignedDivOp(RValue);
				} break;
	#endif
	#if defined(AltNum_EnableIRep)
				case 3:
				{
					RepType RRep = GetIRepType();
                    switch(RRep)
                    {
            			default:
            				throw "Unsupported operation"; break;
                    }
				} break;
	#endif
				default:
				{
					RepType RRep = GetNormRepType();
                    switch(RRep)
                    {
            			default:
            				throw "Unsupported operation"; break;
                    }
				} break;
		} break;
#endif
#if defined(AltNum_EnableERep)
		case 2:
		{
			RepType LRep = GetERepType();
			switch(rValue.DecimalHalf.Flags)
			{
	#if defined(AltNum_EnablePiRep)
				case 1:
				{
					RepType RRep = GetPiRepType();
					ConvertToNormType(LRep);
					auto RValue = rValue.ConvertAsNormType(LRep);
					BasicUnsignedDivOp(RValue);
				} break;
	#endif
				case 2:
				{
					RepType RRep = GetERepType();
					if(LRep==RRep)
					{
                		case RepType::ENum:
                        {
                        } break;
                	#if defined(AltNum_EnableFractionals)
                		case RepType::ENumByDiv = 10:
                        {
                        } break;
                	#endif
                	#if defined(AltNum_EnablePowerOfRepresentation)
                		case RepType::EPower = 18:
                        {
                        } break;
                	#endif
                	#if defined(AltNum_EnableApproachingE)
                		//(Enum Bits:7,2)
                		//equal to IntValue.0..01 e
                		case RepType::ApproachingBottomE:
                        {
                        } break;
                		#if !defined(AltNum_DisableApproachingTop)
                		case RepType::ApproachingTopE:
                        {
                        } break;
                		#endif
                	#endif
            			default:
            				throw "Unsupported operation"; break;
					}
					else
					{
                        switch(LRep)
                        {
                    		case RepType::ENum:
                            {
                            } break;
                    	#if defined(AltNum_EnableFractionals)
                    		case RepType::ENumByDiv = 10:
                            {
                            } break;
                    	#endif
                    	#if defined(AltNum_EnablePowerOfRepresentation)
                    		case RepType::EPower = 18:
                            {
                            } break;
                    	#endif
                    	#if defined(AltNum_EnableApproachingE)
                    		case RepType::ApproachingBottomE:
                            {
                            } break;
                    		#if !defined(AltNum_DisableApproachingTop)
                    		case RepType::ApproachingTopE:
                            {
                            } break;
                    		#endif
                    	#endif
							default:
								throw "Unsupported operation";
                        }
					}
				} break;
	#if defined(AltNum_EnableIRep)
				case 3:
				{
					RepType RRep = GetIRepType();
				} break;
	#endif
				default:
				{
					RepType RRep = GetNormRepType();
				} break;
		} break;
#endif
#if defined(AltNum_EnableIRep)
		case 3:
		{
			RepType LRep = GetIRepType();
			switch(rValue.DecimalHalf.Flags)
			{
	#if defined(AltNum_EnablePiRep)
				case 1:
				{
					RepType RRep = GetPiRepType();
					ConvertToNormType(LRep);
					auto RValue = rValue.ConvertAsNormType(LRep);
					BasicUnsignedDivOp(RValue);
				} break;
	#endif
	#if defined(AltNum_EnableERep)
				case 2:
				{
					RepType RRep = GetERepType();
					ConvertToNormType(LRep);
					auto RValue = rValue.ConvertAsNormType(LRep);
					BasicUnsignedDivOp(RValue);
				} break;
	#endif
				case 3:
				{
					RepType RRep = GetIRepType();
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
				default:
				{
					RepType RRep = GetNormRepType();
				} break;
		} break;
#endif
		default:
		{
			RepType LRep = GetNormRepType();
			switch(rValue.DecimalHalf.Flags)
			{
	#if defined(AltNum_EnablePiRep)
				case 1:
				{
					RepType RRep = GetPiRepType();
					switch(LRep)
					{
						case RepType::NormalType:
						{
							switch(RRep)
							{
							
							}
						} break;
						case RepType::ApproachingBottomRep:
						{
							switch(RRep)
							{
							
							}
						} break;
						case RepType::ApproachingTopRep:
						{
							switch(RRep)
							{
							
							}
						} break;
						case RepType::InfinityRep:
						{
							switch(RRep)
							{
							
							}
						} break;
						default:
							throw "Unsupported operation"; break;
					}
				} break;
	#endif
	#if defined(AltNum_EnableERep)
				case 2:
				{
					RepType RRep = GetERepType();
					switch(LRep)
					{
						case RepType::NormalType:
						{
							switch(RRep)
							{
							
							}
						} break;
						case RepType::ApproachingBottomRep:
						{
							switch(RRep)
							{
							
							}
						} break;
						case RepType::ApproachingTopRep:
						{
							switch(RRep)
							{
							
							}
						} break;
						case RepType::InfinityRep:
						{
							switch(RRep)
							{
							
							}
						} break;
						default:
							throw "Unsupported operation";
					}
				} break;
	#endif
	#if defined(AltNum_EnableIRep)
				case 3:
				{
					RepType RRep = GetIRepType();
					switch(LRep)
					{
						case RepType::NormalType:
						{
							switch(RRep)
							{
							
							}
						} break;
						case RepType::ApproachingBottomRep:
						{
							switch(RRep)
							{
							
							}
						} break;
						case RepType::ApproachingTopRep:
						{
							switch(RRep)
							{
							
							}
						} break;
						case RepType::InfinityRep:
						{
							switch(RRep)
							{
							
							}
						} break;
						default:
							throw "Unsupported operation"; break;
					}
				} break;
	#endif
				default:
				{
					RepType RRep = GetNormRepType();
					if(LRep==RRep)
					{
						switch(LRep)
						{
							case RepType::NormalType:
							{
							}; break;
							case RepType::ApproachingBottomRep:
							{

							} break;
							case RepType::ApproachingTopRep:
							{

							} break;
							case RepType::InfinityRep:
							{

							} break;
							default:
								throw "Unsupported operation"; break;
						}
					}
					else
					{
						switch(LRep)
						{
							case RepType::NormalType:
							{
								switch(RRep)
								{
								
								}
							} break;
							case RepType::ApproachingBottomRep:
							{
								switch(RRep)
								{
								
								}
							} break;
							case RepType::ApproachingTopRep:
							{
								switch(RRep)
								{
								
								}
							} break;
							case RepType::InfinityRep:
							{
								switch(RRep)
								{
								
								}
							} break;
							default:
								throw "Unsupported operation"; break;
						}
					}
				} break;
			}
		} break;
		
	}
	return *this;
}
