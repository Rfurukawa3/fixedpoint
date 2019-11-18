// fixed_point.h�p�̃T���v���t�@�C��

#include "fixed_point.h"
#include <iostream>

using namespace std;

int main() {
	// fixed_pt<�f�[�^�^>�Ő錾����
	// �f�[�^�^�͑S�̂̃r�b�g�������߂�i��Fint�^�Ȃ�32�r�b�g�j
	// �ϐ���(�l�C�������r�b�g���C�������r�b�g���j�Œ�`
	fixed_pt<long long> a(1.1, 9, 13), b(5.55, 12, 12);

	// set(�l)�Œl��ݒ�
	a.set(3.14);

	// setbit(�������r�b�g���C�������r�b�g��)�Ńr�b�g����ݒ�
	a.setbit(12, 12);

	// getd()�Œl��double�Ŏ擾
	cout << "a = " << a.getd() << endl;

	// �����Z�C�����Z�C�|���Z�͂����ʂ�
	// ����Z�͂ł��Ȃ�
	fixed_pt<long long> c(0.0, 12, 12);
	c = a + b;
	cout << "3.14 + 5.55 = " << c.getd() << endl;
	c = a - b;
	cout << "3.14 - 5.55 = " << c.getd() << endl;

	// �|���Z�ł̓r�b�g����������̂Œ���
	// ������Z�q�͍��E�̃r�b�g������v���Ă��Ȃ��ƃG���[��f���̂ł����Ɛݒ肷��
	c.setbit(24, 24);
	c = a * b;
	cout << "3.14 * 5.55 = " << c.getd() << endl;


	return 0; 
}