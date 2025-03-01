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
    { "1 �������", ""cGREY"/gethere", ""cGREY"��������������� ������ � ����", ""cGREY"playerid" }, // 0
    { "1 �������", ""cGREY"/goto", ""cGREY"����������������� � ������", ""cGREY"playerid" }, // 1
    { "1 �������", ""cGREY"/a", ""cGREY"��������� ��������� � �����-���", ""cGREY"message" }, // 2
    { "1 �������", ""cGREY"/alogin", ""cGREY"�������������� � ������ ��������������", ""cGREY"null" }, // 3
    { "1 �������", ""cGREY"/reports", ""cGREY"������ ����� �� �������", ""cGREY"null" }, // 4
    { "1 �������", ""cGREY"/set_virtualworld", ""cGREY"���������� ��� ���� ����������� ���", ""cGREY"id" }, // 5
    { "1 �������", ""cGREY"/set_interior", ""cGREY"���������� ��� ���� ��������", ""cGREY"id" }, // 6
    { "1 �������", ""cGREY"/tppos", ""cGREY"������������ �� ����������� ����������", ""cGREY"x; y; z" }, // 7
    { "1 �������", ""cGREY"/veh", ""cGREY"���������� ���������", ""cGREY"modelid; color1; color2" }, // 8
    { "1 �������", ""cGREY"/set_satiety", ""cGREY"���������� ������� ������", ""cGREY"satiety" }, // 9
    { "6 �������", ""cGREY"/a_warn", ""cGREY"������ �������������� ������-���� ��������������", ""cGREY"playerid; count(1-3)" }, // 10
    { "6 �������", ""cGREY"/a_unwarn", ""cGREY"����� �������������� ������-���� ��������������", ""cGREY"playerid; count(1-3)" }, // 11
    { "1 �������", ""cGREY"/ahelp", ""cGREY"������� ��������������", ""cGREY"null" }, // 12
    { "1 �������", ""cGREY"/gotocar", ""cGREY"����������������� � �/�", ""cGREY"vehicleid" }, // 13
    { "1 �������", ""cGREY"/getherecar", ""cGREY"��������������� �/� � ����", ""cGREY"vehicleid" }, // 14
    { "1 �������", ""cGREY"/fv", ""cGREY"�������� �/�", ""cGREY"vehicleid" }, // 15
    { "6 �������", ""cGREY"/addhouse", ""cGREY"�������� ��� �� ������.", ""cGREY"null" }, // 16
    { "1 �������", ""cGREY"/unmute", ""cGREY"�������������� ��� ������", ""cGREY"playerid" }, // 17
    { "1 �������", ""cGREY"/mute", ""cGREY"������������� ��� ������", ""cGREY"playerid; minutes; reason" }, // 18
    { "1 �������", ""cGREY"/stats", ""cGREY"���������� ���������� ������", ""cGREY"playerid" }, // 19
    { "1 �������", ""cGREY"/offstats", ""cGREY"���������� ���������� ������ � ��������", ""cGREY"Name_Surname" }, // 20
    { "1 �������", ""cGREY"/admins", ""cGREY"������ ������������� � �������", ""cGREY"null" }, // 21
    { "1 �������", ""cGREY"/sethp", ""cGREY"���������� ������� �� ������", ""cGREY"playerid; 1-100" }, // 22
    { "1 �������", ""cGREY"/set_armour", ""cGREY"���������� ������� ����� ������", ""cGREY"playerid; 1-100" } // 23
};
new Iterator: iAdmins<MAX_PLAYERS>;
new Iterator: iReport<MAX_REPORTS>;
// new Iterator: iAdminCommands<MAX_ADMIN_PASSWORD>;
new g_player_rep_page[MAX_PLAYERS];
new g_player_admin_commands_page[MAX_PLAYERS];
new g_player_admins_page[MAX_PLAYERS];
new g_player_rep_idx[MAX_PLAYERS] = {-1, ...};
