menu: menu2.cpp AI.cpp display2.cpp learning.cpp move.cpp player2.cpp table.cpp
	g++ -o menu menu2.cpp AI.cpp display2.cpp learning.cpp move.cpp player2.cpp table.cpp
menu2.cpp: menu.cpp
	iconv -f gb2312 -t utf-8 menu.cpp > menu2.cpp
display2.cpp: display.cpp
	iconv -f gb2312 -t utf-8 display.cpp > display2.cpp
player2.cpp: player.cpp
	iconv -f gb2312 -t utf-8 player.cpp > player2.cpp
