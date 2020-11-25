#pragma once

#include <iostream>
#include <gun_net.h>

enum class CustomMsgTypes : uint32_t
{
	FIreBullet,
	MovePlayer
};

int main()
{
	gun::net::message<CustomMsgTypes> msg;
	msg.header.id = CustomMsgTypes::FIreBullet;

	int a = 1;
	bool b = true;
	float c = 3.14159f;

	struct
	{
		float x=6;
		float y=7;
	} d[5];

	msg << a << b << c << d;

	a = 99;
	b = false;
	c = 99.9f;

	msg >> d >> c >> b >> a;


	return 0;
}
// https://youtu.be/2hNdkYInj4g?t=2447