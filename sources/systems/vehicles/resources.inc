/*
    Macroses
*/
#define MAX_PLAYER_VEHICLES     (100)
#define PERCENT_SALE_VEHICLE    (30)

/*
    Structure
*/
enum e_VEHICLE_INFO {
    eVehicle_ID,
    eVehicle_AccountID,
    eVehicle_Identifier,
    eVehicle_Model,
    eVehicle_Color1,
    eVehicle_Color2,
    eVehicle_Price,
    eVehicle_DynamicZone,
    bool: eVehicle_Loaded,
    bool: eVehicle_Locked,
    Float: eVehicle_X,
    Float: eVehicle_Y,
    Float: eVehicle_Z,
    Float: eVehicle_A
};

enum e_ADDED_CARS {
	eModelID,    		// ID MODEL
	eModel_NAME[64]    	// NAME.
};

/*
    Variables
*/
new VehicleInfo[MAX_PLAYERS][MAX_PLAYER_VEHICLES][e_VEHICLE_INFO];
new VehicleInfo_NULL[e_VEHICLE_INFO] = {
    0, 
    -1, 
    0, 
    0, 
    0, 
    0, 
    0, 
    -1, 
    false, 
    false, 
    0.0, 
    0.0, 
    0.0, 
    0.0
};
new g_player_veh_page[MAX_PLAYERS];
new g_player_veh_idx[MAX_PLAYERS];
new Iterator: IterPlayerVehicles[MAX_PLAYERS]<MAX_PLAYER_VEHICLES>;
new eAddedCars[11][e_ADDED_CARS] = {
	{ 12500, "BMW E30" }, // 0
	{ 12501, "BMW M3 E46" }, // 1
    { 12502, "BMW M3 Competition" }, // 2
    { 12503, "BMW X6M" }, // 3
    { 12504, "Dodge Charger SRT" }, // 4
    { 12505, "Ford Charger Jeep" }, // 5
    { 12506, "Jeep Charger SRT" }, // 6
    { 12507, "Mercedes-Benz CLS" }, // 7
    { 12508, "Mercedes-Benz G63 Brabus" }, // 8
    { 12509, "Mercedes-Benz G63" }, // 9
    { 12510, "Porsche 911" } // 10
};
new VehicleNames[][] = {
    "Landstalker", "Bravura", "Buffalo", "Linerunner", "Perrenial", "Sentinel",
    "Dumper", "Firetruck", "Trashmaster", "Stretch", "Manana", "Infernus",
    "Voodoo", "Pony", "Mule", "Cheetah", "Ambulance", "Leviathan", "Moonbeam",
    "Esperanto", "Taxi", "Washington", "Bobcat", "Whoopee", "BF Injection",
    "Hunter", "Premier", "Enforcer", "Securicar", "Banshee", "Predator", "Bus",
    "Rhino", "Barracks", "Hotknife", "Trailer", "Previon", "Coach", "Cabbie",
    "Stallion", "Rumpo", "RC Bandit", "Romero", "Packer", "Monster", "Admiral",
    "Squalo", "Seasparrow", "Pizzaboy", "Tram", "Trailer", "Turismo", "Speeder",
    "Reefer", "Tropic", "Flatbed", "Yankee", "Caddy", "Solair", "Berkley's RC Van",
    "Skimmer", "PCJ-600", "Faggio", "Freeway", "RC Baron", "RC Raider", "Glendale",
    "Oceanic","Sanchez", "Sparrow", "Patriot", "Quad", "Coastguard", "Dinghy",
    "Hermes", "Sabre", "Rustler", "ZR-350", "Walton", "Regina", "Comet", "BMX",
    "Burrito", "Camper", "Marquis", "Baggage", "Dozer", "Maverick", "News Chopper",
    "Rancher", "FBI Rancher", "Virgo", "Greenwood", "Jetmax", "Hotring", "Sandking",
    "Blista Compact", "Police Maverick", "Boxville", "Benson", "Mesa", "RC Goblin",
    "Hotring Racer A", "Hotring Racer B", "Bloodring Banger", "Rancher", "Super GT",
    "Elegant", "Journey", "Bike", "Mountain Bike", "Beagle", "Cropduster", "Stunt",
    "Tanker", "Roadtrain", "Nebula", "Majestic", "Buccaneer", "Shamal", "Hydra",
    "FCR-900", "NRG-500", "HPV1000", "Cement Truck", "Tow Truck", "Fortune",
    "Cadrona", "FBI Truck", "Willard", "Forklift", "Tractor", "Combine", "Feltzer",
    "Remington", "Slamvan", "Blade", "Freight", "Streak", "Vortex", "Vincent",
    "Bullet", "Clover", "Sadler", "Firetruck", "Hustler", "Intruder", "Primo",
    "Cargobob", "Tampa", "Sunrise", "Merit", "Utility", "Nevada", "Yosemite",
    "Windsor", "Monster", "Monster", "Uranus", "Jester", "Sultan", "Stratium",
    "Elegy", "Raindance", "RC Tiger", "Flash", "Tahoma", "Savanna", "Bandito",
    "Freight Flat", "Streak Carriage", "Kart", "Mower", "Dune", "Sweeper",
    "Broadway", "Tornado", "AT-400", "DFT-30", "Huntley", "Stafford", "BF-400",
    "News Van", "Tug", "Trailer", "Emperor", "Wayfarer", "Euros", "Hotdog", "Club",
    "Freight Box", "Trailer", "Andromada", "Dodo", "RC Cam", "Launch", "Police Car",
    "Police Car", "Police Car", "Police Ranger", "Picador", "S.W.A.T", "Alpha",
    "Phoenix", "Glendale", "Sadler", "Luggage", "Luggage", "Stairs", "Boxville",
    "Tiller", "Utility Trailer"
};


