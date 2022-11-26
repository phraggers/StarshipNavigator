
#ifndef TIMER_H_
#define TIMER_H_

struct FrameTimer
{
	int target_fps;
	u64 perf_freq;
	u64 start;
	u64 end;
	r64 prev_time;
	r64 target_time;
	u64 total_frames;
	r32 avg_fps;

	FrameTimer(void);
	void Init(int _fps);
	void FrameStart(void);
	void FrameEnd(void);
};

struct Timer
{
	u64 perf_freq;
	u64 timer;

	Timer(void);
	r64 Delta(void);
};

#endif // TIMER_H_