#ifndef AZEROTHCORE_INDIVIDUALPROGRESSION_H
#define AZEROTHCORE_INDIVIDUALPROGRESSION_H

#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "ScriptedCreature.h"
#include "GameObjectAI.h"
#include "MapMgr.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Group.h"
#include "Pet.h"
#include "DBCEnums.h"
#include "QuestDef.h"
#include "GameObject.h"
#include "AreaDefines.h"
#include "IWorld.h"
#include <regex>

typedef std::unordered_map<uint32, uint32> questXpMapType;

enum ProgressionBossIDs
{
    PB_RAGNAROS = 11502,
    PB_ONYXIA = 10184,
    PB_NEFARIAN = 11583,
    PB_CTHUN = 15727,
    PB_KELTHUZAD = 15990,
    PB_KELTHUZAD_40 = 351019,
    PB_MALCHEZAAR = 15690,
    PB_KAELTHAS = 19622,
    PB_ILLIDAN = 22917,
    PB_ZULJIN = 23863,
    PB_KILJAEDEN = 25315,
    PB_YOGGSARON = 33288,
    PB_ANUBARAK = 34564,
    PB_LICH_KING = 36597,
    PB_HALION = 39863
};

enum ProgressionAchievementIDs
{
    PA_RAGNAROS = 686,
    PA_ONYXIA = 684,
    PA_NEFARIAN = 685,
    PA_CTHUN = 687,
    PA_MALCHEZAAR = 690,
    PA_KAELTHAS = 696,
    PA_ILLIDAN = 697,
    PA_ZULJIN = 691,
    PA_KILJAEDEN = 698,
    PA_KELTHUZAD_10 = 574,
    PA_KELTHUZAD_25 = 575,
    PA_YOGGSARON_10 = 2892,
    PA_YOGGSARON_25 = 2893,
    PA_ANUBARAK_10 = 3918,
    PA_ANUBARAK_25 = 3812,
    PA_LICH_KING_10 = 4530,
    PA_LICH_KING_25 = 4597,
    PA_HALION_10 = 4817,
    PA_HALION_25 = 4815
};

enum BuffSpells
{
    HP_AURA_SPELL        = 89501,
    ABSORB_SPELL         = 89505,
    RANGED_HASTE_SPELL   = 89507,
    SPELL_RUNE_TAP       = 48982,
    SPELL_LIFE_STEAL     = 20004,
    IPP_PHASE            = 89509,
    IPP_PHASE_AQ         = 89511
};

enum ProgressionQuestIDs
{
    MIGHT_OF_KALIMDOR     = 8742,
    BANG_A_GONG           = 108743,
    CHAOS_AND_DESTRUCTION = 108744,
    QUEST_MORROWGRAIN     = 3803,
    QUEST_TROLL_NECKLACE  = 2881,
    QUEST_DEADWOOD        = 6221,
    QUEST_WINTERFALL      = 6241
};

enum ProgressionMaps
{
    MAP_BLACKWING_LAIR        = 469,
    MAP_ZUL_GURUB             = 309,
    MAP_AQ_20                 = 509,
    MAP_AQ_40                 = 531,
    MAP_OUTLANDS              = 530,
    MAP_NAXXRAMAS             = 533,
    MAP_ZUL_AMAN              = 568,
    MAP_MAGISTER_TERRACE      = 585,
    MAP_SUNWELL               = 580,
    MAP_NORTHREND             = 571,
    MAP_HALLS_OF_LIGHTNING    = 602,
    MAP_HALLS_OF_STONE        = 599,
    MAP_ULDUAR                = 603,
    MAP_TRIAL_OF_THE_CRUSADER = 649,
    MAP_TRIAL_OF_THE_CHAMPION = 650,
    MAP_HALLS_OF_REFLECTION   = 668,
    MAP_FORGE_OF_SOULS        = 632,
    MAP_PIT_OF_SARON          = 658,
    MAP_ICC                   = 631,
    MAP_RAGEFIRE_CHASM        = 389,
    MAP_RUBY_SANCTUM          = 724
};

enum ProgressionZones
{
    ZONE_AZUREMYST       = 3524,
    ZONE_BLOODMYST       = 3525,
    ZONE_GHOSTLANDS      = 3433,
    ZONE_EVERSONG        = 3430,
    ZONE_EXODAR          = 3557,
    ZONE_SILVERMOON      = 3487,
    ZONE_QUELDANAS       = 4080,
    ZONE_VEILED_SEA      = 3479
};

enum ProgressionAreas
{
	AREA_AZSHARA                         = 16,
    AREA_THE_DARK_PORTAL                 = 72,
    AREA_DREADMAUL_ROCK                  = 249,
    AREA_RUINS_OF_THAURISSAN             = 250,
    AREA_BLACKROCK_MOUNTAIN              = 254,
    AREA_SERADANE                        = 356,
    AREA_BOUGH_SHADOW                    = 438,
    AREA_GROVE_OF_THE_ANCIENTS           = 448,
    AREA_WILDBEND_RIVER                  = 454,
    AREA_TWILIGHT_GROVE                  = 856,
    AREA_GADGETZAN                       = 976,
    AREA_DREAM_BOUGH                     = 1111,
    AREA_JADEMIR_LAKE                    = 1112,
    AREA_THE_SHATTERED_STRAND            = 1228,
    AREA_SOUTHRIDGE_BEACH                = 1231,
    AREA_HALDARR_ENCAMPMENT              = 1236,
    AREA_DREADMAUL_HOLD                  = 1437,
    AREA_DREADMAUL_POST                  = 1439,
    AREA_SERPENTS_COIL                   = 1440,
    AREA_VALLEY_OF_HEROES                = 1611,
	AREA_BROKEN_PILLAR                   = 1938,
    AREA_ABYSSAL_SANDS                   = 1939,
    AREA_TWILIGHT_VALE                   = 2077,
    AREA_FROSTSABER_ROCK                 = 2241,
    AREA_TIMBERMAW_POST                  = 2243,
    AREA_MAZTHORIL                       = 2245,
    AREA_ICE_THISTLE_HILLS               = 2247,
    AREA_THE_MARRIS_STEAD                = 2260,
    AREA_LIGHTS_HOPE                     = 2268,
    AREA_DRACO_DAR                       = 2421,
    AREA_RISE_OF_THE_DEFILER             = 2517,
    AREA_PESTILENT_SCAR                  = 2622,
    AREA_HIVE_ASHI                       = 2742,
    AREA_HIVE_ZORA                       = 2743,
    AREA_HIVE_REGAL                      = 2744,
    AREA_ARGENT_SUNREAVER_PAVILION       = 4666,
    AREA_ARGENT_SILVER_COVENANT_PAVILION = 4667,
    AREA_ARGENT_PAVILION                 = 4674,
    AREA_DREADMIST_PEAK                  = 384,
    AREA_FELFIRE_HILL                    = 434,
    AREA_DEMON_FALL_CANYON               = 435,
    AREA_DEMON_FALL_RIDGE                = 436,
    AREA_STONEWATCH                      = 70,
    AREA_STONEWATCH_TOWER                = 999,
    AREA_STONEWATCH_KEEP                 = 2099,
    AREA_VUL_GOL_OGRE_MOUND              = 93
};

enum ProgressionSettings
{
    SETTING_PROGRESSION_STATE = 0
};

enum ProgressionState : uint8
{
    PROGRESSION_START           = 0,
    PROGRESSION_MOLTEN_CORE     = 1,
    PROGRESSION_ONYXIA          = 2,
    PROGRESSION_BLACKWING_LAIR  = 3,  // AQ War effort, AQ quest line
    PROGRESSION_PRE_AQ          = 4,  // AQ outdoors war 
    PROGRESSION_AQ_WAR          = 5,  // AQ gates open, raid available
    PROGRESSION_AQ              = 6,  
    PROGRESSION_NAXX40          = 7,
    PROGRESSION_TBC_TIER_1      = 8,  // Karazhan, Gruul's Lair, Magtheridon's Lair
    PROGRESSION_TBC_TIER_2      = 9,  // Serpentshrine Cavern, Tempest Keep
    PROGRESSION_TBC_TIER_3      = 10, // Hyjal Summit and Black Temple
    PROGRESSION_TBC_TIER_4      = 11, // Zul'Aman
    PROGRESSION_TBC_TIER_5      = 12, // Sunwell Plateau
    PROGRESSION_WOTLK_TIER_1    = 13, // WotLK Naxx, EoE, OS
    PROGRESSION_WOTLK_TIER_2    = 14, // Ulduar
    PROGRESSION_WOTLK_TIER_3    = 15, // TotC
    PROGRESSION_WOTLK_TIER_4    = 16, // ICC
    PROGRESSION_WOTLK_TIER_5    = 17  // Ruby Sanctum
    PROGRESSION_CATA_TIER_1     = 18, // Baradin Hold, Blackwing Descent
    PROGRESSION_CATA_TIER_2     = 19, // The Bastion of Twilight, Throne of the Four Winds
    PROGRESSION_CATA_TIER_3     = 20, // Firelands
    PROGRESSION_CATA_TIER_4     = 21, // Dragonsoul
};

enum RandomDungeonIds : uint16
{
    // Values from LFGDungeons.dbc
    RDF_CLASSIC                       = 258,
    RDF_THE_BURNING_CRUSADE           = 259,
    RDF_THE_BURNING_CRUSADE_HEROIC    = 260,
    RDF_WRATH_OF_THE_LICH_KING        = 261,
    RDF_WRATH_OF_THE_LICH_KING_HEROIC = 262
};

enum PvPTitles : uint32
{
    // Alliance
    TITLE_PRIVATE                  = 1,
    TITLE_CORPORAL                 = 2,
    TITLE_SERGEANT                 = 3,
    TITLE_MASTER_SERGEANT          = 4,
    TITLE_SERGEANT_MAJOR           = 5,
    TITLE_KNIGHT                   = 6,
    TITLE_KNIGHT_LIEUTENANT        = 7,
    TITLE_KNIGHT_CAPTAIN           = 8,
    TITLE_KNIGHT_CHAMPION          = 9,
    TITLE_LIEUTENANT_COMMANDER     = 10,
    TITLE_COMMANDER                = 11,
    TITLE_MARSHAL                  = 12,
    TITLE_FIELD_MARSHAL            = 13,
    TITLE_GRAND_MARSHAL            = 14,

    // Horde
    TITLE_SCOUT                    = 15,
    TITLE_GRUNT                    = 16,
    TITLE_SERGEANT_H               = 17,
    TITLE_SENIOR_SERGEANT          = 18,
    TITLE_FIRST_SERGEANT           = 19,
    TITLE_STONE_GUARD              = 20,
    TITLE_BLOOD_GUARD              = 21,
    TITLE_LEGIONNAIRE              = 22,
    TITLE_CENTURION                = 23,
    TITLE_CHAMPION                 = 24,
    TITLE_LIEUTENANT_GENERAL       = 25,
    TITLE_GENERAL                  = 26,
    TITLE_WARLORD                  = 27,
    TITLE_HIGH_WARLORD             = 28
};

enum ProgressionSoundEntries : uint32
{
    MUSIC_RAGEFIRE_CHASM_HEROIC_LIGHT = 70001
};

class IndividualProgression
{
public:
    static IndividualProgression* instance();

    std::map<uint32, uint8> customProgressionMap;
    questXpMapType questXpMap;
    float vanillaPowerAdjustment, vanillaHealthAdjustment, tbcPowerAdjustment, tbcHealthAdjustment, vanillaHealingAdjustment, tbcHealingAdjustment, previousGearTuning;
    bool enabled, questXpFix, hunterPetLevelFix, enforceGroupRules, fishingFix, simpleConfigOverride, questMoneyAtLevelCap, repeatableVanillaQuestsXp, disableDefaultProgression, earlyDungeonSet2, requireNaxxStrath, pvpGearRequirements, excludeAccounts, progressionSetterAlwaysVisible;
    int progressionLimit, startingProgression, tbcRacesProgressionLevel, tbcRacesStartingProgression, cataRacesProgressionLevel, cataRacesStartingProgression, deathKnightProgressionLevel, deathKnightStartingProgression;
    std::string excludedAccountsRegex;

    ProgressionState GetCurrentProgressionState(Player* player) const;
    bool hasPassedProgression(Player* player, ProgressionState state) const;
    static bool isBeforeProgression(Player* player, ProgressionState state) ;
    void UpdateProgressionState(Player* player, ProgressionState newState) const;
    static void ForceUpdateProgressionState(Player* player, ProgressionState newState);
    void CheckAdjustments(Player* player) const;
    void ApplyGearStatsTuning(Player* player, float& computedAdjustment, ItemTemplate const* item) const;
    void ComputeGearTuning(Player* player, float& computedAdjustment, ItemTemplate const* item) const;
    void AdjustVanillaStats(Player* player) const;
    void AdjustTBCStats(Player* player) const;
    void AdjustWotLKStats(Player* player) const;
    bool hasCustomProgressionValue(uint32 creatureEntry);
    void checkKillProgression(Player* player, Creature* killed);
    void checkAchievementProgression(Player* player, AchievementEntry const* achievement);
    void setProgressionSpell(Player* player, ProgressionState newState);
    void removeAllProgressionSpells(Player* player);
    static void LoadCustomProgressionEntries(const std::string& customProgressionString);
    static void AdjustStats(Player* player, float computedAdjustment, float computedHealingAdjustment);
    static float ComputeVanillaAdjustment(uint8 playerLevel, float configAdjustmentValue);
    static uint8 GetAccountProgression(uint32 accountId);
};

#define sIndividualProgression IndividualProgression::instance()

#endif //AZEROTHCORE_INDIVIDUALPROGRESSION_H
