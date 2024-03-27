#include "../main_app.h"

void init_deposit_field_names(GtkWidget* grid, GtkWidget* field_names[8]) {
  // Initialize field names
  field_names[0] = gtk_label_new_with_mnemonic("Deposit amount");
  field_names[1] = gtk_label_new_with_mnemonic("Deposit term, y");
  field_names[2] = gtk_label_new_with_mnemonic("Interest rate, %");
  field_names[3] = gtk_label_new_with_mnemonic("Tax rate, %");
  field_names[4] = gtk_label_new_with_mnemonic("Capitalization of interest");
  field_names[5] = gtk_label_new_with_mnemonic("Periodicity of payments");
  field_names[6] = gtk_label_new_with_mnemonic("Replenishments");
  field_names[7] = gtk_label_new_with_mnemonic("Partial withdrawals");

  // Attaching them to grid
  for (int i = 0; i < 8; i++) {
    gtk_widget_set_halign(field_names[i], GTK_ALIGN_END);
    gtk_grid_attach(GTK_GRID(grid), field_names[i], 0, i, 1, 1);
  }
}

void init_deposit_inputs(GtkWidget* grid, GtkWidget* inputs[4]) {
  for (int i = 0; i < 4; i++) {
    inputs[i] = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), inputs[i], 1, i, 1, 1);
  }
}

void create_deposit_drop_down(GtkWidget* grid, enum deposit_type type) {
  // Removing previous frop_down, if it exists
  GtkWidget* drop_down = gtk_grid_get_child_at(GTK_GRID(grid), 1, 5);
  if (drop_down) gtk_grid_remove(GTK_GRID(grid), drop_down);

  if (type == NO_CAP)
    drop_down = gtk_drop_down_new_from_strings(
        (const char*[]){"At the end of the term", "Daily", "Monthly",
                        "Quarterly", "Half-yearly", "Annually", NULL});
  else
    drop_down = gtk_drop_down_new_from_strings((const char*[]){
        "Daily", "Monthly", "Quarterly", "Half-yearly", "Annually", NULL});
  gtk_grid_attach(GTK_GRID(grid), drop_down, 1, 5, 1, 1);
}

void change_cap(GtkWidget* grid) {
  GtkWidget* cap_check = gtk_grid_get_child_at(GTK_GRID(grid), 1, 4);
  GtkWidget* label_to_change = gtk_grid_get_child_at(GTK_GRID(grid), 0, 5);
  if (gtk_check_button_get_active(GTK_CHECK_BUTTON(cap_check))) {
    gtk_label_set_label(GTK_LABEL(label_to_change),
                        "Periodicity of capitalization");
    create_deposit_drop_down(grid, INTEREST_CAP);
  } else {
    gtk_label_set_label(GTK_LABEL(label_to_change), "Periodicity of payments");
    create_deposit_drop_down(grid, NO_CAP);
  }
}

int get_button_row(GtkWidget* button, GtkWidget* grid) {
  int res = 6;
  while (1) {
    GtkWidget* child = gtk_grid_get_child_at(GTK_GRID(grid), 1, res);
    if (child == button)
      break;
    else
      res++;
  }
  return res;
}

// Rewrite
void delete_additional_row(GtkWidget* button, gpointer grid) {
  int row = 6;
  while (1) {
    GtkWidget* tmp_button = gtk_grid_get_child_at(GTK_GRID(grid), 3, row);
    if (button == tmp_button) {
      gtk_grid_remove_row(GTK_GRID(grid), row);
      break;
    } else
      row++;
  }
}

void add_additional_row(GtkWidget* button, gpointer grid) {
  // Variables
  GtkWidget* inputs[2];
  GtkWidget* del_button;
  // Find index of button
  int row = get_button_row(button, grid);
  gtk_grid_insert_row(GTK_GRID(grid), row);

  // Initializing additional widgets
  inputs[0] = gtk_entry_new();
  gtk_editable_set_text(GTK_EDITABLE(inputs[0]), "dd.mm.yyyy");
  gtk_grid_attach(GTK_GRID(grid), inputs[0], 1, row, 1, 1);
  inputs[1] = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), inputs[1], 2, row, 1, 1);
  del_button = gtk_button_new_from_icon_name("edit-delete");
  gtk_grid_attach(GTK_GRID(grid), del_button, 3, row, 1, 1);
  g_signal_connect(del_button, "clicked", G_CALLBACK(delete_additional_row),
                   grid);
}

enum periodicity get_periodicity(GtkWidget* grid, enum deposit_type type) {
  enum periodicity period;
  GtkWidget* drop_down = gtk_grid_get_child_at(GTK_GRID(grid), 1, 5);
  int position = gtk_drop_down_get_selected(GTK_DROP_DOWN(drop_down));
  if (type == NO_CAP) {
    switch (position) {
      case 0:
        period = END;
        break;
      case 1:
        period = DAILY;
        break;
      case 2:
        period = MONTHLY;
        break;
      case 3:
        period = QUARTERLY;
        break;
      case 4:
        period = HALF;
        break;
      case 5:
        period = ANNUALLY;
        break;
    }
  } else {
    switch (position) {
      case 0:
        period = DAILY;
        break;
      case 1:
        period = MONTHLY;
        break;
      case 2:
        period = QUARTERLY;
        break;
      case 3:
        period = HALF;
        break;
      case 4:
        period = ANNUALLY;
        break;
    }
  }
  return period;
}

int get_number_of_replanishments(GtkWidget* grid) {
  int row = 6;
  while (1) {
    GtkWidget* child = gtk_grid_get_child_at(GTK_GRID(grid), 0, row);
    if (child)
      break;
    else
      row++;
  }
  return row - 6;
}

char*** get_replanishments(GtkWidget* grid, int* number_of_replanishments) {
  // Getting number of replanishments
  *number_of_replanishments = get_number_of_replanishments(grid);
  char*** replanishments;
  if (number_of_replanishments) {
    replanishments =
        (char***)malloc(sizeof(char**) * (*number_of_replanishments));
    for (int i = 0; i < *number_of_replanishments; i++) {
      replanishments[i] = (char**)malloc(sizeof(char*) * 2);
      for (int j = 1; j < 3; j++) {
        GtkWidget* child = gtk_grid_get_child_at(GTK_GRID(grid), j, i + 6);
        char* str = (char*)gtk_editable_get_text(GTK_EDITABLE(child));
        replanishments[i][j - 1] = str;
        // strcpy(replanishments[i][j - 1], str);
      }
    }
  } else
    replanishments = NULL;
  return replanishments;
}

int get_number_of_withdrawals(GtkWidget* grid, int row) {
  int res = row;
  while (1) {
    GtkWidget* child = gtk_grid_get_child_at(GTK_GRID(grid), 0, res);
    if (child)
      break;
    else
      res++;
  }
  return res - row;
}

int get_row_withdrawals(GtkWidget* grid) {
  int row = 6;
  while (1) {
    GtkWidget* child = gtk_grid_get_child_at(GTK_GRID(grid), 0, row);
    if (child)
      break;
    else
      row++;
  }
  row++;
  return row;
}

char*** get_withdrawals(GtkWidget* grid, int* number_of_withdrawals) {
  // Finding row with label "replanishments"
  int row = get_row_withdrawals(grid);
  *number_of_withdrawals = get_number_of_withdrawals(grid, row);
  char*** withdrawals;
  if (number_of_withdrawals) {
    withdrawals = (char***)malloc(sizeof(char**) * (*number_of_withdrawals));
    for (int i = 0; i < *number_of_withdrawals; i++) {
      withdrawals[i] = (char**)malloc(sizeof(char*) * 2);
      for (int j = 1; j < 3; j++) {
        GtkWidget* child = gtk_grid_get_child_at(GTK_GRID(grid), j, i + row);
        char* str = (char*)gtk_editable_get_text(GTK_EDITABLE(child));
        withdrawals[i][j - 1] = str;
        // strcpy(withdrawals[i][j - 1], str);
      }
    }
  } else
    withdrawals = NULL;
  return withdrawals;
}

void clear_deposit_output(GtkWidget* grid) {
  for (int i = 0; i < 4; i++) {
    GtkWidget* child = gtk_grid_get_child_at(GTK_GRID(grid), 2, i);
    if (child) gtk_grid_remove(GTK_GRID(grid), child);
  }
  for (int i = 0; i < 4; i++) {
    GtkWidget* child = gtk_grid_get_child_at(GTK_GRID(grid), 3, i);
    if (child) gtk_grid_remove(GTK_GRID(grid), child);
  }
  for (int i = 0; i < 6; i++) gtk_grid_remove_column(GTK_GRID(grid), 4);
}

void free_additional_rows(char*** replanishments, int num_of_replanishments,
                          char*** withdrawals, int num_of_withdrawals) {
  if (num_of_replanishments) {
    for (int i = 0; i < num_of_replanishments; i++) {
      if (replanishments[i]) {
        free(replanishments[i]);
        replanishments[i] = NULL;
      }
    }
    free(replanishments);
  }
  if (withdrawals) {
    for (int i = 0; i < num_of_withdrawals; i++) {
      if (withdrawals[i]) {
        free(withdrawals[i]);
        withdrawals[i] = NULL;
      }
    }
    free(withdrawals);
    withdrawals = NULL;
  }
}

void init_deposit_outputs(GtkWidget* grid, deposit* deposit_data) {
  GtkWidget* output_names[3];
  output_names[0] = gtk_label_new_with_mnemonic("Accrued interest");
  output_names[1] = gtk_label_new_with_mnemonic("Deposit amount");
  output_names[2] = gtk_label_new_with_mnemonic("Tax");
  GtkWidget* outputs[3];
  char interest[100];
  sprintf(interest, "%.2lf", deposit_data->accrued_interest);
  outputs[0] = gtk_label_new_with_mnemonic(interest);
  char dep_amount[100];
  sprintf(dep_amount, "%.2lf", deposit_data->deposit_amount);
  outputs[1] = gtk_label_new_with_mnemonic(dep_amount);
  char tax[100];
  sprintf(tax, "%.2lf", deposit_data->tax);
  outputs[2] = gtk_label_new_with_mnemonic(tax);
  for (int i = 0; i < 3; i++) {
    gtk_grid_attach(GTK_GRID(grid), output_names[i], 2, i, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), outputs[i], 3, i, 1, 1);
  }
}

void init_deposit_table(GtkWidget* grid, deposit* deposit_data) {
  GtkWidget* column_names[5];
  column_names[0] = gtk_label_new_with_mnemonic("Dates");
  column_names[1] = gtk_label_new_with_mnemonic("Interest");
  column_names[2] = gtk_label_new_with_mnemonic("Balance change");
  column_names[3] = gtk_label_new_with_mnemonic("Payments");
  column_names[4] = gtk_label_new_with_mnemonic("Balance");
  for (int i = 0; i < 5; i++)
    gtk_grid_attach(GTK_GRID(grid), column_names[i], 5 + i, 0, 1, 1);
  for (int i = 0; i < deposit_data->number_of_rows; i++) {
    GtkWidget* table_columns[5];
    table_columns[0] = gtk_label_new_with_mnemonic(deposit_data->dates[i]);
    char interest[100];
    sprintf(interest, "%.2lf", deposit_data->interest[i]);
    table_columns[1] = gtk_label_new_with_mnemonic(interest);
    char balance_change[100];
    sprintf(balance_change, "%.2lf", deposit_data->balance_change[i]);
    table_columns[2] = gtk_label_new_with_mnemonic(balance_change);
    char payments[100];
    sprintf(payments, "%.2lf", deposit_data->payments[i]);
    table_columns[3] = gtk_label_new_with_mnemonic(payments);
    char balance[100];
    sprintf(balance, "%.2lf", deposit_data->balance[i]);
    table_columns[4] = gtk_label_new_with_mnemonic(balance);
    for (int j = 0; j < 5; j++)
      gtk_grid_attach(GTK_GRID(grid), table_columns[j], 5 + j, i + 1, 1, 1);
  }
}

void handle_incorrect_data(GtkWidget* grid, char* inputs[4],
                           char*** replanishments, int number_of_replanishments,
                           char*** withdrawals, int number_of_withdrawals) {
  // Checking inputs
  for (int i = 0; i < 4; i++)
    if (strcmp(inputs[i], "") == 0) show_error_msg(grid, i, 2);
  // Checking replanishments
  for (int i = 0; i < number_of_replanishments; i++)
    if (strcmp(replanishments[i][0], "") == 0 ||
        strcmp(replanishments[i][1], "") == 0)
      show_error_msg(grid, 6 + i, 4);

  // Checking withdrawals
  int row = get_row_withdrawals(grid);
  for (int i = 0; i < number_of_withdrawals; i++)
    if (strcmp(withdrawals[i][0], "") == 0 ||
        strcmp(withdrawals[i][1], "") == 0)
      show_error_msg(grid, row + i, 4);
}

void to_controller(GtkWidget* grid) {
  // Clearing output
  clear_deposit_output(grid);
  // Initializing amount, term, interest and tax rates
  char* inputs[4];
  for (int i = 0; i < 4; i++) {
    GtkWidget* child = gtk_grid_get_child_at(GTK_GRID(grid), 1, i);
    inputs[i] = (char*)gtk_editable_get_text(GTK_EDITABLE(child));
  }
  // Initializing deposit type
  enum deposit_type type;
  GtkWidget* child = gtk_grid_get_child_at(GTK_GRID(grid), 1, 4);
  if (gtk_check_button_get_active(GTK_CHECK_BUTTON(child)))
    type = INTEREST_CAP;
  else
    type = NO_CAP;
  // Initializing preiodicity of payments
  enum periodicity deposit_preiodicity = get_periodicity(grid, type);
  // Initialize replanishments
  int number_of_replanishments = 0;
  char*** replanishments = get_replanishments(grid, &number_of_replanishments);
  // Initializing withdrawals
  int number_of_withdrawals = 0;
  char*** withdrawals = get_withdrawals(grid, &number_of_withdrawals);
  deposit* deposit_data = process_deposit_data(
      inputs, type, deposit_preiodicity, replanishments,
      number_of_replanishments, withdrawals, number_of_withdrawals);
  if (!deposit_data)
    handle_incorrect_data(grid, inputs, replanishments,
                          number_of_replanishments, withdrawals,
                          number_of_withdrawals);
  else {
    init_deposit_outputs(grid, deposit_data);
    init_deposit_table(grid, deposit_data);
  }
  free_additional_rows(replanishments, number_of_replanishments, withdrawals,
                       number_of_withdrawals);
}

void open_deposit_calc(GtkWidget* window) {
  // Variables
  GtkWidget* grid;
  GtkWidget* scrolled_window;
  GtkWidget* field_names[8];
  GtkWidget* inputs[4];
  GtkWidget* buttons[4];
  GtkWidget* cap_check;

  // Removing main menu from window
  gtk_window_set_child(GTK_WINDOW(window), NULL);

  // Initializing scrolled window
  scrolled_window = gtk_scrolled_window_new();
  gtk_window_set_child(GTK_WINDOW(window), scrolled_window);

  // Initializing grid
  grid = init_grid(scrolled_window);

  // Initializing field names
  init_deposit_field_names(grid, field_names);

  // Initializing inputs
  init_deposit_inputs(grid, inputs);

  // Initializing capitalization switcher
  cap_check = gtk_check_button_new_with_label("Capitalization of interest");
  gtk_grid_attach(GTK_GRID(grid), cap_check, 1, 4, 1, 1);
  g_signal_connect_swapped(cap_check, "toggled", G_CALLBACK(change_cap), grid);

  // Initializing replanishments
  buttons[0] = gtk_button_new_with_label("Add replenishment");
  g_signal_connect(buttons[0], "clicked", G_CALLBACK(add_additional_row), grid);
  gtk_grid_attach(GTK_GRID(grid), buttons[0], 1, 6, 1, 1);

  // Initializing withdrawals
  buttons[1] = gtk_button_new_with_label("Add withdrawal");
  g_signal_connect(buttons[1], "clicked", G_CALLBACK(add_additional_row), grid);
  gtk_grid_attach(GTK_GRID(grid), buttons[1], 1, 7, 1, 1);

  buttons[2] = gtk_button_new_with_label("Back");
  g_signal_connect_swapped(buttons[2], "clicked", G_CALLBACK(create_main_menu),
                           window);
  gtk_grid_attach(GTK_GRID(grid), buttons[2], 0, 8, 1, 1);

  buttons[3] = gtk_button_new_with_label("Calculate");
  g_signal_connect_swapped(buttons[3], "clicked", G_CALLBACK(to_controller),
                           grid);
  gtk_grid_attach(GTK_GRID(grid), buttons[3], 0, 9, 1, 1);

  // Initializing drop_down
  create_deposit_drop_down(grid, NO_CAP);
}