#pragma once
//-----------------------------------------------------------------------
// File : fixed_point.h
// Desc : 固定小数点を扱うクラス
//-----------------------------------------------------------------------

#include <assert.h>
#include <iostream>

template<typename FIX>
class fixed_pt
{
    private:
	FIX value;	// 固定小数点の値
	int INT;	// 整数部のビット数（符号ビット含む）
	int DEC;	// 小数部のビット数
	static const int BitNum = sizeof(FIX) * 8;	// 全ビット数

	FIX one() const { return (static_cast<FIX>(1) << DEC); } // 固定小数点における1

	// 整数部と小数部のビット数がFIXのビット幅を超えてないかチェック
	void CheckBitNum() const {
		if (BitNum < (INT + DEC)) {
			std::cout << "整数部と小数部のビット数の和が"<<BitNum<<"ビットを超えています" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	// 指定した整数部のビット数を超える分を溢れさせて消す
	FIX INTsft(FIX x) const {
		return ((x << (BitNum - INT - DEC)) >> (BitNum - INT - DEC));
	}
	void INTsft() {
		value = (value << (BitNum - INT - DEC)) >> (BitNum - INT - DEC);
	}

	// 固定小数点数へ変換する
	static FIX double_to_fp(const double the_double, const int Int, const int Dec) {
		if (BitNum < (Int + Dec)) {
			std::cout << "整数部と小数部のビット数の和が" << BitNum << "ビットを超えています" << std::endl;
			exit(EXIT_FAILURE);
		}
		FIX result = static_cast<FIX>(the_double * static_cast<double>(1 << Dec));
		result = ((result << (BitNum - Int - Dec)) >> (BitNum - Int - Dec));
	    return result;
	}
	FIX double_to_fp(const double the_double) const {
		FIX result = static_cast<FIX>(the_double * static_cast<double>(one()));
		result = INTsft(result);
		return result;
	}

	// 整数型からfixed_ptを生成するコンストラクタ
	// friendの内部変換に使用する
	fixed_pt(const FIX i_value, const int Int, const int Dec) {
		value = i_value;
		INT = Int;
		DEC = Dec;
		CheckBitNum();
		INTsft();
	}

    public:
	// コンストラクタ
	fixed_pt() : value(0), INT(BitNum), DEC(0) {}
	fixed_pt(const fixed_pt& other_fixed_pt) : value(other_fixed_pt.value), INT(other_fixed_pt.INT), DEC(other_fixed_pt.DEC) {}
	fixed_pt(const double init_real, const int Int, const int Dec) : value(double_to_fp(init_real, Int, Dec)), INT(Int), DEC(Dec) {}

	// デストラクタ
	~fixed_pt() {}

	// 数値を設定する関数
	void set(const double real) {
	    value = double_to_fp(real);
	}
	void setbit(const int Int, const int Dec) {
		if (Dec >= 0) { 
			if (DEC > Dec) value = value >> (DEC - Dec);
			else value = value << (Dec - DEC);
			DEC = Dec; 
		}
		if (Int >= 0) {
			INT = Int;
			CheckBitNum();
			INTsft();
		}
		CheckBitNum();
	}

	// 数値を取得する関数
	FIX get() const { return value; }
	int getINT() const { return INT; }
	int getDEC() const { return DEC; }
	double getd() const {
	    return (static_cast<double>(value) / (one()));
	}

	// 小数部の切り取り
	fixed_pt DecPart() const
	{
		int sign;
		if (value < 0) sign = -1;
		else sign = 1;
		FIX Abs = value * sign;
		int temp = BitNum - DEC;
		Abs = (Abs << temp) >> temp;
		return fixed_pt((Abs * sign), INT, DEC);
	}

	// 整数部の切り取り
	FIX IntPart() const
	{
		int sign;
		if (value < 0) sign = -1;
		else sign = 1;
		FIX Abs = value * sign;
		Abs = Abs >> DEC;
		return (Abs * sign);
	}

	// 代入演算子のオーバーロード
	fixed_pt& operator = (const fixed_pt& oper2) {
		if ((INT != oper2.INT) || (DEC != oper2.DEC)) {
			std::cerr << "ビット幅が一致していません" << std::endl;
			exit(EXIT_FAILURE);
		}
		value = oper2.value;
		return (*this);
	}

	// friend宣言
	template<typename FIX> friend fixed_pt<FIX> operator + (const fixed_pt<FIX>& oper1, const fixed_pt<FIX>& oper2);
	template<typename FIX> friend fixed_pt<FIX> operator - (const fixed_pt<FIX>& oper1, const fixed_pt<FIX>& oper2);
	template<typename FIX> friend fixed_pt<FIX> operator * (const fixed_pt<FIX>& oper1, const fixed_pt<FIX>& oper2);

	template<typename FIX> friend fixed_pt<FIX> operator << (const fixed_pt<FIX>& oper1, const int oper2);
	template<typename FIX> friend fixed_pt<FIX> operator >> (const fixed_pt<FIX>& oper1, const int oper2);

	template<typename FIX> friend bool operator == (const fixed_pt<FIX>& oper1, const fixed_pt<FIX>& oper2);
	template<typename FIX> friend bool operator > (const fixed_pt<FIX>& oper1, const fixed_pt<FIX>& oper2);
	template<typename FIX> friend bool operator >= (const fixed_pt<FIX>& oper1, const fixed_pt<FIX>& oper2);
	template<typename FIX> friend bool operator < (const fixed_pt<FIX>& oper1, const fixed_pt<FIX>& oper2);
	template<typename FIX> friend bool operator <= (const fixed_pt<FIX>& oper1, const fixed_pt<FIX>& oper2);

	template<typename FIX> friend fixed_pt<FIX> operator - (const fixed_pt<FIX>& oper1);
};


// 算術演算子のオーバーロード
// 加減算では大きいほうのビット幅+1に合わせられる
template<typename FIX>
inline fixed_pt<FIX> operator + (const fixed_pt<FIX>& oper1, const fixed_pt<FIX>& oper2)
{
	fixed_pt<FIX> temp1(oper1), temp2(oper2);
	int Int = ((oper1.INT > oper2.INT) ? oper1.INT+1 : oper2.INT+1), Dec;
	if (oper1.DEC > oper2.DEC) { temp2.setbit(-1, oper1.DEC); Dec = oper1.DEC; }
	else { temp1.setbit(-1, oper2.DEC); Dec = oper2.DEC; }
	return fixed_pt<FIX>((temp1.value + temp2.value), Int, Dec);
}

template<typename FIX>
inline fixed_pt<FIX> operator - (const fixed_pt<FIX>& oper1, const fixed_pt<FIX>& oper2)
{
	fixed_pt<FIX> temp1(oper1), temp2(oper2);
	int Int = ((oper1.INT > oper2.INT) ? oper1.INT+1 : oper2.INT+1), Dec;
	if (oper1.DEC > oper2.DEC) { temp2.setbit(-1, oper1.DEC); Dec = oper1.DEC; }
	else { temp1.setbit(-1, oper2.DEC); Dec = oper2.DEC; }
	return fixed_pt<FIX>((temp1.value - temp2.value), Int, Dec);
}

// 積のビット幅は２つの値のビット幅の和となる
template<typename FIX>
inline fixed_pt<FIX> operator * (const fixed_pt<FIX>& oper1, const fixed_pt<FIX>& oper2)
{
	return fixed_pt<FIX>((oper1.value * oper2.value), (oper1.INT + oper2.INT), (oper1.DEC + oper2.DEC));
}

// 割り算はビット幅の処理がよくわからないので未実装


// シフト演算のオーバーロード
template<typename FIX>
inline fixed_pt<FIX> operator << (const fixed_pt<FIX>& oper1, const int oper2)
{
	return fixed_pt<FIX>((oper1.value << oper2), oper1.INT, oper1.DEC);
}

template<typename FIX>
inline fixed_pt<FIX> operator >> (const fixed_pt<FIX>& oper1, const int oper2)
{
	return fixed_pt<FIX>((oper1.value >> oper2), oper1.INT, oper1.DEC);
}


// 比較演算子のオーバーロード
template<typename FIX>
inline bool operator == (const fixed_pt<FIX>& oper1, const fixed_pt<FIX>& oper2)
{
	return ((oper1.value == oper2.value) && (oper1.INT == oper2.INT) && (oper1.DEC == oper2.DEC));
}

template<typename FIX>
inline bool operator != (const fixed_pt<FIX>& oper1, const fixed_pt<FIX>& oper2)
{
    return (!(oper1 == oper2));
}

template<typename FIX>
inline bool operator > (const fixed_pt<FIX>& oper1, const fixed_pt<FIX>& oper2)
{
	assert((oper1.INT == oper2.INT) && (oper1.DEC == oper2.DEC));
	return (oper1.value > oper2.value);
}

template<typename FIX>
inline bool operator >= (const fixed_pt<FIX>& oper1, const fixed_pt<FIX>& oper2)
{
	assert((oper1.INT == oper2.INT) && (oper1.DEC == oper2.DEC));
	return (oper1.value >= oper2.value);
}

template<typename FIX>
inline bool operator < (const fixed_pt<FIX>& oper1, const fixed_pt<FIX>& oper2)
{
	assert((oper1.INT == oper2.INT) && (oper1.DEC == oper2.DEC));
	return (oper1.value < oper2.value);
}

template<typename FIX>
inline bool operator <= (const fixed_pt<FIX>& oper1, const fixed_pt<FIX>& oper2)
{
	assert((oper1.INT == oper2.INT) && (oper1.DEC == oper2.DEC));
	return (oper1.value <= oper2.value);
}

// 単項マイナス
template<typename FIX>
inline fixed_pt<FIX> operator - (const fixed_pt<FIX>& oper1)
{
    return fixed_pt<FIX>(-oper1.value, oper1.INT, oper1.DEC);
}
