#include "Vector2.h"

Vector2::Vector2( float a, float b ) : x(a), y(b)
{
}

Vector2::Vector2( const Vector2 &rhs ) : x(rhs.x), y(rhs.y)
{
}

void Vector2::Set( float a, float b )
{
	x = a;
	y = b;
}

Vector2 Vector2::operator+( const Vector2& rhs ) const
{
	return Vector2(x + rhs.x, y + rhs.y);
}

Vector2 Vector2::operator-( const Vector2& rhs ) const
{
	return Vector2(x - rhs.x, y - rhs.y);
}

Vector2 Vector2::operator-( void ) const
{
	return Vector2(-x, -y);
}

Vector2 Vector2::operator*( float scalar ) const
{
	return Vector2(x * scalar, y * scalar);
}

float Vector2::Length( void ) const
{
	return sqrt(x * x + y * y);
}

float Vector2::Dot( const Vector2& rhs ) const
{
	return (x * rhs.x + y * rhs.y);
}

Vector2 Vector2::Normalized( void )
{
	float d = Length();
	if(d <= Math::EPSILON && -d <= Math::EPSILON)
	{
	  throw DivideByZero();
	}
	return Vector2(x / d, y / d);
}