/*
    Macroses
*/
#define MAX_PASSWORD_LEN                    (32)
#define MAX_REGDATE_LEN                     (20)
#define MAX_PAME_LEN                        (128)
#define Players:%0(%1)                      Players__%0(%1)
#define GetUserData(%0,%1)                  user_data[%0][%1]
#define SetUserData(%0,%1,%2)               user_data[%0][%1] = %2
#define DIALOG_MENU_STATS                   0
#define DIALOG_MENU_COMMUNICATION_ADMINS    1
#define DIALOG_MENU_ACCOUNT_GUARD           2
#define DIALOG_MENU_ACCOUNT_SETTINGS        3
#define DIALOG_MENU_SERVER_HELP             4
#define DIALOG_MENU_SERVER_GPS              5
#define DIALOG_MENU_HEALTH_STATUS           6
#define DIALOG_MENU_CHARACTER_SKILLS        7
#define DIALOG_MENU_DAILY_TASKS             8
#define DIALOG_MENU_PHYSICAL_ABILITY        9
#define DIALOG_MENU_DONATE                  10

/*
    Structures
*/
enum e_USER_DATA {
    u_id,
    u_name          [MAX_PLAYER_NAME],
    u_password      [MAX_PASSWORD_LEN],
    u_regdate       [MAX_REGDATE_LEN],
    u_pame          [MAX_PAME_LEN],
    u_referal       [MAX_PLAYER_NAME],
    Text3D:         pame_id,
    u_skin,
    u_money,
    u_exp,
    u_score,
    u_gender,
    u_brightness,
    Float: u_satiety,
    u_has_passport,
    u_mutetime,
    u_nationality,
    bool:           u_ismute,
    bool:           u_auth
};

/*
    Variables
*/
new user_data[MAX_PLAYERS][e_USER_DATA];
new save_password[MAX_PLAYERS];
new save_gender[MAX_PLAYERS];
new save_nationality[MAX_PLAYERS];
// new player_update_timer[MAX_PLAYERS] = {INVALID_TIMER_ID, ...};
new Bar: g_player_health_bar[MAX_PLAYERS] = {INVALID_BAR_ID, ...};
new Bar: g_player_armour_bar[MAX_PLAYERS] = {INVALID_BAR_ID, ...};
new Bar: g_player_satiety_bar[MAX_PLAYERS] = {INVALID_BAR_ID, ...};
new g_types_nationality[8][32] = {
    ""cWHITE"Русский",
    ""cWHITE"Японец",
    ""cWHITE"Украинец",
    ""cWHITE"Американец",
    ""cWHITE"Итальянец",
    ""cWHITE"Японец",
    ""cWHITE"Француз",
    ""cWHITE"Китаец"
};