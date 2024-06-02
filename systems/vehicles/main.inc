/*

    @---------------------
    @ This is an executable module that creates a personal transportation system.
    @ Developed by the developer: dev.r1nex
    @----------------------

*/

#include <YSI/YSI_Coding/y_hooks>

/*
    Hooks
*/
hook OnGameModeInit() {
    Iter_Init(IterPlayerVehicles);
}

hook OnPlayerDisconnect(playerid, reason) {
    g_player_veh_idx[playerid] = 0;
    g_player_veh_page[playerid] = 0;
}

/*
    Functions
*/
public: OnLoadPlayerVehicles(account_id) {
    static rows;
    cache_get_row_count(rows);
    if (rows) {
        for (new i; i < rows; i++) {
            cache_get_value_name_int(i, "id", VehicleInfo[account_id][i][eVehicle_ID]);
            cache_get_value_name_int(i, "account_id", VehicleInfo[account_id][i][eVehicle_AccountID]);
            cache_get_value_name_int(i, "model", VehicleInfo[account_id][i][eVehicle_Model]);
            cache_get_value_name_int(i, "color1", VehicleInfo[account_id][i][eVehicle_Color1]);
            cache_get_value_name_int(i, "color2", VehicleInfo[account_id][i][eVehicle_Color2]);
            cache_get_value_name_bool(i, "is_loaded", VehicleInfo[account_id][i][eVehicle_Loaded]);
            cache_get_value_name_int(i, "price", VehicleInfo[account_id][i][eVehicle_Price]);
            cache_get_value_name_bool(i, "locked", VehicleInfo[account_id][i][eVehicle_Locked]);
            cache_get_value_name_float(i, "car_x", VehicleInfo[account_id][i][eVehicle_X]);
            cache_get_value_name_float(i, "car_y", VehicleInfo[account_id][i][eVehicle_Y]);
            cache_get_value_name_float(i, "car_z", VehicleInfo[account_id][i][eVehicle_Z]);
            cache_get_value_name_float(i, "car_a", VehicleInfo[account_id][i][eVehicle_A]);
            
            if (VehicleInfo[account_id][i][eVehicle_Loaded] != false) {
                VehicleInfo[account_id][i][eVehicle_Identifier] = CreateVehicle(
                    VehicleInfo[account_id][i][eVehicle_Model], 
                    VehicleInfo[account_id][i][eVehicle_X], VehicleInfo[account_id][i][eVehicle_Y], VehicleInfo[account_id][i][eVehicle_Z], VehicleInfo[account_id][i][eVehicle_A], 
                    VehicleInfo[account_id][i][eVehicle_Color1], VehicleInfo[account_id][i][eVehicle_Color2], 
                    60000
                );
            }

            Iter_Add(IterPlayerVehicles[account_id], i);
        }
    }

    return 1;
}

/*
    Stocks
*/
stock GetPlayerCountVehicles(account_id) {
    new 
        count = Iter_Count(IterPlayerVehicles[account_id])
    ;

    return count;    
}

stock GetVehicleCustomName(model, string[], size = sizeof(string)) {
    if (model < 12500) {
        format(string, size, VehicleNames[model-400]);
    }

    for (new i; i < sizeof eAddedCars; i++) {
        if (eAddedCars[i][eModelID] == model) {
            format(string, size, eAddedCars[i][eModel_NAME]);
        }
    }

    return 1;
}

stock bool: FormatIntWithDelimiters(value, outstr[], delimiter = '.', size = sizeof(outstr))
{
    static delimiter_string[2] = { '\0', '\0' };
    static len;
    delimiter_string[0] = delimiter;
    if (value != cellmin)
        format(outstr, size, "%d", value);
    else
        outstr[0] = '\0', strcat(outstr, "-2147483648", size);
    len = strlen(outstr);
    switch (len - _:(value < 0))
    {
        case 10:
        {
            if (len + 3 >= size)
                return false;
 
            format(outstr, size, "%.*s%s%s", len - 3, outstr, delimiter_string, outstr[len - 3]);
            format(outstr, size, "%.*s%s%s", len - 6, outstr, delimiter_string, outstr[len - 6]);
            format(outstr, size, "%.*s%s%s", len - 9, outstr, delimiter_string, outstr[len - 9]);
        }
        case 7..9:
        {
            if (len + 2 >= size)
                return false;
 
            format(outstr, size, "%.*s%s%s", len - 3, outstr, delimiter_string, outstr[len - 3]);
            format(outstr, size, "%.*s%s%s", len - 6, outstr, delimiter_string, outstr[len - 6]);
        }
        case 4..6:
        {
            if (len + 1 >= size)
                return false;
 
            format(outstr, size, "%.*s%s%s", len - 3, outstr, delimiter_string, outstr[len - 3]);
        }
    }
    return true;
}

stock MYSQL_TASK:EditVehicleInfo(idx, account_id) {
    static
        sql_query[168 + 3 + 3 + 1 + 11 + 1 + 6 + 6 + 6 + 6 + 11 + 11] = EOS
    ;

    format(sql_query, sizeof sql_query, "UPDATE vehicles SET color1 = %d, color2 = %d, is_loaded = %d, price = %d, locked = %d, car_x = %f, car_y = %f, car_z = %f, car_a = %f WHERE id = %d AND account_id = %d", 
        VehicleInfo[account_id][idx][eVehicle_Color1],
        VehicleInfo[account_id][idx][eVehicle_Color2],
        VehicleInfo[account_id][idx][eVehicle_Loaded],
        VehicleInfo[account_id][idx][eVehicle_Price],
        VehicleInfo[account_id][idx][eVehicle_Locked],
        VehicleInfo[account_id][idx][eVehicle_X],
        VehicleInfo[account_id][idx][eVehicle_Y],
        VehicleInfo[account_id][idx][eVehicle_Z],
        VehicleInfo[account_id][idx][eVehicle_A],
        VehicleInfo[account_id][idx][eVehicle_ID],
        account_id
    );

    return mysql_pquery(dbHandle, sql_query);
}

stock MYSQL_TASK:RemoveCar(idx, account_id) {
    static
        sql_query[55 + 11 + 11] = EOS
    ;

    format(sql_query, sizeof sql_query, "DELETE FROM vehicles WHERE id = %d AND account_id = %d", 
        VehicleInfo[account_id][idx][eVehicle_ID],
        account_id
    );

    if (VehicleInfo[account_id][idx][eVehicle_Loaded])
        DestroyVehicle(VehicleInfo[account_id][idx][eVehicle_Identifier]);
 
    Iter_Remove(IterPlayerVehicles[account_id], idx);
    VehicleInfo[account_id][idx] = VehicleInfo_NULL;
    
    return mysql_pquery(dbHandle, sql_query);  
}

/*
    Create Dialog Content
*/
DialogCreate:dVehicles_List(playerid) {
    new count = GetPlayerCountVehicles(GetUserData(playerid, u_id));
    if (!count)
        return SendClientMessage(playerid, -1, MESSAGE_HAVE_CARS);

    new 
        dialog_string[50 + (36 + (11) + (20) + (24)) * 1000 / MAX_PLAYER_VEHICLES + 200] = ""cWHITE"ID:\t"cWHITE"Модель:\t"cWHITE"Гос. цена:\n",
        dialog_header[] = ""PROJECT_NAME"Список личных т/с [%d/%d]",
        dialog_header_size[sizeof dialog_header + (-2+2) * 2 + 5],
        page = g_player_veh_page[playerid]
    ;

    new start = page * MAX_DIALOG_ITEMS_PAGE;
    if (start > count) {
        page = -1;
        if (page < 0) {
            return 0;
        }

        g_player_veh_page[playerid] = page;
        start = page * MAX_DIALOG_ITEMS_PAGE; 
    }

    new end = start + MAX_DIALOG_ITEMS_PAGE;
    if (end > count)
        end = count;

    new 
        idx = -1, 
        str_num[64], 
        model_name[64],
        account_id = GetUserData(playerid, u_id)
    ;

    foreach (new i: IterPlayerVehicles[account_id]) {
        if (++idx < start) 
            continue;

        if (idx >= end) 
            break;

        GetVehicleCustomName(VehicleInfo[account_id][i][eVehicle_Model], model_name);

        FormatIntWithDelimiters(VehicleInfo[account_id][i][eVehicle_Price], str_num);

        format(dialog_string, sizeof dialog_string, "%s"cGREY"%d\t"cGREY"%s\t"cGREEN"%s $\n", 
            dialog_string, 
            i, 
            model_name, 
            str_num
        );
    }

    if (count > MAX_DIALOG_ITEMS_PAGE) {
        if(end < count) {
            strcat(dialog_string, ""cGREEN">>>\t\t\n");
        }

        if(page) {
            strcat(dialog_string, ""cRED"<<<\t\t\n");
        }
    }

    format(dialog_header_size, sizeof dialog_header_size, dialog_header, page, MAX_PLAYER_VEHICLES / MAX_DIALOG_ITEMS_PAGE - 1);

    return Dialog_Open(playerid, Dialog:dVehicles_List, DIALOG_STYLE_TABLIST_HEADERS, dialog_header_size, dialog_string, ""cWHITE"Выбор", ""cWHITE"Закрыть");
}

DialogCreate:dVeh_Menu(playerid) {
    new 
        header[51 + 3],
        account_id = GetUserData(playerid, u_id)
    ;

    format(header, sizeof header, ""cBLUE"Взаимодействие с транспортом "cWHITE"(#%d)", VehicleInfo[account_id][g_player_veh_idx[playerid]][eVehicle_ID]);

    Dialog_Open(playerid, Dialog:dVeh_Menu, 
    DIALOG_STYLE_LIST, 
        header, 
        "\
            "cBLUE"1. "cWHITE"Технический паспорт. \n\
            "cBLUE"2. "cWHITE"Продать транспорт государству. \n\
            "cBLUE"3. "cWHITE"Продать транспорт игроку. \n\
            "cBLUE"4. "cWHITE"Припарковать. \n\
            "cBLUE"5. "cWHITE"Загрузить на сервер. \n\
            "cBLUE"6. "cWHITE"Выгрузить с сервера. \n\
        ", 
        ""cWHITE"Выбрать", ""cWHITE"Назад"
    );
}

DialogCreate:DLG_VEHICLE_SELL(playerid) {
    new
        account_id = GetUserData(playerid, u_id),
        array_idx = g_player_veh_idx[playerid],
        sale_summ = VehicleInfo[account_id][array_idx][eVehicle_Price] - (VehicleInfo[account_id][array_idx][eVehicle_Price] * PERCENT_SALE_VEHICLE / 100),
        dialog[] = 
        "\
            "cWHITE"Вы действительно хотите продать этот транспорт за пол суммы? \n\
            "cWHITE"Цена продажи: "cGREEN"%d $ \n\
        ",
        dialog_size[sizeof dialog + (-2+11)]
    ;

    format(dialog_size, sizeof dialog_size, dialog, sale_summ);

    return Dialog_Open(playerid, Dialog:DLG_VEHICLE_SELL, DIALOG_STYLE_MSGBOX, ""PROJECT_NAME"Продажа личного т/с", dialog_size, ""cWHITE"Продать", ""cWHITE"Назад");
}

/*
    Dialog Response
*/
DialogResponse:DLG_VEHICLE_SELL(playerid, response, listitem, inputtext[]) {
    if (!response)
        return Dialog_Show(playerid, Dialog:dVehicles_List);

    new
        account_id = GetUserData(playerid, u_id),
        array_idx = g_player_veh_idx[playerid],
        sale_summ = VehicleInfo[account_id][array_idx][eVehicle_Price] - (VehicleInfo[account_id][array_idx][eVehicle_Price] * PERCENT_SALE_VEHICLE / 100),
        game_text[7 + 32],
        player_money = GetUserData(playerid, u_money)
    ;

    format(game_text, sizeof game_text, "~g~+%d$", sale_summ);
    GameTextForPlayer(playerid, game_text, 3000, 1);

    player_money += sale_summ;
    SetUserData(playerid, u_money, player_money);
    GivePlayerMoney(playerid, sale_summ);

    MYSQL_TASK:RemoveCar(array_idx, account_id);
    return SendClientMessage(playerid, -1, ""cGREY"Вы продали этот транспорт государству.");
}

DialogResponse:dVehicles_List(playerid, response, listitem, inputtext[]) {
    if (!response)
        return g_player_veh_page[playerid] = 0;

    new page = g_player_veh_page[playerid];

    if (!strcmp(inputtext, ">>>", true)) {
        g_player_veh_page[playerid] = page + 1;
        return Dialog_Show(playerid, Dialog:dVehicles_List);
    } else if (!strcmp(inputtext, "<<<", true)) {
        page = page - 1;
        if (page <= 0)
            g_player_veh_page[playerid] = 0;
        else 
            g_player_veh_page[playerid] = page;

        return Dialog_Show(playerid, Dialog:dVehicles_List);
    } else {
        new
            idx = -1,
            find_idx = page * MAX_DIALOG_ITEMS_PAGE + listitem,
            skip,
            account_id = GetUserData(playerid, u_id)
        ;
        
        foreach (new i: IterPlayerVehicles[account_id]) {
            if (skip != find_idx) {
                skip += 1;
                continue;
            }

            idx = i;
            break;
        }

        if (idx == -1) {
            return SendClientMessage(playerid, -1, ""cERROR"Неизвестный индекс, сообщите разработчикам (idx == -1)");
        }

        g_player_veh_idx[playerid] = idx;
        return Dialog_Show(playerid, Dialog:dVeh_Menu);
    }
}

DialogResponse:dVeh_Menu(playerid, response, listitem, inputtext[]) {
    if (!response)
        return Dialog_Show(playerid, Dialog:dVehicles_List);

    new
        idx = g_player_veh_idx[playerid],
        account_id = GetUserData(playerid, u_id)
    ;

    switch(listitem) {
        case 1: Dialog_Show(playerid, Dialog:DLG_VEHICLE_SELL); // продать тс
        case 3: {
            new 
                Float: x = 0.0,
                Float: y = 0.0,
                Float: z = 0.0,
                Float: angle = 0.0
            ;

            GetPlayerPos(playerid, x, y, z);
            GetPlayerFacingAngle(playerid, angle);

            SendClientMessage(playerid, -1, ""cGREY"Вы припарковали транспорт в этом месте. Теперь при заходе на сервер он будет появляться здесь.");

            VehicleInfo[account_id][idx][eVehicle_X] = x;
            VehicleInfo[account_id][idx][eVehicle_Y] = y;
            VehicleInfo[account_id][idx][eVehicle_Z] = z;
            VehicleInfo[account_id][idx][eVehicle_A] = angle;

            MYSQL_TASK:EditVehicleInfo(idx, account_id);
        }

        case 4: { // загрузка тс
            new
                string[109 + 3] = EOS,
                Float: x,
                Float: y,
                Float: z,
                Float: a
            ;

            if (VehicleInfo[account_id][idx][eVehicle_Loaded]) {
                Dialog_Show(playerid, Dialog:dVehicles_List);
                return SendClientMessage(playerid, -1, ""cGREY"Этот транспорт уже находится на сервере.");
            }

            format(string, sizeof string, ""cGREY"Вы загрузили личный транспорт - "cBLUE"№-%d, "cGREY"он появится рядом с вами.", VehicleInfo[account_id][idx][eVehicle_ID]);
            SendClientMessage(playerid, -1, string);
            
            VehicleInfo[account_id][idx][eVehicle_Loaded] = true;
            GetPlayerPos(playerid, x, y, z);
            GetPlayerFacingAngle(playerid, a);

            VehicleInfo[account_id][idx][eVehicle_X] = x;
            VehicleInfo[account_id][idx][eVehicle_Y] = y;
            VehicleInfo[account_id][idx][eVehicle_Z] = z;
            VehicleInfo[account_id][idx][eVehicle_A] = a;

            VehicleInfo[account_id][idx][eVehicle_Identifier] = CreateVehicle(
                VehicleInfo[account_id][idx][eVehicle_Model], 
                x + 2.0, y, z, a, 
                VehicleInfo[account_id][idx][eVehicle_Color1], 
                VehicleInfo[account_id][idx][eVehicle_Color2], 60000
            );

            MYSQL_TASK:EditVehicleInfo(idx, account_id);
            SetVehicleParamsEx(VehicleInfo[account_id][idx][eVehicle_Identifier], 0, 0, 0, 0, 0, 0, 0);
            Dialog_Show(playerid, Dialog:dVehicles_List);
        }

        case 5: { // выгрузка
            new
                string[74 + 3] = EOS
            ;

            if (!VehicleInfo[account_id][idx][eVehicle_Loaded]) {
                Dialog_Show(playerid, Dialog:dVehicles_List);
                return SendClientMessage(playerid, -1, ""cERROR"Этот транспорт ещё не находится на сервере.");
            }

            format(string, sizeof string, ""cGREY"Вы выгрузили личный транспорт - "cBLUE"№-%d.", VehicleInfo[account_id][idx][eVehicle_ID]);
            SendClientMessage(playerid, -1, string);

            DestroyVehicle(VehicleInfo[account_id][idx][eVehicle_Identifier]);

            VehicleInfo[account_id][idx][eVehicle_Loaded] = false;
            VehicleInfo[account_id][idx][eVehicle_X] = 0.0;
            VehicleInfo[account_id][idx][eVehicle_Y] = 0.0;
            VehicleInfo[account_id][idx][eVehicle_Z] = 0.0;
            VehicleInfo[account_id][idx][eVehicle_A] = 0.0;

            MYSQL_TASK:EditVehicleInfo(idx, account_id);
            SetVehicleParamsEx(VehicleInfo[account_id][idx][eVehicle_Identifier], 0, 0, 0, 0, 0, 0, 0);
            Dialog_Show(playerid, Dialog:dVehicles_List);
        }
    }

    return 1;
}

/*
    Сommands
*/
cmd:cars(playerid) {
    return Dialog_Show(playerid, Dialog:dVehicles_List);
}
