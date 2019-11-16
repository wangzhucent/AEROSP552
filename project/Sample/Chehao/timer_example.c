//
//  Program that illustrates using the timer functions with a pulse.
//
// Notes:  
//    -- Set up to be very similar to "server" example in Krten, Chapter 3
//    -- Timer sends "pulses" to this thread to periodically unblock msgReceive()
//    -- Timer connects to this thread via a channel, which must be "Created" and "Connected/attached"
//
//  Suggested start:  play around with the timer interval values (timer.it_interval) and rerun the program.
//  You should notice the printed messages appearing at a different rate.
//

#include <stdio.h>
#include <time.h>
#include <sys/siginfo.h>
#include <sys/neutrino.h>  // Defines pulse data structure (QNX)

int main ()                     
{
    timer_t             timerid;       // timer ID for timer
    struct sigevent     event;       // event to deliver
    struct itimerspec   timer;      // the timer data structure
	int chan_id, connect_id;  // Connection from timer back to this program
	struct _pulse   pulse;    // a pulse -- the "message" sent to unblock this thread
	int i;  // loop counter...

    if ((chan_id = ChannelCreate (0)) == -1) { // Open channel for timer
        printf("Error creating channel.\n");
        exit (-1);
    }

    if ((connect_id = ConnectAttach (0, 0, chan_id, 0, 0)) == -1) {     // Create a connection back to ourselves
        printf ("Error connecting the channel.\n");
        exit (-1);
    }

    // Set up the kind of event to deliver -- a pulse (a standard type)
    SIGEV_PULSE_INIT (&event, connect_id, SIGEV_PULSE_PRIO_INHERIT, 1, 0);

    // Create the timer, binding it to the pulse event
    if (timer_create (CLOCK_REALTIME, &event, &timerid) == -1) {
        printf ("Error creating timer.\n");
        exit (-1);
    }

    // Set up the timer
    timer.it_value.tv_sec = 1;          // initial delay in seconds
    timer.it_value.tv_nsec = 0;        // initial delay nanoseconds (use to store fraction of a second)
    timer.it_interval.tv_sec = 0;       // interval between pulses (seconds)
    timer.it_interval.tv_nsec = 500000000;     // interval between pulses (nanoseconds) (5E8 = 0.5 seconds)

    // Start the timer
    timer_settime (timerid, 0, &timer, NULL);

    for (i=0;i<10;i++) {
        MsgReceive (chan_id, &pulse, sizeof (pulse), NULL);
        printf("Received timer pulse %d.\n", i);
		// Note:  If this code were running much faster than 2Hz, it would be a bad idea to print
		// to the screen each time.  Printing operations are *very* slow (relatively speaking), especially 
		// since you are logged in remotely.
    }

    return (0);
}

