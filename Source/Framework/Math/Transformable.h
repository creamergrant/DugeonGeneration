#pragma once

#include "../Math/Vector2.h"
#include "../Math/Matrix.h"
#include <entt.hpp>


namespace GameDev2D
{
	class Transformable
	{
	public:
		Transformable();
		virtual ~Transformable();

		entt::sink<void(Transformable* transformable, const Vector2& position, const Vector2& previous)> WorldPositionChangedSlot;
		entt::sink<void(Transformable* transformable, const Vector2& scale, const Vector2& previous)> WorldScaleChangedSlot;
		entt::sink<void(Transformable* transformable, float radians, float previous)> WorldRotationChangedSlot; //In radians

		//Returns the world Transformation matrix
		Matrix GetWorldTransform();

		//Returns the local Transformation matrix
		Matrix GetLocalTransform();

		//Returns the World Position of the Transformable object (factoring a parent's position)
		Vector2 GetWorldPosition();

		//Returns the Local Position of the Transformable object
		Vector2 GetPosition();

		//Sets the Local Position of the Transformable object
		void SetPosition(Vector2 position);

		//Sets the Local Position of the Transformable object
		void SetPosition(float x, float y);

		//Sets the Local Position along the x-axis of the Transformable object
		void SetPositionX(float x);

		//Sets the Local Position along the y-axis of the Transformable object
		void SetPositionY(float y);

		//Translates the Transformable object from its current local position
		void Translate(Vector2 offset);

		//Returns the World Angle (in degrees) of the Transformable object (factoring a parent's rotation)
		float GetWorldDegrees();

		//Returns the World Angle (in radians) of the Transformable object (factoring a parent's rotation)
		float GetWorldRadians();

		//Returns the Local Angle of the Transformable object in Degrees
		float GetDegrees();

		//Returns the Local Angle of the Transformable object in Radians
		float GetRadians();

		//Sets the Local Angle of the Transformable object in Degrees
		void SetDegrees(float degrees);

		//Sets the Local Angle of the Transformable object in Degrees
		void SetRadians(float radians);

		//Rotates the Transformable object from its current angle in Degrees
		void RotateDegrees(float degrees);

		//Rotates the Transformable object from its current local angle in Degrees
		void RotateRadians(float radians);

		//Returns the local direction vector of the Transformable object
		Vector2 GetDirection();

		//Sets the local direction vector of the Transformable object
		void SetDirection(Vector2 direction);

		//Sets the local direction vector of the Transformable object
		void SetDirection(float dx, float dy);

		//Returns the World Scale of the Transformable object (factoring a parent's scale)
		Vector2 GetWorldScale();

		//Returns the local scale of the Transformable object
		Vector2 GetScale();

		//Sets the local scale of the Transformable object
		void SetScale(Vector2 scale);

		//Sets the local scale of the Transformable object
		void SetScale(float scaleX, float scaleY);

		//Sets the local scale along the x-axis of the Transformable object
		void SetScaleX(float scaleX);

		//Sets the local scale along the y-axis of the Transformable object
		void SetScaleY(float scaleY);

		//Returns wether the local transfomation matrix is dirty or not
		bool IsLocalTransformDirty();

		//Returns wether the world transfomation matrix is dirty or not
		bool IsWorldTransformDirty();

		//Attach this Transformable to a parent Transformable, it will move, rotate and scale with the parent Transformable
		void AttachTo(Transformable* parent);

		//Returns the parent Transformable
		Transformable* GetParent();

	protected:
		void CalculateLocalTransform();
		void CalculateWorldTransform();

		virtual void LocalTransformIsDirty();
		virtual void WorldTransformIsDirty();

		//Member variables
		entt::sigh<void(Transformable* transformable, const Vector2& position, const Vector2& previous)> m_WorldPositionChangedSignal;
		entt::sigh<void(Transformable* transformable, const Vector2& scale, const Vector2& previous)> m_WorldScaleChangedSignal;
		entt::sigh<void(Transformable* transformable, float radians, float previous)> m_WorldRotationChangedSignal;
		Matrix m_LocalTransform;
		Matrix m_WorldTransform;
		Vector2 m_LocalPosition;
		Vector2 m_LocalScale;
		std::vector<Transformable*> m_Children;
		Transformable* m_Parent;
		float m_LocalRadians;
		bool m_IsLocalTransformDirty;
		bool m_IsWorldTransformDirty;
	};
}
