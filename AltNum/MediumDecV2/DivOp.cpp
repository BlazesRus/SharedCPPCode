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
					}
					else
					{
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
	#if defined(AltNum_EnableImaginaryNum)
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
					}
					else
					{
					}
				} break;
	#if defined(AltNum_EnableImaginaryNum)
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
#if defined(AltNum_EnableImaginaryNum)
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
							throw "Unsupported operation";
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
	#if defined(AltNum_EnableImaginaryNum)
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
							throw "Unsupported operation";
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
								throw "Unsupported operation";
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
								throw "Unsupported operation";
						}
					}
				} break;
			}
		} break;
		
	}
	return *this;
}
