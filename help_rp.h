#ifndef HELP_RP_H_INCLUDED
#define HELP_RP_H_INCLUDED


#include "myheaders.h"
#include "main_headers.h"

typedef struct
{
    WindowObject *win;
    BoxObject *help_box;
    TableObject *help_table;
    ButtonObject *help_leave;
    ImageObject *help_image;
} help_rp;


help_rp *init_help_win()
{
    help_rp *helpwin=(help_rp*)malloc(sizeof(help_rp));
    if(!helpwin)
    {
        printf("erreur d'allocation !");
        exit(-1);
    }
    helpwin->win=init_window("HELP",300,300,2,0,0,1);
    helpwin->win=create_window(helpwin->win);

    helpwin->help_box=init_box(helpwin->win->window,"boxhelp",'h',1,0);
    helpwin->help_box=create_box(helpwin->help_box);
    add_to_window(helpwin->win,helpwin->help_box->box);

    helpwin->help_table=init_table(helpwin->help_box->box,10,10,1);
    helpwin->help_table=create_table(helpwin->help_table);
    add_to_box_end(helpwin->help_box,helpwin->help_table->table,1,1,0);


    helpwin->help_image=init_image("images/help.png","help",0,0);
    helpwin->help_image=create_image(helpwin->help_image);
    add_to_table(helpwin->help_table,helpwin->help_image->image,0,10,0,9);

    helpwin->help_leave=init_button(helpwin->help_table->table,"leave","leave","images/return.png");
    helpwin->help_leave=create_button(helpwin->help_leave);
    add_to_table(helpwin->help_table,helpwin->help_leave->button,6,9,9,10);

    return ((help_rp*)helpwin);
}



void event_calls_help_rp(help_rp *rpwin)
{
    g_signal_connect(G_OBJECT(rpwin->win->window),"destroy",G_CALLBACK(gtk_main_quit),NULL);
}

#endif // HELP_RP_H_INCLUDED
