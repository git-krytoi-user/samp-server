/*

    @---------------------
    @ This is an executable module that creates a PayDay system on the server.
    @ Developed by the developer: dev.r1nex
    @----------------------

*/

/*
    Functions
*/
public: Server_PayDay() {
    new hours, minutes, seconds;
	gettime(hours, minutes, seconds);

    foreach (new i: Player) {
        if (!GetUserData(i, u_auth)) continue;

        if (!minutes) {
            new exp = GetUserData(i, u_exp);
            new score = GetUserData(i, u_score);
            exp ++;
            SetUserData(i, u_exp, exp);
            if (exp == (score + 1) * 4) {
                score ++;
                SetUserData(i, u_score, score);
                exp = 0;
                SetUserData(i, u_exp, exp);

                SetPlayerScore(i, score);
                SendClientMessage(i, -1, MESSAGE_LEVEL_UP);
            }

            GetUserData(i, u_money) += 100;
            GivePlayerMoney(i, 100);
            GameTextForPlayer(i, "~g~+100$", 3000, 1);

            new string[88 + (-2+11) + (-2+11) + (-2+11)];
            format(string, sizeof string, ""cWHITE"Ваши накопления: "cGREEN"%d $\n"cWHITE"До следующего уровня "cYELLOW"(%d/%d) EXP", GetUserData(i, u_money), exp, (score + 1) * 4);

            SendClientMessage(i, -1, ""cWHITE"-------------------------");
            SendClientMessage(i, -1, "");
            SendClientMessage(i, -1, ""cWHITE"Пособие по безработище.");
            SendClientMessage(i, -1, ""cWHITE"Государство выплачивает Вам "cGREEN"100 $ "cWHITE"за то, что Вы проживайте в этом штате.");
            SendClientMessage(i, -1, string);
            SendClientMessage(i, -1, "");
            SendClientMessage(i, -1, ""cWHITE"-------------------------");
        }

        new Float: satiety = GetUserData(i, u_satiety);

		if (satiety > 0) {
			satiety -= 1.0;
            UpdatePlayerBar(i, 2);
		}

        if (satiety <= 0) {
            new Float: health = 0.0;
            
            GetPlayerHealth(i, health);
            health --;

            SetPlayerHealth(i, health);

            UpdatePlayerBar(i, 0);
        }

		SetUserData(i, u_satiety, satiety);

		if (satiety <= 20) {
			PlayerTextDrawSetString(i, ptd_greenzone[i], "~y~ВЫ ГОЛОДНЫ!");
			PlayerTextDrawFont(i, ptd_greenzone[i], 1);
			PlayerTextDrawSetShadow(i, ptd_greenzone[i], 0);
			ShowBrightnessTextDraw(i);
		}

        /*if (IsPlayerMute(i)) {
            SetUserData(i, u_mutetime, 0);
            SendClientMessage(i, -1, ""cGREY"Время блокировки чата истекло. Теперь Вы можете снова писать в чат.");
        }*/
    }
}