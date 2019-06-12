#include<thread>
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
	semaphore(std::string n)
	{
		name = n;
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
	std::string name;
	int value = 0;
 	std::condition_variable cv;
	int wakeup = 0;
	std::mutex Mutex;
};
bool start = false;
std::vector<semaphore> sign;
semaphore sign1("s1");
semaphore sign2("s2");
semaphore sign3("s3");
semaphore sign4("s4");
semaphore sign5("s5");
semaphore sign6("s6");
void syn1()
{
	while(start);
	printf("s1被调用了\n");
	sign[1].signal();
	sign[2].signal();
}
void syn2()
{
	while(start);
	sign[1].wait();
	printf("s2被调用了\n");
	sign[3].signal();
	sign[4].signal();
}
void syn3()
{
	while(start);
	sign[2].wait();
	printf("s3被调用了\n");
	sign[5].signal();
}
void syn4()
{
	while(start);
	sign[3].wait();
	printf("s4被调用了\n");
	sign[5].signal();
}
void syn5()
{
	while(start);
	sign[4].wait();
	printf("s5被调用了\n");
	sign[5].signal();
}
void syn6()
{
	while(start);
	sign[5].wait();
	printf("s6被调用了\n");
}

std::mutex m;
int main()
{
	std::vector<std::thread> pool;
	sign.emplace_back("s1");
	sign.emplace_back("s2");
	sign.emplace_back("s3");
	sign.emplace_back("s4");
	sign.emplace_back("s5");
	sign.emplace_back("s6");
	pool.emplace_back(syn1);
	pool.emplace_back(syn2);
	pool.emplace_back(syn3);
	pool.emplace_back(syn4);
	pool.emplace_back(syn5);
	pool.emplace_back(syn6);
	std::this_thread::sleep_for(std::chrono::seconds(2));
	start = true;
	for(auto &var : pool)
		var.join();
}
