﻿
# include <Siv3D.hpp>

void Main()
{
	int32 count = 0;

	while (System::Update())
	{
		Window::SetTitle(Cursor::Pos(), L" ", Cursor::ScreenPos(), L" ", count);

		if (MouseL.down())
		{
			++count;
		}
		else if (MouseR.up())
		{
			--count;
		}
		else if (MouseM.pressed())
		{
			count += 100;
		}

		if (MouseL.up())
		{
			Log << MouseL.pressedDuration();
		}
	}
}

