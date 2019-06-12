#include<map>
#include<iostream>
#include<deque>
#include<vector>
#include<algorithm>
using namespace std;

struct line
{
	int pos;
	int hei;
	bool dir;
	line(int p,int h ,int d)
	{
		pos = p;
		hei = h;
		dir = d;
	}
};
deque<vector<int>> buildline(deque<vector<int>>& build)
{
	deque<line> arr;
	int height = 0;
	deque<vector<int>> linearr;
	map<int,int> heimap;
	for(auto var : build)
	{
		arr.push_back(line(var[0],var[2],1));
		arr.push_back(line(var[1],var[2],0));
	}
	sort(arr.begin(),arr.end(),[](line& next, line& cur){return next.pos <cur.pos;});
	for(auto var : arr)
	{
		auto curline =  heimap.find(var.hei);
		if(curline==heimap.end())
			heimap[var.hei] = 1;
		else if(var.dir)
			curline->second++;
		else
		{
			if(heimap[var.hei] == 1)
				heimap.erase(var.hei);
			else
				curline->second--;
		}
		if(heimap.empty())
		{
			linearr.push_back({var.pos,0});
			height = 0;
		}
		else if(heimap.rbegin()->first!=height)
		{
			height = heimap.rbegin()->first;
			linearr.push_back({var.pos,height});
		}
	}
	height =0;
	int start = 0;
	deque<vector<int>> res;
	for(auto var : linearr)
	{
		if(height!=0)
			res.push_back({start,var[0],height});
		height = var[1];
		start = var[0];
	}
	return std::move(res);
}
int main()
{
	deque<vector<int>> build = {{0,2,2},{1,5,3},{3,4,1},{6,7,2}};
	build = buildline(build);
	for(auto var:build)
	{
		cout<<var[0]<<" "<<var[1]<<" "<<var[2]<<endl;
	}
}
