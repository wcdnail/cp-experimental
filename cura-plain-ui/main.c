#include <gtk/gtk.h>
#include "glaunch-pad.h"

int main(int argc, char *argv[])
{
    return g_application_run(G_APPLICATION(glaunch_pad_new()), argc, argv);
}
