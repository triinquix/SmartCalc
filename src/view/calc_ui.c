#include "../main_app.h"

void create_buttons(GtkWidget* buttons[32]) {
  // Creating buttons
  for (int i = 0; i < 10; i++) {
    char tmp_str[2];
    sprintf(tmp_str, "%d", i);
    buttons[i] = gtk_button_new_with_label(tmp_str);
  }
  buttons[10] = gtk_button_new_with_label("(");
  buttons[11] = gtk_button_new_with_label(")");
  buttons[12] = gtk_button_new_with_label("cos");
  buttons[13] = gtk_button_new_with_label("sin");
  buttons[14] = gtk_button_new_with_label("tan");
  buttons[15] = gtk_button_new_with_label("Back");
  buttons[16] = gtk_button_new_with_label("acos");
  buttons[17] = gtk_button_new_with_label("asin");
  buttons[18] = gtk_button_new_with_label("atan");
  buttons[19] = gtk_button_new_with_label("DEL");
  buttons[20] = gtk_button_new_with_label("sqrt");
  buttons[21] = gtk_button_new_with_label("ln");
  buttons[22] = gtk_button_new_with_label("log");
  buttons[23] = gtk_button_new_with_label("AC");
  buttons[24] = gtk_button_new_with_label("+");
  buttons[25] = gtk_button_new_with_label("-");
  buttons[26] = gtk_button_new_with_label("mod");
  buttons[27] = gtk_button_new_with_label(".");
  buttons[28] = gtk_button_new_with_label("*");
  buttons[29] = gtk_button_new_with_label("/");
  buttons[30] = gtk_button_new_with_label("^");
  buttons[31] = gtk_button_new_with_label("=");
}

void init_attach_buttons(GtkWidget* grid, GtkWidget* buttons[32]) {
  create_buttons(buttons);
  int row_index = 2;
  int column_index = 0;
  for (int i = 12; i < 32; i++) {
    gtk_grid_attach(GTK_GRID(grid), buttons[i], column_index, row_index, 1, 1);
    row_index++;
    if (row_index == 6) {
      row_index = 2;
      column_index++;
      if (i == 23) column_index += 3;
    }
  }
  gtk_grid_attach(GTK_GRID(grid), buttons[10], 3, 5, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), buttons[0], 4, 5, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), buttons[11], 5, 5, 1, 1);
  row_index = 4;
  column_index = 3;
  for (int i = 1; i < 10; i++) {
    gtk_grid_attach(GTK_GRID(grid), buttons[i], column_index, row_index, 1, 1);
    column_index++;
    if (column_index == 6) {
      column_index = 3;
      row_index--;
    }
  }
}

void clear_input(GtkWidget* input) {
  gtk_label_set_label(GTK_LABEL(input), "");
}

void str_to_label(GtkWidget* button, gpointer label) {
  char* str_to_print = (char*)gtk_button_get_label(GTK_BUTTON(button));
  char* current_expression = (char*)gtk_label_get_label(GTK_LABEL(label));
  char* res_str = (char*)malloc(
      sizeof(char) * (strlen(str_to_print) + strlen(current_expression)) + 1);
  strcpy(res_str, current_expression);
  strcat(res_str, str_to_print);
  res_str[strlen(str_to_print) + strlen(current_expression)] = '\0';
  gtk_label_set_label(GTK_LABEL(label), res_str);
  free(res_str);
}

void delete_char(GtkWidget* label) {
  char* current_expression = (char*)gtk_label_get_label(GTK_LABEL(label));
  if (strlen(current_expression)) {
    char* res_str =
        (char*)malloc(sizeof(char) * (strlen(current_expression) - 1) + 1);
    strncpy(res_str, current_expression, strlen(current_expression) - 1);
    res_str[strlen(current_expression) - 1] = '\0';
    gtk_label_set_label(GTK_LABEL(label), res_str);
    free(res_str);
  }
}

void calculate(GtkWidget* grid) {
  GtkWidget* label = gtk_grid_get_child_at(GTK_GRID(grid), 0, 0);
  char* str = (char*)gtk_label_get_label(GTK_LABEL(label));
  GtkWidget* x_input = gtk_grid_get_child_at(GTK_GRID(grid), 9, 0);
  char* x = (char*)gtk_editable_get_text(GTK_EDITABLE(x_input));
  char* calculated_value = calculator(str, x);
  GtkWidget* output = gtk_grid_get_child_at(GTK_GRID(grid), 0, 1);
  gtk_label_set_label(GTK_LABEL(output), calculated_value);
  free(calculated_value);
}

void activate_button(GtkWidget* button) {
  gtk_widget_set_sensitive(button, TRUE);
}

double map_x(int width, double x, double x_min, double x_max) {
  return width * (x - x_min) / (x_max - x_min);
}

double map_y(int height, double y, double y_min, double y_max) {
  return height * (1 - (y - y_min) / (y_max - y_min));
}

void add_numbers_to_axes(cairo_t* cr, int width, int height,
                         points* points_to_calc) {
  double x[21];
  double x_min = points_to_calc->x[0];
  double x_max = points_to_calc->x[999];
  double delta_x = (x_max - x_min) / 21;
  for (int i = 0; i < 21; i++) {
    if (i)
      x[i] = x[i - 1] + delta_x;
    else
      x[i] = x_min;
    double x_coordinate = map_x(width, x[i], x_min, x_max);
    cairo_move_to(cr, x_coordinate, height / 2);
    char tmp_str[100];
    sprintf(tmp_str, "%.1lf", x[i]);
    cairo_show_text(cr, tmp_str);
  }
}

void draw_codomain(cairo_t* cr, int width, int height, points* point_to_draw) {
  double x_min = point_to_draw->x[0];
  double x_max = point_to_draw->x[999];
  double y[13];
  double delta_y = (point_to_draw->y_max - point_to_draw->y_min) / 13;
  double y_coordinates[13];
  for (int i = 0; i < 13; i++) {
    if (i)
      y[i] = y[i - 1] + delta_y;
    else
      y[i] = point_to_draw->y_min;
    y_coordinates[i] =
        map_y(height, y[i], point_to_draw->y_min, point_to_draw->y_max);
  }
  if (x_min <= 0 && x_max >= 0) {
    // Draw line and numbers
    double zero_position = map_x(width, 0, x_min, x_max);
    cairo_move_to(cr, zero_position, 0);
    cairo_line_to(cr, zero_position, height);
    for (int i = 0; i < 13; i++) {
      cairo_move_to(cr, zero_position, y_coordinates[i]);
      char tmp_str[100];
      sprintf(tmp_str, "%.2lf", y[i]);
      cairo_show_text(cr, tmp_str);
    }
  } else if (x_min < 0 && x_max <= 0) {
    // Draw numbers on right
    for (int i = 0; i < 13; i++) {
      cairo_move_to(cr, width, y_coordinates[i]);
      char tmp_str[100];
      sprintf(tmp_str, "%.2lf", y[i]);
      cairo_show_text(cr, tmp_str);
    }
  } else if (x_min >= 0 && x_max > 0) {
    // Draw numbers on the left side
    for (int i = 0; i < 13; i++) {
      cairo_move_to(cr, 0, y_coordinates[i]);
      char tmp_str[100];
      sprintf(tmp_str, "%.2lf", y[i]);
      cairo_show_text(cr, tmp_str);
    }
  }
}

void draw_domain(cairo_t* cr, int width, int height, points* point_to_draw) {
  double y_min = point_to_draw->y_min;
  double y_max = point_to_draw->y_max;
  double x[21];
  double delta_x = (point_to_draw->x[999] - point_to_draw->x[0]) / 21;
  double x_coordinates[21];
  for (int i = 0; i < 21; i++) {
    if (i)
      x[i] = x[i - 1] + delta_x;
    else
      x[i] = point_to_draw->x[0];
    x_coordinates[i] =
        map_x(width, x[i], point_to_draw->x[0], point_to_draw->x[999]);
  }
  if (y_min <= 0 && y_max >= 0) {
    double zero_position = map_y(height, 0, y_min, y_max);
    cairo_move_to(cr, 0, zero_position);
    cairo_line_to(cr, width, zero_position);
    for (int i = 0; i < 21; i++) {
      cairo_move_to(cr, x_coordinates[i], zero_position);
      char tmp_str[100];
      sprintf(tmp_str, "%.2lf", x[i]);
      cairo_show_text(cr, tmp_str);
    }
  } else if (y_min < 0 && y_max <= 0) {
    // Draw above
    for (int i = 0; i < 21; i++) {
      cairo_move_to(cr, x_coordinates[i], 0);
      char tmp_str[100];
      sprintf(tmp_str, "%.2lf", x[i]);
      cairo_show_text(cr, tmp_str);
    }
  } else if (y_min >= 0 && y_max > 0) {
    // Draw below
    for (int i = 0; i < 21; i++) {
      cairo_move_to(cr, x_coordinates[i], height);
      char tmp_str[100];
      sprintf(tmp_str, "%.2lf", x[i]);
      cairo_show_text(cr, tmp_str);
    }
  }
}

void draw_axes(cairo_t* cr, int width, int height, points* points_to_calc) {
  draw_domain(cr, width, height, points_to_calc);
  draw_codomain(cr, width, height, points_to_calc);
}

void draw_points(cairo_t* cr, int width, int height, points* points_to_calc) {
  for (int i = 1; i < 1000; i++) {
    if (!isnan(points_to_calc->y[i]) && !isnan(points_to_calc->y[i - 1])) {
      double x_1 = map_x(width, points_to_calc->x[i - 1], points_to_calc->x[0],
                         points_to_calc->x[999]);
      double x_2 = map_x(width, points_to_calc->x[i], points_to_calc->x[0],
                         points_to_calc->x[999]);
      double y_1 = map_y(height, points_to_calc->y[i - 1],
                         points_to_calc->y_min, points_to_calc->y_max);
      double y_2 = map_y(height, points_to_calc->y[i], points_to_calc->y_min,
                         points_to_calc->y_max);
      cairo_move_to(cr, x_1, y_1);
      cairo_line_to(cr, x_2, y_2);
    }
  }
}

void draw_func(GtkDrawingArea* drawing_area, cairo_t* cr, int width, int height,
               gpointer data) {
  points* points_to_calculate = data;
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_set_line_width(cr, 0.5);
  cairo_select_font_face(cr, "Purisa", CAIRO_FONT_SLANT_NORMAL,
                         CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_size(cr, 13);
  draw_axes(cr, width, height, points_to_calculate);
  draw_points(cr, width, height, points_to_calculate);
  cairo_stroke(cr);
  gtk_widget_queue_draw(GTK_WIDGET(drawing_area));
  free(points_to_calculate);
}

void plot_graph(GtkWidget* button, GtkWidget* grid) {
  // Variables
  GtkWidget* d_area;

  // Getting limits for x
  GtkWidget* x_min_entry = gtk_grid_get_child_at(GTK_GRID(grid), 3, 8);
  char* x_min = (char*)gtk_editable_get_text(GTK_EDITABLE(x_min_entry));
  GtkWidget* x_max_entry = gtk_grid_get_child_at(GTK_GRID(grid), 4, 8);
  char* x_max = (char*)gtk_editable_get_text(GTK_EDITABLE(x_max_entry));
  // Getting limits for y
  GtkWidget* y_min_entry = gtk_grid_get_child_at(GTK_GRID(grid), 3, 9);
  char* y_min = (char*)gtk_editable_get_text(GTK_EDITABLE(y_min_entry));
  GtkWidget* y_max_entry = gtk_grid_get_child_at(GTK_GRID(grid), 4, 9);
  char* y_max = (char*)gtk_editable_get_text(GTK_EDITABLE(y_max_entry));
  GtkWidget* label_with_expression =
      gtk_grid_get_child_at(GTK_GRID(grid), 0, 0);
  char* expression =
      (char*)gtk_label_get_label(GTK_LABEL(label_with_expression));
  points* points_to_plot =
      process_plot_data(x_min, x_max, y_min, y_max, expression);
  if (points_to_plot) {
    gtk_widget_set_sensitive(button, FALSE);
    // Initializing new window
    GtkWidget* window_plot = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(window_plot), "Plot");
    gtk_window_set_default_size(GTK_WINDOW(window_plot), 1500, 1000);
    g_signal_connect_swapped(window_plot, "close-request",
                             G_CALLBACK(activate_button), button);
    // Drawing area
    d_area = gtk_drawing_area_new();
    gtk_window_set_child(GTK_WINDOW(window_plot), d_area);
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(d_area), draw_func,
                                   points_to_plot, NULL);

    gtk_widget_show(window_plot);
    // free(points_to_plot);
  }
}

GtkWidget* init_calc_grid(GtkWidget* window) {
  GtkWidget* grid = gtk_grid_new();
  gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);
  gtk_window_set_child(GTK_WINDOW(window), grid);
  gtk_grid_set_column_spacing(GTK_GRID(grid), 1);
  gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 1);
  gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
  return grid;
}

void init_params(GtkWidget** input, GtkWidget* grid, GtkWidget** output,
                 GtkWidget* buttons[32], GtkWidget** x_edit,
                 GtkWidget** x_label) {
  // Initializing input
  *input = gtk_label_new(NULL);
  gtk_widget_set_halign(*input, GTK_ALIGN_START);
  gtk_grid_attach(GTK_GRID(grid), *input, 0, 0, 8, 1);

  // Initializing output
  *output = gtk_label_new(NULL);
  gtk_widget_set_halign(*output, GTK_ALIGN_END);
  gtk_grid_attach(GTK_GRID(grid), *output, 0, 1, 8, 1);

  // Initializing buttons
  init_attach_buttons(grid, buttons);

  // X handler
  *x_edit = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), *x_edit, 9, 0, 1, 1);
  *x_label = gtk_button_new_with_label("x");
  g_signal_connect(*x_label, "clicked", G_CALLBACK(str_to_label), *input);
  gtk_grid_attach(GTK_GRID(grid), *x_label, 9, 2, 1, 1);
}

void init_plot_options(GtkWidget* grid, GtkWidget** plot_button,
                       GtkWidget* domains[2], GtkWidget* codomains[2],
                       GtkWidget* domains_entries[2],
                       GtkWidget* codomain_entries[2]) {
  // Plot button
  *plot_button = gtk_button_new_with_label("Plot");
  g_signal_connect(*plot_button, "clicked", G_CALLBACK(plot_graph), grid);
  gtk_grid_attach(GTK_GRID(grid), *plot_button, 2, 7, 4, 1);

  // Initializing domain
  domains[0] = gtk_label_new("x_min");
  domains[1] = gtk_label_new("x_max");
  gtk_grid_attach(GTK_GRID(grid), domains[0], 2, 8, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), domains[1], 5, 8, 1, 1);
  for (int i = 0; i < 2; i++) domains_entries[i] = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), domains_entries[0], 3, 8, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), domains_entries[1], 4, 8, 1, 1);

  // Initializing codomains
  codomains[0] = gtk_label_new("y_min");
  codomains[1] = gtk_label_new("y_max");
  gtk_grid_attach(GTK_GRID(grid), codomains[0], 2, 9, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), codomains[1], 5, 9, 1, 1);
  for (int i = 0; i < 2; i++) codomain_entries[i] = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), codomain_entries[0], 3, 9, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), codomain_entries[1], 4, 9, 1, 1);
}

void open_calc(GtkWidget* window) {
  // Variables
  GtkWidget* grid;
  GtkWidget* buttons[32];
  GtkWidget* input;
  GtkWidget* output;
  GtkWidget* x_edit;
  GtkWidget* x_label;
  GtkWidget* plot_button;
  GtkWidget* domains[2];
  GtkWidget* codomains[2];
  GtkWidget* domains_entries[2];
  GtkWidget* codomains_entries[2];

  // Removing main menu from window
  gtk_window_set_child(GTK_WINDOW(window), NULL);

  // Attaching grid to main window
  grid = init_calc_grid(window);

  init_params(&input, grid, &output, buttons, &x_edit, &x_label);

  // Back button
  g_signal_connect_swapped(buttons[15], "clicked", G_CALLBACK(create_main_menu),
                           window);

  // AC button
  g_signal_connect_swapped(buttons[23], "clicked", G_CALLBACK(clear_input),
                           input);

  // Delete char button
  g_signal_connect_swapped(buttons[19], "clicked", G_CALLBACK(delete_char),
                           input);

  // Calc button
  g_signal_connect_swapped(buttons[31], "clicked", G_CALLBACK(calculate), grid);
  for (int i = 0; i < 31; i++)
    if (i != 15 && i != 19 && i != 23)
      g_signal_connect(buttons[i], "clicked", G_CALLBACK(str_to_label), input);

  init_plot_options(grid, &plot_button, domains, codomains, domains_entries,
                    codomains_entries);
}