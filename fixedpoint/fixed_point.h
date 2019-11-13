#ifndef __fixed_point_h__	// Avoid double includes
#define __fixed_point_h__	// Prevent double include

//-----------------------------------------------------------------------
// File : fixed_point.h
// Desc : 固定小数点を扱うクラス
//-----------------------------------------------------------------------

#include <assert.h>

template<typename FIX, int SFT>
class fixed_pt
{
    private:
	FIX value;	// 固定小数点の値

	static const int BitNum = sizeof(FIX) * 8;	// 全ビット数
	static const FIX fixed_exp = 1 << SFT;		// 2のSFT乗, 固定小数点における1
	static const FIX mask = fixed_exp - static_cast<FIX>(1); // 整数部全て0，小数部全て1のマスク

	static FIX double_to_fp(const double the_double) {
	    return (static_cast<FIX>(the_double * static_cast<double>(fixed_exp)));
	}

	static FIX int_to_fp(const int the_int) {
	    return (static_cast<FIX>(the_int) << SFT);
	}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           


    public:
	// デフォルトコンストラクタ
	fixed_pt(): value(0) { }

	// コピーコンストラクタ
	fixed_pt(const fixed_pt& other_fixed_pt) :
	    value(other_fixed_pt.value)
	{}
	    
	// doubleからfixed_ptを生成
	fixed_pt(const double init_real) :
	    value(double_to_fp(init_real))
	{}

	// デストラクタは何もしない
	~fixed_pt() {}

	// 数値を設定する関数
	void set(const double real) {
	    value = double_to_fp(real);
	}

	// 数値を取得する関数
	double get() const {
	    return (static_cast<double>(value) / fixed_exp);
	}

	// intで数値を取得する関数
	int geti() const {
		return static_cast<int>(value >> SFT);
	}

	// 小数部の切り取り
	// 注意:intより小さい型はintにキャストされるので上手く動作しない
	//      左シフトは0埋め、右シフトは符号ビット埋めとする環境を前提としている
	fixed_pt DecPart()
	{
		FIX All = value >> (BitNum-1);	// 正数なら全部0, 負数なら全部1
		FIX sign = (All & static_cast<FIX>(-2)) + static_cast<FIX>(1);	// 正数なら1, 負数なら-1
		FIX Abs = (value - (All & static_cast<FIX>(1))) ^ All;	// いったん正の数にする
		return fixed_pt((Abs & mask) * sign);
	}

	// 整数部の切り取り
	// 注意:同上
	fixed_pt IntPart()
	{
		FIX All = value >> (BitNum-1);	// 正数なら全部0, 負数なら全部1
		FIX sign = (All & static_cast<FIX>(-2)) + static_cast<FIX>(1);	// 正数なら1, 負数なら-1
		FIX Abs = (value - (All & static_cast<FIX>(1))) ^ All;	// いったん正の数にする
		return fixed_pt((Abs & (~mask)) * sign);
	}


	// 代入演算子のオーバーロード
	fixed_pt& operator = (const fixed_pt& oper2) {
	   value = oper2.value;
	   return (*this);
	}

	// 複合演算子のオーバーロード
#pragma region Composite operator
	// 足し算
	fixed_pt& operator += (const fixed_pt& oper2) {
	   value += oper2.value;
	   return (*this);
	}

	fixed_pt& operator += (const double oper2) {
	   value += double_to_fp(oper2);
	   return (*this);
	}

	fixed_pt& operator += (const int oper2) {
	   value += int_to_fp(oper2);
	   return (*this);
	}

	// 引き算
	fixed_pt& operator -= (const fixed_pt& oper2) {
	   value -= oper2.value;
	   return (*this);
	}

	fixed_pt& operator -= (const double oper2) {
	   value -= double_to_fp(oper2);
	   return (*this);
	}

	fixed_pt& operator -= (const int oper2) {
	   value -= int_to_fp(oper2);
	   return (*this);
	}

	// 掛け算
	fixed_pt& operator *= (const fixed_pt& oper2) {
	   value *= oper2.value;
	   value >>= SFT;
	   return (*this); 
	}

	fixed_pt& operator *= (const double oper2) {
	   value *= double_to_fp(oper2);
	   value >>= SFT;
	   return (*this);
	}

	fixed_pt& operator *= (const int oper2) {
	   value *= static_cast<FIX>(oper2);
	   return (*this);
	}

	// 割り算
	fixed_pt& operator /= (const fixed_pt& oper2) {
	    assert(oper2.value != 0.0);
		value <<= SFT;
	    value /= oper2.value;
		return (*this);
	}

	fixed_pt& operator /= (const double oper2) {
	    assert(double_to_fp(oper2) != 0.0);
		value <<= SFT;
	    value /= double_to_fp(oper2);
	    return (*this);
	}

	fixed_pt& operator /= (const int oper2) {
	    assert(oper2 != 0);
	    value /= static_cast<FIX>(oper2);
	    return (*this);
	}
#pragma endregion

	// インクリメント・デクリメントのオーバーロード
#pragma region Crement
	// f++
	fixed_pt operator ++(int) {
	   fixed_pt result(*this);
	   value += fixed_exp;
	   return (result);
	}

	// ++f
	fixed_pt& operator ++() {
	   value += fixed_exp;
	   return (*this);
	}

	// f--
	fixed_pt operator --(int) {
	   fixed_pt result(*this);
	   value -= fixed_exp;
	   return (result);
	}

	// --f
	fixed_pt& operator --() {
	   value -= fixed_exp;
	   return (*this);
	}
#pragma endregion

    private:
	// 整数型からfixed_ptを生成するコンストラクタ
	// friendの内部変換に使用する
	fixed_pt(const FIX i_value) : value(i_value){}

	// friend宣言
	template<typename FIX, int SFT> friend fixed_pt<FIX, SFT> operator + (const fixed_pt<FIX, SFT>& oper1, const fixed_pt<FIX, SFT>& oper2);
	template<typename FIX, int SFT> friend fixed_pt<FIX, SFT> operator + (const fixed_pt<FIX, SFT>& oper1, const double oper2);
	template<typename FIX, int SFT> friend fixed_pt<FIX, SFT> operator + (const double oper1, const fixed_pt<FIX, SFT>& oper2);
	template<typename FIX, int SFT> friend fixed_pt<FIX, SFT> operator + (const fixed_pt<FIX, SFT>& oper1, const int oper2);
	template<typename FIX, int SFT> friend fixed_pt<FIX, SFT> operator + (const int oper1, const fixed_pt<FIX, SFT>& oper2);

	template<typename FIX, int SFT> friend fixed_pt<FIX, SFT> operator - (const fixed_pt<FIX, SFT>& oper1, const fixed_pt<FIX, SFT>& oper2);
	template<typename FIX, int SFT> friend fixed_pt<FIX, SFT> operator - (const fixed_pt<FIX, SFT>& oper1, const double oper2);
	template<typename FIX, int SFT> friend fixed_pt<FIX, SFT> operator - (const double oper1, const fixed_pt<FIX, SFT>& oper2);
	template<typename FIX, int SFT> friend fixed_pt<FIX, SFT> operator - (const fixed_pt<FIX, SFT>& oper1, const int oper2);
	template<typename FIX, int SFT> friend fixed_pt<FIX, SFT> operator - (const int oper1, const fixed_pt<FIX, SFT>& oper2);

	template<typename FIX, int SFT> friend fixed_pt<FIX, SFT> operator * (const fixed_pt<FIX, SFT>& oper1, const fixed_pt<FIX, SFT>& oper2);
	template<typename FIX, int SFT> friend fixed_pt<FIX, SFT> operator * (const fixed_pt<FIX, SFT>& oper1, const double oper2);
	template<typename FIX, int SFT> friend fixed_pt<FIX, SFT> operator * (const double oper1, const fixed_pt<FIX, SFT>& oper2);
	template<typename FIX, int SFT> friend fixed_pt<FIX, SFT> operator * (const fixed_pt<FIX, SFT>& oper1, const int oper2);
	template<typename FIX, int SFT> friend fixed_pt<FIX, SFT> operator * (const int oper1, const fixed_pt<FIX, SFT>& oper2);

	template<typename FIX, int SFT> friend fixed_pt<FIX, SFT> operator / (const fixed_pt<FIX, SFT>& oper1, const fixed_pt<FIX, SFT>& oper2);
	template<typename FIX, int SFT> friend fixed_pt<FIX, SFT> operator / (const fixed_pt<FIX, SFT>& oper1, const double oper2);
	template<typename FIX, int SFT> friend fixed_pt<FIX, SFT> operator / (const double oper1, const fixed_pt<FIX, SFT>& oper2);
	template<typename FIX, int SFT> friend fixed_pt<FIX, SFT> operator / (const fixed_pt<FIX, SFT>& oper1, const int oper2);
	template<typename FIX, int SFT> friend fixed_pt<FIX, SFT> operator / (const int oper1, const fixed_pt<FIX, SFT>& oper2);

	template<typename FIX, int SFT> friend bool operator == (const fixed_pt<FIX, SFT>& oper1, const fixed_pt<FIX, SFT>& oper2);
	template<typename FIX, int SFT> friend bool operator > (const fixed_pt<FIX, SFT>& oper1, const fixed_pt<FIX, SFT>& oper2);
	template<typename FIX, int SFT> friend bool operator >= (const fixed_pt<FIX, SFT>& oper1, const fixed_pt<FIX, SFT>& oper2);
	template<typename FIX, int SFT> friend bool operator < (const fixed_pt<FIX, SFT>& oper1, const fixed_pt<FIX, SFT>& oper2);
	template<typename FIX, int SFT> friend bool operator <= (const fixed_pt<FIX, SFT>& oper1, const fixed_pt<FIX, SFT>& oper2);

	template<typename FIX, int SFT> friend fixed_pt<FIX, SFT> operator - (const fixed_pt<FIX, SFT>& oper1);
};

// 算術演算子のオーバーロード
#pragma region Arithmetic operators
// 足し算
template<typename FIX, int SFT>
inline fixed_pt<FIX, SFT> operator + (const fixed_pt<FIX, SFT>& oper1, const fixed_pt<FIX, SFT>& oper2)
{
	return fixed_pt<FIX, SFT>(oper1.value + oper2.value);
}

template<typename FIX, int SFT>
inline fixed_pt<FIX, SFT> operator + (const fixed_pt<FIX, SFT>& oper1, const double oper2)
{
  return fixed_pt<FIX, SFT>(oper1.value + fixed_pt<FIX, SFT>::double_to_fp(oper2));
}

template<typename FIX, int SFT>
inline fixed_pt<FIX, SFT> operator + (const double oper1, const fixed_pt<FIX, SFT>& oper2)
{
  return fixed_pt<FIX, SFT>(fixed_pt<FIX, SFT>::double_to_fp(oper1) + oper2.value);
}

template<typename FIX, int SFT>
inline fixed_pt<FIX, SFT> operator + (const fixed_pt<FIX, SFT>& oper1, const int oper2)
{
  return fixed_pt<FIX, SFT>(oper1.value + fixed_pt<FIX, SFT>::int_to_fp(oper2));
}

template<typename FIX, int SFT>
inline fixed_pt<FIX, SFT> operator + (const int oper1, const fixed_pt<FIX, SFT>& oper2)
{
  return fixed_pt<FIX, SFT>(fixed_pt<FIX, SFT>::int_to_fp(oper1) + oper2.value);
}

// 引き算
template<typename FIX, int SFT>
inline fixed_pt<FIX, SFT> operator - (const fixed_pt<FIX, SFT>& oper1, const fixed_pt<FIX, SFT>& oper2)
{
  return fixed_pt<FIX, SFT>(oper1.value - oper2.value);
}

template<typename FIX, int SFT>
inline fixed_pt<FIX, SFT> operator - (const fixed_pt<FIX, SFT>& oper1, const double oper2)
{
  return fixed_pt<FIX, SFT>(oper1.value - fixed_pt<FIX, SFT>::double_to_fp(oper2));
}

template<typename FIX, int SFT>
inline fixed_pt<FIX, SFT> operator - (double oper1, const fixed_pt<FIX, SFT>& oper2)
{
  return fixed_pt<FIX, SFT>(fixed_pt<FIX, SFT>::double_to_fp(oper1) - oper2.value);
}

template<typename FIX, int SFT>
inline fixed_pt<FIX, SFT> operator - (const fixed_pt<FIX, SFT>& oper1, const int oper2)
{
  return fixed_pt<FIX, SFT>(oper1.value - fixed_pt<FIX, SFT>::int_to_fp(oper2));
}

template<typename FIX, int SFT>
inline fixed_pt<FIX, SFT> operator - (const int oper1, const fixed_pt<FIX, SFT>& oper2)
{
  return fixed_pt<FIX, SFT>(fixed_pt<FIX, SFT>::int_to_fp(oper1) - oper2.value);
}

// 掛け算
template<typename FIX, int SFT>
inline fixed_pt<FIX, SFT> operator * (const fixed_pt<FIX, SFT>& oper1, const fixed_pt<FIX, SFT>& oper2)
{
    return fixed_pt<FIX, SFT>((oper1.value * oper2.value) >> SFT);
}

template<typename FIX, int SFT>
inline fixed_pt<FIX, SFT> operator * (const fixed_pt<FIX, SFT>& oper1, const double oper2)
{
    return fixed_pt<FIX, SFT>((oper1.value * fixed_pt<FIX, SFT>::double_to_fp(oper2)) >> SFT);
}

template<typename FIX, int SFT>
inline fixed_pt<FIX, SFT> operator * (const double oper1, const fixed_pt<FIX, SFT>& oper2)
{
    return fixed_pt<FIX, SFT>((fixed_pt<FIX, SFT>::double_to_fp(oper1) * oper2.value) >> SFT);
}

template<typename FIX, int SFT>
inline fixed_pt<FIX, SFT> operator * (const fixed_pt<FIX, SFT>& oper1, const int oper2)
{
  return fixed_pt<FIX, SFT>(oper1.value * static_cast<FIX>(oper2));
}

template<typename FIX, int SFT>
inline fixed_pt<FIX, SFT> operator * (const int oper1, const fixed_pt<FIX, SFT>& oper2)
{
  return fixed_pt<FIX, SFT>(static_cast<FIX>(oper1) * oper2.value);
}

// 割り算
template<typename FIX, int SFT>
inline fixed_pt<FIX, SFT> operator / (const fixed_pt<FIX, SFT>& oper1, const fixed_pt<FIX, SFT>& oper2)
{
    assert(oper2.value != 0);
    return fixed_pt<FIX, SFT>((oper1.value << SFT) / oper2.value);
}

template<typename FIX, int SFT>
inline fixed_pt<FIX, SFT> operator / (const double oper1, const fixed_pt<FIX, SFT>& oper2)
{
    assert(oper2.value != 0);
    return fixed_pt<FIX, SFT>((fixed_pt<FIX, SFT>::double_to_fp(oper1) << SFT) / oper2.value);
}

template<typename FIX, int SFT>
inline fixed_pt<FIX, SFT> operator / (const fixed_pt<FIX, SFT>& oper1, const double oper2)
{
    assert(oper2 != 0.0);
    return fixed_pt<FIX, SFT>((oper1.value << SFT) / fixed_pt<FIX, SFT>::double_to_fp(oper2));
}

template<typename FIX, int SFT>
inline fixed_pt<FIX, SFT> operator / (const fixed_pt<FIX, SFT>& oper1, const int oper2)
{
	assert(oper2 != 0);
	return fixed_pt<FIX, SFT>(oper1.value / static_cast<FIX>(oper2));
}

template<typename FIX, int SFT>
inline fixed_pt<FIX, SFT> operator / (const int oper1, const fixed_pt<FIX, SFT>& oper2)
{
	assert(oper2.value != 0);
	return fixed_pt<FIX, SFT>((fixed_pt<FIX, SFT>::int_to_fp(oper1) << SFT) / oper2.value);
}
#pragma endregion

// 比較演算子
#pragma region Comparison operator	
template<typename FIX, int SFT>
inline bool operator == (const fixed_pt<FIX, SFT>& oper1, const fixed_pt<FIX, SFT>& oper2)
{
    return (oper1.value == oper2.value);
}

template<typename FIX, int SFT>
inline bool operator != (const fixed_pt<FIX, SFT>& oper1, const fixed_pt<FIX, SFT>& oper2)
{
    return (!(oper1 == oper2));
}

template<typename FIX, int SFT>
inline bool operator > (const fixed_pt<FIX, SFT>& oper1, const fixed_pt<FIX, SFT>& oper2)
{
	return (oper1.value > oper2.value);
}

template<typename FIX, int SFT>
inline bool operator >= (const fixed_pt<FIX, SFT>& oper1, const fixed_pt<FIX, SFT>& oper2)
{
	return (oper1.value >= oper2.value);
}

template<typename FIX, int SFT>
inline bool operator < (const fixed_pt<FIX, SFT>& oper1, const fixed_pt<FIX, SFT>& oper2)
{
	return (oper1.value < oper2.value);
}

template<typename FIX, int SFT>
inline bool operator <= (const fixed_pt<FIX, SFT>& oper1, const fixed_pt<FIX, SFT>& oper2)
{
	return (oper1.value <= oper2.value);
}
#pragma endregion

// 単項マイナス
template<typename FIX, int SFT>
inline fixed_pt<FIX, SFT> operator - (const fixed_pt<FIX, SFT>& oper1)
{
    return fixed_pt<FIX, SFT>(-oper1.value);
}

#endif /* __fixed_point_h__ */	// Avoid double includes