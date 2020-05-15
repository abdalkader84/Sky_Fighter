//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Controls3D.hpp>
#include <FMX.Layers3D.hpp>
#include <FMX.Types.hpp>
#include <FMX.Viewport3D.hpp>
#include <System.Math.Vectors.hpp>
#include <FMX.Objects.hpp>
#include <FMX.Ani.hpp>
#include <FMX.Effects.hpp>
#include <FMX.Graphics.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.Media.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TViewport3D *IDE;
	TLayer3D *plain;
	TTimer *game_engin;
	TPath *plain1;
	TPath *lbsw;
	TPath *rbsw;
	TPath *lfsw;
	TPath *lf1;
	TPath *lf2;
	TPath *Path2;
	TPath *rfsw;
	TPath *rf2;
	TPath *rf1;
	TPath *Path3;
	TPath *plan_body;
	TPath *fhead;
	TPath *mhead;
	TPath *bhead;
	TPath *flame;
	TFloatAnimation *FloatAnimation1;
	TCircle *Circle1;
	TInnerGlowEffect *InnerGlowEffect1;
	TText *Text1;
	TCircle *Circle2;
	TLayout *lay_status;
	TLayout *Layout1;
	TRectangle *Rectangle1;
	TRoundRect *RoundRect1;
	TRoundRect *RoundRect2;
	void __fastcall game_enginTimer(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
	void __fastcall check_boundary(TViewport3D * ide,TLayer3D * pln);
	void __fastcall create_star(TViewport3D * ide,TComponent* Owner);
	void __fastcall move_object(TViewport3D * ide,TComponent* Owner,TLayer3D * pln);
	void __fastcall create_target(TViewport3D * ide,TComponent* Owner);
	void __fastcall create_bulet(TViewport3D * ide,TComponent* Owner,TLayer3D * pln);
	void __fastcall create_shrap(TViewport3D * ide,TComponent* Owner,TCircle * clr);
	unsigned int  __fastcall create_color();
	void __fastcall do_thread_event( );

};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
