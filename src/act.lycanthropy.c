/* ************************************************************************
*   File: act.vampire.c                                   EmpireMUD 2.0b5 *
*  Usage: Code related to the Vampire ability and its commands            *
*                                                                         *
*  EmpireMUD code base by Paul Clarke, (C) 2000-2015                      *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  EmpireMUD based upon CircleMUD 3.0, bpl 17, by Jeremy Elson.           *
*  CircleMUD (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#include "conf.h"
#include "sysdep.h"

#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "interpreter.h"
#include "handler.h"
#include "db.h"
#include "skills.h"
#include "vnums.h"
#include "dg_scripts.h"

/**
* Contents:
*   Helpers
*   Commands
*/

 //////////////////////////////////////////////////////////////////////////////
//// HELPERS /////////////////////////////////////////////////////////////////


// returns TRUE if the character is a lycanthrope and has the ability; sends its own error
bool check_lycanthrope_ability(char_data *ch, any_vnum ability, int cost_pool, int cost_amount, int cooldown_type) {
	if (!IS_LYCANTHROPE(ch)) {
		send_config_msg(ch, "must_be_lycanthrope");
		return FALSE;
	}
	if (!can_use_ability(ch, ability, cost_pool, cost_amount, cooldown_type)) {
		return FALSE;
	}

	return TRUE;
}


/**
* char_data *ch becomes a lycanthrope
* bool lore if TRUE, also adds lore
*/
void make_lycanthrope(char_data *ch, bool lore) {
	void set_skill(char_data *ch, any_vnum skill, int level);

	if (!noskill_ok(ch, SKILL_LYCANTHROPE) {
		return;
	}

	if (!IS_NPC(ch)) {
		/* set BEFORE set as a vampire! */
		GET_APPARENT_AGE(ch) = GET_AGE(ch);

		if (get_skill_level(ch, SKILL_LYCANTHROPE) < 1) {
			gain_skill(ch, find_skill_by_vnum(SKILL_LYCANTHROPE), 1);
		}

		GET_BLOOD(ch) = 30;

		remove_lore(ch, LORE_START_LYCANTHROPE);
		remove_lore(ch, LORE_ALPHA_LYCANTHROPE);
		remove_lore(ch, LORE_PURIFY);
		if (lore) {
			add_lore(ch, LORE_START_LYCANTHROPE, "Sired");
		}

		SAVE_CHAR(ch);
	}
}


