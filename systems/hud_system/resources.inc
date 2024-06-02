/*
    Macroses
*/
#define MAX_HUD_BACKGROUND      (3)
#define MAX_HUD_IMAGE           (3)

/*
    Enums
*/
enum _:E_HUD_TD {
    Text: HUD_BACKGROUND[MAX_HUD_BACKGROUND],
    Text: HUD_IMAGE[MAX_HUD_IMAGE]
};

enum _:E_HUD_PTD {
    PlayerText: BAR_HEALTH,
    PlayerText: BAR_ARMOUR,
    PlayerText: BAR_SATIETY
};

/*
    Variables
*/
new Text: gtd_hud[E_HUD_TD] = {
    Text: -1,
    Text: -1
};

new PlayerText: ptd_hud_null[E_HUD_PTD] = {
    PlayerText: -1,
    PlayerText: -1,
    PlayerText: -1
};

new PlayerText: ptd_hud[MAX_PLAYERS][E_HUD_PTD];

