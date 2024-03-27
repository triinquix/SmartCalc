#include "../main_app.h"

void create_main_menu(GtkWidget *window) {
  // Variables
  GtkWidget *box;
  GtkWidget *buttons[4];

  // Checking if window has child and removing it
  GtkWidget *child = gtk_window_get_child(GTK_WINDOW(window));
  if (child) gtk_window_set_child(GTK_WINDOW(window), NULL);

  // Box initializing
  box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
  gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
  gtk_window_set_child(GTK_WINDOW(window), box);

  // Initializing calc button

  buttons[0] = gtk_button_new_with_label("Calculator");
  g_signal_connect_swapped(buttons[0], "clicked", G_CALLBACK(open_calc),
                           window);
  gtk_box_append(GTK_BOX(box), buttons[0]);

  // Initializing graph calc

  // Initializing "Credit calculator" button
  buttons[1] = gtk_button_new_with_label("Credit calculator");
  g_signal_connect_swapped(buttons[1], "clicked", G_CALLBACK(open_credit_calc),
                           window);
  gtk_box_append(GTK_BOX(box), buttons[1]);

  // Initializing "Deposit calculator" button
  buttons[2] = gtk_button_new_with_label("Deposit calculator");
  g_signal_connect_swapped(buttons[2], "clicked", G_CALLBACK(open_deposit_calc),
                           window);
  gtk_box_append(GTK_BOX(box), buttons[2]);

  // Initializing "Quit" button
  buttons[3] = gtk_button_new_with_label("Quit");
  g_signal_connect_swapped(buttons[3], "clicked", G_CALLBACK(gtk_window_close),
                           window);
  gtk_box_append(GTK_BOX(box), buttons[3]);
}
