/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum Spells
{
    SPELL_GROUNDTREMOR                                     = 6524,
    SPELL_FRENZY                                           = 28371
};

class boss_grizzle : public CreatureScript
{
public:
    boss_grizzle() : CreatureScript("boss_grizzle") {}

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_grizzleAI (creature);
    }

    struct boss_grizzleAI : public ScriptedAI
    {
        boss_grizzleAI(Creature* creature) : ScriptedAI(creature) {}

        uint32 GroundTremor_Timer;
        uint32 Frenzy_Timer;

        void Reset() override
        {
            GroundTremor_Timer = 12000;
            Frenzy_Timer = 0;
        }

        void EnterCombat(Unit* /*who*/) override  {}

        void UpdateAI(uint32 diff) override
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            //GroundTremor_Timer
            if (GroundTremor_Timer <= diff)
            {
                if (auto victim = me->getVictim())
                    DoCast(victim, SPELL_GROUNDTREMOR, false);

                GroundTremor_Timer = 8000;
            }
            else
                GroundTremor_Timer -= diff;

            //Frenzy_Timer
            if (HealthBelowPct(51))
            {
                if (Frenzy_Timer <= diff)
                {
                    DoCast(me, SPELL_FRENZY, false);
                    Talk(0);

                    Frenzy_Timer = 15000;
                }
                else
                    Frenzy_Timer -= diff;
            }

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_grizzle()
{
    new boss_grizzle();
}
