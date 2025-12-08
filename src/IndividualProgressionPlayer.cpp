#include "GameTime.h"
#include "IndividualProgression.h"
#include "shared_mutex"
#include "Playerbots.h"
#include "naxxramas_40.h"

class IndividualPlayerProgression : public PlayerScript
{

private:
    uint32 lastUpdateTime = 0;
    std::unordered_map<ObjectGuid, uint32> lastCheckTime;
    std::shared_mutex lastCheckTimeMutex;
    static bool IsTBCRaceStartingZone(uint32 mapid, float x, float y, float z)
    {
        Map const *map = sMapMgr->FindMap(mapid, 0);
        uint32 zoneId = map->GetZoneId(0, x, y, z);
        return (zoneId == ZONE_AZUREMYST || zoneId == ZONE_BLOODMYST || zoneId == ZONE_GHOSTLANDS ||
                zoneId == ZONE_EVERSONG || zoneId == ZONE_EXODAR || zoneId == ZONE_SILVERMOON ||
                zoneId == ZONE_VEILED_SEA);
    }

public:
    IndividualPlayerProgression() : PlayerScript("IndividualProgression") { }

    void OnPlayerLogin(Player* player) override
    {
        if (!sIndividualProgression->enabled)
        {
            return;
        }

        if (sIndividualProgression->deathKnightStartingProgression && player->getClass() == CLASS_DEATH_KNIGHT && (int32)player->GetLevel() == sConfigMgr->GetOption<int32>("StartHeroicPlayerLevel", 55) && !sIndividualProgression->hasPassedProgression(player, static_cast<ProgressionState>(sIndividualProgression->deathKnightStartingProgression)))
        {
            if (player->getClass() == CLASS_DEATH_KNIGHT && sIndividualProgression->deathKnightStartingProgression && !sIndividualProgression->hasPassedProgression(player, static_cast<ProgressionState>(sIndividualProgression->deathKnightStartingProgression)))
            {
                sIndividualProgression->UpdateProgressionState(player, static_cast<ProgressionState>(sIndividualProgression->deathKnightStartingProgression));
            }
            if (sIndividualProgression->startingProgression && !sIndividualProgression->hasPassedProgression(player, static_cast<ProgressionState>(sIndividualProgression->startingProgression)))
            {
                sIndividualProgression->UpdateProgressionState(player, static_cast<ProgressionState>(sIndividualProgression->startingProgression));
            }
            sIndividualProgression->UpdateProgressionQuests(player);
        }
        else if (sIndividualProgression->tbcRacesStartingProgression && (player->getRace() == RACE_BLOODELF || player->getRace() == RACE_DRAENEI) && (int32)player->GetLevel() == sConfigMgr->GetOption<int32>("StartPlayerLevel", 1) && !sIndividualProgression->hasPassedProgression(player, static_cast<ProgressionState>(sIndividualProgression->tbcRacesStartingProgression)))
        {
            sIndividualProgression->UpdateProgressionState(player, static_cast<ProgressionState>(sIndividualProgression->tbcRacesStartingProgression));
        }
        else if (sIndividualProgression->cataRacesStartingProgression && (player->getRace() == RACE_GOBLIN || player->getRace() == RACE_WORGEN) && (int32)player->GetLevel() == sConfigMgr->GetOption<int32>("StartPlayerLevel", 1) && !sIndividualProgression->hasPassedProgression(player, static_cast<ProgressionState>(sIndividualProgression->cataRacesStartingProgression)))
        {
            sIndividualProgression->UpdateProgressionState(player, static_cast<ProgressionState>(sIndividualProgression->cataRacesStartingProgression));
        }
        else if (!sIndividualProgression->hasPassedProgression(player, static_cast<ProgressionState>(sIndividualProgression->startingProgression)))
        {
            sIndividualProgression->AwardEarnedVanillaPvpTitles(player);
            sIndividualProgression->CleanUpVanillaPvpTitles(player);
        }
    }

    void OnPlayerAfterUpdate(Player* player, uint32 /*diff*/) override
    {
        if (!sIndividualProgression->enabled)
            return;

        if (!player || player->IsDuringRemoveFromWorld() || !player->IsInWorld())
            return;

        if (!player->GetSession()->IsBot())
            return;

        uint32 now = GameTime::GetGameTimeMS().count();
        ObjectGuid guid = player->GetGUID();

        {
            std::shared_lock<std::shared_mutex> readLock(lastCheckTimeMutex);

            auto it = lastCheckTime.find(guid);
            if (it != lastCheckTime.end() && it->second + 5000 > now)
                return;
        }
        {
            std::unique_lock<std::shared_mutex> writeLock(lastCheckTimeMutex);
            lastCheckTime[guid] = now;
        }

        Player* master = player->GetGroup() && GET_PLAYERBOT_AI(player) ? GET_PLAYERBOT_AI(player)->GetMaster() : nullptr;
        if (master && master != player && master->IsInWorld())
        {
            ProgressionState masterState = sIndividualProgression->GetCurrentProgressionState(master);
            ProgressionState myState = sIndividualProgression->GetCurrentProgressionState(player);

            // Set individualprogression phase
            if (myState != masterState)
            {
                sIndividualProgression->ForceUpdateProgressionState(player, masterState);
            }

            // Also set bot phasemask here
            if (player->GetPhaseMask() != master->GetPhaseMask())
            {
                player->SetPhaseMask(master->GetPhaseMask(), true);
            }

            return;
        }

        if (!player->GetMapId() || !player->GetZoneId())
            return;

        ProgressionState progressionState;
        std::vector<uint32> validAuras;
        uint8 level = player->GetLevel();
        ContentLevels mapEntryExpansion = CONTENT_1_60;
        mapEntryExpansion = GetContentLevelsForMapAndZone(player->GetMapId(), player->GetZoneId());
        if (level >= 81 || mapEntryExpansion == CONTENT_81_90)
        {
            progressionState = PROGRESSION_CATA_TIER_4;
            validAuras = { 98654, 98655, 98656, 98657 };
        }
        else if ((level <= 80 && level >= 70) || mapEntryExpansion == CONTENT_71_80)
        {
            progressionState = PROGRESSION_WOTLK_TIER_4;
            validAuras = { 98649, 98650, 98651, 98652, 98653 };
        }
        else if ((level <= 69 && level >= 60) || mapEntryExpansion == CONTENT_61_70)
        {
            progressionState = PROGRESSION_TBC_TIER_4;
            validAuras = { 98644, 98645, 98646, 98647, 98648 };
        }
        else
        {
            progressionState = PROGRESSION_AQ;
            validAuras = { 98636, 98637, 98638, 98639, 98640, 98641, 98642, 98643 };
        }

        bool hasValid = false;
        for (uint32 auraId : validAuras)
        {
            if (player->HasAura(auraId))
            {
                hasValid = true;
                break;
            }
        }

        if (!hasValid)
        {
            sIndividualProgression->ForceUpdateProgressionState(player, progressionState);
        }
    }

    void OnPlayerSetMaxLevel(Player* player, uint32& maxPlayerLevel) override
    {
        if (!sIndividualProgression->enabled)
        {
            return;
        }

        if (!sIndividualProgression->hasPassedProgression(player, PROGRESSION_PRE_TBC))
        {
            maxPlayerLevel = IP_LEVEL_VANILLA;
        }
        else if (!sIndividualProgression->hasPassedProgression(player, PROGRESSION_TBC_TIER_5))
        {
            maxPlayerLevel = IP_LEVEL_TBC;
        }
        else if (!sIndividualProgression->hasPassedProgression(player, PROGRESSION_WOTLK_TIER_5))
        {
            maxPlayerLevel = IP_LEVEL_WOTLK;
        }
        else
        {
            maxPlayerLevel = IP_LEVEL_CATA;
        }
    }

    bool OnPlayerShouldBeRewardedWithMoneyInsteadOfExp(Player* player) override
    {
        if (!sIndividualProgression->questMoneyAtLevelCap)
        {
            return false;
        }

        // Player is still in Vanilla content - give money at 60 level cap
        return ((!sIndividualProgression->hasPassedProgression(player, PROGRESSION_PRE_TBC) && player->GetLevel() == IP_LEVEL_VANILLA) ||
                // Player is in TBC content - give money at 70 level cap
                (!sIndividualProgression->hasPassedProgression(player, PROGRESSION_TBC_TIER_5) && player->GetLevel() == IP_LEVEL_TBC) ||
                // Player is in WotLK content - give money at 80 level cap
                (!sIndividualProgression->hasPassedProgression(player, PROGRESSION_WOTLK_TIER_5) && player->GetLevel() == IP_LEVEL_WOTLK));
    }

    // Returns the responsible player for progression scaling.
    // Currently only player itself is relevant for max health, but this allows future pet support.
    inline Player* GetProgressionPlayerForHealth(Player* player)
    {
        return player;
    }

    // Returns the health scaling multiplier based on progression state
    inline float GetHealthMultiplier(Player* player)
    {
        if (!player || !sIndividualProgression->enabled)
            return 1.0f;

        if (!sIndividualProgression->hasPassedProgression(player, PROGRESSION_PRE_TBC))
            return sIndividualProgression->vanillaHealthAdjustment;

        if (!sIndividualProgression->hasPassedProgression(player, PROGRESSION_TBC_TIER_5))
            return sIndividualProgression->tbcHealthAdjustment;

        return 1.0f;
    }

    void OnPlayerAfterUpdateMaxHealth(Player* player, float& value) override
    {
        if (!sIndividualProgression->enabled || !player)
            return;

        // Never apply progression scaling inside battlegrounds or arenas
        if (player->GetMap()->IsBattlegroundOrArena())
            return;

        Player* progressionPlayer = GetProgressionPlayerForHealth(player);
        if (!progressionPlayer)
            return;

        float mult = GetHealthMultiplier(progressionPlayer);
        if (mult != 1.0f)
            value *= mult;
    }


    void OnPlayerQuestComputeXP(Player* player, Quest const* quest, uint32& xpValue) override
    {
        if (!sIndividualProgression->enabled)
            return;

        if (sIndividualProgression->questXpFix && sIndividualProgression->questXpMap.count(quest->GetQuestId()))
        {
            uint32 vanillaXpValue = sIndividualProgression->questXpMap[quest->GetQuestId()];
            if (player)
            {
                uint32 originalXpValue = quest->XPValue(quest->GetQuestLevel() == -1 ? player->GetLevel() : quest->GetQuestLevel());
                xpValue *= vanillaXpValue * 1.0 / originalXpValue;
                xpValue = player->CalculateModulesXpExtras(xpValue);
            }
        }
        else if (!sIndividualProgression->questXpFix && player)
        {
            xpValue = player->CalculateModulesXpExtras(xpValue);
        }
    }

    void OnPlayerGiveXP(Player* player, uint32& amount, Unit* /*victim*/, uint8 xpSource) override
    {
        if (!sIndividualProgression->enabled)
        {
            return;
        }
        // Player is still in Vanilla content - do not give XP past level 60
        if (!sIndividualProgression->hasPassedProgression(player, PROGRESSION_PRE_TBC) && player->GetLevel() >= IP_LEVEL_VANILLA)
        {
            // Still award XP to pets - they won't be able to pass the player's level
            Pet* pet = player->GetPet();
            if (pet && xpSource == XPSOURCE_KILL)
                pet->GivePetXP(player->GetGroup() ? amount / 2 : amount);
            amount = 0;
        }
            // Player is in TBC content - do not give XP past level 70
        else if (!sIndividualProgression->hasPassedProgression(player, PROGRESSION_TBC_TIER_5) && player->GetLevel() >= IP_LEVEL_TBC)
        {
            // Still award XP to pets - they won't be able to pass the player's level
            Pet* pet = player->GetPet();
            if (pet && xpSource == XPSOURCE_KILL)
                pet->GivePetXP(player->GetGroup() ? amount / 2 : amount);
            amount = 0;
        }
            // Player is in WotLK content - do not give XP past level 80
        else if (!sIndividualProgression->hasPassedProgression(player, PROGRESSION_CATA_TIER_1) && player->GetLevel() >= IP_LEVEL_WOTLK)
        {
            // Still award XP to pets - they won't be able to pass the player's level
            Pet* pet = player->GetPet();
            if (pet && xpSource == XPSOURCE_KILL)
                pet->GivePetXP(player->GetGroup() ? amount / 2 : amount);
            amount = 0;
        }
    }

    static bool isAttuned(Player* player)
    {
        if ((player->GetQuestStatus(NAXX40_ATTUNEMENT_1) == QUEST_STATUS_REWARDED) ||
            (player->GetQuestStatus(NAXX40_ATTUNEMENT_2) == QUEST_STATUS_REWARDED) ||
            (player->GetQuestStatus(NAXX40_ATTUNEMENT_3) == QUEST_STATUS_REWARDED))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool OnPlayerBeforeTeleport(Player* player, uint32 mapid, float x, float y, float z, float /*orientation*/, uint32 /*options*/, Unit* /*target*/) override
    {
        if (!player)
            return false;

        if (!sIndividualProgression->enabled || player->IsGameMaster() || player->GetSession()->IsBot())
        {
            return true;
        }
        if (mapid == MAP_ONYXIAS_LAIR)
        {
            if (!sIndividualProgression->hasPassedProgression(player, PROGRESSION_MOLTEN_CORE))
                return false;

            if (!player->HasItemCount(ITEM_DRAKEFIRE_AMULET) && !sIndividualProgression->hasPassedProgression(player, PROGRESSION_TBC_TIER_5))
                return false;
        } 
        if (mapid == MAP_BLACKWING_LAIR && !sIndividualProgression->hasPassedProgression(player, PROGRESSION_ONYXIA))
        {
            return false;
        }
        if (mapid == MAP_ONYXIAS_LAIR) // needed to prevent summoning invalid characters from inside the instance
        {
			if (!sIndividualProgression->hasPassedProgression(player, PROGRESSION_TBC_TIER_5) && !player->HasItemCount(ITEM_DRAKEFIRE_AMULET)) // Vanilla
            {
                return false;
            }
			else if (sIndividualProgression->hasPassedProgression(player, PROGRESSION_TBC_TIER_5) && player->GetLevel() != IP_LEVEL_WOTLK) // WotLK
            {
                return false;
            }
        }
        if (mapid == MAP_ZUL_GURUB)
        {
            ProgressionState REQUIRED_ZG_PROGRESSION = static_cast<ProgressionState>(sIndividualProgression->RequiredZulGurubProgression);

            if (!sIndividualProgression->hasPassedProgression(player, REQUIRED_ZG_PROGRESSION))
            {
                return false;
            }
        }
        if (mapid == MAP_AHN_QIRAJ_TEMPLE && !sIndividualProgression->hasPassedProgression(player, PROGRESSION_PRE_AQ))
        {
            return false;
        }
        if (mapid == MAP_RUINS_OF_AHN_QIRAJ && !sIndividualProgression->hasPassedProgression(player, PROGRESSION_PRE_AQ))
        {
            return false;
        }
        if (!sIndividualProgression->hasPassedProgression(player, PROGRESSION_TBC_TIER_4) && player->GetMap()->GetZoneId(player->GetPhaseMask(), player->GetPositionX(), player->GetPositionY(), player->GetPositionZ()) == ZONE_QUELDANAS)
        {
            return false;
        }
        if (mapid == MAP_ZUL_AMAN && !sIndividualProgression->hasPassedProgression(player, PROGRESSION_TBC_TIER_3))
        {
            return false;
        }
        if (mapid == MAP_MAGISTERS_TERRACE && !sIndividualProgression->hasPassedProgression(player, PROGRESSION_TBC_TIER_4))
        {
            return false;
        }
        if (mapid == MAP_THE_SUNWELL && !sIndividualProgression->hasPassedProgression(player, PROGRESSION_TBC_TIER_4))
        {
            return false;
        }
        if (mapid == MAP_NORTHREND && !sIndividualProgression->hasPassedProgression(player, PROGRESSION_TBC_TIER_5))
        {
            return false;
        }
        if (mapid == MAP_ULDUAR && !sIndividualProgression->hasPassedProgression(player, PROGRESSION_WOTLK_TIER_1))
        {
            return false;
        }
        if (mapid == MAP_HALLS_OF_LIGHTNING && !sIndividualProgression->hasPassedProgression(player, PROGRESSION_WOTLK_TIER_1))
        {
            return false;
        }
        if (mapid == MAP_HALLS_OF_STONE && !sIndividualProgression->hasPassedProgression(player, PROGRESSION_WOTLK_TIER_1))
        {
            return false;
        }
        if ((mapid == MAP_TRIAL_OF_THE_CHAMPION || mapid == MAP_TRIAL_OF_THE_CRUSADER) && !sIndividualProgression->hasPassedProgression(player, PROGRESSION_WOTLK_TIER_2)){
            return false;
        }
        if ((mapid == MAP_THE_FORGE_OF_SOULS) && !sIndividualProgression->hasPassedProgression(player, PROGRESSION_WOTLK_TIER_3))
        {
            return false;
        }
        if ((mapid == MAP_PIT_OF_SARON) && !sIndividualProgression->hasPassedProgression(player, PROGRESSION_WOTLK_TIER_3))
        {
            return false;
        }
        if ((mapid == MAP_HALLS_OF_REFLECTION) && !sIndividualProgression->hasPassedProgression(player, PROGRESSION_WOTLK_TIER_3))
        {
            return false;
        }
        if ((mapid == MAP_ICECROWN_CITADEL) && !sIndividualProgression->hasPassedProgression(player, PROGRESSION_WOTLK_TIER_3))
        {
            return false;
        }
        if (mapid == MAP_THE_RUBY_SANCTUM && !sIndividualProgression->hasPassedProgression(player, PROGRESSION_WOTLK_TIER_4))
        {
            return false;
        }
        if (mapid == MAP_NAXXRAMAS && !sIndividualProgression->hasPassedProgression(player, PROGRESSION_AQ))
        {
            return false;
        }

        InstanceTemplate const* instanceTemplate = sObjectMgr->GetInstanceTemplate(mapid);
        if (instanceTemplate)
        {
            if (instanceTemplate->Parent == MAP_OUTLAND && !sIndividualProgression->hasPassedProgression(player, PROGRESSION_PRE_TBC))
            {
                return false;
            }
        }
        return true;
    }

    void OnPlayerAchievementComplete(Player* player, AchievementEntry const* achievement) override
    {
        if (!sIndividualProgression->enabled || !player || !achievement)
            return;

        switch (achievement->ID)
        {
            case HALION_KILL: // 4815
                player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, ACHIEVEMENT_SPELL_EVENT_RUBY_SANCTUM);
                sIndividualProgression->UpdateProgressionState(player, PROGRESSION_WOTLK_TIER_5);
                break;

            case LICH_KING_KILL: // 4597
                player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, ACHIEVEMENT_SPELL_EVENT_ICC);
                sIndividualProgression->UpdateProgressionState(player, PROGRESSION_WOTLK_TIER_4);
                break;

            case ANUB_ARAK_KILL: // 3916
                player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, ACHIEVEMENT_SPELL_EVENT_TOTC);
                sIndividualProgression->UpdateProgressionState(player, PROGRESSION_WOTLK_TIER_3);
                break;

            case KEL_THUZAD_KILL: // 575
                player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, ACHIEVEMENT_SPELL_EVENT_NAXXRAMAS);
                sIndividualProgression->UpdateProgressionState(player, PROGRESSION_WOTLK_TIER_1);
                break;

            case KIL_JAEDEN_KILL: // 698
                player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, ACHIEVEMENT_SPELL_EVENT_SUNWELL);
                sIndividualProgression->UpdateProgressionState(player, PROGRESSION_TBC_TIER_5);
                break;

            case ILLIDAN_KILL: // 697
                player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, ACHIEVEMENT_SPELL_EVENT_BLACK_TEMPLE);
                sIndividualProgression->UpdateProgressionState(player, PROGRESSION_TBC_TIER_3);
                break;

            case ZUL_JIN_KILL: // 691
                player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, ACHIEVEMENT_SPELL_EVENT_ZUL_AMAN);
                sIndividualProgression->UpdateProgressionState(player, PROGRESSION_TBC_TIER_4);
                break;

            case KAEL_THAS_KILL: // 696
                player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, ACHIEVEMENT_SPELL_EVENT_TEMPEST_KEEP);
                sIndividualProgression->UpdateProgressionState(player, PROGRESSION_TBC_TIER_2);
                break;

            case MALCHEZAAR_KILL: // 690
                player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, ACHIEVEMENT_SPELL_EVENT_KARAZHAN);
                sIndividualProgression->UpdateProgressionState(player, PROGRESSION_TBC_TIER_1);
                break;

            case KEL_THUZAD_40_KILL: // 5004
                player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, ACHIEVEMENT_SPELL_EVENT_NAXX40);
                sIndividualProgression->UpdateProgressionState(player, PROGRESSION_NAXX40);
                break;

            case C_THUN_KILL: // 687
                player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, ACHIEVEMENT_SPELL_EVENT_AQ);
                sIndividualProgression->UpdateProgressionState(player, PROGRESSION_AQ);
                break;

            case NEFARIAN_KILL: // 685
                player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, ACHIEVEMENT_SPELL_EVENT_BLACKWING_LAIR);
                sIndividualProgression->UpdateProgressionState(player, PROGRESSION_BLACKWING_LAIR);
                break;

            case ONYXIAS_KILL: // 684
                player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, ACHIEVEMENT_SPELL_EVENT_ONYXIA);
                sIndividualProgression->UpdateProgressionState(player, PROGRESSION_ONYXIA);
                break;

            case RAGNAROS_KILL: // 686
                player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, ACHIEVEMENT_SPELL_EVENT_MOLTEN_CORE);
                sIndividualProgression->UpdateProgressionState(player, PROGRESSION_MOLTEN_CORE);
                break;

            default:
                break;
        }
    }

    void OnPlayerCompleteQuest(Player* player, Quest const* quest) override
    {
        if (!sIndividualProgression->enabled)
        {
            return;
        }
        switch (quest->GetQuestId())
        {
            case BANG_A_GONG:
                if (!sIndividualProgression->disableDefaultProgression)
                {
                    player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, ACHIEVEMENT_SPELL_EVENT_PRE_AQ);
                    sIndividualProgression->UpdateProgressionState(player, PROGRESSION_PRE_AQ);
                    sIndividualProgression->UpdateProgressionQuests(player);
                }
                break;
            case SIMPLY_BANG_A_GONG:
                if (!sIndividualProgression->disableDefaultProgression)
                {
                    player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, ACHIEVEMENT_SPELL_EVENT_PRE_AQ);
                    sIndividualProgression->UpdateProgressionState(player, PROGRESSION_PRE_AQ);
                    sIndividualProgression->UpdateProgressionQuests(player);
                }
                break;
            case CHAOS_AND_DESTRUCTION:
                if (!sIndividualProgression->disableDefaultProgression)
                {
                    player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, ACHIEVEMENT_SPELL_EVENT_AQ_WAR);
                    sIndividualProgression->UpdateProgressionState(player, PROGRESSION_AQ_WAR);
                    sIndividualProgression->UpdateProgressionQuests(player);
                }
                break;
            case INTO_THE_BREACH:
                if (!sIndividualProgression->disableDefaultProgression)
                {
                    player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, ACHIEVEMENT_SPELL_EVENT_PRE_TBC);
                    sIndividualProgression->UpdateProgressionState(player, PROGRESSION_PRE_TBC);
                    sIndividualProgression->UpdateProgressionQuests(player);
                }
                break;
            case QUEST_MORROWGRAIN:
            case QUEST_TROLL_NECKLACE:
            case QUEST_DEADWOOD:
            case QUEST_WINTERFALL:
                if (sIndividualProgression->repeatableVanillaQuestsXp)
                {
                    // Reset the quest status so the player can take it and receive rewards again
                    player->RemoveRewardedQuest(quest->GetQuestId());
                }
                break;
        }
    }

    bool OnPlayerCanGroupInvite(Player* player, std::string& membername) override
    {
        if (!sIndividualProgression->enabled || !sIndividualProgression->enforceGroupRules || player->GetSession()->IsBot())
        {
            return true;
        }
        Player* otherPlayer = ObjectAccessor::FindPlayerByName(membername, false);
        uint8 currentState = player->GetPlayerSetting("mod-individual-progression", SETTING_PROGRESSION_STATE).value;
        uint8 otherPlayerState = otherPlayer->GetPlayerSetting("mod-individual-progression", SETTING_PROGRESSION_STATE).value;
        return (currentState == otherPlayerState);
    }

    bool OnPlayerCanGroupAccept(Player* player, Group* group) override
    {
        if (!sIndividualProgression->enabled || !sIndividualProgression->enforceGroupRules || player->GetSession()->IsBot())
        {
            return true;
        }
        Player* groupLeader = ObjectAccessor::FindPlayerByLowGUID(group->GetLeaderGUID().GetCounter());
        uint8 currentState = player->GetPlayerSetting("mod-individual-progression", SETTING_PROGRESSION_STATE).value;
        uint8 otherPlayerState = groupLeader->GetPlayerSetting("mod-individual-progression", SETTING_PROGRESSION_STATE).value;
        return (currentState == otherPlayerState);
    }

    void OnPlayerCreatureKill(Player* killer, Creature* killed) override
    {
        switch (killed->GetEntry())
        {
            case RHAHK_ZOR:
                killer->RemoveAura(IPP_PHASE);
                killer->RemoveAura(IPP_PHASE_II);
                killer->RemoveAura(IPP_PHASE_III);
                killer->CastSpell(killer, IPP_PHASE, false);
                break;
            case SNEED:
                killer->RemoveAura(IPP_PHASE);
                killer->RemoveAura(IPP_PHASE_II);
                killer->RemoveAura(IPP_PHASE_III);
	            killer->CastSpell(killer, IPP_PHASE, false);
                killer->CastSpell(killer, IPP_PHASE_II, false);
                break;
            case GILNID:
                killer->RemoveAura(IPP_PHASE);
                killer->RemoveAura(IPP_PHASE_II);
                killer->RemoveAura(IPP_PHASE_III);
	            killer->CastSpell(killer, IPP_PHASE, false);
                killer->CastSpell(killer, IPP_PHASE_II, false);
                killer->CastSpell(killer, IPP_PHASE_III, false);
                break;
        }

        if (killed->GetCreatureTemplate()->rank > CREATURE_ELITE_NORMAL)
        {
            sIndividualProgression->checkKillProgression(killer, killed);
            Group* group = killer->GetGroup();
            if (!group)
            {
                return;
            }
            for (GroupReference* itr = group->GetFirstMember(); itr != nullptr; itr = itr->next())
            {
                Player* member = itr->GetSource();
                if (!member)
                    continue;

                if (killer->IsAtLootRewardDistance(member))
                    sIndividualProgression->checkKillProgression(member, killed);
            }
        }
    }

    bool OnPlayerUpdateFishingSkill(Player* player, int32 /*skill*/, int32 /*zone_skill*/, int32 chance, int32 roll) override
    {
        if (!sIndividualProgression->enabled || !sIndividualProgression->fishingFix)
            return true;
        if (chance < roll)
            return false;
        return true;
    }

    void OnPlayerUpdateArea(Player* player, uint32 /*oldArea*/, uint32 newArea) override
    {
        sIndividualProgression->checkIPPhasing(player, newArea);

        // Boxhead Custom
        switch (newArea) {
            case AREA_DREADMIST_PEAK:
            case AREA_FELFIRE_HILL:
            case AREA_DEMON_FALL_CANYON:
            case AREA_DEMON_FALL_RIDGE:
            case AREA_STONEWATCH:
            case AREA_STONEWATCH_TOWER:
            case AREA_STONEWATCH_KEEP:
            case AREA_VUL_GOL_OGRE_MOUND:
                if (sIndividualProgression->hasPassedProgression(player, PROGRESSION_WOTLK_TIER_4))
                {
                    player->PlayDirectMusic(MUSIC_RAGEFIRE_CHASM_HEROIC_LIGHT, player);
                    player->GetMap()->SetZoneWeather(player->GetZoneId(), WEATHER_STATE_HEAVY_RAIN, 0.7f);
                }
                break;
            case AREA_ICECROWN_CITADEL:
            case AREA_ICECROWN_CITADEL_OUTSIDE:
                if (sIndividualProgression->hasPassedProgression(player, PROGRESSION_WOTLK_TIER_3))
                {
                    if (!player->GetPlayerSetting("FOTLK-MOVIE", 0).value)
                    {
                        player->SendMovieStart(MOVIE_FOTLK);
                        player->UpdatePlayerSetting("FOTLK-MOVIE", 0, true);
                    }
                }
        }
    }

    // Boxhead Custom
    void OnPlayerUpdateZone(Player* player, uint32 newZone, uint32 newArea) override
    {
        // TBC-Movie Trigger with phase PROGRESSION_NAXX40
        if (!player->GetPlayerSetting("TBC-MOVIE", 0).value && sIndividualProgression->hasPassedProgression(player, PROGRESSION_NAXX40) && (sIndividualProgression->isBeforeProgression(player, PROGRESSION_PRE_TBC)))
        {
            player->SendMovieStart(MOVIE_TBC);
            player->UpdatePlayerSetting("TBC-MOVIE", 0, 1);
        }

        // WotLK-Movie Trigger with phase PROGRESSION_TBC_TIER_5
        if (!player->GetPlayerSetting("WOTLK-MOVIE", 0).value && sIndividualProgression->hasPassedProgression(player, PROGRESSION_TBC_TIER_5) && (sIndividualProgression->isBeforeProgression(player, PROGRESSION_WOTLK_TIER_1)))
        {
            player->SendMovieStart(MOVIE_WOTLK);
            player->UpdatePlayerSetting("WOTLK-MOVIE", 0, 1);
        }
    }
};

class IndividualPlayerProgression_AccountScript: public AccountScript
{
public:
    IndividualPlayerProgression_AccountScript() : AccountScript("IndividualProgression_AccountScript") { }

    bool CanAccountCreateCharacter(uint32 accountId, uint8 charRace, uint8 charClass) override
    {
        if ((!sIndividualProgression->enabled) ||
            (charRace != RACE_DRAENEI && charRace != RACE_BLOODELF && charRace != RACE_GOBLIN && charRace != RACE_WORGEN && charClass != CLASS_DEATH_KNIGHT) ||
            (!sIndividualProgression->tbcRacesProgressionLevel && !sIndividualProgression->deathKnightProgressionLevel))
        {
            return true;
        }
        uint8 highestProgression = sIndividualProgression->GetAccountProgression(accountId);
        if (charRace == RACE_DRAENEI || charRace == RACE_BLOODELF)
        {
            if (sIndividualProgression->tbcRacesProgressionLevel)
            {
                if (highestProgression < sIndividualProgression->tbcRacesProgressionLevel)
                {
                    return false;
                }
            }
        }
        if (charRace == RACE_GOBLIN || charRace == RACE_WORGEN)
        {
            if (sIndividualProgression->cataRacesProgressionLevel)
            {
                if (highestProgression < sIndividualProgression->cataRacesProgressionLevel)
                {
                    return false;
                }
            }
        }
        if (charClass == CLASS_DEATH_KNIGHT && sIndividualProgression->deathKnightProgressionLevel)
        {
            if (highestProgression < sIndividualProgression->deathKnightProgressionLevel)
            {
                return false;
            }
        }
        return true;
    }
};

namespace
{
    // Resolve the responsible player for progression scaling:
    // - direct player
    // - player's pet
    // - charmed/controlled units owned by a player
    inline Player* GetProgressionPlayer(Unit* src)
    {
        if (!src)
            return nullptr;

        if (Player* p = src->ToPlayer())
            return p;

        if (Unit* owner = src->GetOwner())
            if (Player* p = owner->ToPlayer())
                return p;

        if (Unit* charmer = src->GetCharmer())
            if (Player* p = charmer->ToPlayer())
                return p;

        return nullptr;
    }

    // Healing multiplier based on progression state
    inline float GetHealingMultiplier(Player* player)
    {
        if (!player || !sIndividualProgression->enabled)
            return 1.0f;

        if (!sIndividualProgression->hasPassedProgression(player, PROGRESSION_PRE_TBC))
            return sIndividualProgression->vanillaHealingAdjustment;

        if (!sIndividualProgression->hasPassedProgression(player, PROGRESSION_TBC_TIER_5))
            return sIndividualProgression->tbcHealingAdjustment;

        return 1.0f;
    }

    // Damage multiplier based on progression state
    inline float GetDamageMultiplier(Player* player)
    {
        if (!player || !sIndividualProgression->enabled)
            return 1.0f;

        if (!sIndividualProgression->hasPassedProgression(player, PROGRESSION_PRE_TBC))
            return sIndividualProgression->vanillaPowerAdjustment;

        if (!sIndividualProgression->hasPassedProgression(player, PROGRESSION_TBC_TIER_5))
            return sIndividualProgression->tbcPowerAdjustment;

        return 1.0f;
    }
}

class IndividualPlayerProgression_UnitScript : public UnitScript
{
public:
    IndividualPlayerProgression_UnitScript() : UnitScript("IndividualPlayerProgression_UnitScript") { }

    void ModifyHealReceived(Unit* /*target*/, Unit* healer, uint32& heal, SpellInfo const* spellInfo) override
    {
        // Basic validation
        if (!sIndividualProgression->enabled || !healer || !heal || !spellInfo)
            return;

        // Skip potions, bandages, scripted / special heals
        if (spellInfo->HasAttribute(SPELL_ATTR0_NO_IMMUNITIES) || spellInfo->Mechanic == MECHANIC_BANDAGE)
            return;

        // Skip percentage-based heals (Heal Max Health)
        for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        {
            if (spellInfo->Effects[i].Effect == SPELL_EFFECT_HEAL_MAX_HEALTH)
                return;
        }
        if (spellInfo->Id == SPELL_RUNE_TAP || spellInfo->Id == SPELL_LIFE_STEAL || spellInfo->Id == SPELL_CANNIBALISE)
        {
            return;
        }

        // No progression healing nerf in BGs/Arenas
        if (healer->GetMap()->IsBattlegroundOrArena())
            return;

        // Only apply if healer is a player or controlled by a player
        Player* player = GetProgressionPlayer(healer);
        if (!player)
            return;

        float mult = GetHealingMultiplier(player);
        if (mult != 1.0f)
            heal = uint32(float(heal) * mult);
    }

    void ModifySpellDamageTaken(Unit* /*target*/, Unit* attacker, int32& damage, SpellInfo const* /*spellInfo*/) override
    {
        if (!sIndividualProgression->enabled || !attacker || damage <= 0)
            return;

        // No progression damage nerf in BGs/Arenas
        if (attacker->GetMap()->IsBattlegroundOrArena())
            return;

        Player* player = GetProgressionPlayer(attacker);
        if (!player)
            return;

        float mult = GetDamageMultiplier(player);
        if (mult != 1.0f)
            damage = int32(float(damage) * mult);
    }

    void ModifyMeleeDamage(Unit* /*target*/, Unit* attacker, uint32& damage) override
    {
        if (!sIndividualProgression->enabled || !attacker || !damage)
            return;

        // No progression damage nerf in BGs/Arenas
        if (attacker->GetMap()->IsBattlegroundOrArena())
            return;

        Player* player = GetProgressionPlayer(attacker);
        if (!player)
            return;

        float mult = GetDamageMultiplier(player);
        if (mult != 1.0f)
            damage = uint32(float(damage) * mult);
    }

    void ModifyPeriodicDamageAurasTick(Unit* /*target*/, Unit* attacker, uint32& damage, SpellInfo const* spellInfo) override
    {
        if (!sIndividualProgression->enabled || !attacker || !damage || !spellInfo)
            return;

        // Do not apply reductions to periodic healing auras
        for (uint8 j = 0; j < MAX_SPELL_EFFECTS; ++j)
        {
            if (spellInfo->Effects[j].Effect == SPELL_EFFECT_APPLY_AURA &&
                spellInfo->Effects[j].ApplyAuraName == SPELL_AURA_PERIODIC_HEAL)
            {
                return;
            }
        }

        // No progression damage nerf in BGs/Arenas
        if (attacker->GetMap()->IsBattlegroundOrArena())
            return;

        Player* player = GetProgressionPlayer(attacker);
        if (!player)
            return;

        float mult = GetDamageMultiplier(player);
        if (mult != 1.0f)
            damage = uint32(float(damage) * mult);
    }
};

void AddSC_mod_individual_progression_player()
{
    new IndividualPlayerProgression();
    new IndividualPlayerProgression_AccountScript();
    new IndividualPlayerProgression_UnitScript();
}
