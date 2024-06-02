/*
    Macroses
*/
#define MAX_HOUSES                      (500)
#define MIN_HOUSE_PRICE                 (25000)
#define MAX_HOUSE_PRICE                 (750000)
#define MAX_INTERIORS                   (7)
#define MAX_INTERIORS_LIST_PAGE         (20)
#define PERCENT_SALE_HOUSE_DEDUCTION    (0.3)
#define PERCENT_SALE_HOUSE              (30)
#define INVALID_ARRAY_VALUE             (-1)
#define GetHouseData(%0,%1)             house_data[%0][%1]
#define SetHouseData(%0,%1,%2)          house_data[%0][%1] = %2
#define GetInteriorData(%0,%1)          interiors_data[%0][%1]

/*
    Constants
*/
new const house_description[] =
    "\
        "cWHITE"Номер дома - "cBLUE"#%d. \n\
        "cWHITE"Государственная стоимость - "cBLUE"%d $ \n\
        "cWHITE"Двери - "cBLUE"%s \n\
        "cBLUE"%s \n\
    "
;

/*
    Enums
*/
enum e_HOUSE_INFO {
    h_id,
    h_acc_id,
    h_cost,
    h_enter_pickup,
    Text3D: h_3dTEXT,
    h_exit_pickup,
    Float: h_enter_x,
    Float: h_enter_y,
    Float: h_enter_z,
    Float: h_exit_x,
    Float: h_exit_y,
    Float: h_exit_z,
    bool: h_entry,
    h_interior_idx
};

enum e_INTERIORS_INFO {
    interior,
    virtual_world,
    Float: pos_x,
    Float: pos_y,
    Float: pos_z,
    Float: pos_exit_x,
    Float: pos_exit_y,
    Float: pos_exit_z
};

/*
    Variables
*/
new house_data[MAX_HOUSES][e_HOUSE_INFO];
new g_player_page_interiors[MAX_PLAYERS];
new interiors_data[MAX_INTERIORS][e_INTERIORS_INFO] = {
    { 0, 100, 369.0740, 1803.8835, 542.3391, 369.0769, 1805.8754, 542.3391 },
    { 0, 150, 301.3104, 1779.1508, 537.7065, 304.1251, 1780.3705, 537.7065 },
    { 0, 200, 653.8707, 2089.0718, 536.9958, 653.9099, 2087.0332, 536.9958 },
    { 0, 250, 910.4214, 1947.5809, 537.6104, 910.4706, 1945.5288, 537.6104 },
    { 0, 300, 1009.6191, 2048.9692, 535.5986, 1007.3119, 2049.1509, 535.5986 },
    { 0, 350, 1232.6803, 1888.6193, 535.5975, 1232.7075, 1886.3788, 535.5975 },
    { 0, 400, 1079.7793, 1760.9023, 535.0430, 1079.7931, 1762.8513, 535.0430 }
};
new Iterator: IterHouses<MAX_HOUSES>;
new Iterator: IterInteriors<MAX_INTERIORS>;
new temp_array[MAX_PLAYERS][3] = {INVALID_ARRAY_VALUE, ...};