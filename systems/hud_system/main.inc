/*

    @---------------------
    @ This executable file is designed to run the hood system.
    @ Developed by the developer: dev.r1nex
    @----------------------

*/

#include <YSI/YSI_Coding/y_hooks>

/*
    Functions
*/
stock hud_CreateGlobalTextDraws() {
    gtd_hud[HUD_BACKGROUND][0] = TextDrawCreate(557.7906, 116.1630, "Box"); // fon - health
    TextDrawLetterSize(gtd_hud[HUD_BACKGROUND][0], 0.0000, 0.4333);
    TextDrawTextSize(gtd_hud[HUD_BACKGROUND][0], 610.7905, 0.0000);
    TextDrawAlignment(gtd_hud[HUD_BACKGROUND][0], 1);
    TextDrawColor(gtd_hud[HUD_BACKGROUND][0], -1);
    TextDrawUseBox(gtd_hud[HUD_BACKGROUND][0], 1);
    TextDrawBoxColor(gtd_hud[HUD_BACKGROUND][0], 255);
    TextDrawBackgroundColor(gtd_hud[HUD_BACKGROUND][0], 255);
    TextDrawFont(gtd_hud[HUD_BACKGROUND][0], 1);
    TextDrawSetProportional(gtd_hud[HUD_BACKGROUND][0], 1);
    TextDrawSetShadow(gtd_hud[HUD_BACKGROUND][0], 0);

    gtd_hud[HUD_BACKGROUND][1] = TextDrawCreate(557.9240, 125.5628, "Box"); // fon - armour
    TextDrawLetterSize(gtd_hud[HUD_BACKGROUND][1], 0.0000, 0.4333);
    TextDrawTextSize(gtd_hud[HUD_BACKGROUND][1], 610.8000, 0.0000);
    TextDrawAlignment(gtd_hud[HUD_BACKGROUND][1], 1);
    TextDrawColor(gtd_hud[HUD_BACKGROUND][1], -1);
    TextDrawUseBox(gtd_hud[HUD_BACKGROUND][1], 1);
    TextDrawBoxColor(gtd_hud[HUD_BACKGROUND][1], 255);
    TextDrawBackgroundColor(gtd_hud[HUD_BACKGROUND][1], 255);
    TextDrawFont(gtd_hud[HUD_BACKGROUND][1], 1);
    TextDrawSetProportional(gtd_hud[HUD_BACKGROUND][1], 1);
    TextDrawSetShadow(gtd_hud[HUD_BACKGROUND][1], 0);

    gtd_hud[HUD_BACKGROUND][2] = TextDrawCreate(557.9240, 135.2632, "Box"); // fon - satiety
    TextDrawLetterSize(gtd_hud[HUD_BACKGROUND][2], 0.0000, 0.4333);
    TextDrawTextSize(gtd_hud[HUD_BACKGROUND][2], 610.8000, 0.0000);
    TextDrawAlignment(gtd_hud[HUD_BACKGROUND][2], 1);
    TextDrawColor(gtd_hud[HUD_BACKGROUND][2], -1);
    TextDrawUseBox(gtd_hud[HUD_BACKGROUND][2], 1);
    TextDrawBoxColor(gtd_hud[HUD_BACKGROUND][2], 255);
    TextDrawBackgroundColor(gtd_hud[HUD_BACKGROUND][2], 255);
    TextDrawFont(gtd_hud[HUD_BACKGROUND][2], 1);
    TextDrawSetProportional(gtd_hud[HUD_BACKGROUND][2], 1);
    TextDrawSetShadow(gtd_hud[HUD_BACKGROUND][2], 0);

    gtd_hud[HUD_IMAGE][0] = TextDrawCreate(614.6328, 134.3704, "hud:radar_dateFood"); // image
    TextDrawTextSize(gtd_hud[HUD_IMAGE][0], 8.0000, 8.0000);
    TextDrawAlignment(gtd_hud[HUD_IMAGE][0], 1);
    TextDrawColor(gtd_hud[HUD_IMAGE][0], -1);
    TextDrawBackgroundColor(gtd_hud[HUD_IMAGE][0], 255);
    TextDrawFont(gtd_hud[HUD_IMAGE][0], 4);
    TextDrawSetProportional(gtd_hud[HUD_IMAGE][0], 0);
    TextDrawSetShadow(gtd_hud[HUD_IMAGE][0], 0);

    gtd_hud[HUD_IMAGE][1] = TextDrawCreate(614.6328, 114.0703, "hud:radar_hostpital"); // image
    TextDrawTextSize(gtd_hud[HUD_IMAGE][1], 8.0000, 8.0000);
    TextDrawAlignment(gtd_hud[HUD_IMAGE][1], 1);
    TextDrawColor(gtd_hud[HUD_IMAGE][1], -1);
    TextDrawBackgroundColor(gtd_hud[HUD_IMAGE][1], 255);
    TextDrawFont(gtd_hud[HUD_IMAGE][1], 4);
    TextDrawSetProportional(gtd_hud[HUD_IMAGE][1], 0);
    TextDrawSetShadow(gtd_hud[HUD_IMAGE][1], 0);

    gtd_hud[HUD_IMAGE][2] = TextDrawCreate(614.6328, 123.6701, "hud:radar_light"); // image
    TextDrawTextSize(gtd_hud[HUD_IMAGE][2], 8.0000, 8.0000);
    TextDrawAlignment(gtd_hud[HUD_IMAGE][2], 1);
    TextDrawColor(gtd_hud[HUD_IMAGE][2], -1);
    TextDrawBackgroundColor(gtd_hud[HUD_IMAGE][2], 255);
    TextDrawFont(gtd_hud[HUD_IMAGE][2], 4);
    TextDrawSetProportional(gtd_hud[HUD_IMAGE][2], 0);
    TextDrawSetShadow(gtd_hud[HUD_IMAGE][2], 0);

    return 1;
}

stock hud_CreatePlayerTextDraws(playerid) {
    ptd_hud[playerid][BAR_HEALTH] = CreatePlayerTextDraw(playerid, 558.4573, 117.3222, "Box"); // bar - health
    PlayerTextDrawLetterSize(playerid, ptd_hud[playerid][BAR_HEALTH], 0.0000, 0.2060);
    PlayerTextDrawTextSize(playerid, ptd_hud[playerid][BAR_HEALTH], 610.0000, 0.0000);
    PlayerTextDrawAlignment(playerid, ptd_hud[playerid][BAR_HEALTH], 1);
    PlayerTextDrawColor(playerid, ptd_hud[playerid][BAR_HEALTH], -1);
    PlayerTextDrawUseBox(playerid, ptd_hud[playerid][BAR_HEALTH], 1);
    PlayerTextDrawBoxColor(playerid, ptd_hud[playerid][BAR_HEALTH], -16776961);
    PlayerTextDrawBackgroundColor(playerid, ptd_hud[playerid][BAR_HEALTH], 255);
    PlayerTextDrawFont(playerid, ptd_hud[playerid][BAR_HEALTH], 1);
    PlayerTextDrawSetProportional(playerid, ptd_hud[playerid][BAR_HEALTH], 1);
    PlayerTextDrawSetShadow(playerid, ptd_hud[playerid][BAR_HEALTH], 0);

    ptd_hud[playerid][BAR_ARMOUR] = CreatePlayerTextDraw(playerid, 558.4573, 126.2220, "Box"); // bar - armour
    PlayerTextDrawLetterSize(playerid, ptd_hud[playerid][BAR_ARMOUR], 0.0000, 0.2680);
    PlayerTextDrawTextSize(playerid, ptd_hud[playerid][BAR_ARMOUR], 610.0000, 0.0000);
    PlayerTextDrawAlignment(playerid, ptd_hud[playerid][BAR_ARMOUR], 1);
    PlayerTextDrawColor(playerid, ptd_hud[playerid][BAR_ARMOUR], -1);
    PlayerTextDrawUseBox(playerid, ptd_hud[playerid][BAR_ARMOUR], 1);
    PlayerTextDrawBoxColor(playerid, ptd_hud[playerid][BAR_ARMOUR], -1061109505);
    PlayerTextDrawBackgroundColor(playerid, ptd_hud[playerid][BAR_ARMOUR], 255);
    PlayerTextDrawFont(playerid, ptd_hud[playerid][BAR_ARMOUR], 1);
    PlayerTextDrawSetProportional(playerid, ptd_hud[playerid][BAR_ARMOUR], 0);
    PlayerTextDrawSetShadow(playerid, ptd_hud[playerid][BAR_ARMOUR], 0);

    ptd_hud[playerid][BAR_SATIETY] = CreatePlayerTextDraw(playerid, 558.4573, 136.1225, "Box"); // bar - satiety
    PlayerTextDrawLetterSize(playerid, ptd_hud[playerid][BAR_SATIETY], 0.0000, 0.2680);
    PlayerTextDrawTextSize(playerid, ptd_hud[playerid][BAR_SATIETY], 610.0000, 0.0000);
    PlayerTextDrawAlignment(playerid, ptd_hud[playerid][BAR_SATIETY], 1);
    PlayerTextDrawColor(playerid, ptd_hud[playerid][BAR_SATIETY], -1);
    PlayerTextDrawUseBox(playerid, ptd_hud[playerid][BAR_SATIETY], 1);
    PlayerTextDrawBoxColor(playerid, ptd_hud[playerid][BAR_SATIETY], -65281);
    PlayerTextDrawBackgroundColor(playerid, ptd_hud[playerid][BAR_SATIETY], 255);
    PlayerTextDrawFont(playerid, ptd_hud[playerid][BAR_SATIETY], 1);
    PlayerTextDrawSetProportional(playerid, ptd_hud[playerid][BAR_SATIETY], 0);
    PlayerTextDrawSetShadow(playerid, ptd_hud[playerid][BAR_SATIETY], 0);

    return 1;
}

stock UpdatePlayerBar(playerid, bar_type) {
    switch (bar_type) {
        case 0: { // BAR_HEALTH
            new Float: health;
            GetPlayerHealth(playerid, health);
            
            new Float: percent_of_total = (health / 100.0);
            new Float: bar_percent = percent_of_total * 100.0;
            new Float: total = (50.0 / 100.0) * bar_percent;
            
            PlayerTextDrawHide(playerid, ptd_hud[playerid][BAR_HEALTH]);
            PlayerTextDrawTextSize(playerid, ptd_hud[playerid][BAR_HEALTH], 560.1020 + total, 0.0000);
            PlayerTextDrawShow(playerid, ptd_hud[playerid][BAR_HEALTH]);

            return 1;
        }

        case 1: { // BAR_ARMOUR
            new Float: armour;
            GetPlayerArmour(playerid, armour);
            
            new Float: percent_of_total = (armour / 100.0);
            new Float: bar_percent = percent_of_total * 100.0;
            new Float: total = (50.0 / 100.0) * bar_percent;

            PlayerTextDrawHide(playerid, ptd_hud[playerid][BAR_ARMOUR]);
            PlayerTextDrawTextSize(playerid, ptd_hud[playerid][BAR_ARMOUR], 560.1020 + total, 0.0000);
            PlayerTextDrawShow(playerid, ptd_hud[playerid][BAR_ARMOUR]);

            return 1;
        }

        case 2: { // BAR_SATIETY
            new Float: satiety = GetUserData(playerid, u_satiety);

            new Float: percent_of_total = (satiety / 100.0);
            new Float: bar_percent = percent_of_total * 100.0;
            new Float: total = (50.0 / 100.0) * bar_percent;

            PlayerTextDrawHide(playerid, ptd_hud[playerid][BAR_SATIETY]);
            PlayerTextDrawTextSize(playerid, ptd_hud[playerid][BAR_SATIETY], 560.1020 + total, 0.0000);
            PlayerTextDrawShow(playerid, ptd_hud[playerid][BAR_SATIETY]);

            return 1;
        }

        default: printf("> Unknown type bar: ^%d", bar_type);
    }

    return 1;
}

/*
    Hooks
*/
hook OnGameModeInit() {
    hud_CreateGlobalTextDraws();
    return 1;
}

hook OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart) {
    return 1;
}

hook OnPlayerDisconnect(playerid, reason) {
    PlayerTextDrawDestroy(playerid, ptd_hud[playerid][BAR_HEALTH]);
    PlayerTextDrawDestroy(playerid, ptd_hud[playerid][BAR_ARMOUR]);
    PlayerTextDrawDestroy(playerid, ptd_hud[playerid][BAR_SATIETY]);

    ptd_hud[playerid] = ptd_hud_null;
    return 1;
}

hook OnPlayerConnect(playerid) {
    for (new i; i < sizeof gtd_hud[HUD_BACKGROUND]; i++) {
        // printf("gtd_hud[HUD_BACKGROUND]: %d", i);
        TextDrawShowForPlayer(playerid, gtd_hud[HUD_BACKGROUND][i]);
    }

    for (new i; i < sizeof gtd_hud[HUD_IMAGE]; i++) {
        // printf("gtd_hud[HUD_IMAGE]: %d", i);
        TextDrawShowForPlayer(playerid, gtd_hud[HUD_IMAGE][i]);
    }

    hud_CreatePlayerTextDraws(playerid);

    PlayerTextDrawShow(playerid, ptd_hud[playerid][BAR_HEALTH]);
    PlayerTextDrawShow(playerid, ptd_hud[playerid][BAR_ARMOUR]);
    PlayerTextDrawShow(playerid, ptd_hud[playerid][BAR_SATIETY]);

    return 1;
}