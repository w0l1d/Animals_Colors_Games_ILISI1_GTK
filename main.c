#include "main_headers.h"

int main (int argc, char *argv[])
{
   init_app(argc,argv);
   interface *test;
   test=init_interface();
   event_calls_main_interface(test);
   display_all(test->W_GAMES->window);

  return 0;
}

