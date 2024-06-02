/*--------[Launcher]--------*/
native SetLauncherStatus(playerid, bool:status);
native GetLauncherStatus(playerid);
/*--------[Interface]--------*/
enum
{
	Arz_Speedometer, /*Спидометр | 0 - выключен, 1 - первый тип, 2 - второй тип*/
	Arz_Hud, /*Худ | 0 - выключен, 1 - первый тип, 2 - второй тип*/
	Arz_Radar, /*Радар | 0 - выключен, 1 - первый тип, 2 - второй тип*/
	Arz_Fuel, /*Бензин*/
	Arz_Hunger, /*Голод*/
	Arz_Hints, /*Подсказки | 0 - показано, 1 - скрыто (Не работает)*/
	Arz_TransparentChat, /*Прозрачный чат | 0 - выключен, 1 - включен*/
	Arz_SaveDialog, /*Сохранение диалога | 0 - выключен, 1 - включен*/
	Arz_NewNametag, /*Новый нэймтег | 0 - выключен, 1 - включен*/
	Arz_Snow, /*Снег | 0 - выключен, 1 - включен*/
	Arz_RenderDialog /*Рендер диалогов  | 0 - полная, 1 - упрощенный, 2 - простой*/
};
native Interface_Arizona(playerid, interface_id, type);

/*--------[Custom Skins Models]--------*/
forward GetLauncherSkinModel(current_model);
public GetLauncherSkinModel(current_model)
{
	new model = -1;
	switch(current_model)
	{
	    case 312: model = 296;
	    case 313: model = 230;
	    case 908: model = 200;
	}
	return model;
}
/*--------[Custom Vehicles Models]--------*/
forward GetLauncherVehicleModel(current_model);
public GetLauncherVehicleModel(current_model)
{
	new model = -1;
	switch(current_model)
	{
	    case 12501: model = 411;
		case 12502: model = 411;
		case 12503: model = 411;
		case 12504: model = 411;
		case 12505: model = 411;
		case 12506: model = 411;
		case 12507: model = 411;
		case 12508: model = 411;
		case 12509: model = 411;
		case 12510: model = 411;
		case 12511: model = 411;
		case 12512: model = 411;
		case 12513: model = 411;
		case 12514: model = 599;
		case 12515: model = 599;
		case 12516: model = 599;
		case 12517: model = 411;
		case 12518: model = 400;
		case 12519: model = 599;
		case 12520: model = 400;
		case 12521: model = 599;
		case 12522: model = 599;
		case 12523: model = 599;
		case 12524: model = 597;
		case 12525: model = 411;
		case 12526: model = 411;
		case 12527: model = 411;
		case 12528: model = 411;
		case 12529: model = 411;
		case 12530: model = 411;
		case 12531: model = 411;
		case 12532: model = 411;
		case 12533: model = 411;
		case 12534: model = 411;
		case 12535: model = 411;
		case 12536: model = 411;
		case 12537: model = 411;
		case 12538: model = 411;
		case 12539: model = 411;
		case 12540: model = 411;
	}
	return model;
}
/*-------[диса уебан]---------*/