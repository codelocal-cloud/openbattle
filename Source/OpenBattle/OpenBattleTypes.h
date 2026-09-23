#pragma once

#include "CoreMinimal.h"
#include "OpenBattleTypes.generated.h"

UENUM(BlueprintType)
enum class EOBRole : uint8
{
    Rebel,
    Security,
    Auxiliary
};

UENUM(BlueprintType)
enum class EOBFloorState : uint8
{
    Safe,
    Unstable,
    Critical,
    Lost
};

USTRUCT(BlueprintType)
struct FOBAllianceIdentity
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FGuid AllianceId;

    UPROPERTY(BlueprintReadOnly)
    int32 VisualIndex = INDEX_NONE;
};
