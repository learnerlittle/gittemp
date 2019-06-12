#include<bitset>
#include<algorithm>
#include<iostream>
#include<memory>

struct Node:std::enable_shared_from_this<Node>
{
 public:
 	Node() = default;
	void addero(int num)
	{
		std::shared_ptr<Node> tryNode = shared_from_this();
		for(int i=31;i>=0;i--)
		{
			int cur = num>>i&1;
			tryNode->next[cur] = tryNode->next[cur]?tryNode->next[cur]:std::make_shared<Node>();
			tryNode = tryNode->next[cur];
		}
	}
	int getmax(int ero)
	{
		std::shared_ptr<Node> tryNode = shared_from_this();
		int res=0;
		for(int i=31;i>=0;i--)
		{
			int cur = ero>>i&1;
			int best = i==31?cur:cur^1;
			best = tryNode->next[best]?best:best^1;
			res |= (cur^best)<<i;
			tryNode = tryNode->next[best];
		}
		return res;
	}
	int getAllmax(std::vector<int>& arr)
	{
		int res = arr[0];
		addero(0);
		int ero = 0;
		for(int i=0;i<arr.size();i++)
		{
			ero^=arr[i];
			res = std::max(res,getmax(ero));
			addero(ero);
		}
		return res;
	}

 private:
 	std::shared_ptr<Node> next[2];
};

int main()
{
	std::shared_ptr<Node> node = std::make_shared<Node>();
	std::vector<int> arr = {-1};
	std::cout<<node->getAllmax(arr);
}
