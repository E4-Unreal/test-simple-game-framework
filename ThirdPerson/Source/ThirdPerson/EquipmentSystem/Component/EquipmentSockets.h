#pragma once
#include "CoreMinimal.h"
#include "GameplayTagsManager.h"
#include "EquipmentSockets.generated.h"


UCLASS(Blueprintable, BlueprintType, Const, Meta = (DisplayName = "Equipment Sockets", ShortTooltip = "DataAsset for mapping Equipment Socket GameplayTags to Equipment Socket Names"))
class THIRDPERSON_API UEquipmentSockets : public UDataAsset
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USkeletalMesh* SkeletalMeshAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(Categories="EquipmentSocket"))
	TMap<FGameplayTag, FName> RegisteredSockets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(Categories="EquipmentSocket"))
	TMap<FGameplayTag, FName> UnregisteredSockets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UDataTable* SocketTags;

	// 데이터 테이블 애셋 변경 감지
	UPROPERTY()
	UDataTable* DataTableCache;
	
public:
	FORCEINLINE USkeletalMesh* GetSkeletalMeshAsset() const { return SkeletalMeshAsset; }
	FORCEINLINE TMap<FGameplayTag, FName> GetRegisteredSockets() const { return RegisteredSockets; }

protected:
	// RegisteredSockets의 요소들을 UnregisteredSockets로 이전
	void DeregisterAll();
	// SocketTags 목록들을 전부 RegisteredSockets에 추가
	void ApplySocketTags();
	// RegisteredSockets의 요소들 중 SocketTags 목록에 존재하지 않는 것들을 Deregister
	void CheckRegistered(TArray<FName>& EquipmentSocketTags);
	// SocketTags 추출 및 변환
	TArray<FName> GetSocketTags() const;

	void NotifyUnregisteredTag(FGameplayTag UnregisteredTag) const;
	void NotifyRegistered(FGameplayTag UnregisteredTag);
	static void Notify(const FText Text);
	
	virtual void PostLoad() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
};
