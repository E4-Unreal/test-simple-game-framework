#include "EquipmentSockets.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"

void UEquipmentSockets::DeregisterAll()
{
	// UnregisteredSockets의 요소들 중 매핑된 소켓 이름이 None인 경우 자동 삭제
	for(auto& Elem : UnregisteredSockets)
	{
		if(Elem.Value.IsNone())
		{
			UnregisteredSockets.Remove(Elem.Key);
		}
	}
	
	// RegisteredSockets의 요소들 중 매핑된 소켓 이름이 존재하는 것들만 UnregisteredSockets로 이동
	for(auto& Elem : RegisteredSockets)
	{
		if(!UnregisteredSockets.Contains(Elem.Key) && !Elem.Value.IsNone())
		{
			UnregisteredSockets.Add(Elem);
		}
	}

	// RegisteredSockets 비우기
	RegisteredSockets.Empty();
}

// SocketTags 목록들을 전부 RegisteredSockets에 추가
void UEquipmentSockets::ApplySocketTags()
{
	// SocketTags 유효성 검사
	if(!IsValid(SocketTags))
	{
		DeregisterAll();
		DataTableCache = nullptr;
		return;
	}
	
	// 데이터 테이블 애셋 변경 감지
	if(SocketTags != DataTableCache)
	{
		DeregisterAll();
		DataTableCache = SocketTags;
		ApplySocketTags();
		return;
	}

	TArray<FName> EquipmentSocketTags = GetSocketTags();
	// RegisteredSockets의 요소들 중 SocketTags 목록에 존재하지 않는 것들을 Deregister
	CheckRegistered(EquipmentSocketTags);
	
	// SocketTags 읽어들이기
	for(const FName& EquipmentSocketTagName : EquipmentSocketTags)
	{
		FGameplayTag EquipmentSocketTag = FGameplayTag::RequestGameplayTag(EquipmentSocketTagName);
		if(EquipmentSocketTag.IsValid())
		{
			if(RegisteredSockets.Contains(EquipmentSocketTag))
			{
				// UnregisteredSockets의 요소들 중 수정을 통해 SocketTags에 포함되는 것들을 RegisteredSockets로 이동
				if(RegisteredSockets.Find(EquipmentSocketTag)->IsNone() && UnregisteredSockets.Contains(EquipmentSocketTag))
				{
					NotifyRegistered(EquipmentSocketTag);
					*RegisteredSockets.Find(EquipmentSocketTag) = *UnregisteredSockets.Find(EquipmentSocketTag);
					UnregisteredSockets.Remove(EquipmentSocketTag);
				}
			}
			else
			{
				if(UnregisteredSockets.Contains(EquipmentSocketTag))
				{
					// UnregisteredSockets의 요소들 중 SocketTags에 포함되는 것들을 RegisteredSockets로 이동
					RegisteredSockets.Add(EquipmentSocketTag, *UnregisteredSockets.Find(EquipmentSocketTag));
					UnregisteredSockets.Remove(EquipmentSocketTag);
				}
				else
				{
					// SocketTags 목록들 중 RegisteredSockets에 추가되지 않은 것들을 마저 추가 
					RegisteredSockets.Add(EquipmentSocketTag, NAME_None);
				}
			}
		}
	}
}

void UEquipmentSockets::CheckRegistered(TArray<FName>& EquipmentSocketTags)
{
	for(auto& Elem : RegisteredSockets)
	{
		if(!EquipmentSocketTags.Contains(Elem.Key.GetTagName()))
		{
			// 에디터 알림
			NotifyUnregisteredTag(Elem.Key);
			// 값이 설정되지 않은 것들은 삭제
			if(!Elem.Value.IsNone())
			{
				// 중복이 발생하면 최신 것으로 덮어씌움
				if(UnregisteredSockets.Contains(Elem.Key))
				{
					UnregisteredSockets.Remove(Elem.Key);
				}
				UnregisteredSockets.Add(Elem);
			}
			RegisteredSockets.Remove(Elem.Key);

			// 이미 등록되어 있는 소켓들 제외
			EquipmentSocketTags.Remove(Elem.Key.GetTagName());
		}
	}
}

TArray<FName> UEquipmentSockets::GetSocketTags() const
{
	TArray<FGameplayTagTableRow*> Rows;
	SocketTags->GetAllRows("GetAllRows", Rows);

	TArray<FName> EquipmentSocketTags;

	for(FGameplayTagTableRow*& Row : Rows)
	{
		EquipmentSocketTags.Add(Row->Tag);
	}

	return EquipmentSocketTags;
}

void UEquipmentSockets::NotifyUnregisteredTag(FGameplayTag UnregisteredTag) const
{
	const FText NotificationText = FText::Format(NSLOCTEXT("Notifications", "TestNotifications", "{0}은(는) {1}의 목록에 존재하지 않습니다"), FText::FromString(UnregisteredTag.ToString()), FText::FromString(SocketTags->GetName()));
	Notify(NotificationText);
}

void UEquipmentSockets::NotifyRegistered(FGameplayTag UnregisteredTag)
{
	const FText NotificationText = FText::Format(NSLOCTEXT("Notifications", "TestNotifications", "{0}이(가) {1}에 등록되었습니다"), FText::FromName(*UnregisteredSockets.Find(UnregisteredTag)), FText::FromString(UnregisteredTag.ToString()));
	Notify(NotificationText);
}

void UEquipmentSockets::Notify(const FText Text)
{
	FNotificationInfo Info(Text);
	Info.bFireAndForget = true;
	Info.FadeInDuration = 0.5f;
	Info.FadeOutDuration = 1.0f;
	Info.ExpireDuration = 6.0f;
	Info.bUseLargeFont = true;
	
	FSlateNotificationManager::Get().AddNotification(Info);
}

void UEquipmentSockets::PostLoad()
{
	Super::PostLoad();
	ApplySocketTags();
}

void UEquipmentSockets::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	ApplySocketTags();
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
