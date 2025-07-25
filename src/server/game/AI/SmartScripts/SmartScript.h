/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TRINITY_SMARTSCRIPT_H
#define TRINITY_SMARTSCRIPT_H

#include "Define.h"
#include "SmartScriptMgr.h"
#include <memory>

class AreaTrigger;
class Creature;
class GameObject;
class Player;
class Quest;
class SpellInfo;
class Unit;
class WorldObject;
struct AreaTriggerEntry;
struct SceneTemplate;

namespace Scripting::v2
{
class ActionBase;
}

class TC_GAME_API SmartScript
{
    public:
        SmartScript();
        SmartScript(SmartScript const& other);
        SmartScript(SmartScript&& other) noexcept;
        SmartScript& operator=(SmartScript const& other);
        SmartScript& operator=(SmartScript&& other) noexcept;
        ~SmartScript();

        void OnInitialize(WorldObject* obj, AreaTriggerEntry const* at = nullptr, SceneTemplate const* scene = nullptr, Quest const* qst = nullptr, uint32 evnt = 0);
        void GetScript();
        void FillScript(SmartAIEventList&& e, WorldObject* obj, AreaTriggerEntry const* at, SceneTemplate const* scene, Quest const* quest, uint32 event = 0);

        void ProcessEventsFor(SMART_EVENT e, Unit* unit = nullptr, uint32 var0 = 0, uint32 var1 = 0, bool bvar = false, SpellInfo const* spell = nullptr, GameObject* gob = nullptr, std::string_view varString = { });
        void ProcessEvent(SmartScriptHolder& e, Unit* unit = nullptr, uint32 var0 = 0, uint32 var1 = 0, bool bvar = false, SpellInfo const* spell = nullptr, GameObject* gob = nullptr, std::string_view varString = { });
        bool CheckTimer(SmartScriptHolder const& e) const;
        static void RecalcTimer(SmartScriptHolder& e, uint32 min, uint32 max);
        void UpdateTimer(SmartScriptHolder& e, uint32 const diff);
        static void InitTimer(SmartScriptHolder& e);
        void ProcessAction(SmartScriptHolder& e, Unit* unit = nullptr, uint32 var0 = 0, uint32 var1 = 0, bool bvar = false, SpellInfo const* spell = nullptr, GameObject* gob = nullptr, std::string_view varString = { });
        void ProcessTimedAction(SmartScriptHolder& e, uint32 const& min, uint32 const& max, Unit* unit = nullptr, uint32 var0 = 0, uint32 var1 = 0, bool bvar = false, SpellInfo const* spell = nullptr, GameObject* gob = nullptr, std::string_view varString = { });
        void GetTargets(ObjectVector& targets, SmartScriptHolder const& e, WorldObject* invoker = nullptr) const;
        static SmartScriptHolder CreateSmartEvent(SMART_EVENT e, uint32 event_flags, uint32 event_param1, uint32 event_param2, uint32 event_param3, uint32 event_param4, uint32 event_param5, SMART_ACTION action, uint32 action_param1, uint32 action_param2, uint32 action_param3, uint32 action_param4, uint32 action_param5, uint32 action_param6, uint32 action_param7, SMARTAI_TARGETS t, uint32 target_param1, uint32 target_param2, uint32 target_param3, uint32 target_param4, std::string_view targetParamString, uint32 phaseMask);
        void SetPathId(uint32 id) { mPathId = id; }
        uint32 GetPathId() const { return mPathId; }
        WorldObject* GetBaseObject() const;
        WorldObject* GetBaseObjectOrUnitInvoker(Unit* invoker);
        bool HasAnyEventWithFlag(uint32 flag) const { return mAllEventFlags & flag; }

        void OnUpdate(const uint32 diff);
        void OnMoveInLineOfSight(Unit* who);

        Unit* DoSelectLowestHpFriendly(float range, uint32 MinHPDiff) const;
        Unit* DoSelectLowestHpPercentFriendly(float range, uint32 minHpPct, uint32 maxHpPct) const;
        void DoFindFriendlyCC(std::vector<Creature*>& creatures, float range) const;
        void DoFindFriendlyMissingBuff(std::vector<Creature*>& creatures, float range, uint32 spellid) const;
        Unit* DoFindClosestFriendlyInRange(float range, bool playerOnly) const;

        bool IsSmart(Creature* c, bool silent = false) const;
        bool IsSmart(GameObject* g, bool silent = false) const;
        bool IsSmart(bool silent = false) const;

        void StoreTargetList(ObjectVector const& targets, uint32 id);
        void AddToStoredTargetList(ObjectVector const& targets, uint32 id);
        ObjectVector const* GetStoredTargetVector(uint32 id, WorldObject const& ref) const;

        void StoreCounter(uint32 id, uint32 value, uint32 reset);
        uint32 GetCounterValue(uint32 id) const;

        GameObject* FindGameObjectNear(WorldObject* searchObject, ObjectGuid::LowType guid) const;
        Creature* FindCreatureNear(WorldObject* searchObject, ObjectGuid::LowType guid) const;

        void OnReset();
        void ResetBaseObject();

        void SetTimedActionList(SmartScriptHolder& e, uint32 entry, Unit* invoker, uint32 startFromEventId = 0);
        Unit* GetLastInvoker(Unit* invoker = nullptr) const;
        ObjectGuid mLastInvoker;
        typedef std::unordered_map<uint32, uint32> CounterMap;
        CounterMap mCounterList;

    private:

        void IncPhase(uint32 p);
        void DecPhase(uint32 p);

        void SetPhase(uint32 p);
        bool IsInPhase(uint32 p) const;

        void SortEvents(SmartAIEventList& events);
        void RaisePriority(SmartScriptHolder& e);
        void RetryLater(SmartScriptHolder& e, bool ignoreChanceRoll = false);

        SmartAIEventList mEvents;
        SmartAIEventList mInstallEvents;
        SmartAIEventList mTimedActionList;
        ObjectGuid mTimedActionListInvoker;
        std::shared_ptr<Scripting::v2::ActionBase> mTimedActionWaitEvent;
        bool isProcessingTimedActionList;
        Creature* me;
        ObjectGuid meOrigGUID;
        GameObject* go;
        ObjectGuid goOrigGUID;
        Player* player;
        AreaTriggerEntry const* trigger;
        AreaTrigger* areaTrigger;
        SceneTemplate const* sceneTemplate;
        Quest const* quest;
        uint32 event;
        SmartScriptType mScriptType;
        uint32 mEventPhase;

        uint32 mPathId;
        SmartAIEventStoredList mStoredEvents;
        std::vector<uint32> mRemIDs;

        uint32 mTextTimer;
        uint32 mLastTextID;
        uint32 mTalkerEntry;
        bool mUseTextTimer;
        uint32 mCurrentPriority;
        bool mEventSortingRequired;
        uint32 mNestedEventsCounter;
        uint32 mAllEventFlags;

        // Max number of nested ProcessEventsFor() calls to avoid infinite loops
        static constexpr uint32 MAX_NESTED_EVENTS = 10;

        ObjectVectorMap _storedTargets;

        void InstallEvents();

        void RemoveStoredEvent(uint32 id);
};

#endif
