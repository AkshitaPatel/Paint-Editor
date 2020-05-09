#include<graphics.h>
#include<stdio.h>
#include<conio.h>
#include<dos.h>
#include<stdlib.h>
#include<math.h>


int SELTOOL=0,SELCOLOR=0,FILLCOL=0;
int tools[][4]={{0,50,50,100},{50,50,100,100},{0,100,50,150},{50,100,100,150},{0,150,50,200},{50,150,100,200},{0,200,50,250},{50,200,100,250},{0,250,50,300},{50,250,100,300}};


union REGS in,out;
int callmouse()
{	in.x.ax=1;  //shows mouse pointer
	int86(51,&in,&out);           //unions are sent to port
	return 1;
}
void setposi(int xp,int yp)
{	in.x.ax=4;     //sets position of mouse pointer on the screen
	in.x.cx=xp;
	in.x.dx=yp;
	int86(51,&in,&out); // interrupt to the port with interrupt number and union in and out
}

void mouseposi(int *xpos,int *ypos,int *click)
{ 	  in.x.ax=3;         //
	  int86(51,&in,&out);
	  *click=out.x.bx;     //takes the value of button pressed
	  *xpos=out.x.cx;     // takes the value of x coordinate
	  *ypos=out.x.dx;
}


//LAYOUT
void drawLayout()
{	int colpalate[][4]={{0,350,25,375},{25,350,50,375},{50,350,75,375},{75,350,100,375},{0,375,25,400},{25,375,50,400},{50,375,75,400},{75,375,100,400},{0,400,25,425},{25,400,50,425},{50,400,75,425},{75,400,100,425},{0,425,25,450},{25,425,50,450},{50,425,75,450},{75,425,100,450}};
	int arrowpoly[]={20,60,30,75,25,75,20,80,20,60};
	int arpl2[]={24,76,25,74,29,85,28,87,24,76};
	int i;
	cleardevice();

	//TITLE
	setfillstyle(1,BLUE);  //1 for solid fill
	bar(0,0,640,25);      // to draw a filled rectangle left top right bottom
	setcolor(WHITE);
	gotoxy(35,1);
	printf("Graphics Editor");

	//MENU
	setfillstyle(1,8);
	bar(0,25,640,50);

	setcolor(WHITE);
	rectangle(20,25,80,50);
	settextstyle(1,0,1);
	outtextxy(25,25,"Open");
	rectangle(120,25,180,50);
	settextstyle(1,0,1);
	outtextxy(125,25,"Save");

	//TOOLBAR
	setfillstyle(1,9);
	bar(0,50,100,480);

	//TOOLS
	setcolor(WHITE);
	for(i=0;i<10;i++)
	{	rectangle(tools[i][0],tools[i][1],tools[i][2],tools[i][3]);	}

	//1.ARROW
	setfillstyle(1,WHITE);
	fillpoly(5,arrowpoly);
	fillpoly(5,arpl2);

	//2.LINE
	line(60,90,90,60);

	//3.RECTANGLE
	rectangle(10,110,40,140);

	//4.CIRCLE
	circle(75,125,17);

	//5.SCALE UP
	rectangle(5,155,45,195);
	for(i=5;i<25;i+=10)
	{	line(i,175,i+5,175);}
	for(i=175;i<195;i+=10)
	{	line(25,i,25,i+5);	}

	//6.SCALE DOWN
	rectangle(55,175,75,195);
	for(i=155;i<175;i+=10)
	{	line(55,i,55,i+5); }
	for(i=155;i<195;i+=10)
	{	line(95,i,95,i+5); }
	for(i=55;i<95;i+=10)
	{	line(i,155,i+5,155); }
	for(i=95;i>75;i-=10)
	{	line(i,195,i-5,195); }

	//7.DELETE
	setcolor(WHITE);
	line(15,215,35,235);
	line(35,215,15,235);

	//8.SET	COLOR
	settextstyle(1,0,2);  //
	outtextxy(55,205,"COL");

	//9.SELECTED COLOR
	setcolor(WHITE);
	rectangle(10,260,40,290);
	setfillstyle(1,0);
	bar(11,261,39,289);

	//COLOR PALATE
	for(i=0;i<16;i++)
	{	setfillstyle(1,i);
		bar(colpalate[i][0],colpalate[i][1],colpalate[i][2],colpalate[i][3]);
	}

	//DRAWING AREA
	setfillstyle(1,WHITE);
	bar(100,50,640,455);

	//STATUS BAR
	setfillstyle(1,7);
	bar(100,455,640,480);
}

void resetSelection()
{   int i;
	setcolor(WHITE);
	for(i=0;i<10;i++)
	{	rectangle(tools[i][0],tools[i][1],tools[i][2],tools[i][3]);	}
}

void dda(int x1,int y1,int x2,int y2,int cl)
{ int len,i,s;
  float x,y,dx,dy;
  len=(abs(x2-x1)>=abs(y2-y1))?abs(x2-x1):abs(y2-y1);
  dx=(float)(x2-x1)/len;
  dy=(float)(y2-y1)/len;
  s=(dx==0)?0:(x2-x1)/abs(x2-x1);
  x=x1+0.5*s;
  s=(dy==0)?0:(y2-y1)/abs(y2-y1);
  y=y1+0.5*s;
  for(i=1;i<=len;i++)
  { putpixel((int)x,(int)y,cl);
    x+=dx;
	y+=dy;
  }
}

void rect(int x1,int y1,int x2,int y2,int cl)
{ 	dda(x1,y1,x2,y1,cl);
	dda(x2,y1,x2,y2,cl);
	dda(x2,y2,x1,y2,cl);
	dda(x1,y2,x1,y1,cl);
}

void fillRect(int x1,int y1,int x2,int y2,int cl)
{ int y,s1,s2;
  s1=(x1<x2)?1:(-1);
  s2=(y1<y2)?1:(-1);
  for(y=y1+1;y<y2;y+=s2)
  { dda(x1+s1,y,x2-s1,y,cl);}
}

void circlebresen(int x0, int y0, int radius,int col)
{
  int error = 1 - radius;
  int errorY = 1;
  int errorX = -2 * radius;
  int x = radius, y = 0;

  putpixel(x0, y0 + radius,col);
  putpixel(x0, y0 - radius,col);
  putpixel(x0 + radius, y0,col);
  putpixel(x0 - radius, y0,col);

  while(y < x)
  {
    if(error > 0) // >= 0 produces a slimmer circle. =0 produces the circle picture at radius 11 above
    {
      x--;
      errorX += 2;
      error += errorX;
    }
    y++;
    errorY += 2;
    error += errorY;
    putpixel(x0 + x, y0 + y,col);
    putpixel(x0 - x, y0 + y,col);
    putpixel(x0 + x, y0 - y,col);
    putpixel(x0 - x, y0 - y,col);
    putpixel(x0 + y, y0 + x,col);
    putpixel(x0 - y, y0 + x,col);
    putpixel(x0 + y, y0 - x,col);
    putpixel(x0 - y, y0 - x,col);
  }
}

void fillCircle(int x0,int y0,int radius,int fcol)
{ int error = 1 - radius;
  int errorY = 1;
  int errorX = -2 * radius;
  int x = radius, y = 0;

  dda(x0,y0+radius-1,x0,y0-radius+1,fcol);
  dda(x0+radius-1,y0,x0-radius+1,y0,fcol);

  while(y < x)
  {
    if(error > 0) // >= 0 produces a slimmer circle. =0 produces the circle picture at radius 11 above
    {
      x--;
      errorX += 2;
      error += errorX;
    }
    y++;
    errorY += 2;
    error += errorY;    
    dda(x0+x-1,y0+y,x0-x+1,y0+y,fcol);    
	dda(x0+x-1,y0-y,x0-x+1,y0-y,fcol);    
	dda(x0+y,y0+x-1,x0-y,y0+x-1,fcol);    
	dda(x0+y,y0-x+1,x0-y,y0-x+1,fcol);
  }
}

//STORAGE STRUCTURES & FUNCTIONS
struct shape
{ int opcode;
  int *params;
  int bcol,fcol;
  struct shape *next;
}*START;
typedef struct shape node;

void insert(int op,int x1,int y1,int x2,int y2,int bcol,int fcol)
{ node *n1,*ptr;
  n1=(node*)malloc(sizeof(node));
  n1->opcode=op;
  if(op!=3)
  { n1->params=(int*)malloc(4*sizeof(int));
    n1->params[0]=x1;
	n1->params[1]=y1;
	n1->params[2]=x2;
	n1->params[3]=y2;
  }
  else if(op==3)
  { n1->params=(int*)malloc(3*sizeof(int));
    n1->params[0]=x1;
	n1->params[1]=y1;
	n1->params[2]=x2;
  }
  n1->bcol=bcol;
  n1->fcol=fcol;
  n1->next=NULL;
  if(START==NULL)
  {	START=n1; }
  else
  { ptr=START;
    while(ptr->next!=NULL)
	{ ptr=ptr->next; }
	ptr->next=n1;
  }
}

void showDraw()
{ node *ptr;
  ptr=START;
  setfillstyle(1,WHITE);
  bar(100,50,640,455);
  while(ptr!=NULL)
  { if(ptr->opcode==1)
    { dda(ptr->params[0],ptr->params[1],ptr->params[2],ptr->params[3],ptr->bcol); }
	else if(ptr->opcode==2)
	{ rect(ptr->params[0],ptr->params[1],ptr->params[2],ptr->params[3],ptr->bcol); 
	  if(ptr->fcol!=WHITE)
	  { fillRect(ptr->params[0],ptr->params[1],ptr->params[2],ptr->params[3],ptr->fcol);  }
	}
	else if(ptr->opcode==3)
	{ circlebresen(ptr->params[0],ptr->params[1],ptr->params[2],ptr->bcol); 
	  if(ptr->fcol!=WHITE)
	  { fillCircle(ptr->params[0],ptr->params[1],ptr->params[2],ptr->fcol); }
	}
    ptr=ptr->next;
  }
}

node* selObject(int x,int y)
{  int mindist,mx,my;
   long int min;
   node *ptr,*obj;
   ptr=START;
   obj=NULL;
   mindist=700;
   while(ptr!=NULL)
   { if(ptr->opcode==1)
     {  mx=(ptr->params[0]+ptr->params[2])/2; 
		my=(ptr->params[1]+ptr->params[3])/2; 
		min=(long int)(x-mx)*(x-mx)+(long int)(y-my)*(y-my); 
		min=sqrt(min);
	 }
	 else if(ptr->opcode==2)
	 {  mx=(ptr->params[0]+ptr->params[2])/2; 
		my=(ptr->params[1]+ptr->params[3])/2; 
		min=(long int)(x-mx)*(x-mx)+(long int)(y-my)*(y-my); 
		min=sqrt(min);
	 }
	 else
	 {  mx=ptr->params[0];
	    my=ptr->params[1];
		min=(long int)(x-mx)*(x-mx)+(long int)(y-my)*(y-my); 
		min=sqrt(min);
	 }
	 if(min<mindist)
     { mindist=min;
       obj=ptr;
     }	   
	 ptr=ptr->next;
   }
   return obj;
}

void deleteObject(node *obj)
{ node *ptr;
  if(START==obj)
  { START=obj->next;  }
  else
  { ptr=START;
	while(ptr!=NULL)
	{ if(ptr->next==obj)
	  { break;	  }
	  ptr=ptr->next;  
	} 
	if(ptr!=NULL)
	{ if(obj->next==NULL)
	  { ptr->next=NULL; }
	  else
	  { ptr->next=obj->next; }
	}
  }
}

void scaleObj(node *obj,float sf)
{ int xc,yc,x,y,dx,dy;
  if(obj->opcode!=3)
  {  xc=(obj->params[0]+obj->params[2])/2;
	 yc=(obj->params[1]+obj->params[3])/2;
	 x=(float)xc*sf;
	 y=(float)yc*sf;
	 dx=x-xc;
	 dy=y-yc;
	 obj->params[0]=(float)(obj->params[0])*sf-dx;
	 obj->params[2]=(float)(obj->params[2])*sf-dx;
	 obj->params[1]=(float)(obj->params[1])*sf-dy;
	 obj->params[3]=(float)(obj->params[3])*sf-dy;
  }
  else
  {  obj->params[2]=(float)(obj->params[2])*sf; }
}

void saveFile()
{  FILE *fp;
   node *ptr;
   float x1,y1,x2,y2;
   ptr=START;
   fp=fopen("G_FILE.txt","w");
   while(ptr!=NULL)
   { if(ptr->opcode!=3)
     { x1=(float)(ptr->params[0])/639.0;
	   x2=(float)(ptr->params[2])/639.0;
	   y1=(float)(ptr->params[1])/479.0;
	   y2=(float)(ptr->params[3])/479.0;
	 }
	 else
	 { x1=(float)(ptr->params[0])/639.0;
	   y1=(float)(ptr->params[1])/479.0;	   
	   x2=(float)(ptr->params[2])/675.0;
	   y2=0;
	  }
	  fprintf(fp,"%d %f %f %f %f %d %d \n",ptr->opcode,x1,y1,x2,y2,ptr->bcol,ptr->fcol);
	  ptr=ptr->next;
   }
   fclose(fp);
}

void openFile()
{ FILE *fp;
   float x1,y1,x2,y2;
   int op,bcol,fcol;
   char c=' ';
   START=NULL;
   fp=fopen("G_FILE.txt","r");
   if(fp==NULL)
   { gotoxy(30,26);
     printf("\n FILE NOT FOUND"); 
	 return;
   }
   do
   { fscanf(fp,"%d %f %f %f %f %d %d",&op,&x1,&y1,&x2,&y2,&bcol,&fcol);
     if(op!=3)
	 { x1*=639.0;
	   x2*=639.0;
	   y1*=479.0;
	   y2*=479.0;
	   insert(op,x1,y1,x2,y2,bcol,fcol);
	 }
	 else
	 { x1*=639.0;
	   y1*=479.0;
	   x2*=675.0;
	   insert(op,x1,y1,x2,0,bcol,fcol);
	 }
   }while((c=getc(fp))!=EOF);
   fclose(fp);
}
	   
int main()
{	int gd=DETECT,gm;
	int x,y,cl,ix,iy,id=0,oldid=1,x1,y1;
	long dx,dy,rad;
	node *selobj;
	char sts[8][21]={"Select","Line","Rectangle","Circle","Scale Up","Scale Down","Delete Object","Fill Object"};
	initgraph(&gd,&gm,"..//bgi");

	START=NULL;
	drawLayout();
	callmouse();
	setposi(100,130);
	setcolor(BLACK);

	while(!kbhit())
	{	mouseposi(&x,&y,&cl);
		if(x>=0 && x<100 && y>50 && y<250)
		{ 	ix=x/50; iy=(y-50)/50;
			id=ix+iy*2;
			if(id!=oldid)
			{	setfillstyle(1,7);
				bar(100,455,420,480);
				setcolor(0);
				settextstyle(1,0,1);
				outtextxy(150,455,sts[id]);
			}
			oldid=id;
			if(cl==1)
			{	if(id<4)
			    { resetSelection();
				  ix=ix*50;	iy=iy*50+50;
				  setcolor(RED);
				  rectangle(ix,iy,ix+50,iy+50);
				  cl=0;
				  delay(500);
				  SELTOOL=id;
				}
				else
				{ switch(id)
				  { case 4 : if(selobj!=NULL)
				             { scaleObj(selobj,1.1); 
							   showDraw();
							   selobj=NULL;
							 }
							 break;

				    case 5 : if(selobj!=NULL)
					     { scaleObj(selobj,0.9);
							   showDraw();
							   selobj=NULL;
							 }
							 break;
				    case 6 : if(selobj!=NULL)
					     { deleteObject(selobj);
							   showDraw();
							   selobj=NULL;
							 }
							 break;
				    case 7: if(selobj!=NULL)
							{ showDraw();
							  if(selobj->opcode==1)
							  { selobj->bcol=SELCOLOR;
								dda(selobj->params[0],selobj->params[1],selobj->params[2],selobj->params[3],selobj->bcol);
							  }
							  else if(selobj->opcode==2)
							  { selobj->fcol=FILLCOL;
								fillRect(selobj->params[0],selobj->params[1],selobj->params[2],selobj->params[3],selobj->fcol);
							  }
							  else if(selobj->opcode==3)
							  { selobj->fcol=FILLCOL;
								fillCircle(selobj->params[0],selobj->params[1],selobj->params[2],FILLCOL);
							  }
							  selobj=NULL;
							}
							break;
				    default : break;
				  }
				}
			}
		}
		else if(x>=120 && x<180 && y>=25 && y<=50 && cl==1)
		{  saveFile();
		   showDraw();
		}
		else if(x>=20 && x<80 && y>=25 && y<=50 && cl==1)
		{  openFile(); 
		   showDraw();
		}
		else if(x>=0 && x<100 && y>350 && y<450 && cl==1)
		{	ix=x/25; iy=(y-350)/25;
			id=ix+iy*4;
			setfillstyle(1,id);
			bar(11,261,39,289);
			cl=0;
			delay(500);
			SELCOLOR=id;
			FILLCOL=id;
		}
		else if(x>=100 && x<640 && y>50 && y<=455 && cl==1)
		{ switch(SELTOOL)
		  { case 0: showDraw();
					selobj=selObject(x,y);
					if(selobj!=NULL)
					{ if(selobj->opcode==1)
					  { x1=selobj->params[0];
						y1=selobj->params[1];
						x=selobj->params[2];
						y=selobj->params[3];
					  }
					  else if(selobj->opcode==2)
					  { x1=selobj->params[0]-5;
						y1=selobj->params[1]-5;
						x=selobj->params[2]+5;
						y=selobj->params[3]+5;
					  }
					  else if(selobj->opcode==3)
					  { x1=selobj->params[0]-selobj->params[2]-5;
						y1=selobj->params[1]-selobj->params[2]-5;
						x=selobj->params[0]+selobj->params[2]+5;
						y=selobj->params[1]+selobj->params[2]+5;
					  }
					 rect(x1,y1,x,y,7);
					}
					break;
			case 1: x1=x; y1=y;
					while(cl!=0)
					{ mouseposi(&x,&y,&cl);
					  if(x<=100 || x>=639 || y<=55 || y>=450)
					  { cl=0; break; }
					  setfillstyle(1,WHITE);
					  bar(100,50,640,455);
					  dda(x1,y1,x,y,SELCOLOR);
					}
					showDraw();
					dda(x1,y1,x,y,SELCOLOR);
					insert(1,x1,y1,x,y,SELCOLOR,WHITE);
					break;
			case 2: x1=x; y1=y;
					while(cl!=0)
					{ mouseposi(&x,&y,&cl);
					  if(x<=100 || x>=639 || y<=55 || y>=450)
					  { cl=0; break; }
					  setfillstyle(1,WHITE);
					  bar(100,50,640,455);
					  rect(x1,y1,x,y,SELCOLOR);
					}
					showDraw();
					rect(x1,y1,x,y,SELCOLOR);
					insert(2,x1,y1,x,y,SELCOLOR,WHITE);
					break;
		    case 3: x1=x; y1=y;
					while(cl!=0)
					{ mouseposi(&x,&y,&cl);
					  setfillstyle(1,WHITE);
					  bar(100,50,640,455);
					  dx=(long int)(x-x1)*(x-x1);
					  dy=(long int)(y-y1)*(y-y1);
					  rad=sqrt(dx+dy);
					  circlebresen(x1,y1,rad,SELCOLOR);
					  if((x1-rad)<=100 || (x1+rad)>=639 || (y1-rad)<=55 || (y1+rad)>=450)
					  { cl=0; break; }
					}
					showDraw();
					dx=(long int)(x-x1)*(x-x1);
					dy=(long int)(y-y1)*(y-y1);
					rad=sqrt(dx+dy);
					circlebresen(x1,y1,rad,SELCOLOR);
					insert(3,x1,y1,rad,0,SELCOLOR,WHITE);
					delay(500);
					break;
			default: break;
		  }
		}
	}
	getch();
	closegraph();
	return 0;
}

