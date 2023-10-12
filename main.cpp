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
	int stock;                //残機
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
};

//敵の構造体
struct Enemy {
	Vector2 pos;              //場所
	float radius;             //大きさ
	int isAlive;              //生存フラグ
	int changeblock;          //ブロックの切り替えフラグ
	Vector2 direction;         //あげる方向
};

//回転する壁
struct Block {
	Vector2 pos;
	Vector2 Map_Pos;
	int Map_Num;
	float angle; //回転の角度
	float rotationspeed; //回転の速度
};

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
struct RETRY {
	Vector2 pos;
	int flag;
};

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
	FILE* fp;
	enum MapNumber {
		null,
		kabe,
		naname

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
		3,                   //残機
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
		5,                                //HP 仮で3
	};
	int ballMapNum = 0;

	//敵(1ステージに同じ方向が複数あるステージがあるかもなので数字を付けて差別化)
	Enemy enemyUP1{
		{1000,1000},      //場所
		30,         //大きさ
		false,          //生存フラグ
		false,          //ブロックの切り替えフラグ
		{0,-1}
	};
	Enemy enemyUP2{
		{1000,1000},      //場所
		30,         //大きさ
		false,          //生存フラグ
		false,          //ブロックの切り替えフラグ
		{0,-1}
	};
	//
	Enemy enemyDOWN1{
		{1000,1000},      //場所
		30,         //大きさ
		false,          //生存フラグ
		false,          //ブロックの切り替えフラグ
		{0,1}
	};
	//
	Enemy enemyLEFT1{
		{1000,1000},      //場所
		30,         //大きさ
		false,          //生存フラグ
		false,          //ブロックの切り替えフラグ
		{-1,0}
	};
	//
	Enemy enemyRIGHT1{
		{1000,1000},      //場所
		30,         //大きさ
		false,          //生存フラグ
		false,          //ブロックの切り替えフラグ
		{1,0}
	};

	Block block1{
		{0,0},
		{0,0},
		0,
		0,
		0,
	};

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
			fp = fopen("test_ver1.1.txt", "r"); //test,test_ver1.1
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

		float distance1 = Length(ball.pos.x - enemyUP1.pos.x, ball.pos.y - enemyUP1.pos.y);
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
		}


		//マップチップの当たり判定
		ballMapX = int(ball.pos.x / Map_radius + player.direction.x);
		ballMapY = int(ball.pos.y / Map_radius + player.direction.y);
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
				//初期化
				player.pos.x = 300;
				player.pos.y = 650;
				ball.HP = 5; //仮
				enemyUP1.pos.x = 900;
				enemyUP1.pos.y = 400;
				enemyDOWN1.pos.x = 200;
				enemyDOWN1.pos.y = 100;
				enemyLEFT1.pos.x = 900;
				enemyLEFT1.pos.y = 100;
				enemyUP1.isAlive = true;
				enemyDOWN1.isAlive = true;
				enemyLEFT1.isAlive = true;
				Stagescene = STAGE4;
				break;
			case STAGE2:
				player.pos.x = 300;
				player.pos.y = 650;
				ball.HP = 5; //仮
				enemyUP1.pos.x = 100;
				enemyUP1.pos.y = 600;
				enemyUP2.pos.x = 900;
				enemyUP2.pos.y = 300;
				enemyUP1.isAlive = true;
				enemyUP2.isAlive = true;
				break;
			case STAGE3:
				player.pos.x = 300;
				player.pos.y = 650;
				ball.HP = 5; //仮
				enemyUP1.pos.x = 0;
				enemyUP1.pos.y = 0;
				enemyUP2.pos.x = 0;
				enemyUP2.pos.y = 0;
				enemyRIGHT1.pos.x = 0;
				enemyRIGHT1.pos.y = 0;
				enemyUP1.isAlive = true;
				enemyUP2.isAlive = true;
				enemyRIGHT1.isAlive = true;
				break;
			}
			//マップチップ(プロト版)

			//弾が発射できる状態であればプレイヤーを操作できる
			if (!ball.isShot) {

				/*玉の発射方向*/

				//左矢印キーが押されたら左に45度傾く
				if (keys[DIK_LEFTARROW] && !preKeys[DIK_LEFTARROW] && directionpoint > 1) {
					directionpoint -= 1;
					//player.angle -= 45.0f;
				}
				//右矢印キーが押されたら右に45度傾く
				if (keys[DIK_RIGHTARROW] && !preKeys[DIK_RIGHTARROW] && directionpoint < 3) {
					directionpoint += 1;
					//player.angle += 45.0f;
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
				//限界角度以上傾かないようにする
				//if (player.angle > 135.0f) {
					//player.angle = 135.0f;
				//}
				//if (player.angle < 45.0f) {
					//player.angle = 45.0f;
				//}

				/*玉の発射処理*/

				//Spaceキーが押されたら弾を発射する
				if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {

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
				enemyUP1.isAlive = true;
				enemyDOWN1.isAlive = true;
				enemyLEFT1.isAlive = true;
			}


			//全ての敵の生存フラグがfalseならステージクリア
			if (enemyUP1.isAlive && enemyDOWN1.isAlive && enemyLEFT1.isAlive) {
				//stageclear1.flag = true;
			}

			//if(stageclear1.flag){

			//}

			/*if(ステージクリアの文字が出た){
			if(keys[DIK_SPACE]&&preKeys[DIK_SPACE] == 0){
			   screencene = STAGE2   //次のステージへ
			   }
			}*/
			//もし玉を全て打ち切ってしまったらリトライ
		/*
		if(残り玉数が0なら){
		retry.flag = true;
		}

		if(retry.flag){
		//リトライの文字が出る
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

			Novice::ScreenPrintf(0, 20, "BMX %d", ballMapX);
			Novice::ScreenPrintf(0, 40, "BMY %d", ballMapY);
			Novice::ScreenPrintf(0, 60, "MX %d", mapX);
			Novice::ScreenPrintf(0, 80, "MY %d", mapY);
			Novice::ScreenPrintf(0, 100, "PDX %f", player.direction.x);
			Novice::ScreenPrintf(0, 120, "PDY %f", player.direction.y);

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
			if (enemyUP1.isAlive) {
				Novice::DrawEllipse(int(enemyUP1.pos.x), int(enemyUP1.pos.y), int(enemyUP1.radius),
					int(enemyUP1.radius), 0.0f, RED, kFillModeSolid);
			}

			//DOWN
			if (enemyDOWN1.isAlive) {
				Novice::DrawEllipse(int(enemyDOWN1.pos.x), int(enemyDOWN1.pos.y), int(enemyDOWN1.radius),
					int(enemyDOWN1.radius), 0.0f, BLUE, kFillModeSolid);
			}

			//LEFT
			if (enemyLEFT1.isAlive) {
				Novice::DrawEllipse(int(enemyLEFT1.pos.x), int(enemyLEFT1.pos.y), int(enemyLEFT1.radius),
					int(enemyLEFT1.radius), 0.0f, GREEN, kFillModeSolid);
			}

			//LIGHT
			if (enemyRIGHT1.isAlive) {
				Novice::DrawEllipse(int(enemyRIGHT1.pos.x), int(enemyRIGHT1.pos.y),
					int(enemyRIGHT1.radius), int(enemyRIGHT1.radius), 0.0f, WHITE, kFillModeSolid);
			}
			//UI・HUDなど//
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
