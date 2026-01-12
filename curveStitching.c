#include <gtk/gtk.h>
#include <math.h>

#define WIDTH 400
#define HEIGHT 400
#define NUM_LINES 15
#define STEP 10  // distance between points

// Interpolate between two colors (r1,g1,b1) -> (r2,g2,b2) based on t (0.0-1.0)
static void interpolate_color(double t, double r1, double g1, double b1,
                              double r2, double g2, double b2,
                              double *r, double *g, double *b) {
    *r = r1 + t * (r2 - r1);
    *g = g1 + t * (g2 - g1);
    *b = b1 + t * (b2 - b1);
}

// Draw the curve stitching with vertical purple→pink gradient
static gboolean draw_curve_stitching(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    // Shift origin to center
    cairo_translate(cr, WIDTH / 2, HEIGHT / 2);
    // Rotate 45 degrees
    cairo_rotate(cr, M_PI / 4);

    // Define top and bottom colors
    double r_top = 0.5, g_top = 0.0, b_top = 0.5;      // Purple
    double r_bottom = 0.8, g_bottom = 0.0, b_bottom = 0.5; // Dark pink

    for (int i = 0; i <= NUM_LINES; i++) {
        double ratio = (double)i / NUM_LINES;  // 0 = top, 1 = bottom
        double r, g, b;
        interpolate_color(ratio, r_top, g_top, b_top, r_bottom, g_bottom, b_bottom, &r, &g, &b);
        cairo_set_source_rgb(cr, r, g, b);

        // Q1
        cairo_move_to(cr, 0, STEP * NUM_LINES - STEP * i);
        cairo_line_to(cr, STEP * i, 0);
        cairo_stroke(cr);

        // Q2
        cairo_move_to(cr, 0, STEP * NUM_LINES - STEP * i);
        cairo_line_to(cr, -STEP * i, 0);
        cairo_stroke(cr);

        // Q3
        cairo_move_to(cr, 0, -STEP * NUM_LINES + STEP * i);
        cairo_line_to(cr, -STEP * i, 0);
        cairo_stroke(cr);

        // Q4
        cairo_move_to(cr, 0, -STEP * NUM_LINES + STEP * i);
        cairo_line_to(cr, STEP * i, 0);
        cairo_stroke(cr);
    }

    return FALSE;
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Curve Stitching Purple→Pink");
    gtk_window_set_default_size(GTK_WINDOW(window), WIDTH, HEIGHT);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), drawing_area);
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(draw_curve_stitching), NULL);

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
