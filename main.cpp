#include <Novice.h>

const char kWindowTitle[] = "チーム制作";

struct Vector2 {	// ベクトル
	float x;
	float y;
};
struct Size2 {
	const int width;          //幅
	const int height;         //高さ
};

//プレイヤー構造体
struct Player {
	Vector2 pos;              //場所
	float radius;             //大きさ
	unsigned int color;       //色
	int isthere;              //生きているか
	int hp;                   //HP
};

struct Ball {
	Vector2 pos;              //場所
	Vector2 velocity;         //速度
	float radius;             //大きさ
	unsigned int color;       //色
	int isshot;               //生きているか
	int numberofball;         //玉の数
};

struct Enemy {
	Vector2 pos;              //場所
	float radius;             //大きさ
	int isthere;              //生きているか
	int changeblock;          //ブロックの切り替えフラグ
};

struct Stage {
	int number;              //ステージ番号
	int clear;              //クリアしたか

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
		true,               //生きているか
	};

	//玉
	Ball ball{
		{player.pos.x,player.pos.y + 10}, //場所
		{0,0},                            //速度 仮で0
		10,                               //大きさ 仮
		0xffffffff,                       //色 仮
		false,                            //
		3,
	};

	//敵
	Enemy enemyUP{
		{0,0},
		25,
		0,
		0,
	};

	Enemy enemyDOWN{
		{0,0},
		25,
		0,
		0,
	};

	Enemy enemyLEFT{
		{0,0},
		25,
		0,
		0,
	};

	Enemy enemyLIGHT{
		{0,0},
		25,
		0,
		0,
	};

	// 画面変化
	int screenscene = 0;
	enum screen {
		start,  //0
		seen1,  //1
		clear   //2
	};
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
		
		//玉の発射処理

		switch(screenscene){
		case start:
			break;
		case seen1:
			break;
		case clear:
		break;
		}
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

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
