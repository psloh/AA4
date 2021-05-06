#include <math.h>

#include "P3Dv100.H"


ProjectionObject::ProjectionObject(void)
{                                                     //  +z      +y
	_eye_theta = 90;                                   //    |   /
	_eye_phi = -20;                                    //    |
   _eye_spin = 0;                                     //    | /
   _eye_x = 0;                                        //    |________ +x
   _eye_y = -10;                                      //    /
	_eye_z = 4;                                        //   /
   _eye_angle = 45;                                   //  /
   _screen_xdim = 640;                                //   -y
   _screen_ydim = 480;
}

ProjectionObject::ProjectionObject(float t, float p, float s, float x, float y, float z, float a, int sx, int sy)
{
	_eye_theta = t;
   _eye_phi = p;
   _eye_spin = s;
   _eye_x = x;
   _eye_y = y;
   _eye_z = z;
   _eye_angle = a;
   _screen_xdim = sx;
   _screen_ydim = sy;
}

int ProjectionObject::Project(float points3d[][3], float points2d[][2], int numpoints)
{
// if (sizeof(points2d) < numpoints*2) return 1; // error: not enough room in output array.

   float theta, phi;
   float dx, dy, dz, dt, dp;

   while (_eye_phi < -180) _eye_phi += 360;
   while (_eye_phi > 180) _eye_phi -= 360;

   if (_eye_phi < -90)
   {
   	_eye_theta = -_eye_theta;
      _eye_phi = -180 - _eye_phi;
   }
   else if (_eye_phi > 90)
   {
   	_eye_theta = -_eye_theta;
      _eye_phi = 180 - _eye_phi;
   }

   while (_eye_theta < 0) _eye_theta += 360;
   while (_eye_theta > 360) _eye_theta -= 360;

   for (int i = 0; i < numpoints; i++)
   {
   	dx = points3d[i][0] - _eye_x;
      dy = points3d[i][1] - _eye_y;
      dz = points3d[i][2] - _eye_z;

      if (dx == 0)
      	if (dy > 0) theta = 90;
         else theta = 270;
      else
      	if (dx > 0) theta = atan(dy/dx) * 180/3.14159;
         else theta = atan(dy/dx) * 180/3.14159 + 180;
      if (sqrt(dx*dx + dy*dy) == 0)
      	if (dz > 0) phi = 90;
      	else phi = -90;
      else
      	phi = atan(dz/(sqrt(dx*dx + dy*dy))) * 180/3.14159;

      while (theta < 0) theta += 360;
      while (theta > 360) theta -= 360;

      dt = _eye_theta-theta;
      dp = phi-_eye_phi;

      while (dt > 180) dt -= 360;
      while (dt < -180) dt += 360;

      while (_eye_spin < 0) _eye_spin += 360;
      while (_eye_spin > 360) _eye_spin -= 360;

		if (abs(_eye_spin) >= 0.001)
      {
      	dx = dt;
         dy = dp;
         if (dx == 0)
      		if (dy > 0) theta = 90;
         	else theta = 270;
      	else
      		if (dx > 0) theta = atan(dy/dx) * 180/3.14159;
         	else theta = atan(dy/dx) * 180/3.14159 + 180;
         dt = sqrt(dx*dx + dy*dy) * cos(theta - _eye_spin*3.14159/180);
         dp = sqrt(dx*dx + dy*dy) * sin(theta - _eye_spin*3.14159/180);
      }

//    if (abs(dt) > _eye_angle || abs(dp) > (_eye_angle*_screen_xdim/_screen_ydim)) continue;

      points2d[i][0] = _screen_xdim/2 + floor(dt/(_eye_angle*(float)_screen_xdim/(float)_screen_ydim)*(float)_screen_xdim/2);
      points2d[i][1] = _screen_ydim - (_screen_ydim/2 + floor(dp/_eye_angle*(float)_screen_ydim/2));
   }

   return 0;
}

void ProjectionObject::MoveForward(float distance)
{
	_eye_x += distance*cos(_eye_phi*3.14159/180)*cos(_eye_theta*3.14159/180);
	_eye_y += distance*cos(_eye_phi*3.14159/180)*sin(_eye_theta*3.14159/180);
   _eye_z += distance*sin(_eye_phi*3.14159/180);
}

void ProjectionObject::MoveBackward(float distance)
{
	_eye_x -= distance*cos(_eye_phi*3.14159/180)*cos(_eye_theta*3.14159/180);
	_eye_y -= distance*cos(_eye_phi*3.14159/180)*sin(_eye_theta*3.14159/180);
   _eye_z -= distance*sin(_eye_phi*3.14159/180);
}

void ProjectionObject::MoveLeft(float distance)
{
	_eye_x += distance*cos(_eye_phi*3.14159/180)*cos((_eye_theta+90)*3.14159/180);
	_eye_y += distance*cos(_eye_phi*3.14159/180)*sin((_eye_theta+90)*3.14159/180);
}

void ProjectionObject::MoveRight(float distance)
{
	_eye_x += distance*cos(_eye_phi*3.14159/180)*cos((_eye_theta-90)*3.14159/180);
	_eye_y += distance*cos(_eye_phi*3.14159/180)*sin((_eye_theta-90)*3.14159/180);
}

void ProjectionObject::MoveUp(float distance)
{
	_eye_z += distance;
}

void ProjectionObject::MoveDown(float distance)
{
	_eye_z -= distance;
}

void ProjectionObject::TurnRight(float degrees)
{
	_eye_theta -= degrees;
}

void ProjectionObject::TurnLeft(float degrees)
{
	_eye_theta += degrees;
}

void ProjectionObject::TurnUp(float degrees)
{
	_eye_phi += degrees;
}

void ProjectionObject::TurnDown(float degrees)
{
	_eye_phi -= degrees;
}

void ProjectionObject::TurnClockwise(float degrees)
{
	_eye_spin -= degrees;
   if (_eye_spin < 0) _eye_spin += 360;
}

void ProjectionObject::TurnCCwise(float degrees)
{
	_eye_spin += degrees;
   if (_eye_spin > 360) _eye_spin -= 360;
}

void ProjectionObject::MoveEyeXYZ(float x, float y, float z)
{
	_eye_x = x;
   _eye_y = y;
   _eye_z = z;
}

void ProjectionObject::TurnEyeTPS(float theta, float phi, float spin)
{
	_eye_theta = theta;
   _eye_phi = phi;
   _eye_spin = spin;
}

float ProjectionObject::GetTheta(float &theta)
{
	theta = _eye_theta;
   return _eye_theta;
}

float ProjectionObject::GetPhi(float &phi)
{
	phi = _eye_phi;
   return _eye_phi;
}

float ProjectionObject::GetSpin(float &spin)
{
	spin = _eye_spin;
   return _eye_spin;
}

float ProjectionObject::GetX(float &x)
{
	x = _eye_x;
   return _eye_x;
}

float ProjectionObject::GetY(float &y)
{
	y = _eye_y;
   return _eye_y;
}

float ProjectionObject::GetZ(float &z)
{
	z = _eye_z;
   return _eye_z;
}

float ProjectionObject::GetEyeAngle(float &angle)
{
	angle = _eye_angle;
   return _eye_angle;
}

void ProjectionObject::GetScreenSize(int &x, int &y)
{
	x = _screen_xdim;
   y = _screen_ydim;
}

void ProjectionObject::SetEyeAngle(float angle)
{
	_eye_angle = angle;
}

void ProjectionObject::SetScreenSize(int x, int y)
{
	_screen_xdim = x;
   _screen_ydim = y;
}

int ProjectionObject::TranslateX(float points3d[][3], float distance)
{
	return 0;
}

int ProjectionObject::TranslateY(float points3d[][3], float distance)
{
	return 0;
}

int ProjectionObject::TranslateZ(float points3d[][3], float distance)
{
	return 0;
}

int ProjectionObject::TranslateXYZ(float points3d[][3], float x, float y, float z)
{
	return 0;
}

int ProjectionObject::RotateX(float points3d[][3], float xaxis, float degrees)
{
	return 0;
}

int ProjectionObject::RotateY(float points3d[][3], float yaxis, float degrees)
{
	return 0;
}

int ProjectionObject::RotateZ(float points3d[][3], float zaxis, float degrees)
{
	return 0;
}

int ProjectionObject::Dilate(float points3d[][3], float x, float y, float z, float factor)
{
	return 0;
}
