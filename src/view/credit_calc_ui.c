#include "../main_app.h"

// !!! Add another functionalities
void clear_ouput(GtkWidget *grid) {
  for (int i = 2; i < 4; i++) {
    for (int j = 0; j < 3; j++) {
      GtkWidget *child = gtk_grid_get_child_at(GTK_GRID(grid), i, j);
      if (child) gtk_grid_remove(GTK_GRID(grid), child);
    }
  }
  while (1) {
    GtkWidget *child = gtk_grid_get_child_at(GTK_GRID(grid), 0, 5);
    if (child)
      gtk_grid_remove_row(GTK_GRID(grid), 5);
    else
      break;
  }
}

void create_output_widgets(GtkWidget *grid, credit *credit_data,
                           enum credit_type type) {
  if (type == ANNUITY) {
    GtkWidget *mon_pay[2];
    mon_pay[0] = gtk_label_new_with_mnemonic("Monthly payment");
    gtk_grid_attach(GTK_GRID(grid), mon_pay[0], 2, 0, 1, 1);
    char mon_pay_str[100];
    sprintf(mon_pay_str, "%.2lf", credit_data->payments[0]);
    mon_pay[1] = gtk_label_new_with_mnemonic(mon_pay_str);
    gtk_grid_attach(GTK_GRID(grid), mon_pay[1], 3, 0, 1, 1);
  }
  // Variables
  GtkWidget *output_names[2];
  GtkWidget *outputs[2];

  // Initializing output names
  output_names[0] = gtk_label_new_with_mnemonic("Overpayment");
  gtk_grid_attach(GTK_GRID(grid), output_names[0], 2, 1 - type, 1, 1);
  output_names[1] = gtk_label_new_with_mnemonic("Total payment");
  gtk_grid_attach(GTK_GRID(grid), output_names[1], 2, 2 - type, 1, 1);

  // Initializing ouputs
  char overpay_str[50];
  sprintf(overpay_str, "%.2lf", credit_data->overpayment);
  outputs[0] = gtk_label_new_with_mnemonic(overpay_str);
  gtk_grid_attach(GTK_GRID(grid), outputs[0], 3, 1 - type, 1, 1);
  char total_pay_str[50];
  sprintf(total_pay_str, "%.2lf", credit_data->total_payment);
  outputs[1] = gtk_label_new_with_mnemonic(total_pay_str);
  gtk_grid_attach(GTK_GRID(grid), outputs[1], 3, 2 - type, 1, 1);
}

void init_table(GtkWidget *grid, credit *credit_data) {
  //  headers
  GtkWidget *headers[6];

  // Initializing headers
  headers[0] = gtk_label_new_with_mnemonic("№");
  gtk_grid_attach(GTK_GRID(grid), headers[0], 0, 5, 1, 1);
  headers[1] = gtk_label_new_with_mnemonic("Month");
  gtk_grid_attach(GTK_GRID(grid), headers[1], 1, 5, 1, 1);
  headers[2] = gtk_label_new_with_mnemonic("Payment");
  gtk_grid_attach(GTK_GRID(grid), headers[2], 2, 5, 1, 1);
  headers[3] = gtk_label_new_with_mnemonic("Payment for principal loan");
  gtk_grid_attach(GTK_GRID(grid), headers[3], 3, 5, 1, 1);
  headers[4] = gtk_label_new_with_mnemonic("Payment for interest");
  gtk_grid_attach(GTK_GRID(grid), headers[4], 4, 5, 1, 1);
  headers[5] = gtk_label_new_with_mnemonic("Residual");
  gtk_grid_attach(GTK_GRID(grid), headers[5], 5, 5, 1, 1);

  for (int i = 0; i < credit_data->number_of_periods; i++) {
    // One row
    GtkWidget *row[6];

    char tmp_str[5][100];
    sprintf(tmp_str[0], "%d", i + 1);
    row[0] = gtk_label_new_with_mnemonic(tmp_str[0]);
    gtk_grid_attach(GTK_GRID(grid), row[0], 0, 6 + i, 1, 1);
    row[1] = gtk_label_new_with_mnemonic(credit_data->dates[i]);
    gtk_grid_attach(GTK_GRID(grid), row[1], 1, 6 + i, 1, 1);
    sprintf(tmp_str[1], "%.2lf", credit_data->payments[i]);
    row[2] = gtk_label_new_with_mnemonic(tmp_str[1]);
    gtk_grid_attach(GTK_GRID(grid), row[2], 2, 6 + i, 1, 1);
    sprintf(tmp_str[2], "%.2lf", credit_data->payments_for_principal_loan[i]);
    row[3] = gtk_label_new_with_mnemonic(tmp_str[2]);
    gtk_grid_attach(GTK_GRID(grid), row[3], 3, 6 + i, 1, 1);
    sprintf(tmp_str[3], "%.2lf", credit_data->payments_for_interest[i]);
    row[4] = gtk_label_new_with_mnemonic(tmp_str[3]);
    gtk_grid_attach(GTK_GRID(grid), row[4], 4, 6 + i, 1, 1);
    sprintf(tmp_str[4], "%.2lf", credit_data->residuals[i]);
    row[5] = gtk_label_new_with_mnemonic(tmp_str[4]);
    gtk_grid_attach(GTK_GRID(grid), row[5], 5, 6 + i, 1, 1);
  }
}

void update_view(GtkWidget *grid, credit *credit_data, enum credit_type type) {
  create_output_widgets(grid, credit_data, type);
  init_table(grid, credit_data);
}

void data_to_controller(GtkWidget *grid) {
  // Delete previous output
  clear_ouput(grid);
  char *strings_to_check[3];
  for (int i = 0; i < 3; i++) {
    GtkWidget *child = gtk_grid_get_child_at(GTK_GRID(grid), 1, i);
    strings_to_check[i] = (char *)gtk_editable_get_text(GTK_EDITABLE(child));
  }
  enum credit_type type;
  GtkWidget *credit_type_switcher = gtk_grid_get_child_at(GTK_GRID(grid), 1, 3);
  int position =
      gtk_drop_down_get_selected(GTK_DROP_DOWN(credit_type_switcher));
  if (position)
    type = DIFFERENTIATED;
  else
    type = ANNUITY;
  credit *credit_data = process_data(strings_to_check, type);
  if (credit_data) {
    update_view(grid, credit_data, type);
    free_credit_data(credit_data);
  } else
    for (int i = 0; i < 3; i++)
      if (strcmp(strings_to_check[i], "") == 0) show_error_msg(grid, i, 2);
}

void init_credit_field_names(GtkWidget *field_names[4], GtkWidget *grid) {
  // "Total amount" label
  field_names[0] = gtk_label_new_with_mnemonic("Total credit amount");

  // "Term" label
  field_names[1] = gtk_label_new_with_mnemonic("Term, y");

  // "Rate" label
  field_names[2] = gtk_label_new_with_mnemonic("Interest rate, %");

  // "Type" label
  field_names[3] = gtk_label_new_with_mnemonic("Type");

  for (int i = 0; i < 4; i++) {
    gtk_widget_set_halign(field_names[i], GTK_ALIGN_END);
    gtk_grid_attach(GTK_GRID(grid), field_names[i], 0, i, 1, 1);
  }
}

void init_credit_inputs(GtkWidget *inputs[3], GtkWidget *grid) {
  for (int i = 0; i < 3; i++) {
    inputs[i] = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), inputs[i], 1, i, 1, 1);
  }
}

void open_credit_calc(GtkWidget *window) {
  // Variables
  GtkWidget *scrolled_window;
  GtkWidget *grid;
  GtkWidget *buttons[2];
  GtkWidget *field_names[4];
  GtkWidget *inputs[3];
  GtkWidget *credit_type;

  // Removing main menu from window
  gtk_window_set_child(GTK_WINDOW(window), NULL);

  // Initializing scrolled window
  scrolled_window = gtk_scrolled_window_new();
  gtk_window_set_child(GTK_WINDOW(window), scrolled_window);

  // Grid initializing
  grid = init_grid(scrolled_window);

  // Initialize field_names
  init_credit_field_names(field_names, grid);

  // Initialize inputs
  init_credit_inputs(inputs, grid);

  // Credit type switcher
  credit_type = gtk_drop_down_new_from_strings(
      (const char *[]){"Annuity", "Differentiated", NULL});
  gtk_grid_attach(GTK_GRID(grid), credit_type, 1, 3, 1, 1);

  // Initializing calc button
  buttons[0] = gtk_button_new_with_label("Calculate");
  gtk_grid_attach(GTK_GRID(grid), buttons[0], 1, 4, 1, 1);
  g_signal_connect_swapped(buttons[0], "clicked",
                           G_CALLBACK(data_to_controller), grid);

  // Initializing "Back" button
  buttons[1] = gtk_button_new_with_label("Back");
  g_signal_connect_swapped(buttons[1], "clicked", G_CALLBACK(create_main_menu),
                           window);
  gtk_grid_attach(GTK_GRID(grid), buttons[1], 0, 4, 1, 1);
}