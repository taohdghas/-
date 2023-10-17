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
float Length(float x, float y)
{
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
	left = 1,
	up,
	right
};
int directionpoint = 2;
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
};
//回転する壁 (現段階では実装不可能？)
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
void Hitballenemy(Ball ball, Enemy& enemy,Vector2& direction) {
	float distance = Length(ball.pos.x - enemy.pos.x, ball.pos.y - enemy.pos.y);
	if (distance <= Length(ball.radius, enemy.radius) && enemy.isAlive) {
		enemy.hp--;
		if (enemy.hp == 0) {
			enemy.isAlive = false;
		}
		//矢印の方向へ行く
		direction = { enemy.direction.x,enemy.direction.y };
	}
}
// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 736);
	const int Map_W = 40;
	const int Map_H = 23;
	int Map_radius = 32;
	int MapNum = 0;
	int map[Map_H][Map_W];
	int ballMapX = 0;
	int	ballMapY = 0;
	int	mapX = 0;
	int	mapY = 0;
	int mapenemy = 0;
	FILE* fp;
	enum MapNumber {
		null,
		kabe,
		naname,
		enemynum,
		playernum
	};
	/*int blockHandle[];
	blockHandle[0] = Novice::LoadTexture("./block1.png");
	blockHandle[1] = Novice::LoadTexture("./block2.png");
	……
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
	int ballMapNum = 0;
	//敵(1ステージに同じ方向が複数あるステージがあるかもなので数字を付けて差別化)
	Enemy enemy[20]{
		{{0,0},0,true,true,{0,0}}
	};
	for (int i = 0; i < 20; i++) {
		enemy[i].isAlive = true;
		enemy[i].changeblock = true;
	}
	for (int y = 0; y < Map_H; y++) {
		for (int x = 0; x < Map_W; x++) {
			for(int i = 0;i < 20;i++){
				if (map[y][x] == enemynum && enemy[i].isAlive == true) {
					enemy[i].radius = 30;
					enemy[i].pos = { float(x) * enemy[i].radius,float(y) * enemy[i].radius };
					enemy[i].isAlive = false;
					enemy[i].changeblock = false;
					enemy[i].direction = {0,-1};
					break;
				}
			}
		}
	}
	/*Enemy enemyUP2{
		{1000,1000},      //場所
		30,         //大きさ
		false,          //生存フラグ
		false,          //ブロックの切り替えフラグ
		{0,-1}
	};

	Enemy enemyDOWN1{
		{1000,1000},      //場所
		30,         //大きさ
		false,          //生存フラグ
		false,          //ブロックの切り替えフラグ
		{0,1}
	};

	Enemy enemyDOWN2{
		{0,0},
		30,
		false,
		false,
		{0,1}
	};
	Enemy enemyLEFT1{
		{1000,1000},      //場所
		30,         //大きさ
		false,          //生存フラグ
		false,          //ブロックの切り替えフラグ
		{-1,0}
	};

	Enemy enemyLEFT2{
		{0,0},
		30,
		false,
		false,
		{-1,0}
	};
	Enemy enemyRIGHT1{
		{1000,1000},      //場所
		30,         //大きさ
		false,          //生存フラグ
		false,          //ブロックの切り替えフラグ
		{1,0}
	};
	Enemy enemyRIGHT2{
		{0,0},
		30,
		false,
		false,
		{1,0}
	};
	/*Block block1{
		{0,0},
		{0,0},
		0,
		0,
		0,
	};
	*/
	Stage stage1{
		false,
	};
	Stage stage2{
		false,
	};
	Stage stage3{
		false,
	};
	Stage stage4{
		false,
	};
	Stage stage5{
		false,
	};
	StageClear stageclear1{
		{0,0},//仮
		false,
	};
	StageClear stageclear2{
		{0,0},//仮
		false,
	};
	StageClear stageclear3{
		{0,0},//仮
		false,
	};
	StageClear stageclear4{
		{0,0},
		false,
	};
	StageClear stageclear5{
		{0,0},
		false,
	};
	Retry retry{
		{0,0},
		true
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
	};
	int Stagescene = 1;
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
			fp = fopen("test1.txt", "r"); //test,test_ver1.1
			for (int i = 0; i < 23; i++)for (int j = 0; j < 40; j++)fscanf(fp, "%d", &map[i][j]);
			fclose(fp);
			break;
		case STAGE2:
			fp = fopen("test2.txt", "r");
			for (int i = 0; i < 23; i++)for (int j = 0; j < 40; j++)fscanf(fp, "%d", &map[i][j]);
			fclose(fp);
			break;
		case STAGE3:
			fp = fopen("test3.txt", "r");
			for (int i = 0; i < 23; i++)for (int j = 0; j < 40; j++)fscanf(fp, "%d", &map[i][j]);
			fclose(fp);
			break;
		}

		//玉と敵の当たり判定
		for (int i = 0; i < 20; i++) {
			Hitballenemy(ball, enemy[i], player.direction);
		}
		/*float distance1 = Length(ball.pos.x - enemyUP1.pos.x, ball.pos.y - enemyUP1.pos.y);
		if (distance1 <= 31.0f && enemyUP1.isAlive) {
			enemyUP1.isAlive = false;
			//矢印の方向へ行く
			player.direction = { enemyUP1.direction.x,enemyUP1.direction.y };
		}
		float distance1_2 = Length(ball.pos.x - enemyUP2.pos.x, ball.pos.y - enemyUP2.pos.y);
		if (distance1_2 <= 31.0f && enemyUP2.isAlive) {
			enemyUP2.isAlive = false;
			//矢印の方向へ行く
			player.direction = { enemyUP2.direction.x,enemyUP2.direction.y };
		}
		float distance2 = Length(ball.pos.x - enemyDOWN1.pos.x, ball.pos.y - enemyDOWN1.pos.y);
		if (distance2 <= 31.0f && enemyDOWN1.isAlive) {
			enemyDOWN1.isAlive = false;
			//矢印の方向へ行く
			player.direction = { enemyDOWN1.direction.x,enemyDOWN1.direction.y };
		}
		float distance3 = Length(ball.pos.x - enemyLEFT1.pos.x, ball.pos.y - enemyLEFT1.pos.y);
		if (distance3 <= 31.0f && enemyLEFT1.isAlive) {
			enemyLEFT1.isAlive = false;
			//矢印の方向へ行く
			player.direction = { enemyLEFT1.direction.x,enemyLEFT1.direction.y };
		}
		float distance4 = Length(ball.pos.x - enemyRIGHT1.pos.x, ball.pos.y - enemyRIGHT1.pos.y);
		if (distance4 <= 31.0f && enemyRIGHT1.isAlive) {
			enemyRIGHT1.isAlive = false;
			//矢印の方向へ行く
			player.direction = { enemyRIGHT1.direction.x,enemyRIGHT1.direction.y };
		}*/

		//マップチップの当たり判定
		ballMapX = int(ball.pos.x  / Map_radius + player.direction.x);
		ballMapY = int(ball.pos.y  / Map_radius + player.direction.y);
		mapX = int(ball.pos.x / Map_radius);
		mapY = int(ball.pos.y / Map_radius);
		if (player.direction.x == 0.0f) {
			if (map[ballMapY][ballMapX] == kabe) {
				player.direction.y *= -1;
				ball.HP--;
			}
			if (map[ballMapY][ballMapX] == naname) {
				player.direction.x = player.direction.y * -1;
				player.direction.y = 0;
				ball.HP--;
			}
		}
		else if (player.direction.y == 0.0f) {
			if (map[ballMapY][ballMapX] == kabe) {
				player.direction.x *= -1;
				ball.HP--;
			}
			if (map[ballMapY][ballMapX] == naname) {
				player.direction.y = player.direction.x * -1;
				player.direction.x = 0;
				ball.HP--;
			}
		}
		else {
			if (map[ballMapY][mapX] == kabe) {
				player.direction.y *= -1;
				ball.HP--;
			}
			if (map[mapY][ballMapX] == kabe) {
				player.direction.x *= -1;
				ball.HP--;
			}
			if (map[mapY][ballMapX] == naname && map[ballMapY][mapX] == naname || map[ballMapY][ballMapX] == naname) {
				player.direction.x *= -1;
				player.direction.y *= -1;
				ball.HP--;
			}
		}

		switch (screenscene) {
		case TITLE:
			//spaceキー長押しで説明画面に移行(未定)
            if(keys[DIK_SPACE])
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
			switch (Stagescene) {
			case STAGE1:
			case STAGE2:
			case STAGE3:
				//初期化
				ball.HP = 5; //仮
				ball.remainingballs = 3;//仮
				for (int i = 0; i < 20; i++) {
					enemy[i].isAlive = true;
					enemy[i].changeblock = true;
				}
				for (int y = 0; y < Map_H; y++) {
					for (int x = 0; x < Map_W; x++) {
						if(map[y][x] == playernum){
							player.pos = { float(x) * Map_radius,float(y) * Map_radius };
						}
						for (int i = 0; i < 20; i++) {
							if (map[y][x] == enemynum && enemy[i].isAlive == true) {
								enemy[i].radius = 30;
								enemy[i].pos = { float(x) * Map_radius,float(y) * Map_radius };
								enemy[i].isAlive = false;
								enemy[i].changeblock = false;
								enemy[i].direction = { 0,-1 };
								break;
							}
						}
					}
				}
				Stagescene = 0;
				break;
			default:
				break;
			}
			//マップチップ(プロト版)
			//弾が発射できる状態であればプレイヤーを操作できる
			if (!ball.isShot) {

				/*玉の発射方向*/
				//左矢印キーが押されたら左に45度傾く
				if (keys[DIK_LEFTARROW] && !preKeys[DIK_LEFTARROW] && directionpoint > 1) {
					directionpoint -= 1;
				}

				//右矢印キーが押されたら右に45度傾く
				if (keys[DIK_RIGHTARROW] && !preKeys[DIK_RIGHTARROW] && directionpoint < 3) {
					directionpoint += 1;
				}

				switch (directionpoint) {
				case left:
					player.direction = { -1,-1 };
					break;
				case up:
					player.direction = { 0,-1 };
					break;
				case right:
					player.direction = { 1,-1 };
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

			//玉が上限まで反射したらリセット
			if (ball.HP == 0) {
				ball.isShot = false;
				ball.HP = 5;
				for (int i = 0; i < 20; i++) {
					enemy[i].isAlive = true;
				}
			}

			//全ての敵の生存フラグがfalseならステージクリア
			mapenemy = 0;
			for(int i = 0;i < 20;i++){
				if (enemy[i].isAlive == 1) {
					break;
				}
				//stageclear1.flag = true;
			}

			/*if (stageclear1.flag) {
			ステージクリアの文字を表示させる
			何かしらの演出を入れる
			}

			*/
			/*if(ステージクリアの文字が出た){
			//spaceキーを押して次のステージに進む。長押しでタイトルに戻れる
			}*/
			//もし玉を全て打ち切ってしまったらリトライ
			/*
			if (ball.remainingballs == 0) {
				retry.flag = true;
			}
			*/
			/*
			if(retry.flag){
			//リトライの文字が出る
			//spaceキーを押してリトライするか、長押しでタイトルに戻る(未定)
			if(keys[DIK_SPACE] && keys[DIK_SPACE]) == 0{
			//ステージの状況をリセット
			}
			if(keys[DIK_SPACE]){
			screenscene = TITLE;
			 }
			}
			*/
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
			//マップチップ(プロトタイプ)
			for (int i = 0; i < 23; i++) {
				for (int j = 0; j < 40; j++) {
					if (map[i][j] == 1) {
						Novice::DrawBox(j * Map_radius, i * Map_radius, Map_radius, Map_radius, 0.0f, RED, kFillModeWireFrame);
					}
					if (map[i][j] == 2) {
						Novice::DrawBox(j * Map_radius, i * Map_radius, Map_radius, Map_radius, 0.0f, BLUE, kFillModeWireFrame);
					}
				}
			}
			/*
			for(int y = 0;y < Map_H;y++){ 
			for(int x = 0;x < Map_W;x++){
			PrintMap(Map.x,Map.y,rad,TileNum.W,TileNum.H,gazou,color);
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
			}
			///敵///
			//UP
			for (int i = 0; i < 20; i++) {
				if (enemy[i].isAlive) {
					Novice::DrawEllipse(int(enemy[i].pos.x), int(enemy[i].pos.y), int(enemy[i].radius),
						int(enemy[i].radius), 0.0f, RED, kFillModeSolid);
				}
			}
			/*if (enemyUP2.isAlive) {
				Novice::DrawEllipse(int(enemyUP2.pos.x), int(enemyUP2.pos.y), int(enemyUP2.radius),
					int(enemyUP2.radius), 0.0f, RED, kFillModeSolid);
			}
			//DOWN
			if (enemyDOWN1.isAlive) {
				Novice::DrawEllipse(int(enemyDOWN1.pos.x), int(enemyDOWN1.pos.y), int(enemyDOWN1.radius),
					int(enemyDOWN1.radius), 0.0f, BLUE, kFillModeSolid);
			}

			if (enemyDOWN2.isAlive) {
				Novice::DrawEllipse(int(enemyDOWN2.pos.x), int(enemyDOWN2.pos.y), int(enemyDOWN2.radius),
					int(enemyDOWN2.radius), 0.0f, RED, kFillModeSolid);
			}

			//LEFT
			if (enemyLEFT1.isAlive) {
				Novice::DrawEllipse(int(enemyLEFT1.pos.x), int(enemyLEFT1.pos.y), int(enemyLEFT1.radius),
					int(enemyLEFT1.radius), 0.0f, GREEN, kFillModeSolid);
			}

			if (enemyLEFT2.isAlive) {
				Novice::DrawEllipse(int(enemyLEFT2.pos.x), int(enemyLEFT2.pos.y), int(enemyLEFT2.radius),
					int(enemyLEFT2.radius), 0.0f, RED, kFillModeSolid);
			}

			//LIGHT
			if (enemyRIGHT1.isAlive) {
				Novice::DrawEllipse(int(enemyRIGHT1.pos.x), int(enemyRIGHT1.pos.y),
					int(enemyRIGHT1.radius), int(enemyRIGHT1.radius), 0.0f, WHITE, kFillModeSolid);
			}

			if (enemyRIGHT2.isAlive) {
				Novice::DrawEllipse(int(enemyRIGHT2.pos.x), int(enemyRIGHT2.pos.y),
					int(enemyRIGHT2.radius), int(enemyRIGHT2.radius), 0.0f, RED, kFillModeSolid);
			}*/

			//UI・HUDなど//
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