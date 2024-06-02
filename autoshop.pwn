#include <a_samp>
#include <YSI_Coding\y_hooks>

static const ShowRoomPickUpTypeID = 19134; // ид пикапа
static const AmountOfVehiclesInShowRoom = 3; /* Всего количество автомобилей на один автосалон. Трубется для корректной работы текстдравов.
Пытался сделать реализацию с помощью sizeof, но получал множество ошибок, по-этому сделал такой временный костыль. Может быть дойдут руки и сделаю лучше.
Кто его знает?
*/

static const stock ShowRoomNames[][] = {"{49FF00}Эконом", "{FFAA00}Стандарт", "{9E00FF}Элитный"};

static const stock Float:ShowRoomPickupsCoords[][] = {
    {2029.8235, 1347.8411, 10.8203},
    {2029.7965, 1343.2299, 10.8203},
    {2029.7786, 1339.0029, 10.8203}
}; // координаты пикапов + 3д текста автосалонов

static const stock Float:ShowRoomCreateVehicleCoords[3] = {2048.7517,1322.3048,10.6719};
static const stock ShowRoomCarsID[][][] = {
    {400, 401, 402}, // эконом
    {403, 404, 405}, // стандарт
    {406, 407, 408} // элитный
 }; // список продаваемых автомобилей

static const ShowRoomCarNames[][][] = {
    {"TestCar", "TestCar", "TestCar"}, // Названия автомобилей для эконом класса
    {"TestCar", "TestCar", "TestCar"},   // Названия автомобилей для стандарт класса
    {"TestCar", "TestCar", "TestCar"}      // Названия автомобилей для элитного класса
};

static const ShowRoomCarCosts[][][] = {
    {10000, 10001, 10002}, // Стоимость автомобилей для эконом класса
    {20000, 20001, 20002},   // Стоимость автомобилей для стандарт класса
    {30000, 30001, 30002}      // Стоимость автомобилей для элитного класса
};

new ShowRoomsVariablesPickup[][] = { {0}, {0}, {0} }; // Массив для хранения ID пикапов каждого автосалона
new Text:ShowRoomTextDraw_TD[12]; // textdraws
new currentCarIndex[MAX_PLAYERS] = {-1, -1, -1}; // Индекс текущего автомобиля для каждого игрока
new autosaloonID[MAX_PLAYERS] = -1; // ID автосалона для каждого игрока
new ShowRoomVehicleID[MAX_PLAYERS] = 0;

stock CreatePickups() {
    for (new i = 0; i < sizeof(ShowRoomPickupsCoords); i++) {
        new Float:coords[3];

        coords[0] = ShowRoomPickupsCoords[i][0];
        coords[1] = ShowRoomPickupsCoords[i][1];
        coords[2] = ShowRoomPickupsCoords[i][2];

        new pickupid = CreatePickup(ShowRoomPickUpTypeID, 1, coords[0], coords[1], coords[2], 0);
        ShowRoomsVariablesPickup[i][0] = pickupid; // Сохраняем ID пикапа в массив
        Create3DTextLabel(ShowRoomNames[i][0], 0xFFFFFFFF, coords[0], coords[1], coords[2], 10.0, 0);
    }
    CreatePickup(ShowRoomPickUpTypeID, 1, 1959.5544,1345.1099,15.3746, 0);
    return 1;
}

hook OnGameModeInit() {
    CreatePickups();
    CreateTextDraws();
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerPickUpPickup(playerid, pickupid) {
    for (new i = 0; i < sizeof(ShowRoomsVariablesPickup); i++) {
        if (ShowRoomsVariablesPickup[i][0] == pickupid) {
            SendClientMessage(playerid, -1, "{FFD100}[Информация] {ffffff}Для покупки автомобиля выберите его с помощью стрелочек и нажмите {FF0000}Enter{FFFFFF}!");
            currentCarIndex[playerid] = 0; // Начинаем с первой модели
            autosaloonID[playerid] = i; // Сохраняем ID автосалона для игрока

            // Открываем TextDraw и заполняем информацию о модели автомобиля
            TextDrawSetString(ShowRoomTextDraw_TD[8], ShowRoomCarNames[autosaloonID[playerid]][currentCarIndex[playerid]][0]);
            new str[25];
            format (str, sizeof str, "$%d", ShowRoomCarCosts[autosaloonID[playerid]][currentCarIndex[playerid]][0]);
            TextDrawSetString(ShowRoomTextDraw_TD[4], str);
            SetPlayerVirtualWorld(playerid, playerid + 1);

            SetPlayerCameraPos(playerid, ShowRoomCreateVehicleCoords[0] - 10,  ShowRoomCreateVehicleCoords[1],  ShowRoomCreateVehicleCoords[2] + 3);
            SetPlayerCameraLookAt(playerid, ShowRoomCreateVehicleCoords[0] - 10,  ShowRoomCreateVehicleCoords[1],  ShowRoomCreateVehicleCoords[2] + 3);

            ShowRoomCreateVehicle(playerid, ShowRoomCarsID[autosaloonID[playerid]][currentCarIndex[playerid]][0]);

            SelectTextDraw(playerid, 0xFFFFFFFF);
            for (new ii = 0; ii < sizeof ShowRoomTextDraw_TD; ii++) {
                TextDrawShowForPlayer(playerid, ShowRoomTextDraw_TD[ii]);
            }
            break;
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

stock CreateTextDraws() {
    ShowRoomTextDraw_TD[0] = TextDrawCreate(277.8186, 306.0832, "Box"); // пусто
    TextDrawLetterSize(ShowRoomTextDraw_TD[0], -0.0051, 10.5095);
    TextDrawTextSize(ShowRoomTextDraw_TD[0], 367.0000, 0.0000);
    TextDrawAlignment(ShowRoomTextDraw_TD[0], 1);
    TextDrawColor(ShowRoomTextDraw_TD[0], -63);
    TextDrawUseBox(ShowRoomTextDraw_TD[0], 1);
    TextDrawBoxColor(ShowRoomTextDraw_TD[0], 60);
    TextDrawBackgroundColor(ShowRoomTextDraw_TD[0], 200);
    TextDrawFont(ShowRoomTextDraw_TD[0], 1);
    TextDrawSetProportional(ShowRoomTextDraw_TD[0], 1);
    TextDrawSetShadow(ShowRoomTextDraw_TD[0], 0);

    ShowRoomTextDraw_TD[1] = TextDrawCreate(287.6574, 362.0830, "Box"); // пусто
    TextDrawLetterSize(ShowRoomTextDraw_TD[1], 0.0000, 0.5827);
    TextDrawTextSize(ShowRoomTextDraw_TD[1], 361.0000, 0.0000);
    TextDrawAlignment(ShowRoomTextDraw_TD[1], 1);
    TextDrawColor(ShowRoomTextDraw_TD[1], -1);
    TextDrawUseBox(ShowRoomTextDraw_TD[1], 1);
    TextDrawBoxColor(ShowRoomTextDraw_TD[1], -16777133);
    TextDrawBackgroundColor(ShowRoomTextDraw_TD[1], 255);
    TextDrawFont(ShowRoomTextDraw_TD[1], 1);
    TextDrawSetProportional(ShowRoomTextDraw_TD[1], 1);
    TextDrawSetShadow(ShowRoomTextDraw_TD[1], 0);

    ShowRoomTextDraw_TD[2] = TextDrawCreate(277.8183, 306.0832, "Box"); // пусто
    TextDrawLetterSize(ShowRoomTextDraw_TD[2], 0.0000, 1.1449);
    TextDrawTextSize(ShowRoomTextDraw_TD[2], 367.0000, 0.0000);
    TextDrawAlignment(ShowRoomTextDraw_TD[2], 1);
    TextDrawColor(ShowRoomTextDraw_TD[2], -1);
    TextDrawUseBox(ShowRoomTextDraw_TD[2], 1);
    TextDrawBoxColor(ShowRoomTextDraw_TD[2], -16777124);
    TextDrawBackgroundColor(ShowRoomTextDraw_TD[2], 255);
    TextDrawFont(ShowRoomTextDraw_TD[2], 1);
    TextDrawSetProportional(ShowRoomTextDraw_TD[2], 1);
    TextDrawSetShadow(ShowRoomTextDraw_TD[2], 0);

    ShowRoomTextDraw_TD[3] = TextDrawCreate(302.6502, 329.9999, "Box"); // пусто
    TextDrawLetterSize(ShowRoomTextDraw_TD[3], 0.0000, 0.5827);
    TextDrawTextSize(ShowRoomTextDraw_TD[3], 347.0000, 0.0000);
    TextDrawAlignment(ShowRoomTextDraw_TD[3], 1);
    TextDrawColor(ShowRoomTextDraw_TD[3], -1);
    TextDrawUseBox(ShowRoomTextDraw_TD[3], 1);
    TextDrawBoxColor(ShowRoomTextDraw_TD[3], -16777133);
    TextDrawBackgroundColor(ShowRoomTextDraw_TD[3], 255);
    TextDrawFont(ShowRoomTextDraw_TD[3], 1);
    TextDrawSetProportional(ShowRoomTextDraw_TD[3], 1);
    TextDrawSetShadow(ShowRoomTextDraw_TD[3], 0);

    ShowRoomTextDraw_TD[4] = TextDrawCreate(301.7131, 370.8331, "$500000"); // пусто
    TextDrawLetterSize(ShowRoomTextDraw_TD[4], 0.3000, 1.0000);
    TextDrawTextSize(ShowRoomTextDraw_TD[4], 10.0000, 10.0000);
    TextDrawAlignment(ShowRoomTextDraw_TD[4], 1);
    TextDrawColor(ShowRoomTextDraw_TD[4], -1);
    TextDrawBackgroundColor(ShowRoomTextDraw_TD[4], 255);
    TextDrawFont(ShowRoomTextDraw_TD[4], 1);
    TextDrawSetProportional(ShowRoomTextDraw_TD[4], 1);
    TextDrawSetShadow(ShowRoomTextDraw_TD[4], 0);

    ShowRoomTextDraw_TD[5] = TextDrawCreate(282.5035, 304.9165, "A‹ЏOCA‡OH"); // пусто
    TextDrawLetterSize(ShowRoomTextDraw_TD[5], 0.4000, 1.6000);
    TextDrawAlignment(ShowRoomTextDraw_TD[5], 1);
    TextDrawColor(ShowRoomTextDraw_TD[5], -1);
    TextDrawBackgroundColor(ShowRoomTextDraw_TD[5], 255);
    TextDrawFont(ShowRoomTextDraw_TD[5], 1);
    TextDrawSetProportional(ShowRoomTextDraw_TD[5], 1);
    TextDrawSetShadow(ShowRoomTextDraw_TD[5], 0);

    ShowRoomTextDraw_TD[6] = TextDrawCreate(289.0630, 359.1663, "CЏO…MOCЏ’"); // пусто
    TextDrawLetterSize(ShowRoomTextDraw_TD[6], 0.3000, 1.0000);
    TextDrawTextSize(ShowRoomTextDraw_TD[6], 10.0000, 10.0000);
    TextDrawAlignment(ShowRoomTextDraw_TD[6], 1);
    TextDrawColor(ShowRoomTextDraw_TD[6], -1);
    TextDrawBackgroundColor(ShowRoomTextDraw_TD[6], 255);
    TextDrawFont(ShowRoomTextDraw_TD[6], 2);
    TextDrawSetProportional(ShowRoomTextDraw_TD[6], 1);
    TextDrawSetShadow(ShowRoomTextDraw_TD[6], 0);

    ShowRoomTextDraw_TD[7] = TextDrawCreate(374.3339, 351.5833, ">>"); // пусто
    TextDrawLetterSize(ShowRoomTextDraw_TD[7], 0.4000, 1.6000);
    TextDrawTextSize(ShowRoomTextDraw_TD[7], 391.0000, 10.0000);
    TextDrawAlignment(ShowRoomTextDraw_TD[7], 1);
    TextDrawColor(ShowRoomTextDraw_TD[7], -1);
    TextDrawUseBox(ShowRoomTextDraw_TD[7], 1);
    TextDrawBoxColor(ShowRoomTextDraw_TD[7], -16777151);
    TextDrawBackgroundColor(ShowRoomTextDraw_TD[7], -16737904);
    TextDrawFont(ShowRoomTextDraw_TD[7], 3);
    TextDrawSetProportional(ShowRoomTextDraw_TD[7], 1);
    TextDrawSetShadow(ShowRoomTextDraw_TD[7], 100);
    TextDrawSetSelectable(ShowRoomTextDraw_TD[7], 1);

    ShowRoomTextDraw_TD[8] = TextDrawCreate(303.1186, 339.9166, "Infernus"); // пусто
    TextDrawLetterSize(ShowRoomTextDraw_TD[8], 0.3000, 1.0000);
    TextDrawTextSize(ShowRoomTextDraw_TD[8], 10.0000, 10.0000);
    TextDrawAlignment(ShowRoomTextDraw_TD[8], 1);
    TextDrawColor(ShowRoomTextDraw_TD[8], -1);
    TextDrawBackgroundColor(ShowRoomTextDraw_TD[8], 255);
    TextDrawFont(ShowRoomTextDraw_TD[8], 1);
    TextDrawSetProportional(ShowRoomTextDraw_TD[8], 1);
    TextDrawSetShadow(ShowRoomTextDraw_TD[8], 0);

    ShowRoomTextDraw_TD[9] = TextDrawCreate(300.7760, 327.0831, "MOѓE‡’"); // пусто
    TextDrawLetterSize(ShowRoomTextDraw_TD[9], 0.3000, 1.0000);
    TextDrawTextSize(ShowRoomTextDraw_TD[9], 10.0000, 10.0000);
    TextDrawAlignment(ShowRoomTextDraw_TD[9], 1);
    TextDrawColor(ShowRoomTextDraw_TD[9], -1);
    TextDrawBackgroundColor(ShowRoomTextDraw_TD[9], 255);
    TextDrawFont(ShowRoomTextDraw_TD[9], 2);
    TextDrawSetProportional(ShowRoomTextDraw_TD[9], 1);
    TextDrawSetShadow(ShowRoomTextDraw_TD[9], 0);

    ShowRoomTextDraw_TD[10] = TextDrawCreate(251.5812, 351.0000, "<<"); // пусто
    TextDrawLetterSize(ShowRoomTextDraw_TD[10], 0.4000, 1.6000);
    TextDrawTextSize(ShowRoomTextDraw_TD[10], 270.0000, 10.0000);
    TextDrawAlignment(ShowRoomTextDraw_TD[10], 1);
    TextDrawColor(ShowRoomTextDraw_TD[10], -1);
    TextDrawUseBox(ShowRoomTextDraw_TD[10], 1);
    TextDrawBoxColor(ShowRoomTextDraw_TD[10], -16777151);
    TextDrawBackgroundColor(ShowRoomTextDraw_TD[10], -16737904);
    TextDrawFont(ShowRoomTextDraw_TD[10], 3);
    TextDrawSetProportional(ShowRoomTextDraw_TD[10], 1);
    TextDrawSetShadow(ShowRoomTextDraw_TD[10], 100);
    TextDrawSetSelectable(ShowRoomTextDraw_TD[10], true);

    ShowRoomTextDraw_TD[11] = TextDrawCreate(271.7276, 413.4166, "ЊP…OЂPECЏ…"); // пусто
    TextDrawLetterSize(ShowRoomTextDraw_TD[11], 0.4000, 1.6000);
    TextDrawTextSize(ShowRoomTextDraw_TD[11], 354.0000, 10.0000);
    TextDrawAlignment(ShowRoomTextDraw_TD[11], 1);
    TextDrawColor(ShowRoomTextDraw_TD[11], -1);
    TextDrawUseBox(ShowRoomTextDraw_TD[11], 1);
    TextDrawBoxColor(ShowRoomTextDraw_TD[11], -16777150);
    TextDrawBackgroundColor(ShowRoomTextDraw_TD[11], 255);
    TextDrawFont(ShowRoomTextDraw_TD[11], 1);
    TextDrawSetProportional(ShowRoomTextDraw_TD[11], 1);
    TextDrawSetShadow(ShowRoomTextDraw_TD[11], 0);
    TextDrawSetSelectable(ShowRoomTextDraw_TD[11], true);
    return 1;
}

stock NextModelShowRoom(playerid) {
    if (currentCarIndex[playerid] < AmountOfVehiclesInShowRoom) {
        currentCarIndex[playerid]++;
        // TextDrawSetString(ShowRoomTextDraw_TD[8], ShowRoomCarNames[autosaloonID[playerid]][currentCarIndex[playerid]][0]);
        // ShowRoomCreateVehicle(playerid, ShowRoomCarsID[autosaloonID[playerid]][currentCarIndex[playerid]][0]);
        UpdateInfo(playerid);
    }
    return 1;
}

stock PreviousModelShowRoom(playerid) {
    if (currentCarIndex[playerid] > 0) {
        currentCarIndex[playerid]--;
        // TextDrawSetString(ShowRoomTextDraw_TD[8], ShowRoomCarNames[autosaloonID[playerid]][currentCarIndex[playerid]][0]);
        // ShowRoomCreateVehicle(playerid, ShowRoomCarsID[autosaloonID[playerid]][currentCarIndex[playerid]][0]);
        UpdateInfo(playerid);
    }
    return 1;
}

stock UpdateInfo(playerid) {
    new str[24];
    TextDrawSetString(ShowRoomTextDraw_TD[8], ShowRoomCarNames[autosaloonID[playerid]][currentCarIndex[playerid]][0]);
    ShowRoomCreateVehicle(playerid, ShowRoomCarsID[autosaloonID[playerid]][currentCarIndex[playerid]][0]);
    format (str, sizeof str, "$%d", ShowRoomCarCosts[autosaloonID[playerid]][currentCarIndex[playerid]][0]);
    TextDrawSetString(ShowRoomTextDraw_TD[4], str);
}

hook OnPlayerClickTD(playerid, Text:clickedid)
{
    if(!(_:clickedid ^ 0xFFFF)) {
        for (new i = 0; i < sizeof ShowRoomTextDraw_TD; i++) {
            TextDrawHideForPlayer(playerid, ShowRoomTextDraw_TD[i]);
        }
        SetPlayerVirtualWorld(playerid, 0);
        SetPlayerPos(playerid, ShowRoomPickupsCoords[autosaloonID[playerid]][0] - 3, ShowRoomPickupsCoords[autosaloonID[playerid]][1], ShowRoomPickupsCoords[autosaloonID[playerid]][2]);
        SetCameraBehindPlayer(playerid);
    }
    else if (clickedid == ShowRoomTextDraw_TD[7]) {
        return NextModelShowRoom(playerid);
    } else if (clickedid == ShowRoomTextDraw_TD[10]) {
        return PreviousModelShowRoom(playerid);
    } else if (clickedid == ShowRoomTextDraw_TD[11]) {
        new str[120]; // примерный размер
        format(str, sizeof str, "{FFFFFF}Вы действительно хотите купить автомобиль {ff0000}%s{FFFFFF} за {11FF00}$%d{FFFFFF}?", ShowRoomCarNames[autosaloonID[playerid]][currentCarIndex[playerid]][0], ShowRoomCarCosts[autosaloonID[playerid]][currentCarIndex[playerid]][0]);
        return ShowPlayerDialog(playerid, 5555, DIALOG_STYLE_MSGBOX, "Покупка автомобиля", str, "Да", "Нет");
    }
    return 1;
}

stock ShowRoomCreateVehicle(playerid, vehicletype) {
    if (ShowRoomVehicleID[playerid]) { DestroyVehicle(ShowRoomVehicleID[playerid]); }

    ShowRoomVehicleID[playerid] = CreateVehicle(vehicletype, ShowRoomCreateVehicleCoords[0], ShowRoomCreateVehicleCoords[1], ShowRoomCreateVehicleCoords[2], 150.8, -1, -1, -1, 0);
    SetVehicleVirtualWorld(ShowRoomVehicleID[playerid], playerid + 1);
    printf("PlayerID: %d created vehicleID: %d (VehicleType: %d)", playerid, ShowRoomVehicleID[playerid], vehicletype);
    return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]) {
    switch (listitem) {
        case 5555: {
            if (!response) return;
            SendClientMessage(playerid, -1, "Тут система покупки. Мне лень её писать, итак наговнокодил.");
        }
    }
}