#ifndef LTIMER_H
#define LTIMER_H
#include <SDL.h>

/**
Multi-purpose [pseudo] timer (doesn't really tick).
*/
class LTimer{
    public:
		LTimer();

		void start();
		void stop();
		void pause();
		void unpause();

		Uint32 getTicks();

		void reset();

		bool isStarted();
		bool isPaused();

    private:
		Uint32 mStartTicks;

		Uint32 mPausedTicks;

		bool mPaused;
		bool mStarted;
};

#endif // LTIMER_H
