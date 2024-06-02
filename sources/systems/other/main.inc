/*

    @---------------------
    @ This is an executable module that works with different game mod requirements. Such as: pickups, 3D texts, actors and so on.
    @ Developed by the developer: dev.r1nex
    @----------------------

*/

#include <YSI/YSI_Coding/y_hooks>

/*
    Structures
*/
enum E_ACTORS {
    E_ACTOR,
    E_NAME[120],
    E_SKIN,
    Float: E_POSX,
    Float: E_POSY,
    Float: E_POSZ,
    Float: E_POSA
};

enum E_PICKUPS {
    E_PICKUP,
    E_MODEL,
    E_WORLDID,
    E_INTERIORID,
    E_NAME[153],
    Float: E_POSX,
    Float: E_POSY,
    Float: E_POSZ
};

/*
    Variables
*/
new g_server_actors[][E_ACTORS] = {
    { 0, "NPC {D8A903}Francesco Schettino\n\n{ffffff}Нажмите {33aa33}ALT{ffffff} для разговора.", 8, 1677.9977, -2327.2239, 13.5469, 230.1559 }
};
new g_server_pickups[][E_PICKUPS] = {
    { 0, 19902, 0, 0, ""cWHITE"Вход в больницу штата "cBLUE"San-Andreas\n\n"cWHITE"Встаньте на пикап", 1172.5044, -1323.3456, 15.4030 },
    { 1, 1237, 0, 0, "Вы можете воспользоваться нашим бесплатным сервисом, и\n\nарендовать велосипед бесплатно, \n\nдля этого пропишите команду "cBLUE"/bike.", 1666.8091, -2312.4668, 12.6888 },
    { 2, 19902, 0, 1, "Выход из больницы штата "cBLUE"San-Andreas\n\n"cWHITE"Встаньте на пикап", 1155.5032, -1326.2894, 3001.0400 },
    { 3, 1237, 0, 0, "Вы можете воспользоваться нашим бесплатным сервисом, и\n\nарендовать велосипед бесплатно, \n\nдля этого пропишите команду "cBLUE"/bike.", 1456.2039, -1743.0247, 12.5333 },
    { 4, 1237, 0, 0, "Вы можете воспользоваться нашим бесплатным сервисом, и\n\nарендовать велосипед бесплатно, \n\nдля этого пропишите команду "cBLUE"/bike.", 1185.3025, -1316.8020, 12.5333 }
};


/*
    Hooks
*/
hook OnGameModeInit() {
    LoadActors();
    LoadPickups();
    return 1;
}

/*
    Functions
*/
stock LoadActors() {
    for (new i; i < sizeof g_server_actors; i++) {
        g_server_actors[i][E_ACTOR] = CreateActor(g_server_actors[i][E_SKIN], g_server_actors[i][E_POSX], g_server_actors[i][E_POSY], g_server_actors[i][E_POSZ], g_server_actors[i][E_POSA]);
        CreateDynamic3DTextLabel(g_server_actors[i][E_NAME], -1, g_server_actors[i][E_POSX], g_server_actors[i][E_POSY], g_server_actors[i][E_POSZ] + 1.25, 15.0);
    }
}

stock LoadPickups() {
    for (new i; i < sizeof g_server_pickups; i++) {
        g_server_pickups[i][E_PICKUP] = CreateDynamicPickup(g_server_pickups[i][E_MODEL], 
            1, 
            g_server_pickups[i][E_POSX], 
            g_server_pickups[i][E_POSY], 
            g_server_pickups[i][E_POSZ], 
            g_server_pickups[i][E_WORLDID], 
            g_server_pickups[i][E_INTERIORID]
        );
        CreateDynamic3DTextLabel(g_server_pickups[i][E_NAME], -1, g_server_pickups[i][E_POSX], g_server_pickups[i][E_POSY], g_server_pickups[i][E_POSZ] + 1.75, 15.0);
    }
}