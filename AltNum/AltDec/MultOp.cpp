#include "AltDecBase.hpp"
using AltDecBase = BlazesRusCode::AltDecBase;
using RepType = BlazesRusCode::RepType;

void CatchAllOp(const auto& rValue)
{
	RepType RRep = GetERepType();
	auto RValue = rValue.ConvertAsNormType(RRep);
	BasicUnsignedDivOp(RValue);
}

auto& AltDecBase::UnsignedMultOp(const auto& rValue)
{
	switch(DecimalHalf.Flags)
	{
#if defined(AltNum_EnablePiRep)
		case 1:
		{
			RepType LRep = GetPiRepType();
			switch(rValue.DecimalHalf.Flags)
			{
#pragma region PiRep_to_PiRep
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
#pragma endregion PiRep_to_PiRep
	#if defined(AltNum_EnableERep)
				case 2:
					CatchAllOp(rValue); break;
	#endif
	#if defined(AltNum_EnableImaginaryNum)
				case 3:
				{
					RepType RRep = GetIRepType();
				} break;
	#endif
#pragma region PiRep_to_NormRep
				default:
				{
					RepType RRep = GetNormRepType();
				} break;
#pragma endregion PiRep_to_NormRep
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
					CatchAllOp(rValue); break;
				} break;
	#endif
#pragma region ERep_to_ERep
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
#pragma region ERep_to_ERep
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
					CatchAllOp(rValue); break;
	#endif
	#if defined(AltNum_EnableERep)
				case 2:
					CatchAllOp(rValue); break;
	#endif
#pragma region IRep_to_IRep
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
#pragma endregion IRep_to_IRep
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
#pragma region NormRep_to_PiRep
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
	#pragma region AltDecVariantExclusive							
	#pragma endregion AltDecVariantExclusive
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
#pragma endregion NormRep_to_PiRep
	#endif
	#if defined(AltNum_EnableERep)
#pragma region NormRep_to_ERep
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
	#pragma region AltDecVariantExclusive							
	#pragma endregion AltDecVariantExclusive
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
#pragma endregion NormRep_to_ERep
	#endif
	#if defined(AltNum_EnableImaginaryNum)
#pragma endregion NormRep_to_IRep
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
	#pragma region AltDecVariantExclusive							
	#pragma endregion AltDecVariantExclusive
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
#pragma endregion NormRep_to_IRep
	#endif
#pragma region NormRep_to_NormRep
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
	#pragma region AltDecVariantExclusive							
	#pragma endregion AltDecVariantExclusive
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
	#pragma region AltDecVariantExclusive							
	#pragma endregion AltDecVariantExclusive
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
#pragma endregion NormRep_to_NormRep
	}
	return *this;
}
