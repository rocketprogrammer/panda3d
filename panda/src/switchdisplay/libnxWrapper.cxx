
extern "C" {
#include <switch/types.h>
#include <switch/services/applet.h>
#include <switch/display/native_window.h>
}

NWindow* switch_get_native_window() {
    return nwindowGetDefault();
}

bool switch_process_events() {
   u32 msg = 0;
   while (!R_FAILED(appletGetMessage(&msg))) {
       bool result = appletProcessMessage(msg);

       if (!result) {
           return false;
       }
    }
    return true;
}