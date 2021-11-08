#ifndef LIBNXWRAPPER_H
#define LIBNXWRAPPER_H

struct NWindow;

NWindow* switch_get_native_window();
bool switch_process_events();

#endif /* LIBNXWRAPPER_H */