#include <gtk/gtk.h>
#include <math.h>

#define WIDTH 400
#define HEIGHT 400
#define NUM_LINES 15
#define STEP 10

static int frame = 0; // current frame, initial zero

static gboolean draw_curve_stitching(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    // Fill background black
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_paint(cr);

    // make the origin the center of the screen
    cairo_translate(cr, WIDTH / 2, HEIGHT / 2);

    // main logic loop
    for (int i = 0; i <= frame && i <= NUM_LINES; i++) {
        double red = 1.0 - (double)i / NUM_LINES;
        double green = (double)i / NUM_LINES;
        double blue = 0.0; //no blue

        cairo_set_source_rgb(cr, red, green, blue);

        // q1
        cairo_move_to(cr, 0, STEP * NUM_LINES - STEP * i);
        cairo_line_to(cr, STEP * i, 0);

        // q2
        cairo_move_to(cr, 0, STEP * NUM_LINES - STEP * i);
        cairo_line_to(cr, -STEP * i, 0);

        // q3
        cairo_move_to(cr, 0, -STEP * NUM_LINES + STEP * i);
        cairo_line_to(cr, -STEP * i, 0);

        // q4
        cairo_move_to(cr, 0, -STEP * NUM_LINES + STEP * i);
        cairo_line_to(cr, STEP * i, 0);

        cairo_stroke(cr);
    }

    return FALSE;
}


gboolean update_frame(gpointer data) {
    GtkWidget *drawing_area = GTK_WIDGET(data);

    frame++;
    if (frame > NUM_LINES) frame = 0; // loop animation

    gtk_widget_queue_draw(drawing_area); // redraw
    return TRUE; // keep timer running
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Curve Stitching");
    gtk_window_set_default_size(GTK_WINDOW(window), WIDTH, HEIGHT);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), drawing_area);
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(draw_curve_stitching), NULL);

    gtk_widget_show_all(window);

    // Animate every 200 miliseconds
    g_timeout_add(200, update_frame, drawing_area);

    gtk_main();
    return 0;
}
