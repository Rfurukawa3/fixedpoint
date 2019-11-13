// fixed_point.h用のサンプルファイル

#include "fixed_point.h"
#include <iostream>

using namespace std;

int main() {
	const int SFT = 12;

	// fixed_pt<データ型, ビットシフト量>で宣言する
	// データ型は全体のビット数を決める（例：int型なら32ビット）
	// ビットシフト量は小数部のビット数を決める．
	fixed_pt<int, SFT> a;

	// set()で値を設定
	a.set(3.14);

	// get()で値をdoubleで取得
	cout << "a = " << a.get() << endl;

	// 宣言と同時に値を設定することもできる．
	fixed_pt<int, SFT> b(5.55);

	// 四則演算はいつも通り
	// 掛け算と割り算はオーバーフローしやすいので注意
	fixed_pt<int, SFT> c;
	c = a + b;
	cout << "3.14 + 5.55 = " << c.get() << endl;
	c = a - b;
	cout << "3.14 - 5.55 = " << c.get() << endl;
	c = a * b;
	cout << "3.14 * 5.55 = " << c.get() << endl;
	c = a / b;
	cout << "3.14 / 5.55 = " << c.get() << endl;

	return 0;
}