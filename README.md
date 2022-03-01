# Infix

This is the newer Infix Engine. As of right now I have not yet started to implement any of the memory design. 
So all it can do right now is render 2 triangles. Over the next few months I will be adding the memory layout and rending capabilities.

General Idea:
Creating a octree(with diffrent data structures at its leaf nodes) that can be accesed by the CPU and the GPU as if it is like one big volume.

I am also adding support for windows, mac, and linux not only to be able to run the engine but also build it too.
This is a big pririty for the egine because I feel like it should be able to work cross platform. That means anyone can add to the project and it will still work.
As far as c++ and building cross platform code it is a big pain in the neck by I am hoping it will pay off in the end.

So far to build on windows:
1.) download visual studios and make sure to check the boxes for c++ development, cmake, and vcpkg
2.) download glfw3, glm, vulkan with vcpkg
3.) download vulkan api

I am currently working on mac building, however, it should work with MoltanVK and xcode

And as far as linux I have not started a build system for that.

Lukuly the cmake code should not be very complicated since it used vcpkg. However, I am considering linking all files directly like glm and maybe glfw3 and vulkan.

This is all in like beta beta beta because nothing is set in stone. Also, this is more of a test to see if this particular memory design will work in the way that I think it will.

Here is a small overview for the memory design as of right now:
All memory that needs to be held in the ram or on the cpu for the voxel space is contaied within a circular buffer. Circular Buffers ONLY hold data that is in the same hierarchical position
that means that if you want to hold data that is to be rendered each LOD has to have a diffrent cirulatbuffer. This way all data can be treaded as equal and destruction of the data will
be in the correct order.
When data needs to be updatad it will be entered into the queue
(ether of the GPU or CPU) then once a frame finished being drawn the queue is moved from the GPU to the CPU or form the CPU to the GPU respectivly and processed. 
This way data can be processed on the CPU and the GPU at the same time without needing to wait on eachother.
The downside is that the GPU can do random reads and writes form the CPU. So GPU code has to be writen like a fragment shader. That means that you can only have one read or write per
thread. The good news is that if you can garntee that the memory is on the GPU then you can do as many reads and writes as you would like.
This way you basically get a memory outlinde that works with the "Genderal Idea" :)
