#pragma once
#include "Includes.h"
class Camera
{
private:
	int last_mx, last_my;
	float RotationAngleX, RotationAngleY;
	bool updateOrientation;
	glm::fquat orientation;

	glm::vec3 eye;
	glm::vec3 center;
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

public:
	Camera();
	~Camera(void);

	void setLast_mx(int x);
	void setLast_my(int y);
	void setRotationAngleX(float x);
	void setRotationAngleY(float y);
	void setEye(glm::vec3 v);
	void setCenter(glm::vec3 v);

	float getRotationAngleX();
	float getRotationAngleY();
	int getLast_mx();
	int getLast_my();
	glm::mat4 getOrientation();
	glm::vec3 getCenter();
	glm::vec3 getEye(); 
	glm::vec3 computeCameraCenter();

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();

	void updateCamera();
	void setPerspective(float fovy, float aspect, float zNear, float zFar);
	void setOrtogonal(float Left, float  Right, float Top, float Bottom, float Near, float Far);
	void createViewMatrix(float eyex, float eyey, float eyez, float centerx, float centery, float centerz);
	void OffsetOrientation(const glm::vec3 &_axis, float fAngDeg);
};

