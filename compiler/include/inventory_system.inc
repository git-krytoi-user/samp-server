/* Требуется YSI для перехвата функций (https://github.com/pawn-lang/YSI-Includes) */

#if defined _inventory_system
    #endinput
#endif
#define _inventory_system

#if !defined _INC_y_hooks
	#error "Please included <YSI/y_hooks> before include <inventory_system>"
#endif

#if !defined _mdialog_included
	#error "Please included <mdialog> before include <inventory_system>"
#endif

#if !defined foreach
    #error "Please included <foreach> before include <inventory_system>
#endif

#if !defined sscanf
    #error "Please included <sscanf> before include <inventory_system>
#endif

#if !defined mysql_included || !defined cache_get_value_name
    #error "Please included <mysql> ver. R40+ before include <inventory_system>
#endif

/*
	Macroses
*/
#if !defined MAX_ITEMS 									
	#define MAX_ITEMS							(18)
#endif
#if !defined MIN_PAGE
	#define MIN_PAGE							(0) 											
#endif
#if !defined MAX_PAGE
	#define MAX_PAGE							(3) 											
#endif
#if !defined UNKNOWN_ITEM_TYPE
	#define UNKNOWN_ITEM_TYPE					(0)												
#endif
#if !defined ITEM_TYPE_GUN
	#define ITEM_TYPE_GUN						(1) 												
#endif
#if !defined ITEM_TYPE_FOOD
	#define ITEM_TYPE_FOOD						(2) 												
#endif
#if !defined ITEM_TYPE_DRINKS
	#define ITEM_TYPE_DRINKS					(3) 												
#endif
#if !defined ITEM_TYPE_CLOTHING
	#define ITEM_TYPE_CLOTHING					(4) 												
#endif
#if !defined ITEM_TYPE_DRUGS
	#define ITEM_TYPE_DRUGS						(5) 												
#endif
#if !defined ITEM_TYPE_MEDICAL
	#define ITEM_TYPE_MEDICAL					(6) 												
#endif
#if !defined ITEM_TYPE_ACCESSORIES
	#define ITEM_TYPE_ACCESSORIES				(7)
#endif
#if !defined ITEM_TYPE_CARTRIDGS
	#define ITEM_TYPE_CARTRIDGS					(8)
#endif
#if !defined MAX_ITEM_AMOUNT
	#define MAX_ITEM_AMOUNT						(50)												
#endif
#if !defined MAX_NAME_ITEM_LENGHT
	#define MAX_NAME_ITEM_LENGHT				(32) 												
#endif
#if !defined INVENTORY_SIZE_BY_LENGTH
	#define INVENTORY_SIZE_BY_LENGHT			(4)													
#endif
#if !defined INVENTORY_SIZE_BY_HEIGHT
	#define INVENTORY_SIZE_BY_HEIGHT			(4)
#endif
#if !defined INVALID_INVENTORY_PAGE
	#define INVALID_INVENTORY_PAGE				(0)
#endif
#if !defined INVALID_CLICK_SLOT
	#define INVALID_CLICK_SLOT					(-1)													
#endif
#if !defined MAX_ITEMS_SECONDARY
	#define MAX_ITEMS_SECONDARY					(10)
#endif
#if !defined INVENTORY_SIZE	
	#define INVENTORY_SIZE						(INVENTORY_SIZE_BY_LENGHT * INVENTORY_SIZE_BY_HEIGHT)
#endif
#if !defined MAX_ITEMS_PLAYER
	#define MAX_ITEMS_PLAYER					(64)
#endif
#if !defined FIND_IDX
	#define FIND_IDX(%0,%1,%2)					%0+(%1*%2)
#endif
#if !defined MAX_ITEM_DESC_LENGHT
	#define MAX_ITEM_DESC_LENGHT				(144)
#endif
#if !defined MAX_DROP_ITEMS
	#define MAX_DROP_ITEMS						( INVENTORY_SIZE * MAX_PAGE + 1 + (-2+MAX_PLAYERS) ) // 50 заменяйте на то количество, сколько у Вас слотов на сервере
#endif
#if !defined GetItemData
	#define GetItemData(%0,%1)					items_data[%0][%1]
#endif
#if !defined GetInventoryData
	#define GetInventoryData(%0,%1,%2,%3) 		inventory_data[%0][%1][%2][%3]
#endif
#if !defined SetInventoryData
	#define SetInventoryData(%0,%1,%2,%3,%4) 	inventory_data[%0][%1][%2][%3] = %4
#endif
#if !defined GetItemDropInfo
	#define GetItemDropInfo(%0,%1)				drop_items[%0][%1]
#endif
#if !defined SetItemDropInfo
	#define SetItemDropInfo(%0,%1,%2)			drop_items[%0][%1] = %2
#endif

new const item_info[] = 
	"\
		{ffffff}Предмет: {ce7e00}%s \n\
		{ffffff}Количество: {8fce00}%d шт. \n\
		\n\
		{6fa8dc}Описание: \n\
		\n\
		{ffffff}%s \n\
	"
;

new const item_drop_info[] = 
	"\
		{ffffff}Предмет: {ce7e00}%s \n\
		{ffffff}Количество: {8fce00}%d шт. \n\
		\n\
		{6fa8dc}Нажмите ALT, чтобы поднять: \n\
	"
;

/*
	Enums
*/
enum E_PLAYER_INVENTORY_STRUCT {
	ITEM_MYSQL_ID,
	ITEM_PAGE,
	ITEM_SLOT,
	ITEM_AMOUNT,
	ITEM_ID
};

enum E_DROP_ITEM_STRUCT {
	ITEM_AMOUNT,
	ITEM_ID,
	OBJECT_IDENTIFIER,
	ITEM_DYNAMIC_ZONE,
	Float: DROP_X,
	Float: DROP_Y,
	Float: DROP_Z,
	Text3D: ITEM_TEXT
};

enum E_ITEMS_STRUCT {
	ITEM_MODEL,
	ITEM_NAME[MAX_NAME_ITEM_LENGHT],
	ITEM_DESC[MAX_ITEM_DESC_LENGHT],
	ITEM_GUN_ID,
	ITEM_SKIN_ID,
	ITEM_TYPE,
	ITEM_GUN_SLOT,
	Float: X_AXIS_POSITION,
	Float: Y_AXIS_POSITION,
	Float: Z_AXIS_POSITION,
	Float: R_AXIS_POSITION
};

enum _:E_INVENTORY_TD {
	Text: TEXTDRAW_BOX, // ФОН ИНВЕНТАРЯ
	Text: TEXTDRAW_TEXT_PAGE_ONE,
	Text: TEXTDRAW_TEXT_PAGE_TWO,
	Text: TEXTDRAW_TEXT_PAGE_THREE,
	Text: TEXTDRAW_TEXT_CLOSE, // КНОПКА ЗАКРЫТИЯ ИНВЕНТАРЯ
	Text: TEXTDRAW_BOX_SECONDARY_INV, // ФОН ВТОРИЧНОГО ИНВЕНТАРЯ
	Text: TEXTDRAW_BOX_BUTTON_PAGE_ONE,
	Text: TEXTDRAW_BOX_BUTTON_PAGE_TWO,
	Text: TEXTDRAW_BOX_BUTTON_PAGE_THREE
};

enum _:E_INVENTORY_PTD {
	PlayerText: SLOT_ICON[INVENTORY_SIZE],
	PlayerText: OTHER_SLOT_ICON[10],
	PlayerText: PLAYER_SKIN
};

/*
	Variables
*/
new Text: gtd_inventory[E_INVENTORY_TD] = {
	Text: -1,
	Text: -1,
	Text: -1,
	Text: -1,
	Text: -1,
	Text: -1,
	Text: -1,
	Text: -1,
	Text: -1
};
new PlayerText: ptd_inventory[MAX_PLAYERS][E_INVENTORY_PTD];
new PlayerText: td_buttons[MAX_PLAYERS][11];
new PlayerText: ptd_inventory_null[E_INVENTORY_PTD] = {
	PlayerText: -1, // MAIN_SLOT_ICON				 
	PlayerText: -1, // OTHER_SLOT_ICON
	PlayerText: -1 // player skin	
};
new inventory_data[MAX_PLAYERS][MAX_PAGE][MAX_ITEMS_PLAYER][E_PLAYER_INVENTORY_STRUCT];
new drop_items[MAX_DROP_ITEMS][E_DROP_ITEM_STRUCT];
new Iterator: iDropItem<MAX_DROP_ITEMS>;
new secondary_slots_data[MAX_PLAYERS][MAX_ITEMS_SECONDARY][E_PLAYER_INVENTORY_STRUCT];
// new player_accessories[MAX_PLAYERS char][2] = {INVALID_OBJECT_ID, ...};
new bool: is_open_inventory[MAX_PLAYERS] = {false, ...};
new inventory_page[MAX_PLAYERS] = {INVALID_INVENTORY_PAGE, ...};
new clicked_slot[MAX_PLAYERS] = {INVALID_CLICK_SLOT, ...};
new transmit_data[MAX_PLAYERS][5]; // 0 - ITEM_ID, 1 - ITEM_AMOUNT, 2 - RECEIVER, 3 - INDEX ITEM, 4 - PAGE
new items_data[MAX_ITEMS][E_ITEMS_STRUCT] = {
	{ 19374, " ", "  ", -1, -1, UNKNOWN_ITEM_TYPE, -1, 0.0, 0.0, 0.0, 0.0 }, // 0
	{ 337, "Лопата", "Лопату можно купить в магазине оружия.", 6, -1, ITEM_TYPE_GUN, 1, 0.0, 0.0, 0.0, 1.0 }, // 1
	{ 341, "Бензопила", "Бензопилу можно купить в магазине оружия.", 9, -1, ITEM_TYPE_GUN, 1, 0.0, 0.0, 0.0, 1.0 }, // 2
	{ 11736, "Аптечка", "Медикамент, можно получить/купить у врача.", -1, -1, ITEM_TYPE_MEDICAL, -1, 0.0, 0.0, 0.0, 1.0 }, // 3
	{ 2768, "Средний бургер", "Пополняет сытость, можно купить в ближайшей забегаловке или в магазине продуктов.", -1, -1, ITEM_TYPE_FOOD, -1, 0.0, 0.0, 0.0, 1.0 }, // 4
	{ 2880, "Маленький бургер", "Пополняет сытость, можно купить в ближайшей забегаловке или в магазине продуктов.", -1, -1, ITEM_TYPE_FOOD, -1, 0.0, 0.0, 0.0, 1.0 }, // 5
	{ 2769, "Шаурма", "Пополняет сытость, можно купить в ближайшей забегаловке или в магазине продуктов.", -1, -1, ITEM_TYPE_FOOD, -1, 0.0, 0.0, 0.0, 1.0 }, // 6
	{ 19563, "Апельсиновый сок.", "Утоляет жажду, можно купить в ближайшей забегаловке или в магазине продуктов.", -1, -1, ITEM_TYPE_DRINKS, -1, 0.0, 0.0, 0.0, 1.0 }, // 7
	{ 19564, "Яблочный сок.", "Утоляет жажду, можно купить в ближайшей забегаловке или в магазине продуктов.", -1, -1, ITEM_TYPE_DRINKS, -1, 0.0, 0.0, 0.0, 1.0 }, // 8
	{ 352, "MICRO-UZI.", "Огнестрельное оружие, можно купить в магазине оружия.", 28, -1, ITEM_TYPE_GUN, 4, 0.0, 0.0, 0.0, 1.0 }, // 9
	{ 348, "Пустынный орёл \"DESERT-EAGLE\".", "Огнестрельное оружие, можно купить в магазине оружия.", 24, -1, ITEM_TYPE_GUN, 4, 0.0, 0.0, 0.0, 1.0 }, // 10
	{ 349, "Дробовик - \"SHOTGUN\".", "Огнестрельное оружие, можно купить в магазине оружия.", 25, -1, ITEM_TYPE_GUN, 3, 0.0, 0.0, 0.0, 1.0 }, // 11
	{ 351, "Дробовик - \"COMBAT-SHOTGUN\".", "Огнестрельное оружие, можно купить в магазине оружия.", 27, -1, ITEM_TYPE_GUN, 3, 0.0, 0.0, 0.0, 1.0 }, // 12
	{ 353, "MP5.", "Огнестрельное оружие, можно купить в магазине оружия.", 29, -1, ITEM_TYPE_GUN, 4, 0.0, 0.0, 0.0, 1.0 }, // 13
	{ 355, "AK-47.", "Огнестрельное оружие, можно купить в магазине оружия.", 30, -1, ITEM_TYPE_GUN, 5, 0.0, 0.0, 0.0, 1.0 }, // 14
	{ 358, "Винтовка - \"SNIPER-RIFLE\".", "Огнестрельное оружие, можно купить в магазине оружия.", 34, -1, ITEM_TYPE_GUN, 6, 0.0, 0.0, 0.0, 1.0 }, // 15
	{ 367, "Фотоаппарат", "Фотоаппарат, можно купить в магазине техники.", 43, -1, ITEM_TYPE_GUN, 9, 0.0, 0.0, 0.0, 1.0 }, // 16
	{ 2061, "Патроны для оружия.", "Патроны можно купить в магазине оружия.", -1, -1, ITEM_TYPE_CARTRIDGS, -1, 0.0, 0.0, 0.0, 1.0 } // 17
};

/*
	Functions
*/
stock SetClickedSlot(playerid, slot = INVALID_CLICK_SLOT) {
	return clicked_slot[playerid] = slot;
}

stock Inventory_PickupWorldItem(playerid, newkeys) {
	foreach (new i: iDropItem) {
		new area = GetItemDropInfo(i, ITEM_DYNAMIC_ZONE);

		if (IsPlayerInDynamicArea(playerid, area) && newkeys & KEY_WALK) {
			new available_slot = GetAvailableInventorySlot(playerid);
			
			new item_id = GetItemDropInfo(i, ITEM_ID);
			new item_amount = GetItemDropInfo(i, ITEM_AMOUNT);
			new object_identifier = GetItemDropInfo(i, OBJECT_IDENTIFIER);
			new Text3D: item_text = GetItemDropInfo(i, ITEM_TEXT);
			new areaid = GetItemDropInfo(i, ITEM_DYNAMIC_ZONE);

			AddInventoryItem(playerid, item_id, 0, item_amount, available_slot);

			DestroyDynamicArea(areaid);
			DestroyDynamicObject(object_identifier);
			Delete3DTextLabel(item_text);

			SendClientMessage(playerid, -1, "Предмет который Вы подняли, был добавлен к Вам в инвентарь.");

			ApplyAnimation(playerid, "BOMBER", "BOM_PLANT", 4.1, 0, 0, 0, 0, 0, 1);
			ApplyAnimation(playerid, "BOMBER", "BOM_PLANT", 4.1, 0, 0, 0, 0, 0, 1);
		}
	}

	return true;
}

stock Inventory_WorldDropItem(playerid, index, item_amount, item_id, page) {
	SetClickedSlot(playerid);
	
	new
		free_index = Iter_Free(iDropItem),
		Float: x,
		Float: y,
		Float: z,
		item_name[MAX_NAME_ITEM_LENGHT],
		object_model = GetItemData(item_id, ITEM_MODEL),
		info[sizeof item_drop_info + (-2+MAX_NAME_ITEM_LENGHT) + (-2+2) + (-2+MAX_ITEM_DESC_LENGHT)]
	;

	GetPlayerPos(playerid, x, y, z);

	format(item_name, sizeof item_name, GetItemData(item_id, ITEM_NAME));
	format(info, sizeof info, item_drop_info, item_name, item_amount, info);

	GetItemDropInfo(free_index, ITEM_TEXT) = Create3DTextLabel(info, -1, x, y - 0.5, z, 7.0, 0);
	GetItemDropInfo(free_index, OBJECT_IDENTIFIER) = CreateDynamicObject(object_model, x, y - 0.5, z, 0.0, 0.0, 0.0);
	GetItemDropInfo(free_index, ITEM_DYNAMIC_ZONE) = CreateDynamicCircle(x, y - 0.5, 1.0, 0);

	SetInventoryData(playerid, page, index, ITEM_AMOUNT, GetInventoryData(playerid, page, index, ITEM_AMOUNT) - item_amount);
	if (GetInventoryData(playerid, page, index, ITEM_AMOUNT) <= 0) {
		DeleteItemFromDatabase(playerid, item_id, page);
		SetInventoryData(playerid, page, index, ITEM_ID, 0);
		SetInventoryData(playerid, page, index, ITEM_AMOUNT, 0);
		SetInventoryData(playerid, page, index, ITEM_SLOT, 0);
		FillInventoryItems(playerid, page);
	}

	SetItemDropInfo(free_index, ITEM_AMOUNT, item_amount);
	SetItemDropInfo(free_index, ITEM_ID, item_id);
	SetItemDropInfo(free_index, DROP_X, x);
	SetItemDropInfo(free_index, DROP_Y, y - 0.5);
	SetItemDropInfo(free_index, DROP_Z, z);

	return Iter_Add(iDropItem, free_index);
}

stock bool: IsOpenInventory(playerid) {
	return is_open_inventory[playerid];
}

stock UpdateItemAmountInDatabase(playerid, item_id, rewrite_value, page = 0) {
	new
		account_id = GetPlayerAccountID(playerid),
		query[] = "UPDATE inventory SET item_amount = %d WHERE item_id = %d AND account_id = %d AND page = %d",
		query_size[sizeof query + (-2+11) + (-2+4) + (-2+11) + (-2+1)]
	;

	format(query_size, sizeof query_size, query, rewrite_value, item_id, account_id, page);
	mysql_pquery(MYSQL_DEFAULT_HANDLE, query_size);
}

stock DeleteItemFromDatabase(playerid, item_id, page = 0) {
	new
		account_id = GetPlayerAccountID(playerid),
		query[] = "DELETE FROM inventory WHERE account_id = %d AND item_id = %d AND page = %d",
		query_size[sizeof query + (-2+11) + (-2+4)]
	;

	format(query_size, sizeof query_size, query, account_id, item_id, page);
	mysql_pquery(MYSQL_DEFAULT_HANDLE, query_size);
}

stock AddItemInDatabase(playerid, item_id, item_amount, item_slot, page = 0) {
	new
		account_id = GetPlayerAccountID(playerid),
		query[] = "INSERT INTO inventory (account_id, item_id, item_amount, item_slot, page) VALUES (%d, %d, %d, %d, %d)",
		query_size[sizeof query + (-2+11) + (-2+4) + (-2+11) + (-2+4) + (-2+2)]
	;

	format(query_size, sizeof query_size, query, account_id, item_id, item_amount, item_slot, page);
	mysql_pquery(MYSQL_DEFAULT_HANDLE, query_size);
}

stock PlayerClickSlot(playerid, slot) {
	new
		Float: x = (slot % INVENTORY_SIZE_BY_HEIGHT) * (25.0 + 2.0),
		Float: y = (slot / INVENTORY_SIZE_BY_HEIGHT) * (25.0 + 2.0)
	;

	td_buttons[playerid][0] = CreatePlayerTextDraw(playerid, 36.9999 + x, 168.4295 + y, !"_"); // пусто
	PlayerTextDrawLetterSize(playerid, td_buttons[playerid][0], 0.0000, 5.5000);
	PlayerTextDrawTextSize(playerid, td_buttons[playerid][0], 63.0000 + x, 0.0000);
	PlayerTextDrawAlignment(playerid, td_buttons[playerid][0], 1);
	PlayerTextDrawColor(playerid, td_buttons[playerid][0], -1);
	PlayerTextDrawUseBox(playerid, td_buttons[playerid][0], 1);
	PlayerTextDrawBoxColor(playerid, td_buttons[playerid][0], 150);
	PlayerTextDrawBackgroundColor(playerid, td_buttons[playerid][0], 255);
	PlayerTextDrawFont(playerid, td_buttons[playerid][0], 1);
	PlayerTextDrawSetProportional(playerid, td_buttons[playerid][0], 1);
	PlayerTextDrawSetShadow(playerid, td_buttons[playerid][0], 0);

	td_buttons[playerid][1] = CreatePlayerTextDraw(playerid, 37.8666 + x, 169.4888 + y, !"_"); // пусто
	PlayerTextDrawLetterSize(playerid, td_buttons[playerid][1], 0.0000, 0.6226);
	PlayerTextDrawTextSize(playerid, td_buttons[playerid][1], 62.0000 + x, 0.0000);
	PlayerTextDrawAlignment(playerid, td_buttons[playerid][1], 1);
	PlayerTextDrawColor(playerid, td_buttons[playerid][1], -1);
	PlayerTextDrawUseBox(playerid, td_buttons[playerid][1], 1);
	PlayerTextDrawBoxColor(playerid, td_buttons[playerid][1], -1061109615);
	PlayerTextDrawBackgroundColor(playerid, td_buttons[playerid][1], 255);
	PlayerTextDrawFont(playerid, td_buttons[playerid][1], 1);
	PlayerTextDrawSetProportional(playerid, td_buttons[playerid][1], 1);
	PlayerTextDrawSetShadow(playerid, td_buttons[playerid][1], 0);

	td_buttons[playerid][2] = CreatePlayerTextDraw(playerid, 37.8666 + x, 179.8894 + y, !"_"); // пусто
	PlayerTextDrawLetterSize(playerid, td_buttons[playerid][2], 0.0000, 0.6226);
	PlayerTextDrawTextSize(playerid, td_buttons[playerid][2], 62.0000 + x, 0.0000);
	PlayerTextDrawAlignment(playerid, td_buttons[playerid][2], 1);
	PlayerTextDrawColor(playerid, td_buttons[playerid][2], -1);
	PlayerTextDrawUseBox(playerid, td_buttons[playerid][2], 1);
	PlayerTextDrawBoxColor(playerid, td_buttons[playerid][2], -1061109615);
	PlayerTextDrawBackgroundColor(playerid, td_buttons[playerid][2], 255);
	PlayerTextDrawFont(playerid, td_buttons[playerid][2], 1);
	PlayerTextDrawSetProportional(playerid, td_buttons[playerid][2], 1);
	PlayerTextDrawSetShadow(playerid, td_buttons[playerid][2], 0);

	td_buttons[playerid][3] = CreatePlayerTextDraw(playerid, 37.8666 + x, 189.9900 + y, !"_"); // пусто
	PlayerTextDrawLetterSize(playerid, td_buttons[playerid][3], 0.0000, 0.6226);
	PlayerTextDrawTextSize(playerid, td_buttons[playerid][3], 62.0000 + x, 0.0000);
	PlayerTextDrawAlignment(playerid, td_buttons[playerid][3], 1);
	PlayerTextDrawColor(playerid, td_buttons[playerid][3], -1);
	PlayerTextDrawUseBox(playerid, td_buttons[playerid][3], 1);
	PlayerTextDrawBoxColor(playerid, td_buttons[playerid][3], -1061109615);
	PlayerTextDrawBackgroundColor(playerid, td_buttons[playerid][3], 255);
	PlayerTextDrawFont(playerid, td_buttons[playerid][3], 1);
	PlayerTextDrawSetProportional(playerid, td_buttons[playerid][3], 1);
	PlayerTextDrawSetShadow(playerid, td_buttons[playerid][3], 0);

	td_buttons[playerid][4] = CreatePlayerTextDraw(playerid, 37.8666 + x, 200.1907 + y, !"_"); // пусто
	PlayerTextDrawLetterSize(playerid, td_buttons[playerid][4], 0.0000, 0.6226);
	PlayerTextDrawTextSize(playerid, td_buttons[playerid][4], 62.0000 + x, 0.0000);
	PlayerTextDrawAlignment(playerid, td_buttons[playerid][4], 1);
	PlayerTextDrawColor(playerid, td_buttons[playerid][4], -1);
	PlayerTextDrawUseBox(playerid, td_buttons[playerid][4], 1);
	PlayerTextDrawBoxColor(playerid, td_buttons[playerid][4], -1061109615);
	PlayerTextDrawBackgroundColor(playerid, td_buttons[playerid][4], 255);
	PlayerTextDrawFont(playerid, td_buttons[playerid][4], 1);
	PlayerTextDrawSetProportional(playerid, td_buttons[playerid][4], 1);
	PlayerTextDrawSetShadow(playerid, td_buttons[playerid][4], 0);

	td_buttons[playerid][5] = CreatePlayerTextDraw(playerid, 37.8666 + x, 210.8914 + y, !"_"); // пусто
	PlayerTextDrawLetterSize(playerid, td_buttons[playerid][5], 0.0000, 0.6226);
	PlayerTextDrawTextSize(playerid, td_buttons[playerid][5], 62.0000 + x, 0.0000);
	PlayerTextDrawAlignment(playerid, td_buttons[playerid][5], 1);
	PlayerTextDrawColor(playerid, td_buttons[playerid][5], -1);
	PlayerTextDrawUseBox(playerid, td_buttons[playerid][5], 1);
	PlayerTextDrawBoxColor(playerid, td_buttons[playerid][5], -16777076);
	PlayerTextDrawBackgroundColor(playerid, td_buttons[playerid][5], 120);
	PlayerTextDrawFont(playerid, td_buttons[playerid][5], 1);
	PlayerTextDrawSetProportional(playerid, td_buttons[playerid][5], 1);
	PlayerTextDrawSetShadow(playerid, td_buttons[playerid][5], 0);

	td_buttons[playerid][6] = CreatePlayerTextDraw(playerid, 37.2332 + x, 167.2554 + y, "ИСПОЛЬЗОВАТЬ"); // пусто
	PlayerTextDrawLetterSize(playerid, td_buttons[playerid][6], 0.1030, 0.9943);
	PlayerTextDrawTextSize(playerid, td_buttons[playerid][6], 62.0000 + x, 8.0000);
	PlayerTextDrawAlignment(playerid, td_buttons[playerid][6], 1);
	PlayerTextDrawColor(playerid, td_buttons[playerid][6], -1);
	PlayerTextDrawUseBox(playerid, td_buttons[playerid][6], 1);
	PlayerTextDrawBoxColor(playerid, td_buttons[playerid][6], 0);
	PlayerTextDrawBackgroundColor(playerid, td_buttons[playerid][6], 255);
	PlayerTextDrawFont(playerid, td_buttons[playerid][6], 1);
	PlayerTextDrawSetProportional(playerid, td_buttons[playerid][6], 1);
	PlayerTextDrawSetShadow(playerid, td_buttons[playerid][6], 0);
	PlayerTextDrawSetSelectable(playerid, td_buttons[playerid][6], true);

	td_buttons[playerid][7] = CreatePlayerTextDraw(playerid, 37.2332 + x, 177.5561 +  y, "ПЕРЕДАТЬ"); // пусто
	PlayerTextDrawLetterSize(playerid, td_buttons[playerid][7], 0.1659, 0.9943);
	PlayerTextDrawTextSize(playerid, td_buttons[playerid][7], 61.1697 + x, 8.0000);
	PlayerTextDrawAlignment(playerid, td_buttons[playerid][7], 1);
	PlayerTextDrawColor(playerid, td_buttons[playerid][7], -1);
	PlayerTextDrawUseBox(playerid, td_buttons[playerid][7], 1);
	PlayerTextDrawBoxColor(playerid, td_buttons[playerid][7], 0);
	PlayerTextDrawBackgroundColor(playerid, td_buttons[playerid][7], 255);
	PlayerTextDrawFont(playerid, td_buttons[playerid][7], 1);
	PlayerTextDrawSetProportional(playerid, td_buttons[playerid][7], 1);
	PlayerTextDrawSetShadow(playerid, td_buttons[playerid][7], 0);
	PlayerTextDrawSetSelectable(playerid, td_buttons[playerid][7], true);

	td_buttons[playerid][8] = CreatePlayerTextDraw(playerid, 37.2332 + x, 187.6566 + y, "ИНФОРМАЦИЯ"); // пусто
	PlayerTextDrawLetterSize(playerid, td_buttons[playerid][8], 0.1159, 1.0025);
	PlayerTextDrawTextSize(playerid, td_buttons[playerid][8], 62.0000 + x, 8.0000);
	PlayerTextDrawAlignment(playerid, td_buttons[playerid][8], 1);
	PlayerTextDrawColor(playerid, td_buttons[playerid][8], -1);
	PlayerTextDrawUseBox(playerid, td_buttons[playerid][8], 1);
	PlayerTextDrawBoxColor(playerid, td_buttons[playerid][8], 0);
	PlayerTextDrawBackgroundColor(playerid, td_buttons[playerid][8], 255);
	PlayerTextDrawFont(playerid, td_buttons[playerid][8], 1);
	PlayerTextDrawSetProportional(playerid, td_buttons[playerid][8], 1);
	PlayerTextDrawSetShadow(playerid, td_buttons[playerid][8], 0);
	PlayerTextDrawSetSelectable(playerid, td_buttons[playerid][8], true);

	td_buttons[playerid][9] = CreatePlayerTextDraw(playerid, 37.2332 + x, 197.9575 + y, "ВЫБРОСИТЬ"); // пусто
	PlayerTextDrawLetterSize(playerid, td_buttons[playerid][9], 0.1393, 1.0109);
	PlayerTextDrawTextSize(playerid, td_buttons[playerid][9], 62.0000 + x, 8.0000);
	PlayerTextDrawAlignment(playerid, td_buttons[playerid][9], 1);
	PlayerTextDrawColor(playerid, td_buttons[playerid][9], -1);
	PlayerTextDrawUseBox(playerid, td_buttons[playerid][9], 1);
	PlayerTextDrawBoxColor(playerid, td_buttons[playerid][9], 0);
	PlayerTextDrawBackgroundColor(playerid, td_buttons[playerid][9], 255);
	PlayerTextDrawFont(playerid, td_buttons[playerid][9], 1);
	PlayerTextDrawSetProportional(playerid, td_buttons[playerid][9], 1);
	PlayerTextDrawSetShadow(playerid, td_buttons[playerid][9], 0);
	PlayerTextDrawSetSelectable(playerid, td_buttons[playerid][9], true);

	td_buttons[playerid][10] = CreatePlayerTextDraw(playerid, 37.3333 + x, 208.5581 + y, "ЗАКРЫТЬ"); // пусто
	PlayerTextDrawLetterSize(playerid, td_buttons[playerid][10], 0.1743, 0.9983);
	PlayerTextDrawTextSize(playerid, td_buttons[playerid][10], 62.0998 + x, 10.0000);
	PlayerTextDrawAlignment(playerid, td_buttons[playerid][10], 1);
	PlayerTextDrawColor(playerid, td_buttons[playerid][10], -1);
	PlayerTextDrawUseBox(playerid, td_buttons[playerid][10], 1);
	PlayerTextDrawBoxColor(playerid, td_buttons[playerid][10], 0);
	PlayerTextDrawBackgroundColor(playerid, td_buttons[playerid][10], 255);
	PlayerTextDrawFont(playerid, td_buttons[playerid][10], 1);
	PlayerTextDrawSetProportional(playerid, td_buttons[playerid][10], 1);
	PlayerTextDrawSetShadow(playerid, td_buttons[playerid][10], 0);
	PlayerTextDrawSetSelectable(playerid, td_buttons[playerid][10], true);

	for (new i; i < 11; i++) {
		PlayerTextDrawShow(playerid, td_buttons[playerid][i]);
	}

	return true;
}

stock GetAvailableSecondarySlot(playerid) {
	new slot = -1;

	for (new i; i < MAX_ITEMS_SECONDARY; i++) {
		if (!secondary_slots_data[playerid][i][ITEM_ID]) {
			slot = i;
			break;
		}
	}

	return slot;
}

stock bool: HasPlayerWeapon(playerid, weaponid) {
	if (!IsPlayerConnected(playerid)) return true;
	new bool: b = false;

	new
		weapons[13],
		ammo[13]
	;

	for (new slot; slot < 13; slot++) {
		GetPlayerWeaponData(playerid, slot, weapons[slot], ammo[slot]);

		if (weapons[slot] == weaponid && ammo[slot] != 0) b = true;
	}

	return b;
}

stock bool: IsWeaponSecondarySlot(playerid, weapon_id) {
	for (new i; i < MAX_ITEMS_SECONDARY; i++) {
		new
			item_id = secondary_slots_data[playerid][i][ITEM_ID],
			weapon = GetItemData(item_id, ITEM_GUN_ID)
		;

		if (weapon_id == weapon) return true;
	}

	return false;
}

stock IsPlayerWeaponInHand(playerid, weapon_id) {
    static weapon, ammo, slot;
    GetPlayerWeaponData(playerid, slot, weapon, ammo);
    if (weapon != weapon_id || !ammo) return false;
    
    new current_weapon = GetPlayerWeapon(playerid);
    if (current_weapon == weapon_id) return true;
    else return false;
}

stock GetAvailableSpaceAccessories(playerid) {
	new space = -1;
	
	for (new i; i < sizeof player_accessories[]; i++) {
		if (player_accessories[playerid][i] != INVALID_OBJECT_ID) space = i;
	}

	return space;
}

stock Inventory_UseItem(playerid, item_type, index) {
	SetClickedSlot(playerid);
	
	switch(item_type) {
		case UNKNOWN_ITEM_TYPE: return SendClientMessage(playerid, -1, "Неопределённый тип предмета.");
		case ITEM_TYPE_GUN: {
			new
				page = GetOpenInventoryPage(playerid),
				item_id = GetInventoryData(playerid, page, index, ITEM_ID),
				item_amount = GetInventoryData(playerid, page, index, ITEM_AMOUNT),
				weapon_id = GetItemData(item_id, ITEM_GUN_ID)
			;

			new bool: has_weapon = HasPlayerWeapon(playerid, weapon_id);
			if (has_weapon) return SendClientMessage(playerid, -1, "У Вас уже надето это оружие.");

			item_amount--;
			UpdateItemAmountInDatabase(playerid, item_id, item_amount, page);
			SetInventoryData(playerid, page, index, ITEM_AMOUNT, item_amount);
			if (item_amount <= 0) {
				DeleteItemFromDatabase(playerid, item_id, page);
				SetInventoryData(playerid, page, index, ITEM_ID, 0);
				SetInventoryData(playerid, page, index, ITEM_AMOUNT, 0);
				SetInventoryData(playerid, page, index, ITEM_SLOT, 0);

				FillInventoryItems(playerid, page);
			}

			new available_slot = GetAvailableSecondarySlot(playerid);
			if (available_slot == -1) {
				return SendClientMessage(playerid, -1, "Освободите место для активных предметов.");
			}

			secondary_slots_data[playerid][available_slot][ITEM_ID] = item_id;
			secondary_slots_data[playerid][available_slot][ITEM_AMOUNT] = 1;
			secondary_slots_data[playerid][available_slot][ITEM_SLOT] = available_slot;
			secondary_slots_data[playerid][available_slot][ITEM_PAGE] = page;

			ApplyAnimation(playerid, "COLT45", "COLT45_RELOAD", 4.1, 0, 0, 0, 0, 0, 1);
			ApplyAnimation(playerid, "COLT45", "COLT45_RELOAD", 4.1, 0, 0, 0, 0, 0, 1);

			GivePlayerWeapon(playerid, weapon_id, 1);
			FillSecondaryItems(playerid);
		}

		case ITEM_TYPE_CLOTHING: {
			new
				page = GetOpenInventoryPage(playerid),
				item_id = GetInventoryData(playerid, page, index, ITEM_ID),
				item_amount = GetInventoryData(playerid, page, index, ITEM_AMOUNT),
				skin_id = GetItemData(item_id, ITEM_SKIN_ID)
			;

			item_amount--;
			UpdateItemAmountInDatabase(playerid, item_id, item_amount, page);
			SetInventoryData(playerid, page, index, ITEM_AMOUNT, item_amount);
			if (item_amount <= 0) {
				DeleteItemFromDatabase(playerid, item_id, page);
				SetInventoryData(playerid, page, index, ITEM_ID, 0);
				SetInventoryData(playerid, page, index, ITEM_AMOUNT, 0);
				SetInventoryData(playerid, page, index, ITEM_SLOT, 0);

				FillInventoryItems(playerid, page);
			}

			SetPlayerSkin(playerid, skin_id);
		}

		case ITEM_TYPE_CARTRIDGS: {
			new 
				weapon_id = GetPlayerWeapon(playerid),
				page = GetOpenInventoryPage(playerid),
				item_id = GetInventoryData(playerid, page, index, ITEM_ID),
				item_amount = GetInventoryData(playerid, page, index, ITEM_AMOUNT)
			;
			
			if (!IsWeaponSecondarySlot(playerid, weapon_id))
				return SendClientMessage(playerid, -1, "Это оружие должно находится в активном слоте.");

			item_amount--;
			UpdateItemAmountInDatabase(playerid, item_id, item_amount, page);
			SetInventoryData(playerid, page, index, ITEM_AMOUNT, item_amount);
			if (item_amount <= 0) {
				DeleteItemFromDatabase(playerid, item_id, page);
				SetInventoryData(playerid, page, index, ITEM_ID, 0);
				SetInventoryData(playerid, page, index, ITEM_AMOUNT, 0);
				SetInventoryData(playerid, page, index, ITEM_SLOT, 0);

				FillInventoryItems(playerid, page);
			}

			GivePlayerWeapon(playerid, weapon_id, 23);
		}

		case ITEM_TYPE_FOOD: {
			new
				page = GetOpenInventoryPage(playerid),
				item_id = GetInventoryData(playerid, page, index, ITEM_ID),
				item_amount = GetInventoryData(playerid, page, index, ITEM_AMOUNT),
				item_name[MAX_NAME_ITEM_LENGHT],
				string[] = "Вы скушали %s и пополнили 5%% сытости.",
				string_size[sizeof string + (-2+MAX_NAME_ITEM_LENGHT)]
			;

			format(item_name, sizeof item_name, GetItemData(item_id, ITEM_NAME));
			format(string_size, sizeof string_size, string, item_name);

			item_amount--;
			UpdateItemAmountInDatabase(playerid, item_id, item_amount, page);
			SetInventoryData(playerid, page, index, ITEM_AMOUNT, item_amount);
			if (item_amount <= 0) {
				DeleteItemFromDatabase(playerid, item_id, page);
				SetInventoryData(playerid, page, index, ITEM_ID, 0);
				SetInventoryData(playerid, page, index, ITEM_AMOUNT, 0);
				SetInventoryData(playerid, page, index, ITEM_SLOT, 0);

				FillInventoryItems(playerid, page);
			}

			// Установка сытости pData[playerid][pHunger] += 5;
			// Обновление сытости UpdatePlayerHunger(playerid);
		}

		case ITEM_TYPE_MEDICAL: {
			new
				page = GetOpenInventoryPage(playerid),
				item_id = GetInventoryData(playerid, page, index, ITEM_ID),
				item_amount = GetInventoryData(playerid, page, index, ITEM_AMOUNT)
			;

			item_amount--;
			UpdateItemAmountInDatabase(playerid, item_id, item_amount, page);
			SetInventoryData(playerid, page, index, ITEM_AMOUNT, item_amount);
			if (item_amount <= 0) {
				DeleteItemFromDatabase(playerid, item_id, page);
				SetInventoryData(playerid, page, index, ITEM_ID, 0);
				SetInventoryData(playerid, page, index, ITEM_AMOUNT, 0);
				SetInventoryData(playerid, page, index, ITEM_SLOT, 0);

				FillInventoryItems(playerid, page);
			}

			SetPlayerHealth(playerid, 100.0);
			SendClientMessage(playerid, -1, "Вы вылечили себя.");
		}
	}

	return true;
}

stock GetOpenInventoryPage(playerid) {
	return inventory_page[playerid];
}

stock GetClickedSlot(playerid) {
	return clicked_slot[playerid];
}

stock HideInventoryTextDraws(playerid) {
	SetClickedSlot(playerid);
	is_open_inventory[playerid] = false;

	for (new i; i < sizeof gtd_inventory; i++) {
		TextDrawHideForPlayer(playerid, gtd_inventory[i]);
	}

	for (new i; i < INVENTORY_SIZE; i++) {
		PlayerTextDrawSetSelectable(playerid, ptd_inventory[playerid][SLOT_ICON][i], true);
		PlayerTextDrawBackgroundColor(playerid, ptd_inventory[playerid][SLOT_ICON][i], 190);
		PlayerTextDrawHide(playerid, ptd_inventory[playerid][SLOT_ICON][i]);
	}

	for (new i; i < 10; i++) {
		PlayerTextDrawHide(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][i]);
	}

	for (new td; td < sizeof td_buttons[]; td++) {
		PlayerTextDrawHide(playerid, td_buttons[playerid][td]);
	}

	PlayerTextDrawHide(playerid, ptd_inventory[playerid][PLAYER_SKIN]);
	
	CancelSelectTextDraw(playerid);
	inventory_page[playerid] = INVALID_INVENTORY_PAGE;
	return true;
}

stock RemovePlayerWeapon(playerid, weaponid) {
    if(!IsPlayerConnected(playerid) || weaponid < 0 || weaponid > 50) return true;

    new saveweapon[13], saveammo[13];
    for (new slot = 0; slot < 13; slot++) {
        GetPlayerWeaponData(playerid, slot, saveweapon[slot], saveammo[slot]);
	}

    ResetPlayerWeapons(playerid);
    for (new slot; slot < 13; slot++) {
        if(saveweapon[slot] == weaponid || saveammo[slot] == 0) continue;
        GivePlayerWeapon(playerid, saveweapon[slot], saveammo[slot]);
    }

    GivePlayerWeapon(playerid, 0, 1);
	return true;
}

stock SetInventoryPage(playerid, page = 0) {
	if (page < 0 || page > 2) return true;
	if (GetOpenInventoryPage(playerid) == page)
		return SendClientMessage(playerid, -1, "Вы уже находитесь на этой странице.");

	inventory_page[playerid] = page;
	FillInventoryItems(playerid, page);
	return true;
}

stock FillInventoryItems(playerid, page = 0) {
	SetClickedSlot(playerid);
	SelectTextDraw(playerid, 0x6a6570AA);
	
	for (new i; i < INVENTORY_SIZE; i++) {
		new slot_id = FIND_IDX(i, page, INVENTORY_SIZE);

		new
			item_id = GetInventoryData(playerid, page, slot_id, ITEM_ID),
			item_model = GetItemData(item_id, ITEM_MODEL)
		;

		PlayerTextDrawSetPreviewModel(playerid, ptd_inventory[playerid][SLOT_ICON][i], !item_id ? 19374 : item_model);
		PlayerTextDrawSetPreviewRot(playerid, ptd_inventory[playerid][SLOT_ICON][i], 
			GetItemData(item_id, X_AXIS_POSITION), GetItemData(item_id, Y_AXIS_POSITION), GetItemData(item_id, Z_AXIS_POSITION), GetItemData(item_id, R_AXIS_POSITION)	
		);
		PlayerTextDrawShow(playerid, ptd_inventory[playerid][SLOT_ICON][i]);
	}

	return true;
}

stock FillSecondaryItems(playerid) {
	for (new i; i < MAX_ITEMS_SECONDARY; i++) {
		new
			item_id = secondary_slots_data[playerid][i][ITEM_ID],
			item_model = GetItemData(item_id, ITEM_MODEL)
		;

		PlayerTextDrawSetPreviewModel(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][i], !item_id ? 19374 : item_model);
		PlayerTextDrawSetPreviewRot(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][i], 
			GetItemData(item_id, X_AXIS_POSITION), GetItemData(item_id, Y_AXIS_POSITION), GetItemData(item_id, Z_AXIS_POSITION), GetItemData(item_id, R_AXIS_POSITION)
		);
		PlayerTextDrawShow(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][i]);
	}

	return true;
}

stock bool: HasPlayerWeaponAmmoSlot(playerid, weaponid, slot) {
	new ammo = 0;
	GetPlayerWeaponData(playerid, slot, weaponid, ammo);
	if (!ammo) return false;
	else return true;
}

stock ReloadSlot(playerid, slot, page = 0) {
	PlayerTextDrawHide(playerid, ptd_inventory[playerid][SLOT_ICON][slot]);

	new slot_id = FIND_IDX(slot, page, INVENTORY_SIZE);
	new item_id = GetInventoryData(playerid, page, slot_id, ITEM_ID);

	PlayerTextDrawSetPreviewModel(playerid, ptd_inventory[playerid][SLOT_ICON][slot], GetItemData(item_id, ITEM_MODEL));
	PlayerTextDrawSetPreviewRot(playerid, ptd_inventory[playerid][SLOT_ICON][slot], 
		GetItemData(item_id, X_AXIS_POSITION), GetItemData(item_id, Y_AXIS_POSITION), GetItemData(item_id, Z_AXIS_POSITION), GetItemData(item_id, R_AXIS_POSITION)
	);

	return PlayerTextDrawShow(playerid, ptd_inventory[playerid][SLOT_ICON][slot]);
}

stock Inventory_GetItemName(const item_name[]) {
	for (new i; i < sizeof items_data; i++) {
		if (!strcmp(item_name, GetItemData(i, ITEM_NAME))) return i;
	}

	return -1;
}

stock ReturnItemsFromSecondarySlots(playerid) { // ReturnItemsFromSecondarySlots
	new slot = -1;
	for (new page; page < MAX_PAGE; page++) {
		for (new i; i < INVENTORY_SIZE; i++) {
			slot = GetAvailableInventorySlot(playerid, page);
		}
	}
	
	for (new i; i < MAX_ITEMS_SECONDARY; i++) {
		new item_id = secondary_slots_data[playerid][i][ITEM_ID];
		new item_page = secondary_slots_data[playerid][i][ITEM_PAGE];
		new item_amount = secondary_slots_data[playerid][i][ITEM_AMOUNT];
		if (!item_id) continue;

		AddInventoryItem(playerid, item_id, item_page, item_amount, slot);
	}

	return true;
}

stock SaveInventoryData(playerid, acc_id) {
	for (new i; i < INVENTORY_SIZE; i++) {
		new
			index = FIND_IDX(i, 0, INVENTORY_SIZE),
			item_id = GetInventoryData(playerid, 0, index, ITEM_ID),
			item_amount = GetInventoryData(playerid, 0, index, ITEM_AMOUNT),
			item_slot = GetInventoryData(playerid, 0, index, ITEM_SLOT)
		;

		if (!item_id) continue;
		if (item_amount > 0) UpdateItem(item_id, item_amount, item_slot, acc_id);
		else if (item_amount <= 0) DeleteItemFromDatabase(playerid, item_id);
	}

	ClearInventoryData(playerid);
}

stock UpdateItem(item_id, item_amount, item_slot, acc_id) {
	new
		query[] = "UPDATE inventory SET item_amount = %d, item_slot = %d WHERE item_id = %d AND account_id = %d",
		query_size[sizeof query + (-2+4) + (-2+11) + (-2+2) + (-2+4) + (-2+11)]
	;
	format(query_size, sizeof query_size, query, item_amount, item_slot, item_id, acc_id);
	mysql_pquery(MYSQL_DEFAULT_HANDLE, query_size);
}

stock AddInventoryItem(playerid, id, page, amount, slot) {
	if (GetAvailableInventorySlot(playerid, page) == -1)
		return SendClientMessage(playerid, -1, "Свободного пространства в инвентаре не найдено.");
	
	new bool: is = false;

	for (new i; i < INVENTORY_SIZE; i++) {
		new index = FIND_IDX(i, page, INVENTORY_SIZE);
		new item_id = GetInventoryData(playerid, page, index, ITEM_ID);

		if (item_id == id) {
			is = true;
			SetInventoryData(playerid, page, index, ITEM_AMOUNT, GetInventoryData(playerid, page, index, ITEM_AMOUNT) + amount);
			UpdateItemAmountInDatabase(playerid, item_id, GetInventoryData(playerid, page, index, ITEM_AMOUNT), page);
			break;
		}
	} 

	if (!is) {
		new index = FIND_IDX(slot, page, INVENTORY_SIZE);
		SetInventoryData(playerid, page, index, ITEM_SLOT, slot);
		SetInventoryData(playerid, page, index, ITEM_AMOUNT, amount);
		SetInventoryData(playerid, page, index, ITEM_ID, id);
		AddItemInDatabase(playerid, id, amount, slot, page);
	}

	return SendClientMessage(playerid, -1, "Предмет успешно добавлен в инвентарь.");
}

/*
	Commands
*/

cmd:add_item(playerid, params[]) {
	new page, slot, amount, item_name[MAX_NAME_ITEM_LENGHT];
	if (sscanf(params, "ddds[32]", page, slot, amount, item_name))
		return SendClientMessage(playerid, -1, "/add_item <страница 0-2> <слот 0-15> <кол-во 0-50> <название предмета>");

	new id = Inventory_GetItemName(item_name);
	if (id == -1) return SendClientMessage(playerid, -1, "Невалидный предмет.");

	if (page < MIN_PAGE || page > MAX_PAGE - 1)
		return SendClientMessage(playerid, -1, " Укажите валидную страницу.");

	if (amount < 1 || amount > MAX_ITEM_AMOUNT)
		return SendClientMessage(playerid, -1, " Укажите валидное количество предмета. От 1 до 50-и.");

	if (slot < 0 || slot > INVENTORY_SIZE)
		return SendClientMessage(playerid, -1, " Укажите валидный слот.");


	return AddInventoryItem(playerid, id, page, amount, slot);
}

cmd:add_player_item(playerid, params[]) {
	new player, slot, amount, item_name[32], page;
	if (sscanf(params, "ddds[32]d", player, slot, amount, item_name, page))
		return SendClientMessage(playerid, -1, "/add_player_item <playerid> <слот 0-15> <кол-во 0-50> <название предмета> <страница 0-2>");

	new id = Inventory_GetItemName(item_name);
	if (id == -1) return SendClientMessage(playerid, -1, "Невалидный предмет.");

	if (!IsPlayerConnected(player))
		return SendClientMessage(playerid, -1, "Этот игрок не подключён к серверу");

	if (slot < 0 || slot > INVENTORY_SIZE)
		return SendClientMessage(playerid, -1, " Укажите валидный слот.");

	if (page < MIN_PAGE || page > MAX_PAGE - 1)
		return SendClientMessage(playerid, -1, " Укажите валидную страницу.");

	if (amount < 1 || amount > MAX_ITEM_AMOUNT)
		return SendClientMessage(playerid, -1, " Укажите валидное количество предмета. От 1 до 50-и.");

	return AddInventoryItem(playerid, id, page, amount, slot);
}

/* Hooks */
stock GetAvailableInventorySlot(playerid, page = 0) {
	new slot = -1;
	
	for (new i, index, item_id; i < INVENTORY_SIZE; i++) {
		index = FIND_IDX(i, page, INVENTORY_SIZE);
		item_id = GetInventoryData(playerid, page, index, ITEM_ID);

		if (!item_id) {
			slot = i;
			break;
		}
	}

	return slot;
}

stock Inventory_CreateGlobalTextDraw() {
	gtd_inventory[TEXTDRAW_BOX] = TextDrawCreate(13.3332, 121.1407, !"_"); // фон инвентаря
	TextDrawLetterSize(gtd_inventory[TEXTDRAW_BOX], 0.0000, 19.2332);
	TextDrawTextSize(gtd_inventory[TEXTDRAW_BOX], 172.2003, 0.0000);
	TextDrawAlignment(gtd_inventory[TEXTDRAW_BOX], 1);
	TextDrawColor(gtd_inventory[TEXTDRAW_BOX], -1);
	TextDrawUseBox(gtd_inventory[TEXTDRAW_BOX], 1);
	TextDrawBoxColor(gtd_inventory[TEXTDRAW_BOX], 150);
	TextDrawBackgroundColor(gtd_inventory[TEXTDRAW_BOX], 255);
	TextDrawFont(gtd_inventory[TEXTDRAW_BOX], 1);
	TextDrawSetProportional(gtd_inventory[TEXTDRAW_BOX], 1);
	TextDrawSetShadow(gtd_inventory[TEXTDRAW_BOX], 0);
	
	gtd_inventory[TEXTDRAW_BOX_BUTTON_PAGE_ONE] = TextDrawCreate(158.7664, 104.1035, !"_"); // фон кнопки стр 1
	TextDrawLetterSize(gtd_inventory[TEXTDRAW_BOX_BUTTON_PAGE_ONE], 0.0000, 1.3999);
	TextDrawTextSize(gtd_inventory[TEXTDRAW_BOX_BUTTON_PAGE_ONE], 172.0997, 0.0000);
	TextDrawAlignment(gtd_inventory[TEXTDRAW_BOX_BUTTON_PAGE_ONE], 1);
	TextDrawColor(gtd_inventory[TEXTDRAW_BOX_BUTTON_PAGE_ONE], -1);
	TextDrawUseBox(gtd_inventory[TEXTDRAW_BOX_BUTTON_PAGE_ONE], 1);
	TextDrawBoxColor(gtd_inventory[TEXTDRAW_BOX_BUTTON_PAGE_ONE], 150);
	TextDrawBackgroundColor(gtd_inventory[TEXTDRAW_BOX_BUTTON_PAGE_ONE], 255);
	TextDrawFont(gtd_inventory[TEXTDRAW_BOX_BUTTON_PAGE_ONE], 1);
	TextDrawSetProportional(gtd_inventory[TEXTDRAW_BOX_BUTTON_PAGE_ONE], 1);
	TextDrawSetShadow(gtd_inventory[TEXTDRAW_BOX_BUTTON_PAGE_ONE], 0);
	TextDrawSetSelectable(gtd_inventory[TEXTDRAW_BOX_BUTTON_PAGE_ONE], true);

	gtd_inventory[TEXTDRAW_BOX_BUTTON_PAGE_TWO] = TextDrawCreate(140.0653, 104.1035, !"_"); // фон кнопки стр 2
	TextDrawLetterSize(gtd_inventory[TEXTDRAW_BOX_BUTTON_PAGE_TWO], 0.0000, 1.3999);
	TextDrawTextSize(gtd_inventory[TEXTDRAW_BOX_BUTTON_PAGE_TWO], 153.3986, 0.0000);
	TextDrawAlignment(gtd_inventory[TEXTDRAW_BOX_BUTTON_PAGE_TWO], 1);
	TextDrawColor(gtd_inventory[TEXTDRAW_BOX_BUTTON_PAGE_TWO], -1);
	TextDrawUseBox(gtd_inventory[TEXTDRAW_BOX_BUTTON_PAGE_TWO], 1);
	TextDrawBoxColor(gtd_inventory[TEXTDRAW_BOX_BUTTON_PAGE_TWO], 150);
	TextDrawBackgroundColor(gtd_inventory[TEXTDRAW_BOX_BUTTON_PAGE_TWO], 255);
	TextDrawFont(gtd_inventory[TEXTDRAW_BOX_BUTTON_PAGE_TWO], 1);
	TextDrawSetProportional(gtd_inventory[TEXTDRAW_BOX_BUTTON_PAGE_TWO], 1);
	TextDrawSetShadow(gtd_inventory[TEXTDRAW_BOX_BUTTON_PAGE_TWO], 0);
	TextDrawSetSelectable(gtd_inventory[TEXTDRAW_BOX_BUTTON_PAGE_TWO], true);

	gtd_inventory[TEXTDRAW_BOX_BUTTON_PAGE_THREE] = TextDrawCreate(120.8647, 104.1035, !"_"); // фон кнопки стр 3
	TextDrawLetterSize(gtd_inventory[TEXTDRAW_BOX_BUTTON_PAGE_THREE], 0.0000, 1.3999);
	TextDrawTextSize(gtd_inventory[TEXTDRAW_BOX_BUTTON_PAGE_THREE], 134.1974, 0.0000);
	TextDrawAlignment(gtd_inventory[TEXTDRAW_BOX_BUTTON_PAGE_THREE], 1);
	TextDrawColor(gtd_inventory[TEXTDRAW_BOX_BUTTON_PAGE_THREE], -1);
	TextDrawUseBox(gtd_inventory[TEXTDRAW_BOX_BUTTON_PAGE_THREE], 1);
	TextDrawBoxColor(gtd_inventory[TEXTDRAW_BOX_BUTTON_PAGE_THREE], 150);
	TextDrawBackgroundColor(gtd_inventory[TEXTDRAW_BOX_BUTTON_PAGE_THREE], 255);
	TextDrawFont(gtd_inventory[TEXTDRAW_BOX_BUTTON_PAGE_THREE], 1);
	TextDrawSetProportional(gtd_inventory[TEXTDRAW_BOX_BUTTON_PAGE_THREE], 1);
	TextDrawSetShadow(gtd_inventory[TEXTDRAW_BOX_BUTTON_PAGE_THREE], 0);
	TextDrawSetSelectable(gtd_inventory[TEXTDRAW_BOX_BUTTON_PAGE_THREE], true);
	
	gtd_inventory[TEXTDRAW_TEXT_PAGE_ONE] = TextDrawCreate(123.3332, 102.6740, "1"); // страница 1
	TextDrawLetterSize(gtd_inventory[TEXTDRAW_TEXT_PAGE_ONE], 0.3606, 1.4837);
	TextDrawTextSize(gtd_inventory[TEXTDRAW_TEXT_PAGE_ONE], 132.0000, 10.0000);
	TextDrawAlignment(gtd_inventory[TEXTDRAW_TEXT_PAGE_ONE], 1);
	TextDrawColor(gtd_inventory[TEXTDRAW_TEXT_PAGE_ONE], 16777215);
	TextDrawUseBox(gtd_inventory[TEXTDRAW_TEXT_PAGE_ONE], 1);
	TextDrawBoxColor(gtd_inventory[TEXTDRAW_TEXT_PAGE_ONE], 0);
	TextDrawSetOutline(gtd_inventory[TEXTDRAW_TEXT_PAGE_ONE], 1);
	TextDrawBackgroundColor(gtd_inventory[TEXTDRAW_TEXT_PAGE_ONE], 255);
	TextDrawFont(gtd_inventory[TEXTDRAW_TEXT_PAGE_ONE], 1);
	TextDrawSetProportional(gtd_inventory[TEXTDRAW_TEXT_PAGE_ONE], 1);
	TextDrawSetShadow(gtd_inventory[TEXTDRAW_TEXT_PAGE_ONE], 1);
	TextDrawSetSelectable(gtd_inventory[TEXTDRAW_TEXT_PAGE_ONE], true);

	gtd_inventory[TEXTDRAW_TEXT_PAGE_TWO] = TextDrawCreate(142.7998, 102.9739, "2"); // страница 2
	TextDrawLetterSize(gtd_inventory[TEXTDRAW_TEXT_PAGE_TWO], 0.3606, 1.4837);
	TextDrawTextSize(gtd_inventory[TEXTDRAW_TEXT_PAGE_TWO], 152.0000, 10.0000);
	TextDrawAlignment(gtd_inventory[TEXTDRAW_TEXT_PAGE_TWO], 1);
	TextDrawColor(gtd_inventory[TEXTDRAW_TEXT_PAGE_TWO], 16777215);
	TextDrawUseBox(gtd_inventory[TEXTDRAW_TEXT_PAGE_TWO], 1);
	TextDrawBoxColor(gtd_inventory[TEXTDRAW_TEXT_PAGE_TWO], 0);
	TextDrawSetOutline(gtd_inventory[TEXTDRAW_TEXT_PAGE_TWO], 1);
	TextDrawBackgroundColor(gtd_inventory[TEXTDRAW_TEXT_PAGE_TWO], 255);
	TextDrawFont(gtd_inventory[TEXTDRAW_TEXT_PAGE_TWO], 1);
	TextDrawSetProportional(gtd_inventory[TEXTDRAW_TEXT_PAGE_TWO], 1);
	TextDrawSetShadow(gtd_inventory[TEXTDRAW_TEXT_PAGE_TWO], 1);
	TextDrawSetSelectable(gtd_inventory[TEXTDRAW_TEXT_PAGE_TWO], true);

	gtd_inventory[TEXTDRAW_TEXT_PAGE_THREE] = TextDrawCreate(161.4333, 102.7740, "3"); // страница 3	
	TextDrawLetterSize(gtd_inventory[TEXTDRAW_TEXT_PAGE_THREE], 0.3606, 1.4837);
	TextDrawTextSize(gtd_inventory[TEXTDRAW_TEXT_PAGE_THREE], 169.0000, 10.0000);
	TextDrawAlignment(gtd_inventory[TEXTDRAW_TEXT_PAGE_THREE], 1);
	TextDrawColor(gtd_inventory[TEXTDRAW_TEXT_PAGE_THREE], 16777215);
	TextDrawUseBox(gtd_inventory[TEXTDRAW_TEXT_PAGE_THREE], 1);
	TextDrawBoxColor(gtd_inventory[TEXTDRAW_TEXT_PAGE_THREE], 0);
	TextDrawSetOutline(gtd_inventory[TEXTDRAW_TEXT_PAGE_THREE], 1);
	TextDrawBackgroundColor(gtd_inventory[TEXTDRAW_TEXT_PAGE_THREE], 255);
	TextDrawFont(gtd_inventory[TEXTDRAW_TEXT_PAGE_THREE], 1);
	TextDrawSetProportional(gtd_inventory[TEXTDRAW_TEXT_PAGE_THREE], 1);
	TextDrawSetShadow(gtd_inventory[TEXTDRAW_TEXT_PAGE_THREE], 1);
	TextDrawSetSelectable(gtd_inventory[TEXTDRAW_TEXT_PAGE_THREE], true);
	
	gtd_inventory[TEXTDRAW_TEXT_CLOSE] = TextDrawCreate(13.1999, 105.4369, "X"); // кнопка закрытия инвентаря
	TextDrawLetterSize(gtd_inventory[TEXTDRAW_TEXT_CLOSE], 0.3499, 1.4831);
	TextDrawTextSize(gtd_inventory[TEXTDRAW_TEXT_CLOSE], 20.0000, 10.0000);
	TextDrawAlignment(gtd_inventory[TEXTDRAW_TEXT_CLOSE], 1);
	TextDrawColor(gtd_inventory[TEXTDRAW_TEXT_CLOSE], -16776961);
	TextDrawUseBox(gtd_inventory[TEXTDRAW_TEXT_CLOSE], 1);
	TextDrawBoxColor(gtd_inventory[TEXTDRAW_TEXT_CLOSE], -16777216);
	TextDrawSetOutline(gtd_inventory[TEXTDRAW_TEXT_CLOSE], 1);
	TextDrawBackgroundColor(gtd_inventory[TEXTDRAW_TEXT_CLOSE], 255);
	TextDrawFont(gtd_inventory[TEXTDRAW_TEXT_CLOSE], 1);
	TextDrawSetProportional(gtd_inventory[TEXTDRAW_TEXT_CLOSE], 1);
	TextDrawSetShadow(gtd_inventory[TEXTDRAW_TEXT_CLOSE], 1);
	TextDrawSetSelectable(gtd_inventory[TEXTDRAW_TEXT_CLOSE], true);
	
	gtd_inventory[TEXTDRAW_BOX_SECONDARY_INV] = TextDrawCreate(467.9999, 121.1434, !"_"); // фон вторичного инвентаря (вторичные слоты)
	TextDrawLetterSize(gtd_inventory[TEXTDRAW_BOX_SECONDARY_INV], 0.0000, 19.9664);
	TextDrawTextSize(gtd_inventory[TEXTDRAW_BOX_SECONDARY_INV], 631.0000, 0.0000);
	TextDrawAlignment(gtd_inventory[TEXTDRAW_BOX_SECONDARY_INV], 1);
	TextDrawColor(gtd_inventory[TEXTDRAW_BOX_SECONDARY_INV], -1);
	TextDrawUseBox(gtd_inventory[TEXTDRAW_BOX_SECONDARY_INV], 1);
	TextDrawBoxColor(gtd_inventory[TEXTDRAW_BOX_SECONDARY_INV], 150);
	TextDrawBackgroundColor(gtd_inventory[TEXTDRAW_BOX_SECONDARY_INV], 255);
	TextDrawFont(gtd_inventory[TEXTDRAW_BOX_SECONDARY_INV], 1);
	TextDrawSetProportional(gtd_inventory[TEXTDRAW_BOX_SECONDARY_INV], 1);
	TextDrawSetShadow(gtd_inventory[TEXTDRAW_BOX_SECONDARY_INV], 0);
	return true;
}

stock Inventory_CreatePlayerTextDraw(playerid) {
	td_buttons[playerid][0] = CreatePlayerTextDraw(playerid, 36.9999, 168.4295, !"_"); // пусто
	PlayerTextDrawLetterSize(playerid, td_buttons[playerid][0], 0.0000, 5.5000);
	PlayerTextDrawTextSize(playerid, td_buttons[playerid][0], 63.0000, 0.0000);
	PlayerTextDrawAlignment(playerid, td_buttons[playerid][0], 1);
	PlayerTextDrawColor(playerid, td_buttons[playerid][0], -1);
	PlayerTextDrawUseBox(playerid, td_buttons[playerid][0], 1);
	PlayerTextDrawBoxColor(playerid, td_buttons[playerid][0], 150);
	PlayerTextDrawBackgroundColor(playerid, td_buttons[playerid][0], 255);
	PlayerTextDrawFont(playerid, td_buttons[playerid][0], 1);
	PlayerTextDrawSetProportional(playerid, td_buttons[playerid][0], 1);
	PlayerTextDrawSetShadow(playerid, td_buttons[playerid][0], 0);

	td_buttons[playerid][1] = CreatePlayerTextDraw(playerid, 37.8666, 169.4888, !"_"); // пусто
	PlayerTextDrawLetterSize(playerid, td_buttons[playerid][1], 0.0000, 0.6226);
	PlayerTextDrawTextSize(playerid, td_buttons[playerid][1], 62.0000, 0.0000);
	PlayerTextDrawAlignment(playerid, td_buttons[playerid][1], 1);
	PlayerTextDrawColor(playerid, td_buttons[playerid][1], -1);
	PlayerTextDrawUseBox(playerid, td_buttons[playerid][1], 1);
	PlayerTextDrawBoxColor(playerid, td_buttons[playerid][1], -1061109615);
	PlayerTextDrawBackgroundColor(playerid, td_buttons[playerid][1], 255);
	PlayerTextDrawFont(playerid, td_buttons[playerid][1], 1);
	PlayerTextDrawSetProportional(playerid, td_buttons[playerid][1], 1);
	PlayerTextDrawSetShadow(playerid, td_buttons[playerid][1], 0);

	td_buttons[playerid][2] = CreatePlayerTextDraw(playerid, 37.8666, 179.8894, !"_"); // пусто
	PlayerTextDrawLetterSize(playerid, td_buttons[playerid][2], 0.0000, 0.6226);
	PlayerTextDrawTextSize(playerid, td_buttons[playerid][2], 62.0000, 0.0000);
	PlayerTextDrawAlignment(playerid, td_buttons[playerid][2], 1);
	PlayerTextDrawColor(playerid, td_buttons[playerid][2], -1);
	PlayerTextDrawUseBox(playerid, td_buttons[playerid][2], 1);
	PlayerTextDrawBoxColor(playerid, td_buttons[playerid][2], -1061109615);
	PlayerTextDrawBackgroundColor(playerid, td_buttons[playerid][2], 255);
	PlayerTextDrawFont(playerid, td_buttons[playerid][2], 1);
	PlayerTextDrawSetProportional(playerid, td_buttons[playerid][2], 1);
	PlayerTextDrawSetShadow(playerid, td_buttons[playerid][2], 0);

	td_buttons[playerid][3] = CreatePlayerTextDraw(playerid, 37.8666, 189.9900, !"_"); // пусто
	PlayerTextDrawLetterSize(playerid, td_buttons[playerid][3], 0.0000, 0.6226);
	PlayerTextDrawTextSize(playerid, td_buttons[playerid][3], 62.0000, 0.0000);
	PlayerTextDrawAlignment(playerid, td_buttons[playerid][3], 1);
	PlayerTextDrawColor(playerid, td_buttons[playerid][3], -1);
	PlayerTextDrawUseBox(playerid, td_buttons[playerid][3], 1);
	PlayerTextDrawBoxColor(playerid, td_buttons[playerid][3], -1061109615);
	PlayerTextDrawBackgroundColor(playerid, td_buttons[playerid][3], 255);
	PlayerTextDrawFont(playerid, td_buttons[playerid][3], 1);
	PlayerTextDrawSetProportional(playerid, td_buttons[playerid][3], 1);
	PlayerTextDrawSetShadow(playerid, td_buttons[playerid][3], 0);

	td_buttons[playerid][4] = CreatePlayerTextDraw(playerid, 37.8666, 200.1907, !"_"); // пусто
	PlayerTextDrawLetterSize(playerid, td_buttons[playerid][4], 0.0000, 0.6226);
	PlayerTextDrawTextSize(playerid, td_buttons[playerid][4], 62.0000, 0.0000);
	PlayerTextDrawAlignment(playerid, td_buttons[playerid][4], 1);
	PlayerTextDrawColor(playerid, td_buttons[playerid][4], -1);
	PlayerTextDrawUseBox(playerid, td_buttons[playerid][4], 1);
	PlayerTextDrawBoxColor(playerid, td_buttons[playerid][4], -1061109615);
	PlayerTextDrawBackgroundColor(playerid, td_buttons[playerid][4], 255);
	PlayerTextDrawFont(playerid, td_buttons[playerid][4], 1);
	PlayerTextDrawSetProportional(playerid, td_buttons[playerid][4], 1);
	PlayerTextDrawSetShadow(playerid, td_buttons[playerid][4], 0);

	td_buttons[playerid][5] = CreatePlayerTextDraw(playerid, 37.8666, 210.8914, !"_"); // пусто
	PlayerTextDrawLetterSize(playerid, td_buttons[playerid][5], 0.0000, 0.6226);
	PlayerTextDrawTextSize(playerid, td_buttons[playerid][5], 62.0000, 0.0000);
	PlayerTextDrawAlignment(playerid, td_buttons[playerid][5], 1);
	PlayerTextDrawColor(playerid, td_buttons[playerid][5], -1);
	PlayerTextDrawUseBox(playerid, td_buttons[playerid][5], 1);
	PlayerTextDrawBoxColor(playerid, td_buttons[playerid][5], -16777076);
	PlayerTextDrawBackgroundColor(playerid, td_buttons[playerid][5], 120);
	PlayerTextDrawFont(playerid, td_buttons[playerid][5], 1);
	PlayerTextDrawSetProportional(playerid, td_buttons[playerid][5], 1);
	PlayerTextDrawSetShadow(playerid, td_buttons[playerid][5], 0);

	td_buttons[playerid][6] = CreatePlayerTextDraw(playerid, 37.2332, 167.2554, "ИСПОЛЬЗОВАТЬ"); // пусто
	PlayerTextDrawLetterSize(playerid, td_buttons[playerid][6], 0.1030, 0.9943);
	PlayerTextDrawTextSize(playerid, td_buttons[playerid][6], 62.0000, 10.0000);
	PlayerTextDrawAlignment(playerid, td_buttons[playerid][6], 1);
	PlayerTextDrawColor(playerid, td_buttons[playerid][6], -1);
	PlayerTextDrawUseBox(playerid, td_buttons[playerid][6], 1);
	PlayerTextDrawBoxColor(playerid, td_buttons[playerid][6], 0);
	PlayerTextDrawBackgroundColor(playerid, td_buttons[playerid][6], 255);
	PlayerTextDrawFont(playerid, td_buttons[playerid][6], 1);
	PlayerTextDrawSetProportional(playerid, td_buttons[playerid][6], 1);
	PlayerTextDrawSetShadow(playerid, td_buttons[playerid][6], 0);
	PlayerTextDrawSetSelectable(playerid, td_buttons[playerid][6], true);

	td_buttons[playerid][7] = CreatePlayerTextDraw(playerid, 37.2332, 177.5561, "ПЕРЕДАТЬ"); // пусто
	PlayerTextDrawLetterSize(playerid, td_buttons[playerid][7], 0.1659, 0.9943);
	PlayerTextDrawTextSize(playerid, td_buttons[playerid][7], 61.1697, 10.0000);
	PlayerTextDrawAlignment(playerid, td_buttons[playerid][7], 1);
	PlayerTextDrawColor(playerid, td_buttons[playerid][7], -1);
	PlayerTextDrawUseBox(playerid, td_buttons[playerid][7], 1);
	PlayerTextDrawBoxColor(playerid, td_buttons[playerid][7], 0);
	PlayerTextDrawBackgroundColor(playerid, td_buttons[playerid][7], 255);
	PlayerTextDrawFont(playerid, td_buttons[playerid][7], 1);
	PlayerTextDrawSetProportional(playerid, td_buttons[playerid][7], 1);
	PlayerTextDrawSetShadow(playerid, td_buttons[playerid][7], 0);
	PlayerTextDrawSetSelectable(playerid, td_buttons[playerid][7], true);

	td_buttons[playerid][8] = CreatePlayerTextDraw(playerid, 37.2332, 187.6566, "ИНФОРМАЦИЯ"); // пусто
	PlayerTextDrawLetterSize(playerid, td_buttons[playerid][8], 0.1159, 1.0025);
	PlayerTextDrawTextSize(playerid, td_buttons[playerid][8], 62.0000, 10.0000);
	PlayerTextDrawAlignment(playerid, td_buttons[playerid][8], 1);
	PlayerTextDrawColor(playerid, td_buttons[playerid][8], -1);
	PlayerTextDrawUseBox(playerid, td_buttons[playerid][8], 1);
	PlayerTextDrawBoxColor(playerid, td_buttons[playerid][8], 0);
	PlayerTextDrawBackgroundColor(playerid, td_buttons[playerid][8], 255);
	PlayerTextDrawFont(playerid, td_buttons[playerid][8], 1);
	PlayerTextDrawSetProportional(playerid, td_buttons[playerid][8], 1);
	PlayerTextDrawSetShadow(playerid, td_buttons[playerid][8], 0);
	PlayerTextDrawSetSelectable(playerid, td_buttons[playerid][8], true);

	td_buttons[playerid][9] = CreatePlayerTextDraw(playerid, 37.2332, 197.9575, "ВЫБРОСИТЬ"); // пусто
	PlayerTextDrawLetterSize(playerid, td_buttons[playerid][9], 0.1393, 1.0109);
	PlayerTextDrawTextSize(playerid, td_buttons[playerid][9], 62.0000, 10.0000);
	PlayerTextDrawAlignment(playerid, td_buttons[playerid][9], 1);
	PlayerTextDrawColor(playerid, td_buttons[playerid][9], -1);
	PlayerTextDrawUseBox(playerid, td_buttons[playerid][9], 1);
	PlayerTextDrawBoxColor(playerid, td_buttons[playerid][9], 0);
	PlayerTextDrawBackgroundColor(playerid, td_buttons[playerid][9], 255);
	PlayerTextDrawFont(playerid, td_buttons[playerid][9], 1);
	PlayerTextDrawSetProportional(playerid, td_buttons[playerid][9], 1);
	PlayerTextDrawSetShadow(playerid, td_buttons[playerid][9], 0);
	PlayerTextDrawSetSelectable(playerid, td_buttons[playerid][9], true);

	td_buttons[playerid][10] = CreatePlayerTextDraw(playerid, 37.3333, 208.5581, "ЗАКРЫТЬ"); // пусто
	PlayerTextDrawLetterSize(playerid, td_buttons[playerid][10], 0.1743, 0.9983);
	PlayerTextDrawTextSize(playerid, td_buttons[playerid][10], 62.0998, 10.0000);
	PlayerTextDrawAlignment(playerid, td_buttons[playerid][10], 1);
	PlayerTextDrawColor(playerid, td_buttons[playerid][10], -1);
	PlayerTextDrawUseBox(playerid, td_buttons[playerid][10], 1);
	PlayerTextDrawBoxColor(playerid, td_buttons[playerid][10], 0);
	PlayerTextDrawBackgroundColor(playerid, td_buttons[playerid][10], 255);
	PlayerTextDrawFont(playerid, td_buttons[playerid][10], 1);
	PlayerTextDrawSetProportional(playerid, td_buttons[playerid][10], 1);
	PlayerTextDrawSetShadow(playerid, td_buttons[playerid][10], 0);
	PlayerTextDrawSetSelectable(playerid, td_buttons[playerid][10], true);

	ptd_inventory[playerid][SLOT_ICON][0] = CreatePlayerTextDraw(playerid, 14.0000, 121.8143, ""); // пусто
	PlayerTextDrawTextSize(playerid, ptd_inventory[playerid][SLOT_ICON][0], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, ptd_inventory[playerid][SLOT_ICON][0], 1);
	PlayerTextDrawColor(playerid, ptd_inventory[playerid][SLOT_ICON][0], -1);
	PlayerTextDrawBackgroundColor(playerid, ptd_inventory[playerid][SLOT_ICON][0], 190);
	PlayerTextDrawFont(playerid, ptd_inventory[playerid][SLOT_ICON][0], 5);
	PlayerTextDrawSetProportional(playerid, ptd_inventory[playerid][SLOT_ICON][0], 0);
	PlayerTextDrawSetShadow(playerid, ptd_inventory[playerid][SLOT_ICON][0], 0);
	PlayerTextDrawSetSelectable(playerid, ptd_inventory[playerid][SLOT_ICON][0], true);
	PlayerTextDrawSetPreviewModel(playerid, ptd_inventory[playerid][SLOT_ICON][0], 19374);
	PlayerTextDrawSetPreviewRot(playerid, ptd_inventory[playerid][SLOT_ICON][0], 0.0000, 0.0000, 0.0000, 1.0000);

	ptd_inventory[playerid][SLOT_ICON][1] = CreatePlayerTextDraw(playerid, 53.3666, 121.8143, ""); // пусто
	PlayerTextDrawTextSize(playerid, ptd_inventory[playerid][SLOT_ICON][1], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, ptd_inventory[playerid][SLOT_ICON][1], 1);
	PlayerTextDrawColor(playerid, ptd_inventory[playerid][SLOT_ICON][1], -1);
	PlayerTextDrawBackgroundColor(playerid, ptd_inventory[playerid][SLOT_ICON][1], 190);
	PlayerTextDrawFont(playerid, ptd_inventory[playerid][SLOT_ICON][1], 5);
	PlayerTextDrawSetProportional(playerid, ptd_inventory[playerid][SLOT_ICON][1], 0);
	PlayerTextDrawSetShadow(playerid, ptd_inventory[playerid][SLOT_ICON][1], 0);
	PlayerTextDrawSetSelectable(playerid, ptd_inventory[playerid][SLOT_ICON][1], true);
	PlayerTextDrawSetPreviewModel(playerid, ptd_inventory[playerid][SLOT_ICON][1], 19374);
	PlayerTextDrawSetPreviewRot(playerid, ptd_inventory[playerid][SLOT_ICON][1], 0.0000, 0.0000, 0.0000, 1.0000);

	ptd_inventory[playerid][SLOT_ICON][2] = CreatePlayerTextDraw(playerid, 93.9664, 121.5699, ""); // пусто
	PlayerTextDrawTextSize(playerid, ptd_inventory[playerid][SLOT_ICON][2], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, ptd_inventory[playerid][SLOT_ICON][2], 1);
	PlayerTextDrawColor(playerid, ptd_inventory[playerid][SLOT_ICON][2], -1);
	PlayerTextDrawBackgroundColor(playerid, ptd_inventory[playerid][SLOT_ICON][2], 190);
	PlayerTextDrawFont(playerid, ptd_inventory[playerid][SLOT_ICON][2], 5);
	PlayerTextDrawSetProportional(playerid, ptd_inventory[playerid][SLOT_ICON][2], 0);
	PlayerTextDrawSetShadow(playerid, ptd_inventory[playerid][SLOT_ICON][2], 0);
	PlayerTextDrawSetSelectable(playerid, ptd_inventory[playerid][SLOT_ICON][2], true);
	PlayerTextDrawSetPreviewModel(playerid, ptd_inventory[playerid][SLOT_ICON][2], 19374);
	PlayerTextDrawSetPreviewRot(playerid, ptd_inventory[playerid][SLOT_ICON][2], 0.0000, 0.0000, 0.0000, 1.0000);

	ptd_inventory[playerid][SLOT_ICON][3] = CreatePlayerTextDraw(playerid, 134.0000, 121.5550, ""); // пусто
	PlayerTextDrawTextSize(playerid, ptd_inventory[playerid][SLOT_ICON][3], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, ptd_inventory[playerid][SLOT_ICON][3], 1);
	PlayerTextDrawColor(playerid, ptd_inventory[playerid][SLOT_ICON][3], -1);
	PlayerTextDrawBackgroundColor(playerid, ptd_inventory[playerid][SLOT_ICON][3], 190);
	PlayerTextDrawFont(playerid, ptd_inventory[playerid][SLOT_ICON][3], 5);
	PlayerTextDrawSetProportional(playerid, ptd_inventory[playerid][SLOT_ICON][3], 0);
	PlayerTextDrawSetShadow(playerid, ptd_inventory[playerid][SLOT_ICON][3], 0);
	PlayerTextDrawSetSelectable(playerid, ptd_inventory[playerid][SLOT_ICON][3], true);
	PlayerTextDrawSetPreviewModel(playerid, ptd_inventory[playerid][SLOT_ICON][3], 19374);
	PlayerTextDrawSetPreviewRot(playerid, ptd_inventory[playerid][SLOT_ICON][3], 0.0000, 0.0000, 0.0000, 1.0000);

	ptd_inventory[playerid][SLOT_ICON][4] = CreatePlayerTextDraw(playerid, 13.6667, 165.0256, ""); // пусто
	PlayerTextDrawTextSize(playerid, ptd_inventory[playerid][SLOT_ICON][4], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, ptd_inventory[playerid][SLOT_ICON][4], 1);
	PlayerTextDrawColor(playerid, ptd_inventory[playerid][SLOT_ICON][4], -1);
	PlayerTextDrawBackgroundColor(playerid, ptd_inventory[playerid][SLOT_ICON][4], 190);
	PlayerTextDrawFont(playerid, ptd_inventory[playerid][SLOT_ICON][4], 5);
	PlayerTextDrawSetProportional(playerid, ptd_inventory[playerid][SLOT_ICON][4], 0);
	PlayerTextDrawSetShadow(playerid, ptd_inventory[playerid][SLOT_ICON][4], 0);
	PlayerTextDrawSetSelectable(playerid, ptd_inventory[playerid][SLOT_ICON][4], true);
	PlayerTextDrawSetPreviewModel(playerid, ptd_inventory[playerid][SLOT_ICON][4], 19374);
	PlayerTextDrawSetPreviewRot(playerid, ptd_inventory[playerid][SLOT_ICON][4], 0.0000, 0.0000, 0.0000, 1.0000);

	ptd_inventory[playerid][SLOT_ICON][5] = CreatePlayerTextDraw(playerid, 53.6333, 165.0256, ""); // пусто
	PlayerTextDrawTextSize(playerid, ptd_inventory[playerid][SLOT_ICON][5], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, ptd_inventory[playerid][SLOT_ICON][5], 1);
	PlayerTextDrawColor(playerid, ptd_inventory[playerid][SLOT_ICON][5], -1);
	PlayerTextDrawBackgroundColor(playerid, ptd_inventory[playerid][SLOT_ICON][5], 190);
	PlayerTextDrawFont(playerid, ptd_inventory[playerid][SLOT_ICON][5], 5);
	PlayerTextDrawSetProportional(playerid, ptd_inventory[playerid][SLOT_ICON][5], 0);
	PlayerTextDrawSetShadow(playerid, ptd_inventory[playerid][SLOT_ICON][5], 0);
	PlayerTextDrawSetSelectable(playerid, ptd_inventory[playerid][SLOT_ICON][5], true);
	PlayerTextDrawSetPreviewModel(playerid, ptd_inventory[playerid][SLOT_ICON][5], 19374);
	PlayerTextDrawSetPreviewRot(playerid, ptd_inventory[playerid][SLOT_ICON][5], 0.0000, 0.0000, 0.0000, 1.0000);

	ptd_inventory[playerid][SLOT_ICON][6] = CreatePlayerTextDraw(playerid, 94.0000, 165.3699, ""); // пусто
	PlayerTextDrawTextSize(playerid, ptd_inventory[playerid][SLOT_ICON][6], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, ptd_inventory[playerid][SLOT_ICON][6], 1);
	PlayerTextDrawColor(playerid, ptd_inventory[playerid][SLOT_ICON][6], -1);
	PlayerTextDrawBackgroundColor(playerid, ptd_inventory[playerid][SLOT_ICON][6], 190);
	PlayerTextDrawFont(playerid, ptd_inventory[playerid][SLOT_ICON][6], 5);
	PlayerTextDrawSetProportional(playerid, ptd_inventory[playerid][SLOT_ICON][6], 0);
	PlayerTextDrawSetShadow(playerid, ptd_inventory[playerid][SLOT_ICON][6], 0);
	PlayerTextDrawSetSelectable(playerid, ptd_inventory[playerid][SLOT_ICON][6], true);
	PlayerTextDrawSetPreviewModel(playerid, ptd_inventory[playerid][SLOT_ICON][6], 19374);
	PlayerTextDrawSetPreviewRot(playerid, ptd_inventory[playerid][SLOT_ICON][6], 0.0000, 0.0000, 0.0000, 1.0000);

	ptd_inventory[playerid][SLOT_ICON][7] = CreatePlayerTextDraw(playerid, 134.0000, 165.1846, ""); // пусто
	PlayerTextDrawTextSize(playerid, ptd_inventory[playerid][SLOT_ICON][7], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, ptd_inventory[playerid][SLOT_ICON][7], 1);
	PlayerTextDrawColor(playerid, ptd_inventory[playerid][SLOT_ICON][7], -1);
	PlayerTextDrawBackgroundColor(playerid, ptd_inventory[playerid][SLOT_ICON][7], 190);
	PlayerTextDrawFont(playerid, ptd_inventory[playerid][SLOT_ICON][7], 5);
	PlayerTextDrawSetProportional(playerid, ptd_inventory[playerid][SLOT_ICON][7], 0);
	PlayerTextDrawSetShadow(playerid, ptd_inventory[playerid][SLOT_ICON][7], 0);
	PlayerTextDrawSetSelectable(playerid, ptd_inventory[playerid][SLOT_ICON][7], true);
	PlayerTextDrawSetPreviewModel(playerid, ptd_inventory[playerid][SLOT_ICON][7], 19374);
	PlayerTextDrawSetPreviewRot(playerid, ptd_inventory[playerid][SLOT_ICON][7], 0.0000, 0.0000, 0.0000, 1.0000);

	ptd_inventory[playerid][SLOT_ICON][8] = CreatePlayerTextDraw(playerid, 13.6666, 208.3697, ""); // пусто
	PlayerTextDrawTextSize(playerid, ptd_inventory[playerid][SLOT_ICON][8], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, ptd_inventory[playerid][SLOT_ICON][8], 1);
	PlayerTextDrawColor(playerid, ptd_inventory[playerid][SLOT_ICON][8], -1);
	PlayerTextDrawBackgroundColor(playerid, ptd_inventory[playerid][SLOT_ICON][8], 190);
	PlayerTextDrawFont(playerid, ptd_inventory[playerid][SLOT_ICON][8], 5);
	PlayerTextDrawSetProportional(playerid, ptd_inventory[playerid][SLOT_ICON][8], 0);
	PlayerTextDrawSetShadow(playerid, ptd_inventory[playerid][SLOT_ICON][8], 0);
	PlayerTextDrawSetSelectable(playerid, ptd_inventory[playerid][SLOT_ICON][8], true);
	PlayerTextDrawSetPreviewModel(playerid, ptd_inventory[playerid][SLOT_ICON][8], 19374);
	PlayerTextDrawSetPreviewRot(playerid, ptd_inventory[playerid][SLOT_ICON][8], 0.0000, 0.0000, 0.0000, 1.0000);

	ptd_inventory[playerid][SLOT_ICON][9] = CreatePlayerTextDraw(playerid, 53.4667, 208.4846, ""); // пусто
	PlayerTextDrawTextSize(playerid, ptd_inventory[playerid][SLOT_ICON][9], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, ptd_inventory[playerid][SLOT_ICON][9], 1);
	PlayerTextDrawColor(playerid, ptd_inventory[playerid][SLOT_ICON][9], -1);
	PlayerTextDrawBackgroundColor(playerid, ptd_inventory[playerid][SLOT_ICON][9], 190);
	PlayerTextDrawFont(playerid, ptd_inventory[playerid][SLOT_ICON][9], 5);
	PlayerTextDrawSetProportional(playerid, ptd_inventory[playerid][SLOT_ICON][9], 0);
	PlayerTextDrawSetShadow(playerid, ptd_inventory[playerid][SLOT_ICON][9], 0);
	PlayerTextDrawSetSelectable(playerid, ptd_inventory[playerid][SLOT_ICON][9], true);
	PlayerTextDrawSetPreviewModel(playerid, ptd_inventory[playerid][SLOT_ICON][9], 19374);
	PlayerTextDrawSetPreviewRot(playerid, ptd_inventory[playerid][SLOT_ICON][9], 0.0000, 0.0000, 0.0000, 1.0000);

	ptd_inventory[playerid][SLOT_ICON][10] = CreatePlayerTextDraw(playerid, 93.7667, 208.2993, ""); // пусто
	PlayerTextDrawTextSize(playerid, ptd_inventory[playerid][SLOT_ICON][10], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, ptd_inventory[playerid][SLOT_ICON][10], 1);
	PlayerTextDrawColor(playerid, ptd_inventory[playerid][SLOT_ICON][10], -1);
	PlayerTextDrawBackgroundColor(playerid, ptd_inventory[playerid][SLOT_ICON][10], 190);
	PlayerTextDrawFont(playerid, ptd_inventory[playerid][SLOT_ICON][10], 5);
	PlayerTextDrawSetProportional(playerid, ptd_inventory[playerid][SLOT_ICON][10], 0);
	PlayerTextDrawSetShadow(playerid, ptd_inventory[playerid][SLOT_ICON][10], 0);
	PlayerTextDrawSetSelectable(playerid, ptd_inventory[playerid][SLOT_ICON][10], true);
	PlayerTextDrawSetPreviewModel(playerid, ptd_inventory[playerid][SLOT_ICON][10], 19374);
	PlayerTextDrawSetPreviewRot(playerid, ptd_inventory[playerid][SLOT_ICON][10], 0.0000, 0.0000, 0.0000, 1.0000);

	ptd_inventory[playerid][SLOT_ICON][11] = CreatePlayerTextDraw(playerid, 133.8000, 208.5290, ""); // пусто
	PlayerTextDrawTextSize(playerid, ptd_inventory[playerid][SLOT_ICON][11], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, ptd_inventory[playerid][SLOT_ICON][11], 1);
	PlayerTextDrawColor(playerid, ptd_inventory[playerid][SLOT_ICON][11], -1);
	PlayerTextDrawBackgroundColor(playerid, ptd_inventory[playerid][SLOT_ICON][11], 190);
	PlayerTextDrawFont(playerid, ptd_inventory[playerid][SLOT_ICON][11], 5);
	PlayerTextDrawSetProportional(playerid, ptd_inventory[playerid][SLOT_ICON][11], 0);
	PlayerTextDrawSetShadow(playerid, ptd_inventory[playerid][SLOT_ICON][11], 0);
	PlayerTextDrawSetSelectable(playerid, ptd_inventory[playerid][SLOT_ICON][11], true);
	PlayerTextDrawSetPreviewModel(playerid, ptd_inventory[playerid][SLOT_ICON][11], 19374);
	PlayerTextDrawSetPreviewRot(playerid, ptd_inventory[playerid][SLOT_ICON][11], 0.0000, 0.0000, 0.0000, 1.0000);

	ptd_inventory[playerid][SLOT_ICON][12] = CreatePlayerTextDraw(playerid, 13.3999, 252.2142, ""); // пусто
	PlayerTextDrawTextSize(playerid, ptd_inventory[playerid][SLOT_ICON][12], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, ptd_inventory[playerid][SLOT_ICON][12], 1);
	PlayerTextDrawColor(playerid, ptd_inventory[playerid][SLOT_ICON][12], -1);
	PlayerTextDrawBackgroundColor(playerid, ptd_inventory[playerid][SLOT_ICON][12], 190);
	PlayerTextDrawFont(playerid, ptd_inventory[playerid][SLOT_ICON][12], 5);
	PlayerTextDrawSetProportional(playerid, ptd_inventory[playerid][SLOT_ICON][12], 0);
	PlayerTextDrawSetShadow(playerid, ptd_inventory[playerid][SLOT_ICON][12], 0);
	PlayerTextDrawSetSelectable(playerid, ptd_inventory[playerid][SLOT_ICON][12], true);
	PlayerTextDrawSetPreviewModel(playerid, ptd_inventory[playerid][SLOT_ICON][12], 19374);
	PlayerTextDrawSetPreviewRot(playerid, ptd_inventory[playerid][SLOT_ICON][12], 0.0000, 0.0000, 0.0000, 1.0000);

	ptd_inventory[playerid][SLOT_ICON][13] = CreatePlayerTextDraw(playerid, 53.6333, 252.2586, ""); // пусто
	PlayerTextDrawTextSize(playerid, ptd_inventory[playerid][SLOT_ICON][13], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, ptd_inventory[playerid][SLOT_ICON][13], 1);
	PlayerTextDrawColor(playerid, ptd_inventory[playerid][SLOT_ICON][13], -1);
	PlayerTextDrawBackgroundColor(playerid, ptd_inventory[playerid][SLOT_ICON][13], 190);
	PlayerTextDrawFont(playerid, ptd_inventory[playerid][SLOT_ICON][13], 5);
	PlayerTextDrawSetProportional(playerid, ptd_inventory[playerid][SLOT_ICON][13], 0);
	PlayerTextDrawSetShadow(playerid, ptd_inventory[playerid][SLOT_ICON][13], 0);
	PlayerTextDrawSetSelectable(playerid, ptd_inventory[playerid][SLOT_ICON][13], true);
	PlayerTextDrawSetPreviewModel(playerid, ptd_inventory[playerid][SLOT_ICON][13], 19374);
	PlayerTextDrawSetPreviewRot(playerid, ptd_inventory[playerid][SLOT_ICON][13], 0.0000, 0.0000, 0.0000, 1.0000);

	ptd_inventory[playerid][SLOT_ICON][14] = CreatePlayerTextDraw(playerid, 93.8330, 252.4882, ""); // пусто
	PlayerTextDrawTextSize(playerid, ptd_inventory[playerid][SLOT_ICON][14], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, ptd_inventory[playerid][SLOT_ICON][14], 1);
	PlayerTextDrawColor(playerid, ptd_inventory[playerid][SLOT_ICON][14], -1);
	PlayerTextDrawBackgroundColor(playerid, ptd_inventory[playerid][SLOT_ICON][14], 190);
	PlayerTextDrawFont(playerid, ptd_inventory[playerid][SLOT_ICON][14], 5);
	PlayerTextDrawSetProportional(playerid, ptd_inventory[playerid][SLOT_ICON][14], 0);
	PlayerTextDrawSetShadow(playerid, ptd_inventory[playerid][SLOT_ICON][14], 0);
	PlayerTextDrawSetSelectable(playerid, ptd_inventory[playerid][SLOT_ICON][14], true);
	PlayerTextDrawSetPreviewModel(playerid, ptd_inventory[playerid][SLOT_ICON][14], 19374);
	PlayerTextDrawSetPreviewRot(playerid, ptd_inventory[playerid][SLOT_ICON][14], 0.0000, 0.0000, 0.0000, 1.0000);

	ptd_inventory[playerid][SLOT_ICON][15] = CreatePlayerTextDraw(playerid, 133.7666, 252.2438, ""); // пусто
	PlayerTextDrawTextSize(playerid, ptd_inventory[playerid][SLOT_ICON][15], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, ptd_inventory[playerid][SLOT_ICON][15], 1);
	PlayerTextDrawColor(playerid, ptd_inventory[playerid][SLOT_ICON][15], -1);
	PlayerTextDrawBackgroundColor(playerid, ptd_inventory[playerid][SLOT_ICON][15], 190);
	PlayerTextDrawFont(playerid, ptd_inventory[playerid][SLOT_ICON][15], 5);
	PlayerTextDrawSetProportional(playerid, ptd_inventory[playerid][SLOT_ICON][15], 0);
	PlayerTextDrawSetShadow(playerid, ptd_inventory[playerid][SLOT_ICON][15], 0);
	PlayerTextDrawSetSelectable(playerid, ptd_inventory[playerid][SLOT_ICON][15], true);
	PlayerTextDrawSetPreviewModel(playerid, ptd_inventory[playerid][SLOT_ICON][15], 19374);
	PlayerTextDrawSetPreviewRot(playerid, ptd_inventory[playerid][SLOT_ICON][15], 0.0000, 0.0000, 0.0000, 1.0000);

	ptd_inventory[playerid][PLAYER_SKIN] = CreatePlayerTextDraw(playerid, 494.5664, 114.9108, ""); // пусто
	PlayerTextDrawTextSize(playerid, ptd_inventory[playerid][PLAYER_SKIN], 106.0000, 104.0000);
	PlayerTextDrawAlignment(playerid, ptd_inventory[playerid][PLAYER_SKIN], 1);
	PlayerTextDrawColor(playerid, ptd_inventory[playerid][PLAYER_SKIN], -1);
	PlayerTextDrawBackgroundColor(playerid, ptd_inventory[playerid][PLAYER_SKIN], 0);
	PlayerTextDrawFont(playerid, ptd_inventory[playerid][PLAYER_SKIN], 5);
	PlayerTextDrawSetProportional(playerid, ptd_inventory[playerid][PLAYER_SKIN], 0);
	PlayerTextDrawSetShadow(playerid, ptd_inventory[playerid][PLAYER_SKIN], 0);
	PlayerTextDrawSetPreviewModel(playerid, ptd_inventory[playerid][PLAYER_SKIN], 0);
	PlayerTextDrawSetPreviewRot(playerid, ptd_inventory[playerid][PLAYER_SKIN], 0.0000, 0.0000, 180.0000, 1.0000);

	ptd_inventory[playerid][OTHER_SLOT_ICON][0] = CreatePlayerTextDraw(playerid, 468.0000, 121.3995, ""); // пусто
	PlayerTextDrawTextSize(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][0], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][0], 1);
	PlayerTextDrawColor(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][0], -1);
	PlayerTextDrawBackgroundColor(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][0], 190);
	PlayerTextDrawFont(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][0], 5);
	PlayerTextDrawSetProportional(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][0], 0);
	PlayerTextDrawSetShadow(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][0], 0);
	PlayerTextDrawSetSelectable(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][0], true);
	PlayerTextDrawSetPreviewModel(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][0], 19374);
	PlayerTextDrawSetPreviewRot(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][0], 0.0000, 0.0000, 0.0000, 1.0000);

	ptd_inventory[playerid][OTHER_SLOT_ICON][1] = CreatePlayerTextDraw(playerid, 467.9667, 166.6996, ""); // пусто
	PlayerTextDrawTextSize(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][1], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][1], 1);
	PlayerTextDrawColor(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][1], -1);
	PlayerTextDrawBackgroundColor(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][1], 190);
	PlayerTextDrawFont(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][1], 5);
	PlayerTextDrawSetProportional(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][1], 0);
	PlayerTextDrawSetShadow(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][1], 0);
	PlayerTextDrawSetSelectable(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][1], true);
	PlayerTextDrawSetPreviewModel(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][1], 19374);
	PlayerTextDrawSetPreviewRot(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][1], 0.0000, 0.0000, 0.0000, 1.0000);

	ptd_inventory[playerid][OTHER_SLOT_ICON][2] = CreatePlayerTextDraw(playerid, 467.9335, 212.5440, ""); // пусто
	PlayerTextDrawTextSize(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][2], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][2], 1);
	PlayerTextDrawColor(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][2], -1);
	PlayerTextDrawBackgroundColor(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][2], 190);
	PlayerTextDrawFont(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][2], 5);
	PlayerTextDrawSetProportional(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][2], 0);
	PlayerTextDrawSetShadow(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][2], 0);
	PlayerTextDrawSetSelectable(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][2], true);
	PlayerTextDrawSetPreviewModel(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][2], 19374);
	PlayerTextDrawSetPreviewRot(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][2], 0.0000, 0.0000, 0.0000, 1.0000);

	ptd_inventory[playerid][OTHER_SLOT_ICON][3] = CreatePlayerTextDraw(playerid, 467.9000, 258.6033, ""); // пусто
	PlayerTextDrawTextSize(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][3], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][3], 1);
	PlayerTextDrawColor(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][3], -1);
	PlayerTextDrawBackgroundColor(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][3], 190);
	PlayerTextDrawFont(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][3], 5);
	PlayerTextDrawSetProportional(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][3], 0);
	PlayerTextDrawSetShadow(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][3], 0);
	PlayerTextDrawSetSelectable(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][3], true);
	PlayerTextDrawSetPreviewModel(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][3], 19374);
	PlayerTextDrawSetPreviewRot(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][3], 0.0000, 0.0000, 0.0000, 1.0000);

	ptd_inventory[playerid][OTHER_SLOT_ICON][4] = CreatePlayerTextDraw(playerid, 509.9002, 258.6885, ""); // пусто
	PlayerTextDrawTextSize(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][4], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][4], 1);
	PlayerTextDrawColor(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][4], -1);
	PlayerTextDrawBackgroundColor(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][4], 190);
	PlayerTextDrawFont(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][4], 5);
	PlayerTextDrawSetProportional(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][4], 0);
	PlayerTextDrawSetShadow(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][4], 0);
	PlayerTextDrawSetSelectable(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][4], true);
	PlayerTextDrawSetPreviewModel(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][4], 19374);
	PlayerTextDrawSetPreviewRot(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][4], 0.0000, 0.0000, 0.0000, 1.0000);

	ptd_inventory[playerid][OTHER_SLOT_ICON][5] = CreatePlayerTextDraw(playerid, 550.9003, 258.6885, ""); // пусто
	PlayerTextDrawTextSize(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][5], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][5], 1);
	PlayerTextDrawColor(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][5], -1);
	PlayerTextDrawBackgroundColor(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][5], 190);
	PlayerTextDrawFont(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][5], 5);
	PlayerTextDrawSetProportional(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][5], 0);
	PlayerTextDrawSetShadow(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][5], 0);
	PlayerTextDrawSetSelectable(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][5], true);
	PlayerTextDrawSetPreviewModel(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][5], 19374);
	PlayerTextDrawSetPreviewRot(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][5], 0.0000, 0.0000, 0.0000, 1.0000);

	ptd_inventory[playerid][OTHER_SLOT_ICON][6] = CreatePlayerTextDraw(playerid, 592.2661, 258.8033, ""); // пусто
	PlayerTextDrawTextSize(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][6], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][6], 1);
	PlayerTextDrawColor(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][6], -1);
	PlayerTextDrawBackgroundColor(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][6], 190);
	PlayerTextDrawFont(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][6], 5);
	PlayerTextDrawSetProportional(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][6], 0);
	PlayerTextDrawSetShadow(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][6], 0);
	PlayerTextDrawSetSelectable(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][6], true);
	PlayerTextDrawSetPreviewModel(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][6], 19374);
	PlayerTextDrawSetPreviewRot(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][6], 0.0000, 0.0000, 0.0000, 1.0000);

	ptd_inventory[playerid][OTHER_SLOT_ICON][7] = CreatePlayerTextDraw(playerid, 592.2661, 212.6031, ""); // пусто
	PlayerTextDrawTextSize(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][7], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][7], 1);
	PlayerTextDrawColor(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][7], -1);
	PlayerTextDrawBackgroundColor(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][7], 190);
	PlayerTextDrawFont(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][7], 5);
	PlayerTextDrawSetProportional(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][7], 0);
	PlayerTextDrawSetShadow(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][7], 0);
	PlayerTextDrawSetSelectable(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][7], true);
	PlayerTextDrawSetPreviewModel(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][7], 19374);
	PlayerTextDrawSetPreviewRot(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][7], 0.0000, 0.0000, 0.0000, 1.0000);

	ptd_inventory[playerid][OTHER_SLOT_ICON][8] = CreatePlayerTextDraw(playerid, 592.1660, 166.9736, ""); // пусто
	PlayerTextDrawTextSize(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][8], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][8], 1);
	PlayerTextDrawColor(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][8], -1);
	PlayerTextDrawBackgroundColor(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][8], 190);
	PlayerTextDrawFont(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][8], 5);
	PlayerTextDrawSetProportional(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][8], 0);
	PlayerTextDrawSetShadow(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][8], 0);
	PlayerTextDrawSetSelectable(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][8], true);
	PlayerTextDrawSetPreviewModel(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][8], 19374);
	PlayerTextDrawSetPreviewRot(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][8], 0.0000, 0.0000, 0.0000, 1.0000);

	ptd_inventory[playerid][OTHER_SLOT_ICON][9] = CreatePlayerTextDraw(playerid, 592.1660, 121.0289, ""); // пусто
	PlayerTextDrawTextSize(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][9], 38.0000, 42.0000);
	PlayerTextDrawAlignment(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][9], 1);
	PlayerTextDrawColor(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][9], -1);
	PlayerTextDrawBackgroundColor(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][9], 190);
	PlayerTextDrawFont(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][9], 5);
	PlayerTextDrawSetProportional(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][9], 0);
	PlayerTextDrawSetShadow(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][9], 0);
	PlayerTextDrawSetSelectable(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][9], true);
	PlayerTextDrawSetPreviewModel(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][9], 19374);
	PlayerTextDrawSetPreviewRot(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][9], 0.0000, 0.0000, 0.0000, 1.0000);
	return playerid;
}

forward OnLoadInventory(playerid);
public OnLoadInventory(playerid) {
	new rows;
	cache_get_row_count(rows);
	if (rows) {
		new
			id,
			i_id,
			item_amount,
			item_slot,
			page,
			bool: is = false
		;

		for (new i; i < rows; i++) {
			cache_get_value_name_int(i, "id", id); // id = cache_get_field_content_int(i, "id", GetMySQLHandle());
			cache_get_value_name_int(i, "item_id", i_id); // item_id = cache_get_field_content_int(i, "item_id", GetMySQLHandle());
			cache_get_value_name_int(i, "item_amount", item_amount); // item_amount = cache_get_field_content_int(i, "item_amount", GetMySQLHandle());
			cache_get_value_name_int(i, "item_slot", item_slot); // item_slot = cache_get_field_content_int(i, "item_slot", GetMySQLHandle());
			cache_get_value_name_int(i, "page", page); // page = cache_get_field_content_int(i, "page", GetMySQLHandle())

			new index = FIND_IDX(item_slot, page, INVENTORY_SIZE);
			new item_id = GetInventoryData(playerid, page, index, ITEM_ID);

			if (item_id == i_id) {
				is = true;
				SetInventoryData(playerid, page, index, ITEM_AMOUNT, GetInventoryData(playerid, page, index, ITEM_AMOUNT) + item_amount);
				break;
			}

			if (!is) {
				index = FIND_IDX(item_slot, page, INVENTORY_SIZE);
				SetInventoryData(playerid, page, index, ITEM_SLOT, item_slot);
				SetInventoryData(playerid, page, index, ITEM_AMOUNT, item_amount);
				SetInventoryData(playerid, page, index, ITEM_ID, i_id);
			}
		}
	}

	return true;
}

stock ClearInventoryData(playerid) {
	for (new page; page < MAX_PAGE; page++) {
		for (new slot; slot < INVENTORY_SIZE; slot++) {
			new
				index = FIND_IDX(slot, page, INVENTORY_SIZE),
				item_id = GetInventoryData(playerid, page, index, ITEM_ID)
			;

			if (!item_id) continue;

			SetInventoryData(playerid, page, index, ITEM_ID, 0);
			SetInventoryData(playerid, page, index, ITEM_AMOUNT, 0);
			SetInventoryData(playerid, page, index, ITEM_SLOT, 0);
		}
	}

	ptd_inventory[playerid] = ptd_inventory_null;
	is_open_inventory[playerid] = false;

	return true;
}

public OnGameModeInit() {
	Inventory_CreateGlobalTextDraw();
	#if defined Inv_OnGameModeInit
		return Inv_OnGameModeInit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif

#define OnGameModeInit Inv_OnGameModeInit
#if defined Inv_OnGameModeInit
	forward Inv_OnGameModeInit();
#endif

public OnPlayerDisconnect(playerid, reason) {
	// SaveInventoryData(playerid);
	// ClearInventoryData(playerid);
	#if defined Inv_OnPlayerDisconnect
        return Inv_OnPlayerDisconnect(playerid, reason);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif

#define OnPlayerDisconnect Inv_OnPlayerDisconnect
#if defined Inv_OnPlayerDisconnect
    forward Inv_OnPlayerDisconnect(playerid, reason);
#endif


public OnPlayerClickPlayerTextDraw(playerid, PlayerText: playertextid) {
	if (IsOpenInventory(playerid)) {
		new clickedslot = GetClickedSlot(playerid);

 		for (new i; i < INVENTORY_SIZE; i++) {
			if (playertextid == ptd_inventory[playerid][SLOT_ICON][i]) {
				new
					page = GetOpenInventoryPage(playerid),
					slot_id = FIND_IDX(i, page, INVENTORY_SIZE),
					item_id = GetInventoryData(playerid, page, slot_id, ITEM_ID)
				;

				if (clickedslot == INVALID_CLICK_SLOT && !item_id) {
					break;
				}

				if (clickedslot == INVALID_CLICK_SLOT) {
					for (new sl; sl < INVENTORY_SIZE; sl++) {
						PlayerTextDrawHide(playerid, ptd_inventory[playerid][SLOT_ICON][sl]);
						PlayerTextDrawSetSelectable(playerid, ptd_inventory[playerid][SLOT_ICON][sl], false);
						PlayerTextDrawBackgroundColor(playerid, ptd_inventory[playerid][SLOT_ICON][sl], 75);
						PlayerTextDrawShow(playerid, ptd_inventory[playerid][SLOT_ICON][sl]);
					}

					PlayerClickSlot(playerid, i);
					SetClickedSlot(playerid, i);
					return true;
				}

				if (clickedslot != INVALID_CLICK_SLOT && clickedslot != i) {
					new clickedslot_id = FIND_IDX(clickedslot, page, INVENTORY_SIZE);

					new 
						save_amount = GetInventoryData(playerid, page, clickedslot_id, ITEM_AMOUNT),
						save_id = GetInventoryData(playerid, page, clickedslot_id, ITEM_ID),

						old_amount = GetInventoryData(playerid, page, slot_id, ITEM_AMOUNT),
						old_slot = GetInventoryData(playerid, page, slot_id, ITEM_SLOT),
						old_id = GetInventoryData(playerid, page, slot_id, ITEM_ID)
					;

					SetInventoryData(playerid, page, slot_id, ITEM_AMOUNT, save_amount);
					SetInventoryData(playerid, page, slot_id, ITEM_SLOT, i);
					SetInventoryData(playerid, page, slot_id, ITEM_ID, save_id);

					SetInventoryData(playerid, page, clickedslot_id, ITEM_AMOUNT, old_amount);
					SetInventoryData(playerid, page, clickedslot_id, ITEM_ID, old_id);
					SetInventoryData(playerid, page, clickedslot_id, ITEM_SLOT, old_slot);

					ReloadSlot(playerid, i, page);
					ReloadSlot(playerid, clickedslot, page); 

					clicked_slot[playerid] = INVALID_CLICK_SLOT;
				}
			}
		}

		for (new items; items < MAX_ITEMS_SECONDARY; items++) {
			if (playertextid == ptd_inventory[playerid][OTHER_SLOT_ICON][items]) {
				new
					page = GetOpenInventoryPage(playerid),
					item_id = secondary_slots_data[playerid][items][ITEM_ID],
					item_amount = secondary_slots_data[playerid][items][ITEM_AMOUNT],
					item_type = GetItemData(item_id, ITEM_TYPE),
					gun_id = GetItemData(item_id, ITEM_GUN_ID)
				;

				if (!item_id) {
					break;
				}

				switch(item_type) {
					case ITEM_TYPE_GUN: {
						new available_slot = GetAvailableInventorySlot(playerid, page);
						
						if (available_slot == -1)
							return SendClientMessage(playerid, -1, "Освободите пространство в инвентаре этой страницы.");

						new bool: is = false;

						for (new i; i < INVENTORY_SIZE; i++) {
							new index = FIND_IDX(i, page, INVENTORY_SIZE);
							new id = GetInventoryData(playerid, page, index, ITEM_ID);

							if (item_id == id) {
								is = true;
								SetInventoryData(playerid, page, index, ITEM_AMOUNT, GetInventoryData(playerid, page, index, ITEM_AMOUNT) + 1);
								UpdateItemAmountInDatabase(playerid, id, GetInventoryData(playerid, page, index, ITEM_AMOUNT), page);
								break;
							}
						}

						if (!is) {
							new index = FIND_IDX(available_slot, page, INVENTORY_SIZE);
							SetInventoryData(playerid, page, index, ITEM_ID, item_id);
							SetInventoryData(playerid, page, index, ITEM_AMOUNT, item_amount);
							SetInventoryData(playerid, page, index, ITEM_SLOT, available_slot);
							AddItemInDatabase(playerid, item_id, item_amount, available_slot, page);
						}

						secondary_slots_data[playerid][items][ITEM_ID] = 0;
						secondary_slots_data[playerid][items][ITEM_AMOUNT] = 0;
						secondary_slots_data[playerid][items][ITEM_SLOT] = 0;
						secondary_slots_data[playerid][items][ITEM_PAGE] = 0;

						FillInventoryItems(playerid, page);
						FillSecondaryItems(playerid);

						RemovePlayerWeapon(playerid, gun_id);
					}
				}
			}
		}

		if (playertextid == td_buttons[playerid][8]) {
			if (GetClickedSlot(playerid) == INVALID_CLICK_SLOT)
				return SendClientMessage(playerid, -1, "Предмет не выбран.");
			
			new 
				page = GetOpenInventoryPage(playerid),
				slot = GetClickedSlot(playerid),
				slot_id = FIND_IDX(slot, page, INVENTORY_SIZE),
				item_id = GetInventoryData(playerid, page, slot_id, ITEM_ID),
				item_amount = GetInventoryData(playerid, page, slot_id, ITEM_AMOUNT),
				item_info_size[sizeof item_info + (-2+MAX_NAME_ITEM_LENGHT) + (-2+2) + (-2+MAX_ITEM_DESC_LENGHT)],
				item_name[MAX_NAME_ITEM_LENGHT],
				item_desc[MAX_ITEM_DESC_LENGHT]
			;

			format(item_name, sizeof item_name, GetItemData(item_id, ITEM_NAME));
			format(item_desc, sizeof item_desc, GetItemData(item_id, ITEM_DESC));
			format(item_info_size, sizeof item_info_size, item_info, item_name, item_amount, item_desc);

			Dialog_Open(playerid, Dialog:DIALOG_NONE, DIALOG_STYLE_MSGBOX, "{FFFFFF}Информация о предмете.", item_info_size, "Закрыть", "");
		}

		if (playertextid == td_buttons[playerid][10]) {
			for (new i; i < INVENTORY_SIZE; i++) {
				PlayerTextDrawHide(playerid, ptd_inventory[playerid][SLOT_ICON][i]);
				PlayerTextDrawSetSelectable(playerid, ptd_inventory[playerid][SLOT_ICON][i], true);
				PlayerTextDrawBackgroundColor(playerid, ptd_inventory[playerid][SLOT_ICON][i], 190);
				PlayerTextDrawShow(playerid, ptd_inventory[playerid][SLOT_ICON][i]);
			}

			for (new i; i < sizeof td_buttons[]; i++) {
				PlayerTextDrawDestroy(playerid, td_buttons[playerid][i]);
			}
		}

		if (playertextid == td_buttons[playerid][6]) {
			if (GetClickedSlot(playerid) == INVALID_CLICK_SLOT)
				return SendClientMessage(playerid, -1, "Предмет не выбран."); 

			new 
				page = GetOpenInventoryPage(playerid),
				slot = GetClickedSlot(playerid),
				slot_id = FIND_IDX(slot, page, INVENTORY_SIZE),
				item_id = GetInventoryData(playerid, page, slot_id, ITEM_ID),
				item_type = GetItemData(item_id, ITEM_TYPE)
			;

			Inventory_UseItem(playerid, item_type, slot_id);
		}

		if (playertextid == td_buttons[playerid][9]) {
			if (GetClickedSlot(playerid) == INVALID_CLICK_SLOT)
				return SendClientMessage(playerid, -1, "Предмет не выбран.");

			for (new i; i < INVENTORY_SIZE; i++) {
				PlayerTextDrawHide(playerid, ptd_inventory[playerid][SLOT_ICON][i]);
				PlayerTextDrawSetSelectable(playerid, ptd_inventory[playerid][SLOT_ICON][i], true);
				PlayerTextDrawBackgroundColor(playerid, ptd_inventory[playerid][SLOT_ICON][i], 190);
				PlayerTextDrawShow(playerid, ptd_inventory[playerid][SLOT_ICON][i]);
			}

			Dialog_Show(playerid, Dialog:DLG_DROP_ITEM_AMOUNT);
		}

		if (playertextid == td_buttons[playerid][7]) {
			new 
				page = GetOpenInventoryPage(playerid),
				slot = GetClickedSlot(playerid),
				slot_id = FIND_IDX(slot, page, INVENTORY_SIZE),
				item_id = GetInventoryData(playerid, page, slot_id, ITEM_ID)
			;

			if (!item_id) return true;

			Dialog_Show(playerid, Dialog:DLG_TRANSMIT_ITEM);
		}
	}
	
	#if defined Inv_OnPlayerClickPlayerTextDraw
        return Inv_OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerClickPlayerTextDraw
    #undef OnPlayerClickPlayerTextDraw
#else
    #define _ALS_OnPlayerClickPlayerTextDraw
#endif

#define OnPlayerClickPlayerTextDraw Inv_OnPlayerClickPlayerTextDraw
#if defined Inv_OnPlayerClickPlayerTextDraw
    forward Inv_OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid);
#endif

public OnPlayerClickTextDraw(playerid, Text:clickedid) {
	if (IsOpenInventory(playerid)) {
		if (clickedid == Text: INVALID_TEXT_DRAW) {
			SetInventoryPage(playerid);
			SetClickedSlot(playerid);
			is_open_inventory[playerid] = false;
			HideInventoryTextDraws(playerid);
		}	
	
		if (clickedid == Text: gtd_inventory[TEXTDRAW_TEXT_CLOSE]) {
			HideInventoryTextDraws(playerid);
		}

		if (clickedid == Text: gtd_inventory[TEXTDRAW_TEXT_PAGE_ONE]) {
			if (GetClickedSlot(playerid) != INVALID_CLICK_SLOT) {
				return SendClientMessage(playerid, -1, "Вы не можете переместиться на другую страницу, пока у Вас выбран предмет.");
			}
			
			SetInventoryPage(playerid, 0);
		}

		if (clickedid == Text: gtd_inventory[TEXTDRAW_TEXT_PAGE_TWO]) {
			if (GetClickedSlot(playerid) != INVALID_CLICK_SLOT) {
				return SendClientMessage(playerid, -1, "Вы не можете переместиться на другую страницу, пока у Вас выбран предмет.");
			}

			SetInventoryPage(playerid, 1);
		}

		if (clickedid == Text: gtd_inventory[TEXTDRAW_TEXT_PAGE_THREE]) {
			if (GetClickedSlot(playerid) != INVALID_CLICK_SLOT) {
				return SendClientMessage(playerid, -1, "Вы не можете переместиться на другую страницу, пока у Вас выбран предмет.");
			}

			SetInventoryPage(playerid, 2);
		}
	}

	#if defined Inv_OnPlayerClickTextDraw
        return Inv_OnPlayerClickTextDraw(playerid, Text:clickedid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerClickTextDraw
    #undef OnPlayerClickTextDraw
#else
    #define _ALS_OnPlayerClickTextDraw
#endif

#define OnPlayerClickTextDraw Inv_OnPlayerClickTextDraw
#if defined Inv_OnPlayerClickTextDraw
    forward Inv_OnPlayerClickTextDraw(playerid, Text:clickedid);
#endif


public OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
	if ((newkeys & KEY_YES) && (newkeys & KEY_WALK)) {
		if (IsOpenInventory(playerid)) return SendClientMessage(playerid, -1, "Инвентарь уже открыт.");
		is_open_inventory[playerid] = true;

		for (new i; i < sizeof gtd_inventory; i++) {
			TextDrawShowForPlayer(playerid, gtd_inventory[i]);
		}

		for (new i; i < 10; i++) {
			PlayerTextDrawShow(playerid, ptd_inventory[playerid][OTHER_SLOT_ICON][i]);
		}

		PlayerTextDrawShow(playerid, ptd_inventory[playerid][PLAYER_SKIN]);
		FillInventoryItems(playerid);
	}

	if (newkeys & KEY_WALK) {
		Inventory_PickupWorldItem(playerid, newkeys);
	}

	#if defined Inv_OnPlayerKeyStateChange
        return Inv_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerKeyStateChange
    #undef OnPlayerKeyStateChange
#else
    #define _ALS_OnPlayerKeyStateChange
#endif

#define OnPlayerKeyStateChange Inv_OnPlayerKeyStateChange
#if defined Inv_OnPlayerKeyStateChange
    forward Inv_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
#endif

/*
	Dialog Content
*/
DialogCreate:DLG_DROP_ITEM_AMOUNT(playerid) {
	return Dialog_Open(playerid, Dialog:DLG_DROP_ITEM_AMOUNT, 
		DIALOG_STYLE_INPUT, "Выбросить предмет.", 
		"\
			{ffffff}Ввведите количество предмета, которое хотите выбросить. \n\
		", "Выбросить", "Закрыть"
	);
}
DialogCreate:DLG_TRANSMIT_ITEM(playerid) {
	return Dialog_Open(playerid, Dialog:DLG_TRANSMIT_ITEM, 
		DIALOG_STYLE_INPUT, "Передать предмет.", 
		"\
			{ffffff}Ввведите ID игрока, и количество предмета через запятую (Пример: 3, 5) \n\
		", "Отправить", "Закрыть"
	);
}
DialogCreate:DLG_TRANSMIT_CONFIRM(playerid) {
	new item_id = transmit_data[playerid][0];
	new item_amount = transmit_data[playerid][1];
	new item_name[MAX_NAME_ITEM_LENGHT];
	format(item_name, sizeof item_name, GetItemData(item_id, ITEM_NAME));
	new const string[] = 
		"\
			{ffffff}Вам предлагают передать предмет {ffe599}%s. \n\
			{ffffff}В количестве: {8fce00}%d. \n\
			\n\
			{ffffff}Нажмите {8fce00}\"Да\" {ffffff}чтобы забрать предмет, или {f44336}\"Нет\", {ffffff}чтобы отказаться от предложения игрока. \n\
		"
	;
	new string_size[sizeof string + (-2+MAX_NAME_ITEM_LENGHT) + (-2+2)];
	format(string_size, sizeof string_size, string, item_name, item_amount);

	return Dialog_Open(playerid, Dialog:DLG_TRANSMIT_CONFIRM, DIALOG_STYLE_MSGBOX, "Передача предмета.", string_size, "{8fce00}Да", "{f44336}Нет");
}

/*
	Dialog Response
*/
DialogResponse:DLG_TRANSMIT_CONFIRM(playerid, response, listitem, inputtext[]) {
	new player = transmit_data[playerid][2];
	new page = transmit_data[playerid][4];
	new index = transmit_data[playerid][3];
	
	if (!response) {
		SendClientMessage(player, -1, "Игрок отказался от вашего предложения.");
		transmit_data[playerid][0] = 0;
		transmit_data[playerid][1] = INVALID_PLAYER_ID;
		transmit_data[playerid][2] = INVALID_PLAYER_ID;
		transmit_data[playerid][3] = 0;
		transmit_data[playerid][4] = INVALID_INVENTORY_PAGE;
		return true;
	}

	SetInventoryData(player, page, index, ITEM_AMOUNT, GetInventoryData(player, page, index, ITEM_AMOUNT) - 1);
	if (GetInventoryData(player, page, index, ITEM_AMOUNT) <= 0) {
		SetInventoryData(player, page, index, ITEM_ID, 0);
		SetInventoryData(player, page, index, ITEM_AMOUNT, 0);
		SetInventoryData(player, page, index, ITEM_SLOT, 0);
		FillInventoryItems(player, page);
	}

	new free_slot = GetAvailableInventorySlot(playerid, page);
	if (free_slot == -1)
		return SendClientMessage(playerid, -1, "На этой странице не осталась свободного пространства.");

	AddInventoryItem(playerid, transmit_data[playerid][0], page, transmit_data[playerid][1], free_slot);
	transmit_data[playerid][0] = 0;
	transmit_data[playerid][1] = INVALID_PLAYER_ID;
	transmit_data[playerid][2] = INVALID_PLAYER_ID;
	transmit_data[playerid][3] = 0;
	transmit_data[playerid][4] = INVALID_INVENTORY_PAGE;

	return true;
}

DialogResponse:DLG_TRANSMIT_ITEM(playerid, response, listitem, inputtext[]) {
	if (!response) {
		SetClickedSlot(playerid);
		for (new i; i < INVENTORY_SIZE; i++) {
			PlayerTextDrawHide(playerid, ptd_inventory[playerid][SLOT_ICON][i]);
			PlayerTextDrawSetSelectable(playerid, ptd_inventory[playerid][SLOT_ICON][i], true);
			PlayerTextDrawBackgroundColor(playerid, ptd_inventory[playerid][SLOT_ICON][i], 190);
			PlayerTextDrawShow(playerid, ptd_inventory[playerid][SLOT_ICON][i]);
		}

		for (new i; i < sizeof td_buttons[]; i++) {
			PlayerTextDrawHide(playerid, td_buttons[playerid][i]);
		}

		return true;
	}

	new 
		target_dialog = INVALID_PLAYER_ID, 
		amount_dialog,
		page = GetOpenInventoryPage(playerid),
		slot = GetClickedSlot(playerid),
		index = FIND_IDX(slot, page, INVENTORY_SIZE),
		item_id = GetInventoryData(playerid, page, index, ITEM_ID),
		item_amount = GetInventoryData(playerid, page, index, ITEM_AMOUNT)
	;

	if (!item_id) return true;

	if (sscanf(inputtext, "p<,>dd", target_dialog, amount_dialog)) {
		return Dialog_Show(playerid, Dialog:DLG_TRANSMIT_ITEM);
	}

	if (!IsPlayerConnected(target_dialog))
		return Dialog_Show(playerid, Dialog:DLG_TRANSMIT_ITEM);

	if (target_dialog == playerid)
		return Dialog_Show(playerid, Dialog:DLG_TRANSMIT_ITEM);

	if (Inventory_ProxDetector(1.0, playerid, target_dialog))
		return SendClientMessage(playerid, -1, "Этот игрок далеко от Вас.");

	if (amount_dialog > item_amount) {
		return SendClientMessage(playerid, -1, "У Вас нет такого количества предмета.");
	}

	transmit_data[target_dialog][0] = item_id;
	transmit_data[target_dialog][1] = amount_dialog;
	transmit_data[target_dialog][2] = playerid;
	transmit_data[target_dialog][3] = index;
	transmit_data[target_dialog][4] = page;

	return Dialog_Show(target_dialog, Dialog:DLG_TRANSMIT_CONFIRM);
}

DialogResponse:DLG_DROP_ITEM_AMOUNT(playerid, response, listitem, inputtext[]) {
	if (!response) {
		SetClickedSlot(playerid);
		for (new i; i < INVENTORY_SIZE; i++) {
			PlayerTextDrawHide(playerid, ptd_inventory[playerid][SLOT_ICON][i]);
			PlayerTextDrawSetSelectable(playerid, ptd_inventory[playerid][SLOT_ICON][i], true);
			PlayerTextDrawBackgroundColor(playerid, ptd_inventory[playerid][SLOT_ICON][i], 190);
			PlayerTextDrawShow(playerid, ptd_inventory[playerid][SLOT_ICON][i]);
		}

		for (new i; i < sizeof td_buttons[]; i++) {
			PlayerTextDrawHide(playerid, td_buttons[playerid][i]);
		}
		return true;
	}

	new
		slot = GetClickedSlot(playerid),
		page = GetOpenInventoryPage(playerid),
		index = FIND_IDX(slot, page, INVENTORY_SIZE),
		item_id = GetInventoryData(playerid, page, index, ITEM_ID),
		item_amount = GetInventoryData(playerid, page, index, ITEM_AMOUNT),
		amount
	;

	if (sscanf(inputtext, "d", amount))
		return Dialog_Show(playerid, Dialog:DLG_DROP_ITEM_AMOUNT);

	if (amount < 1 || amount > MAX_ITEM_AMOUNT) {
		SendClientMessage(playerid, -1, "Укажите валидное количество предмета. 1-50");
		return Dialog_Show(playerid, Dialog:DLG_DROP_ITEM_AMOUNT);
	}

	if (amount > item_amount)
		return SendClientMessage(playerid, -1, "Недостаточное количество предмета, чтобы выбросить.");

	Inventory_WorldDropItem(playerid, index, amount, item_id, page);
	SetClickedSlot(playerid);
	return true;
}

forward Inventory_ProxDetector(Float:radius, playerid, targetid);
public Inventory_ProxDetector(Float:radius, playerid, targetid)
{
    if(IsPlayerConnected(playerid) && IsPlayerConnected(targetid))
    {
        new Float:posx, Float:posy, Float:posz;
        new Float:oldposx, Float:oldposy, Float:oldposz;
        new Float:tempposx, Float:tempposy, Float:tempposz;
        GetPlayerPos(playerid, oldposx, oldposy, oldposz);
        GetPlayerPos(targetid, posx, posy, posz);
        tempposx = (oldposx -posx);
        tempposy = (oldposy -posy);
        tempposz = (oldposz -posz);
        if (((tempposx < radius) && (tempposx > -radius)) && ((tempposy < radius) && (tempposy > -radius)) && ((tempposz < radius) && (tempposz > -radius)))
        {
            return 1;
        }
    }
    return 0;
}
