
#pragma once

namespace TwoMinConstant
{
	
#pragma region Monster
	
	constexpr int32 MinMonsterLevel = 1;
	constexpr int32 MaxMonsterLevel = 3;
	
#pragma endregion
	
#pragma region Player
	
	const FName Player_Center = TEXT("spine_03");
	
	constexpr float ComputeBlockValue = 0.3f; 
	constexpr float StopThreshold = -0.866f;
	
	
	static const TArray<FIntPoint> SupportedResolutions =
	{
		FIntPoint(1280, 720),
		FIntPoint(1600, 900),
		FIntPoint(1920, 1080),
		FIntPoint(2560, 1440),
	};
	
#pragma endregion
}
