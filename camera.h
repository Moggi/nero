#ifndef CAMERA_INCLUDED_H
#define CAMERA_INCLUDED_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

struct Camera
{
public:
	Camera(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar)
	{
		this->pos = pos;
		this->forward = glm::vec3(0.0f, 0.0f, 1.0f);
		this->up = glm::vec3(0.0f, 1.0f, 0.0f);
		this->projection = glm::perspective(fov, aspect, zNear, zFar);
		Init();
	}

	Camera(const glm::vec3& pos, float zNear, float zFar)
	{
		this->pos = pos;
		this->forward = glm::vec3(0.0f, 0.0f, 1.0f);
		this->up = glm::vec3(0.0f, 1.0f, 0.0f);
		this->projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -10.0f, 1000.0f);
		// this->projection = glm::ortho(0.0f, right, bottom, 0.0f,0.0f,300.0f);
		Init();
	}

	inline glm::mat4 GetViewProjection() const
	{
		return projection * glm::lookAt(pos, pos + forward, up);
	}

	void MoveZ(float amt)
	{
		pos += glm::vec3(0.0f, 0.0f, 1.0f) * amt;
		constrains();
	}

	void MoveForward(float amt)
	{
		pos += forward * amt;
		constrains();
	}

	void MoveUp(float amt)
	{
		pos += up * amt;
		constrains();
	}

	void MoveRight(float amt)
	{
		pos += glm::cross(up, forward) * amt;
		constrains();
	}

	void Pitch(float angle)
	{
		glm::vec3 right = glm::normalize(glm::cross(up, forward));

		forward = glm::vec3(glm::normalize(glm::rotate(angle, right) * glm::vec4(forward, 0.0)));
		up = glm::normalize(glm::cross(forward, right));

		constrains();
	}

	void RotateX(float angle)
	{
		static const glm::vec3 UP(1.0f, 0.0f, 0.0f);

		glm::mat4 rotation = glm::rotate(angle, UP);

		forward = glm::vec3(glm::normalize(rotation * glm::vec4(forward, 0.0)));
		up = glm::vec3(glm::normalize(rotation * glm::vec4(up, 0.0)));

		constrains();
	}

	void RotateY(float angle)
	{
		static const glm::vec3 UP(0.0f, 1.0f, 0.0f);

		glm::mat4 rotation = glm::rotate(angle, UP);

		forward = glm::vec3(glm::normalize(rotation * glm::vec4(forward, 0.0)));
		up = glm::vec3(glm::normalize(rotation * glm::vec4(up, 0.0)));

		constrains();
	}

	void RotateZ(float angle)
	{
		static const glm::vec3 UP(0.0f, 0.0f, 1.0f);

		glm::mat4 rotation = glm::rotate(angle, UP);

		forward = glm::vec3(glm::normalize(rotation * glm::vec4(forward, 0.0)));
		up = glm::vec3(glm::normalize(rotation * glm::vec4(up, 0.0)));
		constrains();
	}

	glm::vec3 GetView()
	{
		return pos + forward*2.0f;
	}

	glm::vec3 & GetPos()
	{
		return pos;
	}

	glm::vec3 GetRot()
	{
		return up;
	}

	void Lock(bool lookFixed)
	{
		this->lookFixed = lookFixed;
	}

	void FixedLook(float min, float max)
	{
		maxAmnt = max;
		// minAmnt = min;
		minAmnt = min;
	}

	void Follow(glm::vec3 position)
	{
		position.y += 3.0f;
		position.z -= 7.0f;
		pos = position;
	}

	void LookAt(glm::vec3 position)
	{
		glm::vec3 direction = glm::normalize(pos-position);
		// float length = glm::length(position-pos);
		float distance = glm::distance(pos,position);
		glm::vec3 vector = position+distance*direction;

		// std::cout << "Vector length:" << length << "\n";
		// std::cout << "Vector distance: " << distance<< "\n";
		// //
		// std::cout << "Direction Vector (normalized): \n";
		// std::cout << direction.x << " =|= " << direction.y << " =|= " << direction.z << "\n";
		// //
		// std::cout << "Distance vector: \n";
		// std::cout << vector.x << " | " << vector.y << " | " << vector.z << "\n";
		//
		// std::cout << "Camera global position: \n";
		// std::cout << pos.x << " | " << pos.y << " | " << pos.z << "\n";
		//
		// std::cout << "Lamborghini global position: \n";
		// std::cout << position.x << " | " << position.y << " | " << position.z << "\n";

		pos = clamp( pos, position + direction*minAmnt,position + direction*maxAmnt );


		// std::cout << "==================\n";
	}

	void infos()
	{
		// std::cout << up.x << " | " << up.y << " | " << up.z << "up\n====";
		std::cout << "Camera global position: \n";
		std::cout << pos.x << " | " << pos.y << " | " << pos.z << "\n";
	}

	void MinimumHeight(float height)
	{
		minHeight = height;
	}


protected:
	void Init()
	{
		minAmnt = 1.0f;
		maxAmnt = 100.0f;
		lookFixed = false;
		minHeight = -100.0f;
	}

	void constrains()
	{
		pos.y = (pos.y < minHeight) ? minHeight : pos.y;
		if(lookFixed){
			// up.y = (up.y < 0.5f) ? 0.5f : up.y;
			// pos.y = (pos.y > fixedHeightMax) ? fixedHeightMax : pos.y;
			// pos.y = (pos.y < fixedHeightMax) ? fixedHeightMax : pos.y;
		}
	}

private:
	glm::mat4 projection;
	glm::vec3 pos;
	glm::vec3 forward;
	glm::vec3 up;
	float minHeight;
	bool lookFixed;
	float maxAmnt;
	float minAmnt;
};

#endif
