#include <Novice.h>
#include <math.h>

const char kWindowTitle[] = "爽快‼はじけ玉‼";

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

//ステージの構造体
struct Stage {
	int clear;                //クリアしたか

};
//ステージクリア演出の構造体
struct StageClear {
	Vector2 pos;
	int flag;
};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

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
		0,                  //大きさ
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

	//敵
	Enemy enemyUP{
		{0,0},      //場所
		25,         //大きさ
		0,          //生存フラグ
		0,          //ブロックの切り替えフラグ
	};

	Enemy enemyDOWN{
		{0,0},      //場所
		25,         //大きさ
		0,          //生存フラグ
		0,          //ブロックの切り替えフラグ
	};

	Enemy enemyLEFT{
		{0,0},      //場所
		25,         //大きさ
		0,          //生存フラグ
		0,          //ブロックの切り替えフラグ
	};

	Enemy enemyLIGHT{
		{0,0},      //場所
		25,         //大きさ
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
		/// 

		//玉と敵の当たり判定(仮で円同士の当たり判定でいきます)
		/*
		 auto dx = ball.pos.x - EnemyUP.pos.x;
		 auto dy = ball.pos.y - EnemyUP.pos.y;
		 auto distance = dx * dx + dy * dy;


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

			//全ての敵の生存フラグがfalseならステージクリア
				/*if(enemyUP.isAlive && enemyDOWN.isAlive && enemyLEFT.isAlive){
				stage1.clear = true;
				stageclear1.flag = true;
				}
			*/

			break;
		case STAGE2:
			//全ての敵の生存フラグがfalseならステージクリア
		    /*if(enemyUP.isAlive){
		    stage2.clear = true;
		    stageclear2.flag = true;
		    }
		*/
			break;
		case STAGE3:
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
