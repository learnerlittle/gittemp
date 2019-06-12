#include<thread>
#include<memory>
#include<chrono>
#include<cstdio>
#include<vector>
#include<condition_variable>
#include<iostream>
#include<mutex>
#include<string>

class semaphore
{
public:
 	void wait()
	{
		std::unique_lock<std::mutex> lock(Mutex);
		if(--value<0)
		{
			cv.wait(lock,[this]{return wakeup>0;});
			--wakeup;
		}
	}
	void signal()
	{
		std::unique_lock<std::mutex> lock(Mutex);
		if(++value<=0)
		{
			++wakeup;
			cv.notify_one();
		}
	}
	semaphore()=default;
	semaphore(int v)
	{
		value = v;
	}
	semaphore(semaphore&& s)
	{
		name = s.name;
		value = s.value;
		wakeup = s.wakeup;
	}
	semaphore(semaphore& s)
	{
		name = s.name;
		value = s.value;
		wakeup = s.wakeup;
	}
	semaphore& operator=(const semaphore& s)
	{
		name = s.name;
		value = s.value;
		wakeup = s.wakeup;
	}	
	int value = 0;
private:
	std::string name;
 	std::condition_variable cv;
	int wakeup = 0;
	std::mutex Mutex;
};
bool start = false;
class procon
{
public:
	procon()
	{
		signempty.value = 2;
	}
	void mom()
	{
		while(start);
		signempty.wait();
		m.lock();
		--empty;
		printf("放入一个苹果，还剩%d个空间\n",empty);
		std::this_thread::sleep_for(std::chrono::seconds(2));
		m.unlock();
		applenum.signal();
	}
	void dad()
	{
		while(start);
		signempty.wait();
		m.lock();
		--empty;
		printf("放入一个香蕉，还剩%d个空间\n",empty);
		std::this_thread::sleep_for(std::chrono::seconds(2));
		m.unlock();
		bananum.signal();
	}

	void daughter()
	{
		while(start);
		applenum.wait();
		m.lock();
		++empty;
		printf("拿走一个苹果，还剩%d个空间\n",empty);
		std::this_thread::sleep_for(std::chrono::seconds(2));
		m.unlock();
		signempty.signal();
	}
	void son()
	{
		while(start);
		bananum.wait();
		m.lock();
		++empty;
		printf("拿走一个香蕉，还剩%d个空间\n",empty);
		std::this_thread::sleep_for(std::chrono::seconds(2));
		m.unlock();
		signempty.signal();
	}
private:
	std::mutex m;
	semaphore signempty;
	semaphore applenum;
	semaphore bananum;
	int empty = 2;
};

int main()
{
	std::shared_ptr<procon> mutil = std::make_shared<procon>();
	std::vector<std::thread> pool;
	pool.emplace_back(&procon::daughter,mutil);
	pool.emplace_back(&procon::son,mutil);
	pool.emplace_back(&procon::dad,mutil);
	pool.emplace_back(&procon::mom,mutil);
	std::this_thread::sleep_for(std::chrono::seconds(2));
	start = true;
	for(auto& var : pool)
	{
		var.join();
	}
}
