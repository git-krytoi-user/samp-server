#if defined _trade_system
    #endinput
#endif
#define _trade_system

#if !defined _INC_y_hooks
	#error "Please included <YSI> before include <trade_system>"
#endif

#if !defined _mdialog_included
	#error "Please included <mdialog> before include <trade_system>"
#endif

#if !defined foreach
    #error "Please included <foreach> before include <trade_system>
#endif

/*
    Macroses
*/
#if !defined MAX_TRADE_ITEMS
    #define MAX_TRADE_ITEMS (5)
#endif
#if !defined GetTradeInfo
    #define GetTradeInfo(%0,%1,%2) trade_data[%0][%1][%2]
#endif
#if !defined SetTradeInfo
    #define SetTradeInfo(%0,%1,%2,%3) trade_data[%0][%1][%2] = %3
#endif

/*
    Enums
*/
enum _:E_TD_TRADE {
    Text: TD_BG[6],
    Text: TD_TEXT[3],
    Text: TD_BUTTON_CONFIRM,
    Text: TD_BUTTON_TRADE
};

enum _:E_PTD_TRADE {
    PlayerText: PTD_TRADER_SLOTS[5],
    PlayerText: PTD_RECEIVER_SLOTS[5]
};

enum E_PLAYER_TRADE_INFO {
    ITEM_ID,
    ITEM_AMOUNT,
    PLAYER_ID
};

enum E_TRADE_ARRAY {
    TARGET_ID
};

/*
    Variables
*/
new PlayerText: ptd_trade[MAX_PLAYERS][E_PTD_TRADE];
new PlayerText: ptd_trade_null[E_PTD_TRADE] = {
    PlayerText: -1,
    PlayerText: -1
};
new Text: td_trade[E_TD_TRADE] = {
    Text: -1, 
    Text: -1,
    Text: -1, 
    Text: -1  
};
new bool: is_player_trader[MAX_PLAYERS char] = {false, ...};
new bool: is_trade[MAX_PLAYERS char] = {false, ...};
new bool: is_trade_confirm[MAX_PLAYERS char] = {false, ...};
new trade_data[MAX_PLAYERS char][MAX_TRADE_ITEMS][E_PLAYER_TRADE_INFO];
new trade_array_data[MAX_PLAYERS char][E_TRADE_ARRAY];

/*
    Stocks
*/
stock bool: IsPlayerTrade(playerid) {
    return is_trade[playerid];
}

stock bool: IsPlayersTradeConfirm(playerid, targetid) {
    if (is_trade_confirm[playerid] && is_trade_confirm[targetid]) return true;
    return false;
}
stock SetPlayerTradeConfirm(playerid, bool: value = false) {
    is_trade_confirm[playerid] = value;
    return true;
}

stock bool: IsPlayerTrader(playerid) {
    return is_player_trader[playerid];
}

stock SetPlayerTrade(playerid, bool: value = false) {
    is_trade[playerid] = value;
    return true;
}

stock ClearTradeData(playerid) {
    for (new i; i < MAX_TRADE_ITEMS; i++) {
        new item_id = GetTradeInfo(playerid, i, ITEM_ID);
        if (!item_id) continue;

        SetTradeInfo(playerid, i, ITEM_ID, 0);
        SetTradeInfo(playerid, i, ITEM_AMOUNT, 0);
        SetTradeInfo(playerid, i, PLAYER_ID, 0);
    }

    return true;
}

/*
    Create TextDraws
*/
stock Trade_CreateGlobalTextDraw() {
    td_trade[TD_BG][0] = TextDrawCreate(418.1661, 132.7554, "Box"); // пусто
    TextDrawLetterSize(td_trade[TD_BG][0], 0.0000, 15.0999);
    TextDrawTextSize(td_trade[TD_BG][0], 617.2072, 0.0000);
    TextDrawAlignment(td_trade[TD_BG][0], 1);
    TextDrawColor(td_trade[TD_BG][0], -1);
    TextDrawUseBox(td_trade[TD_BG][0], 1);
    TextDrawBoxColor(td_trade[TD_BG][0], 150);
    TextDrawBackgroundColor(td_trade[TD_BG][0], 255);
    TextDrawFont(td_trade[TD_BG][0], 1);
    TextDrawSetProportional(td_trade[TD_BG][0], 1);
    TextDrawSetShadow(td_trade[TD_BG][0], 0);

    td_trade[TD_BG][1] = TextDrawCreate(418.2665, 118.2369, "Box"); // пусто
    TextDrawLetterSize(td_trade[TD_BG][1], 0.0000, 1.0664);
    TextDrawTextSize(td_trade[TD_BG][1], 477.5999, 0.0000);
    TextDrawAlignment(td_trade[TD_BG][1], 1);
    TextDrawColor(td_trade[TD_BG][1], -1);
    TextDrawUseBox(td_trade[TD_BG][1], 1);
    TextDrawBoxColor(td_trade[TD_BG][1], 150);
    TextDrawBackgroundColor(td_trade[TD_BG][1], 255);
    TextDrawFont(td_trade[TD_BG][1], 1);
    TextDrawSetProportional(td_trade[TD_BG][1], 1);
    TextDrawSetShadow(td_trade[TD_BG][1], 0);

    td_trade[TD_TEXT][0] = TextDrawCreate(418.8659, 117.2369, "ПРЕДМЕТЫ ДРУГОГО ИГРОКА"); // пусто
    TextDrawLetterSize(td_trade[TD_TEXT][0], 0.1254, 1.2015);
    TextDrawAlignment(td_trade[TD_TEXT][0], 1);
    TextDrawColor(td_trade[TD_TEXT][0], -1378294017);
    TextDrawSetOutline(td_trade[TD_TEXT][0], 1);
    TextDrawBackgroundColor(td_trade[TD_TEXT][0], 255);
    TextDrawFont(td_trade[TD_TEXT][0], 1);
    TextDrawSetProportional(td_trade[TD_TEXT][0], 1);
    TextDrawSetShadow(td_trade[TD_TEXT][0], 0);

    td_trade[TD_BG][2] = TextDrawCreate(418.0664, 273.8741, "Box"); // пусто
    TextDrawLetterSize(td_trade[TD_BG][2], 0.0000, 1.0175);
    TextDrawTextSize(td_trade[TD_BG][2], 458.4002, 0.0000);
    TextDrawAlignment(td_trade[TD_BG][2], 1);
    TextDrawColor(td_trade[TD_BG][2], -1);
    TextDrawUseBox(td_trade[TD_BG][2], 1);
    TextDrawBoxColor(td_trade[TD_BG][2], 150);
    TextDrawBackgroundColor(td_trade[TD_BG][2], 255);
    TextDrawFont(td_trade[TD_BG][2], 1);
    TextDrawSetProportional(td_trade[TD_BG][2], 1);
    TextDrawSetShadow(td_trade[TD_BG][2], 0);

    td_trade[TD_TEXT][1] = TextDrawCreate(417.9999, 272.9629, "ВАШИ ПРЕДМЕТЫ"); // пусто
    TextDrawLetterSize(td_trade[TD_TEXT][1], 0.1476, 1.0939);
    TextDrawAlignment(td_trade[TD_TEXT][1], 1);
    TextDrawColor(td_trade[TD_TEXT][1], -1);
    TextDrawSetOutline(td_trade[TD_TEXT][1], 1);
    TextDrawBackgroundColor(td_trade[TD_TEXT][1], 255);
    TextDrawFont(td_trade[TD_TEXT][1], 1);
    TextDrawSetProportional(td_trade[TD_TEXT][1], 1);
    TextDrawSetShadow(td_trade[TD_TEXT][1], 0);

    td_trade[TD_BG][3] = TextDrawCreate(579.8308, 272.9630, "Box"); // пусто
    TextDrawLetterSize(td_trade[TD_BG][3], 0.0000, 0.9666);
    TextDrawTextSize(td_trade[TD_BG][3], 617.2971, 0.0000);
    TextDrawAlignment(td_trade[TD_BG][3], 1);
    TextDrawColor(td_trade[TD_BG][3], 8388863);
    TextDrawUseBox(td_trade[TD_BG][3], 1);
    TextDrawBoxColor(td_trade[TD_BG][3], 8388708);
    TextDrawBackgroundColor(td_trade[TD_BG][3], 255);
    TextDrawFont(td_trade[TD_BG][3], 1);
    TextDrawSetProportional(td_trade[TD_BG][3], 1);
    TextDrawSetShadow(td_trade[TD_BG][3], 0);

    td_trade[TD_BUTTON_TRADE] = TextDrawCreate(580.2990, 271.7185, "ОБМЕН"); // пусто
    TextDrawLetterSize(td_trade[TD_BUTTON_TRADE], 0.3388, 1.1641);
    TextDrawTextSize(td_trade[TD_BUTTON_TRADE], 617.0000, 10.0000);
    TextDrawAlignment(td_trade[TD_BUTTON_TRADE], 1);
    TextDrawColor(td_trade[TD_BUTTON_TRADE], -1);
    TextDrawUseBox(td_trade[TD_BUTTON_TRADE], 1);
    TextDrawBoxColor(td_trade[TD_BUTTON_TRADE], 0);
    TextDrawSetOutline(td_trade[TD_BUTTON_TRADE], 1);
    TextDrawBackgroundColor(td_trade[TD_BUTTON_TRADE], 255);
    TextDrawFont(td_trade[TD_BUTTON_TRADE], 1);
    TextDrawSetProportional(td_trade[TD_BUTTON_TRADE], 1);
    TextDrawSetShadow(td_trade[TD_BUTTON_TRADE], 0);
    TextDrawSetSelectable(td_trade[TD_BUTTON_TRADE], true);

    td_trade[TD_BG][4] = TextDrawCreate(527.3436, 272.9630, "Box"); // пусто
    TextDrawLetterSize(td_trade[TD_BG][4], 0.0000, 0.9793);
    TextDrawTextSize(td_trade[TD_BG][4], 573.7025, 0.0000);
    TextDrawAlignment(td_trade[TD_BG][4], 1);
    TextDrawColor(td_trade[TD_BG][4], 8388863);
    TextDrawUseBox(td_trade[TD_BG][4], 1);
    TextDrawBoxColor(td_trade[TD_BG][4], 8388708);
    TextDrawBackgroundColor(td_trade[TD_BG][4], 255);
    TextDrawFont(td_trade[TD_BG][4], 1);
    TextDrawSetProportional(td_trade[TD_BG][4], 1);
    TextDrawSetShadow(td_trade[TD_BG][4], 0);

    td_trade[TD_BUTTON_CONFIRM] = TextDrawCreate(528.0988, 272.5184, "ПОДТВЕРДИТЬ"); // пусто
    TextDrawLetterSize(td_trade[TD_BUTTON_CONFIRM], 0.1946, 0.9570);
    TextDrawTextSize(td_trade[TD_BUTTON_CONFIRM], 573.0000, 10.0000);
    TextDrawAlignment(td_trade[TD_BUTTON_CONFIRM], 1);
    TextDrawColor(td_trade[TD_BUTTON_CONFIRM], -1);
    TextDrawUseBox(td_trade[TD_BUTTON_CONFIRM], 1);
    TextDrawBoxColor(td_trade[TD_BUTTON_CONFIRM], 0);
    TextDrawSetOutline(td_trade[TD_BUTTON_CONFIRM], 1);
    TextDrawBackgroundColor(td_trade[TD_BUTTON_CONFIRM], 255);
    TextDrawFont(td_trade[TD_BUTTON_CONFIRM], 1);
    TextDrawSetProportional(td_trade[TD_BUTTON_CONFIRM], 1);
    TextDrawSetShadow(td_trade[TD_BUTTON_CONFIRM], 0);
    TextDrawSetSelectable(td_trade[TD_BUTTON_CONFIRM], true);

    td_trade[TD_BG][5] = TextDrawCreate(555.6334, 120.3107, "Box"); // пусто
    TextDrawLetterSize(td_trade[TD_BG][5], 0.0000, 0.8999);
    TextDrawTextSize(td_trade[TD_BG][5], 617.2999, 0.0000);
    TextDrawAlignment(td_trade[TD_BG][5], 1);
    TextDrawColor(td_trade[TD_BG][5], -1);
    TextDrawUseBox(td_trade[TD_BG][5], 1);
    TextDrawBoxColor(td_trade[TD_BG][5], 150);
    TextDrawBackgroundColor(td_trade[TD_BG][5], 255);
    TextDrawFont(td_trade[TD_BG][5], 1);
    TextDrawSetProportional(td_trade[TD_BG][5], 1);
    TextDrawSetShadow(td_trade[TD_BG][5], 0);

    td_trade[TD_TEXT][2] = TextDrawCreate(556.8670, 119.8962, "ОБМЕН ПРЕДМЕТОВ"); // пусто
    TextDrawLetterSize(td_trade[TD_TEXT][2], 0.1906, 0.8657);
    TextDrawAlignment(td_trade[TD_TEXT][2], 1);
    TextDrawColor(td_trade[TD_TEXT][2], -1);
    TextDrawSetOutline(td_trade[TD_TEXT][2], 1);
    TextDrawBackgroundColor(td_trade[TD_TEXT][2], 255);
    TextDrawFont(td_trade[TD_TEXT][2], 1);
    TextDrawSetProportional(td_trade[TD_TEXT][2], 1);
    TextDrawSetShadow(td_trade[TD_TEXT][2], 0);

    return true;
}

stock Trade_CreatePlayerTextDraw(playerid) {
    ptd_trade[playerid][PTD_RECEIVER_SLOTS][0] = CreatePlayerTextDraw(playerid, 417.9328, 132.6999, ""); // слот который относится к предметам другого игрока 0
    PlayerTextDrawTextSize(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][0], 38.0000, 41.0000);
    PlayerTextDrawAlignment(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][0], 1);
    PlayerTextDrawColor(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][0], -1);
    PlayerTextDrawBackgroundColor(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][0], 190);
    PlayerTextDrawFont(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][0], 5);
    PlayerTextDrawSetProportional(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][0], 0);
    PlayerTextDrawSetShadow(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][0], 0);
    PlayerTextDrawSetSelectable(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][0], true);
    PlayerTextDrawSetPreviewModel(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][0], 19374);
    PlayerTextDrawSetPreviewRot(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][0], 0.0000, 0.0000, 0.0000, 1.0000);

    ptd_trade[playerid][PTD_RECEIVER_SLOTS][1] = CreatePlayerTextDraw(playerid, 457.7329, 132.4851, ""); // слот который относится к предметам другого игрока 1
    PlayerTextDrawTextSize(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][1], 38.0000, 41.0000);
    PlayerTextDrawAlignment(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][1], 1);
    PlayerTextDrawColor(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][1], -1);
    PlayerTextDrawBackgroundColor(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][1], 190);
    PlayerTextDrawFont(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][1], 5);
    PlayerTextDrawSetProportional(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][1], 0);
    PlayerTextDrawSetShadow(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][1], 0);
    PlayerTextDrawSetSelectable(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][1], true);
    PlayerTextDrawSetPreviewModel(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][1], 19374);
    PlayerTextDrawSetPreviewRot(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][1], 0.0000, 0.0000, 0.0000, 1.0000);

    ptd_trade[playerid][PTD_RECEIVER_SLOTS][2] = CreatePlayerTextDraw(playerid, 497.8352, 132.5852, ""); // слот который относится к предметам другого игрока 2
    PlayerTextDrawTextSize(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][2], 38.0000, 41.0000);
    PlayerTextDrawAlignment(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][2], 1);
    PlayerTextDrawColor(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][2], -1);
    PlayerTextDrawBackgroundColor(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][2], 190);
    PlayerTextDrawFont(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][2], 5);
    PlayerTextDrawSetProportional(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][2], 0);
    PlayerTextDrawSetShadow(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][2], 0);
    PlayerTextDrawSetSelectable(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][2], true);
    PlayerTextDrawSetPreviewModel(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][2], 19374);
    PlayerTextDrawSetPreviewRot(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][2], 0.0000, 0.0000, 0.0000, 1.0000);

    ptd_trade[playerid][PTD_RECEIVER_SLOTS][3] = CreatePlayerTextDraw(playerid, 538.4357, 132.4851, ""); // слот который относится к предметам другого игрока 3 
    PlayerTextDrawTextSize(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][3], 38.0000, 41.0000);
    PlayerTextDrawAlignment(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][3], 1);
    PlayerTextDrawColor(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][3], -1);
    PlayerTextDrawBackgroundColor(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][3], 190);
    PlayerTextDrawFont(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][3], 5);
    PlayerTextDrawSetProportional(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][3], 0);
    PlayerTextDrawSetShadow(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][3], 0);
    PlayerTextDrawSetSelectable(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][3], true);
    PlayerTextDrawSetPreviewModel(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][3], 19374);
    PlayerTextDrawSetPreviewRot(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][3], 0.0000, 0.0000, 0.0000, 1.0000);

    ptd_trade[playerid][PTD_RECEIVER_SLOTS][4] = CreatePlayerTextDraw(playerid, 578.7692, 132.5852, ""); // слот который относится к предметам другого игрока 4
    PlayerTextDrawTextSize(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][4], 38.0000, 41.0000);
    PlayerTextDrawAlignment(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][4], 1);
    PlayerTextDrawColor(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][4], -1);
    PlayerTextDrawBackgroundColor(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][4], 190);
    PlayerTextDrawFont(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][4], 5);
    PlayerTextDrawSetProportional(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][4], 0);
    PlayerTextDrawSetShadow(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][4], 0);
    PlayerTextDrawSetSelectable(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][4], true);
    PlayerTextDrawSetPreviewModel(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][4], 19374);
    PlayerTextDrawSetPreviewRot(playerid, ptd_trade[playerid][PTD_RECEIVER_SLOTS][4], 0.0000, 0.0000, 0.0000, 1.0000);

    ptd_trade[playerid][PTD_TRADER_SLOTS][0] = CreatePlayerTextDraw(playerid, 417.8995, 227.7071, ""); // слот который относится к трейдеру предметов 0 
    PlayerTextDrawTextSize(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][0], 38.0000, 41.0000);
    PlayerTextDrawAlignment(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][0], 1);
    PlayerTextDrawColor(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][0], -1);
    PlayerTextDrawBackgroundColor(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][0], 190);
    PlayerTextDrawFont(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][0], 5);
    PlayerTextDrawSetProportional(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][0], 0);
    PlayerTextDrawSetShadow(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][0], 0);
    PlayerTextDrawSetSelectable(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][0], true);
    PlayerTextDrawSetPreviewModel(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][0], 19374);
    PlayerTextDrawSetPreviewRot(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][0], 0.0000, 0.0000, 0.0000, 1.0000);

    ptd_trade[playerid][PTD_TRADER_SLOTS][1] = CreatePlayerTextDraw(playerid, 457.9020, 227.7071, ""); // слот который относится к трейдеру предметов 1
    PlayerTextDrawTextSize(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][1], 38.0000, 41.0000);
    PlayerTextDrawAlignment(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][1], 1);
    PlayerTextDrawColor(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][1], -1);
    PlayerTextDrawBackgroundColor(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][1], 190);
    PlayerTextDrawFont(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][1], 5);
    PlayerTextDrawSetProportional(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][1], 0);
    PlayerTextDrawSetShadow(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][1], 0);
    PlayerTextDrawSetSelectable(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][1], true);
    PlayerTextDrawSetPreviewModel(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][1], 19374);
    PlayerTextDrawSetPreviewRot(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][1], 0.0000, 0.0000, 0.0000, 1.0000);

    ptd_trade[playerid][PTD_TRADER_SLOTS][2] = CreatePlayerTextDraw(playerid, 498.0687, 227.6071, ""); // слот который относится к трейдеру предметов 2
    PlayerTextDrawTextSize(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][2], 38.0000, 41.0000);
    PlayerTextDrawAlignment(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][2], 1);
    PlayerTextDrawColor(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][2], -1);
    PlayerTextDrawBackgroundColor(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][2], 190);
    PlayerTextDrawFont(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][2], 5);
    PlayerTextDrawSetProportional(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][2], 0);
    PlayerTextDrawSetShadow(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][2], 0);
    PlayerTextDrawSetSelectable(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][2], true);
    PlayerTextDrawSetPreviewModel(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][2], 19374);
    PlayerTextDrawSetPreviewRot(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][2], 0.0000, 0.0000, 0.0000, 1.0000);

    ptd_trade[playerid][PTD_TRADER_SLOTS][3] = CreatePlayerTextDraw(playerid, 539.0689, 227.6071, ""); // слот который относится к трейдеру предметов 3
    PlayerTextDrawTextSize(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][3], 38.0000, 41.0000);
    PlayerTextDrawAlignment(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][3], 1);
    PlayerTextDrawColor(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][3], -1);
    PlayerTextDrawBackgroundColor(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][3], 190);
    PlayerTextDrawFont(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][3], 5);
    PlayerTextDrawSetProportional(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][3], 0);
    PlayerTextDrawSetShadow(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][3], 0);
    PlayerTextDrawSetSelectable(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][3], true);
    PlayerTextDrawSetPreviewModel(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][3], 19374);
    PlayerTextDrawSetPreviewRot(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][3], 0.0000, 0.0000, 0.0000, 1.0000);

    ptd_trade[playerid][PTD_TRADER_SLOTS][4] = CreatePlayerTextDraw(playerid, 579.5349, 227.5776, ""); // слот который относится к трейдеру предметов 4
    PlayerTextDrawTextSize(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][4], 38.0000, 41.0000);
    PlayerTextDrawAlignment(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][4], 1);
    PlayerTextDrawColor(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][4], -1);
    PlayerTextDrawBackgroundColor(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][4], 190);
    PlayerTextDrawFont(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][4], 5);
    PlayerTextDrawSetProportional(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][4], 0);
    PlayerTextDrawSetShadow(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][4], 0);
    PlayerTextDrawSetSelectable(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][4], true);
    PlayerTextDrawSetPreviewModel(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][4], 19374);
    PlayerTextDrawSetPreviewRot(playerid, ptd_trade[playerid][PTD_TRADER_SLOTS][4], 0.0000, 0.0000, 0.0000, 1.0000);

    return true;
}

