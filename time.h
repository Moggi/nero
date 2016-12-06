#ifndef TIME_INCLUDED_H
#define TIME_INCLUDED_H

#include <SDL2/SDL.h>

class Time
{
    public:
		Time();

		void start();
		void stop();
		void pause();
		void unpause();

        void showFPS(bool);
        double fps();
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
        Uint32 mBeforeDelay;
        Uint32 mAfterDelay;
        Uint32 mTotalTime;
        double mAvgFPS;
        double mAverageDeltaTime;

		bool mShowFPS;
		bool mPaused;
		bool mStarted;

        Uint32 deltaTimeNotDelayed();
};

Time::Time()
{
    mShowFPS = false;
    mStartTicks = 0;
    mPausedTicks = 0;

    mPaused = false;
    mStarted = false;

    mAvgFPS = 0.0f;
    mFrameRate = 0;
    mAverageDeltaTime = 0.0f;
    mTotalTime = SDL_GetTicks();
    mAfterDelay = SDL_GetTicks();
    mBeforeDelay = SDL_GetTicks();
}

void Time::showFPS(bool showFps)
{
    mShowFPS = showFps;
}

void Time::start()
{
    //Start the timer
    mStarted = true;

    //Unpause the timer
    mPaused = false;

    //Get the current clock time
    mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;

    mTotalTime = SDL_GetTicks();
    mAfterDelay = SDL_GetTicks();
    mBeforeDelay = SDL_GetTicks();
}

void Time::stop()
{
    //Stop the timer
    mStarted = false;

    //Unpause the timer
    mPaused = false;

	//Clear tick variables
	mStartTicks = 0;
	mPausedTicks = 0;
}

void Time::requestFrameRate(Uint32 frameRate)
{
    mFrameRate = frameRate;
}

int Time::update()
{
    int deltaFrameRate = (1000.0f/mFrameRate - deltaTimeNotDelayed())-0.5;

    // To see ticks delayed
    mBeforeDelay = SDL_GetTicks();

    // Only limite if frameRate was requested
    if( mFrameRate>0 && deltaFrameRate>1 ){
        SDL_Delay( deltaFrameRate );
    }

    // Draw time with delay
    deltaFrameRate = deltaTimeNotDelayed();

    // Reset draw time for the next draw
    mAfterDelay = SDL_GetTicks();

    mAverageDeltaTime = (mAverageDeltaTime + deltaFrameRate)/2;
    if( mShowFPS )
        fps();

    return deltaFrameRate;
}

double Time::fps()
{
    int timeElapsed = SDL_GetTicks() - mTotalTime;
    if( timeElapsed >= 1000 )
    {
        mAvgFPS = (double)timeElapsed / mAverageDeltaTime;
        std::cout << "Time elapsed: " << timeElapsed << "ms\n";
        std::cout << "Average time between frames: " << mAverageDeltaTime << "ms\n";
        std::cout << "Current FPS: " << mAvgFPS << "\n";
        std::cout << "==========================================\n";
        mTotalTime = SDL_GetTicks();
    }

    return mAvgFPS;
}

Uint32 Time::deltaTime()
{
    return SDL_GetTicks() - mBeforeDelay;
}

Uint32 Time::deltaTimeNotDelayed()
{
    return SDL_GetTicks() - mAfterDelay;
}

void Time::pause()
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

void Time::unpause()
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

Uint32 Time::getTicks()
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

bool Time::isStarted()
{
	//Time is running and paused or unpaused
    return mStarted;
}

bool Time::isPaused()
{
	//Time is running and paused
    return mPaused && mStarted;
}





#endif
