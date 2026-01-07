// Definition size of housing
// **************************

       // Those settings have to be identical to the cover !!!
width_x      = 205;          // Width of the housing (outer dimension) 
debth_y      = 100;           // Debth of the housing (outer dimension)
wall         = 1.8;          // Wall thickness of the box
cornerradius = 4.0;          // Radius of the corners
                             //   This value also defines the posts for stability and
                             //   for the press-in nuts!

       // Those settings are more or less independent from the cover
height_z     = 4
;           // Heigth of the lower part. Total height is this value plus
                             //   the height of the cover
plate        = 1.4;          // Thickness of the bottom plain

       //Definition size of press-in nuts
nutdiameter  = 4.1;          // Hole diameter for thermal press-in nut
nutlength    = 5.8;          // Depth of the nut hole
screwdia = 3.2;
screwhead = 6;


//Definition of circle angular resolution
resol        = 36;



difference () {

// Construction of housing
   union () {
   // Definition of main body
      difference () {
         union () {
            cube ( [width_x, debth_y - (2* cornerradius), height_z], center = true );
            cube ( [width_x - (2* cornerradius), debth_y, height_z], center = true );
         };
         translate ( [0,0, plate / 2] ){
            cube ( [width_x - (2* wall), debth_y- (2* wall), height_z - plate], center = true ); 
         };
      };

      // Construction of four corner cylinders including nut holes
      // 1st quadrant
      translate ( [(width_x / 2) - cornerradius, (debth_y / 2) - cornerradius, 0 ] ) {
         difference () {
            cylinder (h=height_z, r=cornerradius, center = true, $fn = resol);
            translate ( [ 0,0,(height_z / 2)-(nutlength / 2) ] ) {
               cylinder (h = nutlength, r = screwdia / 2, center = true, $fn = resol);
            };
         };
      };

      // 2nd quadrant
      translate ( [-(width_x / 2) + cornerradius, (debth_y / 2) - cornerradius, 0] ) {
         difference () {
            cylinder (h=height_z, r=cornerradius, center = true, $fn = resol);
            translate ( [ 0,0,(height_z / 2)-(nutlength / 2) ] ) {
               cylinder (h = nutlength, r = screwdia / 2, center = true, $fn = resol);
            };
         };
      };

      // 4th quadrant
      translate ( [(width_x / 2) - cornerradius, -(debth_y / 2) + cornerradius, 0] ) {
         difference () {
            cylinder (h=height_z, r=cornerradius, center = true, $fn = resol);
            translate ( [ 0,0,(height_z / 2)-(nutlength / 2) ] ) {
               cylinder (h = nutlength, r = screwdia / 2, center = true, $fn = resol);
            };
         };
      };

      // 3rd quadrant
      translate ( [-(width_x / 2) + cornerradius, -(debth_y / 2) + cornerradius, 0] ) {
         difference () {
            cylinder (h=height_z, r=cornerradius, center = true, $fn = resol);
            translate ( [ 0,0,(height_z / 2)-(nutlength / 2) ] ) {
               cylinder (h = nutlength, r = screwdia / 2, center = true, $fn = resol);
            };
         };
      }; 
      
      //spacer for rot encoders
      translate ([24,18,0.4]) rotate ([0,0,90]) cylinder (h=2, r=12/2, center = true, $fn = resol);
      translate ([-25,18,0.4]) rotate ([0,0,90]) cylinder (h=2, r=12/2, center = true, $fn = resol);
      translate ([-74,18,0.4]) rotate ([0,0,90]) cylinder (h=2, r=12/2, center = true, $fn = resol);
      
      //side flanges
      translate ([0,debth_y/2-wall*3/2,2]) cube ([width_x-20,wall,4], center=true);
      translate ([0,(-debth_y/2)+wall*3/2,2]) cube ([width_x-20,wall,4], center=true);
      translate ([width_x/2-wall*3/2,0,2]) cube ([wall,debth_y-16,4], center=true);
      translate ([-(width_x/2)+wall*3/2,0,2]) cube ([wall,debth_y-16,4], center=true);
      
      //mount for switch cap assy (7.6mm from button centre)
      translate ([69,33.5+7.6,0]) cube ([10,1.2,1.2], center=true);
      translate ([84,33.5+7.6,0]) cube ([10,1.2,1.2], center=true);
      
      for (i=[0:7]) translate ([(24.75-7.5)-i*12,-16.5+7.6,0]) cube ([10,1.2,1.2], center=true);
      for (i=[0:2]) translate ([(24.75-7.5)-i*12,-35.5+7.6,0]) cube ([10,1.2,1.2], center=true);
      for (i=[6:7]) translate ([(24.75-7.5)-i*12,-35.5+7.6,0]) cube ([10,1.2,1.2], center=true);
      for (i=[0:1]) translate ([(43.5+6)-i*12,-35.5-7.6,0]) cube ([10,1.2,1.2], center=true);
      for (i=[0:1]) translate ([(43.5+6)-i*12,-23.5+7.6,0]) cube ([10,1.2,1.2], center=true);
      
   };

// Space for the construction of holes, breakouts, ... 

   //encoders
   translate ([24,18,0]) rotate ([0,0,90]) cylinder (h=8, r=7/2, center = true, $fn = resol);
   translate ([-25,18,0]) rotate ([0,0,90]) cylinder (h=8, r=7/2, center = true, $fn = resol);
   translate ([-74,18,0]) rotate ([0,0,90]) cylinder (h=8, r=7/2, center = true, $fn = resol);
   
   //slider slots
   translate ([69,-8,0]) cube ( [3,55,8], center = true ); 
   translate ([84,-8,0]) cube ( [3,55,8], center = true ); 
   
   //buttons above sliders
   translate ([69,33.5,0]) cube ( [12,12,8], center = true ); //buttons
   translate ([84,33.5,0]) cube ( [12,12,8], center = true ); //buttons
   
   translate ([-24.75,-16.5,0]) cube ( [12*8,12,8], center = true ); //top row buttons
   translate ([5.25,-35.5,0]) cube ( [12*3,12,8], center = true ); //bot row buttons
   translate ([-60.8,-35.5,0]) cube ( [12*2,12,8], center = true ); //bot row buttons
   
   translate ([43.5,-35.5,0]) cube ( [12*2,12,8], center = true ); //go
   translate ([43.5,-23.5,0]) cube ( [12*2,12,8], center = true ); //other 2

   
   //recess for screws
   translate ( [(width_x / 2) - cornerradius, (debth_y / 2) - cornerradius, -3.4 ] ) cylinder (h = nutlength, r = screwhead / 2, center = true, $fn = resol);
   translate ( [-(width_x / 2) + cornerradius, (debth_y / 2) - cornerradius, -3.4 ] ) cylinder (h = nutlength, r = screwhead / 2, center = true, $fn = resol);
   translate ( [(width_x / 2) - cornerradius, -(debth_y / 2) + cornerradius, -3.4 ] ) cylinder (h = nutlength, r = screwhead / 2, center = true, $fn = resol);
   translate ( [-(width_x / 2) + cornerradius, -(debth_y / 2) + cornerradius, -3.4 ] ) cylinder (h = nutlength, r = screwhead / 2, center = true, $fn = resol);


};


//pcb pillars
    pcbpillar (0,0,4.5-height_z/2+plate,9.5);
    pcbpillar (0,34,4.5-height_z/2+plate,9.5);
    pcbpillar (-82,34,4.5-height_z/2+plate,9.5);
    pcbpillar (94,34,4.5-height_z/2+plate,9.5);
    pcbpillar (-32,-37,4.5-height_z/2+plate,9.5);
    pcbpillar (-82,-37,4.5-height_z/2+plate,9.5);
    pcbpillar (94,-37,4.5-height_z/2+plate,9.5);


module pcbpillar(xp,yp,zp,ht)
{
    difference() {
        translate ([xp,yp,zp]) rotate ([0,0,90]) cylinder (h=ht, r=6/2, center = true, $fn = resol);
        translate ([xp,yp,zp]) rotate ([0,0,90]) cylinder (h=ht, r=nutdiameter/2, center = true, $fn = resol);
    }
};
