#pragma once



/*
640		360 
768		432 // 0.4
896		504
960		540 // 0.5
1024	576
1152	648 // 0.6
1280	720
1408	792
1536	864 // 0.8
1664	936
1792	1008
1920	1080 // 1
*/

const int WindowX = 1920;
const int WindowY = 1080;

const int GameWindowAreaLeft = 480;
const int GameWindowAreaTop = 20;

const int GameWindowSizeX = 960;
const int GameWindowSizeY = WindowY - (20 * 2);

const int GameWindowCenterX = GameWindowAreaLeft + GameWindowSizeX/2;
const int GameWindowCenterY = GameWindowAreaTop + GameWindowSizeY/2;

const int GameWindowAreaRight = GameWindowAreaLeft + GameWindowSizeX;
const int GameWindowAreaBottom = GameWindowAreaTop + GameWindowSizeY;

//              01234567890123456789012345678901234567
//              12345678901234567890123456789012345678
//char str[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_ ";
constexpr int NameEntryFontMax = 26 + 10 + 1;
constexpr int NameEntryFontMax_s = 26 + 10 + 2;

