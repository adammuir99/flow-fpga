#define GREY 0xDEDB	//11011|110110|11011
volatile int pixel_buffer_start; // global variable

int main(void)
{
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    /* Read location of the pixel buffer from the pixel buffer controller */
    pixel_buffer_start = *pixel_ctrl_ptr;

    clear_screen();
    draw_grid();
	draw_box(0,7,0xF800);
	draw_circle(0,0,0xF800);
	highlight_box(1,1);
}

// code not shown for clear_screen() and draw_line() subroutines
void clear_screen(){
	for (int x = 0; x < 320; ++x){
		for (int y = 0; y < 240; ++y){
			plot_pixel(x,y, 0x0000);
		}
	}
}

void draw_line(int x0, int y0, int x1, int y1, short int line_colour){
	int is_steep = abs(y1 - y0) > abs(x1 - x0);
	if (is_steep){
		swap(&x0, &y0);
		swap(&x1, &y1);
	}
	if (x0 > x1){
		swap(&x0, &x1);
		swap(&y0, &y1);
	}
	
	int deltax = x1 - x0;
	int deltay = abs(y1 - y0);
	int error = -(deltax / 2);
	int y = y0;
	int y_step;
	
	if (y0 < y1){
		y_step = 1;
	} else {
		y_step = -1;
	}
	
	for (int x = x0; x <= x1; ++x){
		if (is_steep){
			plot_pixel(y, x, line_colour);
		} else {
			plot_pixel(x, y, line_colour);
		}
		error = error + deltay;
		if (error >= 0){
			y = y + y_step;
			error = error - deltax;
		}
	}
}

void swap(int* a, int* b){
	int temp = *a;
	*a = *b;
	*b = temp;
}

void plot_pixel(int x, int y, short int line_color)
{
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}

void draw_grid(){
	//Draw horizontal lines
	//9 lines, 5 pixels wide, 17 pixels apart
	for (int horiz_line = 32; horiz_line <= 208; horiz_line+=22){	// Each box is 17x17 (not including borders)
		for (int border_y = 0; border_y < 3; border_y++){
			for (int x = 72; x < 250; x++){
				plot_pixel(x, border_y+horiz_line, GREY);
			}
		}
	}
	//Draw vertical lines
	//9 lines, 5 pixels wide, 17 pixels apart
	for (int vert_line = 72; vert_line <= 248; vert_line+=22){	// Each box is 17x17 (not including borders)
		for (int border_x = 0; border_x < 3; border_x++){
			for (int y = 32; y < 210; y++){
				plot_pixel(border_x+vert_line, y, GREY);
			}
		}
	}
}

void draw_box(int row_, int col_, short int colour){
	int col = 75 + (22 * col_);		//First column starts at 72 (+3 line width)
	int row = 35 + (22 * row_);		//First row starts at 32 (+3 line width)
								//Each subsequent row/col is 22 pixels further
	for (int x = 0; x < 19; x++){
		for (int y = 0; y < 19; y++){
			plot_pixel(x+col, y+row, colour);
		}
	}
}

void draw_circle(int row_, int col_, short int colour){
	int col = 84 + (22 * col_);		//Center of the circle at 75 (+8)
	int row = 44 + (22 * row_);		//Center of the circle at 35 (+8)
	
	plot_pixel(col, row, colour);	//Draw the center dot
	
	for (int x = 0; x < 5; x++){	
		plot_pixel(col-2+x, row-4, colour);	//Draw the top of the circle
		plot_pixel(col-2+x, row+4, colour);	//Draw the bottom of the circle
	}
	for (int y = 0; y < 5; y++){
		plot_pixel(col-4, row-2+y, colour);	//Draw the left of the circle
		plot_pixel(col+4, row-2+y, colour);	//Draw the right of the circle
	}
	//Draw the corners of the circle
	plot_pixel(col-3, row-3, colour);
	plot_pixel(col+3, row-3, colour);
	plot_pixel(col-3, row+3, colour);
	plot_pixel(col+3, row+3, colour);

}

void highlight_box(int row_, int col_){
	int col = 72 + (22 * col_);		//Top left coordinate of the column border
	int row = 32 + (22 * row_);		//Top left coordinate of the row border
	
	for (int x = 0; x < 25; x++){	//Each grid tile is 19x19 with a 3 pixel wide border (19+3+3=25)
		for (int y = 0; y < 3; y++){
			plot_pixel(col+x, row+y, 0xEFE3);
			plot_pixel(col+x, row+22+y, 0xEFE3);
		}
	}
	for (int y = 0; y < 25; y++){	//Each grid tile is 19x19 with a 3 pixel wide border (19+3+3=25)
		for (int x = 0; x < 3; x++){
			plot_pixel(col+x, row+y, 0xEFE3);
			plot_pixel(col+22+x, row+y, 0xEFE3);
		}
	}
}