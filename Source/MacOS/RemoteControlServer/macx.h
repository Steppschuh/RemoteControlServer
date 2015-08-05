#import <CoreServices/CoreServices.h>

static io_connect_t get_event_driver (void);

void HIDPostAuxKey (const UInt8 auxKeyCode);

//kAERestart        will cause system to restart
//kAEShutDown       will cause system to shutdown
//kAEReallyLogout   will cause system to logout
//kAESleep          will cause system to sleep
extern OSStatus MDSendAppleEventToSystemProcess(AEEventID eventToSend);
