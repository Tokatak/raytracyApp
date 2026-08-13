
#include "win/main.h"
#include "tracer/tracer.h"

#define PI 3.14159265358979323846

static int frame =0;

void  Renderer_UpdateAndRender(OffscreenBuffer* offscreenBuffer){

  frame ++;
  
  Sphere spheres[] = {
      // position  r  color        specular reflective  r^2(runtime cached)
    {{0, -1, 3}, 1, {255, 0, 0}, 500,     0.2,          1},
    {{2, 0, 4},  1, {0, 0, 255}, 500,     0.3,          1},
    {{-2, 0, 4}, 1, {0, 255, 0}, 10,      0.4,          1},
    
    {{0, -5001, 0}, 5000, {255, 255, 0}, 1000, 0.5,     1},

      // todo remove debug spheres
    {{0, 0, 10}, 1, {255, 0,0},500, 0.2, 1},
    {{0, 0, -10}, 1, {255,0,0}, 500, 0.2,1},      
    {{10, 0, 0}, 1, {0, 255, 0}, 500, 0.2,1},
    {{-10, 0, 0}, 1, {0, 255, 0}, 500, 0.2,1},     
    {{0, 10, 0}, 1, {0, 0, 255}, 500, 0.2,1},
    {{0, -10, 0}, 1, {0, 0, 255}, 500, 0.2,1},
  };

  // note: cant be 0 since is expanded by ARRAY_SIZE macro
  // and having 0 causes internal compilation error ( msvc )
  Light lights[3];
  lights[0] =(Light) {LIGHT_AMBIENT, 0.2, {0, 0, 0}};
  lights[1] =(Light) {LIGHT_POINT, 0.6, {2, 1, 0}};
  lights[2] =(Light) {LIGHT_DIRECTIONAL, 0.2, {1, 4, 4}};
  

  V3 viewportSize = {1.0, 1.0, 0.0};
  float projectionPlane = 1.0;

  Buffer buffer = {0};
  buffer.width = offscreenBuffer->Width;
  buffer.height = offscreenBuffer->Height;
  buffer.size = buffer.width * buffer.height * 3;

  // unsigned char *b = (unsigned char *)malloc((size_t)buffer.size);
  // if (b == NULL) {
  //   return -1;
  // }
  buffer.start = offscreenBuffer->Memory;

  // TOP +
  // RIGHt +
  int topEdge = buffer.height / 2;
  int bottomEdge = -buffer.height / 2;

  int leftEdge = -buffer.width / 2;
  int righEdge = buffer.width / 2;
  Region region;
  region.top = topEdge;
  region.bot = bottomEdge;
  region.left = leftEdge;
  region.right = righEdge;

  V3 origin = {0};
  int recursion_depth = 3;


  float anglePerFrame = 360 / (12*30);
  /* // starting from */
  /* // x-         x+ */
  /* // . . . . . .z+ */
  /* // . . . . . . */
  /* // . . 0 . . . */
  /* // . . . . . . */
  /* // . . * . . .z- */

  float offsetX = 0;
  float offsetZ = 0;
  float radius = 8; // Distance from origin

  float start = -PI;
  float radians = start + frame * anglePerFrame * (PI / 180.0);
  
  origin.x = offsetX + sin(radians) * radius;
  origin.z = offsetZ + cos(radians) * radius;
  origin.y = 0; 

  // Look at the origin (0,0,0)
  V3 lookAtPoint = {0, 0, 0};
  
  V3 cameraDirection = {0};
  cameraDirection.x = lookAtPoint.x - origin.x;
  cameraDirection.y = lookAtPoint.y - origin.y;
  cameraDirection.z = lookAtPoint.z - origin.z;
  
  float dirLen = sqrtf(cameraDirection.x*cameraDirection.x + 
                       cameraDirection.y*cameraDirection.y + 
                       cameraDirection.z*cameraDirection.z);
  if (dirLen > 0) {
    cameraDirection.x /= dirLen;
    cameraDirection.y /= dirLen;
    cameraDirection.z /= dirLen;
  }

  Camera camera = {0};
  camera.position = origin;
  camera.direction = cameraDirection;
  camera.viewportSize = viewportSize;
  camera.projectionPlane = projectionPlane;
  
  fillRegion( region, camera,
	      buffer, PIXEL_LAYOUT_BGRA,
	      1, INFINITY, recursion_depth,
	      spheres, ARRAY_SIZE(spheres), lights, ARRAY_SIZE(lights));
}
