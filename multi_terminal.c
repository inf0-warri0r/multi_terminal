/*
*Author :Tharindra Galahena
*Project:multi-terminal
*Date   :23/05/2012
*/

#include <gtk/gtk.h>
#include <vte/vte.h> 
#include <stdlib.h>
#include <strings.h>

GtkWidget *box_vte;
GtkWidget *window_main;
GtkWidget *box_main;
GtkWidget *scale_opt;
GtkWidget *button_bcolor;
GtkWidget *button_fcolor;
int c = 1;

struct v{
	GtkWidget *vte;
	struct v *next;
	struct v *prev;
};

struct v *list = NULL;
struct v *st = NULL;

void add_vte(){
	struct v *temp = (struct v *)malloc(sizeof(struct v));
	temp-> vte = vte_terminal_new();
	vte_terminal_fork_command(VTE_TERMINAL(temp-> vte), "bash"
		, NULL, NULL, ".", FALSE, FALSE, FALSE); 
	vte_terminal_set_size(VTE_TERMINAL(temp-> vte), 80, 5);
	gtk_box_pack_start (GTK_BOX (box_vte), temp-> vte, FALSE, TRUE, 5);
	gtk_widget_show (temp-> vte);
	temp -> next = list -> next;
	(list -> next) -> prev = temp;
	list -> next = temp;
	temp -> prev = list;
	list = list -> next;
	c++;
}
void remove_vte(){
	if (c > 1){
		if(list == st) list = list -> prev;
		gtk_container_remove (GTK_CONTAINER (box_vte), st -> vte);
		st = st -> next;
		st -> prev = (st -> prev) -> prev;
		(st -> prev) -> next = st;
		vte_terminal_set_size(VTE_TERMINAL(st-> vte), 80, 20);
		c--;
		gtk_window_resize (GTK_WINDOW(window_main), 20, 20);
	}
}
void switch_vte(){
	if (c > 1){
		vte_terminal_set_size(VTE_TERMINAL(st-> vte), 80, 5);
		st = st -> next;
		vte_terminal_set_size(VTE_TERMINAL(st-> vte), 80, 20);
	}
}
void save(){
	GdkColor bcolor;
	gtk_color_button_get_color(GTK_COLOR_BUTTON(button_bcolor), &bcolor);
	vte_terminal_set_color_background(VTE_TERMINAL(st-> vte), &bcolor);
	
	GdkColor fcolor;
	gtk_color_button_get_color(GTK_COLOR_BUTTON(button_fcolor), &fcolor);
	vte_terminal_set_color_foreground(VTE_TERMINAL(st-> vte), &fcolor);
	
	//vte_terminal_set_background_transparent(VTE_TERMINAL(st-> vte), TRUE);
}
void settings(){
	GtkWidget *window_settings;
	
	//GtkWidget *box_main;
	GtkWidget *box_buttons;
	GtkWidget *box_sliders;
	
	GtkWidget* button_save;
	GtkWidget* button_close;
	
	window_settings = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	box_main = gtk_vbox_new(FALSE, 0);
	box_buttons = gtk_hbox_new(FALSE, 0);
	box_sliders = gtk_vbox_new(FALSE, 0);
	
	button_bcolor = gtk_color_button_new();
	gtk_box_pack_start (GTK_BOX (box_sliders), button_bcolor, TRUE, FALSE, 5);
	
	button_fcolor = gtk_color_button_new();
	gtk_box_pack_start (GTK_BOX (box_sliders), button_fcolor, TRUE, FALSE, 5);
	//scale_opt = gtk_hscale_new_with_range(0, 65535, 10);
	
	//gtk_box_pack_start (GTK_BOX (box_sliders), scale_opt, TRUE, FALSE, 5);
	gtk_box_pack_start (GTK_BOX (box_main), box_sliders, TRUE, FALSE, 5);
	
	button_save = gtk_button_new_with_label("Save");
	button_close = gtk_button_new_with_label("Close");
	gtk_signal_connect (GTK_OBJECT (button_save), "clicked",
					GTK_SIGNAL_FUNC (save), NULL);
	gtk_box_pack_start (GTK_BOX (box_buttons), button_save, TRUE, FALSE, 5);
	gtk_signal_connect (GTK_OBJECT (button_close), "clicked",
					GTK_SIGNAL_FUNC (remove_vte), NULL);
	gtk_box_pack_start (GTK_BOX (box_buttons), button_close, TRUE, FALSE, 5);
	
	gtk_box_pack_start (GTK_BOX (box_main), box_buttons, TRUE, FALSE, 5);
	gtk_container_add (GTK_CONTAINER (window_settings), box_main);
	
	gtk_widget_show (box_sliders);
	gtk_widget_show (button_bcolor);
	gtk_widget_show (button_fcolor);
	//gtk_widget_show (scale_opt);
	gtk_widget_show (button_save);
	gtk_widget_show (button_close);
	gtk_widget_show (box_buttons);
	gtk_widget_show (box_main);
	gtk_widget_show (window_settings);
}
int main(int argc, char **argv){
	
	//GtkWidget *window_main;
	
	//GtkWidget *box_main;
	GtkWidget *box_buttons;
	
	GtkWidget* button_new;
	GtkWidget* button_rem;
	GtkWidget* button_switch;
	GtkWidget* button_settings;
	
	gtk_init(&argc, &argv);
	window_main = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	
	box_main = gtk_vbox_new (FALSE, 0);
	box_buttons = gtk_hbox_new (FALSE, 0);
	box_vte = gtk_vbox_new (FALSE, 0);
	
	button_new = gtk_button_new_with_label ("new terminal");
	button_rem = gtk_button_new_with_label ("remove terminal");
	button_switch = gtk_button_new_with_label ("switch >>");
	button_settings = gtk_button_new_with_label ("settings");
	
	list = (struct v*)malloc(sizeof(struct v));
	list -> next = list;
	list -> prev = list;
	st = list;
	st -> vte = vte_terminal_new();

	vte_terminal_fork_command(VTE_TERMINAL(st -> vte), "bash", 
		NULL, NULL, ".", FALSE, FALSE, FALSE); 
	vte_terminal_set_size(VTE_TERMINAL(st -> vte), 80,20);
	
	gtk_signal_connect (GTK_OBJECT (button_new), "clicked",
					GTK_SIGNAL_FUNC (add_vte), NULL);
	gtk_box_pack_start (GTK_BOX (box_buttons), button_new, TRUE, FALSE, 5);
	gtk_widget_show (button_new);
	
	gtk_signal_connect (GTK_OBJECT (button_rem), "clicked",
					GTK_SIGNAL_FUNC (remove_vte), NULL);
	gtk_box_pack_start (GTK_BOX (box_buttons), button_rem, TRUE, FALSE, 5);
	gtk_widget_show (button_rem);
	
	gtk_signal_connect (GTK_OBJECT (button_switch), "clicked",
					GTK_SIGNAL_FUNC (switch_vte), NULL);
	gtk_box_pack_start (GTK_BOX (box_buttons), button_switch, TRUE, FALSE, 5);
	gtk_widget_show (button_switch);
	gtk_signal_connect (GTK_OBJECT (button_settings), "clicked",
					GTK_SIGNAL_FUNC (settings), NULL);
	gtk_box_pack_start (GTK_BOX (box_buttons), button_settings, TRUE, FALSE, 5);
	gtk_widget_show (button_settings);

	gtk_box_pack_start (GTK_BOX (box_vte), st -> vte, FALSE, TRUE, 5);
	
	gtk_widget_show (st -> vte);
	gtk_box_pack_start(GTK_BOX (box_main), box_buttons, FALSE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX (box_main), box_vte, FALSE, TRUE, 5);
	gtk_container_add(GTK_CONTAINER (window_main), box_main);
	gtk_widget_show(box_main);
	gtk_widget_show(box_buttons);
	gtk_widget_show(box_vte);
	gtk_widget_show(window_main);
	gtk_main();
	return 0;
}
