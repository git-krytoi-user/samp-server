/*

    @---------------------
    @ This module file is designed for,
    @ handling incoming database transactions
    @ 
    @ Developed by the developer: dev.r1nex
    @----------------------

*/

#include <YSI/YSI_Coding/y_hooks>

/*
    Variables
*/
new MySQL: dbHandle;

/*
    Macroses
*/
#define MYSQL_TASK:%0(%1) MYSQL_TASK__%0(%1) 

/*
    Hooks
*/
hook OnGameModeInit() {
    MYSQL_TASK:Server_Database();

    mysql_pquery(dbHandle, "CREATE TABLE IF NOT EXISTS `users` (\n\
        `id` INT(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,\n\
        `name` VARCHAR(24) NOT NULL,\n\
        `password` VARCHAR(32) NOT NULL,\n\
        `skin` INT(11) NOT NULL,\n\
        `money` INT(11) NOT NULL,\n\
        `gender` INT(1) NOT NULL,\n\
        `exp` INT(11) NOT NULL,\n\
        `score` INT(4) NOT NULL,\n\
        `satiety` FLOAT NOT NULL,\n\
        `has_passport` INT(1) NOT NULL DEFAULT 0,\n\
        `pame` VARCHAR(128) DEFAULT NULL,\n\
        `idx_nationality` INT(2) DEFAULT 0,\n\
        `referal` VARCHAR(24) DEFAULT NULL,\n\
        `reg_date` VARCHAR(20) DEFAULT NULL,\n\
        `reg_ip` VARCHAR(39) DEFAULT NULL,\n\
        `mute_time` BIGINT(20) DEFAULT NULL)"
    );

    mysql_pquery(dbHandle, "CREATE TABLE IF NOT EXISTS `admins` (\n\
        `id` INT(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,\n\
        `account_id` INT(11) NOT NULL,\n\
        `level` INT(1) NOT NULL,\n\
        `warns` INT(1) NOT NULL,\n\
        `code` VARCHAR(32) NOT NULL)"
    );

    mysql_pquery(dbHandle, "CREATE TABLE IF NOT EXISTS `houses` (\n\
        `id` INT(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,\n\
        `account_id` INT(11) DEFAULT -1,\n\
        `cost` INT(11) NOT NULL,\n\
        `enter_x` FLOAT NOT NULL,\n\
        `enter_y` FLOAT NOT NULL,\n\
        `enter_z` FLOAT NOT NULL,\n\
        `exit_x` FLOAT NOT NULL,\n\
        `exit_y` FLOAT NOT NULL,\n\
        `exit_z` FLOAT NOT NULL,\n\
        `entry` INT(1) NOT NULL,\n\
        `interior_idx` INT(11) NOT NULL)"
    );

    mysql_pquery(dbHandle, "CREATE TABLE IF NOT EXISTS `vehicles` (\n\
        `id` INT(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,\n\
        `account_id` INT(11) NOT NULL,\n\
        `model` INT(3) NOT NULL,\n\
        `color1` INT(11) NOT NULL,\n\
        `color2` INT(11) NOT NULL,\n\
        `is_loaded` INT(1) NOT NULL,\n\
        `price` INT(11) NOT NULL,\n\
        `locked` INT(11) NOT NULL,\n\
        `car_x` FLOAT NOT NULL,\n\
        `car_y` FLOAT NOT NULL,\n\
        `car_z` FLOAT NOT NULL,\n\
        `car_a` FLOAT NOT NULL)"
    );
    return 1;
}

/*
    Stocks
*/
stock MYSQL_TASK:Server_Database() {
    dbHandle = mysql_connect("127.0.0.1", "root", "", "samp");
    switch(mysql_errno()) {
        case 0: print(!"> Database connected success.");
        default: printf(!"> Database not connected. Error code - %d", mysql_errno());
    }

    return 1;
}
