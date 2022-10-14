#pragma once
#include <cmath>

/////////////////////
// Dimensional vector
struct Vector2
{
	float m_x;
	float m_y;

	/////////////////////
	// Multiplies vectors
	Vector2 operator*(float right)
	{
		return Vector2{ m_x * right, m_y * right };
	}
	
	////////////////////
	// Subtracts vectors
	Vector2 operator-(Vector2 b)
	{
		return Vector2{ this->m_x - b.m_x, this->m_y - b.m_y };
	}
	
	////////////////////////////////////////
	// Returns squared length of this vector
	float GetSquareLength()
	{
		return m_x * m_x + m_y * m_y;
	}

	////////////////////////////////////////////////////////
	// Returns vector with same proportions, but length of 1
	Vector2 GetNormalized()
	{
		// Calculate exact length
		float length = sqrtf(GetSquareLength());

		// Account for length of zero
		if (length == 0)
			return Vector2{ 0,0 };
		// Divide by length to get a length of one
		else
			return Vector2{ m_x / length, m_y / length };
	}
};