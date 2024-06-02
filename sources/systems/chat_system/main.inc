/*

    @---------------------
    @ This is an executable module that creates a green space system. (No fighting, no shooting zones).
    @ Developed by the developer: dev.r1nex
    @----------------------

*/

#include <YSI/YSI_Coding/y_hooks>

/*
    Stocks
*/
stock ProxDetector(playerid, Float:max_range, color, const string[], Float:max_ratio = 1.6) {
    new
        Float:pos_x,
        Float:pos_y,
        Float:pos_z,
        Float:range,
        Float:range_ratio,
        Float:range_with_ratio,
        clr_r, clr_g, clr_b,
        Float:color_r, Float:color_g, Float:color_b;

    if (!GetPlayerPos(playerid, pos_x, pos_y, pos_z)) {
        return 0;
    }

    color_r = float(color >> 24 & 0xFF);
    color_g = float(color >> 16 & 0xFF);
    color_b = float(color >> 8 & 0xFF);
    range_with_ratio = max_range * max_ratio;

#if defined foreach
    foreach (new i : Player) {
#else
    for (new i = GetPlayerPoolSize(); i != -1; i--) {
#endif
        if (!IsPlayerStreamedIn(playerid, i)) {
            continue;
        }

        range = GetPlayerDistanceFromPoint(i, pos_x, pos_y, pos_z);
        if (range > max_range) {
            continue;
        }

        range_ratio = (range_with_ratio - range) / range_with_ratio;
        clr_r = floatround(range_ratio * color_r);
        clr_g = floatround(range_ratio * color_g);
        clr_b = floatround(range_ratio * color_b);

        SendClientMessage(i, (color & 0xFF) | (clr_b << 8) | (clr_g << 16) | (clr_r << 24), string);
    }

    SendClientMessage(playerid, color, string);
    return 1;
}

/*
    Hooks
*/
hook OnPlayerText(playerid, text[]) {
    if (!GetUserData(playerid, u_auth)) {
        SendClientMessage(playerid, -1, MESSAGE_NO_AUTH);
        return 0;
    }
    
    new string[120 + (MAX_PLAYER_NAME) + 4 + 64] = EOS;
    if (GetUserData(playerid, u_gender)) {
        format(string, sizeof string, ""cGREEN"%s сказал: %s", user_data[playerid][u_name], text);
    } else {
        format(string, sizeof string, ""cGREEN"%s сказала: %s", user_data[playerid][u_name], text);
    }

    ProxDetector(playerid, 20.0, -1, string);
    SetPlayerChatBubble(playerid, text, -1, 15.0, 1000);
    return 0;
}