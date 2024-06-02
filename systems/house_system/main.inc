/*

    @---------------------
    @ This is an executable module that creates a personal house system on the server.
    @ Developed by the developer: dev.r1nex
    @----------------------

*/

#include <YSI/YSI_Coding/y_hooks>

/*
    Functions
*/
stock initializationOfInteriors() {
    /* У одного интерьера есть баг, на сколько я помню, баг с интерьером дома CJ. И ещё какие-то. */

    CreateDynamicObject(14476, 367.17584, 1798.88477, 541.33124,   0.00000, 0.00000, 0.00000);  // Нормальный
    CreateDynamicObject(17564, 369.36316, 1793.29968, 542.39764,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(14534, 298.27023, 1778.56189, 541.33154,   0.00000, 0.00000, 0.00000);  // Нормальный
    CreateDynamicObject(1504, 303.39651, 1781.01038, 536.69580,   0.00000, 0.00000, 0.00000);
    CreateDynamicObject(14581, 392.27695, 1967.98145, 536.69305,   0.00000, 0.00000, 0.00000);  // Не нормальный
    CreateDynamicObject(14700, 653.91943, 2090.30762, 537.49579,   0.00000, 0.00000, 0.00000);  // Нормальный
    CreateDynamicObject(14714, 910.48193, 1952.39172, 538.41510,   0.00000, 0.00000, 0.00000);  // Нормальный
    CreateDynamicObject(14865, 1010.46350, 2049.87671, 536.59857,   0.00000, 0.00000, 0.00000); // Нормальный
    CreateDynamicObject(15031, 1229.65210, 1890.38123, 534.59753,   0.00000, 0.00000, 0.00000); // Нормальный
    CreateDynamicObject(15041, 1088.51782, 1758.80420, 536.04303,   0.00000, 0.00000, 0.00000); // Нормальный
    CreateDynamicObject(15042, 1026.88269, 1886.46765, 536.77386,   0.00000, 0.00000, 0.00000); // Не нормальный   

    for (new i; i < sizeof interiors_data; i++) {
        Iter_Add(IterInteriors, i);
    }

    return print("> Initialization of the iterator was successful.");
}

stock bool: IsHouseEntry(array_idx, playerid) {
    if (GetHouseData(array_idx, h_acc_id) == GetUserData(playerid, u_id))
        return false;

    return (GetHouseData(array_idx, h_entry)) ? true : false;
}

stock bool: IsPlayerHouseOwner(playerid, array_idx) {
    return (GetHouseData(array_idx, h_acc_id) == GetUserData(playerid, u_id));
}

stock UpdateHouseInDatabase(array_idx) {
    new query[66 + 11 + 1 + 11] = "UPDATE houses SET account_id = '%d', entry = '%d' WHERE id = '%d'";
    format(query, sizeof query, query, GetHouseData(array_idx, h_acc_id), GetHouseData(array_idx, h_entry), GetHouseData(array_idx, h_id));

    return mysql_pquery(dbHandle, query);
}

stock SetHouseEntry(array_idx) {
    GetHouseData(array_idx, h_entry) = !GetHouseData(array_idx, h_entry);
    UpdateHouseInDatabase(array_idx);
    return UpdateHouseDescription(array_idx);
}

stock HouseSale(playerid, array_idx, summ) {
    SetHouseData(array_idx, h_entry, false);
    SetHouseData(array_idx, h_acc_id, 0);

    SetUserData(playerid, u_money, GetUserData(playerid, u_money) + summ);
    temp_array[playerid][0] = INVALID_ARRAY_VALUE;
    temp_array[playerid][1] = INVALID_ARRAY_VALUE;
    temp_array[playerid][2] = INVALID_ARRAY_VALUE;
    GivePlayerMoney(playerid, summ);

    UpdateHouseDescription(array_idx);
    UpdateHouseInDatabase(array_idx);
    return SendClientMessage(playerid, -1, ""cPUSH"Дом был продан государству. Вы больше не его владелец!");
}

stock HouseBuy(playerid, array_idx) {
    new buy_summ = GetHouseData(array_idx, h_cost);

    if (GetUserData(playerid, u_money) < buy_summ)
        return SendClientMessage(playerid, -1, MESSAGE_NO_ENOUGH_MONEY);

    if (GetHouseData(array_idx, h_acc_id) != 0)
        return SendClientMessage(playerid, -1, ""cPUSH"Это имущество уже приобретено.");

    SetUserData(playerid, u_money, GetUserData(playerid, u_money) - buy_summ);
    SetHouseData(array_idx, h_acc_id, GetUserData(playerid, u_id));
    UpdateHouseDescription(array_idx);

    temp_array[playerid][0] = INVALID_ARRAY_VALUE;
    temp_array[playerid][1] = INVALID_ARRAY_VALUE;
    temp_array[playerid][2] = INVALID_ARRAY_VALUE;

    UpdateHouseInDatabase(array_idx);
    return SendClientMessage(playerid, -1, ""cPUSH"Поздравляем с покупкой имущества.");
}

stock UpdateHouseDescription(array_idx) {
    new Text3D: text_id = GetHouseData(array_idx, h_3dTEXT);
    new house_description_size[
        sizeof house_description 
        + (-2+11)
        + (-2+11)
        + (-2+7)
        + (-2+24)
    ];

    format(house_description_size, sizeof house_description_size, house_description, 
        GetHouseData(array_idx, h_id), 
        GetHouseData(array_idx, h_cost),
        GetHouseData(array_idx, h_entry) ? "закрыты" : "открыты",
        GetHouseData(array_idx, h_acc_id) ? "Дом куплен игроком" : "Дом продаётся"
    );    

    return Update3DTextLabelText(text_id, -1, house_description_size);
}

stock AddHouse(playerid, interior_idx) {
    new
        Float: player_x = 0.0,
        Float: player_y = 0.0,
        Float: player_z = 0.0, 
        Float: interior_exit_x = GetInteriorData(interior_idx, pos_exit_x),
        Float: interior_exit_y = GetInteriorData(interior_idx, pos_exit_y),
        Float: interior_exit_z = GetInteriorData(interior_idx, pos_exit_z)
    ;

    GetPlayerPos(playerid, player_x, player_y, player_z);

    new query[149 + 11 * 6 + 4];
    format(query, sizeof query, "INSERT INTO `houses` (enter_x, enter_y, enter_z, exit_x, exit_y, exit_z, interior_idx) VALUES ('%.3f', '%.3f', '%.3f', '%.3f', '%.3f', '%.3f', '%d')",
        player_x, player_y, player_z,
        interior_exit_x, interior_exit_y, interior_exit_z,
        interior_idx
    );

    SendClientMessage(playerid, -1, ""cPUSH"Вы успешно создали новый дом.");
    return mysql_pquery(dbHandle, query);
}

public: OnHouseLoad() {
    new rows;
    cache_get_row_count(rows);

    if (rows) {
        for (new i; i < rows; i++) {
            cache_get_value_name_int(i, "id", GetHouseData(i, h_id));
            cache_get_value_name_int(i, "account_id", GetHouseData(i, h_acc_id));
            cache_get_value_name_int(i, "cost", GetHouseData(i, h_cost));
            cache_get_value_name_float(i, "enter_x", GetHouseData(i, h_enter_x));
            cache_get_value_name_float(i, "enter_y", GetHouseData(i, h_enter_y));
            cache_get_value_name_float(i, "enter_z", GetHouseData(i, h_enter_z));
            cache_get_value_name_float(i, "exit_x", GetHouseData(i, h_exit_x));
            cache_get_value_name_float(i, "exit_y", GetHouseData(i, h_exit_y));
            cache_get_value_name_float(i, "exit_z", GetHouseData(i, h_exit_z));
            cache_get_value_name_bool(i, "entry", GetHouseData(i, h_entry));
            cache_get_value_name_int(i, "interior_idx", GetHouseData(i, h_interior_idx));
            
            if (GetHouseData(i, h_acc_id) == -1)
                house_data[i][h_enter_pickup] = CreateDynamicPickup(1273, 1, GetHouseData(i, h_enter_x), GetHouseData(i, h_enter_y), GetHouseData(i, h_enter_z), 0, 0);
            else house_data[i][h_enter_pickup] = CreateDynamicPickup(1272, 1, GetHouseData(i, h_enter_x), GetHouseData(i, h_enter_y), GetHouseData(i, h_enter_z), 0, 0);
            
            house_data[i][h_exit_pickup] = CreateDynamicPickup(19902, 1, GetHouseData(i, h_exit_x), GetHouseData(i, h_exit_y), GetHouseData(i, h_exit_z), GetHouseData(i, h_id), 0);

            new data[3] = {EOS, ...};
            data[0] = GetHouseData(i, h_id); /* Идентификатор дома */
            data[1] = GetHouseData(i, h_acc_id); /* Идентификатор аккаунта */
            data[2] = i; /* Необходимо для получения информации из массива с домами */

            Streamer_SetArrayData(STREAMER_TYPE_PICKUP, GetHouseData(i, h_enter_pickup), E_STREAMER_EXTRA_ID, data);
            Streamer_SetArrayData(STREAMER_TYPE_PICKUP, GetHouseData(i, h_exit_pickup), E_STREAMER_EXTRA_ID, data);

            new house_description_size[
                sizeof house_description 
                + (-2+11)
                + (-2+11)
                + (-2+7)
                + (-2+24)
            ];

            format(house_description_size, sizeof house_description_size, house_description, 
                GetHouseData(i, h_id), 
                GetHouseData(i, h_cost),
                GetHouseData(i, h_entry) ? "закрыты" : "открыты",
                GetHouseData(i, h_acc_id) == -1 ? "Дом находится в продаже" : "Дом куплен игроком"
            );

            GetHouseData(i, h_3dTEXT) = Create3DTextLabel(house_description_size, -1, GetHouseData(i, h_enter_x), GetHouseData(i, h_enter_y), GetHouseData(i, h_enter_z), 10.0, 0, 1);

            Iter_Add(IterHouses, i);
        }
    }

    return printf("> The OnHouseLoad function loaded %d houses", rows);
}

/*
    Hooks
*/
hook OnGameModeInit() {
    mysql_pquery(dbHandle, "SELECT * FROM houses", "OnHouseLoad");
    initializationOfInteriors();

    return true;
}

hook OnPlayerPickUpDynamicPickup(playerid, pickupid) {
    new data[3] = {EOS, ...};
    Streamer_GetArrayData(STREAMER_TYPE_PICKUP, pickupid, E_STREAMER_EXTRA_ID, data);

    new
        house_id = data[0],
        acc_id = data[1],
        array_idx = data[2]
    ;

    if (pickupid == GetHouseData(array_idx, h_enter_pickup) && !isnull(data)) {
        temp_array[playerid][0] = house_id;
        temp_array[playerid][1] = acc_id;
        temp_array[playerid][2] = array_idx;

        Dialog_Show(playerid, Dialog:DLG_HOUSE_GET_IN);
    }

    if (pickupid == GetHouseData(array_idx, h_exit_pickup) && !isnull(data)) {
        SetPlayerVirtualWorld(playerid, 0);
        SetPlayerPos(playerid, GetHouseData(array_idx, h_enter_x), GetHouseData(array_idx, h_enter_y), GetHouseData(array_idx, h_enter_z));
        SetCameraBehindPlayer(playerid);

        temp_array[playerid][0] = INVALID_ARRAY_VALUE;
        temp_array[playerid][1] = INVALID_ARRAY_VALUE;
        temp_array[playerid][2] = INVALID_ARRAY_VALUE;
    }

    return true;
}

/*
    Commands
*/
cmd:addhouse(playerid) {
    if (!Iter_Contains(iAdmins, playerid)) return SendClientMessage(playerid, -1, MESSAGE_NO_PERMS);
    if (!GetAdminData(playerid, a_isAuth)) return SendClientMessage(playerid, -1, MESSAGE_NO_ADMIN_AUTH);
    if (GetAdminData(playerid, a_level) < 6) return SendClientMessage(playerid, -1, MESSAGE_NO_PERMS);

    return Dialog_Show(playerid, Dialog:DLG_SELECT_INTERIOR);
}

cmd:house(playerid) {
    new 
        house_id = temp_array[playerid][0],
        acc_id = temp_array[playerid][1],
        array_idx = temp_array[playerid][2],
        player_virtual_world = GetPlayerVirtualWorld(playerid)
    ;

    if (house_id == INVALID_ARRAY_VALUE || acc_id == INVALID_ARRAY_VALUE || array_idx == INVALID_ARRAY_VALUE)
        return SendClientMessage(playerid, -1, MESSAGE_IS_NO_HOUSE);

    if (player_virtual_world != house_id)
        return SendClientMessage(playerid, -1, MESSAGE_IS_NO_HOUSE);
    
    if (!IsPlayerHouseOwner(playerid, array_idx))
        return SendClientMessage(playerid, -1, ""cGREY"Вы не владелец этого дома.");

    return Dialog_Show(playerid, Dialog:DLG_HOUSE_MENU);
}

/*
    Dialog Content
*/
DialogCreate:DLG_HOUSE_SALE(playerid) {
    new 
        array_idx = temp_array[playerid][2],
        sale_summ = GetHouseData(array_idx, h_cost) - (GetHouseData(array_idx, h_cost) * PERCENT_SALE_HOUSE / 100)
    ;
    

    new fmt[] = 
    "\
        "cWHITE"Вы действительно хотите продать свой дом государству с процентом "cBLUE"%d%% "cWHITE"от гос. стоимости? \n\
        "cWHITE"Итоговая сумма продажи - "cBLUE"%d$ \n\
    ";
    new fmt_size[sizeof fmt + (-2+3) + (-2+11)];
    format(fmt_size, sizeof fmt_size, fmt, PERCENT_SALE_HOUSE, sale_summ);

    return Dialog_Open(playerid, Dialog:DLG_HOUSE_SALE, DIALOG_STYLE_MSGBOX, ""cBLUE"Взаимодействие с домом.", fmt_size, ""cWHITE"Продать", ""cWHITE"Назад");
}

DialogCreate:DLG_HOUSE_MENU(playerid) {
    return Dialog_Open(playerid, 
        Dialog:DLG_HOUSE_MENU, DIALOG_STYLE_LIST, 
        ""cBLUE"Управление домом", 
        "\
            "cBLUE"1. "cWHITE"Открыть/закрыть вход в дом. \n\
            "cBLUE"2. "cWHITE"Информация о доме. \n\
            "cBLUE"3. "cWHITE"Продать дом государству. \n\
        ", 
        "Выбрать", "Закрыть"
    );
}

DialogCreate:DLG_HOUSE_GET_IN(playerid) {
    return Dialog_Open(
        playerid, Dialog:DLG_HOUSE_GET_IN, DIALOG_STYLE_LIST, 
        ""cBLUE"Взаимодействие с домом", 
        "\
            "cBLUE"1. "cWHITE"Зайти в помещение дома. \n\
            "cBLUE"2. "cWHITE"Купить дом у государства. \n\
        ", 
        "Выбрать", "Закрыть"
    );
}

DialogCreate:DLG_SELECT_INTERIOR(playerid) {
    new count = Iter_Count(IterInteriors);
    if (!count) return SendClientMessage(playerid, -1, ""cERROR"На сервере нету интерьеров для создания дома.");

    new
        dialog_string[50+(20+(11)+(20)+(MAX_PLAYER_NAME))*MAX_INTERIORS_LIST_PAGE+1] = "ID:\tНаименование:\n",
        page = g_player_page_interiors[playerid]
    ;

    new start = page * MAX_INTERIORS_LIST_PAGE;
    if (start > count) {
        page = -1;
        if (page < 0) {
            return 0;
        }

        g_player_page_interiors[playerid] = page;
        start = page * MAX_INTERIORS_LIST_PAGE; 
    }

    new end = start + MAX_INTERIORS_LIST_PAGE;
    if (end > count)
        end = count;

    new idx = -1;
    foreach (new i: IterInteriors) {
        if (++idx < start) 
            continue;

        if (idx >= end) 
            break;

        format(dialog_string, sizeof dialog_string, "%s"cWHITE"#%d\t"cBLUE"[ Нажмите, чтобы выбрать ]\n", dialog_string, i);
    }

    if (count > MAX_INTERIORS_LIST_PAGE) {
        if(end < count) {
            strcat(dialog_string, "{93c47d}>>>\t\t\n");
        }

        if(page) {
            strcat(dialog_string, "{f44336}<<<\t\t\n");
        }
    }

    return Dialog_Open(playerid, Dialog:DLG_SELECT_INTERIOR, DIALOG_STYLE_TABLIST_HEADERS, ""cWHITE"Доступные интерьеры.", dialog_string, "Выбор", "Закрыть");
}

/*
    Dialog Response
*/
DialogResponse:DLG_HOUSE_SALE(playerid, response, listitem, inputtext[]) {
    if (!response) return Dialog_Show(playerid, Dialog:DLG_HOUSE_MENU);
    new
        array_idx = temp_array[playerid][2],
        Float: x = GetHouseData(array_idx, h_enter_x),
        Float: y = GetHouseData(array_idx, h_enter_y),
        Float: z = GetHouseData(array_idx, h_enter_z),
        sale_summ = GetHouseData(array_idx, h_cost) - (GetHouseData(array_idx, h_cost) * PERCENT_SALE_HOUSE / 100)
    ;

    SetPlayerPos(playerid, x, y, z);
    SetPlayerVirtualWorld(playerid, 0);
    SetCameraBehindPlayer(playerid);

    return HouseSale(playerid, array_idx, sale_summ);
}

DialogResponse:DLG_HOUSE_MENU(playerid, response, listitem, inputtext[]) {
    if (!response) return true;

    new array_idx = temp_array[playerid][2];

    switch(listitem) {
        case 0: {
            new string[100];
            SetHouseEntry(array_idx);

            format(string, sizeof string, ""cPUSH"Дом %s.", GetHouseData(array_idx, h_entry) ? ""cRED"закрыт" : ""cGREEN"открыт");
            SendClientMessage(playerid, -1, string);
        }

        case 2: Dialog_Show(playerid, Dialog:DLG_HOUSE_SALE);

        default: Dialog_Show(playerid, Dialog:DLG_HOUSE_MENU);
    }

    return true;
}

DialogResponse:DLG_HOUSE_GET_IN(playerid, response, listitem, inputtext[]) {
    if (!response) {
        temp_array[playerid][0] = INVALID_ARRAY_VALUE;
        temp_array[playerid][1] = INVALID_ARRAY_VALUE;
        temp_array[playerid][2] = INVALID_ARRAY_VALUE;
        return true;
    }

    new
        house_id = temp_array[playerid][0],
        array_idx = temp_array[playerid][2]
    ;

    switch(listitem) {
        case 0: {
            if (house_id == INVALID_ARRAY_VALUE || array_idx == INVALID_ARRAY_VALUE) 
                return true;

            if (IsHouseEntry(array_idx, playerid)) {
                temp_array[playerid][0] = INVALID_ARRAY_VALUE;
                temp_array[playerid][1] = INVALID_ARRAY_VALUE;
                temp_array[playerid][2] = INVALID_ARRAY_VALUE;

                return SendClientMessage(playerid, -1, MESSAGE_HOUSE_ENTRY);
            }

            new
                interior_idx = GetHouseData(array_idx, h_interior_idx), 
                Float: interior_x = GetInteriorData(interior_idx, pos_x), 
                Float: interior_y = GetInteriorData(interior_idx, pos_y), 
                Float: interior_z = GetInteriorData(interior_idx, pos_z)
            ;

            TogglePlayerControllable(playerid, false);
            SetTimerEx("Player_Unfreeze", 2000, false, "d", playerid);
            SetPlayerPos(playerid, interior_x, interior_y, interior_z);
            SetPlayerVirtualWorld(playerid, house_id);

            SetCameraBehindPlayer(playerid); 
        }

        case 1: HouseBuy(playerid, array_idx);

        default: return Dialog_Show(playerid, Dialog:DLG_HOUSE_GET_IN);
    }

    return true;
}

public: Player_Unfreeze(playerid) return TogglePlayerControllable(playerid, true);

DialogResponse:DLG_SELECT_INTERIOR(playerid, response, listitem, inputtext[]) {
    if (!response) return g_player_page_interiors[playerid] = 0;
    new page = g_player_page_interiors[playerid];

    if (!strcmp(inputtext, ">>>", true)) {
        g_player_page_interiors[playerid] = page + 1;
        return Dialog_Show(playerid, Dialog:DLG_SELECT_INTERIOR);
    } else if (!strcmp(inputtext, "<<<", true)) {
        page = page - 1;
        if (page <= 0)
            g_player_page_interiors[playerid] = 0;
        else 
            g_player_page_interiors[playerid] = page;

        return Dialog_Show(playerid, Dialog:DLG_SELECT_INTERIOR);
    }

    new
        idx = -1,
        find_item = page * MAX_INTERIORS_LIST_PAGE + listitem,
        skip
    ;

    foreach(new i: IterInteriors) {
        if (skip != find_item) {
            skip += 1;
            continue;
        }

        idx = i;
        break;
    }

    if (idx == -1) {
        return SendClientMessage(playerid, -1, ""cERROR"Неизвестный индекс, сообщите разработчикам (idx == -1)");
    }

    return AddHouse(playerid, idx);
}