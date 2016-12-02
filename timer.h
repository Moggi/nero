#ifndef TIMER_INCLUDED_H
#define TIMER_INCLUDED_H

#include <SDL2/SDL.h>

class Timer
{
    public:
		Timer();

		void start();
		void stop();
		void pause();
		void unpause();

        int update();
        Uint32 deltaTime();
        void requestFrameRate(Uint32);

		//Gets the timer's time
		Uint32 getTicks();

		//Checks the status of the timer
		bool isStarted();
		bool isPaused();

    private:
		//The clock time when the timer started
		Uint32 mStartTicks;

		//The ticks stored when the timer was paused
		Uint32 mPausedTicks;

        Uint32 mFrameRate;
        Uint32 mLastFrame;
        Uint32 mDeltaTime;

		//The timer status
		bool mPaused;
		bool mStarted;

};

Timer::Timer()
{
    //Initialize the variables
    mStartTicks = 0;
    mPausedTicks = 0;

    mPaused = false;
    mStarted = false;

    mFrameRate = 0;
    mLastFrame = 0;
    mDeltaTime = 0;
}

void Timer::requestFrameRate(Uint32 frameRate)
{
    mFrameRate = frameRate;
}

int Timer::update()
{
    int deltaFrameRate = 1000.0f/mFrameRate - deltaTime();

    // Only limite if frameRate was requested
    if( mFrameRate>0 && deltaFrameRate>1 ){
        SDL_Delay( deltaFrameRate );
    }

    deltaFrameRate = deltaTime();
    mLastFrame = SDL_GetTicks();
    
    return 1000.0f/deltaFrameRate;
}

Uint32 Timer::deltaTime()
{
    return SDL_GetTicks() - mLastFrame;
}

void Timer::start()
{
    //Start the timer
    mStarted = true;

    //Unpause the timer
    mPaused = false;

    //Get the current clock time
    mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;

    mLastFrame = 0;
    mDeltaTime = 0;
}

void Timer::stop()
{
    //Stop the timer
    mStarted = false;

    //Unpause the timer
    mPaused = false;

	//Clear tick variables
	mStartTicks = 0;
	mPausedTicks = 0;
}

void Timer::pause()
{
    //If the timer is running and isn't already paused
    if( mStarted && !mPaused )
    {
        //Pause the timer
        mPaused = true;

        //Calculate the paused ticks
        mPausedTicks = SDL_GetTicks() - mStartTicks;
		mStartTicks = 0;
    }
}

void Timer::unpause()
{
    //If the timer is running and paused
    if( mStarted && mPaused )
    {
        //Unpause the timer
        mPaused = false;

        //Reset the starting ticks
        mStartTicks = SDL_GetTicks() - mPausedTicks;

        //Reset the paused ticks
        mPausedTicks = 0;
    }
}

Uint32 Timer::getTicks()
{
	//The actual timer time
	Uint32 time = 0;

    //If the timer is running
    if( mStarted )
    {
        //If the timer is paused
        if( mPaused )
        {
            //Return the number of ticks when the timer was paused
            time = mPausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            time = SDL_GetTicks() - mStartTicks;
        }
    }

    return time;
}

bool Timer::isStarted()
{
	//Timer is running and paused or unpaused
    return mStarted;
}

bool Timer::isPaused()
{
	//Timer is running and paused
    return mPaused && mStarted;
}





#endif
