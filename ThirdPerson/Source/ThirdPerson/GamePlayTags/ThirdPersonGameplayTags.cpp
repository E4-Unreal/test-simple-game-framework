#include "ThirdPersonGameplayTags.h"
#include "GameplayTagsManager.h"
#include "Engine/EngineTypes.h"

FThirdPersonGameplayTags FThirdPersonGameplayTags::GameplayTags;

void FThirdPersonGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& GameplayTagsManager = UGameplayTagsManager::Get();

	GameplayTags.AddAllTags(GameplayTagsManager);

	GameplayTagsManager.DoneAddingNativeTags();
}

void FThirdPersonGameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	
	// Equipment Slots
	AddTag(EquipmentSlot_Weapon_Main_Primary, "EquipmentSlot.Weapon.Main.Primary", "EquipmentSlot for Primary Weapon");
	AddTag(EquipmentSlot_Weapon_Main_Secondary, "EquipmentSlot.Weapon.Main.Secondary", "EquipmentSlot for Secondary Weapon");
	AddTag(EquipmentSlot_Weapon_SideArm, "EquipmentSlot.Weapon.SideArm", "EquipmentSlot for SideArm Weapon");
	AddTag(EquipmentSlot_Armor_Head, "EquipmentSlot.Armor.Head", "EquipmentSlot for Head Armor");
	AddTag(EquipmentSlot_Armor_Chest, "EquipmentSlot.Armor.Chest", "EquipmentSlot for Chest Armor");
	AddTag(EquipmentSlot_Armor_Leg, "EquipmentSlot.Armor.Leg", "EquipmentSlot for Leg Armor");
	AddTag(EquipmentSlot_Armor_Arm, "EquipmentSlot.Armor.Arm", "EquipmentSlot for Arm Armor");

	// Item Tags
	AddTag(Item_Equipment_Weapon_Melee_Knife, "Item.Equipment.Weapon.Melee.Knife", "Knife is type of Melee Weapon");
	AddTag(Item_Equipment_Weapon_Ranged_Gun_Rifle, "Item.Equipment.Weapon.Ranged.Gun.Rifle", "Rifle is type of Gun");
	AddTag(Item_Equipment_Weapon_Ranged_Gun_ShotGun, "Item.Equipment.Weapon.Ranged.Gun.ShotGun", "ShotGun is type of Gun");
	AddTag(Item_Equipment_Weapon_Ranged_Gun_Pistol, "Item.Equipment.Weapon.Ranged.Gun.Pistol", "Pistol is type of Gun");
	AddTag(Item_Equipment_Armor_Head, "Item.Equipment.Armor.Head", "Head Armor");
	AddTag(Item_Equipment_Armor_Chest, "Item.Equipment.Armor.Chest", "Chest Armor");
	AddTag(Item_Equipment_Armor_Leg, "Item.Equipment.Armor.Leg", "Leg Armor");
	AddTag(Item_Equipment_Armor_Arm, "Item.Equipment.Armor.Arm", "Arm Armor");
}

void FThirdPersonGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));
}