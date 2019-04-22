#include "BigInt.h"
#include <cstdlib>
#include <sstream>
#include <string>
#include <iterator>
#include <iomanip>
#include <algorithm>

using namespace std;

static short Add(short shA, short shB)//[仅限内部实现使用]加法函数，返回两个short型整数的加法结果，定义该函数的目的是为了便于使用函数指针应对不同的情形进行变换并调用对应的函数
{
	return shA + shB;
}
static short Sub(short shA, short shB)//[仅限内部实现使用]减法函数，返回两个short型整数的减法结果，定义该函数目的请参考Add函数
{
	return shA - shB;
}
static void BIntExtend(BigInt *&bint, int num)//[仅限内部实现使用]大整数扩增，直接在大整数的尾端新增节点并设其为num
{
	if (bint == 0)//如果大整数为空则直接退出，无需扩增。
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

BigInt *BIntCreate()//大整数创建
{
	//刚创建的大整数是空的
	//空大整数会被视为0
	//刚创建的大整数只包含头节点
	//由于是循环链表， 在刚创建的时候只有头节点，因此其前驱和后继均为自身（循环链表不会用到空指针）
	BigInt *newInt = new BigInt();
	newInt->head = new DigitQuad();
	newInt->head->data = 0;
	newInt->head->prev = newInt->head;
	newInt->head->next = newInt->head;
	newInt->block = 0;
	newInt->end = newInt->head;
	return newInt;
}
void BIntFree(BigInt *&bint)//大整数释放
{
	DigitQuad *diter = bint->head;
	bint->head->prev = 0;
	bint->end->next = 0;//将循环列表断环为线性链表以方便释放
	while (diter != 0)
	{
		DigitQuad *dbuff = diter->next;
		delete diter;
		diter = dbuff;
	}
	delete bint;
	bint = 0;
}
void BIntAssign(BigInt *&bint, string numstr)//大整数赋值，通过一个字符串为大整数进行赋值
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
		offset = 1;//判断字符串中是否存在打头的正负号，如果存在则绝对值从下一位开始
		break;
	default:
		break;
	}
	stringstream ss;
		//这是用于处理格式字符串用的字符串流
		//只需要知道他和cin类似只是cin是从键盘输入的而字符串流是从字符串输入的即可
	int i = 0, j = 0;
	//字符串四位一间隔，便于直接从字符串流里四位四位读取数据
	for (string::reverse_iterator iter = s.rbegin(); iter != s.rend() - offset; iter++, i++)
	{
		if (i != 0 && i % 4 == 0)
		{
			s.insert(iter.base(), ' ');
			j++;
			iter = s.rbegin() + i + j;
		}
	}
	ss << s;//将处理好的字符串送入字符串流以供读取
	//根据读取的数值为大整数写入数据
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
void BIntClear(BigInt *bint)//大整数清空，释放大整数后续节点并只保留头节点
{
	DigitQuad *diter = bint->head->next;
	while (diter != bint->head)//释放掉除头结点以外的所有节点（只保留头结点）
	{
		DigitQuad *dbuff = diter->next;
		delete diter;
		diter = dbuff;
	}
	//把一切恢复到刚创建时候的样子
	bint->head->next = bint->head;
	bint->head->prev = bint->head;
	bint->block = 0;
	bint->end = bint->head;
}
bool BIntEmpty(BigInt *bint)//判断大整数是否为空（这里认为大整数未被创建也为空）
{
	return bint == 0 || bint->head->next == bint->head || bint->end == bint->head;
}
BigInt *BIntCopy(BigInt *bsrc)//大整数拷贝，对大整数进行深度拷贝并创建其副本，返回指向副本的指针
{
	BigInt *bdes = BIntCreate();
	for (DigitQuad *srciter = bsrc->head->next; srciter != bsrc->head; srciter = srciter->next)
	{
		BIntExtend(bdes, srciter->data);//直接顺序地将bsrc的四元块尾插至待返回的副本bdes
	}
	return bdes;
}
BigInt *BIntAdd(BigInt *bintA, BigInt *bintB)//大整数加法函数，在不修改bintA与bintB的情况下求得bintA+bintB的结果，并返回指向结果的指针
{
	//若输入的bintA或bintB中存在空大整数的情形
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
	//bintA与bintB此时都不为空
	BigInt *bintS = 0;//存放结果的大整数bintS
					  //此处区分哪一个整数更长，并分别存于blng（较长的）和bshr（较短的），等长时直接对应bintA和bintB
	BigInt *blng = (bintB->block > bintA->block) ? bintB : bintA;
	BigInt *bshr = (bintB->block > bintA->block) ? bintA : bintB;
	//获取对应的符号（正负）
	short signLng = (blng->head->next->data < 0) ? -1 : 1, signShr = (bshr->head->next->data < 0) ? -1 : 1;
	//f是一个函数指针，代表要采取的运算
	short(*f)(short, short);
	//为了不修改结果，这里直接拷贝较长的整数blng并赋予给bintS作为临时结果（但此时尚未进行任何加法运算）
	bintS = BIntCopy(blng);
	if (signLng == signShr || bshr->block == 1)
	{
		//如若同号，则决定即将采取的运算为绝对值相加
		//令f为Add函数则接下来调用f的位置就相当于调用Add
		f = Add;
	}
	else
	{
		//如若异号，则决定即将采取的运算为绝对值相减
		f = Sub;
		//将两个数临时转换为绝对值
		bintS->head->next->data *= signLng;
		bshr->head->next->data *= signShr;
	}
	DigitQuad *dsiter = bintS->end;
	for (DigitQuad *iter = bshr->end; iter != bshr->head; iter = iter->prev)
	{
		//逐块相加或相减
		dsiter->data = f(dsiter->data, iter->data);
		dsiter = dsiter->prev;
	}
	//统一处理进位、退位、产生新的四位块的情形
	BIntNormalize(bintS);
	if (f == Sub && !BIntEmpty(bintS))
	{
		//恢复两个数的符号（做减法运算之前被绝对值化，为了保证结果不被改变，因此需要恢复符号）
		bintS->head->next->data *= signLng;
		bshr->head->next->data *= signShr;
	}
	return bintS;
}
bool BIntIsNorm(BigInt *bint)//判断大整数是否规格化
{
	/**************************
	不符合规格的大整数的具体表现或情形：
	1.end的下一位不是头节点（本应该是）
	2.最高四位块的所有后继节点中存在其值大于等于10000或小于0的（存在未处理的进位与退位）
	3.最高四位块绝对值大于等于10000（正负方向存在新增位进位）
	4.头节点的下一位四位块值为0（例如0000,9999，高位0是无效的并可以省略）
	5.大整数所记录自身的长度与实际长度不符
	**************************/
	if (bint == 0)//如果没有创建大整数无从判断是否为规格化的大整数，我们规定此时是规格化大整数
	{
		return true;
	}
	else
	{
		int digblock = 0;
		if (bint->end == 0 || bint->end->next != bint->head)//第1种情况
		{
			return false;
		}
		for (DigitQuad *diter = bint->head->next; diter != 0; diter = diter->next, digblock++)//逐块判断
		{
			if (diter == bint->head->next)
			{
				//第3种情形
				if (diter->data >= 10000 || diter->data <= -10000)
				{
					return false;
				}
			}
			else
			{
				//第2种情形
				if (diter->data >= 10000 || diter->data < 0)
				{
					return false;
				}
			}
		}
		//第5种情形
		if (digblock != bint->block)
		{
			return false;
		}
		//第4种情形
		if (bint->head->next != 0 && bint->head->next->data == 0)
		{
			return false;
		}
		//如果没发生上述任何一种情形则证明大整数是规格化的
		return true;
	}
}
void BIntNormalize(BigInt *bint)//大整数规格化函数，包含进位退位、新增位的处理、移除无效的0等，基本是针对于BIntIsNorm函数可能检测出来的问题进行修正
{
	if (bint == 0 || bint->head->next == bint->head)
	{
		//如果大整数为空则无需修正也无法考虑下面的内容
		return;
	}
	else
	{
		if (bint->end == 0)
		{
			//如若end为空，则将end掷回head（end不是尾后的，而是尾端，因此end无论如何也不能为空）
			bint->end = bint->head;
			bint->head->next = bint->head;
			bint->head->prev = bint->end;
		}
		if (bint->end->next != 0)
		{
			//若end下一位不是头节点（end的下一位本应该是头节点），则在end处截断
			bint->end->next = bint->head;
			bint->head->prev = bint->head;
		}

		for (DigitQuad *rditer = bint->end; rditer != bint->head; rditer = rditer->prev)//逐块修正（从后向前修正），处理进位与借位以及新增位
		{
			if (rditer != bint->head->next)
			{
				//进位与退位
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
				//产生新增位
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
		//移除高位不必要的0
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
	//修正结束
}
string BIntParse(BigInt *bint)//将大整数转化为格式化的字符串
{
	if (bint->head->next == 0)
	{
		return "0";
	}
	stringstream ss;
	string s;
	for (DigitQuad *diter = bint->head->next; diter != bint->head; diter = diter->next)
	{
		if (diter != bint->head->next)//对于非最高位的四位块，其值可能小于1000，如75，但在字面表示时需要用0补全4位即0075
		{
			ss << setw(4) << setfill('0') << diter->data;
		}
		else//否则就正常写入就好了
		{
			ss << diter->data;
		}
		if (diter != bint->end)
		{
			//每出4位加一个逗号隔开
			ss << ',';
		}
	}
	ss >> s;
	return s;
}

bool StringCheck(string str)//[文本分析函数]判断给定字符串是否表示规格化的大整数
{
	/*********
	能表示大整数的字符串需满足：
	1.除第一位可以出现'-'外其他数字位不可以出现数字以外的其他字符
	2.从后向前每隔4位出现一个','（忽略最高位可能出现的'-'，正常的','位不是数字位）
	*********/
	string::reverse_iterator riend = str.rend();
	if (str[0] == '-' || str[0] == '+')
	{
		riend--;//如果有正负号就跳过这一位开始检查数据格式
	}
	int cnt = 0;//cnt记录已经检查的位数，主要是用来看逗号的位置对不对
	for (string::reverse_iterator riter = str.rbegin(); riter != riend; riter++, cnt++)
	{

		if ((cnt + 1) % 5 == 0)//如果cnt到了该出现逗号的位置
		{
			if (*riter != ',')//而没出现逗号
				return false;//字符串格式是不正确的
		}
		else if (!isdigit(*riter))//而在不该出现逗号的其他位置，出现了数字以外的字符
		{
			return false;//格式也是不正确的
		}
	}
	return true;//只有能运行到这里的字符串才是格式正确的
}
string StringCull(string str)//[文本分析函数]字符串抽离函数，用于从给定字符串中抽离其他无用字符与逗号（在赋值时候使用的字符串不含逗号）
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
string StringFix(string str)//[文本分析函数]该函数用于将一个被StringCull过的字符串还原回含有逗号的格式
{
	string s = str;
	int i = 0;//记录读了多少位数，每隔四位会安插一个逗号
	int j = 0;//辅助变量
	for (string::reverse_iterator iter = s.rbegin(); iter != s.rend(); iter++, i++)
	{
		if (i != 0 && i % 4 == 0 && *iter != '-')
		{
			s.insert(iter.base(), ',');//逢4位插入逗号，但前提是前面不是打头的正负号
			j++;
			iter = s.rbegin() + i + j;//因为insert操作会导致iter无效（iter是一个迭代器，理解成一个指针就好了），因此执行插入操作之后必须为iter恢复其指向的位置，以供进行下一轮的迭代
		}
	}
	return s;
}