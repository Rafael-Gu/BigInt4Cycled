#include <iostream>
#include "BigInt.h"
#include <string>
using namespace std;

int main()
{
	BigInt *bintA = 0;//A
	BigInt *bintB = 0;//B
	BigInt *bintS = 0;//和
	string addA, addB;
	while (true)
	{
		int selection;
		//菜单
		cout << "请输入你要执行的操作" << endl;
		cout << "1.进行加法运算" << endl;
		cout << "2.退出" << endl;
		cout << "请选择：";
		//要求输入
		cin >> selection;
		if (selection == 2)//选2退出
		{
			break;
		}
		else if (selection == 1)//选1开始计算
		{
			bool asent = false, bsent = false;//asent与bsent表示A和B是否被正确给值，如果没有，则重复输入请求直至被正确给值为止
			while (!asent)//下方bsent与之同理只是处理addB用的，因此不再重复给注释了
			{
				cout << "请输入A" << endl;
				cin >> addA;
				if (!StringCheck(addA))//输入时检查一下格式
				{
					cout << "输入格式似乎有问题……" << endl;
					addA = StringCull(addA);//对输入字符串进行剔除
					cout << "将被视为" << StringFix(addA) << "处理，是否同意？(Y/N)" << endl;//问询是否按照建议的修改方案进行自动修改
					char c;
					cin >> c;
					c = toupper(c);//排除大小写输入造成的影响
					if (c == 'Y')//同意则继续
					{
						BIntAssign(bintA, StringCull(addA));//给值
						asent = true;
					}
					else//不同意则要求重新输入
					{
						cout << "拒绝处理，输入失败，请重新输入" << endl;
					}
				}
				else
				{
					BIntAssign(bintA, StringCull(addA));//如果格式正确则直接给值无需询问
					asent = true;//A被成功给值
				}
			}
			while (!bsent)
			{
				cout << "请输入B" << endl;
				cin >> addB;
				if (!StringCheck(addB))
				{
					cout << "输入格式似乎有问题……" << endl;
					addB = StringCull(addB);
					cout << "将被视为" << StringFix(addB) << "处理，是否同意？(Y/N)" << endl;
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
						cout << "拒绝处理，输入失败，请重新输入" << endl;
					}
				}
				else
				{
					BIntAssign(bintB, StringCull(addB));
					bsent = true;
				}
			}
			bintS = BIntAdd(bintA, bintB);//作和
			cout << "A+B=" << BIntParse(bintS) << endl;//输出
			BIntFree(bintS);//释放
		}
		else
		{
			cout << "无效操作，请重新输入" << endl;//输入不合法
		}
	}
	return 0;
}