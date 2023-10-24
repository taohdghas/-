#include <Novice.h>
#include <math.h>
#include <stdio.h>
#pragma warning(disable : 4996)
const char kWindowTitle[] = "爽快‼はじけ玉‼";
//関数
float Normalize(float aPos, float length) //正規化
{
	if (length != 0) {
		return aPos / length;
	}
	else {
		return aPos;
	}
}
float Length(float x, float y) {
	return sqrtf(x * x + y * y);
}
void PrintMap(int a, int b, int c, int d, int e, int f, unsigned int g) { //Posx,Posy,Radius,TileNumberW,TileNumberH,gazo,color
	Novice::DrawQuad(
		a * c, b * c,
		a * c + c, b * c,
		a * c, b * c + c,
		a * c + c, b * c + c,
		d * c, e * c, c, c,
		f, g);
}
int CTcount(int ct, int time) {
	if (ct + 1 > time) {
		return 0;
	}
	else {
		return ct + 1;
	}
}
//ベクトル
struct Vector2 {
	float x;                  //X軸
	float y;                  //Y軸
};
//matrix
struct Matrix2x2 {
	float m[2][2];
};
//サイズ
struct Size2 {
	const int width;          //幅
	const int height;         //高さ
};
//プレイヤーの構造体
struct Player {
	Vector2 pos;              //場所
	float radius;             //大きさ
	unsigned int color;       //色
	float angle;              //照準方向
	int isAlive;              //生存フラグ
	Vector2 direction;        //方向
};
enum direction {
	up,
	upriget,
	right,
	downright,
	down,
	downleft,
	left,
	upleft
};
int directionpoint = 0;
//玉の構造体
struct Ball {
	Vector2 pos;              //場所
	Vector2 velocity;         //速度
	float radius;             //大きさ
	unsigned int color;       //色
	float speed;
	int isShot;               //発射フラグ
	int HP;                   //反射に耐えられる回数
	int remainingballs;      //残りの玉の数
};
//敵の構造体
struct Enemy {
	Vector2 pos;              //場所
	float radius;             //大きさ
	int isAlive;              //生存フラグ
	int changeblock;          //ブロックの切り替えフラグ
	Vector2 direction;         //あげる方向
	int hp;
	int count;
	unsigned int color;
};
//回転する壁 (現段階では実装不可能？
struct Woll {
	Vector2 pos;
	int mapX;
	int mapY;
	int kn;
	int isAlive;
	int ct;
	int fl;
};
/*struct Block {
	Vector2 pos;
	Vector2 Map_Pos;
	int Map_Num;
	float angle; //回転の角度
	float rotationspeed; //回転の速度
};
*/
//ステージの構造体
struct Stage {
	int clear;                //クリアしたか
};
//ステージクリア演出の構造体
struct StageClear {
	Vector2 pos;
	int flag;
};
//リトライ演出の構造体
struct Retry {
	Vector2 pos;
	int flag;
};
void Hitballenemy(Ball ball, Enemy& enemy, Vector2& direction) {
	float distance = Length(ball.pos.x - enemy.pos.x, ball.pos.y - enemy.pos.y);
	Vector2 enemydirection = enemy.direction;
	if (distance <= Length(ball.radius, enemy.radius) && enemy.isAlive) {
		enemy.hp--;
		enemy.count++;
		ball.isShot = false;
		direction = { enemy.direction.x,enemy.direction.y };
		if (enemy.hp <= 0) {
			enemy.isAlive = false;
		}
		else {
			if (enemydirection.y == 0 || enemydirection.x == 0) {
				enemy.direction = { enemydirection.y * -1,enemydirection.x };
			}
			else if(enemydirection.y < 0 && enemydirection.x < 0 || enemydirection.y > 0 && enemydirection.x > 0){
				enemy.direction.x = enemydirection.x * -1;
			}
			else{
				enemy.direction.y = enemydirection.y * -1;
			}
		}
	}
}
// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 736);
	const int Map_W = 40;
	const int Map_H = 23;
	const int kEnemyMax = 20;
	int Map_radius = 32;
	int map[Map_H][Map_W];
	int enemyHP[kEnemyMax] = { 0 };
	int ballMapX = 0;
	int	ballMapY = 0;
	int	mapX = 0;
	int	mapY = 0;
	int mapenemy = 0;
	FILE* fp, *fpEnemy;
	enum MapNumber {
		null,//0
		kabe,//1
		naname,//2
		playernum,//3
		enemyUP,//4
		enemyDOWN,//5
		enemyRIGHT,//6
		enemyLEFT,//7
		enemyRIGHTup,//8
		enemyRIGHTdown,//9
		enemyLEFTup,//10
		enemyLEFTdown,//11
		kaiten,//12
	};
	Vector2 enemydirection[8]{
		{0,-1},//上
		{0,1},//下
		{1,0},//右
		{-1,0},//左
		{1,-1},//右上
		{1,1},//右下
		{-1,-1},//左上
		{-1,1}//左下
	};
	/*int blockHandle[];
	blockHandle[0] = Novice::LoadTexture("./block1.png");
	blockHandle[1] = Novice::LoadTexture("./block2.png");
	……
	*/
	//サウンド
	/*
	const int soundHandle[5] = {
	Novice::LoadAudio("./resource/sound/clear.mp3"),
	Novice::LoadAudio("./resource/sound/gekiha.mp3"),
	Novice::LoadAudio("./resource/sound/hannsya.mp3"),
	Novice::LoadAudio("./resource/sound/ritry.mp3"),
	Novice::LoadAudio("./resource/sound/hazikedama.mp3"),
	};

	int voiceHandle[5] = { 0 };
	voiceHandle[0] = -1;
	voiceHandle[1] = -1;
	voiceHandle[2] = -1;
	voiceHandle[3] = -1;
	voiceHandle[4] = -1;

	int hannsyaflag = false;
	*/
	
	///マップデータ///
	//ステージ1
	/*int stage1[][]={};*/
	//ステージ2
	/*int stage2[][]={}*/
	//ステージ3
	/*int stage3[][]={}*/
	float ruto = 0;
	//プレイヤー
	Player player{
		{0,0},              //場所
		25 ,                //大きさ
		0xffffffff,         //色
		90.0f,              //照準方向
		true,               //生存フラグ                
		{0,-1}
	};

	//玉
	Ball ball{
		{player.pos.x,player.pos.y + 10}, //場所
		{15,15},                            //速度 仮で0
		10,                               //大きさ 仮で10
		0xffffffff,                       //色 仮で0
		20.0f,                            //最高速度
		false,                            //発射フラグ
		5,                                //反射可能回数
		3,                                //玉を撃てる回数
	};
	//敵(1ステージに同じ方向が複数あるステージがあるかもなので数字を付けて差別化)
	Enemy enemy[kEnemyMax]{ 0 };
	int enemyhp[kEnemyMax] = { 0 };
	//回転する壁
	Woll woll{
		{0,0},
		0,0,
		naname
	};

	Stage stage[5]{
		false,
	};
	
	StageClear stageclear[5]{
		{0,0},//仮
		false,
	};
	Retry retry{
		{0,0},
		false,
	};
	// 画面変化
	enum screen {
		TITLE,  //タイトル
		//SELECT ステージセレクト(未定)
		STAGE,   //ステージ
		CLEAR,   //クリア画面
		RETRY    //リトライ画面
	};
	enum stage {
		STAGE1 = 1,
		STAGE2,
		STAGE3,
		STAGE4,
		STAGE5,
		STAGE6,
		STAGE7,
		STAGE8,
		STAGE9,
		STAGE10,
	};
	int Stagescene = STAGE;
	int Stagenow = STAGE1;
	int stageflag = true;
	screen screenscene = STAGE; //表示されるシーン
	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };
	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();
		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		//ステージの読み込み(仮)

		switch (Stagescene) {
		case STAGE1:
			fp = fopen("map/test1_1.txt", "r");
			for (int i = 0; i < Map_H; i++)for (int j = 0; j < Map_W; j++)fscanf(fp, "%d", &map[i][j]);
			fclose(fp);
			fpEnemy = fopen("map/st1_EnemyHp.txt", "r");
		    for (int i = 0; i < kEnemyMax; i++)fscanf(fpEnemy, "%d", &enemyhp[i]);
			fclose(fpEnemy);
			Stagescene = 0;
			break;
		case STAGE2:
			fp = fopen("map/test2.txt", "r");
			for (int i = 0; i < Map_H; i++)for (int j = 0; j < Map_W; j++)fscanf(fp, "%d", &map[i][j]);
			fclose(fp);
			fpEnemy = fopen("map/st2_EnemyHp.txt", "r");
			for (int i = 0; i < kEnemyMax; i++)fscanf(fpEnemy, "%d", &enemyhp[i]);
			fclose(fpEnemy);
			Stagescene = 0;
			break;
		case STAGE3:
			fp = fopen("map/test3.txt", "r");
			for (int i = 0; i < Map_H; i++)for (int j = 0; j < Map_W; j++)fscanf(fp, "%d", &map[i][j]);
			fclose(fp);
			fpEnemy = fopen("map/st3_EnemyHp.txt", "r");
			for (int i = 0; i < kEnemyMax; i++)fscanf(fpEnemy, "%d", &enemyhp[i]);
			fclose(fpEnemy);
			Stagescene = 0;
			break;
		case STAGE4:
			fp = fopen("map/test4.txt", "r");
			for (int i = 0; i < Map_H; i++)for (int j = 0; j < Map_W; j++)fscanf(fp, "%d", &map[i][j]);
			fclose(fp);
			fpEnemy = fopen("map/st4_EnemyHp.txt", "r");
			for (int i = 0; i < kEnemyMax; i++)fscanf(fpEnemy, "%d", &enemyhp[i]);
			fclose(fpEnemy);
			Stagescene = 0;
			break;
		case STAGE5:
			fp = fopen("map/st6.txt", "r");
			for (int i = 0; i < Map_H; i++)for (int j = 0; j < Map_W; j++)fscanf(fp, "%d", &map[i][j]);
			fclose(fp);
			fpEnemy = fopen("map/st6_EnemyHp.txt", "r");
			for (int i = 0; i < kEnemyMax; i++)fscanf(fpEnemy, "%d", &enemyhp[i]);
			fclose(fpEnemy);
			Stagescene = 0;
			break;
		case STAGE6:
			fp = fopen("map/st6.txt", "r");
			for (int i = 0; i < Map_H; i++)for (int j = 0; j < Map_W; j++)fscanf(fp, "%d", &map[i][j]);
			fclose(fp);
			fpEnemy = fopen("map/st6_EnemyHp.txt", "r");
			for (int i = 0; i < kEnemyMax; i++)fscanf(fpEnemy, "%d", &enemyhp[i]);
			fclose(fpEnemy);
			Stagescene = 0;
			break;
		case STAGE7:
			fp = fopen("map/st7.txt", "r");
			for (int i = 0; i < Map_H; i++)for (int j = 0; j < Map_W; j++)fscanf(fp, "%d", &map[i][j]);
			fclose(fp);
			fpEnemy = fopen("map/st7_EnemyHp.txt", "r");
			for (int i = 0; i < kEnemyMax; i++)fscanf(fpEnemy, "%d", &enemyhp[i]);
			fclose(fpEnemy);
			Stagescene = 0;
			break;
		case STAGE8:
			fp = fopen("map/st8.txt", "r");
			for (int i = 0; i < Map_H; i++)for (int j = 0; j < Map_W; j++)fscanf(fp, "%d", &map[i][j]);
			fclose(fp);
			fpEnemy = fopen("map/st8_EnemyHp.txt", "r");
			for (int i = 0; i < kEnemyMax; i++)fscanf(fpEnemy, "%d", &enemyhp[i]);
			fclose(fpEnemy);
			Stagescene = 0;
			break;
		case STAGE9:
			fp = fopen("map/st9.txt", "r");
			for (int i = 0; i < Map_H; i++)for (int j = 0; j < Map_W; j++)fscanf(fp, "%d", &map[i][j]);
			fclose(fp);
			fpEnemy = fopen("map/st9_EnemyHp.txt", "r");
			for (int i = 0; i < kEnemyMax; i++)fscanf(fpEnemy, "%d", &enemyhp[i]);
			fclose(fpEnemy);
			Stagescene = 0;
			break;
		case STAGE10:
			fp = fopen("map/st10.txt", "r");
			for (int i = 0; i < Map_H; i++)for (int j = 0; j < Map_W; j++)fscanf(fp, "%d", &map[i][j]);
			fclose(fp);
			fpEnemy = fopen("map/st10_EnemyHp.txt", "r");
			for (int i = 0; i < kEnemyMax; i++)fscanf(fpEnemy, "%d", &enemyhp[i]);
			fclose(fpEnemy);
			Stagescene = 0;
			break;
		}
		//玉と敵の当たり判定
		for (int i = 0; i < kEnemyMax; i++) {
			if (enemy[i].count == 0) {
				Hitballenemy(ball, enemy[i], player.direction);
			}
			else {
				enemy[i].count = CTcount(enemy[i].count, 10);
			}
			if (enemy[i].hp == 1) {
				enemy[i].color = 0xff0000ff;
			}
			if (enemy[i].hp == 2) {
				enemy[i].color = 0x00ff00ff;
			}
		}
	
		//回転当たり判定

		//マップチップの当たり判定
		ballMapX = int(ball.pos.x / Map_radius + player.direction.x);
		ballMapY = int(ball.pos.y / Map_radius + player.direction.y);
		mapX = int(ball.pos.x / Map_radius);
		mapY = int(ball.pos.y / Map_radius);
		if (player.direction.x == 0.0f) {
			if (map[ballMapY][mapX] == kabe || map[ballMapY][mapX] == kaiten && woll.kn == kabe) {
				player.direction.y *= -1;
				ball.HP--;
			}
			if (map[ballMapY][mapX] == naname || map[ballMapY][mapX] == kaiten && woll.kn == naname) {
				if (map[mapY][mapX + 1] == naname || map[mapY][mapX + 1] == kaiten) {
					player.direction.x = player.direction.y * -1;
					if (player.direction.y < 0) {
						player.direction.x *= -1;
					}
					player.direction.y = 0;
					ball.HP--;
				}
				else if (map[mapY][mapX - 1] == naname || map[mapY][mapX - 1] == kaiten) {
					player.direction.x = player.direction.y;
					if (player.direction.y < 0) {
						player.direction.x *= -1;
					}
					player.direction.y = 0;
					ball.HP--;
				}
			}
		}
		else if (player.direction.y == 0.0f) {
			if (map[mapY][ballMapX] == kabe || map[mapY][ballMapX] == kaiten && woll.kn == kabe) {
				player.direction.x *= -1;
				ball.HP--;
			}
			if (map[mapY][ballMapX] == naname || map[mapY][ballMapX] == kaiten && woll.kn == naname) {
				if (map[mapY + 1][mapX] == naname || map[mapY +1][mapX] == kaiten) {
					player.direction.y = player.direction.x * -1;
					if (player.direction.x < 0) {
						player.direction.y *= -1;
					}
					player.direction.x = 0;
					ball.HP--;
				}
				else if (map[mapY - 1][mapX] == naname || map[mapY -1][mapX] == kaiten) {
					player.direction.y = player.direction.x;
					if (player.direction.x < 0) {
						player.direction.y *= -1;
					}
					player.direction.x = 0;
					ball.HP--;
				}
			}
		}
		else {
			if (map[ballMapY][mapX] == kabe || map[ballMapY][mapX] == kaiten && woll.kn == kabe) {
				player.direction.y *= -1;
				ball.HP--;
			}
			if (map[mapY][ballMapX] == kabe || map[mapY][ballMapX] == kaiten && woll.kn == kabe) {
				player.direction.x *= -1;
				ball.HP--;
			}
			if (map[mapY][ballMapX] == naname && map[ballMapY][mapX] == naname || map[ballMapY][ballMapX] == naname ||
				map[mapY][ballMapX] == kaiten && map[ballMapY][mapX] == kaiten && woll.kn == naname || map[ballMapY][ballMapX] == kaiten && woll.kn == naname) {
				player.direction.x *= -1;
				player.direction.y *= -1;
				ball.HP--;
			}
		}

		switch (screenscene) {
		case TITLE:
			//spaceキー長押しで説明画面に移行(未定)
			if (keys[DIK_SPACE])
				//spaceキー押されたらセレクト画面に移行(未定)
				/*if (keys[DIK_SPACE]) {
				   screenscene = SELECT;
				};
				*/
				//spaceキー押されたらステージ画面へ移行
				if (keys[DIK_SPACE]) {
					screenscene = STAGE;

				}
			break;
		case STAGE:
			//case SELECT:
				//break;
			switch (stageflag) {
			case true:
				//初期化
				ball.isShot = false;
				ball.HP = 4; //仮
				ball.remainingballs = 3;//仮
				for (int i = 0; i < kEnemyMax; i++) {
					enemy[i].isAlive = false;
				}
				for (int y = 0; y < Map_H; y++) {
					for (int x = 0; x < Map_W; x++) {
						if (map[y][x] == playernum) {
							player.pos = { float(x) * Map_radius + (Map_radius / 2),float(y) * Map_radius + (Map_radius / 2) };
							ball.pos = { player.pos.x,player.pos.y };
							directionpoint = 0;
						}
						if (map[y][x] == enemyUP || map[y][x] == enemyDOWN || map[y][x] == enemyRIGHT || map[y][x] == enemyLEFT ||
						    map[y][x] == enemyRIGHTup || map[y][x] == enemyRIGHTdown || map[y][x] == enemyLEFTup|| map[y][x] == enemyLEFTdown) {
							for (int i = 0; i < kEnemyMax; i++) {
								if (!enemy[i].isAlive) {
									enemy[i].radius = 30;
									enemy[i].pos = { float(x) * Map_radius + (Map_radius / 2),float(y) * Map_radius + (Map_radius / 2) };
									enemy[i].isAlive = true;
									enemy[i].direction = enemydirection[map[y][x] - 4];
									enemy[i].hp = enemyhp[i];
									break;
								}
							}
						}
						if (map[y][x] == kaiten) {
							woll.pos = { float(x) * Map_radius,float(y) * Map_radius };
							woll.mapX = x;
							woll.mapY = y;
							woll.isAlive = true;
							if (map[y - 1][x - 1] == naname || map[y - 1][x + 1] == naname) {
								woll.kn = naname;
							}
							if (map[y][x - 1] == kabe || map[y - 1][x] == kabe) {
								woll.kn = kabe;
							}
						}
					}
				}
				stageflag = false;
				break;
			case false:
				break;
			}
			//マップチップ(プロト版)
			//弾が発射できる状態であればプレイヤーを操作できる
			if (!ball.isShot) {

				/*玉の発射方向*/
				//左矢印キーが押されたら左に45度傾く
				if (keys[DIK_LEFTARROW] && !preKeys[DIK_LEFTARROW]) {
					if (directionpoint <= 0) {
						directionpoint = 7;
					}
					else {
						directionpoint -= 1;
					}
				}

				//右矢印キーが押されたら右に45度傾く
				if (keys[DIK_RIGHTARROW] && !preKeys[DIK_RIGHTARROW]) {
					if (directionpoint >= 7) {
						directionpoint = 0;
					}
					else {
						directionpoint += 1;
					}

				}

				switch (directionpoint) {
				case up:
					player.direction = { 0,-1 };
					break;
				case upriget:
					player.direction = { 1,-1 };
					break;
				case right:
					player.direction = { 1,0 };
					break;
				case downright:
					player.direction = { 1,1 };
					break;
				case down:
					player.direction = { 0,1 };
					break;
				case downleft:
					player.direction = { -1,1 };
					break;
				case left:
					player.direction = { -1,0 };
					break;
				case upleft:
					player.direction = { -1,-1 };
					break;
				}

				/*玉の発射処理*/
				//Spaceキーが押されたら弾を発射する
				if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
					//玉の残り数を1減らす
					ball.remainingballs--;
					//弾の位置をプレイヤーの頭上に設定
					ball.pos = { player.pos.x,player.pos.y };

					//発射フラグをtrueにする
					//これで弾が消えるまでプレイヤーを操作できなくなる
					ball.isShot = true;

				}
			}

			//弾が発射されたら移動を開始する
			if (ball.isShot) {
				ruto = Length(player.direction.x, player.direction.y);
				if (ruto != 0.0f) {
					ball.pos.x += player.direction.x / ruto * ball.velocity.x;
					ball.pos.y += player.direction.y / ruto * ball.velocity.y;
				}
			}
			//
			if (woll.isAlive && woll.ct == 0) {
				woll.ct++;
				if (woll.kn == kabe) {
					if (map[woll.mapY - 1][woll.mapX] == kabe) {
						for (int i = -2; i < 3; i++) {
							if (i != 0) {
								map[woll.mapY - i][woll.mapX] = null;
								map[woll.mapY + i][woll.mapX - i] = naname;
								woll.kn = naname;
							}
						}
						break;
					}
					else if (map[woll.mapY][woll.mapX - 1] == kabe) {
						for (int i = -2; i < 3; i++) {
							if (i != 0) {
								map[woll.mapY][woll.mapX - i] = null;
								map[woll.mapY - i][woll.mapX - i] = naname;
								woll.kn = naname;
							}
						}
						break;
					}
				}
				if (woll.kn == naname) {
					if (map[woll.mapY - 1][woll.mapX - 1] == naname) {
						for (int i = -2; i < 3; i++) {
							if (i != 0) {
								map[woll.mapY - i][woll.mapX - i] = null;
								map[woll.mapY - i][woll.mapX] = kabe;
								woll.kn = kabe;
							}
						}
					}
					else if (map[woll.mapY - 1][woll.mapX + 1] == naname) {
						for (int i = -2; i < 3; i++) {
							if (i != 0) {
								map[woll.mapY + i][woll.mapX - i] = null;
								map[woll.mapY][woll.mapX - i] = kabe;
								woll.kn = kabe;
							}
						}
					}
				}
			}
			woll.ct = CTcount(woll.ct, 120);
			//玉が上限まで反射したらリセット
			if (ball.HP <= 0) {
				stageflag = true;
			}

			//全ての敵の生存フラグがfalseならステージクリア

			mapenemy = 0;
			for (int i = 0; i < 20; i++) {
				if (enemy[i].isAlive) {
					break;
				}
				if (i == 19) {
					Stagenow++;
					Stagescene = Stagenow;
					stageflag = true;
				}
			}

			/*if (stageclear1.flag) {
			ステージクリアの文字を表示させる
			何かしらの演出を入れる
			}

			*/
			/*if(ステージクリアの文字が出た){
			//次のステージへ
			if(keys[DIK_SPACE] && keys[DIK_SPACE] == 0){
			Stagescene = Stagescene += 1;
			}
			//長押しでタイトルへ戻る(未定)
			if(keys[DIK_SPACE]){
			screeenscene = TITLE;
			}
			}*/
			//もし玉を全て打ち切ってしまったらリトライ

			if (ball.remainingballs < 0) {
				retry.flag = true;
			}


			if (retry.flag) {
				//リトライの文字が出る
				//Novice::DrawSprite();
				//spaceキーを押してリトライするか、長押しでタイトルに戻る(未定)
				if (keys[DIK_SPACE] && keys[DIK_SPACE] == 0) {
					//ステージの状況をリセット
					stageflag = true;
				}
				else if (keys[DIK_SPACE]) {
					screenscene = TITLE;
				}
			}

			break;
		case CLEAR:
			break;
		}
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		switch (screenscene) {
		case TITLE:
			//サウンド
			/*
			Novice::StopAudio(voiceHandle[0]);
			Novice::StopAudio(voiceHandle[1]);
			Novice::StopAudio(voiceHandle[2]);
			Novice::StopAudio(voiceHandle[3]);

			if(Novice::IsPlayingAudio(voiceHandle[4]) == 0 || voiceHandle[4] == -1){
			voiceHandle[4] = Novice::PlayAudio(soundHandle[0],true,0.01f);
			}
			*/
			break;
		case STAGE:
			/*
			Novice::ScreenPrintf(0, 20, "BMX %d", ballMapX);
			Novice::ScreenPrintf(0, 40, "BMY %d", ballMapY);
			Novice::ScreenPrintf(0, 60, "MX %d", mapX);
			Novice::ScreenPrintf(0, 80, "MY %d", mapY);
			Novice::ScreenPrintf(0, 100, "PDX %f", player.direction.x);
			Novice::ScreenPrintf(0, 120, "PDY %f", player.direction.y);
			*/
			//サウンド
			/*
			if(Novice::IsPlayingAudio(voiceHandle[4]) == 0 || voiceHandle[4] == -1){
			voiceHandle[4] = Novice::PlayAudio(soundHandle[4],true,0.01f);
			}
			*/
			
			
			//マップチップ(プロトタイプ)
			for (int i = 0; i < Map_H; i++) {
				for (int j = 0; j < Map_W; j++) {
					if (map[i][j] == kabe) {
						Novice::DrawBox(j * Map_radius, i * Map_radius, Map_radius, Map_radius, 0.0f, RED, kFillModeWireFrame);
					}
					if (map[i][j] == naname) {
						Novice::DrawBox(j * Map_radius, i * Map_radius, Map_radius, Map_radius, 0.0f, BLUE, kFillModeWireFrame);
					}
				}
			}
			/*
			for(int y = 0;y < Map_H;y++){
			for(int x = 0;x < Map_W;x++){
			PrintMap(x,y,Map_rad,Map[y][x],0,mapgazo,color);
			}
			}
			*/
			//プレイヤー　プロトタイプにつき円で。
			if (player.isAlive) {
				Novice::DrawEllipse(int(player.pos.x), int(player.pos.y), int(player.radius),
					int(player.radius), 0.0f, WHITE, kFillModeSolid);
			}
			//玉
			if (ball.isShot) {
				Novice::DrawEllipse(int(ball.pos.x), int(ball.pos.y), int(ball.radius),
					int(ball.radius), 0.0f, RED, kFillModeSolid);
				/*
				if(keys[DIK_SPACE]){
				if(Novice::IsPlayingAudio(voiceHandle[]) == 0 || voiceHandle[] == -1){
				  voiceHandle[] = Novice::PlayAudio(soundHandle[],false,0,5f);			
				  }
				}
				*/
			}

			//玉の発射方向を表す矢印
			/*
			if(ball.isshot){

			}
			*/

			///敵///
			for (int i = 0; i < kEnemyMax; i++) {
				if (enemy[i].isAlive) {
					Novice::DrawEllipse(int(enemy[i].pos.x), int(enemy[i].pos.y), int(enemy[i].radius),
						int(enemy[i].radius), 0.0f, enemy[i].color, kFillModeSolid);
				}
			}
			

			//エフェクト等//

			//UI・HUDなど//
			//タイトル

			//操作・ゲーム説明など(タイトル画面もしくは、ステージセレクト画面から逝けるようにする)

			//ステージセレクト画面(未定)


			//ステージクリア
			/*
			if(stageclear1.flag){
			何かしらの演出をしたうえで描画する
			}
			if(stageclear2.flag){
			何かしらの演出をしたうえで描画する
			}
			if(stageclear3.flag){
			何かしらの演出をしたうえで描画する
			}

			//リトライ
			if(retry.flag){
			何かしらの演出をしたうえで描画する
			}
			//残り玉数(文字にするか、スプライトにするか)

			//反射可能回数(数字で)

			//ステージ(何ステージ目か表示)


			*/

			break;
		case CLEAR:
			break;
		}
		///
		/// ↑描画処理ここまで
		///
	// フレームの終了
		Novice::EndFrame();
		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}
	// ライブラリの終了
	Novice::Finalize();
	return 0;
}