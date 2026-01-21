// Definitions
// **************************

inner_dia = 12;
thick = 1.2;

//Definition of circle angular resolution
$fn = 128;  

difference() {
   union (){
    //difference() {
    //translate ([0,0,0]) cylinder (h=17,r2=(inner_dia+thick*2)/2, r1=(inner_dia+thick+3)/2);
    //translate ([0,0,-13]) sphere (r=30);    
    //translate ([0,0,-43]) cube([100,100,100], center=true); //cut off rest of sphere
      
    //    }
    translate ([0,0,5]) cylinder (h=10.5,r2=16, r1=18); //bottom skirt
        
    }
    
    translate ([0,0,-thick/2]) cylinder (h=17-thick,r=6.05/2); //shaft hole
    
    
    translate ([0,0,0]) cylinder (h=6,r=inner_dia/2); //bottom recess

    //translate ([17,0,19]) sphere (r=10); //finger dimple
    translate ([15,0,14]) cylinder (h=10,r=inner_dia/2); //bottom recess
    translate ([-15,0,14]) cylinder (h=10,r=inner_dia/2); //bottom recess
    translate ([0,15,14]) cylinder (h=10,r=inner_dia/2); //bottom recess
    translate ([0,-15,14]) cylinder (h=10,r=inner_dia/2); //bottom recess

}
     
translate ([0,4.2,10.5]) cube ([7,4,9], center=true); //the D shaft



 