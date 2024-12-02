#include<iostream>
#include<vector>
using namespace std;

int a[325];
void init();
void work();
void testA();
float FIFO(int k);	//队列正常先进后出
float LRU(int k);   //使用某个页面后移至队尾
float OPT(int k);   //查询后续指令，移除后面最晚才使用的那个页面
float LFU(int k);	//记录使用次数，移除使用最少的页面


//发生缺页的判断条件是指令所在的页面不在内存中
int main()
{
	init();
	work();
	return 0;
}

void init()
{
	srand((unsigned)time(NULL));
	int m = 160;
	for (int i = 0; i < 80; i++)
	{
		int j = i * 4;

		a[j] = m;
		a[j + 1] = m + 1;
		a[j + 2] = static_cast<int>(a[j] * 1.0 * rand() / 32767);
		a[j + 3] = a[j + 2] + 1;

		m = static_cast<int>(a[j + 3] + (319 - a[j + 3]) * 1.0 * rand() / 32767);
	}

}
void testA()
{
	for (int i = 0; i <= 319; i++)
	{
		std::cout << a[i] << '\t';
		if (i != 0 && i % 10 == 0) cout << "\n";
	}
}
void work()
{
	for (int i = 4; i <= 32; i++)
	{
		printf("k=%d\tFIFO:%.4f\tLRU:%.4f\tOPT:%.4f\tLFU:%.4f\n", i, FIFO(i), LRU(i),OPT(i),LFU(i));
		//cout << "k=" << i << "\tFIFO:" << FIFO(i) << "\tLRU:" << LRU(i) << '\n';
	}
}

float FIFO(int k)
{
	vector<int> memoryList;
	int errorCnt = 0;

	for (int i = 0; i < 320; i++)
	{
		int pt = a[i] / 10;
		auto it = find(memoryList.begin(), memoryList.end(), pt);

		if (it != memoryList.end()) continue;
		else
		{
			if (memoryList.size() == k)
				memoryList.erase(memoryList.begin(), memoryList.begin() + 1);

			memoryList.emplace_back(pt);

			errorCnt++;
		}
	}
	return static_cast<float>(1 - 1.0 * errorCnt / 320);
}


float LRU(int k)
{
	vector<int> memoryList;
	int errorCnt = 0;

	for (int i = 0; i < 320; i++)
	{
		int pt = a[i] / 10;
		auto it = find(memoryList.begin(), memoryList.end(), pt);

		if (it != memoryList.end())
		{
			memoryList.erase(it);
			memoryList.emplace_back(pt);
		}
		else
		{
			if (memoryList.size() == k)
				memoryList.erase(memoryList.begin(), memoryList.begin() + 1);

			memoryList.emplace_back(pt);

			errorCnt++;
		}
	}
	return static_cast<float>(1 - 1.0 * errorCnt / 320);
}
float OPT(int k)
{
	vector<int> memoryList;
	int errorCnt = 0;

	for (int i = 0; i < 320; i++)
	{
		int pt = a[i] / 10;
		auto it = find(memoryList.begin(), memoryList.end(), pt);
		

		if (it == memoryList.end())
		{
			if (memoryList.size() == k)
			{
				bool flag = false;
				for (int tempIndex = 319; tempIndex >= i+1; tempIndex--)
				{
					auto tempIt = find(memoryList.begin(), memoryList.end(), a[tempIndex] / 10);
					if (tempIt != memoryList.end())
					{
						memoryList.erase(tempIt);
						flag = true;
						break;
					}
				}
				if (!flag) memoryList.erase(memoryList.begin(), memoryList.begin() + 1);
			}


			memoryList.emplace_back(pt);
			errorCnt++;
		}

	}
	return static_cast<float>(1 - 1.0 * errorCnt / 320);
}
float LFU(int k)
{
	vector<int> useCnt;		//memoryList[i]这个页面的访问次数 == useCnt[i]
	vector<int> memoryList; //当前内存中的页面
	int errorCnt = 0;

	for (int i = 0; i < 320; i++)
	{
		int pt = a[i] / 10;
		auto it = find(memoryList.begin(), memoryList.end(), pt);

		if (it == memoryList.end())
		{
			if (memoryList.size() == k)
			{
				//找到最少使用的
				int min_value = 1000, min_index = 0;
				for (int tempIndex = 0; tempIndex < memoryList.size(); tempIndex++)
				{
					if (useCnt[tempIndex] < min_value)
					{
						min_value = useCnt[tempIndex];
						min_index = tempIndex;
					}
				}
				useCnt.erase(useCnt.begin() + min_index, useCnt.begin() + min_index + 1);
				memoryList.erase(memoryList.begin() + min_index, memoryList.begin() + min_index + 1);
			}

			memoryList.emplace_back(pt);
			useCnt.emplace_back(0);
			errorCnt++;

		}


		

	}
	return static_cast<float>(1 - 1.0 * errorCnt / 320);
}
