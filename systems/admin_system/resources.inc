/* 
    Macroses 
*/
#define Admins:%0(%1) Admins__%0(%1)
#define GetAdminData(%0,%1) admin_data[%0][%1]
#define SetAdminData(%0,%1,%2) admin_data[%0][%1] = %2
#define GetReportData(%0,%1) report_data[%0][%1]
#define SetReportData(%0,%1,%2) report_data[%0][%1] = %2
#define MAX_TEXT_REPORT (64)
#define MAX_REPORTS (300)
#define MAX_ADMIN_PASSWORD (32)
#define MAX_ADMIN_COMMANDS (24)

/*
    Structures
*/
enum e_ADMIN_DATA {
    a_id,
    a_level,
    a_accountId,
    a_code[MAX_ADMIN_PASSWORD],
    a_warns,
    bool: a_autoAuth,
    bool: a_isAuth,
    Float: a_ExitX,
    Float: a_ExitY,
    Float: a_ExitZ
};

enum e_ADMIN_COMMAND {
    ac_admin_level[16],
    ac_command[32],
    ac_command_description[128],
    ac_command_arguments[64]
};

enum e_REPORT_DATA {
    r_sender,
    r_text[MAX_TEXT_REPORT]
};

/*
    Variables
*/
new report_data[MAX_REPORTS][e_REPORT_DATA];
new admin_data[MAX_PLAYERS][e_ADMIN_DATA];
new const admin_data_clear[e_ADMIN_DATA] = {
    0,
    0,
    0,
    EOS,
    0,
    false,
    false,
    0.0,
    0.0,
    0.0
};
new const report_data_clear[e_REPORT_DATA] = {
    0,
    EOS
};
new const g_admin_commands[MAX_ADMIN_COMMANDS][e_ADMIN_COMMAND] = {
    { "1 Уровень", ""cGREY"/gethere", ""cGREY"Телепортировать игрока к себе", ""cGREY"playerid" }, // 0
    { "1 Уровень", ""cGREY"/goto", ""cGREY"Телепортироваться к игроку", ""cGREY"playerid" }, // 1
    { "1 Уровень", ""cGREY"/a", ""cGREY"Отправить сообщение в админ-чат", ""cGREY"message" }, // 2
    { "1 Уровень", ""cGREY"/alogin", ""cGREY"Авторизоваться в панели администратора", ""cGREY"null" }, // 3
    { "1 Уровень", ""cGREY"/reports", ""cGREY"Список жалоб от игроков", ""cGREY"null" }, // 4
    { "1 Уровень", ""cGREY"/set_virtualworld", ""cGREY"Установить для себя виртуальный мир", ""cGREY"id" }, // 5
    { "1 Уровень", ""cGREY"/set_interior", ""cGREY"Установить для себя интерьер", ""cGREY"id" }, // 6
    { "1 Уровень", ""cGREY"/tppos", ""cGREY"Телепортация на определённые координаты", ""cGREY"x; y; z" }, // 7
    { "1 Уровень", ""cGREY"/veh", ""cGREY"Заспавнить транспорт", ""cGREY"modelid; color1; color2" }, // 8
    { "1 Уровень", ""cGREY"/set_satiety", ""cGREY"Установить сытость игроку", ""cGREY"satiety" }, // 9
    { "6 Уровень", ""cGREY"/a_warn", ""cGREY"Выдать предупреждение какому-либо администратору", ""cGREY"playerid; count(1-3)" }, // 10
    { "6 Уровень", ""cGREY"/a_unwarn", ""cGREY"Снять предупреждение какому-либо администратору", ""cGREY"playerid; count(1-3)" }, // 11
    { "1 Уровень", ""cGREY"/ahelp", ""cGREY"Команды администратора", ""cGREY"null" }, // 12
    { "1 Уровень", ""cGREY"/gotocar", ""cGREY"Телепортироваться к т/с", ""cGREY"vehicleid" }, // 13
    { "1 Уровень", ""cGREY"/getherecar", ""cGREY"Телепортировать т/с к себе", ""cGREY"vehicleid" }, // 14
    { "1 Уровень", ""cGREY"/fv", ""cGREY"Починить т/с", ""cGREY"vehicleid" }, // 15
    { "6 Уровень", ""cGREY"/addhouse", ""cGREY"Добавить дом на сервер.", ""cGREY"null" }, // 16
    { "1 Уровень", ""cGREY"/unmute", ""cGREY"Разблокировать чат игроку", ""cGREY"playerid" }, // 17
    { "1 Уровень", ""cGREY"/mute", ""cGREY"Заблокировать чат игроку", ""cGREY"playerid; minutes; reason" }, // 18
    { "1 Уровень", ""cGREY"/stats", ""cGREY"Посмотреть статистику игрока", ""cGREY"playerid" }, // 19
    { "1 Уровень", ""cGREY"/offstats", ""cGREY"Посмотреть статистику игрока в оффлайне", ""cGREY"Name_Surname" }, // 20
    { "1 Уровень", ""cGREY"/admins", ""cGREY"Список администрации в онлайне", ""cGREY"null" }, // 21
    { "1 Уровень", ""cGREY"/sethp", ""cGREY"Установить уровень ХП игроку", ""cGREY"playerid; 1-100" }, // 22
    { "1 Уровень", ""cGREY"/set_armour", ""cGREY"Установить уровень брони игроку", ""cGREY"playerid; 1-100" } // 23
};
new Iterator: iAdmins<MAX_PLAYERS>;
new Iterator: iReport<MAX_REPORTS>;
// new Iterator: iAdminCommands<MAX_ADMIN_PASSWORD>;
new g_player_rep_page[MAX_PLAYERS];
new g_player_admin_commands_page[MAX_PLAYERS];
new g_player_admins_page[MAX_PLAYERS];
new g_player_rep_idx[MAX_PLAYERS] = {-1, ...};
