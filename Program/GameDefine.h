#pragma once

/*
640		360
768		432
896		504
1024	576
1152	648
1280	720
1408	792
1536	864
1664	936
1792	1008
1920	1080
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




