#include <gtk/gtk.h>
#include "filemanip.h"

guchar* fileRead(const char *pathname, gssize *readed)
{
    gssize           readv = 0;
    goffset       filesize = 0;
    guchar        *filebuf = NULL;
    const char *errorTitle = "unknown";
    GError          *error = NULL;
    GFileInfo        *info = NULL;
    GFile            *file = g_file_new_for_path(pathname);
    GFileInputStream *istm = g_file_read(file, NULL, &error);
    if (!file) {
        errorTitle = "init";
        goto onError;
    }
    if (!istm) {
        errorTitle = "read";
        goto onError;
    }
    info = g_file_input_stream_query_info(G_FILE_INPUT_STREAM(istm), G_FILE_ATTRIBUTE_STANDARD_SIZE, NULL, &error);
    if (!info) {
        errorTitle = "input stream query info";
        goto onError;
    } 
    if (!g_file_info_has_attribute(info, G_FILE_ATTRIBUTE_STANDARD_SIZE)) {
        errorTitle = "info standart size";
        goto onError;
    }
    filesize = g_file_info_get_size(info);
    if (filesize > 0){
        filebuf = (guchar*)g_malloc(filesize);
        if (!filebuf) {
            errorTitle = "allocating buffer";
            goto onError;
        }
        memset(filebuf, 0, filesize);
        readv = g_input_stream_read(G_INPUT_STREAM(istm), filebuf, filesize, NULL, &error);
        if (readv == -1 || readv != filesize) {
            errorTitle = "input stream read";
            goto onError;
        }
        (*readed) = readv;
    }
    goto noError;
onError:
    g_print("FILEMANIP [%s] %s ERROR: [%d] %s\n", 
        pathname, 
        errorTitle,
        error ? error->code : errno, 
        error ? error->message : strerror(errno));
    if (filebuf) {
        g_free(filebuf);
        filebuf = NULL;
    }
noError:        
    if (info) {
        g_object_unref(info);
    }
    if (istm) {
        g_object_unref(istm);
    }
    if (file) {
        g_object_unref(file);
    }
    return filebuf;
}
