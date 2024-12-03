#include "Transformable.h"
#include "../Math/Math.h"

namespace GameDev2D
{
	Transformable::Transformable() :
		WorldPositionChangedSlot(m_WorldPositionChangedSignal),
		WorldScaleChangedSlot(m_WorldScaleChangedSignal),
		WorldRotationChangedSlot(m_WorldRotationChangedSignal),
		m_LocalTransform(Matrix::Identity()),
		m_WorldTransform(Matrix::Identity()),
		m_LocalPosition(0.0f, 0.0f),
		m_LocalScale(1.0f, 1.0f),
		m_Children{},
		m_Parent(nullptr),
		m_LocalRadians(0.0f),
		m_IsLocalTransformDirty(false),
		m_IsWorldTransformDirty(false)
	{}

	Transformable::~Transformable()
	{
		AttachTo(nullptr);

		//Ensure any children are no longer 'attached' to the parent
		for (Transformable* t : m_Children)
		{
			t->AttachTo(nullptr);
		}

		m_Children.clear();
	}

	Matrix Transformable::GetWorldTransform()
	{
		CalculateWorldTransform();
		return m_WorldTransform;
	}

	Matrix Transformable::GetLocalTransform()
	{
		CalculateLocalTransform();
		return m_LocalTransform;
	}

	Vector2 Transformable::GetWorldPosition()
	{
		return GetWorldTransform().GetTranslation();
	}

	Vector2 Transformable::GetPosition()
	{
		return m_LocalPosition;
	}

	void Transformable::SetPosition(Vector2 aPosition)
	{
		m_LocalPosition = aPosition;
		LocalTransformIsDirty();
	}

	void Transformable::SetPosition(float aX, float aY)
	{
		m_LocalPosition.x = aX;
		m_LocalPosition.y = aY;
		LocalTransformIsDirty();
	}

	void Transformable::SetPositionX(float aX)
	{
		m_LocalPosition.x = aX;
		LocalTransformIsDirty();
	}

	void Transformable::SetPositionY(float aY)
	{
		m_LocalPosition.y = aY;
		LocalTransformIsDirty();
	}

	void Transformable::Translate(Vector2 aOffset)
	{
		m_LocalPosition += aOffset;
		LocalTransformIsDirty();
	}

	float Transformable::GetWorldDegrees()
	{
		return GetWorldTransform().GetDegrees();
	}

	float Transformable::GetWorldRadians()
	{
		return GetWorldTransform().GetRadians();
	}

	float Transformable::GetDegrees()
	{
		return Math::RadiansToDegrees(m_LocalRadians);
	}

	float Transformable::GetRadians()
	{
		return m_LocalRadians;
	}

	void Transformable::SetDegrees(float aDegrees)
	{
		m_LocalRadians = Math::DegreesToRadians(aDegrees);
		LocalTransformIsDirty();
	}

	void Transformable::SetRadians(float aRadians)
	{
		m_LocalRadians = aRadians;
		LocalTransformIsDirty();
	}

	void Transformable::RotateDegrees(float aDegrees)
	{
		RotateRadians(Math::DegreesToRadians(aDegrees));
	}

	void Transformable::RotateRadians(float aRadians)
	{
		m_LocalRadians += aRadians;
		LocalTransformIsDirty();
	}

	Vector2 Transformable::GetDirection()
	{
		return Vector2(cosf(m_LocalRadians), sinf(m_LocalRadians));
	}

	void Transformable::SetDirection(Vector2 aDirection)
	{
		m_LocalRadians = atan2f(aDirection.y, aDirection.x);
		LocalTransformIsDirty();
	}

	void Transformable::SetDirection(float dx, float dy)
	{
		SetDirection(Vector2(dx, dy));
	}

	Vector2 Transformable::GetWorldScale()
	{
		return GetWorldTransform().GetScale();
	}

	Vector2 Transformable::GetScale()
	{
		return m_LocalScale;
	}

	void Transformable::SetScale(Vector2 aScale)
	{
		m_LocalScale = aScale;
		LocalTransformIsDirty();
	}

	void Transformable::SetScale(float aSacleX, float aScaleY)
	{
		m_LocalScale.x = aSacleX;
		m_LocalScale.y = aScaleY;
		LocalTransformIsDirty();
	}

	void Transformable::SetScaleX(float aSacleX)
	{
		m_LocalScale.x = aSacleX;
		LocalTransformIsDirty();
	}

	void Transformable::SetScaleY(float aSacleY)
	{
		m_LocalScale.y = aSacleY;
		LocalTransformIsDirty();
	}

	bool Transformable::IsLocalTransformDirty()
	{
		return m_IsLocalTransformDirty;
	}

	bool Transformable::IsWorldTransformDirty()
	{
		return m_IsWorldTransformDirty;
	}

	void Transformable::AttachTo(Transformable* parent)
	{
		//If parent is different we need to remove this as a child of m_Parent
		if (parent != m_Parent && m_Parent != nullptr)
		{
			m_Parent->m_Children.erase(std::remove(m_Parent->m_Children.begin(), m_Parent->m_Children.end(), this), m_Parent->m_Children.end());
		}

		m_Parent = parent;

		WorldTransformIsDirty();

		if (m_Parent != nullptr)
		{
			m_Parent->m_Children.push_back(this);
		}
	}

	Transformable* Transformable::GetParent()
	{
		return m_Parent;
	}

	void Transformable::CalculateLocalTransform()
	{
		if (m_IsLocalTransformDirty)
		{
			m_IsLocalTransformDirty = false;

			Matrix translationMatrix = Matrix::MakeTranslation(m_LocalPosition);
			Matrix rotationMatrix = Matrix::MakeRotation(m_LocalRadians);
			Matrix scaleMatrix = Matrix::MakeScale(m_LocalScale);

			m_LocalTransform = translationMatrix * rotationMatrix * scaleMatrix;

			//Re-calculate the world transform for any children
			for (Transformable* t : m_Children)
			{
				t->WorldTransformIsDirty();
			}
		}
	}

	void Transformable::CalculateWorldTransform()
	{
		if (m_IsWorldTransformDirty || (m_Parent != nullptr && m_Parent->IsWorldTransformDirty()))
		{
			m_IsWorldTransformDirty = false;

			Matrix previousWorldTransform = m_WorldTransform;

			//Calculate the world transform
			if (m_Parent != nullptr)
			{
				m_WorldTransform = m_Parent->GetWorldTransform() * GetLocalTransform();
			}
			else
			{
				m_WorldTransform = GetLocalTransform();
			}

			//Did the translation change?
			if (previousWorldTransform.GetTranslation() != m_WorldTransform.GetTranslation())
			{
				m_WorldPositionChangedSignal.publish(this, m_WorldTransform.GetTranslation(), previousWorldTransform.GetTranslation());
			}

			//Did the scale change?
			if (previousWorldTransform.GetScale() != m_WorldTransform.GetScale())
			{
				m_WorldScaleChangedSignal.publish(this, m_WorldTransform.GetScale(), previousWorldTransform.GetScale());
			}

			//Did the rotation change?
			if (previousWorldTransform.GetRadians() != m_WorldTransform.GetRadians())
			{
				m_WorldRotationChangedSignal.publish(this, m_WorldTransform.GetRadians(), previousWorldTransform.GetRadians());
			}
		}
	}

	void Transformable::LocalTransformIsDirty()
	{
		m_IsLocalTransformDirty = true;
		m_IsWorldTransformDirty = true;
	}

	void Transformable::WorldTransformIsDirty()
	{
		m_IsWorldTransformDirty = true;
	}
}