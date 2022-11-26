
#include "Starship_Navigator.h"

static inline r64 _GetPerfElapsed(u64 _prev, u64 _perf_freq)
{
	return (r64)((u64)(SDL_GetPerformanceCounter() - _prev) * 1000.0f) / _perf_freq;
}

static inline r64 _GetPerfInterval(u64 _start, u64 _end, u64 _perf_freq)
{
	return (r64)((u64)(_end - _start) * 1000.0f) / _perf_freq;
}

FrameTimer::FrameTimer(void)
{
	target_fps = 0;
	perf_freq = 0;
	start = 0;
	end = 0;
	prev_time = 0;
	target_time = 0;
	total_frames = 0;
	avg_fps = 0;
}

void FrameTimer::Init(int _fps)
{
	target_fps = _fps;
	perf_freq = SDL_GetPerformanceFrequency();
	start = SDL_GetPerformanceCounter();
	end = SDL_GetPerformanceCounter();
	target_time = 1000.0f / target_fps;
}

void FrameTimer::FrameStart(void)
{
    start = SDL_GetPerformanceCounter();
    total_frames++;
}

void FrameTimer::FrameEnd(void)
{
    end = SDL_GetPerformanceCounter();
    prev_time = _GetPerfInterval(start, end, perf_freq);
    avg_fps = (r32)total_frames / (SDL_GetTicks() / 1000.0f);

    if (_GetPerfInterval(start, end, perf_freq) < target_time)
    {
        r64 sleep_time = target_time - prev_time;
        SDL_Delay((u32)sleep_time);
        sleep_time -= (u32)sleep_time; //truncate to value right of decimal point
        sleep_time += SDL_GetPerformanceCounter();
        while (sleep_time > SDL_GetPerformanceCounter()) {};
    }
}

Timer::Timer(void)
{
	perf_freq = SDL_GetPerformanceFrequency();
	timer = SDL_GetPerformanceCounter();
}

r64 Timer::Delta(void)
{
	return _GetPerfElapsed(timer, perf_freq);
}