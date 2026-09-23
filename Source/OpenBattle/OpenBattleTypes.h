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

    bool IsValid() const
    {
        return AllianceId.IsValid();
    }
};

USTRUCT(BlueprintType)
struct FOBIdCardRecord
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FGuid CardId;

    UPROPERTY(BlueprintReadOnly)
    int32 OriginFloor = 1;

    UPROPERTY(BlueprintReadOnly)
    int32 VictimPlayerId = -1;

    UPROPERTY(BlueprintReadOnly)
    EOBRole VictimRole = EOBRole::Rebel;

    bool IsValid() const
    {
        return CardId.IsValid();
    }
};
