#include <iostream>
#include "BigInt.h"
#include <string>
using namespace std;

int main()
{
	BigInt *bintA = 0;//A
	BigInt *bintB = 0;//B
	BigInt *bintS = 0;//��
	string addA, addB;
	while (true)
	{
		int selection;
		//�˵�
		cout << "��������Ҫִ�еĲ���" << endl;
		cout << "1.���мӷ�����" << endl;
		cout << "2.�˳�" << endl;
		cout << "��ѡ��";
		//Ҫ������
		cin >> selection;
		if (selection == 2)//ѡ2�˳�
		{
			break;
		}
		else if (selection == 1)//ѡ1��ʼ����
		{
			bool asent = false, bsent = false;//asent��bsent��ʾA��B�Ƿ���ȷ��ֵ�����û�У����ظ���������ֱ������ȷ��ֵΪֹ
			while (!asent)//�·�bsent��֮ͬ��ֻ�Ǵ���addB�õģ���˲����ظ���ע����
			{
				cout << "������A" << endl;
				cin >> addA;
				if (!StringCheck(addA))//����ʱ���һ�¸�ʽ
				{
					cout << "�����ʽ�ƺ������⡭��" << endl;
					addA = StringCull(addA);//�������ַ��������޳�
					cout << "������Ϊ" << StringFix(addA) << "�����Ƿ�ͬ�⣿(Y/N)" << endl;//��ѯ�Ƿ��ս�����޸ķ��������Զ��޸�
					char c;
					cin >> c;
					c = toupper(c);//�ų���Сд������ɵ�Ӱ��
					if (c == 'Y')//ͬ�������
					{
						BIntAssign(bintA, StringCull(addA));//��ֵ
						asent = true;
					}
					else//��ͬ����Ҫ����������
					{
						cout << "�ܾ���������ʧ�ܣ�����������" << endl;
					}
				}
				else
				{
					BIntAssign(bintA, StringCull(addA));//�����ʽ��ȷ��ֱ�Ӹ�ֵ����ѯ��
					asent = true;//A���ɹ���ֵ
				}
			}
			while (!bsent)
			{
				cout << "������B" << endl;
				cin >> addB;
				if (!StringCheck(addB))
				{
					cout << "�����ʽ�ƺ������⡭��" << endl;
					addB = StringCull(addB);
					cout << "������Ϊ" << StringFix(addB) << "�����Ƿ�ͬ�⣿(Y/N)" << endl;
					char c;
					cin >> c;
					c = toupper(c);
					if (c == 'Y')
					{
						BIntAssign(bintB, addB);
						bsent = true;
					}
					else
					{
						cout << "�ܾ���������ʧ�ܣ�����������" << endl;
					}
				}
				else
				{
					BIntAssign(bintB, StringCull(addB));
					bsent = true;
				}
			}
			bintS = BIntAdd(bintA, bintB);//����
			cout << "A+B=" << BIntParse(bintS) << endl;//���
			BIntFree(bintS);//�ͷ�
		}
		else
		{
			cout << "��Ч����������������" << endl;//���벻�Ϸ�
		}
	}
	return 0;
}