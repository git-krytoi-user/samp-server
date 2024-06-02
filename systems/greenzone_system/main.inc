/*

    @---------------------
    @ This is an executable module that creates a chat system on the server.
    @ Developed by the developer: dev.r1nex
    @----------------------

*/

#include <YSI/YSI_Coding/y_hooks>

/*
    Hooks
*/
hook OnGameModeInit() {
    LoadGreenZones();
    return 1;
}

hook OnPlayerDisconnect(playerid, reason) {
    is_player_greenzone[playerid] = false;
    return 1;
}

hook OnPlayerEnterDynamicArea(playerid, areaid) {
    for (new i; i < sizeof g_server_greenzones; i++) {
        if (areaid == g_server_greenzones[i][__ID]) {
            is_player_greenzone[playerid] = true;
            
            SendClientMessage(playerid, -1, ""cYELLOW"Вы вошли в зелёную зону. Здесь Вы не можете драться, и стрелять из оружия!");
            PlayerTextDrawSetString(playerid, ptd_greenzone[playerid], "~r~GREEN-ZONE");
            PlayerTextDrawFont(playerid, ptd_greenzone[playerid], 1);
            ShowBrightnessTextDraw(playerid);
        }
    }
    
    return 1;
}

hook OnPlayerLeaveDynamicArea(playerid, areaid) {
    for (new i; i < sizeof g_server_greenzones; i++) {
        if (areaid == g_server_greenzones[i][__ID]) {
            is_player_greenzone[playerid] = false;
            
            SendClientMessage(playerid, -1, ""cYELLOW"Вы вышли из зелёной зоны. Теперь Вы можете драться, и стрелять.");
            PlayerTextDrawSetString(playerid, ptd_greenzone[playerid], "~g~NO GREEN-ZONE");
            PlayerTextDrawFont(playerid, ptd_greenzone[playerid], 1);
            ShowBrightnessTextDraw(playerid);
        }
    }

    return 1;
}

hook OnPlayerConnect(playerid) {
    GreenZone_LoadPlayerTextDraw(playerid);
    return 1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
    if (is_player_greenzone[playerid] && newkeys & KEY_FIRE) {
        ApplyAnimation(playerid, "PED", "handscower", 4.1, 1, 0, 0, 0, 1000);
        ApplyAnimation(playerid, "PED", "handscower", 4.1, 1, 0, 0, 0, 1000);

        PlayerTextDrawSetString(playerid, ptd_greenzone[playerid], "~r~ВЫ НЕ МОЖЕТЕ АТАКОВАТЬ");
        PlayerTextDrawFont(playerid, ptd_greenzone[playerid], 1);
        PlayerTextDrawSetShadow(playerid, ptd_greenzone[playerid], 0);
        ShowBrightnessTextDraw(playerid);
    }
}

/*
    Functions
*/
public: ShowBrightnessTextDraw(playerid) {
    new color = GetUserData(playerid, u_brightness);

    PlayerTextDrawHide(playerid, ptd_greenzone[playerid]);
    PlayerTextDrawColor(playerid, ptd_greenzone[playerid], GreenZone_BrightnessColors[color][0]);
    PlayerTextDrawBackgroundColor(playerid, ptd_greenzone[playerid], GreenZone_BrightnessColors[color][1]);
    PlayerTextDrawShow(playerid, ptd_greenzone[playerid]);

    if (color < sizeof GreenZone_BrightnessColors - 1) {
        GetUserData(playerid, u_brightness) ++;
        SetTimerEx("ShowBrightnessTextDraw", 80, false, "d", playerid);
    } else {
        SetUserData(playerid, u_brightness, 0);
    }

    return 1;
}

stock LoadGreenZones() {
    for (new i; i < sizeof g_server_greenzones; i++) {
        g_server_greenzones[i][__ID] = CreateDynamicSphere(
            g_server_greenzones[i][__POSX], 
            g_server_greenzones[i][__POSY], 
            g_server_greenzones[i][__POSZ], 
            g_server_greenzones[i][__RADIUS], 
            g_server_greenzones[i][__WORLDID], 
            g_server_greenzones[i][__INTERIORID], 
            g_server_greenzones[i][__PLAYERID]
        );
    }
}

stock GreenZone_LoadPlayerTextDraw(playerid) {
    ptd_greenzone[playerid] = CreatePlayerTextDraw(playerid, 635.000000, 340.000000, !"/");
	PlayerTextDrawLetterSize(playerid, ptd_greenzone[playerid], 0.700000, 1.900000);
	PlayerTextDrawTextSize(playerid, ptd_greenzone[playerid], 1280.000000, 1280.000000);
	PlayerTextDrawAlignment(playerid, ptd_greenzone[playerid], 3);
	PlayerTextDrawColor(playerid, ptd_greenzone[playerid], 8388863);
	PlayerTextDrawUseBox(playerid, ptd_greenzone[playerid], 0);
	PlayerTextDrawBoxColor(playerid, ptd_greenzone[playerid], 8388863);
	PlayerTextDrawSetShadow(playerid, ptd_greenzone[playerid], 2);
	PlayerTextDrawSetOutline(playerid, ptd_greenzone[playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, ptd_greenzone[playerid], 8388863);
	PlayerTextDrawFont(playerid, ptd_greenzone[playerid], 3);
	PlayerTextDrawSetProportional(playerid, ptd_greenzone[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, ptd_greenzone[playerid], 0);
}