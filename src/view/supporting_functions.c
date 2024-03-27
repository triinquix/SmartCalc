#include "../main_app.h"

GtkWidget *init_grid(GtkWidget *scrolled_window) {
  GtkWidget *grid = gtk_grid_new();
  gtk_widget_set_valign(grid, GTK_ALIGN_START);
  gtk_widget_set_halign(grid, GTK_ALIGN_START);
  gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 1);
  gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
  gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), grid);
  return grid;
}

void show_error_msg(GtkWidget *grid, int row, int column) {
  GtkWidget *error_msg = gtk_label_new_with_mnemonic("Incorrect input");
  gtk_grid_attach(GTK_GRID(grid), error_msg, column, row, 1, 1);
}