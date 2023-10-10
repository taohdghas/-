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
struct Enemy {
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

//リトライ演出の構造体
struct RETRY {
	Vector2 pos;
	int flag;
};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	const int Map_W = 40;
	const int Map_H = 23;
	int Map_radius = 32;
	int MapNum = 0;
	int map[Map_H][Map_W];
	FILE* fp;
	fp = fopen("test.txt", "r");
	for (int i = 0; i < Map_H; i++)for (int j = 0; j < Map_W; j++)fscanf(fp, "%d", &map[i][j]);
	fclose(fp);
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
	int ballMapNum = 0;

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

	RETRY retry{
		{0,0}, //仮
		false,
	};

	// 画面変化
	enum screen {
		TITLE,  //タイトル
		//SELECT ステージセレクト
		STAGE1,   //ステージ1
		STAGE2,
		STAGE3,
		CLEAR,   //クリア画面
		RETRY    //リトライ画面
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

		//玉と敵の当たり判定
		/*
		 auto = Lengh( ball.pos.x - EnemyUP1.pos.x, ball.pos.y - EnemyUP1.pos.y);
		 if(auto <= 00){
		 　 ball.isshot = false;
			enemyUP1.isAlive = false;
			//矢印の方向へ行く
			ball.pos.y -= ball.velocity.y
			}

		 auto = Lengh( ball.pos.x - EnemyDOWN1.pos.x, ball.pos.y - EnemyDOWN1.pos.y);
		 if(auto <= 00){
			ball.isshot = false;
			enemyDOWN1.isAlive = false;
			//矢印の方向へ行く
			ball.pos.y += ball.velocity.y
			}

		auto = Lengh( ball.pos.x - EnemyLEFT1.pos.x, ball.pos.y - EnemyLEFT1.pos.y);
		 if(auto <= 00){
			ball.isshot = false;
			enemyLEFT1.isAlive = false;
			//矢印の方向へ行く
			ball.pos.x += ball.velocity.x
			}

		 auto = Lengh( ball.pos.x - EnemyLIGHT1.pos.x, ball.pos.y - EnemyLIGHT1.pos.y);
		 if(auto <= 00){
			ball.isshot = false;
			enemyLIGHT1.isAlive = false;
			//矢印の方向へ行く
			ball.pos.x -= ball.velocity.x;
			}
		*/

		//マップチップの当たり判定

		for (int y = 0; y < Map_H; y++) {
			for (int x = 0; x < Map_W;x++) {
				MapNum = y * Map_W + x;
				ballMapNum = (ball.pos.y / ball.radius * Map_W) + (ball.pos.x / ball.radius);
				if (MapNum == kabe && ballMapNum == MapNum) {
					if (ball.velocity.x == 0) {
						ball.velocity.y *= -1;
					}
					else if (ball.velocity.y == 0){
						ball.velocity.x *= -1;
					}
					else {
						ball.velocity.x *= -1;
							ball.velocity.y *= -1;
					}
				}
				if (MapNum == naname && ballMapNum == MapNum) {
					if (ball.velocity.x == 0) {
						ball.velocity.y *= -1;
					}
					else if (ball.velocity.y == 0) {
						ball.velocity.x *= -1;
					}
					else {
						ball.velocity.x *= -1;
						ball.velocity.y *= -1;
					}
				}
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
				screenscene = STAGE1;
			}

			break;
			//case SELECT:
				//break;
		case STAGE1:
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

			//マップチップ(プロト版)

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
				ball.pos.x -= ball.velocity.x;
				ball.pos.y -= ball.velocity.y;
			}

			/*
			if (もし玉が反射可能回数に達したら) {
			//発射フラグをfalseに
			//盤面と反射可能回数をリセット
			ball.isshot = false;
			ball.HP = 5;
			enemyUP1.isAlive = true;
			enemyDOWN1.isAlive = true;
			enemyLEFT1.isAlive = true;
			}
			*/

			//全ての敵の生存フラグがfalseならステージクリア
				/*if(enemyUP.isAlive && enemyDOWN.isAlive && enemyLEFT.isAlive){
				stage1clear.flag = true;
				}

				if(stage1clear.flag){
				//ステージクリアの文字が出る
				}
				*/

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
			/*
			if (もし玉が反射可能回数に達したら) {
			//発射フラグをfalseに
			//盤面と反射可能回数をリセット
			ball.issshot = false;
			ball.HP = 5;
			}
			*/

			//全ての敵の生存フラグがfalseならステージクリア
			/*if(enemyUP.isAlive){
			stageclear2.flag = true;
			}
			if(stage2clear.flag){
				//ステージクリアの文字が出る
			}
		*/

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
			/*
			if (もし玉が反射可能回数に達したら) {
			//発射フラグをfalseに
			//盤面と反射可能回数をリセット
			ball.isshot = false;
			ball.HP = 5;
			}
			*/

			//全ての敵の生存フラグがfalseならステージクリア
			/*if(enemyUP.isAlive && enemyDOWN.isAlive && enemyLEFT.isAlive){
			stageclear3.flag = true;
			}
			if(stage3clear.flag){
				//ステージクリアの文字が出る
				}
		*/

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
		case STAGE1:

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
			Novice::DrawEllipse(int(enemyUP1.pos.x), int(enemyUP1.pos.y), int(enemyUP1.radius),
				int(enemyUP1.radius), 0.0f, RED, kFillModeSolid);

			//DOWN
			Novice::DrawEllipse(int(enemyDOWN1.pos.x), int(enemyDOWN1.pos.y), int(enemyDOWN1.radius),
				int(enemyDOWN1.radius), 0.0f, BLUE, kFillModeSolid);

			//LEFT
			Novice::DrawEllipse(int(enemyLEFT1.pos.x), int(enemyLEFT1.pos.y), int(enemyLEFT1.radius),
				int(enemyLEFT1.radius), 0.0f, GREEN, kFillModeSolid);

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
