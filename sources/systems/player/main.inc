/*

    @---------------------
    @ This is an executable module that creates a player system.
    @ Developed by the developer: dev.r1nex
    @----------------------

*/

#include <YSI/YSI_Coding/y_hooks>

/*
    Stocks
*/
stock Players:ClearPlayerData(playerid) {
    new acc_id = GetUserData(playerid, u_id);

    foreach (new i: IterPlayerVehicles[acc_id]) {
        if (VehicleInfo[acc_id][i][eVehicle_Loaded] != false) {
            VehicleInfo[acc_id][i][eVehicle_Loaded] = false;
            VehicleInfo[acc_id][i][eVehicle_ID] = 0;
            VehicleInfo[acc_id][i][eVehicle_AccountID] = 0;
            VehicleInfo[acc_id][i][eVehicle_Model] = 0;
            VehicleInfo[acc_id][i][eVehicle_Color1] = 0;
            VehicleInfo[acc_id][i][eVehicle_Color2] = 0;
            VehicleInfo[acc_id][i][eVehicle_Price] = 0;
            VehicleInfo[acc_id][i][eVehicle_Locked] = false;
            VehicleInfo[acc_id][i][eVehicle_X] = 0.0;
            VehicleInfo[acc_id][i][eVehicle_Y] = 0.0;
            VehicleInfo[acc_id][i][eVehicle_Z] = 0.0;
            VehicleInfo[acc_id][i][eVehicle_A] = 0.0;
            DestroyVehicle(VehicleInfo[acc_id][i][eVehicle_Identifier]);
            DestroyDynamicArea(VehicleInfo[acc_id][i][eVehicle_DynamicZone]);
            VehicleInfo[acc_id][i][eVehicle_Identifier] = 0;
            VehicleInfo[acc_id][i][eVehicle_DynamicZone] = 0;
        }
    }

    DestroyDynamic3DTextLabel(GetUserData(playerid, pame_id));
    
    Iter_Clear(IterPlayerVehicles[acc_id]);
    SetUserData(playerid, u_id, 0);
    SetUserData(playerid, u_name, EOS);
    SetUserData(playerid, u_password, EOS);
    SetUserData(playerid, u_regdate, EOS);
    SetUserData(playerid, u_pame, EOS);
    SetUserData(playerid, u_referal, EOS);
    SetUserData(playerid, u_skin, 0);
    SetUserData(playerid, u_money, 0);
    SetUserData(playerid, u_gender, 0);
    SetUserData(playerid, u_exp, 0);
    SetUserData(playerid, u_score, 0);
    SetUserData(playerid, u_brightness, 0);
    SetUserData(playerid, u_satiety, 0);
    SetUserData(playerid, u_nationality, 0);
    SetUserData(playerid, u_mutetime, 0);
    SetUserData(playerid, pame_id, Text3D: INVALID_3DTEXT_ID);
    SetUserData(playerid, u_ismute, false);
    SetUserData(playerid, u_auth, false);

    save_password[playerid] = EOS;
    save_gender[playerid] = 0;
    save_nationality[playerid] = 0;
    g_player_health_bar[playerid] = Bar: INVALID_BAR_ID;
    g_player_armour_bar[playerid] = Bar: INVALID_BAR_ID;
    g_player_satiety_bar[playerid] = Bar: INVALID_BAR_ID;

    return 1;
}

stock MYSQL_TASK:SaveAccount(playerid) {
    new 
        query[] = 
        "\
            UPDATE users SET\n\
            password =\n\
                '%s',\n\
            skin =\n\
                %d,\n\
            money =\n\
                %d,\n\
            gender =\n\
                %d,\n\
            exp =\n\
                %d,\n\
            score =\n\
                %d,\n\
            satiety =\n\
                %f,\n\
            has_passport = \n\
                %d,\n\
            mute_time =\n\
                %d,\n\
            pame =\n\
                '%s',\n\
            idx_nationality =\n\
                %d,\n\
            referal = \n\
                '%s'\n\
            WHERE id = %d\n\
        ",
        size_query[sizeof query + (-2+MAX_PASSWORD_LEN) + (-2+3) + (-2+11) + (-2+1) + (-2+11) + (-2+4) + (-2+3) + (-2+1) + (-2+20) + (-2+MAX_PAME_LEN) + (-2+2) + (-2+MAX_PLAYER_NAME) + (-2+11)]
    ;

    format(size_query, sizeof size_query, query, 
        GetUserData(playerid, u_password), 
        GetUserData(playerid, u_skin), 
        GetUserData(playerid, u_money), 
        GetUserData(playerid, u_gender),
        GetUserData(playerid, u_exp),
        GetUserData(playerid, u_score),
        GetUserData(playerid, u_satiety),  
        GetUserData(playerid, u_has_passport),  
        GetUserData(playerid, u_mutetime),  
        GetUserData(playerid, u_pame),  
        GetUserData(playerid, u_nationality),  
        GetUserData(playerid, u_referal),  
        GetUserData(playerid, u_id)
    );

    return mysql_tquery(dbHandle, size_query);
}

stock MYSQL_TASK:CreateAccount(playerid, password[MAX_PASSWORD_LEN], gender, nationality_idx = 0, const referal_player[MAX_PLAYER_NAME] = "") {
    new
        sql_query[180 + (-2+MAX_PLAYER_NAME + MAX_PASSWORD_LEN + 3 + 11 + 1 + 3 + 39 + 20 + 2) * 8] = EOS
    ;

    new 
        name[MAX_PLAYER_NAME],
        ip[39],
        reg_date[20],
        year, month, day,
        hour, minute, second
    ;

    getdate(year, month, day);
    gettime(hour, minute, second);

    GetPlayerIp(playerid, ip, 39);
    GetPlayerName(playerid, name, MAX_PLAYER_NAME);
    format(reg_date, sizeof reg_date, "%02d/%02d/%d %02d:%02d:%02d", day, month, year, hour, minute, second);
    
    TogglePlayerSpectating(playerid, false);

    SetSpawnInfo(playerid, 0, 0, 0.0, 0.0, 0.0, 0.0, 0, 0, 0, 0, 0, 0);
    SpawnPlayer(playerid);
    SetCameraBehindPlayer(playerid);

    SetUserData(playerid, u_password, password);
    SetUserData(playerid, u_regdate, reg_date);
    SetUserData(playerid, u_name, name);
    SetUserData(playerid, u_money, 50);
    SetUserData(playerid, u_gender, gender);
    SetUserData(playerid, u_satiety, 100);
    SetUserData(playerid, u_nationality, nationality_idx);
    SetUserData(playerid, u_referal, referal_player);
    SetUserData(playerid, u_auth, true);

    new skin = ( GetUserData(playerid, u_gender) ) ? 21 : 38;

    format(sql_query, sizeof sql_query, 
    "\
        INSERT INTO `users` (\n\
        `name`,\n\
        `password`,\n\
        `skin`,\n\
        `money`,\n\
        `gender`,\n\
        `satiety`,\n\
        `reg_ip`,\n\
        `reg_date`,\n\
        `idx_nationality`,\n\
        `referal`) VALUES\n\
        ('%s',\n\
        '%s',\n\
        '%d',\n\
        '50',\n\
        %d,\n\
        '100',\n\
        '%s',\n\
        '%s',\n\
        '%d',\n\
        '%s') \n\
    ", 
        name, password, 
        skin, gender, 
        ip, reg_date, 
        nationality_idx, referal_player
    );

    SetUserData(playerid, u_skin, skin);
    SetPlayerSkin(playerid, skin);
    GivePlayerMoney(playerid, GetUserData(playerid, u_money));

    TogglePlayerControllable(playerid, false);
    SendClientMessage(playerid, -1, ""cGREEN"Francesco Schettino: Хэй! Подойди ко мне, я помогу тебе освоиться, и начать жить в штате.");
    SetTimerEx("MovePlayerCamera", 1000, false, "dd", playerid, 0);

    return mysql_pquery(dbHandle, sql_query, "UploadAccountID", "d", playerid);
}

/*
    Functions
*/
stock bool: IsValidNickname(playerid) {
    new 
        Regex: regex = Regex_New("^[A-Z][a-z]+_[A-Z][a-z]{1,3}[A-Z]?[a-z]*$"),
        nickname[MAX_PLAYER_NAME]
    ;

    GetPlayerName(playerid, nickname, MAX_PLAYER_NAME);

    return bool: Regex_Check(nickname, regex);
}

stock IsCorrectlyRegex(const text[], const string[]) {
    new Regex:regex = Regex_New(string);
    return Regex_Check(text, regex);
}

stock bool: IsPlayerMute(playerid) {
    new
        unix_mute_time = GetUserData(playerid, u_mutetime),
        unix_time = gettime()
    ;
    
    return (unix_mute_time < unix_time);
}

public: MovePlayerCamera(playerid, cam_type) {
    if (!cam_type) {
        InterpolateCameraPos(playerid, 1684.659667, -2333.333007, 14.106350, 1680.713989, -2327.210205, 15.095467, 3000);
        InterpolateCameraLookAt(playerid, 1684.710449, -2328.354003, 14.561875, 1676.053710, -2327.180419, 13.284148, 2000);
        SetTimerEx("MovePlayerCamera", 2000, false, "dd", playerid, 1);
    }

    if (cam_type) {
        TogglePlayerControllable(playerid, true);
        SetCameraBehindPlayer(playerid);
    }
}

public: UploadAccountID(playerid) return SetUserData(playerid, u_id, cache_insert_id());
public: OnPlayerSearch(playerid, search_type, const player_name[], const referal_player[MAX_PLAYER_NAME]) {
    new rows;
    cache_get_row_count(rows);
    
    switch (search_type) {
        case 0: { // register
            if (rows) {
                cache_get_value_name(0, "password", GetUserData(playerid, u_password));
                cache_get_value_name(0, "name", GetUserData(playerid, u_name));

                Dialog_Show(playerid, Dialog:DLG_LOGIN);
            } else Dialog_Show(playerid, Dialog:DLG_REG);

            return 1;
        }

        case 1: { // statistic
            if (rows) {
                new
                    money = 0,
                    id = 0,
                    exp = 0,
                    score = 0,
                    name[MAX_PLAYER_NAME] = EOS,
                    reg_date[MAX_REGDATE_LEN] = EOS,
                    reg_ip[39] = EOS
                ;

                cache_get_value_name_int(0, "id", id);
                cache_get_value_name(0, "name", name);
                cache_get_value_name(0, "reg_date", reg_date);
                cache_get_value_name(0, "reg_ip", reg_ip);
                cache_get_value_name_int(0, "money", money);
                cache_get_value_name_int(0, "exp", exp);
                cache_get_value_name_int(0, "score", score);

                new fmt[] = 
                    "\
                        \n\
                        "cWHITE"Никнейм \t\t"cBLUE"%s \n\
                        "cWHITE"UID \t\t\t"cBLUE"%d \n\
                        "cWHITE"Уровень \t\t"cBLUE"%d \n\
                        "cWHITE"Дата регистрации \t"cBLUE"%s \n\
                        "cWHITE"Опыт \t\t\t"cYELLOW"%d/%d EXP \n\
                        "cWHITE"Кошелёк \t\t"cGREEN"%d $ \n\
                        "cWHITE"REG-IP \t\t"cRED"%s \n\
                        \n\
                    ",
                    size_fmt[sizeof fmt + (-2+MAX_PLAYER_NAME) + (-2+11) + (-2+4) + (-2+20) + (-2+11) + (-2+11) + (-2+11) + (-2+11) + (-2+39)]
                ;

                format(size_fmt, sizeof size_fmt, fmt, 
                    name,
                    id, 
                    score,
                    reg_date, 
                    exp, 
                    (score + 1) * 4, 
                    money,
                    reg_ip
                );

                return Dialog_Open(playerid, Dialog:DLG_PLAYER_STATS, DIALOG_STYLE_MSGBOX, ""PROJECT_NAME"Статистика игрока. "cRED"(OFFLINE)", size_fmt, ""cWHITE"Закрыть", "");
            }

            return SendClientMessage(playerid, -1, ""cGREY"Этот игрок не найден в базе данных.");
        }

        case 2: { // referal
            if (rows) {
                SendClientMessage(playerid, -1, ""cGREY"Спасибо, что указали реферал при регистрации. При достижении пятого уровня Вы получите вознаграждение!");

                new password[MAX_PASSWORD_LEN];
                format(password, sizeof password, save_password[playerid]);

                return MYSQL_TASK:CreateAccount(playerid, password, save_gender[playerid], save_nationality[playerid], referal_player);
            }

            SendClientMessage(playerid, -1, ""cGREY"Этот игрок не найден в базе данных");
            Dialog_Show(playerid, Dialog:DLG_INPUT_REFERAL_PLAYER);
        }
    }

    return 1;
}

public: LoadPlayerAccount(playerid) {
    new rows;
    cache_get_row_count(rows);

    if (rows) {
        new
            skin = 0,
            money = 0,
            id = 0,
            gender = 0,
            exp = 0,
            score = 0,
            Float: satiety = 0.0,
            has_passport = 0,
            mute_time = 0,
            idx_nationality = 0,
            name[MAX_PLAYER_NAME] = EOS,
            // password[32] = EOS,
            sql_query[512] = EOS,
            reg_date[MAX_REGDATE_LEN] = EOS,
            pame[MAX_PAME_LEN] = EOS,
            referal[MAX_PLAYER_NAME] = EOS
        ;

        cache_get_value_name_int(0, "skin", skin);
        cache_get_value_name_int(0, "id", id);
        cache_get_value_name(0, "name", name);
        // cache_get_value_name(0, "password", password);
        cache_get_value_name(0, "reg_date", reg_date);
        cache_get_value_name(0, "pame", pame);
        cache_get_value_name(0, "referal", referal);
        cache_get_value_name_int(0, "money", money);
        cache_get_value_name_int(0, "gender", gender);
        cache_get_value_name_int(0, "exp", exp);
        cache_get_value_name_int(0, "score", score);
        cache_get_value_name_int(0, "idx_nationality", idx_nationality);
        cache_get_value_name_int(0, "has_passport", has_passport);
        cache_get_value_name_float(0, "satiety", satiety);
        cache_get_value_name_int(0, "mute_time", mute_time);

        SetUserData(playerid, u_id, id);
        SetUserData(playerid, u_name, name);
        // SetUserData(playerid, u_password, password);
        SetUserData(playerid, u_regdate, reg_date); 
        SetUserData(playerid, u_referal, referal); 
        SetUserData(playerid, u_pame, pame); 
        SetUserData(playerid, u_skin, skin);
        SetUserData(playerid, u_money, money);
        SetUserData(playerid, u_gender, gender);
        SetUserData(playerid, u_exp, exp);
        SetUserData(playerid, u_score, score);
        SetUserData(playerid, u_nationality, idx_nationality);
        SetUserData(playerid, u_satiety, satiety);
        SetUserData(playerid, u_has_passport, has_passport);
        SetUserData(playerid, u_mutetime, mute_time);
        SetUserData(playerid, u_auth, true);

        /*if (IsPlayerMute(playerid)) {
            SetUserData(playerid, u_ismute, false);
            SendClientMessage(playerid, -1, ""cGREY"Блокировка чата для Вас истекла. Теперь Вы можете писать в чат.");
        }*/

        if (pame[0] != EOS) {
            new Text3D: label_id = CreateDynamic3DTextLabel(pame, -1, 0.0, 0.0, 0.0, 8.0, playerid);
            SetUserData(playerid, pame_id, label_id);
        }

        GivePlayerMoney(playerid, money);
        SetPlayerSkin(playerid, skin);

        SetPlayerScore(playerid, score);
        
        TogglePlayerSpectating(playerid, false);

        SetSpawnInfo(playerid, 0, skin, 0.0, 0.0, 0.0, 0.0, 0, 0, 0, 0, 0, 0);
        SpawnPlayer(playerid);

        // UpdatePlayerBar(playerid, 0);
        // UpdatePlayerBar(playerid, 1);
        // UpdatePlayerBar(playerid, 2);

        // -- Load other data
        format(sql_query, sizeof sql_query, "SELECT id, price, is_loaded, locked, model, color1, color2, car_x, car_y, car_z, car_a FROM vehicles WHERE account_id = '%d' LIMIT 100", 
            GetUserData(playerid, u_id)
        );
        mysql_pquery(dbHandle, sql_query, "OnLoadPlayerVehicles", "d", GetUserData(playerid, u_id));
        sql_query[0] = '\0';

        format(sql_query, sizeof sql_query, "SELECT * FROM admins WHERE account_id = '%d'", 
            GetUserData(playerid, u_id)
        );
        mysql_pquery(dbHandle, sql_query, "OnLoadAdmins", "dd", playerid, GetUserData(playerid, u_id));
        sql_query[0] = '\0';
    }

    return 1;
}

public: KickPlayer(playerid) {
    return Kick(playerid);
}

/*
    Hooks
*/
hook OnPlayerDisconnect(playerid, reason) {
    MYSQL_TASK:SaveAccount(playerid);
    Players:ClearPlayerData(playerid);
    return true;
}

hook OnPlayerConnect(playerid) {
    if (!IsValidNickname(playerid)) {
		new dialog_string[] = 
			"\
				\n\
				"cWHITE"Вы были кикнуты с сервера за "cRED"неправильный "cWHITE"формат никнейма. \n\
				"cWHITE"Формат должен быть в виде "cBLUE"\"Name_Surname\" \n\
				\n\
				"cWHITE"Пожалуйста, перепроверьте формальность никнейма и снова зайдите на сервер! \n\
				\n\
			"
		;

		Dialog_Open(playerid, Dialog:NICKNAME_VALID, DIALOG_STYLE_MSGBOX, ""PROJECT_NAME"Валидность никнейма", dialog_string, ""cWHITE"Закрыть", "");
		return SetTimerEx("KickPlayer", 2000, false, "d", playerid);
	}

    TogglePlayerSpectating(playerid, true);
    SetPlayerSyncModels(playerid, true);

    new 
        name[MAX_PLAYER_NAME] = EOS,
        sql_query[38 + MAX_PLAYER_NAME] = EOS
    ;

    GetPlayerName(playerid, name, MAX_PLAYER_NAME);
    format(sql_query, sizeof sql_query, "SELECT * FROM users WHERE name = '%s'", name);

    return mysql_pquery(dbHandle, sql_query, !"OnPlayerSearch", "ddss", playerid, 0, name, name);
}

hook OnPlayerSpawn(playerid) {
    SetPlayerPos(playerid, 1685.7618, -2334.2390, 13.5469);
    SetPlayerFacingAngle(playerid, 359.6105);

    return 1;
}

/*
    Create Dialog Content
*/
DialogCreate:DLG_INPUT_REFERAL_PLAYER(playerid) {
    return Dialog_Open(playerid, Dialog:DLG_INPUT_REFERAL_PLAYER, DIALOG_STYLE_INPUT, 
        ""PROJECT_NAME"Ввод реферала.",
        "\
            "cWHITE"Следующим шагом регистрации будет ввод пригласившего игрока Вас на сервер. \n\
            "cWHITE"Если Вы знайте такого, то можете ввести его в поле ниже. \n\
            \n\
            "cWHITE"При достижении "cBLUE"пятого "cWHITE"уровня Вы получите "cGREEN"3.000 $ \n\
        ",
        ""cWHITE"Продолжить",
        ""cWHITE"Пропустить"
    );
}

DialogCreate:DLG_FOUND_SAMP_SERVER(playerid) {
    return Dialog_Open(playerid, Dialog:DLG_FOUND_SAMP_SERVER, DIALOG_STYLE_LIST, 
        ""PROJECT_NAME"Откуда узнали о проекте?", 
        "\
            "cBLUE"1. "cWHITE"От друзей. \n\
            "cBLUE"2. "cWHITE"От ютуб-партнёра. \n\
            "cBLUE"3. "cWHITE"Социальные сети. \n\
            "cBLUE"4. "cWHITE"Посоветовали проект. \n\
            "cBLUE"5. "cWHITE"Другое... \n\
        ", 
        ""cWHITE"Выбрать", 
        ""cWHITE"Контекст"
    );
}

DialogCreate:DLG_SET_PAME(playerid) {
    new fmt[] = 
        "\
            \n\
            "cWHITE"Введите Ваше новое "cBLUE"описание персонажа. \n\
            \n\
            "cWHITE"Текущее описание: \n\
            "cGREY"%s \n\
            \n\
        ",
        fmt_size[sizeof fmt + (-2+MAX_PAME_LEN)]
    ;

    new pame[MAX_PAME_LEN];
    format(pame, sizeof pame, GetUserData(playerid, u_pame));

    format(fmt_size, sizeof fmt_size, fmt, GetUserData(playerid, u_pame) == EOS ? "ОТСУТСТВУЕТ" : pame);
    return Dialog_Open(playerid, Dialog:DLG_SET_PAME, DIALOG_STYLE_INPUT, ""PROJECT_NAME"Описание персонажа", fmt_size, ""cWHITE"Установить", ""cWHITE"Назад");
}

DialogCreate:DLG_LOGIN(playerid) {
    new fmt[] = 
        "\
            \n\
            "cWHITE"Приветствуем Вас на нашем сервере: "cBLUE"Project RP. \n\
            "cWHITE"Этот аккаунт "cRED"занят, "cWHITE"просим Вас ввести пароль от этого аккаунта. \n\
            "cWHITE"Если же Вы не являйтесь владельцем этого аккаунта, то выйдите из игры, \n\
            "cWHITE"и зарегистрируйтесь на свободный никнейм. \n\
            \n\
            "cWHITE"Помните, что администрация никогда не попросит пароль от Вашего аккаунта. \n\
            "cWHITE"За безопасность своего аккаунта отвечайте только Вы. \n\
            \n\
        "
    ;

    Dialog_Open(playerid, Dialog:DLG_LOGIN, DIALOG_STYLE_PASSWORD, ""PROJECT_NAME"Авторизация на сервере.", 
        fmt,
        ""cWHITE"Далее", 
        ""cWHITE"Контекст"
    );
}

DialogCreate:DLG_REG(playerid) {
    new fmt[] = 
        "\
            \n\
            "cWHITE"Приветствуем Вас на нашем сервере: "cBLUE"Project RP. \n\
            "cWHITE"Этот аккаунт свободен, введите Ваш будущий пароль, чтобы продолжишь игру на сервере. \n\
            "cWHITE"Помните, что администрация никогда не попросит пароль от Вашего аккаунта. \n\
            "cWHITE"За безопасность своего аккаунта отвечайте только Вы. \n\
            \n\
        "
    ;

    Dialog_Open(playerid, Dialog:DLG_REG, DIALOG_STYLE_PASSWORD, ""PROJECT_NAME"Авторизация на сервере.", 
        fmt,
        ""cWHITE"Далее", 
        ""cWHITE"Контекст"
    );
}

DialogCreate:DLG_SELECT_GENDER(playerid) {
    new fmt[] = 
        "\
            \n\
            "cWHITE"Следующим шагом Вам предстоит выбрать пол вашего персонажа. \n\
            "cWHITE"Вы можете выбрать "cBLUE"мужской "cWHITE"пол, или же "cRED"женский. \n\
            \n\
        "
    ;

    Dialog_Open(playerid, Dialog:DLG_SELECT_GENDER, DIALOG_STYLE_MSGBOX, ""PROJECT_NAME"Выбор пола.", fmt, ""cWHITE"Мужской", ""cWHITE"Женский");
}

DialogCreate:DLG_MENU(playerid) {
    new fmt[] = 
        "\
            "cBLUE"1. "cWHITE"Статистика персонажа. \n\
            "cBLUE"2. "cWHITE"Связь с администрацией. \n\
            "cBLUE"3. "cWHITE"Безопасность аккаунта. \n\
            "cBLUE"4. "cWHITE"Настройка аккаунта. \n\
            "cBLUE"5. "cWHITE"Помощь по серверу. \n\
            "cBLUE"6. "cWHITE"Поиск локаций "cBLUE"(GPS)"cWHITE". \n\
            "cBLUE"7. "cWHITE"Состояние здоровья вашего персонажа. \n\
            "cBLUE"8. "cWHITE"Навыки вашего персонажа. \n\
            "cBLUE"9. "cWHITE"Ежедневные задания. \n\
            "cBLUE"10. "cWHITE"Физические способности вашего персонажа. \n\
            "cBLUE"11. "cWHITE"Донат услуги. \n\
        "
    ;

    Dialog_Open(playerid, Dialog:DLG_MENU, DIALOG_STYLE_LIST, ""PROJECT_NAME"Меню сервера", fmt, ""cWHITE"Выбрать", ""cWHITE"Закрыть");
}

DialogCreate:DLG_ACCOUNT_SETTINGS(playerid) {
    new fmt[] = 
        "\
            "cBLUE"1. "cWHITE"Установить описание персонажа. \n\
            "cBLUE"2. "cWHITE"Удалить описание персонажа. \n\
        "
    ;

    Dialog_Open(playerid, Dialog:DLG_ACCOUNT_SETTINGS, DIALOG_STYLE_LIST, ""PROJECT_NAME"Настройки аккаунта", fmt, ""cWHITE"Выбрать", ""cWHITE"Назад");
}

DialogCreate:DLG_GPS(playerid) {
    new fmt[] = 
        "\
            "cBLUE"1. "cWHITE"Основные локации. \n\
            "cBLUE"2. "cWHITE"Локации по работам. \n\
        "
    ;

    Dialog_Open(playerid, Dialog:DLG_GPS, DIALOG_STYLE_LIST, ""PROJECT_NAME"Локации сервера", fmt, ""cWHITE"GPS", ""cWHITE"Назад");
}

DialogCreate:DLG_SERVER_HELP(playerid) {
    new fmt[] = 
        "\
            "cBLUE"1. "cWHITE"Основные аспекты сервера. \n\
            "cBLUE"2. "cWHITE"Помощь по командам. \n\
            "cBLUE"3. "cWHITE"Как обезопасть свой аккаунт? \n\
            "cBLUE"4. "cWHITE"Как обезопаситься от мошенников? \n\
        "
    ;

    Dialog_Open(playerid, Dialog:DLG_SERVER_HELP, DIALOG_STYLE_LIST, ""PROJECT_NAME"Помощь по серверу", fmt, ""cWHITE"Выбрать", ""cWHITE"Назад");
}

DialogCreate:DLG_MAIN_ASPECTS(playerid) {
    new fmt[] = 
        "\
            "cBLUE"1. "cWHITE"Как получить гражданство в штате? \n\
            "cBLUE"2. "cWHITE"Как заработать первые деньги? \n\
            "cBLUE"3. "cWHITE"Как открыть счёт накоплений в банке? \n\
            "cBLUE"4. "cWHITE"Как открыть счёт в банке? \n\
            "cBLUE"5. "cWHITE"Что будет с моим имуществом, если я долго не буду заходить на аккаунт? \n\
        "
    ;

    Dialog_Open(playerid, Dialog:DLG_MAIN_ASPECTS, DIALOG_STYLE_LIST, ""PROJECT_NAME"Основные аспекты игры", fmt, ""cWHITE"Выбрать", ""cWHITE"Назад");
}

DialogCreate:DLG_SELECT_NATIONALITY(playerid) {
    new dialog_string[sizeof g_types_nationality[] * 9];

    for (new i; i < sizeof g_types_nationality; i++) {
        format(dialog_string, sizeof dialog_string, "%s%s\n", dialog_string, g_types_nationality[i]);
    }

    return Dialog_Open(playerid, Dialog:DLG_SELECT_NATIONALITY, DIALOG_STYLE_LIST, ""PROJECT_NAME"Выбор национальности персонажа", dialog_string, ""cWHITE"Выбрать", ""cWHITE"Контекст");
}

/*
    Dialog Response
*/
DialogResponse:DLG_INPUT_REFERAL_PLAYER(playerid, response, listitem, inputtext[]) { // финальный вариант регистрации
    new password[MAX_PASSWORD_LEN];
    
    if (!response) {
        format(password, sizeof password, save_password[playerid]);

        return MYSQL_TASK:CreateAccount(playerid, password, save_gender[playerid], save_nationality[playerid]);
    }

    new 
        referal_player[MAX_PLAYER_NAME],
        query[] = "SELECT * FROM users WHERE name = '%s' LIMIT 1",
        query_size[sizeof query + (-2+MAX_PLAYER_NAME)]
    ;

    if (sscanf(inputtext, "s[24]", referal_player))
        return Dialog_Show(playerid, Dialog:DLG_INPUT_REFERAL_PLAYER);

    if (!IsCorrectlyRegex(referal_player, "^[A-Z][a-z]+_[A-Z][a-z]{1,3}[A-Z]?[a-z]*$")) {
        SendClientMessage(playerid, -1, ""cGREY"Некорректный ввод реферала. Необходимый формат: Name_Surname или Name_PreSurname");
        return Dialog_Show(playerid, Dialog:DLG_INPUT_REFERAL_PLAYER);
    }

    format(password, sizeof password, save_password[playerid]);
    format(query_size, sizeof query_size, query, referal_player);

    return mysql_pquery(dbHandle, query_size, !"OnPlayerSearch", "ddss", playerid, 2, referal_player, referal_player);
}

DialogResponse:DLG_FOUND_SAMP_SERVER(playerid, response, listitem, inputtext[]) {
    if (!response)
        return Kick(playerid);

    new password[MAX_PASSWORD_LEN];
    format(password, sizeof password, save_password[playerid]);

    // return Dialog_Show(playerid, Dialog:DLG_INPUT_REFERAL_PLAYER);
    return MYSQL_TASK:CreateAccount(playerid, password, save_gender[playerid], save_nationality[playerid]);
}

DialogResponse:DLG_SELECT_NATIONALITY(playerid, response, listitem, inputtext[]) {
    if (!response)
        return Kick(playerid);

    save_nationality[playerid] = listitem;
    return Dialog_Show(playerid, Dialog:DLG_FOUND_SAMP_SERVER);
}

DialogResponse:DLG_SET_PAME(playerid, response, listitem, inputtext[]) {
    if (!response)
        return Dialog_Show(playerid, Dialog:DLG_ACCOUNT_SETTINGS);

    new 
        pame[MAX_PAME_LEN],
        dialog_string[] = 
        "\
            "cGREY"Вы успешно установили новое описание Вашего персонажа \n\
            "cGREY"%s \n\
        ",
        dialog_size[sizeof dialog_string + (-2+MAX_PAME_LEN)]
    ;

    if (sscanf(inputtext, "s[128]", pame))
        return Dialog_Show(playerid, Dialog:DLG_SET_PAME);

    if (strlen(pame) < 6) {
        Dialog_Show(playerid, Dialog:DLG_SET_PAME);
        return SendClientMessage(playerid, -1, ""cGREY"Минимальная длина описания персонажа должна составлять 6 символов.");
    }

    if (strlen(pame) >= 128) {
        Dialog_Show(playerid, Dialog:DLG_SET_PAME);
        return SendClientMessage(playerid, -1, ""cGREY"Максимальная длина описания персонажа должна составлять не более 128-и символов.");
    }

    if (GetUserData(playerid, u_pame) != EOS) {
        Dialog_Show(playerid, Dialog:DLG_SET_PAME);
        return SendClientMessage(playerid, -1, ""cGREY"У Вас уже установлено описание персонажа.");
    }

    format(dialog_size, sizeof dialog_size, dialog_string, pame);

    DestroyDynamic3DTextLabel(GetUserData(playerid, pame_id));
    if (pame[0] != EOS) {
        new Text3D: label_id = CreateDynamic3DTextLabel(pame, -1, 0.0, 0.0, 0.0, 8.0, playerid);
        SetUserData(playerid, u_pame, pame);
        SetUserData(playerid, pame_id, label_id);
    }

    return Dialog_Open(playerid, Dialog:DLG_PAME_VIEW, DIALOG_STYLE_MSGBOX, ""PROJECT_NAME"Описание персонажа", dialog_size, ""cWHITE"Закрыть", "");
}

DialogResponse:DLG_ACCOUNT_SETTINGS(playerid, response, listitem, inputtext[]) {
    if (!response)
        return Dialog_Show(playerid, Dialog:DLG_MENU);

    switch(listitem) {
        case 0: Dialog_Show(playerid, Dialog:DLG_SET_PAME);
        case 1: {

            if (GetUserData(playerid, u_pame) == EOS) {
                Dialog_Show(playerid, Dialog:DLG_ACCOUNT_SETTINGS);
                return SendClientMessage(playerid, -1, ""cGREY"У Вас не установлено описание.");
            }

            SetUserData(playerid, u_pame, EOS);

            SendClientMessage(playerid, -1, MESSAGE_DELETE_PAME);
            DestroyDynamic3DTextLabel(GetUserData(playerid, pame_id));

            Dialog_Show(playerid, Dialog:DLG_ACCOUNT_SETTINGS);
        }
    }

    return 1;
}

DialogResponse:DLG_MAIN_ASPECTS(playerid, response, listitem, inputtext[]) {
    if (!response)
        return Dialog_Show(playerid, Dialog:DLG_SERVER_HELP);

    switch(listitem) {
        case 0: {
            new const fmt[] = 
                "\
                    "cWHITE"Паспорт - одна из важных вещей каждого гражданина штата "cBLUE"San-Andreas. \n\
                    "cWHITE"Чтобы получить гражданство, Вам необходимо прибыть в гражданское посольство. \n\
                    "cWHITE"Далее, там зарегистрироваться как житель штата "cBLUE"San-Andreas, "cWHITE"и получить свой документ. \n\
                    "cWHITE"\n\
                    "cWHITE"Без наличия паспорта Вы не сможете работать и зарабатывать деньги. \n\
                    "cWHITE"Чтобы вступать в какие-либо организации, Вам понадобится также документ удостоверяющий Вашу личность. \n\
                "
            ;

            Dialog_Open(playerid, Dialog:DLG_HOW_GET_DOCUMENT, DIALOG_STYLE_MSGBOX, ""PROJECT_NAME"Как получить гражданство?", fmt, ""cWHITE"Закрыть", "");
        }

        case 1: {
            new const fmt[] = 
                "\
                    "cWHITE"Чтобы заработать денег, в первую очередь, Вам нужен документ удостоверяющий Вашу личность. \n\
                    "cWHITE"Заработать денег может любой новичок. \n\
                    "cWHITE"Подробнее /gps - Локации по работам. \n\
                "
            ;

            Dialog_Open(playerid, Dialog:DLG_HOW_TO_EARN, DIALOG_STYLE_MSGBOX, ""PROJECT_NAME"Как заработать в штате?", fmt, ""cWHITE"Закрыть", "");
        }

        case 4: {
            new const fmt[] = 
                "\
                    "cWHITE"Каждый гражданин штата "cBLUE"San-Andreas "cWHITE"обязан выплачивать налоги, и платить за имущество. \n\
                    "cWHITE"Если Вы не будете этого делать, то Ваше имущество изымает государство, и к Вам возвращается определённый процент от продажи имущества. \n\
                "
            ;

            Dialog_Open(playerid, Dialog:DLG_PAYMENT, DIALOG_STYLE_MSGBOX, ""PROJECT_NAME"Оплата имущества, налоги", fmt, ""cWHITE"Закрыть", "");
        }

        default: Dialog_Show(playerid, Dialog:DLG_MAIN_ASPECTS);
    }

    return 1;
}

DialogResponse:DLG_SERVER_HELP(playerid, response, listitem, inputtext[]) {
    if (!response)
        return Dialog_Show(playerid, Dialog:DLG_MENU);

    switch(listitem) {
        case 0: {
            Dialog_Show(playerid, Dialog:DLG_MAIN_ASPECTS);
        }

        default: Dialog_Show(playerid, Dialog:DLG_SERVER_HELP);
    }
    return response;
}

DialogResponse:DLG_GPS(playerid, response, listitem, inputtext[]) {
    if (!response)
        return Dialog_Show(playerid, Dialog:DLG_MENU);
    
    return response;
}

DialogResponse:DLG_SELECT_GENDER(playerid, response, listitem, inputtext[]) {
    new password[32];
    format(password, sizeof password, save_password[playerid]);
    
    save_gender[playerid] = response;
    return Dialog_Show(playerid, Dialog:DLG_SELECT_NATIONALITY);
}

DialogResponse:DLG_MENU(playerid, response, listitem, inputtext[]) {
    if (!response) return true;

    switch(listitem) {
        case DIALOG_MENU_STATS: {
            new fmt[] = 
                "\
                    "cYELLOW"Общая информация\n\
                    "cWHITE"\n\
                    "cWHITE"Никнейм: \t\t"cBLUE"%s \n\
                    "cWHITE"UID: \t\t\t"cBLUE"%d \n\
                    "cWHITE"Уровень: \t\t"cBLUE"%d \n\
                    "cWHITE"Дата регистрации: \t"cBLUE"%s \n\
                    "cWHITE"Опыт: \t\t\t"cYELLOW"%d/%d EXP \n\
                    "cWHITE"\n\
                    "cYELLOW"Накопления\n\
                    "cWHITE"\n\
                    "cWHITE"Кошелёк: \t\t"cGREEN"%d $ \n\
                    "cWHITE"Счёт в банке: \t\t"cGREEN"1000 $ \n\
                    "cWHITE"Депозит: \t\t"cGREEN"5000 $ \n\
                ",
                player_name[MAX_PLAYER_NAME],
                size_fmt[sizeof fmt + (-2+MAX_PLAYER_NAME) + (-2+11) + (-2+4) + (-2+20) + (-2+11) + (-2+11) + (-2+11) + (-2+11)]
            ;

            GetPlayerName(playerid, player_name, MAX_PLAYER_NAME);
            format(size_fmt, sizeof size_fmt, fmt, 
                player_name, 
                GetUserData(playerid, u_id), 
                GetUserData(playerid, u_score),
                GetUserData(playerid, u_regdate), 
                GetUserData(playerid, u_exp), 
                (GetUserData(playerid, u_score) + 1) * 4, 
                GetUserData(playerid, u_money)
            );

            if (Admins_IsPlayerAdmin(playerid)) {
                strcat(size_fmt, !""cWHITE"\nСтатус: \t\t"cBLUE"Администратор");
            }

            return Dialog_Open(playerid, Dialog:DLG_PLAYER_STATS, DIALOG_STYLE_MSGBOX, ""PROJECT_NAME"Статистика персонажа.", size_fmt, ""cWHITE"Закрыть", ""cWHITE"Назад");
        }

        case DIALOG_MENU_COMMUNICATION_ADMINS: {
            return SendClientMessage(playerid, -1, ""COMMAND_USAGE_FORMAT"/report [сообщение]");
        }

        case DIALOG_MENU_ACCOUNT_GUARD: {
            return 1;
        }

        case DIALOG_MENU_ACCOUNT_SETTINGS: Dialog_Show(playerid, Dialog:DLG_ACCOUNT_SETTINGS);

        case DIALOG_MENU_SERVER_HELP: {
            Dialog_Show(playerid, Dialog:DLG_SERVER_HELP);
            return 1;
        }

        case DIALOG_MENU_SERVER_GPS: {
            Dialog_Show(playerid, Dialog:DLG_GPS);
            return 1;
        }

        case DIALOG_MENU_HEALTH_STATUS: {
            new fmt[] = 
                "\
                    "cYELLOW"Здоровье — "cGREEN"Стабильное\n\
                    "cWHITE"\n\
                    "cWHITE"Сытость — "cYELLOW"%d %%\n\
                    "cWHITE"\n\
                ",
            size_fmt[sizeof fmt + (-2+3)];
            
            format(size_fmt, sizeof size_fmt, fmt, 
                GetUserData(playerid, u_satiety)
            );

            return Dialog_Open(playerid, Dialog:DLG_PLAYER_STATS, DIALOG_STYLE_MSGBOX, ""PROJECT_NAME"Состояние здоровья.", size_fmt, ""cWHITE"Закрыть", "");
        }

        case DIALOG_MENU_CHARACTER_SKILLS: {
            return 1;
        }
        
        case DIALOG_MENU_DAILY_TASKS: {
            return 1;
        }

        case DIALOG_MENU_PHYSICAL_ABILITY: {
            return 1;
        }

        case DIALOG_MENU_DONATE: {
            return 1;
        }

        default: Dialog_Show(playerid, Dialog:DLG_MENU);
    }

    return true;
}

DialogResponse:DLG_LOGIN(playerid, response, listitem, inputtext[]) {
    if (!response)
        return Kick(playerid);

    new password[32] = EOS;

    if (sscanf(inputtext, "s[32]", password))
        return Dialog_Show(playerid, Dialog:DLG_LOGIN);

    if (!IsCorrectlyRegex(password, "^[-A-Za-z0-9!@#$^&*()_+[\\];\\\\<>,.\\/?~]{6,32}$")) {
        SendClientMessage(playerid, -1, ""cGREY"Пароль должен состоять из латинских букв, цифр (0-9) и специальных знаков (#@!%%^&*), а длина текста от 6 до 32 символов.");
        return Dialog_Show(playerid, Dialog:DLG_LOGIN);
    }

    if (!strcmp(GetUserData(playerid, u_password), password)) {
        new sql_query[38 + MAX_PLAYER_NAME] = EOS;
        new name[MAX_PLAYER_NAME] = EOS;
        GetPlayerName(playerid, name, MAX_PLAYER_NAME);
        format(sql_query, sizeof sql_query, "SELECT * FROM users WHERE name = '%s'", name);
        mysql_pquery(dbHandle, sql_query, !"LoadPlayerAccount", !"d", playerid);
    } else {
        Dialog_Show(playerid, Dialog:DLG_LOGIN);
    }

    return 1;
}

DialogResponse:DLG_REG(playerid, response, listitem, inputtext[]) {
    if (!response)
        return Kick(playerid);

    new password[32];

    if (sscanf(inputtext, "s[32]", password))
        return Dialog_Show(playerid, Dialog:DLG_REG);

    if (!IsCorrectlyRegex(password, "^[-A-Za-z0-9!@#$^&*()_+[\\];\\\\<>,.\\/?~]{6,32}$")) {
        SendClientMessage(playerid, -1, ""cGREY"Пароль должен состоять из латинских букв, цифр (0-9) и специальных знаков (#@!%%^&*), а длина текста от 6 до 32 символов.");
        return Dialog_Show(playerid, Dialog:DLG_REG);
    }

    format(save_password[playerid], 32, password);
    return Dialog_Show(playerid, Dialog:DLG_SELECT_GENDER);
}

/*
    Commands
*/
cmd:menu(playerid) {
    return Dialog_Show(playerid, Dialog:DLG_MENU);
}

cmd:b(playerid, params[]) {
    new message[64], string_message[124], player_name[MAX_PLAYER_NAME];
    if (sscanf(params, "s[64]", message))
        return SendClientMessage(playerid, -1, ""COMMAND_USAGE_FORMAT"/b [сообщение]");

    if (strlen(message) > 64)
        return SendClientMessage(playerid, -1, ""cGREY"Вы превысили максимальную длину сообщения в этот чат.");

    GetPlayerName(playerid, player_name, MAX_PLAYER_NAME);

    format(string_message, sizeof string_message, ""cGREY_BRIGHT"(( %s: %s ))", player_name, message);

    return ProxDetector(playerid, 20.0, -1, string_message);
}

cmd:sb(playerid, params[]) {
    new message[64], string_message[124], player_name[MAX_PLAYER_NAME];
    if (sscanf(params, "s[64]", message))
        return SendClientMessage(playerid, -1, ""COMMAND_USAGE_FORMAT"/sb [сообщение]");

    if (strlen(message) > 64)
        return SendClientMessage(playerid, -1, ""cGREY"Вы превысили максимальную длину сообщения в этот чат.");

    GetPlayerName(playerid, player_name, MAX_PLAYER_NAME);

    format(string_message, sizeof string_message, ""cGREY_BRIGHT"(( %s: %s ))", player_name, message);
    return SetPlayerChatBubble(playerid, string_message, -1, 16.0, 1000);
}

cmd:me(playerid, params[]) {
    new message[64], string_message[124], player_name[MAX_PLAYER_NAME];
    if (sscanf(params, "s[64]", message))
        return SendClientMessage(playerid, -1, ""COMMAND_USAGE_FORMAT"/me [действие]");

    if (strlen(message) > 64)
        return SendClientMessage(playerid, -1, ""cGREY"Вы превысили максимальную длину сообщения в этот чат.");

    GetPlayerName(playerid, player_name, MAX_PLAYER_NAME);

    format(string_message, sizeof string_message, "* %s %s", player_name, message);

    return ProxDetector(playerid, 20.0, cPINK, string_message);
}

cmd:ame(playerid, params[]) {
    new message[64], string_message[124], player_name[MAX_PLAYER_NAME];
    if (sscanf(params, "s[64]", message))
        return SendClientMessage(playerid, -1, ""COMMAND_USAGE_FORMAT"/ame [действие]");

    if (strlen(message) > 64)
        return SendClientMessage(playerid, -1, ""cGREY"Вы превысили максимальную длину сообщения в этот чат.");

    GetPlayerName(playerid, player_name, MAX_PLAYER_NAME);

    format(string_message, sizeof string_message, "* %s %s", player_name, message);
    return SetPlayerChatBubble(playerid, string_message, cPINK, 16.0, 1000);
}

cmd:do(playerid, params[]) {
    new message[64], string_message[124], player_name[MAX_PLAYER_NAME];
    if (sscanf(params, "s[64]", message))
        return SendClientMessage(playerid, -1, ""COMMAND_USAGE_FORMAT"/do [3-е лицо]");

    if (strlen(message) > 64)
        return SendClientMessage(playerid, -1, ""cGREY"Вы превысили максимальную длину сообщения в этот чат.");

    GetPlayerName(playerid, player_name, MAX_PLAYER_NAME);

    format(string_message, sizeof string_message, "%s (( %s ))", player_name, message);
    return ProxDetector(playerid, 20.0, cPINK, string_message);
}

cmd:ado(playerid, params[]) {
    new message[64], string_message[124], player_name[MAX_PLAYER_NAME];
    if (sscanf(params, "s[64]", message))
        return SendClientMessage(playerid, -1, ""COMMAND_USAGE_FORMAT"/ado [3-е лицо]");

    if (strlen(message) > 64)
        return SendClientMessage(playerid, -1, ""cGREY"Вы превысили максимальную длину сообщения в этот чат.");

    GetPlayerName(playerid, player_name, MAX_PLAYER_NAME);

    format(string_message, sizeof string_message, "%s (( %s ))", player_name, message);
    return SetPlayerChatBubble(playerid, string_message, cPINK, 16.0, 1000);
}

cmd:shout(playerid, params[]) {
    new message[64], string_message[124], player_name[MAX_PLAYER_NAME];
    if (sscanf(params, "s[64]", message))
        return SendClientMessage(playerid, -1, ""COMMAND_USAGE_FORMAT"/s(hout) [текст]");

    if (strlen(message) > 64)
        return SendClientMessage(playerid, -1, ""cGREY"Вы превысили максимальную длину сообщения в этот чат.");

    GetPlayerName(playerid, player_name, MAX_PLAYER_NAME);
    if (GetUserData(playerid, u_gender)) {
        format(string_message, sizeof string_message, ""cGREEN"%s крикнул: %s", player_name, message);
    } else {
        format(string_message, sizeof string_message, ""cGREEN"%s крикнула: %s", player_name, message);
    }

    ApplyAnimation(playerid, "ON_LOOKERS", "shout_01", 4.1, 0, 0, 0, 0, 0); 
    ApplyAnimation(playerid, "ON_LOOKERS", "shout_01", 4.1, 0, 0, 0, 0, 0); 
    return ProxDetector(playerid, 60.0, -1, string_message);
}

cmd:gps(playerid) {
    return Dialog_Show(playerid, Dialog:DLG_GPS);
}

cmd:debug(playerid) {
    return Dialog_Show(playerid, Dialog:DLG_SELECT_NATIONALITY);
}

cmd:satiety(playerid) {
    new fmt[] = 
        "\
            "cYELLOW"Здоровье — "cGREEN"Стабильное\n\
            "cWHITE"\n\
            "cWHITE"Сытость — "cYELLOW"%f\n\
            "cWHITE"\n\
        ",
    size_fmt[sizeof fmt + (-2+16)];
    
    format(size_fmt, sizeof size_fmt, fmt, 
        GetUserData(playerid, u_satiety)
    );

    return Dialog_Open(playerid, Dialog:DLG_PLAYER_STATS, DIALOG_STYLE_MSGBOX, ""PROJECT_NAME"Состояние здоровья.", size_fmt, ""cWHITE"Закрыть", "");
}

CMD:id(playerid, params[])
{
    #if !defined COMMAND_ID_MAX_MATCHES
        #define COMMAND_ID_MAX_MATCHES 10
    #endif

    new
        ids[COMMAND_ID_MAX_MATCHES];

    if (sscanf(params, "?<MATCH_NAME_PARTIAL=1>u[" #COMMAND_ID_MAX_MATCHES "]", ids)) {
        SendClientMessage(playerid, -1, ""COMMAND_USAGE_FORMAT"/id [ часть ника ]");
        return 1;
    }
    #undef COMMAND_ID_MAX_MATCHES

    new
        i,
        insert_pos,
        string[MAX_PLAYER_NAME + 18 + 3 + 3];

    while (ids[i] != INVALID_PLAYER_ID) {
        if (ids[i] == cellmin) {
            SendClientMessage(playerid, -1, ""cGREY"и другие...");
            break;
        }

        GetPlayerName(ids[i], string, sizeof(string));

        insert_pos = strfind(string, params, true);
        if (insert_pos != -1) {
            strins(string, "{FFFFFF}", strlen(params) + insert_pos);
            strins(string, "{FF8888}", insert_pos);
        }

        format(string, sizeof(string), ""cGREY"%s - %d", string, ids[i]);
        SendClientMessage(playerid, -1, string);

        ++i;
    }

    if (i == 0) {
        SendClientMessage(playerid, -1, ""cGREY"Никто не найден");
    }
    return 1;
}
