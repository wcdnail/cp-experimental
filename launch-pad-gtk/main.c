#include <gtk/gtk.h>
#include "app.h"

int main(int argc, char *argv[])
{
    return g_application_run(G_APPLICATION(glaunch_pad_new()), argc, argv);
}
