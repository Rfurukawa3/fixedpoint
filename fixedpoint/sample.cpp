// fixed_point.h�p�̃T���v���t�@�C��

#include "fixed_point.h"
#include <iostream>

using namespace std;

int main() {
	const int SFT = 12;

	// fixed_pt<�f�[�^�^, �r�b�g�V�t�g��>�Ő錾����
	// �f�[�^�^�͑S�̂̃r�b�g�������߂�i��Fint�^�Ȃ�32�r�b�g�j
	// �r�b�g�V�t�g�ʂ͏������̃r�b�g�������߂�D
	fixed_pt<int, SFT> a;

	// set()�Œl��ݒ�
	a.set(3.14);

	// get()�Œl��double�Ŏ擾
	cout << "a = " << a.get() << endl;

	// �錾�Ɠ����ɒl��ݒ肷�邱�Ƃ��ł���D
	fixed_pt<int, SFT> b(5.55);

	// �l�����Z�͂����ʂ�
	// �|���Z�Ɗ���Z�̓I�[�o�[�t���[���₷���̂Œ���
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