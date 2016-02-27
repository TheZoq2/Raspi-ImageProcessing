#include "Tracker.h"

Tracker tracker;

int main()
{
    tracker.setup();

    while(!tracker.isDone())
    {
        tracker.loop();
    }
}
