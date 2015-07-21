/*
  
  Pablo Colapinto
  VSR2.0

*/                         

#include "draw/vsr_cga3D_draw.h"

namespace gfx{ 
                          
  
  //namespace cga{

  //using namespace cga;
  using namespace vsr;
  using vsr::cga::op;
  
  template<> void Renderable< cga::Vec >::DrawImmediate (const cga::Vec& s){
    //cout << "vec" << endl; 
    gfx::Glyph::Line(s);
    glPushMatrix();  
      gfx::GL::translate( s.begin() );
      gfx::GL::rotate( op::AA(s).begin() );  
      Glyph::Cone();
    glPopMatrix();
  }  

  
//    template<> void Renderable< cga:: >::DrawImmediateB (const  cga::Vec& s){
//      //cout << "ehl" << endl;
//      gfx::Glyph::Line(s);
//      glPushMatrix();  
//      gfx::GL::translate( s.begin() );
//      gfx::Glyph::SolidSphere(.05,5,5);
//      glPopMatrix();                       
//  }
  
  template<> void Renderable< cga::Biv >::DrawImmediate(const  cga::Biv& s){
		double ta = s.norm(); 
	  bool sn = op::sn( s ,  cga::Biv::xy * (-1));
	
		glPushMatrix();	
			gfx::GL::rotate( op::AA(s).begin() );  
			gfx::Glyph::DirCircle( ta, sn );
		glPopMatrix();
  }
  
  template<> void Renderable< cga::Tnv >::DrawImmediate (const  cga::Tnv& s){
    Renderable< cga::Vec >::DrawImmediate( s.copy< cga::Vec>() );
  }   
  
                    
   template<> void Renderable< cga::Drv >::DrawImmediate (const  cga::Drv& s){ 
    Renderable< cga::Vec >::DrawImmediate( s.copy< cga::Vec>() );
  }

  template<> void Renderable< cga::DualPlane >::DrawImmediate (const  cga::DualPlane& s){
    gfx::GL::translate( op::Pos(s).begin() );
    gfx::GL::rotate( op::AA(s).begin() ); 
    Glyph::SolidGrid(6,6,5);
      //Glyph::Rect(10,10);
  }
  template<> void Renderable< cga::Plane >::DrawImmediate (const  cga::Plane& s){
    Renderable< cga::DualPlane >::DrawImmediate(s.dual());
  }

  template<> void Renderable< cga::Circle >::DrawImmediate( const  cga::Circle& s )  {  
    VSR_PRECISION rad = nga::round::rad( s );
    bool im = nga::round::size(s, false) > 0 ? 1 : 0;  
     
    gfx::GL::translate( op::Pos(s).begin() );
    gfx::GL::rotate( op::AA(s).begin() ); 

    im ? gfx::Glyph::Circle( rad ) :  gfx::Glyph::DashedCircle( rad );            
  }  
  
  template<> void Renderable< cga::Point >::DrawImmediate (const  cga::Point& s){
 
      VSR_PRECISION ta = nga::round::size( s, true );

      //Draw as dual Sphere (if |radius^2| > 0.000001);
      if ( fabs(ta) >  FPERROR ) {
       // printf("spehere!!!!!!!!!!!!!!!!!!!!\n");
          bool real = ta > 0 ? 1 : 0;  

           cga::Pnt p = nga::round::cen( s );
          VSR_PRECISION t = sqrt ( fabs ( ta ) );

          gfx::GL::translate ( p.begin() );
          (real) ? gfx::Glyph::SolidSphere(t, 5+ floor(t*30), 5+floor(t*30)) : Glyph::Sphere(t);  
      } else {  
       // printf("NOOOOOO\n");
          gfx::Glyph::Point(s);
      }
  } 
  
  template<> void Renderable< cga::Sphere >::DrawImmediate (const  cga::Sphere& s){
 
      VSR_PRECISION ta = nga::round::size( s, false );

      //Draw as dual Sphere (if |radius^2| > 0.000001);
      if ( fabs(ta) >  FPERROR ) {
       // printf("spehere!!!!!!!!!!!!!!!!!!!!\n");
          bool real = ta > 0 ? 1 : 0;  

           cga::Pnt p = nga::round::cen( s );
          VSR_PRECISION t = sqrt ( fabs ( ta ) );

          gfx::GL::translate ( p.begin() );
          (real) ? gfx::Glyph::SolidSphere(t, 5+ floor(t*30), 5+floor(t*30)) : Glyph::Sphere(t);  
      } else {  
       // printf("NOOOOOO\n");
          gfx::Glyph::Point(s);
      }
  }
  
  template<> void Renderable< cga::FlatPoint >::DrawImmediate (const cga::FlatPoint& s){
     Renderable< cga::Point >::DrawImmediate( nga::round::point( s[0], s[1], s[2] ) );
  }
  
  template<> void Renderable< cga::Pair >::DrawImmediate (const  cga::Pair& s){
       //Is Imaginary?
       VSR_PRECISION size = nga::round::size( s, false );

       //is null?
       if ( fabs(size) < FPERROR ){
           GL::translate( nga::round::loc(s).begin() );
            Renderable< cga::Vec >::DrawImmediate( -nga::round::dir(s).copy< cga::Vec>() ); 
         
       }else{
       
         std::vector< cga::Pnt> pp = nga::round::split( s );

         VSR_PRECISION ta = nga::round::size( pp[0], true );   
                                      
         if ( fabs(ta) >  FPERROR ) {    
              cga::Pnt p1 = nga::round::cen( pp[0] );
              cga::Pnt p2 = nga::round::cen( pp[1] );
             double t = sqrt ( fabs ( ta ) );
             bool real = size > 0 ? 1 : 0;  

             glPushMatrix();
             gfx::GL::translate ( p1.begin() );//(p1[0], p1[1], p1[2]);
             (real) ? gfx::Glyph::SolidSphere(t, 5+ floor(t*30), 5+floor(t*30)) : gfx::Glyph::Sphere(t);  
             glPopMatrix();

             gfx::GL::translate ( p2.begin() );
             (real) ? gfx::Glyph::SolidSphere(t, 5+ floor(t*30), 5+floor(t*30)) : gfx::Glyph::Sphere(t);  

         } else {
        // pp[0].vprint(); pp[1].vprint();
             gfx::Glyph::DashedLine(pp[0],pp[1]);
             gfx::Glyph::Point(pp[0]);
             gfx::Glyph::Point(pp[1]);

         }
       }
  }  
  
  template<> void Renderable< cga::DualLine >::DrawImmediate (const  cga::DualLine& s){
       cga::Drv d = nga::flat::dir( s.undual() );
       cga::Dls v = nga::flat::loc( s , PAO, true);
      gfx::GL::translate (v.begin());
      gfx::Glyph::DashedLine(d * 10, d * -10);  
  }  
  
  template<> void Renderable< cga::Line >::DrawImmediate (const  cga::Line& s){
       cga::Drv d = nga::flat::dir( s );
       cga::Dls v = nga::flat::loc( s , nga::round::point(0,0,0), false);
      gfx::GL::translate (v.begin());
      gfx::Glyph::Line(d * 10, d * -10);  
  }
  
  template<> void Renderable< cga::Frame >::DrawImmediate( const  cga::Frame& f){
     gfx::GL::translate ( f.pos().begin() );
     gfx::GL::rotate( nga::gen::aa( f.rot() ).begin() ); 
     gfx::GL::scale( f.scale() );  
     gfx::Glyph::Axes(  cga::Vec::x,  cga::Vec::y,  cga::Vec::z );
  }  

//  template<> void Renderable< cga:: >::DrawImmediateB( const  cga::Frame& f){
//
//     gfx::GL::translate( f.pos().begin() );
//     gfx::GL::rotate( nga::gen::aa( f.rot() ).begin() ); 
//     gfx::GL::scale( f.scale() );  
//     Draw(  cga::Vec::x,1,0,0);
//     Draw(  cga::Vec::y,0,1,0);
//     Draw(  cga::Vec::z,0,0,1);
//  }  
  

  template<> void Renderable< Field<cga::Sca> >::DrawImmediate( const Field< cga::Sca>& f){
     static cga::Pnt p =  cga::Pnt();
    for (int i = 0; i < f.num(); ++i){  
      render::drawAt(p, f.grid(i));//, f[i][0], 1, 1 - f[i][0] ); 
    }
  }

  template<> void Renderable< Field<cga::Vec> >::DrawImmediate( const Field< cga::Vec>& f){
    for (int i = 0; i < f.num(); ++i){  
      render::drawAt( f[i], f.grid(i) );// f[i][0], 1, 1 - f[i][0] ); 
    }
  }

//  template<> void Renderable< cga:: >::DrawImmediateB( const Field< cga::Vec>& f){
//    for (int i = 0; i < f.num(); ++i){  
//      DrawAtB( f[i], f.grid(i) );// f[i][0], 1, 1 - f[i][0] ); 
//    }
//  }

  template<> void Renderable< Field<cga::Tnv> >::DrawImmediate( const Field< cga::Tnv>& f){
    for (int i = 0; i < f.num(); ++i){  
      render::drawAt( f[i], f.grid(i) );// f[i][0], 1, 1 - f[i][0] ); 
    }
  }  

  template<> void Renderable< Field<cga::Frame> >::DrawImmediate( const Field< cga::Frame>& f){
    for (int i = 0; i < f.num(); ++i){  
      glPushMatrix(); 
      Renderable< cga::Frame >::DrawImmediate( f[i] ); 
      glPopMatrix(); 
    }
  } 
  
   template<> void Renderable< Field<cga::Point> >::DrawImmediate( const Field< cga::Point>& f){
     for (int i = 0; i < f.num(); ++i){  
      //Draw( f[i] ); 
      render::begin();
      render::draw(f[i]);
    }
  }

//  /*-----------------------------------------------------------------------------
//   *  Specific Strategies
//   *-----------------------------------------------------------------------------*/
//  template<>  void Renderable<Field<cga::Sca>>::DrawImmediate( const Field<cga::Sca>& f){
//    static Pnt p = Pnt();
//    for (int i = 0; i < f.num(); ++i){ 
//      render::color(f[i][0], 1, 1 - f[i][0]);
//      render::drawAt(p,f.grid(i));  
//    }
//  }
//
//  template<>  void Renderable<Field<cga::Vec>>::DrawImmediate( const Field<cga::Vec>& f){
//    for (int i = 0; i < f.num(); ++i){  
//      render::drawAt( f[i], f.grid(i) );
//    }
//  }
//
//
//  template<>  void Renderable<Field<cga::Tnv>>::DrawImmediate( const Field<cga::Tnv>& f){
//    for (int i = 0; i < f.num(); ++i){  
//      render::drawAt( f[i], f.grid(i) );
//    }
//  }  
  


   /* void Immediate( const MFrame& f){ */
   /*   gfx::GL::translate ( f.pos().begin() ); */
   /*   gfx::GL::rotate( nga::gen::aa( f.rot() ).begin() ); */ 
   /*   gfx::GL::scale( f.scale() ); */  
   /*   gfx::Glyph::Axes( Vec::x, Vec::y, Vec::z ); */
  /* } */   

  /* void Immediate( const Field<Frame>& f){ */
  /*   for (int i = 0; i < f.num(); ++i){ */  
  /*     glPushMatrix(); */ 
  /*     Immediate( f[i] ); */ 
  /*     glPopMatrix(); */ 
  /*   } */
  /* } */  
  
  /* void Immediate( const Field<Vec>& f){ */ 
  /*    // printf("draw vec field\n"); */
  /*   for (int i = 0; i < f.num(); ++i){ */  
  /*     glPushMatrix(); */
  /*       gfx::GL::translate( f.grid(i).begin() ); */ 
  /*        //glPushMatrix(); */ 
  /*         Immediate( f[i] ); */ 
  /*       //glPopMatrix(); */ 
  /*     glPopMatrix(); */   
      
  /*   } */
  /* } */ 

//  template<class T>
//  void Immediate(const Field<T>& f){
//    for (int i = 0; i < f.num(); ++i){  
//      Draw(f[i]);
//    }
//  }
//  template void Draw(const  cga::Vec&, float, float, float ,float);
//  template void DrawB(const  cga::Vec&, float, float, float ,float);
//  template void Draw(const  cga::Dlp&, float, float, float ,float);
//  template void Draw(const  cga::Pln&, float, float, float ,float);  
//  template void Draw(const  cga::Pnt&, float, float, float ,float);
//  template void Draw(const  cga::Flp&, float, float, float ,float); 
//  template void Draw(const  cga::Par&, float, float, float ,float);  
//  template void Draw(const  cga::Cir&, float, float, float ,float);  
//  template void Draw(const  cga::Sph&, float, float, float ,float);
//  template void Draw(const  cga::Dll&, float, float, float ,float);
//  template void Draw(const  cga::Lin&, float, float, float ,float);
//  template void Draw(const  cga::Frame&, float, float, float ,float);
//  template void DrawB(const  cga::Frame&, float, float, float ,float);
//
////  template void Draw(const MFrame&, float, float, float ,float);
//  template void Draw(const Field< cga::Vec>&, float, float ,float,float); 
//  template void DrawB(const Field< cga::Vec>&, float, float ,float,float); 
  //template<class T> void Draw( const Field<T>&, float, float, float);
  // template void Draw(const Vec&, float, float, float ,float);
  // template void Draw(const Vec&, float, float, float ,float);
  // template void Draw(const Vec&, float, float, float ,float);  
    

//  }//cga::

} //vsr::
    
