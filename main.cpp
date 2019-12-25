#include"timeWheel.h"
#include<sys/time.h>
#include<sys/epoll.h>
#include<iostream>
using namespace std;
timeval time1;
timeval time2;
timeWheel tw(10*10,100);
void print2()
{
	gettimeofday(&time1, NULL);
	cout <<"2 second :"<< time1.tv_sec << endl;
	tw.add_timer(2000, print2);
}
void print3()
{
	gettimeofday(&time2, NULL);
	cout << "3 second :" << time2.tv_sec << endl;
	tw.add_timer(3000, print3);
}
int main()
{
	int fd=epoll_create(1);
	tw.add_timer(2000, std::bind(print2));
	//tw.add_timer(3000, print3);
	//int i = 50;
	while (1)
	{
		epoll_wait(fd, NULL, 1, 100);
		tw.tick();
		vector<tw_timer*> active = tw.getAvtiveTimer();
		if (active.empty())
			continue;
		else
		{
			for (tw_timer* t : active)
			{
				t->func();
				delete t;
			}
		}

	}
	fflush(stdout);

}