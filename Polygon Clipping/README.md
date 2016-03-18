##Polygon Clipping

#Sutherland Hodgman Polygon Clipping Algorithm
The Sutherland–Hodgman algorithm is used for clipping polygons. It works by extending each line of the convex clip polygon in turn and selecting only vertices from the subject polygon that are on the visible side.

##How to run
* g++ polygon_clipping.cpp -lglut -lGL -lGLU 
* ./a.out


##Basic Idea
* The Sutherland-Hodgman algorithm clips a polygon against all edges of the clipping region
in turn.
* The algorithm steps from vertex to vertex, adding 0, 1, or 2 vertices to the output list at
each step.
* There are 4 cases that can happen :
![Line Clipping demo ](https://github.com/mamexo/CS345NetworkLab/blob/master/Chord/img/sample.png)
 * Assuming vertex A has already been processed,
 * Case 1 — vertex B is added to the output list
 * Case 2 — vertex B’ is added to the output (edge AB is clipped to AB’)
 * Case 3 — no vertex added (segment AB clipped out)
 * Case 4 — vertices A’ and B are added to the output (edge AB is clipped to A’B)
