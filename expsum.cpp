#include<iostream>
#include<memory>
#include<cstdlib>
#include<string>
#include<deque>
#include<string>

void calope(std::deque<std::string>& que, int pre)
{
	if(que.empty())
	{
		que.push_back(std::to_string(pre));
		return;
	}
	std::string cur = que.back();
	que.pop_back();
	if(cur == "*"||cur == "/")
	{
		pre = cur == "*" ? pre * atoi(que.back().c_str()) : pre / atoi(que.back().c_str());
		que.pop_back();
	}
	que.push_back(std::to_string(pre));
}
int getsum(std::deque<std::string>& que)
{
	int res = atoi(que.front().c_str());
	que.pop_front();
	std::string temp;
	while(!que.empty())
	{
		temp = que.front();
		que.pop_front();
		res = temp == "+" ? res+atoi(que.front().c_str()) : res - atoi(que.front().c_str());
		que.pop_front();
	}
	return res;
}
std::unique_ptr<int[]> expsum(std::string s, int i)
{
	int pre = 0;
	std::deque<std::string> que;
	while(i<s.size()&& s[i] != ')')
	{
		if(s[i] >= '0' && s[i] <= '9')
			pre = 10*pre + s[i++]-'0';
		else if(s[i] != '(')
		{
			calope(que,pre);
			que.push_back(std::string(1,s[i++]));
			pre = 0;
		}
		else
		{
			std::unique_ptr<int[]> a = std::move(expsum(s, i+1));
			pre = a[0];
			i = a[1];
		}
	}
	que.push_back(std::to_string(pre));
	int sum = getsum(que);
	std::unique_ptr<int[]> res(new int[2]);
	res[0] = sum;
	res[1] = i+1;
	return res;
}

int main()
{
	std::string exp = "(1+2)*3+4";
	std::unique_ptr<int[]> a = expsum(exp,0);
	std::cout<<a[0];
}
