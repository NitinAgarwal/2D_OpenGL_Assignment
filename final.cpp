
#include<cmath>
#include<GL/glut.h>
#include<iostream>
#include<stdio.h>
#include<fstream>
#include<iostream>    
#include<string>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>

using namespace std;
#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)

void drawScene();
void update(int value);
void update1(int value);
void drawBox(float wid,float hei);
void initRendering();
void handleResize(int w, int h);
void handleKeypress1(unsigned char key, int x, int y);
void handleKeypress2(int key, int x, int y);
void handleMouseclick(int button, int state, int x, int y);
void draw_projector(float a,float b);
void draw_block(float c,float d);
void draw_mirror(float e,float f);
void draw_ray(float a,float b,float c,float d);
void gen_output();

int flag_t=0;
int up_counter=0;
int ray_counter=0;
int flag_rand=0;
int flag_ray=0;
int for_ptr;
float box_mag;
float theta = 0.0f;
float theta2= 0.005f;
float theta3= -0.005f;
float deltu=0.1f;
float world_width,world_height,light_dis;
int proj_no,mir_no,block_no,pixel_no;
float *temp_clr1=new float[3];
float *ini_pt=new float[2];
float *fin_pt=new float[2];
float **dis_arr;
float **temp_arr;
float **temp_clr;
int clr2=0;
int flagy=0;
float *arr=new float[1000];
float *proj,*block,*mir;
int arr_ctr1=0;
int arr_ctr2=0;


int main(int argc,char *argv[])
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	int w = glutGet(GLUT_SCREEN_WIDTH);
    int h = glutGet(GLUT_SCREEN_HEIGHT);
    int windowWidth = w * 2 / 3;
    int windowHeight = h * 2 / 3;
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition((w-windowWidth)/2,(h-windowHeight)/2);

    glutCreateWindow("My Window");
    initRendering();

// File handling

	fstream file;           // declare a file pointer
	file.open(argv[1],fstream::in | fstream::out);     //opens file with name argv[1] and gives in,out permissions
	string line,x_stri;
	float atf;
	int i_ptr;
	for(i_ptr=0;i_ptr<1000;i_ptr++)
		arr[i_ptr]=-1.0;
	while ( file.good() )
	{
		file >> x_stri;
		while ( x_stri[0]!='#' && file.good())
		{
			atf=::atof(x_stri.c_str());
			arr[arr_ctr1]=atf;
			arr_ctr1=arr_ctr1+1;
			file >> x_stri;
		}
		getline (file,line);
	}
	file.close();
	world_width=arr[arr_ctr2];
	arr_ctr2 = arr_ctr2+1;
	world_height = arr[arr_ctr2];
	arr_ctr2 = arr_ctr2+1;
	proj_no = int(arr[arr_ctr2]);
	proj = new float[4*proj_no];
	arr_ctr2 = arr_ctr2+1;
	int wh1,wh2,wh3;
	for(wh1=0;wh1<4*proj_no;wh1++)
	{
		proj[wh1]=arr[arr_ctr2];
		arr_ctr2=arr_ctr2+1;
	}
	light_dis = arr[arr_ctr2];
	arr_ctr2=arr_ctr2+1;
	pixel_no = int(arr[arr_ctr2]);
	arr_ctr2 = arr_ctr2+1;	
	block_no = int(arr[arr_ctr2]);
	block = new float[4*block_no];
	arr_ctr2 = arr_ctr2+1;
	for(wh2=0;wh2<4*block_no;wh2++)
	{
		block[wh2]=arr[arr_ctr2];
		arr_ctr2=arr_ctr2+1;
	}
	mir_no = int(arr[arr_ctr2]);
	mir = new float[4*mir_no];
	arr_ctr2 = arr_ctr2+1;
	for(wh3=0;wh3<4*mir_no;wh3++)
	{
		mir[wh3]=arr[arr_ctr2];
		arr_ctr2=arr_ctr2+1;
	}

// memory allocation
	temp_clr1[0] = 0.0f;
	temp_clr1[1] = 1.0f;
	temp_clr1[2] = 0.0f;
	int hel;
	temp_arr = new float* [proj_no+block_no+mir_no];
	temp_clr = new float* [proj_no+block_no+mir_no];
	for(hel=0;hel<proj_no+block_no+mir_no;hel++)
	{
		temp_arr[hel] = new float [4];
	}
	for(hel=0;hel<proj_no+block_no+mir_no;hel++)
	{
		temp_clr[hel] = new float [3];
	}

// temp array
	int ptr1=0,ptr2=0,flag=1,ptr3=0,ptr4=0,ptr5=0;
	for(ptr1=0;ptr1<proj_no+block_no+mir_no;ptr1++)
	{
		if(flag==1 && ptr2<proj_no)
		{
			temp_arr[ptr1][0] = proj[ptr3];
			temp_arr[ptr1][1] = proj[ptr3+1];
			temp_arr[ptr1][2] = proj[ptr3+2];
			temp_arr[ptr1][3] = proj[ptr3+3];
			temp_clr[ptr1][0] = 0.0f;
			temp_clr[ptr1][1] = 1.0f;
			temp_clr[ptr1][2] = 0.0f;
			ptr3 = ptr3+4;
			ptr2 = ptr2+1;
			if(ptr2>=proj_no)
			{
				flag = 2;
				ptr3 = 0;
			}
		}
		else if(flag==2 && ptr4<block_no)
		{
			temp_arr[ptr1][0] = block[ptr3];
			temp_arr[ptr1][1] = block[ptr3+1];
			temp_arr[ptr1][2] = block[ptr3+2];
			temp_arr[ptr1][3] = block[ptr3+3];
			temp_clr[ptr1][0] = 1.0f;
			temp_clr[ptr1][1] = 0.0f;
			temp_clr[ptr1][2] = 0.0f;
			ptr3 = ptr3+4;
			ptr4 = ptr4+1;
			if(ptr4>=block_no)
			{
				flag=3;
				ptr3=0;
			}
		}
		else if(flag==3 && ptr5<mir_no)
		{
			temp_arr[ptr1][0] = mir[ptr3];
			temp_arr[ptr1][1] = mir[ptr3+1];
			temp_arr[ptr1][2] = mir[ptr3+2];
			temp_arr[ptr1][3] = mir[ptr3+3];
			temp_clr[ptr1][0] = 0.0f;
			temp_clr[ptr1][1] = 0.0f;
			temp_clr[ptr1][2] = 1.0f;
			ptr3 = ptr3+4;
			ptr5 = ptr5+1;
			if(ptr5>=mir_no)
			{
				flag=4;
				ptr3=0;
			}
		}
	}


	if(world_width>world_height)
		box_mag=(1.25f)*world_width;
	else
		box_mag=(1.25f)*world_height;

	glutDisplayFunc(drawScene);
	glutIdleFunc(drawScene);
//	glutKeyboardFunc(handleKeypress1);
//    glutSpecialFunc(handleKeypress2);
//    glutReshapeFunc(handleResize);
//	glutTimerFunc(10,update1,0);
    glutMainLoop();
    return 0;
}

void initRendering()
{
    glEnable(GL_DEPTH_TEST);        // Enable objects to be drawn ahead/behind one another
    glEnable(GL_COLOR_MATERIAL);    // Enable coloring
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Setting a background color
}

void drawScene() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();

// for box
	glTranslatef(0.0f, 0.0f, -box_mag);
	glColor3f(1.0f, 0.0f, 0.0f);
	drawBox(world_width,world_height);

// for projector
	int pro_j;
	for( pro_j=0 ; pro_j<proj_no+block_no+mir_no ; pro_j++ )
	{
		glPushMatrix();
		glTranslatef(temp_arr[pro_j][0], temp_arr[pro_j][1], 0.0f);
		glColor3f(temp_clr[pro_j][0], temp_clr[pro_j][1], temp_clr[pro_j][2]);
		float a,b;
		a=temp_arr[pro_j][2]-temp_arr[pro_j][0];
		b=temp_arr[pro_j][3]-temp_arr[pro_j][1];
		draw_projector(a,b);
		glPopMatrix();
	}


// for rays
	int ray_k,ray_j,ray_i=0;
	for( ray_j=0; ray_j<proj_no; ray_j++)
	{
		glPushMatrix();
		glTranslatef(temp_arr[ray_i][0], temp_arr[ray_i][1], 0.0f);
		float tr_x, tr_y, theta1;
		tr_x = temp_arr[ray_i][2]-temp_arr[ray_i][0];
		tr_y = temp_arr[ray_i][3]-temp_arr[ray_i][1];
		theta1 = (atan2(tr_y,tr_x));
		float pt_x,pt_y;
		glPopMatrix();
		pt_x = ((temp_arr[ray_i][0]+temp_arr[ray_i][2])/2) + light_dis*sin(theta1);
		pt_y = ((temp_arr[ray_i][1]+temp_arr[ray_i][3])/2) - light_dis*cos(theta1);
		float sec_x,sec_y;
		for(ray_k=0; ray_k<pixel_no; ray_k++)
		{
			ray_counter=0;
			if(up_counter%10 == 0 || up_counter%10 == 1 || up_counter%10 == 2 || up_counter%10 == 3 || up_counter%10 == 4)
				ray_counter=1;
			
			float r,g,b;
			sec_x = (((pixel_no-1-ray_k)*(temp_arr[ray_i][0]))+((ray_k)*(temp_arr[ray_i][2])))/(pixel_no-1);
			sec_y = (((pixel_no-1-ray_k)*(temp_arr[ray_i][1]))+((ray_k)*(temp_arr[ray_i][3])))/(pixel_no-1);
			if(ray_k < pixel_no/2)
			{
				r = 1 - 0.1*ray_k;
				g = 0.1*ray_k;
				b = 0;
			}
			else
			{
				r = 0;
				g = 1 - 0.1*(ray_k-pixel_no/2);
				b = 0.1*(ray_k-pixel_no/2);
			}
			glColor3f(r,g,b);
			draw_ray(pt_x,pt_y,sec_x,sec_y);
			ini_pt[0]=sec_x;
			ini_pt[1]=sec_y;
			float theta_here;
			theta_here=atan2(sec_y-pt_y,sec_x-pt_x);
			flag_ray=0;
			while(flag_ray==0)
			{
				if(ini_pt[0]<=-world_width/2 || ini_pt[0]>=world_width/2 || ini_pt[1]<=-world_height/2 || ini_pt[1]>=world_height/2)
				{
					break;
				}
				fin_pt[0]=ini_pt[0]+ 0.1*cos(theta_here);
				fin_pt[1]=ini_pt[1]+ 0.1*sin(theta_here);
				float x1,y1,x2,y2,x3,y3,x4,y4,x5,y5,x6,y6,x7,y7,m1,c1,m2,c2,dis_16,dis_56,dis_57,dis_37,ref_theta;
				for(for_ptr=proj_no;for_ptr<proj_no+block_no+mir_no;for_ptr++)
				{
					x1=ini_pt[0];
					y1=ini_pt[1];
					x2=fin_pt[0];
					y2=fin_pt[1];
					x3=temp_arr[for_ptr][0];
					y3=temp_arr[for_ptr][1];
					x4=temp_arr[for_ptr][2];
					y4=temp_arr[for_ptr][3];
					if(x2!=x1)
					{
						m1=(y2-y1)/(x2-x1);
						c1=(y1*x2-x1*y2)/(x2-x1);
					}
					if(x4!=x3)
					{
						m2=(y4-y3)/(x4-x3);
						c2=(y3*x4-x3*y4)/(x4-x3);
					}
					if(x2!=x1 && x4!=x3 && m1!=m2)
					{
						x5=(c2-c1)/(m1-m2);
						y5=(m1*c2-m2*c1)/(m1-m2);
					}
					else if((x2==x1 && x4==x3) || m1==m2)
						continue;
					else if(x2==x1)
					{
						m1=1;
						c1=1;
						x5=x1;
						y5=m2*x1+c2;
					}
					else if(x4==x3)
					{
						m2=1;
						c2=1;
						x5=x3;
						y5=m1*x3+c1;
					}
					x6=(x1+x2)/2;
					y6=(y1+y2)/2;
					x7=(x3+x4)/2;
					y7=(y3+y4)/2;
					dis_16=sqrt((x6-x1)*(x6-x1)+(y6-y1)*(y6-y1));
					dis_56=sqrt((x6-x5)*(x6-x5)+(y6-y5)*(y6-y5));
					dis_57=sqrt((x7-x5)*(x7-x5)+(y7-y5)*(y7-y5));
					dis_37=sqrt((x7-x3)*(x7-x3)+(y7-y3)*(y7-y3));
					if(dis_56<dis_16 && dis_37>dis_57)
					{
						glColor3f(r,g,b);
						draw_ray(x1,y1,x5,y5);
						if(for_ptr>=proj_no+block_no)
						{
							if(((x4-x3)*(y1-y3)-(x1-x3)*(y4-y3))<0)
							{
								ref_theta=atan2(y4-y3,x4-x3);             
								theta_here=+2*ref_theta-theta_here;
								fin_pt[0]=x5+0.1*cos(theta_here);
								fin_pt[1]=y5+0.1*sin(theta_here);
								continue;
							}
							else
							{
								flag_ray=1;
								break;
							}		
						}
						else
						{
							flag_ray=1;
							break;
						}
						
					}                                                            
				}   
				ray_counter++;                                                                
				if(flag_ray!=1)
				{
					glColor3f(r,g,b);
					if(flag_t==1)
					{
						if(ray_counter%2==0)
							draw_ray(x1,y1,x2,y2);       
					}
					else
						draw_ray(x1,y1,x2,y2);                             
				}
				ini_pt[0]=fin_pt[0];                                     
				ini_pt[1]=fin_pt[1];                                  
			}		                                
		}                                                        
		ray_i=ray_i+1;
	}
	glPopMatrix();
	glutSwapBuffers();       // responsible for window color
}

void drawBox(float wid,float hei) {

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_QUADS);
    glVertex2f(-wid / 2, -hei / 2);
    glVertex2f(wid / 2, -hei / 2);
    glVertex2f(wid / 2, hei / 2);
    glVertex2f(-wid / 2, hei / 2);
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void handleResize(int w, int h)			// responsible for box to come
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / (float)h, 0.1f, 200.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void draw_projector(float x2,float y2)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_LINES);
    glVertex2f(0.0f,0.0f);
    glVertex2f(x2,y2);
    glEnd();
}

void draw_ray(float x1, float y1, float x2,float y2)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_LINES);
    glVertex2f(x1,y1);
    glVertex2f(x2,y2);
    glEnd();
}

void update(int value) 
{
	int a,b;
	a=rand()%4;
	b=rand()%6;
	if(a==0)
	{
		if((temp_arr[b][0]-0.9)>=-world_width/2 && (temp_arr[b][2]-0.9)>=-world_width/2)
		{
       	    		temp_arr[b][0] -= 0.9;
            		temp_arr[b][2] -= 0.9;
		}
		else
		{
			temp_arr[b][0] += 5.0;
			temp_arr[b][2] += 5.0;
		}
	}
	else if(a==1)
	{
		if((temp_arr[b][0]+0.9)<=world_width/2 && (temp_arr[b][2]+0.9)<=world_width/2)
		{
       	    		temp_arr[b][0] += 0.9;
            		temp_arr[b][2] += 0.9;
		}
		else
		{
			temp_arr[b][0] -= 5.0;
			temp_arr[b][2] -= 5.0;
		}
	}
	else if(a==2)
	{
		if((temp_arr[b][1]+0.9)<=world_height/2 && (temp_arr[b][3]+0.9)<=world_height/2)
		{
       	    		temp_arr[b][1] += 0.9;
            		temp_arr[b][3] += 0.9;
		}
		else
		{
			temp_arr[b][1] -= 5.0;
			temp_arr[b][3] -= 5.0;
		}
	}
	else if(a==3)
	{
		if((temp_arr[b][1]-0.9)>=-world_height/2 && (temp_arr[b][3]-0.9)>=-world_height/2)
		{
      	    		temp_arr[b][1] -= 0.9;
            		temp_arr[b][3] -= 0.9;
		}
		else
		{
			temp_arr[b][1] += 5.0;
			temp_arr[b][3] += 5.0;
		}
	}
	if(flag_rand==1)
        	glutTimerFunc(10, update, 0);
}

void update1(int value)
{
	glutTimerFunc(10,update1,0);
	up_counter=up_counter+1;
}

void gen_output()
{
	int i;
	FILE *output;
	output=fopen("output.txt","w");
	fprintf(output,"%0.1f %0.1f\n",world_width,world_height);
	fprintf(output,"%d\n",proj_no);
	for(i=0; i<proj_no; i++)
	{
		fprintf(output,"%0.1f %0.1f %0.1f %0.1f\n",temp_arr[i][0],temp_arr[i][1],temp_arr[i][2],temp_arr[i][3]);
	}
	fprintf(output,"%0.1f\n",light_dis);
	fprintf(output,"%d\n",pixel_no);
	fprintf(output,"%d\n",block_no);
	for(i=proj_no; i<proj_no+block_no; i++)
	{
		fprintf(output,"%0.1f %0.1f %0.1f %0.1f\n",temp_arr[i][0],temp_arr[i][1],temp_arr[i][2],temp_arr[i][3]);
	}
	fprintf(output,"%d\n",mir_no);
	for(i=proj_no+block_no; i<proj_no+block_no+mir_no; i++)
	{
		fprintf(output,"%0.1f %0.1f %0.1f %0.1f\n",temp_arr[i][0],temp_arr[i][1],temp_arr[i][2],temp_arr[i][3]);
	}
	fclose(output);
}

void handleKeypress1(unsigned char key, int x, int y) 
{
	if (key == 27 || key == 'q') 
	{
		gen_output();
       	exit(0);     // escape key is pressed
    }
	if (key == 'c')
	{
		temp_clr[clr2][0] = temp_clr1[0];
		temp_clr[clr2][1] = temp_clr1[1];
		temp_clr[clr2][2] = temp_clr1[2];
		if (clr2>=5)
			clr2=0;
		else
			clr2=clr2+1;
		temp_clr1[0] = temp_clr[clr2][0];
		temp_clr1[1] = temp_clr[clr2][1];
		temp_clr1[2] = temp_clr[clr2][2];
		temp_clr[clr2][0] = 1.0f;
		temp_clr[clr2][1] = 1.0f;
		temp_clr[clr2][2] = 1.0f;
	}
	if (key == 114)   // r key is pressed 
	{
		float a1,b1;
		a1=temp_arr[clr2][2]-temp_arr[clr2][0];
		b1=temp_arr[clr2][3]-temp_arr[clr2][1];
		if(a1*cos(theta3)-b1*sin(theta3)+temp_arr[clr2][0]<=world_width/2 && a1*cos(theta3)-b1*sin(theta3)+temp_arr[clr2][0]>=-world_width/2 && b1*cos(theta3)+a1*sin(theta3)+temp_arr[clr2][1]<=world_height/2 && b1*cos(theta3)+a1*sin(theta3)+temp_arr[clr2][1]>=-world_height/2)
		{
			temp_arr[clr2][2]=a1*cos(theta3)-b1*sin(theta3)+temp_arr[clr2][0];
			temp_arr[clr2][3]=b1*cos(theta3)+a1*sin(theta3)+temp_arr[clr2][1];
		}
	}
	if (key == 108)  // l key is pressed
	{
		float a1,b1;
		a1=temp_arr[clr2][2]-temp_arr[clr2][0];
		b1=temp_arr[clr2][3]-temp_arr[clr2][1];
		if(a1*cos(theta2)-b1*sin(theta2)+temp_arr[clr2][0]<=world_width/2 && a1*cos(theta2)-b1*sin(theta2)+temp_arr[clr2][0]>=-world_width/2 && b1*cos(theta2)+a1*sin(theta2)+temp_arr[clr2][1]<=world_height/2 && b1*cos(theta2)+a1*sin(theta2)+temp_arr[clr2][1]>=-world_height/2)
		{
			temp_arr[clr2][2]=a1*cos(theta2)-b1*sin(theta2)+temp_arr[clr2][0];
			temp_arr[clr2][3]=b1*cos(theta2)+a1*sin(theta2)+temp_arr[clr2][1];
		}
	}
	if (key == 'z')
	{
		if(flag_rand==0)
		{
			flag_rand=1;
        		glutTimerFunc(10, update, 0);
		}
		else if(flag_rand==1)
			flag_rand=0;
	}
	if (key == 'f')
	{
		deltu=deltu+0.005;
		theta3=theta3-0.01;
		theta2=theta2+0.01;
	}
	if (key == 's')
	{
		deltu=deltu/2;
		theta3=theta3/2;
		theta2=theta2/2;
	}
	if (key == 't')
	{
		if(flag_t==0)
			flag_t = 1;
		else if(flag_t==1)
			flag_t = 0;
	}
}

void handleKeypress2(int key, int x, int y) 
{
    if (key == GLUT_KEY_LEFT)
	{
		if((temp_arr[clr2][0]-deltu) >= -world_width/2 && (temp_arr[clr2][2]-deltu) >= -world_width/2)
		{
       	    		temp_arr[clr2][0] -= deltu;
            		temp_arr[clr2][2] -= deltu;
		}
	}
    if (key == GLUT_KEY_RIGHT)
	{
		if((temp_arr[clr2][0]+deltu) <= world_width/2 && (temp_arr[clr2][2]+deltu) <= world_width/2)
		{
       	    		temp_arr[clr2][0] += deltu;
            		temp_arr[clr2][2] += deltu;
		}
	}
    if (key == GLUT_KEY_UP)
	{
		if((temp_arr[clr2][1]+deltu) <= world_height/2 && (temp_arr[clr2][3]+deltu) <= world_height/2)
		{
       	    		temp_arr[clr2][1] += deltu;
            		temp_arr[clr2][3] += deltu;
		}
	}
    if (key == GLUT_KEY_DOWN)
	{
		if((temp_arr[clr2][1]-deltu) >= -world_height/2 && (temp_arr[clr2][3]-deltu) >= -world_height/2)
		{
       	    		temp_arr[clr2][1] -= deltu;
            		temp_arr[clr2][3] -= deltu;
		}
	}
}



