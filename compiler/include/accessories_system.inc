#if defined _accessories_system
    #endinput
#endif
#define _accessories_system

#if !defined _mdialog_included
	#error "Please included <mdialog> before include <accessories_system>"
#endif

#if !defined foreach
    #error "Please included <foreach> before include <accessories_system>
#endif

#if !defined sscanf
    #error "Please included <sscanf> before include <accessories_system>
#endif

#if !defined _streamer_included
    #error "Please included <streamer> before include <accessories_system>"
#endif

// #if !defined mysql_included || !defined cache_get_value_name
//     #error "Please included <mysql> ver. R40+ before include <accessories_system>
// #endif

/*
    Macroses
*/
#if !defined MAX_ACCESSORIES_PLAYER
    #define MAX_ACCESSORIES_PLAYER (25)
#endif

#if !defined MAX_ACCESSORIES
    #define MAX_ACCESSORIES (3)
#endif

#if !defined MAX_ACCESSORY_NAME
    #define MAX_ACCESSORY_NAME (32)
#endif

#if !defined GetAccessoryData
    #define GetAccessoryData(%0,%1) accessories[%0][%1]
#endif

#if !defined GetPlayerAccessoryData
    #define GetPlayerAccessoryData(%0,%1,%2) accessory_data[%0][%1][%2]
#endif

#if !defined SetPlayerAccessoryData
    #define SetPlayerAccessoryData(%0,%1,%2,%3) accessory_data[%0][%1][%2] = %3
#endif

#if !defined MAX_COUNT_ACCESSORIES_DIALOG
    #define MAX_COUNT_ACCESSORIES_DIALOG (20)
#endif

/*
    Enums
*/
enum E_PLAYER_ACCESSORY_STRUCT {
    ACCESSORY_ID,
    OBJECT_ID,
    ACCESSORY_INDEX,
    bool: ACCESSORY_STATUS,
    Float: ACCESSORY_OFFSET_X,
    Float: ACCESSORY_OFFSET_Y,
    Float: ACCESSORY_OFFSET_Z,
    Float: ACCESSORY_OFFSET_RX,
    Float: ACCESSORY_OFFSET_RY,
    Float: ACCESSORY_OFFSET_RZ
};

enum E_ACCESSORY_STRUCT {
    ACCESSORY_COST,
    ACCESSORY_MODEL,
    ACCESSORY_NAME[MAX_ACCESSORY_NAME]
};

/*
    Variables
*/
new g_player_accessory_idx[MAX_PLAYERS] = {-1, ...};
new g_player_accessory_page[MAX_PLAYERS] = {0, ...};
new const accessories[MAX_ACCESSORIES][E_ACCESSORY_STRUCT] = {
    5000, 355, "AK-47 на спину",
    5000, 355, "AK-47 на спину",
    5000, 355, "AK-47 на спину"
};
new accessory_data[MAX_PLAYERS][MAX_ACCESSORIES_PLAYER][E_PLAYER_ACCESSORY_STRUCT];
new Iterator: iAccessories<3>;
new Iterator: iAccessoriesPlayer[MAX_PLAYERS]<MAX_ACCESSORIES_PLAYER>;

/*
    Functions
*/
stock AddAccessory(playerid, accessory_idx) {
    new
        string[] = "Вам был добавлен новый аксессуар - %s.",
        string_size[sizeof string + (-2+MAX_ACCESSORY_NAME)], 
        free_index = Iter_Free(iAccessoriesPlayer[playerid]),
        objectid = GetAccessoryData(accessory_idx, ACCESSORY_MODEL),
        accessory_name[MAX_ACCESSORY_NAME]
    ;

    SetPlayerAccessoryData(playerid, free_index, ACCESSORY_ID, free_index); // заменить на авто инкрементирующийся ID из бд
    SetPlayerAccessoryData(playerid, free_index, ACCESSORY_INDEX, accessory_idx);

    format(accessory_name, sizeof accessory_name, GetAccessoryData(accessory_idx, ACCESSORY_NAME));
    format(string_size, sizeof string_size, string, accessory_name);

    SendClientMessage(playerid, -1, string_size);
    return Iter_Add(iAccessoriesPlayer[playerid], free_index);
}

cmd:add_accessory(playerid, params[]) {
    new
        idx = 0
    ;

    if (sscanf(params, "d", idx))
        return SendClientMessage(playerid, -1, "/add_accessory <id>");

    return AddAccessory(playerid, idx);
}

cmd:accessories(playerid) {
    return Dialog_Show(playerid, Dialog:DLG_ACCESSORIES_LIST); 
}

/*
    Dialog Content
*/
DialogCreate:DLG_ACCESSORIES_BUY(playerid) {
    new count = Iter_Count(iAccessories);
    if (!count)
        return SendClientMessage(playerid, -1, "Аксессуаров на сервере нет.");

    new 
        dialog_string[42+(50+(11)+(20)+(MAX_PLAYER_NAME))*MAX_COUNT_ACCESSORIES_DIALOG+1] = "Стоимость:\tНаименование:\n",
        page = g_player_accessory_page[playerid]
    ;

    new start = page * MAX_COUNT_ACCESSORIES_DIALOG;
    if (start > count) {
        page = -1;
        if (page < 0) {
            return 0;
        }

        g_player_accessory_page[playerid] = page;
        start = page * MAX_COUNT_ACCESSORIES_DIALOG; 
    }

    new end = start + MAX_COUNT_ACCESSORIES_DIALOG;
    if (end > count)
        end = count;

    new idx = -1, accessory_name[MAX_ACCESSORY_NAME];
    foreach (new i: iAccessories) {
        if (++idx < start) 
            continue;

        if (idx >= end) 
            break;

        format(accessory_name, sizeof accessory_name, GetAccessoryData(i, ACCESSORY_NAME));

        format(dialog_string, sizeof dialog_string, "%s{bcbcbc}%d$\t{bcbcbc}%s\n", 
            dialog_string, 
            GetAccessoryData(i, ACCESSORY_COST),
            accessory_name 
        );
    }

    if (count > MAX_COUNT_ACCESSORIES_DIALOG) {
        if(end < count) {
            strcat(dialog_string, "{93c47d}>>>\t\t\n");
        }

        if(page) {
            strcat(dialog_string, "{f44336}<<<\t\t\n");
        }
    }

    return Dialog_Open(playerid, Dialog:DLG_ACCESSORIES_BUY, DIALOG_STYLE_TABLIST_HEADERS, "Список ваших аксессуаров.", dialog_string, "Выбор", "Закрыть");
}

DialogCreate:DLG_ACCESSORIES_LIST(playerid) {
    new count = Iter_Count(iAccessoriesPlayer[playerid]);
    if (!count)
        return SendClientMessage(playerid, -1, "У Вас нет аксессуаров.");

    new 
        dialog_string[42+(50+(11)+(20)+(MAX_PLAYER_NAME))*MAX_COUNT_ACCESSORIES_DIALOG+1] = "ID:\tНаименование:\n",
        page = g_player_accessory_page[playerid]
    ;

    new start = page * MAX_COUNT_ACCESSORIES_DIALOG;
    if (start > count) {
        page = -1;
        if (page < 0) {
            return 0;
        }

        g_player_accessory_page[playerid] = page;
        start = page * MAX_COUNT_ACCESSORIES_DIALOG; 
    }

    new end = start + MAX_COUNT_ACCESSORIES_DIALOG;
    if (end > count)
        end = count;

    new idx = -1, accessory_name[MAX_ACCESSORY_NAME];
    foreach (new i: iAccessoriesPlayer[playerid]) {
        if (++idx < start) 
            continue;

        if (idx >= end) 
            break;

        format(accessory_name, sizeof accessory_name, GetAccessoryData(GetPlayerAccessoryData(playerid, i, ACCESSORY_INDEX), ACCESSORY_NAME));

        format(dialog_string, sizeof dialog_string, "%s{bcbcbc}#%d\t{bcbcbc}%s\n", 
            dialog_string, 
            i,
            accessory_name 
        );
    }

    if (count > MAX_COUNT_ACCESSORIES_DIALOG) {
        if(end < count) {
            strcat(dialog_string, "{93c47d}>>>\t\t\n");
        }

        if(page) {
            strcat(dialog_string, "{f44336}<<<\t\t\n");
        }
    }

    return Dialog_Open(playerid, Dialog:DLG_ACCESSORIES_LIST, DIALOG_STYLE_TABLIST_HEADERS, "Список ваших аксессуаров.", dialog_string, "Выбор", "Закрыть");
}

DialogCreate:DLG_ACCESSORY_MENU(playerid) {
    return Dialog_Open(playerid, Dialog:DLG_ACCESSORY_MENU, 
        DIALOG_STYLE_LIST, "Взаимодействие.", 
        "\
            1. Снять аксессуар. \n\
            2. Надеть аксессуар. \n\
            3. Редактировать положение. \n\
        ", 
        "Выбрать", "Назад"
    );
}

/*
    Dialog Response
*/
DialogResponse:DLG_ACCESSORY_MENU(playerid, response, listitem, inputtext[]) {
    if (!response)
        return Dialog_Show(playerid, Dialog:DLG_ACCESSORIES_LIST);

    new idx = g_player_accessory_idx[playerid];
    printf("idx: %d", idx);

    switch(listitem) {
        case 0: {
            if (!GetPlayerAccessoryData(playerid, idx, ACCESSORY_STATUS))
                return SendClientMessage(playerid, -1, "Аксессуар не надет.");

            DestroyObject(GetPlayerAccessoryData(playerid, idx, OBJECT_ID));
            SetPlayerAccessoryData(playerid, idx, ACCESSORY_STATUS, false);
            SendClientMessage(playerid, -1, "Вы сняли аксессуар.");

            g_player_accessory_idx[playerid] = -1;
            g_player_accessory_page[playerid] = 0;
        }

        case 1: {
            if (GetPlayerAccessoryData(playerid, idx, ACCESSORY_STATUS))
                return SendClientMessage(playerid, -1, "Аксессуар уже надет.");

            GetPlayerAccessoryData(playerid, idx, OBJECT_ID) = CreateObject(GetAccessoryData(idx, ACCESSORY_MODEL), 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
            AttachObjectToPlayer(GetPlayerAccessoryData(playerid, idx, OBJECT_ID), playerid, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

            SetPlayerAccessoryData(playerid, idx, ACCESSORY_STATUS, true);
            SendClientMessage(playerid, -1, "Вы надели аксессуар.");

            g_player_accessory_idx[playerid] = -1;
            g_player_accessory_page[playerid] = 0;
        }

        case 2: {
            if (!GetPlayerAccessoryData(playerid, idx, ACCESSORY_STATUS))
                return SendClientMessage(playerid, -1, "Аксессуар не надет.");

            EditObject(playerid, GetPlayerAccessoryData(playerid, idx, OBJECT_ID));

            g_player_accessory_idx[playerid] = -1;
            g_player_accessory_page[playerid] = 0;
        }

        default: Dialog_Show(playerid, Dialog:DLG_ACCESSORY_MENU);
    }

    return true;
}

DialogResponse:DLG_ACCESSORIES_LIST(playerid, response, listitem, inputtext[]) {
    if (!response) {
        g_player_accessory_idx[playerid] = -1;
        g_player_accessory_page[playerid] = 0;
        return true;
    }

    new page = g_player_accessory_page[playerid];

    if (!strcmp(inputtext, ">>>", true)) {
        g_player_accessory_page[playerid] = page + 1;
        return Dialog_Show(playerid, Dialog:DLG_ACCESSORIES_LIST);
    } else if (!strcmp(inputtext, "<<<", true)) {
        page = page - 1;
        if (page <= 0)
            g_player_accessory_page[playerid] = 0;
        else 
            g_player_accessory_page[playerid] = page;

        return Dialog_Show(playerid, Dialog:DLG_ACCESSORIES_LIST);
    } else {
        new
            idx = -1,
            find_idx = page * MAX_COUNT_ACCESSORIES_DIALOG + listitem,
            skip
        ;
        
        foreach (new i: iAccessoriesPlayer[playerid]) {
            if (skip != find_idx) {
                skip += 1;
                continue;
            }

            idx = i;
            break;
        }

        if (idx == -1) {
            g_player_accessory_idx[playerid] = -1;
            g_player_accessory_page[playerid] = 0;

            return SendClientMessage(playerid, -1, "Неизвестный индекс, сообщите разработчикам (idx == -1)");
        }

        g_player_accessory_idx[playerid] = idx;
        return Dialog_Show(playerid, Dialog:DLG_ACCESSORY_MENU);
    }
}

DialogResponse:DLG_ACCESSORIES_BUY(playerid, response, listitem, inputtext[]) {
    if (!response) {
        g_player_accessory_idx[playerid] = -1;
        g_player_accessory_page[playerid] = 0;
        return true;
    }

    new page = g_player_accessory_page[playerid];

    if (!strcmp(inputtext, ">>>", true)) {
        g_player_accessory_page[playerid] = page + 1;
        return Dialog_Show(playerid, Dialog:DLG_ACCESSORIES_BUY);
    } else if (!strcmp(inputtext, "<<<", true)) {
        page = page - 1;
        if (page <= 0)
            g_player_accessory_page[playerid] = 0;
        else 
            g_player_accessory_page[playerid] = page;

        return Dialog_Show(playerid, Dialog:DLG_ACCESSORIES_BUY);
    } else {
        new
            idx = -1,
            find_idx = page * MAX_COUNT_ACCESSORIES_DIALOG + listitem,
            skip
        ;
        
        foreach (new i: iAccessories) {
            if (skip != find_idx) {
                skip += 1;
                continue;
            }

            idx = i;
            break;
        }

        if (idx == -1) {
            g_player_accessory_idx[playerid] = -1;
            g_player_accessory_page[playerid] = 0;

            return SendClientMessage(playerid, -1, "Неизвестный индекс, сообщите разработчикам (idx == -1)");
        }

        if (idx == 0) return Dialog_Show(playerid, Dialog:DLG_ACCESSORIES_BUY);

        if (GetUserMoney(playerid) < GetAccessoryData(idx, ACCESSORY_COST))
            return SendClientMessage(playerid, -1, "Недостаточно $.");

        TakeUserMoney(playerid, GetAccessoryData(idx, ACCESSORY_COST));
        return SendClientMessage(playerid, -1, "Вы купили новый аксессуар.");
    }
}