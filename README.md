3dengine
========

Here is my own 3d engine, its not totally finished but its able to render objects, put colors, etc...

With this engine you can render 3d models from 3ds files. It loads shapes and color characteristics of the object, 
it calculates normals of the vertices using the smoothing groups information of the 3ds file to apply directionals
and pointing lights, it loads all materials and it applies them correctly over the faces that this material is 
corresponding to. 

Transparencies are applied to object with this information.

For the moment is not implemented the loading and application of textures, shadows or highlights.

The engine implements some optimizations. When it gives vertices, normals, materials,... to opengl, it gives them
as arrays (glNormalPointer, glVertexPointer,...). It improves the performance. 

About physics, the engine is able to detect object collisions. For this, it creates a structure of boxes that 
subdivides the object. The biggest box is the bounding box. A tree with all the boxes and subdivisions is 
generated (AABB tree). In this way, when the engine is going to detect collisions of two objects, it checks trees of 
both objects to detect what leaf-boxes of the first object are collisioning with leaf-boxes of the second object.
Finally, having this boxes collisions, engine checks objects's triangles collisions (only triangles that are into
the leaf-boxes where engine detected collisions).

The source code of a 3ds objects loader is uploaded too.
