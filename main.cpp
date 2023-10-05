#include <Novice.h>

const char kWindowTitle[] = "チーム制作";

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
	int isthere;              //生存フラグ
	int stock;                //残機
};

//弾の構造体
struct Ball {
	Vector2 pos;              //場所
	Vector2 velocity;         //速度
	float radius;             //大きさ
	unsigned int color;       //色
	int isshot;               //発射フラグ
	int HP;                   //反射に耐えられる回数
};

//敵の構造体
struct Enemy {
	Vector2 pos;              //場所
	float radius;             //大きさ
	int isthere;              //生存フラグ
	int changeblock;          //ブロックの切り替えフラグ
};

//ステージの構造体
struct Stage {
	int clear;                //クリアしたか

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

	// 画面変化
	enum screen {
		TITLE,  //タイトル
		//SEKECT ステージセレクト
		PLAY,   //ゲーム画面
		CLEAR   //クリア画面
	};

	screen screenscene = PLAY; //表示されるシーン

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

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

		switch(screenscene){
		case TITLE:
			//spaceキー押されたらセレクト画面に移行(未定)
			/*if (keys[DIK_SPACE]) {
			   screenscene = SELECT;
			};
			*/

			//spaceキー押されたらステージ画面へ移行
			if (keys[DIK_SPACE]) {
				screenscene = PLAY;
			}

			break;
		//case SELECT:
			//break;
		case PLAY:

			//弾が発射できる状態であればプレイヤーを操作できる
			if (!ball.isshot) {
				//玉の発射方向
				if(keys[DIK_LEFTARROW] && !preKeys[DIK_LEFTARROW]){
				}
				//玉の発射処理
				//if (keys[DIK_SPACE] && ball.isshot) {}

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
			break;
		case PLAY:
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
