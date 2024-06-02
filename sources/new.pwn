#pragma warning disable 239
#pragma warning disable 200

// -- Includes
#include <a_samp>
#include <chandlingsvr>
#include <YSI/YSI_Coding/y_hooks>
#include <YSI/YSI_Game/y_weapondata/y_weapondata_entry>
#include "Pawn.CMD"
#include <a_mysql>
#include <foreach>
#include <sscanf2>
#include <streamer>
#include <Pawn.RakNet>
#include <tr_custom-sync>
#include <mdialog>
#include <rustext>
#include <Pawn.Regex>
#include <progress>

main(){}

// -- Core

#include "sources/core/interiors.inc" // Server_Interiors
#include "sources/core/macroses.inc" // Server_Macroses
#include "sources/core/database.inc" // Server_Database
#include "sources/core/load_systems.inc" // Server_Systems
#include "sources/core/raknet.inc" // Server_RakNet
#include "sources/core/aliases_commands.inc" // Server_CommandAliases

public OnPlayerConnect(playerid) {
	for (new i; i < 50; i++) {
		SendClientMessage(playerid, -1, "");
	}
	
	new 
		string[] = ""cYELLOW"Добро пожаловать, %s. Надеемся, что Вы хорошо проведёте своё время у нас.",
		sizeof_string[sizeof string + (-2+MAX_PLAYER_NAME)],
		player_name[MAX_PLAYER_NAME]
	;
	GetPlayerName(playerid, player_name, MAX_PLAYER_NAME);

	format(sizeof_string, sizeof (sizeof_string), string, player_name);
	SendClientMessage(playerid, -1, sizeof_string);
	return 1;
}

public OnGameModeInit()
{
	SendRconCommand("gamemodetext "GAMEMODE_NAME"");
	SendRconCommand("hostname "HOSTNAME"");
	EnableStuntBonusForAll(0);
	DisableInteriorEnterExits();
	print("> Development Server Started - Version 1.0 (Build 1.0)");
	SetTimer("Server_PayDay", 2000 * 30, true);
	SetTimer("Server_OnVehicleUpdate", 3000 * 60, true);

	// -- Initialization custom models
    AddVehicleSyncModel(411, 12500);
	AddVehicleSyncModel(560, 12501);
	AddVehicleSyncModel(560, 12502);
	AddVehicleSyncModel(560, 12503);
	AddVehicleSyncModel(560, 12504);
	AddVehicleSyncModel(560, 12505);
	AddVehicleSyncModel(560, 12506);
	AddVehicleSyncModel(560, 12507);
	AddVehicleSyncModel(560, 12508);
	AddVehicleSyncModel(560, 12509);
	AddVehicleSyncModel(560, 12510);
	AddVehicleSyncModel(579, 12511);
	AddVehicleSyncModel(579, 12512);
	AddVehicleSyncModel(579, 12513);
	AddVehicleSyncModel(579, 12514);
	AddVehicleSyncModel(579, 12515);
	AddVehicleSyncModel(411, 12516);
	AddVehicleSyncModel(411, 12517);
	AddVehicleSyncModel(579, 12518);
	AddVehicleSyncModel(602, 12519);
	AddVehicleSyncModel(426, 12520);

	// -- Initializing administrator commands
	/*for (new i; i < sizeof g_admin_commands; i++) {
		Iter_Add(iAdminCommands, i);
	}*/

	return 1;
}

public OnGameModeExit() {
	mysql_close(dbHandle);
	return 1;
}

public OnPlayerSpawn(playerid) {
	// UpdatePlayerBar(playerid, 0);
	// UpdatePlayerBar(playerid, 1);
	UpdatePlayerBar(playerid, 2);

	return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate) {
	if (newstate == KEY_FIRE && oldstate == PLAYER_STATE_DRIVER) {
		// SendClientMessage(playerid, -1, "Вы вышли из машины");
	}

	if (newstate == PLAYER_STATE_DRIVER && oldstate == PLAYER_STATE_ONFOOT) {
		SendClientMessage(playerid, -1, ""cPUSH"Нажмите клавишу ALT, чтобы завести/заглушить двигатель.");
		SendClientMessage(playerid, -1, ""cPUSH"Нажмите клавишу 2, чтобы включить/выключить фары.");
	}

	return 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason) {
	SpawnPlayer(playerid);
	SetPlayerHealth(playerid, 15.0);
	SetPlayerPos(playerid, 1139.8145, -1316.5994, 3006.8508);
	SetPlayerInterior(playerid, g_server_interiors[0][__INTERIORID]);
    SetPlayerVirtualWorld(playerid, g_server_interiors[0][__WORLDID]);
	return 1;
}

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart) {
	return 1;
}

public OnPlayerPickUpDynamicPickup(playerid, pickupid) {
	if (pickupid == g_server_pickups[0][E_PICKUP]) { // больница
        SetPlayerInterior(playerid, g_server_interiors[0][__INTERIORID]);
        SetPlayerVirtualWorld(playerid, g_server_interiors[0][__WORLDID]);

        SetPlayerPos(playerid, g_server_interiors[0][__POSX], g_server_interiors[0][__POSY], g_server_interiors[0][__POSZ]);
		SetCameraBehindPlayer(playerid);
    }

	if (pickupid == g_server_pickups[2][E_PICKUP]) { // выход из больницы
		SetPlayerInterior(playerid, 0);
		SetPlayerVirtualWorld(playerid, 0);

		SetPlayerPos(playerid, 1177.4281, -1323.3574, 14.0702);
		SetCameraBehindPlayer(playerid);
	}

	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
	SetPlayerPos(playerid, 1958.3783, 1343.1572, 15.3746);
	SetPlayerCameraPos(playerid, 1958.3783, 1343.1572, 15.3746);
	SetPlayerCameraLookAt(playerid, 1958.3783, 1343.1572, 15.3746);
	return 1;
}

public OnPlayerCommandReceived(playerid, cmd[], params[], flags) {
	if (!GetUserData(playerid, u_auth)) {
		SendClientMessage(playerid, -1, MESSAGE_NO_AUTH);
		return 0;
	}

	return 1;
}

public OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ) {
    if (Admins_IsPlayerAdmin(playerid)) {
		new vehicleid = GetPlayerVehicleID(playerid);
		new player_state = GetPlayerState(playerid);
        if (vehicleid > 0 && player_state == PLAYER_STATE_DRIVER) {
            return SetVehiclePos(vehicleid, fX, fY, fZ);
        }
        
		SetPlayerPos(playerid, fX, fY, fZ);

        SendClientMessage(playerid, -1, ""cGREY"Вы были успешно телепортированы.");
	}
	return 1;
}

public OnPlayerEnterDynamicArea(playerid, areaid) {
	return 1;
}

public OnPlayerLeaveDynamicArea(playerid, areaid) {
	return 1;
}

public OnPlayerExitVehicle(playerid, vehicleid) {
	return 1;
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger) {
	return 1;
}
