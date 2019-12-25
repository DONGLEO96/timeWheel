#pragma once
#include<vector>
#include<functional>
using namespace std;
typedef std::function<void()> Func;
struct tw_timer
{
	tw_timer(int rot, int ts,Func _func)
	{
		ratation = rot;
		time_slot = ts;
		func = _func;
	}
	int ratation;
	int time_slot;
	Func func;
	tw_timer* prev = NULL;
	tw_timer* next = NULL;

};

class timeWheel
{
public:
	timeWheel(int _slotNum=60*1000,int _step=1);
	~timeWheel();
	tw_timer* add_timer(int timeout, Func _func);
	void del_timer(tw_timer* _timer);
	void tick();
	vector<tw_timer*> getAvtiveTimer();
//private:
	const int slotNum;//轮子上时间槽的数量
	const int step;//毫秒，每step毫秒轮子移动一个slot
	vector<tw_timer*> slots;
	vector<tw_timer*> activeTimer;
	int cur_slot;

};

