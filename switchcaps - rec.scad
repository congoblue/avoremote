numb=3;
cap=["ent","ex","rec"];

   for (i=[0:numb-1])
   {

difference() {

   union () {
    translate ([i*12,0,-1.5+1.4]) cube ( [11.5,11.5,3], center = true ); //buttons below panel

    translate ([i*12,0,-1.5+3]) cube ( [11.5,11.5,3], center = true ); //buttons above panel

    translate ([i*12,6,-0.8]) cube ( [11.5,2,1.5], center = true ); //stop below

    translate ([i*12,-6.5,-0.4]) cube ( [11.5,3,0.4], center = true ); //hinge

    translate ([i*12,-7.6,-1]) cube ( [12,3,2], center = true ); //mount bar

    translate([i*12+5, 3.75, 2.65]) linear_extrude(0.4) rotate([0,0,180]) text(cap[i],size=5.5);



   //translate ([0,-8.5,0.7]) cube ( [49,4,1.4], center = true ); //buttons

   //translate ([0,-6.5,-0.4]) cube ( [49,4,3], center = true ); //buttons


   };



   translate ([i*12,-7.6,-0.75]) cube ( [11,1.5,1.5], center = true ); //locating recess

   };

   
};