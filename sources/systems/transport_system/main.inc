/*

    @---------------------
    @ This module file, is responsible for updating the transport, and parameters such as: gasoline, speed, etc.
    @ Developed by the developer: dev.r1nex
    @----------------------

*/
#include <YSI/YSI_Coding/y_hooks>

/* 
    CallBacks 
*/
public: Server_OnVehicleUpdate() {
    new 
        engine = 0,
        lights = 0,
        alarm = 0,
        doors = 0,
        bonnet = 0,
        boot = 0,
        objective = 0
    ;

    foreach (new i: Player) {
        if (IsPlayerInAnyVehicle(i) && GetPlayerState(i) == PLAYER_STATE_DRIVER) {
            new vehicle_id = GetPlayerVehicleID(i);
            GetVehicleParamsEx(vehicle_id, engine, lights, alarm, doors, bonnet, boot, objective);

            if (engine) {
                if (!(vehicle_fuel[vehicle_id] <= 0.0)) {
                    vehicle_fuel[vehicle_id] -= 1.0;
                    return 1;
                }

                SendClientMessage(i, -1, ""cGREY"В этом транспорте нету топлива. Его необходимо заправить.");
                SetVehicleParamsEx(vehicle_id, false, lights, alarm, doors, bonnet, boot, objective);
            }
        }
    }

    return 1;
}

/*
    Timers
*/
public: OnStartVehicle(playerid, vehicle_id) {
    new 
        engine = 0,
        lights = 0,
        alarm = 0,
        doors = 0,
        bonnet = 0,
        boot = 0,
        objective = 0
    ;

    GetVehicleParamsEx(vehicle_id, engine, lights, alarm, doors, bonnet, boot, objective);

    if (!engine) {
        SetVehicleParamsEx(vehicle_id, true, lights, alarm, doors, bonnet, boot, objective);
        return SendClientMessage(playerid, -1, ""cGREY"Вы завели двигатель.");
    }

    SetVehicleParamsEx(vehicle_id, false, lights, alarm, doors, bonnet, boot, objective);
    return SendClientMessage(playerid, -1, ""cGREY"Вы заглушили двигатель.");
}

/*
    Hooks
*/
hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
    if (newkeys & KEY_FIRE && IsPlayerInAnyVehicle(playerid) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER) {
        new 
            vehicle_id = GetPlayerVehicleID(playerid),
            engine = 0,
            lights = 0,
            alarm = 0,
            doors = 0,
            bonnet = 0,
            boot = 0,
            objective = 0
        ;

        GetVehicleParamsEx(vehicle_id, engine, lights, alarm, doors, bonnet, boot, objective);
        SetTimerEx("OnStartVehicle", 1500, false, "dd", playerid, vehicle_id);

        PlayerTextDrawSetString(playerid, ptd_greenzone[playerid], engine ? "~r~ГЛУШИМ ДВИГАТЕЛЬ..." : "~g~ЗАВОДИМ ДВИГАТЕЛЬ...");
        PlayerTextDrawFont(playerid, ptd_greenzone[playerid], 1);
        PlayerTextDrawSetShadow(playerid, ptd_greenzone[playerid], 0);
        ShowBrightnessTextDraw(playerid);
        
        return 1;
    }

    if (newkeys & KEY_CTRL_BACK && IsPlayerInAnyVehicle(playerid)) {
        new 
            vehicle_id = GetPlayerVehicleID(playerid),
            engine = 0,
            lights = 0,
            alarm = 0,
            doors = 0,
            bonnet = 0,
            boot = 0,
            objective = 0
        ;

        GetVehicleParamsEx(vehicle_id, engine, lights, alarm, doors, bonnet, boot, objective);

        if (lights) {
            return SetVehicleParamsEx(vehicle_id, engine, false, alarm, doors, bonnet, boot, objective);
        }

        SetVehicleParamsEx(vehicle_id, engine, true, alarm, doors, bonnet, boot, objective);
    }

    return 1;
}
