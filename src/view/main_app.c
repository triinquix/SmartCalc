#include "../main_app.h"

void activate_app(GtkApplication *app) {
  // Variables
  GtkWidget *window;

  // Creating main window
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Smart Calculator");
  gtk_window_set_default_size(GTK_WINDOW(window), 1500, 1000);

  // Creating main menu
  create_main_menu(window);

  // Showing main window
  gtk_widget_show(window);
}

int main(int argc, char **argv) {
  // Variables
  GtkApplication *app;
  int status;

  // Running main application
  app = gtk_application_new("calc.app", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate_app), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);
  return status;
}