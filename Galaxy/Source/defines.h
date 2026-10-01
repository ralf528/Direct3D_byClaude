#pragma once

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// ☆━─ 15-06-26, 디파인, SH. ─━☆
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 윈도우의 크기 
#define		WINSIZE_X	1024 
#define		WINSIZE_Y	768

//< 해제디파인
#define SAFE_DELETE(p) if(p != NULL) { delete p; p = NULL; }
#define SAFE_DELETE_ARR(p) if( p!= NULL ) { delete [] p; p = NULL; }
#define SAFE_RELEASE(h) if( h != NULL ) { h->Release(); h = NULL; }

//< 전역매니져
#define GAME_MGR	Singleton<GameMgr>::getInstance()


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// ☆━─ 15-06-26, 게임 디파인, SH. ─━☆
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//< 적군 개체 수
#define ENEMY_NUM 50
//< 총알 개수
#define BULLET_NUM 20