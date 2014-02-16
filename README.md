apallaktikh
===========

To run the application, "make && ./main" should be enough, if glut is
installed. When up and running, you can choose view:

0: original model
1: voxel model (cubes)
2: reconstructed model (marching cubes)
3: original model angle histogram
4: reconstructed model angle histogram
5: voxel model (spheres)
6: free fall/collision simulation

by pressing the corresponding number, and navigate with mouse (left click drag
and scroll). Right click gives menu options for wireframe or solid
rendering. Press 't' to toggle texture (on some views).

Increase or decrease voxel resolution with 'i' and 'd', respectively.

Press buttons 'f' or 'c' to build a free fall or collision simulation for the
current voxel model, and switch to view '6' to watch it. Adjust frame step with
',' and '.' to rewind or fast forward the simulation.