#include "BigInt.h"
#include <cstdlib>
#include <sstream>
#include <string>
#include <iterator>
#include <iomanip>
#include <algorithm>

using namespace std;

static short Add(short shA, short shB)//[�����ڲ�ʵ��ʹ��]�ӷ���������������short�������ļӷ����������ú�����Ŀ����Ϊ�˱���ʹ�ú���ָ��Ӧ�Բ�ͬ�����ν��б任�����ö�Ӧ�ĺ���
{
	return shA + shB;
}
static short Sub(short shA, short shB)//[�����ڲ�ʵ��ʹ��]������������������short�������ļ������������ú���Ŀ����ο�Add����
{
	return shA - shB;
}
static void BIntExtend(BigInt *&bint, int num)//[�����ڲ�ʵ��ʹ��]������������ֱ���ڴ�������β�������ڵ㲢����Ϊnum
{
	if (bint == 0)//���������Ϊ����ֱ���˳�������������
	{
		return;
	}
	bint->end->next = new DigitQuad();
	bint->end->next->prev = bint->end;
	bint->end = bint->end->next;
	bint->end->next = 0;
	bint->end->data = num;
	bint->block++;
	bint->head->prev = bint->end;
	bint->end->next = bint->head;
}

BigInt *BIntCreate()//����������
{
	//�մ����Ĵ������ǿյ�
	//�մ������ᱻ��Ϊ0
	//�մ����Ĵ�����ֻ����ͷ�ڵ�
	//������ѭ������ �ڸմ�����ʱ��ֻ��ͷ�ڵ㣬�����ǰ���ͺ�̾�Ϊ����ѭ���������õ���ָ�룩
	BigInt *newInt = new BigInt();
	newInt->head = new DigitQuad();
	newInt->head->data = 0;
	newInt->head->prev = newInt->head;
	newInt->head->next = newInt->head;
	newInt->block = 0;
	newInt->end = newInt->head;
	return newInt;
}
void BIntFree(BigInt *&bint)//�������ͷ�
{
	DigitQuad *diter = bint->head;
	bint->head->prev = 0;
	bint->end->next = 0;//��ѭ���б�ϻ�Ϊ���������Է����ͷ�
	while (diter != 0)
	{
		DigitQuad *dbuff = diter->next;
		delete diter;
		diter = dbuff;
	}
	delete bint;
	bint = 0;
}
void BIntAssign(BigInt *&bint, string numstr)//��������ֵ��ͨ��һ���ַ���Ϊ���������и�ֵ
{
	if (bint == 0)
	{
		bint = BIntCreate();
	}
	BIntClear(bint);
	int sign = 1;
	string s = numstr;
	string::reverse_iterator riend = s.rend();
	int offset = 0;
	switch (*s.begin())
	{
	case '+':
	case '-':
		offset = 1;//�ж��ַ������Ƿ���ڴ�ͷ�������ţ�������������ֵ����һλ��ʼ
		break;
	default:
		break;
	}
	stringstream ss;
		//�������ڴ����ʽ�ַ����õ��ַ�����
		//ֻ��Ҫ֪������cin����ֻ��cin�ǴӼ�������Ķ��ַ������Ǵ��ַ�������ļ���
	int i = 0, j = 0;
	//�ַ�����λһ���������ֱ�Ӵ��ַ���������λ��λ��ȡ����
	for (string::reverse_iterator iter = s.rbegin(); iter != s.rend() - offset; iter++, i++)
	{
		if (i != 0 && i % 4 == 0)
		{
			s.insert(iter.base(), ' ');
			j++;
			iter = s.rbegin() + i + j;
		}
	}
	ss << s;//������õ��ַ��������ַ������Թ���ȡ
	//���ݶ�ȡ����ֵΪ������д������
	DigitQuad *iter = bint->head;
	while (!ss.eof())
	{
		if (iter == bint->end)
		{
			iter->next = new DigitQuad();
			iter->next->prev = bint->end;
			iter = iter->next;
			iter->next = 0;
			bint->end = iter;
			ss >> iter->data;
			bint->end->next = bint->head;
			bint->head->prev = bint->end;
			bint->block++;
		}
	}
}
void BIntClear(BigInt *bint)//��������գ��ͷŴ����������ڵ㲢ֻ����ͷ�ڵ�
{
	DigitQuad *diter = bint->head->next;
	while (diter != bint->head)//�ͷŵ���ͷ�����������нڵ㣨ֻ����ͷ��㣩
	{
		DigitQuad *dbuff = diter->next;
		delete diter;
		diter = dbuff;
	}
	//��һ�лָ����մ���ʱ�������
	bint->head->next = bint->head;
	bint->head->prev = bint->head;
	bint->block = 0;
	bint->end = bint->head;
}
bool BIntEmpty(BigInt *bint)//�жϴ������Ƿ�Ϊ�գ�������Ϊ������δ������ҲΪ�գ�
{
	return bint == 0 || bint->head->next == bint->head || bint->end == bint->head;
}
BigInt *BIntCopy(BigInt *bsrc)//�������������Դ�����������ȿ����������丱��������ָ�򸱱���ָ��
{
	BigInt *bdes = BIntCreate();
	for (DigitQuad *srciter = bsrc->head->next; srciter != bsrc->head; srciter = srciter->next)
	{
		BIntExtend(bdes, srciter->data);//ֱ��˳��ؽ�bsrc����Ԫ��β���������صĸ���bdes
	}
	return bdes;
}
BigInt *BIntAdd(BigInt *bintA, BigInt *bintB)//�������ӷ��������ڲ��޸�bintA��bintB����������bintA+bintB�Ľ����������ָ������ָ��
{
	//�������bintA��bintB�д��ڿմ�����������
	if (BIntEmpty(bintA) && !BIntEmpty(bintB))
	{
		return BIntCopy(bintB);
	}
	else if (BIntEmpty(bintB) && !BIntEmpty(bintB))
	{
		return BIntCopy(bintA);
	}
	else if (BIntEmpty(bintA) && BIntEmpty(bintB))
	{
		return BIntCreate();
	}
	//bintA��bintB��ʱ����Ϊ��
	BigInt *bintS = 0;//��Ž���Ĵ�����bintS
					  //�˴�������һ���������������ֱ����blng���ϳ��ģ���bshr���϶̵ģ����ȳ�ʱֱ�Ӷ�ӦbintA��bintB
	BigInt *blng = (bintB->block > bintA->block) ? bintB : bintA;
	BigInt *bshr = (bintB->block > bintA->block) ? bintA : bintB;
	//��ȡ��Ӧ�ķ��ţ�������
	short signLng = (blng->head->next->data < 0) ? -1 : 1, signShr = (bshr->head->next->data < 0) ? -1 : 1;
	//f��һ������ָ�룬����Ҫ��ȡ������
	short(*f)(short, short);
	//Ϊ�˲��޸Ľ��������ֱ�ӿ����ϳ�������blng�������bintS��Ϊ��ʱ���������ʱ��δ�����κμӷ����㣩
	bintS = BIntCopy(blng);
	if (signLng == signShr || bshr->block == 1)
	{
		//����ͬ�ţ������������ȡ������Ϊ����ֵ���
		//��fΪAdd���������������f��λ�þ��൱�ڵ���Add
		f = Add;
	}
	else
	{
		//������ţ������������ȡ������Ϊ����ֵ���
		f = Sub;
		//����������ʱת��Ϊ����ֵ
		bintS->head->next->data *= signLng;
		bshr->head->next->data *= signShr;
	}
	DigitQuad *dsiter = bintS->end;
	for (DigitQuad *iter = bshr->end; iter != bshr->head; iter = iter->prev)
	{
		//�����ӻ����
		dsiter->data = f(dsiter->data, iter->data);
		dsiter = dsiter->prev;
	}
	//ͳһ�����λ����λ�������µ���λ�������
	BIntNormalize(bintS);
	if (f == Sub && !BIntEmpty(bintS))
	{
		//�ָ��������ķ��ţ�����������֮ǰ������ֵ����Ϊ�˱�֤��������ı䣬�����Ҫ�ָ����ţ�
		bintS->head->next->data *= signLng;
		bshr->head->next->data *= signShr;
	}
	return bintS;
}
bool BIntIsNorm(BigInt *bint)//�жϴ������Ƿ���
{
	/**************************
	�����Ϲ��Ĵ������ľ�����ֻ����Σ�
	1.end����һλ����ͷ�ڵ㣨��Ӧ���ǣ�
	2.�����λ������к�̽ڵ��д�����ֵ���ڵ���10000��С��0�ģ�����δ����Ľ�λ����λ��
	3.�����λ�����ֵ���ڵ���10000�����������������λ��λ��
	4.ͷ�ڵ����һλ��λ��ֵΪ0������0000,9999����λ0����Ч�Ĳ�����ʡ�ԣ�
	5.����������¼����ĳ�����ʵ�ʳ��Ȳ���
	**************************/
	if (bint == 0)//���û�д����������޴��ж��Ƿ�Ϊ��񻯵Ĵ����������ǹ涨��ʱ�ǹ�񻯴�����
	{
		return true;
	}
	else
	{
		int digblock = 0;
		if (bint->end == 0 || bint->end->next != bint->head)//��1�����
		{
			return false;
		}
		for (DigitQuad *diter = bint->head->next; diter != 0; diter = diter->next, digblock++)//����ж�
		{
			if (diter == bint->head->next)
			{
				//��3������
				if (diter->data >= 10000 || diter->data <= -10000)
				{
					return false;
				}
			}
			else
			{
				//��2������
				if (diter->data >= 10000 || diter->data < 0)
				{
					return false;
				}
			}
		}
		//��5������
		if (digblock != bint->block)
		{
			return false;
		}
		//��4������
		if (bint->head->next != 0 && bint->head->next->data == 0)
		{
			return false;
		}
		//���û���������κ�һ��������֤���������ǹ�񻯵�
		return true;
	}
}
void BIntNormalize(BigInt *bint)//��������񻯺�����������λ��λ������λ�Ĵ����Ƴ���Ч��0�ȣ������������BIntIsNorm�������ܼ������������������
{
	if (bint == 0 || bint->head->next == bint->head)
	{
		//���������Ϊ������������Ҳ�޷��������������
		return;
	}
	else
	{
		if (bint->end == 0)
		{
			//����endΪ�գ���end����head��end����β��ģ�����β�ˣ����end�������Ҳ����Ϊ�գ�
			bint->end = bint->head;
			bint->head->next = bint->head;
			bint->head->prev = bint->end;
		}
		if (bint->end->next != 0)
		{
			//��end��һλ����ͷ�ڵ㣨end����һλ��Ӧ����ͷ�ڵ㣩������end���ض�
			bint->end->next = bint->head;
			bint->head->prev = bint->head;
		}

		for (DigitQuad *rditer = bint->end; rditer != bint->head; rditer = rditer->prev)//����������Ӻ���ǰ�������������λ���λ�Լ�����λ
		{
			if (rditer != bint->head->next)
			{
				//��λ����λ
				if (rditer->data >= 10000)
				{
					rditer->prev->data++;
					rditer->data -= 10000;
				}
				else if (rditer->data < 0)
				{
					rditer->prev->data--;
					rditer->data += 10000;
				}
			}
			else
			{
				//��������λ
				DigitQuad *dext = 0;
				if (rditer->data >= 10000)
				{
					dext = new DigitQuad();
					dext->data = 1;
					rditer->data -= 10000;
					dext->prev = bint->head;
					dext->next = rditer;
					rditer->prev = dext;
					bint->head->next = dext;
					bint->block++;
				}
				else if (rditer->data <= -10000)
				{
					dext = new DigitQuad();
					dext->data = -1;
					rditer->data += 10000;
					dext->prev = bint->head;
					dext->next = rditer;
					rditer->prev = dext;
					bint->head->next = dext;
					bint->block++;
				}
			}
		}
		//�Ƴ���λ����Ҫ��0
		while (bint->head->next != 0 && bint->head->next->data == 0)
		{
			DigitQuad *bbuff = bint->head->next;
			bint->head->next = bint->head->next->next;
			if (bint->head->next != 0)
			{
				bint->head->next->prev = bint->head;
				delete bbuff;
				bint->block--;

			}
			else
			{
				break;
			}
		}
	}
	//��������
}
string BIntParse(BigInt *bint)//��������ת��Ϊ��ʽ�����ַ���
{
	if (bint->head->next == 0)
	{
		return "0";
	}
	stringstream ss;
	string s;
	for (DigitQuad *diter = bint->head->next; diter != bint->head; diter = diter->next)
	{
		if (diter != bint->head->next)//���ڷ����λ����λ�飬��ֵ����С��1000����75�����������ʾʱ��Ҫ��0��ȫ4λ��0075
		{
			ss << setw(4) << setfill('0') << diter->data;
		}
		else//���������д��ͺ���
		{
			ss << diter->data;
		}
		if (diter != bint->end)
		{
			//ÿ��4λ��һ�����Ÿ���
			ss << ',';
		}
	}
	ss >> s;
	return s;
}

bool StringCheck(string str)//[�ı���������]�жϸ����ַ����Ƿ��ʾ��񻯵Ĵ�����
{
	/*********
	�ܱ�ʾ���������ַ��������㣺
	1.����һλ���Գ���'-'����������λ�����Գ�����������������ַ�
	2.�Ӻ���ǰÿ��4λ����һ��','���������λ���ܳ��ֵ�'-'��������','λ��������λ��
	*********/
	string::reverse_iterator riend = str.rend();
	if (str[0] == '-' || str[0] == '+')
	{
		riend--;//����������ž�������һλ��ʼ������ݸ�ʽ
	}
	int cnt = 0;//cnt��¼�Ѿ�����λ������Ҫ�����������ŵ�λ�öԲ���
	for (string::reverse_iterator riter = str.rbegin(); riter != riend; riter++, cnt++)
	{

		if ((cnt + 1) % 5 == 0)//���cnt���˸ó��ֶ��ŵ�λ��
		{
			if (*riter != ',')//��û���ֶ���
				return false;//�ַ�����ʽ�ǲ���ȷ��
		}
		else if (!isdigit(*riter))//���ڲ��ó��ֶ��ŵ�����λ�ã�����������������ַ�
		{
			return false;//��ʽҲ�ǲ���ȷ��
		}
	}
	return true;//ֻ�������е�������ַ������Ǹ�ʽ��ȷ��
}
string StringCull(string str)//[�ı���������]�ַ������뺯�������ڴӸ����ַ����г������������ַ��붺�ţ��ڸ�ֵʱ��ʹ�õ��ַ����������ţ�
{
	string dest;
	for (int i = 0; i < str.length(); i++)
	{
		if (i == 0)
		{
			if (isdigit(str[i]) || str[i] == '-')
			{
				dest.push_back(str[i]);
			}
		}
		else
		{
			if (isdigit(str[i]))
			{
				dest.push_back(str[i]);
			}
		}
	}
	return dest;
}
string StringFix(string str)//[�ı���������]�ú������ڽ�һ����StringCull�����ַ�����ԭ�غ��ж��ŵĸ�ʽ
{
	string s = str;
	int i = 0;//��¼���˶���λ����ÿ����λ�ᰲ��һ������
	int j = 0;//��������
	for (string::reverse_iterator iter = s.rbegin(); iter != s.rend(); iter++, i++)
	{
		if (i != 0 && i % 4 == 0 && *iter != '-')
		{
			s.insert(iter.base(), ',');//��4λ���붺�ţ���ǰ����ǰ�治�Ǵ�ͷ��������
			j++;
			iter = s.rbegin() + i + j;//��Ϊinsert�����ᵼ��iter��Ч��iter��һ��������������һ��ָ��ͺ��ˣ������ִ�в������֮�����Ϊiter�ָ���ָ���λ�ã��Թ�������һ�ֵĵ���
		}
	}
	return s;
}