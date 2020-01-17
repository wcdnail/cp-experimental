translate([1, 1, 0.5]) 
  cube([1.6, 1.6, 1], center = true);

translate([-1.2, -1.2, 0.98]) 
  sphere(1, $fn=32);
  
translate([-1.2, 1.2, 0]) 
  cylinder(h=1.7, d1=2, d2=0.7, $fn=9);