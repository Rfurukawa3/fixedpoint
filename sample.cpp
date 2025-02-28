// fixed_point.h用のサンプルファイル

#include "fixed_point.h"
#include <iostream>

using namespace std;

int main() {
	// fixed_pt<データ型>で宣言する
	// データ型は全体のビット数を決める（例：int型なら32ビット）
	// 変数名(値，整数部ビット数，小数部ビット数）で定義
	fixed_pt<long long> a(1.1, 9, 13), b(5.55, 10, 12);

	// set(値)で値を設定
	a.set(3.14);

	// setbit(整数部ビット数，小数部ビット数)でビット幅を設定
	a.setbit(12, 10);

	// getd()で値をdoubleで取得
	cout << "a = " << a.getd() << endl;

	// 足し算，引き算，掛け算はいつも通り
	// 割り算はできない
	fixed_pt<long long> c(0.0, 13, 12); // 加減算ではビット幅が大きいほうに合わせられる（整数部は１bit増える）
	c = a + b;
	cout << "3.14 + 5.55 = " << c.getd() << endl;
	c = a - b;
	cout << "3.14 - 5.55 = " << c.getd() << endl;

	// 掛け算ではビット数は２つの数の和になる
	// 代入演算子は左右のビット幅が一致していないとエラーを吐くのでちゃんと設定する
	c.setbit(22, 22);
	c = a * b;
	cout << "3.14 * 5.55 = " << c.getd() << endl;


	return 0; 
}