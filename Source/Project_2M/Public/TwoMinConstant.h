
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
	
#pragma endregion
}
