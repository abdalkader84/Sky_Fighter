//---------------------------------------------------------------------------

#include <fmx.h>

#include <system.Diagnostics.hpp>
#include <system.Threading.hpp>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"

TForm1 *Form1;
unsigned long tim;
unsigned long star_tim=0;
unsigned long max_star_tim=7;

unsigned long target_tim=0;
unsigned long max_target_tim=75;

unsigned long bulet_tim=0;
unsigned long max_bulet_tim=5;

int y_rot_inc=10;
int y_rot_max=50;
int balance=0;
int sped=0;
int sped_max=14;
int star_sped=2;
int target_sped=10;
int bulet_sped=10;
int scor;

TStopwatch st;


//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
    srand(time(NULL));
}
//---------------------------------------------------------------------------
unsigned int  __fastcall TForm1::create_color()
{
	unsigned int clr=255;
	clr= clr << 8;
	int r=Random(255);
	clr = clr | r;
	clr = clr << 8;
	int g=Random(255);
	clr = clr | g;
	clr = clr <<8;
	int b=Random(255);
	clr = clr | b;
    return clr;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::create_star(TViewport3D * ide,TComponent* Owner)
{
	if (tim-star_tim>=max_star_tim)
	{
		TCircle * c=NULL;
		c= new TCircle(Owner);
		c->Position->X=Random(ide->Width);
		c->Position->Y=0;
		c->Width=RandomRange(1,5);
		c->Height=c->Width;
		c->Tag=1;
		ide->AddObject(c);
		star_tim=tim;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::create_target(TViewport3D * ide,TComponent* Owner)
{
	if (tim-target_tim>=max_target_tim)
	{
		TCircle * c=NULL;

		c=(TCircle *) (Circle1->Clone(ide));
        c->Parent=ide;
		c->Visible=true;
		c->Position->X=Random(ide->Width);
		c->Position->Y=0;
		c->Width=RandomRange(30,60);
		c->Height=c->Width;
		c->Tag=2;
		ide->AddObject(c);
		((TInnerGlowEffect *)(c->Children->Items[0]))->GlowColor=create_color();
		target_tim=tim;
	}

}
//---------------------------------------------------------------------------
void __fastcall TForm1::create_bulet(TViewport3D * ide,TComponent* Owner,TLayer3D * pln)
{
	if (tim-bulet_tim>=max_bulet_tim) {
		TCircle * c=NULL;
		c= new TCircle(ide);
		c->Parent=ide;
		c->Position->X=pln->Position->X;
		c->Position->Y=pln->Position->Y-pln->Height/2;
		c->Width=5;
		c->Height=c->Width;
		c->Tag=3;
		c->Fill->Color=0xffff5566;
		ide->AddObject(c);
		bulet_tim=tim;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::create_shrap(TViewport3D * ide,TComponent* Owner,TCircle * clr)
{
	for (int i=0; i<20; i++)
	{
		TCircle * c=NULL;
		c= new TCircle(Owner);
		c->Parent=ide;
		c->Position->Point=clr->AbsoluteRect.CenterPoint();
		c->Position->DefaultValue=clr->AbsoluteRect.CenterPoint();
		c->Width=Random(10);
		c->Height=c->Width;
		c->Tag=4;
        c->TagFloat=Random(360);
		c->Fill->Color=((TInnerGlowEffect *)clr->Children->Items[0])->GlowColor ;
		ide->AddObject(c);

	}
}
//---------------------------------------------------------------------------
TRectF get_rect(TLayer3D * pln)
{
	TRectF r;
	r.Top=pln->Position->Y-pln->Height/4;
	r.left=pln->Position->X-pln->Width/4;
	r.right=pln->Position->X+pln->Width/4;
	r.Bottom=pln->Position->Y+pln->Height/4;
	return r;
}
void __fastcall TForm1::move_object(TViewport3D * ide,TComponent* Owner,TLayer3D * pln)
{
	game_engin->Enabled=false;
	for (int i=0; i<ide->Controls->Count; i++)
	{
		TCircle * c = (TCircle *) ide->Controls->Items[i];

		if (c->Tag==1)
		{//star
			c->Position->Y=c->Position->Y+star_sped*(float)c->Width/5;
			c->Opacity=0.2+ (ide->Height-c->Position->Y ) /ide->Height;
		}
		else if (c->Tag==2)
		{//target
			c->Position->Y=c->Position->Y+target_sped/2 ;
			((TCircle *)c->Children->Items[1])->RotationAngle=
			((TCircle *)c->Children->Items[1])->RotationAngle+1.5*c->Width/60;
			TRectF r=get_rect(pln);
			if (c->AbsoluteRect.Intersects(r)==true) {
				//game_engin->Enabled=false;
			}
		}
		else if (c->Tag==3)
		{//bulet
			for (int j=0; j<ide->Controls->Count; j++) {
				TCircle * t = (TCircle *) ide->Controls->Items[j];
				if (t->Tag==2) {
					if (t->AbsoluteRect.Intersects(c->AbsoluteRect)==true) {
						create_shrap(ide,Owner,t);
						ide->RemoveObject(c);
						ide->RemoveObject(t);
						c->DisposeOf();
						t->DisposeOf();
						c=NULL;
						scor++;
						break;
					}
				}
			}
			if (c==NULL) {continue;	}
			c->Position->Y=c->Position->Y-2*bulet_sped ;
		}
		else if (c->Tag==4)
		{//shrap
			c->Position->Point =c->Position->Point + TPointF(cos(c->TagFloat*3.1415/180),
															 sin(c->TagFloat*3.1415/180));
			c->Opacity=c->Opacity-0.01;
			if ( (c->Position->Point-c->Position->DefaultValue).Length()>30 ) {

			   ide->RemoveObject(c);
			   c->DisposeOf();
               c=NULL;
			}

		}

        if (c==NULL) {continue;	}
		if (c->Position->Y>ide->Height) {
			ide->RemoveObject(c);
			c->DisposeOf();
		}
		else if (c->Position->Y<0){
			ide->RemoveObject(c);
			c->DisposeOf();
		}

	}
	game_engin->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::check_boundary(TViewport3D * ide,TLayer3D * pln)
{
	if (abs(balance)>y_rot_max) {
	   balance=y_rot_max*(abs(balance)/balance);
	}
	if (abs(sped)>sped_max) {
	   sped=sped_max*(abs(sped)/sped);
	}

	if (pln->Position->X-pln->Width/2<0) {
		pln->Position->X=pln->Width/2;
	}
	if (pln->Position->X+pln->Width/2>ide->Width) {
		pln->Position->X=ide->Width-pln->Width/2;
	}
	if (pln->Position->Y-pln->Height/2<0) {
		pln->Position->Y=pln->Height/2;
	}
	if (pln->Position->Y+pln->Height/2>ide->Height) {
		pln->Position->Y=ide->Height-pln->Height/2;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::game_enginTimer(TObject *Sender)
{
	tim++;
	//st.Start();
	create_star(IDE,Form1);
	create_target(IDE,Form1);
	move_object(IDE,Form1,plain);
	Text1->Text="Score : " +IntToStr(scor);

#if defined(__WIN32__)
    	if (GetKeyState(VK_LEFT)& 0x8000) {
    		balance+=y_rot_inc;
    	}
    	else if (GetKeyState(VK_RIGHT)& 0x8000) {
    		balance-=y_rot_inc;
    	}
    	else if (GetKeyState(VK_UP)& 0x8000) {
    		sped++;
    	}
    	else if (GetKeyState(VK_DOWN)& 0x8000) {
    		sped--;
    	}
    	else
    	{
    		balance+=y_rot_inc/20 *-sin(plain->RotationAngle->Y*3.1415/180)+5*sin(90*tim*3.1415/180);
    	}
    	if (GetKeyState(VK_SPACE)& 0x8000) {
    		create_bulet(IDE,Form1,plain);
		}

#endif
	check_boundary(IDE,plain);
	plain->RotationAngle->Y=balance   ;

	plain->Position->X=plain->Position->X +-balance/6;
	plain->Position->Y=plain->Position->Y+-sped/4;
	//st.Stop();
	//Text1->Text=IntToStr(st.ElapsedMilliseconds);
	//st.Reset();


}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
	_di_ITask task = TTask::Create([&](){
		while (1)
		{
			Sleep(10);
			create_star(IDE,Form1);
			create_target(IDE,Form1);
		}

	});
	//task->Start();
}
//---------------------------------------------------------------------------

