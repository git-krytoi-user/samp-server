/*

    @---------------------
    @ This modular file is for customization of the mod.
    @ Developed by the developer: dev.r1nex
    @----------------------

*/

#define public:%0(%1)                  forward %0(%1); public %0(%1)
#define RandomEx(%0,%1) 			    random(%1-%0)+%0

// -- Colors
#define cWHITE                              "{ffffff}"
#define cBLUE                               "{3d85c6}"
#define cGREEN                              "{8fce00}"
#define cGREEN_BRIGHT                       "{aaf00b}"
#define cRED                                "{f44336}"
#define cYELLOW                             "{ffe7a1}"
#define cYELLOW_BRIGHT                      "{ffd966}"
#define cGREY                               "{b7b4b4}"
#define cGREY_BRIGHT                        "{666666}"
#define cPINK                               0xDD90FFFF
// -- Messages
#define PROJECT_NAME                        ""cBLUE"Project RP | "cWHITE""
#define cERROR                              "{f44336}[������] {ffffff}" // old
#define cSUCCESS                            "{8fce00}[������] {ffffff}" // old
#define cPUSH                               "{ffd966}[����������] {ffffff}" // old
#define aTAG                                ""cGREEN"[A] "cGREEN""
#define MESSAGE_IS_RENT_BIKE                ""cGREY"�� ��� ���������� ���������. ����������� /unrentbike, ����� ���������� ������."
#define MESSAGE_NO_RENT_BIKE                ""cGREY"�� ��� �� ���������� ���������."
#define MESSAGE_IS_LONG_RADIUS_RENT         ""cGREY"�� ������� ������ �� ����� ������."
#define MESSAGE_UNRENT_BIKE                 ""cGREY"�� ���������� �� ������."
#define MESSAGE_HAVE_CARS                   ""cGREY"� ��� ��� ������� ����������."
#define MESSAGE_NO_ENOUGH_MONEY             ""cGREY"� ��� ������������ �������."
#define MESSAGE_NO_PERMS                    ""cGREY"� ��� ������������ ����, ����� ������������ ��� �������."
#define MESSAGE_NO_AUTH                     ""cGREY"�� ��� �� �������������� �� �������."
#define MESSAGE_NO_ADMIN_AUTH               ""cGREY"�� ��� �� �������������� ��������������� �� �������."
#define MESSAGE_IS_ADMIN_AUTH               ""cGREY"�� ��� �������������� ��������������� �� �������."
#define MESSAGE_IS_NO_HOUSE                 ""cGREY"�� ������ ���������� � ��������� ����."
#define MESSAGE_HOUSE_ENTRY                 ""cGREY"�� �� ������ ����� � ���, ������-��� �� ������."
#define MESSAGE_LEVEL_UP                    ""cYELLOW"�� �������� ���� �������."
#define MESSAGE_DELETE_PAME                 ""cGREY"�� ������� �������� ������ ���������."
#define COMMAND_USAGE_FORMAT                ""cGREY"�������������: "
// -- Server technical side
#define HOSTNAME                            "|      PROJECT-RP #1      | Server in development"
#define GAMEMODE_NAME                       "PROJECT-GM v1.0"
#define MAX_DIALOG_ITEMS_PAGE               (20)
#define INVALID_TIMER_ID	                (0)
#define REFERAL_AWARD                       (3000)
