# Scenic Screen Saver
![Saver](https://github.com/freddyox/freddyox.github.io/blob/master/images/scenery/scenery.png)

* A video of the program may be seen <a href="https://www.youtube.com/watch?v=lxAgjiuRBeQ">here</a> where the first 20 seconds are sped up by a factor of 12 in order to see the whole program, then the program resumes in real time.
* A more detailed description of the program may be seen <a href="https://freddyox.github.io/blog/scenic-screen-saver/">here</a>.
* The program is written in C++ and makes use of the <a href="https://www.sfml-dev.org/">SFML multimedia libraries</a> which means all visuals were created from scratch with the exception of the tire and owl sprites. For example, the trees, blades of grass, ropes, and moon are built using vertex arrays.

## Overview of Project
* I originally developed this program to simulate a rope; however, it became clear that the rope needs to be anchored to something interesting, *e.g.* a fractal tree which I created in a <a href="https://freddyox.github.io/blog/tree-fractal-v2/">previous post</a>. The project then unintentionally evolved into something much larger.
* The program includes a simulated sunset/sunrise model (and a night-time scene), lightning bugs, an owl, blades of grass, stars, and a moon to make the scenery more interesting.
* All objects are perturbed according to a fictitious wind force which has been generated using a Perlin noise generator.
* Additionally, the background cricket noise becomes louder during the night.
