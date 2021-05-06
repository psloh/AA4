class ProjectionObject
{
	private:       // all measurements in meters.
		float _eye_theta, _eye_phi, _eye_spin; // direction you are looking
		float _eye_x, _eye_y, _eye_z; // posiiton of eye
		float _eye_angle; // cone of eye-sight
      int _screen_xdim, _screen_ydim; // screen size
   public:
   	ProjectionObject(void);
      ProjectionObject(float t, float p, float s, float x, float y, float z, float a, int sx, int sy);
 		int Project(float points3d[][3], float points2d[][2], int numpoints);

		void MoveForward(float distance);
		void MoveBackward(float distance);
      void MoveLeft(float distance);
      void MoveRight(float distance);
      void MoveUp(float distance);
      void MoveDown(float distance);

      void TurnRight(float degrees);
		void TurnLeft(float degrees);
      void TurnUp(float degrees);
      void TurnDown(float degrees);
      void TurnClockwise(float degrees);
      void TurnCCwise(float degrees);

      void MoveEyeXYZ(float x, float y, float z);
      void TurnEyeTPS(float theta, float phi, float spin);

      float GetTheta(float &theta);
      float GetPhi(float &phi);
      float GetSpin(float &spin);
      float GetX(float &x);
      float GetY(float &y);
      float GetZ(float &z);
      float GetEyeAngle(float &angle);
      float GetWorldRadius(float &radius);
      void GetScreenSize(int &x, int &y);
      void SetEyeAngle(float angle);
      void SetWorldRadius(float radius);
      void SetScreenSize(int x, int y);

      int TranslateX(float points3d[][3], float distance);
      int TranslateY(float points3d[][3], float distance);
      int TranslateZ(float points3d[][3], float distance);
      int TranslateXYZ(float points3d[][3], float x, float y, float z);
      int RotateX(float points3d[][3], float xaxis, float degrees);   // using left-hand rule
      int RotateY(float points3d[][3], float yaxis, float degrees);   // turns cc-wise with +axis
      int RotateZ(float points3d[][3], float zaxis, float degrees);   // pointing away
      int Dilate(float points3d[][3], float x, float y, float z, float factor);
};
