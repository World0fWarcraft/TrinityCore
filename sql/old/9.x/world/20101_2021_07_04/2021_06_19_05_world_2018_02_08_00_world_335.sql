-- Remove unique items from reference table 24060, as well as removing Agile Boots (4788) which is sold only and does not drop
DELETE FROM `reference_loot_template` WHERE `Entry`=24060 AND `Item` IN (1218, 1220, 1296, 1300, 1448, 1460, 1934, 2021, 2166, 2167, 2168, 3018, 3227, 4788, 4949, 5182, 5183);

-- Add former items from reference table 24060 to their proper NPCs, as well as removing 4788 which is sold only and does not drop
DELETE FROM `creature_loot_template` WHERE `Item` IN (1218, 1220, 1296, 1300, 1448, 1460, 1934, 2021, 2166, 2167, 2168, 3018, 3227, 4788, 4949, 5182, 5183);
INSERT INTO `creature_loot_template` (`Entry`,`Item`,`Chance`,`GroupId`,`Comment`) VALUES
(580,  1220, 0.1, 0, "Redridge Drudger - Lupine Axe"),
(437,  1296, 0.1, 0, "Blackrock Renegade - Blackrock Mace"),
(4065, 1296, 0.1, 0, "Blackrock Sentry - Blackrock Mace"),
(485,  1448, 0.1, 0, "Blackrock Outrunner - Blackrock Gauntlets"),
(626,  2166,   2, 2, "Foreman Thistlenettle - Foreman's Leggings"),
(626,  2167,   2, 2, "Foreman Thistlenettle - Foreman's Gloves"),
(626,  2168,   2, 2, "Foreman Thistlenettle - Corpse Rompers"),
(7170, 4949, 100, 0, "Thragomm - Orcish Cleaver"),
(3398, 5182,   2, 2, "Gesharahan - Shiver Blade"),
(3398, 5183,   2, 2, "Gesharahan - Pulsating Hydra Heart");
