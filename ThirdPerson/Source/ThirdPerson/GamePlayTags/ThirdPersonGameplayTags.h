// Fill out your copyright notice in the Description page of Project Settings.
// Wiki for simpleFramework
// https://github.com/Eu4ng/simpleFramework/wiki

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

class UGameplayTagsManager;

/**
 * ThirdPersonPlayTags
 *
 *	Singleton containing native gameplay tags.
 */
struct FThirdPersonGameplayTags
{
public:
	static const FThirdPersonGameplayTags& Get() { return GameplayTags; }

	static void InitializeNativeTags();

	// Equipment Slot
	FGameplayTag EquipmentSlot;
	FGameplayTag EquipmentSlot_Active;
	FGameplayTag EquipmentSlot_Passive;
	// EquipmentSlot.Active
	FGameplayTag EquipmentSlot_Active_Main;
	FGameplayTag EquipmentSlot_Active_Main_Primary;
	FGameplayTag EquipmentSlot_Active_Main_Secondary;
	FGameplayTag EquipmentSlot_Active_Sub;
	FGameplayTag EquipmentSlot_Active_Sub_SideArm;
	FGameplayTag EquipmentSlot_Active_Sub_SideArm_Primary;
	FGameplayTag EquipmentSlot_Active_Sub_Throwing;
	FGameplayTag EquipmentSlot_Active_Sub_Throwing_Primary;
	FGameplayTag EquipmentSlot_Active_Sub_Throwing_Secondary;
	// EquipmentSlot.Passive
	FGameplayTag EquipmentSlot_Passive_Armor;
	FGameplayTag EquipmentSlot_Passive_Armor_Head;
	FGameplayTag EquipmentSlot_Passive_Armor_Chest;
	FGameplayTag EquipmentSlot_Passive_Armor_Leg;
	FGameplayTag EquipmentSlot_Passive_Armor_Arm;

	// Item
	FGameplayTag Item;
	FGameplayTag Item_Clutter;
	FGameplayTag Item_Consumable;
	FGameplayTag Item_Equipment;
	// Item.Equipment.Active
	FGameplayTag Item_Equipment_Active;
	FGameplayTag Item_Equipment_Active_Weapon;
	FGameplayTag Item_Equipment_Active_Weapon_Melee;
	FGameplayTag Item_Equipment_Active_Weapon_Melee_Knife;
	FGameplayTag Item_Equipment_Active_Weapon_Ranged;
	FGameplayTag Item_Equipment_Active_Weapon_Ranged_Gun;
	FGameplayTag Item_Equipment_Active_Weapon_Ranged_Gun_Rifle;
	FGameplayTag Item_Equipment_Active_Weapon_Ranged_Gun_ShotGun;
	FGameplayTag Item_Equipment_Active_Weapon_Ranged_Gun_Pistol;
	// Item.Equipment.Passive
	FGameplayTag Item_Equipment_Passive;
	FGameplayTag Item_Equipment_Passive_Armor;
	FGameplayTag Item_Equipment_Passive_Armor_Head;
	FGameplayTag Item_Equipment_Passive_Armor_Chest;
	FGameplayTag Item_Equipment_Passive_Armor_Leg;
	FGameplayTag Item_Equipment_Passive_Armor_Arm;

protected:
	//Registers all of the tags with the GameplayTags Manager
	void AddAllTags(UGameplayTagsManager& Manager);

	//Helper function used by AddAllTags to register a single tag with the GameplayTags Manager
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);

private:

	static FThirdPersonGameplayTags GameplayTags;
};