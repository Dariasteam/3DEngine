typedef struct {
	float r;
	float g;
	float b;    											 
} Color;									
											
typedef struct { 
	int x; 
	int y;
 		
	Color color;							 		
} Point; 						

typedef struct {
	unsigned w;
	unsigned h;
}	Size;

typedef struct {
 	Point a;
 	Point b;
 	Point c;
 
	Color g1;
 	Color g2;
 
 	float slope_1;
 	float slope_2;			
 	
 	float z_value;
} Triangle;					 													
 
float slope (const Point p1, const Point p2) {
	float bx = p2.x;
 	float ax = p1.x;
 	 
	float by = p2.y;
 	float ay = p1.y;													 	 
 
 	float a = (bx - ax);
 	float b = (by - ay);

 	return a / b;
}													 
 
Color get_gradient (const Color color1, 
  									const Color color2,
 										const float p1,
 										const float p2) {				
 										 		
	float distance = p2 - p1;
   
	Color c;
   
  c.r = (color2.r - color1.r) / distance;
  c.g = (color2.g - color1.g) / distance;
  c.b = (color2.b - color1.b) / distance;
   
	return c;
}
 
Color get_color_in_gradient (const Color color1,
 				  									 const Color gradient_ratio,
 													   const float p1,
 														 const float pos) {
 																 
	float distance = pos - p1;
	 
	Color c;
	 
  c.r = color1.r + gradient_ratio.r * distance;
  c.g = color1.g + gradient_ratio.g * distance;
  c.b = color1.b + gradient_ratio.b * distance;														 														 													  
									   	
	return c;						   																															 																
}													 																				 											 
 
void paint_color_at (global unsigned char *A, 
  									 const Color color, 
 										 const unsigned index) {
  A[index + 0] = color.r;
  A[index + 1] = color.g;
	A[index + 2] = color.b;
}		

unsigned get_index (unsigned y, unsigned x, const Size size) {	
	return (y * size.w * 3) + (x * 3);	 
}

void paint_horizontal_line (const unsigned min_x,
													  const unsigned max_x, 
													  const unsigned y, 
													  const Color color1, 
													  const Color color2,
													  const Size size,
													  global unsigned char* A,
													  global float* Z,
													  double z) {
													  
	Color gradient = get_gradient(color1, color2, min_x, max_x);
	
	for (unsigned x = min_x; x <= max_x; x++) {		
		unsigned z_index = y * size.w + x;		
		if (z < Z[z_index]) {
			unsigned img_index = get_index (y, x, size);
			Z[z_index] = z; 		
			Color c = get_color_in_gradient (color1, gradient, min_x, x);				
			paint_color_at (A, c, img_index);
		}								
	}	
}
 
Triangle order_triangle (Triangle t) {
	Point a;
	Point b;
	Point c;

	if (t.a.y <= t.b.y && t.b.y <= t.c.y) {
    a = t.a; b = t.b; c = t.c;
  } else if (t.a.y <= t.c.y && t.c.y <= t.b.y) {									     	 
   	a = t.a; b = t.c; c = t.b;													   
  } else if (t.b.y <= t.a.y && t.a.y <= t.c.y) { 		  										    
    a = t.b; b = t.a; c = t.c;
  } else if (t.b.y <= t.c.y && t.c.y <= t.a.y) { 													     
    a = t.b; b = t.c; c = t.a;
  } else if (t.c.y <= t.a.y && t.a.y <= t.b.y) { 													     
   	a = t.c; b = t.a; c = t.b;
 	} else if (t.c.y <= t.b.y && t.b.y <= t.a.y) { 												   	   
 	 	a = t.c; b = t.b; c = t.a;
 	} 
 	 	
 	t.a = a;
 	t.b = b;
 	t.c = c;
 	
 	return t;
}

Triangle order_triangle_top_flat_x (Triangle t) {	
	if (t.a.x > t.b.x) {
		
		Point aux = t.a;
		t.a = t.b;
		t.b = aux;
	}
	return t;
}

Triangle order_triangle_bttm_flat_x (Triangle t) {
	if (t.b.x > t.c.x) {
		Point aux = t.c;
		t.c = t.b;
		t.b = aux;
	}
	return t;
}

Triangle build_bttm_flat_triangle (Triangle t) {

	t = order_triangle_bttm_flat_x (t);
		
	t.g1 = get_gradient(t.a.color, t.b.color, t.a.y, t.b.y);
	t.g2 = get_gradient(t.a.color, t.c.color, t.a.y, t.c.y);
		
	t.slope_1 = slope (t.a, t.b);
	t.slope_2 = slope (t.a, t.c);
	
	return t;
}

Triangle build_top_flat_triangle (Triangle t) {

	t = order_triangle_top_flat_x (t);
		
	t.g1 = get_gradient(t.a.color, t.c.color, t.a.y, t.c.y),
	t.g2 = get_gradient(t.b.color, t.c.color, t.b.y, t.c.y),
		
	t.slope_1 = slope (t.c, t.a);
	t.slope_2 = slope (t.c, t.b);		

	return t;
}

 /*     /\
  *    /  \
  *   /____\
  */
void rasterize_bttm_flat_triangle (global unsigned char *A,
																	 global float* Z,
																	 const Triangle t,
																	 const Size size) {								 	 	

	float curx1 = t.a.x;
	float curx2 = t.a.x;	

	for (unsigned y = t.a.y; y <= t.c.y; y++) {
			
		curx1 += t.slope_1; 
		curx2 += t.slope_2;		
	
		Color color1 = get_color_in_gradient (t.b.color, t.g1, t.b.y, y);
		Color color2 = get_color_in_gradient (t.c.color, t.g2, t.c.y, y);				
		
		paint_horizontal_line (curx1, curx2, y, color1, color2, size, A, Z, t.z_value);		
	} 										       		 				       		 														       		 									       	
}
 
 /*    ____
  *    \  /
  *     \/
  */
void rasterize_top_flat_triangle (global unsigned char *A, 
																	global float* Z, 
																	const Triangle t, 
																	const Size size) {								 	 

	float curx1 = t.c.x;
	float curx2 = t.c.x;
	
	for (unsigned y = t.c.y; y >= t.a.y; y--) {		
	
		curx1 -= t.slope_1;
		curx2 -= t.slope_2;
		
		Color color1 = get_color_in_gradient (t.a.color, t.g1, t.a.y, y);
		Color color2 = get_color_in_gradient (t.b.color, t.g2, t.b.y, y);
		
		paint_horizontal_line (curx1, curx2, y, color1, color2, size, A, Z, t.z_value);				
	}							       		 														       		 									       	
}

void split_triangle (const Triangle t, 
										 global unsigned char *A, 
										 global float* Z, 
										 const Size size) {	 			 	

	if (t.b.y == t.c.y) {
		Triangle t_bttm = build_bttm_flat_triangle (t);
		rasterize_bttm_flat_triangle (A, Z, t_bttm, size);
	} else if (t.a.y == t.b.y) {	
		Triangle t_top  = build_top_flat_triangle (t);		
		rasterize_top_flat_triangle (A, Z, t_top, size);
	} else {			
		
		float ratio = slope(t.a, t.c);
		float x = ratio * (t.b.y - t.a.y);
		x += t.a.x;

		Point v4;
		v4.x = x;
		v4.y = t.b.y;

		Color g  = get_gradient(t.a.color, t.c.color, t.a.y, t.c.y);																
		v4.color = get_color_in_gradient (t.a.color, g, t.a.y, v4.y);																																																					
		
		Triangle t_bttm;
		t_bttm.a = t.a;
		t_bttm.b = t.b;
		t_bttm.c = v4;
		t_bttm.z_value = t.z_value;				
		 
		Triangle t_top;  
		t_top.a = t.b;
		t_top.b = v4;
		t_top.c = t.c;
		t_top.z_value = t.z_value;

		t_bttm = build_bttm_flat_triangle (t_bttm);
		t_top  = build_top_flat_triangle (t_top);	
		
		rasterize_bttm_flat_triangle (A, Z, t_bttm, size);		
		rasterize_top_flat_triangle  (A, Z, t_top, size);
	}	
}

Triangle extract_triangle (constant unsigned* T, 
													 constant unsigned char* C,
													 constant float* z) {
	unsigned id = get_global_id(0);
	unsigned c_id = id * 3 * 3;
	unsigned t_id = id * 2 * 3;
	
	Triangle t;
	
	Color c1 = {C[c_id + 0], C[c_id + 1], C[c_id + 2]};
	Color c2 = {C[c_id + 3], C[c_id + 4], C[c_id + 5]};
	Color c3 = {C[c_id + 6], C[c_id + 7], C[c_id + 8]};
	
	Point a = {T[t_id + 0], T[t_id + 1], c1};
	Point b = {T[t_id + 2], T[t_id + 3], c2};
	Point c = {T[t_id + 4], T[t_id + 5], c3};
	
	t.a = a;
	t.b = b;
	t.c = c;
	
	t.z_value = z[id];
	
	return t;
} 
 
void kernel rasterize_triangle (global unsigned char *A,
																global float* Z,
																constant unsigned* T,
																constant unsigned char* C,
																constant float* z,																
																const Size size) {		
	
	Triangle t = extract_triangle (T, C, z);	
		
	t = order_triangle (t);
  split_triangle (t, A, Z, size);
}


