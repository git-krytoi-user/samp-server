/*

    @---------------------
    @ This is an executable module that creates a bike rental system.
    @ Developed by the developer: dev.r1nex
    @----------------------

*/

#include <YSI/YSI_Coding/y_hooks>

/*
    Hooks
*/
hook OnPlayerDisconnect(playerid, reason) {
    KillTimer(player_timer_bike[playerid]);
    DestroyVehicle(player_bike[playerid]);
    player_bike[playerid] = INVALID_VEHICLE_ID;
    player_timer_bike[playerid] = INVALID_TIMER_ID;
    return 1;
}

hook OnPlayerExitVehicle(playerid, vehicleid) {
    if (vehicleid == player_bike[playerid]) {
        SendClientMessage(playerid, -1, ""cGREY"Вы покинули арендуемый Вами велосипед, он автоматически исчезнет через 30 секунд.");
        player_timer_bike[playerid] = SetTimerEx("OnPlayerDestroyBike", 1000 * 30, false, "d", playerid);
    }
    return 1;
}

hook OnPlayerEnterVehicle(playerid, vehicleid, ispassenger) {
    if (vehicleid == player_bike[playerid] && !ispassenger) {
        KillTimer(player_timer_bike[playerid]);
    }
    return 1;
}

/*
    Functions
*/
public: OnPlayerDestroyBike(playerid) {
    if (player_bike[playerid] == INVALID_VEHICLE_ID) {
        KillTimer(player_timer_bike[playerid]);
        return 1;
    }

    SendClientMessage(playerid, -1, ""cGREY"Арендованный Вами велосипед был эвакуирован.");
    DestroyVehicle(player_bike[playerid]);
    KillTimer(player_timer_bike[playerid]);

    player_timer_bike[playerid] = INVALID_TIMER_ID;
    player_bike[playerid] = INVALID_VEHICLE_ID;
    return 1;
}

/*
    Commands
*/
cmd:bike(playerid) {
    if (player_bike[playerid] != INVALID_VEHICLE_ID)
        return SendClientMessage(playerid, -1, ""cGREY"Вы уже арендовали велосипед. Используйте /unrentbike, чтобы прекратить аренду.");

    new
        Float: x = 0.0,
        Float: y = 0.0,
        Float: z = 0.0,
        Float: f_angle = 0.0
    ;

    if (IsPlayerInRangeOfPoint(playerid, 2.0, 1666.8091, -2312.4668, 12.5447) || IsPlayerInRangeOfPoint(playerid, 2.0, 1456.2039, -1743.0247, 13.5469) || IsPlayerInRangeOfPoint(playerid, 2.0, 1185.3025, -1316.8020, 13.5707)) {
        GetPlayerFacingAngle(playerid, f_angle);
        GetPlayerPos(playerid, x, y, z);
        player_bike[playerid] = CreateVehicle(510, x, y, z, f_angle, 1, 1, 60000);
    } else return SendClientMessage(playerid, -1, ""cGREY"Вы слишком далеко от точки аренды велосипедов.");
    return 1;
}

cmd:unrentbike(playerid) {
    if (player_bike[playerid] == INVALID_VEHICLE_ID)
        return SendClientMessage(playerid, -1, ""cGREY"Вы ещё не арендовали велосипед.");

    SendClientMessage(playerid, -1, ""cGREY"Вы отказались от аренды.");
    DestroyVehicle(player_bike[playerid]);

    return player_bike[playerid] = INVALID_VEHICLE_ID;
}