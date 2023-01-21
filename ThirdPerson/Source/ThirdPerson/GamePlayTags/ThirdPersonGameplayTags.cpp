// Fill out your copyright notice in the Description page of Project Settings.
// Wiki for simpleFramework
// https://github.com/Eu4ng/simpleFramework/wiki

#include "ThirdPersonGameplayTags.h"
#include "GameplayTagsManager.h"
#include "Engine/EngineTypes.h"

// Convert VariableName to TagName
// ex) FGameplayTag Item_Equipment > "Item.Equipment"
#define ADD_TAG(VariableName, TagComment) AddTag(VariableName, TCHAR_TO_ANSI(*FString(# VariableName).Replace(TEXT("_"), TEXT("."))), TagComment)

FThirdPersonGameplayTags FThirdPersonGameplayTags::GameplayTags;

void FThirdPersonGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& GameplayTagsManager = UGameplayTagsManager::Get();

	GameplayTags.AddAllTags(GameplayTagsManager);

	GameplayTagsManager.DoneAddingNativeTags();
}

void FThirdPersonGameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	// Equipment Slot
	ADD_TAG(EquipmentSlot, "EquipmentSlot for Equipment");
	ADD_TAG(EquipmentSlot_Active, "EquipmentSlot for Active Equipment");
	ADD_TAG(EquipmentSlot_Passive, "EquipmentSlot for Passive Equipment");
	// EquipmentSlot.Active
	ADD_TAG(EquipmentSlot_Active_Main, "EquipmentSlot for Main Active Equipment");
	ADD_TAG(EquipmentSlot_Active_Main_Primary, "Primary EquipmentSlot for Main Active Equipment");
	ADD_TAG(EquipmentSlot_Active_Main_Secondary, "Secondary EquipmentSlot for Main Active Equipment");
	ADD_TAG(EquipmentSlot_Active_Sub, "EquipmentSlot for Sub Active Equipment");
	ADD_TAG(EquipmentSlot_Active_Sub_SideArm, "EquipmentSlot for SideArm");
	ADD_TAG(EquipmentSlot_Active_Sub_Throwing01, "First EquipmentSlot for Throwing");
	ADD_TAG(EquipmentSlot_Active_Sub_Throwing02, "Second EquipmentSlot for Throwing");
	// EquipmentSlot.Passive
	ADD_TAG(EquipmentSlot_Passive_Armor, "EquipmentSlot for Armor");
	ADD_TAG(EquipmentSlot_Passive_Armor_Head, "EquipmentSlot for Head Armor");
	ADD_TAG(EquipmentSlot_Passive_Armor_Chest, "EquipmentSlot for Chest Armor");
	ADD_TAG(EquipmentSlot_Passive_Armor_Leg, "EquipmentSlot for Leg Armor");
	ADD_TAG(EquipmentSlot_Passive_Armor_Arm, "EquipmentSlot for Arm Armor");

	// Item
	ADD_TAG(Item, "Item");
	ADD_TAG(Item_Clutter, "Clutter Item");
	ADD_TAG(Item_Consumable, "Consumable Item");
	ADD_TAG(Item_Equipment, "Equipment Item");
	// Item.Equipment.Active
	ADD_TAG(Item_Equipment_Active, "Active Equipment");
	ADD_TAG(Item_Equipment_Active_Weapon, "Weapon is Active Equipment ");
	ADD_TAG(Item_Equipment_Active_Weapon_Melee, "Melee Weapon");
	ADD_TAG(Item_Equipment_Active_Weapon_Melee_Knife, "Knife is Melee Weapon");
	ADD_TAG(Item_Equipment_Active_Weapon_Ranged, "Ranged Weapon");
	ADD_TAG(Item_Equipment_Active_Weapon_Ranged_Gun, "Gun is Ranged Weapon");
	ADD_TAG(Item_Equipment_Active_Weapon_Ranged_Gun_Rifle, "Rifle is Gun Weapon");
	ADD_TAG(Item_Equipment_Active_Weapon_Ranged_Gun_ShotGun, "ShotGun is Gun Weapon");
	ADD_TAG(Item_Equipment_Active_Weapon_Ranged_Gun_Pistol, "Pistol is Gun Weapon");
	// Item.Equipment.Passive
	ADD_TAG(Item_Equipment_Passive, "Passive Equipment");
	ADD_TAG(Item_Equipment_Passive_Armor, "Armor is Passive Equipment");
	ADD_TAG(Item_Equipment_Passive_Armor_Head, "Head Armor");
	ADD_TAG(Item_Equipment_Passive_Armor_Chest, "Chest Armor");
	ADD_TAG(Item_Equipment_Passive_Armor_Leg, "Leg Armor");
	ADD_TAG(Item_Equipment_Passive_Armor_Arm, "Arm Armor");
}

// Todo OutTag의 변수명을 분석하여 TagName을 자동으로 추가하는 기능 필요
void FThirdPersonGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));
}
