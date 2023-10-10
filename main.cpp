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
};

//弾の構造体
struct Ball {
	Vector2 pos;              //場所
	Vector2 velocity;         //速度
	float radius;             //大きさ
	unsigned int color;       //色
	float speed;              //最高速度
	int isShot;               //発射フラグ
	int HP;                   //反射に耐えられる回数
};

//敵の構造体
struct Enemy{
	Vector2 pos;              //場所
	float radius;             //大きさ
	int isAlive;              //生存フラグ
	int changeblock;          //ブロックの切り替えフラグ
	Vector2 velocity;         //あげる速度
};

//回転する壁
struct Block {
	Vector2 pos;
	Vector2 Map_Pos;
	int Map_Num;

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

struct Map_cip {
	float posX[100];
	float posY[100];
	float w;
	float h;
};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	int map[100][100];
	FILE* fp;
	fp = fopen("test.txt", "r");

	for (int i = 0; i < 23; i++)for (int j = 0; j < 40; j++)fscanf(fp, "%d", &map[i][j]);

	Map_cip block = { {0},{0},32,32 };

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


	//プレイヤー
	Player player{
		{0,0},              //場所
	    25 ,                //大きさ
		0xffffffff,         //色
		90.0f,              //照準方向
		true,               //生存フラグ
		3                   //残機
	};

	//玉
	Ball ball{
		{player.pos.x,player.pos.y + 10}, //場所
		{0,0},                            //速度 仮で0
		10,                               //大きさ 仮で10
		0xffffffff,                       //色 仮で0
		30.0f,                            //最高速度
		false,                            //発射フラグ
		3,                                //HP 仮で3
	};

	//敵(1ステージに同じ方向が複数あるステージがあるかもなので数字を付けて差別化)
	Enemy enemyUP1{
		{0,0},      //場所
		30,         //大きさ
		0,          //生存フラグ
		0,          //ブロックの切り替えフラグ
	};
	Enemy enemyUP2{
		{0,0},      //場所
		30,         //大きさ
		0,          //生存フラグ
		0,          //ブロックの切り替えフラグ
	};
	//
	Enemy enemyDOWN1{
		{0,0},      //場所
		30,         //大きさ
		0,          //生存フラグ
		0,          //ブロックの切り替えフラグ
	};
	//
	Enemy enemyLEFT1{
		{0,0},      //場所
		30,         //大きさ
		0,          //生存フラグ
		0,          //ブロックの切り替えフラグ
	};
	//
	Enemy enemyLIGHT1{
		{0,0},      //場所
		30,         //大きさ
		0,          //生存フラグ
		0,          //ブロックの切り替えフラグ
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

	// 画面変化
	enum screen {
		TITLE,  //タイトル
		//SELECT ステージセレクト
		STAGE1,   //ステージ1
		STAGE2,
		STAGE3,
		CLEAR   //クリア画面
	};

	screen screenscene = STAGE1; //表示されるシーン

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

		//玉と敵の当たり判定(仮で円同士の当たり判定でいきます)
		/*
		 auto = Lengh( ball.pos.x - EnemyUP.pos.x, ball.pos.y - EnemyUP.pos.y);
		 if(auto <= 00){
		    enemyhp -1;
			player.velosity.x = enemy.velosity.x;
			player.velosity.y = enemy.velosity.y;
		    }

		*/

		//マップチップの当たり判定
		/*
		
		*/


		switch (screenscene) {
		case TITLE:
			//spaceキー押されたらセレクト画面に移行(未定)
			/*if (keys[DIK_SPACE]) {
			   screenscene = SELECT;
			};
			*/

			//spaceキー押されたらステージ画面へ移行
			if (keys[DIK_SPACE]) {
				screenscene = STAGE1;
			}

			break;
			//case SELECT:
				//break;
		case STAGE1:
			player.pos.x = 300;
			player.pos.y = 650;
			enemyUP1.pos.x = 900;
			enemyUP1.pos.y = 400;
			enemyDOWN1.pos.x = 200;
			enemyDOWN1.pos.y = 100;
			enemyLEFT1.pos.x = 900;
			enemyLEFT1.pos.y = 100;

			//マップチップ(プロト版)
			for (int i = 0; i < 23; i++) {
				for (int j = 0; j < 40; j++) {
					block.posX[j] = block.w * j;
					block.posY[i] = block.h * i;
				}
			}

			//弾が発射できる状態であればプレイヤーを操作できる
			if (!ball.isShot) {

				/*玉の発射方向*/

				//左矢印キーが押されたら左に45度傾く
				if (keys[DIK_LEFTARROW] && !preKeys[DIK_LEFTARROW]) {
					player.angle += 45.0f;
				}
				//右矢印キーが押されたら右に45度傾く
				if (keys[DIK_RIGHTARROW] && !preKeys[DIK_RIGHTARROW]) {
					player.angle -= 45.0f;
				}

				//限界角度以上傾かないようにする
				if (player.angle > 135.0f) {
					player.angle = 135.0f;
				}
				if (player.angle < 45.0f) {
					player.angle = 45.0f;
				}

				/*玉の発射処理*/

				//Spaceキーが押されたら弾を発射する
				if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {

					//弾の位置をプレイヤーの頭上に設定
					ball.pos = { player.pos.x,player.pos.y };

					//弾の速度を照準方向から設定
					//方向はdegreeからradianに変換する
					ball.velocity = { cosf(player.angle * (3.14f / 180.0f)) * ball.speed
									 ,sinf(player.angle * (3.14f / 180.0f)) * ball.speed };

					//発射フラグをtrueにする
					//これで弾が消えるまでプレイヤーを操作できなくなる
					ball.isShot = true;

				}

			}

			//弾が発射されたら移動を開始する
			if (ball.isShot) {
				ball.pos.x += ball.velocity.x;
				ball.pos.y += ball.velocity.y;
			}

			/*もし敵に当たったら発射フラグをfalseに

			//もし〇回玉が反射したら発射フラグをfalseに


			//全ての敵の生存フラグがfalseならステージクリア
				/*if(enemyUP.isAlive && enemyDOWN.isAlive && enemyLEFT.isAlive){
				stage1.clear = true;
				stageclear1.flag = true;
				}
			*/

			break;
		case STAGE2:
			//弾が発射できる状態であればプレイヤーを操作できる
			if (!ball.isShot) {
				/*玉の発射方向*/

				//左矢印キーが押されたら左に45度傾く
				if (keys[DIK_LEFTARROW] && !preKeys[DIK_LEFTARROW]) {
					player.angle += 45.0f;
				}
				//右矢印キーが押されたら右に45度傾く
				if (keys[DIK_RIGHTARROW] && !preKeys[DIK_RIGHTARROW]) {
					player.angle -= 45.0f;
				}

				//限界角度以上傾かないようにする
				if (player.angle > 45.0f) {
					player.angle = 45.0f;
				}
				if (player.angle < -45.0f) {
					player.angle < -45.0f;
				}

				/*玉の発射処理*/

				//Spaceキーが押されたら弾を発射する
				if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {

					//弾の位置をプレイヤーの頭上に設定
					ball.pos = { player.pos.x,player.pos.y + 10 };

					//弾の速度を照準方向から設定
					//方向はdegreeからradianに変換する
					ball.velocity = { cosf(player.angle * (3.14f / 180.0f)) * ball.speed
									 ,sinf(player.angle * (3.14f / 180.0f)) * ball.speed };

					//発射フラグをtrueにする
					//これで弾が消えるまでプレイヤーを操作できなくなる
					ball.isShot = true;

				}
			}

			/*もし敵に当たったら発射フラグをfalseに

			//もし〇回玉が反射したら発射フラグをfalseに


			//全ての敵の生存フラグがfalseならステージクリア
		    /*if(enemyUP.isAlive){
		    stage2.clear = true;
		    stageclear2.flag = true;
		    }
		*/
			break;
		case STAGE3:
			//弾が発射できる状態であればプレイヤーを操作できる
			if (!ball.isShot) {
				/*玉の発射方向*/

				//左矢印キーが押されたら左に45度傾く
				if (keys[DIK_LEFTARROW] && !preKeys[DIK_LEFTARROW]) {
					player.angle += 45.0f;
				}
				//右矢印キーが押されたら右に45度傾く
				if (keys[DIK_RIGHTARROW] && !preKeys[DIK_RIGHTARROW]) {
					player.angle -= 45.0f;
				}

				//限界角度以上傾かないようにする
				if (player.angle > 45.0f) {
					player.angle = 45.0f;
				}
				if (player.angle < -45.0f) {
					player.angle < -45.0f;
				}

				/*玉の発射処理*/

				//Spaceキーが押されたら弾を発射する
				if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {

					//弾の位置をプレイヤーの頭上に設定
					ball.pos = { player.pos.x,player.pos.y + 10 };

					//弾の速度を照準方向から設定
					//方向はdegreeからradianに変換する
					ball.velocity = { cosf(player.angle * (3.14f / 180.0f)) * ball.speed
									 ,sinf(player.angle * (3.14f / 180.0f)) * ball.speed };

					//発射フラグをtrueにする
					//これで弾が消えるまでプレイヤーを操作できなくなる
					ball.isShot = true;

				}
			}

			/*もし敵に当たったら発射フラグをfalseに
			if(auto <= 00){
			ball.isshot = false;
			}

			//もし〇回玉が反射したら発射フラグをfalseに

			//全ての敵の生存フラグがfalseならステージクリア
			/*if(enemyUP.isAlive && enemyDOWN.isAlive && enemyLEFT.isAlive){
			stage3.clear = true;
			stageclear3.flag = true;
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
		case STAGE1:

			//マップチップ(プロトタイプ)
			for (int i = 0; i < 23; i++) {
				for (int j = 0; j < 40; j++) {
					if (map[i][j] == 1) {
						Novice::DrawBox(int(block.posX[j]), int(block.posY[i]), int(block.w), int(block.h), 0.0f, RED, kFillModeWireFrame);
					}
				}
			}
			/*マップ表示 真
			for(int y = 0;y < Map_H;y++){
			for(int x = 0;x < Map_W;x++){
			PrintMap(Map.x,Map.y,rad,TileNum.W,TileNum.H,gazou,color);
			}
			}
			
			*/
			//プレイヤー　プロトタイプにつき円で。
			if (player.isAlive) {
				Novice::DrawEllipse(int(player.pos.x), int(player.pos.y), int(player.radius),
					int(player.radius),0.0f, WHITE, kFillModeSolid);
			}

			//玉
			if (ball.isShot) {
				Novice::DrawEllipse(int(ball.pos.x), int(ball.pos.y),int(ball.radius), 
					int(ball.radius), 0.0f, RED, kFillModeSolid);
			}

			///敵///
		    //UP
			Novice::DrawEllipse(int(enemyUP1.pos.x), int(enemyUP1.pos.y), int(enemyUP1.radius),
				int(enemyUP1.radius),0.0f, RED, kFillModeSolid);

			//DOWN
			Novice::DrawEllipse(int(enemyDOWN1.pos.x), int(enemyDOWN1.pos.y), int(enemyDOWN1.radius), 
				int(enemyDOWN1.radius),0.0f, BLUE, kFillModeSolid);

			//LEFT
			Novice::DrawEllipse(int(enemyLEFT1.pos.x), int(enemyLEFT1.pos.y), int(enemyLEFT1.radius),
				int(enemyLEFT1.radius),0.0f, GREEN, kFillModeSolid);

			break;
		case STAGE2:
			break;
		case STAGE3:
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
