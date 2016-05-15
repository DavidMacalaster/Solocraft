#include "Config.h"
#include "ScriptMgr.h"
#include "Player.h"
#include "Map.h"
#include "Group.h"
#include "InstanceScript.h"

namespace {

class solocraft_aura_base : public AuraScript {
	/*
	bool IsPassive() const override {
		return true;
	}

	bool IsDeathPersistent() const override {
		return true;
	}

	bool IsPermanent() const override {
		return true;
	}
	*/
};

/*
Aura for 5man instances.
SQL:
REPLACE INTO spell_dbc SET Id = 200000,
	Effect1 = 6, #APPLY_AURA
	Comment = "Solocraft: Aura 5man";
REPLACE INTO spell_script_names VALUES(200000, "solocraft_aura_5man");
*/
class solocraft_aura_5man : public SpellScriptLoader {
public:
	static const uint32 SPELL_ID = 200000;

	solocraft_aura_5man() : SpellScriptLoader("solocraft_aura_5man") {
		TC_LOG_INFO("scripts.solocraft.aura", "[Solocraft] solocraft_aura_5man Loaded");
	}

	class solocraft_aura_5man_AuraScript : public solocraft_aura_base {
		PrepareAuraScript(solocraft_aura_5man_AuraScript);

        void AfterApply(AuraEffect const *aurEff, AuraEffectHandleModes mode) {
            if (GetCaster()) {
                if (Unit* owner = GetCaster()->GetOwner()) {
                	if (Player *player = owner->ToPlayer()) {
                		ChatHandler(player->GetSession()).SendSysMessage("You have been buffed for a 5man");
                	}
                }
            }
        }

        void AfterRemove(AuraEffect const *aurEff, AuraEffectHandleModes mode) {
            if (GetCaster()) {
                if (Unit* owner = GetCaster()->GetOwner()) {
                    if (owner->GetTypeId() == TYPEID_PLAYER) {
                    	if (Player *player = owner->ToPlayer()) {
                    		ChatHandler(player->GetSession()).SendSysMessage("Your 5man buff has been removed");
                    	}
                    }
                }
            }
        }

		void Register() override {
            AfterEffectApply += AuraEffectApplyFn(solocraft_aura_5man_AuraScript::AfterApply,
            		EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(solocraft_aura_5man_AuraScript::AfterRemove,
            		EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
		}
	};

	AuraScript* GetAuraScript() const override {
		return new solocraft_aura_5man_AuraScript();
	}
};

class solocraft_player_instance_handler : public PlayerScript {
public:
	solocraft_player_instance_handler() : PlayerScript("solocraft_player_instance_handler") {
		TC_LOG_INFO("scripts.solocraft.player.instance", "[Solocraft] solocraft_player_instance_handler Loaded");
	}

	void OnLogin(Player *player, bool firstLogin) override {
		ChatHandler(player->GetSession()).SendSysMessage("Welcome to World of Solocraft");
	}

	void OnMapChanged(Player *player) override {
		int difficulty = 1;
		Map *map = player->GetMap();
		if (map) {
			if (map->Is25ManRaid()) {
				difficulty = 25;
			} else if (map->IsHeroic()) {
				difficulty = 10;
			} else if (map->IsRaid()) {
				difficulty = 40;
			} else if (map->IsDungeon()) {
				difficulty = 5;
			}
		}

		if (difficulty > 1) {
			InstanceMap *instanceMap = map->ToInstanceMap();
			InstanceScript *instanceScript = instanceMap->GetInstanceScript();

			int numInGroup = 1;
			Group *group = player->GetGroup();
			if (group) {
				Group::MemberSlotList const& groupMembers = group->GetMemberSlots();
				numInGroup = groupMembers.size();
			}

			ChatHandler(player->GetSession()).PSendSysMessage("Entered %s (difficulty = %d, numInGroup = %d)",
					map->GetMapName(), difficulty, numInGroup);

			player->AddAura(20217, player);
			player->AddAura(solocraft_aura_5man::SPELL_ID, player);
		} else {
			ChatHandler(player->GetSession()).PSendSysMessage("Left to %s", map->GetMapName());
			player->RemoveAura(20217);
			player->RemoveAura(solocraft_aura_5man::SPELL_ID);
		}
	}
private:
};

}

void AddSC_solocraft() {
	new solocraft_aura_5man();

	new solocraft_player_instance_handler();
}
