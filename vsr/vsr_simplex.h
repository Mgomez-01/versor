/*
 * =====================================================================================
 *
 *       Filename:  vsr_simplex.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/03/2014 12:17:02
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */

#include "vsr_products.h"
#include "vsr_generic_op.h"

namespace vsr{



template<int N>
struct Simplex {

  struct Edge{                                            
    int a, b;  
    Edge(int _a, int _b) : a(_a), b(_b){}  
  //	void print() { printf("%d, %d\n", a, b);}
  };

  struct Face{
    int a,b,c;
    Face(int _a, int _b, int _c) : a(_a), b(_b), c(_c) {}
  };

  typedef NEVec<N> Vec;
  typedef NEBiv<N> Biv;

  //temporary higher dimensional elements for init
  typedef NEVec<N+1> TVec;

  vector<Vec>  verts;
  vector<TVec> roots;
	vector<Edge> edges; 
  vector<Face> faces;

  Simplex() { init(); }

  TVec vec;
 // TRot rot;

  TVec doBary(){
    TVec center;
    for (auto i : roots){
        center += i;
    }
    center /= (N+1);
    return center;
  }

   Vec bary(){
    Vec center;
    for (auto i : verts){
        center += i;
    }
    center /= (N+1);
    return center;
  }


  void doRot(){
    //Rotate out of extra dimension
    auto rot = Gen::ratio(doBary().unit(), vec);
    using B =  EGAMV<N+1, typename EGA<N+1>::template e<N+1>>;// NE<>::e<N+1>;
    using SubRot = decltype( doBary().unit() * B() );
    for(auto& i : roots){
      i = i.sp( SubRot(rot) );
    }
    //And project down by flattening
    for(auto& i : roots){
      verts.push_back(i);
    }
    //Rotate last to last
    Vec tvec; tvec[N-1] = 1;
    auto nrot = Gen::ratio( verts.back().unit(), tvec );
    using B2 = EGAMV<N, typename EGA<N>::template e<N>>;// NE<>::e<N>;
    using SubRot2 = decltype( verts.back().unit() * B2() );

    for(auto& i : verts){
      i = i.sp( SubRot2(nrot) );
    }

  }

  void init(){
    TVec center;
    //Roots in N+1 space
    for (int i=0;i<N+1;++i){
      TVec t; t[i] = 1;
      center +=t;
      roots.push_back( t );
      //Edges
      for (int j=i+1;j<N+1;++j){
        edges.push_back( Edge(i,j) );
        //Faces
        bool bSwitch = false;
        for (int k=j+1;k<N+1;++k){
          faces.push_back( bSwitch ? Face(i, j, k) : Face(i, k, j) );
          bSwitch = !bSwitch;
        }
      }
    }

    center /= (N+1);
    vec[N] = 1;
    doRot();

  }

  Biv biv(int which, float amt = 1.0) {
    return (verts[N] ^ verts[int(which)%N]) * amt;
  }

  void spin( const Biv& biv){
    for (auto& i : verts){
       i = i.sp( Gen::rot( biv ) );
     }
  }

  void spin(int which, float amt = 1.0){
    spin( biv(which,amt) );
  }

  vector<NEVec<3>> project (float dist, bool bOrtho=false) const{  
    vector< NEVec<3> > proj;
    for (auto i : verts){
      auto tmp = Proj<N>::Call(dist, i);
      NEVec<3> tmp2 = NEVec<3>(i);//Proj<N>::Ortho3(i);
      proj.push_back( bOrtho ? tmp2 : tmp );
    }
    return proj;
  }



};

} // vsr::
