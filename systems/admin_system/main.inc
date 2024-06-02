/*

    @---------------------
    @ This is an executable module that creates the admin system.
    @ Developed by the developer: dev.r1nex
    @----------------------

*/

#include <YSI/YSI_Coding/y_hooks>

/*
    Functions
*/
public: OnLoadAdmins(playerid, account_id) {
    new rows;
    cache_get_row_count(rows);
    if (rows) {
        new
            id,
            level,
            warns,
            code[MAX_ADMIN_PASSWORD],
            nickname[MAX_PLAYER_NAME],
            string[] = ""cWHITE"Добро пожаловать, Администратор "cBLUE"%s.",
            string_size[sizeof string + (-2+MAX_PLAYER_NAME) + (-2+1)]
        ;

        cache_get_value_name_int(0, "id", id);
        cache_get_value_name_int(0, "account_id", account_id);
        cache_get_value_name_int(0, "level", level);
        cache_get_value_name_int(0, "warns", warns);
        cache_get_value_name(0, "code", code);
        
        SetAdminData(playerid, a_id, id);
        SetAdminData(playerid, a_accountId, account_id);
        SetAdminData(playerid, a_level, level);
        SetAdminData(playerid, a_code, code);
        SetAdminData(playerid, a_warns, warns);

        Iter_Add(iAdmins, playerid);

        GetPlayerName(playerid, nickname, MAX_PLAYER_NAME);
        format(string_size, sizeof (string_size), string, nickname, level);

        SendClientMessage(playerid, -1, string_size);
    }

    return 1;
}

/*
    Stocks
*/
stock Admins_IsPlayerAdmin(playerid) {
    return Iter_Contains(iAdmins, playerid);
}

stock Admins:SetPlayerAdmin(playerid, lvl, password[MAX_ADMIN_PASSWORD]) {
    Iter_Add(iAdmins, playerid);
    new string_query[144] = EOS;
    format(string_query, sizeof string_query, "INSERT INTO admins (account_id, level, warns, code) VALUES (%d, %d, 0, '%s')", GetUserData(playerid, u_id), lvl, password);
    mysql_pquery(dbHandle, string_query);

    return SendClientMessage(playerid, -1, ""cGREY"Вы были назначены на пост администратора.");
}

stock Admins:RemovePlayerAdmin(playerid) {
    new string_query[52 + 11 + 11] = EOS;
    format(string_query, sizeof string_query, "DELETE FROM admins WHERE account_id = %d AND id = %d", GetUserData(playerid, u_id), GetAdminData(playerid, a_id));
    mysql_pquery(dbHandle, string_query);

    Iter_Remove(iAdmins, playerid);
    admin_data[playerid] = admin_data_clear;

    return SendClientMessage(playerid, -1, ""cGREY"Вы были сняты с поста администратора.");
}

stock Admins:SetAdminWarn(playerid, count) {
    SetAdminData(playerid, a_warns, count);

    new string_query[28 + 1] = EOS;
    format(string_query, sizeof string_query, "UPDATE admins SET warns = %d", count);
    mysql_pquery(dbHandle, string_query);

    if (GetAdminData(playerid, a_warns) >= 3) {
        Admins:RemovePlayerAdmin(playerid);    
    }

    return SendClientMessage(playerid, -1, ""cPUSH"Вам было установлено предупреждение.");
}

stock Admins:GiveAdminWarn(playerid, count) {
    GetAdminData(playerid, a_warns) += count;

    new string_query[28 + 1] = EOS;
    format(string_query, sizeof string_query, "UPDATE admins SET warns = %d", GetAdminData(playerid, a_warns));
    mysql_pquery(dbHandle, string_query);

    if (GetAdminData(playerid, a_warns) >= 3) {
        Admins:RemovePlayerAdmin(playerid);    
    }

    return SendClientMessage(playerid, -1, ""cPUSH"Вам было выдано предупреждение.");
}

stock Admins:EditAdminCode(playerid, password[MAX_ADMIN_PASSWORD]) {
    SetAdminData(playerid, a_code, password);
    new text[93 + MAX_ADMIN_PASSWORD] = EOS;
    format(text, sizeof text, ""cPUSH"Ваш пароль входа в систему администратора был изменён "cBLUE"(%s)", password);

    new string_query[51 + MAX_ADMIN_PASSWORD + 11] = EOS;
    format(string_query, sizeof string_query, "UPDATE admins SET code = '%s' WHERE account_id = %d", password, GetUserData(playerid, u_id));
    mysql_pquery(dbHandle, string_query);
    
    return SendClientMessage(playerid, -1, text);
}

stock Admins:SendAdminsMessage(const string[]) {
    foreach (new i: iAdmins) {
        SendClientMessage(i, -1, string);
    }

    return 1;
}

stock Admins:AddReport(playerid, const text[MAX_TEXT_REPORT]) {
    new free_id = Iter_Free(iReport);

    if (free_id == ITER_NONE) {
        return SendClientMessage(playerid, -1, ""cERROR"Список жалоб переполнен.");
    }

    SetReportData(free_id, r_sender, playerid);
    SetReportData(free_id, r_text, text);

    SendClientMessage(playerid, -1, ""cPUSH"Ваша жалоба была отправлена на рассмотрение администрации.");
    return Iter_Add(iReport, free_id);
}

stock Admins:RemoveReportByID(idx) {
    report_data[idx] = report_data_clear;
    return Iter_Remove(iReport, idx);
}

/*
    Dialog Content
*/
DialogCreate:DLG_ALOGIN(playerid) {
    Dialog_Open(playerid, 
        Dialog:DLG_ALOGIN, 
        DIALOG_STYLE_PASSWORD, 
        ""PROJECT_NAME"Авторизация администратора.", 
        ""cWHITE"Введите Ваш пароль от панели администратора.",
        ""cWHITE"Далее", 
        ""cWHITE"Контекст"
    );
}

DialogCreate:DLG_REPORTS_LIST(playerid) {
    new count = Iter_Count(iReport);
    if (!count) return SendClientMessage(playerid, -1, ""cGREY"Список жалоб пуст.");
    
    new 
        dialog_string[107 + 40 + 3 + MAX_PLAYER_NAME + 64 * MAX_REPORTS / MAX_DIALOG_ITEMS_PAGE + 120] = 
        "{ffffff}Порядковый номер:\t{ffffff}Отправитель:\t{ffffff}Текст:\n",
        dialog_header[] = ""PROJECT_NAME"Список всех жалоб [%d/%d]",
        dialog_header_size[sizeof dialog_header + (-2+2) * 2 + 5],
        page = g_player_rep_page[playerid]
    ;

    new start = page * MAX_DIALOG_ITEMS_PAGE;
    if (start > count) {
        page = -1;
        if (page < 0) {
            return 0;
        }

        g_player_rep_page[playerid] = page;
        start = page * MAX_DIALOG_ITEMS_PAGE; 
    }

    new end = start + MAX_DIALOG_ITEMS_PAGE;
    if (end > count)
        end = count;

    new idx = -1, player_name[MAX_PLAYER_NAME];
    foreach (new i: iReport) {
        if(++idx < start) 
            continue;

        if(idx >= end) 
            break;

        new sender_id = GetReportData(i, r_sender);
        GetPlayerName(sender_id, player_name, MAX_PLAYER_NAME);
        format(dialog_string, sizeof dialog_string, "%s{b7b4b4}%d\t{b7b4b4}%s[%d]\t{b7b4b4}%s\n", dialog_string, i, player_name, sender_id, GetReportData(i, r_text));
    }

    if (count > MAX_DIALOG_ITEMS_PAGE) {
        if(end < count) {
            strcat(dialog_string, ""cGREEN">>>\t\t\n");
        }

        if(page) {
            strcat(dialog_string, ""cRED"<<<\t\t\n");
        }
    }

    format(dialog_header_size, sizeof dialog_header_size, dialog_header, page, MAX_REPORTS / MAX_DIALOG_ITEMS_PAGE - 1);

    return Dialog_Open(playerid, Dialog:DLG_REPORTS_LIST, DIALOG_STYLE_TABLIST_HEADERS, dialog_header_size, dialog_string, ""cWHITE"Выбор", ""cWHITE"Закрыть");
}

DialogCreate:DLG_ADMIN_COMMANDS(playerid) {
    new count = sizeof g_admin_commands;
    if (!count) return SendClientMessage(playerid, -1, ""cERROR"Список команд пуст");

    new
        dialog_string[3096] = 
        ""cGREY"Уровень администратора\t"cGREY"Название команды\t"cGREY"Описание команды\t"cGREY"Аргументы команды\n",
        page = g_player_admin_commands_page[playerid]
    ;

    new start = page * MAX_DIALOG_ITEMS_PAGE;
    if (start > count) {
        page = -1;
        if (page < 0) {
            return 0;
        }

        g_player_admin_commands_page[playerid] = page;
        start = page * MAX_DIALOG_ITEMS_PAGE; 
    }

    new end = start + MAX_DIALOG_ITEMS_PAGE;
    if (end > count)
        end = count;

    new idx = -1;
    for (new i; i < sizeof g_admin_commands; i++) {
        if(++idx < start) 
            continue;

        if(idx >= end) 
            break;

        format(dialog_string, sizeof (dialog_string), "%s"cBLUE"%s\t"cGREY"%s\t"cGREY"%s\t"cGREY"%s\n", dialog_string, 
            g_admin_commands[i][ac_admin_level], g_admin_commands[i][ac_command], 
            g_admin_commands[i][ac_command_description], g_admin_commands[i][ac_command_arguments]
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

    return Dialog_Open(playerid, Dialog:DLG_ADMIN_COMMANDS, DIALOG_STYLE_TABLIST_HEADERS, ""PROJECT_NAME"Команды администратора", dialog_string, ""cWHITE"Закрыть", "");
}

DialogCreate:DLG_ADMINS_LIST(playerid) {
    new count = Iter_Count(iAdmins);
    if (!count)
        return SendClientMessage(playerid, -1, ""cGREY"Нет ни одного администратора в сети.");

    new
        dialog_string[2096] = 
        ""cGREY"Администратор:\t"cGREY"Уровень:\t"cGREY"Кол-во предупреждений:\t"cGREY"AUTH:\n",
        page = g_player_admins_page[playerid]
    ;

    new start = page * MAX_DIALOG_ITEMS_PAGE;
    if (start > count) {
        page = -1;
        if (page < 0) {
            return 0;
        }

        g_player_admins_page[playerid] = page;
        start = page * MAX_DIALOG_ITEMS_PAGE; 
    }

    new end = start + MAX_DIALOG_ITEMS_PAGE;
    if (end > count)
        end = count;

    new idx = -1;
    foreach (new i: iAdmins) {
        if(++idx < start) 
            continue;

        if(idx >= end) 
            break;

        format(dialog_string, sizeof dialog_string, "%s"cBLUE"%s[%d]\t%d\t%d из 3\t%s\n", 
            dialog_string, 
            GetUserData(i, u_name), 
            i, 
            GetAdminData(i, a_level), 
            GetAdminData(i, a_warns),
            GetAdminData(i, a_isAuth) ? ""cGREEN"АВТОРИЗОВАН" : ""cRED"НЕ АВТОРИЗОВАН"
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

    return Dialog_Open(playerid, Dialog:DLG_ADMINS_LIST, DIALOG_STYLE_TABLIST_HEADERS, ""PROJECT_NAME"Администрация онлайн", dialog_string, ""cWHITE"Закрыть", "");
}

DialogCreate:DLG_REPORT_ANSWER(playerid) {
    new
        report_index = g_player_rep_idx[playerid],
        report_sender = GetReportData(report_index, r_sender),
        player_name[MAX_PLAYER_NAME], 
        dialog_header[] = ""cWHITE"Ответ игроку - "cBLUE"%s[%d]",
        dialog_header_size[sizeof dialog_header + (-2+MAX_PLAYER_NAME) + (-2+4)],
        dialog_main[] = 
        "\
            "cWHITE"В поле ввода Вы можете отправить ответ игроку. \n\
        "
    ;

    GetPlayerName(report_sender, player_name, MAX_PLAYER_NAME);
    format(dialog_header_size, sizeof dialog_header_size, dialog_header, player_name, report_sender);

    return Dialog_Open(playerid, Dialog:DLG_REPORT_ANSWER, DIALOG_STYLE_INPUT, dialog_header_size, dialog_main, ""cWHITE"Ответить", ""cWHITE"Назад");
}

/*
    Dialog Response
*/
DialogResponse:DLG_ADMINS_LIST(playerid, response, listitem, inputtext[]) {
    if (!response)
        return g_player_admins_page[playerid] = 0;

    new page = g_player_admins_page[playerid];

    if (!strcmp(inputtext, ">>>", true)) {
        g_player_admin_commands_page[playerid] = page + 1;
        return Dialog_Show(playerid, Dialog:DLG_ADMINS_LIST);
    } else if (!strcmp(inputtext, "<<<", true)) {
        page = page - 1;
        if (page <= 0)
            g_player_admins_page[playerid] = 0;
        else 
            g_player_admins_page[playerid] = page;

        return Dialog_Show(playerid, Dialog:DLG_ADMINS_LIST);
    }

    return 1;
}

DialogResponse:DLG_REPORT_ANSWER(playerid, response, listitem, inputtext[]) {
    if (!response)
        return Dialog_Show(playerid, Dialog:DLG_REPORTS_LIST);
    
    new 
        report_index = g_player_rep_idx[playerid],
        report_sender = GetReportData(report_index, r_sender),
        player_name[MAX_PLAYER_NAME],
        answer[144],
        string[] = ""cGREY"Администратор: %s[%d] ответил Вам на ваш репорт: %s",
        string_size[sizeof string + (-2+MAX_PLAYER_NAME) + (-2+4) + (-2+144)],
        string_two[] = ""cRED"A: Администратор: %s[%d] ответил игроку %s[%d]: %s",
        string_two_size[sizeof string_two + (-2+MAX_PLAYER_NAME) + (-2+4) + (-2+MAX_PLAYER_NAME) + (-2+4) + (-2+144)]
    ;
    if (sscanf(inputtext, "s[144]", answer))
        return Dialog_Show(playerid, Dialog:DLG_REPORT_ANSWER);

    if (strlen(answer) > 32) {
        Dialog_Show(playerid, Dialog:DLG_REPORT_ANSWER);

        return SendClientMessage(playerid, -1, ""cGREY"Максимальная длина для Вашего ответа - 32 символа");
    }

    GetPlayerName(report_sender, player_name, MAX_PLAYER_NAME);
    format(string_size, sizeof string_size, string, user_data[playerid][u_name], playerid, answer);
    format(string_two_size, sizeof string_two_size, string_two, user_data[playerid][u_name], playerid, player_name, report_sender, answer);

    Admins:SendAdminsMessage(string_two_size);

    Admins:RemoveReportByID(report_index);
    return SendClientMessage(report_sender, -1, string_size);
}

DialogResponse:DLG_ADMIN_COMMANDS(playerid, response, listitem, inputtext[]) {
    if (!response)
        return g_player_admin_commands_page[playerid] = 0;

    new page = g_player_admin_commands_page[playerid];

    if (!strcmp(inputtext, ">>>", true)) {
        g_player_admin_commands_page[playerid] = page + 1;
        return Dialog_Show(playerid, Dialog:DLG_ADMIN_COMMANDS);
    } else if (!strcmp(inputtext, "<<<", true)) {
        page = page - 1;
        if (page <= 0)
            g_player_admin_commands_page[playerid] = 0;
        else 
            g_player_admin_commands_page[playerid] = page;

        return Dialog_Show(playerid, Dialog:DLG_ADMIN_COMMANDS);
    }

    return Dialog_Show(playerid, Dialog:DLG_ADMIN_COMMANDS);
}

DialogResponse:DLG_REPORTS_LIST(playerid, response, listitem, inputtext[]) {
    if (!response)
        return g_player_rep_page[playerid] = 0;
    
    new page = g_player_rep_page[playerid];

    if (!strcmp(inputtext, ">>>", true)) {
        g_player_rep_page[playerid] = page + 1;
        return Dialog_Show(playerid, Dialog:DLG_REPORTS_LIST);
    } else if (!strcmp(inputtext, "<<<", true)) {
        page = page - 1;
        if (page <= 0)
            g_player_rep_page[playerid] = 0;
        else 
            g_player_rep_page[playerid] = page;

        return Dialog_Show(playerid, Dialog:DLG_REPORTS_LIST);
    }

    new
        idx = -1,
        find_idx = page * MAX_DIALOG_ITEMS_PAGE + listitem,
        skip
    ;

    foreach (new i: iReport) {
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

    g_player_rep_idx[playerid] = idx;
    Dialog_Show(playerid, Dialog:DLG_REPORT_ANSWER);

    return true;
}

DialogResponse:DLG_ALOGIN(playerid, response, listitem, inputtext[]) {
    if (!response) {
        admin_data[playerid] = admin_data_clear;
        Iter_Remove(iAdmins, playerid);
        return Kick(playerid);
    }

    new code[MAX_ADMIN_PASSWORD];
    if (sscanf(inputtext, "s[32]", code))
        return Dialog_Show(playerid, Dialog:DLG_ALOGIN);
    
    if (code[0] == GetAdminData(playerid, a_code)) {
        new string[92 + MAX_PLAYER_NAME] = EOS;
        format(string, sizeof string, ""cWHITE"Администратор "cBLUE"%s "cWHITE"авторизировался ("cGREEN_BRIGHT"%d "cWHITE"уровень).", 
            user_data[playerid][u_name], GetAdminData(playerid, a_level)
        );
        Admins:SendAdminsMessage(string);
        SetAdminData(playerid, a_isAuth, true);

        SendClientMessage(playerid, -1, ""cPUSH"Вы успешно авторизировались в системе администратора.");

        PlayerTextDrawSetString(playerid, ptd_greenzone[playerid], "~g~УСПЕШНАЯ АДМИН-АВТОРИЗАЦИЯ");
        PlayerTextDrawFont(playerid, ptd_greenzone[playerid], 1);
        ShowBrightnessTextDraw(playerid);
    } else {
        return Kick(playerid);
    }

    return 1;
}

/*
    Commands
*/
cmd:alogin(playerid) {
    if (!Iter_Contains(iAdmins, playerid)) return SendClientMessage(playerid, -1, MESSAGE_NO_PERMS);
    if (GetAdminData(playerid, a_isAuth)) return SendClientMessage(playerid, -1, MESSAGE_IS_ADMIN_AUTH);
    return Dialog_Show(playerid, Dialog:DLG_ALOGIN);
}

cmd:ahelp(playerid) {
    if (!Iter_Contains(iAdmins, playerid)) return SendClientMessage(playerid, -1, MESSAGE_NO_PERMS);

    return Dialog_Show(playerid, Dialog:DLG_ADMIN_COMMANDS);
}

cmd:achat(playerid, params[]) {
    if (!Iter_Contains(iAdmins, playerid)) return SendClientMessage(playerid, -1, MESSAGE_NO_PERMS);
    if (!GetAdminData(playerid, a_isAuth)) return SendClientMessage(playerid, -1, MESSAGE_NO_ADMIN_AUTH);
    new 
        msg[64] = EOS,
        string[28 + MAX_PLAYER_NAME + sizeof msg] = EOS
    ;

    if (sscanf(params, "s[64]", msg))
        return SendClientMessage(playerid, -1, ""COMMAND_USAGE_FORMAT"/a [ message ]");

    format(string, sizeof string, ""aTAG"%s: %s", user_data[playerid][u_name], msg);

    return Admins:SendAdminsMessage(string);
}

cmd:goto(playerid, params[]) {
    if (!Iter_Contains(iAdmins, playerid)) return SendClientMessage(playerid, -1, MESSAGE_NO_PERMS);
    if (!GetAdminData(playerid, a_isAuth)) return SendClientMessage(playerid, -1, MESSAGE_NO_ADMIN_AUTH);
    new id;
    if (sscanf(params, "d", id))
        return SendClientMessage(playerid, -1, ""COMMAND_USAGE_FORMAT"/goto [ playerid ]");

    if (!IsPlayerConnected(id)) {
        return SendClientMessage(playerid, -1, ""cGREY"Этот игрок не подключён к серверу!");
    }
    
    new
        Float: x,
        Float: y,
        Float: z,
        interiorId,
        virtualWorld
    ;
    
    GetPlayerPos(id, x, y, z);
    interiorId = GetPlayerInterior(id);
    virtualWorld = GetPlayerVirtualWorld(id);

    SetPlayerPos(playerid, x, y, z);
    SetPlayerInterior(playerid, interiorId);
    SetPlayerVirtualWorld(playerid, virtualWorld);

    return SendClientMessage(playerid, -1, ""cPUSH"Вы были телепортированы к игроку.");
}

cmd:gethere(playerid, params[]) {
    if (!Iter_Contains(iAdmins, playerid)) return SendClientMessage(playerid, -1, MESSAGE_NO_PERMS);
    if (!GetAdminData(playerid, a_isAuth)) return SendClientMessage(playerid, -1, MESSAGE_NO_ADMIN_AUTH);
    new id;
    if (sscanf(params, "d", id))
        return SendClientMessage(playerid, -1, ""COMMAND_USAGE_FORMAT"/gethere [ playerid ]");

    if (!IsPlayerConnected(id)) {
        return SendClientMessage(playerid, -1, ""cGREY"Этот игрок не подключён к серверу!");
    }
    
    new
        string[93 + (MAX_PLAYER_NAME) + (MAX_PLAYER_NAME)] = EOS,
        Float: x,
        Float: y,
        Float: z,
        interiorId,
        virtualWorld
    ;

    GetPlayerPos(playerid, x, y, z);
    interiorId = GetPlayerInterior(playerid);
    virtualWorld = GetPlayerVirtualWorld(playerid);

    SetPlayerPos(id, x, y, z);
    SetPlayerInterior(id, interiorId);
    SetPlayerVirtualWorld(id, virtualWorld);

    format(string, sizeof string, ""aTAG"Администратор "cBLUE"%s "cWHITE"телепортировал к себе игрока "cBLUE"%s", user_data[playerid][u_name], user_data[id][u_name]);
    Admins:SendAdminsMessage(string);

    return SendClientMessage(playerid, -1, ""cPUSH"Вы телепортировали игрока к себе.");
}

cmd:set_admin(playerid, params[]) {
    if (!Iter_Contains(iAdmins, playerid)) return SendClientMessage(playerid, -1, MESSAGE_NO_PERMS);
    if (!GetAdminData(playerid, a_isAuth)) return SendClientMessage(playerid, -1, MESSAGE_NO_ADMIN_AUTH);
    if (GetAdminData(playerid, a_level) < 6) return SendClientMessage(playerid, -1, MESSAGE_NO_PERMS);
    new 
        id,
        level,
        code[MAX_ADMIN_PASSWORD]
    ;
    if (sscanf(params, "dds[32]", id, level, code))
        return SendClientMessage(playerid, -1, ""COMMAND_USAGE_FORMAT"/set_admin [ playerid ] [ lvl 1-6 ] [password]");

    if (!IsPlayerConnected(id)) {
        return SendClientMessage(playerid, -1, ""cGREY"Этот игрок не подключён к серверу!");
    }

    if (Iter_Contains(iAdmins, id)) return SendClientMessage(playerid, -1, ""cGREY"Этот игрок уже администратор.");

    if (id == playerid) return SendClientMessage(playerid, -1, ""cGREY"Вы не можете самому себе выдать права администратора");
    if (level < 1 || level > 6) return SendClientMessage(playerid, -1, ""COMMAND_USAGE_FORMAT"/setadmin [ playerid ] [ lvl 1-6 ] [ password ]");

    return Admins:SetPlayerAdmin(id, level, code);
}

cmd:remove_admin(playerid, params[]) {
    if (!Iter_Contains(iAdmins, playerid)) return SendClientMessage(playerid, -1, MESSAGE_NO_PERMS);
    if (!GetAdminData(playerid, a_isAuth)) return SendClientMessage(playerid, -1, MESSAGE_NO_ADMIN_AUTH);
    if (GetAdminData(playerid, a_level) < 6) return SendClientMessage(playerid, -1, MESSAGE_NO_PERMS);
    new 
        id
    ;
    if (sscanf(params, "d", id))
        return SendClientMessage(playerid, -1, ""COMMAND_USAGE_FORMAT"/remove_admin [ playerid ]");

    if (!IsPlayerConnected(id)) {
        return SendClientMessage(playerid, -1, ""cGREY"Этот игрок не подключён к серверу!");
    }

    if (!Iter_Contains(iAdmins, id)) return SendClientMessage(playerid, -1, ""cGREY"Этот игрок не администратор");

    if (id == playerid) return SendClientMessage(playerid, -1, ""cGREY"Вы не можете самому себе снять права администратора");

    return Admins:RemovePlayerAdmin(id);
}

cmd:a_setpassword(playerid, params[]) {
    if (!Iter_Contains(iAdmins, playerid)) return SendClientMessage(playerid, -1, MESSAGE_NO_PERMS);
    if (!GetAdminData(playerid, a_isAuth)) return SendClientMessage(playerid, -1, MESSAGE_NO_ADMIN_AUTH);
    if (GetAdminData(playerid, a_level) < 6) return SendClientMessage(playerid, -1, MESSAGE_NO_PERMS);
    new id, code[MAX_ADMIN_PASSWORD];
    if (sscanf(params, "ds[32]", id, code))
        return SendClientMessage(playerid, -1, ""COMMAND_USAGE_FORMAT"/a_setpassword [ playerid ] [ new_password ]");

    if (!IsPlayerConnected(id)) {
        return SendClientMessage(playerid, -1, ""cGREY"Этот игрок не подключён к серверу!");
    }

    if (!Iter_Contains(iAdmins, id)) return SendClientMessage(playerid, -1, ""cGREY"Этот игрок не администратор");

    return Admins:EditAdminCode(id, code);
}

/*cmd:a_unwarn(playerid, params[]) {
    if (!Iter_Contains(iAdmins, playerid)) return SendClientMessage(playerid, -1, MESSAGE_NO_PERMS);
    if (!GetAdminData(playerid, a_isAuth)) return SendClientMessage(playerid, -1, MESSAGE_NO_ADMIN_AUTH);
    if (GetAdminData(playerid, a_level) < 6) return SendClientMessage(playerid, -1, MESSAGE_NO_PERMS);
    new id, count;
    if (sscanf(params, "dd", id, count))
        return SendClientMessage(playerid, -1, ""COMMAND_USAGE_FORMAT"/a_unwarn [ playerid ] [ count ]");

    if (!IsPlayerConnected(id)) {
        return SendClientMessage(playerid, -1, ""cGREY"Этот игрок не подключён к серверу!");
    }

    if (!Iter_Contains(iAdmins, id)) return SendClientMessage(playerid, -1, ""cGREY"Этот игрок не администратор");

    SendClientMessage(playerid, -1, ""cPUSH"Администратору было установлено предупреждение.");
    return Admins:SetAdminWarn(id, count);
}

cmd:a_warn(playerid, params[]) {
    if (!Iter_Contains(iAdmins, playerid)) return SendClientMessage(playerid, -1, MESSAGE_NO_PERMS);
    if (!GetAdminData(playerid, a_isAuth)) return SendClientMessage(playerid, -1, MESSAGE_NO_ADMIN_AUTH);
    if (GetAdminData(playerid, a_level) < 6) return SendClientMessage(playerid, -1, MESSAGE_NO_PERMS);
    new id, count;
    if (sscanf(params, "dd", id, count))
        return SendClientMessage(playerid, -1, ""COMMAND_USAGE_FORMAT"/a_warn [ playerid ] [ 1-3 ]");

    if (!IsPlayerConnected(id)) {
        return SendClientMessage(playerid, -1, ""cERROR"Этот игрок не подключён к серверу!");
    }

    if (!Iter_Contains(iAdmins, id)) return SendClientMessage(playerid, -1, ""cGREY"Этот игрок не администратор");

    SendClientMessage(playerid, -1, ""cPUSH"Администратору было установлено предупреждение.");
    return Admins:GiveAdminWarn(id, count);
}*/

cmd:reports(playerid) {
    if (!Iter_Contains(iAdmins, playerid)) return SendClientMessage(playerid, -1, MESSAGE_NO_PERMS);
    if (!GetAdminData(playerid, a_isAuth)) return SendClientMessage(playerid, -1, MESSAGE_NO_ADMIN_AUTH);

    return Dialog_Show(playerid, Dialog:DLG_REPORTS_LIST);
}

cmd:report(playerid, params[]) {
    new const text[MAX_TEXT_REPORT] = EOS;
    if (sscanf(params, "s[64]", text))
        return SendClientMessage(playerid, -1, ""COMMAND_USAGE_FORMAT"/report [ message ]");

    return Admins:AddReport(playerid, text);
}

cmd:set_virtualworld(playerid, params[]) {
    if (!Iter_Contains(iAdmins, playerid)) return SendClientMessage(playerid, -1, MESSAGE_NO_PERMS);
    if (!GetAdminData(playerid, a_isAuth)) return SendClientMessage(playerid, -1, MESSAGE_NO_ADMIN_AUTH);

    new vw_id;
    if (sscanf(params, "d", vw_id))
        return SendClientMessage(playerid, -1, ""COMMAND_USAGE_FORMAT"/set_virtualworld [ ID ]");

    SendClientMessage(playerid, -1, ""cPUSH"Виртуальный мир для Вас был изменён.");
    return SetPlayerVirtualWorld(playerid, vw_id);
}

cmd:set_interior(playerid, params[]) {
    if (!Iter_Contains(iAdmins, playerid)) return SendClientMessage(playerid, -1, MESSAGE_NO_PERMS);
    if (!GetAdminData(playerid, a_isAuth)) return SendClientMessage(playerid, -1, MESSAGE_NO_ADMIN_AUTH);

    new int_id;
    if (sscanf(params, "d", int_id))
        return SendClientMessage(playerid, -1, ""COMMAND_USAGE_FORMAT"/set_interior [ ID ]");

    SendClientMessage(playerid, -1, ""cPUSH"Интерьер для Вас был изменён.");
    return SetPlayerVirtualWorld(playerid, int_id);
}

cmd:tppos(playerid, params[]) {
    if (!Iter_Contains(iAdmins, playerid)) return SendClientMessage(playerid, -1, MESSAGE_NO_PERMS);
    if (!GetAdminData(playerid, a_isAuth)) return SendClientMessage(playerid, -1, MESSAGE_NO_ADMIN_AUTH);

    new
        Float: x,
        Float: y,
        Float: z
    ;

    if (sscanf(params, "fff", x, y, z))
        return SendClientMessage(playerid, -1, ""COMMAND_USAGE_FORMAT"/tppos [ x ] [ y ] [ z ]");

    return SetPlayerPos(playerid, x, y, z);
}

cmd:veh(playerid, params[]) {
	if (!Iter_Contains(iAdmins, playerid)) return SendClientMessage(playerid, -1, MESSAGE_NO_PERMS);
    if (!GetAdminData(playerid, a_isAuth)) return SendClientMessage(playerid, -1, MESSAGE_NO_ADMIN_AUTH);
    new car, color1, color2;
	new Float: x;
	new Float: y;
	new Float: z;
	if (sscanf(params, "ddd", car, color1, color2)) 
		return SendClientMessage(playerid, -1, ""COMMAND_USAGE_FORMAT"/veh [ modelid ] [ color1 ] [ color2 ]");

	GetPlayerPos(playerid, x, y, z);

	return CreateVehicle(car, x + 1.0, y + 5.0, z, 0.0, color1, color2, 60000);
}

cmd:set_satiety(playerid, params[]) {
    if (!Iter_Contains(iAdmins, playerid)) return SendClientMessage(playerid, -1, MESSAGE_NO_PERMS);
    if (!GetAdminData(playerid, a_isAuth)) return SendClientMessage(playerid, -1, MESSAGE_NO_ADMIN_AUTH);

    new 
        player_id = INVALID_PLAYER_ID,
        player_name[MAX_PLAYER_NAME],
        Float: satiety = 0.0
    ;

    if (sscanf(params, "df", player_id, satiety))
        return SendClientMessage(playerid, -1, ""COMMAND_USAGE_FORMAT"/set_satiety [ playerid ] [ 1-100 ]");

    if (!IsPlayerConnected(player_id))
        return SendClientMessage(playerid, -1, ""cGREY"Этот игрок не подключён.");

    if (!GetUserData(player_id, u_auth))
        return SendClientMessage(playerid, -1, ""cGREY"Этот игрок не авторизовался.");

    if (satiety < 1.0)
        return SendClientMessage(playerid, -1, ""cGREY"Вы не можете установить уровень сытости меньше нуля.");

    if (satiety > 100.0)
        return SendClientMessage(playerid, -1, ""cGREY"Вы не можете установить уровень сытости больше ста.");

    if (GetUserData(player_id, u_satiety) >= 100.0)
        return SendClientMessage(playerid, -1, ""cGREY"У этого игрока полная сытость.");

    GetPlayerName(player_id, player_name, MAX_PLAYER_NAME);
    SetUserData(player_id, u_satiety, satiety);

    UpdatePlayerBar(player_id, 2);
    return SendClientMessage(playerid, -1, ""cGREY"Вы изменили сытость этому игроку.");
}

cmd:gotocar(playerid, params[]) {
    if (!Iter_Contains(iAdmins, playerid)) return SendClientMessage(playerid, -1, MESSAGE_NO_PERMS);
    if (!GetAdminData(playerid, a_isAuth)) return SendClientMessage(playerid, -1, MESSAGE_NO_ADMIN_AUTH);

    new 
        vehicle_id = INVALID_VEHICLE_ID,
        Float: vehicle_x = 0.0,
        Float: vehicle_y = 0.0,
        Float: vehicle_z = 0.0
    ;

    if (sscanf(params, "d", vehicle_id))
        return SendClientMessage(playerid, -1, ""COMMAND_USAGE_FORMAT"/gotocar [ vehicleid ]");

    if (!IsValidVehicle(vehicle_id))
        return SendClientMessage(playerid, -1, ""cGREY"Данного транспорта на сервере не существует");

    GetVehiclePos(vehicle_id, vehicle_x, vehicle_y, vehicle_z);
    
    SendClientMessage(playerid, -1, ""cRED"A: Вы были телепортированы");
    return SetPlayerPos(playerid, vehicle_x, vehicle_y, vehicle_z);
}

cmd:getherecar(playerid, params[]) {
    if (!Iter_Contains(iAdmins, playerid)) return SendClientMessage(playerid, -1, MESSAGE_NO_PERMS);
    if (!GetAdminData(playerid, a_isAuth)) return SendClientMessage(playerid, -1, MESSAGE_NO_ADMIN_AUTH);

    new 
        vehicle_id = INVALID_VEHICLE_ID,
        Float: player_x = 0.0,
        Float: player_y = 0.0,
        Float: player_z = 0.0
    ;

    if (sscanf(params, "d", vehicle_id))
        return SendClientMessage(playerid, -1, ""COMMAND_USAGE_FORMAT"/getherecar [ vehicleid ]");

    if (!IsValidVehicle(vehicle_id))
        return SendClientMessage(playerid, -1, ""cGREY"Данного транспорта на сервере не существует");

    GetPlayerPos(playerid, player_x, player_y, player_z);
    
    SendClientMessage(playerid, -1, ""cRED"A: Вы телепортировали транспорт к себе");
    return SetVehiclePos(vehicle_id, player_x, player_y, player_z);
}

cmd:fixvehicle(playerid, params[]) {
    if (!Iter_Contains(iAdmins, playerid)) return SendClientMessage(playerid, -1, MESSAGE_NO_PERMS);
    if (!GetAdminData(playerid, a_isAuth)) return SendClientMessage(playerid, -1, MESSAGE_NO_ADMIN_AUTH);

    new vehicle_id = INVALID_VEHICLE_ID;

    if (sscanf(params, "d", vehicle_id))
        return SendClientMessage(playerid, -1, ""COMMAND_USAGE_FORMAT"/fv [ vehicleid ]");

    if (!IsValidVehicle(vehicle_id))
        return SendClientMessage(playerid, -1, ""cGREY"Данного транспорта на сервере не существует");

    SendClientMessage(playerid, -1, ""cRED"A: Вы починили этот транспорт");
    return RepairVehicle(vehicle_id);
}

cmd:mute(playerid, params[]) {
    if (!Iter_Contains(iAdmins, playerid)) return SendClientMessage(playerid, -1, MESSAGE_NO_PERMS);
    if (!GetAdminData(playerid, a_isAuth)) return SendClientMessage(playerid, -1, MESSAGE_NO_ADMIN_AUTH);

    new 
        player_id,
        mute_time = 0,
        reason[32],
        string[] = "{fa584c}A: Администратор %s заблокировал чат игроку %s на %d минут. Причина: %s",
        string_size[sizeof string + (-2+MAX_PLAYER_NAME) + (-2+MAX_PLAYER_NAME) + (-2+11) + (-2+32)]
    ;

    if (sscanf(params, "uds[32]", player_id, mute_time, reason))
        return SendClientMessage(playerid, -1, ""COMMAND_USAGE_FORMAT"/mute [ playerid / Name_Surname ] [ минуты ] [ причина ]");

    if (!IsPlayerConnected(player_id))
        return SendClientMessage(playerid, -1, ""cGREY"Этот игрок не подключён к серверу.");

    if (!GetUserData(player_id, u_auth))
        return SendClientMessage(playerid, -1, ""cGREY"Этот игрок не авторизовался на сервере.");

    new unix_time = gettime() + mute_time * 60;

    format(string_size, sizeof string_size, string, GetUserData(playerid, u_name), GetUserData(player_id, u_name), mute_time, reason);
    
    SetUserData(player_id, u_mutetime, unix_time);
    return Admins:SendAdminsMessage(string_size);
}

cmd:unmute(playerid, params[]) {
    if (!Iter_Contains(iAdmins, playerid)) return SendClientMessage(playerid, -1, MESSAGE_NO_PERMS);
    if (!GetAdminData(playerid, a_isAuth)) return SendClientMessage(playerid, -1, MESSAGE_NO_ADMIN_AUTH);

    new
        player_id,
        string[] = "{fa584c}A: Администратор %s снял блокировку игрового чата игроку %s",
        string_size[sizeof string + (-2+MAX_PLAYER_NAME) + (-2+MAX_PLAYER_NAME) + (-2+11) + (-2+32)]
    ;

    if (!IsPlayerConnected(player_id))
        return SendClientMessage(playerid, -1, ""cGREY"Этот игрок не подключён к серверу.");

    if (!GetUserData(player_id, u_auth))
        return SendClientMessage(playerid, -1, ""cGREY"Этот игрок не авторизовался на сервере.");

    if (sscanf(params, "u", player_id))
        return SendClientMessage(playerid, -1, ""COMMAND_USAGE_FORMAT"/unmute [ playerid / Name_Surname ]");

    format(string_size, sizeof string_size, string, GetUserData(playerid, u_name), GetUserData(player_id, u_name));
    SetUserData(player_id, u_mutetime, 0);

    return Admins:SendAdminsMessage(string_size);
}

cmd:stats(playerid, params[]) {
    if (!Iter_Contains(iAdmins, playerid)) return SendClientMessage(playerid, -1, MESSAGE_NO_PERMS);
    if (!GetAdminData(playerid, a_isAuth)) return SendClientMessage(playerid, -1, MESSAGE_NO_ADMIN_AUTH);

    new player_id;

    if (sscanf(params, "d", player_id))
        return SendClientMessage(playerid, -1, ""COMMAND_USAGE_FORMAT"/stats [ playerid ]");

    if (!IsPlayerConnected(player_id))
        return SendClientMessage(playerid, -1, ""cGREY"Этот игрок не подключён к серверу.");

    if (!GetUserData(player_id, u_auth))
        return SendClientMessage(playerid, -1, ""cGREY"Этот игрок не авторизовался на сервере.");

    new fmt[] = 
        "\
            \n\
            "cWHITE"Никнейм: \t\t"cBLUE"%s \n\
            "cWHITE"UID: \t\t\t"cBLUE"%d \n\
            "cWHITE"Уровень: \t\t"cBLUE"%d \n\
            "cWHITE"Дата регистрации: \t"cBLUE"%s \n\
            "cWHITE"Опыт: \t\t\t"cYELLOW"%d/%d EXP \n\
            "cWHITE"Кошелёк: \t\t"cGREEN"%d $ \n\
            "cWHITE"Счёт в банке: \t\t"cGREEN"1000 $ \n\
            "cWHITE"Депозит: \t\t"cGREEN"5000 $ \n\
            \n\
        ",
        player_name[MAX_PLAYER_NAME],
        size_fmt[sizeof fmt + (-2+MAX_PLAYER_NAME) + (-2+11) + (-2+4) + (-2+20) + (-2+11) + (-2+11) + (-2+11) + (-2+11)]
    ;

    GetPlayerName(player_id, player_name, MAX_PLAYER_NAME);
    format(size_fmt, sizeof size_fmt, fmt, 
        player_name, 
        GetUserData(player_id, u_id), 
        GetUserData(player_id, u_score),
        GetUserData(player_id, u_regdate), 
        GetUserData(player_id, u_exp), 
        (GetUserData(player_id, u_score) + 1) * 4, 
        GetUserData(player_id, u_money)
    );

    if (Admins_IsPlayerAdmin(player_id)) {
        strcat(size_fmt, !""cWHITE"\nСтатус: \t\t"cBLUE"Администратор");
    }

    return Dialog_Open(playerid, Dialog:DLG_PLAYER_STATS, DIALOG_STYLE_MSGBOX, ""PROJECT_NAME"Статистика игрока.", size_fmt, ""cWHITE"Закрыть", "");
}

cmd:offstats(playerid, params[]) {
    if (!Iter_Contains(iAdmins, playerid)) return SendClientMessage(playerid, -1, MESSAGE_NO_PERMS);
    if (!GetAdminData(playerid, a_isAuth)) return SendClientMessage(playerid, -1, MESSAGE_NO_ADMIN_AUTH);

    new 
        player_name[MAX_PLAYER_NAME],
        string[] = "SELECT * FROM users WHERE name = '%s' LIMIT 1",
        string_size[sizeof string + (-2+MAX_PLAYER_NAME)]
    ;

    if (sscanf(params, "s[24]", player_name))
        return SendClientMessage(playerid, -1, ""COMMAND_USAGE_FORMAT"/offstats [ Name_Surname ]");

    format(string_size, sizeof string_size, string, player_name);

    new 
        Cache: cache_id = mysql_query(dbHandle, string_size, true),
        rows,
        account_id = -1,
        name[MAX_PLAYER_NAME],
        money,
        gender,
        exp,
        score,
        idx_nationality,
        reg_date[MAX_REGDATE_LEN],
        reg_ip[39],
        fmt[] = 
        "\
            \n\
            "cWHITE"Никнейм \t\t"cBLUE"%s \n\
            "cWHITE"UID \t\t\t"cBLUE"%d \n\
            "cWHITE"Уровень \t\t"cBLUE"%d \n\
            "cWHITE"Дата регистрации \t"cBLUE"%s \n\
            "cWHITE"Пол \t\t\t"cBLUE"%s \n\
            "cWHITE"Национальность \t"cBLUE"%s \n\
            "cWHITE"Опыт \t\t\t"cYELLOW"%d/%d EXP \n\
            "cWHITE"Кошелёк \t\t"cGREEN"%d $ \n\
            "cWHITE"REG-IP \t\t"cBLUE"%s \n\
            \n\
        ",
        size_fmt[sizeof fmt + (-2+MAX_PLAYER_NAME) + (-2+11) + (-2+6) + (-2+MAX_REGDATE_LEN) + (-2+13) + (-2+32) + (-2+6) + (-2+11) + (-2+39)]
    ;

    cache_get_row_count(rows);

    if (!rows)
        return SendClientMessage(playerid, -1, ""cGREY"Этот игрок не найден в базе данных.");

    cache_get_value_name_int(0, "id", account_id);
    cache_get_value_name(0, "name", name);
    cache_get_value_name_int(0, "money", money);
    cache_get_value_name_int(0, "gender", gender);
    cache_get_value_name_int(0, "exp", exp);
    cache_get_value_name_int(0, "score", score);
    cache_get_value_name_int(0, "idx_nationality", idx_nationality);
    cache_get_value_name(0, "reg_date", reg_date);
    cache_get_value_name(0, "reg_ip", reg_ip);

    format(size_fmt, sizeof size_fmt, fmt, 
        name,
        account_id, 
        score,
        reg_date,
        gender ? ""cBLUE"Мужской" : ""cBLUE"Женский",
        g_types_nationality[idx_nationality], 
        exp, 
        (score + 1) * 4, 
        money,
        reg_ip
    );

    Dialog_Open(playerid, Dialog:DLG_PLAYER_STATS, DIALOG_STYLE_MSGBOX, ""PROJECT_NAME"Статистика игрока.", size_fmt, ""cWHITE"Закрыть", "");

    return cache_delete(cache_id);
}

cmd:hp(playerid, params[]) {
    if (!Iter_Contains(iAdmins, playerid)) return SendClientMessage(playerid, -1, MESSAGE_NO_PERMS);
    if (!GetAdminData(playerid, a_isAuth)) return SendClientMessage(playerid, -1, MESSAGE_NO_ADMIN_AUTH);

    new
        Float: health = 0.0,
        player_id = INVALID_PLAYER_ID
    ;

    if (sscanf(params, "df", player_id, health))
        return SendClientMessage(playerid, -1, ""COMMAND_USAGE_FORMAT"/hp [ playerid ] [ 1-100 ]");

    if (!IsPlayerConnected(player_id))
        return SendClientMessage(playerid, -1, ""cGREY"Этот игрок не подключён к серверу.");

    if (!GetUserData(player_id, u_auth))
        return SendClientMessage(playerid, -1, ""cGREY"Этот игрок не авторизовался на сервере.");

    if ((health < 1.0) || (health > 100.0))
        return SendClientMessage(playerid, -1, ""cGREY"Диапазон уровня ХП должен быть от 1 до 100");

    return SetPlayerHealth(player_id, health);
}

cmd:set_armour(playerid, params[]) {
    if (!Iter_Contains(iAdmins, playerid)) return SendClientMessage(playerid, -1, MESSAGE_NO_PERMS);
    if (!GetAdminData(playerid, a_isAuth)) return SendClientMessage(playerid, -1, MESSAGE_NO_ADMIN_AUTH);

    new
        player_id = INVALID_PLAYER_ID,
        Float: armour = 0.0
    ;

    if (sscanf(params, "df", player_id, armour))
        return SendClientMessage(playerid, -1, ""COMMAND_USAGE_FORMAT"/set_armour [ playerid ] [ 1-100 ]");

    if (!IsPlayerConnected(player_id))
        return SendClientMessage(playerid, -1, ""cGREY"Этот игрок не подключён к серверу.");

    if (!GetUserData(player_id, u_auth))
        return SendClientMessage(playerid, -1, ""cGREY"Этот игрок не авторизовался на сервере.");

    if ((armour < 1.0) || (armour > 100.0))
        return SendClientMessage(playerid, -1, ""cGREY"Диапазон уровня брони должен быть от 1 до 100");

    return SetPlayerArmour(playerid, armour);
}

cmd:admins(playerid) {
    if (!Iter_Contains(iAdmins, playerid)) return SendClientMessage(playerid, -1, MESSAGE_NO_PERMS);

    return Dialog_Show(playerid, Dialog:DLG_ADMINS_LIST);
}

/*
    Hooks
*/
hook OnPlayerDisconnect(playerid, reason) {
    if (Iter_Contains(iAdmins, playerid)) {
        admin_data[playerid] = admin_data_clear;
        Iter_Remove(iAdmins, playerid);
    }

    g_player_rep_page[playerid] = 0;
    g_player_admins_page[playerid] = 0;
    g_player_rep_idx[playerid] = 0;
    g_player_admin_commands_page[playerid] = 0;

    return 1;
}