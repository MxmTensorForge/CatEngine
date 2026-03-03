3D Software Rendering Engine - README

Project Overview:
A custom 3D software rendering engine built from scratch, featuring CPU-based rendering,
custom physics algorithms, and mathematical animations. This is a personal educational
project demonstrating low-level computer graphics and game engine architecture.

Developer: Egor Kuzmin

Core Features:
- Custom Mathematics Library: Complete linear algebra implementation with vectors,
  matrices, quaternions, and geometric utilities
- Collision Detection: GJK (Gilbert-Johnson-Keerthi) algorithm for convex hull collision
  detection with EPA (Expanding Polytope Algorithm) for collision resolution
- Mathematical Animations: Bezier curves, easing functions, and etc.
- Component System: Object-oriented component-based architecture for game objects
- Software Rendering: Pure CPU-based rasterization without GPU dependencies (only sdl3)

Technical Details:
- All computations performed on CPU (software rendering)
- Custom matrix/vector operations for transformations
- Backface culling and depth testing
- Frustum clipping and triangle rasterization
- Built with SDL3 for window management and input handling
- Visual Studio solution (.sln) for Windows development

Project Status:
!!!!EARLY DEVELOPMENT STAGE!!!!

This project is currently in active development by a single developer.
Many features are experimental and subject to change. The engine is not
production-ready and should be used for educational purposes only.

Inspiration:
This project is inspired by the MIT-licensed "3dzavr" engine by vectozavr:
https://github.com/vectozavr/3dzavr

While inspired by this project, all code in this repository has been
written from scratch as a learning exercise.

Setup Instructions:
1. Open the .sln file in Visual Studio
2. Ensure SDL3 development libraries are properly linked
3. Build the solution (F7)
4. Run the executable (F5)

Disclaimer:
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND. The author
is not responsible for any bugs, crashes, or unexpected behavior that may
occur while using this engine. This is a personal educational project and
should not be used in critical applications.

Performance Notes:
As a CPU-based renderer, performance is limited compared to hardware-accelerated
solutions. Complex scenes with high triangle counts may experience reduced frame rates.

License:
This project is available for educational and personal use. Please contact
the author for commercial licensing inquiries.

Contact:
For questions or contributions, please contact rapapagg123@gmail.com

Version: 0.1.8 Alpha
Build System: Visual Studio Solution (no CMake)
