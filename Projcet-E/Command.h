#pragma once
void* CommandThread(void* data)
{
	string input;
	while (isRunning)
	{
		cin >> input;
		for (int i = 0; i < input.size(); i++)
		{
			if (input[i] >= 'a' || input[i] <= 'z')
			{
				//소문자를 대문자로 만들어줍니다
				input[i] -= 'A' - 'a';
			};
		};
		switch (input)
		{
		case "QUIT":
		case "END":
		case "EXIT":
			isRunning = false;
			break;
		};
	};
}