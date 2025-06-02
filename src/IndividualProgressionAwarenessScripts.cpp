#include "IndividualProgression.h"
#include "SmartAI.h"

class gobject_ipp_preaq : public GameObjectScript
{
public:
    gobject_ipp_preaq() : GameObjectScript("gobject_ipp_preaq") { }

    struct gobject_ipp_preaqAI: GameObjectAI
    {
        explicit gobject_ipp_preaqAI(GameObject* object) : GameObjectAI(object) { };

        bool CanBeSeen(Player const* player) override
        {
            if (player->IsGameMaster() || !sIndividualProgression->enabled)
            {
                return true;
            }
            Player* target = ObjectAccessor::FindConnectedPlayer(player->GetGUID());
            return sIndividualProgression->hasPassedProgression(target, PROGRESSION_BLACKWING_LAIR);
        }
    };

    GameObjectAI* GetAI(GameObject* object) const override
    {
        return new gobject_ipp_preaqAI(object);
    }
};

class gobject_ipp_we : public GameObjectScript
{
public:
    gobject_ipp_we() : GameObjectScript("gobject_ipp_we") { }

    struct gobject_ipp_weAI: GameObjectAI
    {
        explicit gobject_ipp_weAI(GameObject* object) : GameObjectAI(object) { };

        bool CanBeSeen(Player const* player) override
        {
            if (player->IsGameMaster() || !sIndividualProgression->enabled)
            {
                return true;
            }
            
            Player* target = ObjectAccessor::FindConnectedPlayer(player->GetGUID());
            
            if (sIndividualProgression->hasPassedProgression(target, PROGRESSION_PRE_AQ))
            {
                return sIndividualProgression->isBeforeProgression(target, PROGRESSION_PRE_AQ);
            }
            else
            {
                return sIndividualProgression->hasPassedProgression(target, PROGRESSION_BLACKWING_LAIR);
            }
        }
    };

    GameObjectAI* GetAI(GameObject* object) const override
    {
        return new gobject_ipp_weAI(object);
    }
};

class gobject_ipp_aqwar : public GameObjectScript
{
public:
    gobject_ipp_aqwar() : GameObjectScript("gobject_ipp_aqwar") { }

    struct gobject_ipp_aqwarAI: GameObjectAI
    {
        explicit gobject_ipp_aqwarAI(GameObject* object) : GameObjectAI(object) { };

        bool CanBeSeen(Player const* player) override
        {
            if (player->IsGameMaster() || !sIndividualProgression->enabled)
            {
                return true;
            }
            
            Player* target = ObjectAccessor::FindConnectedPlayer(player->GetGUID());
            
            if (sIndividualProgression->hasPassedProgression(target, PROGRESSION_AQ_WAR))
            {
                return sIndividualProgression->isBeforeProgression(target, PROGRESSION_AQ_WAR);
            }
            else
            {
                return sIndividualProgression->hasPassedProgression(target, PROGRESSION_PRE_AQ);
            }
        }
    };

    GameObjectAI* GetAI(GameObject* object) const override
    {
        return new gobject_ipp_aqwarAI(object);
    }
};

class gobject_ipp_si : public GameObjectScript
{
public:
    gobject_ipp_si() : GameObjectScript("gobject_ipp_si") { }

    struct gobject_ipp_siAI: GameObjectAI
    {
        explicit gobject_ipp_siAI(GameObject* object) : GameObjectAI(object) { };

        bool CanBeSeen(Player const* player) override
        {
            if (player->IsGameMaster())
            {
                return true;
            }
            
            Player* target = ObjectAccessor::FindConnectedPlayer(player->GetGUID());
            
            if (sIndividualProgression->hasPassedProgression(target, PROGRESSION_NAXX40))
            {
                return sIndividualProgression->isBeforeProgression(target, PROGRESSION_NAXX40);
            }
            else
            {
                return sIndividualProgression->hasPassedProgression(target, PROGRESSION_AQ);
            }
        }
    };

    GameObjectAI* GetAI(GameObject* object) const override
    {
        return new gobject_ipp_siAI(object);
    }
};

class gobject_ipp_naxx40 : public GameObjectScript
{
public:
    gobject_ipp_naxx40() : GameObjectScript("gobject_ipp_naxx40") { }

    struct gobject_ipp_naxx40AI: GameObjectAI
    {
        explicit gobject_ipp_naxx40AI(GameObject* object) : GameObjectAI(object) { };

        bool CanBeSeen(Player const* player) override
        {
            if (player->IsGameMaster() || !sIndividualProgression->enabled)
            {
                return true;
            }
            Player* target = ObjectAccessor::FindConnectedPlayer(player->GetGUID());
            return sIndividualProgression->hasPassedProgression(target, PROGRESSION_AQ);
        }
    };

    GameObjectAI* GetAI(GameObject* object) const override
    {
        return new gobject_ipp_naxx40AI(object);
    }
};

class gobject_ipp_tbc : public GameObjectScript
{
public:
    gobject_ipp_tbc() : GameObjectScript("gobject_ipp_tbc") { }

    struct gobject_ipp_tbcAI: GameObjectAI
    {
        explicit gobject_ipp_tbcAI(GameObject* object) : GameObjectAI(object) { };

        bool CanBeSeen(Player const* player) override
        {
            if (player->IsGameMaster() || !sIndividualProgression->enabled)
            {
                return true;
            }
            Player* target = ObjectAccessor::FindConnectedPlayer(player->GetGUID());
            return sIndividualProgression->hasPassedProgression(target, PROGRESSION_PRE_TBC);
        }
    };

    GameObjectAI* GetAI(GameObject* object) const override
    {
        return new gobject_ipp_tbcAI(object);
    }
};

class gobject_ipp_wotlk : public GameObjectScript
{
public:
    gobject_ipp_wotlk() : GameObjectScript("gobject_ipp_wotlk") { }

    struct gobject_ipp_wotlkAI: GameObjectAI
    {
        explicit gobject_ipp_wotlkAI(GameObject* object) : GameObjectAI(object) { };

        bool CanBeSeen(Player const* player) override
        {
            if (player->IsGameMaster() || !sIndividualProgression->enabled)
            {
                return true;
            }
            Player* target = ObjectAccessor::FindConnectedPlayer(player->GetGUID());
            return sIndividualProgression->hasPassedProgression(target, PROGRESSION_TBC_TIER_5);
        }
    };

    GameObjectAI* GetAI(GameObject* object) const override
    {
        return new gobject_ipp_wotlkAI(object);
    }
};

class npc_ipp_preaq : public CreatureScript
{
public:
    npc_ipp_preaq() : CreatureScript("npc_ipp_preaq") { }

    struct npc_ipp_preaqAI: SmartAI
    {
        explicit npc_ipp_preaqAI(Creature* creature) : SmartAI(creature) { };

        void AttackStart(Unit* target) override
        {
            if (target->IsPlayer())
            {
                if (!CanBeSeen(target->ToPlayer()))
                    return;
            }

            SmartAI::AttackStart(target);
        }

        bool CanBeSeen(Player const* player) override
        {
            if (player->IsGameMaster() || !sIndividualProgression->enabled || me->IsInCombat())
            {
                return true;
            }
            Player* target = ObjectAccessor::FindConnectedPlayer(player->GetGUID());
            return sIndividualProgression->hasPassedProgression(target, PROGRESSION_BLACKWING_LAIR);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_ipp_preaqAI(creature);
    }
};

class npc_ipp_we : public CreatureScript
{
public:
    npc_ipp_we() : CreatureScript("npc_ipp_we") { }

    struct npc_ipp_weAI: SmartAI
    {
        explicit npc_ipp_weAI(Creature* creature) : SmartAI(creature) { };

        void AttackStart(Unit* target) override
        {
            if (target->IsPlayer())
            {
                if (!CanBeSeen(target->ToPlayer()))
                    return;
            }

            SmartAI::AttackStart(target);
        }

        bool CanBeSeen(Player const* player) override
        {
            if (player->IsGameMaster() || !sIndividualProgression->enabled || me->IsInCombat())
            {
                return true;
            }
			
            Player* target = ObjectAccessor::FindConnectedPlayer(player->GetGUID());
			
            if (sIndividualProgression->hasPassedProgression(target, PROGRESSION_PRE_AQ))
            {
                return sIndividualProgression->isBeforeProgression(target, PROGRESSION_PRE_AQ);
            }
            else
            {
                return sIndividualProgression->hasPassedProgression(target, PROGRESSION_BLACKWING_LAIR);
            }			
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_ipp_weAI(creature);
    }
};

class npc_ipp_aq : public CreatureScript
{
public:
    npc_ipp_aq() : CreatureScript("npc_ipp_aq") { }

    struct npc_ipp_aqAI: SmartAI
    {
        explicit npc_ipp_aqAI(Creature* creature) : SmartAI(creature) { };

        void AttackStart(Unit* target) override
        {
            if (target->IsPlayer())
            {
                if (!CanBeSeen(target->ToPlayer()))
                    return;
            }

            SmartAI::AttackStart(target);
        }

        bool CanBeSeen(Player const* player) override
        {
            if (player->IsGameMaster() || !sIndividualProgression->enabled || me->IsInCombat())
            {
                return true;
            }
            Player* target = ObjectAccessor::FindConnectedPlayer(player->GetGUID());
            return sIndividualProgression->hasPassedProgression(target, PROGRESSION_AQ_WAR);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_ipp_aqAI(creature);
    }
};

class npc_ipp_si : public CreatureScript
{
public:
    npc_ipp_si() : CreatureScript("npc_ipp_si") { }

    struct npc_ipp_siAI: SmartAI
    {
        explicit npc_ipp_siAI(Creature* creature) : SmartAI(creature) { };

        void AttackStart(Unit* target) override
        {
            if (target->IsPlayer())
            {
                if (!CanBeSeen(target->ToPlayer()))
                    return;
            }

            SmartAI::AttackStart(target);
        }

        bool CanBeSeen(Player const* player) override
        {
            if (player->IsGameMaster())
            {
                return true;
            }
			
            Player* target = ObjectAccessor::FindConnectedPlayer(player->GetGUID());
			
            if (sIndividualProgression->hasPassedProgression(target, PROGRESSION_NAXX40))
            {
                return sIndividualProgression->isBeforeProgression(target, PROGRESSION_NAXX40);
            }
            else
            {
                return sIndividualProgression->hasPassedProgression(target, PROGRESSION_AQ);
            }			
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_ipp_siAI(creature);
    }
};

class npc_ipp_pre_naxx40 : public CreatureScript
{
public:
    npc_ipp_pre_naxx40() : CreatureScript("npc_ipp_pre_naxx40") { }

    struct npc_ipp_pre_naxx40AI: SmartAI
    {
        explicit npc_ipp_pre_naxx40AI(Creature* creature) : SmartAI(creature) { };

        void AttackStart(Unit* target) override
        {
            if (target->IsPlayer())
            {
                if (!CanBeSeen(target->ToPlayer()))
                    return;
            }

            SmartAI::AttackStart(target);
        }

        bool CanBeSeen(Player const* player) override
        {
            if (player->IsGameMaster() || !sIndividualProgression->enabled || me->IsInCombat())
            {
                return true;
            }
            Player* target = ObjectAccessor::FindConnectedPlayer(player->GetGUID());
            return sIndividualProgression->isBeforeProgression(target, PROGRESSION_AQ);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_ipp_pre_naxx40AI(creature);
    }
};

class npc_ipp_naxx40 : public CreatureScript
{
public:
    npc_ipp_naxx40() : CreatureScript("npc_ipp_naxx40") { }

    struct npc_ipp_naxx40AI: SmartAI
    {
        explicit npc_ipp_naxx40AI(Creature* creature) : SmartAI(creature) { };

        void AttackStart(Unit* target) override
        {
            if (target->IsPlayer())
            {
                if (!CanBeSeen(target->ToPlayer()))
                    return;
            }

            SmartAI::AttackStart(target);
        }

        bool CanBeSeen(Player const* player) override
        {
            if (player->IsGameMaster() || !sIndividualProgression->enabled || me->IsInCombat())
            {
                return true;
            }
            Player* target = ObjectAccessor::FindConnectedPlayer(player->GetGUID());
            return sIndividualProgression->hasPassedProgression(target, PROGRESSION_AQ);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_ipp_naxx40AI(creature);
    }
};

class npc_ipp_tbc : public CreatureScript
{
public:
    npc_ipp_tbc() : CreatureScript("npc_ipp_tbc") { }

    struct npc_ipp_tbcAI: SmartAI
    {
        explicit npc_ipp_tbcAI(Creature* creature) : SmartAI(creature) { };

        void AttackStart(Unit* target) override
        {
            if (target->IsPlayer())
            {
                if (!CanBeSeen(target->ToPlayer()))
                    return;
            }

            SmartAI::AttackStart(target);
        }

        bool CanBeSeen(Player const* player) override
        {
            if (player->IsGameMaster() || !sIndividualProgression->enabled || me->IsInCombat())
            {
                return true;
            }
            Player* target = ObjectAccessor::FindConnectedPlayer(player->GetGUID());
            return sIndividualProgression->hasPassedProgression(target, PROGRESSION_PRE_TBC);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_ipp_tbcAI(creature);
    }
};

class npc_ipp_tbc_pre_t4 : public CreatureScript
{
public:
    npc_ipp_tbc_pre_t4() : CreatureScript("npc_ipp_tbc_pre_t4") { }

    struct npc_ipp_tbc_pre_t4AI: SmartAI
    {
        explicit npc_ipp_tbc_pre_t4AI(Creature* creature) : SmartAI(creature) { };

        void AttackStart(Unit* target) override
        {
            if (target->IsPlayer())
            {
                if (!CanBeSeen(target->ToPlayer()))
                    return;
            }

            SmartAI::AttackStart(target);
        }

        bool CanBeSeen(Player const* player) override
        {
            if (player->IsGameMaster() || !sIndividualProgression->enabled || me->IsInCombat())
            {
                return true;
            }
            Player* target = ObjectAccessor::FindConnectedPlayer(player->GetGUID());
            return sIndividualProgression->isBeforeProgression(target,PROGRESSION_TBC_TIER_3);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_ipp_tbc_pre_t4AI(creature);
    }
};

class npc_ipp_tbc_t4 : public CreatureScript
{
public:
    npc_ipp_tbc_t4() : CreatureScript("npc_ipp_tbc_t4") { }

    struct npc_ipp_tbc_t4AI: SmartAI
    {
        explicit npc_ipp_tbc_t4AI(Creature* creature) : SmartAI(creature) { };

        void AttackStart(Unit* target) override
        {
            if (target->IsPlayer())
            {
                if (!CanBeSeen(target->ToPlayer()))
                    return;
            }

            SmartAI::AttackStart(target);
        }

        bool CanBeSeen(Player const* player) override
        {
            if (player->IsGameMaster() || !sIndividualProgression->enabled || me->IsInCombat())
            {
                return true;
            }
            Player* target = ObjectAccessor::FindConnectedPlayer(player->GetGUID());
            return sIndividualProgression->hasPassedProgression(target, PROGRESSION_TBC_TIER_3);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_ipp_tbc_t4AI(creature);
    }
};

class npc_ipp_tbc_t5 : public CreatureScript
{
public:
    npc_ipp_tbc_t5() : CreatureScript("npc_ipp_tbc_t5") { }

    struct npc_ipp_tbc_t5AI: SmartAI
    {
        explicit npc_ipp_tbc_t5AI(Creature* creature) : SmartAI(creature) { };

        void AttackStart(Unit* target) override
        {
            if (target->IsPlayer())
            {
                if (!CanBeSeen(target->ToPlayer()))
                    return;
            }

            SmartAI::AttackStart(target);
        }

        bool CanBeSeen(Player const* player) override
        {
            if (player->IsGameMaster() || !sIndividualProgression->enabled || me->IsInCombat())
            {
                return true;
            }
            Player* target = ObjectAccessor::FindConnectedPlayer(player->GetGUID());
            return sIndividualProgression->hasPassedProgression(target, PROGRESSION_TBC_TIER_4);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_ipp_tbc_t5AI(creature);
    }
};

class npc_ipp_wotlk : public CreatureScript
{
public:
    npc_ipp_wotlk() : CreatureScript("npc_ipp_wotlk") { }

    struct npc_ipp_wotlkAI: SmartAI
    {
        explicit npc_ipp_wotlkAI(Creature* creature) : SmartAI(creature) { };

        void AttackStart(Unit* target) override
        {
            if (target->IsPlayer())
            {
                if (!CanBeSeen(target->ToPlayer()))
                    return;
            }

            SmartAI::AttackStart(target);
        }

        bool CanBeSeen(Player const* player) override
        {
            if (player->IsGameMaster() || !sIndividualProgression->enabled || me->IsInCombat())
            {
                return true;
            }
            Player* target = ObjectAccessor::FindConnectedPlayer(player->GetGUID());
            return sIndividualProgression->hasPassedProgression(target, PROGRESSION_TBC_TIER_5);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_ipp_wotlkAI(creature);
    }
};

class npc_ipp_wotlk_ulduar : public CreatureScript
{
public:
    npc_ipp_wotlk_ulduar() : CreatureScript("npc_ipp_wotlk_ulduar") { }

    struct npc_ipp_wotlk_ulduarAI: SmartAI
    {
        explicit npc_ipp_wotlk_ulduarAI(Creature* creature) : SmartAI(creature) { };

        void AttackStart(Unit* target) override
        {
            if (target->IsPlayer())
            {
                if (!CanBeSeen(target->ToPlayer()))
                    return;
            }

            SmartAI::AttackStart(target);
        }

        bool CanBeSeen(Player const* player) override
        {
            if (player->IsGameMaster() || !sIndividualProgression->enabled || me->IsInCombat())
            {
                return true;
            }
            Player* target = ObjectAccessor::FindConnectedPlayer(player->GetGUID());
            return sIndividualProgression->hasPassedProgression(target, PROGRESSION_WOTLK_TIER_1);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_ipp_wotlk_ulduarAI(creature);
    }
};

class npc_ipp_wotlk_totc : public CreatureScript
{
public:
    npc_ipp_wotlk_totc() : CreatureScript("npc_ipp_wotlk_totc") { }

    struct npc_ipp_wotlk_totcAI: SmartAI
    {
        explicit npc_ipp_wotlk_totcAI(Creature* creature) : SmartAI(creature) { };

        void AttackStart(Unit* target) override
        {
            if (target->IsPlayer())
            {
                if (!CanBeSeen(target->ToPlayer()))
                    return;
            }

            SmartAI::AttackStart(target);
        }

        bool CanBeSeen(Player const* player) override
        {
            if (player->IsGameMaster() || !sIndividualProgression->enabled || me->IsInCombat())
            {
                return true;
            }
            Player* target = ObjectAccessor::FindConnectedPlayer(player->GetGUID());
            return sIndividualProgression->hasPassedProgression(target, PROGRESSION_WOTLK_TIER_2);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_ipp_wotlk_totcAI(creature);
    }
};

class npc_ipp_wotlk_icc : public CreatureScript
{
public:
    npc_ipp_wotlk_icc() : CreatureScript("npc_ipp_wotlk_icc") { }

    struct npc_ipp_wotlk_iccAI: SmartAI
    {
        explicit npc_ipp_wotlk_iccAI(Creature* creature) : SmartAI(creature) { };

        void AttackStart(Unit* target) override
        {
            if (target->IsPlayer())
            {
                if (!CanBeSeen(target->ToPlayer()))
                    return;
            }

            SmartAI::AttackStart(target);
        }

        bool CanBeSeen(Player const* player) override
        {
            if (player->IsGameMaster() || !sIndividualProgression->enabled || me->IsInCombat())
            {
                return true;
            }
            Player* target = ObjectAccessor::FindConnectedPlayer(player->GetGUID());
            return sIndividualProgression->hasPassedProgression(target, PROGRESSION_WOTLK_TIER_3);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_ipp_wotlk_iccAI(creature);
    }
};

class npc_ipp_ds2 : public CreatureScript
{
public:
    npc_ipp_ds2() : CreatureScript("npc_ipp_ds2") { }

    struct npc_ipp_ds2AI: SmartAI
    {
        explicit npc_ipp_ds2AI(Creature* creature) : SmartAI(creature) { };

        void AttackStart(Unit* target) override
        {
            if (target->IsPlayer())
            {
                if (!CanBeSeen(target->ToPlayer()))
                    return;
            }

            SmartAI::AttackStart(target);
        }

        bool CanBeSeen(Player const* player) override
        {
            if (player->IsGameMaster() || !sIndividualProgression->enabled || me->IsInCombat())
            {
                return true;
            }
            if (sIndividualProgression->earlyDungeonSet2)
            {
                return true;
            }
            Player* target = ObjectAccessor::FindConnectedPlayer(player->GetGUID());
            return sIndividualProgression->hasPassedProgression(target, PROGRESSION_BLACKWING_LAIR);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_ipp_ds2AI(creature);
    }
};

class npc_ipp_ragefire_chasm : public CreatureScript
{
public:
    npc_ipp_ragefire_chasm() : CreatureScript("npc_ipp_ragefire_chasm") { }

    struct npc_ipp_ragefire_chasmAI: SmartAI
    {
        explicit npc_ipp_ragefire_chasmAI(Creature* creature) : SmartAI(creature) { };

        void AttackStart(Unit* target) override
        {
            if (target->IsPlayer())
            {
                if (!CanBeSeen(target->ToPlayer()))
                    return;
            }

            if (target->GetTypeId() == TYPEID_UNIT && target->GetEntry() == 3986)
            {
                return;
            }

            SmartAI::AttackStart(target);
        }

        bool CanBeSeen(Player const* player) override
        {
            if (player->IsGameMaster() || !sIndividualProgression->enabled || me->IsInCombat())
            {
                return true;
            }
            Player* target = ObjectAccessor::FindConnectedPlayer(player->GetGUID());
            return sIndividualProgression->hasPassedProgression(target, PROGRESSION_WOTLK_TIER_4);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_ipp_ragefire_chasmAI(creature);
    }
};

class npc_ipp_ragefire_chasm_remove : public CreatureScript
{
public:
    npc_ipp_ragefire_chasm_remove() : CreatureScript("npc_ipp_ragefire_chasm_remove") { }

    struct npc_ipp_ragefire_chasm_removeAI: SmartAI
    {
        explicit npc_ipp_ragefire_chasm_removeAI(Creature* creature) : SmartAI(creature) { };

        void AttackStart(Unit* target) override
        {
            if (target->IsPlayer())
            {
                if (!CanBeSeen(target->ToPlayer()))
                    return;
            }

            SmartAI::AttackStart(target);
        }

        bool CanBeSeen(Player const* player) override
        {
            if (player->IsGameMaster() || !sIndividualProgression->enabled || me->IsInCombat())
            {
                return true;
            }
            Player* target = ObjectAccessor::FindConnectedPlayer(player->GetGUID());
            return !sIndividualProgression->hasPassedProgression(target, PROGRESSION_WOTLK_TIER_4);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_ipp_ragefire_chasm_removeAI(creature);
    }
};

class npc_ipp_individual_progression_setter : public CreatureScript
{
public:
    npc_ipp_individual_progression_setter() : CreatureScript("npc_ipp_individual_progression_setter") { }

    struct npc_ipp_individual_progression_setterAI: ScriptedAI
    {
        explicit npc_ipp_individual_progression_setterAI(Creature* creature) : ScriptedAI(creature) { };

        bool CanBeSeen(Player const* player) override
        {
            if (player->IsGameMaster() || !sIndividualProgression->enabled || sIndividualProgression->progressionSetterAlwaysVisible)
            {
                return true;
            }
            Player* target = ObjectAccessor::FindConnectedPlayer(player->GetGUID());
            uint8 highestProgression = sIndividualProgression->GetAccountProgression(target->GetSession()->GetAccountId());
            if (sIndividualProgression->hasPassedProgression(target, PROGRESSION_CATA_TIER_1) || (sIndividualProgression->progressionLimit && highestProgression >= sIndividualProgression->progressionLimit))
                return true;
            else
                return false;
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_ipp_individual_progression_setterAI(creature);
    }
};

class npc_ipp_westfall_riot : public CreatureScript
{
public:
    npc_ipp_westfall_riot() : CreatureScript("npc_ipp_westfall_riot") { }

    struct npc_ipp_westfall_riotAI: SmartAI
    {
        explicit npc_ipp_westfall_riotAI(Creature* creature) : SmartAI(creature) { };

        void AttackStart(Unit* target) override
        {
            if (target->IsPlayer())
            {
                if (!CanBeSeen(target->ToPlayer()))
                    return;
            }

            SmartAI::AttackStart(target);
        }

        bool CanBeSeen(Player const* player) override
        {
            if (player->IsGameMaster() || !sIndividualProgression->enabled || me->IsInCombat())
            {
                return true;
            }
            Player* target = ObjectAccessor::FindConnectedPlayer(player->GetGUID());
            return target->GetQuestRewardStatus(50109);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_ipp_westfall_riotAI(creature);
    }
};

class npc_ipp_duskwood_worgen : public CreatureScript
{
public:
    npc_ipp_duskwood_worgen() : CreatureScript("npc_ipp_duskwood_worgen") { }

    struct npc_ipp_duskwood_worgenAI: SmartAI
    {
        explicit npc_ipp_duskwood_worgenAI(Creature* creature) : SmartAI(creature) { };

        void AttackStart(Unit* target) override
        {
            if (target->IsPlayer())
            {
                if (!CanBeSeen(target->ToPlayer()))
                    return;
            }

            SmartAI::AttackStart(target);
        }

        bool CanBeSeen(Player const* player) override
        {
            if (player->IsGameMaster() || !sIndividualProgression->enabled || me->IsInCombat())
            {
                return true;
            }
            Player* target = ObjectAccessor::FindConnectedPlayer(player->GetGUID());
            return target->HasAura(98927);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_ipp_duskwood_worgenAI(creature);
    }
};

class npc_ipp_hillsbrad_presence : public CreatureScript
{
public:
npc_ipp_hillsbrad_presence() : CreatureScript("npc_ipp_hillsbrad_presence") { }

    struct npc_ipp_hillsbrad_presenceAI: SmartAI
    {
        explicit npc_ipp_hillsbrad_presenceAI(Creature* creature) : SmartAI(creature) { };

        void AttackStart(Unit* target) override
        {
            if (target->IsPlayer())
            {
                if (!CanBeSeen(target->ToPlayer()))
                    return;
            }

            SmartAI::AttackStart(target);
        }

        bool CanBeSeen(Player const* player) override
        {
            if (player->IsGameMaster() || !sIndividualProgression->enabled || me->IsInCombat())
            {
                return true;
            }
            Player* target = ObjectAccessor::FindConnectedPlayer(player->GetGUID());
            return target->HasAura(98997);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_ipp_hillsbrad_presenceAI(creature);
    }
};

class npc_ipp_magatha_tn : public CreatureScript
{
public:
    npc_ipp_magatha_tn() : CreatureScript("npc_ipp_magatha_tn") { }

    struct npc_ipp_magatha_tnAI: SmartAI
    {
        explicit npc_ipp_magatha_tnAI(Creature* creature) : SmartAI(creature) { };

        void AttackStart(Unit* target) override
        {
            if (target->IsPlayer())
            {
                if (!CanBeSeen(target->ToPlayer()))
                    return;
            }

            SmartAI::AttackStart(target);
        }

        bool CanBeSeen(Player const* player) override
        {
            if (player->IsGameMaster() || !sIndividualProgression->enabled || me->IsInCombat())
            {
                return true;
            }
            Player* target = ObjectAccessor::FindConnectedPlayer(player->GetGUID());
            return target->GetQuestRewardStatus(50233);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_ipp_magatha_tnAI(creature);
    }
};

// Add all scripts in one
void AddSC_mod_individual_progression_awareness()
{
    new gobject_ipp_preaq();   // wanted poster Cenarion Hold
    new gobject_ipp_we();      // War Effort supplies in cities
    new gobject_ipp_aqwar();   // AQ war crystals
    new gobject_ipp_si();      // Scourge Invasion
    new gobject_ipp_naxx40();
    new gobject_ipp_tbc();
    new gobject_ipp_wotlk();
    new npc_ipp_preaq();       // Cenarion Hold NPCs
    new npc_ipp_we();          // War Effort NPCs in cities
	new npc_ipp_aq();
    new npc_ipp_si();          // Scourge Invasion
    new npc_ipp_pre_naxx40();  // Scourge Invasion
    new npc_ipp_naxx40();
    new npc_ipp_tbc();
    new npc_ipp_tbc_pre_t4();
    new npc_ipp_tbc_t4();
    new npc_ipp_tbc_t5();
    new npc_ipp_wotlk();
    new npc_ipp_wotlk_ulduar();
    new npc_ipp_wotlk_totc();
    new npc_ipp_wotlk_icc();
    new npc_ipp_ds2();
    new npc_ipp_individual_progression_setter();
    new npc_ipp_ragefire_chasm();
    new npc_ipp_ragefire_chasm_remove();
    new npc_ipp_westfall_riot();
    new npc_ipp_duskwood_worgen();
    new npc_ipp_hillsbrad_presence();
    new npc_ipp_magatha_tn();
}
